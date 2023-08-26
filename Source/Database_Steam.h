#ifndef _DATABASE_STEAM_H
#define _DATABASE_STEAM_H

#include "Database_Common.h"

struct SteamDictionary {
	uint16_t id;
	string name;
};

struct SteamWDictionary {
	uint16_t id;
	wstring name;
};

struct SteamBattleDictionary {
	uint16_t battle_id; // ID of the battle
	uint16_t script_id; // ID of the script file
	uint16_t text_id; // ID of the text file
	uint16_t psx_id; // ID of the psx enemy stats file
	string name;
};

struct SteamFieldDictionary {
	uint16_t script_id;
	uint8_t file_id;
	string script_name;
	string background_name;
	string default_name;
	bool has_localization;
	vector<int> sps_list;
};

extern vector<string> HADES_STRING_EQUIPSET_NAME;
extern vector<string> HADES_STRING_ABILITYSET_NAME;
extern vector<string> SteamCommandTagName;
extern vector<SteamWDictionary> SteamWeaponModel;
extern vector<SteamFieldDictionary> SteamFieldScript;
extern vector<SteamBattleDictionary> SteamBattleScript;
extern vector<SteamDictionary> SteamTextFile;
extern vector<SteamDictionary> SteamIconAtlas;

#endif
