#ifndef _WORLDMAPS_H
#define _WORLDMAPS_H

#define WORLD_MAP_PLACE_AMOUNT			64
#define WORLD_MAP_BATTLE_GROUND_AMOUNT	354
#define WORLD_MAP_BATTLE_SET_AMOUNT		4

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
	
	uint16_t place_name_extra_size;
	FF9String place_name[WORLD_MAP_PLACE_AMOUNT];
	uint16_t* place_unknown_amount;
	uint8_t** place_unknown;
	uint32_t place_name_size; // Steam version only
	
	uint16_t battle_id[WORLD_MAP_BATTLE_GROUND_AMOUNT][WORLD_MAP_BATTLE_SET_AMOUNT];
	uint16_t battle_unknown[WORLD_MAP_BATTLE_GROUND_AMOUNT];
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(unsigned int placeid, wstring newvalue);
	int SetName(unsigned int placeid, FF9String& newvalue);
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, bool usetext = true);
	void WriteHWS(fstream& f);
	void UpdateOffset();
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
	void WriteSteamText(fstream& ffbin, unsigned int texttype);
};

#endif
