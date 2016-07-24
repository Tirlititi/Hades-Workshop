#include "ImageMaps.h"

#include <cstring>

int ImageMapDataStruct::AddDataSingle(uint16_t id, Chunk_Type type, uint8_t unk1, uint32_t offset, uint32_t sz, uint16_t relatedid, Chunk_Type relatedtype, uint8_t unk3) {
	uint16_t* newid = new uint16_t[amount+1];
	Chunk_Type* newtype = new Chunk_Type[amount+1];
	uint8_t* newunknown1 = new uint8_t[amount+1];
	uint32_t* newoffset = new uint32_t[amount+1];
	uint32_t* newsize = new uint32_t[amount+1];
	uint16_t* newrelatedid = new uint16_t[amount+1];
	Chunk_Type* newrelatedtype = new Chunk_Type[amount+1];
	uint8_t* newunknown3 = new uint8_t[amount+1];
	memcpy(newid,data_id,amount*sizeof(uint16_t));
	memcpy(newtype,data_type,amount*sizeof(Chunk_Type));
	memcpy(newunknown1,unknown1,amount*sizeof(uint8_t));
	memcpy(newoffset,data_offset,amount*sizeof(uint32_t));
	memcpy(newsize,data_size,amount*sizeof(uint32_t));
	memcpy(newrelatedid,data_related_id,amount*sizeof(uint16_t));
	memcpy(newrelatedtype,data_related_type,amount*sizeof(Chunk_Type));
	memcpy(newunknown3,unknown3,amount*sizeof(uint8_t));
	newid[amount] = id;
	newtype[amount] = type;
	newunknown1[amount] = unk1;
	newoffset[amount] = offset;
	newsize[amount] = sz;
	newrelatedid[amount] = relatedid;
	newrelatedtype[amount] = relatedtype;
	newunknown3[amount] = unk3;
	delete[] data_id;
	delete[] data_type;
	delete[] unknown1;
	delete[] data_offset;
	delete[] data_size;
	delete[] data_related_id;
	delete[] data_related_type;
	delete[] unknown3;
	data_id = newid;
	data_type = newtype;
	unknown1 = newunknown1;
	data_offset = newoffset;
	data_size = newsize;
	data_related_id = newrelatedid;
	data_related_type = newrelatedtype;
	unknown3 = newunknown3;
	amount++;
	return 1;
}

int ImageMapDataStruct::AddData(Map_Object kind, uint16_t id, GlobalImageMapStruct* theimgmap, ImageMapDataStruct* enmymap, int maxtoadd) {
	Chunk_Type chunktype = GetChunkTypeFromMapObject(kind);
	uint32_t off = 0;
	uint32_t sz = 0;
	bool add = true;
	unsigned int i;
	int addamount;
	int res = 0;
	switch (kind) {
	case MAP_OBJECT_ENEMY: {
		bool addscene = false, addmusic = false, addaudio = false;
		uint16_t sceneid, musicid, audioid = 0;
		uint32_t sceneoff = 0, musicoff = 0, audiooff = 0;
		uint32_t scenesz = 0, musicsz = 0, audiosz = 0;
		for (i=0;i<enmymap->amount;i++)
			if (id==enmymap->data_id[i] && enmymap->data_type[i]==chunktype) {
				if (enmymap->data_related_type[i]==CHUNK_TYPE_BATTLE_SCENE) {
					addscene = true;
					sceneid = enmymap->data_related_id[i];
				} else if (enmymap->data_related_type[i]==CHUNK_TYPE_SEQUENCER) {
					addmusic = true;
					musicid = enmymap->data_related_id[i];
				}
				off = enmymap->data_offset[i];
				sz = enmymap->data_size[i];
				if (addscene && addmusic)
					break;
			}
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (addscene) {
			for (i=0;i<amount;i++)
				if (sceneid==data_id[i] && data_type[i]==CHUNK_TYPE_BATTLE_SCENE) {
					addscene = false;
					break;
				}
			if (addscene)
				for (i=0;i<enmymap->amount;i++)
					if (sceneid==enmymap->data_id[i] && enmymap->data_type[i]==CHUNK_TYPE_BATTLE_SCENE) {
						sceneoff = enmymap->data_offset[i];
						scenesz = enmymap->data_size[i];
						break;
					}
		}
		if (addmusic) {
			for (i=0;i<enmymap->amount;i++)
				if (musicid==enmymap->data_id[i] && enmymap->data_type[i]==CHUNK_TYPE_SEQUENCER) {
					addaudio = true;
					audioid = enmymap->data_related_id[i];
					break;
				}
			for (i=0;i<amount;i++)
				if (musicid==data_id[i] && data_type[i]==CHUNK_TYPE_SEQUENCER) {
					addmusic = false;
					break;
				}
			if (addmusic) {
				for (i=0;i<enmymap->amount;i++)
					if (musicid==enmymap->data_id[i] && enmymap->data_type[i]==CHUNK_TYPE_SEQUENCER) {
						musicoff = enmymap->data_offset[i];
						musicsz = enmymap->data_size[i];
						break;
					}
				if (addaudio) {
					for (i=0;i<amount;i++)
						if (audioid==data_id[i] && data_type[i]==CHUNK_TYPE_AUDIO) {
							addaudio = false;
							break;
						}
					if (addaudio)
						for (i=0;i<enmymap->amount;i++)
							if (audioid==enmymap->data_id[i] && enmymap->data_type[i]==CHUNK_TYPE_AUDIO) {
								audiooff = enmymap->data_offset[i];
								audiosz = enmymap->data_size[i];
								break;
							}
				}
			}
		}
		addamount = (add ? 1 : 0) + (addscene ? 1 : 0) + (addmusic ? 1 : 0) + (addaudio ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,sceneid,CHUNK_TYPE_BATTLE_SCENE,0);
		if (addscene)
			res += AddDataSingle(sceneid,CHUNK_TYPE_BATTLE_SCENE,0,sceneoff,scenesz,id,chunktype,0);
		if (addmusic)
			res += AddDataSingle(musicid,CHUNK_TYPE_SEQUENCER,0,musicoff,musicsz,audioid,CHUNK_TYPE_AUDIO,0);
		if (addaudio)
			res += AddDataSingle(audioid,CHUNK_TYPE_AUDIO,0,audiooff,audiosz,musicid,CHUNK_TYPE_SEQUENCER,0);
		break;
	}
	case MAP_OBJECT_SCENE: {
		uint16_t enemyid = 0;
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (add)
			for (i=0;i<enmymap->amount;i++)
				if (id==enmymap->data_id[i] && enmymap->data_type[i]==chunktype) {
					enemyid = enmymap->data_related_id[i];
					off = enmymap->data_offset[i];
					sz = enmymap->data_size[i];
					break;
				}
		addamount = (add ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,enemyid,CHUNK_TYPE_BATTLE_DATA,0);
		break;
	}
	case MAP_OBJECT_MUSIC: {
		bool addaudio = false;
		uint16_t audioid;
		uint32_t audiooff = 0;
		uint32_t audiosz = 0;
		for (i=0;i<enmymap->amount;i++)
			if (id==enmymap->data_id[i] && enmymap->data_type[i]==chunktype) {
				addaudio = true;
				audioid = enmymap->data_related_id[i];
				off = enmymap->data_offset[i];
				sz = enmymap->data_size[i];
			}
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (addaudio) {
			for (i=0;i<amount;i++)
				if (audioid==data_id[i] && data_type[i]==CHUNK_TYPE_AUDIO) {
					addaudio = false;
					break;
				}
			if (addaudio)
				for (i=0;i<enmymap->amount;i++)
					if (audioid==enmymap->data_id[i] && enmymap->data_type[i]==CHUNK_TYPE_AUDIO) {
						audiooff = enmymap->data_offset[i];
						audiosz = enmymap->data_size[i];
						break;
					}
		}
		addamount = (add ? 1 : 0) + (addaudio ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,0,0,0);
		if (addaudio)
			res += AddDataSingle(audioid,CHUNK_TYPE_AUDIO,0,audiooff,audiosz,id,chunktype,0);
		break;
	}
	case MAP_OBJECT_AUDIO: {
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (add) {
			GlobalMapCommonDirStruct& dir = theimgmap->common_dir[GLOBAL_MAP_DIR_AUDIO];
			for (i=0;i<dir.file_amount;i++)
				if (id==dir.file_id[i]) {
					off = dir.file_offset[i];
					sz = dir.file_size[i];
					break;
				}
		}
		addamount = (add ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,0,0,0);
		break;
	}
	case MAP_OBJECT_FIELD: {
		bool addtext = false;
		uint16_t textid;
		uint32_t textoff;
		uint32_t textsz;
		GlobalMapCommonDirStruct& dir = theimgmap->common_dir[GLOBAL_MAP_DIR_FIELD];
		for (i=0;i<dir.file_amount;i++)
			if (id==dir.file_id[i]) {
				addtext = true;
				textid = dir.file_related_id[i];
				off = dir.file_offset[i];
				sz = dir.file_size[i];
				break;
			}
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (addtext) {
			for (i=0;i<amount;i++)
				if (textid==data_id[i] && data_type[i]==CHUNK_TYPE_TEXT) {
					addtext = false;
					break;
				}
			if (addtext)
				for (i=0;i<theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_amount;i++)
					if (textid==theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_id[i]) {
						textoff = theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_offset[i];
						textsz = theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_size[i];
						break;
					}
		}
		addamount = (add ? 1 : 0) + (addtext ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,textid,CHUNK_TYPE_TEXT,0);
		if (addtext)
			res += AddDataSingle(textid,CHUNK_TYPE_TEXT,0,textoff,textsz,0,0,0);
		break;
	}
	case MAP_OBJECT_WORLD: {
		bool hastext = false;
		bool addtext = true;
		uint16_t textid;
		uint32_t textoff;
		uint32_t textsz;
		GlobalMapCommonDirStruct& dir = theimgmap->common_dir[GLOBAL_MAP_DIR_WORLD_MAP];
		for (i=0;i<dir.file_amount;i++)
			if (id==dir.file_id[i]) {
				hastext = true;
				textid = dir.file_related_id[i];
				off = dir.file_offset[i];
				sz = dir.file_size[i];
				break;
			}
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (hastext) {
			for (i=0;i<amount;i++)
				if (textid==data_id[i] && data_type[i]==CHUNK_TYPE_TEXT) {
					addtext = false;
					break;
				}
			if (addtext)
				for (i=0;i<theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_amount;i++)
					if (textid==theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_id[i]) {
						textoff = theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_offset[i];
						textsz = theimgmap->common_dir[GLOBAL_MAP_DIR_TEXT].file_size[i];
						break;
					}
		}
		addamount = (add ? 1 : 0) + (addtext ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,textid,CHUNK_TYPE_TEXT,0);
		if (addtext)
			res += AddDataSingle(textid,CHUNK_TYPE_TEXT,0,textoff,textsz,0,0,0);
		break;
	}
	case MAP_OBJECT_MODEL: {
		GlobalMapCommonDirStruct* dir = &theimgmap->common_dir[GLOBAL_MAP_DIR_MODEL_PARTY];
		for (i=0;i<dir->file_amount;i++)
			if (id==dir->file_id[i]) {
				off = dir->file_offset[i];
				sz = dir->file_size[i];
				break;
			}
		for (i=0;i<amount;i++)
			if (id==data_id[i] && data_type[i]==chunktype) {
				add = false;
				break;
			}
		if (off==0) {
			dir = &theimgmap->common_dir[GLOBAL_MAP_DIR_MODEL_WEAPON];
			for (i=0;i<dir->file_amount;i++)
				if (id==dir->file_id[i]) {
					off = dir->file_offset[i];
					sz = dir->file_size[i];
					break;
				}
			if (off==0) {
				dir = &theimgmap->common_dir[GLOBAL_MAP_DIR_MODEL_ENEMY];
				for (i=0;i<dir->file_amount;i++)
					if (id==dir->file_id[i]) {
						off = dir->file_offset[i];
						sz = dir->file_size[i];
						break;
					}
			}
		}
		addamount = (add ? 1 : 0);
		if (addamount>maxtoadd)
			return -addamount;
		if (add)
			res += AddDataSingle(id,chunktype,0,off,sz,0,0,0);
	}
	}
	return res;
}

void ImageMapDataStruct::RemoveDataByPos(uint16_t mappos) {
	unsigned int i;
	amount--;
	for (i=mappos;i<amount;i++) {
		data_id[i] = data_id[i+1];
		data_type[i] = data_type[i+1];
		unknown1[i] = unknown1[i+1];
		data_offset[i] = data_offset[i+1];
		data_size[i] = data_size[i+1];
		data_related_id[i] = data_related_id[i+1];
		data_related_type[i] = data_related_type[i+1];
		unknown3[i] = unknown3[i+1];
	}
}

int ImageMapDataStruct::RemoveData(Map_Object kind, uint16_t id) {
	Chunk_Type chunktype = GetChunkTypeFromMapObject(kind);
	int res = 0;
	unsigned int i;
	for (i=0;i<amount;i++)
		if (data_id[i]==id && data_type[i]==chunktype) {
			RemoveDataByPos(i);
			res++;
			i--;
		}
	return res;
}

void ImageMapDataStruct::Copy(ImageMapDataStruct& cpy) {
	if (GetGameType()==GAME_TYPE_PSX) {
		amount = cpy.amount;
		data_id = new uint16_t[amount];
		data_type = new Chunk_Type[amount];
		unknown1 = new uint8_t[amount];
		data_offset = new uint32_t[amount];
		data_size = new uint32_t[amount];
		data_related_id = new uint16_t[amount];
		data_related_type = new Chunk_Type[amount];
		unknown3 = new uint8_t[amount];
		memcpy(data_id,cpy.data_id,amount*sizeof(uint16_t));
		memcpy(data_type,cpy.data_type,amount*sizeof(Chunk_Type));
		memcpy(unknown1,cpy.unknown1,amount*sizeof(uint8_t));
		memcpy(data_offset,cpy.data_offset,amount*sizeof(uint32_t));
		memcpy(data_size,cpy.data_size,amount*sizeof(uint32_t));
		memcpy(data_related_id,cpy.data_related_id,amount*sizeof(uint16_t));
		memcpy(data_related_type,cpy.data_related_type,amount*sizeof(Chunk_Type));
		memcpy(unknown3,cpy.unknown3,amount*sizeof(uint8_t));
	} else {
		unsigned int i,j;
		type_amount = cpy.type_amount;
		data_amount = new unsigned int[type_amount];
		type_name = new string[type_amount];
		data_name = new string*[type_amount];
		memcpy(data_amount,cpy.data_amount,type_amount*sizeof(unsigned int));
		for (i=0;i<type_amount;i++) {
			data_name[i] = new string[data_amount[i]];
			type_name[i] = cpy.type_name[i];
			for (j=0;j<data_amount[i];j++)
				data_name[i][j] = cpy.data_name[i][j];
		}
	}
}

Chunk_Type ImageMapDataStruct::GetChunkTypeFromMapObject(Map_Object kind) {
	switch (kind) {
	case MAP_OBJECT_ENEMY:
		return CHUNK_TYPE_BATTLE_DATA;
	case MAP_OBJECT_SCENE:
		return CHUNK_TYPE_BATTLE_SCENE;
	case MAP_OBJECT_MUSIC:
		return CHUNK_TYPE_SEQUENCER;
	case MAP_OBJECT_AUDIO:
		return CHUNK_TYPE_AUDIO;
	case MAP_OBJECT_FIELD:
	case MAP_OBJECT_WORLD:
		return CHUNK_TYPE_SCRIPT;
	case MAP_OBJECT_MODEL:
		return CHUNK_TYPE_MODEL;
	}
	return 0;
}

#define MACRO_IMAGEMAP_IOFUNCTION(IO,SEEK,READ,PPF) \
	unsigned int i; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,zero); \
	IO ## Short(f,amount); \
	if (READ) { \
		data_id = new uint16_t[amount]; \
		data_type = new Chunk_Type[amount]; \
		unknown1 = new uint8_t[amount]; \
		data_offset = new uint32_t[amount]; \
		data_size = new uint32_t[amount]; \
		data_related_id = new uint16_t[amount]; \
		data_related_type = new Chunk_Type[amount]; \
		unknown3 = new uint8_t[amount]; \
	} \
	for (i=0;i<amount;i++) { \
		IO ## Short(f,data_id[i]); \
		IO ## Char(f,data_type[i]); \
		IO ## Char(f,unknown1[i]); \
		IO ## Long(f,data_offset[i]); \
		IO ## Long(f,data_size[i]); \
		IO ## Short(f,data_related_id[i]); \
		IO ## Char(f,data_related_type[i]); \
		IO ## Char(f,unknown3[i]); \
	} \
	if (PPF) PPFEndScanStep();


void ImageMapDataStruct::ReadSteamFormat(fstream& f) {
	char* buffer = new char[size+1];
	unsigned int i,j,pos,datapos;
	bool typedone;
	f.read(buffer,size);
	buffer[size] = 0;
	type_amount = 0;
	pos = 0;
	while (pos<size)
		if (buffer[pos++]==0x0D)
			if (buffer[pos++]==0x0A)
				type_amount++;
	data_amount = new unsigned int[type_amount];
	type_name = new string[type_amount];
	data_name = new string*[type_amount];
	pos = 0;
	for (i=0;i<type_amount;i++) {
		typedone = false;
		type_name[i] = "";
		while (buffer[pos]!=':' && (buffer[pos]!=0x0D || buffer[pos+1]!=0x0A))
			type_name[i].push_back(buffer[pos++]);
		if (buffer[pos]!=':') {
			data_amount[i] = 0;
			data_name[i] = new string[0];
			continue;
		}
		data_amount[i] = 1;
		datapos = ++pos;
		while (pos<size && !typedone) {
			if (buffer[pos]==',')
				data_amount[i]++;
			else if (buffer[pos]==0x0D && buffer[pos+1]==0x0A) {
				pos++;
				typedone = true;
			}
			pos++;
		}
		data_name[i] = new string[data_amount[i]];
		pos = datapos;
		for (j=0;j<data_amount[i];j++) {
			data_name[i][j] = "";
			while (buffer[pos]!=',' && (buffer[pos]!=0x0D || buffer[pos+1]!=0x0A))
				data_name[i][j].push_back(buffer[pos++]);
			if (buffer[pos]==',')
				pos++;
			else
				pos += 2;
		}
	}
	delete[] buffer;
}

void ImageMapDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_IMAGEMAP_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	} else {
		ReadSteamFormat(f);
	}
	loaded = true;
}

void ImageMapDataStruct::Write(fstream& f) {
	MACRO_IMAGEMAP_IOFUNCTION(FFIXWrite,FFIXSeek,false,false)
	modified = false;
}

void ImageMapDataStruct::WritePPF(fstream& f) {
	MACRO_IMAGEMAP_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true)
}

void ImageMapDataStruct::ReadHWS(fstream& f) {
	if (GetHWSGameType()==GAME_TYPE_PSX) {
		MACRO_IMAGEMAP_IOFUNCTION(HWSRead,HWSSeek,true,false)
	} else {
		ReadSteamFormat(f);
	}
	MarkDataModified();
}

void ImageMapDataStruct::WriteHWS(fstream& f) {
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_IMAGEMAP_IOFUNCTION(HWSWrite,HWSSeek,false,false)
	} else {
		unsigned int i,j,stri;
		for (i=0;i<type_amount;i++) {
			for (stri=0;stri<type_name[i].length();stri++)
				HWSWriteChar(f,type_name[i][stri]);
			HWSWriteChar(f,':');
			for (j=0;j<data_amount[i];j++) {
				for (stri=0;stri<data_name[i][j].length();stri++)
					HWSWriteChar(f,data_name[i][j][stri]);
				if (j+1<data_amount[i]) {
					HWSWriteChar(f,',');
				} else {
					HWSWriteChar(f,0x0D);
					HWSWriteChar(f,0x0A);
				}
			}
		}
	}
}

void ImageMapDataStruct::UpdateOffset() {
	if (GetGameType()==GAME_TYPE_PSX) {
		SetSize(0x4+0x10*amount);
	} else {
		unsigned int i,j;
		uint16_t sz = 0;
		for (i=0;i<type_amount;i++) {
			sz += type_name[i].length()+2;
			for (j=0;j<data_amount[i];j++)
				sz += data_name[i][j].length()+1;
		}
		SetSize(sz);
	}
}

#define MACRO_GLOBAL_MAP_IOFUNCTION(IO,SEEK,READ,PPF) \
	uint32_t headerpos = f.tellg(); \
	unsigned int cam = 0, sam = 0; \
	unsigned int i,j; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Long(f,magic_ff9); \
	IO ## Long(f,unknown1); \
	IO ## Long(f,dir_amount); \
	IO ## Long(f,disc_number); \
	if (READ) { \
		dir_type = new uint32_t[dir_amount]; \
		dir_file_amount = new uint32_t[dir_amount]; \
		dir_offset = new uint32_t[dir_amount]; \
		dir_first_file_offset = new uint32_t[dir_amount]; \
	} \
	for (i=0;i<dir_amount;i++) { \
		IO ## Long(f,dir_type[i]); \
		IO ## Long(f,dir_file_amount[i]); \
		IO ## Long(f,dir_offset[i]); \
		IO ## Long(f,dir_first_file_offset[i]); \
		if (dir_type[i]==2) cam++; \
		if (dir_type[i]==3) sam++; \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) { \
		common_dir = new GlobalMapCommonDirStruct[cam]; \
		spell_dir = new GlobalMapSpellDirStruct[sam]; \
		j = 0; \
		for (i=0;i<cam;i++) { \
			common_dir[i].file_amount = dir_file_amount[j]; \
			common_dir[i].offset = dir_offset[j]; \
			common_dir[i].first_file_offset = dir_first_file_offset[j]; \
			common_dir[i].file_id = new uint16_t[dir_file_amount[j]]; \
			common_dir[i].file_related_id = new uint16_t[dir_file_amount[j]]; \
			common_dir[i].file_offset = new uint32_t[dir_file_amount[j]]; \
			common_dir[i].file_size = new uint32_t[dir_file_amount[j]]; \
			j++; \
		} \
		for (i=0;i<sam;i++) { \
			spell_dir[i].file_amount = dir_file_amount[j]; \
			spell_dir[i].offset = dir_offset[j]; \
			spell_dir[i].first_file_offset = dir_first_file_offset[j]; \
			spell_dir[i].file_offset = new uint16_t[dir_file_amount[j]]; \
			j++; \
		} \
	} \
	for (i=0;i<cam;i++) { \
		SEEK(f,headerpos,common_dir[i].offset*(FILE_IGNORE_DATA_PERIOD-FILE_IGNORE_DATA_AMOUNT)); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<dir_file_amount[i];j++) { \
			IO ## Short(f,common_dir[i].file_id[j]); \
			IO ## Short(f,common_dir[i].file_related_id[j]); \
			IO ## Long(f,common_dir[i].file_offset[j]); \
		} \
		if (PPF) PPFEndScanStep(); \
	} \
	for (i=0;i<sam;i++) { \
		SEEK(f,headerpos,spell_dir[i].offset*(FILE_IGNORE_DATA_PERIOD-FILE_IGNORE_DATA_AMOUNT)); \
		if (PPF) PPFInitScanStep(f); \
		for (j=0;j<spell_dir[i].file_amount;j++) { \
			IO ## Short(f,spell_dir[i].file_offset[j]); \
		} \
		if (PPF) PPFEndScanStep(); \
	}

void GlobalImageMapStruct::Read(fstream& f) {
	f.seekg(IMG_HEADER_OFFSET);
	MACRO_GLOBAL_MAP_IOFUNCTION(FFIXRead,FFIXSeek,true,false)
	for (i=0;i<cam;i++) {
		for (j=0;j<dir_file_amount[i]-1;j++)
			common_dir[i].file_size[j] = common_dir[i].file_offset[j+1]-common_dir[i].file_offset[j];
		common_dir[i].file_size[j] = dir_first_file_offset[i+1]-common_dir[i].file_offset[j];
	}
}
