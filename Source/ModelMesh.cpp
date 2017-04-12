#include "ModelMesh.h"

#include <string.h>

static unsigned int vert_count_origin;
static unsigned int tvert_count_origin;

void ModelMeshData::Read(fstream& f) {
	unsigned int i;
	uint32_t tmp32;
	size_t headerpos = f.tellg();
	material_info_amount = ReadLong(f);
	mat_info.reserve(material_info_amount);
	ModelMeshMaterialInfo matinfobuffer;
	for (i=0;i<material_info_amount;i++) {
		matinfobuffer.vert_list_start = ReadLong(f);
		matinfobuffer.vert_list_amount = ReadLong(f);
		matinfobuffer.unk0 = ReadLong(f);
		matinfobuffer.vert_start = ReadLong(f);
		matinfobuffer.vert_amount = ReadLong(f);
		matinfobuffer.unk1 = ReadFloat(f);
		matinfobuffer.unk2 = ReadFloat(f);
		matinfobuffer.unk3 = ReadFloat(f);
		matinfobuffer.unk4 = ReadFloat(f);
		matinfobuffer.unk5 = ReadFloat(f);
		matinfobuffer.unk6 = ReadFloat(f);
		mat_info.push_back(matinfobuffer);
	}
	f.seekg(0x10,ios::cur);
	unsigned int numunk1 = ReadLong(f);
	f.seekg(numunk1*0x40,ios::cur);
	unsigned int numunk2 = ReadLong(f);
	f.seekg(numunk2*0x4+8,ios::cur);
	unsigned int sizevertlist = ReadLong(f);
	vertex_list_amount = sizevertlist/2;
	vert_list.reserve(vertex_list_amount);
	for (i=0;i<vertex_list_amount;i++)
		vert_list.push_back(ReadShort(f));
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	unsigned int numunk3 = ReadLong(f); // ==vertice_amount ?
	f.seekg(numunk3*0x20+4,ios::cur);
	vertice_amount = ReadLong(f);
	vert_format1 = ReadLong(f);
	vert_format2 = ReadLong(f);
	vert_format3 = ReadLong(f);
	vert_format4 = ReadLong(f);
	vert_format5 = ReadLong(f);
	vert_format6 = ReadLong(f);
	vert_format7 = ReadLong(f);
	vert_format8 = ReadLong(f);
	vert_format9 = ReadLong(f);
	unsigned int sizevert = ReadLong(f);
	size_t vertpos = f.tellg();
	vert.reserve(vertice_amount);
	ModelMeshVertex vertbuffer;
	for (i=0;i<vertice_amount;i++) {
		vertbuffer.x = ReadFloat(f);
		vertbuffer.y = ReadFloat(f);
		vertbuffer.z = ReadFloat(f);
		vertbuffer.nx = ReadFloat(f);
		vertbuffer.ny = ReadFloat(f);
		vertbuffer.nz = ReadFloat(f);
		if (vert_format9==0x4002000) { // DEBUG
			vertbuffer.u = ReadFloat(f);
			vertbuffer.v = ReadFloat(f);
		}
		tmp32 = ReadLong(f);
		tmp32 = ReadLong(f);
		tmp32 = ReadLong(f);
		tmp32 = ReadLong(f);
		vert.push_back(vertbuffer);
	}
	if (vert_format9!=0x4002000) {
		if (vert_format6==0x2000801) // DEBUG
			f.seekg(vertpos+sizevert-vertice_amount*0x10);
		else
			f.seekg(vertpos+sizevert-vertice_amount*0x8);
		for (i=0;i<vertice_amount;i++) {
			vert[i].u = ReadFloat(f);
			vert[i].v = ReadFloat(f);
			if (vert_format6==0x2000801)
				f.seekg(0x8,ios::cur);
		}
	}
}

void ModelMeshData::Export(fstream& output, const char* objname, const char* mtlbasename, bool firstobject) {
	unsigned int i,j;
	if (firstobject) {
		vert_count_origin = 1;
		tvert_count_origin = 1;
	}
	output << "o " << objname << endl;
	output << std::showpoint;
	for (i=0;i<vert.size();i++)
		output << "v " << vert[i].x << " " << vert[i].y << " " << vert[i].z << endl;
	for (i=0;i<vert.size();i++)
		output << "vt " << vert[i].u << " " << vert[i].v << endl;
	for (i=0;i<vert.size();i++)
		output << "vn " << vert[i].nx << " " << vert[i].ny << " " << vert[i].nz << endl;
	int mtlidcur = -1, mtlidnew = -1;
	for (i=0;i<vert_list.size();) {
		for (j=0;j<mat_info.size();j++)
			if (i>=mat_info[j].vert_list_start/2 && i<mat_info[j].vert_list_start/2+mat_info[j].vert_list_amount) {
				mtlidnew = j;
				break;
			}
		if (mtlidnew!=mtlidcur) {
			mtlidcur = mtlidnew;
			output << "usemtl " << mtlbasename << mtlidcur << endl;
		}
		output << "f " << (unsigned int)vert_list[i]+vert_count_origin << "/" << (unsigned int)vert_list[i]+tvert_count_origin; i++;
		output << " " << (unsigned int)vert_list[i]+vert_count_origin << "/" << (unsigned int)vert_list[i]+tvert_count_origin; i++;
		output << " " << (unsigned int)vert_list[i]+vert_count_origin << "/" << (unsigned int)vert_list[i]+tvert_count_origin << endl; i++;
	}
	output << endl;
	vert_count_origin += vertice_amount;
	tvert_count_origin += vertice_amount;
}

void ModelMaterialData::Read(fstream& f) {
	unsigned int i;
	uint32_t tmp32;
	uint32_t fieldlen;
	char fieldbuffer[0x20];
	size_t headerpos = f.tellg();
	f.seekg(headerpos+0x20);
	fieldlen = ReadLong(f);
	while (fieldlen<0x20 && !f.eof()) {
		f.read(fieldbuffer,fieldlen);
		fieldbuffer[fieldlen] = 0;
		f.seekg(GetAlignOffset(f.tellg()),ios::cur);
		if (strcmp(fieldbuffer,"_BumpMap")==0) {
			for (i=0;i<7;i++)
				bumpmap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_DetailAlbedoMap")==0) {
			for (i=0;i<7;i++)
				detailalbedomap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_DetailMask")==0) {
			for (i=0;i<7;i++)
				detailmask_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_DetailNormalMap")==0) {
			for (i=0;i<7;i++)
				detailnormalmap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_EmissionMap")==0) {
			for (i=0;i<7;i++)
				emissionmap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_MainTex")==0) {
			maintex_unk1 = ReadLong(f);
			maintex_file_info = ReadLongLong(f);
			maintex_u = ReadFloat(f);
			maintex_v = ReadFloat(f);
			maintex_unk2 = ReadLong(f);
			maintex_unk3 = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_MetallicGlossMap")==0) {
			for (i=0;i<7;i++)
				metallicglossmap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_OcclusionMap")==0) {
			for (i=0;i<7;i++)
				occlusionmap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_ParallaxMap")==0) {
			for (i=0;i<8;i++)
				parallaxmap_data[i] = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_BumpScale")==0) {
			bumpscale_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_Cutoff")==0) {
			cutoff_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_DetailNormalMapScale")==0) {
			detailnormalmapscale_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_DstBlend")==0) {
			dstblend_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_Glossiness")==0) {
			glossiness_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_Metallic")==0) {
			metallic_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_Mode")==0) {
			mode_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_OcclusionStrength")==0) {
			occlusionstrength_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_Parallax")==0) {
			parallax_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_SrcBlend")==0) {
			srcblend_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_UVSec")==0) {
			uvsec_value = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_ZWrite")==0) {
			zwrite_factor = ReadFloat(f);
			zwrite_mode = ReadLong(f);
		} else if (strcmp(fieldbuffer,"_Color")==0) {
			color_red = ReadFloat(f);
			color_green = ReadFloat(f);
			color_blue = ReadFloat(f);
			color_alpha = ReadFloat(f);
		} else if (strcmp(fieldbuffer,"_EmissionColor")==0) {
			emissioncolor_red = ReadFloat(f);
			emissioncolor_green = ReadFloat(f);
			emissioncolor_blue = ReadFloat(f);
			emissioncolor_alpha = ReadFloat(f);
		} else {
			break;
		}
		fieldlen = ReadLong(f);
	}
}

void ModelMaterialData::Export(fstream& output, const char* mtlname, const char* maintexname) {
	output << "newmtl " << mtlname << endl;
	output << std::showpoint;
	output << "Ka 1.000 1.000 1.000" << endl << "Kd 1.000 1.000 1.000" << endl << "Ks 0.000 0.000 0.000" << endl;
	output << "d 1.0" << endl << "illum 0" << endl;
	output << "map_Kd ";
//	if (maintex_u>0.0 || maintex_v>0.0)
//		output << "-o " << maintex_u << " " << maintex_v << " 0.000 ";
	output << maintexname << endl << endl;
}


