#ifndef _SPELLS_H
#define _SPELLS_H

#define SPELL_AMOUNT		192
#define SPELL_EFFECT_AMOUNT	110
#define STATUS_SET_AMOUNT	64
struct SpellDataStruct;
struct SpellDataSet;

#include <inttypes.h>
#include <fstream>
#include "DllEditor.h"
using namespace std;

typedef uint8_t Spell_Panel;
#define SPELL_PANEL_NONE		0
#define SPELL_PANEL_HP			1
#define SPELL_PANEL_MP			2
#define SPELL_PANEL_BAD_STATUS	3
#define SPELL_PANEL_GOOD_STATUS	4
#define SPELL_PANEL_EMPTY		5
#define SPELL_PANEL_CARDS		6
#define SPELL_PANEL_BUG			7

typedef uint8_t Spell_Target_Priority;
#define SPELL_TARGET_PRIORITY_ENEMIES	0
#define SPELL_TARGET_PRIORITY_ALLIES	1

typedef uint8_t Spell_Target_Type;
#define SPELL_TARGET_TYPE_ANY		0
#define SPELL_TARGET_TYPE_ALLY		1
#define SPELL_TARGET_TYPE_ENEMY		2
#define SPELL_TARGET_TYPE_EVERYONE	3
#define SPELL_TARGET_TYPE_SELF		4

typedef uint8_t Spell_Target_Amount;
#define SPELL_TARGET_AMOUNT_ONE			0
#define SPELL_TARGET_AMOUNT_GROUP		1
#define SPELL_TARGET_AMOUNT_VARIABLE	2
#define SPELL_TARGET_AMOUNT_ZERO		3

#define MENU_FLAG_CAN_USE_IN_MENU		0x01
#define MENU_FLAG_HIDE_AP_PROGRESSION	0x02
#define MENU_FLAG_MP_COST_BOOST			0x04
#define MENU_FLAG_ENEMY_ATTACK			0x80

#define TARGET_FLAG_CAN_TARGET_DEAD		0x20
#define TARGET_FLAG_CAMERA				0x40
#define TARGET_FLAG_TARGET_DEAD_FIRST	0x80
/*
#define SPELL_FLAG_CAN_REFLECT	0x01
#define SPELL_FLAG_MAGIC		0x02
#define SPELL_FLAG_COUNTERABLE	0x08
#define SPELL_FLAG_RETURNABLE	0x80
*/
#define ELEMENT_FIRE	0x01
#define ELEMENT_ICE		0x02
#define ELEMENT_THUNDER	0x04
#define ELEMENT_EARTH	0x08
#define ELEMENT_WATER	0x10
#define ELEMENT_WIND	0x20
#define ELEMENT_HOLY	0x40
#define ELEMENT_SHADOW	0x80

#define STATUS_PETRIFY			0x00000001
#define STATUS_VENOM			0x00000002
#define STATUS_VIRUS			0x00000004
#define STATUS_SILENCE			0x00000008
#define STATUS_BLIND			0x00000010
#define STATUS_TROUBLE			0x00000020
#define STATUS_ZOMBIE			0x00000040
#define STATUS_EASY_KILL		0x00000080
#define STATUS_DEATH			0x00000100
#define STATUS_TARGETING		0x00000200
#define STATUS_CONFUSE			0x00000400
#define STATUS_BERSERK			0x00000800
#define STATUS_STOP				0x00001000
#define STATUS_AUTOLIFE			0x00002000
#define STATUS_TRANCE			0x00004000
#define STATUS_DEFEND			0x00008000
#define STATUS_POISON			0x00010000
#define STATUS_SLEEP			0x00020000
#define STATUS_REGEN			0x00040000
#define STATUS_HASTE			0x00080000
#define STATUS_SLOW				0x00100000
#define STATUS_FLOAT			0x00200000
#define STATUS_SHELL			0x00400000
#define STATUS_PROTECT			0x00800000
#define STATUS_HEAT				0x01000000
#define STATUS_FREEZE			0x02000000
#define STATUS_VANISH			0x04000000
#define STATUS_DOOM				0x08000000
#define STATUS_MINI				0x10000000
#define STATUS_REFLECT			0x20000000
#define STATUS_UNKNOWN			0x40000000
#define STATUS_GRADUAL_PETRIFY	0x80000000

#include "File_Manipulation.h"
#include "Configuration.h"

struct SpellDataStruct {
public:
	FF9String name; // readonly
	FF9String help; // readonly
	uint16_t model;
	uint8_t target_flag;
	uint8_t effect;
	uint8_t power;
	uint8_t element;
	uint8_t accuracy;
	uint8_t flag;
	uint8_t status;
	uint8_t mp;
	uint8_t menu_flag;
	uint16_t model_alt;
	uint16_t help_size_x;
	
	uint8_t perform_name;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);
	
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

private:
	uint16_t name_offset;
	uint16_t help_offset;
	uint8_t target_type;
	SpellDataSet* parent;
	friend SpellDataSet;
};

struct SpellDataSet {
public:
	SpellDataStruct spell[SPELL_AMOUNT];
	uint32_t status_set[STATUS_SET_AMOUNT];
	wstring status_set_name[STATUS_SET_AMOUNT];
	uint16_t name_space_total;
	uint16_t help_space_total;
	uint16_t name_space_used;
	uint16_t help_space_used;
	
	uint32_t steam_method_position;
	uint32_t steam_method_base_length;
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return 0, 1, 2 or 3 for success, names too long, helps too long and both
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(fstream& ffbinbase, ConfigurationSet& config, unsigned int* modifamount);
	// texttype: 0 for name, 1 for help
	void WriteSteamText(fstream& ffbin, unsigned int texttype);
	void UpdateOffset();
};

#endif
