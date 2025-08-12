#include "Gui_Manipulation.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include "main.h"
#include "CommonUtility.h"
#include "Gui_CurveEditor.h"
#include "Gui_FieldTextureEditor.h"
#include "Gui_ScriptEditor.h"
#include "Gui_AssemblyEditor.h"
#include "Gui_SpellAnimationsEditor.h"
#include "Gui_TextEditor.h"
#include "Gui_TextureEditor.h"
#include "Gui_WalkmeshEditor.h"
#include "Gui_SmallDialogs.h"
#include "Hades_Strings.h"
#include "Database_Script.h"
#include "Database_Item.h"
#include "Database_Text.h"
#include "Database_Spell.h"
#include "Database_SpellAnimation.h"
#include "Database_Resource.h"
#include "Database_Animation.h"
#include "Database_CSV.h"

//=============================//
//          Strings            //
//=============================//

#define LIST_MAX_AMOUNT 1000
unsigned int OrderedIndex[LIST_MAX_AMOUNT];

#define MACRO_MULTILANG_INCREASE_COUNTER(BASETEXT,NEWTEXT,BASETEXTARRAY,BASETEXTINDEX,DEFAULTTEXT) \
	for (SteamLanguage macrolangi = 0;macrolangi<STEAM_LANGUAGE_AMOUNT;macrolangi++) \
		if (NEWTEXT.multi_lang_init[macrolangi] && !BASETEXT.multi_lang_init[macrolangi]) \
			for (int macrobackindex = BASETEXTINDEX-1;macrobackindex>=0 && !BASETEXTARRAY[macrobackindex].multi_lang_init[macrolangi];macrobackindex--) \
				BASETEXTARRAY[macrobackindex].SetValue(DEFAULTTEXT,macrolangi);


// Only return sorted indexes.
unsigned int* SortStrings(wxArrayString str) {
	unsigned int* res = new unsigned int[str.GetCount()];
	unsigned int i,j,min,tmpi;
	wxString tmps;
	for (i=0;i<str.GetCount();i++)
		res[i] = i;
	for (i=0;i<str.GetCount();i++) {
		min = i;
		for (j=i+1;j<str.GetCount();j++)
			if (str[j]<str[min])
				min = j;
		tmps = str[min];
		str[min] = str[i];
		str[i] = tmps;
		tmpi = res[min];
		res[min] = res[i];
		res[i] = tmpi;
	}
	return res;
}

void CDDataStruct::SpellDisplayNames(bool create) {
	unsigned int i, prev = spellsorted[m_spelllist->GetSelection()];
	if (!create)
		m_spelllist->Clear();
	wxArrayString names;
	names.Alloc(spellset.spell.size());
	for (i = 0; i < spellset.spell.size(); i++)
		names.Add(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (spellsorted != OrderedIndex)
		delete[] spellsorted;
	if (GetTopWindow()->m_sortspell->IsChecked())
		spellsorted = SortStrings(names);
	else
		spellsorted = OrderedIndex;
	for (i = 0; i < spellset.spell.size(); i++) {
		m_spelllist->Append(names[spellsorted[i]], (void*)&spellsorted[i]);
		if (!create && spellsorted[i] == prev)
			m_spelllist->SetSelection(i);
	}
}

void CDDataStruct::SupportDisplayNames(bool create) {
	unsigned int i, prev = supportsorted[m_supportlist->GetSelection()];
	if (!create)
		m_supportlist->Clear();
	wxArrayString names;
	names.Alloc(supportset.support.size());
	for (i = 0; i < supportset.support.size(); i++)
		names.Add(_(supportset.support[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (supportsorted != OrderedIndex)
		delete[] supportsorted;
	if (GetTopWindow()->m_sortsupport->IsChecked())
		supportsorted = SortStrings(names);
	else
		supportsorted = OrderedIndex;
	for (i = 0; i < supportset.support.size(); i++) {
		m_supportlist->Append(names[supportsorted[i]], (void*)&supportsorted[i]);
		if (!create && supportsorted[i] == prev)
			m_supportlist->SetSelection(i);
	}
}

void CDDataStruct::CommandDisplayNames(bool create) {
	unsigned int i, prev = cmdsorted[m_cmdlist->GetSelection()];
	if (!create)
		m_cmdlist->Clear();
	wxArrayString names;
	names.Alloc(cmdset.cmd.size());
	for (i = 0; i < cmdset.cmd.size(); i++)
		names.Add(_(cmdset.cmd[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (cmdsorted != OrderedIndex)
		delete[] cmdsorted;
	if (GetTopWindow()->m_sortcmd->IsChecked())
		cmdsorted = SortStrings(names);
	else
		cmdsorted = OrderedIndex;
	for (i = 0; i < cmdset.cmd.size(); i++) {
		m_cmdlist->Append(names[cmdsorted[i]], (void*)&cmdsorted[i]);
		if (!create && cmdsorted[i] == prev)
			m_cmdlist->SetSelection(i);
	}
}

void CDDataStruct::StatDisplayNames(bool create) {
	unsigned int i, prev = m_statlist->GetSelection();
	if (!create)
		m_statlist->Clear();
	m_statlist->Append(_(HADES_STRING_STAT_LEVEL));
	for (i=0;i< statset.initial_stat.size();i++)
		m_statlist->Append(_(statset.initial_stat[i].default_name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_statlist->SetSelection(prev);
}

void CDDataStruct::ItemDisplayNames(bool create) {
	unsigned int i, prev = itemsorted[m_itemlist->GetSelection()];
	if (!create)
		m_itemlist->Clear();
	wxArrayString names;
	names.Alloc(itemset.item.size());
	for (i = 0; i < itemset.item.size(); i++)
		names.Add(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (itemsorted != OrderedIndex)
		delete[] itemsorted;
	if (GetTopWindow()->m_sortitem->IsChecked())
		itemsorted = SortStrings(names);
	else
		itemsorted = OrderedIndex;
	for (i = 0; i < itemset.item.size(); i++) {
		m_itemlist->Append(names[itemsorted[i]], (void*)&itemsorted[i]);
		if (!create && itemsorted[i] == prev)
			m_itemlist->SetSelection(i);
	}
}

void CDDataStruct::KeyItemDisplayNames(bool create) {
	unsigned int i, prev = keyitemsorted[m_keyitemlist->GetSelection()];
	if (!create)
		m_keyitemlist->Clear();
	wxArrayString names;
	names.Alloc(itemset.key_item.size());
	for (i = 0; i < itemset.key_item.size(); i++)
		names.Add(_(itemset.key_item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (keyitemsorted != OrderedIndex)
		delete[] keyitemsorted;
	if (GetTopWindow()->m_sortkeyitem->IsChecked())
		keyitemsorted = SortStrings(names);
	else
		keyitemsorted = OrderedIndex;
	for (i = 0; i < itemset.key_item.size(); i++) {
		m_keyitemlist->Append(names[keyitemsorted[i]], (void*)&keyitemsorted[i]);
		if (!create && keyitemsorted[i] == prev)
			m_keyitemlist->SetSelection(i);
	}
}

wxString CDDataStruct::GetShopName(int shopid) {
	int shid = shopset.shop[shopid].id;
	if (shid >= 0 && shid < G_V_ELEMENTS(HADES_STRING_SHOP_NAME))
		return HADES_STRING_SHOP_NAME[shid].label;
	else
		return wxString::Format(wxT(HADES_STRING_SHOP_CUSTOM_NAME), shid);
}

void CDDataStruct::ShopDisplayNames(bool create) {
	unsigned int i, prev = m_shoplist->GetSelection();
	if (!create)
		m_shoplist->Clear();
	wxArrayString names;
	for (i = 0; i < shopset.shop.size(); i++)
		names.Add(GetShopName(i));
	m_shoplist->Append(names);
	if (!create)
		m_shoplist->SetSelection(prev);
}

void CDDataStruct::SynthesisShopDisplayNames(bool create) {
	unsigned int i, prev = m_synthshoplist->GetSelection();
	if (!create)
		m_synthshoplist->Clear();
	wxArrayString names;
	for (i = 0; i < shopset.synthesis.size(); i++)
		names.Add(_(itemset.GetItemById(shopset.synthesis[i].synthesized).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_synthshoplist->Append(names);
	if (!create)
		m_synthshoplist->SetSelection(prev);
}

wxString CDDataStruct::GetMagicSwordName(FF9String& spellname) {
	wxString swordtoken = _(ffuiset.special_text->text_block[SpecialTextDataSet::GetSpellNamingIndex()].text[0].GetStr(hades::TEXT_PREVIEW_TYPE));
	wxString spellstr = _(spellname.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (swordtoken.Len() > 0 && swordtoken[swordtoken.Len() - 1] == L' ')
		return swordtoken + spellstr;
	else if (gametype == GAME_TYPE_PSX)
		return spellstr + swordtoken;
	return spellstr + _(L' ') + swordtoken;
}

wxString CDDataStruct::GetEnemyBattleName(int battleid) {
	wxString groupname;
	if (GetTopWindow()->m_enemyshowid->IsChecked())
		groupname.Printf(wxT("%03u : %s"), enemyset.battle_data[battleid]->object_id, enemyset.battle_name[battleid]);
	else
		groupname = _(enemyset.battle_name[battleid]);
	return groupname;
}

void CDDataStruct::EnemyDisplayNames(bool create) {
	unsigned int i, prev = enemysorted[m_enemylist->GetSelection()];
	if (!create)
		m_enemylist->Clear();
	wxArrayString names;
	names.Alloc(enemyset.battle_amount);
	for (i = 0; i < enemyset.battle_amount; i++)
		names.Add(_(enemyset.battle_name[i]));
	if (enemysorted != OrderedIndex)
		delete[] enemysorted;
	if (GetTopWindow()->m_sortenemy->IsChecked())
		enemysorted = SortStrings(names);
	else
		enemysorted = OrderedIndex;
	for (i = 0; i < enemyset.battle_amount; i++) {
		m_enemylist->Append(GetEnemyBattleName(enemysorted[i]), (void*)&enemysorted[i]);
		if (!create && enemysorted[i] == prev)
			m_enemylist->SetSelection(i);
	}
}

void CDDataStruct::CardDisplayNames(bool create) {
	unsigned int i, prev = cardsorted[m_cardlist->GetSelection()];
	if (!create)
		m_cardlist->Clear();
	wxArrayString names;
	names.Alloc(CARD_AMOUNT);
	for (i = 0; i < CARD_AMOUNT; i++)
		names.Add(_(cardset.card[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (cardsorted != OrderedIndex)
		delete[] cardsorted;
	if (GetTopWindow()->m_sortcard->IsChecked())
		cardsorted = SortStrings(names);
	else
		cardsorted = OrderedIndex;
	for (i = 0; i < CARD_AMOUNT; i++) {
		m_cardlist->Append(names[cardsorted[i]], (void*)&cardsorted[i]);
		if (!create && cardsorted[i] == prev)
			m_cardlist->SetSelection(i);
	}
}

void CDDataStruct::TextDisplayNames(bool create) {
	unsigned int i, prev = textsorted[m_textlist->GetSelection()];
	if (!create)
		m_textlist->Clear();
	wxArrayString names;
	names.Alloc(textset.amount);
	for (i = 0; i < textset.amount; i++)
		names.Add(_(textset.name[i]));
	if (textsorted != OrderedIndex)
		delete[] textsorted;
	if (GetTopWindow()->m_sorttext->IsChecked())
		textsorted = SortStrings(names);
	else
		textsorted = OrderedIndex;
	for (i = 0; i < textset.amount; i++) {
		m_textlist->Append(textset.name[textsorted[i]], (void*)&textsorted[i]);
		if (!create && textsorted[i] == prev)
			m_textlist->SetSelection(i);
	}
}

void CDDataStruct::WorldMapDisplayNames(bool create) {
	unsigned int i, prev = worldsorted[m_worldlist->GetSelection()];
	if (!create)
		m_worldlist->Clear();
	wxArrayString names;
	names.Alloc(worldset.amount);
	for (i = 0; i < worldset.amount; i++)
		names.Add(_(worldset.name[i]));
	if (worldsorted != OrderedIndex)
		delete[] worldsorted;
	if (GetTopWindow()->m_sortworld->IsChecked())
		worldsorted = SortStrings(names);
	else
		worldsorted = OrderedIndex;
	for (i = 0; i < worldset.amount; i++) {
		m_worldlist->Append(worldset.name[worldsorted[i]], (void*)&worldsorted[i]);
		if (!create && worldsorted[i] == prev)
			m_worldlist->SetSelection(i);
	}
}

wxString CDDataStruct::GetFieldName(int fieldid) {
	wxString fieldname;
	if (GetTopWindow()->m_fieldshowid->IsChecked())
		fieldname.Printf(wxT("%04u : %s"), fieldset.script_data[fieldid]->object_id, fieldset.script_data[fieldid]->name.GetStr(hades::TEXT_PREVIEW_TYPE));
	else
		fieldname = _(fieldset.script_data[fieldid]->name.GetStr(hades::TEXT_PREVIEW_TYPE));
	return fieldname;
}

void CDDataStruct::FieldDisplayNames(bool create) {
	unsigned int i, prev = fieldsorted[m_fieldlist->GetSelection()];
	if (!create)
		m_fieldlist->Clear();
	wxArrayString names;
	names.Alloc(fieldset.amount);
	for (i = 0; i < fieldset.amount; i++)
		names.Add(_(fieldset.script_data[i]->name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (fieldsorted != OrderedIndex)
		delete[] fieldsorted;
	if (GetTopWindow()->m_sortfield->IsChecked())
		fieldsorted = SortStrings(names);
	else
		fieldsorted = OrderedIndex;
	for (i = 0; i < fieldset.amount; i++) {
		m_fieldlist->Append(GetFieldName(fieldsorted[i]), (void*)&fieldsorted[i]);
		if (!create && fieldsorted[i] == prev)
			m_fieldlist->SetSelection(i);
	}
}

void CDDataStruct::BattleSceneDisplayNames(bool create) {
	unsigned int i, prev = scenesorted[m_battlescenelist->GetSelection()];
	if (!create)
		m_battlescenelist->Clear();
	wxArrayString names;
	names.Alloc(sceneset.scene_amount);
	for (i = 0; i < sceneset.scene_amount; i++)
		names.Add(_(sceneset.scene_name[i]));
	if (scenesorted != OrderedIndex)
		delete[] scenesorted;
	if (GetTopWindow()->m_sortbattlescene->IsChecked())
		scenesorted = SortStrings(names);
	else
		scenesorted = OrderedIndex;
	for (i = 0; i < sceneset.scene_amount; i++) {
		m_battlescenelist->Append(sceneset.scene_name[scenesorted[i]], (void*)&scenesorted[i]);
		if (!create && scenesorted[i] == prev)
			m_battlescenelist->SetSelection(i);
	}
}

void CDDataStruct::SpellAnimationDisplayNames(bool create) {
	unsigned int i, j, prev = spellanimsorted[m_spellanimlist->GetSelection()];
	if (!create)
		m_spellanimlist->Clear();
	wxArrayString names;
	names.Alloc(spellanimset.amount);
	for (i = 0; i < spellanimset.amount; i++) {
		for (j = 0; j < HADES_STRING_SPELL_MODEL.size(); j++)
			if (HADES_STRING_SPELL_MODEL[j].id == i) {
				names.Add(_(HADES_STRING_SPELL_MODEL[j].label));
				break;
			}
		if (j == HADES_STRING_SPELL_MODEL.size())
			names.Add(wxString::Format(wxT("Spell Animation %u"), i));
	}
	if (spellanimsorted != OrderedIndex)
		delete[] spellanimsorted;
	if (GetTopWindow()->m_sortspellanim->IsChecked())
		spellanimsorted = SortStrings(names);
	else
		spellanimsorted = OrderedIndex;
	for (i = 0; i < spellanimset.amount; i++) {
		m_spellanimlist->Append(names[spellanimsorted[i]], (void*)&spellanimsorted[i]);
		if (!create && spellanimsorted[i] == prev)
			m_spellanimlist->SetSelection(i);
	}
}

void CDDataStruct::CilDisplayNames(bool create) {
	unsigned int i, prev = cilsorted[m_ciltypelist->GetSelection()];
	if (!create)
		m_ciltypelist->Clear();
	wxArrayString names;
	names.Alloc(cilset.data->GetTypeAmount());
	for (i = 0; i < cilset.data->GetTypeAmount(); i++)
		names.Add(_(cilset.data->GetTypeName(i)));
	if (cilsorted != OrderedIndex)
		delete[] cilsorted;
	cilsorted = SortStrings(names);
	for (i = 0; i < cilset.data->GetTypeAmount(); i++) {
		m_ciltypelist->Append(cilset.data->GetTypeName(cilsorted[i]), (void*)&cilsorted[i]);
		if (!create && cilsorted[i] == prev)
			m_ciltypelist->SetSelection(i);
	}
}

void CDDataStruct::ChangeFF9StringCharmap(wchar_t* chmapdef, wchar_t* chmapa, wchar_t* chmapb, ExtendedCharmap& chmapext) {
	unsigned int i, j;
	if (gametype != GAME_TYPE_PSX)
		return;
	if (saveset.sectionloaded[DATA_SECTION_SPELL]) {
		for (i = 0; i < spellset.spell.size(); i++) {
			spellset.spell[i].name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			spellset.spell[i].help.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			UpdateSpellName(i);
		}
		SpellDisplayNames();
		DisplaySpell(m_spelllist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_SUPPORT]) {
		for (i = 0; i < supportset.support.size(); i++) {
			supportset.support[i].name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			supportset.support[i].help.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			UpdateSupportName(i);
		}
		SupportDisplayNames();
		DisplaySupport(m_supportlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_CMD]) {
		for (i = 0; i < cmdset.cmd.size(); i++) {
			cmdset.cmd[i].name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			cmdset.cmd[i].help.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			UpdateCommandName(i);
		}
		CommandDisplayNames();
		DisplayCommand(m_cmdlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_STAT]) {
		for (i = 0; i < statset.initial_stat.size(); i++)
			statset.initial_stat[i].default_name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
		StatDisplayNames();
		DisplayStat(m_statlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_ENMY]) {
		for (i = 0; i < enemyset.battle_amount; i++) {
			wchar_t* txtchmapext = chmapext.GetCharmap(0);
			for (j = 0; j < enemyset.text[i]->amount; j++)
				enemyset.text[i]->text[j].SetCharmaps(chmapdef, chmapa, chmapb, txtchmapext);
			for (j = 0; j < enemyset.battle[i]->stat_amount; j++)
				enemyset.battle[i]->stat[j].name.SetCharmaps(chmapdef, chmapa, chmapb, txtchmapext);
			for (j = 0; j < enemyset.battle[i]->spell_amount; j++)
				enemyset.battle[i]->spell[j].name.SetCharmaps(chmapdef, chmapa, chmapb, txtchmapext);
			enemyset.UpdateBattleName(i);
		}
		EnemyDisplayNames();
		DisplayEnemy(m_enemylist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_ITEM]) {
		for (i = 0; i < itemset.item.size(); i++) {
			itemset.item[i].name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			itemset.item[i].help.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			itemset.item[i].battle_help.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			UpdateItemName(i);
		}
		for (i = 0; i < itemset.key_item.size(); i++) {
			itemset.key_item[i].name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			itemset.key_item[i].help.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			itemset.key_item[i].description.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
		}
		ItemDisplayNames();
		KeyItemDisplayNames();
		DisplayItem(m_itemlist->GetSelection());
		DisplayKeyItem(m_keyitemlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_CARD]) {
		for (i = 0; i < CARD_AMOUNT; i++) {
			cardset.card[i].name.SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			UpdateCardName(i);
		}
		CardDisplayNames();
		DisplayCard(m_cardlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_TEXT]) {
		for (i = 0; i < textset.amount; i++) {
			wchar_t* txtchmapext;
			if (textset.charmap[i])
				txtchmapext = chmapext.GetCharmap(textset.charmap[i]->object_id);
			else
				txtchmapext = NULL;
			if (textset.text_data[i]) {
				for (j = 0; j < textset.text_data[i]->amount; j++) {
					textset.text_data[i]->text[j].SetCharmaps(chmapdef, chmapa, chmapb, txtchmapext);
				}
			}
		}
		DisplayText(m_textlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_WORLD_MAP]) {
		for (i = 0; i < worldset.amount; i++) {
			wchar_t* txtchmapext = chmapext.GetCharmap(worldset.script[i]->related_charmap_id);
			for (j = 0; j < worldset.text_data[i]->amount; j++)
				worldset.text_data[i]->text[j].SetCharmaps(chmapdef, chmapa, chmapb, txtchmapext);
		}
		for (i = 0; i < WORLD_MAP_PLACE_AMOUNT; i++) {
			worldset.world_data->place_name[i].SetCharmaps(chmapdef, chmapa, chmapb, NULL);
			m_worldplacelist->SetString(i, _(worldset.world_data->place_name[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
		DisplayWorldMap(m_worldlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_FIELD]) {
		for (i = 0; i < fieldset.amount; i++) {
			wchar_t* txtchmapext = chmapext.GetCharmap(fieldset.script_data[i]->related_charmap_id);
			fieldset.script_data[i]->name.SetCharmaps(chmapdef, chmapa, chmapb, txtchmapext);
		}
		FieldDisplayNames();
		DisplayField(m_fieldlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_MENU_UI]) {
		for (i = 0; i < ffuiset.special_text->amount; i++)
			for (j = 0; j < ffuiset.special_text->text_block[i].amount; j++)
				ffuiset.special_text->text_block[i].text[j].SetCharmaps(chmapdef, chmapa, chmapb, NULL);
		DisplaySpecialText(m_specialtextlist->GetSelection());
	}
}

void CDDataStruct::ChangeFF9StringOpcodeChar(wchar_t newchar) {
	unsigned int i, j;
	if (gametype != GAME_TYPE_PSX)
		return;
	if (saveset.sectionloaded[DATA_SECTION_SPELL]) {
		for (i = 0; i < spellset.spell.size(); i++) {
			spellset.spell[i].name.SetOpcodeChar(newchar);
			spellset.spell[i].help.SetOpcodeChar(newchar);
			UpdateSpellName(i);
		}
		SpellDisplayNames();
		DisplaySpell(m_spelllist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_SUPPORT]) {
		for (i = 0; i < supportset.support.size(); i++) {
			supportset.support[i].name.SetOpcodeChar(newchar);
			supportset.support[i].help.SetOpcodeChar(newchar);
			UpdateSupportName(i);
		}
		SupportDisplayNames();
		DisplaySupport(m_supportlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_CMD]) {
		for (i = 0; i < cmdset.cmd.size(); i++) {
			cmdset.cmd[i].name.SetOpcodeChar(newchar);
			cmdset.cmd[i].help.SetOpcodeChar(newchar);
			UpdateCommandName(i);
		}
		CommandDisplayNames();
		DisplayCommand(m_cmdlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_STAT]) {
		for (i = 0; i < statset.initial_stat.size(); i++)
			statset.initial_stat[i].default_name.SetOpcodeChar(newchar);
		StatDisplayNames();
		DisplayStat(m_statlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_ENMY]) {
		for (i = 0; i < enemyset.battle_amount; i++) {
			for (j = 0; j < enemyset.text[i]->amount; j++)
				enemyset.text[i]->text[j].SetOpcodeChar(newchar);
			for (j = 0; j < enemyset.battle[i]->stat_amount; j++)
				enemyset.battle[i]->stat[j].name.SetOpcodeChar(newchar);
			for (j = 0; j < enemyset.battle[i]->spell_amount; j++)
				enemyset.battle[i]->spell[j].name.SetOpcodeChar(newchar);
			enemyset.UpdateBattleName(i);
		}
		EnemyDisplayNames();
		DisplayEnemy(m_enemylist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_ITEM]) {
		for (i = 0; i < itemset.item.size(); i++) {
			itemset.item[i].name.SetOpcodeChar(newchar);
			itemset.item[i].help.SetOpcodeChar(newchar);
			itemset.item[i].battle_help.SetOpcodeChar(newchar);
			UpdateItemName(i);
		}
		for (i = 0; i < itemset.key_item.size(); i++) {
			itemset.key_item[i].name.SetOpcodeChar(newchar);
			itemset.key_item[i].help.SetOpcodeChar(newchar);
			itemset.key_item[i].description.SetOpcodeChar(newchar);
		}
		ItemDisplayNames();
		KeyItemDisplayNames();
		DisplayItem(m_itemlist->GetSelection());
		DisplayKeyItem(m_keyitemlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_CARD]) {
		for (i = 0; i < CARD_AMOUNT; i++) {
			cardset.card[i].name.SetOpcodeChar(newchar);
			UpdateCardName(i);
		}
		CardDisplayNames();
		DisplayCard(m_cardlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_TEXT]) {
		for (i = 0; i < textset.amount; i++)
			if (textset.text_data[i] != NULL)
				for (j = 0; j < textset.text_data[i]->amount; j++)
					textset.text_data[i]->text[j].SetOpcodeChar(newchar);
		DisplayText(m_textlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_WORLD_MAP]) {
		for (i = 0; i < worldset.amount; i++)
			for (j = 0; j < worldset.text_data[i]->amount; j++)
				worldset.text_data[i]->text[j].SetOpcodeChar(newchar);
		for (i = 0; i < WORLD_MAP_PLACE_AMOUNT; i++) {
			worldset.world_data->place_name[i].SetOpcodeChar(newchar);
			m_worldplacelist->SetString(i, _(worldset.world_data->place_name[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
		DisplayWorldMap(m_worldlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_FIELD]) {
		for (i = 0; i < fieldset.amount; i++)
			fieldset.script_data[i]->name.SetOpcodeChar(newchar);
		FieldDisplayNames();
		DisplayField(m_fieldlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_MENU_UI]) {
		for (i = 0; i < ffuiset.special_text->amount; i++)
			for (j = 0; j < ffuiset.special_text->text_block[i].amount; j++)
				ffuiset.special_text->text_block[i].text[j].SetOpcodeChar(newchar);
		DisplaySpecialText(m_specialtextlist->GetSelection());
	}
}

void CDDataStruct::ChangeFF9StringSteamLanguage(SteamLanguage newlang) {
	unsigned int i, j;
	if (gametype == GAME_TYPE_PSX)
		return;
	if (saveset.sectionloaded[DATA_SECTION_SPELL]) {
		for (i = 0; i < spellset.spell.size(); i++) {
			spellset.spell[i].name.ChangeSteamLanguage(newlang);
			spellset.spell[i].help.ChangeSteamLanguage(newlang);
			UpdateSpellName(i);
		}
		SpellDisplayNames();
		DisplaySpell(m_spelllist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_SUPPORT]) {
		for (i = 0; i < supportset.support.size(); i++) {
			supportset.support[i].name.ChangeSteamLanguage(newlang);
			supportset.support[i].help.ChangeSteamLanguage(newlang);
			UpdateSupportName(i);
		}
		SupportDisplayNames();
		DisplaySupport(m_supportlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_CMD]) {
		for (i = 0; i < cmdset.cmd.size(); i++) {
			cmdset.cmd[i].name.ChangeSteamLanguage(newlang);
			cmdset.cmd[i].help.ChangeSteamLanguage(newlang);
			UpdateCommandName(i);
		}
		CommandDisplayNames();
		DisplayCommand(m_cmdlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_STAT]) {
		for (i = 0; i < statset.initial_stat.size(); i++)
			statset.initial_stat[i].default_name.ChangeSteamLanguage(newlang);
		StatDisplayNames();
		DisplayStat(m_statlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_ENMY]) {
		for (i = 0; i < enemyset.battle_amount; i++) {
			for (j = 0; j < enemyset.text[i]->amount; j++)
				enemyset.text[i]->text[j].ChangeSteamLanguage(newlang);
			for (j = 0; j < enemyset.battle[i]->stat_amount; j++)
				enemyset.battle[i]->stat[j].name.ChangeSteamLanguage(newlang);
			for (j = 0; j < enemyset.battle[i]->spell_amount; j++)
				enemyset.battle[i]->spell[j].name.ChangeSteamLanguage(newlang);
			enemyset.script[i]->ChangeSteamLanguage(newlang);
			enemyset.UpdateBattleName(i);
		}
		EnemyDisplayNames();
		DisplayEnemy(m_enemylist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_ITEM]) {
		for (i = 0; i < itemset.item.size(); i++) {
			itemset.item[i].name.ChangeSteamLanguage(newlang);
			itemset.item[i].help.ChangeSteamLanguage(newlang);
			itemset.item[i].battle_help.ChangeSteamLanguage(newlang);
			UpdateItemName(i);
		}
		for (i = 0; i < itemset.key_item.size(); i++) {
			itemset.key_item[i].name.ChangeSteamLanguage(newlang);
			itemset.key_item[i].help.ChangeSteamLanguage(newlang);
			itemset.key_item[i].description.ChangeSteamLanguage(newlang);
		}
		ItemDisplayNames();
		KeyItemDisplayNames();
		DisplayItem(m_itemlist->GetSelection());
		DisplayKeyItem(m_keyitemlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_CARD]) {
		for (i = 0; i < CARD_AMOUNT; i++) {
			cardset.card[i].name.ChangeSteamLanguage(newlang);
			UpdateCardName(i);
		}
		CardDisplayNames();
		DisplayCard(m_cardlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_TEXT]) {
		for (i = 0; i < textset.amount; i++)
			if (textset.text_data[i] != NULL)
				for (j = 0; j < textset.text_data[i]->amount; j++)
					textset.text_data[i]->text[j].ChangeSteamLanguage(newlang);
		DisplayText(m_textlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_WORLD_MAP]) {
		for (i = 0; i < worldset.amount; i++) {
			for (j = 0; j < worldset.text_data[i]->amount; j++)
				worldset.text_data[i]->text[j].ChangeSteamLanguage(newlang);
			worldset.script[i]->ChangeSteamLanguage(newlang);
		}
		for (i = 0; i < WORLD_MAP_PLACE_AMOUNT; i++) {
			worldset.world_data->place_name[i].ChangeSteamLanguage(newlang);
			m_worldplacelist->SetString(i, _(worldset.world_data->place_name[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
		DisplayWorldMap(m_worldlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_FIELD]) {
		for (i = 0; i < fieldset.amount; i++) {
			fieldset.script_data[i]->name.ChangeSteamLanguage(newlang);
			fieldset.script_data[i]->ChangeSteamLanguage(newlang);
		}
		FieldDisplayNames();
		DisplayField(m_fieldlist->GetSelection());
	}
	if (saveset.sectionloaded[DATA_SECTION_MENU_UI]) {
		for (i = 0; i < ffuiset.special_text->amount; i++)
			for (j = 0; j < ffuiset.special_text->text_block[i].amount; j++)
				ffuiset.special_text->text_block[i].text[j].ChangeSteamLanguage(newlang);
		DisplaySpecialText(m_specialtextlist->GetSelection());
	}
}

//=============================//
//          Common             //
//=============================//

IOHWSMessage::IOHWSMessage(wxWindow* parent) :
	IOHWSWindow(parent),
	section_box(new wxCheckBox*[DATA_SECTION_AMOUNT]),
	section_text_box(new wxCheckBox*[DATA_SECTION_AMOUNT]) {
	section_box[DATA_SECTION_SPELL] = m_spell;
	section_box[DATA_SECTION_CMD] = m_command;
	section_box[DATA_SECTION_SUPPORT] = m_support;
	section_box[DATA_SECTION_STAT] = m_stat;
	section_box[DATA_SECTION_PARTY_SPECIAL] = m_partyspecial;
	section_box[DATA_SECTION_ITEM] = m_item;
	section_box[DATA_SECTION_SHOP] = m_shop;
	section_box[DATA_SECTION_ENMY] = m_enemy;
	section_box[DATA_SECTION_CARD] = m_card;
	section_box[DATA_SECTION_TEXT] = m_text;
	section_box[DATA_SECTION_WORLD_MAP] = m_world;
	section_box[DATA_SECTION_FIELD] = m_field;
	section_box[DATA_SECTION_BATTLE_SCENE] = m_scene;
	section_box[DATA_SECTION_SPELL_ANIM] = m_spellanim;
	section_box[DATA_SECTION_MENU_UI] = m_ffui;
	section_box[DATA_SECTION_ASSEMBLY] = m_assembly;
	section_text_box[DATA_SECTION_SPELL] = m_spelltext;
	section_text_box[DATA_SECTION_CMD] = m_commandtext;
	section_text_box[DATA_SECTION_SUPPORT] = m_supporttext;
	section_text_box[DATA_SECTION_STAT] = m_stattext;
	section_text_box[DATA_SECTION_PARTY_SPECIAL] = NULL;
	section_text_box[DATA_SECTION_ITEM] = m_itemtext;
	section_text_box[DATA_SECTION_SHOP] = NULL;
	section_text_box[DATA_SECTION_ENMY] = m_enemytext;
	section_text_box[DATA_SECTION_CARD] = m_cardtext;
	section_text_box[DATA_SECTION_TEXT] = NULL;
	section_text_box[DATA_SECTION_WORLD_MAP] = m_worldtext;
	section_text_box[DATA_SECTION_FIELD] = m_fieldtext;
	section_text_box[DATA_SECTION_BATTLE_SCENE] = NULL;
	section_text_box[DATA_SECTION_SPELL_ANIM] = NULL;
	section_text_box[DATA_SECTION_MENU_UI] = NULL;
	section_text_box[DATA_SECTION_ASSEMBLY] = NULL;
}

void IOHWSMessage::UpdateLoadableSection() {
	unsigned int i;
	int hwslen = m_hwspicker->GetPath().Length();
	char* hwsname = new char[hwslen+1];
	hwsname[hwslen] = 0;
	strncpy(hwsname, (const char*)m_hwspicker->GetPath().mb_str(), hwslen);
	if (PreloadHWS(hwsname,section))
		for (i=0;i<DATA_SECTION_AMOUNT;i++)
			section[i] = false;
	for (i=0;i<DATA_SECTION_AMOUNT;i++) {
		section_box[i]->Enable(section[i]);
		section_box[i]->SetValue(section[i]);
		if (section_text_box[i]!=NULL) {
			section_text_box[i]->Show(true);
			section_text_box[i]->Enable(section[i]);
			section_text_box[i]->SetValue(section[i]);
		}
	}
	m_enemylocal->Enable(section[DATA_SECTION_ENMY]);
	m_worldlocal->Enable(section[DATA_SECTION_WORLD_MAP]);
	m_fieldlocal->Enable(section[DATA_SECTION_FIELD]);
	m_enemylocal->SetValue(section[DATA_SECTION_ENMY]);
	m_worldlocal->SetValue(section[DATA_SECTION_WORLD_MAP]);
	m_fieldlocal->SetValue(section[DATA_SECTION_FIELD]);
	delete[] hwsname;
}

void IOHWSMessage::OnChangeFile(wxFileDirPickerEvent& event) {
	if (!save)
		UpdateLoadableSection();
}

void IOHWSMessage::OnCheckSection(wxCommandEvent& event) {
	if (!save) {
		int id = event.GetId();
		unsigned int boxid =	id == wxID_SPELL ? DATA_SECTION_SPELL :
								id == wxID_SUPPORT ? DATA_SECTION_SUPPORT :
								id == wxID_CMD ? DATA_SECTION_CMD :
								id == wxID_STAT ? DATA_SECTION_STAT :
								id == wxID_ENMY ? DATA_SECTION_ENMY :
								id == wxID_ITEM ? DATA_SECTION_ITEM :
								id == wxID_CARD ? DATA_SECTION_CARD :
								id == wxID_FIELD ? DATA_SECTION_FIELD :
								id == wxID_WORLD ? DATA_SECTION_WORLD_MAP : DATA_SECTION_SHOP;
		section_text_box[boxid]->Enable(event.IsChecked());
		section_text_box[boxid]->SetValue(event.IsChecked());
	}
}

int IOHWSMessage::ShowModal(bool sv, bool* sct, bool* scttxt, bool* scton) {
	unsigned int i;
	save = sv;
	section = sct;
	sectext = scttxt;
	if (save) {
		SetTitle(_(HADES_STRING_HWS_SAVE_TITLE));
		m_hwspicker->SetWindowStyle(wxFLP_SAVE|wxFLP_OVERWRITE_PROMPT|wxFLP_USE_TEXTCTRL);
		m_buttonok->SetLabel(_(HADES_STRING_HWS_SAVE_CONFIRM));
		for (i=0;i<DATA_SECTION_AMOUNT;i++) {
			section_box[i]->Enable(section[i]);
			section_box[i]->SetValue(scton[i]);
			if (section_text_box[i]!=NULL)
				section_text_box[i]->Show(false);
		}
		m_enemylocal->Enable(section[DATA_SECTION_ENMY]);
		m_worldlocal->Enable(section[DATA_SECTION_WORLD_MAP]);
		m_fieldlocal->Enable(section[DATA_SECTION_FIELD]);
		m_enemylocal->SetValue(section[DATA_SECTION_ENMY]);
		m_worldlocal->SetValue(section[DATA_SECTION_WORLD_MAP]);
		m_fieldlocal->SetValue(section[DATA_SECTION_FIELD]);
	} else {
		SetTitle(_(HADES_STRING_HWS_OPEN_TITLE));
		m_hwspicker->SetWindowStyle(wxFLP_OPEN|wxFLP_FILE_MUST_EXIST|wxFLP_USE_TEXTCTRL);
		m_buttonok->SetLabel(_(HADES_STRING_HWS_OPEN_CONFIRM));
		UpdateLoadableSection();
	}
	int res = IOHWSWindow::ShowModal();
	for (i=0;i<DATA_SECTION_AMOUNT;i++) {
		section[i] = section_box[i]->GetValue();
		sectext[i] = section_text_box[i]!=NULL && section_text_box[i]->GetValue();
	}
	return res;
}

bool CDDataStruct::OverwriteBinary() {
	if (OverwritePSXBinary(filename, saveset.sectionmodified, config, saveset, cluster) != 0)
		return false;
	for (int i = 0; i < DATA_SECTION_AMOUNT; i++)
		saveset.sectionmodified[i] = false;
	return true;
}

bool CDDataStruct::ExportPPF() {
	fstream f(filename.c_str(),ios::in | ios::binary);
	if (!f.is_open())
		return false;
	if (saveset.sectionmodified[DATA_SECTION_SPELL])
		spellset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_SUPPORT])
		supportset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_CMD])
		cmdset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_STAT])
		statset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_PARTY_SPECIAL])
		partyspecialset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_ITEM])
		itemset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_SHOP])
		shopset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_ENMY])
		enemyset.WritePPF(f,cluster,!saveset.sectionmodified[DATA_SECTION_WORLD_MAP],!saveset.sectionmodified[DATA_SECTION_FIELD]);
	if (saveset.sectionmodified[DATA_SECTION_CARD])
		cardset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_TEXT])
		textset.WritePPF(f,cluster);
	if (saveset.sectionmodified[DATA_SECTION_WORLD_MAP])
		worldset.WritePPF(f,cluster);
	if (saveset.sectionmodified[DATA_SECTION_FIELD])
		fieldset.WritePPF(f,cluster);
	if (saveset.sectionmodified[DATA_SECTION_BATTLE_SCENE])
		sceneset.WritePPF(f,cluster);
	if (saveset.sectionmodified[DATA_SECTION_SPELL_ANIM])
		spellanimset.WritePPF(f,config,cluster.global_map);
	if (saveset.sectionmodified[DATA_SECTION_MENU_UI])
		ffuiset.WritePPF(f,config);
	if (saveset.sectionmodified[DATA_SECTION_ASSEMBLY])
		mipsset.WritePPF(f,config);
	f.close();
	return true;
}

wstring* CDDataStruct::ReadHWS(const char* fname, bool* section, bool* sectext, bool* localsec) {
	copystring = NULL;
	unsigned int i;
	if (section[DATA_SECTION_SPELL])
		InitSpell();
	if (section[DATA_SECTION_SUPPORT])
		InitSupport();
	if (section[DATA_SECTION_CMD])
		InitCommand();
	if (section[DATA_SECTION_STAT])
		InitStat();
	if (section[DATA_SECTION_PARTY_SPECIAL])
		InitPartySpecial();
	if (section[DATA_SECTION_ITEM])
		InitItem();
	if (section[DATA_SECTION_SHOP])
		InitShop();
	if (section[DATA_SECTION_ENMY] || localsec[0])
		InitEnemy();
	if (section[DATA_SECTION_CARD])
		InitCard();
	if (section[DATA_SECTION_TEXT])
		InitText();
	if (section[DATA_SECTION_WORLD_MAP] || localsec[1])
		InitWorldMap();
	if (section[DATA_SECTION_FIELD] || localsec[2])
		InitField();
	if (section[DATA_SECTION_BATTLE_SCENE])
		InitBattleScene();
	if (section[DATA_SECTION_SPELL_ANIM])
		InitSpellAnimation();
	if (section[DATA_SECTION_MENU_UI])
		InitMenuUI();
	if (section[DATA_SECTION_ASSEMBLY]) {
		if (gametype == GAME_TYPE_PSX)
			InitMips();
		else
			InitCil();
	}
	unsigned int spelloldamount = spellset.spell.size();
	unsigned int supportoldamount = supportset.support.size();
	unsigned int cmdoldamount = cmdset.cmd.size();
	unsigned int statoldamount = statset.initial_stat.size();
	unsigned int itemoldamount = itemset.item.size();
	wstring* res = LoadHWS(fname, section, sectext, localsec, saveset, backupset);
	if (section[DATA_SECTION_SPELL]) {
		MarkDataSpellModified();
		SpellDisplayNames();
		for (i = 0; i < spellset.spell.size(); i++) {
			if (i < spelloldamount)
				UpdateSpellName(i);
			else
				RegisterSpellAdded(i);
		}
		for (i = 0; i < STATUS_SET_AMOUNT; i++)
			UpdateSpellStatusName(spellset.status_set[i]);
	}
	if (section[DATA_SECTION_SUPPORT]) {
		MarkDataSupportModified();
		SupportDisplayNames();
		for (i = 0; i < supportset.support.size(); i++) {
			if (i < supportoldamount)
				UpdateSupportName(i);
			else
				RegisterSupportAdded(i);
		}
	}
	if (section[DATA_SECTION_CMD]) {
		MarkDataCommandModified();
		CommandDisplayNames();
		for (i = 0; i < cmdset.cmd.size(); i++) {
			if (i < cmdoldamount)
				UpdateCommandName(i);
			else
				RegisterCommandAdded(i);
		}
	}
	if (section[DATA_SECTION_STAT]) {
		MarkDataStatModified();
		StatDisplayNames();
		for (i = 0; i < statset.initial_stat.size(); i++) {
			if (i < statoldamount)
				UpdateStatName(i);
			else
				RegisterStatAdded(i);
		}
	}
	if (section[DATA_SECTION_PARTY_SPECIAL]) {
		MarkDataPartySpecialModified();
		m_partyspecialmagicswordset->Clear();
		for (i = 0; i < partyspecialset.magic_sword.size(); i++)
			m_partyspecialmagicswordset->Append(wxString::Format(wxT(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SET), i));
		m_partyspecialmagicswordset->SetSelection(0);
		DisplayPartySpecial(m_partyspeciallist->GetSelection());
	}
	if (section[DATA_SECTION_ITEM]) {
		MarkDataItemModified();
		ItemDisplayNames();
		KeyItemDisplayNames();
		for (i = 0; i < itemset.item.size(); i++) {
			if (i < itemoldamount)
				UpdateItemName(i);
			else
				RegisterItemAdded(i);
		}
	}
	if (section[DATA_SECTION_SHOP]) {
		MarkDataShopModified();
		ShopDisplayNames();
		SynthesisShopDisplayNames();
	}
	if (section[DATA_SECTION_ENMY]) {
		saveset.sectionmodified[DATA_SECTION_ENMY] = true;
		EnemyDisplayNames();
	}
	if (section[DATA_SECTION_CARD]) {
		saveset.sectionmodified[DATA_SECTION_CARD] = true;
		CardDisplayNames();
	}
	if (section[DATA_SECTION_TEXT]) {
		saveset.sectionmodified[DATA_SECTION_TEXT] = true;
		TextDisplayNames();
	}
	if (section[DATA_SECTION_WORLD_MAP]) {
		saveset.sectionmodified[DATA_SECTION_WORLD_MAP] = true;
		WorldMapDisplayNames();
		for (i = 0; i < WORLD_MAP_PLACE_AMOUNT; i++)
			m_worldplacelist->SetString(i, _(worldset.world_data->place_name[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	if (section[DATA_SECTION_FIELD]) {
		saveset.sectionmodified[DATA_SECTION_FIELD] = true;
		FieldDisplayNames();
	}
	if (section[DATA_SECTION_BATTLE_SCENE] && gametype == GAME_TYPE_PSX) {
		saveset.sectionmodified[DATA_SECTION_BATTLE_SCENE] = true;
		BattleSceneDisplayNames();
	}
	if (section[DATA_SECTION_SPELL_ANIM]) {
		saveset.sectionmodified[DATA_SECTION_SPELL_ANIM] = true;
		SpellAnimationDisplayNames();
	}
	if (section[DATA_SECTION_MENU_UI]) {
		MarkDataMenuUIModified();
	}
	if (section[DATA_SECTION_ASSEMBLY]) {
		if (gametype == GAME_TYPE_PSX)
			MarkDataMipsModified(0);
		else
			MarkDataCilModified();
	}
	GetTopWindow()->MarkDataModified();
	DisplayCurrentData();
	return res;
}

void CDDataStruct::MarkDataSpellModified() {
	saveset.sectionmodified[DATA_SECTION_SPELL] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataSupportModified() {
	saveset.sectionmodified[DATA_SECTION_SUPPORT] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataCommandModified() {
	saveset.sectionmodified[DATA_SECTION_CMD] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataStatModified() {
	saveset.sectionmodified[DATA_SECTION_STAT] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataPartySpecialModified() {
	saveset.sectionmodified[DATA_SECTION_PARTY_SPECIAL] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataEnemyModified(unsigned int battleid, Chunk_Type chunktype, bool alllang) {
	saveset.sectionmodified[DATA_SECTION_ENMY] = true;
	if (chunktype==CHUNK_TYPE_ENEMY_STATS)
		enemyset.battle[battleid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_BATTLE_DATA)
		enemyset.battle_data[battleid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_TEXT)
		enemyset.text[battleid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_SCRIPT) {
		if (alllang && enemyset.script[battleid]->multi_lang_script!=NULL)
			for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				if (enemyset.script[battleid]->multi_lang_script->is_loaded[lang])
					enemyset.script[battleid]->multi_lang_script->is_modified[lang] = true;
		enemyset.script[battleid]->MarkDataModified();
	} else if (chunktype==CHUNK_TYPE_IMAGE_MAP)
		enemyset.preload[battleid]->MarkDataModified();
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataItemModified() {
	saveset.sectionmodified[DATA_SECTION_ITEM] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataShopModified() {
	saveset.sectionmodified[DATA_SECTION_SHOP] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataCardModified() {
	saveset.sectionmodified[DATA_SECTION_CARD] = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataTextModified(unsigned int textid, Chunk_Type chunktype, unsigned int objectnum) {
	saveset.sectionmodified[DATA_SECTION_TEXT] = true;
	if (chunktype==CHUNK_TYPE_TEXT)
		textset.text_data[textid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_CHARMAP)
		textset.charmap[textid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_TIM)
		textset.chartim[textid][objectnum].MarkDataModified();
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataWorldMapModified(unsigned int worldid, Chunk_Type chunktype, unsigned int objectnum) {
	saveset.sectionmodified[DATA_SECTION_WORLD_MAP] = true;
	if (chunktype==CHUNK_TYPE_VARIOUS)
		worldset.world_data->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_SCRIPT)
		worldset.script[worldid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_TEXT)
		worldset.text_data[worldid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_CHARMAP)
		worldset.charmap[worldid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_TIM)
		worldset.chartim[worldid][objectnum].MarkDataModified();
	else if (chunktype==CHUNK_TYPE_IMAGE_MAP) {
		worldset.preload[worldid]->MarkDataModified();
		if (gametype==GAME_TYPE_PSX)
			worldset.script[worldid]->parent_cluster->MarkDataModified();
	}
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataWorldMapScriptModified(unsigned int worldid, bool alllang) {
	saveset.sectionmodified[DATA_SECTION_WORLD_MAP] = true;
	if (alllang && worldset.script[worldid]->multi_lang_script!=NULL)
		for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			if (worldset.script[worldid]->multi_lang_script->is_loaded[lang])
				worldset.script[worldid]->multi_lang_script->is_modified[lang] = true;
	worldset.script[worldid]->MarkDataModified();
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataFieldModified(unsigned int fieldid, Chunk_Type chunktype, bool alllang) {
	saveset.sectionmodified[DATA_SECTION_FIELD] = true;
	if (chunktype==CHUNK_TYPE_SCRIPT) {
		if (alllang && fieldset.script_data[fieldid]->multi_lang_script!=NULL)
			for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				if (fieldset.script_data[fieldid]->multi_lang_script->is_loaded[lang])
					fieldset.script_data[fieldid]->multi_lang_script->is_modified[lang] = true;
		fieldset.script_data[fieldid]->MarkDataModified();
	} else if (chunktype==CHUNK_TYPE_FIELD_ROLE) {
		fieldset.role[fieldid]->MarkDataModified();
	} else if (chunktype == CHUNK_TYPE_FIELD_TILES) {
		fieldset.background_data[fieldid]->MarkDataModified();
	} else if (chunktype == CHUNK_TYPE_FIELD_WALK) {
		fieldset.walkmesh[fieldid]->MarkDataModified();
	} else if (chunktype==CHUNK_TYPE_IMAGE_MAP) {
		fieldset.preload[fieldid]->MarkDataModified();
		if (gametype==GAME_TYPE_PSX)
			fieldset.script_data[fieldid]->parent_cluster->MarkDataModified();
	}
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataBattleSceneModified(unsigned int sceneid, Chunk_Type chunktype, unsigned int objectnum) {
	saveset.sectionmodified[DATA_SECTION_BATTLE_SCENE] = true;
	if (chunktype==CHUNK_TYPE_BATTLE_SCENE)
		sceneset.scene[sceneid]->MarkDataModified();
	else if (chunktype==CHUNK_TYPE_TIM)
		sceneset.image[sceneid][objectnum].MarkDataModified();
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataSpellAnimationModified(unsigned int spellanimid, Spell_Animation_Data_Type datatype) {
	saveset.sectionmodified[DATA_SECTION_SPELL_ANIM] = true;
	spellanimset.spell[spellanimid].modified_data |= datatype;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataMenuUIModified() {
	saveset.sectionmodified[DATA_SECTION_MENU_UI] = true;
	ffuiset.special_text->modified = true;
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataMipsModified(int datatype) {
	saveset.sectionmodified[DATA_SECTION_ASSEMBLY] = true;
	mipsset.MarkAsModified(datatype);
	GetTopWindow()->MarkDataModified();
}

void CDDataStruct::MarkDataCilModified() {
	saveset.sectionmodified[DATA_SECTION_ASSEMBLY] = true;
	GetTopWindow()->MarkDataModified();
}

int SharedMenuSelection = 0;
int SharedMenuType = 0;
void CDDataStruct::OnSharedRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;

	#define MACRO_ADD_NEW_OBJECT_SHARED_PART1(TYPE, OBJLIST, STR) \
		baselist.Alloc(OBJLIST.size()); \
		for (i = 0; i < OBJLIST.size(); i++) \
			baselist.Add(_(STR)); \
		dialog.m_baseobjectlist->Clear(); \
		dialog.m_baseobjectlist->Append(baselist); \
		dialog.m_baseobjectlist->SetSelection(SharedMenuSelection); \
		bool keepasking = true; \
		while (keepasking) { \
			keepasking = false; \
			if (dialog.ShowModal() == wxID_OK) { \
				int baseid = dialog.m_baseobjectlist->GetSelection(); \
				int newid = dialog.m_newobjectid->GetValue(); \
				int addindex = InsertAtId(OBJLIST, OBJLIST[baseid], newid); \
				if (addindex < 0) { \
					wxMessageDialog popup(this, HADES_STRING_ADD_OBJECT_SAME_ID, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE); \
					popup.ShowModal(); \
					keepasking = true; \
					continue; \
				} \
				Register ## TYPE ## Added(addindex); \
				TYPE ## DisplayNames();

	#define MACRO_ADD_NEW_OBJECT_SHARED_PART2() \
			} else { \
				return; \
			} \
		}

	#define MACRO_REMOVE_OBJECT_SHARED(TYPE, OBJLIST, LISTBOX) \
		int oldsel = LISTBOX->GetSelection(); \
		OBJLIST.erase(OBJLIST.begin() + SharedMenuSelection); \
		Register ## TYPE ## Removed(SharedMenuSelection); \
		TYPE ## DisplayNames(); \
		if (oldsel != wxNOT_FOUND && oldsel < (int)LISTBOX->GetCount()) \
			LISTBOX->SetSelection(oldsel); \
		else if (LISTBOX->GetCount() > 0) \
			LISTBOX->SetSelection(LISTBOX->GetCount() - 1); \
		if (LISTBOX->GetSelection() != wxNOT_FOUND) \
			Display ## TYPE(LISTBOX->GetSelection());

	if (id == wxID_ADD) {
		SharedNewObjectWindow dialog(this);
		wxArrayString baselist;
		if (SharedMenuType == 0) {
			dialog.m_newobjectid->SetRange(SPELL_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(Spell, spellset.spell, spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataSpellModified();
			for (i = 0; i < spellset.spell.size(); i++)
				if (spellset.spell[spellsorted[i]].id == newid) {
					m_spelllist->SetSelection(i);
					DisplaySpell(i);
					break;
				}
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 1) {
			dialog.m_newobjectid->SetRange(SUPPORT_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(Support, supportset.support, supportset.support[i].name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataSupportModified();
			supportset.support[addindex].boosted_support.clear();
			for (i = 0; i < supportset.support.size(); i++)
				if (supportset.support[supportsorted[i]].id == newid) {
					m_supportlist->SetSelection(i);
					DisplaySupport(i);
					break;
				}
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 2) {
			dialog.m_newobjectid->SetRange(100, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(Command, cmdset.cmd, cmdset.cmd[i].name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataCommandModified();
			for (i = 0; i < cmdset.cmd.size(); i++)
				if (cmdset.cmd[cmdsorted[i]].id == newid) {
					m_cmdlist->SetSelection(i);
					DisplayCommand(i);
					break;
				}
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 3) {
			dialog.m_newobjectid->SetRange(PLAYABLE_CHAR_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(Stat, statset.initial_stat, statset.initial_stat[i].default_name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataStatModified();
			InsertAtId(statset.battle_param, statset.battle_param[StatDataSet::GetCharacterBattleParametersIndices(baseid)[max(0, m_statcharadvancedkind->GetSelection())]], newid + 7);
			statset.InitializeNewCharacter(addindex);
			UpdateStatName(addindex);
			m_statlist->SetString(1 + addindex, _(statset.initial_stat[addindex].default_name.GetStr(hades::TEXT_PREVIEW_TYPE)));
			m_statlist->SetSelection(1 + addindex);
			DisplayStat(1 + addindex);
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 4) {
			dialog.m_newobjectid->SetRange(ITEM_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(Item, itemset.item, itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataItemModified();
			itemset.item[addindex].equip_position_type = ITEM_POSITION_SAME_AS;
			itemset.item[addindex].equip_position = 0;
			itemset.item[addindex].menu_position_type = ITEM_POSITION_AFTER;
			itemset.item[addindex].menu_position = 0;
			if (itemset.item[addindex].usable_id >= 0) {
				InsertAtId<ItemUsableDataStruct>(itemset.usable, itemset.GetUsableById(itemset.item[addindex].usable_id), newid);
				itemset.item[addindex].usable_id = newid;
			}
			if (itemset.item[addindex].weapon_id >= 0) {
				InsertAtId<ItemWeaponDataStruct>(itemset.weapon, itemset.GetWeaponById(itemset.item[addindex].weapon_id), newid);
				itemset.item[addindex].weapon_id = newid;
			}
			if (itemset.item[addindex].armor_id >= 0) {
				InsertAtId<ItemArmorDataStruct>(itemset.armor, itemset.GetArmorById(itemset.item[addindex].armor_id), newid);
				itemset.item[addindex].armor_id = newid;
			}
			if (itemset.item[addindex].stat_id >= ITEM_STAT_AMOUNT) {
				InsertAtId<ItemStatDataStruct>(itemset.stat, itemset.GetStatById(itemset.item[addindex].stat_id), newid);
				itemset.item[addindex].stat_id = newid;
			}
			for (i = 0; i < itemset.item.size(); i++)
				if (itemset.item[itemsorted[i]].id == newid) {
					m_itemlist->SetSelection(i);
					DisplayItem(i);
					break;
				}
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 5) {
			dialog.m_newobjectid->SetRange(KEY_ITEM_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(KeyItem, itemset.key_item, itemset.key_item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataItemModified();
			for (i = 0; i < itemset.key_item.size(); i++)
				if (itemset.key_item[keyitemsorted[i]].id == newid) {
					m_keyitemlist->SetSelection(i);
					DisplayKeyItem(i);
					break;
				}
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 6) {
			dialog.m_newobjectid->SetRange(SHOP_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(Shop, shopset.shop, GetShopName(i))
			MarkDataShopModified();
			m_shoplist->SetSelection(addindex);
			DisplayShop(addindex);
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 7) {
			dialog.m_newobjectid->SetRange(SYNTHESIS_AMOUNT, INT32_MAX);
			MACRO_ADD_NEW_OBJECT_SHARED_PART1(SynthesisShop, shopset.synthesis, itemset.GetItemById(shopset.synthesis[i].synthesized).name.GetStr(hades::TEXT_PREVIEW_TYPE))
			MarkDataShopModified();
			m_synthshoplist->SetSelection(addindex);
			DisplaySynthesisShop(addindex);
			MACRO_ADD_NEW_OBJECT_SHARED_PART2()
		} else if (SharedMenuType == 1000) {
			int* cmdset = statset.GetCharacterCommandsIndices(m_statlist->GetSelection() - 1, NULL);
			AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
			ab.entry.insert(ab.entry.begin() + SharedMenuSelection, ab.entry[SharedMenuSelection]);
			DisplayStatAbilityList(cmdset[m_statcharabilityset->GetSelection()]);
			m_statcharabilitylist->SetSelection(SharedMenuSelection);
			DisplayStatSelectedAbility();
			MarkDataStatModified();
		} else if (SharedMenuType == 1010) {
			if (m_partyspecialmagicswordset->GetSelection() < 0 || m_partyspecialmagicswordset->GetSelection() >= (int)partyspecialset.magic_sword.size())
				return;
			MagicSwordDataStruct& mgswd = partyspecialset.magic_sword[m_partyspecialmagicswordset->GetSelection()];
			mgswd.unlocked.insert(mgswd.unlocked.begin() + SharedMenuSelection, mgswd.unlocked[SharedMenuSelection]);
			mgswd.requirement.insert(mgswd.requirement.begin() + SharedMenuSelection, mgswd.requirement[SharedMenuSelection]);
			m_partyspecialmagicswordlist->Insert(GetMagicSwordName(spellset.GetSpellById(mgswd.unlocked[SharedMenuSelection]).name), SharedMenuSelection);
			m_partyspecialmagicswordlist->SetSelection(SharedMenuSelection);
			MarkDataPartySpecialModified();
		}
	} else if (id == wxID_REMOVE) {
		if (SharedMenuType == 0) {
			MACRO_REMOVE_OBJECT_SHARED(Spell, spellset.spell, m_spelllist)
			MarkDataSpellModified();
		} else if (SharedMenuType == 1) {
			MACRO_REMOVE_OBJECT_SHARED(Support, supportset.support, m_supportlist)
			MarkDataSupportModified();
		} else if (SharedMenuType == 2) {
			MACRO_REMOVE_OBJECT_SHARED(Command, cmdset.cmd, m_cmdlist)
			MarkDataCommandModified();
		} else if (SharedMenuType == 3) {
			MACRO_REMOVE_OBJECT_SHARED(Stat, statset.initial_stat, m_statlist)
			MarkDataStatModified();
			statset.battle_param.erase(statset.battle_param.begin() + StatDataSet::GetCharacterBattleParametersIndices(SharedMenuSelection)[0]);
		} else if (SharedMenuType == 4) {
			MACRO_REMOVE_OBJECT_SHARED(Item, itemset.item, m_itemlist)
			MarkDataItemModified();
		} else if (SharedMenuType == 5) {
			MACRO_REMOVE_OBJECT_SHARED(KeyItem, itemset.key_item, m_keyitemlist)
			MarkDataItemModified();
		} else if (SharedMenuType == 6) {
			MACRO_REMOVE_OBJECT_SHARED(Shop, shopset.shop, m_shoplist)
			MarkDataShopModified();
		} else if (SharedMenuType == 7) {
			MACRO_REMOVE_OBJECT_SHARED(SynthesisShop, shopset.synthesis, m_synthshoplist)
			MarkDataShopModified();
		} else if (SharedMenuType == 1000) {
			int* cmdset = statset.GetCharacterCommandsIndices(m_statlist->GetSelection() - 1, NULL);
			AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
			ab.entry.erase(ab.entry.begin() + SharedMenuSelection);
			DisplayStatAbilityList(cmdset[m_statcharabilityset->GetSelection()]);
			m_statcharabilitylist->SetSelection(min(SharedMenuSelection, (int)m_statcharabilitylist->GetCount() - 1));
			DisplayStatSelectedAbility();
			MarkDataStatModified();
		} else if (SharedMenuType == 1010) {
			if (m_partyspecialmagicswordset->GetSelection() < 0 || m_partyspecialmagicswordset->GetSelection() >= (int)partyspecialset.magic_sword.size())
				return;
			MagicSwordDataStruct& mgswd = partyspecialset.magic_sword[m_partyspecialmagicswordset->GetSelection()];
			mgswd.unlocked.erase(mgswd.unlocked.begin() + SharedMenuSelection);
			mgswd.requirement.erase(mgswd.requirement.begin() + SharedMenuSelection);
			m_partyspecialmagicswordlist->Delete(SharedMenuSelection);
			int sel = min(SharedMenuSelection, (int)m_partyspecialmagicswordlist->GetCount() - 1);
			if (sel >= 0) {
				m_partyspecialmagicswordlist->SetSelection(sel);
				m_partyspecialmagicswordrequirement->SetSelection(spellset.GetSpellIndexById(mgswd.requirement[sel]));
				m_partyspecialmagicswordspell->SetSelection(spellset.GetSpellIndexById(mgswd.unlocked[sel]));
			}
			MarkDataPartySpecialModified();
		}
	}
}

FF9String* RightClickTextSelection = NULL;
void CDDataStruct::OnGenericTextRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	bool executesub = true;
	if (id == wxID_COPY) {
		copystring = RightClickTextSelection;
		executesub = false;
	} else if (id == wxID_PASTE) {
		if (RightClickTextSelection != NULL && copystring != NULL)
			*RightClickTextSelection = *copystring;
		else
			executesub = false;
	} else {
		copystring = NULL;
	}
	if (executesub) {
		if (m_notebookmain->GetSelection() == 2) {
			OnEnemyTextRightClickMenu(event);
		} else if (m_notebookmain->GetSelection() == 4) {
			if (m_notebookenvironment->GetSelection() == 0)
				OnTextRightClickMenu(event);
			else if (m_notebookenvironment->GetSelection() == 1)
				OnWorldTextRightClickMenu(event);
		}
	}
}

#define MACRO_FLAG_SET(VARIABLE,IDNAME,VALUE)	if (id==IDNAME) VARIABLE = on ? VARIABLE | VALUE : VARIABLE & ~VALUE;
#define MACRO_FLAG_SET8(VARIABLE,IDNAME)		MACRO_FLAG_SET(VARIABLE,IDNAME ## 1,0x1) MACRO_FLAG_SET(VARIABLE,IDNAME ## 2,0x2) MACRO_FLAG_SET(VARIABLE,IDNAME ## 3,0x4) MACRO_FLAG_SET(VARIABLE,IDNAME ## 4,0x8)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 5,0x10) MACRO_FLAG_SET(VARIABLE,IDNAME ## 6,0x20) MACRO_FLAG_SET(VARIABLE,IDNAME ## 7,0x40) MACRO_FLAG_SET(VARIABLE,IDNAME ## 8,0x80)
#define MACRO_FLAG_SET16(VARIABLE,IDNAME)		MACRO_FLAG_SET(VARIABLE,IDNAME ## 1,0x1) MACRO_FLAG_SET(VARIABLE,IDNAME ## 2,0x2) MACRO_FLAG_SET(VARIABLE,IDNAME ## 3,0x4) MACRO_FLAG_SET(VARIABLE,IDNAME ## 4,0x8)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 5,0x10) MACRO_FLAG_SET(VARIABLE,IDNAME ## 6,0x20) MACRO_FLAG_SET(VARIABLE,IDNAME ## 7,0x40) MACRO_FLAG_SET(VARIABLE,IDNAME ## 8,0x80)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 9,0x100) MACRO_FLAG_SET(VARIABLE,IDNAME ## 10,0x200) MACRO_FLAG_SET(VARIABLE,IDNAME ## 11,0x400) MACRO_FLAG_SET(VARIABLE,IDNAME ## 12,0x800)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 13,0x1000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 14,0x2000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 15,0x4000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 16,0x8000)
#define MACRO_FLAG_SET32(VARIABLE,IDNAME)		MACRO_FLAG_SET(VARIABLE,IDNAME ## 1,0x1) MACRO_FLAG_SET(VARIABLE,IDNAME ## 2,0x2) MACRO_FLAG_SET(VARIABLE,IDNAME ## 3,0x4) MACRO_FLAG_SET(VARIABLE,IDNAME ## 4,0x8)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 5,0x10) MACRO_FLAG_SET(VARIABLE,IDNAME ## 6,0x20) MACRO_FLAG_SET(VARIABLE,IDNAME ## 7,0x40) MACRO_FLAG_SET(VARIABLE,IDNAME ## 8,0x80)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 9,0x100) MACRO_FLAG_SET(VARIABLE,IDNAME ## 10,0x200) MACRO_FLAG_SET(VARIABLE,IDNAME ## 11,0x400) MACRO_FLAG_SET(VARIABLE,IDNAME ## 12,0x800)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 13,0x1000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 14,0x2000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 15,0x4000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 16,0x8000)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 17,0x10000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 18,0x20000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 19,0x40000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 20,0x80000)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 21,0x100000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 22,0x200000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 23,0x400000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 24,0x800000)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 25,0x1000000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 26,0x2000000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 27,0x4000000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 28,0x8000000)\
												MACRO_FLAG_SET(VARIABLE,IDNAME ## 29,0x10000000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 30,0x20000000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 31,0x40000000) MACRO_FLAG_SET(VARIABLE,IDNAME ## 32,0x80000000)
#define MACRO_FLAG_DISPLAY8(VARIABLE,CHECKBOX)	CHECKBOX ## 1->SetValue(VARIABLE & 0x1); CHECKBOX ## 2->SetValue(VARIABLE & 0x2); CHECKBOX ## 3->SetValue(VARIABLE & 0x4); CHECKBOX ## 4->SetValue(VARIABLE & 0x8);\
												CHECKBOX ## 5->SetValue(VARIABLE & 0x10); CHECKBOX ## 6->SetValue(VARIABLE & 0x20); CHECKBOX ## 7->SetValue(VARIABLE & 0x40); CHECKBOX ## 8->SetValue(VARIABLE & 0x80);
#define MACRO_FLAG_DISPLAY16(VARIABLE,CHECKBOX)	CHECKBOX ## 1->SetValue(VARIABLE & 0x1); CHECKBOX ## 2->SetValue(VARIABLE & 0x2); CHECKBOX ## 3->SetValue(VARIABLE & 0x4); CHECKBOX ## 4->SetValue(VARIABLE & 0x8);\
												CHECKBOX ## 5->SetValue(VARIABLE & 0x10); CHECKBOX ## 6->SetValue(VARIABLE & 0x20); CHECKBOX ## 7->SetValue(VARIABLE & 0x40); CHECKBOX ## 8->SetValue(VARIABLE & 0x80);\
												CHECKBOX ## 9->SetValue(VARIABLE & 0x100); CHECKBOX ## 10->SetValue(VARIABLE & 0x200); CHECKBOX ## 11->SetValue(VARIABLE & 0x400); CHECKBOX ## 12->SetValue(VARIABLE & 0x800);\
												CHECKBOX ## 13->SetValue(VARIABLE & 0x1000); CHECKBOX ## 14->SetValue(VARIABLE & 0x2000); CHECKBOX ## 15->SetValue(VARIABLE & 0x4000); CHECKBOX ## 16->SetValue(VARIABLE & 0x8000);
#define MACRO_FLAG_DISPLAY32(VARIABLE,CHECKBOX)	CHECKBOX ## 1->SetValue(VARIABLE & 0x1); CHECKBOX ## 2->SetValue(VARIABLE & 0x2); CHECKBOX ## 3->SetValue(VARIABLE & 0x4); CHECKBOX ## 4->SetValue(VARIABLE & 0x8);\
												CHECKBOX ## 5->SetValue(VARIABLE & 0x10); CHECKBOX ## 6->SetValue(VARIABLE & 0x20); CHECKBOX ## 7->SetValue(VARIABLE & 0x40); CHECKBOX ## 8->SetValue(VARIABLE & 0x80);\
												CHECKBOX ## 9->SetValue(VARIABLE & 0x100); CHECKBOX ## 10->SetValue(VARIABLE & 0x200); CHECKBOX ## 11->SetValue(VARIABLE & 0x400); CHECKBOX ## 12->SetValue(VARIABLE & 0x800);\
												CHECKBOX ## 13->SetValue(VARIABLE & 0x1000); CHECKBOX ## 14->SetValue(VARIABLE & 0x2000); CHECKBOX ## 15->SetValue(VARIABLE & 0x4000); CHECKBOX ## 16->SetValue(VARIABLE & 0x8000);\
												CHECKBOX ## 17->SetValue(VARIABLE & 0x10000); CHECKBOX ## 18->SetValue(VARIABLE & 0x20000); CHECKBOX ## 19->SetValue(VARIABLE & 0x40000); CHECKBOX ## 20->SetValue(VARIABLE & 0x80000);\
												CHECKBOX ## 21->SetValue(VARIABLE & 0x100000); CHECKBOX ## 22->SetValue(VARIABLE & 0x200000); CHECKBOX ## 23->SetValue(VARIABLE & 0x400000); CHECKBOX ## 24->SetValue(VARIABLE & 0x800000);\
												CHECKBOX ## 25->SetValue(VARIABLE & 0x1000000); CHECKBOX ## 26->SetValue(VARIABLE & 0x2000000); CHECKBOX ## 27->SetValue(VARIABLE & 0x4000000); CHECKBOX ## 28->SetValue(VARIABLE & 0x8000000);\
												CHECKBOX ## 29->SetValue(VARIABLE & 0x10000000); CHECKBOX ## 30->SetValue(VARIABLE & 0x20000000); CHECKBOX ## 31->SetValue(VARIABLE & 0x40000000); CHECKBOX ## 32->SetValue(VARIABLE & 0x80000000);


//=============================//
//          Spells             //
//=============================//

wxArrayString SpellModelAnyList;
SpellModelWindow* TheSpellModelWindow;

void CDDataStruct::DisplaySpell(int spellid) {
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(spellid);
	SpellDataStruct& sp = spellset.spell[*sortid];
	unsigned int i;
	m_spellid->Enable(false);
	m_spellcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_spellcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_spelleffectint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_spellpower->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_spellstatusint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : STATUS_SET_AMOUNT - 1);
	m_spellaccuracy->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_spellmp->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_spellid->SetValue(sp.id);
	m_spellname->ChangeValue(_(sp.name.str));
	m_spellperformname->Clear();
	wxArrayString perfname;
	uint8_t** perfvalue = new uint8_t*[SPELL_AMOUNT + ffuiset.special_text->text_block[SpecialTextDataSet::GetSpellNamingIndex()].amount + 1];
	perfname.Alloc(SPELL_AMOUNT + ffuiset.special_text->text_block[SpecialTextDataSet::GetSpellNamingIndex()].amount + 1);
	for (i = 0; i < SPELL_AMOUNT; i++) {
		perfname.Add(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		perfvalue[i] = new uint8_t(i);
	}
	for (i = 1; i < ffuiset.special_text->text_block[SpecialTextDataSet::GetSpellNamingIndex()].amount; i++) {
		perfname.Add(_(ffuiset.special_text->text_block[SpecialTextDataSet::GetSpellNamingIndex()].text[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
		perfvalue[SPELL_AMOUNT + i - 1] = new uint8_t(SPELL_AMOUNT + i - 1);
	}
	perfname.Add(GetMagicSwordName(sp.name));
	perfvalue[perfname.GetCount() - 1] = new uint8_t(0xFE);
	perfname.Add(_(sp.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	perfvalue[perfname.GetCount() - 1] = new uint8_t(0xFF);
	m_spellperformname->Append(perfname, (void**)perfvalue);
	for (i = 0; i < m_spellperformname->GetCount(); i++)
		if (sp.perform_name == *perfvalue[i]) {
			m_spellperformname->SetSelection(i);
			break;
		}
	m_spellhelp->ChangeValue(_(sp.help.str));
	if (sp.effect < HADES_STRING_SPELL_EFFECT_COUNT)
		m_spelleffect->SetSelection(sp.effect);
	m_spelleffectint->SetValue(sp.effect);
	if (sp.effect < (int)HADES_STRING_SPELL_EFFECT.size())
		m_spelleffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[sp.effect].help);
	else
		m_spelleffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
	int x, y;
	m_spelleffecthelp->GetSize(&x, &y);
	m_spelleffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
	m_spellpower->SetValue(sp.power);
	MACRO_FLAG_DISPLAY8(sp.element, m_spellelement)
	if (sp.status < STATUS_SET_AMOUNT)
		m_spellstatus->SetSelection(sp.status);
	m_spellstatusint->SetValue(sp.status);
	m_spellaccuracy->SetValue(sp.accuracy);
	m_spellmp->SetValue(sp.mp);
	MACRO_FLAG_DISPLAY8(sp.menu_flag, m_spellmenuflag)
	bool acc = false;
	m_spellmodel->SetLabel(wxString::Format(wxT("SFX %d"), sp.model));
	m_spellmodelalt->SetLabel(wxString::Format(wxT("SFX %d"), sp.model_alt));
	for (i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++) {
		if (HADES_STRING_SPELL_MODEL[i].id == sp.model) {
			m_spellmodel->SetLabel(HADES_STRING_SPELL_MODEL[i].label);
			if (acc)
				break;
			else
				acc = true;
		}
		if (HADES_STRING_SPELL_MODEL[i].id == sp.model_alt) {
			m_spellmodelalt->SetLabel(HADES_STRING_SPELL_MODEL[i].label);
			if (acc)
				break;
			else
				acc = true;
		}
	}
	Spell_Target_Type tt = sp.GetTargetType();
	m_spelltargettype->SetSelection(tt);
	m_spelltargetamount->SetSelection(sp.GetTargetAmount());
	m_spelltargetpriority->SetSelection(sp.GetTargetPriority());
	m_spelltargetamount->Enable(tt != SPELL_TARGET_TYPE_EVERYONE && tt != SPELL_TARGET_TYPE_SELF && tt != SPELL_TARGET_TYPE_IRRELEVANT);
	m_spelltargetpriority->Enable(tt == SPELL_TARGET_TYPE_ANY);
	m_spelltargetflagdead->SetValue(sp.target_flag & TARGET_FLAG_CAN_TARGET_DEAD);
	m_spelltargetflagcamera->SetValue(sp.target_flag & TARGET_FLAG_CAMERA);
	m_spelltargetflagdeadfirst->SetValue(sp.target_flag & TARGET_FLAG_TARGET_DEAD_FIRST);
	m_spelltargetpanel->SetSelection(sp.GetPanel());
	MACRO_FLAG_DISPLAY8(sp.flag, m_spellflag)
	m_spellscrolledwindow->Layout();
	m_spellscrolledwindow->GetParent()->GetSizer()->Layout();
	m_spellscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxSpell(wxCommandEvent& event) {
	DisplaySpell(m_spelllist->GetSelection());
}

void CDDataStruct::OnSpellRightClick(wxMouseEvent& event) {
	int newsel = m_spelllist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_spelllist->SetSelection(newsel);
		DisplaySpell(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 0;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= SPELL_AMOUNT);
		m_spelllist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnSpellChangeName(wxCommandEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	if (sp.SetName(m_spellname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_spellname->GetInsertionPoint();
		m_spellname->ChangeValue(_(sp.name.str));
		m_spellname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_spelllist->SetString(sel,_(sp.name.str));
		UpdateSpellName(*sortid);
		MarkDataSpellModified();
	}
}

void CDDataStruct::OnSpellChangeHelp(wxCommandEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	if (sp.SetHelp(m_spellhelp->GetValue().ToStdWstring())) {
		wxTextPos ip = m_spellhelp->GetInsertionPoint();
		m_spellhelp->ChangeValue(_(sp.help.str));
		m_spellhelp->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataSpellModified();
	}
}

void CDDataStruct::OnSpellChangeSpin(wxSpinEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	int id = event.GetId();
	if (id == wxID_ID) {
		sp.id = event.GetPosition();
	} else if (id == wxID_EFFECT) {
		sp.effect = event.GetPosition();
		if (sp.effect < HADES_STRING_SPELL_EFFECT_COUNT)
			m_spelleffect->SetSelection(sp.effect);
		if (sp.effect < (int)HADES_STRING_SPELL_EFFECT.size())
			m_spelleffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[sp.effect].help);
		else
			m_spelleffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
		int x, y;
		m_spelleffecthelp->GetSize(&x, &y);
		m_spelleffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
	} else if (id == wxID_STATUS) {
		sp.status = event.GetPosition();
		if (sp.status < STATUS_SET_AMOUNT)
			m_spellstatus->SetSelection(sp.status);
	} else if (id == wxID_POWER) {
		sp.power = event.GetPosition();
	} else if (id == wxID_ACCURACY) {
		sp.accuracy = event.GetPosition();
	} else if (id == wxID_MP) {
		sp.mp = event.GetPosition();
	}
	MarkDataSpellModified();
}

void CDDataStruct::OnSpellChangeChoice(wxCommandEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	int id = event.GetId();
	if (id == wxID_PERFNAME) {
		uint8_t* item = (uint8_t*)event.GetClientData();
		sp.perform_name = *item;
	} else if (id == wxID_EFFECT) {
		sp.effect = event.GetSelection();
		m_spelleffectint->SetValue(sp.effect);
		if (sp.effect < (int)HADES_STRING_SPELL_EFFECT.size())
			m_spelleffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[sp.effect].help);
		else
			m_spelleffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
		int x, y;
		m_spelleffecthelp->GetSize(&x, &y);
		m_spelleffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
	} else if (id == wxID_STATUS) {
		sp.status = event.GetSelection();
		m_spellstatusint->SetValue(sp.status);
	} else if (id == wxID_TARGETTYPE) {
		Spell_Target_Type tt = event.GetSelection();
		sp.SetTargetType(tt);
		m_spelltargetamount->SetSelection(sp.GetTargetAmount());
		m_spelltargetpriority->SetSelection(sp.GetTargetPriority());
		m_spelltargetamount->Enable(tt != SPELL_TARGET_TYPE_EVERYONE && tt != SPELL_TARGET_TYPE_SELF && tt != SPELL_TARGET_TYPE_IRRELEVANT);
		m_spelltargetpriority->Enable(tt == SPELL_TARGET_TYPE_ANY);
	} else if (id == wxID_TARGETAMOUNT) {
		sp.SetTargetAmount(event.GetSelection());
	} else if (id == wxID_TARGETPRIORITY) {
		sp.SetTargetPriority(event.GetSelection());
	} else if (id == wxID_TARGETPANEL) {
		sp.SetPanel(event.GetSelection());
	}
	MarkDataSpellModified();
}

void CDDataStruct::OnSpellChangeFlags(wxCommandEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	int id = event.GetId();
	bool on = event.GetInt();
	MACRO_FLAG_SET8(sp.menu_flag,wxID_MF)
	MACRO_FLAG_SET8(sp.element,wxID_SE)
	MACRO_FLAG_SET(sp.target_flag,wxID_CAN_TARGET_DEAD,TARGET_FLAG_CAN_TARGET_DEAD)
	MACRO_FLAG_SET(sp.target_flag,wxID_TARGET_CAMERA,TARGET_FLAG_CAMERA)
	MACRO_FLAG_SET(sp.target_flag,wxID_TARGET_DEAD_FIRST,TARGET_FLAG_TARGET_DEAD_FIRST)
	MACRO_FLAG_SET8(sp.flag,wxID_SF)
	MarkDataSpellModified();
}

void CDDataStruct::OnSpellChangeButton(wxCommandEvent& event) {
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(m_spelllist->GetSelection());
	SpellDataStruct& sp = spellset.spell[*sortid];
	int id = event.GetId();
	if (id == wxID_NAME) {
		if (gametype == GAME_TYPE_PSX) {
			TextEditDialog ted(this, sp.name, spellset.name_space_total - spellset.name_space_used);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetName(ted.text);
				m_spellname->SetValue(sp.name.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, sp.name);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetName(ted.text);
				m_spellname->SetValue(sp.name.str);
			}
		}
	} else if (id == wxID_HELP) {
		if (gametype == GAME_TYPE_PSX) {
			CharmapDataStruct* chmap = NULL;
			if (saveset.sectionloaded[DATA_SECTION_TEXT])
				chmap = textset.charmap[textset.main_charmap_index];
			TextEditDialog ted(this, sp.help, spellset.help_space_total - spellset.help_space_used, TEXT_STYLE_HELP, chmap);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetHelp(ted.text);
				m_spellhelp->SetValue(sp.help.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, sp.help, TEXT_STYLE_HELP);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetHelp(ted.text);
				m_spellhelp->SetValue(sp.help.str);
			}
		}
	} else if (id == wxID_STATUS) {
		SpellStatusDialog spstat(this);
		if (spellset.GetStatusSetIndexById(sp.status) < 0)
			InsertAtId(spellset.status_set, spellset.status_set[0], sp.status);
		StatusSetStruct& statusset = spellset.GetStatusSetById(sp.status);
		if (spstat.ShowModal(this, statusset) == wxID_OK) {
			statusset.status_list = spstat.status_list;
			UpdateSpellStatusName(statusset);
			MarkDataSpellModified();
		}
	} else if (id == wxID_CFGROUP) {
		DatabaseFormatDialog dial(this, &spellset.csv_header, &spellset.csv_format, &spellset.custom_field);
		if (dial.ShowModal() == wxID_OK) {
			for (unsigned int i = 0; i < spellset.spell.size(); i++)
				UpdateCustomFieldMap(spellset.spell[i].custom_field, spellset.custom_field);
			MarkDataSpellModified();
		}
	} else if (id == wxID_CFENTRY) {
		DatabaseFieldDialog dial(this, &sp.custom_field, spellset.custom_field);
		if (dial.ShowModal() == wxID_OK)
			MarkDataSpellModified();
	}
}

void CDDataStruct::OnButtonClickSpellModel(wxCommandEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	TheSpellModelWindow->m_modelint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	if (TheSpellModelWindow->ShowModal(sp.GetTargetType(), sp.GetTargetAmount(), sp.model, sp.model_alt, false) == wxID_OK) {
		SortedChoiceItemModel* item;
		wxChoice* choice = TheSpellModelWindow->m_enableall->IsChecked() ? TheSpellModelWindow->m_modelall : TheSpellModelWindow->m_modelmain;
		item = (SortedChoiceItemModel*)choice->GetClientData(choice->GetSelection());
		if (TheSpellModelWindow->m_enableall->IsChecked() && item->id != TheSpellModelWindow->m_modelint->GetValue()) {
			sp.model = TheSpellModelWindow->m_modelint->GetValue();
			m_spellmodel->SetLabel(wxString::Format(wxT("SFX %d"), sp.model));
		} else {
			sp.model = item->id;
			m_spellmodel->SetLabel(item->label);
		}
		MarkDataSpellModified();
	}
}

void CDDataStruct::OnButtonClickSpellModelAlt(wxCommandEvent& event) {
	int sel = m_spelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spelllist->GetClientData(sel);
	SpellDataStruct& sp = spellset.spell[*sortid];
	TheSpellModelWindow->m_modelint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	if (TheSpellModelWindow->ShowModal(sp.GetTargetType(), sp.GetTargetAmount(), sp.model, sp.model_alt, true) == wxID_OK) {
		SortedChoiceItemModel* item;
		wxChoice* choice = TheSpellModelWindow->m_enableall->IsChecked() ? TheSpellModelWindow->m_modelall : TheSpellModelWindow->m_modelmain;
		item = (SortedChoiceItemModel*)choice->GetClientData(choice->GetSelection());
		if (TheSpellModelWindow->m_enableall->IsChecked() && item->id != TheSpellModelWindow->m_modelint->GetValue()) {
			sp.model_alt = TheSpellModelWindow->m_modelint->GetValue();
			m_spellmodelalt->SetLabel(wxString::Format(wxT("SFX %d"), sp.model_alt));
		} else {
			sp.model_alt = item->id;
			m_spellmodelalt->SetLabel(item->label);
		}
		MarkDataSpellModified();
	}
}

//=============================//
//         Supports            //
//=============================//

void CDDataStruct::DisplaySupportBoostedList(int supportindex, bool selectinboostlist) {
	SupportDataStruct& sp = supportset.support[supportindex];
	int basesupport = sp.boosted_support.size() > 0 ? supportindex : -1;
	unsigned int i, j;
	for (i = 0; basesupport < 0 && i < supportset.support.size(); i++)
		for (j = 0; basesupport < 0 && j < supportset.support[i].boosted_support.size(); j++)
			if (supportset.support[i].boosted_support[j] == sp.id)
				basesupport = i;
	if (basesupport >= 0) {
		int cumulatecost = supportset.support[basesupport].cost;
		m_supportboostedbase->SetSelection(basesupport);
		m_supportboostedlist->Clear();
		m_supportboostedlist->Append(wxString::Format(wxT("%s (%d)"), supportset.support[basesupport].name.GetStr(hades::TEXT_PREVIEW_TYPE), cumulatecost));
		if (selectinboostlist && basesupport == supportindex)
			m_supportboostedlist->SetSelection(0);
		for (j = 0; j < supportset.support[basesupport].boosted_support.size(); j++) {
			SupportDataStruct& othersp = supportset.GetSupportById(supportset.support[basesupport].boosted_support[j]);
			cumulatecost += othersp.cost;
			m_supportboostedlist->Append(wxString::Format(wxT("%s (%d)"), othersp.name.GetStr(hades::TEXT_PREVIEW_TYPE), cumulatecost));
			if (selectinboostlist && othersp.id == sp.id)
				m_supportboostedlist->SetSelection(j + 1);
		}
	} else {
		m_supportboostedbase->SetSelection(supportindex);
		m_supportboostedlist->Clear();
	}
	m_supportboostedbase->Enable(sp.boosted_support.size() == 0);
	m_supportscrolledwindow->Layout();
	m_supportscrolledwindow->GetParent()->GetSizer()->Layout();
	m_supportscrolledwindow->Refresh();
}

void CDDataStruct::DisplaySupport(int supportid) {
	unsigned int* sortid = (unsigned int*)m_supportlist->GetClientData(supportid);
	SupportDataStruct& sp = supportset.support[*sortid];
	m_supportid->Enable(false);
	m_supportid->SetValue(sp.id);
	m_supportcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_supportcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_supportcost->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_supportname->ChangeValue(_(sp.name.str));
	m_supporthelp->ChangeValue(_(sp.help.str));
	m_supportcost->SetValue(sp.cost);
	if (gametype == GAME_TYPE_STEAM && config.dll_usage != 0) {
		m_supportboostedlabel->Show(true);
		m_supportboostedpanel->Show(true);
		DisplaySupportBoostedList(*sortid, true);
	} else {
		m_supportboostedlabel->Show(false);
		m_supportboostedpanel->Show(false);
		m_supportscrolledwindow->Layout();
		m_supportscrolledwindow->GetParent()->GetSizer()->Layout();
		m_supportscrolledwindow->Refresh();
	}
}

void CDDataStruct::OnListBoxSupport(wxCommandEvent& event) {
	DisplaySupport(m_supportlist->GetSelection());
}

void CDDataStruct::OnSupportRightClick(wxMouseEvent& event) {
	int newsel = m_supportlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_supportlist->SetSelection(newsel);
		DisplaySupport(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 1;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= SUPPORT_AMOUNT);
		m_supportlist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnSupportChangeName(wxCommandEvent& event) {
	int sel = m_supportlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_supportlist->GetClientData(sel);
	SupportDataStruct& sp = supportset.support[*sortid];
	if (sp.SetName(m_supportname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_supportname->GetInsertionPoint();
		m_supportname->ChangeValue(_(sp.name.str));
		m_supportname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_supportlist->SetString(sel,_(sp.name.str));
		UpdateSupportName(*sortid);
		MarkDataSupportModified();
	}
}

void CDDataStruct::OnSupportChangeHelp(wxCommandEvent& event) {
	int sel = m_supportlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_supportlist->GetClientData(sel);
	SupportDataStruct& sp = supportset.support[*sortid];
	if (sp.SetHelp(m_supporthelp->GetValue().ToStdWstring())) {
		wxTextPos ip = m_supporthelp->GetInsertionPoint();
		m_supporthelp->ChangeValue(_(sp.help.str));
		m_supporthelp->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataSupportModified();
	}
}

void CDDataStruct::OnSupportChangeSpin(wxSpinEvent& event) {
	int sel = m_supportlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_supportlist->GetClientData(sel);
	SupportDataStruct& sp = supportset.support[*sortid];
	int id = event.GetId();
	if (id == wxID_ID) {
		sp.id = event.GetPosition();
	} else if (id == wxID_MAGICSTONE) {
		sp.cost = event.GetPosition();
		if (m_supportboostedlist->GetCount() > 0)
			DisplaySupportBoostedList(*sortid);
	}
	MarkDataSupportModified();
}

void CDDataStruct::OnSupportChangeChoice(wxCommandEvent& event) {
	unsigned int* sortid = (unsigned int*)m_supportlist->GetClientData(m_supportlist->GetSelection());
	SupportDataStruct& sp = supportset.support[*sortid];
	int id = event.GetId();
	if (id == wxID_BOOST) {
		unsigned int i, j;
		for (i = 0; i < supportset.support.size(); i++)
			for (j = 0; j < supportset.support[i].boosted_support.size(); j++)
				if (supportset.support[i].boosted_support[j] == sp.id) {
					supportset.support[i].boosted_support.erase(supportset.support[i].boosted_support.begin() + j);
					i = supportset.support.size();
					break;
				}
		SupportDataStruct* basesupp = &supportset.support[event.GetSelection()];
		for (i = 0; i < supportset.support.size(); i++)
			for (j = 0; j < supportset.support[i].boosted_support.size(); j++)
				if (supportset.support[i].boosted_support[j] == basesupp->id) {
					basesupp = &supportset.support[i];
					i = supportset.support.size();
					break;
				}
		if (basesupp != &sp)
			basesupp->boosted_support.push_back(sp.id);
		DisplaySupportBoostedList(*sortid);
	}
	MarkDataSupportModified();
}

void CDDataStruct::OnSupportChangeButton(wxCommandEvent& event) {
	unsigned int* sortid = (unsigned int*)m_supportlist->GetClientData(m_supportlist->GetSelection());
	SupportDataStruct& sp = supportset.support[*sortid];
	int id = event.GetId();
	if (id == wxID_NAME) {
		if (gametype == GAME_TYPE_PSX) {
			TextEditDialog ted(this, sp.name, supportset.name_space_total - supportset.name_space_used);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetName(ted.text);
				m_supportname->SetValue(sp.name.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, sp.name);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetName(ted.text);
				m_supportname->SetValue(sp.name.str);
			}
		}
	} else if (id == wxID_HELP) {
		if (gametype == GAME_TYPE_PSX) {
			CharmapDataStruct* chmap = NULL;
			if (saveset.sectionloaded[DATA_SECTION_TEXT])
				chmap = textset.charmap[textset.main_charmap_index];
			TextEditDialog ted(this, sp.help, supportset.help_space_total - supportset.help_space_used, TEXT_STYLE_HELP, chmap);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetHelp(ted.text);
				m_supporthelp->SetValue(sp.help.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, sp.help, TEXT_STYLE_HELP);
			if (ted.ShowModal() == wxID_OK) {
				sp.SetHelp(ted.text);
				m_supporthelp->SetValue(sp.help.str);
			}
		}
	} else if (id == wxID_CFGROUP) {
		DatabaseFormatDialog dial(this, &supportset.csv_header, &supportset.csv_format, &supportset.custom_field);
		if (dial.ShowModal() == wxID_OK) {
			for (unsigned int i = 0; i < supportset.support.size(); i++)
				UpdateCustomFieldMap(supportset.support[i].custom_field, supportset.custom_field);
			MarkDataSupportModified();
		}
	} else if (id == wxID_CFENTRY) {
		DatabaseFieldDialog dial(this, &sp.custom_field, supportset.custom_field);
		if (dial.ShowModal() == wxID_OK)
			MarkDataSupportModified();
	} else {
		int basesupport = sp.boosted_support.size() > 0 ? *sortid : -1;
		int listsel = m_supportboostedlist->GetSelection();
		int nextsel = wxNOT_FOUND;
		unsigned int i, j;
		for (i = 0; basesupport < 0 && i < supportset.support.size(); i++)
			for (j = 0; basesupport < 0 && j < supportset.support[i].boosted_support.size(); j++)
				if (supportset.support[i].boosted_support[j] == sp.id)
					basesupport = i;
		if (basesupport < 0)
			return;
		if (id == wxID_UP) {
			if (listsel == wxNOT_FOUND || listsel <= 0)
				return;
			if (listsel == 1) {
				SupportDataStruct& newbasesp = supportset.GetSupportById(supportset.support[basesupport].boosted_support[0]);
				newbasesp.boosted_support.clear();
				newbasesp.boosted_support.push_back(supportset.support[basesupport].id);
				for (i = 0; i < supportset.support[basesupport].boosted_support.size(); i++)
					if (supportset.support[basesupport].boosted_support[i] != newbasesp.id)
						newbasesp.boosted_support.push_back(supportset.support[basesupport].boosted_support[i]);
				supportset.support[basesupport].boosted_support.clear();
			} else {
				int tmp = supportset.support[basesupport].boosted_support[listsel - 1];
				supportset.support[basesupport].boosted_support[listsel - 1] = supportset.support[basesupport].boosted_support[listsel - 2];
				supportset.support[basesupport].boosted_support[listsel - 2] = tmp;
			}
			nextsel = listsel - 1;
		} else if (id == wxID_DOWN) {
			if (listsel == wxNOT_FOUND || listsel >= (int)m_supportboostedlist->GetCount() - 1)
				return;
			if (listsel == 0) {
				SupportDataStruct& newbasesp = supportset.GetSupportById(supportset.support[basesupport].boosted_support[0]);
				newbasesp.boosted_support.clear();
				newbasesp.boosted_support.push_back(supportset.support[basesupport].id);
				for (i = 0; i < supportset.support[basesupport].boosted_support.size(); i++)
					if (supportset.support[basesupport].boosted_support[i] != newbasesp.id)
						newbasesp.boosted_support.push_back(supportset.support[basesupport].boosted_support[i]);
				supportset.support[basesupport].boosted_support.clear();
			} else {
				int tmp = supportset.support[basesupport].boosted_support[listsel - 1];
				supportset.support[basesupport].boosted_support[listsel - 1] = supportset.support[basesupport].boosted_support[listsel];
				supportset.support[basesupport].boosted_support[listsel] = tmp;
			}
			nextsel = listsel + 1;
		}
		DisplaySupportBoostedList(*sortid);
		m_supportboostedlist->SetSelection(nextsel);
		MarkDataSupportModified();
	}
}

//=============================//
//         Commands            //
//=============================//

void CDDataStruct::DisplayCommand(int cmdid) {
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(cmdid);
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	int i;
	m_cmdid->Enable(false);
	m_cmdid->SetValue(cmd.id);
	m_cmdcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_cmdcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_cmdname->ChangeValue(_(cmd.name.str));
	m_cmdhelp->ChangeValue(_(cmd.help.str));
	m_cmdpanel->SetSelection(cmd.panel);
	m_cmdlink->SetSelection(cmd.link < 0 ? 0 : cmdset.GetCommandIndexById(cmd.link) + 1);
	m_cmdspelllist->Clear();
	if (cmd.panel == COMMAND_PANEL_SPELL) {
		m_cmdlink->Enable(true);
		m_cmdaddspell->Enable(true);
		m_cmdaddspell->SetLabel(HADES_STRING_COMMAND_ADD_SPELL_ADD);
		m_cmdremovespell->Enable(true);
		m_cmdspellfulllist->Enable(true);
		m_cmdspelllist->Enable(true);
		m_cmdmovespellup->Enable(true);
		m_cmdmovespelldown->Enable(true);
		for (i = 0; i < cmd.spell_amount; i++)
			m_cmdspelllist->Append(spellset.GetSpellById(cmd.spell_list[i]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
	} else {
		m_cmdlink->Enable(false);
		m_cmdaddspell->Enable(true);
		m_cmdaddspell->SetLabel(HADES_STRING_COMMAND_ADD_SPELL_EDIT);
		m_cmdremovespell->Enable(false);
		m_cmdspellfulllist->Enable(true);
		m_cmdspelllist->Enable(true);
		m_cmdmovespellup->Enable(false);
		m_cmdmovespelldown->Enable(false);
		m_cmdspelllist->Append(spellset.GetSpellById(cmd.spell_list[0]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
	}
	m_cmdspecificitems->Enable((cmd.panel == COMMAND_PANEL_ITEM || cmd.panel == COMMAND_PANEL_WEAPON) && gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_cmdspecificitems->SetValue((cmd.panel == COMMAND_PANEL_ITEM || cmd.panel == COMMAND_PANEL_WEAPON) && cmd.spell_amount == -1);
	m_cmdscrolledwindow->Layout();
	m_cmdscrolledwindow->GetParent()->GetSizer()->Layout();
	m_cmdscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxCommand(wxCommandEvent& event) {
	DisplayCommand(m_cmdlist->GetSelection());
}

void CDDataStruct::OnCommandRightClick(wxMouseEvent& event) {
	int newsel = m_cmdlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_cmdlist->SetSelection(newsel);
		DisplayCommand(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 2;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= COMMAND_AMOUNT);
		m_cmdlist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnCommandChangeName(wxCommandEvent& event) {
	int sel = m_cmdlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(sel);
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	if (cmd.SetName(m_cmdname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_cmdname->GetInsertionPoint();
		m_cmdname->ChangeValue(_(cmd.name.str));
		m_cmdname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_cmdlist->SetString(sel,_(cmd.name.str));
		UpdateCommandName(*sortid);
		MarkDataCommandModified();
	}
}

void CDDataStruct::OnCommandChangeHelp(wxCommandEvent& event) {
	int sel = m_cmdlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(sel);
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	if (cmd.SetHelp(m_cmdhelp->GetValue().ToStdWstring())) {
		wxTextPos ip = m_cmdhelp->GetInsertionPoint();
		m_cmdhelp->ChangeValue(_(cmd.help.str));
		m_cmdhelp->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataCommandModified();
	}
}

void CDDataStruct::OnCommandChangeSpin(wxSpinEvent& event) {
	int sel = m_cmdlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(sel);
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	int id = event.GetId();
	if (id == wxID_ID) {
		cmd.id = event.GetPosition();
	}
	MarkDataCommandModified();
}

void CDDataStruct::OnCommandChangeChoice(wxCommandEvent& event) {
	int sel = m_cmdlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(sel);
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	int id = event.GetId();
	if (id == wxID_PANEL) {
		cmd.SetPanel(event.GetSelection());
		DisplayCommand(sel);
	} else if (id == wxID_LINK) {
		if (event.GetSelection() == 0) {
			cmd.BreakLink();
		} else {
			int sel = event.GetSelection() - 1;
			if (cmdset.cmd[sel].link >= 0 || cmdset.cmd[sel].panel != COMMAND_PANEL_SPELL || sel == m_cmdlist->GetSelection()) {
				wxMessageDialog popup(NULL, HADES_STRING_COMMAND_CANT_CHANGE_LINK, HADES_STRING_ERROR, wxOK | wxCENTRE);
				popup.ShowModal();
			} else if (cmd.link < 0)
				cmd.AddLink(sel);
			else
				cmd.ChangeLink(sel);
		}
		DisplayCommand(sel);
	}
	MarkDataCommandModified();
}

void CDDataStruct::OnCommandChangeFlags(wxCommandEvent& event) {
	int sel = m_cmdlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(sel);
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	int id = event.GetId();
	bool on = event.GetInt();
	if (id == wxID_ITEM && (cmd.panel == COMMAND_PANEL_ITEM || cmd.panel == COMMAND_PANEL_WEAPON)) {
		cmd.spell_amount = on ? -1 : 0;
	}
	MarkDataCommandModified();
}

bool DiscardCmdSpellLimit = false;
void CDDataStruct::OnCommandChangeButton(wxCommandEvent& event) {
	unsigned int* sortid = (unsigned int*)m_cmdlist->GetClientData(m_cmdlist->GetSelection());
	CommandDataStruct& cmd = cmdset.cmd[*sortid];
	int listsel = m_cmdspelllist->GetSelection();
	int fulllistsel = m_cmdspellfulllist->GetSelection();
	int id = event.GetId();
	int i;
	if (id == wxID_ADD) {
		if (fulllistsel != wxNOT_FOUND) {
			if (cmd.panel == COMMAND_PANEL_SPELL) {
				if (listsel == wxNOT_FOUND)
					listsel = cmd.spell_amount;
				if (spellset.spell[fulllistsel].id == 0)
					return;
				if (!cmd.AddSpell(spellset.spell[fulllistsel].id, listsel, gametype != GAME_TYPE_STEAM || config.dll_usage == 0)) {
					if (!DiscardCmdSpellLimit) {
						DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_COMMAND_REACH_LIMIT);
						if (popup.ShowModal() == wxID_DISCARD)
							DiscardCmdSpellLimit = true;
					}
					return;
				} else {
					m_cmdspelllist->Append(spellset.GetSpellById(cmd.spell_list[cmd.spell_amount - 1]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
					for (i = listsel; i < cmd.spell_amount - 1; i++)
						m_cmdspelllist->SetString(i, spellset.GetSpellById(cmd.spell_list[i]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
					if (event.GetEventType() == wxEVT_BUTTON && fulllistsel + 1 < (int)m_cmdspellfulllist->GetCount())
						m_cmdspellfulllist->SetSelection(fulllistsel + 1);
				}
			} else {
				cmd.spell_list[0] = spellset.spell[fulllistsel].id;
				m_cmdspelllist->SetString(0, spellset.spell[fulllistsel].name.GetStr(hades::TEXT_PREVIEW_TYPE));
			}
			m_cmdscrolledwindow->Layout();
			m_cmdscrolledwindow->GetParent()->GetSizer()->Layout();
			m_cmdscrolledwindow->Refresh();
		}
	} else if (id == wxID_REMOVE) {
		if (listsel != wxNOT_FOUND && cmd.panel == COMMAND_PANEL_SPELL) {
			cmd.RemoveSpell(listsel, gametype != GAME_TYPE_STEAM || config.dll_usage == 0);
			m_cmdspelllist->Delete(listsel);
			if (cmd.spell_amount)
				m_cmdspelllist->SetSelection(listsel != cmd.spell_amount ? listsel : listsel - 1);
		} else {
			return;
		}
	} else if (id == wxID_UP) {
		if (listsel != wxNOT_FOUND && listsel != 0) {
			cmd.MoveSpell(listsel, listsel - 1);
			m_cmdspelllist->SetString(listsel, spellset.GetSpellById(cmd.spell_list[listsel]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
			m_cmdspelllist->SetString(listsel - 1, spellset.GetSpellById(cmd.spell_list[listsel - 1]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
			m_cmdspelllist->SetSelection(listsel - 1);
		} else {
			return;
		}
	} else if (id == wxID_DOWN) {
		if (listsel != wxNOT_FOUND && listsel != cmd.spell_amount - 1) {
			cmd.MoveSpell(listsel, listsel + 1);
			m_cmdspelllist->SetString(listsel, spellset.GetSpellById(cmd.spell_list[listsel]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
			m_cmdspelllist->SetString(listsel + 1, spellset.GetSpellById(cmd.spell_list[listsel + 1]).name.GetStr(hades::TEXT_PREVIEW_TYPE));
			m_cmdspelllist->SetSelection(listsel + 1);
		} else {
			return;
		}
	} else if (id == wxID_NAME) {
		if (gametype == GAME_TYPE_PSX) {
			TextEditDialog ted(this, cmd.name, cmdset.name_space_total - cmdset.name_space_used);
			if (ted.ShowModal() == wxID_OK) {
				cmd.SetName(ted.text);
				m_cmdname->SetValue(cmd.name.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, cmd.name);
			if (ted.ShowModal() == wxID_OK) {
				cmd.SetName(ted.text);
				m_cmdname->SetValue(cmd.name.str);
			}
		}
	} else if (id == wxID_HELP) {
		if (gametype == GAME_TYPE_PSX) {
			CharmapDataStruct* chmap = NULL;
			if (saveset.sectionloaded[DATA_SECTION_TEXT])
				chmap = textset.charmap[textset.main_charmap_index];
			TextEditDialog ted(this, cmd.help, cmdset.help_space_total - cmdset.help_space_used, TEXT_STYLE_HELP, chmap);
			if (ted.ShowModal() == wxID_OK) {
				cmd.SetHelp(ted.text);
				m_cmdhelp->SetValue(cmd.help.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, cmd.help, TEXT_STYLE_HELP);
			if (ted.ShowModal() == wxID_OK) {
				cmd.SetHelp(ted.text);
				m_cmdhelp->SetValue(cmd.help.str);
			}
		}
	} else if (id == wxID_CFGROUP) {
		DatabaseFormatDialog dial(this, &cmdset.csv_header, &cmdset.csv_format, &cmdset.custom_field);
		if (dial.ShowModal() == wxID_OK) {
			for (unsigned int i = 0; i < cmdset.cmd.size(); i++)
				UpdateCustomFieldMap(cmdset.cmd[i].custom_field, cmdset.custom_field);
			MarkDataCommandModified();
		}
		return;
	} else if (id == wxID_CFENTRY) {
		DatabaseFieldDialog dial(this, &cmd.custom_field, cmdset.custom_field);
		if (dial.ShowModal() == wxID_OK)
			MarkDataCommandModified();
		return;
	}
	MarkDataCommandModified();
}

//=============================//
//           Stats             //
//=============================//

void CDDataStruct::DisplayLevelStat(void) {
	unsigned int i;
	m_statlvlhpspin->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_statlvlmpspin->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_statlvlcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statlvlcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statlvlexplist->Clear();
	m_statlvlhplist->Clear();
	m_statlvlmplist->Clear();
	for (i = 0; i < MAX_LEVEL; i++) {
		stringstream explabel;
		explabel << "Level " << i + 1 << " : " << statset.level[i].exp_table << ends;
		m_statlvlexplist->Append(_(explabel.str()));
		stringstream hplabel;
		hplabel << "Level " << i + 1 << " : " << statset.level[i].hp_table << ends;
		m_statlvlhplist->Append(_(hplabel.str()));
		stringstream mplabel;
		mplabel << "Level " << i + 1 << " : " << statset.level[i].mp_table << ends;
		m_statlvlmplist->Append(_(mplabel.str()));
	}
	m_statlvlexplist->SetSelection(0);
	m_statlvlhplist->SetSelection(0);
	m_statlvlmplist->SetSelection(0);
	m_statlvlexpspin->SetValue(statset.level[0].exp_table);
	m_statlvlhpspin->SetValue(statset.level[0].hp_table);
	m_statlvlmpspin->SetValue(statset.level[0].mp_table);
	m_statlvlscrolledwindow->Layout();
	m_statlvlscrolledwindow->GetParent()->GetSizer()->Layout();
	m_statlvlscrolledwindow->Refresh();
}

void CDDataStruct::DisplayStatSelectedAbilityInfo(AbilitySetDataStruct& ab, int absel, int charindex) {
	if (ab.entry[absel].ability.IsVoid()) {
		m_statcharabilitylearninfo->SetLabelText(wxEmptyString);
		return;
	}
	InitialStatDataStruct& is = statset.initial_stat[charindex];
	wxString learninfo = m_statcharabilitychoice->GetStringSelection() + _(L" can be learned from:\n");
	for (unsigned int i = 0; i < itemset.item.size(); i++) {
		if (find(itemset.item[i].skill.begin(), itemset.item[i].skill.end(), ab.entry[absel].ability) == itemset.item[i].skill.end())
			continue;
		bool canequip;
		if (is.id >= PLAYABLE_CHAR_AMOUNT)
			canequip = find(is.equip_patch.begin(), is.equip_patch.end(), itemset.item[i].id) != is.equip_patch.end();
		else
			canequip = ((itemset.item[i].char_availability >> (PLAYABLE_CHAR_AMOUNT - is.id - 1)) & 1) != 0;
		if (canequip)
			learninfo += _(L" - ") + itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE) + _(L" " HADES_STRING_SPECIAL_CHAR_VALIDATE L"\n");
		else
			learninfo += _(L" - ") + itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE) + _(L" " HADES_STRING_SPECIAL_CHAR_INVALIDATE L"\n");
	}
	m_statcharabilitylearninfo->SetLabelText(learninfo);
}

void CDDataStruct::DisplayStatSelectedAbility() {
	int* cmdset = statset.GetCharacterCommandsIndices(m_statlist->GetSelection() - 1, NULL);
	AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
	int absel = m_statcharabilitylist->GetSelection();
	if (ab.entry[absel].ability.is_active)
		m_statcharabilitychoice->SetSelection(spellset.GetSpellIndexById(ab.entry[absel].ability.id));
	else
		m_statcharabilitychoice->SetSelection(spellset.spell.size() + supportset.GetSupportIndexById(ab.entry[absel].ability.id));
	m_statcharabilitycost->SetValue(ab.entry[absel].ap_cost);
	DisplayStatSelectedAbilityInfo(ab, absel, m_statlist->GetSelection() - 1);
}

void CDDataStruct::DisplayStatAbilityList(int abilsetid) {
	AbilitySetDataStruct& ab = statset.ability_list[abilsetid];
	CommandSetDataStruct& cmd = statset.command_list[abilsetid];
	m_statcharabilitylist->Clear();
	if (ab.entry.size() > 0) {
		for (unsigned int i = 0; i < ab.entry.size(); i++) {
			if (ab.entry[i].ability.is_active)
				m_statcharabilitylist->Append(_(spellset.GetSpellById(ab.entry[i].ability.id).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
			else
				m_statcharabilitylist->Append(_(supportset.GetSupportById(ab.entry[i].ability.id).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
		m_statcharabilitylist->SetSelection(0);
		DisplayStatSelectedAbility();
		m_statcharabilitylist->Enable();
		m_statcharabilitychoice->Enable();
		m_statcharabilitycost->Enable();
	} else {
		m_statcharabilitychoice->SetSelection(0);
		m_statcharabilitycost->SetValue(0);
		m_statcharabilitylist->Enable(false);
		m_statcharabilitychoice->Enable(false);
		m_statcharabilitycost->Enable(false);
	}
	m_statcharcommandattack->SetSelection(cmdset.GetCommandIndexById(cmd.attack_command));
	m_statcharcommanddefend->SetSelection(cmdset.GetCommandIndexById(cmd.defend_command));
	m_statcharcommand1->SetSelection(cmdset.GetCommandIndexById(cmd.first_command));
	m_statcharcommand2->SetSelection(cmdset.GetCommandIndexById(cmd.second_command));
	m_statcharcommanditem->SetSelection(cmdset.GetCommandIndexById(cmd.item_command));
	m_statcharcommandchange->SetSelection(cmdset.GetCommandIndexById(cmd.change_command));
	m_statcharcommandtranceattack->SetSelection(cmdset.GetCommandIndexById(cmd.attack_command_trance));
	m_statcharcommandtrancedefend->SetSelection(cmdset.GetCommandIndexById(cmd.defend_command_trance));
	m_statcharcommandtrance1->SetSelection(cmdset.GetCommandIndexById(cmd.first_command_trance));
	m_statcharcommandtrance2->SetSelection(cmdset.GetCommandIndexById(cmd.second_command_trance));
	m_statcharcommandtranceitem->SetSelection(cmdset.GetCommandIndexById(cmd.item_command_trance));
	m_statcharcommandtrancechange->SetSelection(cmdset.GetCommandIndexById(cmd.change_command_trance));
	m_statchartranceattack->SetSelection(cmd.trance_attack != 0xFF ? cmd.trance_attack + 1 : 0);
}

void CDDataStruct::DisplayStat(int statid) {
	if (statid == 0) {
		m_statcharscrolledwindow->Show(false);
		m_statlvlscrolledwindow->Show(true);
		DisplayLevelStat();
		return;
	}
	statid--;
	m_statlvlscrolledwindow->Show(false);
	m_statcharscrolledwindow->Show(true);
	InitialStatDataStruct& is = statset.initial_stat[statid];
	unsigned int i, cmdamount, equipamount, paramcount;
	m_statcharid->Enable(false);
	m_statcharid->SetValue(is.id);
	m_statcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcustomkind->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statchardefaultname->ChangeValue(_(is.default_name.str));
	m_statchardefaultname->Enable(gametype == GAME_TYPE_PSX || is.id >= PLAYABLE_CHAR_AMOUNT);
	m_statchardefaultname->SetToolTip(is.id >= PLAYABLE_CHAR_AMOUNT ? _(L"Only cosmetic for now\nSetup custom character names\nin your mod's \"DictionaryPatch.txt\"") : wxEmptyString); // TODO: externalise names automatically
	m_statchardefaultnamebutton->Enable(gametype == GAME_TYPE_PSX || is.id >= PLAYABLE_CHAR_AMOUNT);
	m_statcharadvanced->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharadvancedkind->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommandattack->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommanddefend->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommanditem->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommandchange->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommandtranceattack->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommandtrancedefend->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommandtranceitem->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharcommandtrancechange->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_statcharabilitycost->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_statcharspeed->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_statcharstrength->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_statcharmagic->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_statcharspirit->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_statcharmagicstone->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_statcharspeed->SetValue(is.speed);
	m_statcharstrength->SetValue(is.strength);
	m_statcharmagic->SetValue(is.magic);
	m_statcharspirit->SetValue(is.spirit);
	m_statcharmagicstone->SetValue(is.magic_stone);
	int* paramids = StatDataSet::GetCharacterBattleParametersIndices(statid, &paramcount);
	m_statcharadvancedkind->Clear();
	for (i = 0; i < paramcount; i++) {
		stringstream paramlabel;
		paramlabel << "Battle Parameters " << statset.battle_param[paramids[i]].id << ends;
		m_statcharadvancedkind->Append(_(paramlabel.str()));
	}
	m_statcharadvancedkind->SetSelection(0);
	int* cmdset = statset.GetCharacterCommandsIndices(statid, &cmdamount);
	m_statcharabilityset->Clear();
	for (i = 0; i < cmdamount; i++) {
		stringstream cmdlabel;
		cmdlabel << "Ability Set " << statset.command_list[cmdset[i]].id << ends;
		m_statcharabilityset->Append(_(cmdlabel.str()));
	}
	m_statcharabilityset->SetSelection(0);
	DisplayStatAbilityList(cmdset[0]);
	int* equipset = statset.GetCharacterEquipmentsIndices(statid, &equipamount);
	m_statcharequipset->Clear();
	for (i = 0; i + 1 < equipamount; i++) {
		stringstream equiplabel;
		equiplabel << "Equipment Set " << statset.initial_equip[equipset[i]].id << ends;
		m_statcharequipset->Append(_(equiplabel.str()));
	}
	m_statcharequipset->Append(_("Equipment Set Empty (15)"));
	m_statcharequipset->SetSelection(0);
	InitialEquipDataStruct& equip = statset.initial_equip[equipset[0]];
	m_statcharweapon->SetSelection(itemset.GetItemIndexById(equip.weapon));
	m_statcharhead->SetSelection(itemset.GetItemIndexById(equip.head));
	m_statcharwrist->SetSelection(itemset.GetItemIndexById(equip.wrist));
	m_statchararmor->SetSelection(itemset.GetItemIndexById(equip.armor));
	m_statcharaccessory->SetSelection(itemset.GetItemIndexById(equip.accessory));
	m_statcharequiplist->Clear();
	if (is.id >= PLAYABLE_CHAR_AMOUNT) {
		for (i = 0; i < is.equip_patch.size(); i++)
			m_statcharequiplist->Append(_(itemset.GetItemById(is.equip_patch[i]).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	} else {
		for (i = 0;i< itemset.item.size();i++)
			if ((itemset.item[i].char_availability >> (PLAYABLE_CHAR_AMOUNT - is.id - 1)) & 1)
				m_statcharequiplist->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	m_statcharscrolledwindow->Layout();
	m_statcharscrolledwindow->GetParent()->GetSizer()->Layout();
	m_statcharscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxStat(wxCommandEvent& event) {
	DisplayStat(m_statlist->GetSelection());
}

void CDDataStruct::OnStatRightClick(wxMouseEvent& event) {
	int newsel = m_statlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND && newsel > 0) {
		m_statlist->SetSelection(newsel);
		DisplayStat(newsel);
		SharedMenuSelection = newsel - 1;
		SharedMenuType = 3;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel - 1 >= PLAYABLE_CHAR_AMOUNT);
		m_statlist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnStatAbilityRightClick(wxMouseEvent& event) {
	int newsel = m_statcharabilitylist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND && newsel > 0) {
		int* cmdset = statset.GetCharacterCommandsIndices(m_statlist->GetSelection() - 1, NULL);
		AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
		m_statcharabilitylist->SetSelection(newsel);
		DisplayStatSelectedAbility();
		SharedMenuSelection = newsel;
		SharedMenuType = 1000;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && ab.entry.size() > ABILITY_SET_CAPACITY);
		m_statlist->PopupMenu(sharedmenu);
	}
}

AbilityEntryDataStruct StatAbilityEntryCopy;
void CDDataStruct::OnStatKeyDown(wxKeyEvent& event) {
	int sel = m_statlist->GetSelection();
	int setsel = m_statcharabilityset->GetSelection();
	int abilsel = m_statcharabilitylist->GetSelection();
	if (sel != wxNOT_FOUND && sel > 0 && setsel != wxNOT_FOUND && abilsel != wxNOT_FOUND) {
		if (event.GetModifiers() == wxMOD_CONTROL && event.GetKeyCode() == 'C') {
			int* cmdset = statset.GetCharacterCommandsIndices(sel - 1, NULL);
			StatAbilityEntryCopy = statset.ability_list[cmdset[setsel]].entry[abilsel];
			return;
		} else if (event.GetModifiers() == wxMOD_CONTROL && event.GetKeyCode() == 'V') {
			int* cmdset = statset.GetCharacterCommandsIndices(sel - 1, NULL);
			AbilityEntryDataStruct& entry = statset.ability_list[cmdset[setsel]].entry[abilsel];
			entry.ability = StatAbilityEntryCopy.ability;
			entry.ap_cost = StatAbilityEntryCopy.ap_cost;
			entry.custom_field = StatAbilityEntryCopy.custom_field;
			DisplayStatSelectedAbility();
			if (entry.ability.is_active)
				m_statcharabilitylist->SetString(abilsel, _(spellset.GetSpellById(entry.ability.id).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
			else
				m_statcharabilitylist->SetString(abilsel, _(supportset.GetSupportById(entry.ability.id).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
			MarkDataStatModified();
			return;
		}
	}
	event.Skip();
}

void CDDataStruct::OnStatChangeDefaultName(wxCommandEvent& event) {
	int sel = m_statlist->GetSelection();
	InitialStatDataStruct& is = statset.initial_stat[sel - 1];
	if (is.SetDefaultName(m_statchardefaultname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_statchardefaultname->GetInsertionPoint();
		m_statchardefaultname->ChangeValue(_(is.default_name.str));
		m_statchardefaultname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_statlist->SetString(sel, _(is.default_name.str));
		UpdateStatName(sel - 1);
		MarkDataStatModified();
	}
}

void CDDataStruct::OnStatChangeSpin(wxSpinEvent& event) {
	int sel = m_statlist->GetSelection();
	int id = event.GetId();
	if (id == wxID_ID) {
		InitialStatDataStruct& is = statset.initial_stat[sel - 1];
		is.id = event.GetPosition();
	} else if (id == wxID_SPEED) {
		InitialStatDataStruct& is = statset.initial_stat[sel - 1];
		is.speed = event.GetPosition();
	} else if (id == wxID_STRENGTH) {
		InitialStatDataStruct& is = statset.initial_stat[sel - 1];
		is.strength = event.GetPosition();
	} else if (id == wxID_MAGIC) {
		InitialStatDataStruct& is = statset.initial_stat[sel - 1];
		is.magic = event.GetPosition();
	} else if (id == wxID_SPIRIT) {
		InitialStatDataStruct& is = statset.initial_stat[sel - 1];
		is.spirit = event.GetPosition();
	} else if (id == wxID_MAGICSTONE) {
		InitialStatDataStruct& is = statset.initial_stat[sel - 1];
		is.magic_stone = event.GetPosition();
	} else if (id == wxID_ABILITYCOST) {
		int* cmdset = statset.GetCharacterCommandsIndices(sel - 1, NULL);
		AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
		ab.entry[m_statcharabilitylist->GetSelection()].ap_cost = event.GetPosition();
	} else if (id == wxID_EXP) {
		int lvlsel = m_statlvlexplist->GetSelection();
		LevelDataStruct& lv = statset.level[lvlsel];
		lv.exp_table = m_statlvlexpspin->GetValue();
		wxString explabel;
		explabel << "Level " << lvlsel + 1 << " : " << statset.level[lvlsel].exp_table;
		m_statlvlexplist->SetString(lvlsel, explabel);
	} else if (id == wxID_HP) {
		int lvlsel = m_statlvlhplist->GetSelection();
		LevelDataStruct& lv = statset.level[lvlsel];
		lv.hp_table = event.GetPosition();
		stringstream hplabel;
		hplabel << "Level " << lvlsel + 1 << " : " << statset.level[lvlsel].hp_table << ends;
		m_statlvlhplist->SetString(lvlsel, _(hplabel.str()));
	} else if (id == wxID_MP) {
		int lvlsel = m_statlvlmplist->GetSelection();
		LevelDataStruct& lv = statset.level[lvlsel];
		lv.mp_table = event.GetPosition();
		stringstream mplabel;
		mplabel << "Level " << lvlsel + 1 << " : " << statset.level[lvlsel].mp_table << ends;
		m_statlvlmplist->SetString(lvlsel, _(mplabel.str()));
	}
	MarkDataStatModified();
}

void CDDataStruct::OnStatChangeChoice(wxCommandEvent& event) {
	int sel = m_statlist->GetSelection() - 1;
	int id = event.GetId();

#define MACRO_HANDLE_COMMAND_EVENT(ID, FIELD) \
	} else if (id == ID) { \
		int* cmdarray = statset.GetCharacterCommandsIndices(sel, NULL); \
		CommandSetDataStruct& cmd = statset.command_list[cmdarray[m_statcharabilityset->GetSelection()]]; \
		cmd.FIELD = cmdset.cmd[event.GetSelection()].id;

	if (id == wxID_ABILITYSET) {
		int* cmdarray = statset.GetCharacterCommandsIndices(sel, NULL);
		DisplayStatAbilityList(cmdarray[event.GetSelection()]);
		return;
	} else if (id == wxID_ABILITY) {
		int* cmdarray = statset.GetCharacterCommandsIndices(sel, NULL);
		AbilitySetDataStruct& ab = statset.ability_list[cmdarray[m_statcharabilityset->GetSelection()]];
		int absel = m_statcharabilitylist->GetSelection();
		if (event.GetSelection() < (int)spellset.spell.size()) {
			SpellDataStruct& sp = spellset.spell[event.GetSelection()];
			ab.entry[absel].ability.Setup(sp.id, true);
			m_statcharabilitylist->SetString(absel, _(sp.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		} else {
			SupportDataStruct& sp = supportset.support[event.GetSelection() - spellset.spell.size()];
			ab.entry[absel].ability.Setup(sp.id, false);
			m_statcharabilitylist->SetString(absel, _(sp.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
		DisplayStatSelectedAbilityInfo(ab, absel, sel);
		m_statcharscrolledwindow->Layout();
		m_statcharscrolledwindow->GetParent()->GetSizer()->Layout();
		m_statcharscrolledwindow->Refresh();
	MACRO_HANDLE_COMMAND_EVENT(wxID_COMMANDATK, attack_command)
	MACRO_HANDLE_COMMAND_EVENT(wxID_COMMANDDEF, defend_command)
	MACRO_HANDLE_COMMAND_EVENT(wxID_COMMAND1, first_command)
	MACRO_HANDLE_COMMAND_EVENT(wxID_COMMAND2, second_command)
	MACRO_HANDLE_COMMAND_EVENT(wxID_COMMANDITEM, item_command)
	MACRO_HANDLE_COMMAND_EVENT(wxID_COMMANDCHANGE, change_command)
	MACRO_HANDLE_COMMAND_EVENT(wxID_TRANCE_COMMANDATK, attack_command_trance)
	MACRO_HANDLE_COMMAND_EVENT(wxID_TRANCE_COMMANDDEF, defend_command_trance)
	MACRO_HANDLE_COMMAND_EVENT(wxID_TRANCE_COMMAND1, first_command_trance)
	MACRO_HANDLE_COMMAND_EVENT(wxID_TRANCE_COMMAND2, second_command_trance)
	MACRO_HANDLE_COMMAND_EVENT(wxID_TRANCE_COMMANDITEM, item_command_trance)
	MACRO_HANDLE_COMMAND_EVENT(wxID_TRANCE_COMMANDCHANGE, change_command_trance)
	} else if (id == wxID_TRANCE_ATTACK) {
		int* cmdarray = statset.GetCharacterCommandsIndices(sel, NULL);
		CommandSetDataStruct& cmd = statset.command_list[cmdarray[m_statcharabilityset->GetSelection()]];
		cmd.trance_attack = event.GetSelection() != 0 ? event.GetSelection() - 1 : 0xFF;
	} else if (id == wxID_EQUIPSET) {
		int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
		InitialEquipDataStruct& equip = statset.initial_equip[equipset[event.GetSelection()]];
		m_statcharweapon->SetSelection(itemset.GetItemIndexById(equip.weapon));
		m_statcharhead->SetSelection(itemset.GetItemIndexById(equip.head));
		m_statcharwrist->SetSelection(itemset.GetItemIndexById(equip.wrist));
		m_statchararmor->SetSelection(itemset.GetItemIndexById(equip.armor));
		m_statcharaccessory->SetSelection(itemset.GetItemIndexById(equip.accessory));
		return;
	} else if (id == wxID_WEAPON) {
		int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
		InitialEquipDataStruct& equip = statset.initial_equip[equipset[m_statcharequipset->GetSelection()]];
		equip.weapon = itemset.item[event.GetSelection()].id;
	} else if (id == wxID_HEAD) {
		int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
		InitialEquipDataStruct& equip = statset.initial_equip[equipset[m_statcharequipset->GetSelection()]];
		equip.head = itemset.item[event.GetSelection()].id;
	} else if (id == wxID_WRIST) {
		int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
		InitialEquipDataStruct& equip = statset.initial_equip[equipset[m_statcharequipset->GetSelection()]];
		equip.wrist = itemset.item[event.GetSelection()].id;
	} else if (id == wxID_ARMOR) {
		int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
		InitialEquipDataStruct& equip = statset.initial_equip[equipset[m_statcharequipset->GetSelection()]];
		equip.armor = itemset.item[event.GetSelection()].id;
	} else if (id == wxID_ACCESSORY) {
		int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
		InitialEquipDataStruct& equip = statset.initial_equip[equipset[m_statcharequipset->GetSelection()]];
		equip.accessory = itemset.item[event.GetSelection()].id;
	}
	MarkDataStatModified();
}

void CDDataStruct::OnStatChangeButton(wxCommandEvent& event) {
	int sel = m_statlist->GetSelection() - 1;
	int id = event.GetId();
	unsigned int i;
	if (id == wxID_NAME) {
		InitialStatDataStruct& is = statset.initial_stat[sel];
		if (gametype == GAME_TYPE_PSX) {
			TextEditDialog ted(this, is.default_name, statset.default_name_space_total - statset.default_name_space_used);
			if (ted.ShowModal() == wxID_OK) {
				is.SetDefaultName(ted.text);
				m_statchardefaultname->SetValue(is.default_name.str);
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, is.default_name);
			if (ted.ShowModal() == wxID_OK) {
				is.SetDefaultName(ted.text);
				m_statchardefaultname->SetValue(is.default_name.str);
			}
		}
	} else if (id == wxID_PARAM) {
		unsigned int paramcount = 0;
		InitialStatDataStruct& is = statset.initial_stat[sel];
		int* paramids = StatDataSet::GetCharacterBattleParametersIndices(sel, &paramcount);
		CharBattleParameterStruct& bp = statset.battle_param[paramids[max(0, min((int)paramcount - 1, m_statcharadvancedkind->GetSelection()))]];
		CharacterParameterDialog dial(this);
		if (dial.ShowModal(bp, is) == wxID_OK) {
			dial.ApplyModifications(bp, is);
			MarkDataStatModified();
		}
	} else if (id == wxID_MENUPOSUP) {
		int abilsel = m_statcharabilitylist->GetSelection();
		if (abilsel != wxNOT_FOUND && abilsel > 0) {
			int* cmdset = statset.GetCharacterCommandsIndices(sel, NULL);
			AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
			wxString tmplabel = m_statcharabilitylist->GetString(abilsel);
			AbilityEntryDataStruct tmpabil = ab.entry[abilsel];
			ab.entry[abilsel] = ab.entry[abilsel - 1];
			ab.entry[abilsel - 1] = tmpabil;
			m_statcharabilitylist->SetString(abilsel, m_statcharabilitylist->GetString(abilsel - 1));
			m_statcharabilitylist->SetString(abilsel - 1, tmplabel);
			m_statcharabilitylist->SetSelection(abilsel - 1);
			MarkDataStatModified();
		}
	} else if (id == wxID_MENUPOSDOWN) {
		int* cmdset = statset.GetCharacterCommandsIndices(sel, NULL);
		AbilitySetDataStruct& ab = statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]];
		int abilsel = m_statcharabilitylist->GetSelection();
		if (abilsel != wxNOT_FOUND && abilsel + 1 < (int)ab.entry.size()) {
			wxString tmplabel = m_statcharabilitylist->GetString(abilsel);
			AbilityEntryDataStruct tmpabil = ab.entry[abilsel];
			ab.entry[abilsel] = ab.entry[abilsel + 1];
			ab.entry[abilsel + 1] = tmpabil;
			m_statcharabilitylist->SetString(abilsel, m_statcharabilitylist->GetString(abilsel + 1));
			m_statcharabilitylist->SetString(abilsel + 1, tmplabel);
			m_statcharabilitylist->SetSelection(abilsel + 1);
			MarkDataStatModified();
		}
	} else if (id == wxID_EQUIPADD) {
		wxArrayInt itemsels;
		int itemselcount = m_statcharequipfulllist->GetSelections(itemsels);
		for (int selindex = 0; selindex < itemselcount; selindex++) {
			int itemsel = itemsels[selindex];
			InitialStatDataStruct& is = statset.initial_stat[sel];
			ItemDataStruct& it = itemset.item[itemsel];
			if (it.id == 0xFF)
				return;
			int addindex = 0;
			if (is.id >= PLAYABLE_CHAR_AMOUNT) {
				addindex = InsertAtId(is.equip_patch, it.id, it.id);
				if (addindex < 0)
					return;
				MarkDataStatModified();
			} else {
				for (i = 0; i < itemset.item.size(); i++) {
					if (itemset.item[i].id > it.id)
						break;
					if ((itemset.item[i].char_availability >> (PLAYABLE_CHAR_AMOUNT - is.id - 1)) & 1) {
						if (itemset.item[i].id == it.id)
							return;
						addindex++;
					}
				}
				it.char_availability |= 1 << (PLAYABLE_CHAR_AMOUNT - is.id - 1);
				if (itemsel == *(unsigned int*)m_itemlist->GetClientData(m_itemlist->GetSelection()))
					DisplayItem(m_itemlist->GetSelection());
				MarkDataItemModified();
			}
			m_statcharequiplist->Insert(_(it.name.GetStr(hades::TEXT_PREVIEW_TYPE)), addindex);
			m_statcharequiplist->SetSelection(wxNOT_FOUND);
			m_statcharequiplist->SetSelection(addindex);
		}
		if (event.GetEventType() == wxEVT_BUTTON && itemselcount == 1) {
			m_statcharequipfulllist->SetSelection(wxNOT_FOUND);
			m_statcharequipfulllist->SetSelection(min(itemsels[0] + 1, (int)m_statcharequipfulllist->GetCount() - 1));
		}
		m_statcharscrolledwindow->Layout();
		m_statcharscrolledwindow->GetParent()->GetSizer()->Layout();
		m_statcharscrolledwindow->Refresh();
	} else if (id == wxID_EQUIPREMOVE) {
		wxArrayInt itemsels;
		int itemselcount = m_statcharequiplist->GetSelections(itemsels);
		for (int selindex = 0; selindex < itemselcount; selindex++) {
			int itemsel = itemsels[selindex];
			InitialStatDataStruct& is = statset.initial_stat[sel];
			if (is.id >= PLAYABLE_CHAR_AMOUNT) {
				is.equip_patch.erase(is.equip_patch.begin() + itemsel);
				MarkDataStatModified();
			} else {
				int itindex = -1;
				for (i = 0; i < itemset.item.size(); i++)
					if ((itemset.item[i].char_availability >> (PLAYABLE_CHAR_AMOUNT - is.id - 1)) & 1)
						if (itemsel == ++itindex)
							break;
				if (itemsel != itindex)
					return;
				itemset.item[i].char_availability &= ~(1 << (PLAYABLE_CHAR_AMOUNT - is.id - 1));
				if (i == *(unsigned int*)m_itemlist->GetClientData(m_itemlist->GetSelection()))
					DisplayItem(m_itemlist->GetSelection());
				MarkDataItemModified();
			}
			m_statcharequiplist->Delete(itemsel);
			for (int selcnt = selindex + 1; selcnt < itemselcount; selcnt++)
				if (itemsels[selcnt] >= itemsel)
					itemsels[selcnt]--;
		}
		if (itemselcount > 0 && m_statcharequiplist->GetCount() > 0) {
			m_statcharequiplist->SetSelection(wxNOT_FOUND);
			m_statcharequiplist->SetSelection(min(itemsels[itemselcount - 1], (int)m_statcharequiplist->GetCount() - 1));
		}
	} else if (id == wxID_EXP) {
		CurveEditorDialog statdial(this);
		uint32_t lvl[MAX_LEVEL];
		uint32_t exp[MAX_LEVEL];
		for (i = 0; i < MAX_LEVEL; i++) {
			lvl[i] = i + 1;
			exp[i] = statset.level[i].exp_table;
		}
		if (statdial.ShowModal(MAX_LEVEL, lvl, exp) == wxID_OK) {
			for (i = 0; i < MAX_LEVEL; i++)
				statset.level[i].exp_table = statdial.new_y[i];
			DisplayStat(m_statlist->GetSelection());
			MarkDataStatModified();
		}
	} else if (id == wxID_HP) {
		CurveEditorDialog statdial(this);
		uint32_t lvl[MAX_LEVEL];
		uint32_t hp[MAX_LEVEL];
		for (i = 0; i < MAX_LEVEL; i++) {
			lvl[i] = i + 1;
			hp[i] = statset.level[i].hp_table;
		}
		if (statdial.ShowModal(MAX_LEVEL, lvl, hp) == wxID_OK) {
			for (i = 0; i < MAX_LEVEL; i++)
				statset.level[i].hp_table = statdial.new_y[i];
			DisplayStat(m_statlist->GetSelection());
			MarkDataStatModified();
		}
	} else if (id == wxID_MP) {
		CurveEditorDialog statdial(this);
		uint32_t lvl[MAX_LEVEL];
		uint32_t mp[MAX_LEVEL];
		for (i = 0; i < MAX_LEVEL; i++) {
			lvl[i] = i + 1;
			mp[i] = statset.level[i].mp_table;
		}
		if (statdial.ShowModal(MAX_LEVEL, lvl, mp) == wxID_OK) {
			for (i = 0; i < MAX_LEVEL; i++)
				statset.level[i].mp_table = statdial.new_y[i];
			DisplayStat(m_statlist->GetSelection());
			MarkDataStatModified();
		}
	} else if (id == wxID_CFGROUP || id == wxID_CFENTRY) {
		wxString* header;
		wxString* format;
		map<wxString, wxString>* parentfields;
		map<wxString, wxString>* fields;
		int kind = 0;
		if (m_statlist->GetSelection() == 0) {
			header = &statset.csv_header_level;
			format = &statset.csv_format_level;
			parentfields = &statset.custom_field_level;
			fields = &statset.level[m_statlvlexplist->GetSelection()].custom_field;
		} else {
			kind = max(0, m_statcustomkind->GetSelection()) + 1;
			if (kind == 1) {
				header = &statset.csv_header_initial_stat;
				format = &statset.csv_format_initial_stat;
				parentfields = &statset.custom_field_initial_stat;
				fields = &statset.initial_stat[sel].custom_field;
			} else if (kind == 2) {
				header = &statset.csv_header_character_param;
				format = &statset.csv_format_character_param;
				parentfields = &statset.custom_field_initial_stat;
				fields = &statset.initial_stat[sel].custom_field;
			} else if (kind == 3) {
				header = &statset.csv_header_battle_param;
				format = &statset.csv_format_battle_param;
				parentfields = &statset.custom_field_battle_param;
				fields = &statset.battle_param[StatDataSet::GetCharacterBattleParametersIndices(sel)[max(0, m_statcharadvancedkind->GetSelection())]].custom_field;
			} else if (kind == 4) {
				if (m_statcharabilitylist->GetSelection() == wxNOT_FOUND)
					return;
				int* cmdset = statset.GetCharacterCommandsIndices(sel, NULL);
				header = &statset.csv_header_ability_list;
				format = &statset.csv_format_ability_list;
				parentfields = &statset.custom_field_ability_list;
				fields = &statset.ability_list[cmdset[m_statcharabilityset->GetSelection()]].entry[m_statcharabilitylist->GetSelection()].custom_field;
			} else if (kind == 5) {
				int* cmdset = statset.GetCharacterCommandsIndices(sel, NULL);
				header = &statset.csv_header_command_list;
				format = &statset.csv_format_command_list;
				parentfields = &statset.custom_field_command_list;
				fields = &statset.command_list[cmdset[m_statcharabilityset->GetSelection()]].custom_field;
			} else if (kind == 6) {
				int* equipset = statset.GetCharacterEquipmentsIndices(sel, NULL);
				header = &statset.csv_header_initial_equip;
				format = &statset.csv_format_initial_equip;
				parentfields = &statset.custom_field_initial_equip;
				fields = &statset.initial_equip[equipset[m_statcharequipset->GetSelection()]].custom_field;
			} else {
				return;
			}
		}
		if (id == wxID_CFGROUP) {
			DatabaseFormatDialog dial(this, header, format, parentfields);
			if (dial.ShowModal() == wxID_OK) {
				if (kind == 0)
					for (unsigned int i = 0; i < statset.level.size(); i++)
						UpdateCustomFieldMap(statset.level[i].custom_field, *parentfields);
				else if (kind == 1 || kind == 2)
					for (unsigned int i = 0; i < statset.initial_stat.size(); i++)
						UpdateCustomFieldMap(statset.initial_stat[i].custom_field, *parentfields);
				else if (kind == 3)
					for (unsigned int i = 0; i < statset.battle_param.size(); i++)
						UpdateCustomFieldMap(statset.battle_param[i].custom_field, *parentfields);
				else if (kind == 4)
					for (unsigned int i = 0; i < statset.ability_list.size(); i++)
						for (unsigned int j = 0; j < statset.ability_list[i].entry.size(); j++)
							UpdateCustomFieldMap(statset.ability_list[i].entry[j].custom_field, *parentfields);
				else if (kind == 5)
					for (unsigned int i = 0; i < statset.command_list.size(); i++)
						UpdateCustomFieldMap(statset.command_list[i].custom_field, *parentfields);
				else if (kind == 6)
					for (unsigned int i = 0; i < statset.initial_equip.size(); i++)
						UpdateCustomFieldMap(statset.initial_equip[i].custom_field, *parentfields);
				MarkDataStatModified();
			}
		} else {
			DatabaseFieldDialog dial(this, fields, *parentfields);
			if (dial.ShowModal() == wxID_OK)
				MarkDataStatModified();
		}
	}
}

void CDDataStruct::OnStatChangeList(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ABILITY) {
		DisplayStatSelectedAbility();
	} else if (id == wxID_EXP || id == wxID_HP || id == wxID_MP) {
		int sel = max(0, event.GetSelection());
		LevelDataStruct& lv = statset.level[sel];
		if (id != wxID_EXP) m_statlvlexplist->SetSelection(sel);
		if (id != wxID_HP)  m_statlvlhplist->SetSelection(sel);
		if (id != wxID_MP)  m_statlvlmplist->SetSelection(sel);
		m_statlvlexpspin->SetValue(lv.exp_table);
		m_statlvlhpspin->SetValue(lv.hp_table);
		m_statlvlmpspin->SetValue(lv.mp_table);
	}
}

//=============================//
//       Party Special         //
//=============================//

void CDDataStruct::DisplayPartySpecial(int specialid) {
	if (specialid == 0) {
		m_partyspecialcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_partyspecialcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_partyspecialmagicswordadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_partyspecialmagicswordremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && partyspecialset.magic_sword.size() > 0);
		m_partyspecialmagicswordcaster->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_partyspecialmagicswordwielder->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_partyspecialmagicswordspell->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_partyspecialmagicswordcasterstatusbaseint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 31);
		m_partyspecialmagicswordknightstatusbaseint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 31);
		if (partyspecialset.magic_sword.size() == 0 || m_partyspecialmagicswordset->GetSelection() == wxNOT_FOUND)
			return;
		MagicSwordDataStruct& mgswd = partyspecialset.magic_sword[m_partyspecialmagicswordset->GetSelection()];
		m_partyspecialmagicswordlist->Clear();
		for (unsigned int i = 0; i < mgswd.unlocked.size(); i++)
			m_partyspecialmagicswordlist->Append(GetMagicSwordName(spellset.GetSpellById(mgswd.unlocked[i]).name));
		m_partyspecialmagicswordcaster->SetSelection(statset.GetCharacterIndexById(mgswd.supporter));
		m_partyspecialmagicswordwielder->SetSelection(statset.GetCharacterIndexById(mgswd.beneficiary));
		m_partyspecialmagicswordlist->SetSelection(0);
		m_partyspecialmagicswordrequirement->SetSelection(spellset.GetSpellIndexById(mgswd.requirement[0]));
		m_partyspecialmagicswordspell->SetSelection(spellset.GetSpellIndexById(mgswd.unlocked[0]));
		m_partyspecialmagicswordcasterstatuslist->Clear();
		m_partyspecialmagicswordknightstatuslist->Clear();
		for (auto st = mgswd.supporter_status_blocker.begin(); st != mgswd.supporter_status_blocker.end(); st++)
			m_partyspecialmagicswordcasterstatuslist->Append(SpellStatusDialog::StatusDisplayName(*st), new int[1]{ *st });
		for (auto st = mgswd.beneficiary_status_blocker.begin(); st != mgswd.beneficiary_status_blocker.end(); st++)
			m_partyspecialmagicswordknightstatuslist->Append(SpellStatusDialog::StatusDisplayName(*st), new int[1]{ *st });
	}
}

void CDDataStruct::OnListBoxPartySpecial(wxCommandEvent& event) {
	DisplayPartySpecial(m_partyspeciallist->GetSelection());
}

void CDDataStruct::OnPartySpecialChangeSpin(wxSpinEvent& event) {
	int id = event.GetId();
	int status = event.GetPosition();
	if (id == wxID_STATUSCASTER) {
		m_partyspecialmagicswordcasterstatusbaselist->SetSelection(status < STATUS_AMOUNT ? status : -1);
	} else if (id == wxID_STATUSKNIGHT) {
		m_partyspecialmagicswordknightstatusbaselist->SetSelection(status < STATUS_AMOUNT ? status : -1);
	}
}

void CDDataStruct::OnPartySpecialChangeChoice(wxCommandEvent& event) {
	int swdset = m_partyspecialmagicswordset->GetSelection();
	if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
		return;
	MagicSwordDataStruct& mgswd = partyspecialset.magic_sword[swdset];
	int id = event.GetId();
	if (id == wxID_MAGICSEL) {
		DisplayPartySpecial(0);
		return;
	} else if (id == wxID_CASTER) {
		mgswd.supporter = statset.initial_stat[event.GetSelection()].id;
	} else if (id == wxID_WIELDER) {
		mgswd.beneficiary = statset.initial_stat[event.GetSelection()].id;
	} else if (id == wxID_MAGICREQ) {
		int sel = m_partyspecialmagicswordlist->GetSelection();
		mgswd.requirement[sel] = spellset.spell[event.GetSelection()].id;
	} else if (id == wxID_MAGICSWD) {
		int sel = m_partyspecialmagicswordlist->GetSelection();
		mgswd.unlocked[sel] = spellset.spell[event.GetSelection()].id;
		m_partyspecialmagicswordlist->SetString(sel, GetMagicSwordName(spellset.GetSpellById(mgswd.unlocked[sel]).name));
	}
	MarkDataPartySpecialModified();
}

void CDDataStruct::OnPartySpecialChangeList(wxCommandEvent& event) {
	int swdset = m_partyspecialmagicswordset->GetSelection();
	if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
		return;
	MagicSwordDataStruct& mgswd = partyspecialset.magic_sword[swdset];
	int id = event.GetId();
	if (id == wxID_MAGICSWD) {
		m_partyspecialmagicswordrequirement->SetSelection(spellset.GetSpellIndexById(mgswd.requirement[event.GetSelection()]));
		m_partyspecialmagicswordspell->SetSelection(spellset.GetSpellIndexById(mgswd.unlocked[event.GetSelection()]));
	}
}

void CDDataStruct::OnPartySpecialChangeButton(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADD) {
		int swdset = partyspecialset.magic_sword.size();
		partyspecialset.magic_sword.emplace_back();
		partyspecialset.magic_sword[swdset].id = swdset;
		partyspecialset.magic_sword[swdset].parent = &partyspecialset;
		partyspecialset.magic_sword[swdset].requirement.resize(8);
		partyspecialset.magic_sword[swdset].unlocked.resize(8);
		partyspecialset.magic_sword[swdset].supporter_status_blocker.clear();
		partyspecialset.magic_sword[swdset].beneficiary_status_blocker.clear();
		m_partyspecialmagicswordset->Append(wxString::Format(wxT(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SET), swdset));
		m_partyspecialmagicswordset->SetSelection(swdset);
		DisplayPartySpecial(0);
	} else if (id == wxID_REMOVE) {
		int sel = m_partyspecialmagicswordset->GetSelection();
		if (sel == wxNOT_FOUND)
			return;
		partyspecialset.magic_sword.erase(partyspecialset.magic_sword.begin() + sel);
		m_partyspecialmagicswordset->Delete(m_partyspecialmagicswordset->GetCount() - 1);
		for (unsigned int i = sel; i < partyspecialset.magic_sword.size(); i++)
			partyspecialset.magic_sword[i].id = i;
		if (partyspecialset.magic_sword.size() > 0) {
			m_partyspecialmagicswordset->SetSelection(min(sel, (int)m_partyspecialmagicswordset->GetCount() - 1));
		} else {
			m_partyspecialmagicswordset->SetSelection(wxNOT_FOUND);
			m_partyspecialmagicswordlist->Clear();
		}
		DisplayPartySpecial(0);
	} else if (id == wxID_ADDCASTER) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
			return;
		MagicSwordDataStruct& swd = partyspecialset.magic_sword[swdset];
		int status = m_partyspecialmagicswordcasterstatusbaseint->GetValue();
		if (swd.supporter_status_blocker.find(status) != swd.supporter_status_blocker.end())
			return;
		swd.supporter_status_blocker.insert(status);
		m_partyspecialmagicswordcasterstatuslist->Append(SpellStatusDialog::StatusDisplayName(status), new int[1]{ status });
	} else if (id == wxID_REMOVECASTER) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
			return;
		if (m_partyspecialmagicswordcasterstatuslist->GetSelection() == wxNOT_FOUND)
			return;
		int status = *(int*)m_partyspecialmagicswordcasterstatuslist->GetClientData(m_partyspecialmagicswordcasterstatuslist->GetSelection());
		if (partyspecialset.magic_sword[swdset].supporter_status_blocker.erase(status) > 0)
			m_partyspecialmagicswordcasterstatuslist->Delete(m_partyspecialmagicswordcasterstatuslist->GetSelection());
	} else if (id == wxID_ADDKNIGHT) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
			return;
		MagicSwordDataStruct& swd = partyspecialset.magic_sword[swdset];
		int status = m_partyspecialmagicswordknightstatusbaseint->GetValue();
		if (swd.beneficiary_status_blocker.find(status) != swd.beneficiary_status_blocker.end())
			return;
		swd.beneficiary_status_blocker.insert(status);
		m_partyspecialmagicswordknightstatuslist->Append(SpellStatusDialog::StatusDisplayName(status), new int[1]{ status });
	} else if (id == wxID_REMOVEKNIGHT) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
			return;
		if (m_partyspecialmagicswordknightstatuslist->GetSelection() == wxNOT_FOUND)
			return;
		int status = *(int*)m_partyspecialmagicswordknightstatuslist->GetClientData(m_partyspecialmagicswordknightstatuslist->GetSelection());
		if (partyspecialset.magic_sword[swdset].beneficiary_status_blocker.erase(status) > 0)
			m_partyspecialmagicswordknightstatuslist->Delete(m_partyspecialmagicswordknightstatuslist->GetSelection());
	} else if (id == wxID_CFGROUP) {
		DatabaseFormatDialog dial(this, &partyspecialset.csv_header, &partyspecialset.csv_format, &partyspecialset.custom_field);
		if (dial.ShowModal() == wxID_OK) {
			for (unsigned int i = 0; i < partyspecialset.magic_sword.size(); i++)
				UpdateCustomFieldMap(partyspecialset.magic_sword[i].custom_field, partyspecialset.custom_field);
			MarkDataPartySpecialModified();
		}
		return;
	} else if (id == wxID_CFENTRY) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
			return;
		DatabaseFieldDialog dial(this, &partyspecialset.magic_sword[swdset].custom_field, partyspecialset.custom_field);
		if (dial.ShowModal() == wxID_OK)
			MarkDataPartySpecialModified();
		return;
	}
	MarkDataPartySpecialModified();
}

void CDDataStruct::OnPartySpecialChangeListBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADDCASTER) {
		m_partyspecialmagicswordcasterstatusbaseint->SetValue(m_partyspecialmagicswordcasterstatusbaselist->GetSelection());
	} else if (id == wxID_ADDKNIGHT) {
		m_partyspecialmagicswordknightstatusbaseint->SetValue(m_partyspecialmagicswordknightstatusbaselist->GetSelection());
	}
}

void CDDataStruct::OnPartySpecialMagicSwordRightClick(wxMouseEvent& event) {
	int swdset = m_partyspecialmagicswordset->GetSelection();
	if (swdset < 0 || swdset >= (int)partyspecialset.magic_sword.size())
		return;
	MagicSwordDataStruct& mgswd = partyspecialset.magic_sword[swdset];
	int newsel = m_partyspecialmagicswordlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_partyspecialmagicswordlist->SetSelection(newsel);
		m_partyspecialmagicswordrequirement->SetSelection(spellset.GetSpellIndexById(mgswd.requirement[newsel]));
		m_partyspecialmagicswordspell->SetSelection(spellset.GetSpellIndexById(mgswd.unlocked[newsel]));
		SharedMenuSelection = newsel;
		SharedMenuType = 1010;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && mgswd.unlocked.size() > 1);
		m_partyspecialmagicswordlist->PopupMenu(sharedmenu);
	}
}

//=============================//
//           Items             //
//=============================//

ItemStatDataStruct& CDDataStruct::GetItemStat(int itemid) {
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(itemid);
	ItemDataStruct& it = itemset.item[*sortid];
	int statindex = itemset.GetStatIndexById(it.stat_id);
	if (statindex >= 0)
		return itemset.stat[statindex];
	ItemStatDataStruct newstruct;
	newstruct.InitializeDefault(it.stat_id);
	int addindex = InsertAtId<ItemStatDataStruct>(itemset.stat, newstruct, it.stat_id);
	return itemset.stat[addindex];
}

void CDDataStruct::DisplayItemStatIdHelp(void) {
	ItemStatDataStruct& is = GetItemStat(m_itemlist->GetSelection());
	wxString infolabel;
	infolabel << L"Strength +" << (int)is.strength << L"\tSpeed +" << (int)is.speed << L"\nMagic +" << (int)is.magic << L"\tSpirit +" << (int)is.spirit << L"\n";
	if (is.element_absorb) {
		bool addslash = false;
		infolabel << L"Absorb ";
		#define MACRO_ITEMSTAT_ELEMENT_TOKEN(FIELD,ID,STR) \
			if (is.FIELD & ID) { \
				if (addslash) { \
					infolabel << L" / " << STR; \
				} else { \
					infolabel << STR; \
					addslash = true; \
				} \
			}
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_FIRE,L"Fire")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_ICE,L"Ice")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_THUNDER,L"Thunder")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_EARTH,L"Earth")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_WATER,L"Water")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_WIND,L"Wind")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_HOLY,L"Holy")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_absorb,ELEMENT_SHADOW,L"Shadow")
		infolabel << L"\n";
	}
	if (is.element_immune) {
		bool addslash = false;
		infolabel << L"Immune against ";
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_FIRE,L"Fire")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_ICE,L"Ice")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_THUNDER,L"Thunder")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_EARTH,L"Earth")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_WATER,L"Water")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_WIND,L"Wind")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_HOLY,L"Holy")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_immune,ELEMENT_SHADOW,L"Shadow")
		infolabel << L"\n";
	}
	if (is.element_half) {
		bool addslash = false;
		infolabel << L"Half damage from ";
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_FIRE,L"Fire")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_ICE,L"Ice")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_THUNDER,L"Thunder")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_EARTH,L"Earth")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_WATER,L"Water")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_WIND,L"Wind")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_HOLY,L"Holy")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_half,ELEMENT_SHADOW,L"Shadow")
		infolabel << L"\n";
	}
	if (is.element_weak) {
		bool addslash = false;
		infolabel << L"Weaken against ";
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_FIRE,L"Fire")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_ICE,L"Ice")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_THUNDER,L"Thunder")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_EARTH,L"Earth")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_WATER,L"Water")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_WIND,L"Wind")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_HOLY,L"Holy")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_weak,ELEMENT_SHADOW,L"Shadow")
		infolabel << L"\n";
	}
	if (is.element_boost) {
		bool addslash = false;
		infolabel << L"Boost damage of ";
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_FIRE,L"Fire")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_ICE,L"Ice")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_THUNDER,L"Thunder")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_EARTH,L"Earth")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_WATER,L"Water")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_WIND,L"Wind")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_HOLY,L"Holy")
		MACRO_ITEMSTAT_ELEMENT_TOKEN(element_boost,ELEMENT_SHADOW,L"Shadow")
		infolabel << L"\n";
	}
	m_itemstatinfo->SetLabelText(infolabel);
}

void CDDataStruct::DisplayItemIcon(void) {
	if (gametype == GAME_TYPE_PSX) {
		TIMImageDataStruct& tim = textset.chartim[1][2];
		uint16_t palpos = m_itemiconcolor->GetValue();
		uint16_t texpos = ((m_itemicon->GetValue() % 18) * 7) | (((m_itemicon->GetValue() / 18) * 13) << 8);
		m_itemiconpreview->SetBitmap(ConvertTIMToBitmap(tim, tim, 21, 13, palpos, texpos, true));
	} else {
		wxString spritename = wxString::Format(wxT("item%02d_%02d"), m_itemicon->GetValue(), m_itemiconcolor->GetValue());
		wxImage tmpicon = ffuiset.steam_atlas[0].sprite[spritename].ConvertToImage();
		wxBitmap icon(tmpicon.Rescale(21, 13, wxIMAGE_QUALITY_HIGH));
		m_itemiconpreview->SetBitmap(icon);
	}
}

void CDDataStruct::DisplayItem(int itemid) {
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(itemid);
	ItemDataStruct& it = itemset.item[*sortid];
	bool isusable = it.usable_id >= 0;
	bool isweapon = it.weapon_id >= 0;
	bool isarmor = it.armor_id >= 0;
	unsigned int i;
	m_itemid->Enable(false);
	m_itemcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemcustomkind->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemhasusable->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemhasweapon->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemhasarmor->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemsellprice->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_itemprice->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_itemsellprice->SetRange(-1, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_itemicon->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 35);
	m_itemiconcolor->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? UINT8_MAX : 15);
	m_itemid->SetValue(it.id);
	m_itemname->ChangeValue(_(it.name.str));
	m_itemhelp->ChangeValue(_(it.help.str));
	m_itemhelp2->ChangeValue(_(it.battle_help.str));
	m_itemhasusable->SetValue(isusable);
	m_itemhasweapon->SetValue(isweapon);
	m_itemhasarmor->SetValue(isarmor);
	MACRO_FLAG_DISPLAY8(it.type, m_itemtype)
	m_itemprice->SetValue(it.price);
	m_itemsellprice->SetValue(it.sell_price);
	MACRO_FLAG_DISPLAY16(it.char_availability, m_itemcharavailability)
	m_itemstatid->SetSelection(min(ITEM_STAT_AMOUNT, itemset.GetStatIndexById(it.stat_id)));
	DisplayItemStatIdHelp();
	m_itemskilllist->Clear();
	for (i = 0; i < it.skill.size(); i++)
		if (!it.skill[i].IsVoid()) {
			if (it.skill[i].is_active) {
				SpellDataStruct& sp = spellset.GetSpellById(it.skill[i].id);
				m_itemskilllist->Append(wxString::Format(wxT("%s (A %d)"), sp.name.GetStr(hades::TEXT_PREVIEW_TYPE), sp.id));
			} else {
				SupportDataStruct& sp = supportset.GetSupportById(it.skill[i].id);
				m_itemskilllist->Append(wxString::Format(wxT("%s (S %d)"), sp.name.GetStr(hades::TEXT_PREVIEW_TYPE), sp.id));
			}
		}
	if (it.equip_position_type == ITEM_POSITION_ABSOLUTE) {
		m_itemmenuposnormalpanel->Show(true);
		m_itemmenuposextrapanel->Show(false);
		m_itemmenuposbuttonup->Enable(*sortid != 0xFF);
		m_itemmenuposbuttondown->Enable(*sortid != 0xFF);
		m_itemmenuposlist->SetSelection(it.menu_position);
	} else {
		m_itemmenuposnormalpanel->Show(false);
		m_itemmenuposextrapanel->Show(true);
		m_itemmenuposrelative->SetSelection(it.menu_position_type == ITEM_POSITION_AFTER ? 1 : 0);
		m_itemmenuposchoice->SetSelection(itemset.GetItemIndexById(it.menu_position));
	}
	if (it.equip_position_type == ITEM_POSITION_ABSOLUTE) {
		m_itemequipposnormalpanel->Show(true);
		m_itemequipposextrapanel->Show(false);
		m_itemequipposbuttonup->Enable(*sortid != 0xFF);
		m_itemequipposbuttondown->Enable(*sortid != 0xFF);
		m_itemequipposlist->SetSelection(it.equip_position);
	} else {
		m_itemequipposnormalpanel->Show(false);
		m_itemequipposextrapanel->Show(true);
		m_itemequipposrelative->SetSelection(it.equip_position_type - ITEM_POSITION_BEFORE);
		m_itemequipposchoice->SetSelection(itemset.GetItemIndexById(it.equip_position));
	}
	m_itemicon->SetValue(it.icon);
	m_itemiconcolor->SetValue(it.icon_color);
	DisplayItemIcon();
	m_itemzero->SetValue(it.zero);
	m_itemusablelabel->Show(isusable);
	m_itemusablepanel->Show(isusable);
	if (isusable) {
		ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
		m_itemusableeffectint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemusablepower->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemusablestatusbaseint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 31);
		m_itemusableaccuracy->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		if (ic.effect < HADES_STRING_SPELL_EFFECT_COUNT)
			m_itemusableeffect->SetSelection(ic.effect);
		m_itemusableeffectint->SetValue(ic.effect);
		if (ic.effect < (int)HADES_STRING_SPELL_EFFECT.size())
			m_itemusableeffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[ic.effect].help);
		else
			m_itemusableeffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
		int x, y;
		m_itemusableeffecthelp->GetSize(&x, &y);
		m_itemusableeffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
		m_itemusablepower->SetValue(ic.power);
		MACRO_FLAG_DISPLAY8(ic.element, m_itemusableelement)
		m_itemusablestatuslist->Clear();
		for (auto st = ic.status.begin(); st != ic.status.end(); st++)
			m_itemusablestatuslist->Append(SpellStatusDialog::StatusDisplayName(*st), new int[1]{ *st });
		m_itemusableaccuracy->SetValue(ic.accuracy);
		m_itemusablemodel->SetLabel(wxString::Format(wxT("SFX %d"), ic.model));
		for (i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++)
			if (HADES_STRING_SPELL_MODEL[i].id == ic.model) {
				m_itemusablemodel->SetLabel(HADES_STRING_SPELL_MODEL[i].label);
				break;
			}
		Spell_Target_Type tt = ic.GetTargetType();
		m_itemusabletargettype->SetSelection(tt);
		m_itemusabletargetamount->SetSelection(ic.GetTargetAmount());
		m_itemusabletargetpriority->SetSelection(ic.GetTargetPriority());
		m_itemusabletargetamount->Enable(tt != SPELL_TARGET_TYPE_EVERYONE && tt != SPELL_TARGET_TYPE_SELF && tt != SPELL_TARGET_TYPE_IRRELEVANT);
		m_itemusabletargetpriority->Enable(tt == SPELL_TARGET_TYPE_ANY);
		m_itemusabletargetflagdead->SetValue(ic.target_flag & TARGET_FLAG_CAN_TARGET_DEAD);
		m_itemusabletargetflagcamera->SetValue(ic.target_flag & TARGET_FLAG_CAMERA);
		m_itemusabletargetflagdeadfirst->SetValue(ic.target_flag & TARGET_FLAG_TARGET_DEAD_FIRST);
		m_itemusabletargetpanel->SetSelection(ic.GetPanel());
		m_itemusablepanel->Layout();
	}
	m_itemweaponlabel->Show(isweapon);
	m_itemweaponpanel->Show(isweapon);
	if (isweapon) {
		ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
		m_itemweaponformulaint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemweaponpower->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemweaponstatusint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : STATUS_SET_AMOUNT - 1);
		m_itemweaponaccuracy->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemweaponmodel->Enable(gametype == GAME_TYPE_PSX);
		m_itemweaponmodelname->Enable(gametype != GAME_TYPE_PSX);
		m_itemweapontexture->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_itemweaponsfx->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		m_itemweaponformula->SetSelection(iw.damage_formula);
		m_itemweaponformulaint->SetValue(iw.damage_formula);
		m_itemweaponpower->SetValue(iw.power);
		MACRO_FLAG_DISPLAY8(iw.element, m_itemweaponelement)
		if (iw.status < STATUS_SET_AMOUNT)
			m_itemweaponstatus->SetSelection(iw.status);
		m_itemweaponstatusint->SetValue(iw.status);
		m_itemweaponaccuracy->SetValue(iw.status_accuracy);
		MACRO_FLAG_DISPLAY8(iw.flag, m_itemweaponflag)
		m_itemweaponunk1->SetValue(iw.offset1);
		m_itemweaponunk2->SetValue(iw.offset2);
		m_itemweaponmodel->SetValue(iw.model);
		m_itemweaponmodelname->ChangeValue(iw.model_name);
		m_itemweapontexture->ChangeValue(iw.texture_names.size() > 0 ? iw.texture_names[0] : L"");
		m_itemweaponsfx->SetSelection(iw.hit_sfx);
		m_itemweaponpanel->Layout();
	}
	m_itemarmorlabel->Show(isarmor);
	m_itemarmorpanel->Show(isarmor);
	if (isarmor) {
		ItemArmorDataStruct& ia = itemset.GetArmorById(it.armor_id);
		m_itemarmordefence->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemarmorevade->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemarmormagicdefence->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemarmormagicevade->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
		m_itemarmordefence->SetValue(ia.defence);
		m_itemarmorevade->SetValue(ia.evade);
		m_itemarmormagicdefence->SetValue(ia.magic_defence);
		m_itemarmormagicevade->SetValue(ia.magic_evade);
		m_itemarmorpanel->Layout();
	}
	m_itemscrolledwindow->Layout();
	m_itemscrolledwindow->GetParent()->GetSizer()->Layout();
	m_itemscrolledwindow->Refresh();
}

void CDDataStruct::DisplayKeyItem(int keyitemid) {
	unsigned int* sortid = (unsigned int*)m_keyitemlist->GetClientData(keyitemid);
	KeyItemDataStruct& ki = itemset.key_item[*sortid];
	m_keyitemid->Enable(false);
	m_keyitemid->SetValue(ki.id);
	m_keyitemname->ChangeValue(_(ki.name.str));
	m_keyitemhelp->ChangeValue(_(ki.help.str));
	m_keyitemdescription->ChangeValue(_(ki.description.str));
	m_keyitemscrolledwindow->Layout();
	m_keyitemscrolledwindow->GetParent()->GetSizer()->Layout();
	m_keyitemscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxItem(wxCommandEvent& event) {
	DisplayItem(m_itemlist->GetSelection());
}

void CDDataStruct::OnListBoxKeyItem(wxCommandEvent& event) {
	DisplayKeyItem(m_keyitemlist->GetSelection());
}

void CDDataStruct::OnItemRightClick(wxMouseEvent& event) {
	int newsel = m_itemlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_itemlist->SetSelection(newsel);
		DisplayItem(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 4;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= ITEM_AMOUNT);
		m_itemlist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnKeyItemRightClick(wxMouseEvent& event) {
	int newsel = m_keyitemlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_keyitemlist->SetSelection(newsel);
		DisplayKeyItem(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 5;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= KEY_ITEM_AMOUNT);
		m_keyitemlist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnItemChangeName(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	if (it.SetName(m_itemname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_itemname->GetInsertionPoint();
		m_itemname->ChangeValue(_(it.name.str));
		m_itemname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_itemlist->SetString(sel,_(it.name.str));
		UpdateItemName(*sortid);
		MarkDataItemModified();
	}
}

void CDDataStruct::OnItemChangeHelp(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	if (it.SetHelp(m_itemhelp->GetValue().ToStdWstring())) {
		wxTextPos ip = m_itemhelp->GetInsertionPoint();
		m_itemhelp->ChangeValue(_(it.help.str));
		m_itemhelp->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataItemModified();
	}
}

void CDDataStruct::OnItemChangeBattleHelp(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	if (it.SetBattleHelp(m_itemhelp2->GetValue().ToStdWstring())) {
		wxTextPos ip = m_itemhelp2->GetInsertionPoint();
		m_itemhelp2->ChangeValue(_(it.battle_help.str));
		m_itemhelp2->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataItemModified();
	}
}

void CDDataStruct::OnKeyItemChangeName(wxCommandEvent& event) {
	int sel = m_keyitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_keyitemlist->GetClientData(sel);
	KeyItemDataStruct& ki = itemset.key_item[*sortid];
	if (ki.SetName(m_keyitemname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_keyitemname->GetInsertionPoint();
		m_keyitemname->ChangeValue(_(ki.name.str));
		m_keyitemname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_keyitemlist->SetString(sel,_(ki.name.str));
		MarkDataItemModified();
	}
}

void CDDataStruct::OnKeyItemChangeHelp(wxCommandEvent& event) {
	int sel = m_keyitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_keyitemlist->GetClientData(sel);
	KeyItemDataStruct& ki = itemset.key_item[*sortid];
	if (ki.SetHelp(m_keyitemhelp->GetValue().ToStdWstring())) {
		wxTextPos ip = m_keyitemhelp->GetInsertionPoint();
		m_keyitemhelp->ChangeValue(_(ki.help.str));
		m_keyitemhelp->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataItemModified();
	}
}

void CDDataStruct::OnKeyItemChangeDescription(wxCommandEvent& event) {
	int sel = m_keyitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_keyitemlist->GetClientData(sel);
	KeyItemDataStruct& ki = itemset.key_item[*sortid];
	if (ki.SetDescription(m_keyitemdescription->GetValue().ToStdWstring())) {
		wxTextPos ip = m_keyitemdescription->GetInsertionPoint();
		m_keyitemdescription->ChangeValue(_(ki.description.str));
		m_keyitemdescription->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		MarkDataItemModified();
	}
}

void CDDataStruct::OnItemChangeModelName(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
	iw.model_name = m_itemweaponmodelname->GetValue().ToStdWstring();
	iw.UpdateModelId();
	m_itemweaponmodel->SetValue(iw.model);
	MarkDataItemModified();
}

void CDDataStruct::OnItemChangeTextureName(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
	if (m_itemweapontexture->GetValue().IsEmpty()) {
		iw.texture_names.clear();
	} else {
		iw.texture_names.resize(1);
		iw.texture_names[0] = m_itemweapontexture->GetValue().ToStdWstring();
	}
	MarkDataItemModified();
}

void CDDataStruct::OnItemChangeSpin(wxSpinEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	int id = event.GetId();
	if (id == wxID_ID) {
		it.id = event.GetPosition();
	} else if (id == wxID_PRICE) {
		it.price = event.GetPosition();
		it.sell_price = it.price / 2;
		m_itemsellprice->SetValue(it.sell_price);
	} else if (id == wxID_SELLPRICE) {
		it.sell_price = event.GetPosition();
	} else if (id == wxID_ICON) {
		it.icon = event.GetPosition();
		DisplayItemIcon();
	} else if (id == wxID_ICONCOLOR) {
		it.icon_color = event.GetPosition();
		DisplayItemIcon();
	} else if (id == wxID_ZERO) {
		it.zero = event.GetPosition();
	} else if (id == wxID_EFFECT) {
		ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
		ic.effect = event.GetPosition();
		if (ic.effect < HADES_STRING_SPELL_EFFECT_COUNT)
			m_itemusableeffect->SetSelection(ic.effect);
		if (ic.effect < (int)HADES_STRING_SPELL_EFFECT.size())
			m_itemusableeffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[ic.effect].help);
		else
			m_itemusableeffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
		int x, y;
		m_itemusableeffecthelp->GetSize(&x, &y);
		m_itemusableeffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
	} else if (id == wxID_POWER) {
		itemset.GetUsableById(it.usable_id).power = event.GetPosition();
	} else if (id == wxID_STATUSINT) {
		int status = event.GetPosition();
		m_itemusablestatusbaselist->SetSelection(status < STATUS_AMOUNT ? status : -1);
	} else if (id == wxID_ACCURACY) {
		itemset.GetUsableById(it.usable_id).accuracy = event.GetPosition();
	} else if (id == wxID_WEAPONFORMULA) {
		ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
		iw.damage_formula = event.GetPosition();
		if (iw.damage_formula < (int)m_itemweaponformula->GetCount())
			m_itemweaponformula->SetSelection(iw.damage_formula);
	} else if (id == wxID_WEAPONPOWER) {
		itemset.GetWeaponById(it.weapon_id).power = event.GetPosition();
	} else if (id == wxID_WEAPONACC) {
		itemset.GetWeaponById(it.weapon_id).status_accuracy = event.GetPosition();
	} else if (id == wxID_WEAPONSTATUS) {
		itemset.GetWeaponById(it.weapon_id).status = event.GetSelection();
		if (event.GetSelection() < STATUS_SET_AMOUNT)
			m_itemweaponstatus->SetSelection(event.GetSelection());
	} else if (id == wxID_WEAPONUNK1) {
		itemset.GetWeaponById(it.weapon_id).offset1 = event.GetPosition();
	} else if (id == wxID_WEAPONUNK2) {
		itemset.GetWeaponById(it.weapon_id).offset2 = event.GetPosition();
	} else if (id == wxID_WEAPONMODEL) {
		ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
		iw.model = event.GetPosition();
		iw.UpdateModelName();
		m_itemweaponmodelname->ChangeValue(iw.model_name);
	} else if (id == wxID_DEFENCE) {
		itemset.GetArmorById(it.armor_id).defence = event.GetPosition();
	} else if (id == wxID_EVADE) {
		itemset.GetArmorById(it.armor_id).evade = event.GetPosition();
	} else if (id == wxID_MDEFENCE) {
		itemset.GetArmorById(it.armor_id).magic_defence = event.GetPosition();
	} else if (id == wxID_MEVADE) {
		itemset.GetArmorById(it.armor_id).magic_evade = event.GetPosition();
	}
	MarkDataItemModified();
}

void CDDataStruct::OnItemChangeChoice(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	int id = event.GetId();
	if (id == wxID_STAT) {
		if (event.GetSelection() < ITEM_STAT_AMOUNT)
			it.stat_id = itemset.stat[event.GetSelection()].id;
		else
			it.stat_id = it.id;
		DisplayItemStatIdHelp();
	} else if (id == wxID_MENUPOSREL) {
		it.menu_position_type = event.GetSelection() != 0 ? ITEM_POSITION_AFTER : ITEM_POSITION_BEFORE;
	} else if (id == wxID_MENUPOS) {
		it.menu_position = itemset.item[event.GetSelection()].id;
	} else if (id == wxID_EQUIPPOSREL) {
		it.equip_position_type = ITEM_POSITION_BEFORE + event.GetSelection();
	} else if (id == wxID_EQUIPPOS) {
		it.equip_position = itemset.item[event.GetSelection()].id;
	} else if (id == wxID_EFFECT) {
		itemset.GetUsableById(it.usable_id).effect = event.GetSelection();
		m_itemusableeffectint->SetValue(event.GetSelection());
		SortedChoiceItemWithHelp* item = (SortedChoiceItemWithHelp*)event.GetClientData();
		if (item != NULL)
			m_itemusableeffecthelp->SetLabel(item->help);
		else
			m_itemusableeffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
		int x, y;
		m_itemusableeffecthelp->GetSize(&x, &y);
		m_itemusableeffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
	} else if (id == wxID_TARGETTYPE) {
		ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
		Spell_Target_Type tt = event.GetSelection();
		ic.SetTargetType(tt);
		m_itemusabletargetamount->SetSelection(ic.GetTargetAmount());
		m_itemusabletargetpriority->SetSelection(ic.GetTargetPriority());
		m_itemusabletargetamount->Enable(tt != SPELL_TARGET_TYPE_EVERYONE && tt != SPELL_TARGET_TYPE_SELF && tt != SPELL_TARGET_TYPE_IRRELEVANT);
		m_itemusabletargetpriority->Enable(tt == SPELL_TARGET_TYPE_ANY);
	} else if (id == wxID_TARGETAMOUNT) {
		itemset.GetUsableById(it.usable_id).SetTargetAmount(event.GetSelection());
	} else if (id == wxID_TARGETPRIORITY) {
		itemset.GetUsableById(it.usable_id).SetTargetPriority(event.GetSelection());
	} else if (id == wxID_TARGETPANEL) {
		itemset.GetUsableById(it.usable_id).SetPanel(event.GetSelection());
	} else if (id == wxID_WEAPONFORMULA) {
		itemset.GetWeaponById(it.weapon_id).damage_formula = event.GetSelection();
		m_itemweaponformulaint->SetValue(event.GetSelection());
	} else if (id == wxID_WEAPONSTATUS) {
		itemset.GetWeaponById(it.weapon_id).status = event.GetSelection();
		m_itemweaponstatusint->SetValue(event.GetSelection());
	} else if (id == wxID_WEAPONSFX) {
		itemset.GetWeaponById(it.weapon_id).hit_sfx = event.GetSelection();
	}
	MarkDataItemModified();
}

void CDDataStruct::OnItemChangeFlags(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	int id = event.GetId();
	bool on = event.GetInt();
	if (id == wxID_HASUSABLE || id == wxID_HASWEAPON || id == wxID_HASARMOR) {
		unsigned int subindex;
		int subid = it.id;
		bool alreadyexists = false;
		if (id == wxID_HASARMOR && it.id < ITEM_AMOUNT)
			subid += it.id >= ITEM_WEAPON_AMOUNT ? -ITEM_WEAPON_AMOUNT : ITEM_ARMOR_AMOUNT + ITEM_USABLE_AMOUNT;
		else if (id == wxID_HASUSABLE && it.id < ITEM_AMOUNT)
			subid += it.id >= ITEM_WEAPON_AMOUNT + ITEM_ARMOR_AMOUNT ? -(ITEM_WEAPON_AMOUNT + ITEM_ARMOR_AMOUNT) : ITEM_USABLE_AMOUNT;
		if (id == wxID_HASUSABLE) {
			if (on) {
				for (subindex = 0; subindex < itemset.usable.size(); subindex++)
					if (itemset.usable[subindex].id == subid) {
						alreadyexists = true;
						break;
					}
				if (!alreadyexists) {
					ItemUsableDataStruct newstruct;
					newstruct.InitializeDefault(subid);
					subindex = InsertAtId<ItemUsableDataStruct>(itemset.usable, newstruct, subid);
				}
				it.usable_id = subid;
			} else {
				it.usable_id = -1;
			}
		} else if (id == wxID_HASWEAPON) {
			if (on) {
				for (subindex = 0; subindex < itemset.weapon.size(); subindex++)
					if (itemset.weapon[subindex].id == subid) {
						alreadyexists = true;
						break;
					}
				if (!alreadyexists) {
					ItemWeaponDataStruct newstruct;
					newstruct.InitializeDefault(subid);
					subindex = InsertAtId<ItemWeaponDataStruct>(itemset.weapon, newstruct, subid);
				}
				it.weapon_id = subid;
			} else {
				it.weapon_id = -1;
			}
		} else if (id == wxID_HASARMOR) {
			if (on) {
				for (subindex = 0; subindex < itemset.armor.size(); subindex++)
					if (itemset.armor[subindex].id == subid) {
						alreadyexists = true;
						break;
					}
				if (!alreadyexists) {
					ItemArmorDataStruct newstruct;
					newstruct.InitializeDefault(subid);
					subindex = InsertAtId<ItemArmorDataStruct>(itemset.armor, newstruct, subid);
				}
				it.armor_id = subid;
			} else {
				it.armor_id = -1;
			}
		}
		DisplayItem(sel);
	} else {
		MACRO_FLAG_SET8(it.type, wxID_IT)
		MACRO_FLAG_SET16(it.char_availability, wxID_IA)
		if (it.usable_id >= 0) {
			ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
			MACRO_FLAG_SET8(ic.element, wxID_UE)
			MACRO_FLAG_SET(ic.target_flag, wxID_CAN_TARGET_DEAD, TARGET_FLAG_CAN_TARGET_DEAD)
			MACRO_FLAG_SET(ic.target_flag, wxID_TARGET_CAMERA, TARGET_FLAG_CAMERA)
			MACRO_FLAG_SET(ic.target_flag, wxID_TARGET_DEAD_FIRST, TARGET_FLAG_TARGET_DEAD_FIRST)
		}
		if (it.weapon_id >= 0) {
			ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
			MACRO_FLAG_SET8(iw.element, wxID_WE)
			MACRO_FLAG_SET8(iw.flag, wxID_WF)
		}
		if (id == wxID_IA1 || id == wxID_IA2 || id == wxID_IA3 || id == wxID_IA4 || id == wxID_IA5 || id == wxID_IA6 || id == wxID_IA7 || id == wxID_IA8 || id == wxID_IA9 || id == wxID_IA10 || id == wxID_IA11 || id == wxID_IA12)
			if (saveset.sectionloaded[DATA_SECTION_STAT] && m_statlist->GetSelection() != wxNOT_FOUND)
				DisplayStat(m_statlist->GetSelection());
	}
	MarkDataItemModified();
}

void CDDataStruct::OnItemChangeButton(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id == wxID_KEYNAME || id == wxID_KEYHELP || id == wxID_KEYDESCRIPTION) {
		int sel = m_keyitemlist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_keyitemlist->GetClientData(sel);
		KeyItemDataStruct& ki = itemset.key_item[*sortid];
		if (id == wxID_KEYNAME) {
			if (gametype == GAME_TYPE_PSX) {
				TextEditDialog ted(this, ki.name, itemset.key_name_space_total - itemset.key_name_space_used);
				if (ted.ShowModal() == wxID_OK) {
					ki.SetName(ted.text);
					m_keyitemname->SetValue(ki.name.str);
				}
			} else {
				TextSteamEditDialog ted(this, &ffuiset, ki.name);
				if (ted.ShowModal() == wxID_OK) {
					ki.SetName(ted.text);
					m_keyitemname->SetValue(ki.name.str);
				}
			}
		} else if (id == wxID_KEYHELP) {
			if (gametype == GAME_TYPE_PSX) {
				CharmapDataStruct* chmap = NULL;
				if (saveset.sectionloaded[DATA_SECTION_TEXT])
					chmap = textset.charmap[textset.main_charmap_index];
				TextEditDialog ted(this, ki.help, itemset.key_help_space_total - itemset.key_help_space_used, TEXT_STYLE_HELP, chmap);
				if (ted.ShowModal() == wxID_OK) {
					ki.SetHelp(ted.text);
					m_keyitemhelp->SetValue(ki.help.str);
				}
			} else {
				TextSteamEditDialog ted(this, &ffuiset, ki.help, TEXT_STYLE_HELP);
				if (ted.ShowModal() == wxID_OK) {
					ki.SetHelp(ted.text);
					m_keyitemhelp->SetValue(ki.help.str);
				}
			}
		} else if (id == wxID_KEYDESCRIPTION) {
			if (gametype == GAME_TYPE_PSX) {
				CharmapDataStruct* chmap = NULL;
				if (saveset.sectionloaded[DATA_SECTION_TEXT])
					chmap = textset.charmap[textset.main_charmap_index];
				TextEditDialog ted(this, ki.description, itemset.key_desc_space_total - itemset.key_desc_space_used, TEXT_STYLE_DESCRIPTION, chmap);
				if (ted.ShowModal() == wxID_OK) {
					ki.SetDescription(ted.text);
					m_keyitemdescription->SetValue(ki.description.str);
				}
			} else {
				TextSteamEditDialog ted(this, &ffuiset, ki.description, TEXT_STYLE_DESCRIPTION);
				if (ted.ShowModal() == wxID_OK) {
					ki.SetDescription(ted.text);
					m_keyitemdescription->SetValue(ki.description.str);
				}
			}
		}
	} else {
		int sel = m_itemlist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
		ItemDataStruct& it = itemset.item[*sortid];
		if (id == wxID_NAME) {
			if (gametype == GAME_TYPE_PSX) {
				TextEditDialog ted(this, it.name, itemset.name_space_total - itemset.name_space_used);
				if (ted.ShowModal() == wxID_OK) {
					it.SetName(ted.text);
					m_itemname->SetValue(it.name.str);
				}
			} else {
				TextSteamEditDialog ted(this, &ffuiset, it.name);
				if (ted.ShowModal() == wxID_OK) {
					it.SetName(ted.text);
					m_itemname->SetValue(it.name.str);
				}
			}
		} else if (id == wxID_HELP) {
			if (gametype == GAME_TYPE_PSX) {
				CharmapDataStruct* chmap = NULL;
				if (saveset.sectionloaded[DATA_SECTION_TEXT])
					chmap = textset.charmap[textset.main_charmap_index];
				TextEditDialog ted(this, it.help, itemset.help_space_total - itemset.help_space_used, TEXT_STYLE_HELP, chmap);
				if (ted.ShowModal() == wxID_OK) {
					it.SetHelp(ted.text);
					m_itemhelp->SetValue(it.help.str);
				}
			} else {
				TextSteamEditDialog ted(this, &ffuiset, it.help, TEXT_STYLE_HELP);
				if (ted.ShowModal() == wxID_OK) {
					it.SetHelp(ted.text);
					m_itemhelp->SetValue(it.help.str);
				}
			}
		} else if (id == wxID_BATTLEHELP) {
			if (gametype == GAME_TYPE_PSX) {
				CharmapDataStruct* chmap = NULL;
				if (saveset.sectionloaded[DATA_SECTION_TEXT])
					chmap = textset.charmap[textset.main_charmap_index];
				TextEditDialog ted(this, it.battle_help, itemset.help2_space_total - itemset.help2_space_used, TEXT_STYLE_HELP, chmap);
				if (ted.ShowModal() == wxID_OK) {
					it.SetBattleHelp(ted.text);
					m_itemhelp2->SetValue(it.battle_help.str);
				}
			} else {
				TextSteamEditDialog ted(this, &ffuiset, it.battle_help, TEXT_STYLE_HELP);
				if (ted.ShowModal() == wxID_OK) {
					it.SetBattleHelp(ted.text);
					m_itemhelp2->SetValue(it.battle_help.str);
				}
			}
		} else if (id == wxID_STAT) {
			ItemStatDataStruct& is = GetItemStat(sel);
			ItemStatDialog statdialog(this);
			if (statdialog.ShowModal(is) == wxID_OK) {
				is.speed = statdialog.m_speed->GetValue();
				is.strength = statdialog.m_strength->GetValue();
				is.magic = statdialog.m_magic->GetValue();
				is.spirit = statdialog.m_spirit->GetValue();
				is.element_absorb = (statdialog.m_elementabsorb1->IsChecked() ? 1 : 0) | (statdialog.m_elementabsorb2->IsChecked() << 1) | (statdialog.m_elementabsorb3->IsChecked() << 2) | (statdialog.m_elementabsorb4->IsChecked() << 3) | /*
								*/(statdialog.m_elementabsorb5->IsChecked() << 4) | (statdialog.m_elementabsorb6->IsChecked() << 5) | (statdialog.m_elementabsorb7->IsChecked() << 6) | (statdialog.m_elementabsorb8->IsChecked() << 7);
				is.element_immune = (statdialog.m_elementimmune1->IsChecked() ? 1 : 0) | (statdialog.m_elementimmune2->IsChecked() << 1) | (statdialog.m_elementimmune3->IsChecked() << 2) | (statdialog.m_elementimmune4->IsChecked() << 3) | /*
								*/(statdialog.m_elementimmune5->IsChecked() << 4) | (statdialog.m_elementimmune6->IsChecked() << 5) | (statdialog.m_elementimmune7->IsChecked() << 6) | (statdialog.m_elementimmune8->IsChecked() << 7);
				is.element_half = (statdialog.m_elementhalf1->IsChecked() ? 1 : 0) | (statdialog.m_elementhalf2->IsChecked() << 1) | (statdialog.m_elementhalf3->IsChecked() << 2) | (statdialog.m_elementhalf4->IsChecked() << 3) | /*
								*/(statdialog.m_elementhalf5->IsChecked() << 4) | (statdialog.m_elementhalf6->IsChecked() << 5) | (statdialog.m_elementhalf7->IsChecked() << 6) | (statdialog.m_elementhalf8->IsChecked() << 7);
				is.element_weak = (statdialog.m_elementweak1->IsChecked() ? 1 : 0) | (statdialog.m_elementweak2->IsChecked() << 1) | (statdialog.m_elementweak3->IsChecked() << 2) | (statdialog.m_elementweak4->IsChecked() << 3) | /*
								*/(statdialog.m_elementweak5->IsChecked() << 4) | (statdialog.m_elementweak6->IsChecked() << 5) | (statdialog.m_elementweak7->IsChecked() << 6) | (statdialog.m_elementweak8->IsChecked() << 7);
				is.element_boost = (statdialog.m_elementboost1->IsChecked() ? 1 : 0) | (statdialog.m_elementboost2->IsChecked() << 1) | (statdialog.m_elementboost3->IsChecked() << 2) | (statdialog.m_elementboost4->IsChecked() << 3) | /*
								*/(statdialog.m_elementboost5->IsChecked() << 4) | (statdialog.m_elementboost6->IsChecked() << 5) | (statdialog.m_elementboost7->IsChecked() << 6) | (statdialog.m_elementboost8->IsChecked() << 7);
				DisplayItemStatIdHelp();
				MarkDataItemModified();
			}
		} else if (id == wxID_SKILLADD) {
			int fulllistsel = m_itemskillfulllist->GetSelection();
			if (fulllistsel != wxNOT_FOUND && (m_itemskilllist->GetCount() < 3 || (gametype == GAME_TYPE_STEAM && config.dll_usage != 0))) {
				int listsel = m_itemskilllist->GetSelection();
				if (listsel == wxNOT_FOUND)
					for (listsel = 0; listsel < (int)it.skill.size() && !it.skill[listsel].IsVoid(); listsel++) {}
				if (fulllistsel < (int)spellset.spell.size()) {
					SpellDataStruct& sp = spellset.spell[fulllistsel];
					if (sp.id == 0)
						return;
					AnyAbilityStruct newabil;
					newabil.Setup(sp.id, true);
					it.skill.insert(it.skill.begin() + listsel, newabil);
					m_itemskilllist->Insert(wxString::Format(wxT("%s (A %d)"), sp.name.GetStr(hades::TEXT_PREVIEW_TYPE), sp.id), listsel);
				} else {
					SupportDataStruct& sp = supportset.support[fulllistsel - spellset.spell.size()];
					AnyAbilityStruct newabil;
					newabil.Setup(sp.id, false);
					it.skill.insert(it.skill.begin() + listsel, newabil);
					m_itemskilllist->Insert(wxString::Format(wxT("%s (S %d)"), sp.name.GetStr(hades::TEXT_PREVIEW_TYPE), sp.id), listsel);
				}
				if (listsel + 1 < (int)m_itemskilllist->GetCount())
					m_itemskilllist->SetSelection(listsel);
				if (event.GetEventType() == wxEVT_BUTTON && fulllistsel + 1 < (int)m_itemskillfulllist->GetCount())
					m_itemskillfulllist->SetSelection(fulllistsel + 1);
				m_itemscrolledwindow->Layout();
				m_itemscrolledwindow->GetParent()->GetSizer()->Layout();
				m_itemscrolledwindow->Refresh();
				MarkDataItemModified();
			}
		} else if (id == wxID_SKILLREMOVE) {
			int listsel = m_itemskilllist->GetSelection();
			if (listsel != wxNOT_FOUND) {
				m_itemskilllist->Delete(listsel);
				it.skill.erase(it.skill.begin() + listsel);
				if (it.skill.size() < 3) {
					AnyAbilityStruct nullabil;
					nullabil.Setup(0);
					it.skill.push_back(nullabil);
				}
				if (m_itemskilllist->GetCount() > 0)
					m_itemskilllist->SetSelection(listsel < (int)m_itemskilllist->GetCount() ? listsel : m_itemskilllist->GetCount() - 1);
				MarkDataItemModified();
			}
		} else if (id == wxID_SKILLUP) {
			int listsel = m_itemskilllist->GetSelection();
			if (listsel != wxNOT_FOUND && listsel != 0) {
				AnyAbilityStruct tmp = it.skill[listsel];
				it.skill[listsel] = it.skill[listsel - 1];
				it.skill[listsel - 1] = tmp;
				wxString tmpstr = m_itemskilllist->GetString(listsel);
				m_itemskilllist->SetString(listsel, m_itemskilllist->GetString(listsel - 1));
				m_itemskilllist->SetString(listsel - 1, tmpstr);
				m_itemskilllist->SetSelection(listsel - 1);
				MarkDataItemModified();
			}
		} else if (id == wxID_SKILLDOWN) {
			int listsel = m_itemskilllist->GetSelection();
			if (listsel != wxNOT_FOUND && listsel < (int)m_itemskilllist->GetCount() - 1) {
				AnyAbilityStruct tmp = it.skill[listsel];
				it.skill[listsel] = it.skill[listsel + 1];
				it.skill[listsel + 1] = tmp;
				wxString tmpstr = m_itemskilllist->GetString(listsel);
				m_itemskilllist->SetString(listsel, m_itemskilllist->GetString(listsel + 1));
				m_itemskilllist->SetString(listsel + 1, tmpstr);
				m_itemskilllist->SetSelection(listsel + 1);
				MarkDataItemModified();
			}
		} else if (id == wxID_MENUPOSUP) {
			if (it.menu_position > 0) {
				it.menu_position--;
				m_itemmenuposlist->SetString(it.menu_position, _(it.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
				for (i = 0; i < ITEM_AMOUNT - 1; i++)
					if (itemset.item[i].menu_position == it.menu_position && i != *sortid) {
						itemset.item[i].menu_position++;
						m_itemmenuposlist->SetString(itemset.item[i].menu_position, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						break;
					}
				m_itemmenuposlist->SetSelection(it.menu_position);
				MarkDataItemModified();
			}
		} else if (id == wxID_MENUPOSDOWN) {
			if (it.menu_position < ITEM_AMOUNT - 2) {
				it.menu_position++;
				m_itemmenuposlist->SetString(it.menu_position, _(it.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
				for (i = 0; i < ITEM_AMOUNT - 1; i++)
					if (itemset.item[i].menu_position == it.menu_position && i != *sortid) {
						itemset.item[i].menu_position--;
						m_itemmenuposlist->SetString(itemset.item[i].menu_position, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						break;
					}
				m_itemmenuposlist->SetSelection(it.menu_position);
				MarkDataItemModified();
			}
		} else if (id == wxID_EQUIPPOSUP) {
			if (it.equip_position > 0) {
				bool equipused1 = false;
				bool equipused2 = false;
				int epos1 = it.equip_position--;
				int epos2 = it.equip_position;
				for (i = 0; i < ITEM_AMOUNT - 1; i++) {
					if (itemset.item[i].equip_position == epos1) {
						if (equipused1)
							m_itemequipposlist->SetString(epos1, m_itemequipposlist->GetString(epos1) + _(L" - ") + _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						else
							m_itemequipposlist->SetString(epos1, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						equipused1 = true;
					} else if (itemset.item[i].equip_position == epos2) {
						if (equipused2)
							m_itemequipposlist->SetString(epos2, m_itemequipposlist->GetString(epos2) + _(L" - ") + _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						else
							m_itemequipposlist->SetString(epos2, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						equipused2 = true;
					}
				}
				if (!equipused1)
					m_itemequipposlist->SetString(epos1, _(L"_"));
				m_itemequipposlist->SetSelection(it.equip_position);
				MarkDataItemModified();
			}
		} else if (id == wxID_EQUIPPOSDOWN) {
			if (it.equip_position < ITEM_AMOUNT - 2) {
				bool equipused1 = false;
				bool equipused2 = false;
				int epos1 = it.equip_position++;
				int epos2 = it.equip_position;
				for (i = 0; i < ITEM_AMOUNT - 1; i++) {
					if (itemset.item[i].equip_position == epos1) {
						if (equipused1)
							m_itemequipposlist->SetString(epos1, m_itemequipposlist->GetString(epos1) + _(L" - ") + _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						else
							m_itemequipposlist->SetString(epos1, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						equipused1 = true;
					} else if (itemset.item[i].equip_position == epos2) {
						if (equipused2)
							m_itemequipposlist->SetString(epos2, m_itemequipposlist->GetString(epos2) + _(L" - ") + _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						else
							m_itemequipposlist->SetString(epos2, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
						equipused2 = true;
					}
				}
				if (!equipused1)
					m_itemequipposlist->SetString(epos1, _(L"_"));
				m_itemequipposlist->SetSelection(it.equip_position);
				MarkDataItemModified();
			}
		} else if (id == wxID_ADDSTATUS) {
			ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
			int status = m_itemusablestatusbaseint->GetValue();
			if (ic.status.find(status) != ic.status.end())
				return;
			ic.status.insert(status);
			m_itemusablestatuslist->Append(SpellStatusDialog::StatusDisplayName(status), new int[1]{ status });
			MarkDataItemModified();
		} else if (id == wxID_REMOVESTATUS) {
			ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
			if (m_itemusablestatuslist->GetSelection() == wxNOT_FOUND)
				return;
			int status = *(int*)m_itemusablestatuslist->GetClientData(m_itemusablestatuslist->GetSelection());
			if (ic.status.erase(status) == 0)
				return;
			m_itemusablestatuslist->Delete(m_itemusablestatuslist->GetSelection());
			MarkDataItemModified();
		} else if (id == wxID_WEAPONSTATUS) {
			SpellStatusDialog spstat(this);
			ItemWeaponDataStruct& iw = itemset.GetWeaponById(it.weapon_id);
			if (spellset.GetStatusSetIndexById(iw.status) < 0)
				InsertAtId(spellset.status_set, spellset.status_set[0], iw.status);
			StatusSetStruct& statusset = spellset.GetStatusSetById(iw.status);
			if (spstat.ShowModal(this, statusset) == wxID_OK) {
				statusset.status_list = spstat.status_list;
				UpdateSpellStatusName(statusset);
				MarkDataItemModified();
			}
		} else if (id == wxID_CFGROUP || id == wxID_CFENTRY) {
			wxString* header;
			wxString* format;
			int kind = max(0, m_itemcustomkind->GetSelection());
			if (kind == 0) {
				header = &itemset.csv_header;
				format = &itemset.csv_format;
			} else if (kind == 1) {
				header = &itemset.csv_header_stat;
				format = &itemset.csv_format_stat;
			} else if (kind == 2) {
				header = &itemset.csv_header_weapon;
				format = &itemset.csv_format_weapon;
			} else if (kind == 3) {
				header = &itemset.csv_header_armor;
				format = &itemset.csv_format_armor;
			} else if (kind == 4) {
				header = &itemset.csv_header_usable;
				format = &itemset.csv_format_usable;
			} else {
				return;
			}
			if (id == wxID_CFGROUP) {
				DatabaseFormatDialog dial(this, header, format, &itemset.custom_field);
				if (dial.ShowModal() == wxID_OK) {
					for (unsigned int i = 0; i < itemset.item.size(); i++)
						UpdateCustomFieldMap(itemset.item[i].custom_field, itemset.custom_field);
					MarkDataItemModified();
				}
			} else {
				DatabaseFieldDialog dial(this, &it.custom_field, itemset.custom_field);
				if (dial.ShowModal() == wxID_OK)
					MarkDataItemModified();
			}
		}
	}
}

void CDDataStruct::OnItemChangeListBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADDSTATUS) {
		m_itemusablestatusbaseint->SetValue(m_itemusablestatusbaselist->GetSelection());
	}
}

void CDDataStruct::OnKeyItemChangeSpin(wxSpinEvent& event) {
	int sel = m_keyitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_keyitemlist->GetClientData(sel);
	KeyItemDataStruct& ki = itemset.key_item[*sortid];
	int id = event.GetId();
	if (id == wxID_ID) {
		ki.id = event.GetPosition();
	}
	MarkDataItemModified();
}

void CDDataStruct::OnButtonClickItemModel(wxCommandEvent& event) {
	int sel = m_itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_itemlist->GetClientData(sel);
	ItemDataStruct& it = itemset.item[*sortid];
	ItemUsableDataStruct& ic = itemset.GetUsableById(it.usable_id);
	TheSpellModelWindow->m_modelint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	if (TheSpellModelWindow->ShowModal(ic.GetTargetType(), ic.GetTargetAmount(), ic.model, ic.model, false) == wxID_OK) {
		SortedChoiceItemModel* item;
		wxChoice* choice = TheSpellModelWindow->m_enableall->IsChecked() ? TheSpellModelWindow->m_modelall : TheSpellModelWindow->m_modelmain;
		item = (SortedChoiceItemModel*)choice->GetClientData(choice->GetSelection());
		if (TheSpellModelWindow->m_enableall->IsChecked() && item->id != TheSpellModelWindow->m_modelint->GetValue()) {
			ic.model = TheSpellModelWindow->m_modelint->GetValue();
			m_itemusablemodel->SetLabel(wxString::Format(wxT("SFX %d"), ic.model));
		} else {
			ic.model = item->id;
			m_itemusablemodel->SetLabel(item->label);
		}
		MarkDataItemModified();
	}
}

void CDDataStruct::OnItemPositionListClick(wxMouseEvent& event) {
	wxWindow* obj = dynamic_cast<wxWindow*>(event.GetEventObject());
	obj->SetFocus();
	event.Skip(false);
}

//=============================//
//           Shops             //
//=============================//

void CDDataStruct::DisplayShop(int shopid) {
	ShopDataStruct& sh = shopset.shop[shopid];
	int i;
	m_shopid->Enable(false);
	m_shopid->SetValue(sh.id);
	m_shopcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_shopcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_shopitemlist->Clear();
	for (i = 0; i < sh.item_amount; i++)
		m_shopitemlist->Append(_(itemset.GetItemById(sh.item_list[i]).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_shopscrolledwindow->Layout();
	m_shopscrolledwindow->GetParent()->GetSizer()->Layout();
	m_shopscrolledwindow->Refresh();
}

void CDDataStruct::DisplaySynthesisShop(int synthshopid) {
	SynthesisDataStruct& sy = shopset.synthesis[synthshopid];
	unsigned int i;
	m_synthshopid->Enable(false);
	m_synthshopcustomformat->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_synthshopcustomfields->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_synthshoprecipeflex->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_synthshopprice->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_synthshopid->SetValue(sy.id);
	m_synthshopsynthitem->SetSelection(itemset.GetItemIndexById(sy.synthesized));
	m_synthshoprecipelist->Clear();
	for (i = 0; i < sy.recipe.size(); i++)
		m_synthshoprecipelist->Append(_(itemset.GetItemById(sy.recipe[i]).name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_synthshopprice->SetValue(sy.price);
	if (!m_synthshoprecipeflex->IsEnabled())
		m_synthshoprecipeflex->SetValue(false);
	if (m_synthshoprecipeflex->IsChecked()) {
		m_synthshoprecipeadd->SetLabel(HADES_STRING_SYNTH_ADD_ITEM_ADD);
		m_synthshopreciperemove->Show(true);
	} else {
		m_synthshoprecipeadd->SetLabel(HADES_STRING_SYNTH_ADD_ITEM_EDIT);
		m_synthshopreciperemove->Show(false);
	}
	if (gametype == GAME_TYPE_STEAM && config.dll_usage != 0)
		m_synthshopshopint->SetRange(0, INT32_MAX);
	else
		m_synthshopshopint->SetRange(32, 39);
	m_synthshopshoplist->Clear();
	for (i = 0; i < sy.shops.size(); i++) {
		if (sy.shops[i] >= 0 && sy.shops[i] < 40) {
			if (sy.shops[i] < 32 || (gametype == GAME_TYPE_STEAM && config.dll_usage != 0))
				m_synthshopshoplist->Append(HADES_STRING_SHOP_NAME[sy.shops[i]].label);
			else
				m_synthshopshoplist->Append(HADES_STRING_SYNTH_NAME[sy.shops[i] - 32]);
		} else {
			m_synthshopshoplist->Append(wxString::Format(wxT(HADES_STRING_SHOP_CUSTOM_NAME), sy.shops[i]));
		}
	}
	m_synthshopscrolledwindow->Layout();
	m_synthshopscrolledwindow->GetParent()->GetSizer()->Layout();
	m_synthshopscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxShop(wxCommandEvent& event) {
	DisplayShop(m_shoplist->GetSelection());
}

void CDDataStruct::OnListBoxSynthesisShop(wxCommandEvent& event) {
	DisplaySynthesisShop(m_synthshoplist->GetSelection());
}

void CDDataStruct::OnShopRightClick(wxMouseEvent& event) {
	int newsel = m_shoplist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_shoplist->SetSelection(newsel);
		DisplayShop(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 6;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= SHOP_AMOUNT);
		m_shoplist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnSynthesisRightClick(wxMouseEvent& event) {
	int newsel = m_synthshoplist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_synthshoplist->SetSelection(newsel);
		DisplaySynthesisShop(newsel);
		SharedMenuSelection = newsel;
		SharedMenuType = 7;
		sharedmenuadd->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
		sharedmenuremove->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0 && newsel >= SYNTHESIS_AMOUNT);
		m_synthshoplist->PopupMenu(sharedmenu);
	}
}

void CDDataStruct::OnShopChangeSpin(wxSpinEvent& event) {
	int id = event.GetId();
	if (id == wxID_SHOPID) {
		ShopDataStruct& sh = shopset.shop[m_shoplist->GetSelection()];
		sh.id = event.GetPosition();
	} else if (id == wxID_SYNTHID) {
		SynthesisDataStruct& sy = shopset.synthesis[m_synthshoplist->GetSelection()];
		sy.id = event.GetPosition();
	} else if (id == wxID_PRICE) {
		SynthesisDataStruct& sy = shopset.synthesis[m_synthshoplist->GetSelection()];
		sy.price = m_synthshopprice->GetValue();
	} else if (id == wxID_SYNTHPICK) {
		int synthid = event.GetPosition();
		if (synthid >= 32 && synthid < 40)
			m_synthshopdefaultlist->SetSelection(synthid - 32);
		return;
	}
	MarkDataShopModified();
}

void CDDataStruct::OnShopChangeChoice(wxCommandEvent& event) {
	SynthesisDataStruct& sy = shopset.synthesis[m_synthshoplist->GetSelection()];
	int id = event.GetId();
	if (id == wxID_SYNTH) {
		ItemDataStruct& it = itemset.item[m_synthshopsynthitem->GetSelection()];
		sy.synthesized = it.id;
		m_synthshoplist->SetString(m_synthshoplist->GetSelection(), _(it.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	} else if (id == wxID_SYNTHPICK) {
		m_synthshopshopint->SetValue(32 + m_synthshopdefaultlist->GetSelection());
		return;
	}
	MarkDataShopModified();
}

bool DiscardSynthShopLimit = false;
void CDDataStruct::OnShopChangeFlags(wxCommandEvent& event) {
	int id = event.GetId();
	bool on = event.GetInt();
	if (id == wxID_FLEXRECIPE) {
		if (on) {
			m_synthshoprecipeadd->SetLabel(HADES_STRING_SYNTH_ADD_ITEM_ADD);
			m_synthshopreciperemove->Show(true);
		} else {
			m_synthshoprecipeadd->SetLabel(HADES_STRING_SYNTH_ADD_ITEM_EDIT);
			m_synthshopreciperemove->Show(false);
		}
		return;
	}
	MarkDataShopModified();
}

bool DiscardShopItemLimit = false;
void CDDataStruct::OnShopChangeButton(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (m_notebookshop->GetSelection() == 1) {
		SynthesisDataStruct& sy = shopset.synthesis[m_synthshoplist->GetSelection()];
		if (id == wxID_RECIPEADD) {
			int fulllistsel = m_synthshoprecipefulllist->GetSelection();
			if (fulllistsel != wxNOT_FOUND && itemset.item[fulllistsel].id != 0xFF) {
				int listsel = m_synthshoprecipelist->GetSelection();
				if (m_synthshoprecipeflex->IsChecked()) {
					if (listsel == wxNOT_FOUND)
						listsel = sy.recipe.size();
					sy.recipe.insert(sy.recipe.begin() + listsel, itemset.item[fulllistsel].id);
					m_synthshoprecipelist->Insert(_(itemset.item[fulllistsel].name.GetStr(hades::TEXT_PREVIEW_TYPE)), listsel);
					if (listsel + 1 < (int)m_synthshoprecipelist->GetCount())
						m_synthshoprecipelist->SetSelection(listsel);
				} else {
					if (sy.recipe.empty())
						return;
					if (listsel == wxNOT_FOUND)
						listsel = 0;
					sy.recipe[listsel] = itemset.item[fulllistsel].id;
					m_synthshoprecipelist->SetString(listsel, _(itemset.item[fulllistsel].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
				}
			} else {
				return;
			}
			m_synthshopscrolledwindow->Layout();
			m_synthshopscrolledwindow->GetParent()->GetSizer()->Layout();
			m_synthshopscrolledwindow->Refresh();
		} else if (id == wxID_RECIPEREMOVE) {
			if (!m_synthshoprecipeflex->IsChecked())
				return;
			int listsel = m_synthshoprecipelist->GetSelection();
			if (listsel != wxNOT_FOUND) {
				m_synthshoprecipelist->Delete(listsel);
				sy.recipe.erase(sy.recipe.begin() + listsel);
				if (m_synthshoprecipelist->GetCount() > 0)
					m_synthshoprecipelist->SetSelection(listsel < (int)m_synthshoprecipelist->GetCount() ? listsel : m_synthshoprecipelist->GetCount() - 1);
			} else {
				return;
			}
		} else if (id == wxID_RECIPEUP) {
			int listsel = m_synthshoprecipelist->GetSelection();
			if (listsel != wxNOT_FOUND && listsel != 0) {
				int tmp = sy.recipe[listsel];
				sy.recipe[listsel] = sy.recipe[listsel - 1];
				sy.recipe[listsel - 1] = tmp;
				wxString tmpstr = m_synthshoprecipelist->GetString(listsel);
				m_synthshoprecipelist->SetString(listsel, m_synthshoprecipelist->GetString(listsel - 1));
				m_synthshoprecipelist->SetString(listsel - 1, tmpstr);
				m_synthshoprecipelist->SetSelection(listsel - 1);
			} else {
				return;
			}
		} else if (id == wxID_RECIPEDOWN) {
			int listsel = m_synthshoprecipelist->GetSelection();
			if (listsel != wxNOT_FOUND && listsel < (int)m_synthshoprecipelist->GetCount() - 1) {
				int tmp = sy.recipe[listsel];
				sy.recipe[listsel] = sy.recipe[listsel + 1];
				sy.recipe[listsel + 1] = tmp;
				wxString tmpstr = m_synthshoprecipelist->GetString(listsel);
				m_synthshoprecipelist->SetString(listsel, m_synthshoprecipelist->GetString(listsel + 1));
				m_synthshoprecipelist->SetString(listsel + 1, tmpstr);
				m_synthshoprecipelist->SetSelection(listsel + 1);
			} else {
				return;
			}
		} else if (id == wxID_SYNTHADD) {
			int synthid = m_synthshopshopint->GetValue();
			for (i = 0; i < sy.shops.size(); i++)
				if (sy.shops[i] == synthid)
					return;
			int listsel = m_synthshopshoplist->GetSelection();
			if (listsel == wxNOT_FOUND)
				listsel = sy.shops.size();
			sy.shops.insert(sy.shops.begin() + listsel, synthid);
			if (synthid >= 0 && synthid < 40) {
				if (synthid < 32 || (gametype == GAME_TYPE_STEAM && config.dll_usage != 0))
					m_synthshopshoplist->Insert(HADES_STRING_SHOP_NAME[synthid].label, listsel);
				else
					m_synthshopshoplist->Insert(HADES_STRING_SYNTH_NAME[synthid - 32], listsel);
			} else {
				m_synthshopshoplist->Insert(wxString::Format(wxT(HADES_STRING_SHOP_CUSTOM_NAME), synthid), listsel);
			}
			if (listsel + 1 < (int)sy.shops.size())
				m_synthshopshoplist->SetSelection(listsel);
			if (!DiscardSynthShopLimit && (gametype != GAME_TYPE_STEAM || config.dll_usage == 0)) {
				bool toomanyitem = false;
				unsigned int itamount, j, k;
				wxString warning;
				warning.Printf(wxT(HADES_STRING_SYNTH_REACH_LIMIT), SYNTHESIS_MAX_ITEMS);
				for (i = 0; i < HADES_STRING_SYNTH_NAME.size(); i++) {
					itamount = 0;
					for (j = 0; j < shopset.synthesis.size(); j++)
						for (k = 0; k < shopset.synthesis[j].shops.size(); k++)
							if (shopset.synthesis[j].shops[k] == 32 + i)
								itamount++;
					if (itamount > SYNTHESIS_MAX_ITEMS) {
						toomanyitem = true;
						wxString warningtoken;
						warningtoken.Printf(wxT(HADES_STRING_SYNTH_REACH_LIMIT_TOKEN), HADES_STRING_SYNTH_NAME[i], itamount);
						warning += warningtoken;
					}
				}
				if (toomanyitem) {
					DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), warning);
					if (popup.ShowModal() == wxID_DISCARD)
						DiscardSynthShopLimit = true;
				}
			}
			m_synthshopscrolledwindow->Layout();
			m_synthshopscrolledwindow->GetParent()->GetSizer()->Layout();
			m_synthshopscrolledwindow->Refresh();
		} else if (id == wxID_SYNTHREMOVE) {
			int listsel = m_synthshopshoplist->GetSelection();
			if (listsel != wxNOT_FOUND) {
				m_synthshopshoplist->Delete(listsel);
				sy.shops.erase(sy.shops.begin() + listsel);
				if (m_synthshopshoplist->GetCount() > 0)
					m_synthshopshoplist->SetSelection(listsel < (int)m_synthshopshoplist->GetCount() ? listsel : m_synthshopshoplist->GetCount() - 1);
			} else {
				return;
			}
		} else if (id == wxID_SYNTHUP) {
			int listsel = m_synthshopshoplist->GetSelection();
			if (listsel != wxNOT_FOUND && listsel != 0) {
				int tmp = sy.shops[listsel];
				sy.shops[listsel] = sy.shops[listsel - 1];
				sy.shops[listsel - 1] = tmp;
				wxString tmpstr = m_synthshopshoplist->GetString(listsel);
				m_synthshopshoplist->SetString(listsel, m_synthshopshoplist->GetString(listsel - 1));
				m_synthshopshoplist->SetString(listsel - 1, tmpstr);
				m_synthshopshoplist->SetSelection(listsel - 1);
			} else {
				return;
			}
		} else if (id == wxID_SYNTHDOWN) {
			int listsel = m_synthshopshoplist->GetSelection();
			if (listsel != wxNOT_FOUND && listsel < (int)m_synthshopshoplist->GetCount() - 1) {
				int tmp = sy.shops[listsel];
				sy.shops[listsel] = sy.shops[listsel + 1];
				sy.shops[listsel + 1] = tmp;
				wxString tmpstr = m_synthshopshoplist->GetString(listsel);
				m_synthshopshoplist->SetString(listsel, m_synthshopshoplist->GetString(listsel + 1));
				m_synthshopshoplist->SetString(listsel + 1, tmpstr);
				m_synthshopshoplist->SetSelection(listsel + 1);
			} else {
				return;
			}
		} else if (id == wxID_CFGROUP) {
			DatabaseFormatDialog dial(this, &shopset.csv_header_synthesis, &shopset.csv_format_synthesis, &shopset.custom_field_synthesis);
			if (dial.ShowModal() == wxID_OK) {
				for (unsigned int i = 0; i < shopset.synthesis.size(); i++)
					UpdateCustomFieldMap(shopset.synthesis[i].custom_field, shopset.custom_field_synthesis);
				MarkDataShopModified();
			}
			return;
		} else if (id == wxID_CFENTRY) {
			DatabaseFieldDialog dial(this, &sy.custom_field, shopset.custom_field_synthesis);
			if (dial.ShowModal() == wxID_OK)
				MarkDataShopModified();
			return;
		}
	} else {
		ShopDataStruct& sh = shopset.shop[m_shoplist->GetSelection()];
		int fulllistsel = m_shopitemfulllist->GetSelection();
		int listsel = m_shopitemlist->GetSelection();
		if (id == wxID_ADD) {
			if (fulllistsel != wxNOT_FOUND) {
				if (sh.item_amount == SHOP_ITEM_AMOUNT && (gametype != GAME_TYPE_STEAM || config.dll_usage == 0)) {
					if (!DiscardShopItemLimit) {
						DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_SHOP_REACH_LIMIT);
						if (popup.ShowModal() == wxID_DISCARD)
							DiscardShopItemLimit = true;
					}
					return;
				} else {
					ItemDataStruct& it = itemset.item[fulllistsel];
					if (it.id == 0xFF)
						return;
					if (listsel == wxNOT_FOUND)
						listsel = sh.item_amount;
					sh.item_list.insert(sh.item_list.begin() + listsel, it.id);
					m_shopitemlist->Insert(_(it.name.GetStr(hades::TEXT_PREVIEW_TYPE)), listsel);
					sh.item_amount++;
					if (listsel + 1 < sh.item_amount)
						m_shopitemlist->SetSelection(listsel);
					if (event.GetEventType() == wxEVT_BUTTON && fulllistsel + 1 < (int)m_shopitemfulllist->GetCount())
						m_shopitemfulllist->SetSelection(fulllistsel + 1);
					if (sh.item_amount < SHOP_ITEM_AMOUNT)
						sh.item_list[sh.item_amount] = 0xFF;
				}
			} else {
				return;
			}
			m_shopscrolledwindow->Layout();
			m_shopscrolledwindow->GetParent()->GetSizer()->Layout();
			m_shopscrolledwindow->Refresh();
		} else if (id == wxID_REMOVE) {
			if (listsel != wxNOT_FOUND) {
				sh.item_list.erase(sh.item_list.begin() + listsel);
				if (sh.item_list.size() < SHOP_ITEM_AMOUNT)
					sh.item_list.resize(SHOP_ITEM_AMOUNT);
				sh.item_amount--;
				m_shopitemlist->Delete(listsel);
				if (sh.item_amount)
					m_shopitemlist->SetSelection(listsel != sh.item_amount ? listsel : listsel - 1);
				if (sh.item_amount < SHOP_ITEM_AMOUNT)
					sh.item_list[sh.item_amount] = 0xFF;
			} else {
				return;
			}
		} else if (id == wxID_UP) {
			if (listsel != wxNOT_FOUND && listsel != 0) {
				int tmp = sh.item_list[listsel];
				sh.item_list[listsel] = sh.item_list[listsel - 1];
				sh.item_list[listsel - 1] = tmp;
				wxString tmpstr = m_shopitemlist->GetString(listsel);
				m_shopitemlist->SetString(listsel, m_shopitemlist->GetString(listsel - 1));
				m_shopitemlist->SetString(listsel - 1, tmpstr);
				m_shopitemlist->SetSelection(listsel - 1);
			} else {
				return;
			}
		} else if (id == wxID_DOWN) {
			if (listsel != wxNOT_FOUND && listsel != sh.item_amount - 1) {
				int tmp = sh.item_list[listsel];
				sh.item_list[listsel] = sh.item_list[listsel + 1];
				sh.item_list[listsel + 1] = tmp;
				wxString tmpstr = m_shopitemlist->GetString(listsel);
				m_shopitemlist->SetString(listsel, m_shopitemlist->GetString(listsel + 1));
				m_shopitemlist->SetString(listsel + 1, tmpstr);
				m_shopitemlist->SetSelection(listsel + 1);
			} else {
				return;
			}
		} else if (id == wxID_CFGROUP) {
			DatabaseFormatDialog dial(this, &shopset.csv_header_shop, &shopset.csv_format_shop, &shopset.custom_field_shop);
			if (dial.ShowModal() == wxID_OK) {
				for (unsigned int i = 0; i < shopset.shop.size(); i++)
					UpdateCustomFieldMap(shopset.shop[i].custom_field, shopset.custom_field_shop);
				MarkDataShopModified();
			}
			return;
		} else if (id == wxID_CFENTRY) {
			DatabaseFieldDialog dial(this, &sh.custom_field, shopset.custom_field_shop);
			if (dial.ShowModal() == wxID_OK)
				MarkDataShopModified();
			return;
		}
	}
	MarkDataShopModified();
}

//=============================//
//        Cards & Decks        //
//=============================//

void CDDataStruct::DisplayCardSetHelp(void) {
	CardDeckDataStruct& cd = cardset.deck[m_carddecklist->GetSelection()];
	wxString infolabel1, infolabel2;
	for (unsigned int i=0;i<CARD_SET_CAPACITY;) {
		infolabel1 << _(cardset.card[cardset.set[cd.set].card[i++]].name.GetStr(hades::TEXT_PREVIEW_TYPE)) << _(L"\n");
		infolabel2 << _(cardset.card[cardset.set[cd.set].card[i++]].name.GetStr(hades::TEXT_PREVIEW_TYPE)) << _(L"\n");
	}
	m_carddecksetinfo1->SetLabel(infolabel1);
	m_carddecksetinfo2->SetLabel(infolabel2);
}

void CDDataStruct::DisplayCardPowerHelp(void) {
	unsigned int* sortid = (unsigned int*)m_cardlist->GetClientData(m_cardlist->GetSelection());
	CardDataStruct& cd = cardset.card[*sortid];
	wxString infolabelplayer, infolabelnpc;
	wxString typestr = cd.player_type==0 ? _(L"P") : cd.player_type==1 ? _(L"M") : cd.player_type==2 ? _(L"X") : _(L"A");
	infolabelplayer.Printf(wxT("%X%s%X%X\n%X%s%X%X"),/*
						*/ cd.player_attack >> 5,typestr,cd.player_defence >> 5,cd.player_magicdefence >> 5,/*
						*/ max(0,cd.player_attack-2) >> 4,typestr,max(0,cd.player_defence-2) >> 4,max(0,cd.player_magicdefence-2) >> 4);
	typestr = cd.npc_type==0 ? _(L"P") : cd.npc_type==1 ? _(L"M") : cd.npc_type==2 ? _(L"X") : _(L"A");
	infolabelnpc.Printf(wxT("%X%s%X%X\n%X%s%X%X"),/*
						*/ cd.npc_attack >> 5,typestr,cd.npc_defence >> 5,cd.npc_magicdefence >> 5,/*
						*/ max(0,cd.npc_attack-2) >> 4,typestr,max(0,cd.npc_defence-2) >> 4,max(0,cd.npc_magicdefence-2) >> 4);
	if (gametype==GAME_TYPE_PSX) {
		if (infolabelplayer!=m_cardplayerpowerlabel->GetLabel())
			m_cardplayerpowerlabel->SetLabel(infolabelplayer);
		if (infolabelnpc!=m_cardnpcpowerlabel->GetLabel())
			m_cardnpcpowerlabel->SetLabel(infolabelnpc);
	} else {
		if (infolabelplayer!=m_cardpowerlabel->GetLabel())
			m_cardpowerlabel->SetLabel(infolabelplayer);
	}
}

void CDDataStruct::DisplayCard(int cardid) {
	unsigned int* sortid = (unsigned int*)m_cardlist->GetClientData(cardid);
	CardDataStruct& cd = cardset.card[*sortid];
	m_cardname->ChangeValue(_(cd.name.str));
	if (gametype==GAME_TYPE_PSX) {
		m_cardstatsteampanel->Show(false);
		m_cardstatpsxpanel->Show(true);
		m_cardplayertype->SetSelection(cd.player_type);
		m_cardplayerattack->SetValue(cd.player_attack >> 1);
		m_cardplayerdefence->SetValue(cd.player_defence >> 1);
		m_cardplayermagicdefence->SetValue(cd.player_magicdefence >> 1);
		m_cardnpctype->SetSelection(cd.npc_type);
		m_cardnpcattack->SetValue(cd.npc_attack >> 1);
		m_cardnpcdefence->SetValue(cd.npc_defence >> 1);
		m_cardnpcmagicdefence->SetValue(cd.npc_magicdefence >> 1);
	} else {
		m_cardstatpsxpanel->Show(false);
		m_cardstatsteampanel->Show(true);
		m_cardtype->SetSelection(cd.player_type);
		m_cardattack->SetValue(cd.player_attack >> 1);
		m_carddefence->SetValue(cd.player_defence >> 1);
		m_cardmagicdefence->SetValue(cd.player_magicdefence >> 1);
	}
	m_cardpoints->SetValue(cd.points);
	DisplayCardPowerHelp();
	m_cardscrolledwindow->Layout();
	m_cardscrolledwindow->GetParent()->GetSizer()->Layout();
	m_cardscrolledwindow->Refresh();
}

void CDDataStruct::DisplayCardDeck(int deckid) {
	CardDeckDataStruct& cd = cardset.deck[deckid];
	m_carddeckdifficulty->SetValue(cd.difficulty);
	m_carddeckset->SetSelection(cd.set);
	DisplayCardSetHelp();
	m_carddeckscrolledwindow->Layout();
	m_carddeckscrolledwindow->GetParent()->GetSizer()->Layout();
	m_carddeckscrolledwindow->Refresh();
}

void CDDataStruct::UpdateCardName(unsigned int cardid) {
	if (!m_enemystatdropcard->IsEmpty())
		m_enemystatdropcard->SetString(cardid+1,_(cardset.card[cardid].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
}

void CDDataStruct::OnListBoxCard(wxCommandEvent& event) {
	DisplayCard(m_cardlist->GetSelection());
}

void CDDataStruct::OnListBoxCardDeck(wxCommandEvent& event) {
	DisplayCardDeck(m_carddecklist->GetSelection());
}

void CDDataStruct::OnCardChangeName(wxCommandEvent& event) {
	int sel = m_cardlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cardlist->GetClientData(sel);
	CardDataStruct& cd = cardset.card[*sortid];
	if (cd.SetName(m_cardname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_cardname->GetInsertionPoint();
		m_cardname->ChangeValue(_(cd.name.str));
		m_cardname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_cardlist->SetString(sel,_(cd.name.str));
		UpdateCardName(*sortid);
		MarkDataCardModified();
	}
}

void CDDataStruct::OnCardChangeButton(wxCommandEvent& event) {
	unsigned int sel = m_cardlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cardlist->GetClientData(sel);
	CardDataStruct& cd = cardset.card[*sortid];
	CardSetDataStruct& cs = cardset.set[cardset.deck[m_carddecklist->GetSelection()].set];
	int id = event.GetId();
	if (id == wxID_NAME) {
		if (gametype==GAME_TYPE_PSX) {
			TextEditDialog ted(this,cd.name,cardset.name_space_total-cardset.name_space_used);
			if (ted.ShowModal()==wxID_OK) {
				cd.SetName(ted.text);
				m_cardname->SetValue(cd.name.str);
			}
		} else {
			TextSteamEditDialog ted(this,&ffuiset,cd.name);
			if (ted.ShowModal()==wxID_OK) {
				cd.SetName(ted.text);
				m_cardname->SetValue(cd.name.str);
			}
		}
	} else if (id == wxID_DECKSET) {
		CardSetDialog dial(this);
		if (dial.ShowModal(cardset,m_carddeckset->GetSelection())==wxID_OK) {
			cs.card[0] = dial.m_choicecard1->GetSelection();	cs.card[1] = dial.m_choicecard2->GetSelection();
			cs.card[2] = dial.m_choicecard3->GetSelection();	cs.card[3] = dial.m_choicecard4->GetSelection();
			cs.card[4] = dial.m_choicecard5->GetSelection();	cs.card[5] = dial.m_choicecard6->GetSelection();
			cs.card[6] = dial.m_choicecard7->GetSelection();	cs.card[7] = dial.m_choicecard8->GetSelection();
			cs.card[8] = dial.m_choicecard9->GetSelection();	cs.card[9] = dial.m_choicecard10->GetSelection();
			cs.card[10] = dial.m_choicecard11->GetSelection();	cs.card[11] = dial.m_choicecard12->GetSelection();
			cs.card[12] = dial.m_choicecard13->GetSelection();	cs.card[13] = dial.m_choicecard14->GetSelection();
			cs.card[14] = dial.m_choicecard15->GetSelection();	cs.card[15] = dial.m_choicecard16->GetSelection();
			DisplayCardSetHelp();
			MarkDataCardModified();
		}
	}
}

void CDDataStruct::OnCardChangeChoice(wxCommandEvent& event) {
	unsigned int cardsel = m_cardlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cardlist->GetClientData(cardsel);
	CardDataStruct& cs = cardset.card[*sortid];
	CardDeckDataStruct& cd = cardset.deck[m_carddecklist->GetSelection()];
	int id = event.GetId();
	if (id == wxID_TYPEPLY) {
		cs.player_type = event.GetSelection();
		DisplayCardPowerHelp();
	} else if (id == wxID_TYPENPC) {
		cs.npc_type = event.GetSelection();
		DisplayCardPowerHelp();
	} else if (id == wxID_DECKSET) {
		cd.set = event.GetSelection();
		DisplayCardSetHelp();
	}
	MarkDataCardModified();
}

void CDDataStruct::OnCardChangeSpin(wxSpinEvent& event) {
	unsigned int cardsel = m_cardlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_cardlist->GetClientData(cardsel);
	CardDataStruct& cs = cardset.card[*sortid];
	CardDeckDataStruct& cd = cardset.deck[m_carddecklist->GetSelection()];
	int id = event.GetId();
	if (id == wxID_ATTPLY) { // Keep the parity of the stats
		if (gametype==GAME_TYPE_PSX)
			cs.player_attack = (cs.player_attack & 1) | (m_cardplayerattack->GetValue() << 1);
		else
			cs.player_attack = (cs.player_attack & 1) | (m_cardattack->GetValue() << 1);
		DisplayCardPowerHelp();
	} else if (id == wxID_DEFPLY) {
		if (gametype==GAME_TYPE_PSX)
			cs.player_defence = (cs.player_defence & 1) | (m_cardplayerdefence->GetValue() << 1);
		else
			cs.player_defence = (cs.player_defence & 1) | (m_carddefence->GetValue() << 1);
		DisplayCardPowerHelp();
	} else if (id == wxID_MDEFPLY) {
		if (gametype==GAME_TYPE_PSX)
			cs.player_magicdefence = (cs.player_magicdefence & 1) | (m_cardplayermagicdefence->GetValue() << 1);
		else
			cs.player_magicdefence = (cs.player_magicdefence & 1) | (m_cardmagicdefence->GetValue() << 1);
		DisplayCardPowerHelp();
	} else if (id == wxID_ATTNPC) {
		cs.npc_attack = (cs.npc_attack & 1) | (m_cardnpcattack->GetValue() << 1);
		DisplayCardPowerHelp();
	} else if (id == wxID_DEFNPC) {
		cs.npc_defence = (cs.npc_defence & 1) | (m_cardnpcdefence->GetValue() << 1);
		DisplayCardPowerHelp();
	} else if (id == wxID_MDEFNPC) {
		cs.npc_magicdefence = (cs.npc_magicdefence & 1) | (m_cardnpcmagicdefence->GetValue() << 1);
		DisplayCardPowerHelp();
	} else if (id == wxID_POINTS) {
		cs.points = m_cardpoints->GetValue();
	} else if (id == wxID_DIFFICULTY) {
		cd.difficulty = m_carddeckdifficulty->GetValue();
	}
	MarkDataCardModified();
}

//=============================//
//          Enemies            //
//=============================//

void CDDataStruct::DisplayEnemyStat(int battleid, int statid) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(battleid);
	EnemyStatDataStruct& es = enemyset.battle[*sortid]->stat[statid];
	unsigned int i;
	m_enemystatscrolledwindow->Show(true);
	m_enemygroupscrolledwindow->Show(false);
	m_enemyspellscrolledwindow->Show(false);
	m_enemystatid->Enable(false);
	m_enemystatmodel->Enable(gametype != GAME_TYPE_PSX);
	m_enemystatmodelid->Enable(gametype != GAME_TYPE_PSX);
	m_enemystatitemstealrate1->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemstealrate2->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemstealrate3->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemstealrate4->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemdroprate1->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemdroprate2->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemdroprate3->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatitemdroprate4->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatdropcardrate->Enable(gametype == GAME_TYPE_STEAM && config.dll_usage != 0);
	m_enemystatlvl->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystathp->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_enemystatmp->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_enemystatgils->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_enemystatexp->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_enemystatattack->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_enemystataccuracy->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatspeed->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatstrength->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatmagic->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatspirit->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatdefence->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatevade->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatmagicdefence->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatmagicevade->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemystatimmunebaseint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 31);
	m_enemystatautobaseint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 31);
	m_enemystatinitialbaseint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 31);
	m_enemystatid->SetValue(statid);
	m_enemyspelllist->SetSelection(wxNOT_FOUND);
	m_enemygrouplist->SetSelection(wxNOT_FOUND);
	m_enemystatname->ChangeValue(_(es.name.str));
	m_enemystatlvl->SetValue(es.lvl);
	m_enemystathp->SetValue(es.hp);
	m_enemystatmp->SetValue(es.mp);
	m_enemystatattack->SetValue(es.attack);
	m_enemystataccuracy->SetValue(es.accuracy);
	m_enemystatspeed->SetValue(es.speed);
	m_enemystatstrength->SetValue(es.strength);
	m_enemystatmagic->SetValue(es.magic);
	m_enemystatspirit->SetValue(es.spirit);
	m_enemystatdefence->SetValue(es.defence);
	m_enemystatevade->SetValue(es.evade);
	m_enemystatmagicdefence->SetValue(es.magic_defence);
	m_enemystatmagicevade->SetValue(es.magic_evade);
	m_enemystatitemsteal1->SetSelection(itemset.GetItemIndexById(es.item_steal[0]));
	m_enemystatitemsteal2->SetSelection(itemset.GetItemIndexById(es.item_steal[1]));
	m_enemystatitemsteal3->SetSelection(itemset.GetItemIndexById(es.item_steal[2]));
	m_enemystatitemsteal4->SetSelection(itemset.GetItemIndexById(es.item_steal[3]));
	m_enemystatitemstealrate1->SetValue(es.item_steal_rate[0]);
	m_enemystatitemstealrate2->SetValue(es.item_steal_rate[1]);
	m_enemystatitemstealrate3->SetValue(es.item_steal_rate[2]);
	m_enemystatitemstealrate4->SetValue(es.item_steal_rate[3]);
	m_enemystatitemdrop1->SetSelection(itemset.GetItemIndexById(es.item_drop[0]));
	m_enemystatitemdrop2->SetSelection(itemset.GetItemIndexById(es.item_drop[1]));
	m_enemystatitemdrop3->SetSelection(itemset.GetItemIndexById(es.item_drop[2]));
	m_enemystatitemdrop4->SetSelection(itemset.GetItemIndexById(es.item_drop[3]));
	m_enemystatitemdroprate1->SetValue(es.item_drop_rate[0]);
	m_enemystatitemdroprate2->SetValue(es.item_drop_rate[1]);
	m_enemystatitemdroprate3->SetValue(es.item_drop_rate[2]);
	m_enemystatitemdroprate4->SetValue(es.item_drop_rate[3]);
	if (es.card_drop >= CARD_AMOUNT)
		m_enemystatdropcard->SetSelection(0);
	else
		m_enemystatdropcard->SetSelection(es.card_drop + 1);
	m_enemystatdropcardrate->SetValue(es.card_drop_rate);
	m_enemystatexp->SetValue(es.exp);
	m_enemystatgils->SetValue(es.gils);
	m_enemystatdefaultattack->SetSelection(es.default_attack);
	MACRO_FLAG_DISPLAY8(es.death_flag, m_enemydeathflag)
	for (i = 0; i < m_enemystatmodel->GetCount(); i++) {
		SortedChoiceItem* modname = (SortedChoiceItem*)m_enemystatmodel->GetClientData(i);
		if (modname->id == es.model) {
			m_enemystatmodel->SetSelection(i);
			break;
		}
	}
	m_enemystatmodelid->SetValue(es.model);
	m_enemystatzerostat->SetValue(es.zerostat);
	m_enemystatzero1->SetValue(es.zero1);
	m_enemystatzero2->SetValue(es.zero2);
	m_enemystatzero3->SetValue(es.zero3);

	MACRO_FLAG_DISPLAY8(es.element_absorb, m_enemystatelementabsorb)
	MACRO_FLAG_DISPLAY8(es.element_immune, m_enemystatelementimmune)
	MACRO_FLAG_DISPLAY8(es.element_half, m_enemystatelementhalf)
	MACRO_FLAG_DISPLAY8(es.element_weak, m_enemystatelementweak)
	MACRO_FLAG_DISPLAY8(es.classification, m_enemystatclass)
	m_enemystatimmunelist->Clear();
	m_enemystatautolist->Clear();
	m_enemystatinitiallist->Clear();
	for (auto st = es.status_immune.begin(); st != es.status_immune.end(); st++)
		m_enemystatimmunelist->Append(SpellStatusDialog::StatusDisplayName(*st), new int[1]{ *st });
	for (auto st = es.status_auto.begin(); st != es.status_auto.end(); st++)
		m_enemystatautolist->Append(SpellStatusDialog::StatusDisplayName(*st), new int[1]{ *st });
	for (auto st = es.status_initial.begin(); st != es.status_initial.end(); st++)
		m_enemystatinitiallist->Append(SpellStatusDialog::StatusDisplayName(*st), new int[1]{ *st });
	int bluemagicspellid = AnyAbilityStruct::ConvertIdToSpellId(es.blue_magic);
	int bluemagicsupportid = AnyAbilityStruct::ConvertIdToSupportId(es.blue_magic);
	if (bluemagicspellid >= 0)
		m_enemystatbluemagic->SetSelection(spellset.GetSpellIndexById(bluemagicspellid));
	else if (bluemagicsupportid >= 0)
		m_enemystatbluemagic->SetSelection(spellset.spell.size() + supportset.GetSupportIndexById(bluemagicsupportid));
	m_enemystatscrolledwindow->Layout();
	m_enemystatscrolledwindow->GetParent()->GetSizer()->Layout();
	m_enemystatscrolledwindow->Refresh();
}

void CDDataStruct::DisplayEnemySpell(int battleid, int spellid) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(battleid);
	EnemyDataStruct& ed = *enemyset.battle[*sortid];
	EnemySpellDataStruct& ep = ed.spell[spellid];
	BattleDataStruct& bd = *enemyset.battle_data[*sortid];
	unsigned int i;
	m_enemystatscrolledwindow->Show(false);
	m_enemygroupscrolledwindow->Show(false);
	m_enemyspellscrolledwindow->Show(true);
	m_enemyspellid->Enable(false);
	m_enemyspellstatusint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : STATUS_SET_AMOUNT - 1);
	m_enemyspellanimreflect->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 511);
	m_enemyspellmodelalt->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : (int)UINT16_MAX);
	m_enemyspelleffectint->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemyspellpower->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemyspellaccuracy->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemyspellmp->SetRange(0, gametype == GAME_TYPE_STEAM && config.dll_usage != 0 ? INT32_MAX : 255);
	m_enemyspellid->SetValue(spellid);
	m_enemystatlist->SetSelection(wxNOT_FOUND);
	m_enemygrouplist->SetSelection(wxNOT_FOUND);
	m_enemyspellname->ChangeValue(_(ep.name.str));
	if (ep.effect < HADES_STRING_SPELL_EFFECT_COUNT)
		m_enemyspelleffect->SetSelection(ep.effect);
	m_enemyspelleffectint->SetValue(ep.effect);
	if (ep.effect < (int)HADES_STRING_SPELL_EFFECT.size())
		m_enemyspelleffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[ep.effect].help);
	else
		m_enemyspelleffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
	int x, y;
	m_enemyspelleffecthelp->GetSize(&x, &y);
	m_enemyspelleffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
	m_enemyspellpower->SetValue(ep.power);
	MACRO_FLAG_DISPLAY8(ep.element, m_enemyspellelement)
	m_enemyspellaccuracy->SetValue(ep.accuracy);
	if (ep.status < STATUS_SET_AMOUNT)
		m_enemyspellstatus->SetSelection(ep.status);
	m_enemyspellstatusint->SetValue(ep.status);
	m_enemyspellmp->SetValue(ep.mp);
	m_enemyspellanimreflect->SetValue(ep.model);
	wxString modeltooltip = wxString::Format(wxT("SFX %d"), ep.model);
	for (i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++)
		if (HADES_STRING_SPELL_MODEL[i].id == ep.model) {
			modeltooltip = HADES_STRING_SPELL_MODEL[i].label;
			break;
		}
	m_enemyspellanimreflectlabel->SetLabel(modeltooltip);
	m_enemyspellbaseanim->SetSelection(bd.sequence_stat_id[spellid]);
	m_enemyspelltargetflagdead->SetValue(ep.target_flag & TARGET_FLAG_CAN_TARGET_DEAD);
	m_enemyspelltargetflagcamera->SetValue(ep.target_flag & TARGET_FLAG_CAMERA);
	m_enemyspelltargetflagdeadfirst->SetValue(ep.target_flag & TARGET_FLAG_TARGET_DEAD_FIRST);
	MACRO_FLAG_DISPLAY8(ep.flag, m_enemyspellflag)
	MACRO_FLAG_DISPLAY8(ep.menu_flag, m_enemyspellmenuflag)
	Spell_Target_Type tt = ep.GetTargetType();
	m_enemyspelltargettype->SetSelection(tt);
	m_enemyspelltargetamount->SetSelection(ep.GetTargetAmount());
	m_enemyspelltargetpriority->SetSelection(ep.GetTargetPriority());
	m_enemyspelltargetamount->Enable(tt != SPELL_TARGET_TYPE_EVERYONE && tt != SPELL_TARGET_TYPE_SELF && tt != SPELL_TARGET_TYPE_IRRELEVANT);
	m_enemyspelltargetpriority->Enable(tt == SPELL_TARGET_TYPE_ANY);
	m_enemyspelltargetpanel->SetSelection(ep.GetPanel());
	m_enemyspellmodelalt->SetValue(ep.model_alt);
	m_enemyspellnameoffset->SetValue(ep.name_offset);
	m_enemyspellscrolledwindow->Layout();
	m_enemyspellscrolledwindow->GetParent()->GetSizer()->Layout();
	m_enemyspellscrolledwindow->Refresh();
}

void CDDataStruct::DisplayEnemyGroup(int battleid, int groupid) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(battleid);
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	EnemyGroupDataStruct& eg = eb.group[groupid];
	unsigned int i;
	m_enemystatscrolledwindow->Show(false);
	m_enemygroupscrolledwindow->Show(true);
	m_enemyspellscrolledwindow->Show(false);
	m_enemygroupid->Enable(false);
	m_enemygroupid->SetValue(groupid);
	m_enemystatlist->SetSelection(wxNOT_FOUND);
	m_enemyspelllist->SetSelection(wxNOT_FOUND);
	m_enemygroupfrequence->SetValue(eg.frequence);
	m_enemygroupfrequence->Enable(groupid + 1 < eb.group_amount);
	m_enemygroupamount->SetValue(eg.enemy_amount);
	m_enemygroupap->SetValue(eg.ap);
	m_enemygroupcamera->SetValue(eg.camera_engage);
	m_enemygroupid1->Clear();
	m_enemygroupid2->Clear();
	m_enemygroupid3->Clear();
	m_enemygroupid4->Clear();
	for (i=0;i<eb.stat_amount;i++) {
		m_enemygroupid1->Append(_(eb.stat[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemygroupid2->Append(_(eb.stat[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemygroupid3->Append(_(eb.stat[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemygroupid4->Append(_(eb.stat[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	m_enemygroupid1->SetSelection(eg.enemy_id[0]);
	m_enemygroupid1->Enable(eg.enemy_amount>0);
	m_enemygrouptargetable1->SetValue(eg.targetable[0] & 1);
	m_enemygrouplink1->SetValue(eg.targetable[0] & 2);
	m_enemygroupposx1->SetValue(eg.enemy_posx[0]);
	m_enemygroupposy1->SetValue(eg.enemy_posy[0]);
	m_enemygroupposz1->SetValue(-eg.enemy_posz[0]);
	m_enemygroupangle1->SetValue((eg.enemy_angle[0]*360/0x1000));
	m_enemygroupid2->SetSelection(eg.enemy_id[1]);
	m_enemygroupid2->Enable(eg.enemy_amount>1);
	m_enemygrouptargetable2->SetValue(eg.targetable[1] & 1);
	m_enemygrouplink2->SetValue(eg.targetable[1] & 2);
	m_enemygroupposx2->SetValue(eg.enemy_posx[1]);
	m_enemygroupposy2->SetValue(eg.enemy_posy[1]);
	m_enemygroupposz2->SetValue(-eg.enemy_posz[1]);
	m_enemygroupangle2->SetValue((eg.enemy_angle[1]*360/0x1000));
	m_enemygroupid3->SetSelection(eg.enemy_id[2]);
	m_enemygroupid3->Enable(eg.enemy_amount>2);
	m_enemygrouptargetable3->SetValue(eg.targetable[2] & 1);
	m_enemygrouplink3->SetValue(eg.targetable[2] & 2);
	m_enemygroupposx3->SetValue(eg.enemy_posx[2]);
	m_enemygroupposy3->SetValue(eg.enemy_posy[2]);
	m_enemygroupposz3->SetValue(-eg.enemy_posz[2]);
	m_enemygroupangle3->SetValue((eg.enemy_angle[2]*360/0x1000));
	m_enemygroupid4->SetSelection(eg.enemy_id[3]);
	m_enemygroupid4->Enable(eg.enemy_amount>3);
	m_enemygrouptargetable4->SetValue(eg.targetable[3] & 1);
	m_enemygrouplink4->SetValue(eg.targetable[3] & 2);
	m_enemygroupposx4->SetValue(eg.enemy_posx[3]);
	m_enemygroupposy4->SetValue(eg.enemy_posy[3]);
	m_enemygroupposz4->SetValue(-eg.enemy_posz[3]);
	m_enemygroupangle4->SetValue((eg.enemy_angle[3]*360/0x1000));
	m_enemygroupscrolledwindow->Layout();
	m_enemygroupscrolledwindow->GetParent()->GetSizer()->Layout();
	m_enemygroupscrolledwindow->Refresh();
}

void CDDataStruct::DisplayEnemy(int battleid) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(battleid);
	unsigned int i;
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	TextDataStruct& td = *enemyset.text[*sortid];
	ScriptDataStruct& sc = *enemyset.script[*sortid];
	m_enemystatlist->Clear();
	m_enemyspellbaseanim->Clear();
	for (i = 0; i < eb.stat_amount; i++) {
		m_enemystatlist->Append(_(eb.stat[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemyspellbaseanim->Append(_(eb.stat[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	m_enemyspelllist->Clear();
	m_enemystatdefaultattack->Clear();
	for (i = 0; i < eb.spell_amount; i++) {
		m_enemyspelllist->Append(_(eb.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatdefaultattack->Append(_(eb.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	m_enemygrouplist->Clear();
	for (i = 0; i < eb.group_amount; i++) {
		stringstream groupname;
		groupname << "Group " << i + 1 << ends;
		m_enemygrouplist->Append(_(groupname.str()));
	}
	m_enemytextlist->Clear();
	for (i = eb.stat_amount + eb.spell_amount; i < td.amount; i++)
		m_enemytextlist->Append(_(td.text[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME); i++)
		if (eb.scene_id == HADES_STRING_BATTLE_SCENE_NAME[i].id) {
			m_enemyscene->SetSelection(i);
			break;
		}
	m_enemyscriptlink->Enable(sc.multi_lang_script != NULL);
	MACRO_FLAG_DISPLAY16(eb.flag, m_enemyflag)
	m_enemystatlist->SetSelection(0);
	DisplayEnemyStat(battleid, 0);
	m_enemyscrolledwindow->Layout();
	m_enemyscrolledwindow->GetParent()->GetSizer()->Layout();
	m_enemyscrolledwindow->Refresh();
}

void CDDataStruct::UpdateEnemyName(unsigned int battleid) {
	if (!m_worldbattlebattlechoice11->IsEmpty()) {
		wxString newname = GetEnemyBattleName(battleid);
		m_worldbattlebattlechoice11->SetString(battleid, newname);
		m_worldbattlebattlechoice12->SetString(battleid, newname);
		m_worldbattlebattlechoice13->SetString(battleid, newname);
		m_worldbattlebattlechoice14->SetString(battleid, newname);
		m_worldbattlebattlechoice21->SetString(battleid, newname);
		m_worldbattlebattlechoice22->SetString(battleid, newname);
		m_worldbattlebattlechoice23->SetString(battleid, newname);
		m_worldbattlebattlechoice24->SetString(battleid, newname);
		m_worldbattlebattlechoice31->SetString(battleid, newname);
		m_worldbattlebattlechoice32->SetString(battleid, newname);
		m_worldbattlebattlechoice33->SetString(battleid, newname);
		m_worldbattlebattlechoice34->SetString(battleid, newname);
		m_worldbattlebattlechoice41->SetString(battleid, newname);
		m_worldbattlebattlechoice42->SetString(battleid, newname);
		m_worldbattlebattlechoice43->SetString(battleid, newname);
		m_worldbattlebattlechoice44->SetString(battleid, newname);
	}
}

void CDDataStruct::OnListBoxEnemy(wxCommandEvent& event) {
	DisplayEnemy(m_enemylist->GetSelection());
}

void CDDataStruct::OnListBoxEnemyStat(wxCommandEvent& event) {
	DisplayEnemyStat(m_enemylist->GetSelection(),m_enemystatlist->GetSelection());
}

void CDDataStruct::OnListBoxEnemySpell(wxCommandEvent& event) {
	DisplayEnemySpell(m_enemylist->GetSelection(),m_enemyspelllist->GetSelection());
}

void CDDataStruct::OnListBoxEnemyGroup(wxCommandEvent& event) {
	DisplayEnemyGroup(m_enemylist->GetSelection(),m_enemygrouplist->GetSelection());
}

void CDDataStruct::OnListBoxEnemyText(wxCommandEvent& event) {
	unsigned int sel = m_enemylist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	unsigned int textsel = m_enemytextlist->GetSelection();
	unsigned int id = textsel+enemyset.battle[*sortid]->stat_amount+enemyset.battle[*sortid]->spell_amount;
	TextDataStruct& td = *enemyset.text[*sortid];
	if (gametype==GAME_TYPE_PSX) {
		TextEditDialog ted(this,td.text[id],td.GetExtraSize());
		if (ted.ShowModal()==wxID_OK) {
			td.SetText(id,ted.text);
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_TEXT);
			m_enemytextlist->SetString(textsel,td.text[id].GetStr(hades::TEXT_PREVIEW_TYPE));
		}
	} else {
		TextSteamEditDialog ted(this,&ffuiset,td.text[id]);
		if (ted.ShowModal()==wxID_OK) {
			MACRO_MULTILANG_INCREASE_COUNTER(td.text[id],ted.text,td.text,id,L"[STRT=0,1]")
			td.SetText(id,ted.text);
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_TEXT);
			m_enemytextlist->SetString(textsel,td.text[id].GetStr(hades::TEXT_PREVIEW_TYPE));
		}
	}
}

void CDDataStruct::OnEnemyStatChangeName(wxCommandEvent& event) {
	int sel = m_enemylist->GetSelection();
	int statsel = m_enemystatlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	EnemyStatDataStruct& es = enemyset.battle[*sortid]->stat[statsel];
	wstring newname = m_enemystatname->GetValue().ToStdWstring();
	if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
		unsigned int i,j,nb,*battleid;
		EnemyStatDataStruct** stats = enemyset.GetSimilarEnemyStats(es,&nb,&battleid);
		for (i=0;i<nb;i++) {
			if (stats[i]->SetName(newname)) {
				wxMessageDialog popup(this,HADES_STRING_GROUPEDIT_ERROR_TEXT,HADES_STRING_WARNING,wxOK|wxSTAY_ON_TOP|wxCENTRE);
				popup.ShowModal();
				if (stats[i]==&es) {
					wxTextPos ip = m_enemystatname->GetInsertionPoint();
					m_enemystatname->ChangeValue(_(es.name.str));
					m_enemystatname->SetInsertionPoint(ip);
				}
			} else {
				if (GetTopWindow()->m_sortenemy->IsChecked()) {
					for (j=0;j<enemyset.battle_amount;j++)
						if (*(unsigned int*)m_enemylist->GetClientData(j)==battleid[i])
							m_enemylist->SetString(j,GetEnemyBattleName(battleid[i]));
				} else
					m_enemylist->SetString(battleid[i],GetEnemyBattleName(battleid[i]));
				UpdateEnemyName(battleid[i]);
				MarkDataEnemyModified(battleid[i],CHUNK_TYPE_TEXT);
			}
		}
	} else {
		if (es.SetName(newname)) {
			wxTextPos ip = m_enemystatname->GetInsertionPoint();
			m_enemystatname->ChangeValue(_(es.name.str));
			m_enemystatname->SetInsertionPoint(ip);
			TextReachLimit();
		} else {
			m_enemylist->SetString(sel,GetEnemyBattleName(*sortid));
			UpdateEnemyName(*sortid);
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_TEXT);
		}
	}
	m_enemystatlist->SetString(statsel,_(es.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_enemyspellbaseanim->SetString(statsel,_(es.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
}

void CDDataStruct::OnEnemySpellChangeName(wxCommandEvent& event) {
	int spellsel = m_enemyspelllist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(m_enemylist->GetSelection());
	EnemySpellDataStruct& ep = enemyset.battle[*sortid]->spell[spellsel];
	wstring newname = m_enemyspellname->GetValue().ToStdWstring();
	if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
		unsigned int i,nb,*similarbattleid;
		EnemySpellDataStruct** similarspells = enemyset.GetSimilarEnemySpells(ep,&nb,&similarbattleid);
		for (i=0;i<nb;i++) {
			if (similarspells[i]->SetName(newname)) {
				wxMessageDialog popup(this,HADES_STRING_GROUPEDIT_ERROR_TEXT,HADES_STRING_WARNING,wxOK|wxSTAY_ON_TOP|wxCENTRE);
				popup.ShowModal();
				if (similarspells[i]==&ep) {
					wxTextPos ip = m_enemyspellname->GetInsertionPoint();
					m_enemyspellname->ChangeValue(_(ep.name.str));
					m_enemyspellname->SetInsertionPoint(ip);
				}
			} else {
				MarkDataEnemyModified(similarbattleid[i],CHUNK_TYPE_TEXT);
			}
		}
	} else {
		if (ep.SetName(newname)) {
			wxTextPos ip = m_enemyspellname->GetInsertionPoint();
			m_enemyspellname->ChangeValue(_(ep.name.str));
			m_enemyspellname->SetInsertionPoint(ip);
			TextReachLimit();
		} else {
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_TEXT);
		}
	}
	m_enemyspelllist->SetString(spellsel,_(ep.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_enemystatdefaultattack->SetString(spellsel,_(ep.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
}

#define MACRO_ENEMY_CHANGE_DATA(TYPE,DATA,VALUE) \
	if (GetTopWindow()->m_editsimilarenemy->IsChecked()) { \
		unsigned int macroi,macronb,*macrobattleid; \
		Enemy ## TYPE ## DataStruct** macrolist = enemyset.GetSimilarEnemy ## TYPE ## s(enmydata,&macronb,&macrobattleid); \
		for (macroi=0;macroi<macronb;macroi++) { \
			macrolist[macroi]->DATA = VALUE; \
			MarkDataEnemyModified(macrobattleid[macroi],CHUNK_TYPE_ENEMY_STATS); \
		} \
	} else { \
		enmydata.DATA = VALUE; \
		MarkDataEnemyModified(*sortid,CHUNK_TYPE_ENEMY_STATS); \
	}

bool DiscardStatTooHigh = false;
bool DiscardSimilarEnemyUnaffected = false;
void CDDataStruct::OnEnemyChangeSpin(wxSpinEvent& event) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(m_enemylist->GetSelection());
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int id = event.GetId();
	if (m_enemystatlist->GetSelection() != wxNOT_FOUND) {
		EnemyStatDataStruct& enmydata = eb.stat[m_enemystatlist->GetSelection()];
		if (id == wxID_LVL) {
			MACRO_ENEMY_CHANGE_DATA(Stat, lvl, event.GetPosition())
		} else if (id == wxID_HP) {
			MACRO_ENEMY_CHANGE_DATA(Stat, hp, event.GetPosition())
		} else if (id == wxID_MP) {
			MACRO_ENEMY_CHANGE_DATA(Stat, mp, event.GetPosition())
		} else if (id == wxID_ATTACK) {
			MACRO_ENEMY_CHANGE_DATA(Stat, attack, event.GetPosition())
		} else if (id == wxID_ACCURACY) {
			MACRO_ENEMY_CHANGE_DATA(Stat, accuracy, event.GetPosition())
		} else if (id == wxID_SPEED) {
			if (event.GetPosition() >= 60 && !DiscardStatTooHigh) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_STAT_TOO_HIGH);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardStatTooHigh = true;
			}
			MACRO_ENEMY_CHANGE_DATA(Stat, speed, event.GetPosition())
		} else if (id == wxID_STRENGTH) {
			MACRO_ENEMY_CHANGE_DATA(Stat, strength, event.GetPosition())
		} else if (id == wxID_MAGIC) {
			MACRO_ENEMY_CHANGE_DATA(Stat, magic, event.GetPosition())
		} else if (id == wxID_SPIRIT) {
			if (event.GetPosition() >= 60 && !DiscardStatTooHigh) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_STAT_TOO_HIGH);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardStatTooHigh = true;
			}
			MACRO_ENEMY_CHANGE_DATA(Stat, spirit, event.GetPosition())
		} else if (id == wxID_DEFENCE) {
			MACRO_ENEMY_CHANGE_DATA(Stat, defence, event.GetPosition())
		} else if (id == wxID_EVADE) {
			MACRO_ENEMY_CHANGE_DATA(Stat, evade, event.GetPosition())
		} else if (id == wxID_MDEFENCE) {
			MACRO_ENEMY_CHANGE_DATA(Stat, magic_defence, event.GetPosition())
		} else if (id == wxID_MEVADE) {
			MACRO_ENEMY_CHANGE_DATA(Stat, magic_evade, event.GetPosition())
		} else if (id == wxID_EXP) {
			MACRO_ENEMY_CHANGE_DATA(Stat, exp, event.GetPosition())
		} else if (id == wxID_GILS) {
			MACRO_ENEMY_CHANGE_DATA(Stat, gils, event.GetPosition())
		} else if (id == wxID_STEALRATE1) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal_rate[0], event.GetPosition())
		} else if (id == wxID_STEALRATE2) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal_rate[1], event.GetPosition())
		} else if (id == wxID_STEALRATE3) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal_rate[2], event.GetPosition())
		} else if (id == wxID_STEALRATE4) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal_rate[3], event.GetPosition())
		} else if (id == wxID_DROPRATE1) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop_rate[0], event.GetPosition())
		} else if (id == wxID_DROPRATE2) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop_rate[1], event.GetPosition())
		} else if (id == wxID_DROPRATE3) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop_rate[2], event.GetPosition())
		} else if (id == wxID_DROPRATE4) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop_rate[3], event.GetPosition())
		} else if (id == wxID_CARDRATE) {
			MACRO_ENEMY_CHANGE_DATA(Stat, card_drop_rate, event.GetPosition())
		} else if (id == wxID_ZERO0) {
			MACRO_ENEMY_CHANGE_DATA(Stat, zerostat, event.GetPosition())
		} else if (id == wxID_ZERO1) {
			MACRO_ENEMY_CHANGE_DATA(Stat, zero1, event.GetPosition())
		} else if (id == wxID_ZERO2) {
			MACRO_ENEMY_CHANGE_DATA(Stat, zero2, event.GetPosition())
		} else if (id == wxID_ZERO3) {
			MACRO_ENEMY_CHANGE_DATA(Stat, zero3, event.GetPosition())
		} else if (id == wxID_MODEL) {
			BattleModelLinks& linkeddata = BattleModelLinks::GetLinksByModel(event.GetPosition());
			// Due to the need to change the spell sequences, it is not a good idea to use the "Edit Similar Enemies" feature
			enemyset.ChangeBattleModel(*sortid, m_enemystatlist->GetSelection(), linkeddata);
			enmydata.model = event.GetPosition();
			MarkDataEnemyModified(*sortid, CHUNK_TYPE_BATTLE_DATA);
			if (linkeddata.model != 0xFFFF)
				for (unsigned int i = 0; i < m_enemystatmodel->GetCount(); i++)
					if (((SortedChoiceItem*)m_enemystatmodel->GetClientData(i))->id == event.GetPosition()) {
						m_enemystatmodel->SetSelection(i);
						break;
					}
			if (GetTopWindow()->m_editsimilarenemy->IsChecked() && !DiscardSimilarEnemyUnaffected) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_NOGROUPEDIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardSimilarEnemyUnaffected = true;
			}
		} else if (id == wxID_IMMUNESTATUS) {
			int status = event.GetPosition();
			m_enemystatimmunebaselist->SetSelection(status < STATUS_AMOUNT ? status : -1);
		} else if (id == wxID_AUTOSTATUS) {
			int status = event.GetPosition();
			m_enemystatautobaselist->SetSelection(status < STATUS_AMOUNT ? status : -1);
		} else if (id == wxID_INITIALSTATUS) {
			int status = event.GetPosition();
			m_enemystatinitialbaselist->SetSelection(status < STATUS_AMOUNT ? status : -1);
		}
	} else if (m_enemyspelllist->GetSelection() != wxNOT_FOUND) {
		EnemySpellDataStruct& enmydata = eb.spell[m_enemyspelllist->GetSelection()];
		if (id == wxID_EFFECT) {
			MACRO_ENEMY_CHANGE_DATA(Spell, effect, event.GetPosition())
			if (enmydata.effect < HADES_STRING_SPELL_EFFECT_COUNT)
				m_enemyspelleffect->SetSelection(enmydata.effect);
			if (enmydata.effect < (int)HADES_STRING_SPELL_EFFECT.size())
				m_enemyspelleffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[enmydata.effect].help);
			else
				m_enemyspelleffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
			int x, y;
			m_enemyspelleffecthelp->GetSize(&x, &y);
			m_enemyspelleffecthelpwindow->SetScrollbars(20, 5, x / 20, y / 5);
		} else if (id == wxID_POWER) {
			MACRO_ENEMY_CHANGE_DATA(Spell, power, event.GetPosition())
		} else if (id == wxID_ACCURACY) {
			MACRO_ENEMY_CHANGE_DATA(Spell, accuracy, event.GetPosition())
		} else if (id == wxID_STATUS) {
			MACRO_ENEMY_CHANGE_DATA(Spell, status, event.GetPosition())
			if (enmydata.status < STATUS_SET_AMOUNT)
				m_enemyspellstatus->SetSelection(enmydata.status);
		} else if (id == wxID_SPELLMP) {
			MACRO_ENEMY_CHANGE_DATA(Spell, mp, event.GetPosition())
		} else if (id == wxID_ANIM) {
			MACRO_ENEMY_CHANGE_DATA(Spell, model, event.GetPosition())
			wxString modeltooltip = wxString::Format(wxT("SFX %d"), enmydata.model);
			for (unsigned int i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++)
				if (HADES_STRING_SPELL_MODEL[i].id == enmydata.model) {
					modeltooltip = HADES_STRING_SPELL_MODEL[i].label;
					break;
				}
			m_enemyspellanimreflectlabel->SetLabel(modeltooltip);
		} else if (id == wxID_MODELALT) {
			MACRO_ENEMY_CHANGE_DATA(Spell, model_alt, event.GetPosition())
		} else if (id == wxID_SPELLNO) {
			MACRO_ENEMY_CHANGE_DATA(Spell, name_offset, event.GetPosition())
		}
	} else if (m_enemygrouplist->GetSelection() != wxNOT_FOUND) {
		EnemyGroupDataStruct& eg = eb.group[m_enemygrouplist->GetSelection()];
		if (id == wxID_FREQUENCE) {
			eg.frequence = event.GetPosition();
		} else if (id == wxID_AMOUNT) {
			eg.enemy_amount = event.GetPosition();
			m_enemygroupid1->Enable(eg.enemy_amount > 0);
			m_enemygroupid2->Enable(eg.enemy_amount > 1);
			m_enemygroupid3->Enable(eg.enemy_amount > 2);
			m_enemygroupid4->Enable(eg.enemy_amount > 3);
		} else if (id == wxID_AP) {
			eg.ap = event.GetPosition();
		} else if (id == wxID_CAMERA) {
			eg.camera_engage = event.GetPosition();
		} else if (id == wxID_POSX1) {
			eg.enemy_posx[0] = event.GetPosition();
		} else if (id == wxID_POSY1) {
			eg.enemy_posy[0] = event.GetPosition();
		} else if (id == wxID_POSZ1) {
			eg.enemy_posz[0] = -event.GetPosition();
		} else if (id == wxID_ANGLE1) {
			eg.enemy_angle[0] = (event.GetPosition() * 0x1000 / 360);
		} else if (id == wxID_POSX2) {
			eg.enemy_posx[1] = event.GetPosition();
		} else if (id == wxID_POSY2) {
			eg.enemy_posy[1] = event.GetPosition();
		} else if (id == wxID_POSZ2) {
			eg.enemy_posz[1] = -event.GetPosition();
		} else if (id == wxID_ANGLE2) {
			eg.enemy_angle[1] = (event.GetPosition() * 0x1000 / 360);
		} else if (id == wxID_POSX3) {
			eg.enemy_posx[2] = event.GetPosition();
		} else if (id == wxID_POSY3) {
			eg.enemy_posy[2] = event.GetPosition();
		} else if (id == wxID_POSZ3) {
			eg.enemy_posz[2] = -event.GetPosition();
		} else if (id == wxID_ANGLE3) {
			eg.enemy_angle[2] = (event.GetPosition() * 0x1000 / 360);
		} else if (id == wxID_POSX4) {
			eg.enemy_posx[3] = event.GetPosition();
		} else if (id == wxID_POSY4) {
			eg.enemy_posy[3] = event.GetPosition();
		} else if (id == wxID_POSZ4) {
			eg.enemy_posz[3] = -event.GetPosition();
		} else if (id == wxID_ANGLE4) {
			eg.enemy_angle[3] = (event.GetPosition() * 0x1000 / 360);
		}
	}
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_ENEMY_STATS);
}

void CDDataStruct::OnEnemyChangeChoice(wxCommandEvent& event) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(m_enemylist->GetSelection());
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int id = event.GetId();
	if (id == wxID_SCENE) {
		unsigned int* newscene = (unsigned int*)event.GetClientData();
		if (gametype==GAME_TYPE_PSX) {
			ImageMapDataStruct& em = *enemyset.preload[*sortid];
			int extrablock = em.RemoveData(MAP_OBJECT_SCENE,eb.scene_id);
			em.AddData(MAP_OBJECT_SCENE,*newscene,&cluster.global_map,cluster.enemy_shared_map,em.GetExtraSize()/0x10+extrablock);
			em.UpdateOffset();
		}
		enemyset.ChangeBattleScene(enemyset.battle_data[*sortid]->object_id,*newscene);
		MarkDataEnemyModified(*sortid,gametype==GAME_TYPE_PSX ? CHUNK_TYPE_IMAGE_MAP : CHUNK_TYPE_BATTLE_SCENE);
		return;
	} else if (m_enemystatlist->GetSelection()!=wxNOT_FOUND) {
		EnemyStatDataStruct& enmydata = eb.stat[m_enemystatlist->GetSelection()];
		if (id == wxID_STEAL1) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal[0], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_STEAL2) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal[1], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_STEAL3) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal[2], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_STEAL4) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_steal[3], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_DROP1) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop[0], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_DROP2) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop[1], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_DROP3) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop[2], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_DROP4) {
			MACRO_ENEMY_CHANGE_DATA(Stat, item_drop[3], itemset.item[event.GetSelection()].id)
		} else if (id == wxID_CARD) {
			if (event.GetSelection() == 0) {
				MACRO_ENEMY_CHANGE_DATA(Stat, card_drop, 255)
			} else {
				MACRO_ENEMY_CHANGE_DATA(Stat, card_drop, event.GetSelection() - 1)
			}
		} else if (id == wxID_DEFATTACK) {
			MACRO_ENEMY_CHANGE_DATA(Stat,default_attack,event.GetSelection())
		} else if (id == wxID_MODEL) {
			SortedChoiceItem* item = (SortedChoiceItem*)event.GetClientData();
			BattleModelLinks& linkeddata = BattleModelLinks::GetLinksByModel(item->id);
			// Due to the need to change the spell sequences, it is not a good idea to use the "Edit Similar Enemies" feature
			enemyset.ChangeBattleModel(*sortid,m_enemystatlist->GetSelection(),linkeddata);
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_BATTLE_DATA);
			m_enemystatmodelid->SetValue(linkeddata.model);
			if (GetTopWindow()->m_editsimilarenemy->IsChecked() && !DiscardSimilarEnemyUnaffected) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(),HADES_STRING_NOGROUPEDIT);
				if (popup.ShowModal()==wxID_DISCARD)
					DiscardSimilarEnemyUnaffected = true;
			}
		} else if (id == wxID_BLUEMAGIC) {
			int blueid;
			if (event.GetSelection() < (int)spellset.spell.size())
				blueid = AnyAbilityStruct::ConvertSpellIdToId(spellset.spell[event.GetSelection()].id);
			else
				blueid = AnyAbilityStruct::ConvertSupportIdToId(supportset.support[event.GetSelection() - spellset.spell.size()].id);
			MACRO_ENEMY_CHANGE_DATA(Stat, blue_magic, blueid)
		}
	} else if (m_enemyspelllist->GetSelection()!=wxNOT_FOUND) {
		EnemySpellDataStruct& enmydata = eb.spell[m_enemyspelllist->GetSelection()];
		if (id == wxID_EFFECT) {
			MACRO_ENEMY_CHANGE_DATA(Spell, effect, event.GetSelection())
			m_enemyspelleffectint->SetValue(enmydata.effect);
			if (enmydata.effect < (int)HADES_STRING_SPELL_EFFECT.size())
				m_enemyspelleffecthelp->SetLabel(HADES_STRING_SPELL_EFFECT[enmydata.effect].help);
			else
				m_enemyspelleffecthelp->SetLabel(_(HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION));
			int x,y;
			m_enemyspelleffecthelp->GetSize(&x,&y);
			m_enemyspelleffecthelpwindow->SetScrollbars(20,5,x/20,y/5);
		} else if (id == wxID_STATUS) {
			MACRO_ENEMY_CHANGE_DATA(Spell,status,event.GetSelection())
			m_enemyspellstatusint->SetValue(enmydata.status);
		} else if (id == wxID_SEQANIM) {
			enemyset.battle_data[*sortid]->sequence_stat_id[m_enemyspelllist->GetSelection()] = event.GetSelection();
			enemyset.battle_data[*sortid]->sequence_base_anim[m_enemyspelllist->GetSelection()] = enemyset.battle[*sortid]->stat[event.GetSelection()].sequence_anim_base;
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_BATTLE_DATA);
			if (GetTopWindow()->m_editsimilarenemy->IsChecked() && !DiscardSimilarEnemyUnaffected) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(),HADES_STRING_NOGROUPEDIT);
				if (popup.ShowModal()==wxID_DISCARD)
					DiscardSimilarEnemyUnaffected = true;
			}
			return;
		} else if (id == wxID_TARGETTYPE) {
			Spell_Target_Type tt = event.GetSelection();
			enmydata.SetTargetType(tt);
			m_enemyspelltargetamount->SetSelection(enmydata.GetTargetAmount());
			m_enemyspelltargetpriority->SetSelection(enmydata.GetTargetPriority());
			m_enemyspelltargetamount->Enable(tt != SPELL_TARGET_TYPE_EVERYONE && tt != SPELL_TARGET_TYPE_SELF && tt != SPELL_TARGET_TYPE_IRRELEVANT);
			m_enemyspelltargetpriority->Enable(tt == SPELL_TARGET_TYPE_ANY);
			MACRO_ENEMY_CHANGE_DATA(Spell,target_type,enmydata.target_type)
		} else if (id == wxID_TARGETAMOUNT) {
			enmydata.SetTargetAmount(event.GetSelection());
			MACRO_ENEMY_CHANGE_DATA(Spell,target_type,enmydata.target_type)
		} else if (id == wxID_TARGETPRIORITY) {
			enmydata.SetTargetPriority(event.GetSelection());
			MACRO_ENEMY_CHANGE_DATA(Spell,target_type,enmydata.target_type)
		} else if (id == wxID_TARGETPANEL) {
			enmydata.SetPanel(event.GetSelection());
			MACRO_ENEMY_CHANGE_DATA(Spell,target_type,enmydata.target_type)
		}
	} else if (m_enemygrouplist->GetSelection()!=wxNOT_FOUND) {
		EnemyGroupDataStruct& eg = eb.group[m_enemygrouplist->GetSelection()];
		if (id == wxID_ENEMY1) {
			eg.enemy_id[0] = event.GetSelection();
		} else if (id == wxID_ENEMY2) {
			eg.enemy_id[1] = event.GetSelection();
		} else if (id == wxID_ENEMY3) {
			eg.enemy_id[2] = event.GetSelection();
		} else if (id == wxID_ENEMY4) {
			eg.enemy_id[3] = event.GetSelection();
		}
	}
	MarkDataEnemyModified(*sortid,CHUNK_TYPE_ENEMY_STATS);
}

void CDDataStruct::OnEnemyChangeFlags(wxCommandEvent& event) {
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(m_enemylist->GetSelection());
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int id = event.GetId();
	bool on = event.GetInt();
	if (m_enemystatlist->GetSelection()!=wxNOT_FOUND) {
		EnemyStatDataStruct& enmydata = eb.stat[m_enemystatlist->GetSelection()];

		#define MACRO_ENEMY_CHECK_STAT_FLAG(STAT) \
			MACRO_FLAG_SET8(STAT ## .death_flag, wxID_ED) \
			MACRO_FLAG_SET8(STAT ## .element_absorb, wxID_EA) \
			MACRO_FLAG_SET8(STAT ## .element_immune, wxID_EI) \
			MACRO_FLAG_SET8(STAT ## .element_half, wxID_EH) \
			MACRO_FLAG_SET8(STAT ## .element_weak, wxID_EW) \
			MACRO_FLAG_SET8(STAT ## .classification, wxID_EC)

		if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
			unsigned int macroi,macronb,*macrobattleid;
			EnemyStatDataStruct** macrolist = enemyset.GetSimilarEnemyStats(enmydata,&macronb,&macrobattleid);
			for (macroi=0;macroi<macronb;macroi++) {
				MACRO_ENEMY_CHECK_STAT_FLAG(macrolist[macroi][0])
				MarkDataEnemyModified(macrobattleid[macroi],CHUNK_TYPE_ENEMY_STATS);
			}
		} else {
			MACRO_ENEMY_CHECK_STAT_FLAG(enmydata)
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_ENEMY_STATS);
		}
	} else if (m_enemyspelllist->GetSelection()!=wxNOT_FOUND) {
		EnemySpellDataStruct& enmydata = eb.spell[m_enemyspelllist->GetSelection()];

		#define MACRO_ENEMY_CHECK_SPELL_FLAG(SPELL) \
			MACRO_FLAG_SET8(SPELL ## .element, wxID_SE) \
			MACRO_FLAG_SET8(SPELL ## .flag, wxID_SF) \
			MACRO_FLAG_SET8(SPELL ## .menu_flag, wxID_SM) \
			MACRO_FLAG_SET(SPELL ## .target_flag, wxID_CAN_TARGET_DEAD, TARGET_FLAG_CAN_TARGET_DEAD) \
			MACRO_FLAG_SET(SPELL ## .target_flag, wxID_TARGET_CAMERA, TARGET_FLAG_CAMERA) \
			MACRO_FLAG_SET(SPELL ## .target_flag, wxID_TARGET_DEAD_FIRST, TARGET_FLAG_TARGET_DEAD_FIRST)

		if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
			unsigned int macroi,macronb,*macrobattleid;
			EnemySpellDataStruct** macrolist = enemyset.GetSimilarEnemySpells(enmydata,&macronb,&macrobattleid);
			for (macroi=0;macroi<macronb;macroi++) {
				MACRO_ENEMY_CHECK_SPELL_FLAG(macrolist[macroi][0])
				MarkDataEnemyModified(macrobattleid[macroi],CHUNK_TYPE_ENEMY_STATS);
			}
		} else {
			MACRO_ENEMY_CHECK_SPELL_FLAG(enmydata)
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_ENEMY_STATS);
		}
	} else if (m_enemygrouplist->GetSelection()!=wxNOT_FOUND) {
		EnemyGroupDataStruct& eg = eb.group[m_enemygrouplist->GetSelection()];
		if (id == wxID_TARGETABLE1) {
			eg.targetable[0] = (eg.targetable[0] & 0xFE) | (event.IsChecked() ? 1 : 0);
		} else if (id == wxID_TARGETABLE2) {
			eg.targetable[1] = (eg.targetable[1] & 0xFE) | (event.IsChecked() ? 1 : 0);
		} else if (id == wxID_TARGETABLE3) {
			eg.targetable[2] = (eg.targetable[2] & 0xFE) | (event.IsChecked() ? 1 : 0);
		} else if (id == wxID_TARGETABLE4) {
			eg.targetable[3] = (eg.targetable[3] & 0xFE) | (event.IsChecked() ? 1 : 0);
		} else if (id == wxID_LINK1) {
			eg.targetable[0] = (eg.targetable[0] & 0xFD) | (event.IsChecked() << 1);
		} else if (id == wxID_LINK2) {
			eg.targetable[1] = (eg.targetable[1] & 0xFD) | (event.IsChecked() << 1);
		} else if (id == wxID_LINK3) {
			eg.targetable[2] = (eg.targetable[2] & 0xFD) | (event.IsChecked() << 1);
		} else if (id == wxID_LINK4) {
			eg.targetable[3] = (eg.targetable[3] & 0xFD) | (event.IsChecked() << 1);
		}
	}
	MACRO_FLAG_SET16(eb.flag,wxID_BF)
	MarkDataEnemyModified(*sortid,CHUNK_TYPE_ENEMY_STATS);
}

void CDDataStruct::OnEnemyChangeButton(wxCommandEvent& event) {
	int sel = m_enemylist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int id = event.GetId();

	#define MACRO_ENEMY_CHANGE_STATUS(TYPEUPPER, TYPELOWER) \
		} else if (id == wxID_ADD ## TYPEUPPER) { \
			int status = m_enemystat ## TYPELOWER ## baseint->GetValue(); \
			int statsel = m_enemystatlist->GetSelection(); \
			EnemyStatDataStruct& es = eb.stat[statsel]; \
			vector<pair<EnemyStatDataStruct*, int>> similares; \
			if (GetTopWindow()->m_editsimilarenemy->IsChecked()) { \
				unsigned int i, nb, * similarbattleid; \
				EnemyStatDataStruct** similarstats = enemyset.GetSimilarEnemyStats(es, &nb, &similarbattleid); \
				for (i = 0; i < nb; i++) \
					similares.push_back({ similarstats[i], similarbattleid[i] }); \
			} else { \
				similares.push_back({ &es, *sortid }); \
			} \
			for (auto it = similares.begin(); it != similares.end(); it++) { \
				if (it->first->status_ ## TYPELOWER.find(status) != it->first->status_ ## TYPELOWER.end()) \
					continue; \
				it->first->status_ ## TYPELOWER.insert(status); \
				if (it->first == &es) \
					m_enemystat ## TYPELOWER ## list->Append(SpellStatusDialog::StatusDisplayName(status), new int[1]{ status }); \
				MarkDataEnemyModified(it->second, CHUNK_TYPE_ENEMY_STATS); \
			} \
		} else if (id == wxID_REMOVE ## TYPEUPPER) { \
			if (m_enemystat ## TYPELOWER ## list->GetSelection() == wxNOT_FOUND) \
				return; \
			int status = *(int*)m_enemystat ## TYPELOWER ## list->GetClientData(m_enemystat ## TYPELOWER ## list->GetSelection()); \
			int statsel = m_enemystatlist->GetSelection(); \
			EnemyStatDataStruct& es = eb.stat[statsel]; \
			vector<pair<EnemyStatDataStruct*, int>> similares; \
			if (GetTopWindow()->m_editsimilarenemy->IsChecked()) { \
				unsigned int i, nb, * similarbattleid; \
				EnemyStatDataStruct** similarstats = enemyset.GetSimilarEnemyStats(es, &nb, &similarbattleid); \
				for (i = 0; i < nb; i++) \
					similares.push_back({ similarstats[i], similarbattleid[i] }); \
			} else { \
				similares.push_back({ &es, *sortid }); \
			} \
			for (auto it = similares.begin(); it != similares.end(); it++) { \
				if (it->first->status_ ## TYPELOWER.erase(status) > 0) { \
					if (it->first == &es) \
						m_enemystat ## TYPELOWER ## list->Delete(m_enemystat ## TYPELOWER ## list->GetSelection()); \
					MarkDataEnemyModified(it->second, CHUNK_TYPE_ENEMY_STATS); \
				} \
			}

	if (id == wxID_STATNAME) {
		int statsel = m_enemystatlist->GetSelection();
		EnemyStatDataStruct& es = eb.stat[statsel];
		FF9String newstr;
		bool confirmed = false;
		if (gametype == GAME_TYPE_PSX) {
			TextEditDialog ted(this, es.name, eb.GetExtraSize());
			if (ted.ShowModal() == wxID_OK) {
				newstr = ted.text;
				confirmed = true;
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, es.name);
			if (ted.ShowModal() == wxID_OK) {
				newstr = ted.text;
				confirmed = true;
			}
		}
		if (confirmed) {
			if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
				unsigned int i, j, nb, * battleid;
				EnemyStatDataStruct** stats = enemyset.GetSimilarEnemyStats(es, &nb, &battleid);
				for (i = 0; i < nb; i++) {
					if (stats[i]->SetName(newstr)) {
						wxMessageDialog popup(this, HADES_STRING_GROUPEDIT_ERROR_TEXT, HADES_STRING_WARNING, wxOK | wxSTAY_ON_TOP | wxCENTRE);
						popup.ShowModal();
					} else {
						if (GetTopWindow()->m_sortenemy->IsChecked()) {
							for (j = 0; j < enemyset.battle_amount; j++)
								if (*(unsigned int*)m_enemylist->GetClientData(j) == battleid[i])
									m_enemylist->SetString(j, GetEnemyBattleName(battleid[i]));
						} else
							m_enemylist->SetString(battleid[i], GetEnemyBattleName(battleid[i]));
						MarkDataEnemyModified(battleid[i], CHUNK_TYPE_TEXT);
					}
				}
			} else {
				es.SetName(newstr);
				m_enemylist->SetString(sel, GetEnemyBattleName(*sortid));
				MarkDataEnemyModified(*sortid, CHUNK_TYPE_TEXT);
			}
			m_enemystatname->ChangeValue(_(es.name.str));
			m_enemystatlist->SetString(statsel, _(es.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
	MACRO_ENEMY_CHANGE_STATUS(IMMUNE, immune)
	MACRO_ENEMY_CHANGE_STATUS(AUTO, auto)
	MACRO_ENEMY_CHANGE_STATUS(INITIAL, initial)
	} else if (id == wxID_SPELLNAME) {
		unsigned int spellsel = m_enemyspelllist->GetSelection();
		EnemySpellDataStruct& ep = eb.spell[spellsel];
		FF9String newstr;
		bool confirmed = false;
		if (gametype == GAME_TYPE_PSX) {
			TextEditDialog ted(this, ep.name, eb.GetExtraSize());
			if (ted.ShowModal() == wxID_OK) {
				newstr = ted.text;
				confirmed = true;
			}
		} else {
			TextSteamEditDialog ted(this, &ffuiset, ep.name);
			if (ted.ShowModal() == wxID_OK) {
				newstr = ted.text;
				confirmed = true;
			}
		}
		if (confirmed) {
			if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
				unsigned int i, nb, * similarbattleid;
				EnemySpellDataStruct** similarspells = enemyset.GetSimilarEnemySpells(ep, &nb, &similarbattleid);
				for (i = 0; i < nb; i++) {
					if (similarspells[i]->SetName(newstr)) {
						wxMessageDialog popup(this, HADES_STRING_GROUPEDIT_ERROR_TEXT, HADES_STRING_WARNING, wxOK | wxSTAY_ON_TOP | wxCENTRE);
						popup.ShowModal();
					} else {
						MarkDataEnemyModified(similarbattleid[i], CHUNK_TYPE_TEXT);
					}
				}
			} else {
				ep.SetName(newstr);
				MarkDataEnemyModified(*sortid, CHUNK_TYPE_TEXT);
			}
			m_enemyspellname->ChangeValue(ep.name.str);
			m_enemyspelllist->SetString(spellsel, _(ep.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
			m_enemystatdefaultattack->SetString(spellsel, _(ep.name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		}
	} else if (id == wxID_STATUS) {
		EnemySpellDataStruct& ep = eb.spell[m_enemyspelllist->GetSelection()];
		SpellStatusDialog spstat(this);
		if (spellset.GetStatusSetIndexById(ep.status) < 0)
			InsertAtId(spellset.status_set, spellset.status_set[0], ep.status);
		StatusSetStruct& statusset = spellset.GetStatusSetById(ep.status);
		if (spstat.ShowModal(this, statusset) == wxID_OK) {
			statusset.status_list = spstat.status_list;
			UpdateSpellStatusName(statusset);
			MarkDataSpellModified();
		}
	} else if (id == wxID_SCRIPT) {
		ScriptEditDialog dial(this, *enemyset.script[*sortid], SCRIPT_TYPE_BATTLE, &saveset, &eb, enemyset.text[*sortid]);
		if (dial.ShowModal() == wxID_OK) {
			*enemyset.script[*sortid] = dial.script;
			MarkDataEnemyModified(*sortid, CHUNK_TYPE_SCRIPT);
		}
	} else if (id == wxID_ENTRY) {
		ScriptEditEntryDialog dial(this, *enemyset.script[*sortid], SCRIPT_TYPE_BATTLE);
		if (dial.ShowModal() == wxID_OK) {
			dial.ApplyModifications(*enemyset.script[*sortid]);
			MarkDataEnemyModified(*sortid, CHUNK_TYPE_SCRIPT);
		}
	} else if (id == wxID_SCRIPTLINK) {
		ScriptEditLinkDialog dial(this, *enemyset.script[*sortid], *enemyset.text[*sortid]);
		if (dial.ShowModal() == wxID_OK) {
			dial.ApplyModifications(*enemyset.script[*sortid]);
			MarkDataEnemyModified(*sortid, CHUNK_TYPE_SCRIPT, true);
		}
/*	} else if (id == wxID_PRELOAD) {
		uint16_t battlescene = 0xFFFF, newscene = 0xFFFF;
		unsigned int i;
		for (i=0;i<enemyset.preload[*sortid]->amount;i++)
			if (enemyset.preload[*sortid]->data_type[i]==CHUNK_TYPE_BATTLE_SCENE) {
				battlescene = enemyset.preload[*sortid]->data_id[i];
				break;
			}
		ImageMapEditDialog dial(this,enemyset.preload[*sortid],cluster.enemy_shared_map,&cluster.global_map,saveset);
		if (dial.ShowModal()==wxID_OK) {
			enemyset.preload[*sortid]->Copy(dial.map);
			enemyset.preload[*sortid]->UpdateOffset();
			MarkDataEnemyModified(*sortid,CHUNK_TYPE_IMAGE_MAP);
			for (i=0;i<enemyset.preload[*sortid]->amount;i++)
				if (enemyset.preload[*sortid]->data_type[i]==CHUNK_TYPE_BATTLE_SCENE) {
					newscene = enemyset.preload[*sortid]->data_id[i];
					break;
				}
			if (battlescene!=newscene)
				enemyset.ChangeBattleScene(enemyset.battle_data[*sortid]->object_id,newscene);
		}*/
	} else if (id == wxID_ANIM) {
		int spellsel = m_enemyspelllist->GetSelection();
		EnemyAnimSequenceEditDialog dial(this, *enemyset.battle_data[*sortid], spellsel);
		uint32_t battlesize = enemyset.battle_data[*sortid]->size;
		if (dial.ShowModal() == wxID_OK) {
			for (unsigned int i = 0; i < enemyset.battle_data[*sortid]->sequence_code_amount[spellsel]; i++)
				delete[] enemyset.battle_data[*sortid]->sequence_code[spellsel][i].arg;
			enemyset.battle_data[*sortid]->sequence_code[spellsel] = dial.sequence_code;
			enemyset.battle_data[*sortid]->sequence_code_amount[spellsel] = dial.sequence_code_amount;
			MarkDataEnemyModified(*sortid, CHUNK_TYPE_BATTLE_DATA);
			if (GetTopWindow()->m_editsimilarenemy->IsChecked() && !DiscardSimilarEnemyUnaffected) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_NOGROUPEDIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardSimilarEnemyUnaffected = true;
			}
		} else
			enemyset.battle_data[*sortid]->SetSize(battlesize);
	} else if (id == wxID_RESOURCES) {
		int statsel = m_enemystatlist->GetSelection();
		EnemyStatDataStruct& es = eb.stat[statsel];
		BattleDataStruct& bd = *enemyset.battle_data[*sortid];
		EnemyResourceDialog dial(this);
		if (dial.ShowModal(es, bd) == wxID_OK) {
			if (GetTopWindow()->m_editsimilarenemy->IsChecked()) {
				unsigned int i, nb, * similarbattleid;
				EnemyStatDataStruct** similarstats = enemyset.GetSimilarEnemyStats(es, &nb, &similarbattleid);
				for (i = 0; i < nb; i++) {
					if (es.model != similarstats[i]->model)
						continue;
					if (dial.ApplyModifications(*similarstats[i], *similarstats[i]->parent->parent->battle_data[similarstats[i]->parent->id])) {
						if (gametype != GAME_TYPE_PSX)
							MarkDataEnemyModified(similarbattleid[i], CHUNK_TYPE_BATTLE_DATA);
					} else {
						wxMessageDialog popup(this, HADES_STRING_GROUPEDIT_ERROR_DATA, HADES_STRING_WARNING, wxOK | wxSTAY_ON_TOP | wxCENTRE);
						popup.ShowModal();
					}
					MarkDataEnemyModified(similarbattleid[i], CHUNK_TYPE_ENEMY_STATS);
				}
			} else {
				if (dial.ApplyModifications(es, bd)) {
					if (gametype != GAME_TYPE_PSX)
						MarkDataEnemyModified(*sortid, CHUNK_TYPE_BATTLE_DATA);
				} else {
					wxMessageDialog popup(this, HADES_STRING_DATA_REACH_LIMIT, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
					popup.ShowModal();
				}
				MarkDataEnemyModified(*sortid, CHUNK_TYPE_ENEMY_STATS);
			}
		}
	}
}

void CDDataStruct::OnEnemyChangeListBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADDIMMUNE) {
		m_enemystatimmunebaseint->SetValue(m_enemystatimmunebaselist->GetSelection());
	} else if (id == wxID_ADDAUTO) {
		m_enemystatautobaseint->SetValue(m_enemystatautobaselist->GetSelection());
	} else if (id == wxID_ADDINITIAL) {
		m_enemystatinitialbaseint->SetValue(m_enemystatinitialbaselist->GetSelection());
	}
}

bool DiscardEnemySizeLimit = false;
bool DiscardEnemyNumberLimit = false;
void CDDataStruct::OnEnemyStatRightClick(wxMouseEvent& event) {
	int newsel = m_enemystatlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_enemystatlist->SetSelection(newsel);
		DisplayEnemyStat(m_enemylist->GetSelection(), newsel);
		enemystatmenupaste->Enable(copyenemystat_battleid >= 0);
		if (gametype != GAME_TYPE_PSX)
			m_enemystatlist->PopupMenu(enemystatmenu);
	}
}

void CDDataStruct::OnEnemySpellRightClick(wxMouseEvent& event) {
	int newsel = m_enemyspelllist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_enemyspelllist->SetSelection(newsel);
		DisplayEnemySpell(m_enemylist->GetSelection(), newsel);
		enemyspellmenupaste->Enable(copyenemyspell_battleid >= 0);
		m_enemyspelllist->PopupMenu(enemyspellmenu);
	}
}

void CDDataStruct::OnEnemyGroupRightClick(wxMouseEvent& event) {
	int newsel = m_enemygrouplist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_enemygrouplist->SetSelection(newsel);
		DisplayEnemyGroup(m_enemylist->GetSelection(), newsel);
		m_enemygrouplist->PopupMenu(enemygroupmenu);
	}
}

void CDDataStruct::OnEnemyTextRightClick(wxMouseEvent& event) {
	int newsel = m_enemytextlist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		m_enemytextlist->SetSelection(newsel);
		int enmysel = m_enemylist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(enmysel);
		EnemyDataStruct& eb = *enemyset.battle[*sortid];
		TextDataStruct& td = *enemyset.text[*sortid];
		RightClickTextSelection = &td.text[eb.stat_amount + eb.spell_amount + newsel];
	} else {
		RightClickTextSelection = NULL;
	}
	textmenuadd->Enable(true);
	textmenuremove->Enable(newsel != wxNOT_FOUND);
	textmenucopy->Enable(newsel != wxNOT_FOUND);
	textmenupaste->Enable(newsel != wxNOT_FOUND && copystring != NULL);
	m_enemytextlist->PopupMenu(textmenu);
}

void CDDataStruct::OnEnemyStatRightClickMenu(wxCommandEvent& event) {
	int sel = m_enemylist->GetSelection();
	int id = event.GetId();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int objid = m_enemystatlist->GetSelection();
	int newsel = -1;
	if (id == wxID_COPY) {
		copyenemystat_battleid = *sortid;
		copyenemystat_statid = objid;
		return;
	} else if (id == wxID_PASTE) {
		newsel = eb.stat_amount;
		if (eb.AddStat(&enemyset.battle[copyenemystat_battleid]->stat[copyenemystat_statid])) {
			newsel = -1;
			if (!DiscardEnemySizeLimit) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_DATA_REACH_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardEnemySizeLimit = true;
			}
		} else if (eb.stat_amount >= 4 && (gametype != GAME_TYPE_STEAM || config.dll_usage == 0)) {
			if (!DiscardEnemyNumberLimit) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_STAT_WARN_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardEnemyNumberLimit = true;
			}
		}
	} else if (id == wxID_REMOVE && eb.stat_amount > 1) {
		eb.RemoveStat(objid);
		newsel = min(eb.stat_amount - 1, objid);
		if (*sortid == copyenemystat_battleid) {
			if (objid == copyenemystat_statid) {
				copyenemystat_battleid = -1;
				copyenemystat_statid = -1;
			} else if (objid < copyenemystat_statid) {
				copyenemystat_statid--;
			}
		}
	}
	if (newsel >= 0) {
		DisplayEnemy(m_enemylist->GetSelection());
		m_enemystatlist->SetSelection(newsel);
		DisplayEnemyStat(m_enemylist->GetSelection(), newsel);
	}
	m_enemylist->SetString(sel, GetEnemyBattleName(*sortid));
	UpdateEnemyName(*sortid);
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_BATTLE_DATA);
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_ENEMY_STATS);
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_TEXT);
}

void CDDataStruct::OnEnemySpellRightClickMenu(wxCommandEvent& event) {
	int sel = m_enemylist->GetSelection();
	int id = event.GetId();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int objid = m_enemyspelllist->GetSelection();
	int newsel = -1;
	if (id == wxID_COPY) {
		copyenemyspell_battleid = *sortid;
		copyenemyspell_spellid = objid;
		return;
	} else if (id == wxID_PASTE) {
		newsel = eb.spell_amount;
		if (copyenemyspell_battleid >= (int)enemyset.battle.size() || copyenemyspell_spellid >= (int)enemyset.battle[copyenemyspell_battleid]->spell.size())
			return; // Something changed, cancel pasting
		if (eb.AddSpell(&enemyset.battle[copyenemyspell_battleid]->spell[copyenemyspell_spellid])) {
			newsel = -1;
			if (!DiscardEnemySizeLimit) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_DATA_REACH_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardEnemySizeLimit = true;
			}
		} else if (eb.spell_amount >= 19 && (gametype != GAME_TYPE_STEAM || config.dll_usage == 0)) {
			if (!DiscardEnemyNumberLimit) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_SPELL_WARN_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardEnemyNumberLimit = true;
			}
		}
	} else if (id == wxID_REMOVE && eb.spell_amount > 1) {
		eb.RemoveSpell(objid);
		newsel = min(eb.spell_amount - 1, objid);
		if (*sortid == copyenemyspell_battleid) {
			if (objid == copyenemyspell_spellid) {
				copyenemyspell_battleid = -1;
				copyenemyspell_spellid = -1;
			} else if (objid < copyenemyspell_spellid) {
				copyenemyspell_spellid--;
			}
		}
	}
	if (newsel >= 0) {
		DisplayEnemy(m_enemylist->GetSelection());
		m_enemyspelllist->SetSelection(newsel);
		DisplayEnemySpell(m_enemylist->GetSelection(), newsel);
	}
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_BATTLE_DATA);
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_ENEMY_STATS);
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_TEXT);
}

void CDDataStruct::OnEnemyGroupRightClickMenu(wxCommandEvent& event) {
	int sel = m_enemylist->GetSelection();
	int id = event.GetId();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	int objid = m_enemygrouplist->GetSelection();
	unsigned int i;
	int newsel = -1;
	if (id == wxID_ADD) {
		newsel = eb.group_amount;
		if (eb.AddGroup()) {
			newsel = -1;
			if (!DiscardEnemySizeLimit) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_DATA_REACH_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardEnemySizeLimit = true;
			}
		}
	} else if (id == wxID_REMOVE && eb.group_amount > 1) {
		eb.RemoveGroup(objid);
		newsel = min(eb.group_amount - 1, objid);
	}
	if (newsel >= 0) {
		m_enemygrouplist->Clear();
		for (i = 0; i < eb.group_amount; i++) {
			stringstream groupname;
			groupname << "Group " << i + 1 << ends;
			m_enemygrouplist->Append(_(groupname.str()));
		}
		m_enemygrouplist->SetSelection(newsel);
		DisplayEnemyGroup(m_enemylist->GetSelection(), newsel);
	}
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_ENEMY_STATS);
}

void CDDataStruct::OnEnemyTextRightClickMenu(wxCommandEvent& event) {
	int sel = m_enemylist->GetSelection();
	int id = event.GetId();
	unsigned int* sortid = (unsigned int*)m_enemylist->GetClientData(sel);
	EnemyDataStruct& eb = *enemyset.battle[*sortid];
	TextDataStruct& td = *enemyset.text[*sortid];
	int objid = m_enemytextlist->GetSelection();
	unsigned int i;
	int newsel = -1;
	if (id == wxID_ADD) {
		newsel = td.amount;
		FF9String newstr;
		newstr.CreateEmpty();
		if (gametype != GAME_TYPE_PSX)
			newstr.SetValue(L"[STRT=0,1]");
		if (td.AddText(td.amount, newstr)) {
			newsel = -1;
			TextReachLimit();
		}
	} else if (id == wxID_REMOVE && objid != wxNOT_FOUND) {
		td.RemoveText(eb.stat_amount + eb.spell_amount + objid);
		newsel = min(td.amount - eb.stat_amount - eb.spell_amount - 1, objid);
		if (newsel < 0)
			m_enemytextlist->Clear();
	} else if (id == wxID_PASTE) {
		newsel = objid;
	}
	if (newsel >= 0) {
		m_enemytextlist->Clear();
		for (i = eb.stat_amount + eb.spell_amount; i < td.amount; i++)
			m_enemytextlist->Append(_(td.text[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemytextlist->SetSelection(newsel);
	}
	MarkDataEnemyModified(*sortid, CHUNK_TYPE_TEXT);
}

//=============================//
//           Texts             //
//=============================//

#define MACRO_TEXT_DISPLAYTEXTURE(IMG,PREVWIN,PREVBMP) \
	img.Rescale(IMG.GetWidth()*2,IMG.GetHeight()*2); \
	wxBitmap bmp(IMG); \
	if (bmp.IsOk()) { \
		PREVWIN->SetVirtualSize(bmp.GetWidth(),bmp.GetHeight()); \
		wxClientDC dc(PREVWIN); \
		dc.Clear(); \
		PREVWIN->DoPrepareDC(dc); \
		dc.DrawBitmap(bmp,0,0); \
		*PREVBMP = bmp; \
	}

void CDDataStruct::DisplayText(int textid) {
	unsigned int* sortid = (unsigned int*)m_textlist->GetClientData(textid);
	TextDataStruct* td = textset.text_data[*sortid];
	unsigned int i,tamount;
	tamount = td ? td->amount : 0;
	m_textdatalist->Clear();
	for (i=0;i<tamount;i++)
		m_textdatalist->Append(_(td->text[i].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
	m_textcharmaplist->Clear();
	for (i=0;i<textset.tim_amount[*sortid];i++) {
		wstringstream buffer;
		buffer << "Picture " << i+1 << ends;
		m_textcharmaplist->Append(_(buffer.str()));
	}
	m_textdatalistlabel->Show(td!=NULL);
	m_textdatalist->Show(td!=NULL);
	m_textexportlabel->Show(td!=NULL);
	m_textexport->Show(td!=NULL);
	m_textcharmaptexturelistlabel->Show(textset.tim_amount[*sortid]>0);
	m_textmanagecharmapbutton->Show(textset.tim_amount[*sortid]>0);
	m_textcharmappallabel->Show(textset.tim_amount[*sortid]>0);
	m_textcharmaplist->Show(textset.tim_amount[*sortid]>0);
	m_textcharmappalchoice->Show(textset.tim_amount[*sortid]>0);
	m_textcharmapexportlabel->Show(textset.tim_amount[*sortid]>0);
	m_textcharmapexport->Show(textset.tim_amount[*sortid]>0);
	m_textcharmapwindowlabel->Show(textset.tim_amount[*sortid]>0);
	m_textcharmapwindow->Show(textset.tim_amount[*sortid]>0);
	if (textset.tim_amount[*sortid]>0) {
		TIMImageDataStruct& tdtex = textset.chartim[*sortid][0];
		m_textcharmappalchoice->Clear();
		for (i=0;i<tdtex.pal_height;i++) {
			wstringstream buffer;
			buffer << L"Palette " << i+1 << ends;
			m_textcharmappalchoice->Append(_(buffer.str()));
		}
		m_textcharmaplist->SetSelection(0);
		m_textcharmappalchoice->SetSelection(0);
		wxImage img = ConvertFullTIMToImage(tdtex,NULL,0,false);
		MACRO_TEXT_DISPLAYTEXTURE(img,m_textcharmapwindow,&chartexpreview)
	}
	m_textscrolledwindow->Layout();
	m_textscrolledwindow->GetParent()->GetSizer()->Layout();
	m_textscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxText(wxCommandEvent& event) {
	DisplayText(m_textlist->GetSelection());
}

void CDDataStruct::OnTextEditText(wxCommandEvent& event) {
	bool istext = event.GetId() == wxID_TEXT;
	if (!istext && event.GetId() != wxID_WORLD)
		return;
	wxListBox* itemlist = istext ? m_textlist : m_worldlist;
	wxListBox* subitemlist = istext ? m_textdatalist : m_worldtextlist;
	vector<TextDataStruct*> strlist = istext ? textset.text_data : worldset.text_data;
	void (CDDataStruct::*markmodified)(unsigned int, Chunk_Type, unsigned int) = istext ? &CDDataStruct::MarkDataTextModified : &CDDataStruct::MarkDataWorldMapModified;
	CharmapDataStruct* chmap = NULL, * chmapext = NULL;
	int sel = itemlist->GetSelection();
	int textsel = subitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)itemlist->GetClientData(sel);
	TextDataStruct& td = *strlist[*sortid];
	FF9String& text = td.text[textsel];
	int sizex = -1, sizey = -1;
	uint16_t formatamount = 0;
	TextFormatStruct* formatdata = NULL;
	if (gametype==GAME_TYPE_PSX) {
		chmap = textset.charmap[textset.main_charmap_index];
		if (event.GetId()==wxID_TEXT)
			chmapext = textset.charmap[*sortid];
		else if (event.GetId()==wxID_WORLD)
			chmapext = worldset.charmap[*sortid];
		sizex = td.size_x[textsel] & 0xFFF;
		sizey = td.size_y[textsel] & 0x7F;
		if (config.language & LANGUAGE_VERSION_JAPAN)
			sizey = -1;
		formatamount = td.format_amount[textsel];
		formatdata = td.format_data[textsel];
	}
	bool modified = false;
	if (gametype==GAME_TYPE_PSX) {
		TextEditDialog ted(this,text,td.GetExtraSize(),TEXT_STYLE_DEFAULT,chmap,chmapext,sizex,sizey,formatamount,formatdata);
		if (ted.ShowModal()==wxID_OK) {
			td.SetText(textsel,ted.text);
			td.SetDialogBoxSize(textsel,ted.m_sizex->GetValue(),ted.m_sizey->GetValue(),!(config.language & LANGUAGE_VERSION_JAPAN));
			unsigned int i,j = 0;
			unsigned int famount = td.format_amount[textsel];
			for (i=0;i<famount;i++) {
				if (ted.format_removed[i])
					td.RemoveFormatCode(textsel,j);
				else
					j++;
			}
//			td.format_amount[textsel] = ted.format_amount;
			modified = true;
		}
	} else {
		TextSteamEditDialog ted(this,&ffuiset,text);
		if (ted.ShowModal()==wxID_OK) {
			MACRO_MULTILANG_INCREASE_COUNTER(text,ted.text,td.text,textsel,L"[STRT=0,1]")
			td.SetText(textsel,ted.text);
			modified = true;
		}
	}
	if (modified) {
		subitemlist->SetString(textsel,_(text.GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
		(this->*markmodified)(*sortid,CHUNK_TYPE_TEXT,0);
	}
}

TextExportDialog* TheTextExportDialog;
void CDDataStruct::OnTextExportText(wxCommandEvent& event) {
	if (!TheTextExportDialog)
		TheTextExportDialog = new TextExportDialog(GetTopWindow());
	bool istext = event.GetId() == wxID_TEXT;
	if (!istext && event.GetId() != wxID_WORLD)
		return;
	wxListBox* itemlist = istext ? m_textlist : m_worldlist;
	vector<TextDataStruct*> strlist = istext ? textset.text_data : worldset.text_data;
	vector<wstring> namelist = istext ? textset.name : worldset.name;
	int sel = itemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)itemlist->GetClientData(sel);
	if (strlist[*sortid]==NULL)
		return;
	if (TheTextExportDialog->ShowModal() == wxID_OK) {
		wxFile txtfile(TheTextExportDialog->m_filepicker->GetPath(), wxFile::write);
		if (!txtfile.IsOpened()) {
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE, TheTextExportDialog->m_filepicker->GetPath());
			return;
		}
		unsigned int i;
		wchar_t tmpstr[2000];
		wchar_t tmpheader[2000];
		for (i = 0; i < namelist[*sortid].length(); i++)
			tmpheader[i] = namelist[*sortid][i];
		tmpheader[namelist[*sortid].length()] = 0;
		wsprintf(tmpstr, TheTextExportDialog->m_textheader->GetValue().wc_str(), tmpheader);
		txtfile.Write(_(tmpstr) + _(L'\n'));
		TextDataStruct& td = *strlist[*sortid];
		for (i = 0; i < td.amount; i++) {
			wsprintf(tmpstr, TheTextExportDialog->m_textseparator->GetValue().wc_str(), i);
			txtfile.Write(_(tmpstr) + _(L'\n') + _(td.text[i].GetStr(TheTextExportDialog->m_strtype->GetSelection())) + _(L'\n'));
		}
		txtfile.Close();
		wxMessageDialog popupsuccess(this, HADES_STRING_TXT_SAVE_SUCCESS, HADES_STRING_SUCCESS, wxOK | wxCENTRE);
		popupsuccess.ShowModal();
	}
}

void CDDataStruct::OnTextCharmapListSelection(wxCommandEvent& event) {
	bool istext = event.GetId() == wxID_TEXT;
	if (!istext && event.GetId() != wxID_WORLD)
		return;
	wxListBox* itemlist = istext ? m_textlist : m_worldlist;
	wxListBox* subitemlist = istext ? m_textcharmaplist : m_worldtextcharmaplist;
	wxChoice* palchoice = istext ? m_textcharmappalchoice : m_worldtextcharmappalchoice;
	wxScrolledWindow* previewwindow = istext ? m_textcharmapwindow : m_worldtextcharmapwindow;
	wxBitmap* previewbmp = istext ? &chartexpreview : &worldchartexpreview;
	vector<TIMImageDataStruct*> timlist = istext ? textset.chartim : worldset.chartim;
	int sel = itemlist->GetSelection();
	int texsel = subitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)itemlist->GetClientData(sel);
	unsigned int i;
	TIMImageDataStruct& tdtex = timlist[*sortid][texsel];
	palchoice->Clear();
	for (i=0;i<tdtex.pal_height;i++) {
		wstringstream buffer;
		buffer << L"Palette " << i+1 << ends;
		palchoice->Append(_(buffer.str()));
	}
	palchoice->SetSelection(0);
	wxImage img = ConvertFullTIMToImage(tdtex,NULL,0,false);
	MACRO_TEXT_DISPLAYTEXTURE(img,previewwindow,previewbmp)
}

void CDDataStruct::OnTextCharmapPaletteChoice(wxCommandEvent& event) {
	bool istext = event.GetId() == wxID_TEXT;
	if (!istext && event.GetId() != wxID_WORLD)
		return;
	wxListBox* itemlist = istext ? m_textlist : m_worldlist;
	wxListBox* subitemlist = istext ? m_textcharmaplist : m_worldtextcharmaplist;
	wxChoice* palchoice = istext ? m_textcharmappalchoice : m_worldtextcharmappalchoice;
	wxScrolledWindow* previewwindow = istext ? m_textcharmapwindow : m_worldtextcharmapwindow;
	wxBitmap* previewbmp = istext ? &chartexpreview : &worldchartexpreview;
	vector<TIMImageDataStruct*> timlist = istext ? textset.chartim : worldset.chartim;
	int sel = itemlist->GetSelection();
	int texsel = subitemlist->GetSelection();
	if (texsel==wxNOT_FOUND)
		return;
	unsigned int* sortid = (unsigned int*)itemlist->GetClientData(sel);
	TIMImageDataStruct& tdtex = timlist[*sortid][texsel];
	wxImage img = ConvertFullTIMToImage(tdtex,NULL,palchoice->GetSelection(),false);
	MACRO_TEXT_DISPLAYTEXTURE(img,previewwindow,previewbmp)
//	previewwindow->Layout(); // For some reasons, m_textcharmappalchoice has difficulties to validate its selection...
//	previewwindow->Refresh();
}

CharmapTextureExportWindow* TheCharmapTextureExportDialog;
void CDDataStruct::OnTextExportCharmap(wxCommandEvent& event) {
	bool istext = event.GetId() == wxID_TEXT;
	if (!istext && event.GetId() != wxID_WORLD)
		return;
	wxListBox* itemlist = istext ? m_textlist : m_worldlist;
	wxListBox* subitemlist = istext ? m_textcharmaplist : m_worldtextcharmaplist;
	wxChoice* palchoice = istext ? m_textcharmappalchoice : m_worldtextcharmappalchoice;
	vector<TIMImageDataStruct*> timlist = istext ? textset.chartim : worldset.chartim;
	if (!TheCharmapTextureExportDialog)
		TheCharmapTextureExportDialog = new CharmapTextureExportWindow(GetTopWindow());
	int sel = itemlist->GetSelection();
	int texsel = subitemlist->GetSelection();
	if (texsel==wxNOT_FOUND)
		return;
	unsigned int* sortid = (unsigned int*)itemlist->GetClientData(sel);
	if (TheCharmapTextureExportDialog->ShowModal()==wxID_OK) {
		TIMImageDataStruct& im = timlist[*sortid][texsel];
		int res = im.Export(TheCharmapTextureExportDialog->m_filepicker->GetPath().mb_str(),true,0,NULL,palchoice->GetSelection(),false);
		if (res==1)
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE, TheCharmapTextureExportDialog->m_filepicker->GetPath().c_str());
		else if (res==2)
			wxLogError(HADES_STRING_ERROR_UNKNOWN);
		else {
			wxMessageDialog popupsuccess(this,HADES_STRING_TEXTURE_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
		}
	}
}

void CDDataStruct::OnTextManageCharmap(wxCommandEvent& event) {
	bool istext = event.GetId() == wxID_TEXT;
	if (!istext && event.GetId() != wxID_WORLD)
		return;
	wxListBox* itemlist = istext ? m_textlist : m_worldlist;
	wxListBox* subitemlist = istext ? m_textcharmaplist : m_worldtextcharmaplist;
	vector<CharmapDataStruct*> chlist = istext ? textset.charmap : worldset.charmap;
	vector<TIMImageDataStruct*> timlist = istext ? textset.chartim : worldset.chartim;
	wxScrolledWindow* previewwindow = istext ? m_textcharmapwindow : m_worldtextcharmapwindow;
	wxBitmap* previewbmp = istext ? &chartexpreview : &worldchartexpreview;
	void (CDDataStruct::*markmodified)(unsigned int, Chunk_Type, unsigned int) = istext ? &CDDataStruct::MarkDataTextModified : &CDDataStruct::MarkDataWorldMapModified;
	int sel = itemlist->GetSelection();
	int texsel = subitemlist->GetSelection();
	unsigned int* sortid = (unsigned int*)itemlist->GetClientData(sel);
	if (texsel==wxNOT_FOUND)
		return;
	unsigned int i;
	CharmapDataStruct& cm = *chlist[*sortid];
	TIMImageDataStruct& tdtex = timlist[*sortid][texsel];
	unsigned int palcount = tdtex.pal_height;
	uint32_t** pal = new uint32_t*[palcount];
	for (i=0;i<palcount;i++) {
		pal[i] = new uint32_t[256];
		uint32_t* palbuff = tdtex.ConvertAsPalette(i,false); // Temp array
		memcpy(pal[i],palbuff,256*sizeof(uint32_t));
	}
	unsigned int texcount = 0;
	uint16_t* texx = NULL;
	uint16_t* texy = NULL;
	uint16_t* texw = NULL;
	uint8_t* texp = NULL;
	if (texsel==0) {
		texcount = cm.amount;
		texx = new uint16_t[texcount];
		texy = new uint16_t[texcount];
		texw = new uint16_t[texcount];
		texp = new uint8_t[texcount];
		for (i=0;i<texcount;i++) {
			texx[i] = cm.img_x[i]/2;
			texy[i] = cm.img_y[i]-(tdtex.pos_y & 0xFF);
			texw[i] = (cm.img_width[i] & 0xF)/2;
			texp[i] = (cm.img_width[i] & 0x40) ? 1 : 0;
		}
	}
	ManageTextureDialog dial(this,tdtex,texcount,0,0,-7,13,true,texx,texy,3,1,texw,NULL,3,1,palcount,pal,false,tdtex.pal_width,texsel==0,texp);
	if (dial.ShowModal()==wxID_OK) {
		uint32_t newsize = dial.tex_amount*4;
		uint32_t oldsize = texcount*4;
		int id = wxID_OK;
		while (id == wxID_OK && newsize>oldsize+tdtex.GetExtraSize()) {
			LogStruct log;
			wstringstream buffer;
			buffer << L" - Not enough space : the new charmap takes " << newsize << L" bytes for " << oldsize+tdtex.GetExtraSize() << L" bytes available." << endl;
			log.AddError(buffer.str());
			LogDialog lw(this,log);
			lw.wxDialog::ShowModal();
			id = dial.ShowModal();
			newsize = dial.tex_amount*4;
		}
		if (id == wxID_OK) {
			if (tdtex.img_size!=dial.tim.img_size) {
				tdtex.SetSize(tdtex.size+dial.tim.img_size-tdtex.img_size);
				tdtex.img_size = dial.tim.img_size;
			}
			tdtex.pos_x = dial.tim.pos_x;
			tdtex.pos_y = dial.tim.pos_y;
			tdtex.width = dial.tim.width;
			tdtex.height = dial.tim.height;
			delete[] tdtex.pixel_value;
			tdtex.pixel_value = dial.tim.pixel_value;
			if (texsel==0) {
				cm.amount = dial.tex_amount;
				uint8_t* newimgx = new uint8_t[dial.tex_amount];
				uint8_t* newimgy = new uint8_t[dial.tex_amount];
				uint8_t* newimgwidth = new uint8_t[dial.tex_amount];
				uint8_t* newwidth = new uint8_t[dial.tex_amount];
				for (i=0;i<dial.tex_amount;i++) {
					newimgx[i] = dial.tex_pos_x[i]*2;
					newimgy[i] = dial.tex_pos_y[i]+(tdtex.pos_y & 0xFF);
					newimgwidth[i] = dial.tex_width[i]*2 | (dial.char_tex_pos[i] ? 0x40 : 0);
					if (dial.tex_width[i]>0)
						newwidth[i] = dial.tex_width[i]*0x20;
					else
						newwidth[i] = 0x30;
				}
				delete[] cm.img_x;
				delete[] cm.img_y;
				delete[] cm.img_width;
				delete[] cm.width;
				cm.img_x = newimgx;
				cm.img_y = newimgy;
				cm.img_width = newimgwidth;
				cm.width = newwidth;
				(this->*markmodified)(*sortid,CHUNK_TYPE_CHARMAP,0);
			}
			wxImage img = ConvertFullTIMToImage(tdtex,NULL,0,false);
			MACRO_TEXT_DISPLAYTEXTURE(img,previewwindow,previewbmp)
			(this->*markmodified)(*sortid,CHUNK_TYPE_TIM,texsel);
		}
	}
}

void CDDataStruct::OnTextRightClick(wxMouseEvent& event) {
	if (event.GetId() == wxID_TEXT) {
		int newsel = m_textdatalist->HitTest(event.GetPosition());
		if (newsel != wxNOT_FOUND) {
			m_textdatalist->SetSelection(newsel);
			int textsel = m_textlist->GetSelection();
			unsigned int* sortid = (unsigned int*)m_textlist->GetClientData(textsel);
			TextDataStruct& td = *textset.text_data[*sortid];
			RightClickTextSelection = &td.text[newsel];
		} else {
			RightClickTextSelection = NULL;
		}
		textmenuadd->Enable(true);
		textmenuremove->Enable(newsel != wxNOT_FOUND);
		textmenucopy->Enable(newsel != wxNOT_FOUND);
		textmenupaste->Enable(newsel != wxNOT_FOUND && copystring != NULL);
		m_textdatalist->PopupMenu(textmenu);
	} else if (event.GetId() == wxID_WORLD) {
		int newsel = m_worldtextlist->HitTest(event.GetPosition());
		if (newsel != wxNOT_FOUND) {
			m_worldtextlist->SetSelection(newsel);
			int worldsel = m_worldlist->GetSelection();
			unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(worldsel);
			TextDataStruct& td = *worldset.text_data[*sortid];
			RightClickTextSelection = &td.text[newsel];
		} else {
			RightClickTextSelection = NULL;
		}
		textmenuadd->Enable(true);
		textmenuremove->Enable(newsel != wxNOT_FOUND);
		textmenucopy->Enable(newsel != wxNOT_FOUND);
		textmenupaste->Enable(newsel != wxNOT_FOUND && copystring != NULL);
		m_worldtextlist->PopupMenu(textmenu);
	}
}

void CDDataStruct::OnTextRightClickMenu(wxCommandEvent& event) {
	int sel = m_textlist->GetSelection();
	int id = event.GetId();
	unsigned int* sortid = (unsigned int*)m_textlist->GetClientData(sel);
	TextDataStruct& td = *textset.text_data[*sortid];
	int objid = m_textdatalist->GetSelection();
	unsigned int i;
	int newsel = -1;
	if (id == wxID_ADD) {
		newsel = td.amount;
		FF9String newstr;
		newstr.CreateEmpty();
		if (gametype != GAME_TYPE_PSX)
			newstr.SetValue(L"[STRT=0,1]");
		if (td.AddText(td.amount, newstr)) {
			newsel = -1;
			TextReachLimit();
		}
	} else if (id == wxID_REMOVE && objid != wxNOT_FOUND) {
		td.RemoveText(objid);
		newsel = min(td.amount - 1, objid);
	} else if (id == wxID_PASTE) {
		newsel = objid;
	}
	if (newsel >= 0) {
		m_textdatalist->Clear();
		for (i = 0; i < td.amount; i++)
			m_textdatalist->Append(_(td.text[i].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0, 100)));
		m_textdatalist->SetSelection(newsel);
	}
	MarkDataTextModified(*sortid, CHUNK_TYPE_TEXT);
}

void CDDataStruct::OnTextCharmapPaint(wxPaintEvent &event) {
	wxScrolledWindow* previewwindow = NULL;
	wxBitmap* previewbmp = NULL;
	if (event.GetId()==wxID_TEXT) {
		previewwindow = m_textcharmapwindow;
		previewbmp = &chartexpreview;
	} else if (event.GetId()==wxID_WORLD) {
		previewwindow = m_worldtextcharmapwindow;
		previewbmp = &worldchartexpreview;
	}
	if (previewbmp->IsOk()) {
		wxPaintDC dc(previewwindow);
		previewwindow->DoPrepareDC(dc);
		dc.DrawBitmap(*previewbmp,0,0);
	}
}

//=============================//
//           Fields            //
//=============================//

#define MACRO_FIELD_DISPLAY_BACKGROUND(FIELDID,BGID) \
	uint32_t* imgdata = fieldset.background_data[FIELDID]->ConvertAsImageAccurate(BGID); \
	wxBitmap bmp = wxBitmap(ConvertDataToImage(imgdata,fieldset.background_data[FIELDID]->camera[BGID].width,fieldset.background_data[FIELDID]->camera[BGID].height)); \
	delete[] imgdata; \
	if (bmp.IsOk()) { \
		m_fieldtexturepreview->SetVirtualSize(bmp.GetWidth(),bmp.GetHeight()); \
		wxClientDC dc(m_fieldtexturepreview); \
		dc.Clear(); \
		m_fieldtexturepreview->DoPrepareDC(dc); \
		dc.DrawBitmap(bmp,0,0); \
		fieldtexpreview = bmp; \
	}

void CDDataStruct::DisplayField(int fieldid) {
	unsigned int* sortid = (unsigned int*)m_fieldlist->GetClientData(fieldid);
	fieldselection = *sortid;
	unsigned int i;
	if (gametype != GAME_TYPE_PSX && fieldset.tim_data[fieldselection] != NULL) {
		fstream ftmp;
		fieldset.tim_data[*sortid]->Read(ftmp);
	}
	m_fieldname->ChangeValue(_(fieldset.script_data[fieldselection]->name.str));
	m_fieldpreload->Enable(fieldset.preload[fieldselection] != NULL);
	m_fieldscriptlink->Enable(fieldset.script_data[fieldselection]->multi_lang_script != NULL);
	m_fieldtexturechoice->Enable(fieldset.background_data[fieldselection] != NULL);
	m_fieldtexturemanage->Enable(fieldset.background_data[fieldselection] != NULL);
	m_fieldtexturechoice->Clear();
	if (fieldset.background_data[fieldselection] != NULL) {
		for (i = 0; i < fieldset.background_data[fieldselection]->camera_amount; i++) {
			wstringstream buffer;
			buffer << "Background " << i + 1 << ends;
			m_fieldtexturechoice->Append(_(buffer.str()));
		}
		m_fieldtexturechoice->SetSelection(0);
		MACRO_FIELD_DISPLAY_BACKGROUND(fieldselection, 0)
	} else {
		wxClientDC dc(m_fieldtexturepreview);
		dc.Clear();
		m_fieldtexturepreview->DoPrepareDC(dc);
		fieldtexpreview = wxNullBitmap;
	}
	m_fieldeditwalk->Enable(fieldset.walkmesh[fieldselection] != NULL);
	m_fieldexportwalk->Enable(fieldset.walkmesh[fieldselection] != NULL);
	m_fieldimportwalk->Enable(fieldset.walkmesh[fieldselection] != NULL);
	m_fieldscrolledwindow->Layout();
	m_fieldscrolledwindow->GetParent()->GetSizer()->Layout();
	m_fieldscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxField(wxCommandEvent& event) {
	if (gametype!=GAME_TYPE_PSX && fieldset.tim_data[fieldselection]!=NULL)
		fieldset.tim_data[fieldselection]->Flush();
	DisplayField(m_fieldlist->GetSelection());
}

void CDDataStruct::OnFieldChangeName(wxCommandEvent& event) {
	unsigned int sel = m_fieldlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_fieldlist->GetClientData(sel);
	ScriptDataStruct* sc = fieldset.script_data[*sortid];
	if (fieldset.SetFieldName(*sortid,m_fieldname->GetValue().ToStdWstring())) {
		wxTextPos ip = m_fieldname->GetInsertionPoint();
		m_fieldname->ChangeValue(_(sc->name.str));
		m_fieldname->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_fieldlist->SetString(sel,GetFieldName(*sortid));
		MarkDataFieldModified(*sortid,CHUNK_TYPE_SCRIPT);
	}
}

void CDDataStruct::OnFieldChangeChoice(wxCommandEvent& event) {
	unsigned int sel = m_fieldlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_fieldlist->GetClientData(sel);
	ScriptDataStruct* sc = fieldset.script_data[*sortid];
//	TIMImageDataStruct* tdtex = fieldset.background_data[*sortid];
	int id = event.GetId();
	if (id == wxID_TEXTURE) {
		MACRO_FIELD_DISPLAY_BACKGROUND(*sortid,event.GetSelection())
	}
}

WalkmeshExportWindow* TheWalkmeshExportWindow;
WalkmeshImportWindow* TheWalkmeshImportWindow;
void CDDataStruct::OnFieldChangeButton(wxCommandEvent& event) {
	unsigned int sel = m_fieldlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_fieldlist->GetClientData(sel);
	ScriptDataStruct* sc = fieldset.script_data[*sortid];
	int id = event.GetId();
	if (id == wxID_NAME) {
		if (gametype==GAME_TYPE_PSX) {
			CharmapDataStruct* chmap = NULL;
			if (saveset.sectionloaded[DATA_SECTION_TEXT])
				chmap = textset.charmap[textset.main_charmap_index];
			TextEditDialog ted(this,sc->name,SCRIPT_NAME_MAX_LENGTH-sc->name.length,TEXT_STYLE_DEFAULT,chmap);
			if (ted.ShowModal()==wxID_OK) {
				fieldset.SetFieldName(*sortid,ted.text);
				m_fieldname->SetValue(sc->name.str);
			}
		} else {
			TextSteamEditDialog ted(this,&ffuiset,sc->name);
			if (ted.ShowModal()==wxID_OK) {
				fieldset.SetFieldName(*sortid,ted.text);
				m_fieldname->SetValue(sc->name.str);
			}
		}
	} else if (id == wxID_SCRIPT) {
		TextDataStruct* textobj = fieldset.related_text[*sortid];
//fstream fout("aaaa.txt",ios::app|ios::out); fout << "Go" << endl; fout.close();
		ScriptEditDialog dial(this,*sc,SCRIPT_TYPE_FIELD,&saveset,NULL,textobj);
//fout.open("aaaa.txt",ios::app|ios::out); fout << "Step" << endl; fout.close();
		if (dial.ShowModal()==wxID_OK) {
			*sc = dial.script;
			MarkDataFieldModified(*sortid,CHUNK_TYPE_SCRIPT);
		}
/* DEBUG
wxBitmap walkbmp(10000,10000);
wxMemoryDC walkimgdc;
walkimgdc.SelectObject(walkbmp);
walkimgdc.SetPen(*wxBLACK_PEN);
walkimgdc.SetBrush(*wxGREEN_BRUSH);
FieldWalkmeshDataStruct* wk = fieldset.walkmesh[*sortid];
wxPoint vert[50];
for (unsigned int i=0;i<wk->triangle_amount;i++) {
	vert[0].x = wk->vertice_x[wk->triangle_vertice1[i]]+wk->walkpath_offsetx[wk->triangle_walkpath[i]];
	vert[1].x = wk->vertice_x[wk->triangle_vertice2[i]]+wk->walkpath_offsetx[wk->triangle_walkpath[i]];
	vert[2].x = wk->vertice_x[wk->triangle_vertice3[i]]+wk->walkpath_offsetx[wk->triangle_walkpath[i]];
	vert[0].y = 10000-wk->vertice_y[wk->triangle_vertice1[i]]-wk->walkpath_offsety[wk->triangle_walkpath[i]];
	vert[1].y = 10000-wk->vertice_y[wk->triangle_vertice2[i]]-wk->walkpath_offsety[wk->triangle_walkpath[i]];
	vert[2].y = 10000-wk->vertice_y[wk->triangle_vertice3[i]]-wk->walkpath_offsety[wk->triangle_walkpath[i]];
	if (wk->triangle_unk1[i]!=0x2001) walkimgdc.SetBrush(*wxRED_BRUSH);
	else walkimgdc.SetBrush(*wxGREEN_BRUSH);
	walkimgdc.DrawPolygon(3,vert);
}
walkimgdc.SetPen(*wxWHITE_PEN);
for (unsigned int i=0;i<wk->triangle_amount;i++) {
	wxPoint meanpoint(wk->triangle_centerx[i]+wk->walkpath_offsetx[wk->triangle_walkpath[i]],10000-wk->triangle_centery[i]-wk->walkpath_offsety[wk->triangle_walkpath[i]]);
	walkimgdc.DrawLabel(wxString::Format(wxT("%u\nwalkpath %u\nunk9 %u\nunk10 %u"),i,wk->triangle_walkpath[i],wk->triangle_unk9[i],wk->triangle_unk10[i]),wxRect(meanpoint,wxSize(1000,1000)));
}
walkimgdc.SetBrush(*wxBLUE_BRUSH);
for (unsigned int i=0;i<sc->entry_amount;i++) for (unsigned int j=0;j<sc->entry_function_amount[i];j++) for (unsigned int k=0;k<sc->func[i][j].op_amount;k++)
if (sc->func[i][j].op[k].opcode==0x29) {
	for (unsigned int l=0;l<sc->func[i][j].op[k].arg_amount;l++) {
		ScriptArgument& arg = sc->func[i][j].op[k].arg[l];
		vert[l].x = (int16_t)(arg.GetValue() & 0xFFFF)-wk->offset_x;
		vert[l].y = 10000-(int16_t)((arg.GetValue() >> 16) & 0xFFFF)+wk->offset_y;
	}
	walkimgdc.DrawPolygon(sc->func[i][j].op[k].arg_amount,vert);
}
walkbmp.SaveFile(_(L"aaaa.bmp"),wxBITMAP_TYPE_BMP);*/
	} else if (id == wxID_ENTRY) {
		ScriptEditEntryDialog dial(this,*sc,SCRIPT_TYPE_FIELD);
		if (dial.ShowModal()==wxID_OK) {
			dial.ApplyModifications(*sc);
			MarkDataFieldModified(*sortid,CHUNK_TYPE_SCRIPT);
		}
	} else if (id == wxID_SCRIPTLINK) {
		ScriptEditLinkDialog dial(this,*sc,*fieldset.related_text[*sortid]);
		if (dial.ShowModal()==wxID_OK) {
			dial.ApplyModifications(*sc);
			MarkDataFieldModified(*sortid,CHUNK_TYPE_SCRIPT,true);
		}
	} else if (id == wxID_PRELOAD) {
		if (gametype==GAME_TYPE_PSX) {
			ImageMapEditDialog dial(this,fieldset.preload[*sortid],cluster.enemy_shared_map,&cluster.global_map,saveset);
			if (dial.ShowModal()==wxID_OK) {
/*				// Field Role updates : don't seem to be required in the end
				unsigned int i,j;
				bool newmodel;
				for (i=0;i<dial.map.amount;i++)
					if (dial.map.data_type[i]==CHUNK_TYPE_MODEL) {
						newmodel = true;
						for (j=0;j<fieldset.preload[*sortid]->amount;j++)
							if (fieldset.preload[*sortid]->data_type[j]==CHUNK_TYPE_MODEL && fieldset.preload[*sortid]->data_id[j]==dial.map.data_id[i]) {
								newmodel = false;
								break;
							}
						if (newmodel) {
							fieldset.role[*sortid]->AddModelRole(dial.map.data_id[i]);
							MarkDataFieldModified(*sortid,CHUNK_TYPE_FIELD_ROLE);
						}
					}*/
				fieldset.preload[*sortid]->Copy(dial.map);
				fieldset.preload[*sortid]->UpdateOffset();
				MarkDataFieldModified(*sortid,CHUNK_TYPE_IMAGE_MAP);
			}
		} else {
			SteamImageMapEditDialog dial(this,fieldset.preload[*sortid]);
			if (dial.ShowModal()==wxID_OK) {
				fieldset.preload[*sortid]->Copy(dial.map);
				fieldset.preload[*sortid]->UpdateOffset();
				MarkDataFieldModified(*sortid,CHUNK_TYPE_IMAGE_MAP);
			}
		}
	} else if (id == wxID_TEXTURE) {
		ManageFieldTextureDialog dial(this,*fieldset.background_data[*sortid]);
		if (dial.ShowModal(m_fieldtexturechoice->GetSelection())==wxID_OK && dial.field.modified) {
			dial.field.SetupDataInfos(false);
			fieldset.background_data[*sortid]->Copy(dial.field);
			MarkDataFieldModified(*sortid,CHUNK_TYPE_FIELD_TILES);
		}
	} else if (id == wxID_EXPORT) {
		if (!TheWalkmeshExportWindow)
			TheWalkmeshExportWindow = new WalkmeshExportWindow(GetTopWindow());
		if (TheWalkmeshExportWindow->ShowModal() == wxID_OK && !TheWalkmeshExportWindow->m_filepickerexport->GetPath().IsEmpty()) {
			int res = fieldset.walkmesh[*sortid]->ExportAsObj(TheWalkmeshExportWindow->m_filepickerexport->GetPath().ToStdString(), fieldset.script_data[*sortid]->name, fieldset.script_data[*sortid]->object_id);
			if (res == 1) {
				wxLogError(HADES_STRING_OPEN_ERROR_CREATE, TheWalkmeshExportWindow->m_filepickerexport->GetPath().c_str());
			} else {
				wxMessageDialog popupsuccess(this, HADES_STRING_WALKMESH_SAVE_SUCCESS, HADES_STRING_SUCCESS, wxOK | wxCENTRE);
				popupsuccess.ShowModal();
			}
		}
	} else if (id == wxID_IMPORT) {
		if (!TheWalkmeshImportWindow)
			TheWalkmeshImportWindow = new WalkmeshImportWindow(GetTopWindow());
		if (TheWalkmeshImportWindow->ShowModal() == wxID_OK && !TheWalkmeshImportWindow->m_filepickerimport->GetPath().IsEmpty()) {
			wxString successmsg;
			int res = fieldset.walkmesh[*sortid]->ImportFromObj(TheWalkmeshImportWindow->m_filepickerimport->GetPath().ToStdString(), &successmsg);
			if (res == 0) {
				wxMessageDialog popupsuccess(this, successmsg, HADES_STRING_SUCCESS, wxOK | wxCENTRE);
				popupsuccess.ShowModal();
				MarkDataFieldModified(*sortid, CHUNK_TYPE_FIELD_WALK);
			} else if (res == -1) {
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL, TheWalkmeshImportWindow->m_filepickerimport->GetPath().c_str());
			} else if (res < -1) {
				wxLogError(HADES_STRING_WALKMESH_IMPORT_FAIL);
			} else if (res == 1) {
				wxLogError(HADES_STRING_DATA_REACH_LIMIT);
			}
		}
	} else if (id == wxID_WALKMESH) {
		WalkmeshEditDialog dial(this, fieldset.background_data[*sortid], fieldset.walkmesh[*sortid]);
		if (dial.ShowModal() == wxID_OK) {
			unsigned int pathamount = fieldset.walkmesh[*sortid]->walkpath_amount;
			vector<pair<int, int>> triangleshift;
			int shift, triindex = 0;
			unsigned int i;
			for (i = 0; i + 1 < pathamount && i < dial.walkmesh.walkpath_amount; i++) {
				triindex += fieldset.walkmesh[*sortid]->walkpath_triangleamount[i];
				shift = dial.walkmesh.walkpath_triangleamount[i] - fieldset.walkmesh[*sortid]->walkpath_triangleamount[i];
				if (shift != 0)
					triangleshift.push_back({ triindex, shift });
			}
			if (triangleshift.size() > 0) {
				if (sc->ShiftArgument(AT_WALKTRIANGLE, triangleshift) > 0)
					MarkDataFieldModified(*sortid, CHUNK_TYPE_SCRIPT);
			}
			*fieldset.walkmesh[*sortid] = dial.walkmesh;
			MarkDataFieldModified(*sortid, CHUNK_TYPE_FIELD_WALK);
		}
	}
}

void CDDataStruct::OnFieldTexturePaint(wxPaintEvent &event) {
	if (fieldtexpreview.IsOk()) {
		wxPaintDC dc(m_fieldtexturepreview);
		m_fieldtexturepreview->DoPrepareDC(dc);
		dc.DrawBitmap(fieldtexpreview,0,0);
	}
}

//=============================//
//         World Maps          //
//=============================//

// World Battle Spots
int CDDataStruct::GetWorldBattleSetFromSpot(int spot, int spotversion) {
	if (spotversion==1)
		return 2*spot+1;
	else if (spotversion==2)
		return 2*spot+255;
	else if (spotversion==3)
		return 2*spot+254;
	return 2*spot;
}

void CDDataStruct::DisplayWorldBattleHelp(int spotversion, int whichbattle) {
	if (!saveset.sectionloaded[DATA_SECTION_ENMY])
		return;
	unsigned int i, j;
	if (spotversion == -1) {
		for (i = 0; i < 4; i++)
			DisplayWorldBattleHelp(i, whichbattle);
		return;
	}
	if (whichbattle == -1) {
		for (i = 0; i < 4; i++)
			DisplayWorldBattleHelp(spotversion, i);
		return;
	}
	wxStaticText* txtctrl = NULL;
	wxChoice* choicectrl = NULL;

	#define MACRO_WORLD_BATTLE_HELP(VER,BTL) \
		if (spotversion==VER-1 && whichbattle==BTL-1) { \
			txtctrl = m_worldbattlehelplabel ## VER ## BTL; \
			choicectrl = m_worldbattlebattlechoice ## VER ## BTL; \
		} else 

	MACRO_WORLD_BATTLE_HELP(1, 1)
	MACRO_WORLD_BATTLE_HELP(1, 2)
	MACRO_WORLD_BATTLE_HELP(1, 3)
	MACRO_WORLD_BATTLE_HELP(1, 4)
	MACRO_WORLD_BATTLE_HELP(2, 1)
	MACRO_WORLD_BATTLE_HELP(2, 2)
	MACRO_WORLD_BATTLE_HELP(2, 3)
	MACRO_WORLD_BATTLE_HELP(2, 4)
	MACRO_WORLD_BATTLE_HELP(3, 1)
	MACRO_WORLD_BATTLE_HELP(3, 2)
	MACRO_WORLD_BATTLE_HELP(3, 3)
	MACRO_WORLD_BATTLE_HELP(3, 4)
	MACRO_WORLD_BATTLE_HELP(4, 1)
	MACRO_WORLD_BATTLE_HELP(4, 2)
	MACRO_WORLD_BATTLE_HELP(4, 3)
	MACRO_WORLD_BATTLE_HELP(4, 4)
		return;
	if (spotversion >= 2 && !m_worldbattlepanelchoice3->IsShown()) {
		txtctrl->SetLabelText(_(L""));
		return;
	}
	uint16_t bsceneid = enemyset.battle[choicectrl->GetSelection()]->scene_id;
	uint16_t bid = enemyset.battle_data[choicectrl->GetSelection()]->object_id;
	wxString label = _(L"");
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME); i++)
		if (HADES_STRING_BATTLE_SCENE_NAME[i].id == bsceneid) {
			label = HADES_STRING_BATTLE_SCENE_NAME[i].label;
			if (label.Mid(0, 9).IsSameAs(L"World Map"))
				label = label.Mid(11);
			break;
		}
	bool isfriendly = false;
	for (i = 0; i < WORLD_MAP_FRIENDLY_AMOUNT; i++)
		for (j = 0; j < WorldMapDataStruct::friendly_battle_id[i].size(); j++)
			if (bid == WorldMapDataStruct::friendly_battle_id[i][j]) {
				isfriendly = true;
				break;
			}
	if (isfriendly && whichbattle == 3)
		label = _("<i>") + _(HADES_STRING_WORLD_FRIENDLY) + label + _("</i>");
	txtctrl->SetLabelMarkup(label);
}

// Others
void CDDataStruct::DisplayWorldMap(int worldid) {
	unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(worldid);
	TextDataStruct& td = *worldset.text_data[*sortid];
	unsigned int i;
	m_worldscriptedit->Enable(!(worldset.script[*sortid]->object_id>=9100));
	m_worldtextlist->Clear();
	for (i=0;i<td.amount;i++)
		m_worldtextlist->Append(_(td.text[i].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
	m_worldpreload->Enable(gametype==GAME_TYPE_PSX);
	m_worldscriptlink->Enable(worldset.script[*sortid]->multi_lang_script!=NULL);
	m_worldtextcharmaplist->Clear();
	for (i=0;i<worldset.tim_amount[*sortid];i++) {
		wstringstream buffer;
		buffer << "Picture " << i+1 << ends;
		m_worldtextcharmaplist->Append(_(buffer.str()));
	}
	m_worldtextcharmaptexturelistlabel->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextmanagecharmapbutton->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmappallabel->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmaplist->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmappalchoice->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmapexportlabel->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmapexport->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmapwindowlabel->Show(worldset.tim_amount[*sortid]>0);
	m_worldtextcharmapwindow->Show(worldset.tim_amount[*sortid]>0);
	if (worldset.tim_amount[*sortid]>0) {
		TIMImageDataStruct& tdtex = worldset.chartim[*sortid][0];
		m_worldtextcharmappalchoice->Clear();
		for (i=0;i<tdtex.pal_height;i++) {
			wstringstream buffer;
			buffer << L"Palette " << i+1 << ends;
			m_worldtextcharmappalchoice->Append(_(buffer.str()));
		}
		m_worldtextcharmaplist->SetSelection(0);
		m_worldtextcharmappalchoice->SetSelection(0);
		wxImage img = ConvertFullTIMToImage(tdtex,NULL,0,false);
		MACRO_TEXT_DISPLAYTEXTURE(img,m_worldtextcharmapwindow,&worldchartexpreview)
	}
	m_worldscrolledwindow->Layout();
	m_worldscrolledwindow->GetParent()->GetSizer()->Layout();
	m_worldscrolledwindow->Refresh();
// DEBUG
/*wxImage img;
img.SetAlpha();
img.LoadFile(_(L"QuMarsh.bmp"),wxBITMAP_TYPE_ANY);
uint8_t* atlasbin, *atlasrgb;
uint32_t datasize;
atlasrgb = new uint8_t[img.GetWidth()*img.GetHeight()*4];
for (unsigned int j=0;j<img.GetHeight();j++) for (i=0;i<img.GetWidth();i++) {
atlasrgb[(i+j*img.GetWidth())*4] = img.GetRed(i,j);
atlasrgb[(i+j*img.GetWidth())*4+1] = img.GetGreen(i,j);
atlasrgb[(i+j*img.GetWidth())*4+2] = img.GetBlue(i,j);
atlasrgb[(i+j*img.GetWidth())*4+3] = 255;
}
atlasbin = TIMImageDataStruct::CreateSteamTextureFile(datasize,img.GetWidth(),img.GetHeight(),atlasrgb);
fstream ftga("atlas.tex",ios::binary|ios::out); ftga.write((const char*)atlasbin,datasize); ftga.close();
delete[] atlasbin; delete[] atlasrgb;*/
}

void CDDataStruct::DisplayWorldPlace(int placeid) {
	WorldMapDataStruct& wm = *worldset.world_data;
	m_worldplacename->ChangeValue(_(wm.place_name[placeid].str));
}

void CDDataStruct::DisplayWorldBattle(int worldbattleid) {
	WorldMapDataStruct& wm = *worldset.world_data;
	int bset0 = GetWorldBattleSetFromSpot(worldbattleid,0);
	int bset1 = GetWorldBattleSetFromSpot(worldbattleid,1);
	int bset2 = GetWorldBattleSetFromSpot(worldbattleid,2);
	int bset3 = GetWorldBattleSetFromSpot(worldbattleid,3);
	bool hasalt = bset0<100;
	m_worldbattlelabelalt->Show(hasalt);
	m_worldbattlelabelaltmist->Show(hasalt);
	if (saveset.sectionloaded[DATA_SECTION_ENMY]) {
		unsigned int i;
		m_worldbattlepanelchoice3->Show(hasalt);
		m_worldbattlepanelchoice4->Show(hasalt);
		for (i=0;i<enemyset.battle_amount;i++) {
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset0][0])	m_worldbattlebattlechoice11->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset0][1])	m_worldbattlebattlechoice12->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset0][2])	m_worldbattlebattlechoice13->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset0][3])	m_worldbattlebattlechoice14->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset1][0])	m_worldbattlebattlechoice21->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset1][1])	m_worldbattlebattlechoice22->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset1][2])	m_worldbattlebattlechoice23->SetSelection(i);
			if (enemyset.battle_data[i]->object_id==wm.battle_id[bset1][3])	m_worldbattlebattlechoice24->SetSelection(i);
			if (hasalt) {
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset2][0])	m_worldbattlebattlechoice31->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset2][1])	m_worldbattlebattlechoice32->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset2][2])	m_worldbattlebattlechoice33->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset2][3])	m_worldbattlebattlechoice34->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset3][0])	m_worldbattlebattlechoice41->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset3][1])	m_worldbattlebattlechoice42->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset3][2])	m_worldbattlebattlechoice43->SetSelection(i);
				if (enemyset.battle_data[i]->object_id==wm.battle_id[bset3][3])	m_worldbattlebattlechoice44->SetSelection(i);
			}
		}
	} else {
		m_worldbattlepanelspin3->Show(hasalt);
		m_worldbattlepanelspin4->Show(hasalt);
		m_worldbattlebattlespin11->SetValue(wm.battle_id[bset0][0]);
		m_worldbattlebattlespin12->SetValue(wm.battle_id[bset0][1]);
		m_worldbattlebattlespin13->SetValue(wm.battle_id[bset0][2]);
		m_worldbattlebattlespin14->SetValue(wm.battle_id[bset0][3]);
		m_worldbattlebattlespin21->SetValue(wm.battle_id[bset1][0]);
		m_worldbattlebattlespin22->SetValue(wm.battle_id[bset1][1]);
		m_worldbattlebattlespin23->SetValue(wm.battle_id[bset1][2]);
		m_worldbattlebattlespin24->SetValue(wm.battle_id[bset1][3]);
		if (hasalt) {
			m_worldbattlebattlespin31->SetValue(wm.battle_id[bset2][0]);
			m_worldbattlebattlespin32->SetValue(wm.battle_id[bset2][1]);
			m_worldbattlebattlespin33->SetValue(wm.battle_id[bset2][2]);
			m_worldbattlebattlespin34->SetValue(wm.battle_id[bset2][3]);
			m_worldbattlebattlespin41->SetValue(wm.battle_id[bset3][0]);
			m_worldbattlebattlespin42->SetValue(wm.battle_id[bset3][1]);
			m_worldbattlebattlespin43->SetValue(wm.battle_id[bset3][2]);
			m_worldbattlebattlespin44->SetValue(wm.battle_id[bset3][3]);
		}
	}
	DisplayWorldBattleHelp(-1,-1);
}

void CDDataStruct::OnListBoxWorldMap(wxCommandEvent& event) {
	DisplayWorldMap(m_worldlist->GetSelection());
}

void CDDataStruct::OnListBoxWorldPlace(wxCommandEvent& event) {
	DisplayWorldPlace(m_worldplacelist->GetSelection());
}

void CDDataStruct::OnListBoxWorldBattle(wxCommandEvent& event) {
	DisplayWorldBattle(m_worldbattlelist->GetSelection());
}

void CDDataStruct::OnWorldChangeName(wxCommandEvent& event) {
	unsigned int sel = m_worldplacelist->GetSelection();
	WorldMapDataStruct& wm = *worldset.world_data;
	if (wm.SetName(sel,m_worldplacename->GetValue().ToStdWstring())) {
		wxTextPos ip = m_worldplacename->GetInsertionPoint();
		m_worldplacename->ChangeValue(_(wm.place_name[sel].str));
		m_worldplacename->SetInsertionPoint(ip);
		TextReachLimit();
	} else {
		m_worldplacelist->SetString(sel,_(wm.place_name[sel].GetStr(hades::TEXT_PREVIEW_TYPE)));
		MarkDataWorldMapModified(sel,CHUNK_TYPE_VARIOUS);
	}
}

void CDDataStruct::OnWorldChangeButton(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_SCRIPT) {
		unsigned int sel = m_worldlist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(sel);
		ScriptDataStruct* sc = worldset.script[*sortid];
		ScriptEditDialog dial(this,*sc,SCRIPT_TYPE_WORLD,&saveset,NULL,worldset.text_data[*sortid]);
		if (dial.ShowModal()==wxID_OK) {
			*sc = dial.script;
			MarkDataWorldMapModified(*sortid,CHUNK_TYPE_SCRIPT);
		}
	} else if (id == wxID_ENTRY) {
		unsigned int sel = m_worldlist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(sel);
		ScriptEditEntryDialog dial(this,*worldset.script[*sortid],SCRIPT_TYPE_WORLD);
		if (dial.ShowModal()==wxID_OK) {
			dial.ApplyModifications(*worldset.script[*sortid]);
			MarkDataWorldMapModified(*sortid,CHUNK_TYPE_SCRIPT);
		}
	} else if (id == wxID_SCRIPTLINK) {
		unsigned int sel = m_worldlist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(sel);
		ScriptEditLinkDialog dial(this,*worldset.script[*sortid],*worldset.text_data[*sortid]);
		if (dial.ShowModal()==wxID_OK) {
			dial.ApplyModifications(*worldset.script[*sortid]);
			MarkDataWorldMapScriptModified(*sortid,true);
		}
	} else if (id == wxID_PRELOAD) {
		unsigned int sel = m_worldlist->GetSelection();
		unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(sel);
		if (gametype==GAME_TYPE_PSX) {
			ImageMapEditDialog dial(this,worldset.preload[*sortid],cluster.enemy_shared_map,&cluster.global_map,saveset);
			if (dial.ShowModal()==wxID_OK) {
				worldset.preload[*sortid]->Copy(dial.map);
				worldset.preload[*sortid]->UpdateOffset();
				MarkDataWorldMapModified(*sortid,CHUNK_TYPE_IMAGE_MAP);
			}
		} else {
			SteamImageMapEditDialog dial(this,worldset.preload[*sortid]);
			if (dial.ShowModal()==wxID_OK) {
				worldset.preload[*sortid]->Copy(dial.map);
				worldset.preload[*sortid]->UpdateOffset();
				MarkDataWorldMapModified(*sortid,CHUNK_TYPE_IMAGE_MAP);
			}
		}
	} else if (id == wxID_NAME) {
		unsigned int sel = m_worldplacelist->GetSelection();
		WorldMapDataStruct& wm = *worldset.world_data;
		if (gametype==GAME_TYPE_PSX) {
			CharmapDataStruct* chmap = worldset.charmap[0];
			TextEditDialog ted(this,wm.place_name[sel],wm.place_name_extra_size,TEXT_STYLE_DEFAULT,chmap);
			if (ted.ShowModal()==wxID_OK) {
				wm.SetName(sel,ted.text);
				m_worldplacename->SetValue(wm.place_name[sel].str);
			}
		} else {
			TextSteamEditDialog ted(this,&ffuiset,wm.place_name[sel]);
			if (ted.ShowModal()==wxID_OK) {
				wm.SetName(sel,ted.text);
				m_worldplacename->SetValue(wm.place_name[sel].str);
			}
		}
	}
}

void CDDataStruct::OnWorldChangeChoice(wxCommandEvent& event) {
	unsigned int sel = m_worldbattlelist->GetSelection();
	WorldMapDataStruct& wm = *worldset.world_data;
	BattleDataStruct& bd = *enemyset.battle_data[event.GetSelection()];
	int id = event.GetId();

	#define MACRO_WORLD_CHANGE_CHOICE(VER,BTL) \
		if (id == wxID_BATTLE ## VER ## BTL) { \
			int bset = GetWorldBattleSetFromSpot(sel,VER-1); \
			if (wm.ChangeBattle(bset, BTL-1, bd.object_id) != 0) { \
				wxMessageDialog popup(NULL, HADES_STRING_WORLD_FRIENDLY_HELP, HADES_STRING_WARNING, wxOK | wxCENTRE); \
				popup.ShowModal(); \
			} \
			DisplayWorldBattleHelp(VER-1,BTL-1); \
		} else 

	MACRO_WORLD_CHANGE_CHOICE(1,1)
	MACRO_WORLD_CHANGE_CHOICE(1,2)
	MACRO_WORLD_CHANGE_CHOICE(1,3)
	MACRO_WORLD_CHANGE_CHOICE(1,4)
	MACRO_WORLD_CHANGE_CHOICE(2,1)
	MACRO_WORLD_CHANGE_CHOICE(2,2)
	MACRO_WORLD_CHANGE_CHOICE(2,3)
	MACRO_WORLD_CHANGE_CHOICE(2,4)
	MACRO_WORLD_CHANGE_CHOICE(3,1)
	MACRO_WORLD_CHANGE_CHOICE(3,2)
	MACRO_WORLD_CHANGE_CHOICE(3,3)
	MACRO_WORLD_CHANGE_CHOICE(3,4)
	MACRO_WORLD_CHANGE_CHOICE(4,1)
	MACRO_WORLD_CHANGE_CHOICE(4,2)
	MACRO_WORLD_CHANGE_CHOICE(4,3)
	MACRO_WORLD_CHANGE_CHOICE(4,4) {}
	MarkDataWorldMapModified(sel,CHUNK_TYPE_VARIOUS);
}

void CDDataStruct::OnWorldChangeSpin(wxSpinEvent& event) {
	unsigned int sel = m_worldbattlelist->GetSelection();
	WorldMapDataStruct& wm = *worldset.world_data;
	int id = event.GetId();

	#define MACRO_WORLD_CHANGE_SPIN(VER,BTL) \
		if (id == wxID_BATTLE ## VER ## BTL) { \
			int bset = GetWorldBattleSetFromSpot(sel,VER-1); \
			if (wm.ChangeBattle(bset, BTL-1, event.GetPosition()) != 0) { \
				wxMessageDialog popup(NULL, HADES_STRING_WORLD_FRIENDLY_HELP, HADES_STRING_WARNING, wxOK | wxCENTRE); \
				popup.ShowModal(); \
			} \
			DisplayWorldBattleHelp(VER-1,BTL-1); \
		} else 

	MACRO_WORLD_CHANGE_SPIN(1,1)
	MACRO_WORLD_CHANGE_SPIN(1,2)
	MACRO_WORLD_CHANGE_SPIN(1,3)
	MACRO_WORLD_CHANGE_SPIN(1,4)
	MACRO_WORLD_CHANGE_SPIN(2,1)
	MACRO_WORLD_CHANGE_SPIN(2,2)
	MACRO_WORLD_CHANGE_SPIN(2,3)
	MACRO_WORLD_CHANGE_SPIN(2,4)
	MACRO_WORLD_CHANGE_SPIN(3,1)
	MACRO_WORLD_CHANGE_SPIN(3,2)
	MACRO_WORLD_CHANGE_SPIN(3,3)
	MACRO_WORLD_CHANGE_SPIN(3,4)
	MACRO_WORLD_CHANGE_SPIN(4,1)
	MACRO_WORLD_CHANGE_SPIN(4,2)
	MACRO_WORLD_CHANGE_SPIN(4,3)
	MACRO_WORLD_CHANGE_SPIN(4,4) {}
	MarkDataWorldMapModified(sel,CHUNK_TYPE_VARIOUS);
}

void CDDataStruct::OnWorldTextRightClickMenu(wxCommandEvent& event) {
	int sel = m_worldlist->GetSelection();
	int id = event.GetId();
	unsigned int* sortid = (unsigned int*)m_worldlist->GetClientData(sel);
	TextDataStruct& td = *worldset.text_data[*sortid];
	int objid = m_worldtextlist->GetSelection();
	unsigned int i;
	int newsel = -1;
	if (id == wxID_ADD) {
		newsel = td.amount;
		FF9String newstr;
		newstr.CreateEmpty();
		if (gametype != GAME_TYPE_PSX)
			newstr.SetValue(L"[STRT=0,1]");
		if (td.AddText(td.amount, newstr)) {
			newsel = -1;
			TextReachLimit();
		}
	} else if (id == wxID_REMOVE && objid != wxNOT_FOUND) {
		td.RemoveText(objid);
		newsel = min(td.amount - 1, objid);
	} else if (id == wxID_PASTE) {
		newsel = objid;
	}
	if (newsel >= 0) {
		m_worldtextlist->Clear();
		for (i = 0; i < td.amount; i++)
			m_worldtextlist->Append(_(td.text[i].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0, 100)));
		m_worldtextlist->SetSelection(newsel);
	}
	MarkDataWorldMapModified(*sortid, CHUNK_TYPE_TEXT);
}

//=============================//
//        Battle Scenes        //
//=============================//

#define MACRO_BATTLESCENE_DISPLAYTEXTURE(BMP) \
	if (BMP.IsOk()) { \
		m_battlescenetexture->SetVirtualSize(BMP.GetWidth(),BMP.GetHeight()); \
		wxClientDC dc(m_battlescenetexture); \
		dc.Clear(); \
		m_battlescenetexture->DoPrepareDC(dc); \
		dc.DrawBitmap(BMP,0,0); \
		scenetexpreview = BMP; \
	}

void CDDataStruct::DisplayBattleScene(int sceneid) {
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sceneid);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	unsigned int i;
	m_battlescenetexturelist->Clear();
	m_battlescenetexturechoice->Clear();
	m_battlescenepalettechoice->Clear();
	if (sceneset.image[*sortid]) {
		for (i=0;i<bs.tim_amount;i++) {
			wstringstream buffer;
			buffer << L"Texture " << i+1 << ends;
			m_battlescenetexturelist->Append(_(buffer.str()));
		}
		if (scenetex)
			delete[] scenetex;
		if (scenepal)
			delete[] scenepal;
		TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
		TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
		unsigned int numtex = tdtex.width*(tdtex.format & 0x3)/128;
		scenetex = new uint16_t[numtex];
		scenepal = new uint16_t[tdpal.height];
		uint16_t texbeg = (tdtex.pos_x >> 6) | 0x80;
		for (i=0;i<numtex;i++) {
			scenetex[i] = texbeg+i;
			wstringstream buffer;
			buffer << L"Painting " << i+1 << ends;
			m_battlescenetexturechoice->Append(_(buffer.str()));
		}
		for (i=0;i<tdpal.height;i++) {
			scenepal[i] = ((tdpal.pos_y+i) << 6) | (tdpal.pos_x >> 5);
			wstringstream buffer;
			buffer << L"Palette " << i+1 << ends;
			m_battlescenepalettechoice->Append(_(buffer.str()));
		}
		m_battlescenetexturelist->SetSelection(0);
		for (i=0;i<tdtex.width*(tdtex.format & 0x3)/128;i++)
			if (scenetex[i]==bs.tim_texpos[0])
				m_battlescenetexturechoice->SetSelection(i);
		for (i=0;i<tdpal.height;i++)
			if (scenepal[i]==bs.tim_palpos[0])
				m_battlescenepalettechoice->SetSelection(i);
		wxBitmap bmp = ConvertTIMToBitmap(tdtex,tdpal,256,256,bs.tim_palpos[0],bs.tim_texpos[0]);
		MACRO_BATTLESCENE_DISPLAYTEXTURE(bmp)
	}
	m_battlescenescrolledwindow->Layout();
	m_battlescenescrolledwindow->GetParent()->GetSizer()->Layout();
	m_battlescenescrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxBattleScene(wxCommandEvent& event) {
	DisplayBattleScene(m_battlescenelist->GetSelection());
}

void CDDataStruct::OnBattleSceneImportLinkTexturePaint(wxPaintEvent& event) {
	if (scenetexlinkpreview.IsOk()) {
		wxClientDC dc(scenetexturelink->m_texturepanel);
		dc.Clear();
		scenetexturelink->m_texturepanel->DoPrepareDC(dc);
		dc.DrawBitmap(scenetexlinkpreview,0,0);
	}
}

void CDDataStruct::OnBattleSceneImportLinkTextureChoice(wxCommandEvent& event) {
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	int texsel = event.GetSelection();
	scenetexlinkpreview = ConvertTIMToBitmap(tdtex,tdpal,256,256,bs.tim_palpos[texsel],bs.tim_texpos[texsel]);
	if (scenetexlinkpreview.IsOk()) {
		wxClientDC dc(scenetexturelink->m_texturepanel);
		dc.Clear();
		scenetexturelink->m_texturepanel->DoPrepareDC(dc);
		dc.DrawBitmap(scenetexlinkpreview,0,0);
	}
}

void CDDataStruct::OnBattleSceneImportLinkTextureFocus(wxFocusEvent& event) {
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	wxChoice* choice = (wxChoice*)event.GetEventObject();
	int texsel = choice->GetSelection();
	scenetexlinkpreview = ConvertTIMToBitmap(tdtex,tdpal,256,256,bs.tim_palpos[texsel],bs.tim_texpos[texsel]);
	if (scenetexlinkpreview.IsOk()) {
		wxClientDC dc(scenetexturelink->m_texturepanel);
		dc.Clear();
		scenetexturelink->m_texturepanel->DoPrepareDC(dc);
		dc.DrawBitmap(scenetexlinkpreview,0,0);
	}
	event.Skip();
}

BattleSceneExportWindow* TheBattleSceneExportDialog;
void CDDataStruct::OnBattleSceneExportButton(wxCommandEvent& event) {
	if (!TheBattleSceneExportDialog)
		TheBattleSceneExportDialog = new BattleSceneExportWindow(GetTopWindow());
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	if (TheBattleSceneExportDialog->ShowModal()==wxID_OK) {
		BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
		if (bs.Export(TheBattleSceneExportDialog->m_filepicker->GetPath().mb_str(),TheBattleSceneExportDialog->m_usequads->IsChecked())) {
			wxLogError(HADES_STRING_ERROR_UNKNOWN);
		} else {
			wxMessageDialog popupsuccess(this,HADES_STRING_SCENE_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
		}
	}
}

BattleSceneImportWindow* TheBattleSceneImportDialog;
void CDDataStruct::OnBattleSceneImportButton(wxCommandEvent& event) {
	if (!TheBattleSceneImportDialog)
		TheBattleSceneImportDialog = new BattleSceneImportWindow(GetTopWindow());
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	if (sceneset.image[*sortid] && TheBattleSceneImportDialog->ShowModal() == wxID_OK) {
		BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
		unsigned int numtex;
		wstring* texname;
		LogStruct log = bs.Import(TheBattleSceneImportDialog->m_filepicker->GetPath().mb_str(), &numtex, &texname);
		LogDialog lw(this, log);
		lw.wxDialog::ShowModal();
		if (log.ok) {
			unsigned int i, j;
			BattleSceneImportLinkTextureWindow linkwindow(this);
			scenetexturelink = &linkwindow;
			wxArrayString currenttexlist = m_battlescenetexturelist->GetStrings();
			wxChoice** currenttexchoices = new wxChoice * [numtex];
			linkwindow.m_linksizer->SetRows(numtex + 1);
			for (i = 0; i < numtex; i++) {
				wxTextCtrl* importtexctrl = new wxTextCtrl(linkwindow.m_linkscrolled, wxID_ANY, _(texname[i]), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
				linkwindow.m_linksizer->Add(importtexctrl, 0, wxALIGN_RIGHT | wxALL, 5);
				wxStaticText* arrowtext = new wxStaticText(linkwindow.m_linkscrolled, wxID_ANY, _("------------>"));
				linkwindow.m_linksizer->Add(arrowtext, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
				currenttexchoices[i] = new wxChoice(linkwindow.m_linkscrolled, wxID_ANY, wxDefaultPosition, wxDefaultSize, currenttexlist);
				currenttexchoices[i]->SetSelection(i % m_battlescenetexturelist->GetCount());
				linkwindow.m_linksizer->Add(currenttexchoices[i], 0, wxALIGN_LEFT | wxALL, 5);
				currenttexchoices[i]->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CDDataStruct::OnBattleSceneImportLinkTextureChoice), NULL, this);
				currenttexchoices[i]->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(CDDataStruct::OnBattleSceneImportLinkTextureFocus), NULL, this);
			}
			TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0], sceneset.image[*sortid][1]);
			TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0], sceneset.image[*sortid][1]);
			scenetexlinkpreview = ConvertTIMToBitmap(tdtex, tdpal, 256, 256, bs.tim_palpos[0], bs.tim_texpos[0]);
			linkwindow.m_texturepanel->Connect(wxEVT_PAINT, wxPaintEventHandler(CDDataStruct::OnBattleSceneImportLinkTexturePaint), NULL, this);
			linkwindow.wxDialog::ShowModal();
			for (i = 0; i < numtex; i++) {
				currenttexchoices[i]->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CDDataStruct::OnBattleSceneImportLinkTextureChoice), NULL, this);
				currenttexchoices[i]->Disconnect(wxEVT_SET_FOCUS, wxFocusEventHandler(CDDataStruct::OnBattleSceneImportLinkTextureFocus), NULL, this);
			}
			linkwindow.m_texturepanel->Disconnect(wxEVT_PAINT, wxPaintEventHandler(CDDataStruct::OnBattleSceneImportLinkTexturePaint), NULL, this);
			scenetexlinkpreview = wxNullBitmap;
			for (i = 0; i < bs.obj_amount; i++)
				for (j = 0; j < (unsigned int)bs.obj_quad_amount[i] + (unsigned int)bs.obj_trgl_amount[i]; j++)
					bs.obj_tex_id[i][j] = currenttexchoices[bs.obj_tex_id[i][j]]->GetSelection();
			delete[] texname;
			delete[] currenttexchoices;
			MarkDataBattleSceneModified(*sortid, CHUNK_TYPE_BATTLE_SCENE);
		}
	}
}

void CDDataStruct::OnBattleSceneTextureListBox(wxCommandEvent& event) {
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	int texsel = m_battlescenetexturelist->GetSelection();
	unsigned int i;
	for (i=0;i<tdtex.width*(tdtex.format & 0x3)/128;i++)
		if (scenetex[i]==bs.tim_texpos[texsel])
			m_battlescenetexturechoice->SetSelection(i);
	for (i=0;i<tdpal.height;i++)
		if (scenepal[i]==bs.tim_palpos[texsel])
			m_battlescenepalettechoice->SetSelection(i);
	wxBitmap bmp = ConvertTIMToBitmap(tdtex,tdpal,256,256,bs.tim_palpos[texsel],bs.tim_texpos[texsel]);
	MACRO_BATTLESCENE_DISPLAYTEXTURE(bmp)
}

void CDDataStruct::OnBattleSceneManageTexturesButton(wxCommandEvent& event) {
	unsigned int i;
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	if (sceneset.image[*sortid]==NULL)
		return;
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	unsigned int palcount = m_battlescenepalettechoice->GetCount();
	unsigned int texcount = m_battlescenetexturechoice->GetCount();
	uint32_t** pal = new uint32_t*[palcount];
	uint16_t* texx = new uint16_t[texcount];
	uint16_t* texy = new uint16_t[texcount];
	for (i=0;i<palcount;i++) {
		pal[i] = new uint32_t[256];
		uint32_t* palbuff = tdpal.ConvertAsPalette(scenepal[i]); // Temp array
		memcpy(pal[i],palbuff,256*4);
	}
	for (i=0;i<texcount;i++) {
		texx[i] = 128*i;
		texy[i] = 0;
	}
	ManageTextureDialog dial(this,tdtex,texcount,0,1,256,256,false,texx,texy,1,1,NULL,NULL,1,1,palcount,pal,true,256);
	if (dial.ShowModal()==wxID_OK) {
		uint32_t newsize = dial.pal_amount*512+dial.tim.img_size;
		uint32_t oldsize = palcount*512+tdtex.img_size;
		int id = wxID_OK;
		while (id == wxID_OK && newsize>oldsize+tdtex.GetExtraSize()) {
			LogStruct log;
			wstringstream buffer;
			buffer << L" - Not enough space : the imported texture takes " << newsize << L" bytes for " << oldsize+tdtex.GetExtraSize() << L" bytes available." << endl;
			log.AddError(buffer.str());
			LogDialog lw(this,log);
			lw.wxDialog::ShowModal();
			id = dial.ShowModal();
			newsize = dial.pal_amount*512+dial.tim.img_size;
		}
		if (id == wxID_OK) {
			if (tdtex.img_size!=dial.tim.img_size) {
				tdtex.SetSize(tdtex.size+dial.tim.img_size-tdtex.img_size);
				tdtex.img_size = dial.tim.img_size;
			}
			tdtex.pos_x = dial.tim.pos_x;
			tdtex.pos_y = dial.tim.pos_y;
			tdtex.width = dial.tim.width;
			tdtex.height = dial.tim.height;
			delete[] tdtex.pixel_value;
			tdtex.pixel_value = dial.tim.pixel_value;
			tdpal.ImportPalette(dial.pal_amount,dial.pal);
			bool bsmodified = false;
			for (i=0;i<bs.tim_amount;i++)
				if ((bs.tim_palpos[i] >> 6)>=tdpal.pos_y+tdpal.height) {
					bs.tim_palpos[i] = (tdpal.pos_y << 6) | (tdpal.pos_x >> 5);
					bsmodified = true;
				}
			DisplayBattleScene(sel);
			MarkDataBattleSceneModified(*sortid,CHUNK_TYPE_TIM,0);
			MarkDataBattleSceneModified(*sortid,CHUNK_TYPE_TIM,1);
			if (bsmodified)
				MarkDataBattleSceneModified(*sortid,CHUNK_TYPE_BATTLE_SCENE);
		}
	}
}

void CDDataStruct::OnBattleSceneTextureChoice(wxCommandEvent& event) {
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	int texsel = m_battlescenetexturelist->GetSelection();
	int pntsel = event.GetSelection();
	if (texsel!=wxNOT_FOUND) {
		bs.tim_texpos[texsel] = scenetex[pntsel];
		MarkDataBattleSceneModified(*sortid,CHUNK_TYPE_BATTLE_SCENE);
		wxBitmap bmp = ConvertTIMToBitmap(tdtex,tdpal,256,256,bs.tim_palpos[texsel],bs.tim_texpos[texsel]);
		MACRO_BATTLESCENE_DISPLAYTEXTURE(bmp)
	}
}

void CDDataStruct::OnBattleScenePaletteChoice(wxCommandEvent& event) {
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	TIMImageDataStruct& tdtex = TIMImageDataStruct::GetTIMTextureStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	TIMImageDataStruct& tdpal = TIMImageDataStruct::GetTIMPaletteStruct(sceneset.image[*sortid][0],sceneset.image[*sortid][1]);
	int texsel = m_battlescenetexturelist->GetSelection();
	int palsel = event.GetSelection();
	if (texsel!=wxNOT_FOUND) {
		bs.tim_palpos[texsel] = scenepal[palsel];
		MarkDataBattleSceneModified(*sortid,CHUNK_TYPE_BATTLE_SCENE);
		wxBitmap bmp = ConvertTIMToBitmap(tdtex,tdpal,256,256,bs.tim_palpos[texsel],bs.tim_texpos[texsel]);
		MACRO_BATTLESCENE_DISPLAYTEXTURE(bmp)
	}
}

bool DiscardTIMLimit = false;
void CDDataStruct::OnBattleSceneTextureSelectRightClickMenu(wxCommandEvent& event) {
	int sel = m_battlescenelist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_battlescenelist->GetClientData(sel);
	BattleSceneDataStruct& bs = *sceneset.scene[*sortid];
	int id = event.GetId();
	int texsel = m_battlescenetexturelist->GetSelection();
	if (id == wxID_ADD) {
		if (bs.GetExtraSize() < 0x4) {
			if (!DiscardTIMLimit) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_TIM_REACH_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardTIMLimit = true;
			}
			return;
		} else {
			uint16_t* newtexpos = new uint16_t[bs.tim_amount + 1];
			uint16_t* newpalpos = new uint16_t[bs.tim_amount + 1];
			memcpy(newtexpos, bs.tim_texpos, bs.tim_amount * sizeof(uint16_t));
			memcpy(newpalpos, bs.tim_palpos, bs.tim_amount * sizeof(uint16_t));
			delete[] bs.tim_texpos;
			delete[] bs.tim_palpos;
			newtexpos[bs.tim_amount] = newtexpos[texsel];
			newpalpos[bs.tim_amount] = newpalpos[texsel];
			bs.tim_texpos = newtexpos;
			bs.tim_palpos = newpalpos;
			bs.tim_amount++;
			wstringstream buffer;
			buffer << L"Texture " << bs.tim_amount << ends;
			m_battlescenetexturelist->Append(_(buffer.str()));
		}
	} else if (id == wxID_REMOVE) {
		if (bs.tim_amount > 1) {
			memmove(bs.tim_texpos + texsel, bs.tim_texpos + texsel + 1, (bs.tim_amount - texsel - 1) * sizeof(uint16_t));
			memmove(bs.tim_palpos + texsel, bs.tim_palpos + texsel + 1, (bs.tim_amount - texsel - 1) * sizeof(uint16_t));
			bs.tim_amount--;
			m_battlescenetexturelist->Delete(bs.tim_amount);
			m_battlescenetexturelist->SetSelection(min(texsel, bs.tim_amount - 1));
			DisplayBattleScene(sel);
		}
	}
}

void CDDataStruct::OnBattleSceneTextureRightClick(wxMouseEvent& event) {
	int newsel = m_battlescenetexturelist->HitTest(event.GetPosition());
	if (newsel!=wxNOT_FOUND) {
		m_battlescenetexturelist->SetSelection(newsel);
		wxCommandEvent dummyevent;
		OnBattleSceneTextureListBox(dummyevent);
		m_battlescenetexturelist->PopupMenu(scenetexmenu);
	}
	event.Skip();
}

void CDDataStruct::OnBattleSceneTexturePaint(wxPaintEvent &event) {
	if (scenetexpreview.IsOk()) {
		wxPaintDC dc(m_battlescenetexture);
		m_battlescenetexture->DoPrepareDC(dc);
		dc.DrawBitmap(scenetexpreview,0,0);
	}
}

//=============================//
//       Spell Animations      //
//=============================//

void CDDataStruct::DisplaySpellAnimation(int spellanimid) {
	unsigned int* sortid = (unsigned int*)m_spellanimlist->GetClientData(spellanimid);
	SpellAnimationDataStruct& sa = spellanimset.spell[*sortid];
	m_spellanimsequence->Enable(!sa.is_empty);
	if (sa.is_empty)
		return;
}

void CDDataStruct::OnListBoxSpellAnimation(wxCommandEvent& event) {
	DisplaySpellAnimation(m_spellanimlist->GetSelection());
}

void CDDataStruct::OnSpellAnimationChangeButton(wxCommandEvent& event) {
	unsigned int sel = m_spellanimlist->GetSelection();
	unsigned int* sortid = (unsigned int*)m_spellanimlist->GetClientData(sel);
	SpellAnimationDataStruct& sa = spellanimset.spell[*sortid];
	int id = event.GetId();
	if (id == wxID_SEQUENCE) {
		AnimSequenceEditDialog dial(this,sa);
		if (dial.ShowModal()==wxID_OK) {
			sa.Copy(dial.anim);
			MarkDataSpellAnimationModified(*sortid,SPELL_ANIMATION_DATA_TYPE_SEQUENCE);
		}
	}
}

//=============================//
//           Menu UI           //
//=============================//

void CDDataStruct::DisplaySpecialText(int textblockid) {
	SpecialTextDataStruct& st = ffuiset.special_text->text_block[textblockid];
	unsigned int i;
	m_specialtextdatalist->Clear();
	if (st.is_localization)
		for (i=0;i<st.amount;i++)
			m_specialtextdatalist->Append(_(st.localization_field[i])+_(L": ")+_(st.text[i].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
	else
		for (i=0;i<st.amount;i++)
			m_specialtextdatalist->Append(_(st.text[i].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
	m_textscrolledwindow->Layout();
	m_textscrolledwindow->GetParent()->GetSizer()->Layout();
	m_textscrolledwindow->Refresh();
}

void CDDataStruct::OnListBoxSpecialText(wxCommandEvent& event) {
	DisplaySpecialText(m_specialtextlist->GetSelection());
}

void CDDataStruct::OnSpecialTextEditText(wxCommandEvent& event) {
	int textsel = m_specialtextdatalist->GetSelection();
	SpecialTextDataStruct& st = ffuiset.special_text->text_block[m_specialtextlist->GetSelection()];
	if (gametype==GAME_TYPE_PSX) {
		CharmapDataStruct* chmap = NULL;
		if (saveset.sectionloaded[DATA_SECTION_TEXT])
			chmap = textset.charmap[textset.main_charmap_index];
		TextEditDialog ted(this,st.text[textsel],st.space_total-st.space_used,TEXT_STYLE_DESCRIPTION,chmap);
		if (ted.ShowModal()==wxID_OK) {
			st.SetText(textsel,ted.text);
			if (st.is_localization)
				m_specialtextdatalist->SetString(textsel,_(st.localization_field[textsel])+_(L": ")+_(st.text[textsel].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
			else
				m_specialtextdatalist->SetString(textsel,_(st.text[textsel].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
			MarkDataMenuUIModified();
		}
	} else {
		TextSteamEditDialog ted(this,&ffuiset,st.text[textsel],TEXT_STYLE_DESCRIPTION);
		if (ted.ShowModal()==wxID_OK) {
			MACRO_MULTILANG_INCREASE_COUNTER(st.text[textsel],ted.text,st.text,textsel,L"")
			st.SetText(textsel,ted.text);
			if (st.is_localization)
				m_specialtextdatalist->SetString(textsel,_(st.localization_field[textsel])+_(L": ")+_(st.text[textsel].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
			else
				m_specialtextdatalist->SetString(textsel,_(st.text[textsel].GetStr(hades::TEXT_PREVIEW_TYPE).substr(0,100)));
			MarkDataMenuUIModified();
		}
	}
}

void CDDataStruct::OnSpecialTextRightClick(wxMouseEvent& event) {
	int newsel = m_specialtextdatalist->HitTest(event.GetPosition());
	if (newsel!=wxNOT_FOUND)
		m_specialtextdatalist->SetSelection(newsel);
	m_specialtextdatalist->PopupMenu(specialtextmenu);
}

void CDDataStruct::OnSpecialTextRightClickMenu(wxCommandEvent& event) {
	int sel = m_specialtextlist->GetSelection();
	int id = event.GetId();
	SpecialTextDataStruct& td = ffuiset.special_text->text_block[sel];
	int objid = m_specialtextdatalist->GetSelection();
	int newsel = -1;
	if (id == wxID_ADD) {
		newsel = td.amount;
		FF9String newstr;
		newstr.CreateEmpty();
		if (gametype!=GAME_TYPE_PSX)
			for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				newstr.SetValue(L"",lang);
		if (td.AddText(td.amount,newstr)) {
			newsel = -1;
			TextReachLimit();
		}
	} else if (id == wxID_REMOVE && objid!=wxNOT_FOUND) {
		td.RemoveText(objid);
		newsel = min((int)td.amount-1,objid);
	}
	if (newsel>=0) {
		DisplaySpecialText(sel);
		m_specialtextdatalist->SetSelection(newsel);
	}
	MarkDataMenuUIModified();
}

//=============================//
//          MIPS Code          //
//=============================//

void CDDataStruct::DisplayMipsBattle(int mipsbattleid) {
	if (mipsbattleid==0) {
		m_mipsbattlescrolledwindow->Show(false);
		m_mipsbattlegenscrolledwindow->Show(true);
		m_mipsbattlegenscrolledwindow->Layout();
		m_mipsbattlegenscrolledwindow->GetParent()->GetSizer()->Layout();
		m_mipsbattlegenscrolledwindow->Refresh();
	} else {
		mipsbattleid--;
		wxString ramposlabel;
		ramposlabel.Printf(wxT("0x%X"),mipsset.battle_ram_pos[MIPS_BATTLE_SPELL_EFFECT_INDEX+mipsbattleid] & 0x7FFFFFFF);
		m_mipsbattlerampos->SetLabel(ramposlabel);
		m_mipsbattlegenscrolledwindow->Show(false);
		m_mipsbattlescrolledwindow->Show(true);
		m_mipsbattlescrolledwindow->Layout();
		m_mipsbattlescrolledwindow->GetParent()->GetSizer()->Layout();
		m_mipsbattlescrolledwindow->Refresh();
	}
}

void CDDataStruct::OnListBoxMipsBattle(wxCommandEvent& event) {
	DisplayMipsBattle(m_mipsbattlelist->GetSelection());
}

void CDDataStruct::OnMipsBattleButton(wxCommandEvent& event) {
	bool customfunc = m_mipsbattlelist->GetSelection()==0;
	MipsFunction* func;
	if (customfunc) {
		unsigned int instindex, i;
		wstringstream hexstr;
		func = new MipsFunction;
		func->amount = m_mipsbattleramlengthgen->GetValue();
		func->instruction = new MipsInstruction*[func->amount];
		hexstr << std::hex << m_mipsbattleramposgen->GetValue().ToStdWstring();
		hexstr >> instindex;
		instindex &= 0x7FFFFFFF;
		if (instindex<(mipsset.battle_code.ram_pos & 0x7FFFFFFF) || instindex>=(mipsset.battle_code.ram_pos & 0x7FFFFFFF)+(mipsset.battle_code.amount << 2)) {
			wxString oobstr = wxString::Format(wxT(HADES_STRING_MIPS_OOB),mipsset.battle_code.ram_pos & 0x7FFFFFFF,(mipsset.battle_code.ram_pos & 0x7FFFFFFF)+(mipsset.battle_code.amount << 2));
			wxMessageDialog popup(NULL,oobstr,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popup.ShowModal();
			delete[] func->instruction;
			delete func;
			return;
		}
		instindex = (instindex-(mipsset.battle_code.ram_pos & 0x7FFFFFFF)) >> 2;
		for (i=0;i<func->amount && instindex+i<mipsset.battle_code.amount;i++)
			func->instruction[i] = &mipsset.battle_code.instruction[instindex+i];
		func->amount = i;
	} else
		func = &mipsset.battle_spell_effect[m_mipsbattlelist->GetSelection()-1];
	MipsScriptEditDialog dial(this,func);
	if (dial.ShowModal()==wxID_OK) {
		for (unsigned int i=0;i<func->amount;i++)
			func->instruction[i]->CopyValue(*dial.function.instruction[i]);
		MarkDataMipsModified(1);
	}
	if (customfunc) {
		delete[] func->instruction;
		delete func;
	}
}

//=============================//
//          CIL Code           //
//=============================//

void CDDataStruct::DisplayCilStruct(int cilstructid) {
	unsigned int sortid = *(unsigned int*)m_ciltypelist->GetClientData(cilstructid);
	unsigned int i,mamount;
	m_cilmethodlist->Clear();
	mamount = cilset.data->GetTypeMethodAmount(sortid);
	for (i=0;i<mamount;i++)
		m_cilmethodlist->Append(_(cilset.data->GetMethodName(sortid,i)));
	if (mamount>0)
		m_cilmethodlist->SetSelection(0);
	m_cilmethodedit->Enable(mamount>0);
}

void CDDataStruct::DisplayCilMacro(int cilmacroid) {
	CILMacro macro(CILMacroIDList[cilmacroid].id,cilset.data);
	bool canenable = true;
	unsigned int i;
	if (cilset.GetEnabledMacroIndex(macro.type->id)>=0)
		m_cilmacrobutton->SetLabel(_(HADES_STRING_CIL_UNAPPLY_MACRO));
	else
		m_cilmacrobutton->SetLabel(_(HADES_STRING_CIL_APPLY_MACRO));
	for (i=0;i<cilset.rawmodifamount;i++)
		if (macro.FindMethodById(cilset.rawmodif[i].method_id)>=0) {
			canenable = false;
			break;
		}
	m_cilmacrobutton->Enable(canenable);
	m_cilmacrodescription->SetValue(_(macro.type->description));
	m_cilmacromethods->Clear();
	for (i=0;i<macro.info->GetMethodCount();i++) {
		wxString methodlabel;
		methodlabel << _(macro.info->GetMethodTypeName(i)) << _(L"::") << _(macro.info->GetMethodName(i));
		m_cilmacromethods->Append(methodlabel);
	}
	m_cilmacroparametersizer->Clear(true);
	if (macro.type->id==50) {
		wxSpinCtrl* resspin = new wxSpinCtrl(m_cilmacroscrolledwindow,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,16,123,32);
		m_cilmacroparametersizer->Add(resspin,0,wxALL,3);
		m_cilmacroscrolledwindow->Layout();
		m_cilmacroscrolledwindow->GetParent()->GetSizer()->Layout();
		m_cilmacroscrolledwindow->Refresh();
		m_spellpower->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDDataStruct::OnCilParameterResolution ), NULL, this );
	}
}

void CDDataStruct::OnListBoxCilStruct(wxCommandEvent& event) {
	DisplayCilStruct(m_ciltypelist->GetSelection());
}

void CDDataStruct::OnListBoxCilMethod(wxCommandEvent& event) {
	
}

void CDDataStruct::OnListBoxCilMacro(wxCommandEvent& event) {
	DisplayCilMacro(m_cilmacrolist->GetSelection());
}

void CDDataStruct::OnCilMethodButton(wxCommandEvent& event) {
	int tid = *(unsigned int*)m_ciltypelist->GetClientData(m_ciltypelist->GetSelection());
	int mid = m_cilmethodlist->GetSelection();
	CilScriptEditDialog dial(this,&cilset,tid,mid);
	if (dial.error_type==0) {
		if (dial.ShowModal()==wxID_OK) {
			cilset.UpdateWithNewModification(*dial.ComputeModification());
			MarkDataCilModified();
		}
	} else if (dial.error_type==1) {
		wxLogError(HADES_STRING_CIL_NO_METHOD);
	} else if (dial.error_type==2) {
		wxMessageDialog popup(NULL,HADES_STRING_CIL_PROTECTED,HADES_STRING_WARNING,wxOK|wxCENTRE);
		popup.ShowModal();
		dial.ShowModal();
	}
}

void CDDataStruct::OnCilMacroButton(wxCommandEvent& event) {
	int macrosel = m_cilmacrolist->GetSelection();
	uint32_t macroid = CILMacroIDList[macrosel].id;
	if (cilset.GetEnabledMacroIndex(macroid)>=0) {
		cilset.RemoveMacroModif(macroid);
		m_cilmacrobutton->SetLabel(_(HADES_STRING_CIL_APPLY_MACRO));
	} else {
		cilset.AddMacroModif(macroid);
		if (macroid==50) {
			uint32_t param[] = {(uint32_t)dynamic_cast<wxSpinCtrl*>(m_cilmacroparametersizer->GetItem((size_t)0)->GetWindow())->GetValue()};
			cilset.macromodif[cilset.GetEnabledMacroIndex(macroid)].info->SetParameters(param);
		}
		m_cilmacrobutton->SetLabel(_(HADES_STRING_CIL_UNAPPLY_MACRO));
	}
	MarkDataCilModified();
}

void CDDataStruct::OnCilParameterResolution(wxSpinEvent& event) {
	int macroindex = cilset.GetEnabledMacroIndex(50);
	if (macroindex >= 0) {
		uint32_t param[] = { (uint32_t)event.GetPosition() };
		cilset.macromodif[macroindex].info->SetParameters(param);
	}
}

//=============================//
//           Main              //
//=============================//

void CDDataStruct::InitSpell(void) {
	if (saveset.sectionloaded[DATA_SECTION_SPELL])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_MENU_UI])
		InitMenuUI();
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	spellset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	SpellDisplayNames(true);
	for (i = 0; i < HADES_STRING_SPELL_EFFECT_COUNT; i++) {
		if (i < HADES_STRING_SPELL_EFFECT.size())
			m_spelleffect->Append(HADES_STRING_SPELL_EFFECT[i].label, (void*)&HADES_STRING_SPELL_EFFECT[i]);
		else
			m_spelleffect->Append(wxString::Format(wxT(HADES_STRING_SPELL_EFFECT_UNUSED_NAME), i), (void*)NULL);
	}
	for (i = 0; i < STATUS_SET_AMOUNT; i++) {
		UpdateSpellStatusName(spellset.status_set[i]);
		m_spellstatus->Append(spellset.status_set[i].name);
	}
	if (!TheSpellModelWindow) {
		TheSpellModelWindow = new SpellModelWindow(wxGetApp().GetTopWindow());
		void** vptany = new void*[HADES_STRING_SPELL_MODEL.size()];
		SpellModelAnyList.Alloc(HADES_STRING_SPELL_MODEL.size());
		for (i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++) {
			SpellModelAnyList.Add(HADES_STRING_SPELL_MODEL[i].label);
			vptany[i] = (void*)&HADES_STRING_SPELL_MODEL[i];
		}
		TheSpellModelWindow->m_modelall->Append(SpellModelAnyList, vptany);
	}
	saveset.sectionloaded[DATA_SECTION_SPELL] = true;
	m_spelllist->SetSelection(0);
}

void CDDataStruct::InitSupport(void) {
	if (saveset.sectionloaded[DATA_SECTION_SUPPORT])
		return;
	unsigned int i;
	fstream f;
	if (gametype==GAME_TYPE_PSX) f.open(filename.c_str(),ios::in | ios::binary);
	supportset.Load(f,config);
	if (gametype==GAME_TYPE_PSX) f.close();
	SupportDisplayNames(true);
	for (i = 0; i < supportset.support.size(); i++)
		m_supportboostedbase->Append(_(supportset.support[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	saveset.sectionloaded[DATA_SECTION_SUPPORT] = true;
	m_supportlist->SetSelection(0);
}

void CDDataStruct::InitCommand(void) {
	if (saveset.sectionloaded[DATA_SECTION_CMD])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_SPELL])
		InitSpell();
	if (!saveset.sectionloaded[DATA_SECTION_ITEM])
		InitItem();
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	cmdset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	m_cmdlink->Append(_("None"));
	CommandDisplayNames(true);
	for (i = 0; i < cmdset.cmd.size(); i++)
		m_cmdlink->Append(_(cmdset.cmd[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < spellset.spell.size(); i++)
		m_cmdspellfulllist->Append(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	saveset.sectionloaded[DATA_SECTION_CMD] = true;
	m_cmdlist->SetSelection(0);
}

void CDDataStruct::InitStat(void) {
	if (saveset.sectionloaded[DATA_SECTION_STAT])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_SPELL])
		InitSpell();
	if (!saveset.sectionloaded[DATA_SECTION_CMD])
		InitCommand();
	if (!saveset.sectionloaded[DATA_SECTION_SUPPORT])
		InitSupport();
	if (!saveset.sectionloaded[DATA_SECTION_ITEM])
		InitItem();
	unsigned int i, j;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	statset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	StatDisplayNames(true);
	for (i = 0; i < spellset.spell.size(); i++)
		m_statcharabilitychoice->Append(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < supportset.support.size(); i++)
		m_statcharabilitychoice->Append(_(supportset.support[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < cmdset.cmd.size(); i++)
		for (j = 0; j < commandchoicelist.size(); j++)
			commandchoicelist[j]->Append(_(cmdset.cmd[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < itemset.item.size(); i++) {
		m_statcharweapon->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_statcharhead->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_statcharwrist->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_statchararmor->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_statcharaccessory->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_statcharequipfulllist->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	saveset.sectionloaded[DATA_SECTION_STAT] = true;
	m_statlist->SetSelection(0);
}

void CDDataStruct::InitPartySpecial(void) {
	if (saveset.sectionloaded[DATA_SECTION_PARTY_SPECIAL])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_SPELL])
		InitSpell();
	if (!saveset.sectionloaded[DATA_SECTION_MENU_UI])
		InitMenuUI();
	if (!saveset.sectionloaded[DATA_SECTION_STAT])
		InitStat();
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	partyspecialset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	for (i = 0; i < HADES_STRING_PARTY_SPECIAL_DATA.size(); i++)
		m_partyspeciallist->Append(HADES_STRING_PARTY_SPECIAL_DATA[i]);
	m_partyspecialmagicswordset->Append(wxString::Format(wxT(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SET), 0));
	for (i = 0; i < statset.initial_stat.size(); i++) {
		m_partyspecialmagicswordcaster->Append(_(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SUPPORTER + statset.initial_stat[i].default_name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_partyspecialmagicswordwielder->Append(_(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_BENEFICIARY + statset.initial_stat[i].default_name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	for (i = 0; i < spellset.spell.size(); i++) {
		m_partyspecialmagicswordrequirement->Append(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_partyspecialmagicswordspell->Append(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	for (int i = 0; i < STATUS_AMOUNT; i++) {
		m_partyspecialmagicswordcasterstatusbaselist->Append(HADES_STRING_STATUS_NAME[i]);
		m_partyspecialmagicswordknightstatusbaselist->Append(HADES_STRING_STATUS_NAME[i]);
	}
	saveset.sectionloaded[DATA_SECTION_PARTY_SPECIAL] = true;
	m_partyspecialmagicswordset->SetSelection(0);
	m_partyspeciallist->SetSelection(0);
}

void CDDataStruct::InitItem(void) {
	if (saveset.sectionloaded[DATA_SECTION_ITEM])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_SPELL])
		InitSpell();
	if (!saveset.sectionloaded[DATA_SECTION_SUPPORT])
		InitSupport();
	if (!saveset.sectionloaded[DATA_SECTION_TEXT])
		InitText();
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	itemset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	ItemDisplayNames(true);
	KeyItemDisplayNames(true);
	for (i = 0; i < ITEM_STAT_AMOUNT; i++) {
		wxString statlabel;
		statlabel << HADES_STRING_ITEM_STAT_SET << i;
		m_itemstatid->Append(statlabel);
	}
	if (gametype == GAME_TYPE_STEAM && config.dll_usage != 0)
		m_itemstatid->Append(_(HADES_STRING_ITEM_STAT_SET_PERSO));
	for (i = 0; i < spellset.spell.size(); i++)
		m_itemskillfulllist->Append(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < supportset.support.size(); i++)
		m_itemskillfulllist->Append(_(supportset.support[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	bool equipused[ITEM_AMOUNT];
	for (i = 0; i < ITEM_AMOUNT - 1; i++) {
		m_itemmenuposlist->Append(wxEmptyString);
		m_itemequipposlist->Append(_("_"));
		equipused[i] = false;
	}
	for (i = 0; i < ITEM_AMOUNT - 1; i++) {
		m_itemmenuposlist->SetString(itemset.item[i].menu_position, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		if (equipused[itemset.item[i].equip_position])
			m_itemequipposlist->SetString(itemset.item[i].equip_position, m_itemequipposlist->GetString(itemset.item[i].equip_position) + _(L" - ") + _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		else
			m_itemequipposlist->SetString(itemset.item[i].equip_position, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		equipused[itemset.item[i].equip_position] = true;
	}
	for (i = 0; i < itemset.item.size(); i++) {
		m_itemmenuposchoice->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_itemequipposchoice->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	for (i = 0; i < HADES_STRING_SPELL_EFFECT_COUNT; i++) {
		if (i < HADES_STRING_SPELL_EFFECT.size())
			m_itemusableeffect->Append(HADES_STRING_SPELL_EFFECT[i].label, (void*)&HADES_STRING_SPELL_EFFECT[i]);
		else
			m_itemusableeffect->Append(wxString::Format(wxT(HADES_STRING_SPELL_EFFECT_UNUSED_NAME), i), (void*)NULL);
	}
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		m_itemweaponstatus->Append(spellset.status_set[i].name);
	for (int i = 0; i < STATUS_AMOUNT; i++)
		m_itemusablestatusbaselist->Append(HADES_STRING_STATUS_NAME[i]);
	for (i = 0; i < ITEM_WEAPON_AMOUNT; i++)
		m_itemweaponsfx->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	if (!TheSpellModelWindow) {
		TheSpellModelWindow = new SpellModelWindow(wxGetApp().GetTopWindow());
		void** vptany = new void*[HADES_STRING_SPELL_MODEL.size()];
		SpellModelAnyList.Alloc(HADES_STRING_SPELL_MODEL.size());
		for (i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++) {
			SpellModelAnyList.Add(HADES_STRING_SPELL_MODEL[i].label);
			vptany[i] = (void*)&HADES_STRING_SPELL_MODEL[i];
		}
		TheSpellModelWindow->m_modelall->Append(SpellModelAnyList, vptany);
	}
	saveset.sectionloaded[DATA_SECTION_ITEM] = true;
	m_itemlist->SetSelection(0);
	m_keyitemlist->SetSelection(0);
}

void CDDataStruct::InitShop(void) {
	if (saveset.sectionloaded[DATA_SECTION_SHOP])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_ITEM])
		InitItem();
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	shopset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	ShopDisplayNames(true);
	SynthesisShopDisplayNames(true);
	for (i = 0; i < itemset.item.size(); i++)
		m_shopitemfulllist->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < itemset.item.size(); i++) {
		m_synthshopsynthitem->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_synthshoprecipefulllist->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	for (i = 0; i < HADES_STRING_SYNTH_NAME.size(); i++)
		m_synthshopdefaultlist->Append(HADES_STRING_SYNTH_NAME[i]);
	m_synthshopdefaultlist->SetSelection(0);
	m_synthshopshopint->SetValue(32);
	saveset.sectionloaded[DATA_SECTION_SHOP] = true;
	m_shoplist->SetSelection(0);
	m_synthshoplist->SetSelection(0);
}

void CDDataStruct::InitEnemy(void) {
	if (saveset.sectionloaded[DATA_SECTION_ENMY])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_SPELL])
		InitSpell();
	if (!saveset.sectionloaded[DATA_SECTION_SUPPORT])
		InitSupport();
	if (!saveset.sectionloaded[DATA_SECTION_ITEM])
		InitItem();
	if (!saveset.sectionloaded[DATA_SECTION_CARD])
		InitCard();
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	enemyset.Load(f, cluster);
	if (gametype == GAME_TYPE_PSX) f.close();
	EnemyDisplayNames(true);
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME); i++)
		m_enemyscene->Append(HADES_STRING_BATTLE_SCENE_NAME[i].label, (void*)&HADES_STRING_BATTLE_SCENE_NAME[i]);
	for (i = 0; i < itemset.item.size(); i++) {
		m_enemystatitemsteal1->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemsteal2->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemsteal3->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemsteal4->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemdrop1->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemdrop2->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemdrop3->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
		m_enemystatitemdrop4->Append(_(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_MODEL_NAME); i++)
		if (BattleModelLinks::IsBattleModel(HADES_STRING_MODEL_NAME[i].id))
			m_enemystatmodel->Append(HADES_STRING_MODEL_NAME[i].label, (void*)&HADES_STRING_MODEL_NAME[i]);
	for (i = 0; i < spellset.spell.size(); i++)
		m_enemystatbluemagic->Append(_(spellset.spell[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < supportset.support.size(); i++)
		m_enemystatbluemagic->Append(_(supportset.support[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i = 0; i < HADES_STRING_SPELL_EFFECT_COUNT; i++) {
		if (i < HADES_STRING_SPELL_EFFECT.size())
			m_enemyspelleffect->Append(HADES_STRING_SPELL_EFFECT[i].label, (void*)&HADES_STRING_SPELL_EFFECT[i]);
		else
			m_enemyspelleffect->Append(wxString::Format(wxT(HADES_STRING_SPELL_EFFECT_UNUSED_NAME), i), (void*)NULL);
	}
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		m_enemyspellstatus->Append(spellset.status_set[i].name);
	m_enemystatdropcard->Append(HADES_STRING_CARD_NOCARD);
	for (i = 0; i < CARD_AMOUNT; i++)
		m_enemystatdropcard->Append(cardset.card[i].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	for (int i = 0; i < STATUS_AMOUNT; i++) {
		m_enemystatimmunebaselist->Append(HADES_STRING_STATUS_NAME[i]);
		m_enemystatautobaselist->Append(HADES_STRING_STATUS_NAME[i]);
		m_enemystatinitialbaselist->Append(HADES_STRING_STATUS_NAME[i]);
	}
	saveset.sectionloaded[DATA_SECTION_ENMY] = true;
	if (saveset.sectionloaded[DATA_SECTION_WORLD_MAP]) {
		bool showalt = m_worldbattlepanelspin3->IsShown();
		wxArrayString battlenames;
		battlenames.Alloc(enemyset.battle_amount);
		for (i = 0; i < enemyset.battle_amount; i++)
			battlenames.Add(GetEnemyBattleName(i));
		m_worldbattlebattlechoice11->Append(battlenames);
		m_worldbattlebattlechoice12->Append(battlenames);
		m_worldbattlebattlechoice13->Append(battlenames);
		m_worldbattlebattlechoice14->Append(battlenames);
		m_worldbattlebattlechoice21->Append(battlenames);
		m_worldbattlebattlechoice22->Append(battlenames);
		m_worldbattlebattlechoice23->Append(battlenames);
		m_worldbattlebattlechoice24->Append(battlenames);
		m_worldbattlebattlechoice31->Append(battlenames);
		m_worldbattlebattlechoice32->Append(battlenames);
		m_worldbattlebattlechoice33->Append(battlenames);
		m_worldbattlebattlechoice34->Append(battlenames);
		m_worldbattlebattlechoice41->Append(battlenames);
		m_worldbattlebattlechoice42->Append(battlenames);
		m_worldbattlebattlechoice43->Append(battlenames);
		m_worldbattlebattlechoice44->Append(battlenames);
		m_worldbattlepanelspin1->Show(false);
		m_worldbattlepanelspin2->Show(false);
		m_worldbattlepanelspin3->Show(false);
		m_worldbattlepanelspin4->Show(false);
		m_worldbattlepanelchoice1->Show(true);
		m_worldbattlepanelchoice2->Show(true);
		m_worldbattlepanelchoice3->Show(showalt);
		m_worldbattlepanelchoice4->Show(showalt);
		DisplayWorldBattle(m_worldbattlelist->GetSelection());
	}
	m_enemylist->SetSelection(0);
	GetTopWindow()->m_exportenemyscript->Enable();
	GetTopWindow()->m_importenemyscript->Enable();
}

void CDDataStruct::InitCard(void) {
	if (saveset.sectionloaded[DATA_SECTION_CARD])
		return;
	unsigned int i;
	fstream f;
	if (gametype == GAME_TYPE_PSX) f.open(filename.c_str(), ios::in | ios::binary);
	cardset.Load(f, config);
	if (gametype == GAME_TYPE_PSX) f.close();
	CardDisplayNames(true);
	for (i = 0; i < HADES_STRING_DECK_NAME.size(); i++)
		m_carddecklist->Append(HADES_STRING_DECK_NAME[i].label);
	for (i = 0; i < CARD_SET_AMOUNT; i++) {
		wxString cardsetinfo;
		cardsetinfo << _(L"Card Set ") << (i + 1);
		m_carddeckset->Append(cardsetinfo);
	}
	saveset.sectionloaded[DATA_SECTION_CARD] = true;
	m_cardlist->SetSelection(0);
	m_carddecklist->SetSelection(0);
}

void CDDataStruct::InitText(void) {
	if (saveset.sectionloaded[DATA_SECTION_TEXT])
		return;
	fstream f;
	if (gametype==GAME_TYPE_PSX) f.open(filename.c_str(),ios::in | ios::binary);
	textset.Load(f,cluster);
	if (gametype==GAME_TYPE_PSX) f.close();
	TextDisplayNames(true);
	saveset.sectionloaded[DATA_SECTION_TEXT] = true;
	m_textlist->SetSelection(0);
	GetTopWindow()->m_exporttext->Enable();
	GetTopWindow()->m_importtext->Enable();
}

void CDDataStruct::InitWorldMap(void) {
	if (saveset.sectionloaded[DATA_SECTION_WORLD_MAP])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_TEXT])
		InitText();
	unsigned int i;
	fstream f;
	if (gametype==GAME_TYPE_PSX) f.open(filename.c_str(),ios::in | ios::binary);
	worldset.Load(f,cluster);
	if (gametype==GAME_TYPE_PSX) f.close();
	WorldMapDisplayNames(true);
	for (i=0;i<WORLD_MAP_PLACE_AMOUNT;i++)
		m_worldplacelist->Append(_(worldset.world_data->place_name[i].GetStr(hades::TEXT_PREVIEW_TYPE)));
	for (i=0;i<WORLD_MAP_BATTLE_SPOT_AMOUNT;i++)
		m_worldbattlelist->Append(wxString::Format(wxT(HADES_STRING_WORLD_BATTLE_NAME),i+1)); 
	if (saveset.sectionloaded[DATA_SECTION_ENMY]) {
		wxArrayString battlenames;
		battlenames.Alloc(enemyset.battle_amount);
		for (i=0;i<enemyset.battle_amount;i++)
			battlenames.Add(GetEnemyBattleName(i));
		m_worldbattlebattlechoice11->Append(battlenames);
		m_worldbattlebattlechoice12->Append(battlenames);
		m_worldbattlebattlechoice13->Append(battlenames);
		m_worldbattlebattlechoice14->Append(battlenames);
		m_worldbattlebattlechoice21->Append(battlenames);
		m_worldbattlebattlechoice22->Append(battlenames);
		m_worldbattlebattlechoice23->Append(battlenames);
		m_worldbattlebattlechoice24->Append(battlenames);
		m_worldbattlebattlechoice31->Append(battlenames);
		m_worldbattlebattlechoice32->Append(battlenames);
		m_worldbattlebattlechoice33->Append(battlenames);
		m_worldbattlebattlechoice34->Append(battlenames);
		m_worldbattlebattlechoice41->Append(battlenames);
		m_worldbattlebattlechoice42->Append(battlenames);
		m_worldbattlebattlechoice43->Append(battlenames);
		m_worldbattlebattlechoice44->Append(battlenames);
	}
	m_worldbattlepanelspin1->Show(!saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelspin2->Show(!saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelspin3->Show(!saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelspin4->Show(!saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelchoice1->Show(saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelchoice2->Show(saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelchoice3->Show(saveset.sectionloaded[DATA_SECTION_ENMY]);
	m_worldbattlepanelchoice4->Show(saveset.sectionloaded[DATA_SECTION_ENMY]);
	saveset.sectionloaded[DATA_SECTION_WORLD_MAP] = true;
	m_worldlist->SetSelection(0);
	m_worldplacelist->SetSelection(0);
	m_worldbattlelist->SetSelection(0);
	GetTopWindow()->m_exportworldscript->Enable();
	GetTopWindow()->m_importworldscript->Enable();
}

void CDDataStruct::InitField(void) {
	if (saveset.sectionloaded[DATA_SECTION_FIELD])
		return;
	if (!saveset.sectionloaded[DATA_SECTION_TEXT])
		InitText();
	fstream f;
	if (gametype==GAME_TYPE_PSX) {
		f.open(filename.c_str(),ios::in | ios::binary);
		fieldset.Load(f,cluster);
		f.close();
	} else {
		fieldset.Load(f,cluster,&textset);
	}
	FieldDisplayNames(true);
	saveset.sectionloaded[DATA_SECTION_FIELD] = true;
	m_fieldlist->SetSelection(0);
	GetTopWindow()->m_exportfieldscript->Enable();
	GetTopWindow()->m_importfieldscript->Enable();
	GetTopWindow()->m_exportfieldbackground->Enable();
	GetTopWindow()->m_exportfieldwalkmesh->Enable();
}

void CDDataStruct::InitBattleScene(void) {
	if (saveset.sectionloaded[DATA_SECTION_BATTLE_SCENE] || gametype!=GAME_TYPE_PSX)
		return;
	fstream f;
	if (gametype==GAME_TYPE_PSX) f.open(filename.c_str(),ios::in | ios::binary);
	sceneset.Load(f,cluster);
	if (gametype==GAME_TYPE_PSX) f.close();
	BattleSceneDisplayNames(true);
	saveset.sectionloaded[DATA_SECTION_BATTLE_SCENE] = true;
	m_battlescenelist->SetSelection(0);
}

void CDDataStruct::InitSpellAnimation(void) {
	if (saveset.sectionloaded[DATA_SECTION_SPELL_ANIM])
		return;
	fstream f;
	if (gametype==GAME_TYPE_PSX) f.open(filename.c_str(),ios::in | ios::binary);
	spellanimset.Load(f,config,cluster.global_map);
	if (gametype==GAME_TYPE_PSX) f.close();
	SpellAnimationDisplayNames(true);
	saveset.sectionloaded[DATA_SECTION_SPELL_ANIM] = true;
	m_spellanimlist->SetSelection(0);
}

void CDDataStruct::InitMenuUI(void) {
	if (saveset.sectionloaded[DATA_SECTION_MENU_UI])
		return;
	unsigned int i;
	fstream f;
	if (gametype==GAME_TYPE_PSX) f.open(filename.c_str(),ios::in | ios::binary);
	ffuiset.Load(f,config);
	if (gametype==GAME_TYPE_PSX) f.close();
	if (gametype==GAME_TYPE_PSX)
		for (i=0;i<ffuiset.special_text->amount;i++)
			m_specialtextlist->Append(HADES_STRING_SPECIAL_TEXT_BLOCK[i]);
	else
		for (i=0;i<ffuiset.special_text->amount;i++)
			m_specialtextlist->Append(HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[i]);
	saveset.sectionloaded[DATA_SECTION_MENU_UI] = true;
	m_specialtextlist->SetSelection(0);
	GetTopWindow()->m_exportuitext->Enable();
	GetTopWindow()->m_importuitext->Enable();
}

void CDDataStruct::InitMips(void) {
	if (saveset.sectionloaded[DATA_SECTION_ASSEMBLY] || gametype != GAME_TYPE_PSX)
		return;
	unsigned int i;
	fstream f;
	f.open(filename.c_str(), ios::in | ios::binary);
	mipsset.Load(f, config);
	f.close();
	m_mipsbattlelist->Append(HADES_STRING_MIPS_FULL_CODE);
	for (i = 0; i < HADES_STRING_SPELL_EFFECT.size(); i++)
		m_mipsbattlelist->Append(HADES_STRING_SPELL_EFFECT[i].label);
	m_mipsbattleramposgen->SetValue(wxString::Format(wxT("0x%X"), mipsset.battle_code.ram_pos & 0x7FFFFFFF));
	saveset.sectionloaded[DATA_SECTION_ASSEMBLY] = true;
	m_mipsbattlelist->SetSelection(0);
}

void CDDataStruct::InitCil(void) {
	if (saveset.sectionloaded[DATA_SECTION_ASSEMBLY] || gametype == GAME_TYPE_PSX)
		return;
	fstream f;
	cilset.Init(&config.meta_dll);
	CilDisplayNames(true);
	for (unsigned int i = 0; i < CILMacroIDList.size(); i++)
		m_cilmacrolist->Append(_(CILMacroIDList[i].name));
	saveset.sectionloaded[DATA_SECTION_ASSEMBLY] = true;
	m_ciltypelist->SetSelection(0);
	m_cilmacrolist->SetSelection(0);
}

void CDDataStruct::UpdateSpellName(unsigned int spellid) {
	wxString newname = _(spellset.spell[spellid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (!m_cmdspellfulllist->IsEmpty())
		m_cmdspellfulllist->SetString(spellid, newname);
	if (!m_statcharabilitychoice->IsEmpty())
		m_statcharabilitychoice->SetString(spellid, newname);
	if (!m_partyspecialmagicswordlist->IsEmpty()) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		for (unsigned int i = 0; i < partyspecialset.magic_sword[swdset].unlocked.size(); i++)
			if (partyspecialset.magic_sword[swdset].unlocked[i] == spellset.spell[spellid].id)
				m_partyspecialmagicswordlist->SetString(i, GetMagicSwordName(spellset.spell[spellid].name));
	}
	if (!m_partyspecialmagicswordrequirement->IsEmpty())
		m_partyspecialmagicswordrequirement->SetString(spellid, newname);
	if (!m_partyspecialmagicswordspell->IsEmpty())
		m_partyspecialmagicswordspell->SetString(spellid, newname);
	if (!m_enemystatbluemagic->IsEmpty())
		m_enemystatbluemagic->SetString(spellid, newname);
	if (!m_itemskillfulllist->IsEmpty())
		m_itemskillfulllist->SetString(spellid, newname);
}

void CDDataStruct::RegisterSpellAdded(unsigned int spellid) {
	wxString newname = _(spellset.spell[spellid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (!m_cmdspellfulllist->IsEmpty())
		m_cmdspellfulllist->Insert(newname, spellid);
	if (!m_statcharabilitychoice->IsEmpty())
		m_statcharabilitychoice->Insert(newname, spellid);
	if (!m_partyspecialmagicswordlist->IsEmpty()) {
		int swdset = m_partyspecialmagicswordset->GetSelection();
		for (unsigned int i = 0; i < partyspecialset.magic_sword[swdset].unlocked.size(); i++)
			if (partyspecialset.magic_sword[swdset].unlocked[i] == spellset.spell[spellid].id)
				m_partyspecialmagicswordlist->SetString(i, GetMagicSwordName(spellset.spell[spellid].name));
	}
	if (!m_partyspecialmagicswordrequirement->IsEmpty())
		m_partyspecialmagicswordrequirement->Insert(newname, spellid);
	if (!m_partyspecialmagicswordspell->IsEmpty())
		m_partyspecialmagicswordspell->Insert(newname, spellid);
	if (!m_enemystatbluemagic->IsEmpty())
		m_enemystatbluemagic->Insert(newname, spellid);
	if (!m_itemskillfulllist->IsEmpty())
		m_itemskillfulllist->Insert(newname, spellid);
}

void CDDataStruct::RegisterSpellRemoved(unsigned int spellid) {
	if (!m_cmdspellfulllist->IsEmpty())
		m_cmdspellfulllist->Delete(spellid);
	if (!m_statcharabilitychoice->IsEmpty())
		m_statcharabilitychoice->Delete(spellid);
	if (!m_partyspecialmagicswordrequirement->IsEmpty())
		m_partyspecialmagicswordrequirement->Delete(spellid);
	if (!m_partyspecialmagicswordspell->IsEmpty())
		m_partyspecialmagicswordspell->Delete(spellid);
	if (!m_enemystatbluemagic->IsEmpty())
		m_enemystatbluemagic->Delete(spellid);
	if (!m_itemskillfulllist->IsEmpty())
		m_itemskillfulllist->Delete(spellid);
}

void CDDataStruct::UpdateSpellStatusName(StatusSetStruct& statusset) {
	set<int>& statuslist = statusset.status_list;
	wxString name = _(L"");
	unsigned int i;
	if (statuslist.empty()) {
		name = _(HADES_STRING_STATUS_NONE);
		statusset.name = name.ToStdWstring();
	} else {
		uint32_t statusbits = GetStatusBitList(statusset.status_list);
		uint32_t statusdone = 0;
		for (i = 0; i < HADES_STRING_STATUS_PACK.size(); i++)
			if ((statusbits & HADES_STRING_STATUS_PACK[i].id) == HADES_STRING_STATUS_PACK[i].id) {
				if (name.Len() > 0)
					name += _(L" + ") + _(HADES_STRING_STATUS_PACK[i].label);
				else
					name += _(HADES_STRING_STATUS_PACK[i].label);
				statusbits ^= HADES_STRING_STATUS_PACK[i].id;
				statusdone |= HADES_STRING_STATUS_PACK[i].id;
			}
		set<int>::iterator it;
		for (it = statuslist.begin(); it != statuslist.end() && name.Len() < 30; it++) {
			if (*it < STATUS_AMOUNT && (statusdone & (1u << *it)) != 0)
				continue;
			if (name.Len() > 0)
				name += _(L" + ");
			if (*it < (int)HADES_STRING_STATUS.size())
				name += HADES_STRING_STATUS[*it];
			else
				name += wxString::Format(wxT("Status %d"), *it);
		}
		if (it != statuslist.end())
			name += _(L" + ...");
		statusset.name = name.ToStdWstring();
	}
	if (statusset.id < (int)m_spellstatus->GetCount()) {
		if (!m_spellstatus->IsEmpty())
			m_spellstatus->SetString(statusset.id, name);
		if (!m_itemweaponstatus->IsEmpty())
			m_itemweaponstatus->SetString(statusset.id, name);
		if (!m_enemyspellstatus->IsEmpty())
			m_enemyspellstatus->SetString(statusset.id, name);
	}
}

void CDDataStruct::UpdateSupportName(unsigned int supportid) {
	wxString newname = _(supportset.support[supportid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (!m_supportboostedbase->IsEmpty())
		m_supportboostedbase->SetString(supportid, newname);
	if (!m_statcharabilitychoice->IsEmpty())
		m_statcharabilitychoice->SetString(spellset.spell.size() + supportid, newname);
	if (!m_itemskillfulllist->IsEmpty())
		m_itemskillfulllist->SetString(spellset.spell.size() + supportid, newname);
	if (!m_enemystatbluemagic->IsEmpty())
		m_enemystatbluemagic->SetString(spellset.spell.size() + supportid, newname);
}

void CDDataStruct::RegisterSupportAdded(unsigned int supportid) {
	wxString newname = _(supportset.support[supportid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (!m_supportboostedbase->IsEmpty())
		m_supportboostedbase->Insert(newname, supportid);
	if (!m_statcharabilitychoice->IsEmpty())
		m_statcharabilitychoice->Insert(newname, spellset.spell.size() + supportid);
	if (!m_itemskillfulllist->IsEmpty())
		m_itemskillfulllist->Insert(newname, spellset.spell.size() + supportid);
	if (!m_enemystatbluemagic->IsEmpty())
		m_enemystatbluemagic->Insert(newname, spellset.spell.size() + supportid);
}

void CDDataStruct::RegisterSupportRemoved(unsigned int supportid) {
	if (!m_supportboostedbase->IsEmpty())
		m_supportboostedbase->Delete(supportid);
	if (!m_statcharabilitychoice->IsEmpty())
		m_statcharabilitychoice->Delete(spellset.spell.size() + supportid);
	if (!m_itemskillfulllist->IsEmpty())
		m_itemskillfulllist->Delete(spellset.spell.size() + supportid);
	if (!m_enemystatbluemagic->IsEmpty())
		m_enemystatbluemagic->Delete(spellset.spell.size() + supportid);
}

void CDDataStruct::UpdateCommandName(unsigned int cmdid) {
	wxString newname = _(cmdset.cmd[cmdid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	for (unsigned int i = 0; i < commandchoicelist.size(); i++)
		if (!commandchoicelist[i]->IsEmpty())
			commandchoicelist[i]->SetString(cmdid, newname);
}

void CDDataStruct::RegisterCommandAdded(unsigned int cmdid) {
	wxString newname = _(cmdset.cmd[cmdid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	for (unsigned int i = 0; i < commandchoicelist.size(); i++)
		if (!commandchoicelist[i]->IsEmpty())
			commandchoicelist[i]->Insert(newname, cmdid);
}

void CDDataStruct::RegisterCommandRemoved(unsigned int cmdid) {
	for (unsigned int i = 0; i < commandchoicelist.size(); i++)
		if (!commandchoicelist[i]->IsEmpty())
			commandchoicelist[i]->Delete(cmdid);
}

void CDDataStruct::UpdateStatName(unsigned int statid) {
	wxString newname = _(statset.initial_stat[statid].default_name.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (!m_partyspecialmagicswordcaster->IsEmpty())
		m_partyspecialmagicswordcaster->SetString(statid, _(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SUPPORTER) + newname);
	if (!m_partyspecialmagicswordwielder->IsEmpty())
		m_partyspecialmagicswordwielder->SetString(statid, _(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_BENEFICIARY) + newname);
}

void CDDataStruct::RegisterStatAdded(unsigned int statid) {
	wxString newname = _(statset.initial_stat[statid].default_name.GetStr(hades::TEXT_PREVIEW_TYPE));
	if (!m_partyspecialmagicswordcaster->IsEmpty())
		m_partyspecialmagicswordcaster->Insert(_(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SUPPORTER) + newname, statid);
	if (!m_partyspecialmagicswordwielder->IsEmpty())
		m_partyspecialmagicswordwielder->Insert(_(HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_BENEFICIARY) + newname, statid);
}

void CDDataStruct::RegisterStatRemoved(unsigned int statid) {
	if (!m_partyspecialmagicswordcaster->IsEmpty())
		m_partyspecialmagicswordcaster->Delete(statid);
	if (!m_partyspecialmagicswordwielder->IsEmpty())
		m_partyspecialmagicswordwielder->Delete(statid);
}

void CDDataStruct::UpdateItemName(unsigned int itemid) {
	wxString newname = _(itemset.item[itemid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	unsigned int i;
	for (i = 0; i < itemchoicelist.size(); i++)
		if (!itemchoicelist[i]->IsEmpty())
			itemchoicelist[i]->SetString(itemid, newname);
	if (!m_statcharequipfulllist->IsEmpty())
		m_statcharequipfulllist->SetString(itemid, newname);
	if (!m_shopitemfulllist->IsEmpty())
		m_shopitemfulllist->SetString(itemid, newname);
	if (!m_synthshoplist->IsEmpty()) {
		for (i = 0; i < shopset.synthesis.size(); i++)
			if (shopset.synthesis[i].synthesized == itemset.item[itemid].id)
				m_synthshoplist->SetString(i, newname);
		m_synthshopsynthitem->SetString(itemid, newname);
		m_synthshoprecipefulllist->SetString(itemid, newname);
	}
	if (!m_itemmenuposlist->IsEmpty() && itemid != 0xFF && itemset.item[itemid].menu_position_type == ITEM_POSITION_ABSOLUTE)
		m_itemmenuposlist->SetString(itemset.item[itemid].menu_position, newname);
	if (!m_itemequipposlist->IsEmpty() && itemid != 0xFF && itemset.item[itemid].equip_position_type == ITEM_POSITION_ABSOLUTE) {
		bool equipused = false;
		int epos = itemset.item[itemid].equip_position;
		for (i = 0; i < ITEM_AMOUNT - 1; i++) {
			if (itemset.item[i].equip_position == epos) {
				if (equipused)
					m_itemequipposlist->SetString(epos, m_itemequipposlist->GetString(epos) + _(L" - ") + _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
				else
					m_itemequipposlist->SetString(epos, _(itemset.item[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
				equipused = true;
			}
		}
	}
	if (!m_itemweaponsfx->IsEmpty() && itemid < ITEM_WEAPON_AMOUNT)
		m_itemweaponsfx->SetString(itemid, newname);
}

void CDDataStruct::RegisterItemAdded(unsigned int itemid) {
	wxString newname = _(itemset.item[itemid].name.GetStr(hades::TEXT_PREVIEW_TYPE));
	unsigned int i;
	for (i = 0; i < itemchoicelist.size(); i++)
		if (!itemchoicelist[i]->IsEmpty())
			itemchoicelist[i]->Insert(newname, itemid);
	if (!m_statcharequipfulllist->IsEmpty())
		m_statcharequipfulllist->Insert(newname, itemid);
	if (!m_shopitemfulllist->IsEmpty())
		m_shopitemfulllist->Insert(newname, itemid);
	if (!m_synthshoplist->IsEmpty()) {
		for (i = 0; i < shopset.synthesis.size(); i++)
			if (shopset.synthesis[i].synthesized == itemset.item[itemid].id)
				m_synthshoplist->SetString(i, newname);
		m_synthshopsynthitem->Insert(newname, itemid);
		m_synthshoprecipefulllist->Insert(newname, itemid);
	}
}

void CDDataStruct::RegisterItemRemoved(unsigned int itemid) {
	for (unsigned int i = 0; i < itemchoicelist.size(); i++)
		if (!itemchoicelist[i]->IsEmpty())
			itemchoicelist[i]->Delete(itemid);
	if (!m_statcharequipfulllist->IsEmpty())
		m_statcharequipfulllist->Delete(itemid);
	if (!m_shopitemfulllist->IsEmpty())
		m_shopitemfulllist->Delete(itemid);
	if (!m_synthshoplist->IsEmpty()) {
		m_synthshopsynthitem->Delete(itemid);
		m_synthshoprecipefulllist->Delete(itemid);
	}
}

void CDDataStruct::RegisterKeyItemAdded(unsigned int keyitemid) {
}

void CDDataStruct::RegisterKeyItemRemoved(unsigned int keyitemid) {
}

void CDDataStruct::RegisterShopAdded(unsigned int shopid) {
	if (shopset.shop[shopid].id < G_V_ELEMENTS(HADES_STRING_SHOP_NAME))
		m_shoplist->Insert(HADES_STRING_SHOP_NAME[shopset.shop[shopid].id].label, shopid);
	else
		m_shoplist->Insert(wxString::Format(wxT(HADES_STRING_SHOP_CUSTOM_NAME), shopset.shop[shopid].id), shopid);
}

void CDDataStruct::RegisterShopRemoved(unsigned int shopid) {
	m_shoplist->Delete(shopid);
}

void CDDataStruct::RegisterSynthesisShopAdded(unsigned int synthshopid) {
	m_synthshoplist->Insert(_(itemset.GetItemById(shopset.synthesis[synthshopid].synthesized).name.GetStr(hades::TEXT_PREVIEW_TYPE)), synthshopid);
}

void CDDataStruct::RegisterSynthesisShopRemoved(unsigned int synthshopid) {
	m_synthshoplist->Delete(synthshopid);
}

void CDDataStruct::DisplayCurrentData() {
	unsigned int sel1 = m_notebookmain->GetSelection();
	unsigned int sel2;
	if (sel1 == 0) {
		sel2 = m_notebookparty->GetSelection();
		if (sel2 == 0)
			DisplaySpell(m_spelllist->GetSelection());
		else if (sel2 == 1)
			DisplaySupport(m_supportlist->GetSelection());
		else if (sel2 == 2)
			DisplayCommand(m_cmdlist->GetSelection());
		else if (sel2 == 3)
			DisplayStat(m_statlist->GetSelection());
		else if (sel2 == 4)
			DisplayPartySpecial(m_partyspeciallist->GetSelection());
	} else if (sel1 == 1) {
		sel2 = m_notebookinventory->GetSelection();
		if (sel2 == 0) {
			DisplayItem(m_itemlist->GetSelection());
			DisplayKeyItem(m_keyitemlist->GetSelection());
		} else if (sel2 == 1) {
			DisplayShop(m_shoplist->GetSelection());
			DisplaySynthesisShop(m_synthshoplist->GetSelection());
		}
	} else if (sel1 == 2) {
		DisplayEnemy(m_enemylist->GetSelection());
	} else if (sel1 == 3) {
		sel2 = m_notebookcard->GetSelection();
		if (sel2 == 0)
			DisplayCard(m_cardlist->GetSelection());
		else if (sel2 == 1)
			DisplayCardDeck(m_carddecklist->GetSelection());
	} else if (sel1 == 4) {
		sel2 = m_notebookenvironment->GetSelection();
		if (sel2 == 0) {
			DisplayText(m_textlist->GetSelection());
		} else if (sel2 == 1) {
			DisplayWorldMap(m_worldlist->GetSelection());
			DisplayWorldPlace(m_worldplacelist->GetSelection());
			DisplayWorldBattle(m_worldbattlelist->GetSelection());
		} else if (sel2 == 2) {
			DisplayField(m_fieldlist->GetSelection());
		} else if (sel2 == 3) {
			DisplayBattleScene(m_battlescenelist->GetSelection());
		} else if (sel2 == 4) {
			DisplaySpellAnimation(m_spellanimlist->GetSelection());
		}
	} else if (sel1 == 5) {
		sel2 = m_notebookinterface->GetSelection();
		if (sel2 == 0)
			DisplaySpecialText(m_specialtextlist->GetSelection());
	} else if (sel1 == 6) {
		if (gametype == GAME_TYPE_PSX) {
			sel2 = m_notebookmips->GetSelection();
			if (sel2 == 0)
				DisplayMipsBattle(m_mipsbattlelist->GetSelection());
		} else {
			sel2 = m_notebookcil->GetSelection();
			if (sel2 == 0) {
				DisplayCilStruct(m_ciltypelist->GetSelection());
			} else if (sel2 == 1) {
				DisplayCilMacro(m_cilmacrolist->GetSelection());
			}
		}
	}
}

void CDDataStruct::OnNotebookMain(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitSpell();
		DisplaySpell(m_spelllist->GetSelection());
	} else if (sel == 1) {
		InitItem();
		DisplayItem(m_itemlist->GetSelection());
		DisplayKeyItem(m_keyitemlist->GetSelection());
	} else if (sel == 2) {
		InitEnemy();
		DisplayEnemy(m_enemylist->GetSelection());
	} else if (sel == 3) {
		InitCard();
		DisplayCard(m_cardlist->GetSelection());
	} else if (sel == 4) {
		InitText();
		DisplayText(m_textlist->GetSelection());
	} else if (sel == 5) {
		InitMenuUI();
		DisplaySpecialText(m_specialtextlist->GetSelection());
	} else if (sel == 6) {
		if (gametype == GAME_TYPE_PSX) {
			InitMips();
			DisplayMipsBattle(m_mipsbattlelist->GetSelection());
		} else {
			InitCil();
			DisplayCilStruct(m_ciltypelist->GetSelection());
		}
	}
}

void CDDataStruct::OnNotebookParty(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitSpell();
		DisplaySpell(m_spelllist->GetSelection());
	} else if (sel == 1) {
		InitSupport();
		DisplaySupport(m_supportlist->GetSelection());
	} else if (sel == 2) {
		InitCommand();
		DisplayCommand(m_cmdlist->GetSelection());
	} else if (sel == 3) {
		InitStat();
		DisplayStat(m_statlist->GetSelection());
	} else if (sel == 4) {
		InitPartySpecial();
		DisplayPartySpecial(m_partyspeciallist->GetSelection());
	}
}

void CDDataStruct::OnNotebookInventory(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitItem();
		DisplayItem(m_itemlist->GetSelection());
		DisplayKeyItem(m_keyitemlist->GetSelection());
	} else if (sel == 1) {
		InitShop();
		DisplayShop(m_shoplist->GetSelection());
		DisplaySynthesisShop(m_synthshoplist->GetSelection());
	}
}

void CDDataStruct::OnNotebookCard(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitCard();
		DisplayCard(m_cardlist->GetSelection());
	} else if (sel == 1) {
		InitCard();
		DisplayCardDeck(m_carddecklist->GetSelection());
	}
}

void CDDataStruct::OnNotebookEnvironment(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitText();
		DisplayText(m_textlist->GetSelection());
	} else if (sel == 1) {
		InitWorldMap();
		DisplayWorldMap(m_worldlist->GetSelection());
		DisplayWorldPlace(m_worldplacelist->GetSelection());
		DisplayWorldBattle(m_worldbattlelist->GetSelection());
	} else if (sel == 2) {
		InitField();
		DisplayField(m_fieldlist->GetSelection());
	} else if (sel == 3) {
		if (gametype == GAME_TYPE_PSX) {
			InitBattleScene();
			DisplayBattleScene(m_battlescenelist->GetSelection());
		} else {
			m_notebookenvironment->ChangeSelection(0);
			wxMessageDialog popup(NULL, HADES_STRING_NO_STEAM, HADES_STRING_ERROR, wxOK | wxCENTRE);
			popup.ShowModal();
		}
	} else if (sel == 4) {
		InitSpellAnimation();
		DisplaySpellAnimation(m_spellanimlist->GetSelection());
	}
}

void CDDataStruct::OnNotebookInterface(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitMenuUI();
		DisplaySpecialText(m_specialtextlist->GetSelection());
	}
}

void CDDataStruct::OnNotebookMips(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	if (sel == 0) {
		InitMips();
		DisplayMipsBattle(m_mipsbattlelist->GetSelection());
	}
}

void CDDataStruct::OnNotebookCil(wxNotebookEvent& event) {
	unsigned int sel = event.GetSelection();
	InitCil();
	if (sel == 0) {
		DisplayCilStruct(m_ciltypelist->GetSelection());
	} else if (sel == 1) {
		DisplayCilMacro(m_cilmacrolist->GetSelection());
	}
}

bool DiscardTextLimit = false;
void CDDataStruct::TextReachLimit() {
	if (DiscardTextLimit)
		return;
	DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_TEXT_REACH_LIMIT);
	if (popup.ShowModal() == wxID_DISCARD)
		DiscardTextLimit = true;
}

CDDataStruct::CDDataStruct(wxWindow* parent, string fname, ConfigurationSet& cfg) :
	CDPanel(parent),
	filename(fname),
	config(cfg),
	saveset(&spellset, &cmdset, &enemyset, &shopset, &textset, &worldset, &sceneset, &itemset, &supportset, &statset, &cardset, &fieldset, &spellanimset, &ffuiset, &partyspecialset, &mipsset, &cilset),
	backupset(),
	spellsorted(OrderedIndex),
	supportsorted(OrderedIndex),
	cmdsorted(OrderedIndex),
	enemysorted(OrderedIndex),
	cardsorted(OrderedIndex),
	itemsorted(OrderedIndex),
	keyitemsorted(OrderedIndex),
	textsorted(OrderedIndex),
	worldsorted(OrderedIndex),
	fieldsorted(OrderedIndex),
	scenesorted(OrderedIndex),
	spellanimsorted(OrderedIndex),
	cilsorted(OrderedIndex),
	scenetex(NULL),
	scenepal(NULL),
	copyenemystat_battleid(-1),
	copyenemystat_statid(-1),
	copyenemyspell_battleid(-1),
	copyenemyspell_spellid(-1),
	copystring(NULL),
	scenetexpreview(wxNullBitmap),
	scenetexlinkpreview(wxNullBitmap),
	fieldtexpreview(wxNullBitmap) {
	unsigned int i;
	gametype = GetGameType();
	cluster.config = &config;
	if (gametype == GAME_TYPE_PSX) {
		m_notebookmain->RemovePage(7);
		fstream f(filename.c_str(), ios::in | ios::binary);
		cluster.Read(f, config);
		cluster.CreateImageMaps(f);
		f.close();
	} else {
		m_notebookmain->RemovePage(6);
		cluster.enemy_amount = config.enmy_amount;
		cluster.text_amount = config.text_amount;
		cluster.world_amount = config.world_amount;
		cluster.field_amount = config.field_amount;
//		cluster.battle_scene_amount = config.battle_scene_amount;
		cluster.image_map_amount = 0;
		if (config.atlas_icontexture_file >= 0 && config.atlas_iconsprite_file >= 0) {
			fstream atlasfile(config.steam_dir_data + "sharedassets2.assets", ios::in | ios::binary);
			ffuiset.steam_atlas[0].LoadAtlas(atlasfile, config.meta_atlas, config.atlas_icontexture_file, config.atlas_iconsprite_file);
			atlasfile.close();
		}
	}
	for (i = 0; i < LIST_MAX_AMOUNT; i++)
		OrderedIndex[i] = i;
	sharedmenu = new wxMenu();
	sharedmenuadd = new wxMenuItem(sharedmenu, wxID_ADD, HADES_STRING_GENERIC_ADD);
	sharedmenuremove = new wxMenuItem(sharedmenu, wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	sharedmenu->Append(sharedmenuadd);
	sharedmenu->Append(sharedmenuremove);
	sharedmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnSharedRightClickMenu), NULL, this);
	scenetexmenu = new wxMenu();
	scenetexmenu->Append(wxID_ADD, HADES_STRING_GENERIC_ADD);
	scenetexmenu->Append(wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	scenetexmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnBattleSceneTextureSelectRightClickMenu), NULL, this);
	enemystatmenu = new wxMenu();
	enemystatmenupaste = new wxMenuItem(enemystatmenu, wxID_PASTE, HADES_STRING_GENERIC_PASTE);
	enemystatmenu->Append(wxID_COPY, HADES_STRING_GENERIC_COPY);
	enemystatmenu->Append(wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	enemystatmenu->Append(enemystatmenupaste);
	enemystatmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnEnemyStatRightClickMenu), NULL, this);
	enemyspellmenu = new wxMenu();
	enemyspellmenupaste = new wxMenuItem(enemyspellmenu, wxID_PASTE, HADES_STRING_GENERIC_PASTE);
	enemyspellmenu->Append(wxID_COPY, HADES_STRING_GENERIC_COPY);
	enemyspellmenu->Append(wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	enemyspellmenu->Append(enemyspellmenupaste);
	enemyspellmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnEnemySpellRightClickMenu), NULL, this);
	enemygroupmenu = new wxMenu();
	enemygroupmenu->Append(wxID_ADD, HADES_STRING_GENERIC_ADD);
	enemygroupmenu->Append(wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	enemygroupmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnEnemyGroupRightClickMenu), NULL, this);
	textmenu = new wxMenu();
	textmenuadd = new wxMenuItem(textmenu, wxID_ADD, HADES_STRING_GENERIC_ADD);
	textmenuremove = new wxMenuItem(textmenu, wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	textmenucopy = new wxMenuItem(textmenu, wxID_COPY, HADES_STRING_GENERIC_COPY);
	textmenupaste = new wxMenuItem(textmenu, wxID_PASTE, HADES_STRING_GENERIC_PASTE);
	textmenu->Append(textmenuadd);
	textmenu->Append(textmenuremove);
	textmenu->Append(textmenucopy);
	textmenu->Append(textmenupaste);
	textmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnGenericTextRightClickMenu), NULL, this);
	specialtextmenu = new wxMenu();
	specialtextmenu->Append(wxID_ADD, HADES_STRING_GENERIC_ADD);
	specialtextmenu->Append(wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	specialtextmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CDDataStruct::OnSpecialTextRightClickMenu), NULL, this);
	commandchoicelist.push_back(m_statcharcommandattack);
	commandchoicelist.push_back(m_statcharcommanddefend);
	commandchoicelist.push_back(m_statcharcommand1);
	commandchoicelist.push_back(m_statcharcommand2);
	commandchoicelist.push_back(m_statcharcommanditem);
	commandchoicelist.push_back(m_statcharcommandchange);
	commandchoicelist.push_back(m_statcharcommandtranceattack);
	commandchoicelist.push_back(m_statcharcommandtrancedefend);
	commandchoicelist.push_back(m_statcharcommandtrance1);
	commandchoicelist.push_back(m_statcharcommandtrance2);
	commandchoicelist.push_back(m_statcharcommandtranceitem);
	commandchoicelist.push_back(m_statcharcommandtrancechange);
	itemchoicelist.push_back(m_statcharweapon);
	itemchoicelist.push_back(m_statcharhead);
	itemchoicelist.push_back(m_statcharwrist);
	itemchoicelist.push_back(m_statchararmor);
	itemchoicelist.push_back(m_statcharaccessory);
	itemchoicelist.push_back(m_enemystatitemsteal1);
	itemchoicelist.push_back(m_enemystatitemsteal2);
	itemchoicelist.push_back(m_enemystatitemsteal3);
	itemchoicelist.push_back(m_enemystatitemsteal4);
	itemchoicelist.push_back(m_enemystatitemdrop1);
	itemchoicelist.push_back(m_enemystatitemdrop2);
	itemchoicelist.push_back(m_enemystatitemdrop3);
	itemchoicelist.push_back(m_enemystatitemdrop4);
	itemchoicelist.push_back(m_itemmenuposchoice);
	itemchoicelist.push_back(m_itemequipposchoice);
	InitSpell();
	DisplaySpell(0);
}





void CDDataStruct::DebugWrite() {
	fstream fout("HadesWorkshopOutput.txt",ios::out);
	unsigned int i;
	if (saveset.sectionloaded[DATA_SECTION_ITEM]) {
		fout << "ITEMS\n";
		for (i=0;i<itemset.item.size();i++) {
			ItemStatDataStruct& stat = itemset.GetStatById(itemset.item[i].stat_id);
			fout << itemset.item[i].name.GetStr(2) << " : ";
			fout << (unsigned int)itemset.item[i].type << " type | ";
			fout << (unsigned int)itemset.item[i].char_availability << " can_equip | ";
			fout << (unsigned int)stat.speed << " speed | ";
			fout << (unsigned int)stat.strength << " strength | ";
			fout << (unsigned int)stat.magic << " magic | ";
			fout << (unsigned int)stat.spirit << " spirit\n";
		}
	}
}
