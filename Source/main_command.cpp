#include "main_command.h"

#include <wx/tokenzr.h>
#include <set>
#include <functional>
#include "Gui_LoadingDialog.h"
#include "Hades_Strings.h"
#include "File_Batching.h"
#include "main.h"

#define COMMA ,

// Database

wxString ConvertFF9StringToArg(wstring str) {
	wxString result = wxString(str);
	result.Replace(L"\n", L"[NEWLINE]");
	result.Replace(L"\"", L"[QUOTE]");
	return result;
}

wstring ConvertArgToFF9String(wxString str) {
	str.Replace(L"[NEWLINE]", L"\n");
	str.Replace(L"[QUOTE]", L"\"");
	return str.ToStdWstring();
}

static const wxString const_section_name[DATA_SECTION_AMOUNT] = {
	L"spell",
	L"command",
	L"battle",
	L"shop",
	L"text",
	L"battle_scene",
	L"world",
	L"item",
	L"support",
	L"stat",
	L"card",
	L"field",
	L"spell_animation",
	L"interface",
	L"party_special",
	L"assembly"
};

struct editable_field {
	wxString name;
	function<bool(SaveSet&, FieldSelector&)> checker;

	editable_field(wxString n, function<bool(SaveSet&, FieldSelector&)> c) : name(n), checker(c) {}
	virtual int get_type() = 0;
};
struct editable_field_numerical : public editable_field {
	function<void(SaveSet&, FieldSelector&, long long)> setter;
	function<long long(SaveSet&, FieldSelector&)> getter;

	editable_field_numerical(wxString n, function<bool(SaveSet&, FieldSelector&)> c, function<void(SaveSet&, FieldSelector&, long long)> s, function<long long(SaveSet&, FieldSelector&)> g) : editable_field(n, c), setter(s), getter(g) {}
	virtual int get_type() { return 0; }
};
struct editable_field_string : public editable_field {
	function<void(SaveSet&, FieldSelector&, wxString)> setter;
	function<wxString(SaveSet&, FieldSelector&)> getter;

	editable_field_string(wxString n, function<bool(SaveSet&, FieldSelector&)> c, function<void(SaveSet&, FieldSelector&, wxString)> s, function<wxString(SaveSet&, FieldSelector&)> g) : editable_field(n, c), setter(s), getter(g) {}
	virtual int get_type() { return 1; }
};

#define EDITABLE_FIELD_COUNT 360
editable_field** const_editable_field = NULL;
void InitConstEditableField() {
	if (const_editable_field != NULL)
		return;
	const_editable_field = new editable_field*[EDITABLE_FIELD_COUNT] {
		new editable_field_string(L"spell.name", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.spellset->spell[sel.spell_index].SetName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.spellset->spell[sel.spell_index].name.str); }),
		new editable_field_string(L"spell.help", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.spellset->spell[sel.spell_index].SetHelp(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.spellset->spell[sel.spell_index].help.str); }),
		new editable_field_numerical(L"spell.target_type", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].target_type = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].target_type; }),
		new editable_field_numerical(L"spell.model", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].model = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].model; }),
		new editable_field_numerical(L"spell.target_flag", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].target_flag = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].target_flag; }),
		new editable_field_numerical(L"spell.effect", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].effect = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].effect; }),
		new editable_field_numerical(L"spell.power", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].power = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].power; }),
		new editable_field_numerical(L"spell.element", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].element = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].element; }),
		new editable_field_numerical(L"spell.accuracy", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].accuracy = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].accuracy; }),
		new editable_field_numerical(L"spell.flag", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].flag = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].flag; }),
		new editable_field_numerical(L"spell.status", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].status = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].status; }),
		new editable_field_numerical(L"spell.mp", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].mp = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].mp; }),
		new editable_field_numerical(L"spell.menu_flag", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].menu_flag = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].menu_flag; }),
		new editable_field_numerical(L"spell.model_alt", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].model_alt = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].model_alt; }),
		new editable_field_numerical(L"spell.help_size_x", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].help_size_x = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].help_size_x; }),
		new editable_field_numerical(L"spell.perform_name", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_index < SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->spell[sel.spell_index].perform_name = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->spell[sel.spell_index].perform_name; }),
		new editable_field_numerical(L"status_set.status", [](SaveSet& sv, FieldSelector& sel) { return sel.status_set_index < STATUS_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellset->status_set[sel.status_set_index].status = val; sv.sectionmodified[DATA_SECTION_SPELL] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellset->status_set[sel.status_set_index].status; }),
		new editable_field_string(L"support.name", [](SaveSet& sv, FieldSelector& sel) { return sel.support_index < SUPPORT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.supportset->support[sel.support_index].SetName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_SUPPORT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.supportset->support[sel.support_index].name.str); }),
		new editable_field_string(L"support.help", [](SaveSet& sv, FieldSelector& sel) { return sel.support_index < SUPPORT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.supportset->support[sel.support_index].SetHelp(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_SUPPORT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.supportset->support[sel.support_index].help.str); }),
		new editable_field_numerical(L"support.help_size_x", [](SaveSet& sv, FieldSelector& sel) { return sel.support_index < SUPPORT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.supportset->support[sel.support_index].help_size_x = val; sv.sectionmodified[DATA_SECTION_SUPPORT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.supportset->support[sel.support_index].help_size_x; }),
		new editable_field_numerical(L"support.category", [](SaveSet& sv, FieldSelector& sel) { return sel.support_index < SUPPORT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.supportset->support[sel.support_index].category = val; sv.sectionmodified[DATA_SECTION_SUPPORT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.supportset->support[sel.support_index].category; }),
		new editable_field_numerical(L"support.cost", [](SaveSet& sv, FieldSelector& sel) { return sel.support_index < SUPPORT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.supportset->support[sel.support_index].cost = val; sv.sectionmodified[DATA_SECTION_SUPPORT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.supportset->support[sel.support_index].cost; }),
		new editable_field_string(L"command.name", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.cmdset->cmd[sel.command_index].SetName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.cmdset->cmd[sel.command_index].name.str); }),
		new editable_field_string(L"command.help", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.cmdset->cmd[sel.command_index].SetHelp(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.cmdset->cmd[sel.command_index].help.str); }),
		new editable_field_numerical(L"command.panel", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cmdset->cmd[sel.command_index].panel = val; sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cmdset->cmd[sel.command_index].panel; }),
		new editable_field_numerical(L"command.spell_amount", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cmdset->cmd[sel.command_index].spell_amount = val; sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cmdset->cmd[sel.command_index].spell_amount; }),
		new editable_field_numerical(L"command.help_size_x", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cmdset->cmd[sel.command_index].help_size_x = val; sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cmdset->cmd[sel.command_index].help_size_x; }),
		new editable_field_numerical(L"command.link", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cmdset->cmd[sel.command_index].link = val; sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cmdset->cmd[sel.command_index].link; }),
		new editable_field_numerical(L"command.spell.spell_list", [](SaveSet& sv, FieldSelector& sel) { return sel.command_index < COMMAND_AMOUNT && sel.command__spell_index < COMMAND_SPELL_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cmdset->cmd[sel.command_index].spell_list[sel.command__spell_index] = val; sv.sectionmodified[DATA_SECTION_CMD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cmdset->cmd[sel.command_index].spell_list[sel.command__spell_index]; }),
		new editable_field_string(L"stat.default_name", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.statset->initial_stat[sel.stat_index].SetDefaultName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.statset->initial_stat[sel.stat_index].default_name.str); }),
		new editable_field_numerical(L"stat.default_name_size_x", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_stat[sel.stat_index].default_name_size_x = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_stat[sel.stat_index].default_name_size_x; }),
		new editable_field_numerical(L"stat.speed", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_stat[sel.stat_index].speed = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_stat[sel.stat_index].speed; }),
		new editable_field_numerical(L"stat.strength", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_stat[sel.stat_index].strength = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_stat[sel.stat_index].strength; }),
		new editable_field_numerical(L"stat.magic", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_stat[sel.stat_index].magic = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_stat[sel.stat_index].magic; }),
		new editable_field_numerical(L"stat.spirit", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_stat[sel.stat_index].spirit = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_stat[sel.stat_index].spirit; }),
		new editable_field_numerical(L"stat.magic_stone", [](SaveSet& sv, FieldSelector& sel) { return sel.stat_index < PLAYABLE_CHAR_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_stat[sel.stat_index].magic_stone = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_stat[sel.stat_index].magic_stone; }),
		new editable_field_numerical(L"initial_equip.weapon", [](SaveSet& sv, FieldSelector& sel) { return sel.initial_equip_index < EQUIP_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_equip[sel.initial_equip_index].weapon = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_equip[sel.initial_equip_index].weapon; }),
		new editable_field_numerical(L"initial_equip.head", [](SaveSet& sv, FieldSelector& sel) { return sel.initial_equip_index < EQUIP_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_equip[sel.initial_equip_index].head = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_equip[sel.initial_equip_index].head; }),
		new editable_field_numerical(L"initial_equip.wrist", [](SaveSet& sv, FieldSelector& sel) { return sel.initial_equip_index < EQUIP_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_equip[sel.initial_equip_index].wrist = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_equip[sel.initial_equip_index].wrist; }),
		new editable_field_numerical(L"initial_equip.armor", [](SaveSet& sv, FieldSelector& sel) { return sel.initial_equip_index < EQUIP_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_equip[sel.initial_equip_index].armor = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_equip[sel.initial_equip_index].armor; }),
		new editable_field_numerical(L"initial_equip.accessory", [](SaveSet& sv, FieldSelector& sel) { return sel.initial_equip_index < EQUIP_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->initial_equip[sel.initial_equip_index].accessory = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->initial_equip[sel.initial_equip_index].accessory; }),
		new editable_field_numerical(L"ability_list.slot.ability", [](SaveSet& sv, FieldSelector& sel) { return sel.ability_list_index < ABILITY_SET_AMOUNT && sel.ability_list__slot_index < ABILITY_SET_CAPACITY; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->ability_list[sel.ability_list_index].ability[sel.ability_list__slot_index].id = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->ability_list[sel.ability_list_index].ability[sel.ability_list__slot_index].id; }),
		new editable_field_numerical(L"ability_list.slot.ap_cost", [](SaveSet& sv, FieldSelector& sel) { return sel.ability_list_index < ABILITY_SET_AMOUNT && sel.ability_list__slot_index < ABILITY_SET_CAPACITY; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->ability_list[sel.ability_list_index].ap_cost[sel.ability_list__slot_index] = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->ability_list[sel.ability_list_index].ap_cost[sel.ability_list__slot_index]; }),
		new editable_field_numerical(L"command_list.first_command", [](SaveSet& sv, FieldSelector& sel) { return sel.command_list_index < COMMAND_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->command_list[sel.command_list_index].first_command = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->command_list[sel.command_list_index].first_command; }),
		new editable_field_numerical(L"command_list.second_command", [](SaveSet& sv, FieldSelector& sel) { return sel.command_list_index < COMMAND_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->command_list[sel.command_list_index].second_command = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->command_list[sel.command_list_index].second_command; }),
		new editable_field_numerical(L"command_list.first_command_trance", [](SaveSet& sv, FieldSelector& sel) { return sel.command_list_index < COMMAND_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->command_list[sel.command_list_index].first_command_trance = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->command_list[sel.command_list_index].first_command_trance; }),
		new editable_field_numerical(L"command_list.second_command_trance", [](SaveSet& sv, FieldSelector& sel) { return sel.command_list_index < COMMAND_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->command_list[sel.command_list_index].second_command_trance = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->command_list[sel.command_list_index].second_command_trance; }),
		new editable_field_numerical(L"command_list.trance_attack", [](SaveSet& sv, FieldSelector& sel) { return sel.command_list_index < COMMAND_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->command_list[sel.command_list_index].trance_attack = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->command_list[sel.command_list_index].trance_attack; }),
		new editable_field_numerical(L"level.hp_table", [](SaveSet& sv, FieldSelector& sel) { return sel.level_index < MAX_LEVEL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->level[sel.level_index].hp_table = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->level[sel.level_index].hp_table; }),
		new editable_field_numerical(L"level.mp_table", [](SaveSet& sv, FieldSelector& sel) { return sel.level_index < MAX_LEVEL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->level[sel.level_index].mp_table = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->level[sel.level_index].mp_table; }),
		new editable_field_numerical(L"level.exp_table", [](SaveSet& sv, FieldSelector& sel) { return sel.level_index < MAX_LEVEL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.statset->level[sel.level_index].exp_table = val; sv.sectionmodified[DATA_SECTION_STAT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.statset->level[sel.level_index].exp_table; }),
		new editable_field_string(L"item.name", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.itemset->item[sel.item_index].SetName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.itemset->item[sel.item_index].name.str); }),
		new editable_field_string(L"item.help", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.itemset->item[sel.item_index].SetHelp(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.itemset->item[sel.item_index].help.str); }),
		new editable_field_string(L"item.battle_help", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.itemset->item[sel.item_index].SetBattleHelp(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.itemset->item[sel.item_index].battle_help.str); }),
		new editable_field_numerical(L"item.price", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].price = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].price; }),
		new editable_field_numerical(L"item.char_availability", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].char_availability = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].char_availability; }),
		new editable_field_numerical(L"item.icon", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].icon = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].icon; }),
		new editable_field_numerical(L"item.icon_color", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].icon_color = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].icon_color; }),
		new editable_field_numerical(L"item.equip_position", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].equip_position = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].equip_position; }),
		new editable_field_numerical(L"item.stat_id", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].stat_id = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].stat_id; }),
		new editable_field_numerical(L"item.type", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].type = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].type; }),
		new editable_field_numerical(L"item.menu_position", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].menu_position = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].menu_position; }),
		new editable_field_numerical(L"item.skillslot.skill", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT && sel.item__skillslot_index < 3; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->item[sel.item_index].skill[sel.item__skillslot_index].id = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->item[sel.item_index].skill[sel.item__skillslot_index].id; }),
		new editable_field_numerical(L"item.target_type", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].target_type = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].target_type; }),
		new editable_field_numerical(L"item.model", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].model = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].model; }),
		new editable_field_numerical(L"item.target_flag", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].target_flag = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].target_flag; }),
		new editable_field_numerical(L"item.effect", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].effect = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].effect; }),
		new editable_field_numerical(L"item.power", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].power = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].power; }),
		new editable_field_numerical(L"item.element", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].element = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].element; }),
		new editable_field_numerical(L"item.accuracy", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].accuracy = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].accuracy; }),
		new editable_field_numerical(L"item.status", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].usable_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].status = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->usable[sv.itemset->item[sel.item_index].usable_id].status; }),
		new editable_field_numerical(L"item.flag", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].flag = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].flag; }),
		new editable_field_numerical(L"item.status", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].status = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].status; }),
		new editable_field_numerical(L"item.model", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].model = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].model; }),
		new editable_field_numerical(L"item.damage_formula", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].damage_formula = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].damage_formula; }),
		new editable_field_numerical(L"item.power", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].power = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].power; }),
		new editable_field_numerical(L"item.element", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].element = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].element; }),
		new editable_field_numerical(L"item.status_accuracy", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].status_accuracy = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].status_accuracy; }),
		new editable_field_numerical(L"item.offset1", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].offset1 = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].offset1; }),
		new editable_field_numerical(L"item.offset2", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].weapon_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].offset2 = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->weapon[sv.itemset->item[sel.item_index].weapon_id].offset2; }),
		new editable_field_numerical(L"item.defence", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].armor_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].defence = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].defence; }),
		new editable_field_numerical(L"item.evade", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].armor_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].evade = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].evade; }),
		new editable_field_numerical(L"item.magic_defence", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].armor_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].magic_defence = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].magic_defence; }),
		new editable_field_numerical(L"item.magic_evade", [](SaveSet& sv, FieldSelector& sel) { return sel.item_index < ITEM_AMOUNT&& sv.itemset->item[sel.item_index].armor_id >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].magic_evade = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->armor[sv.itemset->item[sel.item_index].armor_id].magic_evade; }),
		new editable_field_numerical(L"item_stat.speed", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].speed = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].speed; }),
		new editable_field_numerical(L"item_stat.strength", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].strength = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].strength; }),
		new editable_field_numerical(L"item_stat.magic", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].magic = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].magic; }),
		new editable_field_numerical(L"item_stat.spirit", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].spirit = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].spirit; }),
		new editable_field_numerical(L"item_stat.element_immune", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].element_immune = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].element_immune; }),
		new editable_field_numerical(L"item_stat.element_absorb", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].element_absorb = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].element_absorb; }),
		new editable_field_numerical(L"item_stat.element_half", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].element_half = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].element_half; }),
		new editable_field_numerical(L"item_stat.element_weak", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].element_weak = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].element_weak; }),
		new editable_field_numerical(L"item_stat.element_boost", [](SaveSet& sv, FieldSelector& sel) { return sel.item_stat_index < ITEM_STAT_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->stat[sel.item_stat_index].element_boost = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->stat[sel.item_stat_index].element_boost; }),
		new editable_field_string(L"key_item.name", [](SaveSet& sv, FieldSelector& sel) { return sel.key_item_index < KEY_ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.itemset->key_item[sel.key_item_index].SetName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.itemset->key_item[sel.key_item_index].name.str); }),
		new editable_field_string(L"key_item.help", [](SaveSet& sv, FieldSelector& sel) { return sel.key_item_index < KEY_ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.itemset->key_item[sel.key_item_index].SetHelp(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.itemset->key_item[sel.key_item_index].help.str); }),
		new editable_field_string(L"key_item.description", [](SaveSet& sv, FieldSelector& sel) { return sel.key_item_index < KEY_ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.itemset->key_item[sel.key_item_index].SetDescription(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.itemset->key_item[sel.key_item_index].description.str); }),
		new editable_field_numerical(L"key_item.help_size_x", [](SaveSet& sv, FieldSelector& sel) { return sel.key_item_index < KEY_ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.itemset->key_item[sel.key_item_index].help_size_x = val; sv.sectionmodified[DATA_SECTION_ITEM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.itemset->key_item[sel.key_item_index].help_size_x; }),
		new editable_field_numerical(L"shop.item_amount", [](SaveSet& sv, FieldSelector& sel) { return sel.shop_index < SHOP_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->shop[sel.shop_index].item_amount = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->shop[sel.shop_index].item_amount; }),
		new editable_field_numerical(L"shop.slot.item", [](SaveSet& sv, FieldSelector& sel) { return sel.shop_index < SHOP_AMOUNT && sel.shop__slot_index < SHOP_ITEM_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->shop[sel.shop_index].item_list[sel.shop__slot_index] = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->shop[sel.shop_index].item_list[sel.shop__slot_index]; }),
		new editable_field_numerical(L"synthesis.price", [](SaveSet& sv, FieldSelector& sel) { return sel.synthesis_index < SYNTHESIS_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->synthesis[sel.synthesis_index].price = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->synthesis[sel.synthesis_index].price; }),
		new editable_field_numerical(L"synthesis.recipe1", [](SaveSet& sv, FieldSelector& sel) { return sel.synthesis_index < SYNTHESIS_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->synthesis[sel.synthesis_index].recipe[0] = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->synthesis[sel.synthesis_index].recipe[0]; }),
		new editable_field_numerical(L"synthesis.recipe2", [](SaveSet& sv, FieldSelector& sel) { return sel.synthesis_index < SYNTHESIS_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->synthesis[sel.synthesis_index].recipe[1] = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->synthesis[sel.synthesis_index].recipe[1]; }),
		new editable_field_numerical(L"synthesis.synthesized", [](SaveSet& sv, FieldSelector& sel) { return sel.synthesis_index < SYNTHESIS_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->synthesis[sel.synthesis_index].synthesized = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->synthesis[sel.synthesis_index].synthesized; }),
		//new editable_field_numerical(L"synthesis.shops", [](SaveSet& sv, FieldSelector& sel) { return sel.synthesis_index < SYNTHESIS_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.shopset->synthesis[sel.synthesis_index].shops = val; sv.sectionmodified[DATA_SECTION_SHOP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.shopset->synthesis[sel.synthesis_index].shops; }),
		new editable_field_string(L"card.name", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.cardset->card[sel.card_index].SetName(ConvertArgToFF9String(val)); sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.cardset->card[sel.card_index].name.str); }),
		new editable_field_numerical(L"card.name_size_x", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].name_size_x = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].name_size_x; }),
		new editable_field_numerical(L"card.player_attack", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].player_attack = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].player_attack; }),
		new editable_field_numerical(L"card.player_type", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].player_type = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].player_type; }),
		new editable_field_numerical(L"card.player_defence", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].player_defence = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].player_defence; }),
		new editable_field_numerical(L"card.player_magicdefence", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].player_magicdefence = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].player_magicdefence; }),
		new editable_field_numerical(L"card.npc_attack", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].npc_attack = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].npc_attack; }),
		new editable_field_numerical(L"card.npc_type", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].npc_type = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].npc_type; }),
		new editable_field_numerical(L"card.npc_defence", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].npc_defence = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].npc_defence; }),
		new editable_field_numerical(L"card.npc_magicdefence", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].npc_magicdefence = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].npc_magicdefence; }),
		new editable_field_numerical(L"card.points", [](SaveSet& sv, FieldSelector& sel) { return sel.card_index < CARD_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->card[sel.card_index].points = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->card[sel.card_index].points; }),
		new editable_field_numerical(L"card_set.slot.card", [](SaveSet& sv, FieldSelector& sel) { return sel.card_set_index < CARD_SET_AMOUNT && sel.card_set__slot_index < CARD_SET_CAPACITY; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->set[sel.card_set_index].card[sel.card_set__slot_index] = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->set[sel.card_set_index].card[sel.card_set__slot_index]; }),
		new editable_field_numerical(L"deck.set", [](SaveSet& sv, FieldSelector& sel) { return sel.card_set_index < CARD_DECK_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->deck[sel.deck_index].set = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->deck[sel.deck_index].set; }),
		new editable_field_numerical(L"deck.difficulty", [](SaveSet& sv, FieldSelector& sel) { return sel.card_set_index < CARD_DECK_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.cardset->deck[sel.deck_index].difficulty = val; sv.sectionmodified[DATA_SECTION_CARD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.cardset->deck[sel.deck_index].difficulty; }),
		new editable_field_numerical(L"spell_animation.code.opcode", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_animation_index < SPELL_ANIMATION_AMOUNT && sel.spell_animation__code_index < sv.spellanimset->spell[sel.spell_animation_index].seq_code_amount; sv.sectionmodified[DATA_SECTION_SPELL_ANIM] = true; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellanimset->spell[sel.spell_animation_index].seq_code[sel.spell_animation__code_index].code = val; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellanimset->spell[sel.spell_animation_index].seq_code[sel.spell_animation__code_index].code; }),
		new editable_field_numerical(L"spell_animation.code.arg.value", [](SaveSet& sv, FieldSelector& sel) { return sel.spell_animation_index < SPELL_ANIMATION_AMOUNT && sel.spell_animation__code_index < sv.spellanimset->spell[sel.spell_animation_index].seq_code_amount && sel.spell_animation__code__arg_index < SPELL_ANIMATION_SEQUENCE_ARG; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.spellanimset->spell[sel.spell_animation_index].seq_code[sel.spell_animation__code_index].arg[sel.spell_animation__code__arg_index] = val; sv.sectionmodified[DATA_SECTION_SPELL_ANIM] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.spellanimset->spell[sel.spell_animation_index].seq_code[sel.spell_animation__code_index].arg[sel.spell_animation__code__arg_index]; }),
		new editable_field_numerical(L"battle.flag", [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->GetIndexById(sel.battle_id) >= 0; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->flag = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->flag; }),
		new editable_field_numerical(L"battle.group.frequence", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].frequence = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].frequence; }),
		new editable_field_numerical(L"battle.group.enemy_amount", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].enemy_amount = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].enemy_amount; }),
		new editable_field_numerical(L"battle.group.camera_engage", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].camera_engage = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].camera_engage; }),
		new editable_field_numerical(L"battle.group.ap", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].ap = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].ap; }),
		new editable_field_numerical(L"battle.group.slot.enemy_id", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount&& sel.battle__group__slot_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].enemy_id[sel.battle__group__slot_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].enemy_id[sel.battle__group__slot_index]; }),
		new editable_field_numerical(L"battle.group.slot.targetable", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount&& sel.battle__group__slot_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].targetable[sel.battle__group__slot_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].targetable[sel.battle__group__slot_index]; }),
		new editable_field_numerical(L"battle.group.slot.enemy_posx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount&& sel.battle__group__slot_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].enemy_posx[sel.battle__group__slot_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].enemy_posx[sel.battle__group__slot_index]; }),
		new editable_field_numerical(L"battle.group.slot.enemy_posz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount&& sel.battle__group__slot_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].enemy_posz[sel.battle__group__slot_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].enemy_posz[sel.battle__group__slot_index]; }),
		new editable_field_numerical(L"battle.group.slot.enemy_posy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount&& sel.battle__group__slot_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].enemy_posy[sel.battle__group__slot_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].enemy_posy[sel.battle__group__slot_index]; }),
		new editable_field_numerical(L"battle.group.slot.enemy_angle", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__group_index < sv.enemyset->battle[index]->group_amount&& sel.battle__group__slot_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->group[sel.battle__group_index].enemy_angle[sel.battle__group__slot_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->group[sel.battle__group_index].enemy_angle[sel.battle__group__slot_index]; }),
		new editable_field_string(L"battle.enemy.name", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].SetName(ConvertArgToFF9String(val)); sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].name.str); }),
		new editable_field_numerical(L"battle.enemy.lvl", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].lvl = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].lvl; }),
		new editable_field_numerical(L"battle.enemy.classification", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].classification = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].classification; }),
		new editable_field_numerical(L"battle.enemy.hp", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].hp = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].hp; }),
		new editable_field_numerical(L"battle.enemy.mp", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].mp = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].mp; }),
		new editable_field_numerical(L"battle.enemy.gils", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].gils = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].gils; }),
		new editable_field_numerical(L"battle.enemy.exp", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].exp = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].exp; }),
		new editable_field_numerical(L"battle.enemy.card_drop", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].card_drop = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].card_drop; }),
		new editable_field_numerical(L"battle.enemy.attack", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].attack = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].attack; }),
		new editable_field_numerical(L"battle.enemy.accuracy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].accuracy = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].accuracy; }),
		new editable_field_numerical(L"battle.enemy.cur_capa", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].cur_capa = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].cur_capa; }),
		new editable_field_numerical(L"battle.enemy.max_capa", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].max_capa = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].max_capa; }),
		new editable_field_numerical(L"battle.enemy.trans", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].trans = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].trans; }),
		new editable_field_numerical(L"battle.enemy.speed", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].speed = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].speed; }),
		new editable_field_numerical(L"battle.enemy.strength", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].strength = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].strength; }),
		new editable_field_numerical(L"battle.enemy.magic", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].magic = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].magic; }),
		new editable_field_numerical(L"battle.enemy.spirit", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].spirit = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].spirit; }),
		new editable_field_numerical(L"battle.enemy.defence", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].defence = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].defence; }),
		new editable_field_numerical(L"battle.enemy.evade", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].evade = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].evade; }),
		new editable_field_numerical(L"battle.enemy.magic_defence", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].magic_defence = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].magic_defence; }),
		new editable_field_numerical(L"battle.enemy.magic_evade", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].magic_evade = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].magic_evade; }),
		new editable_field_numerical(L"battle.enemy.element_immune", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].element_immune = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].element_immune; }),
		new editable_field_numerical(L"battle.enemy.element_absorb", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].element_absorb = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].element_absorb; }),
		new editable_field_numerical(L"battle.enemy.element_half", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].element_half = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].element_half; }),
		new editable_field_numerical(L"battle.enemy.element_weak", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].element_weak = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].element_weak; }),
		new editable_field_numerical(L"battle.enemy.status_immune", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].status_immune = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].status_immune; }),
		new editable_field_numerical(L"battle.enemy.status_auto", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].status_auto = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].status_auto; }),
		new editable_field_numerical(L"battle.enemy.status_initial", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].status_initial = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].status_initial; }),
		new editable_field_numerical(L"battle.enemy.default_attack", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].default_attack = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].default_attack; }),
		new editable_field_numerical(L"battle.enemy.blue_magic", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].blue_magic = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].blue_magic; }),
		new editable_field_numerical(L"battle.enemy.death_flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].death_flag = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].death_flag; }),
		new editable_field_numerical(L"battle.enemy.text_amount", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].text_amount = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].text_amount; }),
		new editable_field_numerical(L"battle.enemy.radius", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].radius = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].radius; }),
		new editable_field_numerical(L"battle.enemy.model", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].model = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].model; }),
		new editable_field_numerical(L"battle.enemy.anim_idle", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].anim_idle = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].anim_idle; }),
		new editable_field_numerical(L"battle.enemy.anim_idle_alt", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].anim_idle_alt = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].anim_idle_alt; }),
		new editable_field_numerical(L"battle.enemy.anim_hit", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].anim_hit = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].anim_hit; }),
		new editable_field_numerical(L"battle.enemy.anim_hit_alt", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].anim_hit_alt = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].anim_hit_alt; }),
		new editable_field_numerical(L"battle.enemy.anim_death", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].anim_death = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].anim_death; }),
		new editable_field_numerical(L"battle.enemy.anim_death_alt", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].anim_death_alt = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].anim_death_alt; }),
		new editable_field_numerical(L"battle.enemy.mesh", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].mesh = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].mesh; }),
		new editable_field_numerical(L"battle.enemy.mesh_vanish", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].mesh_vanish = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].mesh_vanish; }),
		new editable_field_numerical(L"battle.enemy.bone_camera1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].bone_camera1 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].bone_camera1; }),
		new editable_field_numerical(L"battle.enemy.bone_camera2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].bone_camera2 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].bone_camera2; }),
		new editable_field_numerical(L"battle.enemy.bone_camera3", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].bone_camera3 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].bone_camera3; }),
		new editable_field_numerical(L"battle.enemy.bone_target", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].bone_target = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].bone_target; }),
		new editable_field_numerical(L"battle.enemy.shadow_size_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].shadow_size_x = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].shadow_size_x; }),
		new editable_field_numerical(L"battle.enemy.shadow_size_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].shadow_size_y = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].shadow_size_y; }),
		new editable_field_numerical(L"battle.enemy.shadow_bone1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].shadow_bone1 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].shadow_bone1; }),
		new editable_field_numerical(L"battle.enemy.shadow_bone2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].shadow_bone2 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].shadow_bone2; }),
		new editable_field_numerical(L"battle.enemy.shadow_offset_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].shadow_offset_x = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].shadow_offset_x; }),
		new editable_field_numerical(L"battle.enemy.shadow_offset_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].shadow_offset_y = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].shadow_offset_y; }),
		new editable_field_numerical(L"battle.enemy.sound_engage", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].sound_engage = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].sound_engage; }),
		new editable_field_numerical(L"battle.enemy.sound_death", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].sound_death = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].sound_death; }),
		new editable_field_numerical(L"battle.enemy.zerostat", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].zerostat = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].zerostat; }),
		new editable_field_numerical(L"battle.enemy.zero1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].zero1 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].zero1; }),
		new editable_field_numerical(L"battle.enemy.zero2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].zero2 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].zero2; }),
		new editable_field_numerical(L"battle.enemy.zero3", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].zero3 = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].zero3; }),
		new editable_field_numerical(L"battle.enemy.selection.selection_bone", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount && sel.battle__enemy__selection_index < 6; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].selection_bone[sel.battle__enemy__selection_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].selection_bone[sel.battle__enemy__selection_index]; }),
		new editable_field_numerical(L"battle.enemy.selection.selection_offsetz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount && sel.battle__enemy__selection_index < 6; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].selection_offsetz[sel.battle__enemy__selection_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].selection_offsetz[sel.battle__enemy__selection_index]; }),
		new editable_field_numerical(L"battle.enemy.selection.selection_offsety", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount && sel.battle__enemy__selection_index < 6; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].selection_offsety[sel.battle__enemy__selection_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].selection_offsety[sel.battle__enemy__selection_index]; }),
		new editable_field_numerical(L"battle.enemy.item.item_drop", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount && sel.battle__enemy__item_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].item_drop[sel.battle__enemy__item_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].item_drop[sel.battle__enemy__item_index]; }),
		new editable_field_numerical(L"battle.enemy.item.item_steal", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__enemy_index < sv.enemyset->battle[index]->stat_amount && sel.battle__enemy__item_index < 4; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->stat[sel.battle__enemy_index].item_steal[sel.battle__enemy__item_index] = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->stat[sel.battle__enemy_index].item_steal[sel.battle__enemy__item_index]; }),
		new editable_field_string(L"battle.attack.name", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].SetName(ConvertArgToFF9String(val)); sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].name.str); }),
		new editable_field_numerical(L"battle.attack.effect", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].effect = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].effect; }),
		new editable_field_numerical(L"battle.attack.power", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].power = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].power; }),
		new editable_field_numerical(L"battle.attack.element", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].element = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].element; }),
		new editable_field_numerical(L"battle.attack.accuracy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].accuracy = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].accuracy; }),
		new editable_field_numerical(L"battle.attack.flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].flag = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].flag; }),
		new editable_field_numerical(L"battle.attack.status", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].status = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].status; }),
		new editable_field_numerical(L"battle.attack.mp", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].mp = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].mp; }),
		new editable_field_numerical(L"battle.attack.model", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].model = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].model; }),
		new editable_field_numerical(L"battle.attack.target_type", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].target_type = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].target_type; }),
		new editable_field_numerical(L"battle.attack.target_flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].target_flag = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].target_flag; }),
		new editable_field_numerical(L"battle.attack.menu_flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].menu_flag = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].menu_flag; }),
		new editable_field_numerical(L"battle.attack.model_alt", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].model_alt = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].model_alt; }),
		new editable_field_numerical(L"battle.attack.name_offset", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__attack_index < sv.enemyset->battle[index]->spell_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle[index]->spell[sel.battle__attack_index].name_offset = val; sv.enemyset->battle[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle[sv.enemyset->GetIndexById(sel.battle_id)]->spell[sel.battle__attack_index].name_offset; }),
		new editable_field_numerical(L"battle.animation.id", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__animation_index < sv.enemyset->battle_data[index]->animation_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle_data[index]->animation_id[sel.battle__animation_index] = val; sv.enemyset->battle_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle_data[sv.enemyset->GetIndexById(sel.battle_id)]->animation_id[sel.battle__animation_index]; }),
		new editable_field_numerical(L"battle.camera_raw.byte", [](SaveSet& sv, FieldSelector& sel) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return sel.battle__camera_raw_index < sv.enemyset->battle_data[index]->camera_size; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.enemyset->GetIndexById(sel.battle_id); sv.enemyset->battle_data[index]->camera_raw[sel.battle__camera_raw_index] = val; sv.enemyset->battle_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_ENMY] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.enemyset->battle_data[sv.enemyset->GetIndexById(sel.battle_id)]->camera_raw[sel.battle__camera_raw_index]; }),
		new editable_field_numerical(L"field.walkmesh_orgx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_orgx = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_orgx; }),
		new editable_field_numerical(L"field.walkmesh_orgz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_orgz = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_orgz; }),
		new editable_field_numerical(L"field.walkmesh_orgy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_orgy = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_orgy; }),
		new editable_field_numerical(L"field.walkmesh_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_x = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_x; }),
		new editable_field_numerical(L"field.walkmesh_z", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_z = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_z; }),
		new editable_field_numerical(L"field.walkmesh_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_y = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_y; }),
		new editable_field_numerical(L"field.walkmesh_minx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_minx = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_minx; }),
		new editable_field_numerical(L"field.walkmesh_minz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_minz = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_minz; }),
		new editable_field_numerical(L"field.walkmesh_miny", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_miny = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_miny; }),
		new editable_field_numerical(L"field.walkmesh_maxx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_maxx = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_maxx; }),
		new editable_field_numerical(L"field.walkmesh_maxz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_maxz = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_maxz; }),
		new editable_field_numerical(L"field.walkmesh_maxy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_maxy = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_maxy; }),
		new editable_field_numerical(L"field.walkmesh_charx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_charx = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_charx; }),
		new editable_field_numerical(L"field.walkmesh_charz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_charz = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_charz; }),
		new editable_field_numerical(L"field.walkmesh_chary", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->offset_chary = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->offset_chary; }),
		new editable_field_numerical(L"field.walkmesh_active_walkpath", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->active_walkpath = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->active_walkpath; }),
		new editable_field_numerical(L"field.walkmesh_active_triangle", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->walkmesh[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->active_triangle = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->active_triangle; }),
		new editable_field_numerical(L"field.triangle.flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_flag[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_flag[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.data", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_data[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_data[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.normal", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_normal[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_normal[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.thetax", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_thetax[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_thetax[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.thetay", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_thetay[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_thetay[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.centerx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_centerx[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_centerx[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.centerz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_centerz[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_centerz[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.centery", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_centery[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_centery[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.triangle.d", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__triangle_index < sv.fieldset->walkmesh[index]->triangle_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->triangle_d[sel.field__triangle_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->triangle_d[sel.field__triangle_index]; }),
		new editable_field_numerical(L"field.edge.flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__edge_index < sv.fieldset->walkmesh[index]->edge_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->edge_flag[sel.field__edge_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->edge_flag[sel.field__edge_index]; }),
		new editable_field_numerical(L"field.walkpath.flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_flag[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_flag[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.id", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_id[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_id[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.orgx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_orgx[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_orgx[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.orgz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_orgz[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_orgz[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.orgy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_orgy[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_orgy[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.offsetx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_offsetx[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_offsetx[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.offsetz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_offsetz[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_offsetz[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.offsety", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_offsety[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_offsety[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.minx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_minx[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_minx[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.minz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_minz[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_minz[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.miny", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_miny[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_miny[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.maxx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_maxx[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_maxx[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.maxz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_maxz[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_maxz[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.walkpath.maxy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__walkpath_index < sv.fieldset->walkmesh[index]->walkpath_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->walkpath_maxy[sel.field__walkpath_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->walkpath_maxy[sel.field__walkpath_index]; }),
		new editable_field_numerical(L"field.normal.x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__normal_index < sv.fieldset->walkmesh[index]->normal_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->normal_x[sel.field__normal_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->normal_x[sel.field__normal_index]; }),
		new editable_field_numerical(L"field.normal.z", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__normal_index < sv.fieldset->walkmesh[index]->normal_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->normal_z[sel.field__normal_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->normal_z[sel.field__normal_index]; }),
		new editable_field_numerical(L"field.normal.y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__normal_index < sv.fieldset->walkmesh[index]->normal_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->normal_y[sel.field__normal_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->normal_y[sel.field__normal_index]; }),
		new editable_field_numerical(L"field.normal.overz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__normal_index < sv.fieldset->walkmesh[index]->normal_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->normal_overz[sel.field__normal_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->normal_overz[sel.field__normal_index]; }),
		new editable_field_numerical(L"field.vertex.x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__vertex_index < sv.fieldset->walkmesh[index]->vertex_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->vertex_x[sel.field__vertex_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->vertex_x[sel.field__vertex_index]; }),
		new editable_field_numerical(L"field.vertex.z", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__vertex_index < sv.fieldset->walkmesh[index]->vertex_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->vertex_z[sel.field__vertex_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->vertex_z[sel.field__vertex_index]; }),
		new editable_field_numerical(L"field.vertex.y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return sel.field__vertex_index < sv.fieldset->walkmesh[index]->vertex_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->walkmesh[index]->vertex_y[sel.field__vertex_index] = val; sv.fieldset->walkmesh[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->walkmesh[sv.fieldset->GetIndexById(sel.field_id)]->vertex_y[sel.field__vertex_index]; }),
		new editable_field_numerical(L"field.tile_depth_shift", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->depth_shift = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->depth_shift; }),
		new editable_field_numerical(L"field.tile_anim_unk1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim_unk1 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim_unk1; }),
		new editable_field_numerical(L"field.tile_tiles_unk1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles_unk1 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles_unk1; }),
		new editable_field_numerical(L"field.tile_light_unk1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->light_unk1 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->light_unk1; }),
		new editable_field_numerical(L"field.tile_camera_unk1", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera_unk1 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera_unk1; }),
		new editable_field_numerical(L"field.tile_anim_unk2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim_unk2 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim_unk2; }),
		new editable_field_numerical(L"field.tile_tiles_unk2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles_unk2 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles_unk2; }),
		new editable_field_numerical(L"field.tile_light_unk2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->light_unk2 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->light_unk2; }),
		new editable_field_numerical(L"field.tile_camera_unk2", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera_unk2 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera_unk2; }),
		new editable_field_numerical(L"field.tile_anim_unk3", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim_unk3 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim_unk3; }),
		new editable_field_numerical(L"field.tile_tiles_unk3", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles_unk3 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles_unk3; }),
		new editable_field_numerical(L"field.tile_light_unk3", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->light_unk3 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->light_unk3; }),
		new editable_field_numerical(L"field.tile_camera_unk3", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); return index >= 0 && sv.fieldset->background_data[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera_unk3 = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera_unk3; }),
		new editable_field_numerical(L"field.tile_anim.flag", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].flag = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].flag; }),
		new editable_field_numerical(L"field.tile_anim.camera_id", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].camera_id = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].camera_id; }),
		new editable_field_numerical(L"field.tile_anim.default_frame", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].default_frame = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].default_frame; }),
		new editable_field_numerical(L"field.tile_anim.rate", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].rate = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].rate; }),
		new editable_field_numerical(L"field.tile_anim.counter", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].counter = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].counter; }),
		new editable_field_numerical(L"field.tile_anim.tile.tile_index", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount && sel.field__tile_anim__tile_index < sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].tile_list[sel.field__tile_anim__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].tile_list[sel.field__tile_anim__tile_index]; }),
		new editable_field_numerical(L"field.tile_anim.tile.duration", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_anim_index < sv.fieldset->background_data[index]->anim_amount && sel.field__tile_anim__tile_index < sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].tile_duration[sel.field__tile_anim__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->anim[sel.field__tile_anim_index].tile_duration[sel.field__tile_anim__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.width", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].width = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].width; }),
		new editable_field_numerical(L"field.tile_block.height", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].height = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].height; }),
		new editable_field_numerical(L"field.tile_block.default_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].default_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].default_x; }),
		new editable_field_numerical(L"field.tile_block.default_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].default_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].default_y; }),
		new editable_field_numerical(L"field.tile_block.pos_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_x; }),
		new editable_field_numerical(L"field.tile_block.pos_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_y; }),
		new editable_field_numerical(L"field.tile_block.pos_minx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_minx = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_minx; }),
		new editable_field_numerical(L"field.tile_block.pos_maxx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_maxx = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_maxx; }),
		new editable_field_numerical(L"field.tile_block.pos_miny", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_miny = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_miny; }),
		new editable_field_numerical(L"field.tile_block.pos_maxy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_maxy = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_maxy; }),
		new editable_field_numerical(L"field.tile_block.screen_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].screen_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].screen_x; }),
		new editable_field_numerical(L"field.tile_block.screen_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].screen_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].screen_y; }),
		new editable_field_numerical(L"field.tile_block.pos_dx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_dx = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_dx; }),
		new editable_field_numerical(L"field.tile_block.pos_dy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_dy = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_dy; }),
		new editable_field_numerical(L"field.tile_block.pos_fracx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_fracx = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_fracx; }),
		new editable_field_numerical(L"field.tile_block.pos_fracy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].pos_fracy = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].pos_fracy; }),
		new editable_field_numerical(L"field.tile_block.camera_id", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].camera_id = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].camera_id; }),
		new editable_field_numerical(L"field.tile_block.tile_tpage", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_tpage = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_tpage; }),
		new editable_field_numerical(L"field.tile_block.is_screen_static", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].is_screen_static = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].is_screen_static; }),
		new editable_field_numerical(L"field.tile_block.use_attaching", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].use_attaching = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].use_attaching; }),
		new editable_field_numerical(L"field.tile_block.is_looping", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].is_looping = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].is_looping; }),
		new editable_field_numerical(L"field.tile_block.has_parallax", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].has_parallax = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].has_parallax; }),
		new editable_field_numerical(L"field.tile_block.is_static", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].is_static = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].is_static; }),
		new editable_field_numerical(L"field.tile_block.is_scroll_with_offset", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].is_scroll_with_offset = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].is_scroll_with_offset; }),
		new editable_field_numerical(L"field.tile_block.depth", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].depth = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].depth; }),
		new editable_field_numerical(L"field.tile_block.default_depth", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].default_depth = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].default_depth; }),
		new editable_field_numerical(L"field.tile_block.is_x_offset", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].is_x_offset = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].is_x_offset; }),
		new editable_field_numerical(L"field.tile_block.viewport_id", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].viewport_id = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].viewport_id; }),
		new editable_field_numerical(L"field.tile_block.tile.depth", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_depth[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_depth[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.pos_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_pos_y[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_pos_y[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.pos_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_pos_x[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_pos_x[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.clut_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_clut_y[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_clut_y[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.clut_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_clut_x[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_clut_x[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.page_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_page_y[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_page_y[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.page_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_page_x[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_page_x[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.res", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_res[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_res[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.alpha", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_alpha[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_alpha[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.source_v", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_source_v[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_source_v[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.source_u", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_source_u[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_source_u[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.h", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_h[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_h[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.w", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_w[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_w[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.tile_block.tile.trans", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__tile_block_index < sv.fieldset->background_data[index]->tiles_amount && sel.field__tile_block__tile_index < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_trans[sel.field__tile_block__tile_index] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->tiles[sel.field__tile_block_index].tile_trans[sel.field__tile_block__tile_index]; }),
		new editable_field_numerical(L"field.camera.distance", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].distance = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].distance; }),
		new editable_field_numerical(L"field.camera.matrix_xx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[0] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[0]; }),
		new editable_field_numerical(L"field.camera.matrix_xy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[1] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[1]; }),
		new editable_field_numerical(L"field.camera.matrix_xz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[2] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[2]; }),
		new editable_field_numerical(L"field.camera.matrix_yx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[3] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[3]; }),
		new editable_field_numerical(L"field.camera.matrix_yy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[4] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[4]; }),
		new editable_field_numerical(L"field.camera.matrix_yz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[5] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[5]; }),
		new editable_field_numerical(L"field.camera.matrix_zx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[6] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[6]; }),
		new editable_field_numerical(L"field.camera.matrix_zy", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[7] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[7]; }),
		new editable_field_numerical(L"field.camera.matrix_zz", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].matrix[8] = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].matrix[8]; }),
		new editable_field_numerical(L"field.camera.offset_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_x; }),
		new editable_field_numerical(L"field.camera.offset_z", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_z = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_z; }),
		new editable_field_numerical(L"field.camera.offset_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_y; }),
		new editable_field_numerical(L"field.camera.offset_centerx", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_centerx = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_centerx; }),
		new editable_field_numerical(L"field.camera.offset_centery", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_centery = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_centery; }),
		new editable_field_numerical(L"field.camera.offset_width", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_width = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_width; }),
		new editable_field_numerical(L"field.camera.offset_height", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].offset_height = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].offset_height; }),
		new editable_field_numerical(L"field.camera.min_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].min_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].min_x; }),
		new editable_field_numerical(L"field.camera.max_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].max_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].max_x; }),
		new editable_field_numerical(L"field.camera.min_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].min_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].min_y; }),
		new editable_field_numerical(L"field.camera.max_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].max_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].max_y; }),
		new editable_field_numerical(L"field.camera.depth", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].depth = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].depth; }),
		new editable_field_numerical(L"field.camera.pos_x", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].pos_x = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].pos_x; }),
		new editable_field_numerical(L"field.camera.pos_y", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].pos_y = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].pos_y; }),
		new editable_field_numerical(L"field.camera.width", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].width = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].width; }),
		new editable_field_numerical(L"field.camera.height", [](SaveSet& sv, FieldSelector& sel) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return sel.field__camera_index < sv.fieldset->background_data[index]->camera_amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.fieldset->GetIndexById(sel.field_id); sv.fieldset->background_data[index]->camera[sel.field__camera_index].height = val; sv.fieldset->background_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_FIELD] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.fieldset->background_data[sv.fieldset->GetIndexById(sel.field_id)]->camera[sel.field__camera_index].height; }),
		new editable_field_string(L"world_place.name", [](SaveSet& sv, FieldSelector& sel) { return sel.world_place_index < WORLD_MAP_PLACE_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { sv.worldset->world_data->SetName(sel.world_place_index, ConvertArgToFF9String(val)); sv.worldset->world_data->MarkDataModified(); sv.sectionmodified[DATA_SECTION_WORLD_MAP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.worldset->world_data->place_name[sel.world_place_index].str); }),
		new editable_field_numerical(L"world_place.raw.byte", [](SaveSet& sv, FieldSelector& sel) { return sel.world_place_index < WORLD_MAP_PLACE_AMOUNT && sel.world_place__raw_index < sv.worldset->world_data->place_unknown_amount[sel.world_place_index]; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.worldset->world_data->place_unknown[sel.world_place_index][sel.world_place__raw_index] = val; sv.worldset->world_data->MarkDataModified(); sv.sectionmodified[DATA_SECTION_WORLD_MAP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.worldset->world_data->place_unknown[sel.world_place_index][sel.world_place__raw_index]; }),
		new editable_field_numerical(L"world_spot.flag", [](SaveSet& sv, FieldSelector& sel) { return sel.world_spot_index < WORLD_MAP_BATTLE_GROUND_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.worldset->world_data->battle_flag[sel.world_spot_index] = val; sv.worldset->world_data->MarkDataModified(); sv.sectionmodified[DATA_SECTION_WORLD_MAP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.worldset->world_data->battle_flag[sel.world_spot_index]; }),
		new editable_field_numerical(L"world_spot.battle.id", [](SaveSet& sv, FieldSelector& sel) { return sel.world_spot_index < WORLD_MAP_BATTLE_GROUND_AMOUNT && sel.world_spot__battle_index < WORLD_MAP_BATTLE_SET_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.worldset->world_data->battle_id[sel.world_spot_index][sel.world_spot__battle_index] = val; sv.worldset->world_data->MarkDataModified(); sv.sectionmodified[DATA_SECTION_WORLD_MAP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.worldset->world_data->battle_id[sel.world_spot_index][sel.world_spot__battle_index]; }),
		new editable_field_numerical(L"world_friendly.spot.area", [](SaveSet& sv, FieldSelector& sel) { return sel.world_friendly_index < WORLD_MAP_FRIENDLY_AMOUNT && sel.world_friendly__spot_index < WORLD_MAP_FRIENDLY_AREA_AMOUNT; }, [](SaveSet& sv, FieldSelector& sel, long long val) { sv.worldset->world_data->friendly_area[sel.world_friendly_index][sel.world_friendly__spot_index] = val; sv.worldset->world_data->MarkDataModified(); sv.sectionmodified[DATA_SECTION_WORLD_MAP] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.worldset->world_data->friendly_area[sel.world_friendly_index][sel.world_friendly__spot_index]; }),
		new editable_field_string(L"text_block.slot.text", [](SaveSet& sv, FieldSelector& sel) { int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0) return false; return sel.text_block__slot_index < sv.textset->text_data[index]->amount; }, [](SaveSet& sv, FieldSelector& sel, wxString val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->text_data[index]->SetText(sel.text_block__slot_index, ConvertArgToFF9String(val)); sv.textset->text_data[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return ConvertFF9StringToArg(sv.textset->text_data[sv.textset->GetIndexById(sel.text_block_id)]->text[sel.text_block__slot_index].str); }),
		new editable_field_numerical(L"text_block.charmap_unknown1", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); return index >= 0 && sv.textset->charmap[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->unknown1 = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->unknown1; }),
		new editable_field_numerical(L"text_block.charmap_unknown2", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); return index >= 0 && sv.textset->charmap[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->unknown2 = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->unknown2; }),
		new editable_field_numerical(L"text_block.charmap_unknown3", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); return index >= 0 && sv.textset->charmap[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->unknown3 = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->unknown3; }),
		new editable_field_numerical(L"text_block.charmap_unknown4", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); return index >= 0 && sv.textset->charmap[index] != NULL; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->unknown4 = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->unknown4; }),
		new editable_field_numerical(L"text_block.charmap_glyph.img_x", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0 || sv.textset->charmap[index] == NULL) return false; return sel.text_block__charmap_glyph_index < sv.textset->charmap[index]->amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->img_x[sel.text_block__charmap_glyph_index] = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->img_x[sel.text_block__charmap_glyph_index]; }),
		new editable_field_numerical(L"text_block.charmap_glyph.img_y", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0 || sv.textset->charmap[index] == NULL) return false; return sel.text_block__charmap_glyph_index < sv.textset->charmap[index]->amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->img_y[sel.text_block__charmap_glyph_index] = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->img_y[sel.text_block__charmap_glyph_index]; }),
		new editable_field_numerical(L"text_block.charmap_glyph.img_width", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0 || sv.textset->charmap[index] == NULL) return false; return sel.text_block__charmap_glyph_index < sv.textset->charmap[index]->amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->img_width[sel.text_block__charmap_glyph_index] = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->img_width[sel.text_block__charmap_glyph_index]; }),
		new editable_field_numerical(L"text_block.charmap_glyph.width", [](SaveSet& sv, FieldSelector& sel) { if (sv.textset->charmap.size() == 0) return false; int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0 || sv.textset->charmap[index] == NULL) return false; return sel.text_block__charmap_glyph_index < sv.textset->charmap[index]->amount; }, [](SaveSet& sv, FieldSelector& sel, long long val) { int index = sv.textset->GetIndexById(sel.text_block_id); sv.textset->charmap[index]->width[sel.text_block__charmap_glyph_index] = val; sv.textset->charmap[index]->MarkDataModified(); sv.sectionmodified[DATA_SECTION_TEXT] = true; }, [](SaveSet& sv, FieldSelector& sel) { return sv.textset->charmap[sv.textset->GetIndexById(sel.text_block_id)]->width[sel.text_block__charmap_glyph_index]; }),
	};
}

struct index_parameter {
	wxString name;
	unsigned int section;
	function<unsigned int(FieldSelector&)> getter;
	function<void(FieldSelector&, unsigned int)> setter;
	function<bool(SaveSet&, FieldSelector&, unsigned int)> enumerator;
};

#define INDEX_PARAMETER_COUNT 56
index_parameter* const_index_parameter = NULL;
void InitConstIndexParameter() {
	if (const_index_parameter != NULL)
		return;
	const_index_parameter = new index_parameter[INDEX_PARAMETER_COUNT]{
		{ L"spell", DATA_SECTION_SPELL, [](FieldSelector& sel) { return sel.spell_index; }, [](FieldSelector& sel, unsigned int val) { sel.spell_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.spell_index = i) < SPELL_AMOUNT; } },
		{ L"status_set", DATA_SECTION_SPELL, [](FieldSelector& sel) { return sel.status_set_index; }, [](FieldSelector& sel, unsigned int val) { sel.status_set_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.status_set_index = i) < STATUS_SET_AMOUNT; } },
		{ L"support", DATA_SECTION_SUPPORT, [](FieldSelector& sel) { return sel.support_index; }, [](FieldSelector& sel, unsigned int val) { sel.support_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.support_index = i) < SUPPORT_AMOUNT; } },
		{ L"command", DATA_SECTION_CMD, [](FieldSelector& sel) { return sel.command_index; }, [](FieldSelector& sel, unsigned int val) { sel.command_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.command_index = i) < COMMAND_AMOUNT; } },
		{ L"stat", DATA_SECTION_STAT, [](FieldSelector& sel) { return sel.stat_index; }, [](FieldSelector& sel, unsigned int val) { sel.stat_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.stat_index = i) < PLAYABLE_CHAR_AMOUNT; } },
		{ L"initial_equip", DATA_SECTION_STAT, [](FieldSelector& sel) { return sel.initial_equip_index; }, [](FieldSelector& sel, unsigned int val) { sel.initial_equip_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.initial_equip_index = i) < EQUIP_SET_AMOUNT; } },
		{ L"ability_list", DATA_SECTION_STAT, [](FieldSelector& sel) { return sel.ability_list_index; }, [](FieldSelector& sel, unsigned int val) { sel.ability_list_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.ability_list_index = i) < ABILITY_SET_AMOUNT; } },
		{ L"command_list", DATA_SECTION_STAT, [](FieldSelector& sel) { return sel.command_list_index; }, [](FieldSelector& sel, unsigned int val) { sel.command_list_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.command_list_index = i) < COMMAND_SET_AMOUNT; } },
		{ L"level", DATA_SECTION_STAT, [](FieldSelector& sel) { return sel.level_index; }, [](FieldSelector& sel, unsigned int val) { sel.level_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.level_index = i) < MAX_LEVEL; } },
		{ L"magic_sword", DATA_SECTION_PARTY_SPECIAL, [](FieldSelector& sel) { return sel.magic_sword_index; }, [](FieldSelector& sel, unsigned int val) { sel.magic_sword_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.magic_sword_index = i) < MAGIC_SWORD_AMOUNT; } },
		{ L"item", DATA_SECTION_ITEM, [](FieldSelector& sel) { return sel.item_index; }, [](FieldSelector& sel, unsigned int val) { sel.item_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.item_index = i) < ITEM_AMOUNT; } },
		{ L"item_stat", DATA_SECTION_ITEM, [](FieldSelector& sel) { return sel.item_stat_index; }, [](FieldSelector& sel, unsigned int val) { sel.item_stat_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.item_stat_index = i) < ITEM_STAT_AMOUNT; } },
		{ L"key_item", DATA_SECTION_ITEM, [](FieldSelector& sel) { return sel.key_item_index; }, [](FieldSelector& sel, unsigned int val) { sel.key_item_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.key_item_index = i) < KEY_ITEM_AMOUNT; } },
		{ L"shop", DATA_SECTION_SHOP, [](FieldSelector& sel) { return sel.shop_index; }, [](FieldSelector& sel, unsigned int val) { sel.shop_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.shop_index = i) < SHOP_AMOUNT; } },
		{ L"synthesis", DATA_SECTION_SHOP, [](FieldSelector& sel) { return sel.synthesis_index; }, [](FieldSelector& sel, unsigned int val) { sel.synthesis_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.synthesis_index = i) < SYNTHESIS_AMOUNT; } },
		{ L"deck", DATA_SECTION_CARD, [](FieldSelector& sel) { return sel.deck_index; }, [](FieldSelector& sel, unsigned int val) { sel.deck_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.deck_index = i) < CARD_DECK_AMOUNT; } },
		{ L"card_set", DATA_SECTION_CARD, [](FieldSelector& sel) { return sel.card_set_index; }, [](FieldSelector& sel, unsigned int val) { sel.card_set_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.card_set_index = i) < CARD_SET_AMOUNT; } },
		{ L"card", DATA_SECTION_CARD, [](FieldSelector& sel) { return sel.card_index; }, [](FieldSelector& sel, unsigned int val) { sel.card_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.card_index = i) < CARD_AMOUNT; } },
		{ L"spell_animation", DATA_SECTION_SPELL_ANIM, [](FieldSelector& sel) { return sel.spell_animation_index; }, [](FieldSelector& sel, unsigned int val) { sel.spell_animation_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.spell_animation_index = i) < SPELL_ANIMATION_AMOUNT; } },
		{ L"battle", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle_id; }, [](FieldSelector& sel, unsigned int val) { sel.battle_id = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { if (i >= sv.enemyset->battle_amount) return false; sel.battle_id = sv.enemyset->struct_id[i]; return true; } },
		{ L"field", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field_id; }, [](FieldSelector& sel, unsigned int val) { sel.field_id = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { if (i >= sv.fieldset->amount) return false; sel.field_id = sv.fieldset->struct_id[i]; return true; } },
		{ L"world", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_id; }, [](FieldSelector& sel, unsigned int val) { sel.world_id = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { if (i >= sv.worldset->amount) return false; sel.world_id = sv.worldset->struct_id[i]; return true; } },
		{ L"text_block", DATA_SECTION_TEXT, [](FieldSelector& sel) { return sel.text_block_id; }, [](FieldSelector& sel, unsigned int val) { sel.text_block_id = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { if (i >= sv.textset->amount) return false; sel.text_block_id = sv.textset->struct_id[i]; return true; } },
		{ L"command.spell", DATA_SECTION_CMD, [](FieldSelector& sel) { return sel.command__spell_index; }, [](FieldSelector& sel, unsigned int val) { sel.command__spell_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.command__spell_index = i) < COMMAND_SPELL_AMOUNT && sv.cmdset->cmd[sel.command_index].spell_list[sel.command__spell_index] != 0; } },
		{ L"ability_list.slot", DATA_SECTION_STAT, [](FieldSelector& sel) { return sel.ability_list__slot_index; }, [](FieldSelector& sel, unsigned int val) { sel.ability_list__slot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.ability_list__slot_index = i) < ABILITY_SET_CAPACITY; } },
		{ L"item.skillslot", DATA_SECTION_ITEM, [](FieldSelector& sel) { return sel.item__skillslot_index; }, [](FieldSelector& sel, unsigned int val) { sel.item__skillslot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.item__skillslot_index = i) < 3; } },
		{ L"shop.slot", DATA_SECTION_SHOP, [](FieldSelector& sel) { return sel.shop__slot_index; }, [](FieldSelector& sel, unsigned int val) { sel.shop__slot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.shop__slot_index = i) < SHOP_ITEM_AMOUNT; } },
		{ L"card_set.slot", DATA_SECTION_CARD, [](FieldSelector& sel) { return sel.card_set__slot_index; }, [](FieldSelector& sel, unsigned int val) { sel.card_set__slot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.card_set__slot_index = i) < CARD_SET_CAPACITY; } },
		{ L"spell_animation.code", DATA_SECTION_SPELL_ANIM, [](FieldSelector& sel) { return sel.spell_animation__code_index; }, [](FieldSelector& sel, unsigned int val) { sel.spell_animation__code_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return sel.spell_animation_index < SPELL_ANIMATION_AMOUNT && (sel.spell_animation__code_index = i) < sv.spellanimset->spell[sel.spell_animation_index].seq_code_amount; } },
		{ L"spell_animation.code.arg", DATA_SECTION_SPELL_ANIM, [](FieldSelector& sel) { return sel.spell_animation__code__arg_index; }, [](FieldSelector& sel, unsigned int val) { sel.spell_animation__code__arg_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return sel.spell_animation_index < SPELL_ANIMATION_AMOUNT && sel.spell_animation__code_index < sv.spellanimset->spell[sel.spell_animation_index].seq_code_amount && (sel.spell_animation__code__arg_index = i) < SPELL_ANIMATION_SEQUENCE_ARG; } },
		{ L"battle.animation", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__animation_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__animation_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return (sel.battle__animation_index = i) < sv.enemyset->battle_data[index]->animation_amount; } },
		{ L"battle.camera_raw", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__camera_raw_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__camera_raw_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return (sel.battle__camera_raw_index = i) < sv.enemyset->battle_data[index]->camera_size; } },
		{ L"battle.group", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__group_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__group_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return (sel.battle__group_index = i) < sv.enemyset->battle[index]->group_amount; } },
		{ L"battle.group.slot", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__group__slot_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__group__slot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; if (sel.battle__group_index >= sv.enemyset->battle[index]->group_amount) return false; return (sel.battle__group__slot_index = i) < 4; } },
		{ L"battle.enemy", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__enemy_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__enemy_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return (sel.battle__enemy_index = i) < sv.enemyset->battle[index]->stat_amount; } },
		{ L"battle.enemy.item", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__enemy__item_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__enemy__item_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; if (sel.battle__enemy_index >= sv.enemyset->battle[index]->stat_amount) return false; return (sel.battle__enemy__item_index = i) < 4; } },
		{ L"battle.enemy.selection", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__enemy__selection_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__enemy__selection_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; if (sel.battle__enemy_index >= sv.enemyset->battle[index]->stat_amount) return false; return (sel.battle__enemy__selection_index = i) < 6; } },
		{ L"battle.attack", DATA_SECTION_ENMY, [](FieldSelector& sel) { return sel.battle__attack_index; }, [](FieldSelector& sel, unsigned int val) { sel.battle__attack_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.enemyset->GetIndexById(sel.battle_id); if (index < 0) return false; return (sel.battle__attack_index = i) < sv.enemyset->battle[index]->spell_amount; } },
		{ L"field.triangle", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__triangle_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__triangle_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return (sel.field__triangle_index = i) < sv.fieldset->walkmesh[index]->triangle_amount; } },
		{ L"field.edge", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__edge_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__edge_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return (sel.field__edge_index = i) < sv.fieldset->walkmesh[index]->edge_amount; } },
		{ L"field.walkpath", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__walkpath_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__walkpath_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return (sel.field__walkpath_index = i) < sv.fieldset->walkmesh[index]->walkpath_amount; } },
		{ L"field.normal", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__normal_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__normal_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return (sel.field__normal_index = i) < sv.fieldset->walkmesh[index]->normal_amount; } },
		{ L"field.vertex", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__vertex_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__vertex_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->walkmesh[index] == NULL) return false; return (sel.field__vertex_index = i) < sv.fieldset->walkmesh[index]->vertex_amount; } },
		{ L"field.tile_anim", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__tile_anim_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__tile_anim_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return (sel.field__tile_anim_index = i) < sv.fieldset->background_data[index]->anim_amount; } },
		{ L"field.tile_anim.tile", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__tile_anim__tile_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__tile_anim__tile_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; if (sel.field__tile_anim_index >= sv.fieldset->background_data[index]->anim_amount) return false; return (sel.field__tile_anim__tile_index = i) < sv.fieldset->background_data[index]->anim[sel.field__tile_anim_index].tile_amount; } },
		{ L"field.tile_block", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__tile_block_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__tile_block_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return (sel.field__tile_block_index = i) < sv.fieldset->background_data[index]->tiles_amount; } },
		{ L"field.tile_block.tile", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__tile_block__tile_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__tile_block__tile_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; if (sel.field__tile_block_index >= sv.fieldset->background_data[index]->tiles_amount) return false; return (sel.field__tile_block__tile_index = i) < sv.fieldset->background_data[index]->tiles[sel.field__tile_block_index].tile_amount; } },
		{ L"field.camera", DATA_SECTION_FIELD, [](FieldSelector& sel) { return sel.field__camera_index; }, [](FieldSelector& sel, unsigned int val) { sel.field__camera_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.fieldset->GetIndexById(sel.field_id); if (index < 0 || sv.fieldset->background_data[index] == NULL) return false; return (sel.field__camera_index = i) < sv.fieldset->background_data[index]->camera_amount; } },
		{ L"world_place", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_place_index; }, [](FieldSelector& sel, unsigned int val) { sel.world_place_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.world_place_index = i) < WORLD_MAP_PLACE_AMOUNT; } },
		{ L"world_place.raw", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_place__raw_index; }, [](FieldSelector& sel, unsigned int val) { sel.world_place__raw_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { if (sel.world_place_index >= WORLD_MAP_PLACE_AMOUNT) return false; return (sel.world_place__raw_index = i) < sv.worldset->world_data->place_unknown_amount[sel.world_place_index]; } },
		{ L"world_spot", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_spot_index; }, [](FieldSelector& sel, unsigned int val) { sel.world_spot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.world_spot_index = i) < WORLD_MAP_BATTLE_GROUND_AMOUNT; } },
		{ L"world_spot.battle", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_spot__battle_index; }, [](FieldSelector& sel, unsigned int val) { sel.world_spot__battle_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.world_spot__battle_index = i) < WORLD_MAP_BATTLE_SET_AMOUNT; } },
		{ L"world_friendly", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_friendly_index; }, [](FieldSelector& sel, unsigned int val) { sel.world_friendly_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.world_friendly_index = i) < WORLD_MAP_FRIENDLY_AMOUNT; } },
		{ L"world_friendly.spot", DATA_SECTION_WORLD_MAP, [](FieldSelector& sel) { return sel.world_friendly__spot_index; }, [](FieldSelector& sel, unsigned int val) { sel.world_friendly__spot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { return (sel.world_friendly__spot_index = i) < WORLD_MAP_FRIENDLY_AREA_AMOUNT; } },
		{ L"text_block.slot", DATA_SECTION_TEXT, [](FieldSelector& sel) { return sel.text_block__slot_index; }, [](FieldSelector& sel, unsigned int val) { sel.text_block__slot_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0) return false; return (sel.text_block__slot_index = i) < sv.textset->text_data[index]->amount; } },
		{ L"text_block.charmap_glyph", DATA_SECTION_TEXT, [](FieldSelector& sel) { return sel.text_block__charmap_glyph_index; }, [](FieldSelector& sel, unsigned int val) { sel.text_block__charmap_glyph_index = val; }, [](SaveSet& sv, FieldSelector& sel, unsigned int i) { int index = sv.textset->GetIndexById(sel.text_block_id); if (index < 0 || (unsigned int)index >= sv.textset->charmap.size() || sv.textset->charmap[index] == NULL) return false; return (sel.text_block__charmap_glyph_index = i) < sv.textset->charmap[index]->amount; } },
	};
}

struct DataStructureLeaf {
	wxString name;
	editable_field* field;

	DataStructureLeaf(wxString n, editable_field* f) : name(n), field(f) {}
};
struct DataStructureBranch {
	wxString name;
	index_parameter* parameter;
	vector<DataStructureBranch> next;
	vector<DataStructureLeaf> children;

	DataStructureBranch(wxString n, index_parameter* p) : name(n), parameter(p) {}
};

// Processing

inline wxString GetWxStringLine(wxString& str) {
	wxString tmpstr;
	wxString res = str.BeforeFirst(L'\n', &tmpstr);
	str = tmpstr;
	return res;
}

class CommandThread : public wxThread {
private:
	CommandFrame* cmdframe;

	virtual ExitCode Entry() {
		wxTextAttr styledefault, stylecurrentline, styleexecutedline, styleerrline, stylewarningline;
		wxString line;
		long linepos = -1, lineposp1 = cmdframe->m_commands->XYToPosition(0, cmdframe->line_no + 1);
		bool keepgoing;
		if (cmdframe->executing_extra_commands) {
			while (cmdframe->m_commands->XYToPosition(0, cmdframe->line_no) >= 0)
				cmdframe->line_no++;
			cmdframe->line_no -= 3;
			lineposp1 = cmdframe->m_commands->XYToPosition(0, cmdframe->line_no + 1);
		}
		styledefault.SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));
		stylecurrentline.SetBackgroundColour(*wxLIGHT_GREY);
		styleexecutedline.SetBackgroundColour(*wxCYAN);
		styleerrline.SetBackgroundColour(*wxRED);
		stylewarningline.SetBackgroundColour(*wxYELLOW);
		while (!cmdframe->completefile.IsEmpty()) {
			cmdframe->line_no++;
			linepos = lineposp1;
			lineposp1 = cmdframe->m_commands->XYToPosition(0, cmdframe->line_no + 1);
			if (lineposp1 < 0)
				lineposp1 = cmdframe->m_commands->GetLastPosition();
			cmdframe->m_commands->SetStyle(linepos, lineposp1, stylecurrentline);
			cmdframe->current_line_executed = false;
			cmdframe->current_line_error = false;
			cmdframe->current_line_warning = false;
			line = GetWxStringLine(cmdframe->completefile);
			keepgoing = cmdframe->ProcessLine(line);
			if (linepos >= 0) {
				if (cmdframe->current_line_error)
					cmdframe->m_commands->SetStyle(linepos, lineposp1, styleerrline);
				else if (cmdframe->current_line_warning)
					cmdframe->m_commands->SetStyle(linepos, lineposp1, stylewarningline);
				else if (cmdframe->current_line_executed)
					cmdframe->m_commands->SetStyle(linepos, lineposp1, styleexecutedline);
				else
					cmdframe->m_commands->SetStyle(linepos, lineposp1, styledefault);
				cmdframe->m_commands->SetInsertionPoint(linepos);
			}
			if (!keepgoing) {
				if (cmdframe->exit_on_stop)
					cmdframe->exit = true;
				break;
			}
		}
		cmdframe->executing_commands = false;
		cmdframe->m_morecommand->Enable(true);
		cmdframe->m_morecommand->SetFocus();
		if (cmdframe->exit)
			cmdframe->Close();
		return (wxThread::ExitCode)0;
	}

public:
	CommandThread(CommandFrame* handler) : wxThread(wxTHREAD_DETACHED), cmdframe(handler) {}
	~CommandThread() {}
};

CommandFrame::CommandFrame(wxWindow* parent) : CommandFrameBase(parent), executing_commands(false) {
	LoadingDialogCreate(this);
	SetSteamLanguage(STEAM_LANGUAGE_US);
	InitConstEditableField();
	InitConstIndexParameter();
}

SaveSet CommandFrame::MergeDataSets() {
	SaveSet result(NULL, NULL, new EnemyDataSet, NULL, new TextDataSet, new WorldMapDataSet, new BattleSceneDataSet, NULL, NULL, NULL, NULL, new FieldDataSet, NULL, NULL, NULL, NULL, NULL);
	unsigned int i, j;
	set<uint16_t> enemyid, textid, worldid, sceneid, fieldid;
	for (i = 0; i < dataset.size(); i++) {

		#define MACRO_HWC_MERGE_NONCLUSTER(SECTION, SET) \
			if (!result.sectionloaded[SECTION] && dataset[i].saveset.sectionloaded[SECTION]) { \
				result.SET = dataset[i].saveset.SET; \
				result.sectionloaded[SECTION] = true; \
				result.sectionmodified[SECTION] = dataset[i].saveset.sectionmodified[SECTION]; \
			}

		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_SPELL, spellset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_CMD, cmdset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_SHOP, shopset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_ITEM, itemset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_SUPPORT, supportset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_STAT, statset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_CARD, cardset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_SPELL_ANIM, spellanimset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_MENU_UI, ffuiset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_PARTY_SPECIAL, partyspecialset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_MIPS, mipsset)
		MACRO_HWC_MERGE_NONCLUSTER(DATA_SECTION_CIL, cilset);

		if (dataset[i].saveset.sectionloaded[DATA_SECTION_ENMY]) {
			for (j = 0; j < dataset[i].enemyset.battle_amount; j++) {
				if (enemyid.count(dataset[i].enemyset.struct_id[j]) == 0) {
					if (dataset[i].gametype == GAME_TYPE_PSX) {
						result.enemyset->cluster_id.push_back(dataset[i].enemyset.cluster_id[j]);
						result.enemyset->preload.push_back(dataset[i].enemyset.preload[j]);
					}
					result.enemyset->struct_id.push_back(dataset[i].enemyset.struct_id[j]);
					result.enemyset->battle_name.push_back(dataset[i].enemyset.battle_name[j]);
					result.enemyset->battle_data.push_back(dataset[i].enemyset.battle_data[j]);
					result.enemyset->battle.push_back(dataset[i].enemyset.battle[j]);
					result.enemyset->text.push_back(dataset[i].enemyset.text[j]);
					result.enemyset->script.push_back(dataset[i].enemyset.script[j]);
					result.sectionloaded[DATA_SECTION_ENMY] = true;
					if (dataset[i].saveset.sectionmodified[DATA_SECTION_ENMY])
						result.sectionmodified[DATA_SECTION_ENMY] = true;
					enemyid.insert(dataset[i].enemyset.struct_id[j]);
				}
			}
		}
		if (dataset[i].saveset.sectionloaded[DATA_SECTION_TEXT]) {
			for (j = 0; j < dataset[i].textset.amount; j++) {
				if (textid.count(dataset[i].textset.struct_id[j]) == 0) {
					if (dataset[i].gametype == GAME_TYPE_PSX) {
						result.textset->main_charmap_index = dataset[i].textset.main_charmap_index;
						result.textset->cluster_id.push_back(dataset[i].textset.cluster_id[j]);
						result.textset->charmap.push_back(dataset[i].textset.charmap[j]);
						result.textset->chartim.push_back(dataset[i].textset.chartim[j]);
					}
					result.textset->struct_id.push_back(dataset[i].textset.struct_id[j]);
					result.textset->name.push_back(dataset[i].textset.name[j]);
					result.textset->tim_amount.push_back(dataset[i].textset.tim_amount[j]);
					result.textset->text_data.push_back(dataset[i].textset.text_data[j]);
					result.sectionloaded[DATA_SECTION_TEXT] = true;
					if (dataset[i].saveset.sectionmodified[DATA_SECTION_TEXT])
						result.sectionmodified[DATA_SECTION_TEXT] = true;
					textid.insert(dataset[i].textset.struct_id[j]);
				}
			}
		}
		if (dataset[i].saveset.sectionloaded[DATA_SECTION_WORLD_MAP]) {
			for (j = 0; j < dataset[i].worldset.amount; j++) {
				if (worldid.count(dataset[i].worldset.struct_id[j]) == 0) {
					if (dataset[i].gametype == GAME_TYPE_PSX) {
						result.worldset->cluster_id.push_back(dataset[i].worldset.cluster_id[j]);
						result.worldset->charmap.push_back(dataset[i].worldset.charmap[j]);
						result.worldset->chartim.push_back(dataset[i].worldset.chartim[j]);
						result.worldset->preload.push_back(dataset[i].worldset.preload[j]);
					}
					result.worldset->struct_id.push_back(dataset[i].worldset.struct_id[j]);
					result.worldset->name.push_back(dataset[i].worldset.name[j]);
					result.worldset->tim_amount.push_back(dataset[i].worldset.tim_amount[j]);
					result.worldset->script.push_back(dataset[i].worldset.script[j]);
					result.worldset->text_data.push_back(dataset[i].worldset.text_data[j]);
					result.worldset->world_data = dataset[i].worldset.world_data;
					result.sectionloaded[DATA_SECTION_WORLD_MAP] = true;
					if (dataset[i].saveset.sectionmodified[DATA_SECTION_WORLD_MAP])
						result.sectionmodified[DATA_SECTION_WORLD_MAP] = true;
					worldid.insert(dataset[i].worldset.struct_id[j]);
				}
			}
		}
		if (dataset[i].gametype == GAME_TYPE_PSX) {
			if (dataset[i].saveset.sectionloaded[DATA_SECTION_BATTLE_SCENE]) {
				for (j = 0; j < dataset[i].sceneset.scene_amount; j++) {
					if (sceneid.count(dataset[i].sceneset.cluster_id[j]) == 0) {
						result.sceneset->cluster_id.push_back(dataset[i].sceneset.cluster_id[j]);
						result.sceneset->scene_name.push_back(dataset[i].sceneset.scene_name[j]);
						result.sceneset->scene.push_back(dataset[i].sceneset.scene[j]);
						result.sceneset->image.push_back(dataset[i].sceneset.image[j]);
						result.sectionloaded[DATA_SECTION_BATTLE_SCENE] = true;
						if (dataset[i].saveset.sectionmodified[DATA_SECTION_BATTLE_SCENE])
							result.sectionmodified[DATA_SECTION_BATTLE_SCENE] = true;
						sceneid.insert(dataset[i].sceneset.cluster_id[j]);
					}
				}
			}
		}
		if (dataset[i].saveset.sectionloaded[DATA_SECTION_FIELD]) {
			for (j = 0; j < dataset[i].fieldset.amount; j++) {
				if (fieldid.count(dataset[i].fieldset.struct_id[j]) == 0) {
					if (dataset[i].gametype == GAME_TYPE_PSX)
						result.fieldset->cluster_id.push_back(dataset[i].fieldset.cluster_id[j]);
					result.fieldset->struct_id.push_back(dataset[i].fieldset.struct_id[j]);
					result.fieldset->script_data.push_back(dataset[i].fieldset.script_data[j]);
					result.fieldset->preload.push_back(dataset[i].fieldset.preload[j]);
					result.fieldset->background_data.push_back(dataset[i].fieldset.background_data[j]);
					result.fieldset->walkmesh.push_back(dataset[i].fieldset.walkmesh[j]);
					result.fieldset->tim_data.push_back(dataset[i].fieldset.tim_data[j]);
					result.fieldset->role.push_back(dataset[i].fieldset.role[j]);
					result.fieldset->related_text.push_back(dataset[i].fieldset.related_text[j]);
					result.sectionloaded[DATA_SECTION_FIELD] = true;
					if (dataset[i].saveset.sectionmodified[DATA_SECTION_FIELD])
						result.sectionmodified[DATA_SECTION_FIELD] = true;
					fieldid.insert(dataset[i].fieldset.struct_id[j]);
				}
			}
		}
	}
	result.enemyset->battle_amount = result.enemyset->struct_id.size();
	result.textset->amount = result.textset->struct_id.size();
	result.worldset->amount = result.worldset->struct_id.size();
	result.sceneset->scene_amount = result.sceneset->cluster_id.size();
	result.fieldset->amount = result.fieldset->struct_id.size();
	for (i = 0; i < dataset.size(); i++)
		for (j = 0; j < DATA_SECTION_AMOUNT; j++)
			if (dataset[i].saveset.sectionmodified[j])
				result.sectionmodified[j] = true;
	return result;
}

void CommandFrame::DisposeOfMergedDataSet(SaveSet saveset) {
	delete saveset.enemyset;
	delete saveset.textset;
	delete saveset.worldset;
	delete saveset.sceneset;
	delete saveset.fieldset;
}

void CommandFrame::ExecuteCommands(wxFile& input, wxString outputname) {
	if (!input.ReadAll(&completefile)) {
		Close();
		return;
	}
	while (completefile.EndsWith(_(L"\n")))
		completefile.Truncate(completefile.Len() - 1);
	if (!outputname.IsEmpty())
		output.Open(outputname, wxFile::write);
	m_morecommand->Enable(false);
	executing_commands = true;
	executing_extra_commands = false;
	line_no = -1;
	m_commands->SetValue(completefile + _(L"\n"));
	CommandThread* cmdthread = new CommandThread(this);
	if (cmdthread->Run() != wxTHREAD_NO_ERROR) {
		delete cmdthread;
		executing_commands = false;
		m_morecommand->Enable(true);
		WriteToOutput(_(L"the program has no resource to run"));
	}
}

void CommandFrame::WriteToOutput(wxString str) {
	m_output->AppendText(str);
	if (output.IsOpened())
		output.Write(str);
}

bool CommandFrame::RaiseError(wxString errstr) {
	current_line_error = true;
	WriteToOutput(wxString::Format(wxT("Error L%d: "), line_no + 1) + errstr);
	return !error_stop;
}

bool CommandFrame::RaiseWarning(wxString warnstr) {
	current_line_warning = true;
	WriteToOutput(wxString::Format(wxT("Warning L%d: "), line_no + 1) + warnstr);
	return !warning_stop;
}

bool CommandFrame::ProcessLine(wxString line) {
	line.Trim().Trim(false);
	if (line.StartsWith(L"//") || line.StartsWith(L"#") || line.IsEmpty())
		return true;
	size_t next, pos = line.find_first_of(_(L" \t"));
	wxString command = line.substr(0, pos);
	wxArrayString argarr;
	wxString argtoken;
	if (pos == string::npos)
		return ProcessCommand(command, argarr);
	pos++;
	next = line.find_first_of(_(L" \t\""), pos);
	while (true) {
		if (next == string::npos) {
			argtoken = line.substr(pos);
			argtoken.Trim().Trim(false);
			if (!argtoken.IsEmpty())
				argarr.Add(argtoken);
			break;
		}
		if (line[next] == L'\"') {
			pos = next + 1;
			next = line.find(L'\"', pos);
			if (next == string::npos) {
				argarr.Add(line.substr(pos));
				break;
			} else {
				argarr.Add(line.substr(pos, next - pos));
				pos = next + 1;
				next = line.find_first_of(_(L" \t\""), pos);
			}
		} else {
			argtoken = line.substr(pos, next - pos);
			argtoken.Trim().Trim(false);
			if (!argtoken.IsEmpty())
				argarr.Add(argtoken);
			pos = next + 1;
			next = line.find_first_of(_(L" \t\""), pos);
		}
	}
	return ProcessCommand(command, argarr);
}

bool CommandFrame::ProcessCommand(wxString command, wxArrayString args) {
	current_line_executed = true;
	if (command.IsSameAs("set")) {
		if (dataset.size() == 0)
			return RaiseError(_(L"'set' cannot be used before opening at least 1 game\n"));
		return ProcessCommandSet(args);
	} else if (command.IsSameAs("select")) {
		if (dataset.size() == 0)
			return RaiseError(_(L"'select' cannot be used before opening at least 1 game\n"));
		return ProcessCommandSelect(args);
	} else if (command.IsSameAs("config")) {
		return ProcessCommandConfig(args);
	} else if (command.IsSameAs("load")) {
		if (dataset.size() == 0)
			return RaiseError(_(L"'load' cannot be used before opening at least 1 game\n"));
		return ProcessCommandLoad(args);
	} else if (command.IsSameAs("save")) {
		if (dataset.size() == 0)
			return RaiseError(_(L"'save' cannot be used before opening at least 1 game\n"));
		return ProcessCommandSave(args);
	} else if (command.IsSameAs("open")) {
		return ProcessCommandOpen(args);
	} else if (command.IsSameAs("import")) {
		if (dataset.size() == 0)
			return RaiseError(_(L"'import' cannot be used before opening at least 1 game\n"));
		return ProcessCommandImport(args);
	} else if (command.IsSameAs("export")) {
		if (dataset.size() == 0)
			return RaiseError(_(L"'export' cannot be used before opening at least 1 game\n"));
		return ProcessCommandExport(args);
	} else if (command.IsSameAs("exit")) {
		exit = true;
		return false;
	} else if (command.IsSameAs("help")) {
		return ProcessCommandHelp(args);
	}
	return RaiseWarning(wxString::Format(wxT("unrecognized command '%s'\n"), command));
}

bool CommandFrame::ProcessCommandConfig(wxArrayString args) {
	bool errorformat = false;
	if (args.GetCount() < 2) {
		errorformat = true;
	} else {
		if (args[0].IsSameAs("exit_on_stop")) {
			exit_on_stop = args[1].IsSameAs("true", false) || args[1].IsSameAs("1");
			WriteToOutput(_("Configure exit_on_stop to ") + (exit_on_stop ? _("true\n") : _("false\n")));
		} else if (args[0].IsSameAs("warning_stop")) {
			warning_stop = args[1].IsSameAs("true", false) || args[1].IsSameAs("1");
			WriteToOutput(_("Configure warning_stop to ") + (warning_stop ? _("true\n") : _("false\n")));
		} else if (args[0].IsSameAs("error_stop")) {
			error_stop = args[1].IsSameAs("true", false) || args[1].IsSameAs("1");
			WriteToOutput(_("Configure error_stop to ") + (error_stop ? _("true\n") : _("false\n")));
		} else if(args[0].IsSameAs("auto_load")) {
			auto_load = args[1].IsSameAs("true", false) || args[1].IsSameAs("1");
			WriteToOutput(_("Configure auto_load to ") + (auto_load ? _("true\n") : _("false\n")));
		} else if (args[0].IsSameAs("background_resolution")) {
			hades::FIELD_BACKGROUND_RESOLUTION = wxAtoi(args[1]);
			WriteToOutput(wxString::Format(wxT("Configure background_resolution to %d\n"), hades::FIELD_BACKGROUND_RESOLUTION));
		} else if (args[0].IsSameAs("steam_language")) {
			SteamLanguage sl = (SteamLanguage)max(0, min(STEAM_LANGUAGE_AMOUNT - 1, wxAtoi(args[1])));
			SetSteamLanguage(sl);
			WriteToOutput(wxString::Format(wxT("Configure steam_language to %d (%s)\n"), sl, HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX[sl]));
		} else if (args[0].IsSameAs("steam_language_single")) {
			hades::STEAM_SINGLE_LANGUAGE_MODE = args[1].IsSameAs("true", false) || args[1].IsSameAs("1");
			WriteToOutput(_("Configure steam_language_single to ") + (hades::STEAM_SINGLE_LANGUAGE_MODE ? _("true\n") : _("false\n")));
		} else if (args[0].IsSameAs("steam_language_save")) {
			int listvalue = wxAtoi(args[1]);
			for (int i = 0; i < STEAM_LANGUAGE_AMOUNT; i++) {
				hades::STEAM_LANGUAGE_SAVE_LIST[i] = (listvalue & (1 << i)) != 0;
				WriteToOutput(wxString::Format(wxT("Configure steam_language_save[%s] to %s\n"), HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX[i], (hades::STEAM_LANGUAGE_SAVE_LIST[i] ? _("true") : _("false"))));
			}
		} else {
			errorformat = true;
		}
	}
	if (errorformat)
		return RaiseWarning(_("usage is 'config NAME VALUE'\nNAME can be one of the followings: 'exit_on_stop', 'warning_stop', 'error_stop', 'auto_load', 'background_resolution', 'steam_language', 'steam_language_single', 'steam_language_save'\n"));
	return true;
}

bool CommandFrame::ProcessCommandOpen(wxArrayString args) {
	if (args.GetCount() < 1)
		return RaiseError(_("usage is 'open FILENAME'\nFILENAME can be either a PSX binary image or the 'FF9_Launcher.exe' of the PC version\n"));
	for (unsigned int i = 0; i < args.GetCount(); i++) {
		string filename = args[i].ToStdString();
		GameType gt = GAME_TYPE_PSX;
		if (filename.substr(filename.length() - 4).compare(".exe") == 0)
			gt = GAME_TYPE_STEAM;
		if (dataset.size() > 0 && dataset[0].gametype != gt) {
			if (!RaiseError(wxString::Format(wxT("%s is not of the same game type as %s; you may only open different games of the same type\n"), dataset[0].displayname, filename)))
				return false;
			continue;
		}
		ConfigurationSet* configalloc = new ConfigurationSet;
		ConfigurationSet& config = *configalloc;
		size_t dirsep = filename.find_last_of("/\\") + 1;
		int res;
		if (gt == GAME_TYPE_PSX) {
			WriteToOutput(wxString::Format(wxT("Reading the .hwf of the file '%s'..."), filename));
			res = FileCheckConfig(filename, config);
			WriteToOutput(res == 0 ? _(" done\n") : _(" fail\n"));
		} else {
			WriteToOutput(wxString::Format(wxT("Opening the folder '%s' as a Steam game..."), filename.substr(0, dirsep)));
			res = InitSteamConfiguration(filename.substr(0, dirsep), config);
			WriteToOutput(res == 0 ? _(" done\n") : _(" fail\n"));
		}
		switch (res) {
		case -1:
			if (!RaiseError(wxString::Format(wxT("cannot open the file '%s'\n"), filename)))
				return false;
			continue;
		case 1:
		case -2: {
			WriteToOutput(wxString::Format(wxT("Scanning the file to see if it is a PSX binary image..."), filename));
			int fres = FindConfiguration(filename, config);
			WriteToOutput(fres == 0 ? _(" done\n") : _(" fail\n"));
			switch (fres) {
			case -1:
				if (!RaiseError(wxString::Format(wxT("cannot open '%s'\n"), filename)))
					return false;
				continue;
			case -2:
				if (!RaiseError(wxString::Format(wxT("'%s' doesn't seem to be a PSX binary image\n"), filename)))
					return false;
				continue;
			case -3:
				if (!RaiseError(wxString::Format(wxT("'%s' seems to be RNC-compressed\n"), filename)))
					return false;
				continue;
			default:
				WriteToOutput(wxString::Format(wxT("Generating a .hwf file..."), filename));
				WriteToOutput(WriteConfiguration(filename, config) == 0 ? _(" done\n") : _(" fail\n"));
			}
			break;
		}
		case -3:
			if (!RaiseError(_("a key file is missing\n")))
				return false;
			continue;
		case -4:
			if (!RaiseError(_("could not find an usable Assembly-CSharp.dll\n")))
				return false;
			continue;
		}
		if (gt == GAME_TYPE_PSX) {
			dataset.emplace_back(config, filename, gt);
		} else {
			dataset.emplace_back(config, filename.substr(0, dirsep), gt);
			if (config.dll_usage == 1)
				WriteToOutput(_("Due to the installation of Memoria, the backup DLL 'x64/FF9_Data/Managed/Assembly-CSharp_Vanilla.dll' is used\n"));
			else if (config.dll_usage == 2)
				WriteToOutput(_("Due to the installation of Memoria, the backup DLL 'x64/FF9_Data/Managed/Assembly-CSharp.bak' is used\n"));
			else if (config.dll_usage == 3)
				WriteToOutput(_("Due to the installation of Memoria, the backup DLL 'Assembly-CSharp_Vanilla.dll' is used\n"));
		}
	}
	return true;
}

bool CommandFrame::ProcessCommandLoad(wxArrayString args) {
	if (args.GetCount() < 1)
		return RaiseError(_("usage is 'load SECTION'\nSECTION can be one (or several) of the followings: spell, support, command, stat, party_special, enemy, item, shop, card, text, world, field, scene, spell_animation, interface, assembly\n"));
	unsigned int i, j;
	bool found;
	fstream f;

	#define MACRO_HWC_LOAD_SECTION(SECTID, VAR, ARGS) \
		if (args[i].IsSameAs(const_section_name[SECTID])) { \
			found = true; \
			if (dataset[j].saveset.sectionloaded[SECTID]) { \
				WriteToOutput(_(L"The section ") + const_section_name[SECTID] + _(L" of ") + dataset[j].displayname + _(L" was already loaded\n")); \
				continue; \
			} \
			WriteToOutput(_("Loading the section ") + const_section_name[SECTID] + _(L" of ") + dataset[j].displayname + _(L"...")); \
			if (const_section_name[SECTID].IsSameAs(L"battle_scene") && dataset[j].gametype != GAME_TYPE_PSX) { \
				WriteToOutput(_(" fail\n")); \
				if (!RaiseWarning(_("can't load the section 'battle_scene' of a Steam game\n"))) \
					return false; \
			} else { \
				if (dataset[j].gametype == GAME_TYPE_PSX) f.open(dataset[j].filename.c_str(), ios::in | ios::binary); \
				dataset[j].VAR ## set.Load(f, ARGS); \
				if (dataset[j].gametype == GAME_TYPE_PSX) f.close(); \
				dataset[j].saveset.sectionloaded[SECTID] = true; \
				WriteToOutput(_(" done\n")); \
			} \
		} else

	for (j = 0; j < dataset.size(); j++) {
		SetGameType(dataset[j].gametype);
		for (i = 0; i < args.GetCount(); i++) {
			found = false;
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_SPELL, spell, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_SUPPORT, support, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_CMD, cmd, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_STAT, stat, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_PARTY_SPECIAL, partyspecial, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_ENMY, enemy, dataset[j].cluster)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_ITEM, item, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_SHOP, shop, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_CARD, card, dataset[j].config)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_TEXT, text, dataset[j].cluster)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_WORLD_MAP, world, dataset[j].cluster)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_FIELD, field, dataset[j].cluster COMMA &dataset[j].textset)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_BATTLE_SCENE, scene, dataset[j].cluster)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_SPELL_ANIM, spellanim, dataset[j].config COMMA dataset[j].cluster.global_map)
			MACRO_HWC_LOAD_SECTION(DATA_SECTION_MENU_UI, ffui, dataset[j].config)
			if (dataset[j].gametype == GAME_TYPE_PSX) {
				MACRO_HWC_LOAD_SECTION(DATA_SECTION_ASSEMBLY, mips, dataset[j].config) {}
			} else {
				MACRO_HWC_LOAD_SECTION(DATA_SECTION_ASSEMBLY, cil, &dataset[j].config.meta_dll) {}
			}
			if (!found && i == 0) {
				if (!RaiseWarning(wxString::Format(wxT("unrecognized section '%'\n"), args[i])))
					return false;
			}
		}
	}
	return true;
}

bool CommandFrame::ProcessCommandImport(wxArrayString args) {
	bool errorformat = false;
	if (args.GetCount() < 2) {
		errorformat = true;
	} else {
		unsigned int i, j;
		for (i = 0; i < dataset.size(); i++) {
			SetGameType(dataset[i].gametype);
			if (args[0].IsSameAs(L"hws")) {
				bool existingsection[DATA_SECTION_AMOUNT];
				bool section[DATA_SECTION_AMOUNT];
				bool sectext[DATA_SECTION_AMOUNT];
				bool initval = args.GetCount() == 2;
				bool localsec[3]{ initval, initval, initval };
				for (j = 0; j < DATA_SECTION_AMOUNT; j++) {
					section[j] = initval;
					sectext[j] = initval;
				}
				for (j = 2; j < args.GetCount(); j++) {
					if (args[j].IsSameAs(L"spell"))					section[DATA_SECTION_SPELL] = true;
					else if (args[j].IsSameAs(L"command"))			section[DATA_SECTION_CMD] = true;
					else if (args[j].IsSameAs(L"battle"))			section[DATA_SECTION_ENMY] = true;
					else if (args[j].IsSameAs(L"shop"))				section[DATA_SECTION_SHOP] = true;
					else if (args[j].IsSameAs(L"text"))				section[DATA_SECTION_TEXT] = true;
					else if (args[j].IsSameAs(L"battle_scene"))		section[DATA_SECTION_BATTLE_SCENE] = true;
					else if (args[j].IsSameAs(L"world"))			section[DATA_SECTION_WORLD_MAP] = true;
					else if (args[j].IsSameAs(L"item"))				section[DATA_SECTION_ITEM] = true;
					else if (args[j].IsSameAs(L"support"))			section[DATA_SECTION_SUPPORT] = true;
					else if (args[j].IsSameAs(L"stat"))				section[DATA_SECTION_STAT] = true;
					else if (args[j].IsSameAs(L"card"))				section[DATA_SECTION_CARD] = true;
					else if (args[j].IsSameAs(L"field"))			section[DATA_SECTION_FIELD] = true;
					else if (args[j].IsSameAs(L"spell_animation"))	section[DATA_SECTION_SPELL_ANIM] = true;
					else if (args[j].IsSameAs(L"interface"))		section[DATA_SECTION_MENU_UI] = true;
					else if (args[j].IsSameAs(L"party_special"))	section[DATA_SECTION_PARTY_SPECIAL] = true;
					else if (args[j].IsSameAs(L"assembly"))			section[DATA_SECTION_ASSEMBLY] = true;
					else if (args[j].IsSameAs(L"spell.text"))		sectext[DATA_SECTION_SPELL] = true;
					else if (args[j].IsSameAs(L"command.text"))		sectext[DATA_SECTION_CMD] = true;
					else if (args[j].IsSameAs(L"enemy.text"))		sectext[DATA_SECTION_ENMY] = true;
					else if (args[j].IsSameAs(L"world.text"))		sectext[DATA_SECTION_WORLD_MAP] = true;
					else if (args[j].IsSameAs(L"item.text"))		sectext[DATA_SECTION_ITEM] = true;
					else if (args[j].IsSameAs(L"support.text"))		sectext[DATA_SECTION_SUPPORT] = true;
					else if (args[j].IsSameAs(L"stat.text"))		sectext[DATA_SECTION_STAT] = true;
					else if (args[j].IsSameAs(L"card.text"))		sectext[DATA_SECTION_CARD] = true;
					else if (args[j].IsSameAs(L"field.text"))		sectext[DATA_SECTION_FIELD] = true;
					else if (args[j].IsSameAs(L"enemy.local"))		localsec[0] = true;
					else if (args[j].IsSameAs(L"world.local"))		localsec[1] = true;
					else if (args[j].IsSameAs(L"field.local"))		localsec[2] = true;
					else if (!RaiseWarning(wxString::Format(wxT("unrecognized HWS section '%s'\n"), args[j])))
						return false;
				}
				PreloadHWS(args[1].ToStdString(), existingsection);
				wxArrayString loadlist;
				for (j = 0; j < DATA_SECTION_AMOUNT; j++) {
					if (existingsection[j] && section[j] && !dataset[i].saveset.sectionloaded[j]) {
						if (auto_load) {
							loadlist.Add(const_section_name[j]);
						} else if (warning_stop) {
							return RaiseWarning(wxString::Format(wxT("the section '%s' was not loaded\n"), const_section_name[j]));
						} else {
							RaiseWarning(wxString::Format(wxT("the section '%s' was not loaded and is thus ignored\n"), const_section_name[j]));
							section[j] = false;
						}
					}
				}
				if (auto_load && !loadlist.IsEmpty())
					if (!ProcessCommandLoad(loadlist))
						return false;
				WriteToOutput(_(L"importing hws file in ") + dataset[i].displayname + _(L"..."));
				wstring* resstr = LoadHWS(args[1].ToStdString(), section, sectext, localsec, dataset[i].saveset, dataset[i].backupset);
				wxString info = resstr[0] + _(L"\n");
				bool iserror = resstr[1].compare(HADES_STRING_ERROR) == 0;
				bool iswarning = resstr[1].compare(HADES_STRING_WARNING) == 0;
				if (iserror || (iswarning && warning_stop))
					WriteToOutput(_(L" fail\n"));
				else
					WriteToOutput(_(L" done\n"));
				delete[] resstr;
				if (iserror) {
					if (!RaiseError(info))
						return false;
				} else if (iswarning) {
					if (!RaiseWarning(info))
						return false;
				}
			} else if (args[0].IsSameAs(L"text")) {
				if (!dataset[i].saveset.sectionloaded[DATA_SECTION_TEXT]) {
					if (auto_load) {
						if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_TEXT] })))
							return false;
					} else if (!RaiseWarning(_("cannot import text before the related section is loaded\n"))) {
						return false;
					}
				}
				wxFile input(args[1]);
				wxString filestr;
				if (!input.IsOpened() || !input.ReadAll(&filestr))
					return RaiseError(wxString::Format(wxT("cannot open the file '%s'\n"), args[1]));
				WriteToOutput(_("importing text blocks..."));
				LogStruct res = BatchImportDialog::ImportText(dataset[i].textset, filestr, true, dataset[i].gametype == GAME_TYPE_PSX && (dataset[i].config.language & LANGUAGE_VERSION_JAPAN) != 0, warning_stop);
				WriteToOutput(res.ok ? _(L" done\n") : _(L" fail\n"));
				if (res.error_amount > 0 && !RaiseError(res.error))
					return false;
				if (res.warning_amount > 0 && !RaiseWarning(res.warning))
					return false;
			} else if (args[0].IsSameAs(L"uitext")) {
				if (!dataset[i].saveset.sectionloaded[DATA_SECTION_MENU_UI]) {
					if (auto_load) {
						if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_MENU_UI] })))
							return false;
					} else if (!RaiseWarning(_("cannot import UI text before the related section is loaded\n"))) {
						return false;
					}
				}
				wxFile input(args[1]);
				wxString filestr;
				if (!input.IsOpened() || !input.ReadAll(&filestr))
					return RaiseError(wxString::Format(wxT("cannot open the file '%s'\n"), args[1]));
				WriteToOutput(_("importing UI text blocks..."));
				LogStruct res = BatchImportDialog::ImportSpecialText(*dataset[i].ffuiset.special_text, filestr, warning_stop);
				WriteToOutput(res.ok ? _(L" done\n") : _(L" fail\n"));
				if (res.error_amount > 0 && !RaiseError(res.error))
					return false;
				if (res.warning_amount > 0 && !RaiseWarning(res.warning))
					return false;
			} else if (args[0].IsSameAs(L"script")) {
				wxFile input(args[1]);
				wxString filestr;
				if (!input.IsOpened() || !input.ReadAll(&filestr))
					return RaiseError(wxString::Format(wxT("cannot open the file '%s'\n"), args[1]));
				WriteToOutput(_("importing scripts..."));
				LogStruct res = BatchImportDialog::ImportScript(&dataset[i].saveset, SCRIPT_TYPE_ANY, filestr, warning_stop);
				WriteToOutput(res.ok ? _(L" done\n") : _(L" fail\n"));
				bool keeploading = true;
				while (!res.ok && auto_load && keeploading) {
					if (res.error.compare(HADES_STRING_BATCH_FIELD_NOT_LOADED) == 0)
						keeploading = ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_FIELD] }));
					else if (res.error.compare(HADES_STRING_BATCH_WORLD_NOT_LOADED) == 0)
						keeploading = ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_WORLD_MAP] }));
					else if (res.error.compare(HADES_STRING_BATCH_ENEMY_NOT_LOADED) == 0)
						keeploading = ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_ENMY] }));
					else
						keeploading = false;
					if (keeploading) {
						WriteToOutput(_("retrying to import scripts..."));
						res = BatchImportDialog::ImportScript(&dataset[i].saveset, SCRIPT_TYPE_ANY, args[1], warning_stop);
						WriteToOutput(res.ok ? _(L" done\n") : _(L" fail\n"));
					}
				}
				if (res.error_amount > 0 && !RaiseError(res.error))
					return false;
				if (res.warning_amount > 0 && !RaiseWarning(res.warning))
					return false;
			} else {
				errorformat = true;
			}
		}
	}
	if (errorformat)
		return RaiseError(_("usage is one of the followings:\n 'import hws FILENAME [SECTIONS]'\n 'import text FILENAME'\n 'import uitext FILENAME'\n 'import script FILENAME'\n"));
	return true;
}

void ProcessCommandExport_Rec(CommandFrame* frame, int format, wxFile& file, SaveSet& mset, FieldSelector& treesel, DataStructureBranch* branch) {
	unsigned int paramindex = 0;
	unsigned int i;
	if (format == 1) {
		if (file.Tell() > 0)
			file.Write(_(L"\n"));
		if (branch->children.size() == 0)
			file.Write(wxString::Format(wxT("# %s"), branch->name));
		else
			file.Write(wxString::Format(wxT("# %s\nID/index"), branch->name));
		for (i = 0; i < branch->children.size(); i++)
			file.Write(wxString::Format(wxT(";%s"), branch->children[i].name));
	}
	bool firstit = true;
	while (branch->parameter->enumerator(mset, treesel, paramindex++)) {
		if (format == 1) {
			if (firstit && branch->children.size() == 0)
				file.Write(wxString::Format(wxT(" (%d)"), branch->parameter->getter(treesel)));
			if (!firstit && branch->next.size() > 0) {
				if (branch->children.size() == 0)
					file.Write(wxString::Format(wxT("\n# ...%s (%d)"), branch->name, branch->parameter->getter(treesel)));
				else
					file.Write(wxString::Format(wxT("\n# ...%s"), branch->name));
			}
			if (branch->children.size() > 0)
				file.Write(wxString::Format(wxT("\n%d"), branch->parameter->getter(treesel)));
		} else {
			file.Write(wxString::Format(wxT("select %s %d\n"), branch->parameter->name, branch->parameter->getter(treesel)));
		}
		for (i = 0; i < branch->children.size(); i++) {
			if (branch->children[i].field->checker(mset, treesel)) {
				if (branch->children[i].field->get_type() == 0) {
					editable_field_numerical* numfield = dynamic_cast<editable_field_numerical*>(branch->children[i].field);
					if (format == 1)
						file.Write(wxString::Format(wxT(";%d"), numfield->getter(mset, treesel)));
					else
						file.Write(wxString::Format(wxT("set %s %d\n"), numfield->name, numfield->getter(mset, treesel)));
				} else if (branch->children[i].field->get_type() == 1) {
					editable_field_string* strfield = dynamic_cast<editable_field_string*>(branch->children[i].field);
					if (format == 1)
						file.Write(wxString::Format(wxT(";\"%s\""), strfield->getter(mset, treesel)));
					else
						file.Write(wxString::Format(wxT("set %s \"%s\"\n"), strfield->name, strfield->getter(mset, treesel)));
				}
			} else {
				if (format == 1)
					file.Write(_(";_"));
			}
		}
		// TODO: more formats...
		for (i = 0; i < branch->next.size(); i++)
			ProcessCommandExport_Rec(frame, format, file, mset, treesel, &branch->next[i]);
		firstit = false;
	}
}

bool CommandFrame::ProcessCommandExport(wxArrayString args) {
	bool errorformat = false;
	if (args.GetCount() < 2) {
		errorformat = true;
	} else {
		unsigned int i, j;
		SetGameType(dataset[0].gametype);
		if (args[0].IsSameAs(L"hws")) {
			bool section[DATA_SECTION_AMOUNT];
			bool localsec[3]{ false, false, false };
			for (j = 0; j < DATA_SECTION_AMOUNT; j++)
				section[j] = false;
			if (args.GetCount() == 2) {
				for (i = 0; i < dataset.size(); i++)
					for (j = 0; j < DATA_SECTION_AMOUNT; j++)
						if (dataset[i].saveset.sectionmodified[j]) {
							section[j] = true;
							if (j == DATA_SECTION_ENMY)
								localsec[0] = true;
							else if (j == DATA_SECTION_WORLD_MAP)
								localsec[1] = true;
							else if (j == DATA_SECTION_FIELD)
								localsec[2] = true;
						}
			} else {
				for (j = 2; j < args.GetCount(); j++) {
					if (args[j].IsSameAs(L"spell"))					section[DATA_SECTION_SPELL] = true;
					else if (args[j].IsSameAs(L"command"))			section[DATA_SECTION_CMD] = true;
					else if (args[j].IsSameAs(L"battle"))			section[DATA_SECTION_ENMY] = true;
					else if (args[j].IsSameAs(L"shop"))				section[DATA_SECTION_SHOP] = true;
					else if (args[j].IsSameAs(L"text"))				section[DATA_SECTION_TEXT] = true;
					else if (args[j].IsSameAs(L"battle_scene"))		section[DATA_SECTION_BATTLE_SCENE] = true;
					else if (args[j].IsSameAs(L"world"))			section[DATA_SECTION_WORLD_MAP] = true;
					else if (args[j].IsSameAs(L"item"))				section[DATA_SECTION_ITEM] = true;
					else if (args[j].IsSameAs(L"support"))			section[DATA_SECTION_SUPPORT] = true;
					else if (args[j].IsSameAs(L"stat"))				section[DATA_SECTION_STAT] = true;
					else if (args[j].IsSameAs(L"card"))				section[DATA_SECTION_CARD] = true;
					else if (args[j].IsSameAs(L"field"))			section[DATA_SECTION_FIELD] = true;
					else if (args[j].IsSameAs(L"spell_animation"))	section[DATA_SECTION_SPELL_ANIM] = true;
					else if (args[j].IsSameAs(L"interface"))		section[DATA_SECTION_MENU_UI] = true;
					else if (args[j].IsSameAs(L"party_special"))	section[DATA_SECTION_PARTY_SPECIAL] = true;
					else if (args[j].IsSameAs(L"assembly"))			section[DATA_SECTION_ASSEMBLY] = true;
					else if (args[j].IsSameAs(L"enemy.local"))		localsec[0] = true;
					else if (args[j].IsSameAs(L"world.local"))		localsec[1] = true;
					else if (args[j].IsSameAs(L"field.local"))		localsec[2] = true;
					else if (!RaiseWarning(wxString::Format(wxT("unrecognized HWS section '%s'\n"), args[j])))
						return false;
				}
			}
			wxArrayString loadlist;
			for (j = 0; j < DATA_SECTION_AMOUNT; j++) {
				if (section[j] && !dataset[i].saveset.sectionloaded[j]) {
					if (auto_load) {
						loadlist.Add(const_section_name[j]);
					} else if (warning_stop) {
						return RaiseWarning(wxString::Format(wxT("the section '%s' was not loaded\n"), const_section_name[j]));
					} else {
						RaiseWarning(wxString::Format(wxT("the section '%s' was not loaded and is thus ignored\n"), const_section_name[j]));
						section[j] = false;
					}
				}
			}
			if (auto_load && !loadlist.IsEmpty())
				if (!ProcessCommandLoad(loadlist))
					return false;
			WriteToOutput(_(L"exporting hws file..."));
			SaveSet mergedset = MergeDataSets();
			UnusedSaveBackup nullbackup;
			int res = WriteHWS(args[1].ToStdString(), section, localsec, mergedset, nullbackup);
			if (res != 0)
				WriteToOutput(_(L" fail\n"));
			else
				WriteToOutput(_(L" done\n"));
			DisposeOfMergedDataSet(mergedset);
			return res == 0 || !error_stop;
		} else if (args[0].IsSameAs(L"text")) {
			bool hastextloaded = false;
			for (i = 0; i < dataset.size(); i++)
				if (dataset[i].saveset.sectionloaded[DATA_SECTION_TEXT])
					hastextloaded = true;
			if (!hastextloaded) {
				if (auto_load) {
					if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_TEXT] })))
						return false;
				} else if (!RaiseWarning(_("cannot export texts before the related section is loaded\n"))) {
					return false;
				}
			}
			if (!hastextloaded && !auto_load)
				return true;
			TextDataSet mergedtext;
			set<uint16_t> hastext;
			for (i = 0; i < dataset.size(); i++) {
				for (j = 0; j < dataset[i].textset.amount; j++) {
					if (hastext.count(dataset[i].textset.struct_id[j]) == 0) {
						mergedtext.struct_id.push_back(dataset[i].textset.struct_id[j]);
						mergedtext.name.push_back(dataset[i].textset.name[j]);
						mergedtext.text_data.push_back(dataset[i].textset.text_data[j]);
						mergedtext.tim_amount.push_back(0);
						hastext.insert(dataset[i].textset.struct_id[j]);
					}
				}
			}
			mergedtext.amount = mergedtext.struct_id.size();
			if (args.GetCount() == 2) {
				WriteToOutput(wxString::Format(wxT("exporting %d text blocks..."), mergedtext.amount));
				int res = BatchExportDialog::ExportText(mergedtext, args[1], NULL, false);
				WriteToOutput(res == 0 ? _(L" done\n") : _(L" fail\n"));
			} else {
				long textid;
				bool* exportlist = new bool[mergedtext.amount];
				int exportcount = 0;
				for (i = 0; i < mergedtext.amount; i++)
					exportlist[i] = false;
				for (i = 2; i < args.GetCount(); i++) {
					if (!args[i].ToLong(&textid)) {
						errorformat = true;
						break;
					}
					if (hastext.count(textid) == 0) {
						if (!RaiseWarning(wxString::Format(wxT("there is no text block with ID %d in the data\n"), textid)))
							return false;
					} else {
						for (j = 0; j < mergedtext.amount; j++)
							if (mergedtext.struct_id[j] == textid) {
								exportlist[j] = true;
								exportcount++;
								break;
							}
					}
				}
				if (!errorformat) {
					WriteToOutput(wxString::Format(wxT("exporting %d text blocks..."), exportcount));
					int res = BatchExportDialog::ExportText(mergedtext, args[1], exportlist, false);
					WriteToOutput(res == 0 ? _(L" done\n") : _(L" fail\n"));
				}
				delete[] exportlist;
			}
		} else if (args[0].IsSameAs(L"uitext")) {
			if (!dataset[0].saveset.sectionloaded[DATA_SECTION_MENU_UI]) {
				if (auto_load) {
					if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[DATA_SECTION_MENU_UI] })))
						return false;
				} else if (!RaiseWarning(_("cannot export UI texts before the related section is loaded\n"))) {
					return false;
				}
			}
			if (!dataset[0].saveset.sectionloaded[DATA_SECTION_MENU_UI] && !auto_load)
				return true;
			unsigned int amount = dataset[0].ffuiset.special_text->amount;
			if (args.GetCount() == 2) {
				WriteToOutput(wxString::Format(wxT("exporting %d UI text blocks..."), amount));
				int res = BatchExportDialog::ExportSpecialText(*dataset[0].ffuiset.special_text, args[1], NULL, false);
				WriteToOutput(res == 0 ? _(L" done\n") : _(L" fail\n"));
			} else {
				long uitextid;
				bool* exportlist = new bool[amount];
				int exportcount = 0;
				for (i = 0; i < amount; i++)
					exportlist[i] = false;
				for (i = 2; i < args.GetCount(); i++) {
					if (!args[i].ToLong(&uitextid)) {
						errorformat = true;
						break;
					}
					if (uitextid < 0 || uitextid >= (long)amount) {
						if (!RaiseWarning(wxString::Format(wxT("there is no UI text block with ID %d in the data\n"), uitextid)))
							return false;
					} else {
						exportlist[uitextid] = true;
						exportcount++;
					}
				}
				if (!errorformat) {
					WriteToOutput(wxString::Format(wxT("exporting %d UI text blocks..."), exportcount));
					int res = BatchExportDialog::ExportSpecialText(*dataset[0].ffuiset.special_text, args[1], exportlist, false);
					WriteToOutput(res == 0 ? _(L" done\n") : _(L" fail\n"));
				}
				delete[] exportlist;
			}
		} else if (args[0].IsSameAs(L"script")) {
			if (args.GetCount() < 3)
				errorformat = true;
			int datasection = 0;
			if (args[1].IsSameAs("field"))
				datasection = DATA_SECTION_FIELD;
			else if (args[1].IsSameAs("battle"))
				datasection = DATA_SECTION_ENMY;
			else if (args[1].IsSameAs("world"))
				datasection = DATA_SECTION_WORLD_MAP;
			else
				errorformat = true;
			if (!errorformat) {
				bool hasscriptloaded = false;
				for (i = 0; i < dataset.size(); i++)
					if (dataset[i].saveset.sectionloaded[datasection])
						hasscriptloaded = true;
				if (!hasscriptloaded) {
					if (auto_load) {
						if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[datasection] })))
							return false;
					} else if (!RaiseWarning(wxString::Format(wxT("cannot export %s scripts before the related section is loaded\n"), args[1]))) {
						return false;
					}
				}
				if (!hasscriptloaded && !auto_load)
					return true;
				SaveSet mergedset(dataset[0].saveset);
				EnemyDataSet mergedenmy;
				FieldDataSet mergedfield;
				WorldMapDataSet mergedworld;
				uint16_t* mergedamountptr;
				vector<uint16_t>* mergedstructidptr;
				if (datasection == DATA_SECTION_ENMY) {
					mergedset.enemyset = &mergedenmy;
					mergedamountptr = &mergedenmy.battle_amount;
					mergedstructidptr = &mergedenmy.struct_id;
				} else if (datasection == DATA_SECTION_FIELD) {
					mergedset.fieldset = &mergedfield;
					mergedamountptr = &mergedfield.amount;
					mergedstructidptr = &mergedfield.struct_id;
				} else {
					mergedset.worldset = &mergedworld;
					mergedamountptr = &mergedworld.amount;
					mergedstructidptr = &mergedworld.struct_id;
				}
				*mergedamountptr = 0;
				set<uint16_t> hasscript;
				for (i = 0; i < dataset.size(); i++) {
					uint16_t amount = datasection == DATA_SECTION_FIELD ? dataset[i].fieldset.amount :
						datasection == DATA_SECTION_ENMY ? dataset[i].enemyset.battle_amount : dataset[i].worldset.amount;
					vector<uint16_t> id = datasection == DATA_SECTION_FIELD ? dataset[i].fieldset.struct_id :
						datasection == DATA_SECTION_ENMY ? dataset[i].enemyset.struct_id : dataset[i].worldset.struct_id;
					for (j = 0; j < amount; j++) {
						if (hasscript.count(id[j]) == 0) {
							if (datasection == DATA_SECTION_FIELD) {
								mergedfield.related_text.push_back(dataset[i].fieldset.related_text[j]);
								mergedfield.script_data.push_back(dataset[i].fieldset.script_data[j]);
							} else if (datasection == DATA_SECTION_ENMY) {
								mergedenmy.battle_name.push_back(dataset[i].enemyset.battle_name[j]);
								mergedenmy.battle_data.push_back(dataset[i].enemyset.battle_data[j]);
								mergedenmy.battle.push_back(dataset[i].enemyset.battle[j]);
								mergedenmy.text.push_back(dataset[i].enemyset.text[j]);
								mergedenmy.script.push_back(dataset[i].enemyset.script[j]);
							} else {
								mergedworld.text_data.push_back(dataset[i].worldset.text_data[j]);
								mergedworld.script.push_back(dataset[i].worldset.script[j]);
							}
							mergedstructidptr->push_back(id[j]);
							(*mergedamountptr)++;
							hasscript.insert(id[j]);
						}
					}
				}
				int (*exportfunc)(SaveSet*, wxString, bool*, bool, int) = datasection == DATA_SECTION_FIELD ? &BatchExportDialog::ExportFieldScript :
					datasection == DATA_SECTION_ENMY ? &BatchExportDialog::ExportEnemyScript : &BatchExportDialog::ExportWorldScript;
				if (args.GetCount() == 3) {
					WriteToOutput(wxString::Format(wxT("exporting %d %s scripts..."), *mergedamountptr, args[1]));
					int res = (*exportfunc)(&mergedset, args[2], NULL, false, BATCHING_SCRIPT_INFO_ALL);
					WriteToOutput(res == 0 ? _(L" done\n") : _(L" fail\n"));
				} else {
					long scriptid;
					bool* exportlist = new bool[*mergedamountptr];
					int exportcount = 0;
					for (i = 0; i < *mergedamountptr; i++)
						exportlist[i] = false;
					for (i = 3; i < args.GetCount(); i++) {
						if (!args[i].ToLong(&scriptid)) {
							errorformat = true;
							break;
						}
						if (hasscript.count(scriptid) == 0) {
							if (!RaiseWarning(wxString::Format(wxT("there is no %s scripts with ID %d in the data\n"), args[1], scriptid)))
								return false;
						} else {
							for (j = 0; j < *mergedamountptr; j++)
								if ((*mergedstructidptr)[j] == scriptid) {
									exportlist[j] = true;
									exportcount++;
									break;
								}
						}
					}
					if (!errorformat) {
						WriteToOutput(wxString::Format(wxT("exporting %d %s scripts..."), exportcount, args[1]));
						int res = (*exportfunc)(&mergedset, args[2], exportlist, false, BATCHING_SCRIPT_INFO_ALL);
						WriteToOutput(res == 0 ? _(L" done\n") : _(L" fail\n"));
					}
					delete[] exportlist;
				}
			}
		} else if (args[0].IsSameAs(L"data")) {
			unsigned int i, j, k;
			int format = 0;
			if (args[1].IsSameAs("spreadsheet"))	format = 1;
			else if (args[1].IsSameAs("reimport"))	format = 2;
			else if (args[1].IsSameAs("plain"))		format = 0;
			//else if (!RaiseWarning(wxString::Format(wxT("'%s' is not recognized as a valid format for data exporting\n"), args[1])))
			//	return false;
			wxFile dataoutput(args[2], wxFile::write);
			if (!dataoutput.IsOpened())
				return RaiseError(wxString::Format(wxT("unable to create or overwrite %s\n"), args[2]));
			bool dataloaded[DATA_SECTION_AMOUNT];
			bool dataloadedchecked[DATA_SECTION_AMOUNT];
			for (i = 0; i < DATA_SECTION_AMOUNT; i++) {
				dataloaded[i] = false;
				dataloadedchecked[i] = false;
			}
			for (j = 0; j < dataset.size(); j++)
				for (i = 0; i < DATA_SECTION_AMOUNT; i++)
					if (dataset[j].saveset.sectionloaded[i])
						dataloaded[i] = true;
			DataStructureBranch datatreeroot(wxEmptyString, NULL);
			bool foundargfit, foundnode;
			set<int> parameterused;
			set<int> fieldused;
			wxString prefix;
			size_t dotpos;
			for (i = 3; i < args.GetCount(); i++) {
				foundargfit = false;
				for (j = 0; j < EDITABLE_FIELD_COUNT; j++) {
					if (const_editable_field[j]->name.StartsWith(args[i]) && (const_editable_field[j]->name.Len() == args[i].Len() || const_editable_field[j]->name[args[i].Len()] == L'.')) {
						foundargfit = true;
						if (fieldused.count(j) == 0) {
							fieldused.insert(j);
							DataStructureBranch* node = &datatreeroot;
							dotpos = const_editable_field[j]->name.find(L'.', 0);
							while (dotpos != wxString::npos) {
								prefix = const_editable_field[j]->name.Mid(0, dotpos);
								foundnode = false;
								for (k = 0; k < node->next.size(); k++)
									if (prefix.IsSameAs(node->next[k].name)) {
										node = &node->next[k];
										foundnode = true;
										break;
									}
								if (!foundnode) {
									for (k = 0; k < INDEX_PARAMETER_COUNT; k++) {
										if (prefix.IsSameAs(const_index_parameter[k].name)) {
											if (!dataloaded[const_index_parameter[k].section] && !dataloadedchecked[const_index_parameter[k].section]) {
												if (auto_load) {
													if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[const_index_parameter[k].section] })))
														return false;
													dataloaded[const_index_parameter[k].section] = true;
												} else if (!RaiseWarning(wxString::Format(wxT("cannot export %s-related data before the section is loaded\n"), const_section_name[const_index_parameter[k].section]))) {
													return false;
												}
												dataloadedchecked[const_index_parameter[k].section] = true;
											}
											if (dataloaded[const_index_parameter[k].section]) {
												node->next.emplace_back(const_index_parameter[k].name, &const_index_parameter[k]);
												node = &node->next.back();
											}
											break;
										}
									}
								}
								dotpos = const_editable_field[j]->name.find(L'.', dotpos + 1);
							}
							node->children.emplace_back(const_editable_field[j]->name, const_editable_field[j]);
						}
					}
				}
				if (!foundargfit) {
					if (!RaiseWarning(wxString::Format(wxT("'%s' doesn't correspond to a data field\n"), args[i])))
						return false;
				}
			}
			if (datatreeroot.next.size() == 0)
				return RaiseWarning(wxString::Format(wxT("no data to export\n"), args[i]));
			WriteToOutput(wxString::Format(wxT("exporting a database to '%s'..."), args[2]));
			SaveSet mergedset = MergeDataSets();
			FieldSelector treeselector;
			for (i = 0; i < datatreeroot.next.size(); i++)
				ProcessCommandExport_Rec(this, format, dataoutput, mergedset, treeselector, &datatreeroot.next[i]);
			DisposeOfMergedDataSet(mergedset);
			WriteToOutput(_(" done\n"));
		} else {
			errorformat = true;
		}
	}
	if (errorformat)
		return RaiseError(_("usage is one of the followings:\n 'export hws FILENAME [SECTIONS]'\n 'export text FILENAME [TEXTID]'\n 'export uitext FILENAME [UITEXTID]'\n 'export script field/enemy/world FILENAME [FIELDID/BATTLEID/WORLDID]'\n 'export data FORMAT FILENAME DATASTRUCTURE'\n"));
	return true;
}

bool CommandFrame::ProcessCommandSave(wxArrayString args) {
	bool errorformat = false;
	if (args.GetCount() < 1) {
		errorformat = true;
	} else {
		unsigned int i, j;
		SetGameType(dataset[0].gametype);
		if (args[0].IsSameAs(L"binary")) {
			if (dataset[0].gametype != GAME_TYPE_PSX) {
				if (!RaiseError(_("cannot overwrite the binary format of Steam games\n")))
					return false;
				return true;
			}
			bool section[DATA_SECTION_AMOUNT];
			if (args.GetCount() > 1) {
				for (j = 0; j < DATA_SECTION_AMOUNT; j++)
					section[j] = false;
				bool foundsection;
				for (i = 1; i < args.GetCount(); i++) {
					foundsection = false;
					for (j = 0; j < DATA_SECTION_AMOUNT; j++)
						if (args[i].IsSameAs(const_section_name[j])) {
							section[j] = true;
							foundsection = true;
							break;
						}
					if (!foundsection) {
						if (!RaiseWarning(wxString::Format(wxT("unrecognized HWS section '%s'\n"), args[i])))
							return false;
					}
				}
			}
			for (i = 0; i < dataset.size(); i++) {
				WriteToOutput(wxString::Format(wxT("overwriting the game file %s..."), dataset[i].displayname));
				int res = OverwritePSXBinary(dataset[i].filename, args.GetCount() == 1 ? dataset[i].saveset.sectionmodified : section, dataset[i].config, dataset[i].saveset, dataset[i].cluster);
				if (res != 0)
					WriteToOutput(_(L" fail\n"));
				else
					WriteToOutput(_(L" done\n"));
				if (res == 1) {
					if (!RaiseError(_("unable to overwrite the file\n")))
						return false;
				}
			}
		} else if (args.GetCount() < 2) {
			errorformat = true;
		} else if (args[0].IsSameAs(L"steam")) {
			if (dataset[0].gametype != GAME_TYPE_STEAM) {
				if (!RaiseError(_("cannot generate a Steam mod folder using PSX files as a basis\n")))
					return false;
				return true;
			}
			int dllformat = dataset[0].config.dll_usage != 0 ? 1 : 0;
			int assetformat = dataset[0].config.dll_usage != 0 ? 1 : 0;
			unsigned int sectionindex = 2;
			while (sectionindex < 4) {
				if (args.GetCount() >= sectionindex) {
					if (args[sectionindex].IsSameAs(L"dll")) {
						dllformat = 0;
						sectionindex++;
					} else if (args[sectionindex].IsSameAs(L"csv")) {
						dllformat = 1;
						sectionindex++;
					} else if (args[sectionindex].IsSameAs(L"c#")) {
						dllformat = 2;
						sectionindex++;
					} else if (args[sectionindex].IsSameAs(L"archive")) {
						assetformat = 0;
						sectionindex++;
					} else if (args[sectionindex].IsSameAs(L"asset")) {
						assetformat = 1;
						sectionindex++;
					} else {
						break;
					}
				} else {
					break;
				}
			}
			bool section[DATA_SECTION_AMOUNT];
			for (j = 0; j < DATA_SECTION_AMOUNT; j++)
				section[j] = false;
			if (args.GetCount() == sectionindex) {
				for (i = 0; i < dataset.size(); i++)
					for (j = 0; j < DATA_SECTION_AMOUNT; j++)
						if (dataset[i].saveset.sectionmodified[j])
							section[j] = true;
			} else {
				bool foundsection;
				for (i = sectionindex; i < args.GetCount(); i++) {
					foundsection = false;
					for (j = 0;j<DATA_SECTION_AMOUNT;j++)
						if (args[i].IsSameAs(const_section_name[j])) {
							section[j] = true;
							foundsection = true;
							break;
						}
					if (!foundsection) {
						if (!RaiseWarning(wxString::Format(wxT("unrecognized HWS section '%s'\n"), args[i])))
							return false;
					}
				}
			}
			if (dllformat == 0)
				wxFileName::Mkdir(args[1] + _(L"\\x64\\FF9_Data\\Managed\\"), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
			if (assetformat == 0) {
				wxFileName::Mkdir(args[1] + _(L"\\StreamingAssets\\"), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
				wxFileName::Mkdir(args[1] + _(L"\\x64\\FF9_Data\\"), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
			}
			WriteToOutput(_(L"generating a Steam mod folder..."));
			SaveSet mergedset = MergeDataSets();
			int res = CreateSteamMod(args[1].ToStdString(), section, dataset[0].config, mergedset, dllformat, assetformat, false);
			if (res > 0)
				WriteToOutput(_(L" fail\n"));
			else
				WriteToOutput(_(L" done\n"));
			DisposeOfMergedDataSet(mergedset);
			if (res == 1) {
				if (!RaiseError(_("trying to override the Steam game currently opened\n")))
					return false;
			} else if (res == 2) {
				if (!RaiseError(_("unable to open a file for reading\n")))
					return false;
			} else if (res == 3) {
				if (!RaiseError(_("unable to create or overwrite a file\n")))
					return false;
			} else if (res == -1) {
				if (!RaiseWarning(_("CIL code modifications cannot be saved as CSV spreadsheets\n")))
					return false;
			}
		} else if (args.GetCount() < 3) {
			errorformat = true;
		} else if (args[0].IsSameAs(L"ppf")) {
			if (dataset[0].gametype != GAME_TYPE_PSX) {
				if (!RaiseError(_("cannot generate a .ppf from Steam game data\n")))
					return false;
				return true;
			}
			unsigned int sectionindex = 3;
			bool undo = false;
			bool blockcheck = false;
			while (sectionindex < 4) {
				if (args.GetCount() >= sectionindex) {
					if (args[sectionindex].IsSameAs(L"undo")) {
						undo = true;
						sectionindex++;
					} else if (args[sectionindex].IsSameAs(L"blockcheck")) {
						blockcheck = true;
						sectionindex++;
					} else {
						break;
					}
				} else {
					break;
				}
			}
			bool section[DATA_SECTION_AMOUNT];
			if (args.GetCount() != sectionindex) {
				for (j = 0; j < DATA_SECTION_AMOUNT; j++)
					section[j] = false;
				bool foundsection;
				for (i = sectionindex; i < args.GetCount(); i++) {
					foundsection = false;
					for (j = 0; j < DATA_SECTION_AMOUNT; j++)
						if (args[i].IsSameAs(const_section_name[j])) {
							section[j] = true;
							foundsection = true;
							break;
						}
					if (!foundsection) {
						if (!RaiseWarning(wxString::Format(wxT("unrecognized HWS section '%s'\n"), args[i])))
							return false;
					}
				}
			}
			for (i = 0; i < dataset.size(); i++) {
				WriteToOutput(wxString::Format(wxT("generating a PPF patch for %s..."), dataset[i].displayname));
				int res = CreatePPF(dataset[i].filename, args[1].ToStdString(), args.GetCount() == sectionindex ? dataset[i].saveset.sectionmodified : section, dataset[i].config, dataset[i].saveset, dataset[i].cluster, undo, blockcheck, args[2].ToStdString());
				if (res != 0)
					WriteToOutput(_(L" fail\n"));
				else
					WriteToOutput(_(L" done\n"));
				if (res == 1) {
					if (!RaiseError(_("unable to create or overwrite a file\n")))
						return false;
				} else if (res == 2 || res == 3) {
					if (!RaiseError(_("unable to open a file for reading\n")))
						return false;
				}
			}
		} else {
			errorformat = true;
		}
	}
	if (errorformat)
		return RaiseError(_("usage is one of the followings:\n 'save binary'\n 'save ppf FILENAME DESCRIPTION [undo/blockcheck] [SECTIONS]'\n 'save steam FOLDERNAME [dll/csv/c#] [archive/asset] [SECTIONS]'\n"));
	return true;
}

bool CommandFrame::ProcessCommandSelect(wxArrayString args) {
	if (args.GetCount() < 2)
		return RaiseError(_("usage is 'select INDEXNAME VALUE' (use 'help index_name' for displaying a list of possible names)\n"));
	unsigned int i, j;
	long long val;
	if (!args[1].ToLongLong(&val))
		return RaiseError(wxString::Format(wxT("'%s' is not a valid number\n"), args[1]));
	bool dataloaded[DATA_SECTION_AMOUNT];
	for (i = 0; i < DATA_SECTION_AMOUNT; i++)
		dataloaded[i] = false;
	for (j = 0; j < dataset.size(); j++)
		for (i = 0; i < DATA_SECTION_AMOUNT; i++)
			if (dataset[j].saveset.sectionloaded[i])
				dataloaded[i] = true;
	for (i = 0; i < INDEX_PARAMETER_COUNT; i++)
		if (args[0].IsSameAs(const_index_parameter[i].name)) {
			if (!dataloaded[const_index_parameter[i].section]) {
				if (auto_load) {
					if (!ProcessCommandLoad(wxArrayString(1, { const_section_name[const_index_parameter[i].section] })))
						return false;
				} else {
					return RaiseWarning(wxString::Format(wxT("cannot select %s-related data before the section is loaded\n"), const_section_name[const_index_parameter[i].section]));
				}
			}
			if (val >= 0) {
				const_index_parameter[i].setter(selector, val);
				WriteToOutput(wxString::Format(wxT("selecting %s[%d]\n"), args[0], val));
			} else {
				const_index_parameter[i].setter(selector, UINT32_MAX);
				WriteToOutput(wxString::Format(wxT("unselecting %s\n"), args[0]));
			}
			return true;
		}
	return RaiseError(wxString::Format(wxT("'%s' is not a valid index name\n"), args[0]));
}

bool CommandFrame::ProcessCommandSet(wxArrayString args) {
	if (args.GetCount() < 2)
		return RaiseError(_("usage is 'set FIELDNAME VALUE' (use 'help field_name' for displaying a list of possible names)\n"));
	bool modifiedone = false;
	bool foundfield = false;
	long long val = 0;
	unsigned int i, j;
	for (i = 0; i < EDITABLE_FIELD_COUNT; i++) {
		if (args[0].IsSameAs(const_editable_field[i]->name)) {
			foundfield = true;
			if (const_editable_field[i]->get_type() == 0 && !args[1].ToLongLong(&val))
				return RaiseError(wxString::Format(wxT("'%s' is not a valid number\n"), args[1]));
			for (j = 0; j < dataset.size(); j++) {
				if (!const_editable_field[i]->checker(dataset[j].saveset, selector))
					continue;
				modifiedone = true;
				if (const_editable_field[i]->get_type() == 0) {
					editable_field_numerical* edit = dynamic_cast<editable_field_numerical*>(const_editable_field[i]);
					edit->setter(dataset[j].saveset, selector, val);
					WriteToOutput(wxString::Format(wxT("set selected %s to %d\n"), args[0], edit->getter(dataset[j].saveset, selector)));
				} else if (const_editable_field[i]->get_type() == 1) {
					editable_field_string* edit = dynamic_cast<editable_field_string*>(const_editable_field[i]);
					edit->setter(dataset[j].saveset, selector, args[1]);
					WriteToOutput(wxString::Format(wxT("set selected %s to %s\n"), args[0], edit->getter(dataset[j].saveset, selector)));
				}
			}
		}
	}
	if (!foundfield)
		return RaiseError(wxString::Format(wxT("'%s' doesn't correspond to a data field\n"), args[0]));
	if (!modifiedone)
		return RaiseWarning(wxString::Format(wxT("the current selection parameters are invalid for modifying %s\n"), args[0]));
	return true;
}

bool CommandFrame::ProcessCommandHelp(wxArrayString args) {
	if (args.GetCount() == 0) {
		WriteToOutput(_(L"Possible commands are:\n"\
			L"- 'help'\n"\
			L"- 'help section_name'\n"\
			L"- 'help index_name'\n"\
			L"- 'help field_name'\n"\
			L"- 'config NAME VALUE'\n"\
			L"- 'open FILENAME'\n"\
			L"- 'load SECTION'\n"\
			L"- 'import hws FILENAME [SECTIONS]'\n"\
			L"- 'import text FILENAME'\n"\
			L"- 'import uitext FILENAME'\n"\
			L"- 'import script FILENAME'\n"\
			L"- 'save binary'\n"\
			L"- 'save ppf FILENAME DESCRIPTION [undo/blockcheck] [SECTIONS]'\n"\
			L"- 'save steam FOLDERNAME [dll/csv/c#] [archive/asset] [SECTIONS]'\n"\
			L"- 'export hws FILENAME [SECTIONS]'\n"\
			L"- 'export text FILENAME [TEXTID]'\n"\
			L"- 'export uitext FILENAME [UITEXTID]'\n"\
			L"- 'export script field/enemy/world FILENAME [FIELDID/BATTLEID/WORLDID]'\n"\
			L"- 'export data FORMAT FILENAME DATASTRUCTURE'\n"\
			L"- 'select INDEXNAME VALUE'\n"\
			L"- 'set FIELDNAME VALUE'\n"\
			L"- 'exit'\n"));
	} else {
		if (args[0].IsSameAs(L"section_name")) {
			WriteToOutput(_(L"Possible sections are: spell, support, command, stat, party_special, enemy, item, shop, card, text, world, field, scene, spell_animation, interface, assembly\n"));
		} else if (args[0].IsSameAs(L"index_name")) {
			wxString helpstr = _(L"Possible index names are:");
			unsigned int i;
			for (i = 0; i < INDEX_PARAMETER_COUNT; i++)
				helpstr += (i == 0 ? " " : ", ") + const_index_parameter[i].name;
			helpstr += "\n";
			WriteToOutput(helpstr);
		} else if (args[0].IsSameAs(L"field_name")) {
			wxString helpstr = _(L"Possible field names are:");
			unsigned int i;
			for (i = 0; i < EDITABLE_FIELD_COUNT; i++)
				helpstr += (i == 0 ? " " : ", ") + const_editable_field[i]->name;
			helpstr += "\n";
			WriteToOutput(helpstr);
		} else {
			return RaiseWarning(wxString::Format(wxT("'%s' is not recognized as a help command\n"), args[0]));
		}
	}
	return true;
}

void CommandFrame::OnAddCommand(wxCommandEvent& event) {
	completefile = m_morecommand->GetValue();
	m_morecommand->SetValue(_(L""));
	m_morecommand->Enable(false);
	executing_commands = true;
	executing_extra_commands = true;
	m_commands->AppendText(completefile + _(L"\n"));
	CommandThread* cmdthread = new CommandThread(this);
	if (cmdthread->Run() != wxTHREAD_NO_ERROR) {
		delete cmdthread;
		executing_commands = false;
		m_morecommand->Enable(true);
		WriteToOutput(_(L"the program has no resource to run"));
	}
}

void CommandFrame::OnClosing(wxCloseEvent& event) {
	if (executing_commands) {
		event.Veto();
		return;
	}
	if (output.IsOpened())
		output.Close();
	event.Skip();
}

FF9DataSet::FF9DataSet(ConfigurationSet& cfg, string fname, GameType gt) :
	filename(fname),
	gametype(gt),
	config(cfg),
	saveset(&spellset, &cmdset, &enemyset, &shopset, &textset, &worldset, &sceneset, &itemset, &supportset, &statset, &cardset, &fieldset, &spellanimset, &ffuiset, &partyspecialset, &mipsset, &cilset) {
	cluster.config = &config;
	if (gt == GAME_TYPE_PSX) {
		fstream f(fname.c_str(), ios::in | ios::binary);
		cluster.Read(f, config);
		cluster.CreateImageMaps(f);
		f.close();
		size_t dirpos = fname.find_last_of("/\\");
		if (dirpos == string::npos)
			displayname = fname;
		else
			displayname = fname.substr(dirpos + 1);
	} else {
		cluster.enemy_amount = config.enmy_amount;
		cluster.text_amount = config.text_amount;
		cluster.world_amount = config.world_amount;
		cluster.field_amount = config.field_amount;
		cluster.image_map_amount = 0;
		if (config.atlas_icontexture_file >= 0 && config.atlas_iconsprite_file >= 0) {
			fstream atlasfile(config.steam_dir_data + "sharedassets2.assets", ios::in | ios::binary);
			ffuiset.steam_atlas[0].LoadAtlas(atlasfile, config.meta_atlas, config.atlas_icontexture_file, config.atlas_iconsprite_file);
			atlasfile.close();
		}
		displayname = "the Steam game";
	}
	for (int i = 0; i < DATA_SECTION_AMOUNT; i++)
		saveset.sectionloaded[i] = false;
}
