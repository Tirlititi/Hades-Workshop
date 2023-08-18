#ifndef _SUPPORTS_H
#define _SUPPORTS_H

#define SUPPORT_AMOUNT 64
struct SupportDataStruct;
struct SupportDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"
using namespace std;

#define SUPPORT_ABILITY_HP10 0x20ull
#define SUPPORT_ABILITY_HP20 0x40ull
#define SUPPORT_ABILITY_ACCURACY_PLUS 0x200ull
#define SUPPORT_ABILITY_DISTRACT 0x400ull
#define SUPPORT_ABILITY_LONG_REACH 0x800ull
#define SUPPORT_ABILITY_MP_ATTACK 0x1000ull
#define SUPPORT_ABILITY_BIRD_KILLER 0x2000ull
#define SUPPORT_ABILITY_BUG_KILLER 0x4000ull
#define SUPPORT_ABILITY_STONE_KILLER 0x8000ull
#define SUPPORT_ABILITY_UNDEAD_KILLER 0x10000ull
#define SUPPORT_ABILITY_DRAGON_KILLER 0x20000ull
#define SUPPORT_ABILITY_DEVIL_KILLER 0x40000ull
#define SUPPORT_ABILITY_BEAST_KILLER 0x80000ull
#define SUPPORT_ABILITY_MAN_EATER 0x100000ull
#define SUPPORT_ABILITY_HIGH_JUMP 0x200000ull
#define SUPPORT_ABILITY_MASTER_THIEF 0x400000ull
#define SUPPORT_ABILITY_HEALER 0x1000000ull
#define SUPPORT_ABILITY_ADD_STATUS 0x2000000ull
#define SUPPORT_ABILITY_GAMBLE_DEFENCE 0x4000000ull
#define SUPPORT_ABILITY_POWER_UP 0x20000000ull
#define SUPPORT_ABILITY_REFLECTx2 0x80000000ull
#define SUPPORT_ABILITY_MAG_ELEM_NULL 0x100000000ull
#define SUPPORT_ABILITY_CONCENTRATE 0x200000000ull
#define SUPPORT_ABILITY_BANDIT 0x4000000000000000ull

struct SupportDataStruct {
public:
	int id;
	FF9String name; // readonly
	FF9String help; // readonly
	uint16_t help_size_x;
	uint8_t category;
	uint8_t cost;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);

private:
	uint16_t name_offset;
	uint16_t help_offset;
	SupportDataSet* parent;
	friend SupportDataSet;
};

struct SupportDataSet {
public:
	vector<SupportDataStruct> support;
	uint16_t name_space_total;
	uint16_t help_space_total;
	uint16_t name_space_used;
	uint16_t help_space_used;
	
	uint32_t steam_method_position;
	uint32_t steam_method_base_length;

	int GetSupportIndexById(int supportid);
	SupportDataStruct& GetSupportById(int supportid);
	
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
