#ifndef _MODELMESH_H
#define _MODELMESH_H

struct ModelMeshVertex;
struct ModelMeshMaterialInfo;
struct ModelMeshData;
struct ModelMaterialData;

#include <vector>
#include "File_IOCommon.h"
using namespace std;

struct ModelMeshVertex {
	// Position
	float x;
	float y;
	float z;
	// Normal
	float nx;
	float ny;
	float nz;
	// Texture UV
	float u;
	float v;
};

struct ModelMeshMaterialInfo {
	uint32_t vert_list_start;
	uint32_t vert_list_amount;
	uint32_t unk0; // mostly 0
	uint32_t vert_start;
	uint32_t vert_amount;
	float unk1; // ToDo: bounding box?
	float unk2;
	float unk3;
	float unk4;
	float unk5;
	float unk6;
};

struct ModelMeshData {
	uint32_t material_info_amount;
	uint32_t vertice_amount;
	uint32_t vertex_list_amount;
	vector<ModelMeshMaterialInfo> mat_info;
	vector<ModelMeshVertex> vert;
	vector<uint16_t> vert_list;
	
	uint32_t vert_format1;
	uint32_t vert_format2;
	uint32_t vert_format3;
	uint32_t vert_format4;
	uint32_t vert_format5;
	uint32_t vert_format6;
	uint32_t vert_format7;
	uint32_t vert_format8;
	uint32_t vert_format9;
	
	void Read(fstream& f);
	void Export(fstream& output, const char* objname, const char* mtlbasename, bool firstobject = true);
};

struct ModelMaterialData {
	uint32_t bumpmap_data[7];
	uint32_t detailalbedomap_data[7];
	uint32_t detailmask_data[7];
	uint32_t detailnormalmap_data[7];
	uint32_t emissionmap_data[7];
	uint32_t maintex_unk1;
	uint64_t maintex_file_info = 0;
	float maintex_u = 0.0;
	float maintex_v = 0.0;
	uint32_t maintex_unk2;
	uint32_t maintex_unk3;
	uint32_t metallicglossmap_data[7];
	uint32_t occlusionmap_data[7];
	uint32_t parallaxmap_data[8];
	float bumpscale_value = 1.0;
	float cutoff_value;
	float detailnormalmapscale_value = 1.0;
	float dstblend_value = 0.0;
	float glossiness_value = 0.0;
	float metallic_value = 0.0;
	float mode_value = 0.0;
	float occlusionstrength_value = 1.0;
	float parallax_value = 0.02;
	float srcblend_value = 1.0;
	float uvsec_value = 0.0;
	float zwrite_factor = 1.0;
	uint32_t zwrite_mode;
	float color_red = 1.0;
	float color_green = 1.0;
	float color_blue = 1.0;
	float color_alpha = 1.0;
	float emissioncolor_red = 0.0;
	float emissioncolor_green = 0.0;
	float emissioncolor_blue = 0.0;
	float emissioncolor_alpha = 1.0;
	
	void Read(fstream& f);
	void Export(fstream& output, const char* mtlname, const char* maintexname);
};

#endif
