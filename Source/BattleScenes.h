#ifndef _BATTLESCENES_H
#define _BATTLESCENES_H

struct BattleSceneDataStruct;
struct BattleSceneDataSet;

#include <inttypes.h>
#include <fstream>
#include "Configuration.h"
using namespace std;

struct BattleSceneDataStruct : public ChunkChild {
public:
	uint16_t total_size;
	uint16_t face_amount;
	uint16_t obj_amount;
	uint16_t obj_offset;
	uint16_t tim_amount;
	uint16_t tim_offset;
	uint16_t vert_amount;
	uint16_t vert_offset;
	uint16_t tex_amount;
	uint16_t tex_offset;
	uint16_t face_offset;
	uint16_t tvert_offset;
	uint16_t* obj_tim_index;
	uint16_t* obj_vert_amount;
	uint16_t* obj_vert_index;
	uint16_t* obj_tex_offset;
	uint16_t* obj_face_offset;
	uint16_t* obj_tvert_offset;
	uint16_t* obj_quad_amount;
	uint16_t* obj_trgl_amount;
	uint8_t** obj_tex_unknown1;
	uint8_t** obj_tex_unknown2;
	uint8_t** obj_tex_unknown3;
	uint8_t** obj_tex_id;
	uint16_t** obj_face_quadp;
	uint16_t** obj_face_trglp;
	uint8_t** obj_tvert_quadx;
	uint8_t** obj_tvert_quady;
	uint8_t** obj_tvert_trglx;
	uint8_t** obj_tvert_trgly;
	uint16_t* tim_texpos;
	uint16_t* tim_palpos;
	uint16_t* vert_x;
	uint16_t* vert_y;
	uint16_t* vert_z;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void UpdateOffset();
	int Export(const char* outputbase, bool usequads);
	// numtex and texname are outputs (unchanged if the log has errors)
	LogStruct Import(const char* inputfile, unsigned int* numtex, wstring** texname);

private:
	BattleSceneDataSet* parent;
	uint16_t id;
	
	friend BattleSceneDataSet;
};

struct BattleSceneDataSet {
public:
	uint16_t scene_amount;
	vector<uint16_t> cluster_id;
	vector<wstring> scene_name;
	vector<BattleSceneDataStruct*> scene;
	vector<TIMImageDataStruct*> image;
	
	void Load(fstream &ffbin, ClusterSet& clusset);
	void Write(fstream &ffbin, ClusterSet& clusset);
	void WritePPF(fstream &ffbin, ClusterSet& clusset);
	// return value is int[3] to be deleted
	// {Number of oversized scenes, Number of unknown data, Number of unused scenes}
	int* LoadHWS(fstream &ffhws, UnusedSaveBackupPart& backup);
	void WriteHWS(fstream &ffhws, UnusedSaveBackupPart& backup);
	int GetIndexById(uint16_t sceneid);
};

#endif
