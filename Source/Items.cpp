#include "Items.h"

#include <set>
#include <unordered_map>
#include "DllEditor.h"
#include "main.h"
#include "Database_Item.h"
#include "Database_CSV.h"
#include "CommonUtility.h"

#define ITEM_HWS_VERSION 3

#define ITEM_CSV_CHECK L"%id%;%weapon_id%;%armor_id%;%usable_id%;%price%;%sell_price%;%icon%;%icon_color%;%equip_position%;%stat_id%;%skill_list%;%type_weapon%;%type_armlet%;%type_helmet%;%type_armor%;%type_accessory%;%type_item%;%type_gem%;%type_usable%;%menu_position%;%zidane%;%vivi%;%garnet%;%steiner%;%freya%;%quina%;%eiko%;%amarant%;%cinna%;%marcus%;%blank%;%beatrix%"
#define ITEM_CSV_DEFAULT ITEM_CSV_CHECK L";# %id% - %name%"
#define WEAPON_CSV_CHECK L"%weapon_id%;%weapon_category%;%weapon_status%;%weapon_model%;%weapon_script%;%weapon_power%;%weapon_element%;%weapon_accuracy%;%weapon_offset1%;%weapon_offset2%;%weapon_hitsfx%;%weapon_textures%"
#define WEAPON_CSV_DEFAULT L"%name%;" WEAPON_CSV_CHECK
#define ARMOR_CSV_CHECK L"%armor_id%;%armor_defence%;%armor_evade%;%armor_magic_defence%;%armor_magic_evade%"
#define ARMOR_CSV_DEFAULT L"%name%;" ARMOR_CSV_CHECK
#define USABLE_CSV_CHECK L"%usable_id%;%usable_target_type%;%usable_target_default_ally%;%usable_panel%;%usable_model%;%usable_target_for_dead%;%usable_target_default_on_dead%;%usable_script%;%usable_power%;%usable_accuracy%;%usable_element%;%usable_status%"
#define USABLE_CSV_DEFAULT USABLE_CSV_CHECK L";# %id% - %name%"
#define ITEMSTAT_CSV_CHECK L"%stat_id%;%stat_speed%;%stat_strength%;%stat_magic%;%stat_spirit%;%stat_element_boost%;%stat_element_immune%;%stat_element_absorb%;%stat_element_half%;%stat_element_weak%"
#define ITEMSTAT_CSV_DEFAULT L"Bonus %stat_id% - %name%;" ITEMSTAT_CSV_CHECK

const unsigned int steam_item_field_size[] = { 16, 16, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
const unsigned int steam_item_field_array[] = { 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 };
const unsigned int steam_usable_field_size[] = { 4, 1, 3, 9, 12, 1, 1, 1, 8, 8, 8, 8, 32 };
const unsigned int steam_armor_field_size[] = { 8, 8, 8, 8 };
const unsigned int steam_stat_field_size[] = { 8, 8, 8, 8, 8, 8, 8, 8, 32 };
const unsigned int steam_weapon_field_size[] = { 8, 8, 16, 8, 8, 8, 8, 16, 16 };
const unsigned int steam_weapon_field_array[] = { 0, 0, 0, 0, 0, 0, 0, 2, 0 };

int ItemDataStruct::SetName(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(name);
		tmp.SetValue(newvalue);
		int oldlen = name.length;
		int newlen = tmp.length;
		if (parent->name_space_used + newlen > parent->name_space_total + oldlen)
			return 1;
		parent->name_space_used += newlen - oldlen;
	}
	name.SetValue(newvalue);
	return 0;
}

int ItemDataStruct::SetName(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = name.length;
		int newlen = newvalue.length;
		if (parent->name_space_used + newlen > parent->name_space_total + oldlen)
			return 1;
		parent->name_space_used += newlen - oldlen;
	}
	name = newvalue;
	return 0;
}

int ItemDataStruct::SetHelp(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(help);
		tmp.SetValue(newvalue);
		int oldlen = help.length;
		int newlen = tmp.length;
		if (parent->help_space_used + newlen > parent->help_space_total + oldlen)
			return 1;
		parent->help_space_used += newlen - oldlen;
	}
	help.SetValue(newvalue);
	return 0;
}

int ItemDataStruct::SetHelp(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = help.length;
		int newlen = newvalue.length;
		if (parent->help_space_used + newlen > parent->help_space_total + oldlen)
			return 1;
		parent->help_space_used += newlen - oldlen;
	}
	help = newvalue;
	return 0;
}

int ItemDataStruct::SetBattleHelp(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(battle_help);
		tmp.SetValue(newvalue);
		int oldlen = battle_help.length;
		int newlen = tmp.length;
		if (parent->help2_space_used + newlen > parent->help2_space_total + oldlen)
			return 1;
		parent->help2_space_used += newlen - oldlen;
	}
	battle_help.SetValue(newvalue);
	return 0;
}

int ItemDataStruct::SetBattleHelp(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = battle_help.length;
		int newlen = newvalue.length;
		if (parent->help2_space_used + newlen > parent->help2_space_total + oldlen)
			return 1;
		parent->help2_space_used += newlen - oldlen;
	}
	battle_help = newvalue;
	return 0;
}

wxString GetEquipPositionStr(ItemDataStruct& it, ItemDataSet* itemset) {
	if (it.equip_position_type == ITEM_POSITION_ABSOLUTE)
		return wxString::Format(wxT("%d"), it.equip_position);
	int index;
	for (index = 0; index < (int)itemset->item.size(); index++)
		if (itemset->item[index].id == it.id)
			break;
	if (index >= (int)itemset->item.size())
		return wxString::Format(wxT("%d"), it.id);
	int posprecision = itemset->item.size() > ITEM_AMOUNT ? (int)ceil(log10(itemset->item.size() - ITEM_AMOUNT + 1)) : 0;
	float posstep = 1.0f / pow(10.0f, posprecision);
	float pos;
	if (itemset->GetEquipPositionFloat(index, posstep, &pos))
		return wxString::Format(wxString::Format(wxT("%%.%df"), posprecision), pos);
	return wxString::Format(wxT("%d"), it.id);
}

wxString GetMenuPositionStr(ItemDataStruct& it, ItemDataSet* itemset) {
	if (it.menu_position_type == ITEM_POSITION_ABSOLUTE)
		return wxString::Format(wxT("%d"), it.menu_position);
	int index;
	for (index = 0; index < (int)itemset->item.size(); index++)
		if (itemset->item[index].id == it.id)
			break;
	if (index >= (int)itemset->item.size())
		return wxString::Format(wxT("%d"), it.id);
	int posprecision = itemset->item.size() > ITEM_AMOUNT ? (int)ceil(log10(itemset->item.size() - ITEM_AMOUNT + 1)) : 0;
	float posstep = 1.0f / pow(10.0f, posprecision);
	float pos;
	if (itemset->GetMenuPositionFloat(index, posstep, &pos))
		return wxString::Format(wxString::Format(wxT("%%.%df"), posprecision), pos);
	return wxString::Format(wxT("%d"), it.id);
}

wxString ItemDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("weapon_id")) return wxString::Format(wxT("%d"), weapon_id);
	if (fieldname.IsSameAs("armor_id")) return wxString::Format(wxT("%d"), armor_id);
	if (fieldname.IsSameAs("usable_id")) return wxString::Format(wxT("%d"), usable_id);
	if (fieldname.IsSameAs("stat_id")) return wxString::Format(wxT("%d"), stat_id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("%s"), name.str_nice);
	if (fieldname.IsSameAs("help")) return wxString::Format(wxT("%s"), help.str_nice);
	if (fieldname.IsSameAs("price")) return wxString::Format(wxT("%d"), price);
	if (fieldname.IsSameAs("sell_price")) return wxString::Format(wxT("%d"), sell_price);
	if (fieldname.IsSameAs("icon")) return wxString::Format(wxT("%d"), icon);
	if (fieldname.IsSameAs("icon_color")) return wxString::Format(wxT("%d"), icon_color);
	if (fieldname.IsSameAs("equip_position")) return GetEquipPositionStr(*this, parent);
	if (fieldname.IsSameAs("skill_list")) {
		string concat = ConcatenateStrings<AnyAbilityStruct>(", ", skill, [](AnyAbilityStruct ab) { return ab.IsVoid() || (hades::PREFER_EXPORT_AS_PATCHES && ab.GetRawId() >= 256) ? "" : ab.GetStringId(); }, true);
		return wxString::Format(wxT("%s"), concat.empty() ? "0" : concat);
	}
	if (fieldname.IsSameAs("type")) return wxString::Format(wxT("%d"), type);
	if (fieldname.IsSameAs("type_weapon")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_WEAPON) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_armlet")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_WRIST) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_helmet")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_HEAD) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_armor")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_ARMOR) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_accessory")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_ACCESSORY) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_item")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_USABLE) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_gem")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_SPECIAL) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("type_usable")) return wxString::Format(wxT("%d"), (type & ITEM_TYPE_CONSUMABLE) != 0 ? 1 : 0);
	if (fieldname.IsSameAs("menu_position")) return GetMenuPositionStr(*this, parent);
	if (fieldname.IsSameAs("char_availability")) return wxString::Format(wxT("%d"), char_availability);
	if (fieldname.IsSameAs("zidane")) return wxString::Format(wxT("%d"), (char_availability >> 11) & 1);
	if (fieldname.IsSameAs("vivi")) return wxString::Format(wxT("%d"), (char_availability >> 10) & 1);
	if (fieldname.IsSameAs("garnet")) return wxString::Format(wxT("%d"), (char_availability >> 9) & 1);
	if (fieldname.IsSameAs("steiner")) return wxString::Format(wxT("%d"), (char_availability >> 8) & 1);
	if (fieldname.IsSameAs("freya")) return wxString::Format(wxT("%d"), (char_availability >> 7) & 1);
	if (fieldname.IsSameAs("quina")) return wxString::Format(wxT("%d"), (char_availability >> 6) & 1);
	if (fieldname.IsSameAs("eiko")) return wxString::Format(wxT("%d"), (char_availability >> 5) & 1);
	if (fieldname.IsSameAs("amarant")) return wxString::Format(wxT("%d"), (char_availability >> 4) & 1);
	if (fieldname.IsSameAs("cinna")) return wxString::Format(wxT("%d"), (char_availability >> 3) & 1);
	if (fieldname.IsSameAs("marcus")) return wxString::Format(wxT("%d"), (char_availability >> 2) & 1);
	if (fieldname.IsSameAs("blank")) return wxString::Format(wxT("%d"), (char_availability >> 1) & 1);
	if (fieldname.IsSameAs("beatrix")) return wxString::Format(wxT("%d"), char_availability & 1);
	if (fieldname.StartsWith("weapon_") && parent->GetWeaponIndexById(weapon_id) >= 0) {
		ItemWeaponDataStruct& weap = parent->GetWeaponById(weapon_id);
		if (fieldname.IsSameAs("weapon_category")) return wxString::Format(wxT("%d"), weap.flag);
		if (fieldname.IsSameAs("weapon_status")) return wxString::Format(wxT("%d"), weap.status);
		if (fieldname.IsSameAs("weapon_model")) return wxString::Format(wxT("%s"), weap.model_name);
		if (fieldname.IsSameAs("weapon_model_id")) return wxString::Format(wxT("%d"), weap.model);
		if (fieldname.IsSameAs("weapon_script")) return wxString::Format(wxT("%d"), weap.damage_formula);
		if (fieldname.IsSameAs("weapon_power")) return wxString::Format(wxT("%d"), weap.power);
		if (fieldname.IsSameAs("weapon_element")) return wxString::Format(wxT("%d"), weap.element);
		if (fieldname.IsSameAs("weapon_accuracy")) return wxString::Format(wxT("%d"), weap.status_accuracy);
		if (fieldname.IsSameAs("weapon_offset1")) return wxString::Format(wxT("%d"), weap.offset1);
		if (fieldname.IsSameAs("weapon_offset2")) return wxString::Format(wxT("%d"), weap.offset2);
		if (fieldname.IsSameAs("weapon_hitsfx")) return wxString::Format(wxT("%d"), weap.hit_sfx);
		if (fieldname.IsSameAs("weapon_textures")) return wxString::Format(wxT("%s"), ConcatenateStrings<wstring>(", ", weap.texture_names, [](wstring str) { return ConvertWStrToStr(str); }, true));
	}
	if (fieldname.StartsWith("armor_") && parent->GetArmorIndexById(armor_id) >= 0) {
		ItemArmorDataStruct& armor = parent->GetArmorById(armor_id);
		if (fieldname.IsSameAs("armor_defence")) return wxString::Format(wxT("%d"), armor.defence);
		if (fieldname.IsSameAs("armor_evade")) return wxString::Format(wxT("%d"), armor.evade);
		if (fieldname.IsSameAs("armor_magic_defence")) return wxString::Format(wxT("%d"), armor.magic_defence);
		if (fieldname.IsSameAs("armor_magic_evade")) return wxString::Format(wxT("%d"), armor.magic_evade);
	}
	if (fieldname.StartsWith("usable_") && parent->GetUsableIndexById(usable_id) >= 0) {
		ItemUsableDataStruct& usable = parent->GetUsableById(usable_id);
		if (fieldname.IsSameAs("usable_target_type_var")) return wxString::Format(wxT("%d"), usable.target_type);
		if (fieldname.IsSameAs("usable_target_type")) return wxString::Format(wxT("%d"), usable.target_type & 0xF);
		if (fieldname.IsSameAs("usable_target_default_ally")) return wxString::Format(wxT("%d"), (usable.target_type >> 4) & 0x1);
		if (fieldname.IsSameAs("usable_target_for_dead")) return wxString::Format(wxT("%d"), (usable.target_flag >> 5) & 0x1);
		if (fieldname.IsSameAs("usable_target_default_on_dead")) return wxString::Format(wxT("%d"), (usable.target_flag >> 7) & 0x1);
		if (fieldname.IsSameAs("usable_target_default_camera")) return wxString::Format(wxT("%d"), (usable.target_flag >> 6) & 0x1);
		if (fieldname.IsSameAs("usable_panel")) return wxString::Format(wxT("%d"), usable.GetPanel());
		if (fieldname.IsSameAs("usable_model")) return wxString::Format(wxT("%d"), usable.model);
		if (fieldname.IsSameAs("usable_script")) return wxString::Format(wxT("%d"), usable.effect);
		if (fieldname.IsSameAs("usable_power")) return wxString::Format(wxT("%d"), usable.power);
		if (fieldname.IsSameAs("usable_accuracy")) return wxString::Format(wxT("%d"), usable.accuracy);
		if (fieldname.IsSameAs("usable_element")) return wxString::Format(wxT("%d"), usable.element);
		if (fieldname.IsSameAs("usable_status")) return FormatStatusSet(usable.status); // TODO: https://github.com/Albeoris/Memoria/blob/main/Assembly-CSharp/Memoria/Data/Items/ItemEffect.cs / https://github.com/Albeoris/Memoria/blob/main/Assembly-CSharp/Global/BTL_SCENE.cs
	}
	if (fieldname.StartsWith("stat_") && parent->GetStatIndexById(stat_id) >= 0) {
		ItemStatDataStruct& stat = parent->GetStatById(stat_id);
		if (fieldname.IsSameAs("stat_speed")) return wxString::Format(wxT("%d"), stat.speed);
		if (fieldname.IsSameAs("stat_strength")) return wxString::Format(wxT("%d"), stat.strength);
		if (fieldname.IsSameAs("stat_magic")) return wxString::Format(wxT("%d"), stat.magic);
		if (fieldname.IsSameAs("stat_spirit")) return wxString::Format(wxT("%d"), stat.spirit);
		if (fieldname.IsSameAs("stat_element_boost")) return wxString::Format(wxT("%d"), stat.element_boost);
		if (fieldname.IsSameAs("stat_element_immune")) return wxString::Format(wxT("%d"), stat.element_immune);
		if (fieldname.IsSameAs("stat_element_absorb")) return wxString::Format(wxT("%d"), stat.element_absorb);
		if (fieldname.IsSameAs("stat_element_half")) return wxString::Format(wxT("%d"), stat.element_half);
		if (fieldname.IsSameAs("stat_element_weak")) return wxString::Format(wxT("%d"), stat.element_weak);
	}
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field.find(fieldname); search != parent->custom_field.end()) return search->second;
	return _(L"");
}

bool ItemDataStruct::CompareWithCSV(wxArrayString entries) {
	if (entries.GetCount() == 0)
		return false;
	if (!custom_field.empty())
		return false;
	int fieldcount = entries[0].Replace(";", ";") + 1;
	if (fieldcount == 33) {
		if (id >= (int)entries.GetCount()) return false;
		wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(ITEM_CSV_CHECK));
		return MemoriaUtility::CompareEntries(entries[id].BeforeLast(L';'), rightcsv);
	}
	if (fieldcount == 13) {
		if (entries[0].find(L'#') != wxString::npos) {
			if (usable_id < 0 || usable_id >= (int)entries.GetCount()) return false;
			wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(USABLE_CSV_CHECK));
			return MemoriaUtility::CompareEntries(entries[usable_id].BeforeLast(L';'), rightcsv);
		}
		if (weapon_id < 0 || weapon_id >= (int)entries.GetCount()) return false;
		wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(WEAPON_CSV_CHECK));
		return MemoriaUtility::CompareEntries(entries[weapon_id].AfterFirst(L';'), rightcsv);
	}
	if (fieldcount == 6) {
		if (armor_id < 0 || armor_id >= (int)entries.GetCount()) return false;
		wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(ARMOR_CSV_CHECK));
		return MemoriaUtility::CompareEntries(entries[armor_id].AfterFirst(L';'), rightcsv);
	}
	if (fieldcount == 11) {
		if (stat_id < 0 || stat_id >= (int)entries.GetCount()) return false;
		wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(ITEMSTAT_CSV_CHECK));
		return MemoriaUtility::CompareEntries(entries[stat_id].AfterFirst(L';'), rightcsv);
	}
	return false;
}

int KeyItemDataStruct::SetName(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(name);
		tmp.SetValue(newvalue);
		int oldlen = name.length;
		int newlen = tmp.length;
		if (parent->key_name_space_used + newlen > parent->key_name_space_total + oldlen)
			return 1;
		parent->key_name_space_used += newlen - oldlen;
	}
	name.SetValue(newvalue);
	return 0;
}

int KeyItemDataStruct::SetName(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = name.length;
		int newlen = newvalue.length;
		if (parent->key_name_space_used + newlen > parent->key_name_space_total + oldlen)
			return 1;
		parent->key_name_space_used += newlen - oldlen;
	}
	name = newvalue;
	return 0;
}

int KeyItemDataStruct::SetHelp(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(help);
		tmp.SetValue(newvalue);
		int oldlen = help.length;
		int newlen = tmp.length;
		if (parent->key_help_space_used + newlen > parent->key_help_space_total + oldlen)
			return 1;
		parent->key_help_space_used += newlen - oldlen;
	}
	help.SetValue(newvalue);
	return 0;
}

int KeyItemDataStruct::SetHelp(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = help.length;
		int newlen = newvalue.length;
		if (parent->key_help_space_used + newlen > parent->key_help_space_total + oldlen)
			return 1;
		parent->key_help_space_used += newlen - oldlen;
	}
	help = newvalue;
	return 0;
}

int KeyItemDataStruct::SetDescription(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		if (description_out_of_bounds)
			return 2;
		FF9String tmp(description);
		tmp.SetValue(newvalue);
		int oldlen = description.length;
		int newlen = tmp.length;
		if (parent->key_desc_space_used + newlen > parent->key_desc_space_total + oldlen)
			return 1;
		parent->key_desc_space_used += newlen - oldlen;
	}
	description.SetValue(newvalue);
	return 0;
}

int KeyItemDataStruct::SetDescription(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		if (description_out_of_bounds)
			return 2;
		int oldlen = description.length;
		int newlen = newvalue.length;
		if (parent->key_desc_space_used + newlen > parent->key_desc_space_total + oldlen)
			return 1;
		parent->key_desc_space_used += newlen - oldlen;
	}
	description = newvalue;
	return 0;
}

void ItemUsableDataStruct::InitializeDefault(int dataid) {
	id = dataid;
	target_type = 0;
	model = 0;
	target_flag = 0;
	effect = 0;
	power = 0;
	element = 0;
	accuracy = 0;
	status.clear();
}

Spell_Panel ItemUsableDataStruct::GetPanel() {
	return (target_type >> 5) & 0x7;
}

void ItemUsableDataStruct::SetPanel(Spell_Panel newvalue) {
	target_type = (target_type & 0x1F) | (newvalue << 5);
}

Spell_Target_Priority ItemUsableDataStruct::GetTargetPriority() {
	return (target_type >> 4) & 0x1;
}

void ItemUsableDataStruct::SetTargetPriority(Spell_Target_Priority newvalue) {
	target_type = (target_type & 0xEF) | (newvalue << 4);
}

Spell_Target_Type ItemUsableDataStruct::GetTargetType() {
	uint8_t val = target_type & 0xF;
	if (val == 0x0 || val == 0x3 || val == 0x6 || val == 0x9) // In Steam, 0x6 and 0x9 are SPELL_TARGET_TYPE_EVERYONE
		return SPELL_TARGET_TYPE_ANY;
	if (val == 0x1 || val == 0x4 || val == 0x7 || val == 0xA)
		return SPELL_TARGET_TYPE_ALLY;
	if (val == 0x2 || val == 0x5 || val == 0x8 || val == 0xB)
		return SPELL_TARGET_TYPE_ENEMY;
	if (val == 0xC)
		return SPELL_TARGET_TYPE_EVERYONE;
	if (val == 0xD)
		return SPELL_TARGET_TYPE_SELF;
	return SPELL_TARGET_TYPE_IRRELEVANT;
}

void ItemUsableDataStruct::SetTargetType(Spell_Target_Type newvalue) {
	uint8_t ta = target_type & 0xF;
	ta = ta >= 0xC ? 0 : ta / 3;
	if (newvalue == SPELL_TARGET_TYPE_EVERYONE)
		target_type = (target_type & 0xF0) + 0xC;
	else if (newvalue == SPELL_TARGET_TYPE_IRRELEVANT)
		target_type = (target_type & 0xF0) + 0xE;
	else if (newvalue == SPELL_TARGET_TYPE_SELF)
		target_type = (target_type & 0xE0) + 0x10 + 0xD;
	else if (newvalue == SPELL_TARGET_TYPE_ANY)
		target_type = (target_type & 0xF0) + 3 * ta;
	else if (newvalue == SPELL_TARGET_TYPE_ALLY)
		target_type = (target_type & 0xE0) + 0x10 + 3 * ta + 1;
	else if (newvalue == SPELL_TARGET_TYPE_ENEMY)
		target_type = (target_type & 0xE0) + 3 * ta + 2;
}

Spell_Target_Amount ItemUsableDataStruct::GetTargetAmount() {
	uint8_t val = target_type & 0xF;
	if (val < 0x3 || val == 0xD)
		return SPELL_TARGET_AMOUNT_ONE;
	if (val < 0x6)
		return SPELL_TARGET_AMOUNT_VARIABLE;
	if (val < 0x9 || val >= 0xC)
		return SPELL_TARGET_AMOUNT_GROUP;
	return SPELL_TARGET_AMOUNT_RANDOM;
}

void ItemUsableDataStruct::SetTargetAmount(Spell_Target_Amount newvalue) {
	if (GetTargetType() == SPELL_TARGET_TYPE_SELF && newvalue != SPELL_TARGET_AMOUNT_ONE)
		SetTargetType(SPELL_TARGET_TYPE_ALLY);
	if (GetTargetType() == SPELL_TARGET_TYPE_EVERYONE && newvalue != SPELL_TARGET_AMOUNT_GROUP)
		SetTargetType(SPELL_TARGET_TYPE_ANY);
	if (GetTargetType() == SPELL_TARGET_TYPE_IRRELEVANT)
		SetTargetType(SPELL_TARGET_TYPE_ANY);
	uint8_t tt = target_type & 0xF;
	if (newvalue == SPELL_TARGET_AMOUNT_ONE)
		target_type = (target_type & 0xF0) + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_VARIABLE)
		target_type = (target_type & 0xF0) + 0x3 + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_GROUP)
		target_type = (target_type & 0xF0) + 0x6 + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_RANDOM)
		target_type = (target_type & 0xF0) + 0x9 + tt % 3;
}

uint16_t ItemUsableDataStruct::GetSound() {
	return (model >> 9) | ((target_flag & 0x1F) << 7);
}

void ItemUsableDataStruct::SetSound(uint16_t newvalue) {
	model = (model & 0x1FF) | (newvalue & 0x7F << 9);
	target_flag = (target_flag & 0xE0) | ((newvalue & 0xF80) >> 7);
}

#define MACRO_ITEM_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	uint8_t zero8 = 0; \
	uint16_t zero16 = 0; \
	int skillrawid[3]; \
	int skillsize; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<itemamount;i++) { \
		if (useextendedtype) { \
			IO ## FlexibleChar(ffbin, item[i].usable_id, true); \
			IO ## FlexibleChar(ffbin, item[i].weapon_id, true); \
			IO ## FlexibleChar(ffbin, item[i].armor_id, true); \
		} \
		IO ## Short(ffbin,item[i].name_offset); \
		IO ## Short(ffbin,item[i].help_offset); \
		IO ## FlexibleShort(ffbin,item[i].price, useextendedtype); \
		if (useextendedtype2)	IO ## FlexibleChar(ffbin,item[i].sell_price, true); \
		else if (READ)			item[i].sell_price = item[i].price / 2; \
		IO ## Short(ffbin,item[i].char_availability); \
		IO ## FlexibleChar(ffbin,item[i].icon, useextendedtype); \
		IO ## Char(ffbin,item[i].icon_color); \
		if (useextendedtype) IO ## Char(ffbin, item[i].equip_position_type); \
		IO ## FlexibleChar(ffbin,item[i].equip_position, useextendedtype); \
		IO ## FlexibleChar(ffbin,item[i].stat_id, useextendedtype); \
		if (useextendedtype) { \
			if (!READ) skillsize = item[i].skill.size(); \
			IO ## FlexibleChar(ffbin, skillsize, true); \
			if (READ) item[i].skill.resize(skillsize); \
			for (j = 0; j < skillsize; j++) { \
				IO ## Char(ffbin, item[i].skill[j].is_active); \
				IO ## FlexibleChar(ffbin, item[i].skill[j].id, true); \
			} \
		} else { \
			if (READ) { \
				item[i].skill.resize(3); \
			} else { \
				skillrawid[0] = item[i].skill[0].GetRawId(); \
				skillrawid[1] = item[i].skill[1].GetRawId(); \
				skillrawid[2] = item[i].skill[2].GetRawId(); \
			} \
			IO ## FlexibleChar(ffbin,skillrawid[0], false); \
			IO ## FlexibleChar(ffbin,skillrawid[1], false); \
			IO ## FlexibleChar(ffbin,skillrawid[2], false); \
			if (READ) { \
				item[i].skill[0].Setup(skillrawid[0]); \
				item[i].skill[1].Setup(skillrawid[1]); \
				item[i].skill[2].Setup(skillrawid[2]); \
			} \
		} \
		IO ## Char(ffbin,item[i].type); \
		if (useextendedtype) IO ## Char(ffbin, item[i].menu_position_type); \
		IO ## FlexibleChar(ffbin,item[i].menu_position, useextendedtype); \
		IO ## Char(ffbin,item[i].zero); \
		if (useextendedtype2) IO ## CSVFields(ffbin, item[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<itemamount;i++) { \
		SEEK(ffbin,txtpos,item[i].name_offset); \
		IO ## FF9String(ffbin,item[i].name); \
		if (READ) name_space_used += item[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,name_space_total); \
	txtpos = ffbin.tellg(); \
	SEEK(ffbin,txtpos,key_name_space_total);

#define MACRO_ITEM_IOFUNCTIONUSABLE(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<usableamount;i++) { \
		IO ## Char(ffbin,usable[i].target_type); \
		IO ## FlexibleShort(ffbin,usable[i].model, useextendedtype); \
		IO ## Char(ffbin,usable[i].target_flag); \
		IO ## FlexibleChar(ffbin,usable[i].effect, useextendedtype); \
		IO ## FlexibleChar(ffbin,usable[i].power, useextendedtype); \
		IO ## Char(ffbin,usable[i].element); \
		IO ## FlexibleChar(ffbin,usable[i].accuracy, useextendedtype); \
		MACRO_IOFUNCTIONGENERIC_STATUS(ffbin, useextendedtype2, IO, READ, usable[i].status) \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONKEY(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<keyitemamount;i++) { \
		IO ## Short(ffbin,key_item[i].name_offset); \
		IO ## Short(ffbin,key_item[i].help_offset); \
		IO ## Short(ffbin,key_item[i].help_size_x); \
		IO ## Short(ffbin,key_item[i].description_offset); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONKEYNAME(IO,SEEK,READ,PPF) \
	SEEK(ffbin,txtpos,0); \
	if (READ) key_name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,key_name_space_total); \
	for (i=0;i<keyitemamount;i++) { \
		SEEK(ffbin,txtpos,key_item[i].name_offset); \
		IO ## FF9String(ffbin,key_item[i].name); \
		if (READ) key_name_space_used += key_item[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,key_name_space_total+ITEM_USABLE_AMOUNT*12+KEY_ITEM_AMOUNT*8);

#define MACRO_ITEM_IOFUNCTIONARMOR(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<armoramount;i++) { \
		IO ## FlexibleChar(ffbin, armor[i].defence, useextendedtype2); \
		IO ## FlexibleChar(ffbin, armor[i].evade, useextendedtype2); \
		IO ## FlexibleChar(ffbin, armor[i].magic_defence, useextendedtype2); \
		IO ## FlexibleChar(ffbin, armor[i].magic_evade, useextendedtype2); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONSTAT(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<statamount;i++) { \
		IO ## Char(ffbin,stat[i].speed); \
		IO ## Char(ffbin,stat[i].strength); \
		IO ## Char(ffbin,stat[i].magic); \
		IO ## Char(ffbin,stat[i].spirit); \
		IO ## Char(ffbin,stat[i].element_immune); \
		IO ## Char(ffbin,stat[i].element_absorb); \
		IO ## Char(ffbin,stat[i].element_half); \
		IO ## Char(ffbin,stat[i].element_weak); \
		IO ## Char(ffbin,stat[i].element_boost); \
		IO ## Char(ffbin,zero8); \
		IO ## Short(ffbin,zero16); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONWEAPONSTAT(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<weaponamount;i++) { \
		IO ## Char(ffbin,weapon[i].flag); \
		IO ## FlexibleChar(ffbin,weapon[i].status, useextendedtype2); \
		IO ## Short(ffbin,weapon[i].model); \
		IO ## FlexibleChar(ffbin,weapon[i].damage_formula, useextendedtype); \
		IO ## FlexibleChar(ffbin,weapon[i].power, useextendedtype); \
		IO ## Char(ffbin,weapon[i].element); \
		IO ## FlexibleChar(ffbin,weapon[i].status_accuracy, useextendedtype); \
		IO ## Short(ffbin,(uint16_t&)weapon[i].offset1); \
		IO ## Short(ffbin,(uint16_t&)weapon[i].offset2); \
		if (READ) weapon[i].UpdateModelName(); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONBATTLEHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	SEEK(ffbin,txtpos,help2_space_total); \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<itemamount;i++) \
		IO ## Short(ffbin,item[i].battle_help_offset); \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,0); \
	if (READ) help2_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,help2_space_total); \
	for (i=0;i<itemamount;i++) { \
		SEEK(ffbin,txtpos,item[i].battle_help_offset); \
		IO ## FF9String(ffbin,item[i].battle_help); \
		if (READ) help2_space_used += item[i].battle_help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help2_space_total+2*itemamount);

#define MACRO_ITEM_IOFUNCTIONHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) help_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,help_space_total); \
	for (i=0;i<itemamount;i++) { \
		SEEK(ffbin,txtpos,item[i].help_offset); \
		IO ## FF9String(ffbin,item[i].help); \
		if (READ) help_space_used += item[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total);

#define MACRO_ITEM_IOFUNCTIONKEYHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) key_help_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,key_help_space_total); \
	for (i=0;i<keyitemamount;i++) { \
		SEEK(ffbin,txtpos,key_item[i].help_offset); \
		IO ## FF9String(ffbin,key_item[i].help); \
		if (READ) key_help_space_used += key_item[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,key_help_space_total);

#define MACRO_ITEM_IOFUNCTIONKEYDESC(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) key_desc_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,key_desc_space_total); \
	for (i=0;i<keyitemamount;i++) { \
		if (key_item[i].description_offset<key_desc_space_total) { \
			SEEK(ffbin,txtpos,key_item[i].description_offset); \
			IO ## FF9String(ffbin,key_item[i].description); \
			if (READ) { \
				key_desc_space_used += key_item[i].description.length; \
				key_item[i].description_out_of_bounds = false; \
			} \
		} else if (READ) { \
			key_item[i].description.CreateEmpty(); \
			key_item[i].description_out_of_bounds = true; \
		} \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,key_desc_space_total);


void ItemDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int itemamount = ITEM_AMOUNT;
	int weaponamount = ITEM_WEAPON_AMOUNT;
	int armoramount = ITEM_ARMOR_AMOUNT;
	int usableamount = ITEM_USABLE_AMOUNT;
	int statamount = ITEM_STAT_AMOUNT;
	int keyitemamount = KEY_ITEM_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i, j;
	uint32_t txtpos;
	csv_header = _(HADES_STRING_CSV_ITEM_HEADER);
	csv_header_weapon = _(HADES_STRING_CSV_WEAPON_HEADER);
	csv_header_armor = _(HADES_STRING_CSV_ARMOR_HEADER);
	csv_header_usable = _(HADES_STRING_CSV_USABLE_HEADER);
	csv_header_stat = _(HADES_STRING_CSV_ITEMSTAT_HEADER);
	csv_format = _(ITEM_CSV_DEFAULT);
	csv_format_weapon = _(WEAPON_CSV_DEFAULT);
	csv_format_armor = _(ARMOR_CSV_DEFAULT);
	csv_format_usable = _(USABLE_CSV_DEFAULT);
	csv_format_stat = _(ITEMSTAT_CSV_DEFAULT);
	name_space_total = config.item_name_space_total;
	help_space_total = config.item_help_space_total;
	help2_space_total = config.item_help_space2_total;
	key_name_space_total = config.item_key_name_space_total;
	key_help_space_total = config.item_key_help_space_total;
	key_desc_space_total = config.item_key_desc_space_total;
	item.resize(ITEM_AMOUNT);
	key_item.resize(KEY_ITEM_AMOUNT);
	usable.resize(ITEM_USABLE_AMOUNT);
	weapon.resize(ITEM_WEAPON_AMOUNT);
	armor.resize(ITEM_ARMOR_AMOUNT);
	stat.resize(ITEM_STAT_AMOUNT);
	for (i = 0; i < ITEM_AMOUNT; i++) {
		item[i].parent = this;
		item[i].id = i;
		item[i].skill.resize(3);
	}
	for (i = 0; i < KEY_ITEM_AMOUNT; i++) {
		key_item[i].parent = this;
		key_item[i].id = i;
	}
	for (i = 0; i < ITEM_USABLE_AMOUNT; i++)
		usable[i].id = i;
	for (i = 0; i < ITEM_WEAPON_AMOUNT; i++) {
		weapon[i].id = i;
		weapon[i].hit_sfx = i;
	}
	for (i = 0; i < ITEM_ARMOR_AMOUNT; i++)
		armor[i].id = i;
	for (i = 0; i < ITEM_STAT_AMOUNT; i++)
		stat[i].id = i;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.item_data_offset);
		MACRO_ITEM_IOFUNCTIONDATA(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONNAME(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONUSABLE(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEY(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEYNAME(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_stat_offset);
		MACRO_ITEM_IOFUNCTIONARMOR(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONSTAT(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_weaponstat_offset);
		MACRO_ITEM_IOFUNCTIONWEAPONSTAT(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_help_offset[0]);
		MACRO_ITEM_IOFUNCTIONBATTLEHELP(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_help_offset[2]);
		MACRO_ITEM_IOFUNCTIONHELP(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEYHELP(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEYDESC(FFIXRead,FFIXSeek,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.item_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.item_name_file[lang]);
			for (i=0;i<ITEM_AMOUNT;i++)
				SteamReadFF9String(ffbin,item[i].name,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.item_help_file[lang]));
			help_space_used = config.meta_res.GetFileSizeByIndex(config.item_help_file[lang]);
			for (i=0;i<ITEM_AMOUNT;i++)
				SteamReadFF9String(ffbin,item[i].help,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.item_help2_file[lang]));
			help2_space_used = config.meta_res.GetFileSizeByIndex(config.item_help2_file[lang]);
			for (i=0;i<ITEM_AMOUNT;i++)
				SteamReadFF9String(ffbin,item[i].battle_help,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.itemkey_name_file[lang]));
			key_name_space_used = config.meta_res.GetFileSizeByIndex(config.itemkey_name_file[lang]);
			for (i=0;i<KEY_ITEM_AMOUNT;i++)
				SteamReadFF9String(ffbin,key_item[i].name,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.itemkey_help_file[lang]));
			key_help_space_used = config.meta_res.GetFileSizeByIndex(config.itemkey_help_file[lang]);
			for (i=0;i<KEY_ITEM_AMOUNT;i++)
				SteamReadFF9String(ffbin,key_item[i].help,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.itemkey_desc_file[lang]));
			key_desc_space_used = config.meta_res.GetFileSizeByIndex(config.itemkey_desc_file[lang]);
			for (i=0;i<KEY_ITEM_AMOUNT;i++)
				SteamReadFF9String(ffbin,key_item[i].description,lang);
		}
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_item_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstit[3] = {
			{ 0x20, ITEM_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9ITEM_DATA") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstit);
		steam_method_position[0] = dlldata.dll_file.tellg();
		uint8_t* rawitemdata = dlldata.ConvertScriptToRaw_Object(ITEM_AMOUNT,14,steam_item_field_size,steam_item_field_array);
		steam_method_base_length[0] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[0];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_item_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstuse[3] = {
			{ 0x1F, ITEM_USABLE_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("ITEM_DATA","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstuse);
		steam_method_position[1] = dlldata.dll_file.tellg();
		uint8_t* rawusabledata = dlldata.ConvertScriptToRaw_Object(ITEM_USABLE_AMOUNT,13,steam_usable_field_size);
		steam_method_base_length[1] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[1];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_armor_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstarmor[3] = {
			{ 0x20, ITEM_ARMOR_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("DEF_PARAMS") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstarmor);
		steam_method_position[2] = dlldata.dll_file.tellg();
		uint8_t* rawarmordata = dlldata.ConvertScriptToRaw_Object(ITEM_ARMOR_AMOUNT,4,steam_armor_field_size);
		steam_method_base_length[2] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[2];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_equip_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinststat[3] = {
			{ 0x20, ITEM_STAT_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("EQUIP_PRIVILEGE","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinststat);
		steam_method_position[3] = dlldata.dll_file.tellg();
		uint8_t* rawitstatdata = dlldata.ConvertScriptToRaw_Object(ITEM_STAT_AMOUNT,9,steam_stat_field_size);
		steam_method_base_length[3] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[3];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_weapon_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstweap[3] = {
			{ 0x1F, ITEM_WEAPON_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("WEAPON") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstweap);
		steam_method_position[4] = dlldata.dll_file.tellg();
		uint8_t* rawweapdata = dlldata.ConvertScriptToRaw_Object(ITEM_WEAPON_AMOUNT,9,steam_weapon_field_size,steam_weapon_field_array);
		steam_method_base_length[4] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[4];
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		ffbin.write((const char*)rawitemdata,0x12*ITEM_AMOUNT);
		ffbin.write((const char*)rawusabledata,0xC*ITEM_USABLE_AMOUNT);
		ffbin.write((const char*)rawarmordata,4*ITEM_ARMOR_AMOUNT);
		ffbin.write((const char*)rawitstatdata,0xC*ITEM_STAT_AMOUNT);
		ffbin.write((const char*)rawweapdata,0xC*ITEM_WEAPON_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_ITEM_IOFUNCTIONDATA(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONUSABLE(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONARMOR(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONSTAT(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONWEAPONSTAT(SteamRead,SteamSeek,true,false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawitemdata;
		delete[] rawusabledata;
		delete[] rawarmordata;
		delete[] rawitstatdata;
		delete[] rawweapdata;
	}
	for (i=0;i<ITEM_WEAPON_AMOUNT;i++) {
		item[i].weapon_id = i;
		item[i].armor_id = -1;
		item[i].usable_id = -1;
	}
	for (i=0;i<ITEM_ARMOR_AMOUNT;i++) {
		item[i+ITEM_WEAPON_AMOUNT].weapon_id = -1;
		item[i+ITEM_WEAPON_AMOUNT].armor_id = i;
		item[i+ITEM_WEAPON_AMOUNT].usable_id = -1;
	}
	for (i=0;i<ITEM_USABLE_AMOUNT;i++) {
		item[i+ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT].weapon_id = -1;
		item[i+ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT].armor_id = -1;
		item[i+ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT].usable_id = i;
	}
}

DllMetaDataModification* ItemDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[5];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t*[ITEM_AMOUNT];
	unsigned int i;
	for (i = 0; i < ITEM_AMOUNT; i++) {
		argvalue[i] = new uint32_t[14];
		argvalue[i][0] = item[i].name_offset;
		argvalue[i][1] = item[i].help_offset;
		argvalue[i][2] = item[i].price;
		argvalue[i][3] = item[i].char_availability;
		argvalue[i][4] = item[i].icon;
		argvalue[i][5] = item[i].icon_color;
		argvalue[i][6] = item[i].equip_position;
		argvalue[i][7] = item[i].stat_id;
		argvalue[i][8] = item[i].skill[0].GetRawId();
		argvalue[i][9] = item[i].skill[1].GetRawId();
		argvalue[i][10] = item[i].skill[2].GetRawId();
		argvalue[i][11] = item[i].type;
		argvalue[i][12] = item[i].menu_position;
		argvalue[i][13] = item[i].zero;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position[0], steam_method_base_length[0], ITEM_AMOUNT, 14, steam_item_field_size, steam_item_field_array);
	for (i = 0; i < ITEM_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_USABLE_AMOUNT];
	for (i = 0; i < ITEM_USABLE_AMOUNT; i++) {
		argvalue[i] = new uint32_t[13];
		argvalue[i][0] = usable[i].target_type & 0xF;
		argvalue[i][1] = (usable[i].target_type >> 4) & 0x1;
		argvalue[i][2] = (usable[i].target_type >> 5) & 0x7;
		argvalue[i][3] = usable[i].model & 0x1FF;
		argvalue[i][4] = usable[i].GetSound();
		argvalue[i][5] = (usable[i].target_flag >> 5) & 0x1;
		argvalue[i][6] = (usable[i].target_flag >> 6) & 0x1;
		argvalue[i][7] = (usable[i].target_flag >> 7) & 0x1;
		argvalue[i][8] = usable[i].effect;
		argvalue[i][9] = usable[i].power;
		argvalue[i][10] = usable[i].element;
		argvalue[i][11] = usable[i].accuracy;
		argvalue[i][12] = GetStatusBitList(usable[i].status);
	}
	res[1] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position[1], steam_method_base_length[1], ITEM_USABLE_AMOUNT, 13, steam_usable_field_size);
	for (i = 0; i < ITEM_USABLE_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_ARMOR_AMOUNT];
	for (i = 0; i < ITEM_ARMOR_AMOUNT; i++) {
		argvalue[i] = new uint32_t[4];
		argvalue[i][0] = armor[i].defence;
		argvalue[i][1] = armor[i].evade;
		argvalue[i][2] = armor[i].magic_defence;
		argvalue[i][3] = armor[i].magic_evade;
	}
	res[2] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position[2], steam_method_base_length[2], ITEM_ARMOR_AMOUNT, 4, steam_armor_field_size);
	for (i = 0; i < ITEM_ARMOR_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_STAT_AMOUNT];
	for (i = 0; i < ITEM_STAT_AMOUNT; i++) {
		argvalue[i] = new uint32_t[9];
		argvalue[i][0] = stat[i].speed;
		argvalue[i][1] = stat[i].strength;
		argvalue[i][2] = stat[i].magic;
		argvalue[i][3] = stat[i].spirit;
		argvalue[i][4] = stat[i].element_immune;
		argvalue[i][5] = stat[i].element_absorb;
		argvalue[i][6] = stat[i].element_half;
		argvalue[i][7] = stat[i].element_weak;
		argvalue[i][8] = stat[i].element_boost;
	}
	res[3] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position[3], steam_method_base_length[3], ITEM_STAT_AMOUNT, 9, steam_stat_field_size);
	for (i = 0; i < ITEM_STAT_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_WEAPON_AMOUNT];
	for (i = 0; i < ITEM_WEAPON_AMOUNT; i++) {
		argvalue[i] = new uint32_t[9];
		argvalue[i][0] = weapon[i].flag;
		argvalue[i][1] = weapon[i].status;
		argvalue[i][2] = weapon[i].model;
		argvalue[i][3] = weapon[i].damage_formula;
		argvalue[i][4] = weapon[i].power;
		argvalue[i][5] = weapon[i].element;
		argvalue[i][6] = weapon[i].status_accuracy;
		argvalue[i][7] = (uint16_t)weapon[i].offset1;
		argvalue[i][8] = (uint16_t)weapon[i].offset2;
	}
	res[4] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position[4], steam_method_base_length[4], ITEM_WEAPON_AMOUNT, 9, steam_weapon_field_size, steam_weapon_field_array);
	for (i = 0; i < ITEM_WEAPON_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	*modifamount = 5;
	return res;
}

void ItemDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i;
	stringstream itemdb;
	itemdb << "// Method: ff9item::.cctor\n\n";
	itemdb << "\tff9item._FF9Item_Data = new FF9ITEM_DATA[] {\n";
	for (i = 0; i < ITEM_AMOUNT; i++)
		itemdb	<< "\t\tnew FF9ITEM_DATA(" << (int)item[i].name_offset << ", " << (int)item[i].help_offset << ", " << (int)item[i].price << ", " << StreamAsHex(item[i].char_availability) << ", " << (int)item[i].icon << ", " << (int)item[i].icon_color << ", " << (int)item[i].equip_position << ", " << (int)item[i].stat_id
				<< ", new byte[]{ " << (int)item[i].skill[0].GetRawId() << ", " << (int)item[i].skill[1].GetRawId() << ", " << (int)item[i].skill[2].GetRawId() << " }, " << StreamAsHex(item[i].type) << ", " << (int)item[i].menu_position << ", " << (int)item[i].zero << (i+1==ITEM_AMOUNT ? ")" : "),") << " // " << ConvertWStrToStr(item[i].name.str_nice) << "\n";
	itemdb << "\t};\n";
	itemdb << "\tff9item._FF9Item_Info = new ITEM_DATA[] {\n";
	for (i = 0; i < ITEM_USABLE_AMOUNT; i++)
		itemdb	<< "\t\tnew ITEM_DATA(new CMD_INFO(" << (int)(usable[i].target_type & 0xF) << ", " << (int)((usable[i].target_type >> 4) & 0x1) << ", " << (int)((usable[i].target_type >> 5) & 0x7) << ", " << (int)(usable[i].model & 0x1FF) << ", " << (int)usable[i].GetSound() << ", " << (int)((usable[i].target_flag >> 5) & 0x1) << ", " << (int)((usable[i].target_flag >> 6) & 0x1) << ", " << (int)((usable[i].target_flag >> 7) & 0x1)
				<< "), new BTL_REF(" << (int)usable[i].effect << ", " << (int)usable[i].power << ", " << StreamAsHex(usable[i].element) << ", " << (int)usable[i].accuracy
				 << "), " << StreamAsHex(GetStatusBitList(usable[i].status)) << (i+1==ITEM_USABLE_AMOUNT ? "u)" : "u),") << " // " << ConvertWStrToStr(item[ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT+i].name.str_nice) << "\n";
	itemdb << "\t};\n";
	buffer.push_back(itemdb.str());
	stringstream weapondb;
	weapondb << "// Method: FF9.ff9weap::.cctor\n\n";
	weapondb << "\tff9weap._FF9Weapon_Data = new WEAPON[] {\n";
	for (i = 0; i < ITEM_WEAPON_AMOUNT; i++) {
		string modelsteamid = weapon[i].model == 0 ? "null" : "\"" + ConvertWStrToStr(weapon[i].model_name) + "\"";
		weapondb	<< "\t\tnew WEAPON(" << StreamAsHex(weapon[i].flag) << ", " << (int)weapon[i].status << ", " << modelsteamid << ", new BTL_REF(" << (int)weapon[i].damage_formula << ", " << (int)weapon[i].power << ", " << StreamAsHex(weapon[i].element) << ", " << (int)weapon[i].status_accuracy
					<< "), new short[]{ " << (short)weapon[i].offset1 << ", " << (short)weapon[i].offset2 << (i+1==ITEM_WEAPON_AMOUNT ? " })" : " }),") << " // " << ConvertWStrToStr(item[i].name.str_nice) << "\n";
	}
	weapondb << "\t};\n";
	buffer.push_back(weapondb.str());
	stringstream armordb;
	armordb << "// Method: FF9.ff9armor::.cctor\n\n";
	armordb << "\tff9armor._FF9Armor_Data = new DEF_PARAMS[] {\n";
	for (i = 0; i < ITEM_ARMOR_AMOUNT; i++)
		armordb << "\t\tnew DEF_PARAMS(" << (int)armor[i].defence << ", " << (int)armor[i].evade << ", " << (int)armor[i].magic_defence << ", " << (int)armor[i].magic_evade << (i+1==ITEM_ARMOR_AMOUNT ? ")" : "),") << " // " << ConvertWStrToStr(item[ITEM_WEAPON_AMOUNT+i].name.str_nice) << "\n";
	armordb << "\t};\n";
	buffer.push_back(armordb.str());
	stringstream itemstatdb;
	itemstatdb << "// Method: FF9.ff9equip::.cctor\n\n";
	itemstatdb << "\tff9equip._FF9EquipBonus_Data = new EQUIP_PRIVILEGE[] {\n";
	for (i = 0; i < ITEM_STAT_AMOUNT; i++)
		itemstatdb	<< "\t\tnew EQUIP_PRIVILEGE(" << (int)stat[i].speed << ", " << (int)stat[i].strength << ", " << (int)stat[i].magic << ", " << (int)stat[i].spirit
					<< ", new DEF_ATTR(" << StreamAsHex(stat[i].element_immune) << ", " << StreamAsHex(stat[i].element_absorb) << ", " << StreamAsHex(stat[i].element_half) << ", " << StreamAsHex(stat[i].element_weak)
					<< "), " << StreamAsHex(stat[i].element_boost) << (i+1==ITEM_STAT_AMOUNT ? ")\n" : "),\n");
	itemstatdb << "\t};\n";
	buffer.push_back(itemstatdb.str());
}

bool ItemDataSet::GetEquipPositionFloat(int itemindex, float step, float* position) {
	if (item[itemindex].equip_position_type == ITEM_POSITION_ABSOLUTE) {
		*position = item[itemindex].equip_position;
		return true;
	}
	set<int> safetycheck;
	int nextid = item[itemindex].equip_position;
	float delta = item[itemindex].equip_position_type == ITEM_POSITION_AFTER ? step :
		item[itemindex].equip_position_type == ITEM_POSITION_BEFORE ? -step :
		0.0f;
	while (true) {
		if (safetycheck.count(nextid) > 0)
			return false;
		safetycheck.insert(nextid);
		ItemDataStruct& nextit = GetItemById(nextid);
		if (nextit.id < 0)
			return false;
		if (nextit.equip_position_type == ITEM_POSITION_ABSOLUTE) {
			*position = (float)nextit.equip_position + delta;
			return true;
		}
		if (nextit.equip_position_type == ITEM_POSITION_AFTER)
			delta += step;
		else if (nextit.equip_position_type == ITEM_POSITION_BEFORE)
			delta -= step;
		nextid = nextit.equip_position;
	}
}

bool ItemDataSet::GetMenuPositionFloat(int itemindex, float step, float* position) {
	if (item[itemindex].menu_position_type == ITEM_POSITION_ABSOLUTE) {
		*position = item[itemindex].menu_position;
		return true;
	}
	set<int> safetycheck;
	int nextid = item[itemindex].menu_position;
	float delta = item[itemindex].menu_position_type == ITEM_POSITION_AFTER ? step :
		item[itemindex].menu_position_type == ITEM_POSITION_BEFORE ? -step :
		0.0f;
	while (true) {
		if (safetycheck.count(nextid) > 0)
			return false;
		safetycheck.insert(nextid);
		ItemDataStruct& nextit = GetItemById(nextid);
		if (nextit.id < 0)
			return false;
		if (nextit.menu_position_type == ITEM_POSITION_ABSOLUTE) {
			*position = (float)nextit.menu_position + delta;
			return true;
		}
		if (nextit.menu_position_type == ITEM_POSITION_AFTER)
			delta += step;
		else if (nextit.menu_position_type == ITEM_POSITION_BEFORE)
			delta -= step;
		nextid = nextit.menu_position;
	}
}

bool ItemDataSet::GenerateCSV(string basefolder) {
	vector<ItemDataStruct> weaponmap;
	vector<ItemDataStruct> armormap;
	vector<ItemDataStruct> usablemap;
	vector<ItemDataStruct> statmap;
	unsigned int i, j;
	for (i = 0; i < item.size(); i++) {
		if (item[i].weapon_id >= 0)		weaponmap.push_back(item[i]);
		if (item[i].armor_id >= 0)		armormap.push_back(item[i]);
		if (item[i].usable_id >= 0)		usablemap.push_back(item[i]);
		if (item[i].stat_id >= 0)		statmap.push_back(item[i]);
	}
	std::sort(statmap.begin(), statmap.end(), [](ItemDataStruct& a, ItemDataStruct& b) { return a.stat_id > b.stat_id; });
	for (i = 1; i < statmap.size(); i++) {
		if (statmap[i].stat_id == statmap[i - 1].stat_id) {
			statmap.erase(statmap.begin() + i);
			i--;
		}
	}
	if (!MemoriaUtility::GenerateDatabaseGeneric<ItemDataStruct>(_(basefolder), _(HADES_STRING_CSV_ITEM_FILE), csv_header, _(L"\n"), _(L"\n"), item, csv_format, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<ItemDataStruct>(_(basefolder), _(HADES_STRING_CSV_WEAPON_FILE), csv_header_weapon, _(L"\n"), _(L"\n"), weaponmap, csv_format_weapon, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<ItemDataStruct>(_(basefolder), _(HADES_STRING_CSV_ARMOR_FILE), csv_header_armor, _(L"\n"), _(L"\n"), armormap, csv_format_armor, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<ItemDataStruct>(_(basefolder), _(HADES_STRING_CSV_USABLE_FILE), csv_header_usable, _(L"\n"), _(L"\n"), usablemap, csv_format_usable, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<ItemDataStruct>(_(basefolder), _(HADES_STRING_CSV_ITEMSTAT_FILE), csv_header_stat, _(L"\n"), _(L"\n"), statmap, csv_format_stat, true))
		return false;
	if (hades::PREFER_EXPORT_AS_PATCHES && GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_SPELL] && GetGameSaveSet()->sectionloaded[DATA_SECTION_SUPPORT]) {
		SupportDataSet* supportset = GetGameSaveSet()->supportset;
		SpellDataSet* spellset = GetGameSaveSet()->spellset;
		wxString skillpatchstr = _(L"");
		set<int> releventabil;
		for (i = 0; i < item.size(); i++)
			for (j = 0; j < item[i].skill.size(); j++)
				if (item[i].skill[j].GetRawId() >= 256)
					releventabil.insert(item[i].skill[j].GetRawId());
		for (auto it = releventabil.begin(); it != releventabil.end(); it++) {
			vector<ItemDataStruct*> teaching;
			AnyAbilityStruct abil;
			abil.Setup(*it);
			skillpatchstr += wxString::Format(wxT("// %s: "), abil.is_active ? spellset->GetSpellById(abil.id).name.str : supportset->GetSupportById(abil.id).name.str);
			for (i = 0; i < item.size(); i++)
				for (j = 0; j < item[i].skill.size(); j++)
					if (item[i].skill[j] == abil)
						teaching.push_back(&item[i]);
			for (i = 0; i < teaching.size(); i++) {
				if (i > 0)
					skillpatchstr += _(L", ");
				skillpatchstr += teaching[i]->name.str;
			}
			skillpatchstr += wxString::Format(wxT("\n%s Add"), abil.GetStringId());
			for (i = 0; i < teaching.size(); i++)
				skillpatchstr += wxString::Format(wxT(" %d"), teaching[i]->id);
			skillpatchstr += _(L"\n\n");
		}
		if (!skillpatchstr.IsEmpty()) {
			wxFile skilltxtfile;
			if (!skilltxtfile.Open(_(basefolder) + _(HADES_STRING_SKILL_PATCH_FILE), wxFile::write))
				return false;
			if (!skilltxtfile.Write(skillpatchstr))
				return false;
			skilltxtfile.Close();
		}
	}
	return true;
}

void ItemDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype, bool onlymodified, bool asmes, SteamLanguage lang) {
	vector<int> writesubset;

	#define MACRO_WRITE_STEAM_TEXT(ITTYPE, ITLIST, ITFILE, ITTXT) \
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<ITTYPE>(&writesubset, GetGameConfiguration()->ITFILE, ITLIST, [lang](ITTYPE& it) { return it.ITTXT.multi_lang_str[lang]; }, lang)) \
			WriteSteamTextGeneric(ffbin, ITLIST, &ITTYPE::ITTXT, &writesubset, asmes, lang); \
		else \
			WriteSteamTextGeneric(ffbin, ITLIST, &ITTYPE::ITTXT, NULL, asmes, lang);

	if (texttype == 0) {
		MACRO_WRITE_STEAM_TEXT(ItemDataStruct, item, item_name_file, name)
	} else if (texttype == 1) {
		MACRO_WRITE_STEAM_TEXT(ItemDataStruct, item, item_help_file, help)
	} else if (texttype == 2) {
		MACRO_WRITE_STEAM_TEXT(ItemDataStruct, item, item_help2_file, battle_help)
	} else if (texttype == 3) {
		MACRO_WRITE_STEAM_TEXT(KeyItemDataStruct, key_item, itemkey_name_file, name)
	} else if (texttype == 4) {
		MACRO_WRITE_STEAM_TEXT(KeyItemDataStruct, key_item, itemkey_help_file, help)
	} else {
		MACRO_WRITE_STEAM_TEXT(KeyItemDataStruct, key_item, itemkey_desc_file, description)
	}
}

void ItemDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	int itemamount = ITEM_AMOUNT;
	int weaponamount = ITEM_WEAPON_AMOUNT;
	int armoramount = ITEM_ARMOR_AMOUNT;
	int usableamount = ITEM_USABLE_AMOUNT;
	int statamount = ITEM_STAT_AMOUNT;
	int keyitemamount = KEY_ITEM_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i, j;
	uint32_t txtpos;
	UpdateOffset();
	ffbin.seekg(config.item_data_offset);
	MACRO_ITEM_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONUSABLE(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEY(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEYNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_stat_offset);
	MACRO_ITEM_IOFUNCTIONARMOR(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONSTAT(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_weaponstat_offset);
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[0]);
	MACRO_ITEM_IOFUNCTIONBATTLEHELP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[1]);
	MACRO_ITEM_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[2]);
	MACRO_ITEM_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEYHELP(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEYDESC(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[3]);
	MACRO_ITEM_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
}

void ItemDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	int itemamount = ITEM_AMOUNT;
	int weaponamount = ITEM_WEAPON_AMOUNT;
	int armoramount = ITEM_ARMOR_AMOUNT;
	int usableamount = ITEM_USABLE_AMOUNT;
	int statamount = ITEM_STAT_AMOUNT;
	int keyitemamount = KEY_ITEM_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i, j;
	uint32_t txtpos;
	UpdateOffset();
	ffbin.seekg(config.item_data_offset);
	MACRO_ITEM_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONUSABLE(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEY(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEYNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_stat_offset);
	MACRO_ITEM_IOFUNCTIONARMOR(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONSTAT(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_weaponstat_offset);
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[0]);
	MACRO_ITEM_IOFUNCTIONBATTLEHELP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[1]);
	MACRO_ITEM_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[2]);
	MACRO_ITEM_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEYHELP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEYDESC(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[3]);
	MACRO_ITEM_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
}

int ItemDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	int itemamount = ITEM_AMOUNT;
	int weaponamount = ITEM_WEAPON_AMOUNT;
	int armoramount = ITEM_ARMOR_AMOUNT;
	int usableamount = ITEM_USABLE_AMOUNT;
	int statamount = ITEM_STAT_AMOUNT;
	int keyitemamount = KEY_ITEM_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	vector<ItemDataStruct> nonmodifieditem;
	vector<ItemWeaponDataStruct> nonmodifiedweapon;
	vector<ItemArmorDataStruct> nonmodifiedarmor;
	vector<ItemUsableDataStruct> nonmodifiedusable;
	vector<ItemStatDataStruct> nonmodifiedstat;
	vector<KeyItemDataStruct> nonmodifiedkeyitem;
	int i, j;
	uint32_t txtpos;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total, helpsize = help_space_total, helpsize2 = help2_space_total;
	uint16_t knamesize = key_name_space_total, khelpsize = key_help_space_total, kdescsize = key_desc_space_total;
	HWSReadShort(ffbin, version);
	HWSReadShort(ffbin, name_space_total);
	HWSReadShort(ffbin, help_space_total);
	HWSReadShort(ffbin, help2_space_total);
	HWSReadShort(ffbin, key_name_space_total);
	HWSReadShort(ffbin, key_help_space_total);
	HWSReadShort(ffbin, key_desc_space_total);
	if (version >= 2) {
		useextendedtype = true;
		vector<int> added;
		itemamount = PrepareHWSFlexibleList(ffbin, item, nonmodifieditem, added);
		for (i = 0; i < (int)added.size(); i++) {
			item[added[i]].name.CreateEmpty(true);
			item[added[i]].help.CreateEmpty(true);
			item[added[i]].battle_help.CreateEmpty(true);
			item[added[i]].parent = this;
		}
		weaponamount = PrepareHWSFlexibleList(ffbin, weapon, nonmodifiedweapon, added);
		armoramount = PrepareHWSFlexibleList(ffbin, armor, nonmodifiedarmor, added);
		usableamount = PrepareHWSFlexibleList(ffbin, usable, nonmodifiedusable, added);
		statamount = PrepareHWSFlexibleList(ffbin, stat, nonmodifiedstat, added);
		keyitemamount = PrepareHWSFlexibleList(ffbin, key_item, nonmodifiedkeyitem, added);
		for (i = 0; i < (int)added.size(); i++) {
			key_item[added[i]].name.CreateEmpty(true);
			key_item[added[i]].help.CreateEmpty(true);
			key_item[added[i]].description.CreateEmpty(true);
			key_item[added[i]].parent = this;
		}
	}
	if (version >= 3) {
		useextendedtype2 = true;
		HWSReadCSVFormatting(ffbin, csv_header, csv_format);
		HWSReadCSVFormatting(ffbin, csv_header_weapon, csv_format_weapon);
		HWSReadCSVFormatting(ffbin, csv_header_armor, csv_format_armor);
		HWSReadCSVFormatting(ffbin, csv_header_usable, csv_format_usable);
		HWSReadCSVFormatting(ffbin, csv_header_stat, csv_format_stat);
		HWSReadCSVFields(ffbin, custom_field);
	}
	MACRO_ITEM_IOFUNCTIONDATA(HWSRead, HWSSeek, true, false)
	if (GetHWSGameType() == GAME_TYPE_PSX) {
		if (name_space_total <= namesize && usetext) {
			MACRO_ITEM_IOFUNCTIONNAME(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < itemamount; i++)
					item[i].name.PSXToSteam();
		} else {
			ffbin.seekg(name_space_total, ios::cur);
			txtpos = ffbin.tellg();
			ffbin.seekg(key_name_space_total, ios::cur);
			if (usetext)
				res |= 0x1;
		}
	}
	MACRO_ITEM_IOFUNCTIONUSABLE(HWSRead, HWSSeek, true, false)
	MACRO_ITEM_IOFUNCTIONKEY(HWSRead, HWSSeek, true, false)
	if (GetHWSGameType() == GAME_TYPE_PSX) {
		if (key_name_space_total <= knamesize && usetext) {
			MACRO_ITEM_IOFUNCTIONKEYNAME(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < keyitemamount; i++)
					key_item[i].name.PSXToSteam();
		} else if (usetext) {
			res |= 0x8;
		}
	}
	MACRO_ITEM_IOFUNCTIONARMOR(HWSRead, HWSSeek, true, false)
	MACRO_ITEM_IOFUNCTIONSTAT(HWSRead, HWSSeek, true, false)
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(HWSRead, HWSSeek, true, false)
	if (GetHWSGameType() == GAME_TYPE_PSX) {
		if (help_space_total <= helpsize && usetext) {
			MACRO_ITEM_IOFUNCTIONHELP(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < itemamount; i++)
					item[i].help.PSXToSteam();
		} else {
			ffbin.seekg(help_space_total, ios::cur);
			if (usetext)
				res |= 0x2;
		}
		if (help2_space_total <= helpsize2 && usetext) {
			MACRO_ITEM_IOFUNCTIONBATTLEHELP(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < itemamount; i++)
					item[i].battle_help.PSXToSteam();
		} else {
			ffbin.seekg(help2_space_total + 2 * itemamount, ios::cur);
			if (usetext)
				res |= 0x4;
		}
		if (key_help_space_total <= khelpsize && usetext) {
			MACRO_ITEM_IOFUNCTIONKEYHELP(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < keyitemamount; i++)
					key_item[i].help.PSXToSteam();
		} else {
			ffbin.seekg(key_help_space_total, ios::cur);
			if (usetext)
				res |= 0x10;
		}
		if (key_desc_space_total <= kdescsize && usetext) {
			MACRO_ITEM_IOFUNCTIONKEYDESC(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < keyitemamount; i++)
					key_item[i].description.PSXToSteam();
		} else {
			ffbin.seekg(key_desc_space_total, ios::cur);
			if (usetext)
				res |= 0x20;
		}
	}
	if (version >= 2) {
		for (i = 0; i < weaponamount; i++) {
			int texturecount;
			HWSReadWString(ffbin, weapon[i].model_name);
			HWSReadFlexibleChar(ffbin, weapon[i].hit_sfx, true);
			HWSReadFlexibleChar(ffbin, texturecount, true);
			weapon[i].texture_names.resize(texturecount);
			for (j = 0; j < texturecount; j++)
				HWSReadWString(ffbin, weapon[i].texture_names[j]);
		}
	}
	for (i = 0; i < (int)nonmodifieditem.size(); i++)
		InsertAtId(item, nonmodifieditem[i], nonmodifieditem[i].id);
	for (i = 0; i < (int)nonmodifiedweapon.size(); i++)
		InsertAtId(weapon, nonmodifiedweapon[i], nonmodifiedweapon[i].id);
	for (i = 0; i < (int)nonmodifiedarmor.size(); i++)
		InsertAtId(armor, nonmodifiedarmor[i], nonmodifiedarmor[i].id);
	for (i = 0; i < (int)nonmodifiedusable.size(); i++)
		InsertAtId(usable, nonmodifiedusable[i], nonmodifiedusable[i].id);
	for (i = 0; i < (int)nonmodifiedstat.size(); i++)
		InsertAtId(stat, nonmodifiedstat[i], nonmodifiedstat[i].id);
	for (i = 0; i < (int)nonmodifiedkeyitem.size(); i++)
		InsertAtId(key_item, nonmodifiedkeyitem[i], nonmodifiedkeyitem[i].id);
	name_space_total = namesize;
	help_space_total = helpsize;
	help2_space_total = helpsize2;
	key_name_space_total = knamesize;
	key_help_space_total = khelpsize;
	key_desc_space_total = kdescsize;
	if (GetHWSGameType() != GAME_TYPE_PSX) {
		SteamLanguage lg;
		int txtspace;
		uint32_t tmppos;

		#define MACRO_ITEM_HWSSTEAMTEXT(STR, AMT) \
			HWSReadChar(ffbin, lg); \
			while (lg != STEAM_LANGUAGE_NONE) { \
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype); \
				tmppos = ffbin.tellg(); \
				if (usetext) { \
					if (GetGameType() != GAME_TYPE_PSX) \
						for (i = 0; i < AMT; i++) \
							SteamReadFF9String(ffbin, STR, lg); \
					else if (lg == GetSteamLanguage()) \
						for (i = 0; i < AMT; i++) { \
							SteamReadFF9String(ffbin, STR); \
							STR.SteamToPSX(); \
						} \
				} \
				ffbin.seekg(tmppos + txtspace); \
				HWSReadChar(ffbin, lg); \
			}

		MACRO_ITEM_HWSSTEAMTEXT(item[i].name, itemamount)
		MACRO_ITEM_HWSSTEAMTEXT(item[i].help, itemamount)
		MACRO_ITEM_HWSSTEAMTEXT(item[i].battle_help, itemamount)
		MACRO_ITEM_HWSSTEAMTEXT(key_item[i].name, keyitemamount)
		MACRO_ITEM_HWSSTEAMTEXT(key_item[i].help, keyitemamount)
		MACRO_ITEM_HWSSTEAMTEXT(key_item[i].description, keyitemamount)
	}
	UpdateOffset();
	return res;
}

void ItemDataSet::WriteHWS(fstream& ffbin) {
	int itemamount = item.size();
	int weaponamount = weapon.size();
	int armoramount = armor.size();
	int usableamount = usable.size();
	int statamount = stat.size();
	int keyitemamount = key_item.size();
	bool useextendedtype = true;
	bool useextendedtype2 = true;
	int i, j;
	uint32_t txtpos;
	UpdateOffset();
	HWSWriteShort(ffbin, ITEM_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total, helpsize2 = help2_space_total;
	uint16_t knamesize = key_name_space_total, khelpsize = key_help_space_total, kdescsize = key_desc_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	help2_space_total = help2_space_used;
	key_name_space_total = key_name_space_used;
	key_help_space_total = key_help_space_used;
	key_desc_space_total = key_desc_space_used;
	HWSWriteShort(ffbin, name_space_total);
	HWSWriteShort(ffbin, help_space_total);
	HWSWriteShort(ffbin, help2_space_total);
	HWSWriteShort(ffbin, key_name_space_total);
	HWSWriteShort(ffbin, key_help_space_total);
	HWSWriteShort(ffbin, key_desc_space_total);
	HWSWriteFlexibleChar(ffbin, itemamount, true);
	for (i = 0; i < itemamount; i++)
		HWSWriteFlexibleChar(ffbin, item[i].id, true);
	HWSWriteFlexibleChar(ffbin, weaponamount, true);
	for (i = 0; i < weaponamount; i++)
		HWSWriteFlexibleChar(ffbin, weapon[i].id, true);
	HWSWriteFlexibleChar(ffbin, armoramount, true);
	for (i = 0; i < armoramount; i++)
		HWSWriteFlexibleChar(ffbin, armor[i].id, true);
	HWSWriteFlexibleChar(ffbin, usableamount, true);
	for (i = 0; i < usableamount; i++)
		HWSWriteFlexibleChar(ffbin, usable[i].id, true);
	HWSWriteFlexibleChar(ffbin, statamount, true);
	for (i = 0; i < statamount; i++)
		HWSWriteFlexibleChar(ffbin, stat[i].id, true);
	HWSWriteFlexibleChar(ffbin, keyitemamount, true);
	for (i = 0; i < keyitemamount; i++)
		HWSWriteFlexibleChar(ffbin, key_item[i].id, true);
	HWSWriteCSVFormatting(ffbin, csv_header, csv_format);
	HWSWriteCSVFormatting(ffbin, csv_header_weapon, csv_format_weapon);
	HWSWriteCSVFormatting(ffbin, csv_header_armor, csv_format_armor);
	HWSWriteCSVFormatting(ffbin, csv_header_usable, csv_format_usable);
	HWSWriteCSVFormatting(ffbin, csv_header_stat, csv_format_stat);
	HWSWriteCSVFields(ffbin, custom_field);
	MACRO_ITEM_IOFUNCTIONDATA(HWSWrite, HWSSeek, false, false)
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_ITEM_IOFUNCTIONNAME(HWSWrite, HWSSeek, false, false)
	}
	MACRO_ITEM_IOFUNCTIONUSABLE(HWSWrite, HWSSeek, false, false)
	MACRO_ITEM_IOFUNCTIONKEY(HWSWrite, HWSSeek, false, false)
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_ITEM_IOFUNCTIONKEYNAME(HWSWrite, HWSSeek, false, false)
	}
	MACRO_ITEM_IOFUNCTIONARMOR(HWSWrite, HWSSeek, false, false)
	MACRO_ITEM_IOFUNCTIONSTAT(HWSWrite, HWSSeek, false, false)
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(HWSWrite, HWSSeek, false, false)
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_ITEM_IOFUNCTIONHELP(HWSWrite, HWSSeek, false, false)
		MACRO_ITEM_IOFUNCTIONBATTLEHELP(HWSWrite, HWSSeek, false, false)
		MACRO_ITEM_IOFUNCTIONKEYHELP(HWSWrite, HWSSeek, false, false)
		MACRO_ITEM_IOFUNCTIONKEYDESC(HWSWrite, HWSSeek, false, false)
	}
	for (i = 0; i < (int)weapon.size(); i++) {
		HWSWriteWString(ffbin, weapon[i].model_name);
		HWSWriteFlexibleChar(ffbin, weapon[i].hit_sfx, true);
		HWSWriteFlexibleChar(ffbin, weapon[i].texture_names.size(), true);
		for (j = 0; j < (int)weapon[i].texture_names.size(); j++)
			HWSWriteWString(ffbin, weapon[i].texture_names[j]);
	}
	name_space_total = namesize;
	help_space_total = helpsize;
	help2_space_total = helpsize2;
	key_name_space_total = knamesize;
	key_help_space_total = khelpsize;
	key_desc_space_total = kdescsize;
	if (GetGameType() != GAME_TYPE_PSX) {
		SteamLanguage lg;
		function<int()> textsizegetter[] = {
			[&]() { return GetSteamTextSizeGeneric(item, &ItemDataStruct::name, false, lg); },
			[&]() { return GetSteamTextSizeGeneric(item, &ItemDataStruct::help, false, lg); },
			[&]() { return GetSteamTextSizeGeneric(item, &ItemDataStruct::battle_help, false, lg); },
			[&]() { return GetSteamTextSizeGeneric(key_item, &KeyItemDataStruct::name, false, lg); },
			[&]() { return GetSteamTextSizeGeneric(key_item, &KeyItemDataStruct::help, false, lg); },
			[&]() { return GetSteamTextSizeGeneric(key_item, &KeyItemDataStruct::description, false, lg); }
		};
		for (i = 0; i < 6; i++) {
			for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
				if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
					HWSWriteChar(ffbin, lg);
					HWSWriteFlexibleShort(ffbin, textsizegetter[i](), true);
					WriteSteamText(ffbin, i, false, false, lg);
				}
			}
			HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
		}
	}
}

void ItemDataSet::UpdateOffset() {
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	uint16_t j = 0, k = 0, l = 0;
	int i;
	for (i = 0; i < ITEM_AMOUNT; i++) {
		item[i].name_offset = j;
		j += item[i].name.length;
		item[i].help_offset = k;
		k += item[i].help.length;
		item[i].battle_help_offset = l;
		l += item[i].battle_help.length;
	}
	name_space_used = j;
	help_space_used = k;
	help2_space_used = l;
	j = 0;
	k = 0;
	l = 0;
	for (i = 0; i < KEY_ITEM_AMOUNT; i++) {
		key_item[i].name_offset = j;
		j += key_item[i].name.length;
		key_item[i].help_offset = k;
		k += key_item[i].help.length;
		if (key_item[i].description_out_of_bounds)
			key_item[i].description_offset = key_desc_space_total;
		else {
			key_item[i].description_offset = l;
			l += key_item[i].description.length;
		}
	}
	key_name_space_used = j;
	key_help_space_used = k;
	key_desc_space_used = l;
}

int ItemDataSet::GetItemIndexById(int itemid) {
	if (itemid < ITEM_AMOUNT)
		return itemid;
	for (unsigned int i = ITEM_AMOUNT; i < item.size(); i++)
		if (item[i].id == itemid)
			return i;
	return -1;
}

int ItemDataSet::GetKeyItemIndexById(int keyitemid) {
	if (keyitemid < KEY_ITEM_AMOUNT)
		return keyitemid;
	for (unsigned int i = KEY_ITEM_AMOUNT; i < key_item.size(); i++)
		if (key_item[i].id == keyitemid)
			return i;
	return -1;
}

int ItemDataSet::GetUsableIndexById(int usableid) {
	if (usableid < ITEM_USABLE_AMOUNT)
		return usableid;
	for (unsigned int i = ITEM_USABLE_AMOUNT; i < usable.size(); i++)
		if (usable[i].id == usableid)
			return i;
	return -1;
}

int ItemDataSet::GetWeaponIndexById(int weaponid) {
	if (weaponid < ITEM_WEAPON_AMOUNT)
		return weaponid;
	for (unsigned int i = ITEM_WEAPON_AMOUNT; i < weapon.size(); i++)
		if (weapon[i].id == weaponid)
			return i;
	return -1;
}

int ItemDataSet::GetArmorIndexById(int armorid) {
	if (armorid < ITEM_ARMOR_AMOUNT)
		return armorid;
	for (unsigned int i = ITEM_ARMOR_AMOUNT; i < armor.size(); i++)
		if (armor[i].id == armorid)
			return i;
	return -1;
}

int ItemDataSet::GetStatIndexById(int statid) {
	if (statid < ITEM_STAT_AMOUNT)
		return statid;
	for (unsigned int i = ITEM_STAT_AMOUNT; i < stat.size(); i++)
		if (stat[i].id == statid)
			return i;
	return -1;
}

ItemDataStruct dummyitem;
ItemDataStruct& ItemDataSet::GetItemById(int itemid) {
	int index = GetItemIndexById(itemid);
	if (index >= 0)
		return item[index];
	dummyitem.parent = this;
	dummyitem.id = -1;
	dummyitem.name.CreateEmpty();
	dummyitem.name.SetValue(L"[Invalid]");
	return dummyitem;
}

KeyItemDataStruct dummykeyitem;
KeyItemDataStruct& ItemDataSet::GetKeyItemById(int keyitemid) {
	int index = GetKeyItemIndexById(keyitemid);
	if (index >= 0)
		return key_item[index];
	dummykeyitem.id = -1;
	dummykeyitem.name.CreateEmpty();
	dummykeyitem.name.SetValue(L"[Invalid]");
	return dummykeyitem;
}

ItemUsableDataStruct dummyusable;
ItemUsableDataStruct& ItemDataSet::GetUsableById(int usableid) {
	int index = GetUsableIndexById(usableid);
	if (index >= 0)
		return usable[index];
	dummyusable.id = -1;
	return dummyusable;
}

ItemWeaponDataStruct dummyweapon;
ItemWeaponDataStruct& ItemDataSet::GetWeaponById(int weaponid) {
	int index = GetWeaponIndexById(weaponid);
	if (index >= 0)
		return weapon[index];
	dummyweapon.id = -1;
	return dummyweapon;
}

ItemArmorDataStruct dummyarmor;
ItemArmorDataStruct& ItemDataSet::GetArmorById(int armorid) {
	int index = GetArmorIndexById(armorid);
	if (index >= 0)
		return armor[index];
	dummyarmor.id = -1;
	return dummyarmor;
}

ItemStatDataStruct dummystat;
ItemStatDataStruct& ItemDataSet::GetStatById(int statid) {
	int index = GetStatIndexById(statid);
	if (index >= 0)
		return stat[index];
	dummystat.id = -1;
	return dummystat;
}

void ItemWeaponDataStruct::InitializeDefault(int dataid) {
	id = dataid;
	flag = 0;
	status = 0;
	model = 0;
	model_name = L"";
	damage_formula = 0;
	power = 0;
	element = 0;
	status_accuracy = 0;
	offset1 = 0;
	offset2 = 0;
	hit_sfx = 0;
}

void ItemWeaponDataStruct::UpdateModelName() {
	unsigned int i;
	for (i = 0; i < G_V_ELEMENTS(SteamWeaponModel); i++)
		if (model == SteamWeaponModel[i].id) {
			model_name = SteamWeaponModel[i].name;
			return;
		}
	model_name = L"<null>";
}

void ItemWeaponDataStruct::UpdateModelId() {
	unsigned int i;
	for (i = 0; i < G_V_ELEMENTS(SteamWeaponModel); i++)
		if (model_name.compare(SteamWeaponModel[i].name) == 0) {
			model = SteamWeaponModel[i].id;
			return;
		}
	model = 0;
}

void ItemArmorDataStruct::InitializeDefault(int dataid) {
	id = dataid;
	defence = 0;
	evade = 0;
	magic_defence = 0;
	magic_evade = 0;
}

void ItemStatDataStruct::InitializeDefault(int dataid) {
	id = dataid;
	speed = 0;
	strength = 0;
	magic = 0;
	spirit = 0;
	element_immune = 0;
	element_absorb = 0;
	element_half = 0;
	element_weak = 0;
	element_boost = 0;
}

bool AnyAbilityStruct::operator==(AnyAbilityStruct other) {
	return id == other.id && (is_active != 0) == (other.is_active != 0);
}

void AnyAbilityStruct::Setup(int rawid) {
	int spellid = ConvertIdToSpellId(rawid);
	if (spellid >= 0) {
		id = spellid;
		is_active = true;
	} else {
		id = ConvertIdToSupportId(rawid);
		is_active = false;
	}
}

void AnyAbilityStruct::Setup(int abilid, bool active) {
	id = abilid;
	is_active = active;
}

int AnyAbilityStruct::GetRawId() {
	if (is_active)
		return ConvertSpellIdToId(id);
	return ConvertSupportIdToId(id);
}

bool AnyAbilityStruct::IsVoid() {
	return is_active && id == 0;
}

string AnyAbilityStruct::GetStringId() {
	if (IsVoid())
		return "0";
	if (is_active)
		return "AA:" + to_string(id);
	return "SA:" + to_string(id);
}

int AnyAbilityStruct::ConvertSpellIdToId(int spellid) {
	int poolnum = spellid / SPELL_AMOUNT;
	int idinpool = spellid % SPELL_AMOUNT;
	return poolnum * 256 + idinpool;
}

int AnyAbilityStruct::ConvertSupportIdToId(int supportid) {
	int poolnum = supportid / SUPPORT_AMOUNT;
	int idinpool = supportid % SUPPORT_AMOUNT;
	return poolnum * 256 + SPELL_AMOUNT + idinpool;
}

int AnyAbilityStruct::ConvertIdToSpellId(int id) {
	if (id % 256 >= SPELL_AMOUNT)
		return -1;
	int poolnum = id / 256;
	int idinpool = id % 256;
	return poolnum * SPELL_AMOUNT + idinpool;
}

int AnyAbilityStruct::ConvertIdToSupportId(int id) {
	if (id % 256 < SPELL_AMOUNT)
		return -1;
	int poolnum = id / 256;
	int idinpool = id % 256 - SPELL_AMOUNT;
	return poolnum * SUPPORT_AMOUNT + idinpool;
}
