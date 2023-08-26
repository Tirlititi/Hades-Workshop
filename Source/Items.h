#ifndef _ITEMS_H
#define _ITEMS_H

#define ITEM_AMOUNT 256
#define ITEM_WEAPON_AMOUNT 88
#define ITEM_ARMOR_AMOUNT 136
#define ITEM_USABLE_AMOUNT 32
#define ITEM_STAT_AMOUNT 176
#define KEY_ITEM_AMOUNT 256
struct AnyAbilityStruct;
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
#define ITEM_TYPE_ANY_EQUIP		0xF8

#define ITEM_POSITION_ABSOLUTE	0
#define ITEM_POSITION_BEFORE	1
#define ITEM_POSITION_SAME_AS	2
#define ITEM_POSITION_AFTER		3

#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"

struct AnyAbilityStruct {
	int id;
	uint8_t is_active;

	void Setup(uint8_t rawid);
	void Setup(int abilid, bool active);
	uint8_t GetRawId();
	bool IsVoid();
	string GetStringId();
};

struct ItemDataStruct {
public:
	int id;
	FF9String name; // readonly
	FF9String help; // readonly
	FF9String battle_help; // readonly
	int price;
	uint16_t char_availability;
	int icon;
	uint8_t icon_color;
	int equip_position;
	int stat_id;
	vector<AnyAbilityStruct> skill;
	uint8_t type;
	int menu_position;
	uint8_t zero;

	uint8_t equip_position_type = ITEM_POSITION_ABSOLUTE;
	uint8_t menu_position_type = ITEM_POSITION_ABSOLUTE;
	
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
	int id;
	uint8_t target_type;
	int model;
	uint8_t target_flag;
	int effect;
	int power;
	uint8_t element;
	int accuracy;
	uint32_t status;
	
	void InitializeDefault(int dataid);

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
	int id;
	uint8_t flag;
	uint8_t status;
	uint16_t model;
	wstring model_name;
	int damage_formula;
	int power;
	uint8_t element;
	int status_accuracy;
	int16_t offset1;
	int16_t offset2;
	int hit_sfx;

	vector<wstring> texture_names;

	void InitializeDefault(int dataid);

	void UpdateModelName();
	void UpdateModelId();
};

struct ItemArmorDataStruct {
public:
	int id;
	uint8_t defence;
	uint8_t evade;
	uint8_t magic_defence;
	uint8_t magic_evade;

	void InitializeDefault(int dataid);
};

struct ItemStatDataStruct {
public:
	int id;
	uint8_t speed;
	uint8_t strength;
	uint8_t magic;
	uint8_t spirit;
	uint8_t element_immune;
	uint8_t element_absorb;
	uint8_t element_half;
	uint8_t element_weak;
	uint8_t element_boost;

	void InitializeDefault(int dataid);
};

struct KeyItemDataStruct {
public:
	int id;
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
	vector<ItemDataStruct> item;
	vector<KeyItemDataStruct> key_item;
	vector<ItemUsableDataStruct> usable;
	vector<ItemWeaponDataStruct> weapon;
	vector<ItemArmorDataStruct> armor;
	vector<ItemStatDataStruct> stat;
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

	int GetItemIndexById(int itemid);
	int GetKeyItemIndexById(int keyitemid);
	int GetUsableIndexById(int usableid);
	int GetWeaponIndexById(int weaponid);
	int GetArmorIndexById(int armorid);
	int GetStatIndexById(int statid);
	ItemDataStruct& GetItemById(int itemid);
	KeyItemDataStruct& GetKeyItemById(int keyitemid);
	ItemUsableDataStruct& GetUsableById(int usableid);
	ItemWeaponDataStruct& GetWeaponById(int weaponid);
	ItemArmorDataStruct& GetArmorById(int armorid);
	ItemStatDataStruct& GetStatById(int statid);
	bool GetEquipPositionFloat(int itemindex, float step, float* position);
	bool GetMenuPositionFloat(int itemindex, float step, float* position);
	
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
	void WriteSteamText(fstream& ffbin, unsigned int texttype, bool onlymodified, bool asmes, SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();
};

#endif
