#include "Gui_ScriptEditor.h"

#include <stack>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include "Gui_Manipulation.h"
#include "Hades_Strings.h"
#include "Database_Script.h"
#include "Database_Resource.h"
#include "Database_Animation.h"
#include "Database_Item.h"
#include "Database_Spell.h"
#include "main.h"

#define SCRIPT_ID_NO_ENTRY			0xFFFF
#define POSITION_PANEL_SIZE			200
#define WORLDMAP_PANEL_WIDTH		256
#define WORLDMAP_PANEL_HEIGHT		216
#define SCRIPT_FIXED_ENTRY_AMOUNT	9

const static wxColour FUNCTION_COLOR_NORMAL(255, 255, 255);
const static wxColour FUNCTION_COLOR_MODIFIED(255, 200, 0);
const static wxColour FUNCTION_COLOR_PARSED(40, 255, 40);
const static wxColour FUNCTION_COLOR_FAILED(255, 0, 0);
const static wxColour POSITION_POINT_COLOR(0, 0, 255);
const static wxColour POSITION_REGION_COLOR(255, 0, 180);

/*********************************************************************
 * Interpreting and displaying the game scripts is done there.
 * 
 * Here are the different structures of the code controls.
 * On the left, the binary code,
 * On the right, corresponding C-like code.
 * 
 * ************************* if/then *********************************
 * 05 Condition					* if (Condition) {
 * 02 Jump ---\					*   CODE_A()
 * CODE_A     |					* }
 *        <---/					*
 * 								*
 * *********************** if/then/else ******************************
 * 05 Condition					* if (Condition) {
 * 02 Jump ---\					*   CODE_A()
 * CODE_A     |					* } else {
 * 01 Jump ---|--\				*   CODE_B()
 * CODE_B <---/	 |  			* }
 *        <------/				*
 * 								*
 * ************************ ifnot/then *******************************
 * 05 Condition					* ifnot (Condition) {
 * 03 Jump ---\					*   CODE_A()
 * CODE_A     |					* }
 *        <---/					*
 * 								*
 * ************************** while **********************************
 * 01 Jump ----------\			* while (Condition) {
 * CODE_A  <------\   |			*   CODE_A()
 * 05 Condition <-|---/			* }
 * 03 Jump -------/				*
 * 								*
 * ************************* do/while ********************************
 * CODE_A <------\				* do {
 * 05 Condition  |				*   CODE_A()
 * 03 Jump ------/				* } while (Condition)
 * 								*
 * ************************ switch(ex) *******************************
 * 05 Condition					* switch (Condition) from X {
 * 0B Switches 0  1 DEFAULT		* case 0:
 * CODE_A <----/  |    |		*   CODE_A()
 * 01 Jump -------|----|		*   break
 * CODE_B <-------/    |		* case 1:
 * 01 Jump ------------|		*   CODE_B()
 *        <------------/		*   break
 * 								* }
 * 								*
 * ******************** switch(ex)+default ***************************
 * 05 Condition					* switch (Condition) from X {
 * 0B Switches 0  1 DEFAULT		* case 0:
 * CODE_A <----/  |    |		*   CODE_A()
 * 01 Jump -------|----|---\	*   break
 * CODE_B <-------/    |   |	* case 1:
 * 01 Jump ------------|---|	*   CODE_B()
 * CODE_C <------------/   |	*   break
 * 01 Jump ----------------|	* default:
 *        <----------------/	*   CODE_C()
 * 								*	break
 * 								* }
 * 								*
 * *************************** loop **********************************
 * FUNC_START <---\				* function {
 * CODE_A         |				*   CODE_A()
 * 01 Jump -------/				*   loop
 * 								* }
 * 								*
 *********************************************************************/

#define DEFAULT_SET_STR	L"set VAR_GenInt8_0 = 0"
#define TAB_STR			L"    "
#define SS_ARG_ID		500
#define SS_ARGBOX_MAXID	32
#define TIMER_TIMEOUT	5

#define SCRPT_MAX_OPERAND 0x100		// Max operand amount in variable code expressions
#define SCRPT_MAX_FUNC_LINE 0x10000	// Max line amount in one function
#define SCRPT_MAX_SWITCH_CASE 0x100	// Max case amount in a switch or switchex block

#define ARG_CONTROL_TEXT		0
#define ARG_CONTROL_FLAG		1
#define ARG_CONTROL_FLAGS		2
#define ARG_CONTROL_SPIN		3
#define ARG_CONTROL_CHOICE		4
#define ARG_CONTROL_LINKED_DIAL	5
#define ARG_CONTROL_DISC		6
#define ARG_CONTROL_POSITION	7
#define ARG_CONTROL_COLOR_CMY	8
#define ARG_CONTROL_COLOR_RGB	9

#define SCRIPT_TEXT_LINK_PREVIEW_LENGTH	100

//=============================//
//          Generic            //
//=============================//

void RemoveSurroundingSpaces(wxString& str) {
	size_t pos = str.find_first_not_of(" \t");
	if (pos==string::npos) {
		str = wxEmptyString;
		return;
	}
	str = str.Mid(pos);
	pos = str.find_last_of(" \t");
	if (pos!=string::npos)
		str = str.Mid(0,pos);
}

wxString GetNextWord(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(L" \t");
	if (pos==string::npos)
		return wxEmptyString;
	size_t lineshift = pos;
	res = line.Mid(pos);
	pos = res.find_first_of(L" ,():;{}\t");
	if (pos!=string::npos) {
		res = res.Mid(0,pos);
		lineshift += pos;
		line = line.Mid(lineshift);
	} else
		line = wxEmptyString;
	return res;
}

wxString GetNextArg(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(" \t");
	if (pos == string::npos)
		return wxEmptyString;
	res = line.Mid(pos);
	unsigned int parlvl = 0;
	size_t lineshift = pos, arglen = 0;
	bool ok = false;
	wxString tmp = res;
	while (!ok) {
		pos = tmp.find_first_of(L",()");
		arglen += pos;
		if (pos == string::npos) {
			line = wxEmptyString;
			return wxEmptyString;
		} else if (tmp[pos] == L'(') {
			parlvl++;
			tmp = tmp.Mid(pos + 1);
			arglen++;
			while (parlvl > 0) {
				pos = tmp.find_first_of(L"()");
				if (pos == string::npos) {
					line = wxEmptyString;
					return wxEmptyString;
				} else if (tmp[pos] == L'(')
					parlvl++;
				else
					parlvl--;
				tmp = tmp.Mid(pos + 1);
				arglen += pos + 1;
			}
		} else {
			ok = true;
		}
	}
	res = res.Mid(0, arglen);
	res.Trim();
	lineshift += arglen;
	line = line.Mid(lineshift);
	return res;
}

wxString GetNextPunc(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(" \t");
	if (pos == string::npos)
		return wxEmptyString;
	size_t lineshift = pos;
	res = line.Mid(pos);
	pos = res.find_first_not_of(L",():;{}");
	if (pos != string::npos) {
		if (pos > 1)
			pos = 1;
		res = res.Mid(0, pos);
		lineshift += pos;
		line = line.Mid(lineshift);
	} else if (res.Length() > 1) {
		res = res.Mid(0, 1);
		line = line.Mid(lineshift + 1);
	} else {
		if (res.Length() > 0)
			res = res.Mid(0, 1);
		line = wxEmptyString;
	}
	return res;
}

wxString GetNextThing(wxString& line) {
	wxString res;
	size_t pos = line.find_first_not_of(" \t");
	if (pos==string::npos)
		return wxEmptyString;
	res = line.Mid(pos);
	static wxString punclist = L",():;{}";
	for (unsigned int i=0;i<punclist.Length();i++)
		if (res[0]==punclist[i]) {
			res = res.Mid(0,1);
			line = line.Mid(pos+1);
			return res;
		}
	size_t lineshift = pos;
	pos = res.find_first_of(L" ,():;{}\t");
	if (pos!=string::npos) {
		res = res.Mid(0,pos);
		lineshift += pos;
		line = line.Mid(lineshift);
	} else
		line = wxEmptyString;
	return res;
}

wxString ScriptEditHandler::GetArgumentDescription(int64_t argvalue, uint8_t argtype) {
	int64_t i;
	switch (argtype) {
	case AT_TEXT:
		if (use_text) {
			if (argvalue >= text->amount) return _(L"[Invalid Text ID]");
			wxString onelinestr = _(text->text[argvalue].str_nice);
			onelinestr.Replace(_(L"\n"), _(L" "));
			return _(L"\"") + onelinestr + _(L"\"");
		}
		break;
	case AT_BATTLE:
		if (use_battle) {
			for (i = 0; i < datas->enemyset->battle_amount; i++)
				if (datas->enemyset->battle_data[i]->object_id == argvalue)
					return _(datas->enemyset->battle_name[i]);
			return _(L"[Missing Battle]");
		}
		break;
	case AT_FIELD:
		if (use_field) {
			for (i = 0; i < datas->fieldset->amount; i++)
				if (datas->fieldset->script_data[i]->object_id == argvalue)
					return _(datas->fieldset->script_data[i]->name.str_nice);
			return _(L"[Missing Field]");
		}
		break;
	case AT_ATTACK:
		if (use_attack) {
			if (argvalue >= enemy->spell_amount) return _(L"[Invalid Attack ID]");
			return _(enemy->spell[argvalue].name.str_nice);
		}
		break;
	case AT_ITEM:
		if (use_item && argvalue < ITEM_AMOUNT) return _(datas->itemset->item[argvalue].name.str_nice);
		if (use_item && argvalue >= 0x100 && argvalue < 0x100 + KEY_ITEM_AMOUNT) return _(datas->itemset->key_item[argvalue - 0x100].name.str_nice);
		if (use_card && argvalue >= 0x200 && argvalue < 0x200 + CARD_AMOUNT) return _(datas->cardset->card[argvalue - 0x200].name.str_nice);
		break;
	case AT_CHARACTER:
		if (use_character) {
			wxString charlist;
			for (i = 0; i < 12; i++)
				if (argvalue & (1LL << i))
					charlist += _(datas->statset->initial_stat[i].default_name.str_nice) + _(L"|");
			if (charlist.Len() > 0) charlist.Truncate(charlist.Len() - 1);
			return charlist;
		}
		break;
	case AT_LCHARACTER:
		if (use_character) {
			if (argvalue == 0xFF || argvalue == 0xFFFF) return _(HADES_STRING_NULL_CHARACTER_SLOT);
			if (argvalue < 12) return _(datas->statset->initial_stat[argvalue].default_name.str_nice);
			return _(L"[Invalid Character ID]");
		}
		break;
	case AT_ENTRY:
		if (argvalue == 0xFA) return _(L"Player Character");
		if (argvalue == 0xFB) return _(L"Team Character 1");
		if (argvalue == 0xFC) return _(L"Team Character 2");
		if (argvalue == 0xFD) return _(L"Team Character 3");
		if (argvalue == 0xFE) return _(L"Team Character 4");
		if (argvalue == 0xFF) return _(L"This");
		if (argvalue >= script.entry_amount) return _(L"[Invalid Entry ID]");
		return entry_name[argvalue];
	case AT_MODEL:
		for (i = 0; i < G_V_ELEMENTS(HADES_STRING_MODEL_NAME); i++)
			if (HADES_STRING_MODEL_NAME[i].id == argvalue)
				return _(HADES_STRING_MODEL_NAME[i].label);
		return _(L"[Invalid Model ID]");
	case AT_SOUND:
	case AT_AUDIO:
		for (i = 0; i < G_V_ELEMENTS(HADES_STRING_MUSIC_NAME); i++) // DEBUG: Should retrieve whether it's audio or music
			if (HADES_STRING_MUSIC_NAME[i].id == argvalue)
				return _(HADES_STRING_MUSIC_NAME[i].label);
		for (i = 0; i < G_V_ELEMENTS(HADES_STRING_AUDIO_NAME); i++)
			if (HADES_STRING_AUDIO_NAME[i].id == argvalue)
				return _(HADES_STRING_AUDIO_NAME[i].label);
		return _(L"[Unknown Audio]");
	case AT_WORLDMAP:
		for (i = 0; i < G_V_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME); i++)
			if (HADES_STRING_WORLD_BLOCK_NAME[i].id == argvalue)
				return _(HADES_STRING_WORLD_BLOCK_NAME[i].label);
		return _(L"[Unknown World Map]");
	case AT_ABILITY:
		if (use_ability && argvalue < SPELL_AMOUNT) return _(datas->spellset->spell[argvalue].name.str_nice);
		if (use_support && argvalue >= SPELL_AMOUNT) return _(datas->supportset->support[argvalue - SPELL_AMOUNT].name.str_nice);
		break;
	case AT_BATTLECODE:
		for (i = 0; i < G_V_ELEMENTS(BattleCodeName); i++)
			if (BattleCodeName[i].id == argvalue)
				return _(BattleCodeName[i].name);
		return _(L"[Invalid Battle Code]");
	case AT_MODELCODE:
		for (i = 0; i < G_V_ELEMENTS(ModelCodeName); i++)
			if (ModelCodeName[i].id == argvalue)
				return _(ModelCodeName[i].name);
		return _(L"[Invalid Model Code]");
	case AT_WORLDCODE:
		for (i = 0; i < G_V_ELEMENTS(WorldCodeName); i++)
			if (WorldCodeName[i].id == argvalue)
				return _(WorldCodeName[i].name);
		return _(L"[Invalid World Map Code]");
	case AT_SOUNDCODE:
		for (i = 0; i < G_V_ELEMENTS(SoundCodeName); i++)
			if (SoundCodeName[i].id == argvalue)
				return _(SoundCodeName[i].name);
		return _(L"[Invalid Sound Code]");
	case AT_SPSCODE:
		for (i = 0; i < G_V_ELEMENTS(SpsCodeName); i++)
			if (SpsCodeName[i].id == argvalue)
				return _(SpsCodeName[i].name);
		return _(L"[Invalid SPS Code]");
	case AT_ANIMATION:
		i = AnimationDatabase::GetIndex(argvalue);
		if (i >= 0) return AnimationDatabase::GetDescription(i);
		return _(L"[Invalid Animation ID]");
	case AT_DECK:
		return _(HADES_STRING_DECK_NAME[argvalue].label);
	case AT_BUTTONLIST: {
		wxString btnlist = wxEmptyString;
		for (i = 0; i < G_V_ELEMENTS(PlaystationButton); i++)
			if (argvalue & (1LL << i))
				btnlist += _(PlaystationButton[i]) + _(L"|");
		if (btnlist.Len() > 0) btnlist.Truncate(btnlist.Len() - 1);
		return btnlist;
	}
	}
	return wxEmptyString;
}

wxString ScriptEditHandler::GetFunctionName(unsigned int entry, uint16_t functype) {
	if (functype < G_V_ELEMENTS(FunctionTypeName))
		return _(L"Function ") + wxString::Format(wxT("%s_%s"), entry_name[entry], FunctionTypeName[functype]);
	if (script_type == SCRIPT_TYPE_WORLD && entry == 0 && functype >= 0x8000)
		return _(L"Function ") + wxString::Format(wxT("%s_x%u_y%u_%s"), entry_name[entry], (functype & 0xFC) >> 2, (functype & 0x3F00) >> 8, WorldFunctionTriggerType[functype & 3]);
	return _(L"Function ") + wxString::Format(wxT("%s_%u"), entry_name[entry], functype);
}

ScriptEditHandler::ScriptEditHandler(ScriptDataStruct& scpt, int scpttype, SaveSet* sv, EnemyDataStruct* ed, TextDataStruct* td) :
	enemy(ed),
	text(td),
	datas(sv),
	script_type(scpttype),
	use_character(sv->sectionloaded[DATA_SECTION_STAT]),
	use_text(td),
	use_battle(sv->sectionloaded[DATA_SECTION_ENMY]),
	use_attack(ed),
	use_field(sv->sectionloaded[DATA_SECTION_FIELD]),
	use_item(sv->sectionloaded[DATA_SECTION_ITEM]),
	use_ability(sv->sectionloaded[DATA_SECTION_SPELL]),
	use_support(sv->sectionloaded[DATA_SECTION_SUPPORT]),
	use_command(sv->sectionloaded[DATA_SECTION_CMD]),
	use_card(sv->sectionloaded[DATA_SECTION_CARD]),
	entry_selection(SCRIPT_ID_NO_ENTRY),
	handler_dialog(NULL) {
	unsigned int i;
	script = scpt;
	entry_name.Alloc(script.entry_amount);
	if (script.entry_amount>0)
		entry_name.Add(_(L"Main"));
	for (i=1;i<script.entry_amount;i++)
		entry_name.Add(wxString::Format(wxT("Entry%d"),i));
	entry_model_index.resize(script.entry_amount);
	for (i=0;i<script.entry_amount;i++)
		entry_model_index[i] = -1;
	func_str.resize(script.entry_amount);
	localvar_str.resize(script.entry_amount);
	for (i=0;i<script.entry_amount;i++)
		func_str[i].resize(script.entry_function_amount[i]);
}

ScriptEditHandler::~ScriptEditHandler() {
}

ScriptEditDialog::ScriptEditDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype, SaveSet* sv, EnemyDataStruct* ed, TextDataStruct* td) :
	ScriptEditWindow(parent),
	ScriptEditHandler(scpt, scpttype, sv, ed, td),
	current_opcode(0xFFFF),
	arg_control_type(NULL),
	extra_size(script.GetExtraSize()),
	refresh_control_disable(false),
	refresh_help_force(false),
	timer(new wxTimer(this)),
	help_dial(NULL) {
	unsigned int i;
	handler_dialog = this;
	m_panelbattle->Enable(script_type == SCRIPT_TYPE_BATTLE);
	m_panelfield->Enable(script_type == SCRIPT_TYPE_FIELD);
	m_panelworld->Enable(script_type == SCRIPT_TYPE_WORLD);
	if (script_type == SCRIPT_TYPE_FIELD) {
		gl_window = new GLWindow(m_fielddisplaypanel, NULL);
		FieldTilesDataStruct* tiles = NULL;
		FieldWalkmeshDataStruct* walk = NULL;
		if (GetGameType() == GAME_TYPE_PSX) {
			tiles = (FieldTilesDataStruct*)&script.parent_cluster->chunk[script.parent_cluster->SearchChunkType(CHUNK_TYPE_FIELD_TILES)].GetObject(0);
			walk = (FieldWalkmeshDataStruct*)&script.parent_cluster->chunk[script.parent_cluster->SearchChunkType(CHUNK_TYPE_FIELD_WALK)].GetObject(0);
		} else {
			for (i = 0; i < datas->fieldset->amount; i++)
				if (scpt.object_id == datas->fieldset->script_data[i]->object_id) {
					tiles = datas->fieldset->background_data[i];
					walk = datas->fieldset->walkmesh[i];
					break;
				}
		}
		gl_window->DisplayField(tiles, walk);
		m_fielddisplaybackground->Enable(tiles != NULL);
		m_fielddisplaymesh->Enable(walk != NULL);
		gl_window->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(ScriptEditDialog::OnPickWalkmesh), NULL, this);
	} else if (script_type == SCRIPT_TYPE_WORLD) {
		world_pos_type = 0;
		world_region_x = -1;
		world_region_y = -1;
	}
	for (i = 1; i < script.entry_amount; i++)
		if (enemy && i <= enemy->stat_amount) {
			entry_name[i] = _(enemy->stat[i - 1].name.GetStr(2));
			entry_name[i].Replace(_(L" "), _(L"_"));
		}
	modellist_id.resize(G_V_ELEMENTS(HADES_STRING_MODEL_NAME));
	modellist_str.Alloc(G_V_ELEMENTS(HADES_STRING_MODEL_NAME));
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_MODEL_NAME); i++) {
		modellist_str.Add(_(HADES_STRING_MODEL_NAME[i].label));
		modellist_id[i] = new uint16_t(HADES_STRING_MODEL_NAME[i].id);
	}
	DisplayFunctionList();
	if (use_character) {
		character_str.Alloc(13);
		for (i = 0; i < 8; i++)
			character_str.Add(_(datas->statset->initial_stat[i].default_name.str_nice));
		character_str.Add(_(datas->statset->initial_stat[11].default_name.str_nice));
		character_str.Add(_(datas->statset->initial_stat[8].default_name.str_nice));
		character_str.Add(_(datas->statset->initial_stat[9].default_name.str_nice));
		character_str.Add(_(datas->statset->initial_stat[10].default_name.str_nice));
		character_str.Add(_(HADES_STRING_NULL_CHARACTER_SLOT));
		character_id.resize(13);
		for (i = 0; i < 12; i++)
			character_id[i] = new uint16_t(i);
		character_id[12] = new uint16_t(0xFF);
	}
	if (use_battle) {
		battle_str.Alloc(datas->enemyset->battle_amount);
		battle_id.resize(datas->enemyset->battle_amount);
		for (i = 0; i < datas->enemyset->battle_amount; i++) {
			battle_str.Add(_(datas->enemyset->battle_name[i]));
			battle_id[i] = new uint16_t(datas->enemyset->battle_data[i]->object_id);
		}
	}
	m_intvalueattack->Enable(enemy != NULL);
	m_intvalueattacklabel->Enable(enemy != NULL);
	if (use_attack) {
		attack_str.Alloc(enemy->spell_amount);
		for (i = 0; i < enemy->spell_amount; i++)
			attack_str.Add(_(enemy->spell[i].name.str_nice));
	}
	if (use_field) {
		field_str.Alloc(datas->fieldset->amount + 1);
		field_id.resize(datas->fieldset->amount + 1);
		for (i = 0; i < datas->fieldset->amount; i++) {
			field_str.Add(_(datas->fieldset->script_data[i]->name.str_nice));
			field_id[i] = new uint16_t(datas->fieldset->script_data[i]->object_id);
		}
		field_str.Add(_(FIELD_ENDING_NAME));
		field_id[datas->fieldset->amount] = new uint16_t(FIELD_ENDING_ID);
	}
	m_intvalueitem->Enable(use_item);
	m_intvalueitemlabel->Enable(use_item);
	if (use_item) {
		item_str.Alloc(ITEM_AMOUNT + KEY_ITEM_AMOUNT + CARD_AMOUNT);
		item_id.resize(ITEM_AMOUNT + KEY_ITEM_AMOUNT + CARD_AMOUNT);
		for (i = 0; i < ITEM_AMOUNT; i++) {
			item_str.Add(_(datas->itemset->item[i].name.str_nice));
			item_id[i] = new uint16_t(i);
		}
		for (i = 0; i < KEY_ITEM_AMOUNT; i++) {
			item_str.Add(_(datas->itemset->key_item[i].name.str_nice));
			item_id[ITEM_AMOUNT + i] = new uint16_t(0x100 + i);
		}
		if (use_card) {
			for (i = 0; i < CARD_AMOUNT; i++) {
				item_str.Add(_(datas->cardset->card[i].name.str_nice));
				item_id[ITEM_AMOUNT + KEY_ITEM_AMOUNT + i] = new uint16_t(0x200 + i);
			}
		} else {
			for (i = 0; i < CARD_AMOUNT; i++) {
				item_str.Add(_(HADES_STRING_CARD_NAME[i].label));
				item_id[ITEM_AMOUNT + KEY_ITEM_AMOUNT + i] = new uint16_t(0x200 + HADES_STRING_CARD_NAME[i].id);
			}
		}
	}
	m_intvaluespell->Enable(use_ability);
	m_intvaluespelllabel->Enable(use_ability);
	if (use_ability) {
		ability_str.Alloc(use_support ? SPELL_AMOUNT + SUPPORT_AMOUNT : SPELL_AMOUNT);
		for (i = 0; i < SPELL_AMOUNT; i++)
			ability_str.Add(_(datas->spellset->spell[i].name.str_nice));
		if (use_support)
			for (i = 0; i < SUPPORT_AMOUNT; i++)
				ability_str.Add(_(datas->supportset->support[i].name.str_nice));
	}
	m_intvaluecmd->Enable(use_command);
	m_intvaluecmdlabel->Enable(use_command);
	if (use_command) {
		command_str.Alloc(COMMAND_AMOUNT + G_V_ELEMENTS(CommandAddendaName) + 1);
		for (i = 0; i + 1 < COMMAND_AMOUNT; i++)
			command_str.Add(_(datas->cmdset->cmd[i].name.str_nice));
		for (i = 0; i < G_V_ELEMENTS(CommandAddendaName); i++)
			command_str.Add(_(CommandAddendaName[i]));
	}
	if (use_text) {
		text_str.Alloc(text->amount);
		for (i = 0; i < text->amount; i++)
			text_str.Add(_(text->text[i].str_nice.substr(0, 30)));
	}
	defaultbool_str.Alloc(SS_ARGBOX_MAXID);
	for (i = 0; i < SS_ARGBOX_MAXID; i++)
		defaultbool_str.Add(_(wxString::Format(wxT("%u"), i + 1)));
	disc_str.Alloc(G_V_ELEMENTS(DiscName));
	for (i = 0; i < G_V_ELEMENTS(DiscName); i++)
		disc_str.Add(_(DiscName[i]));
	menutype_str.Alloc(G_V_ELEMENTS(MenuType));
	for (i = 0; i < G_V_ELEMENTS(MenuType); i++)
		menutype_str.Add(_(MenuType[i]));
	shop_str.Alloc(HADES_STRING_SHOP_NAME.size());
	for (i = 0; i < HADES_STRING_SHOP_NAME.size(); i++)
		shop_str.Add(_(HADES_STRING_SHOP_NAME[i].label));
	abilityset_str.Alloc(G_V_ELEMENTS(AbilitySetName));
	for (i = 0; i < G_V_ELEMENTS(AbilitySetName); i++)
		abilityset_str.Add(_(AbilitySetName[i]));
	bubblesymbol_str.Alloc(G_V_ELEMENTS(BubbleSymbolName));
	for (i = 0; i < G_V_ELEMENTS(BubbleSymbolName); i++)
		bubblesymbol_str.Add(_(BubbleSymbolName[i]));
	deck_str.Alloc(HADES_STRING_DECK_NAME.size());
	for (i = 0; i < HADES_STRING_DECK_NAME.size(); i++)
		deck_str.Add(_(HADES_STRING_DECK_NAME[i].label));
	equipset_id.resize(G_V_ELEMENTS(EquipSetName));
	equipset_str.Alloc(G_V_ELEMENTS(EquipSetName));
	for (i = 0; i < G_V_ELEMENTS(EquipSetName); i++) {
		equipset_str.Add(_(EquipSetName[i].name));
		equipset_id[i] = new uint16_t(EquipSetName[i].id);
	}
	fmv_id.resize(G_V_ELEMENTS(FMVNameList));
	fmv_str.Alloc(G_V_ELEMENTS(FMVNameList));
	for (i = 0; i < G_V_ELEMENTS(FMVNameList); i++) {
		fmv_str.Add(_(FMVNameList[i].name));
		fmv_id[i] = new uint16_t(FMVNameList[i].id);
	}
	battlecode_id.resize(G_V_ELEMENTS(BattleCodeName));
	battlecode_str.Alloc(G_V_ELEMENTS(BattleCodeName));
	for (i = 0; i < G_V_ELEMENTS(BattleCodeName); i++) {
		battlecode_str.Add(_(BattleCodeName[i].name));
		battlecode_id[i] = new uint16_t(BattleCodeName[i].id);
	}
	modelcode_id.resize(G_V_ELEMENTS(ModelCodeName));
	modelcode_str.Alloc(G_V_ELEMENTS(ModelCodeName));
	for (i = 0; i < G_V_ELEMENTS(ModelCodeName); i++) {
		modelcode_str.Add(_(ModelCodeName[i].name));
		modelcode_id[i] = new uint16_t(ModelCodeName[i].id);
	}
	worldcode_id.resize(G_V_ELEMENTS(WorldCodeName));
	worldcode_str.Alloc(G_V_ELEMENTS(WorldCodeName));
	for (i = 0; i < G_V_ELEMENTS(WorldCodeName); i++) {
		worldcode_str.Add(_(WorldCodeName[i].name));
		worldcode_id[i] = new uint16_t(WorldCodeName[i].id);
	}
	soundcode_id.resize(G_V_ELEMENTS(SoundCodeName));
	soundcode_str.Alloc(G_V_ELEMENTS(SoundCodeName));
	for (i = 0; i < G_V_ELEMENTS(SoundCodeName); i++) {
		soundcode_str.Add(_(SoundCodeName[i].name));
		soundcode_id[i] = new uint16_t(SoundCodeName[i].id);
	}
	spscode_id.resize(G_V_ELEMENTS(SpsCodeName));
	spscode_str.Alloc(G_V_ELEMENTS(SpsCodeName));
	for (i = 0; i < G_V_ELEMENTS(SpsCodeName); i++) {
		spscode_str.Add(_(SpsCodeName[i].name));
		spscode_id[i] = new uint16_t(SpsCodeName[i].id);
	}
	worldmap_id.resize(G_V_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME));
	worldmap_str.Alloc(G_V_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME));
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME); i++) {
		worldmap_str.Add(_(HADES_STRING_WORLD_BLOCK_NAME[i].label));
		worldmap_id[i] = new uint16_t(HADES_STRING_WORLD_BLOCK_NAME[i].id);
	}
	func_popup_menu = new wxMenu();
	func_popup_menu->Append(wxID_ADD, HADES_STRING_GENERIC_ADD);
	func_popup_menu->Append(wxID_REMOVE, HADES_STRING_GENERIC_REMOVE);
	func_popup_menu->Append(wxID_SET, HADES_STRING_GENERIC_PROPERTIES);
	func_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ScriptEditDialog::OnFunctionRightClickMenu), NULL, this);
	wxImage tmpimg = wxBITMAP(bulletdown_image).ConvertToImage();
	m_localvarshowimg->SetBitmap(wxBitmap(tmpimg));
	Connect(wxEVT_TIMER, wxTimerEventHandler(ScriptEditDialog::OnTimer), NULL, this);
}

ScriptEditDialog::~ScriptEditDialog() {
	unsigned int i;
	for (i = 0; i < modellist_id.size(); i++)
		delete modellist_id[i];
	for (i = 0; i < entrylist_id.size(); i++)
		delete entrylist_id[i];
	if (use_character) {
		for (i = 0; i < character_id.size(); i++)
			delete character_id[i];
	}
	if (use_battle) {
		for (i = 0; i < battle_id.size(); i++)
			delete battle_id[i];
	}
	if (use_field) {
		for (i = 0; i < field_id.size(); i++)
			delete field_id[i];
	}
	if (use_item) {
		for (i = 0; i < item_id.size(); i++)
			delete item_id[i];
	}
	for (i = 0; i < equipset_id.size(); i++)
		delete equipset_id[i];
	for (i = 0; i < fmv_id.size(); i++)
		delete fmv_id[i];
	for (i = 0; i < battlecode_id.size(); i++)
		delete battlecode_id[i];
	for (i = 0; i < modelcode_id.size(); i++)
		delete modelcode_id[i];
	for (i = 0; i < worldcode_id.size(); i++)
		delete worldcode_id[i];
	for (i = 0; i < soundcode_id.size(); i++)
		delete soundcode_id[i];
	for (i = 0; i < spscode_id.size(); i++)
		delete spscode_id[i];
	for (i = 0; i < worldmap_id.size(); i++)
		delete worldmap_id[i];
	for (i = 0; i < animlist_id.size(); i++)
		delete animlist_id[i];
	func_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ScriptEditDialog::OnFunctionRightClickMenu), NULL, this);
	Disconnect(wxEVT_TIMER, wxTimerEventHandler(ScriptEditDialog::OnTimer), NULL, this);
	if (script_type == SCRIPT_TYPE_FIELD)
		gl_window->Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(ScriptEditDialog::OnPickWalkmesh), NULL, this);
	delete timer;
}

int ScriptEditDialog::ShowModal() {
	unsigned int entryid = 0;
	GenerateEntryNames();
	GenerateFunctionStrings(false);
	m_buttonok->SetFocus();
	while (script.entry_function_amount[entryid]==0)
		entryid++;
	DisplayFunction(entryid,0);
	timer->Start(TIMER_TIMEOUT);
	return wxDialog::ShowModal();
}

void ScriptEditHandler::AddFunction(int entryid, int funcidpos, uint16_t functype) {
	script.AddFunction(entryid, funcidpos, functype);
	unsigned int absoluteid = GetFunctionAbsolutePos(entryid, funcidpos);
	functionlist_id.insert(functionlist_id.begin() + absoluteid, new uint16_t(functype));
	functionlist_str.insert(functionlist_str.begin() + absoluteid, GetFunctionName(entryid, functype));
	func_str[entryid].Insert(functionlist_str[absoluteid] + _(L"\n"), funcidpos);
}

void ScriptEditHandler::AddEntry(int entrypos, uint8_t entrytype) {
	if (entrypos < script.entry_amount) {
		script.AddEntry(entrypos, entrytype);
		localvar_str.Insert(globalvar_str, entrypos);
		entry_name.Insert(wxString::Format(wxT("NewEntry%d"), entrypos), entrypos);
		entry_model_index.insert(entry_model_index.begin()+entrypos, -1);
	}
	while (script.entry_amount <= entrypos) {
		int newentrypos = script.entry_amount;
		script.AddEntry(newentrypos, entrytype);
		localvar_str.Add(globalvar_str);
		entry_name.Add(wxString::Format(wxT("NewEntry%d"), newentrypos));
		entry_model_index.push_back(-1);
	}
}

//=============================//
//        Display Code         //
//=============================//

unsigned int ScriptEditHandler::GetFunctionAbsolutePos(unsigned int entry, unsigned int func) {
	unsigned int i, res = func;
	for (i = 0; i < entry; i++)
		res += script.entry_function_amount[i];
	return res;
}

void ScriptEditHandler::GenerateFunctionList() {
	unsigned int funcamount = 0;
	unsigned int i, j;
	for (i = 0; i < functionlist_id.size(); i++)
		delete functionlist_id[i];
	for (i = 0; i < script.entry_amount; i++)
		funcamount += script.entry_function_amount[i];
	functionlist_str.resize(funcamount);
	functionlist_id.resize(funcamount);
	funcamount = 0;
	for (i = 0; i < script.entry_amount; i++) {
		for (j = 0; j < script.entry_function_amount[i]; j++) {
			functionlist_id[funcamount] = new uint16_t(script.function_type[i][j]);
			functionlist_str[funcamount] = GetFunctionName(i, script.function_type[i][j]);
			funcamount++;
		}
	}
}

void ScriptEditDialog::DisplayFunctionList(int newfunc, int removedfunc) {
	unsigned int funcamount = 0;
	unsigned int i,j;
	GenerateFunctionList();
	for (i=0;i<entrylist_id.size();i++)
		delete entrylist_id[i];
	entry_selection = SCRIPT_ID_NO_ENTRY;
	entrylist_str.resize(script.entry_amount+6);
	entrylist_id.resize(script.entry_amount+6);
	for (i=0;i<script.entry_amount;i++) {
		entrylist_str[i] = _(L"Entry ")+wxString::Format(wxT("%s"), entry_name[i]);
		entrylist_id[i] = new uint16_t(i);
		funcamount += script.entry_function_amount[i];
	}
	entrylist_str[i] = _(L"This");
	entrylist_id[i++] = new uint16_t(0xFF);
	entrylist_str[i] = _(L"Player Character");
	entrylist_id[i++] = new uint16_t(0xFA);
	entrylist_str[i] = _(L"Team Character 1");
	entrylist_id[i++] = new uint16_t(0xFB);
	entrylist_str[i] = _(L"Team Character 2");
	entrylist_id[i++] = new uint16_t(0xFC);
	entrylist_str[i] = _(L"Team Character 3");
	entrylist_id[i++] = new uint16_t(0xFD);
	entrylist_str[i] = _(L"Team Character 4");
	entrylist_id[i++] = new uint16_t(0xFE);
	m_functionlist->ClearAll();
	m_functionlist->AppendColumn(_(L"Functions"),wxLIST_FORMAT_LEFT,202);
	vector<bool> newshouldparse;
	bool firsttime = func_should_parse.size()==0;
	if (firsttime)
		func_should_parse.resize(funcamount);
	else if (newfunc>=0)
		newshouldparse.resize(funcamount);
	else
		newshouldparse.resize(funcamount);
	funcamount = 0;
	for (i=0;i<script.entry_amount;i++) {
		for (j=0;j<script.entry_function_amount[i];j++) {
			m_functionlist->InsertItem(funcamount,functionlist_str[funcamount]);
			if (firsttime)
				func_should_parse[funcamount] = false;
			else if (newfunc>=0) {
				if ((int)funcamount<newfunc)
					newshouldparse[funcamount] = func_should_parse[funcamount];
				else if (funcamount==newfunc)
					newshouldparse[funcamount] = false;
				else
					newshouldparse[funcamount] = func_should_parse[funcamount-1];
				if (newshouldparse[funcamount])
					m_functionlist->SetItemTextColour(funcamount,FUNCTION_COLOR_MODIFIED);
			} else {
				if ((int)funcamount<removedfunc)
					newshouldparse[funcamount] = func_should_parse[funcamount];
				else
					newshouldparse[funcamount] = func_should_parse[funcamount+1];
				if (newshouldparse[funcamount])
					m_functionlist->SetItemTextColour(funcamount,FUNCTION_COLOR_MODIFIED);
			}
			funcamount++;
		}
	}
	if (!firsttime)
		func_should_parse = newshouldparse;
}

wxString operandtmp[SCRPT_MAX_OPERAND];
unsigned int funcpostrack[SCRPT_MAX_FUNC_LINE];
unsigned int functrackline;
wxString ScriptEditHandler::ConvertVarArgument(ScriptArgument& arg, wxArrayString* argcomment, bool* ignorenulljump) {
	if (!arg.is_var)
		return wxEmptyString;

	unsigned int macroi;
	uint8_t varcat;
	uint16_t varid;

	#define MACRO_SEARCHVARNAME(STRING) \
		for (macroi=0;macroi<VarNameList.size();macroi++) \
			if (VarNameList[macroi].cat==varcat && VarNameList[macroi].id==varid) { \
				STRING = _(VarNameList[macroi].name); \
				break; \
			} \
		if (macroi==VarNameList.size()) { \
			for (macroi=0;macroi<script.local_data[entry_selection].amount;macroi++) \
				if (script.local_data[entry_selection].cat[macroi]==varcat && script.local_data[entry_selection].id[macroi]==varid) { \
					STRING = _(script.local_data[entry_selection].name[macroi]); \
					break; \
				} \
			if (macroi==script.local_data[entry_selection].amount) { \
				for (macroi=0;macroi<script.global_data.amount;macroi++) \
					if (script.global_data.cat[macroi]==varcat && script.global_data.id[macroi]==varid) { \
						STRING = _(script.global_data.name[macroi]); \
						break; \
					} \
				if (macroi==script.global_data.amount) { \
					STRING = _(VarOpList[varcat].opstring); \
					STRING << (int)varid; \
				} \
			} \
		}

	unsigned int operand = 0, i = 0, j, k, operandtotalcount = 0;
	uint8_t varbyte = arg.var[i++];
	int vartype = VarOpList[varbyte].type;
	bool isignorenulljumpsetter = false;
	int64_t argvalue;
	bool argisnum;
	while (vartype != -1) {
		if (argcomment != NULL) {
			k = 0;
			for (j = 0; j < G_V_ELEMENTS(VarOpTypeList); j++) {
				if (VarOpTypeList[j].op == varbyte) {
					k++;
					argisnum = operandtmp[operand - k].ToLongLong(&argvalue);
					if (argisnum || (operandtmp[operand - k].Len() > 0 && operandtmp[operand - k].Last() == L'L')) {
						wxString argcommenttoken = GetArgumentDescription(argvalue, VarOpTypeList[j].type);
						if (argcommenttoken.Len() > 0)
							argcomment->Add(argcommenttoken);
					}
				}
			}
		}
		if (vartype == 0) {
			operandtmp[operand - 1] = _(L"( ") + _(VarOpList[varbyte].opstring) + operandtmp[operand - 1] + _(L" )");
		} else if (vartype == 1) {
			operandtmp[operand - 1] = _(L"( ") + operandtmp[operand - 1] + _(VarOpList[varbyte].opstring) + _(L" )");
		} else if (vartype == 2) {
			operandtmp[operand - 2] = _(L"( ") + operandtmp[operand - 2] + _(L" ") + _(VarOpList[varbyte].opstring) + _(L" ") + operandtmp[operand - 1] + _(L" )");
			operand--;
		} else if (vartype == 3) {
			operandtmp[operand - 1] += _(L"[");
			for (j = 0; j < G_V_ELEMENTS(ScriptCharacterField); j++)
				if (ScriptCharacterField[j].id == arg.var[i]) {
					operandtmp[operand - 1] << _(ScriptCharacterField[j].name);
					break;
				}
			if (j == G_V_ELEMENTS(ScriptCharacterField))
				operandtmp[operand - 1] << (int)arg.var[i];
			i++;
			operandtmp[operand - 1] += _(L"]");
		} else if (vartype == 5) {
			operandtmp[operand].Empty();
			operandtmp[operand] << (unsigned int)arg.var[i++];
			operandtmp[operand++] += L"S";
		} else if (vartype == 6) {
			operandtmp[operand].Empty();
			operandtmp[operand++] << (unsigned int)(arg.var[i] | (arg.var[i + 1] << 8));
			i += 2;
		} else if (vartype == 7) {
			operandtmp[operand].Empty();
			operandtmp[operand] << (unsigned int)(arg.var[i] | (arg.var[i + 1] << 8) | (arg.var[i + 2] << 16) | (arg.var[i + 3] << 24));
			operandtmp[operand++] += L"L";
			i += 4;
		} else if (vartype >= 10 && vartype < 30) {
			varcat = varbyte;
			varid = arg.var[i++];
			if (vartype >= 20)
				varid |= arg.var[i++] << 8;
			MACRO_SEARCHVARNAME(operandtmp[operand])
			operand++;
			isignorenulljumpsetter = (varbyte == 0x7A && varid == 9)	// "GetDialogChoice"
								  || (varcat >= 0xD0);					// "General_ScenarioCounter", "General_FieldEntrance", "VAR_Glob..." etc
		} else if (vartype == 50) {
			operandtmp[operand - 1] = _(VarOpList[varbyte].opstring) + _(L"(") + operandtmp[operand - 1] + _(L")");
		} else if (vartype == 51) {
			operandtmp[operand - 2] = _(VarOpList[varbyte].opstring) + _(L"(") + operandtmp[operand - 2] + _(L", ") + operandtmp[operand - 1] + _(L")");
			operand--;
		} else if (vartype == 55) {
			if (arg.var[i + 1] < G_V_ELEMENTS(VarEntryPropList)) {
				operandtmp[operand].Empty();
				operandtmp[operand] << _(VarEntryPropList[arg.var[i + 1]].opstring) << _(L"(") << (unsigned int)arg.var[i] << _(L")");
				operand++;
				i += 2;
			} else {
				operandtmp[operand].Empty();
				operandtmp[operand] << _(VarOpList[varbyte].opstring) << _(L"(") << (unsigned int)arg.var[i + 1] << _(L", ") << (unsigned int)arg.var[i] << _(L")");
				operand++;
				i += 2;
			}
		} else if (vartype == 60) {
			varid = arg.var[i] | (arg.var[i + 1] << 8);
			unsigned int argcount = arg.var[i + 2];
			i += 3;
			auto func = FlexibleFunctionNameList.find(varid);
			wxString funcname = func != FlexibleFunctionNameList.end() ? _(func->second.name) : wxString::Format(wxT(HADES_STRING_FLEXIBLE_FUNCTION_DEFAULT_NAME "%u_%u"), varid, argcount);
			operandtmp[operand - argcount] = funcname + _(L"(") + operandtmp[operand - argcount];
			for (j = 1; j < argcount; j++)
				operandtmp[operand - argcount] += _(L", ") + operandtmp[operand - argcount + j];
			operandtmp[operand - argcount] += _(L")");
			operand -= argcount - 1;
		}
		varbyte = arg.var[i++];
		vartype = VarOpList[varbyte].type;
		operandtotalcount++;
	}
	if (operand == 1) {
		if (operandtotalcount <= 1 && isignorenulljumpsetter && ignorenulljump != NULL)
			*ignorenulljump = true;
		if (operandtmp[0][0] == L'(')
			operandtmp[0] = operandtmp[0].substr(2, operandtmp[0].length() - 4);
		return operandtmp[0];
	}
	return wxEmptyString;
}

#define BLOCK_TYPE_ROOT			0
#define BLOCK_TYPE_IF			1
#define BLOCK_TYPE_IFN			2
#define BLOCK_TYPE_ELSE			3
#define BLOCK_TYPE_WHILE		4
#define BLOCK_TYPE_SWITCH		5
#define BLOCK_TYPE_SWITCHDEF	6
#define BLOCK_TYPE_SWITCHEX		7
#define BLOCK_TYPE_SWITCHEXDEF	8
bool ScriptEditHandler::GenerateFunctionStrings_Rec(wxString& str, ScriptFunction& func, unsigned int& funcpos, unsigned int& oppos, int endfuncpos, unsigned int tabpos, int blocktype, int endblockpos, bool appendcomment) {
	int macroop,macropos;
	#define MACRO_FINDOP(OFFSET) \
		macroop = oppos+1; \
		macropos = 0; \
		if (OFFSET>=0) \
			while (macropos<OFFSET) \
				macropos += func.op[macroop++].size; \
		else \
			while (macropos>OFFSET) \
				macropos -= func.op[--macroop].size;
	
	#define MACRO_WRITECEOL() \
		if (argcomment.GetCount()>0) { \
			str += _(L" // ")+argcomment[0]; \
			for (i=1;i<argcomment.GetCount();i++) \
				str += _(L" ; ")+argcomment[i]; \
			argcomment.Empty(); \
		} \
		str += _(L"\n");

	// ignorenulljump's purpose is to avoid adding a "break" in some situations
	// The functions that lead to World Maps are examples of a situation that can occur
	// In those, there's a "set Global_ScenarioCounter" dummy line followed by another "JUMP 0" dummy line not to be turned into a "break"
	// The same thing happens in "Ramuh_SpeakBTN" (when chosing the order of the story) but this time with a line "set GetDialogChoice"
	// And the same appear with "set VAR_GlobUInt8_24" / "set VAR_GlobInt16_29" / "set VAR_GlobInt16_28" at very minor places
	// A bit differently but in the same vein: very rarely, a couple of "switch" fallback to the "default" case
	// In that situation, the script simply discards the "default" case and end the switch instead, which has an equivalent effect,
	// except that the "default" case's "break" (ie. a "JUMP 0" code) must be discarded as well
	bool ignorenulljump = false;
	unsigned int i;
	wxString tabstr = L"";
	wxArrayString argcomment;
	wxArrayString* argcommentptr = appendcomment ? &argcomment : NULL;
	for (i = 0; i < tabpos; i++)
		tabstr += _(TAB_STR);
	if (endfuncpos == -1)
		endfuncpos = func.length;
	if (endblockpos == -1)
		endblockpos = func.length;
	while ((int)funcpos<endfuncpos) {
		// Assume there are 0x05 opcodes before jump opcodes recquiring a stack value
		switch (func.op[oppos].opcode) {
		case 0x01: {
			if (blocktype==BLOCK_TYPE_ROOT && funcpos+3+func.op[oppos].arg[0].GetValue()==0) {
				str += tabstr+_(L"loop\n");
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.length;
				return false;
			} else if (funcpos+3+func.op[oppos].arg[0].GetValue()<endblockpos) {
				MACRO_FINDOP(func.op[oppos].arg[0].GetValue());
				if (macroop+1 < (int)func.op_amount && func.op[macroop+1].opcode==0x03 && 3+macropos+func.op[macroop].size+func.op[macroop+1].arg[0].GetValue()==0) {
					str += tabstr+_(L"while ( ");
					str += ConvertVarArgument(func.op[macroop].arg[0],argcommentptr);
					str += _(L" ) {");
					MACRO_WRITECEOL()
					funcpostrack[functrackline++] = funcpos;
					funcpos += func.op[oppos++].size;
					GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+macropos,tabpos+1,BLOCK_TYPE_WHILE,funcpos+macropos,appendcomment);
					str += tabstr+_(L"}\n");
					funcpostrack[functrackline] = funcpostrack[functrackline-1];
					functrackline++;
					funcpos += func.op[oppos++].size;
					funcpos += func.op[oppos++].size;
				} else if (blocktype==BLOCK_TYPE_IF && funcpos+3==endfuncpos) {
					funcpos += func.op[oppos++].size;
					return true;
				} else if (func.op[oppos].arg[0].GetValue() == 0 && ignorenulljump && endfuncpos > (int)(funcpos + func.op[oppos].size)) {
					funcpos += func.op[oppos++].size;
					ignorenulljump = false;
					str += tabstr + _(L"// JMP(0)\n");
				} else {
					str += tabstr+_(L"break\n");
					funcpostrack[functrackline++] = funcpos;
					funcpos += func.op[oppos++].size;
					return false;
				}
			} else if (blocktype==BLOCK_TYPE_IF && funcpos+3==endfuncpos && funcpos+3+func.op[oppos].arg[0].GetValue()<=endblockpos) {
				funcpos += func.op[oppos++].size;
				return true;
			} else {
				str += tabstr+_(L"break\n");
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.op[oppos++].size;
				if (blocktype==BLOCK_TYPE_IF && funcpos+3==endfuncpos && func.op[oppos].opcode==0x01) {
					funcpos += func.op[oppos++].size;
					return true;
				}
				return false;
			}
			break;
		}
		case 0x02: {
			str += tabstr+_(L"if ( ");
			str += ConvertVarArgument(func.op[oppos-1].arg[0],argcommentptr);
			str += _(L" ) {");
			MACRO_WRITECEOL()
			funcpostrack[functrackline++] = funcpos;
			funcpos += func.op[oppos++].size;
			if (GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+func.op[oppos-1].arg[0].GetValue(),tabpos+1,BLOCK_TYPE_IF,endblockpos,appendcomment)) {
				str += tabstr+_(L"} else {\n");
				funcpostrack[functrackline++] = funcpos;
				bool allreturn = GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+func.op[oppos-1].arg[0].GetValue(),tabpos+1,BLOCK_TYPE_ELSE,endblockpos,appendcomment);
				str += tabstr+_(L"}\n");
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				functrackline++;
			} else {
				str += tabstr+_(L"}\n");
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				functrackline++;
			}
			break;
		}
		case 0x03: {
			if (func.op[oppos].arg[0].GetValue()>=0) {
				str += tabstr+_(L"ifnot ( ");
				str += ConvertVarArgument(func.op[oppos-1].arg[0],argcommentptr);
				str += _(L" ) {");
				MACRO_WRITECEOL()
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.op[oppos++].size;
				GenerateFunctionStrings_Rec(str,func,funcpos,oppos,funcpos+func.op[oppos-1].arg[0].GetValue(),tabpos+1,BLOCK_TYPE_IFN,endblockpos,appendcomment);
				str += tabstr+_(L"}\n");
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				functrackline++;
			} else {
				unsigned int j;
				wxString lstr = str, rstr, keywordcheck;
				str = L"";
				funcpostrack[functrackline] = funcpostrack[functrackline-1];
				lstr = lstr.BeforeLast(L'\n',&rstr);
				for (j=functrackline-1;j>0 && funcpostrack[j]>=funcpos+3+func.op[oppos].arg[0].GetValue();j--) {
					if (funcpostrack[j] == funcpos + 3 + func.op[oppos].arg[0].GetValue()) {
						keywordcheck = lstr.AfterLast(L'\n');
						keywordcheck = GetNextThing(keywordcheck);
						if (keywordcheck.IsSameAs(L"case") || keywordcheck.IsSameAs(L"default"))
							break; // "do while" loop immediately after a switch case: keep the "case" or "default" line out of the loop
					}
					funcpostrack[j] = funcpostrack[j-1];
					lstr = lstr.BeforeLast(L'\n',&rstr);
					str = _(L"\n")+_(TAB_STR)+rstr+str;
				}
				funcpostrack[j+1] = funcpostrack[j+2];
				str = lstr+_(L"\n")+tabstr+_(L"do {")+str+_(L"\n");
				str += tabstr+_(L"} while ( ");
				str += ConvertVarArgument(func.op[oppos-1].arg[0],argcommentptr);
				str += _(L" )");
				MACRO_WRITECEOL()
				functrackline++;
				funcpostrack[functrackline++] = funcpos;
				funcpos += func.op[oppos++].size;
			}
			break;
		}
		case 0x04: {
			str += tabstr+_(HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].label)+_(L"\n");
			funcpostrack[functrackline++] = funcpos;
			if (blocktype==BLOCK_TYPE_ROOT) {
				funcpos = func.length;
				return false;
			}
			funcpos += func.op[oppos++].size;
			if (funcpos<func.length && func.op[oppos].opcode==0x01) {
				if (funcpos+3+func.op[oppos].arg[0].GetValue()<endblockpos) {
					MACRO_FINDOP(func.op[oppos].arg[0].GetValue());
					if (macroop + 1 <= (int)func.op_amount && func.op[macroop + 1].opcode == 0x03 && 3 + macropos + func.op[macroop].size + func.op[macroop + 1].arg[0].GetValue() == 0) {
						return blocktype == BLOCK_TYPE_ELSE;
					} else if (funcpos + 3 == endfuncpos) {
						funcpos += func.op[oppos++].size;
						return true;
					}
				} else if (funcpos+3==endfuncpos && funcpos+3+func.op[oppos].arg[0].GetValue()<=endblockpos) {
					funcpos += func.op[oppos++].size;
					return true;
				}
			}
			return blocktype==BLOCK_TYPE_ELSE;
		}
		case 0x05: {
			if (func.op[oppos + 1].opcode != 0x02 && func.op[oppos + 1].opcode != 0x03 && func.op[oppos + 1].opcode != 0x06 && func.op[oppos + 1].opcode != 0x0B) {
				str += tabstr + _(HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].label) + _(L" ") + ConvertVarArgument(func.op[oppos].arg[0], argcommentptr, &ignorenulljump);
				MACRO_WRITECEOL()
				funcpostrack[functrackline++] = funcpos;
			}
			funcpos += func.op[oppos++].size;
			break;
		}
		case 0x06: {
			ScriptOperation& swop = func.op[oppos];
			funcpostrack[functrackline++] = funcpos;
			unsigned int nbcasedone = 0;
			unsigned int swoppos = oppos;
			unsigned int swpos = funcpos + 4;
			unsigned int swdefpos = swop.arg[0].GetValue();
			int swendofdefpos = -1;
			int swendpos = -1;
			unsigned int highestpos = swdefpos;
			unsigned int highestendpos = swpos + swdefpos;
			unsigned int currentcasepos = 0xFFFF;
			int currentcaseendpos = -1;
			bool casedone[SCRPT_MAX_SWITCH_CASE];
			bool defislastcase = true;
			for (i = 0; i < swop.size_byte; i++) {
				casedone[i] = swop.arg[2 + i * 2].GetValue() == swdefpos;
				if (casedone[i])
					nbcasedone++;
				if (highestpos < swop.arg[2 + i * 2].GetValue())
					highestpos = swop.arg[2 + i * 2].GetValue();
				// Default position may be one of the case position
				// It's problematic if that is not the last case because it removes info on case's limits
				// Cases are then re-ordered
				if (swop.arg[2 + i * 2].GetValue() > swdefpos) {
					defislastcase = false;
					if (swendofdefpos == -1 || swendofdefpos < swpos + swop.arg[2 + i * 2].GetValue())
						swendofdefpos = swpos + swop.arg[2 + i * 2].GetValue();
				}
			}
			str += tabstr + _(L"switchex ");
			str << (unsigned int)(swop.size_byte - nbcasedone);
			str += _(L" ( ") + ConvertVarArgument(func.op[oppos - 1].arg[0], argcommentptr);
			str += _(L" ) {");
			MACRO_WRITECEOL()
			while (nbcasedone < swop.size_byte) {
				currentcasepos = 0xFFFF;
				for (i = 0; i < swop.size_byte; i++)
					if (!casedone[i] && currentcasepos > swop.arg[2 + i * 2].GetValue())
						currentcasepos = swop.arg[2 + i * 2].GetValue();
				if (currentcasepos == 0xFFFF || (swendpos >= 0 && swpos + currentcasepos >= (unsigned int)swendpos))
					break;
				currentcaseendpos = -1;
				for (i = 0; i < swop.size_byte; i++)
					if ((currentcaseendpos == -1 || currentcaseendpos > swpos + swop.arg[2 + i * 2].GetValue()) && swop.arg[2 + i * 2].GetValue() > currentcasepos)
						currentcaseendpos = swpos + swop.arg[2 + i * 2].GetValue();
				if (currentcaseendpos == -1) {
					if (swdefpos > currentcasepos)
						currentcaseendpos = (int)(swpos + swdefpos);
					else
						currentcaseendpos = swendpos;
				}
				bool needsemicolon = false;
				str += tabstr + _(L"case ");
				for (i = 0; i < swop.size_byte; i++)
					if (currentcasepos == swop.arg[2 + i * 2].GetValue()) {
						if (needsemicolon)
							str << _(L" ; ");
						else
							needsemicolon = true;
						str << (int)swop.arg[1 + i * 2].GetValue();
						casedone[i] = true;
						nbcasedone++;
					}
				str += _(L":\n");
				funcpos = swpos + swop.size - 4;
				oppos = swoppos + 1;
				while (funcpos < swpos + currentcasepos) {
					funcpos += func.op[oppos].size;
					oppos++;
				}
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str, func, funcpos, oppos, currentcaseendpos, tabpos + 1, BLOCK_TYPE_SWITCHEX, currentcaseendpos, appendcomment);
				if (highestendpos < funcpos)
					highestendpos = funcpos;
				if (func.op[oppos - 1].opcode == 0x01 && swendpos == -1 && func.op[oppos - 1].arg[0].GetValue() >= 0) {
					swendpos = funcpos + func.op[oppos - 1].arg[0].GetValue();
					if (defislastcase)
						swendofdefpos = swendpos;
				}
			}
			if ((int)highestendpos < swendofdefpos)
				highestendpos = swendofdefpos;
			funcpos = swpos + swop.size - 4;
			oppos = swoppos + 1;
			while (funcpos < swpos + swdefpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			if (swendofdefpos >= 0 && (int)funcpos < swendofdefpos) {
				str += tabstr + _(L"default:\n");
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str, func, funcpos, oppos, swendofdefpos, tabpos + 1, BLOCK_TYPE_SWITCHEXDEF, swendofdefpos, appendcomment);
			} else if (swendofdefpos < 0 && (int)funcpos < endblockpos && func.op[oppos - 1].opcode != 0x01) {
				ignorenulljump = true;
			}
			while (funcpos < highestendpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			str += tabstr + _(L"}\n");
			funcpostrack[functrackline] = funcpostrack[functrackline - 1];
			functrackline++;
			break;
		}
		case 0x0B: {
			ScriptOperation& swop = func.op[oppos];
			str += tabstr + _(L"switch ");
			str << (unsigned int)swop.size_byte;
			str += _(L" ( ") + ConvertVarArgument(func.op[oppos - 1].arg[0], argcommentptr);
			str += _(L" ) from ");
			str << (unsigned int)swop.arg[0].GetValue();
			str += _(" {");
			MACRO_WRITECEOL()
			funcpostrack[functrackline++] = funcpos;
			unsigned int nbcasedone = 0;
			unsigned int swoppos = oppos;
			unsigned int swpos = funcpos + 1;
			unsigned int swdefpos = swop.arg[1].GetValue();
			int swendofdefpos = -1;
			int swendpos = -1;
			unsigned int highestpos = swdefpos;
			unsigned int highestendpos = swpos + swdefpos;
			unsigned int currentcasepos = 0xFFFF;
			int currentcaseendpos = -1;
			bool casedone[SCRPT_MAX_SWITCH_CASE];
			bool defislastcase = true;
			for (i = 0; i < swop.size_byte; i++) {
				casedone[i] = swop.arg[2 + i].GetValue() == swdefpos;
				if (casedone[i])
					nbcasedone++;
				if (highestpos < swop.arg[2 + i].GetValue())
					highestpos = swop.arg[2 + i].GetValue();
				// Default position may be one of the case position
				// It's problematic if that is not the last case because it removes info on case's limits
				// Cases are then re-ordered
				if (swop.arg[2 + i].GetValue() > swdefpos) {
					defislastcase = false;
					if (swendofdefpos == -1 || swendofdefpos < swpos + swop.arg[2 + i].GetValue())
						swendofdefpos = swpos + swop.arg[2 + i].GetValue();
				}
			}
			while (nbcasedone < swop.size_byte) {
				currentcasepos = 0xFFFF;
				for (i = 0; i < swop.size_byte; i++)
					if (!casedone[i] && currentcasepos > swop.arg[2 + i].GetValue())
						currentcasepos = swop.arg[2 + i].GetValue();
				if (currentcasepos == 0xFFFF || (swendpos >= 0 && swpos + currentcasepos >= (unsigned int)swendpos))
					break;
				currentcaseendpos = -1;
				for (i = 0; i < swop.size_byte; i++)
					if ((currentcaseendpos == -1 || currentcaseendpos > swpos + swop.arg[2 + i].GetValue()) && swop.arg[2 + i].GetValue() > currentcasepos)
						currentcaseendpos = swpos + swop.arg[2 + i].GetValue();
				if (currentcaseendpos == -1) {
					if (swdefpos > currentcasepos)
						currentcaseendpos = (int)(swpos + swdefpos);
					else
						currentcaseendpos = swendpos;
				}
				bool needsemicolon = false;
				str += tabstr + _(L"case +");
				for (i = 0; i < swop.size_byte; i++)
					if (currentcasepos == swop.arg[2 + i].GetValue()) {
						if (needsemicolon)
							str << _(L" ; +");
						else
							needsemicolon = true;
						str << (int)i;
						casedone[i] = true;
						nbcasedone++;
					}
				str += _(L":\n");
				funcpos = swpos + swop.size - 1;
				oppos = swoppos + 1;
				while (funcpos < swpos + currentcasepos) {
					funcpos += func.op[oppos].size;
					oppos++;
				}
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str, func, funcpos, oppos, currentcaseendpos, tabpos + 1, BLOCK_TYPE_SWITCH, currentcaseendpos, appendcomment);
				if (highestendpos < funcpos)
					highestendpos = funcpos;
				if (func.op[oppos - 1].opcode == 0x01 && swendpos == -1 && func.op[oppos - 1].arg[0].GetValue() >= 0) {
					swendpos = funcpos + func.op[oppos - 1].arg[0].GetValue();
					if (defislastcase)
						swendofdefpos = swendpos;
				}
			}
			if ((int)highestendpos < swendofdefpos)
				highestendpos = swendofdefpos;
			funcpos = swpos + swop.size - 1;
			oppos = swoppos + 1;
			while (funcpos < swpos + swdefpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			if (swendofdefpos >= 0 && (int)funcpos < swendofdefpos) {
				str += tabstr + _(L"default:\n");
				funcpostrack[functrackline++] = funcpos;
				GenerateFunctionStrings_Rec(str, func, funcpos, oppos, swendofdefpos, tabpos + 1, BLOCK_TYPE_SWITCHDEF, swendofdefpos, appendcomment);
			} else if (swendofdefpos < 0 && (int)funcpos < endblockpos && func.op[oppos - 1].opcode != 0x01) {
				ignorenulljump = true;
			}
			while (funcpos < highestendpos) {
				funcpos += func.op[oppos].size;
				oppos++;
			}
			str += tabstr + _(L"}\n");
			funcpostrack[functrackline] = funcpostrack[functrackline - 1];
			functrackline++;
			break;
		}
		default:
			str += tabstr+_(HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].label)+_(L"( ");
			for (i=0;i<func.op[oppos].arg_amount;i++) {
				ScriptArgument& arg = func.op[oppos].arg[i];
				if (arg.is_var)
					str += ConvertVarArgument(arg,argcommentptr);
				else if (func.op[oppos].opcode==0x29)
					str << L"( " << (int16_t)(arg.GetValue() & 0xFFFF) << L", " << (int16_t)((arg.GetValue() >> 16) & 0xFFFF) << L" )";
				else
					str << arg.GetValue();
				if (i+1<func.op[oppos].arg_amount)
					str += _(L", ");
				if (appendcomment && !arg.is_var) {
					wxString argcommenttoken = GetArgumentDescription(arg.GetValue(),HADES_STRING_SCRIPT_OPCODE[func.op[oppos].opcode].arg_type[i]);
					if (argcommenttoken.Len()>0)
						argcomment.Add(argcommenttoken);
				}
			}
			str += _(L" )");
			MACRO_WRITECEOL()
			funcpostrack[functrackline++] = funcpos;
			funcpos += func.op[oppos++].size;
		}
	}
	return false;
}

void ScriptEditHandler::GenerateFunctionStrings(bool appendcomment) {
	unsigned int i, j, funci = 0, funcpos, oppos, entrytmp = entry_selection;
	func_str.resize(script.entry_amount);
	localvar_str.resize(script.entry_amount);
	/*fstream fout("aaaa.txt",ios::app|ios::out);
	for (i=0;i<script.entry_amount;i++) for (j=0;j<script.entry_function_amount[i];j++) {
	fout << "New Function : " << i << " " << j << endl;
	for (unsigned int k=0;k<script.func[i][j].op_amount;k++) {fout << std::hex << (unsigned int)script.func[i][j].op[k].size << " : " << std::hex << (unsigned int)script.func[i][j].op[k].opcode;
	for (unsigned int l=0;l<script.func[i][j].op[k].arg_amount;l++) fout << " " << std::hex << (unsigned int)script.func[i][j].op[k].arg[l].value;
	fout << endl;} fout << endl;}*/
	globalvar_str = _(L"");
	if (script.global_data.amount > 0) {
		for (j = 0; j < script.global_data.amount; j++) {
			globalvar_str += _(L"global ");
			if (script.global_data.type[j] == SCRIPT_VARIABLE_TYPE_BOOL)
				globalvar_str += _(L"bool ");
			else {
				if (script.global_data.type[j] == SCRIPT_VARIABLE_TYPE_INT)
					globalvar_str += _(L"int");
				else if (script.global_data.type[j] == SCRIPT_VARIABLE_TYPE_UINT)
					globalvar_str += _(L"uint");
				globalvar_str += wxString::Format(wxT("%u "), script.global_data.size[j]);
			}
			globalvar_str += wxString::Format(wxT("%s %s%u\n"), script.global_data.name[j], VarOpList[script.global_data.cat[j]].opstring, script.global_data.id[j]);
		}
	}
	for (i = 0; i < script.entry_amount; i++) {
		localvar_str[i] = _(L"");
		if (script.entry_local_var[i] > 0)
			localvar_str[i] += wxString::Format(wxT("allocate %u\n"), script.entry_local_var[i]);
		if (script.global_data.amount > 0) {
			if (script.entry_local_var[i] > 0)
				localvar_str[i] += _(L"\n");
			localvar_str[i] += globalvar_str;
		}
		if (script.local_data[i].amount > 0) {
			localvar_str[i] += _(L"\n");
			for (j = 0; j < script.local_data[i].amount; j++) {
				localvar_str[i] += _(L"local ");
				if (script.local_data[i].type[j] == SCRIPT_VARIABLE_TYPE_BOOL)
					localvar_str[i] += _(L"bool ");
				else {
					if (script.local_data[i].type[j] == SCRIPT_VARIABLE_TYPE_INT)
						localvar_str[i] += _(L"int");
					else if (script.local_data[i].type[j] == SCRIPT_VARIABLE_TYPE_UINT)
						localvar_str[i] += _(L"uint");
					localvar_str[i] += wxString::Format(wxT("%u "), script.local_data[i].size[j]);
				}
				localvar_str[i] += wxString::Format(wxT("%s %s%u\n"), script.local_data[i].name[j], VarOpList[script.local_data[i].cat[j]].opstring, script.local_data[i].id[j]);
			}
		}
		func_str[i].resize(script.entry_function_amount[i]);
		entry_selection = i;
		for (j = 0; j < script.entry_function_amount[i]; j++) {
			funcpos = 0;
			oppos = 0;
			functrackline = 0;
			funcpostrack[functrackline++] = funcpos;
			if (script.func[i][j].length == 0)
				func_str[i][j] = _(TAB_STR) + _(L"forward");
			while (funcpos < script.func[i][j].length)
				GenerateFunctionStrings_Rec(func_str[i][j], script.func[i][j], funcpos, oppos, -1, 1, BLOCK_TYPE_ROOT, -1, appendcomment);
			funcpostrack[functrackline++] = script.func[i][j].length;
			func_str[i][j] = functionlist_str[funci] + _(L"\n") + func_str[i][j];
			funci++;
		}
	}
	entry_selection = entrytmp;
}

void ScriptEditHandler::GenerateEntryNames() {
	unsigned int i, j, k, funcpos, oppos;
	vector<bool> foundname;
	foundname.resize(script.entry_amount, false);
	for (i = 0; i < script.entry_amount; i++) {
		for (j = 0; j < script.entry_function_amount[i] && !foundname[i]; j++) {
			funcpos = 0;
			oppos = 0;
			while (funcpos < script.func[i][j].length && !foundname[i]) {
				ScriptOperation& op = script.func[i][j].op[oppos++];
				if (op.opcode == 0x2F && !foundname[i]) {
					for (k = 0; k < G_V_ELEMENTS(HADES_STRING_MODEL_NAME); k++)
						if (op.arg[0].GetValue() == HADES_STRING_MODEL_NAME[k].id) {
							entry_model_index[i] = k;
							wxString entrynewname = HADES_STRING_MODEL_NAME[k].label;
							entrynewname.Replace(_(L" "), _(L"_"));
							EntryChangeName(i, entrynewname);
							foundname[i] = true;
							break;
						}
				} else if (op.opcode == 0x07) {
					if (!op.arg[0].is_var && op.arg[0].GetValue() < script.entry_amount) {
						EntryChangeName(op.arg[0].GetValue(), wxString::Format(wxT("Code%u"), op.arg[0].GetValue()));
						foundname[op.arg[0].GetValue()] = true;
					}
				} else if (op.opcode == 0x08) {
					if (!op.arg[0].is_var && op.arg[0].GetValue() < script.entry_amount) {
						EntryChangeName(op.arg[0].GetValue(), wxString::Format(wxT("Region%u"), op.arg[0].GetValue()));
						foundname[op.arg[0].GetValue()] = true;
					}
				}
				funcpos += op.size;
			}
		}
	}
}

void ScriptEditHandler::UpdateGlobalLocalStrings(int ignoreentry) {
	unsigned int i,j;
	wxString line, tmpstr, tmprest;
	size_t pos;
	globalvar_str = _(L"");
	for (j=0;j<script.global_data.amount;j++) {
		globalvar_str += _(L"global ");
		if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_BOOL)
			globalvar_str += _(L"bool ");
		else {
			if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_INT)
				globalvar_str += _(L"int");
			else if (script.global_data.type[j]==SCRIPT_VARIABLE_TYPE_UINT)
				globalvar_str += _(L"uint");
			globalvar_str += wxString::Format(wxT("%u "),script.global_data.size[j]);
		}
		globalvar_str += wxString::Format(wxT("%s %s%u\n"),script.global_data.name[j],VarOpList[script.global_data.cat[j]].opstring,script.global_data.id[j]);
	}
	for (i=0;i<script.entry_amount;i++) {
		if (i==ignoreentry)
			continue;
		wxString localstr = localvar_str[i];
		wxString localstrtmp = localstr;
		pos = 0;
		while (localstrtmp.Len()>0) {
			line = localstrtmp.BeforeFirst(L'\n',&tmprest);
			localstrtmp = tmprest;
			if (line.Mid(0,7).IsSameAs(L"global ")) {
				tmpstr = localstr.Mid(0,pos);
				localstr = tmpstr+localstr.Mid(pos+line.Len()+1);
			} else
				pos += line.Len()+1;
		}
		if (localstr.Mid(0,9).IsSameAs(L"allocate ")) {
			tmpstr = localstr.BeforeFirst(L'\n',&tmprest)+_(L"\n\n");
			if (tmprest[0]==L'\n' && tmprest[1]==L'\n')
				tmprest = tmprest.Mid(1);
		} else
			tmpstr = _(L"");
		if (script.global_data.amount>0) {
			tmpstr += globalvar_str;
		}
		localvar_str[i] = tmpstr+tmprest;
	}
}

void ScriptEditHandler::EntryChangeName(unsigned int entry, wxString newname) {
	unsigned int i, j, funci = 0;
	entry_name[entry] = newname;
	if (handler_dialog)
		handler_dialog->entrylist_str[entry] = _(L"Entry ") + newname;
	for (i = 0; i < script.entry_amount; i++)
		for (j = 0; j < script.entry_function_amount[i]; j++) {
			if (i == entry) {
				functionlist_str[funci] = GetFunctionName(entry, script.function_type[i][j]);
				if (handler_dialog)
					handler_dialog->m_functionlist->SetItemText(funci, functionlist_str[funci]);
			}
			funci++;
		}
}

//=============================//
//         Parse Code          //
//=============================//

struct VarargSubBlock {
public:
	stack<uint8_t> binary_op;
	stack<int> buffer_catchup;
	unsigned int arg_count = 0;
	int immediate_catchup = 0;
	bool wait_for_value = true;
	wxString block_function = wxEmptyString;

	int ResolveAllCatchup(unsigned int* immediate) {
		int catchup = 0;
		*immediate = immediate_catchup;
		immediate_catchup = 0;
		while (!buffer_catchup.empty()) {
			catchup += buffer_catchup.top();
			buffer_catchup.pop();
			binary_op.pop();
		}
		return *immediate + catchup;
	}

	int ResolveImmediateCatchup(unsigned int* immediate) {
		*immediate = immediate_catchup;
		immediate_catchup = 0;
		return *immediate;
	}
};

bool IsAlphaNum(wchar_t c) {
	return (c >= L'0' && c <= L'9') || (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || c == L'_';
}
bool IsOperator(wchar_t c) {
	return c == L'+' || c == L'-' || c == L'*' || c == L'/' || c == L'%' || c == L'&' || c == L'|' || c == L'<' || c == L'>' || c == L'=' || c == L'!' || c == L'~' || c == L'#' || c == L'^' || c == L'$'; // || c==L',' || c==L'[' || c==L']'
}
wxString GetNextVarThing(wxString& varstr) {
	wxString tmp, res = L"";
	size_t pos = varstr.find_first_not_of(L' ');
	if (pos == string::npos)
		return wxEmptyString;
	size_t lineshift = pos;
	tmp = varstr.Mid(pos);
	res += tmp[0];
	if (IsAlphaNum(tmp[0])) {
		for (pos = 1; IsAlphaNum(tmp[pos]); pos++)
			res += tmp[pos];
	} else if (IsOperator(tmp[0])) {
		for (pos = 1; IsOperator(tmp[pos]); pos++)
			res += tmp[pos];
	} else {
		pos = 1;
	}
	lineshift += pos;
	varstr = varstr.Mid(lineshift);
	return res;
}
inline wstring GetRenameOrDefault(map<wstring, wstring>& renamemap, wstring name) {
	auto it = renamemap.find(name);
	if (it != renamemap.end())
		return it->second;
	return name;
}
vector<uint8_t> ConvertStringToVararg(wxString varstr, wxString& errmsg, ScriptLocalVariableSet* localvar) {
	int objecttype = -1;
	unsigned int i, j, len, lastimmediatecatchup = 0;
	int expectedval = 1;
	uint8_t sz = 0, bufferpos = 0;
	wxString tmpstr;
	vector<uint8_t> res;
	uint32_t tmpval, tmpval2;
	stack<VarargSubBlock> blockstack;
	map<wstring, wstring> renamemap;
	if (varstr.IsEmpty()) {
		errmsg = _(HADES_STRING_SCRIPT_VARARG_EMPTY);
		return vector<uint8_t>();
	}

	#define MACRO_APPEND_BYTE(BYTE, INCREASEPOS) \
		res.insert(res.begin() + bufferpos, BYTE); \
		sz++; \
		if (INCREASEPOS) bufferpos++;

	size_t replacepos;
	for (i = 0; i < VarNameList.size(); i++) {
		tmpstr = wxEmptyString;
		replacepos = varstr.find(VarNameList[i].name);
		while (replacepos != wxNOT_FOUND) {
			if (tmpstr.IsEmpty()) {
				tmpstr = wxString::Format(wxT("%s%u"), VarOpList[VarNameList[i].cat].opstring, VarNameList[i].id);
				renamemap[tmpstr.ToStdWstring()] = VarNameList[i].name;
			}
			if ((replacepos == 0 || !IsAlphaNum(varstr[replacepos - 1])) && (replacepos + VarNameList[i].name.length() >= varstr.Len() || !IsAlphaNum(varstr[replacepos + VarNameList[i].name.length()])))
				varstr.replace(replacepos, VarNameList[i].name.length(), tmpstr);
			replacepos = varstr.find(VarNameList[i].name, replacepos + 1);
		}
	}
	for (auto it = FlexibleFunctionNameList.begin(); it != FlexibleFunctionNameList.end(); it++) {
		tmpstr = wxEmptyString;
		replacepos = varstr.find(it->second.name);
		while (replacepos != wxNOT_FOUND) {
			if (tmpstr.IsEmpty()) {
				tmpstr = wxString::Format(wxT("%s%u_%u"), HADES_STRING_FLEXIBLE_FUNCTION_DEFAULT_NAME, it->second.id, it->second.argcount);
				renamemap[tmpstr.ToStdWstring()] = it->second.name;
			}
			if ((replacepos == 0 || !IsAlphaNum(varstr[replacepos - 1])) && (replacepos + it->second.name.length() >= varstr.Len() || !IsAlphaNum(varstr[replacepos + it->second.name.length()])))
				varstr.replace(replacepos, it->second.name.length(), tmpstr);
			replacepos = varstr.find(it->second.name, replacepos + 1);
		}
	}
	for (i = 0; i < localvar->amount; i++) {
		tmpstr = wxEmptyString;
		replacepos = varstr.find(localvar->name[i]);
		while (replacepos != wxNOT_FOUND) {
			if (tmpstr.IsEmpty()) {
				tmpstr = wxString::Format(wxT("%s%u"), VarOpList[localvar->cat[i]].opstring, localvar->id[i]);
				renamemap[tmpstr.ToStdWstring()] = localvar->name[i];
			}
			if ((replacepos == 0 || !IsAlphaNum(varstr[replacepos - 1])) && (replacepos + localvar->name[i].length() >= varstr.Len() || !IsAlphaNum(varstr[replacepos + localvar->name[i].length()])))
				varstr.replace(replacepos, localvar->name[i].length(), tmpstr);
			replacepos = varstr.find(localvar->name[i], replacepos + 1);
		}
	}
	blockstack.push(VarargSubBlock());
	MACRO_APPEND_BYTE(0x7F, false)
	while (varstr.Length() > 0) {
		tmpstr = GetNextVarThing(varstr);
		len = tmpstr.Length();
		if (tmpstr.IsSameAs(L"//"))
			break;
		if (tmpstr.IsSameAs(L"[")) {
			bufferpos -= lastimmediatecatchup;
			if (blockstack.top().wait_for_value) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MISS_VALUE), tmpstr);
				return vector<uint8_t>();
			}
			MACRO_APPEND_BYTE(0x29, true)
			tmpstr = GetNextVarThing(varstr);
			if (tmpstr.IsNumber()) {
				tmpval = wxAtoi(tmpstr);
				if (tmpval > 0xFF) {
					errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), tmpstr, 0xFF);
					return vector<uint8_t>();
				}
				MACRO_APPEND_BYTE(tmpval, true)
			} else {
				for (j = 0; j < G_V_ELEMENTS(ScriptCharacterField); j++)
					if (tmpstr.IsSameAs(ScriptCharacterField[j].name)) {
						MACRO_APPEND_BYTE(ScriptCharacterField[j].id, true)
						break;
					}
				if (j == G_V_ELEMENTS(ScriptCharacterField)) {
					errmsg = _(HADES_STRING_SCRIPT_VARARG_ARRAY);
					return vector<uint8_t>();
				}
			}
			tmpstr = GetNextVarThing(varstr);
			if (!tmpstr.IsSameAs(L"]")) {
				errmsg = _(HADES_STRING_SCRIPT_VARARG_BRACKETS);
				return vector<uint8_t>();
			}
			bufferpos += lastimmediatecatchup;
			continue;
		}
		lastimmediatecatchup = 0;
		if (tmpstr.IsSameAs(L"(")) {
			if (!blockstack.top().wait_for_value) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNEXPECTED), tmpstr);
				return vector<uint8_t>();
			}
			blockstack.push(VarargSubBlock());
		} else if (tmpstr.IsSameAs(L")")) {
			if (blockstack.size() <= 1) {
				errmsg = _(HADES_STRING_SCRIPT_VARARG_TOO_PAR);
				return vector<uint8_t>();
			}
			if (blockstack.top().wait_for_value) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MISS_VALUE), tmpstr);
				return vector<uint8_t>();
			}
			if (blockstack.top().arg_count > 1) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MISS_COMMA), blockstack.top().block_function);
				return vector<uint8_t>();
			}
			bufferpos += blockstack.top().ResolveAllCatchup(&lastimmediatecatchup);
			blockstack.pop();
			bufferpos += blockstack.top().ResolveImmediateCatchup(&lastimmediatecatchup);
			lastimmediatecatchup = 0; // Make sure that "SomeFunction(0)[HP]" is interpreted as "(SomeFunction(0))[HP]" while still having "~1[HP]" be interpreted as "~(1[HP])"
			blockstack.top().wait_for_value = false;
		} else if (tmpstr.IsSameAs(L",")) {
			if (blockstack.top().wait_for_value) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MISS_VALUE), tmpstr);
				return vector<uint8_t>();
			}
			if (blockstack.top().arg_count < 2) {
				if (blockstack.top().arg_count == 1)
					errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_COMMA), blockstack.top().block_function);
				else
					errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNEXPECTED), tmpstr);
				return vector<uint8_t>();
			}
			bufferpos += blockstack.top().ResolveAllCatchup(&lastimmediatecatchup);
			blockstack.top().wait_for_value = true;
			blockstack.top().arg_count--;
		} else if (tmpstr.IsNumber() && !tmpstr.IsSameAs(L"-") && !tmpstr.IsSameAs(L"+")) {
			objecttype = 6;
			if (!blockstack.top().wait_for_value) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_VALUE), tmpstr);
				return vector<uint8_t>();
			}
			tmpval = wxAtoi(tmpstr);
			if (tmpval > 0xFFFF) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), tmpstr, 0xFFFF);
				return vector<uint8_t>();
			}
			MACRO_APPEND_BYTE(0x7D, true)
			MACRO_APPEND_BYTE(tmpval & 0xFF, true)
			MACRO_APPEND_BYTE((tmpval >> 8) & 0xFF, true)
			blockstack.top().wait_for_value = false;
			bufferpos += blockstack.top().ResolveImmediateCatchup(&lastimmediatecatchup);
			expectedval--;
		} else if (tmpstr.Mid(0, len - 1).IsNumber() && tmpstr[len - 1] == L'L') {
			objecttype = 7;
			if (!blockstack.top().wait_for_value) {
				errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_VALUE), tmpstr);
				return vector<uint8_t>();
			}
			tmpstr.ToULong((unsigned long*)&tmpval);
			MACRO_APPEND_BYTE(0x7E, true)
			MACRO_APPEND_BYTE(tmpval & 0xFF, true)
			MACRO_APPEND_BYTE((tmpval >> 8) & 0xFF, true)
			MACRO_APPEND_BYTE((tmpval >> 16) & 0xFF, true)
			MACRO_APPEND_BYTE((tmpval >> 24) & 0xFF, true)
			blockstack.top().wait_for_value = false;
			bufferpos += blockstack.top().ResolveImmediateCatchup(&lastimmediatecatchup);
			expectedval--;
		} else {
			for (i = 0; i < 256; i++) {
				if (tmpstr.IsSameAs(VarOpList[i].opstring) || (((VarOpList[i].type >= 10 && VarOpList[i].type < 30) || VarOpList[i].type == 60) && tmpstr.Mid(0, VarOpList[i].opstring.length()).IsSameAs(VarOpList[i].opstring))) {
					if (VarOpList[i].type == 1) // Determine whether it's the pre-fixed or the post-fixed operator
						if (blockstack.top().wait_for_value)
							i += 2;
					objecttype = VarOpList[i].type;
					if (objecttype == 100) { // unknown/unused
						errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
						return vector<uint8_t>();
					}
					if (objecttype == 0) { // unary operator pre-fixed
						if (!blockstack.top().wait_for_value) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNEXPECTED), tmpstr);
							return vector<uint8_t>();
						}
						MACRO_APPEND_BYTE(i, false)
						blockstack.top().immediate_catchup++;
					} else if (objecttype == 1) { // unary operator post-fixed
						if (blockstack.top().wait_for_value) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNEXPECTED), tmpstr);
							return vector<uint8_t>();
						}
						MACRO_APPEND_BYTE(i, true)
					} else if (objecttype == 2) { // binary operator
						if (blockstack.top().wait_for_value) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MISS_VALUE), tmpstr);
							return vector<uint8_t>();
						}
						while (!blockstack.top().binary_op.empty() && VarBinaryOpPrecedence[blockstack.top().binary_op.top()] <= VarBinaryOpPrecedence[i]) {
							bufferpos += blockstack.top().buffer_catchup.top();
							blockstack.top().buffer_catchup.pop();
							blockstack.top().binary_op.pop();
						}
						MACRO_APPEND_BYTE(i, false)
						blockstack.top().buffer_catchup.push(1);
						blockstack.top().binary_op.push(i);
						blockstack.top().wait_for_value = true;
						expectedval++;
					} else {
						if (!blockstack.top().wait_for_value) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_VALUE), GetRenameOrDefault(renamemap, tmpstr.ToStdWstring()));
							return vector<uint8_t>();
						}
						blockstack.top().wait_for_value = false;
						if (objecttype >= 10 && objecttype < 20) { // get variable (array byte = 1)
							MACRO_APPEND_BYTE(i, true)
							wxString num = tmpstr.Mid(VarOpList[i].opstring.length());
							if (!num.IsNumber()) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
								return vector<uint8_t>();
							}
							tmpval = wxAtoi(num);
							if (tmpval > 0xFF) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), num, 0xFF);
								return vector<uint8_t>();
							}
							MACRO_APPEND_BYTE(tmpval, true)
							expectedval--;
						} else if (objecttype >= 20 && objecttype < 30) { // get variable (array byte = 2)
							MACRO_APPEND_BYTE(i, true)
							wxString num = tmpstr.Mid(VarOpList[i].opstring.length());
							if (!num.IsNumber()) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
								return vector<uint8_t>();
							}
							tmpval = wxAtoi(num);
							if (tmpval > 0xFFFF) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), num, 0xFFFF);
								return vector<uint8_t>();
							}
							MACRO_APPEND_BYTE(tmpval & 0xFF, true)
							MACRO_APPEND_BYTE((tmpval >> 8) & 0xFF, true)
							expectedval--;
						} else if (objecttype == 50) { // function (1 arg)
							MACRO_APPEND_BYTE(i, false)
							blockstack.top().immediate_catchup++;
							blockstack.push(VarargSubBlock());
							blockstack.top().arg_count = 1;
							blockstack.top().block_function = _(GetRenameOrDefault(renamemap, tmpstr.ToStdWstring()));
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsSameAs(L"(")) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
								return vector<uint8_t>();
							}
						} else if (objecttype == 51) { // function (2 args)
							MACRO_APPEND_BYTE(i, false)
							expectedval++;
							blockstack.top().immediate_catchup++;
							blockstack.push(VarargSubBlock());
							blockstack.top().arg_count = 2;
							blockstack.top().block_function = _(GetRenameOrDefault(renamemap, tmpstr.ToStdWstring()));
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsSameAs(L"(")) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
								return vector<uint8_t>();
							}
						} else if (objecttype == 55) { // GetEntryProperty
							MACRO_APPEND_BYTE(i, true)
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsSameAs(L"(")) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
								return vector<uint8_t>();
							}
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsNumber()) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_GETENTRY);
								return vector<uint8_t>();
							}
							tmpval = wxAtoi(tmpstr);
							if (tmpval > 0xFF) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), tmpstr, 0xFF);
								return vector<uint8_t>();
							}
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsSameAs(L",")) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MISS_COMMA), "GetEntryProperty");
								return vector<uint8_t>();
							}
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsNumber()) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_GETENTRY);
								return vector<uint8_t>();
							}
							tmpval2 = wxAtoi(tmpstr);
							if (tmpval2 > 0xFF) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), tmpstr, 0xFF);
								return vector<uint8_t>();
							}
							MACRO_APPEND_BYTE(tmpval2, true)
							MACRO_APPEND_BYTE(tmpval, true)
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsSameAs(L")")) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_PAR);
								return vector<uint8_t>();
							}
							expectedval--;
						} else if (objecttype == 60) { // custom function (flexible number of args)
							size_t oplen = VarOpList[i].opstring.length();
							size_t underpos = tmpstr.find_first_of(L'_', oplen);
							if (underpos == string::npos) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
								return vector<uint8_t>();
							}
							wxString num = tmpstr.Mid(oplen, underpos - oplen);
							if (!num.IsNumber()) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
								return vector<uint8_t>();
							}
							tmpval = wxAtoi(num);
							if (tmpval > 0xFFFF) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), num, 0xFFFF);
								return vector<uint8_t>();
							}
							num = tmpstr.Mid(underpos + 1);
							if (!num.IsNumber()) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
								return vector<uint8_t>();
							}
							tmpval2 = wxAtoi(num);
							if (tmpval2 > 0xFF) {
								errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), num, 0xFF);
								return vector<uint8_t>();
							}
							MACRO_APPEND_BYTE(tmpval2, false)
							MACRO_APPEND_BYTE((tmpval >> 8) & 0xFF, false)
							MACRO_APPEND_BYTE(tmpval & 0xFF, false)
							MACRO_APPEND_BYTE(i, false)
							expectedval += tmpval2 - 1;
							blockstack.top().immediate_catchup += 4;
							blockstack.push(VarargSubBlock());
							blockstack.top().arg_count = tmpval2;
							blockstack.top().block_function = _(GetRenameOrDefault(renamemap, tmpstr.ToStdWstring()));
							tmpstr = GetNextVarThing(varstr);
							if (!tmpstr.IsSameAs(L"(")) {
								errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
								return vector<uint8_t>();
							}
						}
						bufferpos += blockstack.top().ResolveImmediateCatchup(&lastimmediatecatchup);
					}
					break;
				}
			}
			if (i >= 256) {
				for (i = 0; i < G_V_ELEMENTS(VarEntryPropList); i++)
					if (tmpstr.IsSameAs(VarEntryPropList[i].opstring)) { // GetEntry specific property
						if (!blockstack.top().wait_for_value) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_VALUE), tmpstr);
							return vector<uint8_t>();
						}
						blockstack.top().wait_for_value = false;
						MACRO_APPEND_BYTE(0x78, true)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L"(")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_OPEN_PAR);
							return vector<uint8_t>();
						}
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsNumber()) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_GETENTRY);
							return vector<uint8_t>();
						}
						tmpval = wxAtoi(tmpstr);
						if (tmpval > 0xFF) {
							errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_TOO_HIGH), tmpstr, 0xFF);
							return vector<uint8_t>();
						}
						MACRO_APPEND_BYTE(tmpval, true)
						tmpstr = GetNextVarThing(varstr);
						if (!tmpstr.IsSameAs(L")")) {
							errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_PAR);
							return vector<uint8_t>();
						}
						MACRO_APPEND_BYTE(VarEntryPropList[i].type - 1000, true)
						bufferpos += blockstack.top().ResolveImmediateCatchup(&lastimmediatecatchup);
						expectedval--;
						break;
					}
				if (i >= G_V_ELEMENTS(VarEntryPropList)) {
					errmsg.Printf(wxT(HADES_STRING_SCRIPT_VARARG_UNKNOWN), tmpstr);
					return vector<uint8_t>();
				}
			}
		}
	}
	if (blockstack.size() > 1) {
		errmsg = _(HADES_STRING_SCRIPT_VARARG_MISS_PAR);
		return vector<uint8_t>();
	}
	if (expectedval != 0) { // Extra (legacy) check that should be redundant with the checks performed with blockstack
		errmsg = _(HADES_STRING_SCRIPT_VARARG_OPVAMISMATCH);
		return vector<uint8_t>();
	}
	return res;
}

uint8_t GetLocalCategoryFromType(int localtype, int type, int size) {
	if (localtype==SCRIPT_VARIABLE_LOCALTYPE_LOCAL) {
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==16)
			return 0xDE;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==16)
			return 0xDA;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==8)
			return 0xD6;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==8)
			return 0xD2;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==24)
			return 0xCE;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==24)
			return 0xCA;
		if (type==SCRIPT_VARIABLE_TYPE_BOOL && size==1)
			return 0xC6;
		if (type==SCRIPT_VARIABLE_TYPE_SBOOL && size==1)
			return 0xC2;
		return 0xCA;
	} else if (localtype==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL) {
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==16)
			return 0xDD;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==16)
			return 0xD9;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==8)
			return 0xD5;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==8)
			return 0xD1;
		if (type==SCRIPT_VARIABLE_TYPE_UINT && size==24)
			return 0xCD;
		if (type==SCRIPT_VARIABLE_TYPE_INT && size==24)
			return 0xC9;
		if (type==SCRIPT_VARIABLE_TYPE_BOOL && size==1)
			return 0xC5;
		if (type==SCRIPT_VARIABLE_TYPE_SBOOL && size==1)
			return 0xC1;
		return 0xC9;
	}
	return 0xCA;
}

struct ScriptLocalVariableSetTmp {
	int local_type;
	int type;
	int size;
	wstring name;
	uint8_t cat;
	uint16_t id;
};
ScriptLocalVariableSetTmp localvartmp[0x100];
ScriptLocalVariableSet* ScriptEditHandler::ParseLocal(LogStruct& log, wxString str) {
	ScriptLocalVariableSet* res = new ScriptLocalVariableSet;
	wxString linestr, token, errstr;
	unsigned int i,j,k,vari;
	size_t pos;
	bool isok;
	res->allocate_amount = 0;
	
	#define MACRO_REINIT_LOCAL() \
		localvartmp[vari].local_type = SCRIPT_VARIABLE_LOCALTYPE_UNKNOWN; \
		localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_UNKNOWN; \
		localvartmp[vari].size = 0; \
		localvartmp[vari].name = L""; \
		localvartmp[vari].cat = 0; \
		localvartmp[vari].id = 0;
	
	vari = 0;
	MACRO_REINIT_LOCAL()
	while (!str.IsEmpty()) {
		pos = str.find_first_of(L'\n');
		linestr = str.Mid(0,pos);
		if (pos!=string::npos)
			str = str.Mid(pos+1);
		else
			str = wxEmptyString;
		token = GetNextThing(linestr);
		if (token==wxEmptyString)
			continue;
		if (token.IsSameAs(L"allocate")) {
			token = GetNextThing(linestr);
			if (!token.IsNumber())
				log.AddError(HADES_STRING_LSCRIPT_ALLOC_NUMBER);
			else
				res->allocate_amount = wxAtoi(token);
		} else if (token.IsSameAs(L"local") || token.IsSameAs(L"global")) {
			isok = true;
			if (token.IsSameAs(L"local"))
				localvartmp[vari].local_type = SCRIPT_VARIABLE_LOCALTYPE_LOCAL;
			else
				localvartmp[vari].local_type = SCRIPT_VARIABLE_LOCALTYPE_GLOBAL;
			token = GetNextThing(linestr);
			if (token.Mid(0,3).IsSameAs(L"int")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_INT;
				if (token.Mid(3).IsNumber()) {
					localvartmp[vari].size = wxAtoi(token.Mid(3));
					if (localvartmp[vari].size!=8 && localvartmp[vari].size!=16 && localvartmp[vari].size!=24) {
						errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
						log.AddWarning(errstr.ToStdWstring());
						isok = false;
					}
				} else {
					errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
					log.AddWarning(errstr.ToStdWstring());
					isok = false;
				}
			} else if (token.Mid(0,4).IsSameAs(L"uint")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_UINT;
				if (token.Mid(4).IsNumber()) {
					localvartmp[vari].size = wxAtoi(token.Mid(4));
					if (localvartmp[vari].size!=8 && localvartmp[vari].size!=16 && localvartmp[vari].size!=24) {
						errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
						log.AddWarning(errstr.ToStdWstring());
						isok = false;
					}
				} else {
					errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
					log.AddWarning(errstr.ToStdWstring());
					isok = false;
				}
			} else if (token.IsSameAs(L"bool")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_BOOL;
				localvartmp[vari].size = 1;
			} else if (token.IsSameAs(L"sbool")) {
				localvartmp[vari].type = SCRIPT_VARIABLE_TYPE_SBOOL;
				localvartmp[vari].size = 1;
			} else {
				errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
				log.AddWarning(errstr.ToStdWstring());
				isok = false;
			}
			if (isok) {
				token = GetNextThing(linestr);
				if (token==wxEmptyString) {
					log.AddWarning(HADES_STRING_LSCRIPT_MISS_NAME);
					isok = false;
				} else
					localvartmp[vari].name = token.ToStdWstring();
			}
			token = GetNextThing(linestr);
			while (isok && token!=wxEmptyString) {
				if (token.Mid(0,4).IsSameAs(L"VAR_")) {
					isok = false;
					for (i = 0; i < G_V_ELEMENTS(VarOpList); i++)
						if (VarOpList[i].type >= 10 && VarOpList[i].type < 30 && token.Mid(0, VarOpList[i].opstring.length()).IsSameAs(VarOpList[i].opstring)) {
							localvartmp[vari].cat = i;
							localvartmp[vari].id = wxAtoi(token.Mid(VarOpList[i].opstring.length()));
							isok = true;
							break;
						}
					if (!isok) {
						errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED), token);
						log.AddWarning(errstr.ToStdWstring());
					}
				} else {
					errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
					log.AddWarning(errstr.ToStdWstring());
					isok = false;
				}
				token = GetNextThing(linestr);
			}
			if (isok)
				vari++;
			MACRO_REINIT_LOCAL()
		} else {
			errstr.Printf(wxT(HADES_STRING_LSCRIPT_UNEXPECTED),token);
			log.AddWarning(errstr.ToStdWstring());
		}
	}
	bool localvarused[0x100];
	bool globalvarused[0x100];
	for (i = 0; i < 0x100; i++) {
		localvarused[i] = false;
		globalvarused[i] = false;
	}
	for (i = 0; i < vari; i++)
		if (localvartmp[i].cat > 0) {
			if (localvartmp[i].size == 1) {
				if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
					localvarused[localvartmp[i].id >> 3] = true;
				else if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
					globalvarused[localvartmp[i].id >> 3] = true;
			} else {
				for (j = 0; (int)j < (localvartmp[i].size >> 3); j++) {
					if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
						localvarused[localvartmp[i].id + j] = true;
					else if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
						globalvarused[localvartmp[i].id + j] = true;
				}
			}
		}
	res->amount = vari;
	res->local_type.resize(vari);
	res->type.resize(vari);
	res->size.resize(vari);
	res->name.resize(vari);
	res->cat.resize(vari);
	res->id.resize(vari);
	for (i = 0; i < res->amount; i++) {
		res->local_type[i] = localvartmp[i].local_type;
		res->type[i] = localvartmp[i].type;
		res->size[i] = localvartmp[i].size;
		res->name[i] = localvartmp[i].name;
		if (localvartmp[i].cat > 0) {
			res->cat[i] = localvartmp[i].cat;
			res->id[i] = localvartmp[i].id;
		} else {
			res->cat[i] = GetLocalCategoryFromType(res->local_type[i], res->type[i], res->size[i]);
			for (j = 0; j < 0x100; j++) {
				isok = true;
				if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_LOCAL) {
					for (k = 0; (int)k < max(1, res->size[i] >> 3); k++)
						if (j + k >= 0x100 || localvarused[j + k]) {
							isok = false;
							j += k;
							break;
						}
				} else if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_GLOBAL) {
					for (k = 0; (int)k < max(1, res->size[i] >> 3); k++)
						if (j + k >= 0x100 || globalvarused[j + k]) {
							isok = false;
							j += k;
							break;
						}
				}
				if (isok) {
					if (res->size[i] == 1)
						res->id[i] = j << 3;
					else
						res->id[i] = j;
					for (k = 0; (int)k < max(1, res->size[i] >> 3); k++) {
						if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
							localvarused[j + k] = true;
						else if (localvartmp[i].local_type == SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
							globalvarused[j + k] = true;
					}
					break;
				}
			}
		}
	}
	for (int l = 0xFF; l >= res->allocate_amount; l--)
		if (localvarused[l]) {
			errstr.Printf(wxT(HADES_STRING_LSCRIPT_MISS_ALLOC), l + 1);
			log.AddWarning(errstr.ToStdWstring());
			break;
		}
	return res;
}

static wxString keywords[] = {
	L"if",		L"else",		L"ifnot",
	L"while",	L"do",			L"loop",
	L"switch",	L"switchex",	L"case",
	L"default",	L"break",		L"Function",
	L"forward"
};
ScriptOperation parseop[0x8000];
unsigned int parseopamount = 0;
unsigned int parseoplength = 0;
LogStruct ScriptEditHandler::ParseFunction(wxString str, unsigned int entry, unsigned int function, unsigned int startinglinenumber) {
	entry_selection = entry;
	function_selection = function;
	size_t pos;
	unsigned int rawpos = 0, lvlamount = 0, lvlrawpos[SCRPT_MAX_OPERAND], lvloppos[SCRPT_MAX_OPERAND], lvltype[SCRPT_MAX_OPERAND], lvlcaseamount[SCRPT_MAX_OPERAND];
	unsigned int breaklvlamount = 0, breaklvli[SCRPT_MAX_OPERAND], breaklvlrawpos[SCRPT_MAX_OPERAND][SCRPT_MAX_OPERAND], breaklvloppos[SCRPT_MAX_OPERAND][SCRPT_MAX_OPERAND];
	unsigned int i, j, opi = 0, line = startinglinenumber;
	wxString linestr, token, errstr;
	wxString argstr[SCRPT_MAX_OPERAND];
	LogStruct res = LogStruct();
	vector<uint8_t> lvlargval[SCRPT_MAX_OPERAND];
	vector<uint8_t> argval, argvalarray[SCRPT_MAX_OPERAND];
	uint8_t varargbyte;
	int tokentype;
	bool islastarg;
	bool isfunctionreturned = false;
	
	unsigned int macroi;
	wxString macrostr;
	#define MACRO_NEW_OPCODE(OPCODE) \
		parseop[opi].parent = &script.func[entry_selection][function_selection]; \
		parseop[opi].opcode = OPCODE; \
		parseop[opi].arg_amount = HADES_STRING_SCRIPT_OPCODE[OPCODE].arg_amount; \
		if (parseop[opi].arg_amount>=0) { \
			parseop[opi].arg = NewScriptArgumentArray(parseop[opi].arg_amount,&parseop[opi]); \
			for (macroi=0;macroi<parseop[opi].arg_amount;macroi++) { \
				parseop[opi].arg[macroi].is_signed = IsScriptArgTypeSigned(HADES_STRING_SCRIPT_OPCODE[OPCODE].arg_type[macroi]); \
				parseop[opi].arg[macroi].typesize = HADES_STRING_SCRIPT_OPCODE[OPCODE].arg_length[macroi]; \
			} \
		}
	#define MACRO_OPCODE_SIZE_DONE() \
		parseop[opi].size = 1 + (parseop[opi].opcode >> 8); \
		if (HADES_STRING_SCRIPT_OPCODE[parseop[opi].opcode].use_vararg) \
			parseop[opi].size += 1; \
		if (parseop[opi].opcode==0x06 || parseop[opi].opcode==0x0B || parseop[opi].opcode==0x29) \
			parseop[opi].size += 1; \
		for (macroi=0;macroi<parseop[opi].arg_amount;macroi++) \
			parseop[opi].size += parseop[opi].arg[macroi].size; \
		rawpos += parseop[opi++].size;
	#define MACRO_RISE_LEVEL(RAW,OP) \
		lvlrawpos[lvlamount] = RAW; \
		lvloppos[lvlamount] = OP; \
		lvltype[lvlamount] = tokentype; \
		lvlcaseamount[lvlamount++] = 0;
	#define MACRO_CHECK_PUNC_ERROR(PUNC) \
		if (!token.IsSameAs(PUNC)) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,PUNC); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_NUMBER_ERROR() \
		if (!token.IsNumber()) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_NUMBER),line); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_EMPTY_ERROR() \
		if (token.IsEmpty()) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_EMPTY),line); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_VARARG_ERROR(DEST) \
		DEST = ConvertStringToVararg(token,macrostr,localvar); \
		if (DEST.size()==0) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_VARARG_MAIN),line,macrostr); \
			res.AddError(errstr.ToStdWstring()); \
			break; \
		}
	#define MACRO_CHECK_TRAILING_WARNING() \
		linestr.Trim(false); \
		if (linestr.Length()>0 && !(linestr.Length()>=2 && linestr[0]==L'/' && linestr[1]==L'/')) { \
			errstr.Printf(wxT(HADES_STRING_SCRIPT_IGNORE),line,linestr); \
			res.AddWarning(errstr.ToStdWstring()); \
		}
	
	ScriptLocalVariableSet* localvar = ParseLocal(res, currentvar_str);
	while (!str.IsEmpty()) {
		pos = str.find_first_of(L'\n');
		linestr = str.Mid(0, pos);
		if (pos!=string::npos)
			str = str.Mid(pos+1);
		else
			str = wxEmptyString;
		line++;
		token = GetNextThing(linestr);
		tokentype = -1;
		if (token==wxEmptyString || (token.Length()>=2 && token[0]==L'/' && token[1]==L'/'))
			continue;
		if (isfunctionreturned) {
			errstr.Printf(wxT(HADES_STRING_SCRIPT_IGNORE_POSTRET),line);
			res.AddWarning(errstr.ToStdWstring());
			if (opi == 0)
				continue;
		}
		for (i = 0; i < G_N_ELEMENTS(keywords); i++) {
			if (token.IsSameAs(keywords[i])) {
				tokentype = i + 1;
				switch (i) {
				case 0: // if
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x02)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos, opi - 2)
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 1: // else
					errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT), line, L"}");
					res.AddError(errstr.ToStdWstring());
					break;
				case 2: // ifnot
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x03)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos, opi - 2)
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 3: // while
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(lvlargval[lvlamount])
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x01)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos, opi - 1)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 4: // do
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_RISE_LEVEL(rawpos, opi)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 5: // loop
					if (lvlamount > 0) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_LOOP), line);
						res.AddError(errstr.ToStdWstring());
						break;
					}
					MACRO_NEW_OPCODE(0x01)
					parseop[opi].arg[0].SetValue(-((long long)rawpos + 3));
					MACRO_OPCODE_SIZE_DONE()
					MACRO_CHECK_TRAILING_WARNING()
					isfunctionreturned = true;
					break;
				case 6: // switch
					token = GetNextThing(linestr);
					MACRO_CHECK_NUMBER_ERROR()
					argstr[0] = token;
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextThing(linestr);
					MACRO_CHECK_PUNC_ERROR(L"from")
					token = GetNextThing(linestr);
					MACRO_CHECK_NUMBER_ERROR()
					argstr[1] = token;
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x0B)
					parseop[opi].size_byte = wxAtoi(argstr[0]);
					parseop[opi].arg_amount = 2 + parseop[opi].size_byte;
					parseop[opi].arg = NewScriptArgumentArray(parseop[opi].arg_amount, &parseop[opi]);
					parseop[opi].arg[0].is_signed = false;
					parseop[opi].arg[0].typesize = 2;
					parseop[opi].arg[0].SetValue(wxAtoi(argstr[1]));
					for (j = 1; j < parseop[opi].arg_amount; j++) {
						parseop[opi].arg[j].is_signed = true;
						parseop[opi].arg[j].typesize = 2;
						parseop[opi].arg[j].SetValue(-1);
					}
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos + 1 - parseop[opi - 1].size, opi - 2)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 7: // switchex
					token = GetNextThing(linestr);
					MACRO_CHECK_NUMBER_ERROR()
					argstr[0] = token;
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"(")
					token = GetNextArg(linestr);
					MACRO_CHECK_VARARG_ERROR(argval)
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L")")
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_NEW_OPCODE(0x05)
					parseop[opi].arg[0].SetValueVar(argval);
					MACRO_OPCODE_SIZE_DONE()
					MACRO_NEW_OPCODE(0x06)
					parseop[opi].size_byte = wxAtoi(argstr[0]);
					parseop[opi].arg_amount = 1 + parseop[opi].size_byte * 2;
					parseop[opi].arg = NewScriptArgumentArray(parseop[opi].arg_amount, &parseop[opi]);
					parseop[opi].arg[0].is_signed = true;
					parseop[opi].arg[0].typesize = 2;
					parseop[opi].arg[0].SetValue(-1);
					for (j = 1; j < parseop[opi].arg_amount; j++) {
						parseop[opi].arg[j].is_signed = j % 2 == 0;
						parseop[opi].arg[j].typesize = 2;
						parseop[opi].arg[j].SetValue(0);
					}
					MACRO_OPCODE_SIZE_DONE()
					MACRO_RISE_LEVEL(rawpos + 4 - parseop[opi - 1].size, opi - 2)
					MACRO_CHECK_TRAILING_WARNING()
					breaklvli[breaklvlamount++] = 0;
					break;
				case 8: { // case
					if (lvltype[lvlamount - 1] != 7 && lvltype[lvlamount - 1] != 8) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_NOTSWITCH), line, L"default");
						res.AddError(errstr.ToStdWstring());
						break;
					}
					bool endenum = false;
					token = GetNextThing(linestr);
					while (!endenum) {
						if (lvltype[lvlamount - 1] == 7) {
							if (token[0] != L'+') {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT), line, L"+");
								res.AddError(errstr.ToStdWstring());
								break;
							}
							token = token.Mid(1);
						}
						MACRO_CHECK_NUMBER_ERROR()
						if (lvlcaseamount[lvlamount - 1] >= parseop[lvloppos[lvlamount - 1] + 1].size_byte) {
							errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE), line);
							res.AddError(errstr.ToStdWstring());
							break;
						}
						if (lvltype[lvlamount - 1] == 7) {
							if (wxAtoi(token) >= parseop[lvloppos[lvlamount - 1] + 1].size_byte || wxAtoi(token) < 0) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE_RANGE), line);
								res.AddError(errstr.ToStdWstring());
								break;
							}
							if (parseop[lvloppos[lvlamount - 1] + 1].arg[2 + wxAtoi(token)].GetValue() != -1) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE_TWICE), line, token);
								res.AddError(errstr.ToStdWstring());
								break;
							}
							parseop[lvloppos[lvlamount - 1] + 1].arg[2 + wxAtoi(token)].SetValue(rawpos - lvlrawpos[lvlamount - 1]);
						} else {
							parseop[lvloppos[lvlamount - 1] + 1].arg[1 + lvlcaseamount[lvlamount - 1] * 2].SetValue(wxAtoi(token));
							parseop[lvloppos[lvlamount - 1] + 1].arg[2 + lvlcaseamount[lvlamount - 1] * 2].SetValue(rawpos - lvlrawpos[lvlamount - 1]);
						}
						lvlcaseamount[lvlamount - 1]++;
						token = GetNextThing(linestr);
						if (token.IsSameAs(L";")) {
							token = GetNextThing(linestr);
						} else if (token.IsSameAs(L":")) {
							endenum = true;
						} else {
							errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT), line, L";' or ':");
							res.AddError(errstr.ToStdWstring());
							break;
						}
					}
					if (!endenum)
						break;
					MACRO_CHECK_TRAILING_WARNING()
					break;
				}
				case 9: // default
					if (lvltype[lvlamount - 1] != 7 && lvltype[lvlamount - 1] != 8) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_NOTSWITCH), line, L"default");
						res.AddError(errstr.ToStdWstring());
						break;
					}
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L":")
					if (lvltype[lvlamount - 1] == 7)
						parseop[lvloppos[lvlamount - 1] + 1].arg[1].SetValue(rawpos - lvlrawpos[lvlamount - 1]);
					else
						parseop[lvloppos[lvlamount - 1] + 1].arg[0].SetValue(rawpos - lvlrawpos[lvlamount - 1]);
					for (i = 0; i < parseop[lvloppos[lvlamount - 1] + 1].size_byte; i++)
						if (parseop[lvloppos[lvlamount - 1] + 1].arg[2 + i].GetValue() == -1)
							parseop[lvloppos[lvlamount - 1] + 1].arg[2 + i].SetValue(rawpos - lvlrawpos[lvlamount - 1]);
					MACRO_CHECK_TRAILING_WARNING()
					break;
				case 10: // break
					if (breaklvlamount > 0) {
						MACRO_NEW_OPCODE(0x01)
						parseop[opi].arg[0].SetValue(0);
						breaklvloppos[breaklvlamount - 1][breaklvli[breaklvlamount - 1]] = opi;
						MACRO_OPCODE_SIZE_DONE()
						breaklvlrawpos[breaklvlamount - 1][breaklvli[breaklvlamount - 1]] = rawpos;
						breaklvli[breaklvlamount - 1]++;
						MACRO_CHECK_TRAILING_WARNING()
					} else {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_NOBREAK), line, L"break");
						res.AddError(errstr.ToStdWstring());
					}
					break;
				case 11: // Function
					functionlist_str[GetFunctionAbsolutePos(entry_selection, function_selection)] = _(L"Function") + linestr;
					break;
				case 12: // forward
					if (opi > 0) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_FORWARD), line, L"forward");
						res.AddError(errstr.ToStdWstring());
					} else if (function_selection + 1 >= script.entry_function_amount[entry_selection]) {
						errstr.Printf(wxT(HADES_STRING_SCRIPT_FORWARD_END), line);
						res.AddError(errstr.ToStdWstring());
					} else {
						isfunctionreturned = true;
					}
				}
				break;
			}
		}
		if (tokentype<0 && token.IsSameAs(L"}")) {
			tokentype = 0x80;
			if (lvlamount==0) {
				errstr.Printf(wxT(HADES_STRING_SCRIPT_BLOCKTOOMANY),line);
				res.AddError(errstr.ToStdWstring());
				break;
			}
			lvlamount--;
			switch (lvltype[lvlamount]) {
			case 1: // if
				token = GetNextThing(linestr);
				if (token.IsSameAs(L"else")) {
					token = GetNextPunc(linestr);
					MACRO_CHECK_PUNC_ERROR(L"{")
					MACRO_CHECK_TRAILING_WARNING()
					MACRO_NEW_OPCODE(0x01)
					parseop[opi].arg[0].SetValue(0);
					MACRO_OPCODE_SIZE_DONE()
					parseop[lvloppos[lvlamount] + 1].arg[0].SetValue(rawpos - lvlrawpos[lvlamount]);
					tokentype = 2;
					MACRO_RISE_LEVEL(rawpos, opi - 1)
				} else {
					parseop[lvloppos[lvlamount] + 1].arg[0].SetValue(rawpos - lvlrawpos[lvlamount]);
				}
				break;
			case 2: // else
				parseop[lvloppos[lvlamount]].arg[0].SetValue(rawpos - lvlrawpos[lvlamount]);
				break;
			case 3: // ifnot
				parseop[lvloppos[lvlamount] + 1].arg[0].SetValue(rawpos - lvlrawpos[lvlamount]);
				break;
			case 4: // while
				parseop[lvloppos[lvlamount]].arg[0].SetValue(rawpos - lvlrawpos[lvlamount]);
				MACRO_NEW_OPCODE(0x05)
				parseop[opi].arg[0].SetValueVar(lvlargval[lvlamount]);
				MACRO_OPCODE_SIZE_DONE()
				MACRO_NEW_OPCODE(0x03)
				parseop[opi].arg[0].SetValue(lvlrawpos[lvlamount] - (rawpos + 3));
				MACRO_OPCODE_SIZE_DONE()
				breaklvlamount--;
				for (i = 0; i < breaklvli[breaklvlamount]; i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos - breaklvlrawpos[breaklvlamount][i]);
				break;
			case 5: // do
				token = GetNextThing(linestr);
				MACRO_CHECK_PUNC_ERROR(L"while")
				token = GetNextPunc(linestr);
				MACRO_CHECK_PUNC_ERROR(L"(")
				token = GetNextArg(linestr);
				MACRO_CHECK_VARARG_ERROR(argval)
				token = GetNextPunc(linestr);
				MACRO_CHECK_PUNC_ERROR(L")")
				MACRO_NEW_OPCODE(0x05)
				parseop[opi].arg[0].SetValueVar(argval);
				MACRO_OPCODE_SIZE_DONE()
				MACRO_NEW_OPCODE(0x03)
				parseop[opi].arg[0].SetValue(lvlrawpos[lvlamount] - (rawpos + 3));
				MACRO_OPCODE_SIZE_DONE()
				breaklvlamount--;
				for (i = 0; i < breaklvli[breaklvlamount]; i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos - breaklvlrawpos[breaklvlamount][i]);
				break;
			case 7: // switch
				for (i = 0; i < parseop[lvloppos[lvlamount] + 1].size_byte; i++)
					if (parseop[lvloppos[lvlamount] + 1].arg[2 + i].GetValue() == -1)
						parseop[lvloppos[lvlamount] + 1].arg[2 + i].SetValue(rawpos - lvlrawpos[lvlamount]);
				if (parseop[lvloppos[lvlamount] + 1].arg[1].GetValue() == -1)
					parseop[lvloppos[lvlamount] + 1].arg[1].SetValue(rawpos - lvlrawpos[lvlamount]);
				breaklvlamount--;
				for (i = 0; i < breaklvli[breaklvlamount]; i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos - breaklvlrawpos[breaklvlamount][i]);
				break;
			case 8: // switchex
				if (lvlcaseamount[lvlamount] < parseop[lvloppos[lvlamount] + 1].size_byte) {
					errstr.Printf(wxT(HADES_STRING_SCRIPT_CASE), line);
					res.AddError(errstr.ToStdWstring());
					break;
				}
				if (parseop[lvloppos[lvlamount] + 1].arg[0].GetValue() == -1)
					parseop[lvloppos[lvlamount] + 1].arg[0].SetValue(rawpos - lvlrawpos[lvlamount]);
				breaklvlamount--;
				for (i = 0; i < breaklvli[breaklvlamount]; i++)
					parseop[breaklvloppos[breaklvlamount][i]].arg[0].SetValue(rawpos - breaklvlrawpos[breaklvlamount][i]);
				break;
			}
			MACRO_CHECK_TRAILING_WARNING()
		}
		if (tokentype < 0) {
			for (auto it = HADES_STRING_SCRIPT_OPCODE.begin(); it != HADES_STRING_SCRIPT_OPCODE.end(); it++) {
				if (token.IsSameAs(it->second.label)) {
					tokentype = 0x100 + it->second.id;
					switch (it->second.id) {
					case 0x04:
						MACRO_NEW_OPCODE(0x04)
						MACRO_OPCODE_SIZE_DONE()
						if (lvlamount==0)
							isfunctionreturned = true;
						break;
					case 0x05:
						token = linestr;
						linestr = wxEmptyString;
						token.Trim(true);
						token.Trim(false);
						MACRO_CHECK_VARARG_ERROR(argval)
						MACRO_NEW_OPCODE(0x05)
						parseop[opi].arg[0].SetValueVar(argval);
						MACRO_OPCODE_SIZE_DONE()
						break;
					case 0x29:
						token = GetNextPunc(linestr);
						MACRO_CHECK_PUNC_ERROR(L"(")
						islastarg = false;
						varargbyte = 0;
						j = 0;
						while (!islastarg) {
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L"(")
							token = GetNextThing(linestr);
							MACRO_CHECK_NUMBER_ERROR()
							argstr[j++] = token;
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L",")
							token = GetNextThing(linestr);
							MACRO_CHECK_NUMBER_ERROR()
							argstr[j++] = token;
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L")")
							// TODO: can't use vararg with FIELD_REGION opcode for now
/*							token = GetNextArg(linestr);
							if (!token.IsNumber()) {
								MACRO_CHECK_VARARG_ERROR(argvalarray[j])
								varargbyte |= 0x1 << j;
							}
							argstr[j++] = token;*/
							token = GetNextPunc(linestr);
							if (token.IsSameAs(L")"))
								islastarg = true;
							else if (!token.IsSameAs(L",")) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L",' or ')");
								res.AddError(errstr.ToStdWstring());
								break;
							}
						}
						if (!islastarg)
							break;
						MACRO_NEW_OPCODE(0x29)
						parseop[opi].vararg_flag = varargbyte;
						parseop[opi].size_byte = j/2;
						parseop[opi].arg_amount = j/2;
						parseop[opi].arg = NewScriptArgumentArray(j/2,&parseop[opi]);
						for (j=0;j<parseop[opi].arg_amount;j++) {
							parseop[opi].arg[j].is_signed = true;
							parseop[opi].arg[j].typesize = 4;
							if (argstr[2*j].IsNumber())
								parseop[opi].arg[j].SetValue((wxAtoi(argstr[2*j]) & 0xFFFF) | ((wxAtoi(argstr[2*j+1]) & 0xFFFF) << 16));
							else
								parseop[opi].arg[j].SetValueVar(argvalarray[2*j]);
						}
						MACRO_OPCODE_SIZE_DONE()
						break;
					default:
						token = GetNextPunc(linestr);
						MACRO_CHECK_PUNC_ERROR(L"(")
						if (it->second.arg_amount>0) {
							islastarg = false;
							varargbyte = 0;
							j = 0;
							while (!islastarg) {
								token = GetNextArg(linestr);
								MACRO_CHECK_EMPTY_ERROR()
								if (!token.IsNumber()) {
									MACRO_CHECK_VARARG_ERROR(argvalarray[j])
									varargbyte |= 0x1 << j;
								}
								argstr[j++] = token;
								token = GetNextPunc(linestr);
								if (token.IsSameAs(L")"))
									islastarg = true;
								else if (!token.IsSameAs(L",")) {
									errstr.Printf(wxT(HADES_STRING_SCRIPT_EXPECT),line,L",' or ')");
									res.AddError(errstr.ToStdWstring());
									break;
								}
							}
							if (!islastarg)
								break;
							islastarg = false; // becomes a check for errors
							if (j != it->second.arg_amount) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_ARGAMOUNT), line, _(it->second.label), it->second.arg_amount, j);
								res.AddError(errstr.ToStdWstring());
								islastarg = true;
							}
							if (!it->second.use_vararg && varargbyte > 0) {
								errstr.Printf(wxT(HADES_STRING_SCRIPT_NOVARARG), line, _(it->second.label));
								res.AddError(errstr.ToStdWstring());
								islastarg = true;
							}
							if (islastarg)
								break;
						} else {
							token = GetNextPunc(linestr);
							MACRO_CHECK_PUNC_ERROR(L")")
						}
						if ((it->second.id & 0xFF) == 0xFF) {
							errstr.Printf(wxT(HADES_STRING_SCRIPT_NOT_AN_OPCODE), line, it->second.label, it->second.id);
							res.AddError(errstr.ToStdWstring());
							break;
						}
						MACRO_NEW_OPCODE(it->second.id)
						if (it->second.use_vararg)
							parseop[opi].vararg_flag = varargbyte;
						for (j=0;j<parseop[opi].arg_amount;j++) {
							if (argstr[j].IsNumber())
								parseop[opi].arg[j].SetValue(wxAtoi(argstr[j]));
							else
								parseop[opi].arg[j].SetValueVar(argvalarray[j]);
						}
						MACRO_OPCODE_SIZE_DONE()
						break;
					}
					MACRO_CHECK_TRAILING_WARNING()
				}
			}
		}
		if (tokentype<0) {
			errstr.Printf(wxT(HADES_STRING_SCRIPT_UNKNOWN),line,token);
			res.AddError(errstr.ToStdWstring());
		}
	}
	if (lvlamount>0) {
		errstr.Printf(wxT(HADES_STRING_SCRIPT_BLOCKMISSING),line,lvlamount);
		res.AddError(errstr.ToStdWstring());
	}
	if (!isfunctionreturned) {
		errstr.Printf(wxT(HADES_STRING_SCRIPT_RETURNMISSING),line);
		res.AddError(errstr.ToStdWstring());
	}
	parseopamount = opi;
	parseoplength = rawpos;
	if (res.ok) {
		unsigned int localam = 0, globalam = 0;
		for (i=0;i<localvar->amount;i++)
			if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_LOCAL)
				localam++;
			else if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL)
				globalam++;
		script.entry_local_var[entry] = localvar->allocate_amount;
		script.local_data[entry].allocate_amount = localvar->allocate_amount;
		script.local_data[entry].amount = localam;
		script.local_data[entry].local_type.resize(localam);
		script.local_data[entry].type.resize(localam);
		script.local_data[entry].size.resize(localam);
		script.local_data[entry].name.resize(localam);
		script.local_data[entry].cat.resize(localam);
		script.local_data[entry].id.resize(localam);
		script.global_data.allocate_amount = 0;
		script.global_data.amount = globalam;
		script.global_data.local_type.resize(globalam);
		script.global_data.type.resize(globalam);
		script.global_data.size.resize(globalam);
		script.global_data.name.resize(globalam);
		script.global_data.cat.resize(globalam);
		script.global_data.id.resize(globalam);
		localam = 0;
		globalam = 0;
		for (i=0;i<localvar->amount;i++)
			if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_LOCAL) {
				script.local_data[entry].local_type[localam] = localvar->local_type[i];
				script.local_data[entry].type[localam] = localvar->type[i];
				script.local_data[entry].size[localam] = localvar->size[i];
				script.local_data[entry].name[localam] = localvar->name[i];
				script.local_data[entry].cat[localam] = localvar->cat[i];
				script.local_data[entry].id[localam] = localvar->id[i];
				localam++;
			} else if (localvar->local_type[i]==SCRIPT_VARIABLE_LOCALTYPE_GLOBAL) {
				script.global_data.local_type[globalam] = localvar->local_type[i];
				script.global_data.type[globalam] = localvar->type[i];
				script.global_data.size[globalam] = localvar->size[i];
				script.global_data.name[globalam] = localvar->name[i];
				script.global_data.cat[globalam] = localvar->cat[i];
				script.global_data.id[globalam] = localvar->id[i];
				globalam++;
			}
		delete localvar;
		UpdateGlobalLocalStrings(entry);
	}
	return res;
}

unsigned int ScriptEditHandler::GetParsedEntryNewSize() {
	unsigned int entrynewsize = script.entry_size[entry_selection]-script.func[entry_selection][function_selection].length+parseoplength;
	if (entrynewsize%4)
		entrynewsize += 4-entrynewsize%4;
	return entrynewsize;
}

void ScriptEditHandler::ApplyParsedFunction() {
	script.entry_size[entry_selection] = GetParsedEntryNewSize();
	script.func[entry_selection][function_selection].length = parseoplength;
	script.func[entry_selection][function_selection].op_amount = parseopamount;
	script.func[entry_selection][function_selection].op.resize(parseopamount);
	for (unsigned int i=0;i<parseopamount;i++)
		script.func[entry_selection][function_selection].op[i] = parseop[i];
}

//=============================//
//          Code List          //
//=============================//

class ScriptHelpDialog : public ScriptHelpWindow {
public:
	wxArrayString func_list;
	wxArrayString var_list;
	wxArrayString varcode_list;
	void** func_list_item;
	void** var_list_item;
	void** varcode_list_item;
	
	ScriptHelpDialog(ScriptEditDialog* p);
	~ScriptHelpDialog();
	void DisplayHelpFunction(SortedChoiceItemScriptOpcode* item);
	void DisplayHelpVariable(VariableName* item);
	void DisplayHelpVarCode(VariableOperation* item);
	void SearchUpdate();

private:
	ScriptEditDialog* parent;
	VariableOperation* flexible_var_op;
	
	void OnSearch(wxCommandEvent& event);
	void OnSearchButton(wxMouseEvent& event);
	void OnListClick(wxCommandEvent& event);
	void OnListDoubleClick(wxCommandEvent& event);
};

ScriptHelpDialog::ScriptHelpDialog(ScriptEditDialog* p) :
	ScriptHelpWindow(p),
	parent(p) {
	unsigned int i, j;
	j = 0;
	func_list.Alloc(HADES_STRING_SCRIPT_OPCODE.size());
	func_list_item = (void**)new SortedChoiceItemScriptOpcode*[HADES_STRING_SCRIPT_OPCODE.size()];
	for (auto it = HADES_STRING_SCRIPT_OPCODE.begin(); it != HADES_STRING_SCRIPT_OPCODE.end(); it++)
		if (it->first >= 0x07 && it->first != 0x0B && it->first != 0xFF) {
			func_list.Add(it->second.label);
			func_list_item[j] = (void*)&it->second;
			m_listfunction->Append(func_list[j], func_list_item[j]);
			j++;
		}
	var_list.Alloc(VarNameList.size());
	var_list_item = (void**)new VariableName*[VarNameList.size()];
	for (i = 0; i < VarNameList.size(); i++) {
		var_list.Add(VarNameList[i].name);
		var_list_item[i] = (void*)&VarNameList[i];
		m_listvariable->Append(var_list[i], var_list_item[i]);
	}
	j = 0;
	varcode_list.Alloc(VarOpList.size() + VarEntryPropList.size() + FlexibleFunctionNameList.size());
	varcode_list_item = (void**)new VariableOperation*[VarOpList.size() + VarEntryPropList.size() + FlexibleFunctionNameList.size()];
	flexible_var_op = new VariableOperation[FlexibleFunctionNameList.size()];
	varcode_list.Add(_(L"[DATA_ACCESS]"));
	varcode_list_item[j] = (void*)&VarOpList[0x29];
	m_listvarcode->Append(varcode_list[j], varcode_list_item[j]);
	j++;
	for (i = 0; i < VarOpList.size(); i++)
		if (VarOpList[i].type == 0 || VarOpList[i].type == 2 || VarOpList[i].type == 50 || VarOpList[i].type == 51 || VarOpList[i].type == 55) {
			varcode_list.Add(_(VarOpList[i].opstring));
			varcode_list_item[j] = (void*)&VarOpList[i];
			m_listvarcode->Append(varcode_list[j], varcode_list_item[j]);
			j++;
		}
	for (i = 0; i < VarEntryPropList.size(); i++) {
		varcode_list.Add(_(VarEntryPropList[i].opstring));
		varcode_list_item[j] = (void*)&VarEntryPropList[i];
		m_listvarcode->Append(varcode_list[j], varcode_list_item[j]);
		j++;
	}
	i = 0;
	for (auto it = FlexibleFunctionNameList.begin(); it != FlexibleFunctionNameList.end(); it++) {
		varcode_list.Add(_(it->second.name));
		flexible_var_op[i].type = 2000 + it->second.id;
		flexible_var_op[i].opstring = it->second.name;
		flexible_var_op[i].description = it->second.description;
		varcode_list_item[j] = (void*)&flexible_var_op[i];
		m_listvarcode->Append(varcode_list[j], varcode_list_item[j]);
		i++;
		j++;
	}
	wxImage tmpimg = wxBITMAP(findglass_image).ConvertToImage();
	m_searchbtn->SetBitmap(tmpimg.Rescale(16, 16, wxIMAGE_QUALITY_HIGH));
}

ScriptHelpDialog::~ScriptHelpDialog() {
	parent->help_dial = NULL;
	delete[] func_list_item;
	delete[] var_list_item;
	delete[] varcode_list_item;
	delete[] flexible_var_op;
}

void ScriptHelpDialog::DisplayHelpFunction(SortedChoiceItemScriptOpcode* item) {
	m_helptextctrl->SetValue(item->help);
}

void ScriptHelpDialog::DisplayHelpVariable(VariableName* item) {
	m_helptextctrl->SetValue(item->name+_(L"\n")+_(VarOpList[item->cat].opstring)+wxString::Format(wxT("%u"),item->id)+_(L"\n\n")+_(item->description));
}

void ScriptHelpDialog::DisplayHelpVarCode(VariableOperation* item) {
	if (item->type == 3) {
		wxString desc = _(item->description);
		for (unsigned int i = 0; i < G_V_ELEMENTS(ScriptCharacterField); i++)
			desc += _(ScriptCharacterField[i].name) + _(L"\n");
		desc += _(ARRAY_ADDITIONAL_INFO);
		m_helptextctrl->SetValue(desc);
	} else if (item->type < 1000) {
		m_helptextctrl->SetValue(item->description);
	} else if (item->type < 2000) {
		m_helptextctrl->SetValue(wxString::Format(wxT("%s(%d, Entry)\n\n%s"), VarOpList[0x78].opstring, item->type - 1000, item->description));
	} else {
		FlexibleFunctionName& flexfunc = FlexibleFunctionNameList[item->type - 2000];
		m_helptextctrl->SetValue(wxString::Format(wxT("%s (%d argument%s)\n\n%s"), flexfunc.name, flexfunc.argcount, flexfunc.argcount <= 1 ? "" : "s", flexfunc.description));
	}
}

void ScriptHelpDialog::SearchUpdate() {
	wxString srch = m_searchtextctrl->GetValue().Lower();
	unsigned int i;
	m_listfunction->Clear();
	for (i = 0; i < func_list.Count(); i++)
		if (func_list[i].Lower().Find(srch) != wxNOT_FOUND)
			m_listfunction->Append(func_list[i], func_list_item[i]);
	m_listvariable->Clear();
	for (i = 0; i < var_list.Count(); i++)
		if (var_list[i].Lower().Find(srch) != wxNOT_FOUND)
			m_listvariable->Append(var_list[i], var_list_item[i]);
	m_listvarcode->Clear();
	for (i = 0; i < varcode_list.Count(); i++)
		if (varcode_list[i].Lower().Find(srch) != wxNOT_FOUND)
			m_listvarcode->Append(varcode_list[i], varcode_list_item[i]);
}

void ScriptHelpDialog::OnSearch(wxCommandEvent& event) {
	SearchUpdate();
}

void ScriptHelpDialog::OnSearchButton(wxMouseEvent& event) {
	SearchUpdate();
}

void ScriptHelpDialog::OnListClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_FUNCTION)
		DisplayHelpFunction(static_cast<SortedChoiceItemScriptOpcode*>(event.GetClientData()));
	else if (id == wxID_VARIABLE)
		DisplayHelpVariable(static_cast<VariableName*>(event.GetClientData()));
	else if (id == wxID_VARCODE)
		DisplayHelpVarCode(static_cast<VariableOperation*>(event.GetClientData()));
}

void ScriptHelpDialog::OnListDoubleClick(wxCommandEvent& event) {
	int id = event.GetId();
	int i;
	if (id == wxID_FUNCTION) {
		SortedChoiceItemScriptOpcode* item = static_cast<SortedChoiceItemScriptOpcode*>(event.GetClientData());
		parent->m_scripttext->WriteText(_(item->label) + _(L"( "));
		for (i = 0; i + 1 < item->arg_amount; i++)
			parent->m_scripttext->WriteText(_(L"0, "));
		if (item->arg_amount > 0)
			parent->m_scripttext->WriteText(_(L"0 "));
		parent->m_scripttext->WriteText(_(L")"));
	} else if (id == wxID_VARIABLE) {
		VariableName* item = static_cast<VariableName*>(event.GetClientData());
		parent->m_scripttext->WriteText(_(item->name));
	} else if (id == wxID_VARCODE) {
		VariableOperation* item = static_cast<VariableOperation*>(event.GetClientData());
		parent->m_scripttext->WriteText(_(item->opstring));
		if (item->type == 3) {
			parent->m_scripttext->WriteText(_(L"[0]"));
		} else if (item->type == 50) {
			parent->m_scripttext->WriteText(_(L"(0)"));
		} else if (item->type == 51 || item->type == 55) {
			parent->m_scripttext->WriteText(_(L"(0, 0)"));
		} else if (item->type >= 1000 && item->type < 2000) {
			parent->m_scripttext->WriteText(_(L"(0)"));
		} else {
			FlexibleFunctionName& flexfunc = FlexibleFunctionNameList[item->type - 2000];
			parent->m_scripttext->WriteText(_(L"(0"));
			for (i = 1; i < (int)flexfunc.argcount; i++)
				parent->m_scripttext->WriteText(_(L", 0"));
			parent->m_scripttext->WriteText(_(L")"));
		}
	}
}

//=============================//
//  Display Hints & Arguments  //
//=============================//

struct FlagsStruct {
	unsigned int amount;
	wxCheckBox** box;
};
struct LinkedDialogStruct {
	wxChoice* dialog;
	wxButton* link_btn;
};
struct DiscFieldStruct {
	wxChoice* field;
	wxChoice* disc;
};
struct PositionStruct {
	int zoom;
	wxPoint drawpt;
	bool use_z;
	int32_t x;
	int32_t y;
	int32_t z;

	wxPoint GetAsTranslation() {
		return wxPoint(drawpt.x - POSITION_PANEL_SIZE / 2, -drawpt.y + POSITION_PANEL_SIZE / 2) * 2 * zoom / POSITION_PANEL_SIZE;
	}
};

void WorldMapCoordinatesToBmpPos(int32_t& x, int32_t& y) {
	x = (x * (WORLDMAP_PANEL_WIDTH - 16) / 0x60000L) % (WORLDMAP_PANEL_WIDTH - 16);
	y = (y * (WORLDMAP_PANEL_HEIGHT - 16) / 0x50000L) % (WORLDMAP_PANEL_HEIGHT - 16);
	if (x < 0)
		x += (WORLDMAP_PANEL_WIDTH - 16);
	if (y < 0)
		y += (WORLDMAP_PANEL_HEIGHT - 16);
	x += 8;
	y = WORLDMAP_PANEL_HEIGHT - 8 - y;
}

void ScriptEditDialog::WorldMapDraw(wxDC& dc) {
	bool drawx = world_pos_type & 1, drawy = world_pos_type & 2;
	int32_t x = world_pos_x, y = world_pos_y;
	unsigned char* bmpdata = (unsigned char*)malloc(WORLDMAP_PANEL_WIDTH * WORLDMAP_PANEL_HEIGHT * 3 * sizeof(unsigned char));
	for (unsigned int i = 0; i < WORLDMAP_PANEL_WIDTH * WORLDMAP_PANEL_HEIGHT * 3; i++)
		bmpdata[i] = 255;
	wxImage mdcimg(WORLDMAP_PANEL_WIDTH, WORLDMAP_PANEL_HEIGHT, bmpdata);
	wxBitmap mdcbmp(mdcimg);
	wxMemoryDC mdc(mdcbmp);
	WorldMapCoordinatesToBmpPos(x, y);
	mdc.DrawBitmap(wxBITMAP(worldmap_image), 0, 0);
	mdc.SetPen(wxPen(POSITION_POINT_COLOR));
	mdc.SetBrush(wxBrush(POSITION_POINT_COLOR));
	if (drawx && drawy)
		mdc.DrawCircle(x, y, 2);
	else if (drawx)
		mdc.DrawLine(x, 0, x, WORLDMAP_PANEL_HEIGHT);
	else if (drawy)
		mdc.DrawLine(0, y, WORLDMAP_PANEL_WIDTH, y);
	if (world_region_x >= 0 && world_region_y >= 0)
	{
		mdc.SetPen(wxPen(POSITION_REGION_COLOR));
		mdc.SetBrush(wxBrush(POSITION_REGION_COLOR));
		x = world_region_x;
		y = world_region_y;
		mdc.DrawRectangle(8 + x * (WORLDMAP_PANEL_WIDTH - 16) / 48, 8 + y * (WORLDMAP_PANEL_HEIGHT - 16) / 40, (WORLDMAP_PANEL_WIDTH - 16) / 48, (WORLDMAP_PANEL_HEIGHT - 16) / 40);
	}
	dc.Blit(wxPoint(0, 0), mdc.GetSize(), &mdc, wxPoint(0, 0));
}

void ScriptEditDialog::UpdateWorldRegion(unsigned int entry, unsigned int function) {
	if (script_type == SCRIPT_TYPE_WORLD) {
		if (entry == 0 && script.function_type[entry][function] >= 0x8000) {
			world_region_x = (script.function_type[entry][function] & 0xFC) >> 2;
			world_region_y = (script.function_type[entry][function] & 0x3F00) >> 8;
		} else {
			world_region_x = -1;
			world_region_y = -1;
		}
	}
}

void ScriptEditDialog::DisplayFunction(unsigned int entry, unsigned int function) {
	m_scripttext->ChangeValue(func_str[entry][function]);
	m_localvartext->ChangeValue(localvar_str[entry]);
	line_selection = -1;
	text_x_selection = -1;
	DisplayOperation(wxEmptyString);
	entry_selection = entry;
	function_selection = function;
	UpdateWorldRegion(entry, function);
}

void ScriptEditDialog::DisplayOperation(wxString line, bool refreshargcontrol, bool refresharg) {
	wxString tmpstr;
	uint16_t opcode = 0xFFFF;
	unsigned int argi;
	int i;
	unsigned int j;
	bool cleanarg = true;
	if (arg_control_type.size() > 0 && refreshargcontrol) {
		for (i = 0; i < (int)arg_amount; i++) {
			arg_label[i]->Destroy();
			arg_control[i]->Destroy();
		}
		arg_control_type.clear();
		arg_label.clear();
		arg_control.clear();
	}
	if (refreshargcontrol && animlist_id.size() > 0) {
		for (i = 0; i < (int)animlist_id.size(); i++)
			delete animlist_id[i];
		animlist_id.clear();
		animlist_str.Empty();
	}
	tmpstr = GetNextWord(line);
	for (auto it = HADES_STRING_SCRIPT_OPCODE.begin(); it != HADES_STRING_SCRIPT_OPCODE.end(); it++)
		if (tmpstr.IsSameAs(it->second.label)) {
			opcode = it->second.id;
			break;
		}
	if (refreshargcontrol)
		current_opcode = opcode;
	if (current_opcode == 0xFFFF) {
		if (refreshargcontrol) {
			m_helptextctrl->SetValue(_(HADES_STRING_SCRIPT_OPCODE[0].help));
			arg_amount = 0;
			arg_control_type.clear();
			Layout();
		}
		return;
	}
	SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[current_opcode];
	if (refreshargcontrol) {
		m_helptextctrl->SetValue(_(scriptop.help));
		arg_amount = 0;
		for (i = 0; i < scriptop.arg_amount; i++)
			if (scriptop.arg_type[i] != AT_NONE && /*
			 */ scriptop.arg_type[i] != AT_POSITION_Y && scriptop.arg_type[i] != AT_POSITION_Z && /*
			 */ scriptop.arg_type[i] != AT_COLOR_MAGENTA && scriptop.arg_type[i] != AT_COLOR_YELLOW && /*
			 */ scriptop.arg_type[i] != AT_COLOR_GREEN && scriptop.arg_type[i] != AT_COLOR_BLUE)
					arg_amount++;
		arg_control_type.resize(arg_amount);
		arg_label.resize(arg_amount);
		arg_control.resize(arg_amount);
	}
	wxString arg;
	tmpstr = GetNextPunc(line);
	if (!tmpstr.IsSameAs(L"("))
		cleanarg = false;
	argi = 0;
	if (current_opcode == 0x29 && script_type == SCRIPT_TYPE_FIELD) {
		vector<int16_t> regionpts;
		bool islastarg = false;
		while (!islastarg && cleanarg) {
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L"("))
				cleanarg = false;
			tmpstr = GetNextThing(line);
			regionpts.push_back(wxAtoi(tmpstr));
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			tmpstr = GetNextThing(line);
			regionpts.push_back(wxAtoi(tmpstr));
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L")"))
				cleanarg = false;
			tmpstr = GetNextPunc(line);
			if (tmpstr.IsSameAs(L")"))
				islastarg = true;
			else if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
		}
		if (cleanarg)
			gl_window->DisplayFieldRegion(regionpts);
	}
	for (i = 0; i < (int)arg_amount; i++) {
		if (refreshargcontrol && scriptop.arg_type[argi] != AT_NONE) {
			arg_label[i] = new wxStaticText(m_argpanel, wxID_ANY, _(scriptop.arg_help[i]));
			arg_label[i]->Wrap(-1);
			argsizer->Add(arg_label[i], 0, wxALL, 5);
		}
		tmpstr = GetNextArg(line);
		if (cleanarg)
			arg = tmpstr;
		else
			arg = wxEmptyString;
		if (scriptop.arg_type[argi] == AT_NONE) {
			i--;
			tmpstr = GetNextPunc(line);
			if (!(tmpstr.IsSameAs(L",") && i + 1 < (int)arg_amount) && !(tmpstr.IsSameAs(L")") && i + 1 == arg_amount))
				cleanarg = false;
			argi++;
			continue;
		}
		if (refreshargcontrol) {
			switch (scriptop.arg_type[argi]) {
			case AT_JUMP:
				arg_control[i] = ArgCreateText(arg, i);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_SPIN:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], true);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_USPIN:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_BOOL:
				arg_control[i] = ArgCreateFlag(arg, i);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_BOOLLIST:
			case AT_BUTTONLIST:
				arg_control[i] = ArgCreateFlags(arg, i, 8 * scriptop.arg_length[argi], defaultbool_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_TEXT:
				if (use_text)
					arg_control[i] = ArgCreateDialog(arg, i, vector<uint16_t*>(), text_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_BATTLE:
				if (use_battle)
					arg_control[i] = ArgCreateChoice(arg, i, battle_id, battle_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_FIELD:
				if (use_field)
					arg_control[i] = ArgCreateChoice(arg, i, field_id, field_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_DISC_FIELD:
				if (use_field)
					arg_control[i] = ArgCreateDiscFieldChoice(arg, i, field_id, field_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_ATTACK:
				if (use_attack)
					arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), attack_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_ITEM:
				if (use_item)
					arg_control[i] = ArgCreateChoice(arg, i, item_id, item_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_MENUTYPE:
				arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), menutype_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_MENU:
				if (static_cast<wxChoice*>(arg_control[0])->GetSelection() == 1 && use_character)
					arg_control[i] = ArgCreateChoice(arg, i, character_id, character_str);
				else if (static_cast<wxChoice*>(arg_control[0])->GetSelection() == 2)
					arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), shop_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_CHARACTER:
				if (use_character)
					arg_control[i] = ArgCreateFlags(arg, i, PLAYABLE_CHAR_AMOUNT, character_str);
				else
					arg_control[i] = ArgCreateFlags(arg, i, PLAYABLE_CHAR_AMOUNT, defaultbool_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_LCHARACTER:
				if (use_character)
					arg_control[i] = ArgCreateChoice(arg, i, character_id, character_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_ABILITYSET:
				arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), abilityset_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_EQUIPSET:
				arg_control[i] = ArgCreateChoice(arg, i, equipset_id, equipset_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_FMV:
				arg_control[i] = ArgCreateChoice(arg, i, fmv_id, fmv_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_BATTLECODE:
				arg_control[i] = ArgCreateChoice(arg, i, battlecode_id, battlecode_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_MODELCODE:
				arg_control[i] = ArgCreateChoice(arg, i, modelcode_id, modelcode_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_WORLDCODE:
				arg_control[i] = ArgCreateChoice(arg, i, worldcode_id, worldcode_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_SOUNDCODE:
				arg_control[i] = ArgCreateChoice(arg, i, soundcode_id, soundcode_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_SPSCODE:
				arg_control[i] = ArgCreateChoice(arg, i, spscode_id, spscode_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_ENTRY:
				arg_control[i] = ArgCreateChoice(arg, i, entrylist_id, entrylist_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_FUNCTION: {
				arg_control[i] = ArgCreateChoice(arg, i, functionlist_id, functionlist_str);
				if ((argi > 0 && scriptop.arg_type[argi - 1] == AT_ENTRY) && static_cast<wxChoice*>(arg_control[i - 1])->GetSelection() != wxNOT_FOUND) {
					unsigned int entrysel = static_cast<wxChoice*>(arg_control[i - 1])->GetSelection();
					unsigned int k;
					uint16_t funcid = wxAtoi(arg), funclistpos = 0;
					for (j = 0; j < script.entry_amount; j++)
						for (k = 0; k < script.entry_function_amount[j]; k++) {
							if (entrysel == j && script.function_type[j][k] == funcid)
								static_cast<wxChoice*>(arg_control[i])->SetSelection(funclistpos);
							funclistpos++;
						}
				}
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			}
			case AT_MODEL:
				arg_control[i] = ArgCreateChoice(arg, i, modellist_id, modellist_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_ANIMATION:
				if (entry_model_index[entry_selection] >= 0) {
					int32_t animindex = AnimationDatabase::GetIndexFromModelIndex(entry_model_index[entry_selection]);
					int32_t animindex2 = animindex;
					while (AnimationDatabase::GetModelId(animindex) == HADES_STRING_MODEL_NAME[entry_model_index[entry_selection]].id)
						animlist_str.Add(AnimationDatabase::GetDescription(animindex++));
					animlist_id.resize(animlist_str.GetCount());
					for (j = 0; j < animlist_str.GetCount(); j++)
						animlist_id[j] = new uint16_t(AnimationDatabase::GetId(animindex2 + j));
					arg_control[i] = ArgCreateChoice(arg, i, animlist_id, animlist_str);
					for (j = 0; j < animlist_str.GetCount(); j++)
						if (AnimationDatabase::GetId2(animindex2 + j) == wxAtoi(arg)) {
							static_cast<wxChoice*>(arg_control[i])->SetSelection(j);
							break;
						}
				} else {
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				}
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_SOUND:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_AUDIO:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_BUBBLESYMBOL:
				arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), bubblesymbol_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_WORLDMAP:
				arg_control[i] = ArgCreateChoice(arg, i, worldmap_id, worldmap_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_TILE:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_TILEANIM:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_ABILITY:
				if (use_ability)
					arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), ability_str);
				else
					arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_DECK:
				arg_control[i] = ArgCreateChoice(arg, i, vector<uint16_t*>(), deck_str);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
				break;
			case AT_POSITION_X:
			case AT_COLOR_CYAN:
			case AT_COLOR_RED:
				break;
			default:
				arg_control[i] = ArgCreateSpin(arg, i, scriptop.arg_length[argi], false);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
			}
		}
		if (refresharg) {
			if (!refreshargcontrol) {
				switch (arg_control_type[i]) {
				case ARG_CONTROL_TEXT:
					break;
				case ARG_CONTROL_FLAG:
					static_cast<wxCheckBox*>(arg_control[i])->SetValue(wxAtoi(arg) != 0);
					break;
				case ARG_CONTROL_FLAGS: {
					FlagsStruct* flagctrl = static_cast<FlagsStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
					uint32_t flagval = wxAtoi(arg);
					for (unsigned int j = 0; j < flagctrl->amount; j++)
						flagctrl->box[j]->SetValue(flagval & (0x1 << j));
					break;
				}
				case ARG_CONTROL_SPIN:
					static_cast<wxSpinCtrl*>(arg_control[i])->SetValue(wxAtoi(arg));
					break;
				case ARG_CONTROL_CHOICE: {
					wxChoice* choicectrl = static_cast<wxChoice*>(arg_control[i]);
					uint16_t choiceval = wxAtoi(arg);
					if (choicectrl->GetClientData(0)) {
						for (unsigned int j = 0; j < choicectrl->GetCount(); j++)
							if (*(uint16_t*)choicectrl->GetClientData(j) == choiceval) {
								choicectrl->SetSelection(j);
								break;
							}
					} else
						choicectrl->SetSelection(choiceval);
					break;
				}
				case ARG_CONTROL_LINKED_DIAL: {
					LinkedDialogStruct* dialctrl = static_cast<LinkedDialogStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
					wxChoice* choicectrl = dialctrl->dialog;
					uint16_t choiceval = wxAtoi(arg);
					if (choicectrl->GetClientData(0)) {
						for (unsigned int j = 0; j < choicectrl->GetCount(); j++)
							if (*(uint16_t*)choicectrl->GetClientData(j) == choiceval) {
								choicectrl->SetSelection(j);
								break;
							}
					} else
						choicectrl->SetSelection(choiceval);
					break;
				}
				case ARG_CONTROL_DISC: {
					DiscFieldStruct* discctrl = static_cast<DiscFieldStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
					uint16_t choicefieldval = wxAtoi(arg) & 0x3FFF;
					if (discctrl->field->GetClientData(0)) {
						for (unsigned int j = 0; j < discctrl->field->GetCount(); j++)
							if (*(uint16_t*)discctrl->field->GetClientData(j) == choicefieldval) {
								discctrl->field->SetSelection(j);
								break;
							}
					} else
						discctrl->field->SetSelection(choicefieldval);
					discctrl->disc->SetSelection(wxAtoi(arg) >> 14);
					break;
				}
				}
			}
		}
		switch (scriptop.arg_type[argi]) {
		case AT_POSITION_X: {
			wxArrayString arrarg;
			bool usez;
			arrarg.Add(arg);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			argi++;
			tmpstr = GetNextArg(line);
			if (cleanarg)
				arg = tmpstr;
			else
				arg = wxEmptyString;
			arrarg.Add(arg);
			usez = scriptop.arg_type[argi] == AT_POSITION_Z;
			if (usez) {
				tmpstr = GetNextPunc(line);
				if (!tmpstr.IsSameAs(L","))
					cleanarg = false;
				argi++;
				tmpstr = GetNextArg(line);
				if (cleanarg)
					arg = tmpstr;
				else
					arg = wxEmptyString;
				arrarg.Add(arg);
			}
			if (refreshargcontrol) {
				arg_control[i] = ArgCreatePosition(arrarg, i);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
			} else if (refresharg) {
				PositionStruct* posctrl = static_cast<PositionStruct*>(static_cast<wxPanel*>(arg_control[i])->GetClientData());
				posctrl->x = wxAtoi(arrarg[0]);
				if (usez) {
					posctrl->z = wxAtoi(arrarg[1]);
					posctrl->y = wxAtoi(arrarg[2]);
				} else {
					posctrl->y = wxAtoi(arrarg[1]);
				}
			}
			if (script_type == SCRIPT_TYPE_FIELD) {
				if (usez)
					gl_window->DisplayFieldPoint3D(wxAtoi(arrarg[0]), wxAtoi(arrarg[2]), wxAtoi(arrarg[1]));
				else
					gl_window->DisplayFieldPoint2D(wxAtoi(arrarg[0]), wxAtoi(arrarg[1]));
			} else if (script_type == SCRIPT_TYPE_WORLD) {
				world_pos_type = 3;
				world_pos_x = wxAtoi(arrarg[0]);
				if (usez)
					world_pos_y = wxAtoi(arrarg[2]);
				else
					world_pos_y = wxAtoi(arrarg[1]);
				wxClientDC dc(m_worlddisplaypanel);
				WorldMapDraw(dc);
			}
			break;
		}
		case AT_COLOR_CYAN:
		case AT_COLOR_RED: {
			bool rgb = scriptop.arg_type[argi] == AT_COLOR_RED;
			wxArrayString arrarg;
			arrarg.Add(arg);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			argi++;
			tmpstr = GetNextArg(line);
			if (cleanarg)
				arg = tmpstr;
			else
				arg = wxEmptyString;
			arrarg.Add(arg);
			tmpstr = GetNextPunc(line);
			if (!tmpstr.IsSameAs(L","))
				cleanarg = false;
			argi++;
			tmpstr = GetNextArg(line);
			if (cleanarg)
				arg = tmpstr;
			else
				arg = wxEmptyString;
			arrarg.Add(arg);
			if (refreshargcontrol) {
				arg_control[i] = ArgCreateColorPicker(arrarg, i, rgb);
				argsizer->Add(arg_control[i], 0, wxALL, 5);
			} else if (refresharg) {
				if (rgb)
					static_cast<wxColourPickerCtrl*>(arg_control[i])->SetColour(wxColour(wxAtoi(arrarg[0]), wxAtoi(arrarg[1]), wxAtoi(arrarg[2])));
				else
					static_cast<wxColourPickerCtrl*>(arg_control[i])->SetColour(wxColour(255 - wxAtoi(arrarg[0]), 255 - wxAtoi(arrarg[1]), 255 - wxAtoi(arrarg[2])));
			}
			break;
		}
		case AT_WALKTRIANGLE:
			gl_window->DisplayFieldTrianglePath(wxAtoi(arg));
			break;
		case AT_WALKPATH:
			gl_window->DisplayFieldPath(wxAtoi(arg));
			break;
		}
		tmpstr = GetNextPunc(line);
		if (!(tmpstr.IsSameAs(L",") && i + 1 < (int)arg_amount) && !(tmpstr.IsSameAs(L")") && i + 1 == arg_amount))
			cleanarg = false;
		argi++;
	}
	if (refreshargcontrol) {
		Layout();
		Refresh();
	}
}

void ScriptEditDialog::UpdateLineHelp(long x, long y) {
	wxString line = m_scripttext->GetLineText(y);
	wxString tmp = line;
	size_t relpos = tmp.find_first_of(L"-0123456789"), abspos = relpos, numbegin = 0;
	bool selnum = x == abspos;
	unsigned int i;
	while (relpos != string::npos && (long)abspos <= x) {
		numbegin = abspos;
		tmp = tmp.Mid(relpos);
		relpos = tmp.find_first_not_of(L"-0123456789");
		abspos += relpos;
		if (relpos == string::npos || (long)abspos >= x) {
			selnum = true;
			break;
		}
		tmp = tmp.Mid(relpos);
		relpos = tmp.find_first_of(L"-0123456789");
		abspos += relpos;
	}
	tmp = line.Mid(numbegin, relpos);
	uint32_t selectedint = 0;
	if (selnum) tmp.ToULong((unsigned long*)&selectedint);
	m_intvaluesignedint->ChangeValue(wxString::Format(wxT("%hd"), selectedint & 0xFFFF));
	m_intvaluesignedlong->ChangeValue(wxString::Format(wxT("%d"), selectedint));
	m_intvaluebase16->ChangeValue(wxString::Format(wxT("0x%.4x"), selectedint));
	wxString base64str = _(L"[ ");
	for (i = 0; i <= 3; i++) {
		base64str << (unsigned int)((selectedint >> (6 * i)) & 0x3F);
		if (i < 3)
			base64str << _(L" ; ");
	}
	base64str << _(L" ]");
	m_intvaluebase64->ChangeValue(base64str);
	if (use_item && selectedint >= 0) {
		for (i = 0; i < item_str.GetCount(); i++)
			if (*item_id[i] == selectedint) {
				m_intvalueitem->ChangeValue(item_str[i]);
				break;
			}
		if (i == item_str.GetCount())
			m_intvalueitem->ChangeValue(_(HADES_STRING_VOID));
	} else {
		m_intvalueitem->ChangeValue(_(HADES_STRING_VOID));
	}
	if (use_attack && selectedint >= 0) {
		wxString attackstr = _(L"[ ");
		for (i = 0; i <= 3; i++) {
			uint8_t atk = (selectedint >> (6 * i)) & 0x3F;
			if (atk < enemy->spell_amount)
				attackstr << attack_str[atk];
			else
				attackstr << _(HADES_STRING_VOID);
			if (i < 3)
				attackstr << _(L" ; ");
		}
		attackstr << _(L" ]");
		m_intvalueattack->ChangeValue(attackstr);
	} else {
		m_intvalueattack->ChangeValue(_(HADES_STRING_VOID));
	}
	if (use_ability && selectedint >= 0 && selectedint < SPELL_AMOUNT)
		m_intvaluespell->ChangeValue(ability_str[selectedint]);
	else
		m_intvaluespell->ChangeValue(_(HADES_STRING_VOID));
	if (use_command && selectedint >= 0 && selectedint < command_str.GetCount())
		m_intvaluecmd->ChangeValue(command_str[selectedint]);
	else
		m_intvaluecmd->ChangeValue(_(HADES_STRING_VOID));
	wxString buttonstr;
	unsigned int btncount = 0;
	for (i = 0; i < PlaystationButton.size(); i++) {
		if ((selectedint >> i) & 1) {
			if (btncount > 0)
				buttonstr << _(L" | ");
			buttonstr << _(PlaystationButton[i]);
			btncount++;
		}
	}
	m_intvaluebutton->ChangeValue(btncount > 0 ? buttonstr : _(HADES_STRING_VOID));
	wxString statusstr;
	unsigned int statuscount = 0;
	for (i = 0; i < 24; i++) {
		if ((selectedint >> i) & 1) {
			if (statuscount > 0)
				statusstr << _(L" | ");
			statusstr << _(HADES_STRING_STATUS[i]);
			statuscount++;
		}
	}
	m_intvaluestatusa->ChangeValue(statuscount > 0 ? statusstr : _(HADES_STRING_STATUS_NONE));
	statusstr = wxEmptyString;
	statuscount = 0;
	for (i = 24; i < HADES_STRING_STATUS.size(); i++) {
		if ((selectedint >> (i - 24)) & 1) {
			if (statuscount > 0)
				statusstr << _(L" | ");
			statusstr << _(HADES_STRING_STATUS[i]);
			statuscount++;
		}
	}
	m_intvaluestatusb->ChangeValue(statuscount > 0 ? statusstr : _(HADES_STRING_STATUS_NONE));
	if (script_type == SCRIPT_TYPE_FIELD && !refresh_control_disable) {
		gl_window->DisplayFieldClear();
		gl_window->DisplayFieldPlane(m_fieldcoordchoice->GetSelection(), selectedint);
	} else if (script_type == SCRIPT_TYPE_WORLD && !refresh_control_disable) {
		world_pos_type = m_worldcoordchoice->GetSelection();
		world_pos_x = selectedint;
		world_pos_y = selectedint;
		wxClientDC dc(m_worlddisplaypanel);
		WorldMapDraw(dc);
	}
}

void ScriptEditDialog::UpdateMultiLangDialogHelp(wxChoice* dialogchoice) {
	if (!use_text || script.multi_lang_script==NULL) {
		dialogchoice->UnsetToolTip();
		return;
	}
	SteamLanguage lang, baselang = script.multi_lang_script->base_script_lang[script.current_language];
	int dialsel = dialogchoice->GetSelection();
	if (dialsel==wxNOT_FOUND)
		dialsel = 0;
	uint16_t transdialid, basedialid = dialsel;
	unsigned int i;
	if (baselang!=script.current_language)
		for (i=0;i<script.multi_lang_script->lang_script_text_id[script.current_language].size();i++)
			if (dialsel==script.multi_lang_script->lang_script_text_id[script.current_language][i]) {
				basedialid = script.multi_lang_script->base_script_text_id[script.current_language][i];
				break;
			}
	wxString line,translationhelp = _(L"");
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (lang!=script.current_language && script.multi_lang_script->base_script_lang[lang]==baselang) {
			transdialid = basedialid;
			if (lang!=baselang)
				for (i=0;i<script.multi_lang_script->base_script_text_id[lang].size();i++)
					if (basedialid==script.multi_lang_script->base_script_text_id[lang][i]) {
						transdialid = script.multi_lang_script->lang_script_text_id[lang][i];
						break;
					}
			if (translationhelp.Len()>0)
				translationhelp += _(L"\n");
			line = HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[lang].Upper()+_(L": ")+_(FF9String::RemoveOpcodes(text->text[transdialid].multi_lang_str[lang],SCRIPT_TEXT_LINK_PREVIEW_LENGTH/2));
			line.Replace(_(L"\n"),_(L" "));
			translationhelp += line;
		}
	if (translationhelp.Len()>0)
		dialogchoice->SetToolTip(translationhelp);
	else
		dialogchoice->UnsetToolTip();
}

wxCheckBox* ScriptEditDialog::ArgCreateFlag(wxString& arg, unsigned int id) {
	wxCheckBox* res = new wxCheckBox(m_argpanel,SS_ARG_ID+id,wxEmptyString);
	res->SetLabel(_(HADES_STRING_ON_OFF_BUTTON));
	if (arg.IsNumber())
		res->SetValue(wxAtoi(arg));
	res->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(ScriptEditDialog::OnArgFlag),NULL,this);
	arg_control_type[id] = ARG_CONTROL_FLAG;
	return res;
}

wxTextCtrl* ScriptEditDialog::ArgCreateText(wxString& arg, unsigned int id) {
	wxTextCtrl* res = new wxTextCtrl(m_argpanel,SS_ARG_ID+id,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	res->SetLabel(_(HADES_STRING_NYI));
	arg_control_type[id] = ARG_CONTROL_TEXT;
	return res;
}

wxSpinCtrl* ScriptEditDialog::ArgCreateSpin(wxString& arg, unsigned int id, int size, bool sign) {
	wxSpinCtrl* res = new wxSpinCtrl(m_argpanel,SS_ARG_ID+id);
	long long range = (1LL << (size*8));
	if (sign)
		res->SetRange(-range/2,range/2-1);
	else
		res->SetRange(0,range-1);
	if (arg.IsNumber())
		res->SetValue(wxAtoi(arg));
	else
		res->SetValue(0);
	res->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED,wxSpinEventHandler(ScriptEditDialog::OnArgSpin),NULL,this);
	arg_control_type[id] = ARG_CONTROL_SPIN;
	return res;
}

wxChoice* ScriptEditDialog::ArgCreateChoice(wxString& arg, unsigned int id, vector<uint16_t*> choiceid, wxArrayString& choicestr) {
	wxChoice* res = new wxChoice(m_argpanel,SS_ARG_ID+id);
	if (choiceid.size()>0)
		res->Append(choicestr,(void**)choiceid.data());
	else
		res->Append(choicestr);
	if (arg.IsNumber()) {
		int argval = wxAtoi(arg);
		if (choiceid.size()>0) {
			for (unsigned int i=0;i<choiceid.size();i++)
				if (argval==*choiceid[i]) {
					res->SetSelection(i);
					break;
				}
		} else {
			res->SetSelection(argval);
		}
	}
	res->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgChoice),NULL,this);
	arg_control_type[id] = ARG_CONTROL_CHOICE;
	return res;
}

wxWindow* ScriptEditDialog::ArgCreateDialog(wxString& arg, unsigned int id, vector<uint16_t*> choiceid, wxArrayString& choicestr) {
	if (script.multi_lang_script==NULL)
		return ArgCreateChoice(arg,id,choiceid,choicestr);
	bool uselink = false;
	SteamLanguage lang;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (lang!=script.current_language && script.multi_lang_script->base_script_lang[lang]==script.multi_lang_script->base_script_lang[script.current_language]) {
			uselink = true;
			break;
		}
	if (!uselink)
		return ArgCreateChoice(arg,id,choiceid,choicestr);
	wxPanel* res = new wxPanel(m_argpanel);
	wxFlexGridSizer* grid = new wxFlexGridSizer(2);
	wxChoice* dialchoice = new wxChoice(res,SS_ARG_ID+id);
	wxButton* linkbtn = new wxButton(res,SS_ARG_ID+id,_(L"..."),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
	LinkedDialogStruct* datagroup = new LinkedDialogStruct;
	grid->AddGrowableCol(0);
	grid->SetFlexibleDirection(wxBOTH);
	grid->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	grid->Add(dialchoice,0,wxALL | wxEXPAND,5);
	grid->Add(linkbtn,0,wxALL,5);
	if (choiceid.size()>0)
		dialchoice->Append(choicestr,(void**)choiceid.data());
	else
		dialchoice->Append(choicestr);
	if (arg.IsNumber()) {
		int argval = wxAtoi(arg);
		if (choiceid.size()>0) {
			for (unsigned int i=0;i<choiceid.size();i++)
				if (argval==*choiceid[i]) {
					dialchoice->SetSelection(i);
					break;
				}
		} else {
			dialchoice->SetSelection(argval);
		}
		UpdateMultiLangDialogHelp(dialchoice);
	}
	datagroup->dialog = dialchoice;
	datagroup->link_btn = linkbtn;
	res->SetClientData((void*)datagroup);
	dialchoice->SetClientData((void*)datagroup);
	linkbtn->SetClientData((void*)datagroup);
	dialchoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgChoice),NULL,this);
	linkbtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(ScriptEditDialog::OnArgDialogLink),NULL,this);
	res->SetSizer(grid);
	res->Layout();
	grid->Fit(res);
	arg_control_type[id] = ARG_CONTROL_LINKED_DIAL;
	return res;
}

wxPanel* ScriptEditDialog::ArgCreateDiscFieldChoice(wxString& arg, unsigned int id, vector<uint16_t*> choiceid, wxArrayString& choicestr) {
	wxPanel* res = new wxPanel(m_argpanel);
	wxGridSizer* grid = new wxGridSizer(0,1,0,0);
	wxChoice* field = new wxChoice(res,SS_ARG_ID+id);
	wxChoice* disc = new wxChoice(res,SS_ARG_ID+id);
	unsigned int i;
	if (choiceid.size()>0)
		field->Append(choicestr,(void**)choiceid.data());
	else
		field->Append(choicestr);
	disc->Append(disc_str);
	if (arg.IsNumber()) {
		int argval = wxAtoi(arg);
		if (choiceid.size()>0) {
			for (i=0;i<choiceid.size();i++)
				if ((argval & 0x3FFF)==*choiceid[i]) {
					field->SetSelection(i);
					break;
				}
			if (i==choicestr.Count())
				field->SetSelection(0);
		} else
			field->SetSelection(argval & 0x3FFF);
		disc->SetSelection(argval >> 14);
	} else {
		field->SetSelection(0);
		disc->SetSelection(0);
	}
	grid->Add(field,0,wxALL,5);
	grid->Add(disc,0,wxALL,5);
	DiscFieldStruct* datagroup = new DiscFieldStruct;
	datagroup->disc = disc;
	datagroup->field = field;
	res->SetClientData((void*)datagroup);
	field->SetClientData((void*)datagroup);
	disc->SetClientData((void*)datagroup);
	field->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgField),NULL,this);
	disc->Connect(wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(ScriptEditDialog::OnArgDisc),NULL,this);
	res->SetSizer(grid);
	res->Layout();
	grid->Fit(res);
	arg_control_type[id] = ARG_CONTROL_DISC;
	return res;
}

wxPanel* ScriptEditDialog::ArgCreateFlags(wxString& arg, unsigned int id, unsigned int amount, wxArrayString& flagstr) {
	wxPanel* res = new wxPanel(m_argpanel);
	wxGridSizer* grid = new wxGridSizer(0,4,0,0);
	FlagsStruct* datagroup = new FlagsStruct;
	unsigned int i;
	datagroup->amount = amount;
	datagroup->box = new wxCheckBox*[amount];
	wxCheckBox**& box = datagroup->box;
	for (i=0;i<amount;i++) {
		box[i] = new wxCheckBox(res,SS_ARG_ID+id*SS_ARGBOX_MAXID+i,wxEmptyString);
		box[i]->SetLabel(flagstr[i]);
		grid->Add(box[i],0,wxALL,5);
		box[i]->SetClientData((void*)datagroup);
		box[i]->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(ScriptEditDialog::OnArgFlags),NULL,this);
	}
	res->SetClientData((void*)datagroup);
	res->SetSizer(grid);
	res->Layout();
	grid->Fit(res);
	if (arg.IsNumber())
		for (i=0;i<amount;i++)
			box[i]->SetValue(wxAtoi(arg) & (0x1 << i));
	arg_control_type[id] = ARG_CONTROL_FLAGS;
	return res;
}

void ArgPositionDraw(wxDC& dc, PositionStruct* posdata) {
	unsigned char* bmpdata = (unsigned char*)malloc(POSITION_PANEL_SIZE*POSITION_PANEL_SIZE*3*sizeof(unsigned char));
	for (unsigned int i=0;i<POSITION_PANEL_SIZE*POSITION_PANEL_SIZE*3;i++)
		bmpdata[i] = 255;
	wxImage mdcimg(POSITION_PANEL_SIZE,POSITION_PANEL_SIZE,bmpdata);
	wxBitmap mdcbmp(mdcimg);
	wxMemoryDC mdc(mdcbmp);
	mdc.SetFont(wxFont(wxFontInfo(8)));
	mdc.DrawLine(POSITION_PANEL_SIZE/2,0,POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE);
	mdc.DrawLine(0,POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE,POSITION_PANEL_SIZE/2);
	mdc.DrawText(wxString::Format(wxT("%d"),-2*posdata->zoom/3),POSITION_PANEL_SIZE/6-15,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),-posdata->zoom/3),POSITION_PANEL_SIZE/3-10,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),posdata->zoom/3),2*POSITION_PANEL_SIZE/3-10,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(2*POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2-3,2*POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),2*posdata->zoom/3),5*POSITION_PANEL_SIZE/6-15,POSITION_PANEL_SIZE/2+5);
	mdc.DrawLine(5*POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2-3,5*POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4);
	mdc.DrawText(wxString::Format(wxT("%d"),2*posdata->zoom/3),POSITION_PANEL_SIZE/2+5,POSITION_PANEL_SIZE/6-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4,POSITION_PANEL_SIZE/6);
	mdc.DrawText(wxString::Format(wxT("%d"),posdata->zoom/3),POSITION_PANEL_SIZE/2+5,POSITION_PANEL_SIZE/3-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4,POSITION_PANEL_SIZE/3);
	mdc.DrawText(wxString::Format(wxT("%d"),-posdata->zoom/3),POSITION_PANEL_SIZE/2+5,2*POSITION_PANEL_SIZE/3-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,2*POSITION_PANEL_SIZE/3,POSITION_PANEL_SIZE/2+4,2*POSITION_PANEL_SIZE/3);
	mdc.DrawText(wxString::Format(wxT("%d"),-2*posdata->zoom/3),POSITION_PANEL_SIZE/2+5,5*POSITION_PANEL_SIZE/6-8);
	mdc.DrawLine(POSITION_PANEL_SIZE/2-3,5*POSITION_PANEL_SIZE/6,POSITION_PANEL_SIZE/2+4,5*POSITION_PANEL_SIZE/6);
	mdc.SetPen(wxPen(POSITION_POINT_COLOR));
	mdc.SetBrush(wxBrush(POSITION_POINT_COLOR));
	mdc.DrawCircle(posdata->drawpt,5);
	dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
}

wxPanel* ScriptEditDialog::ArgCreatePosition(wxArrayString& arg, unsigned int id) {
	wxPanel* res = new wxPanel(m_argpanel, SS_ARG_ID + id, wxDefaultPosition, wxSize(POSITION_PANEL_SIZE, POSITION_PANEL_SIZE));
	PositionStruct* posdata = new PositionStruct;
	posdata->zoom = script_type == SCRIPT_TYPE_WORLD ? 2400 : 150;
	posdata->drawpt = wxPoint(POSITION_PANEL_SIZE / 2, POSITION_PANEL_SIZE / 2);
	posdata->use_z = arg.Count() == 3;
	posdata->x = wxAtoi(arg[0]);
	if (posdata->use_z) {
		posdata->y = wxAtoi(arg[2]);
		posdata->z = wxAtoi(arg[1]);
	} else {
		posdata->y = wxAtoi(arg[1]);
	}
	res->SetClientData((void*)posdata);
	res->Connect(wxEVT_PAINT, wxPaintEventHandler(ScriptEditDialog::OnArgPositionPaint), NULL, this);
	res->Connect(wxEVT_MOTION, wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseMove), NULL, this);
	res->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseMove), NULL, this);
	res->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseUp), NULL, this);
	res->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(ScriptEditDialog::OnArgPositionMouseWheel), NULL, this);
	res->Connect(wxEVT_CHAR_HOOK, wxKeyEventHandler(ScriptEditDialog::OnArgPositionKeyboard), NULL, this);
	arg_control_type[id] = ARG_CONTROL_POSITION;
	return res;
}

wxColourPickerCtrl* ScriptEditDialog::ArgCreateColorPicker(wxArrayString& arg, unsigned int id, bool rgb) {
	wxColourPickerCtrl* res = new wxColourPickerCtrl(m_argpanel,SS_ARG_ID+id);
	if (rgb) {
		res->SetColour(wxColour(wxAtoi(arg[0]),wxAtoi(arg[1]),wxAtoi(arg[2])));
		arg_control_type[id] = ARG_CONTROL_COLOR_RGB;
	} else {
		res->SetColour(wxColour(255-wxAtoi(arg[0]),255-wxAtoi(arg[1]),255-wxAtoi(arg[2])));
		arg_control_type[id] = ARG_CONTROL_COLOR_CMY;
	}
	res->Connect(wxEVT_COLOURPICKER_CHANGED,wxColourPickerEventHandler(ScriptEditDialog::OnArgColorPicker),NULL,this);
	return res;
}

void ScriptEditDialog::ScriptChangeArg(int argi, int64_t value, int argshift) {
	wxString scripttxt = m_scripttext->GetLineText(line_selection);
	size_t argpos = scripttxt.Length();
	wxString token, newtxt = scripttxt;
	int shiftedargi = argi;
	int i;
	GetNextWord(scripttxt);
	token = GetNextPunc(scripttxt);
	if (!token.IsSameAs(L'('))
		return;
	SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[current_opcode];
	for (i = 0; i < shiftedargi; i++) {
		GetNextArg(scripttxt);
		token = GetNextPunc(scripttxt);
		if (!token.IsSameAs(L','))
			return;
		if (current_opcode != 0xFFFF && i < scriptop.arg_amount) {
			if (scriptop.arg_type[i] == AT_POSITION_X || scriptop.arg_type[i] == AT_POSITION_Z || /*
			 */ scriptop.arg_type[i] == AT_COLOR_CYAN || scriptop.arg_type[i] == AT_COLOR_MAGENTA || /*
			 */ scriptop.arg_type[i] == AT_COLOR_RED || scriptop.arg_type[i] == AT_COLOR_GREEN)
					shiftedargi++;
		}
	}
	for (i = 0; i < argshift; i++) {
		GetNextArg(scripttxt);
		token = GetNextPunc(scripttxt);
		if (!token.IsSameAs(L','))
			return;
	}
	scripttxt.Trim(false);
	argpos -= scripttxt.Length();
	GetNextArg(scripttxt);
	newtxt = newtxt.Mid(0, argpos);
	newtxt << value;
	if (argi + 1 == arg_amount)
		newtxt += _(L" ");
	newtxt += scripttxt;
	long from = m_scripttext->XYToPosition(0, line_selection);
	long to = from + m_scripttext->GetLineLength(line_selection);
	m_scripttext->Replace(from, to, newtxt);
	refresh_help_force = true;
}

void ScriptEditDialog::OnFunctionChoose(wxListEvent& event) {
	unsigned int entryid = 0, funcid = 0;
	long sel = event.GetIndex();
	while (script.entry_function_amount[entryid]==0)
		entryid++;
	while (sel>0) {
		funcid++;
		while (funcid>=script.entry_function_amount[entryid]) {
			entryid++;
			funcid = 0;
		}
		sel--;
	}
	if (entry_selection!=SCRIPT_ID_NO_ENTRY) {
		localvar_str[entry_selection] = m_localvartext->GetValue();
		func_str[entry_selection][function_selection] = m_scripttext->GetValue();
	}
	DisplayFunction(entryid,funcid);
}

void ScriptEditDialog::OnIntValueText(wxCommandEvent& event) {
	int id = event.GetId();
	wxString intvalue = _(L"");
	wxString txtvalue = event.GetString();
	unsigned int i, j;
	if (id == wxID_INT) {
		uint16_t val = wxAtoi(txtvalue);
		intvalue = wxString::Format(wxT("%u"), val);
	} else if (id == wxID_LONG) {
		uint32_t val = wxAtoi(txtvalue);
		intvalue = wxString::Format(wxT("%uL"), wxAtoi(txtvalue));
	} else if (id == wxID_HEXA) {
		uint32_t val;
		txtvalue.ToULong((unsigned long*)&val, 16);
		if (val > 0x7FFF)
			intvalue = wxString::Format(wxT("%uL"), val);
		else
			intvalue = wxString::Format(wxT("%u"), val);
	} else if (id == wxID_ITEM) {
		for (i = 0; i < item_str.Count(); i++)
			if (item_str[i].IsSameAs(txtvalue, false)) {
				intvalue = wxString::Format(wxT("%u"), *item_id[i]);
				break;
			}
	} else if (id == wxID_BUTTON) {
		wxArrayString tokens = wxStringTokenize(txtvalue, L"|", wxTOKEN_STRTOK);
		long val = 0;
		for (i = 0; i < tokens.Count(); i++) {
			RemoveSurroundingSpaces(tokens[i]);
			for (j = 0; j < G_V_ELEMENTS(PlaystationButton); j++) {
				if (tokens[i].IsSameAs(PlaystationButton[j], false)) {
					val |= (1 << j);
					break;
				}
			}
		}
		if (val > 0x7FFF)
			intvalue = wxString::Format(wxT("%uL"), val);
		else if (val > 0)
			intvalue = wxString::Format(wxT("%u"), val);
	} else if (id == wxID_STATUSA || id == wxID_STATUSB) {
		wxArrayString tokens = wxStringTokenize(txtvalue, L"|", wxTOKEN_STRTOK);
		long val = 0;
		unsigned int min = id == wxID_STATUSA ? 0 : 24;
		unsigned int max = id == wxID_STATUSA ? 24 : HADES_STRING_STATUS.size();
		for (i = 0; i < tokens.Count(); i++) {
			RemoveSurroundingSpaces(tokens[i]);
			for (j = min; j < max; j++) {
				if (tokens[i].IsSameAs(HADES_STRING_STATUS[j], false)) {
					val |= (1 << (j - min));
					break;
				}
			}
		}
		if (val > 0x7FFF)
			intvalue = wxString::Format(wxT("%uL"), val);
		else if (val > 0)
			intvalue = wxString::Format(wxT("%u"), val);
	} else if (id == wxID_LIST) {
		wxArrayString tokens = wxStringTokenize(txtvalue, L";");
		long val = 0;
		for (i = 0; i < tokens.Count(); i++) {
			if (tokens[i][0] == L'[')
				tokens[i] = tokens[i].Mid(1);
			if (tokens[i].Last() == L']')
				tokens[i] = tokens[i].Mid(0, tokens[i].Len() - 1);
			RemoveSurroundingSpaces(tokens[i]);
			val |= ((wxAtoi(tokens[i]) & 0x3F) << (i * 6));
		}
		if (val > 0x7FFF)
			intvalue = wxString::Format(wxT("%uL"), val);
		else
			intvalue = wxString::Format(wxT("%u"), val);
	} else if (id == wxID_SPLIST) {
		wxArrayString tokens = wxStringTokenize(txtvalue, L";");
		long val = 0;
		for (i = 0; i < tokens.Count(); i++) {
			if (tokens[i][0] == L'[')
				tokens[i] = tokens[i].Mid(1);
			if (tokens[i].Last() == L']')
				tokens[i] = tokens[i].Mid(0, tokens[i].Len() - 1);
			RemoveSurroundingSpaces(tokens[i]);
			for (j = 0; j < enemy->spell_amount; j++) {
				if (tokens[i].IsSameAs(attack_str[j], false)) {
					val |= (j << (i * 6));
					break;
				}
			}
		}
		if (val > 0x7FFF)
			intvalue = wxString::Format(wxT("%uL"), val);
		else
			intvalue = wxString::Format(wxT("%u"), val);
	} else if (id == wxID_SPELL) {
		for (i = 0; i < ability_str.Count(); i++)
			if (ability_str[i].IsSameAs(txtvalue, false)) {
				intvalue = wxString::Format(wxT("%u"), i);
				break;
			}
	} else if (id == wxID_CMD) {
		for (i = 0; i < command_str.Count(); i++)
			if (command_str[i].IsSameAs(txtvalue, false)) {
				intvalue = wxString::Format(wxT("%u"), i);
				break;
			}
	}
	m_scripttext->WriteText(intvalue);
	if (intvalue.Last() == L'L')
		m_scripttext->SetInsertionPoint(m_scripttext->GetInsertionPoint() - 1);
}

void ScriptEditDialog::OnFunctionRightClick(wxListEvent& event) {
	unsigned int entryid = 0, funcid = 0;
	long sel = event.GetIndex();
	if (sel!=wxNOT_FOUND) {
		while (script.entry_function_amount[entryid]==0)
			entryid++;
		while (sel>0) {
			funcid++;
			while (funcid>=script.entry_function_amount[entryid]) {
				entryid++;
				funcid = 0;
			}
			sel--;
		}
		m_functionlist->SetItemState(event.GetItem(), wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		m_functionlist->PopupMenu(func_popup_menu);
	}
}

class ScriptEditPropertiesDialog : public ScriptEditPropertiesWindow
{
public:
	ScriptEditPropertiesDialog(wxWindow* parent, bool enableWorldType) : ScriptEditPropertiesWindow(parent) {
		m_typeworldxlabel->Enable(enableWorldType);
		m_typeworldylabel->Enable(enableWorldType);
		m_typeterrainlabel->Enable(enableWorldType);
		m_typeworldx->Enable(enableWorldType);
		m_typeworldy->Enable(enableWorldType);
		m_typeterrain->Enable(enableWorldType);
	}

private:
	void ScriptEditPropertiesDialog::OnWorldSpin(wxSpinEvent& event) {
		m_typectrl->SetValue(0x8000 | (m_typeworldy->GetValue() << 8 & 0x3F00) | (m_typeworldx->GetValue() << 2 & 0xFC) | (m_typeterrain->GetValue() & 3));
	}
};

void ScriptEditDialog::OnFunctionRightClickMenu(wxCommandEvent& event) {
	long sel = m_functionlist->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	long seltmp = sel;
	if (sel == -1)
		return;
	unsigned int entryid = 0, funcid = 0;
	while (script.entry_function_amount[entryid] == 0)
		entryid++;
	while (seltmp > 0) {
		funcid++;
		while (funcid >= script.entry_function_amount[entryid]) {
			entryid++;
			funcid = 0;
		}
		seltmp--;
	}
	int id = event.GetId();
	unsigned int i;
	if (id == wxID_ADD) {
		if (extra_size < 8) {
			wxMessageDialog popup(NULL, HADES_STRING_MISSING_SPACE, HADES_STRING_ERROR, wxOK | wxCENTRE);
			popup.ShowModal();
			return;
		}
		ScriptEditPropertiesDialog dial(this, script_type == SCRIPT_TYPE_WORLD && entryid == 0);
		for (i = 0; i < script.entry_amount; i++)
			dial.m_entryctrl->Append(entry_name[i]);
		dial.m_entryctrl->SetSelection(entryid);
		dial.m_entrytypectrl->SetValue(script.entry_type[entryid]);
		dial.m_entrytypectrl->Enable(false);
		dial.m_typectrl->SetValue(script.function_type[entryid][funcid]);
		bool showdiag = true;
		while (showdiag)
		{
			showdiag = false;
			if (dial.ShowModal() == wxID_OK) {
				unsigned int entrysel = dial.m_entryctrl->GetSelection();
				for (i = 0; i < script.entry_function_amount[entrysel]; i++)
					if (script.function_type[entrysel][i] == dial.m_typectrl->GetValue())
					{
						showdiag = true;
						break;
					}
				if (showdiag)
				{
					wxMessageDialog popup(NULL, HADES_STRING_SCRIPT_SAME_FUNCTYPE, HADES_STRING_ERROR, wxOK | wxCENTRE);
					popup.ShowModal();
					continue;
				}
				if (entryid != entrysel) {
					entryid = entrysel;
					funcid = script.entry_function_amount[entryid];
					seltmp = 0;
					sel = 0;
					while (seltmp < (long)entryid) {
						sel += script.entry_function_amount[seltmp];
						seltmp++;
					}
					seltmp = 0;
					while (seltmp < (long)funcid) {
						sel++;
						seltmp++;
					}
				} else {
					funcid++;
					sel++;
				}
				//script.entry_type[entryid] = dial.m_entrytypectrl->GetValue();
				if (script.entry_function_amount[entryid] == 0) {
					extra_size -= 8;
					if (script.entry_type[entryid] == 0xFF)
						script.entry_type[entryid] = 0;
				} else {
					extra_size -= 4;
				}
				AddFunction(entryid, funcid, dial.m_typectrl->GetValue());
				DisplayFunctionList(sel, -1);
				m_functionlist->SetItemState(sel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
			}
		}
	} else if (id == wxID_REMOVE) {
		if (entryid == 0 && funcid == 0) {
			wxMessageDialog popup(NULL, HADES_STRING_SCRIPT_NO_DELETE, HADES_STRING_ERROR, wxOK | wxCENTRE);
			popup.ShowModal();
			return;
		}
		extra_size += script.RemoveFunction(entryid, funcid);
		DisplayFunctionList(-1, sel);
		for (i = funcid; i < script.entry_function_amount[entryid]; i++)
			func_str[entryid][i] = func_str[entryid][i + 1];
		if ((int)funcid < m_functionlist->GetItemCount())
			m_functionlist->SetItemState(funcid, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	} else if (id == wxID_SET) {
		ScriptEditPropertiesDialog dial(this, script_type == SCRIPT_TYPE_WORLD && entryid == 0);
		for (i = 0; i < script.entry_amount; i++)
			dial.m_entryctrl->Append(entry_name[i]);
		dial.m_entryctrl->SetSelection(entryid);
		dial.m_entryctrl->Enable(false);
		dial.m_entrytypectrl->SetValue(script.entry_type[entryid]);
		dial.m_typectrl->SetValue(script.function_type[entryid][funcid]);
		if (script.function_type[entryid][funcid] >= 0x8000)
		{
			dial.m_typeworldx->SetValue((script.function_type[entryid][funcid] & 0xFC) >> 2);
			dial.m_typeworldy->SetValue((script.function_type[entryid][funcid] & 0x3F00) >> 8);
			dial.m_typeterrain->SetValue(script.function_type[entryid][funcid] & 3);
		}
		if (dial.ShowModal() == wxID_OK) {
			script.entry_type[entryid] = dial.m_entrytypectrl->GetValue();
			script.function_type[entryid][funcid] = dial.m_typectrl->GetValue();
			UpdateWorldRegion(entryid, funcid);
			functionlist_str[sel] = GetFunctionName(entryid, script.function_type[entryid][funcid]);
			DisplayFunctionList(-1, -1);
			m_functionlist->SetItemState(sel, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	}
}

void ScriptEditDialog::OnFunctionUpdate(wxCommandEvent& event) {
	unsigned int i,j,funclistpos = 0;
	for (i=0;i<entry_selection;i++)
		for (j=0;j<script.entry_function_amount[i];j++)
			funclistpos++;
	for (j=0;j<function_selection;j++)
		funclistpos++;
	m_functionlist->SetItemTextColour(funclistpos,FUNCTION_COLOR_MODIFIED);
	func_should_parse[funclistpos] = true;
}

void ScriptEditDialog::OnFunctionNewLine(wxCommandEvent& event) {
	long selfrom, selto;
	m_scripttext->GetSelection(&selfrom,&selto);
	if (selfrom!=selto)
		return;
	wxString indentstr = _(L"");
	long y, ip = m_scripttext->GetInsertionPoint();
	if (ip>1 && m_scripttext->GetRange(ip-2,ip-1).IsSameAs(L"{"))
		indentstr += TAB_STR;
	if (m_scripttext->PositionToXY(m_scripttext->GetInsertionPoint(),NULL,&y) && y>0) {
		wxString scriptline = m_scripttext->GetLineText(y-1);
		indentstr += scriptline.Mid(0,scriptline.find_first_not_of(L' '));
	}
	if (indentstr.Length()>0)
		m_scripttext->WriteText(indentstr);
}

void ScriptEditDialog::OnChoiceSelection(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_FIELD) {
		if (event.GetSelection()>0)
			UpdateLineHelp(text_x_selection,line_selection);
		else
			gl_window->DisplayFieldPlane(0,0);
	} else if (id==wxID_WORLD) {
		if (event.GetSelection()>0)
			UpdateLineHelp(text_x_selection,line_selection);
		else {
			world_pos_type = 0;
			wxClientDC dc(m_worlddisplaypanel);
			WorldMapDraw(dc);
		}
	}
}

void ScriptEditDialog::OnCheckBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_FBACK) {
		gl_window->field_showtiles = event.IsChecked();
		gl_window->Draw();
	} else if (id==wxID_FWALK) {
		gl_window->field_showwalk = event.IsChecked();
		gl_window->Draw();
	}
}

void ScriptEditDialog::OnWorldMapPaint(wxPaintEvent& event) {
	wxPaintDC dc(m_worlddisplaypanel);
	WorldMapDraw(dc);
}

void ScriptEditDialog::OnShowHideLocalVar(wxMouseEvent& event) {
	if (m_localvartext->IsShown()) {
		m_localvartext->Hide();
		wxImage tmpimg = wxBITMAP(bulletright_image).ConvertToImage();
		m_localvarshowimg->SetBitmap(wxBitmap(tmpimg));
	} else {
		m_localvartext->Show();
		wxImage tmpimg = wxBITMAP(bulletdown_image).ConvertToImage();
		m_localvarshowimg->SetBitmap(tmpimg);
	}
	m_scripttext->Layout();
	Layout();
	Refresh();
	event.Skip();
}

void ScriptEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i,j,funclistpos = 0;
	if (id==wxID_PARSE) {
		currentvar_str = m_localvartext->GetValue();
		LogStruct log = ParseFunction(m_scripttext->GetValue(), entry_selection, function_selection);
		int sizegap = extra_size+script.entry_size[entry_selection]-GetParsedEntryNewSize();
		if (sizegap<0) {
			wxString errstr;
			errstr.Printf(wxT(HADES_STRING_LOGERROR_SPACE), -sizegap);
			log.AddError(errstr.ToStdWstring());
		}
		LogDialog dial(this, log);
		dial.ShowModal();
		if (log.ok) {
			extra_size = sizegap;
			ApplyParsedFunction();
			for (i=0;i<entry_selection;i++)
				for (j=0;j<script.entry_function_amount[i];j++)
					funclistpos++;
			for (j=0;j<function_selection;j++)
				funclistpos++;
			m_functionlist->SetItemTextColour(funclistpos, FUNCTION_COLOR_PARSED);
			func_should_parse[funclistpos] = false;
		} else {
			unsigned int funclistpos = 0;
			for (i=0;i<entry_selection;i++)
				for (j=0;j<script.entry_function_amount[i];j++)
					funclistpos++;
			for (j=0;j<function_selection;j++)
				funclistpos++;
			m_functionlist->SetItemTextColour(funclistpos, FUNCTION_COLOR_FAILED);
			func_should_parse[funclistpos] = true;
		}
	} else if (id==wxID_HELP) {
		if (help_dial==NULL || !help_dial->IsShown()) {
			help_dial = new ScriptHelpDialog(this);
			help_dial->Show();
		} else {
			help_dial->SetFocus();
		}
	} else {
		if (id==wxID_OK) {
			bool shouldparse = false;
			for (i=0;i<script.entry_amount;i++)
				for (j=0;j<script.entry_function_amount[i];j++)
					if (func_should_parse[funclistpos++])
						shouldparse = true;
			if (shouldparse) {
				wxMessageDialog popup(this, _(HADES_STRING_SCRIPT_SHOULDPARSE), _(HADES_STRING_WARNING), wxYES_NO|wxCENTRE);
				if (popup.ShowModal()==wxID_YES)
					EndModal(id);
			} else
				EndModal(id);
		} else
			EndModal(id);
	}
}

void ScriptEditDialog::OnTimer(wxTimerEvent& event) {
	if (m_functionlist->GetSelectedItemCount() == 0)
		m_functionlist->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	long x, y;
	if (!m_scripttext->PositionToXY(m_scripttext->GetInsertionPoint(), &x, &y))
		return;
	if (!wxGetMouseState().LeftIsDown())
		refresh_control_disable = false;
	if (x == text_x_selection && y == line_selection && !refresh_help_force)
		return;
	refresh_help_force = false;
	wxString line = m_scripttext->GetLineText(y);
	bool refreshctrl = y != line_selection;
	UpdateLineHelp(x, y);
	text_x_selection = x;
	line_selection = y;
	if (!refresh_control_disable)
		DisplayOperation(line, refreshctrl);
}

void ScriptEditDialog::OnArgFlag(wxCommandEvent& event) {
	int argi = event.GetId() - SS_ARG_ID;
	ScriptChangeArg(argi, event.IsChecked() ? 1 : 0);
}

void ScriptEditDialog::OnArgSpin(wxSpinEvent& event) {
	int argi = event.GetId() - SS_ARG_ID;
	ScriptChangeArg(argi, event.GetPosition());
}

void ScriptEditDialog::OnArgChoice(wxCommandEvent& event) {
	int argi = event.GetId() - SS_ARG_ID;
	if (arg_control_type[argi] == ARG_CONTROL_LINKED_DIAL) {
		LinkedDialogStruct* datagroup = static_cast<LinkedDialogStruct*>(static_cast<wxChoice*>(event.GetEventObject())->GetClientData());
		uint16_t* objid = (uint16_t*)datagroup->dialog->GetClientData(event.GetInt());
		if (objid)
			ScriptChangeArg(argi, *objid);
		else
			ScriptChangeArg(argi, event.GetInt());
		UpdateMultiLangDialogHelp(static_cast<wxChoice*>(event.GetEventObject()));
	} else {
		uint16_t* objid = (uint16_t*)event.GetClientData();
		if (objid)
			ScriptChangeArg(argi, *objid);
		else
			ScriptChangeArg(argi, event.GetInt());
	}
}

void ScriptEditDialog::OnArgFlags(wxCommandEvent& event) {
	int argi = (event.GetId() - SS_ARG_ID) / SS_ARGBOX_MAXID;
	FlagsStruct* datagroup = static_cast<FlagsStruct*>(static_cast<wxCheckBox*>(event.GetEventObject())->GetClientData());
	int64_t value = 0;
	unsigned int i;
	for (i = 0; i < datagroup->amount; i++)
		value |= (datagroup->box[i]->IsChecked() ? 1 : 0) << i;
	ScriptChangeArg(argi, value);
}

void ScriptEditDialog::OnArgField(wxCommandEvent& event) {
	int argi = event.GetId() - SS_ARG_ID;
	DiscFieldStruct* datagroup = static_cast<DiscFieldStruct*>(static_cast<wxChoice*>(event.GetEventObject())->GetClientData());
	uint16_t* objid = (uint16_t*)datagroup->field->GetClientData(event.GetInt());
	if (objid)
		ScriptChangeArg(argi, (datagroup->disc->GetSelection() << 14) | *objid);
	else
		ScriptChangeArg(argi, (datagroup->disc->GetSelection() << 14) | event.GetInt());
}

void ScriptEditDialog::OnArgDisc(wxCommandEvent& event) {
	int argi = event.GetId() - SS_ARG_ID;
	DiscFieldStruct* datagroup = static_cast<DiscFieldStruct*>(static_cast<wxChoice*>(event.GetEventObject())->GetClientData());
	uint16_t* objid = (uint16_t*)datagroup->field->GetClientData(datagroup->field->GetSelection());
	if (objid)
		ScriptChangeArg(argi, (event.GetInt() << 14) | *objid);
	else
		ScriptChangeArg(argi, (event.GetInt() << 14) | datagroup->field->GetSelection());
}

void ScriptEditDialog::OnArgDialogLink(wxCommandEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	LinkedDialogStruct* datagroup = static_cast<LinkedDialogStruct*>(static_cast<wxButton*>(event.GetEventObject())->GetClientData());
	SteamLanguage lang, baselang = script.multi_lang_script->base_script_lang[script.current_language];
	SteamLanguage langShiftedlink[STEAM_LANGUAGE_AMOUNT];
	int dialsel = datagroup->dialog->GetSelection();
	if (dialsel==wxNOT_FOUND)
		dialsel = 0;
	uint16_t basedialid = dialsel;
	unsigned int i;
	if (baselang!=script.current_language)
		for (i=0;i<script.multi_lang_script->lang_script_text_id[script.current_language].size();i++)
			if (dialsel==script.multi_lang_script->lang_script_text_id[script.current_language][i]) {
				basedialid = script.multi_lang_script->base_script_text_id[script.current_language][i];
				break;
			}
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (script.multi_lang_script->base_script_lang[lang]==baselang)
			langShiftedlink[lang] = script.current_language;
		else
			langShiftedlink[lang] = script.multi_lang_script->base_script_lang[lang];
	ScriptEditTextLinkDialog dial(this,langShiftedlink,*text,script.current_language);
	dial.message_link_base.assign(1,dialsel);
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (lang!=script.current_language && langShiftedlink[lang]==script.current_language) {
			dial.message_link[lang].assign(1,basedialid);
			if (lang!=baselang)
				for (i=0;i<script.multi_lang_script->base_script_text_id[lang].size();i++)
					if (basedialid==script.multi_lang_script->base_script_text_id[lang][i]) {
						dial.message_link[lang][0] = script.multi_lang_script->lang_script_text_id[lang][i];
						break;
					}
		}
	if (dial.ShowModal(dialsel)==wxID_OK) {
		if (baselang!=script.current_language)
			basedialid = dial.message_link[baselang][0];
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			if (lang!=baselang && langShiftedlink[lang]==script.current_language) {
				uint16_t langtextid = lang==script.current_language ? dial.message_link_base[0] : dial.message_link[lang][0];
				for (i=0;i<script.multi_lang_script->base_script_text_id[lang].size();i++)
					if (basedialid==script.multi_lang_script->base_script_text_id[lang][i]) {
						script.multi_lang_script->lang_script_text_id[lang][i] = langtextid;
						break;
					}
				if (i>=script.multi_lang_script->base_script_text_id[lang].size()) {
					script.multi_lang_script->base_script_text_id[lang].push_back(basedialid);
					script.multi_lang_script->lang_script_text_id[lang].push_back(langtextid);
				}
			}
		UpdateMultiLangDialogHelp(datagroup->dialog);
	}
}

void ScriptEditDialog::OnArgPositionPaint(wxPaintEvent& event) {
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	wxPaintDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc, posdata);
}

void ScriptEditDialog::OnArgPositionMouseMove(wxMouseEvent& event) {
	if (!event.LeftIsDown())
		return;
	refresh_control_disable = true;
	int argi = event.GetId() - SS_ARG_ID;
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	posdata->drawpt = wxPoint(event.GetPosition());
	ScriptChangeArg(argi, posdata->x + posdata->GetAsTranslation().x);
	if (posdata->use_z)
		ScriptChangeArg(argi, posdata->y + posdata->GetAsTranslation().y, 2);
	else
		ScriptChangeArg(argi, posdata->y + posdata->GetAsTranslation().y, 1);
	wxString line = m_scripttext->GetLineText(line_selection);
	DisplayOperation(line, false, false);
	wxClientDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc, posdata);
	event.Skip();
}

void ScriptEditDialog::OnArgPositionMouseUp(wxMouseEvent& event) {
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	posdata->x += posdata->GetAsTranslation().x;
	posdata->y += posdata->GetAsTranslation().y;
	posdata->drawpt = wxPoint(POSITION_PANEL_SIZE/2,POSITION_PANEL_SIZE/2);
	wxClientDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc,posdata);
	event.Skip();
}

void ScriptEditDialog::OnArgPositionMouseWheel(wxMouseEvent& event) {
	if (event.LeftIsDown())
		return;
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	unsigned int maxzoom = script_type == SCRIPT_TYPE_WORLD ? 153600 : 2400;
	if (event.GetWheelRotation() > 0 && posdata->zoom > 75)
		posdata->zoom /= 2;
	else if (event.GetWheelRotation() < 0 && posdata->zoom < (int)maxzoom)
		posdata->zoom *= 2;
	wxClientDC dc((wxWindow*)event.GetEventObject());
	ArgPositionDraw(dc, posdata);
}

void ScriptEditDialog::OnArgPositionKeyboard(wxKeyEvent& event) {
	PositionStruct* posdata = static_cast<PositionStruct*>(static_cast<wxPanel*>(event.GetEventObject())->GetClientData());
	int argi = event.GetId() - SS_ARG_ID;
	int key = event.GetKeyCode();
	if (key == WXK_UP && posdata->use_z) {
		posdata->z -= posdata->zoom / 6;
		ScriptChangeArg(argi, posdata->z, 1);
		wxString line = m_scripttext->GetLineText(line_selection);
		DisplayOperation(line, false, false);
	} else if (key == WXK_DOWN && posdata->use_z) {
		posdata->z += posdata->zoom / 6;
		ScriptChangeArg(argi, posdata->z, 1);
		wxString line = m_scripttext->GetLineText(line_selection);
		DisplayOperation(line, false, false);
	} else {
		event.Skip();
	}
}

void ScriptEditDialog::OnArgColorPicker(wxColourPickerEvent& event) {
	int argi = event.GetId()-SS_ARG_ID;
	if (arg_control_type[argi]==ARG_CONTROL_COLOR_RGB) {
		ScriptChangeArg(argi,event.GetColour().Red());
		ScriptChangeArg(argi,event.GetColour().Green(),1);
		ScriptChangeArg(argi,event.GetColour().Blue(),2);
	} else {
		ScriptChangeArg(argi,255-event.GetColour().Red());
		ScriptChangeArg(argi,255-event.GetColour().Green(),1);
		ScriptChangeArg(argi,255-event.GetColour().Blue(),2);
	}
}

void ScriptEditDialog::OnPickWalkmesh(wxMouseEvent& event) {
	if (current_opcode == 0xFFFF || script_type != SCRIPT_TYPE_FIELD || gl_window->field_walk == NULL) {
		event.Skip();
		return;
	}
	int argi = -1;
	bool pickpath = false;
	SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[current_opcode];
	for (unsigned int i = 0; i < arg_amount; i++) {
		if (scriptop.arg_type[i] == AT_WALKTRIANGLE) {
			argi = i;
			break;
		} else if (scriptop.arg_type[i] == AT_WALKPATH) {
			pickpath = true;
			argi = i;
			break;
		}
	}
	if (argi >= 0) {
		int triid = gl_window->GetMouseTriangle(event);
		if (triid >= 0) {
			if (pickpath)
				ScriptChangeArg(argi, gl_window->field_walk->triangle_walkpath[triid]);
			else
				ScriptChangeArg(argi, triid);
		}
	}
	event.Skip();
}

ScriptEditEntryDialog::ScriptEditEntryDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype) :
	ScriptEditEntryWindow(parent),
	script_type(scpttype) {
	unsigned int i;
	base_entry_amount = scpt.entry_amount;
	entry_amount = scpt.entry_amount;
	base_entry_id.resize(entry_amount);
	entry_type.resize(entry_amount);
	for (i=0;i<entry_amount;i++) {
		base_entry_id[i] = i;
		entry_type[i] = scpt.entry_type[i];
	}
	has_character_entry = entry_amount>SCRIPT_FIXED_ENTRY_AMOUNT;
	extra_size = scpt.GetExtraSize();
}

ScriptEditEntryDialog::~ScriptEditEntryDialog() {
}

int ScriptEditEntryDialog::ShowModal() {
	unsigned int i,normalentry = entry_amount;
	if (entry_amount>0)
		m_entrylist->Append(_(L"Main"));
	if (has_character_entry)
		normalentry -= SCRIPT_FIXED_ENTRY_AMOUNT;
	for (i=1;i<normalentry;i++) {
		if (entry_type[i]==0)
			m_entrylist->Append(_(L"Code"));
		else if (entry_type[i]==1)
			m_entrylist->Append(_(L"Region"));
		else if (entry_type[i]==2)
			m_entrylist->Append(_(L"Object"));
		else 
			m_entrylist->Append(_(L"Unknown"));
	}
	if (has_character_entry) {
		for (i=0;i<8;i++)
			m_entrylist->Append(HADES_STRING_CHARACTER_DEFAULT_NAME[i]);
		m_entrylist->Append(HADES_STRING_CHARACTER_DEFAULT_NAME[11]); // Beatrix
	}
	if (entry_amount>0) {
		m_entrylist->SetSelection(0);
		DisplayEntry(0);
	}
	return wxDialog::ShowModal();
}

void ScriptEditEntryDialog::ApplyModifications(ScriptDataStruct& scpt) {
	unsigned int i,j;
	int lostentryarg = 0;
	j = 0;
	for (i=0;i<entry_amount;i++) {
		if (j<base_entry_amount && base_entry_id[j]==i) {
			j++;
		} else {
			while (j<base_entry_amount && base_entry_id[j]<0) {
				scpt.RemoveEntry(i,&lostentryarg);
				j++;
			}
			if (j>=base_entry_amount || base_entry_id[j]!=i)
				scpt.AddEntry(i,entry_type[i]);
			else
				j++;
		}
	}
	while (j<base_entry_amount && base_entry_id[j]<0) {
		scpt.RemoveEntry(entry_amount,&lostentryarg);
		j++;
	}
	if (lostentryarg>0) {
		wxString warningstr;
		warningstr.Printf(wxT(HADES_STRING_ENTRY_ARG_LOST),lostentryarg);
		wxMessageDialog popup(NULL,warningstr,HADES_STRING_WARNING,wxOK|wxCENTRE);
		popup.ShowModal();
	}
}

void ScriptEditEntryDialog::DisplayEntry(int sel) {
	m_entrytype->SetValue(entry_type[sel]);
	if (has_character_entry) {
		m_buttonadd->Enable(sel + SCRIPT_FIXED_ENTRY_AMOUNT < (int)entry_amount);
		m_buttonremove->Enable(sel + SCRIPT_FIXED_ENTRY_AMOUNT < (int)entry_amount && sel > 0);
	} else {
		m_buttonremove->Enable(sel > 0);
	}
}

void ScriptEditEntryDialog::OnEntrySelect(wxCommandEvent& event) {
	int sel = event.GetSelection();
	DisplayEntry(sel);
}

void ScriptEditEntryDialog::OnSpinCtrl(wxSpinEvent& event) {
	int sel = m_entrylist->GetSelection();
	if (sel==wxNOT_FOUND)
		return;
	entry_type[sel] = event.GetPosition();
	if (entry_type[sel]==0)
		m_entrylist->SetString(sel,_(L"Code"));
	else if (entry_type[sel]==1)
		m_entrylist->SetString(sel,_(L"Region"));
	else if (entry_type[sel]==2)
		m_entrylist->SetString(sel,_(L"Object"));
	else
		m_entrylist->SetString(sel,_(L"Unknown"));
}

void ScriptEditEntryDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	int sel = m_entrylist->GetSelection();
	unsigned int i;
	if (id==wxID_ADD) {
		if (extra_size<0x10) {
			wxMessageDialog popup(NULL,HADES_STRING_MISSING_SPACE,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popup.ShowModal();
			return;
		}
		sel++;
		entry_type.insert(entry_type.begin()+sel,0);
		for (i=0;i<base_entry_amount;i++)
			if (base_entry_id[i]>=sel)
				base_entry_id[i]++;
		entry_amount++;
		extra_size -= 0x10;
		m_entrylist->Insert(_(L"Code"),sel);
		m_entrylist->SetSelection(sel);
		DisplayEntry(sel);
	} else if (id==wxID_REMOVE) {
		if (sel==wxNOT_FOUND)
			return;
		entry_amount--;
		entry_type.erase(entry_type.begin()+sel);
		for (i=0;i<base_entry_amount;i++)
			if (base_entry_id[i]==sel)
				base_entry_id[i] = -1;
			else if (base_entry_id[i]>sel)
				base_entry_id[i]--;
		extra_size += 0x10;
		m_entrylist->Delete(sel);
		if (sel==entry_amount)
			sel--;
		m_entrylist->SetSelection(sel);
		DisplayEntry(sel);
	} else
		EndModal(id);
}

//=====================================//
// Language Links and Correspondancies //
//=====================================//

ScriptEditLinkDialog::ScriptEditLinkDialog(wxWindow* parent, ScriptDataStruct& scpt, TextDataStruct& td) :
	ScriptEditLinkWindow(parent),
	text(&td) {
	SteamLanguage lang,baselang,sublang;
	unsigned int i,j;
	lang_link[0] = m_langlink1;	lang_link[1] = m_langlink2;	lang_link[2] = m_langlink3;	lang_link[3] = m_langlink4;
	lang_link[4] = m_langlink5;	lang_link[5] = m_langlink6;	lang_link[6] = m_langlink7;
	lang_text[0] = m_langtext1;	lang_text[1] = m_langtext2;	lang_text[2] = m_langtext3;	lang_text[3] = m_langtext4;
	lang_text[4] = m_langtext5;	lang_text[5] = m_langtext6;	lang_text[6] = m_langtext7;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
		baselang = scpt.multi_lang_script->base_script_lang[lang];
		lang_link[lang]->SetSelection(baselang);
		message_link[lang] = message_link_base[baselang];
		for (i=0;i<scpt.multi_lang_script->base_script_text_id[lang].size();i++) {
			for (j=0;j<message_link_base[baselang].size();j++)
				if (message_link_base[baselang][j]==scpt.multi_lang_script->base_script_text_id[lang][i]) {
					message_link[lang][j] = scpt.multi_lang_script->lang_script_text_id[lang][i];
					break;
				}
			if (j>=message_link_base[baselang].size()) {
				message_link_base[baselang].push_back(scpt.multi_lang_script->base_script_text_id[lang][i]);
				message_link[lang].push_back(scpt.multi_lang_script->lang_script_text_id[lang][i]);
			}
		}
		lang_link[lang]->Enable(scpt.multi_lang_script->is_loaded[lang] && !hades::STEAM_SINGLE_LANGUAGE_MODE);
		if (scpt.multi_lang_script->is_loaded[lang] && !hades::STEAM_SINGLE_LANGUAGE_MODE) {
			bool enabletext = false;
			for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
				if (lang!=sublang && scpt.multi_lang_script->base_script_lang[sublang]==lang) {
					enabletext = true;
					break;
				}
			lang_text[lang]->Enable(enabletext);
		} else {
			lang_text[lang]->Enable(false);
		}
		is_modified[lang] = false;
	}
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
		baselang = scpt.multi_lang_script->base_script_lang[lang];
		for (i=message_link[lang].size();i<message_link_base[baselang].size();i++)
			message_link[lang].push_back(message_link_base[baselang][i]);
	}
}

ScriptEditLinkDialog::~ScriptEditLinkDialog() {
}

void ScriptEditLinkDialog::ApplyModifications(ScriptDataStruct& scpt) {
	SteamLanguage lang,baselang;
	scpt.UpdateSteamMultiLanguage();
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (scpt.multi_lang_script->is_loaded[lang]) {
			baselang = lang_link[lang]->GetSelection();
			if (is_modified[lang] || is_modified[baselang]) {
				if (scpt.multi_lang_script->base_script_lang[baselang]!=baselang)
					scpt.LinkLanguageScripts(baselang,baselang,message_link[baselang],message_link[baselang]);
				scpt.LinkLanguageScripts(lang,baselang,message_link[lang],message_link_base[baselang]);
			}
		}
}

void ScriptEditLinkDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_OK || id==wxID_CANCEL) {
		EndModal(id);
		return;
	}
	SteamLanguage lang, sublang;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (static_cast<wxButton*>(event.GetEventObject())==lang_text[lang]) {
			SteamLanguage link[STEAM_LANGUAGE_AMOUNT];
			for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
				link[sublang] = lang_link[sublang]->GetSelection();
			lang = lang_link[lang]->GetSelection();
			ScriptEditTextLinkDialog dial(this,link,*text,lang);
			dial.message_link_base = message_link_base[lang];
			for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
				if (link[sublang]==lang)
					dial.message_link[sublang] = message_link[sublang];
			if (dial.ShowModal()==wxID_OK) {
				message_link_base[lang] = dial.message_link_base;
				for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
					if (link[sublang]==lang)
						message_link[sublang] = dial.message_link[sublang];
				is_modified[lang] = true;
			}
			break;
		}
}

void ScriptEditLinkDialog::OnChangeLink(wxCommandEvent& event) {
	SteamLanguage lang,baselang,sublang;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (static_cast<wxChoice*>(event.GetEventObject())==lang_link[lang]) {
			baselang = lang_link[event.GetSelection()]->GetSelection();
			if (lang!=event.GetSelection() && lang_link[event.GetSelection()]->GetSelection()!=event.GetSelection())
				lang_link[lang]->SetSelection(baselang);
			if (lang!=baselang) {
				for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
					if (lang_link[sublang]->GetSelection()==lang) {
						lang_link[sublang]->SetSelection(sublang);
						is_modified[sublang] = true;
					}
				message_link_base[lang].clear();
				message_link[lang] = message_link_base[baselang];
			}
			is_modified[lang] = true;
			break;
		}
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (lang==lang_link[lang]->GetSelection()) {
			bool islinked = false;
			for (sublang=0;sublang<STEAM_LANGUAGE_AMOUNT;sublang++)
				if (sublang!=lang && lang_link[sublang]->GetSelection()==lang) {
					islinked = true;
					break;
				}
			lang_text[lang]->Enable(islinked);
			if (!islinked) {
				message_link_base[lang].clear();
				message_link[lang].clear();
			}
		} else {
			lang_text[lang]->Enable(false);
			message_link_base[lang].clear();
		}
}

ScriptEditTextLinkDialog::ScriptEditTextLinkDialog(wxWindow* parent, SteamLanguage* linklang, TextDataStruct& td, SteamLanguage baselang) :
	ScriptEditTextLinkWindow(parent),
	text(&td),
	base_lang(baselang) {
	wxArrayString dialoglist;
	SteamLanguage lang;
	unsigned int i;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		link[lang] = linklang[lang];
	for (i=0;i<text->amount;i++)
		if (text->text[i].multi_lang_init[base_lang])
			dialoglist.Add(_(FF9String::RemoveOpcodes(text->text[i].multi_lang_str[base_lang],SCRIPT_TEXT_LINK_PREVIEW_LENGTH)));
		else
			break;
	m_baselangdialog->Append(dialoglist);
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (link[lang]==base_lang && lang!=base_lang) {
			dialoglist.Clear();
			link_choice[lang] = new wxChoice(this,wxID_ANY);
			for (i=0;i<text->amount;i++)
				if (text->text[i].multi_lang_init[lang])
					dialoglist.Add(_(FF9String::RemoveOpcodes(text->text[i].multi_lang_str[lang],SCRIPT_TEXT_LINK_PREVIEW_LENGTH)));
				else
					break;
			link_choice[lang]->Append(dialoglist);
			m_translationsizer->Add(link_choice[lang],0,wxEXPAND|wxALL,2);
			link_choice[lang]->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ScriptEditTextLinkDialog::OnChooseDialog ), NULL, this );
		} else {
			link_choice[lang] = NULL;
		}
	Fit();
	Layout();
	Refresh();
}

ScriptEditTextLinkDialog::~ScriptEditTextLinkDialog() {
	for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (link_choice[lang]!=NULL)
			link_choice[lang]->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ScriptEditTextLinkDialog::OnChooseDialog ), NULL, this );
}

int ScriptEditTextLinkDialog::ShowModal(int dialogid) {
	if (dialogid<0)
		dialogid = 0;
	else
		m_baselangdialog->Enable(false);
	m_baselangdialog->SetSelection(dialogid);
	UpdateDialogSelection();
	return ScriptEditTextLinkWindow::ShowModal();
}

void ScriptEditTextLinkDialog::UpdateDialogSelection() {
	SteamLanguage lang;
	unsigned int i;
	int dialogid = m_baselangdialog->GetSelection();
	for (i=0;i<message_link_base.size();i++)
		if (message_link_base[i]==dialogid) {
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				if (link_choice[lang]!=NULL)
					link_choice[lang]->SetSelection(message_link[lang][i]);
			break;
		}
	if (i>=message_link_base.size()) {
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			if (link_choice[lang]!=NULL)
				link_choice[lang]->SetSelection(dialogid);
	}
}

void ScriptEditTextLinkDialog::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}

void ScriptEditTextLinkDialog::OnChooseDialog(wxCommandEvent& event) {
	wxChoice* dialchoice = static_cast<wxChoice*>(event.GetEventObject());
	if (dialchoice==m_baselangdialog) {
		UpdateDialogSelection();
	} else {
		SteamLanguage lang;
		unsigned int dialindex;
		for (dialindex=0;dialindex<message_link_base.size();dialindex++)
			if (message_link_base[dialindex]==m_baselangdialog->GetSelection())
				break;
		if (dialindex>=message_link_base.size()) {
			dialindex = message_link_base.size();
			message_link_base.push_back(m_baselangdialog->GetSelection());
			for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
				if (link_choice[lang]!=NULL)
					message_link[lang].push_back(m_baselangdialog->GetSelection());
		}
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			if (dialchoice==link_choice[lang]) {
				message_link[lang][dialindex] = event.GetSelection();
				break;
			}
	}
}
