#pragma once

#include "Configuration.h"
#include "gui.h"
using namespace std;

struct FF9DataSet {
	ConfigurationSet& config;
	GameType gametype;
	string filename;
	string displayname;
	ClusterSet cluster;
	SaveSet saveset;
	UnusedSaveBackup backupset;
	SpellDataSet spellset;
	SupportDataSet supportset;
	CommandDataSet cmdset;
	StatDataSet statset;
	PartySpecialDataSet partyspecialset;
	EnemyDataSet enemyset;
	ItemDataSet itemset;
	ShopDataSet shopset;
	CardDataSet cardset;
	TextDataSet textset;
	WorldMapDataSet worldset;
	FieldDataSet fieldset;
	BattleSceneDataSet sceneset;
	SpellAnimationDataSet spellanimset;
	MenuUIDataSet ffuiset;
	MipsDataSet mipsset;
	CILDataSet cilset;

	FF9DataSet(ConfigurationSet& cfg, string fname, GameType gt);
};

struct FieldSelector {
	// DataSet indices
	unsigned int spell_index = UINT32_MAX;
	unsigned int status_set_index = UINT32_MAX;
	unsigned int support_index = UINT32_MAX;
	unsigned int command_index = UINT32_MAX;
	unsigned int stat_index = UINT32_MAX;
	unsigned int initial_equip_index = UINT32_MAX;
	unsigned int ability_list_index = UINT32_MAX;
	unsigned int command_list_index = UINT32_MAX;
	unsigned int level_index = UINT32_MAX;
	unsigned int magic_sword_index = UINT32_MAX;
	unsigned int item_index = UINT32_MAX;
	unsigned int item_stat_index = UINT32_MAX;
	unsigned int key_item_index = UINT32_MAX;
	unsigned int shop_index = UINT32_MAX;
	unsigned int synthesis_index = UINT32_MAX;
	unsigned int deck_index = UINT32_MAX;
	unsigned int card_set_index = UINT32_MAX;
	unsigned int card_index = UINT32_MAX;
	unsigned int spell_animation_index = UINT32_MAX;
	// DataSet ids
	unsigned int battle_id = UINT32_MAX;
	unsigned int field_id = UINT32_MAX;
	unsigned int world_id = UINT32_MAX;
	unsigned int text_block_id = UINT32_MAX;
	// DataStruct indices
	unsigned int command__spell_index = UINT32_MAX;
	unsigned int ability_list__slot_index = UINT32_MAX;
	unsigned int item__skillslot_index = UINT32_MAX;
	unsigned int shop__slot_index = UINT32_MAX;
	unsigned int card_set__slot_index = UINT32_MAX;
	unsigned int spell_animation__code_index = UINT32_MAX;
	unsigned int spell_animation__code__arg_index = UINT32_MAX;
	unsigned int battle__animation_index = UINT32_MAX;
	unsigned int battle__camera_raw_index = UINT32_MAX;
	unsigned int battle__group_index = UINT32_MAX;
	unsigned int battle__group__slot_index = UINT32_MAX;
	unsigned int battle__enemy_index = UINT32_MAX;
	unsigned int battle__enemy__item_index = UINT32_MAX;
	unsigned int battle__enemy__selection_index = UINT32_MAX;
	unsigned int battle__attack_index = UINT32_MAX;
	unsigned int field__triangle_index = UINT32_MAX;
	unsigned int field__edge_index = UINT32_MAX;
	unsigned int field__walkpath_index = UINT32_MAX;
	unsigned int field__normal_index = UINT32_MAX;
	unsigned int field__vertex_index = UINT32_MAX;
	unsigned int field__tile_anim_index = UINT32_MAX;
	unsigned int field__tile_anim__tile_index = UINT32_MAX;
	unsigned int field__tile_block_index = UINT32_MAX;
	unsigned int field__tile_block__tile_index = UINT32_MAX;
	unsigned int field__camera_index = UINT32_MAX;
	unsigned int world_place_index = UINT32_MAX;
	unsigned int world_place__raw_index = UINT32_MAX;
	unsigned int world_spot_index = UINT32_MAX;
	unsigned int world_spot__battle_index = UINT32_MAX;
	unsigned int world_friendly_index = UINT32_MAX;
	unsigned int world_friendly__spot_index = UINT32_MAX;
	unsigned int text_block__slot_index = UINT32_MAX;
	unsigned int text_block__charmap_glyph_index = UINT32_MAX;
};

class CommandFrame : public CommandFrameBase {
public:
	vector<FF9DataSet> dataset;
	wxString completefile;
	FieldSelector selector;
	bool executing_commands;
	bool executing_extra_commands;
	bool current_line_executed;
	bool current_line_error;
	bool current_line_warning;
	wxFile output;
	bool exit_on_stop = true;
	bool warning_stop = true;
	bool error_stop = true;
	bool auto_load = false;
	bool exit = false;
	long line_no;

	CommandFrame(wxWindow* parent);
	SaveSet MergeDataSets(); // For reading purposes only
	void DisposeOfMergedDataSet(SaveSet saveset);
	void ExecuteCommands(wxFile& input, wxString outputname);

	void WriteToOutput(wxString str);
	bool RaiseError(wxString errstr);
	bool RaiseWarning(wxString warnstr);
	bool ProcessLine(wxString line);
	bool ProcessCommand(wxString command, wxArrayString args);
	bool ProcessCommandConfig(wxArrayString args);
	bool ProcessCommandOpen(wxArrayString args);
	bool ProcessCommandLoad(wxArrayString args);
	bool ProcessCommandImport(wxArrayString args);
	bool ProcessCommandExport(wxArrayString args);
	bool ProcessCommandSave(wxArrayString args);
	bool ProcessCommandSelect(wxArrayString args);
	bool ProcessCommandSet(wxArrayString args);
	bool ProcessCommandHelp(wxArrayString args);

private:
	void OnAddCommand(wxCommandEvent& event);
	void OnClosing(wxCloseEvent& event);
};
