#include "Stats.h"

#include "main.h"
#include "DllEditor.h"
#include "Hades_Strings.h"
#include "Database_Steam.h"
#include "Database_CSV.h"
#include "CommonUtility.h"

#define STAT_HWS_VERSION 4

#define BTLPARAM_CSV_CHECK L"%id%;%avatar_sprite%;%model%;%model_trance%;%trance_color%;%animations%;%attack_sfx%;%weapon_bone%;%shadow_bone%, %shadow_size%;%status_bone%;%status_offy%;%status_offz%;%attack_sound%"
#define BTLPARAM_CSV_DEFAULT BTLPARAM_CSV_CHECK L";# %name%"
#define CHARPARAM_CSV_CHECK L"%id%;%default_row%;%default_winpose%;%default_category%;%commandset%;%equipset%;%battle_param%;%name_keyword%"
#define CHARPARAM_CSV_DEFAULT CHARPARAM_CSV_CHECK L";# %name%"
#define INITSTAT_CSV_CHECK L"%id%;%speed%;%strength%;%magic%;%spirit%;%magic_stone%"
#define INITSTAT_CSV_DEFAULT L"%name%;" INITSTAT_CSV_CHECK
#define INITEQUIP_CSV_CHECK L"%id%;%weapon%;%head%;%wrist%;%armor%;%accessory%"
#define INITEQUIP_CSV_DEFAULT L"%name%;" INITEQUIP_CSV_CHECK L";# %id% - %name% (%description%)"
#define ABILLIST_CSV_CHECK L"%ability%;%ap_cost%"
#define ABILLIST_CSV_DEFAULT ABILLIST_CSV_CHECK L";# %ability_name%"
#define CMDLIST_CSV_CHECK L"%id%;%attack_command%;%defend_command%;%first_command%;%second_command%;%item_command%;%change_command%;%attack_command_trance%;%defend_command_trance%;%first_command_trance%;%second_command_trance%;%item_command_trance%;%change_command_trance%"
#define CMDLIST_CSV_DEFAULT CMDLIST_CSV_CHECK L";# %name%: %description%"
#define LEVEL_CSV_CHECK L"%exp%;%hp%;%mp%"
#define LEVEL_CSV_DEFAULT LEVEL_CSV_CHECK L";# Level %level%"

int CharBattleParameterStruct::GetCharacterIndex() {
	if (id >= 19)
		return id - 7;
	unsigned int paramcount;
	for (int charindex = 0; charindex < 12; charindex++) {
		int* params = parent->GetCharacterBattleParametersIndices(charindex, &paramcount);
		for (unsigned int i = 0; i < paramcount; i++)
			if (params[i] == id)
				return charindex;
	}
	return -1;
}

int InitialEquipDataStruct::GetCharacterIndex() {
	if (id >= 16)
		return id - 4;
	unsigned int equipcount;
	for (int charindex = 0; charindex < 12; charindex++) {
		int* equips = parent->GetCharacterEquipmentsIndices(charindex, &equipcount);
		for (unsigned int i = 0; i < equipcount; i++)
			if (equips[i] == id)
				return charindex;
	}
	return -1;
}

int AbilitySetDataStruct::GetCharacterIndex() {
	if (id >= 20)
		return id - 8;
	unsigned int cmdcount;
	for (int charindex = 0; charindex < 12; charindex++) {
		int* cmds = parent->GetCharacterCommandsIndices(charindex, &cmdcount);
		for (unsigned int i = 0; i < cmdcount; i++)
			if (cmds[i] == id)
				return charindex;
	}
	return -1;
}

void CommandSetDataStruct::InitializeDefaultCommands() {
	attack_command = attack_command_trance = 1;
	defend_command = defend_command_trance = 4;
	change_command = change_command_trance = 7;
	item_command = item_command_trance = 14;
}

int CommandSetDataStruct::GetCharacterIndex() {
	if (id >= 20)
		return id - 8;
	unsigned int cmdcount;
	for (int charindex = 0; charindex < 12; charindex++) {
		int* cmds = parent->GetCharacterCommandsIndices(charindex, &cmdcount);
		for (unsigned int i = 0; i < cmdcount; i++)
			if (cmds[i] == id)
				return charindex;
	}
	return -1;
}

wxString CharBattleParameterStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("%s"), parent->GetCharacterById(GetCharacterIndex()).default_name.str_nice);
	if (fieldname.IsSameAs("avatar_sprite")) return wxString::Format(wxT("%s"), avatar_sprite);
	if (fieldname.IsSameAs("model")) return wxString::Format(wxT("%s"), model);
	if (fieldname.IsSameAs("model_trance")) return wxString::Format(wxT("%s"), model_trance);
	if (fieldname.IsSameAs("trance_color")) return wxString::Format(wxT("%d, %d, %d"), trance_color[0], trance_color[1], trance_color[2]);
	if (fieldname.IsSameAs("animations")) {
		wxString result = _(L"");
		for (int i = 0; i < BATTLE_MOTION_AMOUNT; i++)
			result += (i == 0 ? _(L"") : _(L";")) + wxString::Format(wxT("%s"), anim[i]);
		return result;
	}
	if (fieldname.IsSameAs("attack_sfx")) return wxString::Format(wxT("%d"), attack_sfx);
	if (fieldname.IsSameAs("weapon_bone")) return wxString::Format(wxT("%d"), weapon_bone);
	if (fieldname.IsSameAs("shadow_bone")) return wxString::Format(wxT("%d, %d"), shadow_bone[0], shadow_bone[1]);
	if (fieldname.IsSameAs("shadow_size")) return wxString::Format(wxT("%d, %d, %d"), shadow_size[0], shadow_size[1], shadow_size[2]);
	if (fieldname.IsSameAs("status_bone")) return wxString::Format(wxT("%d, %d, %d, %d, %d, %d"), status_bone[0], status_bone[1], status_bone[2], status_bone[3], status_bone[4], status_bone[5]);
	if (fieldname.IsSameAs("status_offy")) return wxString::Format(wxT("%d, %d, %d, %d, %d, %d"), status_offy[0], status_offy[1], status_offy[2], status_offy[3], status_offy[4], status_offy[5]);
	if (fieldname.IsSameAs("status_offz")) return wxString::Format(wxT("%d, %d, %d, %d, %d, %d"), status_offz[0], status_offz[1], status_offz[2], status_offz[3], status_offz[4], status_offz[5]);
	if (fieldname.IsSameAs("attack_sound")) return wxString::Format(wxT("%d, %d"), attack_sound[0], attack_sound[1]);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_battle_param.find(fieldname); search != parent->custom_field_battle_param.end()) return search->second;
	return _(L"");
}

bool CharBattleParameterStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(BTLPARAM_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].BeforeLast(L';'), rightcsv);
}

wxString InitialStatDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("%s"), default_name.str_nice);
	if (fieldname.IsSameAs("speed")) return wxString::Format(wxT("%d"), speed);
	if (fieldname.IsSameAs("strength")) return wxString::Format(wxT("%d"), strength);
	if (fieldname.IsSameAs("magic")) return wxString::Format(wxT("%d"), magic);
	if (fieldname.IsSameAs("spirit")) return wxString::Format(wxT("%d"), spirit);
	if (fieldname.IsSameAs("magic_stone")) return wxString::Format(wxT("%d"), magic_stone);
	if (fieldname.IsSameAs("default_row")) return wxString::Format(wxT("%d"), default_row);
	if (fieldname.IsSameAs("default_winpose")) return wxString::Format(wxT("%d"), default_winpose);
	if (fieldname.IsSameAs("default_category")) return wxString::Format(wxT("%d"), default_category);
	if (fieldname.IsSameAs("commandset")) return wxString::Format(wxT("%d"), parent->GetCharacterCommandsIndices(id, NULL)[0]);
	if (fieldname.IsSameAs("equipset")) return wxString::Format(wxT("%d"), parent->GetCharacterEquipmentsIndices(id, NULL)[0]);
	if (fieldname.IsSameAs("battle_param")) return wxString::Format(wxT("%s"), battle_param);
	if (fieldname.IsSameAs("name_keyword")) return wxString::Format(wxT("%s"), name_keyword);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_initial_stat.find(fieldname); search != parent->custom_field_initial_stat.end()) return search->second;
	return _(L"");
}

bool InitialStatDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	int fieldcount = entries[0].Replace(";", ";") + 1;
	if (fieldcount == 7) {
		wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(INITSTAT_CSV_CHECK));
		return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';'), rightcsv);
	}
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(CHARPARAM_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].BeforeLast(L';'), rightcsv);
}

wxString InitialEquipDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("%s"), parent->GetCharacterById(GetCharacterIndex()).default_name.str_nice);
	if (fieldname.IsSameAs("description")) {
		wxString result = _(L"");
		if (GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_ITEM]) {
			ItemDataSet* itset = GetGameSaveSet()->itemset;
			bool addcomma = false;
			if (weapon != 0xFF) {
				result += itset->GetItemById(weapon).name.str_nice;
				addcomma = true;
			}
			if (head != 0xFF) {
				result += (addcomma ? _(L", ") : _(L"")) + itset->GetItemById(head).name.str_nice;
				addcomma = true;
			}
			if (wrist != 0xFF) {
				result += (addcomma ? _(L", ") : _(L"")) + itset->GetItemById(wrist).name.str_nice;
				addcomma = true;
			}
			if (armor != 0xFF) {
				result += (addcomma ? _(L", ") : _(L"")) + itset->GetItemById(armor).name.str_nice;
				addcomma = true;
			}
			if (accessory != 0xFF)
				result += (addcomma ? _(L", ") : _(L"")) + itset->GetItemById(accessory).name.str_nice;
		}
		if (result.IsEmpty())
			return _(L"Empty");
		return result;
	}
	if (fieldname.IsSameAs("weapon")) return wxString::Format(wxT("%d"), weapon == 0xFF ? -1 : weapon);
	if (fieldname.IsSameAs("head")) return wxString::Format(wxT("%d"), head == 0xFF ? -1 : head);
	if (fieldname.IsSameAs("wrist")) return wxString::Format(wxT("%d"), wrist == 0xFF ? -1 : wrist);
	if (fieldname.IsSameAs("armor")) return wxString::Format(wxT("%d"), armor == 0xFF ? -1 : armor);
	if (fieldname.IsSameAs("accessory")) return wxString::Format(wxT("%d"), accessory == 0xFF ? -1 : accessory);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_initial_equip.find(fieldname); search != parent->custom_field_initial_equip.end()) return search->second;
	return _(L"");
}

bool InitialEquipDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(INITEQUIP_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';').BeforeLast(L';'), rightcsv);
}

wxString AbilityEntryDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("ability_name")) {
		if (GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_SPELL] && GetGameSaveSet()->sectionloaded[DATA_SECTION_SUPPORT]) {
			if (ability.is_active)
				return wxString::Format(wxT("%s"), GetGameSaveSet()->spellset->GetSpellById(ability.id).name.str_nice);
			return wxString::Format(wxT("%s"), GetGameSaveSet()->supportset->GetSupportById(ability.id).name.str_nice);
		}
		return _(L"");
	}
	if (fieldname.IsSameAs("ability")) return wxString::Format(wxT("%s"), ability.GetStringId());
	if (fieldname.IsSameAs("ap_cost")) return wxString::Format(wxT("%d"), ap_cost);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->parent->custom_field_ability_list.find(fieldname); search != parent->parent->custom_field_ability_list.end()) return search->second;
	return _(L"");
}

bool AbilityEntryDataStruct::CompareWithCSV(wxArrayString entries) {
	return false;
}

wxString CommandSetDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) {
		int charindex = GetCharacterIndex();
		if (charindex < 0)
			return _(L"None");
		return wxString::Format(wxT("%s"), parent->GetCharacterById(charindex).default_name.str_nice);
	}
	if (fieldname.IsSameAs("description")) {
		if (GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_CMD]) {
			CommandDataSet* cmdset = GetGameSaveSet()->cmdset;
			wxString result = wxString::Format(wxT("%s"), cmdset->GetCommandById(first_command).name.str_nice);
			if (first_command != first_command_trance && first_command_trance != 0)
				result += wxString::Format(wxT(" (%s)"), cmdset->GetCommandById(first_command_trance).name.str_nice);
			result += wxString::Format(wxT(", %s"), cmdset->GetCommandById(second_command).name.str_nice);
			if (second_command != second_command_trance && second_command_trance != 0)
				result += wxString::Format(wxT(" (%s)"), cmdset->GetCommandById(second_command_trance).name.str_nice);
			return result;
		}
		return _(L"");
	}
	if (fieldname.IsSameAs("trance_attack")) return wxString::Format(wxT("%d"), trance_attack);
	if (fieldname.IsSameAs("attack_command")) return wxString::Format(wxT("%d"), attack_command);
	if (fieldname.IsSameAs("defend_command")) return wxString::Format(wxT("%d"), defend_command);
	if (fieldname.IsSameAs("first_command")) return wxString::Format(wxT("%d"), first_command);
	if (fieldname.IsSameAs("second_command")) return wxString::Format(wxT("%d"), second_command);
	if (fieldname.IsSameAs("item_command")) return wxString::Format(wxT("%d"), item_command);
	if (fieldname.IsSameAs("change_command")) return wxString::Format(wxT("%d"), change_command);
	if (fieldname.IsSameAs("attack_command_trance")) return wxString::Format(wxT("%d"), attack_command_trance);
	if (fieldname.IsSameAs("defend_command_trance")) return wxString::Format(wxT("%d"), defend_command_trance);
	if (fieldname.IsSameAs("first_command_trance")) return wxString::Format(wxT("%d"), first_command_trance);
	if (fieldname.IsSameAs("second_command_trance")) return wxString::Format(wxT("%d"), second_command_trance);
	if (fieldname.IsSameAs("item_command_trance")) return wxString::Format(wxT("%d"), item_command_trance);
	if (fieldname.IsSameAs("change_command_trance")) return wxString::Format(wxT("%d"), change_command_trance);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_command_list.find(fieldname); search != parent->custom_field_command_list.end()) return search->second;
	return _(L"");
}

bool CommandSetDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(CMDLIST_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].BeforeLast(L';'), rightcsv);
}

wxString LevelDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("level")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("exp")) return wxString::Format(wxT("%u"), exp_table);
	if (fieldname.IsSameAs("hp")) return wxString::Format(wxT("%d"), hp_table);
	if (fieldname.IsSameAs("mp")) return wxString::Format(wxT("%d"), mp_table);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_level.find(fieldname); search != parent->custom_field_level.end()) return search->second;
	return _(L"");
}

bool LevelDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(LEVEL_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].BeforeLast(L';'), rightcsv);
}

const unsigned int steam_statabil_field_size[] = { 8, 8 };
const unsigned int steam_stat_field_size[] = { 8, 8, 8, 8, 16 };
const unsigned int steam_lvlhpmp_field_size[] = { 16, 16 };

void InitialStatDataStruct::GenerateDefaultName() {
	wstring name = id < PLAYABLE_CHAR_AMOUNT ? HADES_STRING_CHARACTER_DEFAULT_NAME[id].ToStdWstring() : (L"PC n°" + to_string(id));
	default_name.CreateEmpty();
	if (GetGameType() == GAME_TYPE_PSX || hades::STEAM_SINGLE_LANGUAGE_MODE) {
		default_name.SetValue(name);
	} else {
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
			default_name.SetValue(name, lang);
	}
}

int InitialStatDataStruct::SetDefaultName(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(default_name);
		tmp.SetValue(newvalue);
		int oldlen = default_name.length;
		int newlen = tmp.length;
		if (parent->default_name_space_used + newlen > parent->default_name_space_total + oldlen)
			return 1;
		parent->default_name_space_used += newlen - oldlen;
	}
	default_name.SetValue(newvalue);
	return 0;
}

int InitialStatDataStruct::SetDefaultName(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = default_name.length;
		int newlen = newvalue.length;
		if (parent->default_name_space_used + newlen > parent->default_name_space_total + oldlen)
			return 1;
		parent->default_name_space_used += newlen - oldlen;
	}
	default_name = newvalue;
	return 0;
}

int character_equip_set[3];
int* StatDataSet::GetCharacterEquipmentsIndices(int charindex, unsigned int* amount) {
	character_equip_set[0] = charindex;
	if (charindex < 9) {
		character_equip_set[1] = EQUIP_SET_AMOUNT - 1;
		if (amount)
			*amount = 2;
	} else if (charindex == 9) {
		character_equip_set[1] = 12;
		character_equip_set[2] = EQUIP_SET_AMOUNT - 1;
		if (amount)
			*amount = 3;
	} else if (charindex == 10) {
		character_equip_set[1] = 14;
		character_equip_set[2] = EQUIP_SET_AMOUNT - 1;
		if (amount)
			*amount = 3;
	} else if (charindex == 11) {
		character_equip_set[1] = 13;
		character_equip_set[2] = EQUIP_SET_AMOUNT - 1;
		if (amount)
			*amount = 3;
	} else {
		character_equip_set[0] = charindex + 4;
		character_equip_set[1] = EQUIP_SET_AMOUNT - 1;
		if (amount)
			*amount = 2;
	}
	return character_equip_set;
}

int character_command_set[3];
int* StatDataSet::GetCharacterCommandsIndices(int charindex, unsigned int* amount) {
	if (charindex == 0) {
		character_command_set[0] = 0;
		character_command_set[1] = 16;
		if (amount)
			*amount = 2;
	} else if (charindex < 8) {
		character_command_set[0] = charindex;
		if (amount)
			*amount = 1;
	} else if (charindex == 8) {
		character_command_set[0] = 8;
		character_command_set[1] = 9;
		character_command_set[2] = 17;
		if (amount)
			*amount = 3;
	} else if (charindex == 9) {
		character_command_set[0] = 10;
		character_command_set[1] = 11;
		character_command_set[2] = 18;
		if (amount)
			*amount = 3;
	} else if (charindex == 10) {
		character_command_set[0] = 12;
		character_command_set[1] = 13;
		character_command_set[2] = 19;
		if (amount)
			*amount = 3;
	} else if (charindex == 11) {
		character_command_set[0] = 14;
		character_command_set[1] = 15;
		if (amount)
			*amount = 2;
	} else {
		character_command_set[0] = charindex + 8;
		if (amount)
			*amount = 1;
	}
	return character_command_set;
}

int character_btlparam_set[4];
int* StatDataSet::GetCharacterBattleParametersIndices(int charindex, unsigned int* amount) {
	if (charindex == 0) {
		character_btlparam_set[0] = 0;
		character_btlparam_set[1] = 1;
		if (amount)
			*amount = 2;
	} else if (charindex == 1) {
		character_btlparam_set[0] = 2;
		if (amount)
			*amount = 1;
	} else if (charindex == 2) {
		character_btlparam_set[0] = 3;
		character_btlparam_set[1] = 4;
		character_btlparam_set[2] = 5;
		character_btlparam_set[3] = 6;
		if (amount)
			*amount = 4;
	} else if (charindex == 3) {
		character_btlparam_set[0] = 7;
		character_btlparam_set[1] = 8;
		if (amount)
			*amount = 2;
	} else if (charindex == 4) {
		character_btlparam_set[0] = 12;
		if (amount)
			*amount = 1;
	} else if (charindex == 5) {
		character_btlparam_set[0] = 9;
		if (amount)
			*amount = 1;
	} else if (charindex == 6) {
		character_btlparam_set[0] = 10;
		character_btlparam_set[1] = 11;
		if (amount)
			*amount = 2;
	} else if (charindex == 7) {
		character_btlparam_set[0] = 13;
		if (amount)
			*amount = 1;
	} else if (charindex == 8) {
		character_btlparam_set[0] = 14;
		if (amount)
			*amount = 1;
	} else if (charindex == 9) {
		character_btlparam_set[0] = 15;
		if (amount)
			*amount = 1;
	} else if (charindex == 10) {
		character_btlparam_set[0] = 16;
		character_btlparam_set[1] = 17;
		if (amount)
			*amount = 2;
	} else if (charindex == 11) {
		character_btlparam_set[0] = 18;
		if (amount)
			*amount = 1;
	} else {
		character_btlparam_set[0] = charindex + 7;
		if (amount)
			*amount = 1;
	}
	return character_btlparam_set;
}

int StatDataSet::GetCharacterIndexById(int charid) {
	if (charid < PLAYABLE_CHAR_AMOUNT)
		return charid;
	for (unsigned int i = PLAYABLE_CHAR_AMOUNT; i < initial_stat.size(); i++)
		if (initial_stat[i].id == charid)
			return i;
	return -1;
}

InitialStatDataStruct dummystat;
InitialStatDataStruct& StatDataSet::GetCharacterById(int charid) {
	int index = GetCharacterIndexById(charid);
	if (index >= 0)
		return initial_stat[index];
	dummystat.parent = this;
	dummystat.id = -1;
	dummystat.default_name.CreateEmpty();
	dummystat.default_name.SetValue(L"[Invalid]");
	return dummystat;
}

void StatDataSet::InitializeNewCharacter(int charindex) {
	InitialStatDataStruct& stat = initial_stat[charindex];
	int reverseindex = initial_stat.size() - charindex - 1;
	int i;
	stat.GenerateDefaultName();
	stat.name_keyword = stat.id < 100 ? L"CH" + to_string(stat.id) : L"C" + to_string(stat.id).substr(0, 3);
	stat.battle_param = to_wstring(stat.id + 7);
	/*stat.avatar_sprite = L"face" + to_string(stat.id + 1);
	stat.default_row = 0;
	stat.default_winpose = 0;
	stat.default_category = 0;
	stat.attack_sfx = 100;
	stat.attack_sound[0] = stat.attack_sound[1] = 0;
	stat.weapon_bone = 0;
	for (i = 0; i < 6; i++) {
		stat.status_bone[i] = 0;
		stat.status_offy[i] = 0;
		stat.status_offz[i] = 0;
	}
	stat.shadow_bone[0] = stat.shadow_bone[1] = 0;
	stat.shadow_size[0] = stat.shadow_size[1] = stat.shadow_size[2] = 0;
	stat.model = L"";
	stat.model_trance = L"";
	stat.trance_color[0] = 0xFF;
	stat.trance_color[1] = 0x60;
	stat.trance_color[2] = 0x60;
	for (i = 0; i < BATTLE_MOTION_AMOUNT; i++)
		stat.anim[i] = L"";*/
	InitialEquipDataStruct equip;
	equip.parent = this;
	equip.id = stat.id + 4;
	equip.weapon = 0xFF;
	equip.head = 0xFF;
	equip.wrist = 0xFF;
	equip.armor = 0xFF;
	equip.accessory = 0xFF;
	AbilitySetDataStruct abil;
	abil.parent = this;
	abil.id = stat.id + 8;
	abil.entry.resize(ABILITY_SET_CAPACITY);
	for (i = 0; i < ABILITY_SET_CAPACITY; i++) {
		abil.entry[i].parent = &abil;
		abil.entry[i].id = i;
		abil.entry[i].ability.Setup(0, true);
	}
	CommandSetDataStruct command;
	command.parent = this;
	command.id = stat.id + 8;
	command.first_command = 0;
	command.second_command = 0;
	command.first_command_trance = 0;
	command.second_command_trance = 0;
	command.InitializeDefaultCommands();
	initial_equip.insert(initial_equip.begin() + initial_equip.size() - reverseindex, equip);
	ability_list.insert(ability_list.begin() + ability_list.size() - reverseindex, abil);
	command_list.insert(command_list.begin() + command_list.size() - reverseindex, command);
}

#define MACRO_STAT_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	uint16_t zero16 = 0; \
	uint32_t txtpos; \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Short(ffbin,amount); \
	IO ## Short(ffbin,zero16); \
	txtpos = ffbin.tellg(); \
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) { \
		IO ## Short(ffbin,initial_stat[i].default_name_offset); \
		IO ## Short(ffbin,initial_stat[i].default_name_size_x); \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,0); \
	if (READ) default_name_space_used = PLAYABLE_CHAR_AMOUNT*4; \
	if (PPF) PPFInitScanStep(ffbin,true,default_name_space_total); \
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,initial_stat[i].default_name_offset); \
		IO ## FF9String(ffbin,initial_stat[i].default_name); \
		if (READ) default_name_space_used += initial_stat[i].default_name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,default_name_space_total);

#define MACRO_STAT_IOFUNCTIONEQUIP(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < equipamount; i++) { \
		IO ## FlexibleChar(ffbin, initial_equip[i].weapon, useextendedtype); \
		IO ## FlexibleChar(ffbin, initial_equip[i].head, useextendedtype); \
		IO ## FlexibleChar(ffbin, initial_equip[i].wrist, useextendedtype); \
		IO ## FlexibleChar(ffbin, initial_equip[i].armor, useextendedtype); \
		IO ## FlexibleChar(ffbin, initial_equip[i].accessory, useextendedtype); \
		if (useextendedtype2) IO ## CSVFields(ffbin, initial_equip[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONABIL(IO,SEEK,READ,PPF) \
	uint8_t rawid; \
	int abilsetsize = ABILITY_SET_CAPACITY; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<abilsetamount;i++) { \
		if (useextendedtype) { \
			if (!READ) abilsetsize = ability_list[i].entry.size(); \
			IO ## FlexibleChar(ffbin, abilsetsize, true); \
			if (READ) { \
				ability_list[i].entry.resize(abilsetsize); \
				for (j=0;j<abilsetsize;j++) { \
					ability_list[i].entry[j].parent = &ability_list[i]; \
					ability_list[i].entry[j].id = j; \
				} \
			} \
		} \
		for (j=0;j<abilsetsize;j++) { \
			if (READ) ability_list[i].entry[j].ability.Setup(rawid); \
			if (useextendedtype) { \
				IO ## Char(ffbin, ability_list[i].entry[j].ability.is_active); \
				IO ## FlexibleChar(ffbin, ability_list[i].entry[j].ability.id, true); \
			} else { \
				if (!READ) rawid = ability_list[i].entry[j].ability.GetRawId(); \
				IO ## Char(ffbin, rawid); \
				if (READ) ability_list[i].entry[j].ability.Setup(rawid); \
			} \
			IO ## FlexibleChar(ffbin, ability_list[i].entry[j].ap_cost, useextendedtype2); \
			if (useextendedtype2) IO ## CSVFields(ffbin, ability_list[i].entry[j].custom_field); \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONSTAT(IO,SEEK,READ,PPF) \
	uint8_t zero8 = 0; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < charamount; i++) { \
		IO ## FlexibleChar(ffbin, initial_stat[i].speed, useextendedtype2); \
		IO ## FlexibleChar(ffbin, initial_stat[i].strength, useextendedtype2); \
		IO ## FlexibleChar(ffbin, initial_stat[i].magic, useextendedtype2); \
		IO ## FlexibleChar(ffbin, initial_stat[i].spirit, useextendedtype2); \
		IO ## FlexibleChar(ffbin, initial_stat[i].magic_stone, useextendedtype2); \
		IO ## Char(ffbin, zero8); \
		if (useextendedtype2) IO ## CSVFields(ffbin, initial_stat[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONLEVEL(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < MAX_LEVEL; i++) { \
		IO ## Short(ffbin, level[i].hp_table); \
		IO ## Short(ffbin, level[i].mp_table); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONLEVELEXP(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < MAX_LEVEL; i++) { \
		IO ## Long(ffbin, level[i].exp_table); \
		if (useextendedtype2) IO ## CSVFields(ffbin, level[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Char(ffbin,shared_command_attack); \
	IO ## Char(ffbin,shared_command_2); \
	IO ## Char(ffbin,shared_command_3); \
	IO ## Char(ffbin,shared_command_item); \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONCOMMAND(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < cmdsetamount; i++) { \
		IO ## FlexibleChar(ffbin, command_list[i].first_command, useextendedtype); \
		IO ## FlexibleChar(ffbin, command_list[i].second_command, useextendedtype); \
		if (useextendedtype2) { \
			IO ## FlexibleChar(ffbin, command_list[i].attack_command, useextendedtype); \
			IO ## FlexibleChar(ffbin, command_list[i].defend_command, useextendedtype); \
			IO ## FlexibleChar(ffbin, command_list[i].item_command, useextendedtype); \
			IO ## FlexibleChar(ffbin, command_list[i].change_command, useextendedtype); \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONTRANCECOMMAND(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < cmdsetamount; i++) { \
		IO ## FlexibleChar(ffbin, command_list[i].first_command_trance, useextendedtype); \
		IO ## FlexibleChar(ffbin, command_list[i].second_command_trance, useextendedtype); \
		if (useextendedtype2) { \
			IO ## FlexibleChar(ffbin, command_list[i].attack_command_trance, useextendedtype); \
			IO ## FlexibleChar(ffbin, command_list[i].defend_command_trance, useextendedtype); \
			IO ## FlexibleChar(ffbin, command_list[i].item_command_trance, useextendedtype); \
			IO ## FlexibleChar(ffbin, command_list[i].change_command_trance, useextendedtype); \
		} \
		IO ## Char(ffbin, command_list[i].trance_attack); \
		if (useextendedtype2) IO ## CSVFields(ffbin, command_list[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();


void StatDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int btlamount = GetGameType() == GAME_TYPE_STEAM && config.dll_usage != 0 ? CHAR_BATTLE_PARAMETER_AMOUNT : 0;
	int charamount = PLAYABLE_CHAR_AMOUNT;
	int equipamount = EQUIP_SET_AMOUNT;
	int abilsetamount = ABILITY_SET_AMOUNT;
	int cmdsetamount = COMMAND_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i, j;
	csv_header_battle_param = _(HADES_STRING_CSV_BATTLEPARAM_HEADER);
	csv_header_character_param = _(HADES_STRING_CSV_CHARPARAM_HEADER);
	csv_header_initial_stat = _(HADES_STRING_CSV_STATINIT_HEADER);
	csv_header_initial_equip = _(HADES_STRING_CSV_STATEQUIP_HEADER);
	csv_header_ability_list = wxEmptyString;
	csv_header_command_list = _(HADES_STRING_CSV_STATCMD_HEADER);
	csv_header_level = _(HADES_STRING_CSV_STATLEVEL_HEADER);
	csv_format_battle_param = _(BTLPARAM_CSV_DEFAULT);
	csv_format_character_param = _(CHARPARAM_CSV_DEFAULT);
	csv_format_initial_stat = _(INITSTAT_CSV_DEFAULT);
	csv_format_initial_equip = _(INITEQUIP_CSV_DEFAULT);
	csv_format_ability_list = _(ABILLIST_CSV_DEFAULT);
	csv_format_command_list = _(CMDLIST_CSV_DEFAULT);
	csv_format_level = _(LEVEL_CSV_DEFAULT);
	default_name_space_total = config.stat_defaultname_space_total;
	battle_param.resize(btlamount);
	initial_stat.resize(PLAYABLE_CHAR_AMOUNT);
	initial_equip.resize(EQUIP_SET_AMOUNT);
	ability_list.resize(COMMAND_SET_AMOUNT);
	command_list.resize(COMMAND_SET_AMOUNT);
	level.resize(MAX_LEVEL);
	for (i = 0; i < btlamount; i++) {
		battle_param[i].parent = this;
		battle_param[i].id = i;
	}
	for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++) {
		initial_stat[i].parent = this;
		initial_stat[i].id = i;
	}
	for (i = 0; i < EQUIP_SET_AMOUNT; i++) {
		initial_equip[i].parent = this;
		initial_equip[i].id = i;
	}
	for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
		ability_list[i].parent = this;
		ability_list[i].id = i;
		if (i < ABILITY_SET_AMOUNT) {
			ability_list[i].entry.resize(ABILITY_SET_CAPACITY);
			for (j = 0; j < ABILITY_SET_CAPACITY; j++) {
				ability_list[i].entry[j].id = j;
				ability_list[i].entry[j].parent = &ability_list[i];
			}
		}
	}
	for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
		command_list[i].parent = this;
		command_list[i].id = i;
		command_list[i].InitializeDefaultCommands();
	}
	for (i = 0; i < MAX_LEVEL; i++) {
		level[i].parent = this;
		level[i].id = i;
	}
	initial_stat[0].battle_param = L"WeaponShape == 1 ? 0 : 1";
	initial_stat[1].battle_param = L"2";
	initial_stat[2].battle_param = L"ScenarioCounter < 10300 ? (WeaponShape == 7 ? 4 : 3) : (WeaponShape == 7 ? 6 : 5)";
	initial_stat[3].battle_param = L"7";
	initial_stat[4].battle_param = L"12";
	initial_stat[5].battle_param = L"9";
	initial_stat[6].battle_param = L"WeaponShape == 7 ? 11 : 10";
	initial_stat[7].battle_param = L"13";
	initial_stat[8].battle_param = L"14";
	initial_stat[9].battle_param = L"15";
	initial_stat[10].battle_param = L"(1500 <= ScenarioCounter && ScenarioCounter < 1600) ? 17 : 16";
	initial_stat[11].battle_param = L"18";
	if (GetGameType() == GAME_TYPE_PSX) {
		ffbin.seekg(config.stat_defaultname_offset);
		MACRO_STAT_IOFUNCTIONNAME(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.stat_initialequip_offset);
		MACRO_STAT_IOFUNCTIONEQUIP(FFIXRead, FFIXSeek, true, false)
		MACRO_STAT_IOFUNCTIONABIL(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.stat_initialstat_offset);
		MACRO_STAT_IOFUNCTIONSTAT(FFIXRead, FFIXSeek, true, false)
		MACRO_STAT_IOFUNCTIONLEVEL(FFIXRead, FFIXSeek, true, false)
		MACRO_STAT_IOFUNCTIONLEVELEXP(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.stat_cmddata_offset[0]);
		MACRO_STAT_IOFUNCTIONCOMMAND(FFIXRead, FFIXSeek, true, false)
		MACRO_STAT_IOFUNCTIONTRANCECOMMAND(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.stat_cmddata_offset[2]);
		MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(FFIXRead, FFIXSeek, true, false)
	} else {
		if (config.dll_usage != 0) {
			wxArrayString csvlines = MemoriaUtility::LoadCSVLines(_(config.steam_dir_assets) + HADES_STRING_CSV_CHARPARAM_FILE);
			for (i = 0; i < (int)csvlines.GetCount(); i++) {
				wxArrayString csventry = MemoriaUtility::LoadCSVEntry(csvlines[i]);
				if (csventry.GetCount() >= 8) {
					int charid = wxAtoi(csventry[0]);
					if (charid < PLAYABLE_CHAR_AMOUNT) {
						initial_stat[charid].default_row = wxAtoi(csventry[1]);
						initial_stat[charid].default_winpose = wxAtoi(csventry[2]);
						initial_stat[charid].default_category = wxAtoi(csventry[3]);
						initial_stat[charid].name_keyword = csventry[7];
					}
				}
			}
			csvlines = MemoriaUtility::LoadCSVLines(_(config.steam_dir_assets) + HADES_STRING_CSV_BATTLEPARAM_FILE);
			for (i = 0; i < (int)csvlines.GetCount(); i++) {
				wxArrayString csventry = MemoriaUtility::LoadCSVEntry(csvlines[i]);
				if (csventry.GetCount() >= 45) {
					int btlid = wxAtoi(csventry[0]);
					if (btlid < btlamount) {
						battle_param[btlid].avatar_sprite = csventry[1];
						battle_param[btlid].model = csventry[2];
						battle_param[btlid].model_trance = csventry[3];
						wxArrayString color = MemoriaUtility::SplitEntryArray(csventry[4]);
						for (j = 0; j < 3 && j < (int)color.GetCount(); j++)
							battle_param[btlid].trance_color[j] = wxAtoi(color[j]);
						for (j = 0; j < BATTLE_MOTION_AMOUNT; j++)
							battle_param[btlid].anim[j] = csventry[5 + j];
						battle_param[btlid].attack_sfx = wxAtoi(csventry[39]);
						battle_param[btlid].weapon_bone = wxAtoi(csventry[40]);
						wxArrayString shadowdata = MemoriaUtility::SplitEntryArray(csventry[41]);
						for (j = 0; j < 2 && j < (int)shadowdata.GetCount(); j++)
							battle_param[btlid].shadow_bone[j] = wxAtoi(shadowdata[j]);
						for (j = 2; j < 5 && j < (int)shadowdata.GetCount(); j++)
							battle_param[btlid].shadow_size[j - 2] = wxAtoi(shadowdata[j]);
						wxArrayString statusbones = MemoriaUtility::SplitEntryArray(csventry[42]);
						for (j = 0; j < 6 && j < (int)statusbones.GetCount(); j++)
							battle_param[btlid].status_bone[j] = wxAtoi(statusbones[j]);
						wxArrayString offy = MemoriaUtility::SplitEntryArray(csventry[43]);
						for (j = 0; j < 6 && j < (int)offy.GetCount(); j++)
							battle_param[btlid].status_offy[j] = wxAtoi(offy[j]);
						wxArrayString offz = MemoriaUtility::SplitEntryArray(csventry[44]);
						for (j = 0; j < 6 && j < (int)offz.GetCount(); j++)
							battle_param[btlid].status_offz[j] = wxAtoi(offz[j]);
						if (csventry.GetCount() >= 46) {
							wxArrayString snd = MemoriaUtility::SplitEntryArray(csventry[45]);
							for (j = 0; j < 2 && j < (int)snd.GetCount(); j++)
								battle_param[btlid].attack_sound[j] = wxAtoi(snd[j]);
						}
					}
				}
			}
		}
		for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++) // Steam default names are in Assembly-CSharp.dll, FF9TextTool's script...
			initial_stat[i].GenerateDefaultName();
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname;
		for (i = 0; i < EQUIP_SET_AMOUNT; i++) {
			dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_equipset_field_id[i]));
			SteamReadFlexibleChar(dlldata.dll_file, initial_equip[i].weapon, false);
			SteamReadFlexibleChar(dlldata.dll_file, initial_equip[i].head, false);
			SteamReadFlexibleChar(dlldata.dll_file, initial_equip[i].wrist, false);
			SteamReadFlexibleChar(dlldata.dll_file, initial_equip[i].armor, false);
			SteamReadFlexibleChar(dlldata.dll_file, initial_equip[i].accessory, false);
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_ability_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstabil[3] = {
			{ 0x1F, ABILITY_SET_CAPACITY },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("PA_DATA","FF9") },
			{ 0x25 }
		};
		uint8_t* rawabildata[ABILITY_SET_AMOUNT];
		for (i = 0; i < ABILITY_SET_AMOUNT; i++) {
			methinfo.JumpToInstructions(dlldata.dll_file, 3, initinstabil);
			steam_method_position_abilset[i] = dlldata.dll_file.tellg();
			rawabildata[i] = dlldata.ConvertScriptToRaw_Object(ABILITY_SET_CAPACITY, 2, steam_statabil_field_size);
			steam_method_base_length_abilset[i] = (unsigned int)dlldata.dll_file.tellg() - steam_method_position_abilset[i];
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_level_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinststat[3] = {
			{ 0x1F, PLAYABLE_CHAR_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9LEVEL_BASE") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 3, initinststat);
		steam_method_position_initstat = dlldata.dll_file.tellg();
		uint8_t* rawstatdata = dlldata.ConvertScriptToRaw_Object(PLAYABLE_CHAR_AMOUNT, 5, steam_stat_field_size);
		steam_method_base_length_initstat = (unsigned int)dlldata.dll_file.tellg() - steam_method_position_initstat;
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_level_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinsthpmp[3] = {
			{ 0x1F, MAX_LEVEL },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9LEVEL_HPMP") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 3, initinsthpmp);
		steam_method_position_hpmp = dlldata.dll_file.tellg();
		uint8_t* rawlvldata = dlldata.ConvertScriptToRaw_Object(MAX_LEVEL, 2, steam_lvlhpmp_field_size);
		steam_method_base_length_hpmp = (unsigned int)dlldata.dll_file.tellg() - steam_method_position_hpmp;
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_statexp_field_id));
		for (i = 0; i < MAX_LEVEL; i++) {
			SteamReadLong(dlldata.dll_file, level[i].exp_table);
			dlldata.dll_file.seekg(4, ios::cur);
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_rdata_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstcmd[4] = {
			{ 0x1F, COMMAND_SET_AMOUNT },
			{ 0x18 },
			{ 0x73, dlldata.GetMemberTokenIdentifier("void valuetype FF9.command_tags[,]::.ctor( int, int )") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 4, initinstcmd);
		steam_method_position_cmdset = dlldata.dll_file.tellg();
		int64_t** rawcmddata = dlldata.ConvertScriptToRaw_Array2(COMMAND_SET_AMOUNT, 2);
		steam_method_base_length_cmdset = (unsigned int)dlldata.dll_file.tellg() - steam_method_position_cmdset;
		for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
			command_list[i].first_command = rawcmddata[i][0];
			command_list[i].second_command = rawcmddata[i][1];
			delete[] rawcmddata[i];
		}
		delete[] rawcmddata;
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_statcmdtrance_field_id));
		for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
			SteamReadFlexibleChar(dlldata.dll_file, command_list[i].first_command_trance, false);
			SteamReadFlexibleChar(dlldata.dll_file, command_list[i].second_command_trance, false);
			SteamReadChar(dlldata.dll_file, command_list[i].trance_attack);
		}
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(), ios::out | ios::binary);
		for (i = 0; i < ABILITY_SET_AMOUNT; i++)
			ffbin.write((const char*)rawabildata[i], 2 * ABILITY_SET_CAPACITY);
		ffbin.write((const char*)rawstatdata, 6 * PLAYABLE_CHAR_AMOUNT);
		ffbin.write((const char*)rawlvldata, 4 * MAX_LEVEL);
		ffbin.close();
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		MACRO_STAT_IOFUNCTIONABIL(SteamRead, SteamSeek, true, false)
		MACRO_STAT_IOFUNCTIONSTAT(SteamRead, SteamSeek, true, false)
		MACRO_STAT_IOFUNCTIONLEVEL(SteamRead, SteamSeek, true, false)
		ffbin.close();
		remove(fname.c_str());
		for (i = 0; i < ABILITY_SET_AMOUNT; i++)
			delete[] rawabildata[i];
		delete[] rawstatdata;
		delete[] rawlvldata;
		default_name_space_used = 0;
	}
}

DllMetaDataModification* StatDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[EQUIP_SET_AMOUNT + ABILITY_SET_AMOUNT + 6];
	DllMetaData& dlldata = config.meta_dll;
	unsigned int i, j, modcounter = 0;
	uint32_t** argvalue;
	for (i = 0; i < EQUIP_SET_AMOUNT; i++) {
		res[modcounter].position = config.meta_dll.GetStaticFieldOffset(config.dll_equipset_field_id[i]);
		res[modcounter].base_length = 5; // config.meta_dll.GetStaticFieldRange(config.dll_equipset_field_id[i]);
		res[modcounter].new_length = 5;
		res[modcounter].value = new uint8_t[res[modcounter].new_length];
		res[modcounter].value[0] = initial_equip[i].weapon;
		res[modcounter].value[1] = initial_equip[i].head;
		res[modcounter].value[2] = initial_equip[i].wrist;
		res[modcounter].value[3] = initial_equip[i].armor;
		res[modcounter].value[4] = initial_equip[i].accessory;
		modcounter++;
	}
	argvalue = new uint32_t * [ABILITY_SET_CAPACITY];
	for (j = 0; j < ABILITY_SET_CAPACITY; j++)
		argvalue[j] = new uint32_t[2];
	for (i = 0; i < ABILITY_SET_AMOUNT; i++) {
		for (j = 0; j < ABILITY_SET_CAPACITY; j++) {
			argvalue[j][0] = ability_list[i].entry[j].ability.GetRawId();
			argvalue[j][1] = ability_list[i].entry[j].ap_cost;
		}
		res[modcounter] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position_abilset[i], steam_method_base_length_abilset[i], ABILITY_SET_CAPACITY, 2, steam_statabil_field_size);
		modcounter++;
	}
	for (j = 0; j < ABILITY_SET_CAPACITY; j++)
		delete[] argvalue[j];
	delete[] argvalue;
	res[modcounter].position = config.meta_dll.GetStaticFieldOffset(config.dll_statcmdtrance_field_id);
	res[modcounter].base_length = 3 * COMMAND_SET_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_statcmdtrance_field_id);
	res[modcounter].new_length = 3 * COMMAND_SET_AMOUNT;
	res[modcounter].value = new uint8_t[res[modcounter].new_length];
	for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
		res[modcounter].value[i * 3] = command_list[i].first_command_trance;
		res[modcounter].value[i * 3 + 1] = command_list[i].second_command_trance;
		res[modcounter].value[i * 3 + 2] = command_list[i].trance_attack;
	}
	modcounter++;
	res[modcounter].position = config.meta_dll.GetStaticFieldOffset(config.dll_statexp_field_id);
	res[modcounter].base_length = 8 * MAX_LEVEL; // config.meta_dll.GetStaticFieldRange(config.dll_statexp_field_id);
	res[modcounter].new_length = 8 * MAX_LEVEL;
	res[modcounter].value = new uint8_t[res[modcounter].new_length];
	BufferInitPosition();
	for (i = 0; i < MAX_LEVEL; i++)
		BufferWriteLongLong(res[modcounter].value, level[i].exp_table);
	modcounter++;
	argvalue = new uint32_t * [PLAYABLE_CHAR_AMOUNT];
	for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++) {
		argvalue[i] = new uint32_t[5];
		argvalue[i][0] = initial_stat[i].speed;
		argvalue[i][1] = initial_stat[i].strength;
		argvalue[i][2] = initial_stat[i].magic;
		argvalue[i][3] = initial_stat[i].spirit;
		argvalue[i][4] = initial_stat[i].magic_stone;
	}
	res[modcounter] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position_initstat, steam_method_base_length_initstat, PLAYABLE_CHAR_AMOUNT, 5, steam_stat_field_size);
	for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	modcounter++;
	argvalue = new uint32_t * [MAX_LEVEL];
	for (i = 0; i < MAX_LEVEL; i++) {
		argvalue[i] = new uint32_t[2];
		argvalue[i][0] = level[i].hp_table;
		argvalue[i][1] = level[i].mp_table;
	}
	res[modcounter] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position_hpmp, steam_method_base_length_hpmp, MAX_LEVEL, 2, steam_lvlhpmp_field_size);
	for (i = 0; i < MAX_LEVEL; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	modcounter++;
	argvalue = new uint32_t * [COMMAND_SET_AMOUNT];
	for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
		argvalue[i] = new uint32_t[2];
		argvalue[i][0] = command_list[i].first_command;
		argvalue[i][1] = command_list[i].second_command;
	}
	res[modcounter] = dlldata.ConvertRawToScript_Array2(argvalue, steam_method_position_cmdset, steam_method_base_length_cmdset, COMMAND_SET_AMOUNT, 2, dlldata.GetMemberTokenIdentifier("void valuetype FF9.command_tags[,]::Set( int, int, valuetype FF9.command_tags )"));
	for (i = 0; i < COMMAND_SET_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	modcounter++;
	if (config.dll_cmdtrancenones_size > 0) {
		res[modcounter].position = config.dll_cmdtrancenones_offset;
		res[modcounter].base_length = config.dll_cmdtrancenones_size;
		res[modcounter].new_length = 0;
		res[modcounter].value = new uint8_t[0];
		*modifamount = EQUIP_SET_AMOUNT + ABILITY_SET_AMOUNT + 6;
	} else {
		*modifamount = EQUIP_SET_AMOUNT + ABILITY_SET_AMOUNT + 5;
	}
	return res;
}

void StatDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i, j;
	stringstream equipsetdb;
	equipsetdb << "// Method: ff9play::FF9Play_SetDefEquips\n\n";
	equipsetdb << "\tbyte[][] array = new byte[][] {\n";
	for (i = 0; i < EQUIP_SET_AMOUNT; i++)
		equipsetdb << "\t\tnew byte[] { " << (int)initial_equip[i].weapon << ", " << (int)initial_equip[i].head << ", " << (int)initial_equip[i].wrist << ", " << (int)initial_equip[i].armor << ", " << (int)initial_equip[i].accessory << (i + 1 == EQUIP_SET_AMOUNT ? "}" : "},") << " // " << HADES_STRING_EQUIPSET_NAME[i] << "\n";
	equipsetdb << "\t};\n";
	equipsetdb << "\tfor (int i = 0; i < 5; i++) {\n";
	equipsetdb << "\t\tequip[i] = array[eqp_id][i];\n";
	equipsetdb << "\t};\n";
	buffer.push_back(equipsetdb.str());
	stringstream learndb;
	learndb << "// Method: FF9.ff9abil::.cctor\n\n";
	learndb << "\tff9abil._FF9Abil_PaData = new PA_DATA[][] {\n";
	for (i = 0; i < ABILITY_SET_AMOUNT; i++) {
		learndb << "\t\tnew PA_DATA[] { // " << HADES_STRING_ABILITYSET_NAME[i] << "\n";
		for (j = 0; j < ABILITY_SET_CAPACITY; j++)
			learndb << "\t\t\tnew PA_DATA(" << (int)ability_list[i].entry[j].ability.GetRawId() << ", " << (int)ability_list[i].entry[j].ap_cost << (j + 1 == ABILITY_SET_CAPACITY ? ")\n" : "),\n");
		learndb << (i + 1 == ABILITY_SET_AMOUNT ? "\t\t}\n" : "\t\t},\n");
	}
	learndb << "\t};\n";
	buffer.push_back(learndb.str());
	stringstream rdatadb;
	rdatadb << "// Method: rdata::.cctor\n\n";
	rdatadb << "\trdata._FF9BMenu_MenuNormal = new command_tags[,] {\n";
	for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
		rdatadb << "\t\t{ command_tags." << SteamCommandTagName[command_list[i].first_command] << ", command_tags."
			<< SteamCommandTagName[command_list[i].second_command] << (i + 1 == COMMAND_SET_AMOUNT ? " }" : " },") << " // " << HADES_STRING_ABILITYSET_NAME[i] << "\n";
	}
	rdatadb << "\t};\n";
	rdatadb << "\trdata._FF9BMenu_MenuTrance = new byte[,] {\n";
	for (i = 0; i < COMMAND_SET_AMOUNT; i++) {
		rdatadb << "\t\t{ (byte)command_tags." << SteamCommandTagName[command_list[i].first_command_trance] << ", (byte)command_tags." << SteamCommandTagName[command_list[i].second_command_trance]
			<< ", " << (int)command_list[i].trance_attack << (i + 1 == COMMAND_SET_AMOUNT ? " }" : " },") << " // " << HADES_STRING_ABILITYSET_NAME[i] << "\n";
	}
	rdatadb << "\t};\n";
	buffer.push_back(rdatadb.str());
	stringstream leveldb;
	leveldb << "// Method: ff9level::.cctor\n\n";
	leveldb << "\tff9level._FF9Level_Base = new FF9LEVEL_BASE[] {\n";
	for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++)
		leveldb << "\t\tnew FF9LEVEL_BASE(" << (int)initial_stat[i].speed << ", " << (int)initial_stat[i].strength << ", " << (int)initial_stat[i].magic << ", " << (int)initial_stat[i].spirit << ", " << (int)initial_stat[i].magic_stone << (i + 1 == PLAYABLE_CHAR_AMOUNT ? ")" : "),") << " // " << ConvertWStrToStr(initial_stat[i].default_name.str_nice) << "\n";
	leveldb << "\t};\n";
	leveldb << "\tff9level._FF9Level_HpMp = new FF9LEVEL_HPMP[] {\n";
	for (i = 0; i < MAX_LEVEL; i++) {
		leveldb << "\t\tnew FF9LEVEL_HPMP(" << (int)level[i].hp_table << ", " << (int)level[i].mp_table << (i + 1 == MAX_LEVEL ? ")" : "),");
		if ((i % 10) == 9) leveldb << " // Level " << i + 1 << "\n";
		else leveldb << "\n";
	}
	leveldb << "\t};\n";
	leveldb << "\tff9level._FF9Level_Exp = new ulong[] {\n";
	for (i = 0; i < MAX_LEVEL; i++) {
		leveldb << "\t\t" << (unsigned long)level[i].exp_table << (i + 1 == MAX_LEVEL ? "UL" : "UL,");
		if ((i % 10) == 9) leveldb << " // Level " << i + 1 << "\n";
		else leveldb << "\n";
	}
	leveldb << "\t};\n";
	buffer.push_back(leveldb.str());
}

bool CSV_AbilitySetGenerator(wxString modfolder, wxString csvpath, wxString customheader, wxString format, AbilitySetDataStruct& as) {
	wxArrayString abilbasecsv;
	if (as.id < ABILITY_SET_AMOUNT && GetGameConfiguration() != NULL)
		abilbasecsv = MemoriaUtility::LoadCSVLines(_(GetGameConfiguration()->steam_dir_assets) + csvpath);
	bool generateabil = abilbasecsv.GetCount() != as.entry.size();
	unsigned int i;
	if (!generateabil) {
		for (i = 0; i < abilbasecsv.GetCount(); i++) {
			wxArrayString abilentry = MemoriaUtility::LoadCSVEntry(abilbasecsv[i]);
			if (abilentry.GetCount() < 2 || !abilentry[0].IsSameAs(as.entry[i].ability.GetStringId()) || !abilentry[1].IsSameAs(to_string(as.entry[i].ap_cost))) {
				generateabil = true;
				break;
			}
		}
		if (!generateabil)
			return true;
	}
	wxString header =	!customheader.IsEmpty() ? customheader :
						as.id < (int)HADES_STRING_CSV_STATABIL_HEADER.size() ? _(HADES_STRING_CSV_STATABIL_HEADER[as.id]) :
						_(HADES_STRING_CSV_STATABIL_DEFAULT_HEADER);
	return MemoriaUtility::GenerateDatabaseGeneric<AbilityEntryDataStruct>(modfolder, csvpath, header, _(L"\n"), _(L"\n"), as.entry, format, false);
}

bool StatDataSet::GenerateCSV(string basefolder) {
	unsigned int i, j;
	if (!MemoriaUtility::GenerateDatabaseGeneric<CharBattleParameterStruct>(_(basefolder), _(HADES_STRING_CSV_BATTLEPARAM_FILE), csv_header_battle_param, _(L"\n"), _(L"\n"), battle_param, csv_format_battle_param, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<InitialStatDataStruct>(_(basefolder), _(HADES_STRING_CSV_CHARPARAM_FILE), csv_header_character_param, _(L"\n"), _(L"\n"), initial_stat, csv_format_character_param, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<InitialStatDataStruct>(_(basefolder), _(HADES_STRING_CSV_STATINIT_FILE), csv_header_initial_stat, _(L"\n"), _(L"\n"), initial_stat, csv_format_initial_stat, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<CommandSetDataStruct>(_(basefolder), _(HADES_STRING_CSV_STATCMD_FILE), csv_header_command_list, _(L"\n"), _(L"\n"), command_list, csv_format_command_list, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<InitialEquipDataStruct>(_(basefolder), _(HADES_STRING_CSV_STATEQUIP_FILE), csv_header_initial_equip, _(L"\n"), _(L"\n"), initial_equip, csv_format_initial_equip, true))
		return false;
	for (unsigned int i = 0; i < ability_list.size(); i++) {
		if (ability_list[i].entry.size() == 0) // Theater sets
			continue;
		if (ability_list[i].id < (int)HADES_STRING_CSV_STATABIL_FILE.size()) {
			if (!CSV_AbilitySetGenerator(_(basefolder), _(HADES_STRING_CSV_STATABIL_FILE[ability_list[i].id]), csv_header_ability_list, csv_format_ability_list, ability_list[i]))
				return false;
		} else {
			if (!CSV_AbilitySetGenerator(_(basefolder), wxString::Format(wxT("%s%d.csv"), HADES_STRING_CSV_STATABIL_DEFAULT_FILE, ability_list[i].id), csv_header_ability_list, csv_format_ability_list, ability_list[i]))
				return false;
		}
	}
	wxString equippatchstr = _(L"");
	for (i = 0; i < initial_stat.size(); i++) {
		if (initial_stat[i].equip_patch.size() > 0) {
			vector<vector<int>> equipbytype(6);
			equippatchstr += wxString::Format(wxT("// %s:\n"), initial_stat[i].default_name.str);
			for (j = 0; j < initial_stat[i].equip_patch.size(); j++) {
				if (GetGameSaveSet() == NULL || !GetGameSaveSet()->sectionloaded[DATA_SECTION_ITEM]) {
					equipbytype[5].push_back(initial_stat[i].equip_patch[j]);
					continue;
				}
				ItemDataStruct& it = GetGameSaveSet()->itemset->GetItemById(initial_stat[i].equip_patch[j]);
				if ((it.type & ITEM_TYPE_ANY_EQUIP) == ITEM_TYPE_WEAPON)
					equipbytype[0].push_back(initial_stat[i].equip_patch[j]);
				else if ((it.type & ITEM_TYPE_ANY_EQUIP) == ITEM_TYPE_WRIST)
					equipbytype[1].push_back(initial_stat[i].equip_patch[j]);
				else if ((it.type & ITEM_TYPE_ANY_EQUIP) == ITEM_TYPE_HEAD)
					equipbytype[2].push_back(initial_stat[i].equip_patch[j]);
				else if ((it.type & ITEM_TYPE_ANY_EQUIP) == ITEM_TYPE_ARMOR)
					equipbytype[3].push_back(initial_stat[i].equip_patch[j]);
				else if ((it.type & ITEM_TYPE_ANY_EQUIP) == ITEM_TYPE_ACCESSORY)
					equipbytype[4].push_back(initial_stat[i].equip_patch[j]);
				else
					equipbytype[5].push_back(initial_stat[i].equip_patch[j]);
			}
			for (j = 0; j < 6; j++)
				if (!equipbytype[j].empty())
					equippatchstr += wxString::Format(wxT("%d Add %s\n"), initial_stat[i].id, ConcatenateStrings<int>(" ", equipbytype[j], static_cast<string(*)(int)>(&to_string)));
			equippatchstr += _(L"\n");
		}
	}
	if (!equippatchstr.IsEmpty()) {
		wxFile equiptxtfile;
		if (!equiptxtfile.Open(_(basefolder) + _(HADES_STRING_EQUIP_PATCH_FILE), wxFile::write))
			return false;
		if (!equiptxtfile.Write(equippatchstr))
			return false;
		equiptxtfile.Close();
	}
	bool generatelevels = custom_field_level.size() > 0;
	if (!generatelevels) {
		wxArrayString levelbasecsv;
		if (GetGameConfiguration() != NULL)
			levelbasecsv = MemoriaUtility::LoadCSVLines(_(GetGameConfiguration()->steam_dir_assets) + _(HADES_STRING_CSV_STATLEVEL_FILE));
		generatelevels = levelbasecsv.GetCount() != level.size();
		if (!generatelevels) {
			for (i = 0; i < levelbasecsv.GetCount(); i++) {
				wxArrayString levelentry = MemoriaUtility::LoadCSVEntry(levelbasecsv[i]);
				if (levelentry.GetCount() < 3 || wxAtol(levelentry[0]) != level[i].exp_table || wxAtoi(levelentry[1]) != level[i].hp_table || wxAtoi(levelentry[2]) != level[i].mp_table) {
					generatelevels = true;
					break;
				}
			}
		}
	}
	if (generatelevels && !MemoriaUtility::GenerateDatabaseGeneric<LevelDataStruct>(_(basefolder), _(HADES_STRING_CSV_STATLEVEL_FILE), csv_header_level, _(L"\n"), _(L"\n"), level, csv_format_level, false))
		return false;
	return true;
}

void StatDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	int charamount = PLAYABLE_CHAR_AMOUNT;
	int equipamount = EQUIP_SET_AMOUNT;
	int abilsetamount = ABILITY_SET_AMOUNT;
	int cmdsetamount = COMMAND_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i, j;
	UpdateOffset();
	ffbin.seekg(config.stat_defaultname_offset);
	MACRO_STAT_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_initialequip_offset);
	MACRO_STAT_IOFUNCTIONEQUIP(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONABIL(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_initialstat_offset);
	MACRO_STAT_IOFUNCTIONSTAT(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONLEVEL(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONLEVELEXP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_cmddata_offset[0]);
	MACRO_STAT_IOFUNCTIONCOMMAND(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONTRANCECOMMAND(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_cmddata_offset[1]);
	MACRO_STAT_IOFUNCTIONCOMMAND(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_cmddata_offset[2]);
	MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONCOMMAND(FFIXWrite,FFIXSeek,false,false)
}

void StatDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	int charamount = PLAYABLE_CHAR_AMOUNT;
	int equipamount = EQUIP_SET_AMOUNT;
	int abilsetamount = ABILITY_SET_AMOUNT;
	int cmdsetamount = COMMAND_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i,j;
	UpdateOffset();
	ffbin.seekg(config.stat_defaultname_offset);
	MACRO_STAT_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_initialequip_offset);
	MACRO_STAT_IOFUNCTIONEQUIP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONABIL(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_initialstat_offset);
	MACRO_STAT_IOFUNCTIONSTAT(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONLEVEL(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONLEVELEXP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_cmddata_offset[0]);
	MACRO_STAT_IOFUNCTIONCOMMAND(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONTRANCECOMMAND(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_cmddata_offset[1]);
	MACRO_STAT_IOFUNCTIONCOMMAND(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_cmddata_offset[2]);
	MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONCOMMAND(PPFStepAdd,FFIXSeek,false,true)
}

int StatDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	int btlamount = CHAR_BATTLE_PARAMETER_AMOUNT;
	int charamount = PLAYABLE_CHAR_AMOUNT;
	int equipamount = EQUIP_SET_AMOUNT;
	int abilsetamount = ABILITY_SET_AMOUNT;
	int cmdsetamount = COMMAND_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	vector<CharBattleParameterStruct> nonmodifiedbtl;
	vector<InitialStatDataStruct> nonmodifiedchar;
	vector<InitialEquipDataStruct> nonmodifiedequip;
	vector<AbilitySetDataStruct> nonmodifiedabilset;
	vector<CommandSetDataStruct> nonmodifiedcmdset;
	int i, j;
	int res = 0;
	uint16_t version;
	uint16_t defnamesize = default_name_space_total;
	HWSReadShort(ffbin, version);
	HWSReadShort(ffbin, default_name_space_total);
	if (version >= 3) {
		useextendedtype = true;
		vector<int> added;
		btlamount = PrepareHWSFlexibleList(ffbin, battle_param, nonmodifiedbtl, added);
		for (i = 0; i < (int)added.size(); i++)
			battle_param[added[i]].parent = this;
		charamount = PrepareHWSFlexibleList(ffbin, initial_stat, nonmodifiedchar, added);
		for (i = 0; i < (int)added.size(); i++) {
			initial_stat[added[i]].GenerateDefaultName();
			initial_stat[added[i]].battle_param = to_wstring(initial_stat[added[i]].id + 7);
			initial_stat[added[i]].parent = this;
		}
		equipamount = PrepareHWSFlexibleList(ffbin, initial_equip, nonmodifiedequip, added);
		for (i = 0; i < (int)added.size(); i++)
			initial_equip[added[i]].parent = this;
		abilsetamount = PrepareHWSFlexibleList(ffbin, ability_list, nonmodifiedabilset, added);
		for (i = 0; i < (int)added.size(); i++) {
			AbilitySetDataStruct& abilset = ability_list[added[i]];
			abilset.parent = this;
			abilset.entry.resize(ABILITY_SET_CAPACITY);
			for (j = 0; j < (int)abilset.entry.size(); j++) {
				abilset.entry[j].id = j;
				abilset.entry[j].parent = &abilset;
			}
		}
		cmdsetamount = PrepareHWSFlexibleList(ffbin, command_list, nonmodifiedcmdset, added);
		for (i = 0; i < (int)added.size(); i++) {
			command_list[added[i]].InitializeDefaultCommands();
			command_list[added[i]].parent = this;
		}
	}
	if (version >= 4) {
		useextendedtype2 = true;
		HWSReadCSVFormatting(ffbin, csv_header_battle_param, csv_format_battle_param);
		HWSReadCSVFields(ffbin, custom_field_battle_param);
		HWSReadCSVFormatting(ffbin, csv_header_character_param, csv_format_character_param);
		HWSReadCSVFormatting(ffbin, csv_header_initial_stat, csv_format_initial_stat);
		HWSReadCSVFields(ffbin, custom_field_initial_stat);
		HWSReadCSVFormatting(ffbin, csv_header_initial_equip, csv_format_initial_equip);
		HWSReadCSVFields(ffbin, custom_field_initial_equip);
		HWSReadCSVFormatting(ffbin, csv_header_ability_list, csv_format_ability_list);
		HWSReadCSVFields(ffbin, custom_field_ability_list);
		HWSReadCSVFormatting(ffbin, csv_header_command_list, csv_format_command_list);
		HWSReadCSVFields(ffbin, custom_field_command_list);
		HWSReadCSVFormatting(ffbin, csv_header_level, csv_format_level);
		HWSReadCSVFields(ffbin, custom_field_level);
	}
	if (default_name_space_total <= defnamesize && usetext) {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			MACRO_STAT_IOFUNCTIONNAME(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < charamount; i++)
					initial_stat[i].default_name.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin, txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX) {
					for (i = 0; i < charamount; i++)
						SteamReadFF9String(ffbin, initial_stat[i].default_name, lg);
				} else if (lg == GetSteamLanguage()) {
					for (i = 0; i < charamount; i++) {
						SteamReadFF9String(ffbin, initial_stat[i].default_name);
						initial_stat[i].default_name.SteamToPSX();
					}
				}
				ffbin.seekg(tmppos + txtspace);
				HWSReadChar(ffbin, lg);
			}
		}
	} else {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			ffbin.seekg(default_name_space_total + 4, ios::cur);
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin, txtspace);
				ffbin.seekg(txtspace, ios::cur);
				HWSReadChar(ffbin, lg);
			}
		}
		if (usetext && GetGameType() == GAME_TYPE_PSX)
			res |= 1;
	}
	MACRO_STAT_IOFUNCTIONEQUIP(HWSRead, HWSSeek, true, false)
	MACRO_STAT_IOFUNCTIONABIL(HWSRead, HWSSeek, true, false)
	MACRO_STAT_IOFUNCTIONSTAT(HWSRead, HWSSeek, true, false)
	MACRO_STAT_IOFUNCTIONLEVEL(HWSRead, HWSSeek, true, false)
	MACRO_STAT_IOFUNCTIONLEVELEXP(HWSRead, HWSSeek, true, false)
	if (version >= 2) {
		MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(HWSRead, HWSSeek, true, false)
		MACRO_STAT_IOFUNCTIONCOMMAND(HWSRead, HWSSeek, true, false)
		MACRO_STAT_IOFUNCTIONTRANCECOMMAND(HWSRead, HWSSeek, true, false)
	}
	if (version >= 3) {
		for (i = 0; i < btlamount; i++) {
			HWSReadWString(ffbin, battle_param[i].avatar_sprite);
			HWSReadFlexibleChar(ffbin, battle_param[i].attack_sfx, true);
			HWSReadFlexibleChar(ffbin, battle_param[i].attack_sound[0], true);
			HWSReadFlexibleChar(ffbin, battle_param[i].attack_sound[1], true);
			HWSReadChar(ffbin, battle_param[i].weapon_bone);
			for (j = 0; j < 6; j++) {
				HWSReadChar(ffbin, battle_param[i].status_bone[j]);
				HWSReadChar(ffbin, (uint8_t&)battle_param[i].status_offy[j]);
				HWSReadChar(ffbin, (uint8_t&)battle_param[i].status_offz[j]);
			}
			HWSReadChar(ffbin, battle_param[i].shadow_bone[0]);
			HWSReadChar(ffbin, battle_param[i].shadow_bone[1]);
			HWSReadChar(ffbin, battle_param[i].shadow_size[0]);
			HWSReadChar(ffbin, battle_param[i].shadow_size[1]);
			HWSReadChar(ffbin, battle_param[i].shadow_size[2]);
			HWSReadWString(ffbin, battle_param[i].model);
			HWSReadWString(ffbin, battle_param[i].model_trance);
			for (j = 0; j < BATTLE_MOTION_AMOUNT; j++)
				HWSReadWString(ffbin, battle_param[i].anim[j]);
			HWSReadChar(ffbin, battle_param[i].trance_color[0]);
			HWSReadChar(ffbin, battle_param[i].trance_color[1]);
			HWSReadChar(ffbin, battle_param[i].trance_color[2]);
			if (version >= 4)
				HWSReadCSVFields(ffbin, battle_param[i].custom_field);
		}
		for (i = 0; i < charamount; i++) {
			int equipcount;
			HWSReadWString(ffbin, initial_stat[i].name_keyword);
			HWSReadChar(ffbin, initial_stat[i].default_row);
			HWSReadChar(ffbin, initial_stat[i].default_winpose);
			HWSReadChar(ffbin, initial_stat[i].default_category);
			if (version >= 4)
				HWSReadWString(ffbin, initial_stat[i].battle_param);
			HWSReadFlexibleChar(ffbin, equipcount, true);
			initial_stat[i].equip_patch.resize(equipcount);
			for (j = 0; j < equipcount; j++)
				HWSReadFlexibleChar(ffbin, initial_stat[i].equip_patch[j], true);
		}
	}
	for (i = 0; i < (int)nonmodifiedbtl.size(); i++)
		InsertAtId(battle_param, nonmodifiedbtl[i], nonmodifiedbtl[i].id);
	for (i = 0; i < (int)nonmodifiedchar.size(); i++)
		InsertAtId(initial_stat, nonmodifiedchar[i], nonmodifiedchar[i].id);
	for (i = 0; i < (int)nonmodifiedequip.size(); i++)
		InsertAtId(initial_equip, nonmodifiedequip[i], nonmodifiedequip[i].id);
	for (i = 0; i < (int)nonmodifiedabilset.size(); i++)
		InsertAtId(ability_list, nonmodifiedabilset[i], nonmodifiedabilset[i].id);
	for (i = 0; i < (int)nonmodifiedcmdset.size(); i++)
		InsertAtId(command_list, nonmodifiedcmdset[i], nonmodifiedcmdset[i].id);
	default_name_space_total = defnamesize;
	UpdateOffset();
	return res;
}

void StatDataSet::WriteHWS(fstream& ffbin) {
	int btlamount = battle_param.size();
	int charamount = initial_stat.size();
	int equipamount = initial_equip.size();
	int abilsetamount = ability_list.size();
	int cmdsetamount = command_list.size();
	bool useextendedtype = true;
	bool useextendedtype2 = true;
	int i, j;
	UpdateOffset();
	HWSWriteShort(ffbin, STAT_HWS_VERSION);
	uint16_t defnamesize = default_name_space_total;
	default_name_space_total = default_name_space_used;
	HWSWriteShort(ffbin, default_name_space_total);
	HWSWriteFlexibleChar(ffbin, btlamount, true);
	for (i = 0; i < btlamount; i++)
		HWSWriteFlexibleChar(ffbin, battle_param[i].id, true);
	HWSWriteFlexibleChar(ffbin, charamount, true);
	for (i = 0; i < charamount; i++)
		HWSWriteFlexibleChar(ffbin, initial_stat[i].id, true);
	HWSWriteFlexibleChar(ffbin, equipamount, true);
	for (i = 0; i < equipamount; i++)
		HWSWriteFlexibleChar(ffbin, initial_equip[i].id, true);
	HWSWriteFlexibleChar(ffbin, abilsetamount, true);
	for (i = 0; i < abilsetamount; i++)
		HWSWriteFlexibleChar(ffbin, ability_list[i].id, true);
	HWSWriteFlexibleChar(ffbin, cmdsetamount, true);
	for (i = 0; i < cmdsetamount; i++)
		HWSWriteFlexibleChar(ffbin, command_list[i].id, true);
	HWSWriteCSVFormatting(ffbin, csv_header_battle_param, csv_format_battle_param);
	HWSWriteCSVFields(ffbin, custom_field_battle_param);
	HWSWriteCSVFormatting(ffbin, csv_header_character_param, csv_format_character_param);
	HWSWriteCSVFormatting(ffbin, csv_header_initial_stat, csv_format_initial_stat);
	HWSWriteCSVFields(ffbin, custom_field_initial_stat);
	HWSWriteCSVFormatting(ffbin, csv_header_initial_equip, csv_format_initial_equip);
	HWSWriteCSVFields(ffbin, custom_field_initial_equip);
	HWSWriteCSVFormatting(ffbin, csv_header_ability_list, csv_format_ability_list);
	HWSWriteCSVFields(ffbin, custom_field_ability_list);
	HWSWriteCSVFormatting(ffbin, csv_header_command_list, csv_format_command_list);
	HWSWriteCSVFields(ffbin, custom_field_command_list);
	HWSWriteCSVFormatting(ffbin, csv_header_level, csv_format_level);
	HWSWriteCSVFields(ffbin, custom_field_level);
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_STAT_IOFUNCTIONNAME(HWSWrite, HWSSeek, false, false)
	} else {
		SteamLanguage lg;
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteShort(ffbin, GetSteamTextSizeGeneric(initial_stat, &InitialStatDataStruct::default_name, false, lg));
				WriteSteamTextGeneric(ffbin, initial_stat, &InitialStatDataStruct::default_name, NULL, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
	}
	MACRO_STAT_IOFUNCTIONEQUIP(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONABIL(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONSTAT(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONLEVEL(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONLEVELEXP(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONCOMMAND(HWSWrite, HWSSeek, false, false)
	MACRO_STAT_IOFUNCTIONTRANCECOMMAND(HWSWrite, HWSSeek, false, false)
	for (i = 0; i < btlamount; i++) {
		HWSWriteWString(ffbin, battle_param[i].avatar_sprite);
		HWSWriteFlexibleChar(ffbin, battle_param[i].attack_sfx, true);
		HWSWriteFlexibleChar(ffbin, battle_param[i].attack_sound[0], true);
		HWSWriteFlexibleChar(ffbin, battle_param[i].attack_sound[1], true);
		HWSWriteChar(ffbin, battle_param[i].weapon_bone);
		for (j = 0; j < 6; j++) {
			HWSWriteChar(ffbin, battle_param[i].status_bone[j]);
			HWSWriteChar(ffbin, (uint8_t&)battle_param[i].status_offy[j]);
			HWSWriteChar(ffbin, (uint8_t&)battle_param[i].status_offz[j]);
		}
		HWSWriteChar(ffbin, battle_param[i].shadow_bone[0]);
		HWSWriteChar(ffbin, battle_param[i].shadow_bone[1]);
		HWSWriteChar(ffbin, battle_param[i].shadow_size[0]);
		HWSWriteChar(ffbin, battle_param[i].shadow_size[1]);
		HWSWriteChar(ffbin, battle_param[i].shadow_size[2]);
		HWSWriteWString(ffbin, battle_param[i].model);
		HWSWriteWString(ffbin, battle_param[i].model_trance);
		for (j = 0; j < BATTLE_MOTION_AMOUNT; j++)
			HWSWriteWString(ffbin, battle_param[i].anim[j]);
		HWSWriteChar(ffbin, battle_param[i].trance_color[0]);
		HWSWriteChar(ffbin, battle_param[i].trance_color[1]);
		HWSWriteChar(ffbin, battle_param[i].trance_color[2]);
		HWSWriteCSVFields(ffbin, battle_param[i].custom_field);
	}
	for (i = 0; i < charamount; i++) {
		HWSWriteWString(ffbin, initial_stat[i].name_keyword);
		HWSWriteChar(ffbin, initial_stat[i].default_row);
		HWSWriteChar(ffbin, initial_stat[i].default_winpose);
		HWSWriteChar(ffbin, initial_stat[i].default_category);
		HWSWriteWString(ffbin, initial_stat[i].battle_param);
		HWSWriteFlexibleChar(ffbin, initial_stat[i].equip_patch.size(), true);
		for (j = 0; j < (int)initial_stat[i].equip_patch.size(); j++)
			HWSWriteFlexibleChar(ffbin, initial_stat[i].equip_patch[j], true);
	}
	default_name_space_total = defnamesize;
}

void StatDataSet::UpdateOffset() {
	uint16_t j;
	int i;
	if (GetGameType() == GAME_TYPE_PSX) {
		j = PLAYABLE_CHAR_AMOUNT * 4;
		for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++) {
			initial_stat[i].default_name_offset = j;
			j += initial_stat[i].default_name.length;
		}
		default_name_space_used = j;
	} else {
		j = 0;
		for (i = 0; i < PLAYABLE_CHAR_AMOUNT; i++)
			j += initial_stat[i].default_name.length;
		default_name_space_used = j;
	}
}
