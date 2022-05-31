#ifndef _WORLDMAPS_H
#define _WORLDMAPS_H

#define WORLD_MAP_PLACE_AMOUNT			64
#define WORLD_MAP_BATTLE_SPOT_AMOUNT	127 // Number of Battle Spots
#define WORLD_MAP_BATTLE_GROUND_AMOUNT	354 // 50 Battle Spots with 4 grounds + 77 with 2 grounds
#define WORLD_MAP_BATTLE_SET_AMOUNT		4
#define WORLD_MAP_FRIENDLY_AMOUNT		9
#define WORLD_MAP_FRIENDLY_AREA_AMOUNT	12

struct WorldMapInfoStruct;
struct WorldMapDataStruct;
struct WorldMapDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct WorldMapInfoStruct : public ChunkChild {
};

struct WorldMapDataStruct : public ChunkChild {
	uint32_t section_amount;
	uint32_t* section_offset;
	uint32_t* section_size;
	uint8_t** section_raw_data;
	uint32_t steam_chunk_pos_disc[2]; // Steam version only
	uint32_t steam_chunk_pos_discmr[2]; // Steam version only

	uint16_t place_name_extra_size;
	FF9String place_name[WORLD_MAP_PLACE_AMOUNT];
	uint16_t* place_unknown_amount;
	uint8_t** place_unknown;
	uint32_t place_name_size; // Steam version only
	
	uint16_t battle_id[WORLD_MAP_BATTLE_GROUND_AMOUNT][WORLD_MAP_BATTLE_SET_AMOUNT];
	uint16_t battle_flags[WORLD_MAP_BATTLE_GROUND_AMOUNT]; // 1: hasFog
	
	uint16_t friendly_area[WORLD_MAP_FRIENDLY_AMOUNT][WORLD_MAP_FRIENDLY_AREA_AMOUNT];
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(unsigned int placeid, wstring newvalue);
	int SetName(unsigned int placeid, FF9String& newvalue);

	int ChangeBattle(unsigned int groundid, unsigned int setid, uint16_t newid);
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, bool usetext = true);
	void WriteHWS(fstream& f, bool saveversion = true, int steamdiscordiscmr = -1); // 0-1: disc.img, 2-3: dicmr.img
	void UpdateOffset();

	static const vector<uint16_t> friendly_battle_id[WORLD_MAP_FRIENDLY_AMOUNT];
};

struct WorldMapDataSet {
public:
	uint16_t amount;
	uint16_t* cluster_id;
	wstring* name;
	uint16_t* tim_amount;
	ScriptDataStruct** script;
	TextDataStruct** text_data;
	CharmapDataStruct** charmap; // PSX only
	TIMImageDataStruct** chartim; // PSX only
	ImageMapDataStruct** preload; // PSX only (for now)
	WorldMapDataStruct* world_data;
	
	void Load(fstream& ffbin, ClusterSet& clusset);
	void Write(fstream& ffbin, ClusterSet& clusset);
	void WritePPF(fstream& ffbin, ClusterSet& clusset);
	// return value is int[5] to be deleted
	// {Number of oversized blocks, Number of unknown data, Number of unused blocks, Number of oversized parent block, Number of ignored Preload data}
	// Parent block is a data block containing preloading datas and TIM images in Field & World Map clusters.
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup, bool usetext, unsigned int localflag);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup, unsigned int localflag);
	// texttype: 0 for world texts, 1 for place name
	void WriteSteamText(fstream& ffbin, unsigned int texttype, SteamLanguage lang = GetSteamLanguage());
	int GetSteamTextSize(unsigned int texttype, SteamLanguage lang = GetSteamLanguage());
};

#endif
