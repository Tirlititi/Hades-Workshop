#include "ClusterData.h"

#include <cstring>
#include <sstream>
#include <iomanip>
#include "File_Manipulation.h"

#define CLUSTER_DEFAULT_EXTRA_SIZE 0xFFFFFF

void ChunkChild::Init(ChunkData* pc, Chunk_Type ct, uint16_t objid) {
	if (pc)
		parent_cluster = pc->parent;
	else
		parent_cluster = NULL;
	parent_chunk = pc;
	type = ct;
	object_id = objid;
	loaded = false;
	modified = false;
}

void ChunkChild::Init(bool createdummyclus, Chunk_Type ct, uint16_t objid, ClusterData** dummyclus, Cluster_Type clustype) {
	unsigned int chki;
	if (createdummyclus) {
		ClusterData* dumcl = new ClusterData[1];
		dumcl->Init(NULL,CHUNK_TYPE_CLUSTER_DATA,objid);
		dumcl->clus_type = clustype;
		dumcl->size = 0;
		dumcl->extra_size = CLUSTER_DEFAULT_EXTRA_SIZE;
		dumcl->size_modified = false;
		dumcl->chunk_amount = 0;
		dumcl->chunk = new ChunkData[0];
		*dummyclus = dumcl;
	}
	parent_cluster = *dummyclus;
	chki = parent_cluster->chunk_amount;
	parent_cluster->chunk_amount++;
	ChunkData* newchunk = new ChunkData[parent_cluster->chunk_amount];
	memcpy(newchunk,parent_cluster->chunk,chki*sizeof(ChunkData));
	newchunk[chki].parent = parent_cluster;
	newchunk[chki].modified = false;
	newchunk[chki].type = ct;
	newchunk[chki].object_amount = 1;
	newchunk[chki].object_id = new uint16_t[1];
	newchunk[chki].object_id[0] = objid;
	newchunk[chki].size = 0;
	newchunk[chki].object = this;
	parent_chunk = &newchunk[chki];
	delete[] parent_cluster->chunk;
	parent_cluster->chunk = newchunk;
	for (chki=0;chki<parent_cluster->chunk_amount;chki++)
		parent_cluster->chunk[chki].object[0].parent_chunk = &parent_cluster->chunk[chki];
	type = ct;
	object_id = objid;
	loaded = false;
	modified = false;
}

void ChunkChild::MarkDataModified() {
	modified = true;
	if (parent_chunk)
		parent_chunk->modified = true;
	if (parent_cluster)
		parent_cluster->MarkDataModified();
}

uint32_t ChunkChild::GetExtraSize() {
	if (parent_cluster)
		return parent_cluster->extra_size;
	return CLUSTER_DEFAULT_EXTRA_SIZE;
}

void ChunkChild::SetSize(uint32_t newsize) {
	if (size==newsize)
		return;
	if (GetGameType()!=GAME_TYPE_PSX) {
		size = newsize;
		return;
	}
	parent_chunk->size += newsize-size;
	parent_cluster->size += newsize-size;
	parent_cluster->extra_size += size-newsize;
	parent_cluster->size_modified = true;
	if (parent_cluster->parent_cluster) {
		parent_cluster->parent_cluster->size += newsize-size;
//		if (this==&parent_chunk->GetObject(0))
//			parent_cluster->parent_cluster->extra_size += size-newsize;
		parent_cluster->parent_cluster->size_modified = true;
	}
	size = newsize;
}

void ChunkChild::Read(fstream& f) {
	if (loaded)
		return;
	raw_data = new uint8_t[size];
	for (unsigned int i=0;i<size;i++)
		FFIXReadChar(f,raw_data[i]);
	loaded = true;
}

void ChunkChild::Write(fstream& f) {
	for (unsigned int i=0;i<size;i++)
		FFIXWriteChar(f,raw_data[i]);
}

void ChunkChild::WritePPF(fstream& f) {
	PPFInitScanStep(f);
	for (unsigned int i=0;i<size;i++)
		PPFStepAddChar(f,raw_data[i]);
	PPFEndScanStep();
}

ChunkChild& ChunkData::GetObject(unsigned int i) {
	switch (type) {
	case CHUNK_TYPE_VARIOUS: { // DEBUG & for creating
		WorldMapDataStruct* res = (WorldMapDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_TIM: {
		TIMImageDataStruct* res = (TIMImageDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_SCRIPT: {
		ScriptDataStruct* res = (ScriptDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_TEXT: {
		TextDataStruct* res = (TextDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_FIELD_TILES: {
		FieldTilesDataStruct* res = (FieldTilesDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_FIELD_WALK: {
		FieldWalkmeshDataStruct* res = (FieldWalkmeshDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_BATTLE_SCENE: {
		BattleSceneDataStruct* res = (BattleSceneDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_CHARMAP: {
		CharmapDataStruct* res = (CharmapDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_ENEMY_STATS: {
		EnemyDataStruct* res = (EnemyDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_BATTLE_DATA: {
		BattleDataStruct* res = (BattleDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_IMAGE_MAP: {
		ImageMapDataStruct* res = (ImageMapDataStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_CLUSTER_DATA: {
		ClusterData* res = (ClusterData*)object;
		return res[i];
	}
	case CHUNK_TYPE_WORLD_MAP: {
		WorldMapInfoStruct* res = (WorldMapInfoStruct*)object;
		return res[i];
	}
	case CHUNK_TYPE_FIELD_ROLE: {
		FieldRoleDataStruct* res = (FieldRoleDataStruct*)object;
		return res[i];
	}
	}
	return object[i];
}

#define MACRO_CHUNK_IOFUNCTION(IO,SEEK,CHILDFUNC,READ,PPF,DOCHILDCLUSTER) \
	unsigned int i; \
	uint16_t zero16 = 0; \
	if (READ) offset = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Char(f,type); \
	IO ## Char(f,object_amount); \
	IO ## Short(f,zero16); \
	if (READ) { \
		object_id = new uint16_t[object_amount]; \
		object_offset = new uint32_t[object_amount]; \
		object_relative_offset = new uint32_t[object_amount]; \
		switch (type) { \
		case CHUNK_TYPE_VARIOUS: { \
			object = new WorldMapDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_TIM: { \
			object = new TIMImageDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_SCRIPT: { \
			object = new ScriptDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_TEXT: { \
			object = new TextDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_FIELD_TILES: { \
			object = new FieldTilesDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_FIELD_WALK: { \
			object = new FieldWalkmeshDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_BATTLE_SCENE: { \
			object = new BattleSceneDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_CHARMAP: { \
			object = new CharmapDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_ENEMY_STATS: { \
			object = new EnemyDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_BATTLE_DATA: { \
			object = new BattleDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_IMAGE_MAP: { \
			object = new ImageMapDataStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_CLUSTER_DATA: { \
			object = new ClusterData[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_WORLD_MAP: { \
			object = new WorldMapInfoStruct[object_amount]; \
			break; \
		} \
		case CHUNK_TYPE_FIELD_ROLE: { \
			object = new FieldRoleDataStruct[object_amount]; \
			break; \
		} \
		default: { \
			object = new ChunkChild[object_amount]; \
		} \
		} \
	} \
	for (i=0;i<object_amount;i++) { \
		IO ## Short(f,object_id[i]); \
		if (READ) GetObject(i).Init(this,type,object_id[i]); \
	} \
	if (object_amount & 1) \
		IO ## Short(f,zero16); \
	uint32_t offsetstart = f.tellg(); \
	for (i=0;i<object_amount;i++) \
		IO ## Long(f,object_relative_offset[i]); \
	IO ## Long(f,object_relative_end); \
	if (object_relative_end==4) object_relative_end = 8; \
	if (PPF) PPFEndScanStep(); \
	for (i=0;i<object_amount;i++) { \
		SEEK(f,offsetstart,object_relative_offset[i]+i*4); \
		if (READ) { \
			object_offset[i] = f.tellg(); \
			if (i+1!=object_amount) \
				GetObject(i).size = object_relative_offset[i+1]+4-object_relative_offset[i]; \
			else \
				GetObject(i).size = object_relative_end-object_relative_offset[i]-4*(object_amount-1); \
		} \
		if ((!READ && type!=CHUNK_TYPE_CLUSTER_DATA) || (DOCHILDCLUSTER && type==CHUNK_TYPE_CLUSTER_DATA)) { \
			if (type==CHUNK_TYPE_CLUSTER_DATA) ((ClusterData&)GetObject(i)).CHILDFUNC(f); \
			else GetObject(i).CHILDFUNC(f); \
		} \
	} \
	if (READ) { \
		SEEK(f,offsetstart,object_relative_end+object_amount*4); \
		object_end = f.tellg(); \
		size = 4+2*object_amount+2*(object_amount%2)+object_relative_end; \
	}

void ChunkData::Read(fstream& f, bool savechildcluster) {
	MACRO_CHUNK_IOFUNCTION(FFIXRead,FFIXSeek,Read,true,false,true)
}

void ChunkData::Write(fstream& f, bool savechildcluster) {
	if (!modified && !parent->size_modified && (!parent->parent_cluster || !parent->parent_cluster->size_modified))
		return;
	MACRO_CHUNK_IOFUNCTION(FFIXWrite,FFIXSeek,Write,false,false,savechildcluster)
	modified = false;
}

void ChunkData::WritePPF(fstream& f, bool savechildcluster) {
	if (!modified && !parent->size_modified && (!parent->parent_cluster || !parent->parent_cluster->size_modified))
		return;
	MACRO_CHUNK_IOFUNCTION(PPFStepAdd,FFIXSeek,WritePPF,false,true,savechildcluster)
}

void ChunkData::Flush() {
	delete[] object_id;
	delete[] object_offset;
	delete[] object_relative_offset;
	for (unsigned int i=0;i<object_amount;i++)
		GetObject(i).Flush();
	delete[] object;
}

void ChunkData::UpdateOffset() {
	uint32_t pos = 4+4*object_amount;
	for (unsigned int i=0;i<object_amount;i++) {
		// Cluster datas can't be placed anywhere (every 0x20 bytes maybe) so I don't move them
		// Clusters' "size" include the size of child cluster
		// but "extra_size" is the blank left between the regular child chunk and the 1st child cluster
		// That is the reason of the strange "extra_size" calculation in the Read method
		if (type==CHUNK_TYPE_CLUSTER_DATA) {
			uint8_t chunkid = 0;
			while (chunkid<parent->chunk_amount && this!=&parent->chunk[chunkid]) chunkid++;
			if (chunkid<parent->chunk_amount) {
				uint32_t clusteroff = object_offset[i]-parent->offset;
				clusteroff -= FILE_IGNORE_DATA_AMOUNT*(clusteroff/FILE_IGNORE_DATA_PERIOD);
				clusteroff -= 8+4*chunkid+parent->chunk_relative_offset[chunkid]+4*((object_amount+1)/2)+4*i;
				pos = clusteroff;
			}
		}
		if (object_relative_offset[i]!=pos)
			parent->size_modified = true;
		object_relative_offset[i] = pos;
		if (type!=CHUNK_TYPE_CLUSTER_DATA || (type==CHUNK_TYPE_CLUSTER_DATA && ((ClusterData&)GetObject(i)).loaded_children))
			GetObject(i).UpdateOffset();
		pos += GetObject(i).size-4;
	}
	object_relative_end = pos+4*object_amount;
}

#define MACRO_CLUSTER_IOFUNCTION(IO,SEEK,CHILDFUNC,READ,PPF,DOCHILDCLUSTER) \
	unsigned int i; \
	uint16_t zero16 = 0; \
	uint8_t db = 0xDB; \
	if (READ) offset = f.tellg(); \
	if (PPF) PPFInitScanStep(f); \
	IO ## Char(f,db); \
	IO ## Char(f,chunk_amount); \
	IO ## Short(f,zero16); \
	uint32_t offsetstart = f.tellg(); \
	if (READ) { \
		chunk_type = new Chunk_Type[chunk_amount]; \
		chunk_offset = new uint32_t[chunk_amount]; \
		chunk_relative_offset = new uint32_t[chunk_amount]; \
		chunk = new ChunkData[chunk_amount]; \
	} \
	for (i=0;i<chunk_amount;i++) { \
		if (READ) chunk[i].parent = this; \
		if (READ) chunk[i].modified = false; \
		IO ## Long3(f,chunk_relative_offset[i]); \
		IO ## Char(f,chunk_type[i]); \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i=0;i<chunk_amount;i++) { \
		SEEK(f,offsetstart,chunk_relative_offset[i]+i*4); \
		if (READ) chunk_offset[i] = f.tellg(); \
		if (!READ || chunk_type[i]==CHUNK_TYPE_CLUSTER_DATA) chunk[i].CHILDFUNC(f,DOCHILDCLUSTER); \
	} \
	if (READ) { \
		SEEK(f,chunk_offset[chunk_amount-1],1); \
		uint8_t lastchunkamount; \
		IO ## Char(f,lastchunkamount); \
		SEEK(f,f.tellg(),2); \
		SEEK(f,f.tellg(),2*(lastchunkamount+lastchunkamount%2)); \
		uint32_t lastchunkoffsetstart = f.tellg(); \
		SEEK(f,f.tellg(),4*lastchunkamount); \
		uint32_t lastchunkendoffset; \
		IO ## Long(f,lastchunkendoffset); \
		SEEK(f,lastchunkoffsetstart,lastchunkendoffset); \
		IO ## Short(f,ending_data1); \
		IO ## Short(f,ending_data2); \
		size = 4+GetFFIXOffsetSub(offsetstart,f.tellg()); \
		int clusterid = SearchChunkType(CHUNK_TYPE_CLUSTER_DATA); \
		if (clusterid>0) { \
			uint32_t tmppos = f.tellg(); \
			SEEK(f,chunk_offset[clusterid],1); \
			uint8_t cluschunkamount; \
			IO ## Char(f,cluschunkamount); \
			SEEK(f,f.tellg(),2); \
			SEEK(f,f.tellg(),2*(cluschunkamount+cluschunkamount%2)); \
			uint32_t cluschunkoffsetstart = f.tellg(); \
			uint32_t cluschunkobjectoffset; \
			IO ## Long(f,cluschunkobjectoffset); \
			SEEK(f,f.tellg(),4*cluschunkamount); \
			uint32_t cluschunkheaderend = f.tellg(); \
			SEEK(f,cluschunkoffsetstart,cluschunkobjectoffset); \
			extra_size = GetFFIXOffsetSub(cluschunkheaderend,f.tellg()); \
			SEEK(f,tmppos,0); \
		} else if (clusterid==0) \
			extra_size = 0; \
		else \
			extra_size = GetFFIXNextIgnore(f.tellg()); \
	} else { \
		SEEK(f,offset,size-4); \
		if (PPF) PPFInitScanStep(f); \
		IO ## Short(f,ending_data1); \
		IO ## Short(f,ending_data2); \
		if (PPF) PPFEndScanStep(); \
	}


void ClusterData::Read(fstream& f) {
	size_modified = false;
	loaded_children = false;
	MACRO_CLUSTER_IOFUNCTION(FFIXRead,FFIXSeek,Read,true,false,true)
}

void ClusterData::CreateChildren(fstream& f) {
	if (loaded_children)
		return;
	for (unsigned int i=0;i<chunk_amount;i++) {
		if (chunk_type[i]!=CHUNK_TYPE_CLUSTER_DATA) {
			f.seekg(chunk_offset[i]);
			chunk[i].Read(f);
		}
	}
	loaded_children = true;
}

void ClusterData::Write(fstream& f, bool savechildcluster) {
	if (!modified && (!parent_cluster || !parent_cluster->size_modified))
		return;
	UpdateOffset();
	MACRO_CLUSTER_IOFUNCTION(FFIXWrite,FFIXSeek,Write,false,false,savechildcluster)
	modified = false;
	size_modified = false;
}

void ClusterData::WritePPF(fstream& f, bool savechildcluster) {
	if (!modified && (!parent_cluster || !parent_cluster->size_modified))
		return;
	UpdateOffset();
	MACRO_CLUSTER_IOFUNCTION(PPFStepAdd,FFIXSeek,WritePPF,false,true,savechildcluster)
}

void ClusterData::Flush() {
	delete[] chunk_type;
	delete[] chunk_offset;
	delete[] chunk_relative_offset;
	if (loaded_children) {
		for (unsigned int i=0;i<chunk_amount;i++)
			chunk[i].Flush();
		delete[] chunk;
	}
}

int ClusterData::SearchChunkType(Chunk_Type type) {
	for (unsigned int i=0;i<chunk_amount;i++)
		if (chunk_type[i]==type)
			return i;
	return -1;
}

void ClusterData::UpdateOffset() {
	unsigned int i,j;
	if (GetGameType()==GAME_TYPE_PSX) {
		uint32_t pos = 4*chunk_amount;
		for (i=0;i<chunk_amount;i++) {
			chunk_relative_offset[i] = pos;
			chunk[i].UpdateOffset();
			pos += chunk[i].size-4;
		}
	} else {
		for (i=0;i<chunk_amount;i++)
			for (j=0;j<chunk[i].object_amount;j++)
				chunk[i].GetObject(j).UpdateOffset();
	}
}

void ClusterSet::Read(fstream& f, ConfigurationSet& config) {
	unsigned int i;
	global_map.Read(f);
	amount = config.cluster_amount;
	enemy_amount = 0;
	text_amount = 0;
	world_amount = 0;
	field_amount = 0;
	battle_scene_amount = 0;
	clus_type = new Cluster_Type[amount];
	clus = new ClusterData[amount];
	for (i=0;i<amount;i++) {
		clus[i].Init(NULL,CHUNK_TYPE_CLUSTER_DATA,0);
		f.seekg(config.cluster_offset[i]);
		clus[i].Read(f);
		clus_type[i] = CLUSTER_TYPE_UNKNOWN;
		clus[i].clus_type = CLUSTER_TYPE_UNKNOWN;
		if (clus[i].SearchChunkType(CHUNK_TYPE_VARIOUS)>=0) {
			uint8_t objamount;
			f.seekg(clus[i].chunk_offset[clus[i].SearchChunkType(CHUNK_TYPE_VARIOUS)]);
			FFIXReadChar(f,objamount);
			FFIXReadChar(f,objamount);
			if (objamount==1)
				unknown_shared_data_index = i;
			else if (objamount==11)
				world_map_shared_data_index = i;
			else if (objamount==5 || objamount==7)
				battle_shared_data_index = i;
		}
		int childclusi = clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA);
		if (childclusi>=0) {
			if (clus[i].chunk_amount==2) {
				ClusterData& childclus = static_cast<ClusterData&>(clus[i].chunk[childclusi].GetObject(1));
				if (childclus.SearchChunkType(CHUNK_TYPE_FIELD_TILES)>=0) {
					clus_type[i] = CLUSTER_TYPE_FIELD;
					clus[i].clus_type = CLUSTER_TYPE_FIELD;
					field_amount++;
				} else {
					clus_type[i] = CLUSTER_TYPE_WORLD_MAP;
					clus[i].clus_type = CLUSTER_TYPE_WORLD_MAP;
					world_amount++;
				}
			} else {
				ClusterData& childclus = static_cast<ClusterData&>(clus[i].chunk[childclusi].GetObject(0));
				if (childclus.SearchChunkType(CHUNK_TYPE_IMAGE_MAP)>=0) {
					clus_type[i] = CLUSTER_TYPE_WEAPON_MODEL;
					clus[i].clus_type = CLUSTER_TYPE_WEAPON_MODEL;
				}
			}
		} else if (clus[i].chunk_amount==1 && clus[i].chunk_type[0]==CHUNK_TYPE_IMAGE_MAP) {
			clus_type[i] = CLUSTER_TYPE_IMAGE_MAP;
			clus[i].clus_type = CLUSTER_TYPE_IMAGE_MAP;
			enemy_shared_map_index = i; // Assume there is exactly one
		} else if (clus[i].SearchChunkType(CHUNK_TYPE_ENEMY_STATS)>=0) {
			clus_type[i] = CLUSTER_TYPE_ENEMY;
			clus[i].clus_type = CLUSTER_TYPE_ENEMY;
			enemy_amount++;
		} else if (clus[i].SearchChunkType(CHUNK_TYPE_BATTLE_SCENE)>=0) {
			clus_type[i] = CLUSTER_TYPE_BATTLE_SCENE;
			clus[i].clus_type = CLUSTER_TYPE_BATTLE_SCENE;
			battle_scene_amount++;
		} else if (clus[i].SearchChunkType(CHUNK_TYPE_TEXT)>=0 || clus[i].SearchChunkType(CHUNK_TYPE_CHARMAP)>=0) {
			clus_type[i] = CLUSTER_TYPE_FIELD_TEXT;
			clus[i].clus_type = CLUSTER_TYPE_FIELD_TEXT;
			text_amount++;
		}
	}
	loaded_children = false;
	loaded_map = false;
}

void ClusterSet::CreateChildren(fstream& f) {
	if (loaded_children)
		return;
	for (unsigned int i=0;i<amount;i++)
		clus[i].CreateChildren(f);
	loaded_children = true;
}

ImageMapDataStruct* imgmaptmp[0x1000];
void ClusterSet::CreateImageMaps(fstream& f) {
	if (loaded_map)
		return;
	clus[enemy_shared_map_index].CreateChildren(f);
	enemy_shared_map = (ImageMapDataStruct*)&clus[enemy_shared_map_index].chunk[0].GetObject(0);
	f.seekg(enemy_shared_map->parent_chunk->object_offset[0]);
	enemy_shared_map->Read(f);
	ChunkData* chunkmap;
	image_map_amount = 0;
	for (unsigned int i=0;i<amount;i++) {
		if (clus_type[i]==CLUSTER_TYPE_IMAGE_MAP || clus_type[i]==CLUSTER_TYPE_ENEMY || clus_type[i]==CLUSTER_TYPE_WORLD_MAP || clus_type[i]==CLUSTER_TYPE_FIELD || clus_type[i]==CLUSTER_TYPE_WEAPON_MODEL) {
			clus[i].CreateChildren(f);
			if (clus_type[i]==CLUSTER_TYPE_WEAPON_MODEL) {
				ClusterData& childclus = (ClusterData&)clus[i].chunk[clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA)].GetObject(0);
				childclus.CreateChildren(f);
				chunkmap = &childclus.chunk[childclus.SearchChunkType(CHUNK_TYPE_IMAGE_MAP)];
			} else
				chunkmap = &clus[i].chunk[clus[i].SearchChunkType(CHUNK_TYPE_IMAGE_MAP)];
			f.seekg(chunkmap->object_offset[0]);
			chunkmap->GetObject(0).Read(f);
			imgmaptmp[image_map_amount++] = (ImageMapDataStruct*)&chunkmap->GetObject(0);
		}
	}
	image_map = new ImageMapDataStruct*[image_map_amount];
	memcpy(image_map,imgmaptmp,image_map_amount*sizeof(ImageMapDataStruct*));
	loaded_map = true;
}

ChunkChild* ClusterSet::FindObjectById(Chunk_Type type, uint16_t id) {
	unsigned int i,j,k;
	int chk,cls;
	for (i=0;i<amount;i++) {
		chk = clus[i].SearchChunkType(type);
		if (chk>=0) {
			for (k=0;k<clus[i].chunk[chk].object_amount;k++)
				if (clus[i].chunk[chk].object_id[k]==id)
					return &clus[i].chunk[chk].GetObject(k);
		}
		cls = clus[i].SearchChunkType(CHUNK_TYPE_CLUSTER_DATA);
		if (cls>=0) {
			for (j=0;j<clus[i].chunk[cls].object_amount;j++) {
				ClusterData& childclus = (ClusterData&)clus[i].chunk[cls].GetObject(j);
				chk = childclus.SearchChunkType(type);
				if (chk>=0) {
					for (k=0;k<childclus.chunk[chk].object_amount;k++)
						if (childclus.chunk[chk].object_id[k]==id)
							return &childclus.chunk[chk].GetObject(k);
				}
			}
		}
	}
	return NULL;
}

