#ifndef _STATS_H
#define _STATS_H

#define PLAYABLE_CHAR_AMOUNT 12
#define EQUIP_SET_AMOUNT 16
#define ABILITY_SET_AMOUNT 16
#define COMMAND_SET_AMOUNT 20
#define ABILITY_SET_CAPACITY 48
#define MAX_LEVEL 99

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

struct InitialStatDataStruct {
public:
	FF9String default_name; // readonly
	uint16_t default_name_size_x;
	uint8_t speed;
	uint8_t strength;
	uint8_t magic;
	uint8_t spirit;
	uint8_t magic_stone;
	
	// Return 0 if success ; 1 if the value is too long
	int SetDefaultName(wstring newvalue);
	int SetDefaultName(FF9String& newvalue);

private:
	uint16_t default_name_offset;
	StatDataSet* parent;
	friend StatDataSet;
};

struct InitialEquipDataStruct {
public:
	uint8_t weapon;
	uint8_t head;
	uint8_t wrist;
	uint8_t armor;
	uint8_t accessory;
};

struct AbilitySetDataStruct {
public:
	uint8_t ability[ABILITY_SET_CAPACITY];
	uint8_t ap_cost[ABILITY_SET_CAPACITY];
};

struct CommandSetDataStruct {
public:
	uint8_t first_command;
	uint8_t second_command;
	uint8_t first_command_trance;
	uint8_t second_command_trance;
	uint8_t trance_attack;
};

struct LevelDataStruct {
public:
	uint16_t hp_table;
	uint16_t mp_table;
	uint32_t exp_table;
};

struct StatDataSet {
public:
	InitialStatDataStruct initial_stat[PLAYABLE_CHAR_AMOUNT];
	InitialEquipDataStruct initial_equip[EQUIP_SET_AMOUNT];
	AbilitySetDataStruct ability_list[ABILITY_SET_AMOUNT];
	CommandSetDataStruct command_list[COMMAND_SET_AMOUNT];
	LevelDataStruct level[MAX_LEVEL];
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
	uint8_t* GetCharacterEquipmentsId(int charid, unsigned int* amount);
	uint8_t* GetCharacterCommandsId(int charid, unsigned int* amount);
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return 0 on success and 1 if names are too long
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(fstream& ffbinbase, ConfigurationSet& config, unsigned int* modifamount);
	void UpdateOffset();
};

#endif
