#ifndef _CLUSTER_DATA_H
#define _CLUSTER_DATA_H

/*  Cluster Datas are objects containing several sub-files in the PSX version.
 *  A lot of the game's datas are stored in a cluster even if it's not all of them.
 * 
 *  Generally, all the sub-files stored in 1 cluster are related to each other. For
 * example, some clusters contain informations needed for a battle, such as the
 * enemies' statistics and the sounds used by them, the stats and the sound being
 * stored in 2 separate files inside the 1 cluster.
 *  Thus clusters are categorized depending on what kind of data they contain :
 * it is its Cluster_Type.
 *  There is also one step between the cluster and the file itself. This step is
 * called a Chunk Data. It basically contains several files of the same type
 * (Chunk_Type) being either music or model or whatever.
 *  Be aware that Chunk_Type is a categorization written in the binary file while
 * Cluster_Type is not (for what I know) and is guessed by the program.
 * 
 *  For the Steam version, the files of the PSX's cluster datas are often present
 * untouched in the Steam archives. Those files are thus read the same way. However,
 * the cluster structure itself is not the same at all (all the files related to a
 * battle are not stored at the same place, for instance) and the ClusterData struct
 * is less important. Dummy ClusterData objects are still created to mimic the
 * behavior of the PSX format though.
 * 
 *  About how the program reads Clusters : the clusters' header offsets are stored
 * in the .hwf file. Upon opening, all the clusters are Read, meaning the Chunk Types
 * of the files they contain is read and their Cluster Type is guessed from that.
 * Also, their size is retrieved.
 *  All those clusters are stored in the main struct, ClusterSet.
 *  However, the chunks themselves are only allocated, not read. That is with one
 * exception being a special Chunk Type, CHUNK_TYPE_CLUSTER_DATA, that contains
 * sub-clusters (which are needed to guess the Cluster Type of the top-level cluster).
 *  When the datas are requested, the chunks can be read by calling the method
 * ClusterData::CreateChildren. Once the chunks are read, the files they contain may
 * also be read, by seeking to their offset (object_offset[]) and calling the virtual
 * method ChunkChild::Read.
 * 
 *  Note that one may read all the chunks in one shot with ClusterSet::CreateChildren
 * but it misses the chunks stored in the sub-clusters : those must be fully read
 * by a direct call of ClusterData::CreateChildren (this is to always read the
 * less datas possible because there are quiet a lot of them - around 1780 top-level
 * clusters and 1420 sub-clusters in one disc -). There is no sub-sub-clusters, that
 * is to say top-level clusters containing a chunk of type CHUNK_TYPE_CLUSTER_DATA
 * containing a cluster containing another chunk of type CHUNK_TYPE_CLUSTER_DATA.
 */

struct ChunkChild;
struct ChunkData;
struct ClusterData;
struct ClusterSet;

#include <inttypes.h>
#include <fstream>
using namespace std;

typedef uint8_t Chunk_Type;
#define CHUNK_TYPE_VARIOUS			0x00
#define CHUNK_TYPE_MODEL			0x02
#define CHUNK_TYPE_ANIMATION		0x03
#define CHUNK_TYPE_TIM				0x04
#define CHUNK_TYPE_SCRIPT			0x05
#define CHUNK_TYPE_TEXT				0x06
#define CHUNK_TYPE_SEQUENCER		0x07
#define CHUNK_TYPE_SOUND			0x08
#define CHUNK_TYPE_AUDIO			0x09
#define CHUNK_TYPE_FIELD_TILES		0x0A
#define CHUNK_TYPE_FIELD_WALK		0x0B
#define CHUNK_TYPE_BATTLE_SCENE		0x0C
#define CHUNK_TYPE_CHARMAP			0x0D
#define CHUNK_TYPE_ENEMY_STATS		0x10
#define CHUNK_TYPE_BATTLE_DATA		0x11
#define CHUNK_TYPE_CLUT_TPAGE		0x12
#define CHUNK_TYPE_IMAGE_MAP		0x14
#define CHUNK_TYPE_CLUSTER_DATA		0x1B
#define CHUNK_TYPE_WORLD_MAP		0x1D
#define CHUNK_TYPE_FIELD_ROLE		0x1F
#define CHUNK_STEAM_FIELD_NAME		0x80 // Steam field single-language name
#define CHUNK_STEAM_FIELD_MULTINAME	0x81 // Steam field multi-language name
#define CHUNK_SPECIAL_TYPE_LOCAL	0xFE // Hades Workshop's custom type
#define CHUNK_SPECIAL_END			0xFF // End of cluster

typedef uint8_t Cluster_Type;
#define CLUSTER_TYPE_UNKNOWN		0
#define CLUSTER_TYPE_ENEMY			1 // USED - EnemyDataSet
#define CLUSTER_TYPE_FIELD_TEXT		2 // USED - TextDataSet
#define CLUSTER_TYPE_WORLD_MAP		3 // USED - WorldMapDataSet
#define CLUSTER_TYPE_FIELD			4 // USED - FieldDataSet
#define CLUSTER_TYPE_BATTLE_SCENE	5 // USED - BattleSceneSet
#define CLUSTER_TYPE_IMAGE_MAP		6 // USED - ImageMapDataStruct
#define CLUSTER_TYPE_WEAPON_MODEL	7 // UNUSED

struct ChunkChild {
public:
	ClusterData* parent_cluster;
	ChunkData* parent_chunk;
	Chunk_Type type;
	uint32_t size;
	uint16_t object_id;
	uint8_t* raw_data;
	bool loaded;
	bool modified;
	
	void Init(ChunkData* pc, Chunk_Type ct, uint16_t objid);
	// For Steam version ; *dummyclus is a cluster shared by a set of chunkchilds ; if createdummyclus==true, *dummyclus is initialized
	void Init(bool createdummyclus, Chunk_Type ct, uint16_t objid, ClusterData** dummyclus, Cluster_Type clustype = CLUSTER_TYPE_UNKNOWN);
	void MarkDataModified();
	uint32_t GetExtraSize();
	void SetSize(uint32_t newsize);
	virtual void Read(fstream& f);
	virtual void Write(fstream& f);
	virtual void WritePPF(fstream& f);
	virtual void UpdateOffset() {}
	virtual void Flush() {}
};

struct ChunkData {
public:
	ClusterData* parent;
	uint32_t offset;
	Chunk_Type type;
	uint8_t object_amount;
	uint16_t* object_id;
	uint32_t* object_offset;
	uint32_t* object_relative_offset;
	uint32_t object_relative_end;
	uint32_t object_end;
	uint32_t size;
	bool modified;
	
	ChunkChild& GetObject(unsigned int i);
	void Read(fstream& f, bool savechildcluster = true); // 2nd argument is dummy there
	void Write(fstream& f, bool savechildcluster = true);
	void WritePPF(fstream& f, bool savechildcluster = true);
	void UpdateOffset();
	void Flush();
	
private:
	ChunkChild* object;
	friend ChunkChild;
};

struct ClusterData : public ChunkChild {
public:
	Cluster_Type clus_type;
	uint32_t offset;
	uint8_t chunk_amount;
	uint32_t extra_size;
	uint16_t ending_data1;
	uint16_t ending_data2;
	Chunk_Type* chunk_type;
	uint32_t* chunk_offset;
	uint32_t* chunk_relative_offset;
	ChunkData* chunk;
	bool size_modified;
	
	void Read(fstream& f); // Read from current offset ; Init everything but chunks
	void CreateChildren(fstream& f); // Allocate and read chunk and allocate objects (but don't read them)
	void Write(fstream& f, bool savechildcluster = true);
	void WritePPF(fstream& f, bool savechildcluster = true);
	void Flush();
	void UpdateOffset();
	int SearchChunkType(Chunk_Type type); // Only search in main cluster, not children
	
private:
	bool loaded_children;
	friend ChunkData;
};

#include "ImageMaps.h"
#include "Configuration.h"

struct ClusterSet {
public:
	uint16_t amount;
	uint16_t enemy_amount;
	uint16_t text_amount;
	uint16_t world_amount;
	uint16_t field_amount;
	uint16_t battle_scene_amount;
	Cluster_Type* clus_type;
	ClusterData* clus;
	
	ConfigurationSet* config;
	
	GlobalImageMapStruct global_map;
	uint16_t enemy_shared_map_index;
	uint16_t world_map_shared_data_index;
	uint16_t unknown_shared_data_index;
	uint16_t battle_shared_data_index;
	ImageMapDataStruct* enemy_shared_map; // map linking battle, battle scene and battle music
	ImageMapDataStruct** image_map; // list of image maps including the enemy_map
	unsigned int image_map_amount;
	
	void Read(fstream& f, ConfigurationSet& config); // Get the clus_type and allocate the cluster (don't allocate the chunks)
	void CreateChildren(fstream& f); // Read everything at once ; really slow
	void CreateImageMaps(fstream& f);
	ChunkChild* FindObjectById(Chunk_Type type, uint16_t id); // The result must have been allocated before

private:
	bool loaded_children;
	bool loaded_map;
};

#endif
