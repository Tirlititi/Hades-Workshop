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
};

struct InitialStatDataStruct {
public:
	int id;
	FF9String default_name; // readonly
	uint16_t default_name_size_x;
	uint8_t speed;
	uint8_t strength;
	uint8_t magic;
	uint8_t spirit;
	uint8_t magic_stone;

	// Parameters for Memoria mod
	wstring name_keyword;
	uint8_t default_row;
	uint8_t default_winpose;
	uint8_t default_category;
	vector<int> equip_patch;
	
	// Return 0 if success ; 1 if the value is too long
	void GenerateDefaultName();
	int SetDefaultName(wstring newvalue);
	int SetDefaultName(FF9String& newvalue);

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
};

struct AbilitySetDataStruct {
public:
	int id;
	vector<AnyAbilityStruct> ability;
	vector<uint8_t> ap_cost;
};

struct CommandSetDataStruct {
public:
	int id;
	int first_command;
	int second_command;
	int first_command_trance;
	int second_command_trance;
	uint8_t trance_attack;
};

struct LevelDataStruct {
public:
	int id;
	uint16_t hp_table;
	uint16_t mp_table;
	uint32_t exp_table;
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
