#ifndef _IMAGEMAPS_H
#define _IMAGEMAPS_H

struct GlobalMapCommonDirStruct;
struct GlobalMapSpellDirStruct;
struct GlobalImageMapStruct;
struct ImageMapDataStruct;

#include <inttypes.h>
#include <fstream>
using namespace std;

#define IMG_HEADER_OFFSET	0x233508L

#define GLOBAL_MAP_DIR_AMOUNT		15
#define GLOBAL_MAP_DIR_MENU			0
#define GLOBAL_MAP_DIR_GENERIC		1
#define GLOBAL_MAP_DIR_TEXT			2
#define GLOBAL_MAP_DIR_WORLD_MAP	3
#define GLOBAL_MAP_DIR_FIELD		4
#define GLOBAL_MAP_DIR_ENEMY		5
#define GLOBAL_MAP_DIR_BATTLE_SCENE	6
#define GLOBAL_MAP_DIR_MODEL_ENEMY	7
#define GLOBAL_MAP_DIR_MODEL_WEAPON	8
#define GLOBAL_MAP_DIR_MUSIC		9
#define GLOBAL_MAP_DIR_MODEL_PARTY	10
#define GLOBAL_MAP_DIR_AUDIO		11
#define GLOBAL_MAP_DIR_UNKNOWN		12
#define GLOBAL_MAP_DIR_SPELL_ANIM	13
#define GLOBAL_MAP_DIR_END			14

typedef uint8_t Map_Object;
#define MAP_OBJECT_ENEMY	0
#define MAP_OBJECT_SCENE	1
#define MAP_OBJECT_MUSIC	2
#define MAP_OBJECT_AUDIO	3
#define MAP_OBJECT_FIELD	4
#define MAP_OBJECT_WORLD	5
#define MAP_OBJECT_MODEL	6

/* Offsets and sizes are measured in 2048-bytes blocks in Image Maps
 * 
 * The Global Image Map is assumed to contain these data directories in the order :
 * 
 * 	Directory1 : 21 files
 * 		???,SaveMenu,Battle,???,TetraMaster,
 * 		???,???,???,SkillMenu+EquipMenu,ConfigMenu+Map+???,
 * 		CardMenu,ItemMenu,StatusMenu,LoadMenu,NamingMenu,
 * 		TeamMenu,ShopMenu,???,???,???,???
 * 	Directory2 :
 * 		Generic Cluster Datas (charmaps, backgrounds...)
 * 	Directory3 :
 * 		Dialog & Text Cluster Datas
 * 	Directory4 :
 * 		World Map Cluster Datas
 * 	Directory5 :
 * 		Field Cluster Datas
 * 	Directory6 :
 * 		Enemy Cluster Datas
 * 	Directory7 :
 * 		Battle Scene Cluster Datas
 * 	Directory8 :
 * 		Enemy Model Cluster Datas
 * 	Directory9 :
 * 		Weapon Model Cluster Datas
 * 	Directory10 :
 * 		Music Cluster Datas
 * 	Directory11 :
 * 		Party Battle Model Cluster Datas
 * 	Directory12 :
 * 		Audio Cluster Datas
 * 	Directory13 :
 * 		Unknown
 * 	Directory14 :
 * 		Spell Animation Datas
 * 	Directory15 :
 * 		Dummy for directory ending
 */

struct GlobalMapCommonDirStruct {
public:
	uint32_t file_amount;
	uint32_t offset;
	uint32_t first_file_offset;
	
	uint16_t* file_id;
	uint16_t* file_related_id;
	uint32_t* file_offset;
	
	uint32_t* file_size;
};

struct GlobalMapSpellDirStruct {
	uint32_t file_amount;
	uint32_t offset;
	uint32_t first_file_offset;
	
	uint16_t* file_offset;
};

struct GlobalImageMapStruct {
public:
	uint32_t magic_ff9;
	uint32_t unknown1;
	uint32_t dir_amount; // == GLOBAL_MAP_DIR_AMOUNT with every dir in the right order assumed
	uint32_t disc_number;
	uint32_t* dir_type;
	uint32_t* dir_file_amount;
	uint32_t* dir_offset;
	uint32_t* dir_first_file_offset;
	GlobalMapCommonDirStruct* common_dir;
	GlobalMapSpellDirStruct* spell_dir;
	
	void Read(fstream& f);
};

#include "File_Manipulation.h"
#include "Configuration.h"

struct ImageMapDataStruct : public ChunkChild {
public:
	// PSX version
	uint16_t zero;
	uint16_t amount;
	uint16_t* data_id;
	Chunk_Type* data_type;
	uint8_t* unknown1;
	uint32_t* data_offset;
	uint32_t* data_size;
	uint16_t* data_related_id;
	Chunk_Type* data_related_type;
	uint8_t* unknown3;
	
	uint32_t extra_size; // ToDo : get rid of this and add a way to change the Sub-Clusters' offsets. Warning : sub-clusters are not necessarily loaded
	
	// Return the amount added or negative on fail
	int AddDataSingle(uint16_t id, Chunk_Type type, uint8_t unk1, uint32_t offset, uint32_t sz, uint16_t relatedid, Chunk_Type relatedtype, uint8_t unk3);
	int AddData(Map_Object kind, uint16_t id, GlobalImageMapStruct* theimgmap, ImageMapDataStruct* enmymap, int maxtoadd);
	void RemoveDataByPos(uint16_t mappos);
	int RemoveData(Map_Object kind, uint16_t id);
	static Chunk_Type GetChunkTypeFromMapObject(Map_Object kind);
	
	// Steam version
	unsigned int type_amount;
	unsigned int* data_amount;
	string* type_name;
	string** data_name;
	
	void Copy(ImageMapDataStruct& cpy);
	void ReadSteamFormat(fstream& f);
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void UpdateOffset();
};

#endif
