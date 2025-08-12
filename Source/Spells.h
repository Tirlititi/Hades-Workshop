#ifndef _SPELLS_H
#define _SPELLS_H

#define SPELL_AMOUNT		192
#define SPELL_EFFECT_AMOUNT	110
#define STATUS_AMOUNT		32
#define STATUS_SET_AMOUNT	64
struct SpellDataStruct;
struct StatusSetStruct;
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
#define SPELL_TARGET_TYPE_ANY			0
#define SPELL_TARGET_TYPE_ALLY			1
#define SPELL_TARGET_TYPE_ENEMY			2
#define SPELL_TARGET_TYPE_EVERYONE		3
#define SPELL_TARGET_TYPE_SELF			4
#define SPELL_TARGET_TYPE_IRRELEVANT	5

typedef uint8_t Spell_Target_Amount;
#define SPELL_TARGET_AMOUNT_ONE			0
#define SPELL_TARGET_AMOUNT_GROUP		1
#define SPELL_TARGET_AMOUNT_VARIABLE	2
#define SPELL_TARGET_AMOUNT_RANDOM		3
#define SPELL_TARGET_AMOUNT_ZERO		4

#define MENU_FLAG_CAN_USE_IN_MENU		0x01
#define MENU_FLAG_HIDE_AP_PROGRESSION	0x02
#define MENU_FLAG_MP_COST_BOOST			0x04
#define MENU_FLAG_ENEMY_ATTACK			0x80

#define TARGET_FLAG_CAN_TARGET_DEAD		0x20
#define TARGET_FLAG_CAMERA				0x40
#define TARGET_FLAG_TARGET_DEAD_FIRST	0x80

#define SPELL_FLAG_REFLECTABLE	0x01
#define SPELL_FLAG_SILENCE		0x02
#define SPELL_FLAG_NO_MSWORD	0x04
#define SPELL_FLAG_PHYSICAL		0x08
#define SPELL_FLAG_MAGICAL		0x10
#define SPELL_FLAG_SHORT		0x20
#define SPELL_FLAG_HIT_ANIM_OFF	0x40
#define SPELL_FLAG_RETURNABLE	0x80

#define ELEMENT_FIRE	0x01
#define ELEMENT_ICE		0x02
#define ELEMENT_THUNDER	0x04
#define ELEMENT_EARTH	0x08
#define ELEMENT_WATER	0x10
#define ELEMENT_WIND	0x20
#define ELEMENT_HOLY	0x40
#define ELEMENT_SHADOW	0x80

#define SPELL_ATTACK_ID	176

#include "File_Manipulation.h"
#include "Configuration.h"

struct SpellDataStruct {
public:
	int id;
	FF9String name; // readonly
	FF9String help; // readonly
	uint8_t target_type;
	int model;
	uint8_t target_flag;
	int effect;
	int power;
	uint8_t element;
	int accuracy;
	uint8_t flag;
	int status;
	int mp;
	uint8_t menu_flag;
	int model_alt;
	uint16_t help_size_x;
	
	uint8_t perform_name;

	map<wxString, wxString> custom_field;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);
	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);
	
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
	SpellDataSet* parent;
	friend SpellDataSet;
};

struct StatusSetStruct {
public:
	int id;
	wstring name;
	set<int> status_list;

	map<wxString, wxString> custom_field;

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	SpellDataSet* parent;
	friend SpellDataSet;
};

struct SpellDataSet {
public:
	vector<SpellDataStruct> spell;
	vector<StatusSetStruct> status_set;
	uint16_t name_space_total;
	uint16_t help_space_total;
	uint16_t name_space_used;
	uint16_t help_space_used;
	
	uint32_t steam_method_position;
	uint32_t steam_method_base_length;

	wxString csv_header;
	wxString csv_header_status;
	wxString csv_format;
	wxString csv_format_status;
	map<wxString, wxString> custom_field;
	map<wxString, wxString> custom_field_status;

	wxArrayString GetModifiableSpellFields();
	wxArrayString GetModifiableStatusSetFields();

	int GetSpellIndexById(int spellid);
	SpellDataStruct& GetSpellById(int spellid);
	int GetStatusSetIndexById(int statusid);
	StatusSetStruct& GetStatusSetById(int statusid);
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return 0, 1, 2 or 3 for success, names too long, helps too long and both
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	void GenerateCSharp(vector<string>& buffer);
	bool GenerateCSV(string basefolder);
	// texttype: 0 for name, 1 for help
	void WriteSteamText(fstream& ffbin, unsigned int texttype, bool onlymodified, bool asmes, SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();
};

#endif
