#ifndef _ITEMS_H
#define _ITEMS_H

#define ITEM_AMOUNT 256
#define ITEM_WEAPON_AMOUNT 88
#define ITEM_ARMOR_AMOUNT 136
#define ITEM_USABLE_AMOUNT 32
#define ITEM_STAT_AMOUNT 176
#define KEY_ITEM_AMOUNT 256
struct ItemDataStruct;
struct ItemUsableDataStruct;
struct ItemWeaponDataStruct;
struct ItemArmorDataStruct;
struct ItemStatDataStruct;
struct KeyItemDataStruct;
struct ItemDataSet;

#include <inttypes.h>
#include <fstream>
using namespace std;

#define ITEM_TYPE_CONSUMABLE	0x1
#define ITEM_TYPE_SPECIAL		0x2
#define ITEM_TYPE_USABLE		0x4
#define ITEM_TYPE_ACCESSORY		0x8
#define ITEM_TYPE_ARMOR			0x10
#define ITEM_TYPE_HEAD			0x20
#define ITEM_TYPE_WRIST			0x40
#define ITEM_TYPE_WEAPON		0x80

#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"

struct ItemDataStruct {
public:
	FF9String name; // readonly
	FF9String help; // readonly
	FF9String battle_help; // readonly
	uint16_t price;
	uint16_t char_availability;
	uint8_t icon;
	uint8_t icon_color;
	uint8_t equip_position;
	uint8_t stat_id;
	uint8_t skill[3];
	uint8_t type;
	uint8_t menu_position;
	uint8_t zero;
	
	int usable_id; // -1 if not an usable item
	int weapon_id; // -1 if not a weapon
	int armor_id; // -1 if not a armor
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);
	int SetBattleHelp(wstring newvalue);
	int SetBattleHelp(FF9String& newvalue);

private:
	uint16_t name_offset;
	uint16_t help_offset;
	uint16_t battle_help_offset;
	ItemDataSet* parent;
	friend ItemDataSet;
};

struct ItemUsableDataStruct {
public:
	uint8_t target_type;
	uint16_t model;
	uint8_t target_flag;
	uint8_t effect;
	uint8_t power;
	uint8_t element;
	uint8_t accuracy;
	uint32_t status;
	
	Spell_Panel GetPanel();
	void SetPanel(Spell_Panel newvalue);
	Spell_Target_Priority GetTargetPriority();
	void SetTargetPriority(Spell_Target_Priority newvalue);
	Spell_Target_Amount GetTargetAmount();
	void SetTargetAmount(Spell_Target_Amount newvalue);
	Spell_Target_Type GetTargetType();
	void SetTargetType(Spell_Target_Type newvalue); // Automatically change target_amount and target_priority if needed
	uint16_t GetSound();
	void SetSound(uint16_t newvalue);
};

struct ItemWeaponDataStruct {
public:
	uint8_t flag;
	uint8_t status;
	uint16_t model;
	uint8_t damage_formula;
	uint8_t power;
	uint8_t element;
	uint8_t status_accuracy;
	int16_t offset1;
	int16_t offset2;
};

struct ItemArmorDataStruct {
public:
	uint8_t defence;
	uint8_t evade;
	uint8_t magic_defence;
	uint8_t magic_evade;
};

struct ItemStatDataStruct {
public:
	uint8_t speed;
	uint8_t strength;
	uint8_t magic;
	uint8_t spirit;
	uint8_t element_immune;
	uint8_t element_absorb;
	uint8_t element_half;
	uint8_t element_weak;
	uint8_t element_boost;
};

struct KeyItemDataStruct {
public:
	FF9String name; // readonly
	FF9String help; // readonly
	FF9String description; // readonly
	uint16_t help_size_x;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);
	int SetDescription(wstring newvalue);
	int SetDescription(FF9String& newvalue);

private:
	uint16_t name_offset;
	uint16_t help_offset;
	uint16_t description_offset;
	bool description_out_of_bounds;
	ItemDataSet* parent;
	friend ItemDataSet;
};

struct ItemDataSet {
public:
	ItemDataStruct item[ITEM_AMOUNT];
	KeyItemDataStruct key_item[KEY_ITEM_AMOUNT];
	ItemUsableDataStruct usable[ITEM_USABLE_AMOUNT];
	ItemWeaponDataStruct weapon[ITEM_WEAPON_AMOUNT];
	ItemArmorDataStruct armor[ITEM_ARMOR_AMOUNT];
	ItemStatDataStruct stat[ITEM_STAT_AMOUNT];
	uint16_t name_space_total;
	uint16_t help_space_total;
	uint16_t help2_space_total;
	uint16_t key_name_space_total;
	uint16_t key_help_space_total;
	uint16_t key_desc_space_total;
	uint16_t name_space_used;
	uint16_t help_space_used;
	uint16_t help2_space_used;
	uint16_t key_name_space_used;
	uint16_t key_help_space_used;
	uint16_t key_desc_space_used;
	
	uint32_t steam_method_position[5];
	uint32_t steam_method_base_length[5];
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return something positive if some text is too long
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	void GenerateCSharp(vector<string>& buffer);
	bool GenerateCSV(string basefolder);
	// texttype: 0 for item name, 1 for item help, 2 for item battle help,
	//  3 for key item name, 4 for key item help, 5 for key item description
	void WriteSteamText(fstream& ffbin, unsigned int texttype, SteamLanguage lang = GetSteamLanguage());
	int GetSteamTextSize(unsigned int texttype, SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();
};

#endif
