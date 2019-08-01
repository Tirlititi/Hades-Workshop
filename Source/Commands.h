#ifndef _COMMANDS_H
#define _COMMANDS_H

#define COMMAND_AMOUNT 48
#define COMMAND_SPELL_AMOUNT 192
struct CommandDataStruct;
struct CommandDataSet;

#include <inttypes.h>
#include <fstream>
#include "DllEditor.h"
using namespace std;

typedef uint8_t Command_Panel;
#define COMMAND_PANEL_NONE		0
#define COMMAND_PANEL_SPELL		1
#define COMMAND_PANEL_ITEM		2
#define COMMAND_PANEL_WEAPON	3

#include "File_Manipulation.h"
#include "Configuration.h"

struct CommandDataStruct {
public:
	FF9String name; // readonly
	FF9String help; // readonly
	uint8_t panel; // readonly
	uint8_t spell_amount; // readonly
	uint8_t spell_list[COMMAND_SPELL_AMOUNT]; // readonly
	uint16_t help_size_x;
	
	uint8_t link; // id of linked cmd : they share the same spell list. 0xFF if not linked.
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);
	void SetPanel(uint8_t newvalue);
	bool AddSpell(uint8_t spellid, uint8_t spellpos); // Assume spellid < SPELL_AMOUNT and spellpos < spell_amount
	void RemoveSpell(uint8_t spellpos); // Assume spellpos < spell_amount
	void MoveSpell(uint8_t spellpos, uint8_t newpos); // Assume spellpos < spell_amount and newpos < spell_amount
	void AddLink(uint8_t cmdid); // Assume there is no link yet and both use COMMAND_PANEL_SPELL
	void ChangeLink(uint8_t newlink); // Assume there is a link and the new cmd is valid
	void BreakLink(); // Assume there a link

private:
	uint8_t id;
	uint16_t name_offset;
	uint16_t help_offset;
	uint8_t spell_index;
	CommandDataSet* parent;
	friend CommandDataSet;
};

struct CommandDataSet {
public:
	CommandDataStruct cmd[COMMAND_AMOUNT];
	uint16_t name_space_total;
	uint16_t help_space_total;
	uint16_t name_space_used;
	uint16_t help_space_used;
	uint8_t spell_space_remaining;
	
	uint32_t steam_method_position;
	uint32_t steam_method_base_length;
	
	void Load(fstream &ffbin, ConfigurationSet& config);
	void Write(fstream &ffbin, ConfigurationSet& config);
	void WritePPF(fstream &ffbin, ConfigurationSet& config);
	// Return 0, 1, 2 or 3 for success, names too long, helps too long and both
	int LoadHWS(fstream &ffbin, bool usetext);
	void WriteHWS(fstream &ffbin);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	void GenerateCSharp(vector<string>& buffer);
	// texttype: 0 for name, 1 for help
	void WriteSteamText(fstream& ffbin, unsigned int texttype, SteamLanguage lang = GetSteamLanguage());
	int GetSteamTextSize(unsigned int texttype, SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();

private:
	uint8_t spell_full_list[COMMAND_SPELL_AMOUNT];
	
	void SetupLinks();
	void UpdateSpellsDatas();
};

#endif
