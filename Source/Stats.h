#ifndef _STATS_H
#define _STATS_H

#define CHAR_BATTLE_PARAMETER_AMOUNT 19
#define PLAYABLE_CHAR_AMOUNT 12
#define EQUIP_SET_AMOUNT 16
#define ABILITY_SET_AMOUNT 16
#define COMMAND_SET_AMOUNT 20
#define ABILITY_SET_CAPACITY 48
#define MAX_LEVEL 99
#define BATTLE_MOTION_AMOUNT 34

struct CharBattleParameterStruct;
struct InitialStatDataStruct;
struct InitialEquipDataStruct;
struct AbilitySetDataStruct;
struct CommandSetDataStruct;
struct LevelDataStruct;
struct StatDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"
using namespace std;

struct CharBattleParameterStruct {
public:
	int id;
	wstring avatar_sprite;
	int attack_sfx;
	int attack_sound[2];
	uint8_t weapon_bone;
	uint8_t status_bone[6];
	int8_t status_offy[6];
	int8_t status_offz[6];
	uint8_t shadow_bone[2];
	uint8_t shadow_size[3];
	wstring model;
	wstring model_trance;
	wstring anim[BATTLE_MOTION_AMOUNT];
	uint8_t trance_color[3];

	map<wxString, wxString> custom_field;

	int GetCharacterIndex();

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	StatDataSet* parent;
	friend StatDataSet;
};

struct InitialStatDataStruct {
public:
	int id;
	FF9String default_name; // readonly
	uint16_t default_name_size_x;
	int speed;
	int strength;
	int magic;
	int spirit;
	int magic_stone;

	// Parameters for Memoria mod
	wstring name_keyword;
	uint8_t default_row;
	uint8_t default_winpose;
	uint8_t default_category;
	wstring battle_param;
	vector<int> equip_patch;

	map<wxString, wxString> custom_field;
	
	// Return 0 if success ; 1 if the value is too long
	void GenerateDefaultName();
	int SetDefaultName(wstring newvalue);
	int SetDefaultName(FF9String& newvalue);

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	uint16_t default_name_offset;
	StatDataSet* parent;
	friend StatDataSet;
};

struct InitialEquipDataStruct {
public:
	int id;
	int weapon;
	int head;
	int wrist;
	int armor;
	int accessory;

	map<wxString, wxString> custom_field;

	int GetCharacterIndex();

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	StatDataSet* parent;
	friend StatDataSet;
};

struct AbilityEntryDataStruct {
public:
	int id;
	AnyAbilityStruct ability;
	int ap_cost;

	map<wxString, wxString> custom_field;

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	AbilitySetDataStruct* parent;
	friend AbilitySetDataStruct;
	friend StatDataSet;
};

struct AbilitySetDataStruct {
public:
	int id;
	vector<AbilityEntryDataStruct> entry;

	int GetCharacterIndex();

private:
	StatDataSet* parent;
	friend AbilityEntryDataStruct;
	friend StatDataSet;
};

struct CommandSetDataStruct {
public:
	int id;
	int first_command;
	int second_command;
	int first_command_trance;
	int second_command_trance;
	int attack_command;
	int defend_command;
	int item_command;
	int change_command;
	int attack_command_trance;
	int defend_command_trance;
	int item_command_trance;
	int change_command_trance;
	uint8_t trance_attack;

	map<wxString, wxString> custom_field;

	void InitializeDefaultCommands();
	int GetCharacterIndex();

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	StatDataSet* parent;
	friend StatDataSet;
};

struct LevelDataStruct {
public:
	int id;
	uint16_t hp_table;
	uint16_t mp_table;
	uint32_t exp_table;

	map<wxString, wxString> custom_field;

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	StatDataSet* parent;
	friend StatDataSet;
};

struct StatDataSet {
public:
	vector<CharBattleParameterStruct> battle_param; // Memoria mod only
	vector<InitialStatDataStruct> initial_stat;
	vector<InitialEquipDataStruct> initial_equip;
	vector<AbilitySetDataStruct> ability_list;
	vector<CommandSetDataStruct> command_list;
	vector<LevelDataStruct> level;
	uint16_t amount; // == PLAYABLE_CHAR_AMOUNT
	uint8_t shared_command_attack;
	uint8_t shared_command_2;
	uint8_t shared_command_3;
	uint8_t shared_command_item;
	uint16_t default_name_space_total;
	uint16_t default_name_space_used;
	
	uint32_t steam_method_position_abilset[ABILITY_SET_AMOUNT];
	uint32_t steam_method_base_length_abilset[ABILITY_SET_AMOUNT];
	uint32_t steam_method_position_initstat;
	uint32_t steam_method_base_length_initstat;
	uint32_t steam_method_position_hpmp;
	uint32_t steam_method_base_length_hpmp;
	uint32_t steam_method_position_cmdset;
	uint32_t steam_method_base_length_cmdset;

	wxString csv_header_battle_param;
	wxString csv_header_character_param;
	wxString csv_header_initial_stat;
	wxString csv_header_initial_equip;
	wxString csv_header_ability_list;
	wxString csv_header_command_list;
	wxString csv_header_level;
	wxString csv_format_battle_param;
	wxString csv_format_character_param;
	wxString csv_format_initial_stat;
	wxString csv_format_initial_equip;
	wxString csv_format_ability_list;
	wxString csv_format_command_list;
	wxString csv_format_level;
	map<wxString, wxString> custom_field_battle_param;
	map<wxString, wxString> custom_field_initial_stat;
	map<wxString, wxString> custom_field_initial_equip;
	map<wxString, wxString> custom_field_ability_list;
	map<wxString, wxString> custom_field_command_list;
	map<wxString, wxString> custom_field_level;
	
	// Return a temporary array, not to be deleted
	static int* GetCharacterEquipmentsIndices(int charindex, unsigned int* amount = NULL);
	static int* GetCharacterCommandsIndices(int charindex, unsigned int* amount = NULL);
	static int* GetCharacterBattleParametersIndices(int charindex, unsigned int* amount = NULL);

	int GetCharacterIndexById(int charid);
	InitialStatDataStruct& GetCharacterById(int charid);
	void InitializeNewCharacter(int charindex);
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return 0 on success and 1 if names are too long
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	void GenerateCSharp(vector<string>& buffer);
	bool GenerateCSV(string basefolder);
	void UpdateOffset();
};

#endif
