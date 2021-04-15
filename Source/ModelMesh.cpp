#include "ModelMesh.h"

#include <algorithm>
#include "Database_Animation.h"
#include "fbxsdk.h"

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager);
void SetupAxisSystem(FbxScene*& sdkscene);
bool SaveScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene, int format = MODEL_FILE_FORMAT_FBX_ASCII, bool pEmbedMedia = false);
bool LoadScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene, bool retrieveanims = true);
bool ConvertModelToFBX(ModelDataStruct& model, FbxManager*& sdkmanager, FbxScene*& sdkscene);
bool ConvertFBXToModel(ModelDataStruct& model, FbxManager*& sdkmanager, FbxScene*& sdkscene, bool retrieveanims = true);

inline FbxString RemoveFbxStringExtension(FbxString value) {
	int dotpos = value.ReverseFind('.');
	if (dotpos>=0)
		value = value.Left(dotpos);
	return value;
}

//=============================//
//     Model Mesh Methods      //
//=============================//

static unsigned int vert_count_origin;
static unsigned int tvert_count_origin;

void ModelMeshData::Read(fstream& f) {
	ModelMeshVertexAttachment vertattachbuffer;
	ModelMeshMaterialInfo matinfobuffer;
	ModelMeshVertex vertbuffer;
	ModelMeshBone bonebuffer;
	unsigned int i,j,k;
	material_info_amount = ReadLong(f);
//fstream fout("aaaa.txt",ios::out|ios::app); fout << "material_info_amount: " << (unsigned int)material_info_amount << endl; fout.close();
	mat_info.reserve(material_info_amount);
	for (i=0;i<material_info_amount;i++) {
		matinfobuffer.vert_list_start = ReadLong(f);
		matinfobuffer.vert_list_amount = ReadLong(f);
		matinfobuffer.unk0 = ReadLong(f);
		matinfobuffer.vert_start = ReadLong(f);
		matinfobuffer.vert_amount = ReadLong(f);
		ModelDataStruct::ReadCoordinates(f,matinfobuffer.center_x,matinfobuffer.center_y,matinfobuffer.center_z);
		ModelDataStruct::ReadCoordinates(f,matinfobuffer.radius_x,matinfobuffer.radius_y,matinfobuffer.radius_z,false);
		mat_info.push_back(matinfobuffer);
	}
	f.seekg(0x10,ios::cur);
	bone_amount = ReadLong(f);
//fout.open("aaaa.txt",ios::out|ios::app); fout << "bone_amount: " << (unsigned int)bone_amount << endl; fout.close();
	bone.reserve(bone_amount);
	for (i=0;i<bone_amount;i++) {
		for (j=0;j<4;j++)
			for (k=0;k<4;k++)
				bonebuffer.transform_matrix.value[j][k] = (k==3 ? -ReadFloat(f) : ReadFloat(f));
		bone.push_back(bonebuffer);
	}
	bone_unk_amount = ReadLong(f);
//fout.open("aaaa.txt",ios::out|ios::app); fout << "bone_unk_amount: " << (unsigned int)bone_unk_amount << endl; fout.close();
	bone_unk_list.reserve(bone_unk_amount);
	for (i=0;i<bone_unk_amount;i++)
		bone_unk_list.push_back(ReadLong(f));
	bone_unk_main = ReadLong(f);
	unk_flag1 = f.get();
	unk_flag2 = f.get();
	unk_flag3 = f.get();
	unk_flag4 = f.get();
	unsigned int sizevertlist = ReadLong(f);
	vertex_list_amount = sizevertlist/2;
//fout.open("aaaa.txt",ios::out|ios::app); fout << "vertex_list_amount: " << (unsigned int)vertex_list_amount << endl; fout.close();
	vert_list.reserve(vertex_list_amount);
	for (i=0;i<vertex_list_amount;i++)
		vert_list.push_back(ReadShort(f));
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	vertice_attachment_amount = ReadLong(f);
//fout.open("aaaa.txt",ios::out|ios::app); fout << "vertice_attachment_amount: " << (unsigned int)vertice_attachment_amount << endl; fout.close();
	vert_attachment.reserve(vertice_attachment_amount);
	for (i=0;i<vertice_attachment_amount;i++) {
		vertattachbuffer.bone_factor[0] = ReadFloat(f);
		vertattachbuffer.bone_factor[1] = ReadFloat(f);
		vertattachbuffer.bone_factor[2] = ReadFloat(f);
		vertattachbuffer.bone_factor[3] = ReadFloat(f);
		vertattachbuffer.bone_id[0] = ReadLong(f);
		vertattachbuffer.bone_id[1] = ReadLong(f);
		vertattachbuffer.bone_id[2] = ReadLong(f);
		vertattachbuffer.bone_id[3] = ReadLong(f);
		vert_attachment.push_back(vertattachbuffer);
	}
	vert_format0 = ReadLong(f);
	vertice_amount = ReadLong(f);
//fout.open("aaaa.txt",ios::out|ios::app); fout << "vertice_attachment_amount: " << (unsigned int)vertice_amount << endl; fout.close();
	vert_format1 = ReadLong(f);
	vert_format2 = ReadLong(f);
	vert_format3 = ReadLong(f);
	vert_format4 = ReadLong(f);
	vert_format5 = ReadLong(f);
	vert_format6 = ReadLong(f);
	vert_format7 = ReadLong(f);
	vert_format8 = ReadLong(f);
	vert_format9 = ReadLong(f);
	/* Examples of format0-9:
	p0data4/1/133 (Moogle Book): 8B ... 8 3000000 3000C00 0 2000001 0       0 0 4001800
	p0data4/5/667 (Kuja Burned): 9B ... 8 3000000 3000C00 0 2000001 2000801 0 0 4001800
	worldmap + battle models   : 8B ... 8 3000000 3000C00 0 2001800 0       0 0 4002000
	*/
	vert_datasize = ReadLong(f);
	size_t vertpos = f.tellg();
	vert.reserve(vertice_amount);
	for (i=0;i<vertice_amount;i++) {
		ModelDataStruct::ReadCoordinates(f,vertbuffer.x,vertbuffer.y,vertbuffer.z);
		ModelDataStruct::ReadCoordinates(f,vertbuffer.nx,vertbuffer.ny,vertbuffer.nz);
		if (vert_format9==0x4002000) { // DEBUG
			vertbuffer.u = ReadFloat(f);
			vertbuffer.v = ReadFloat(f);
		}
		ModelDataStruct::ReadCoordinates(f,vertbuffer.tx,vertbuffer.ty,vertbuffer.tz);
		vertbuffer.unkf = ReadFloat(f);
		vert.push_back(vertbuffer);
	}
	if (vert_format9!=0x4002000) {
		if (vert_format6==0x2000801) // DEBUG
			f.seekg(vertpos+vert_datasize-vertice_amount*0x10);
		else
			f.seekg(vertpos+vert_datasize-vertice_amount*0x8);
		for (i=0;i<vertice_amount;i++) {
			vert[i].u = ReadFloat(f);
			vert[i].v = ReadFloat(f);
			if (vert_format6==0x2000801) {
				vert[i].unkuv1 = ReadFloat(f);
				vert[i].unkuv2 = ReadFloat(f);
			}
		}
	}
	f.seekg(vertpos+vert_datasize+0xA4);
	ModelDataStruct::ReadCoordinates(f,center_x,center_y,center_z);
	ModelDataStruct::ReadCoordinates(f,radius_x,radius_y,radius_z,false);
	unk_num1 = ReadLong(f);
	unk_num2 = ReadLong(f);
	unk_num3 = ReadLong(f);
//fout.open("aaaa.txt",ios::out|ios::app); fout << "READ DONE" << endl; fout.close();
}

void ModelMeshData::Write(fstream & f) {
	unsigned int i,j,k;
	WriteLong(f,material_info_amount);
	for (i=0;i<material_info_amount;i++) {
		WriteLong(f,mat_info[i].vert_list_start);
		WriteLong(f,mat_info[i].vert_list_amount);
		WriteLong(f,mat_info[i].unk0);
		WriteLong(f,mat_info[i].vert_start);
		WriteLong(f,mat_info[i].vert_amount);
		ModelDataStruct::WriteCoordinates(f,mat_info[i].center_x,mat_info[i].center_y,mat_info[i].center_z);
		ModelDataStruct::WriteCoordinates(f,mat_info[i].radius_x,mat_info[i].radius_y,mat_info[i].radius_z,false);
	}
	for (i=0;i<0x10;i++)
		f.put(0);
	WriteLong(f,bone_amount);
	for (i=0;i<bone_amount;i++) {
		for (j=0;j<4;j++) {
			for (k=0;k<3;k++)
				WriteFloat(f,bone[i].transform_matrix.value[j][k]);
			WriteFloat(f,-bone[i].transform_matrix.value[j][3]);
		}
	}
	WriteLong(f,bone_unk_amount);
	for (i=0;i<bone_unk_amount;i++)
		WriteLong(f,bone_unk_list[i]);
	WriteLong(f,bone_unk_main);
	f.put(unk_flag1);
	f.put(unk_flag2);
	f.put(unk_flag3);
	f.put(unk_flag4);
	unsigned int sizevertlist = vertex_list_amount*2;
	WriteLong(f,sizevertlist);
	for (i=0;i<vertex_list_amount;i++)
		WriteShort(f,vert_list[i]);
	while (f.tellg()%4!=0)
		f.put(0);
	WriteLong(f,vertice_attachment_amount);
	for (i=0;i<vertice_attachment_amount;i++) {
		WriteFloat(f,vert_attachment[i].bone_factor[0]);
		WriteFloat(f,vert_attachment[i].bone_factor[1]);
		WriteFloat(f,vert_attachment[i].bone_factor[2]);
		WriteFloat(f,vert_attachment[i].bone_factor[3]);
		WriteLong(f,vert_attachment[i].bone_id[0]);
		WriteLong(f,vert_attachment[i].bone_id[1]);
		WriteLong(f,vert_attachment[i].bone_id[2]);
		WriteLong(f,vert_attachment[i].bone_id[3]);
	}
	WriteLong(f,vert_format0);
	WriteLong(f,vertice_amount);
	WriteLong(f,vert_format1);
	WriteLong(f,vert_format2);
	WriteLong(f,vert_format3);
	WriteLong(f,vert_format4);
	WriteLong(f,vert_format5);
	WriteLong(f,vert_format6);
	WriteLong(f,vert_format7);
	WriteLong(f,vert_format8);
	WriteLong(f,vert_format9);
	WriteLong(f,vert_datasize);
	size_t vertposalign = f.tellg()%0x10;
	for (i=0;i<vertice_amount;i++) {
		ModelDataStruct::WriteCoordinates(f,vert[i].x,vert[i].y,vert[i].z);
		ModelDataStruct::WriteCoordinates(f,vert[i].nx,vert[i].ny,vert[i].nz);
		if (vert_format9==0x4002000) {
			WriteFloat(f,vert[i].u);
			WriteFloat(f,vert[i].v);
		}
		ModelDataStruct::WriteCoordinates(f,vert[i].tx,vert[i].ty,vert[i].tz);
		WriteFloat(f,vert[i].unkf);
	}
	while (f.tellg()%0x10!=vertposalign)
		f.put(0);
	if (vert_format9!=0x4002000) {
		for (i=0;i<vertice_amount;i++) {
			WriteFloat(f,vert[i].u);
			WriteFloat(f,vert[i].v);
			if (vert_format6==0x2000801) {
				WriteFloat(f,vert[i].unkuv1);
				WriteFloat(f,vert[i].unkuv2);
			}
		}
	}
	for (i=0;i<0xA4;i++)
		f.put(0);
	ModelDataStruct::WriteCoordinates(f,center_x,center_y,center_z);
	ModelDataStruct::WriteCoordinates(f,radius_x,radius_y,radius_z,false);
	WriteLong(f,unk_num1);
	WriteLong(f,unk_num2);
	WriteLong(f,unk_num3);
}

int ModelMeshData::GetDataSize() {
	return 292+44*material_info_amount+64*bone_amount+4*bone_unk_amount+32*vertice_attachment_amount
		+2*vertex_list_amount+2*(vertex_list_amount%2)
		+(vert_format9==0x4002000 ? 48*vertice_amount :
			40*vertice_amount+8*(vertice_amount%2)
			+(vert_format6==0x2000801 ? 16*vertice_amount : 8*vertice_amount));
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

void ModelMaterialData::Read(fstream& f, UnityArchiveMetaData& metadata) {
	uint32_t fieldlen;
	char fieldbuffer[0x20];
	size_t headerpos = f.tellg();
	shader_unk = ReadLong(f);
	shader_info = ReadLongLong(f);
	fieldlen = ReadLong(f);
	if (fieldlen==0) {
		extheader_value1 = ReadLong(f); // DEBUG: save that and optional fields in FBX
		extheader_value2 = ReadLong(f);
		extheader_value3 = ReadLong(f);
		extheader_value4 = ReadLong(f);
		fieldlen = ReadLong(f);
		has_extheader = true;
	} else {
		has_extheader = false;
	}
	while (fieldlen<0x20 && !f.eof()) {
		f.read(fieldbuffer,fieldlen);
		fieldbuffer[fieldlen] = 0;
		f.seekg(GetAlignOffset(f.tellg()),ios::cur);

		#define MACRO_MATERIAL_READFILE(MAP) \
			MAP.unk1 = ReadLong(f); \
			MAP.file_info = ReadLongLong(f); \
			MAP.u = ReadFloat(f); \
			MAP.v = ReadFloat(f); \
			MAP.unk2 = ReadLong(f); \
			MAP.unk3 = ReadLong(f); \
			if (MAP.file_info==0) { \
				MAP.file_name = ""; \
			} else { \
				int32_t texfileid = metadata.GetFileIndexByInfo(MAP.file_info); \
				MAP.file_name = (texfileid>=0 ? metadata.file_name[texfileid]+".png" : "TextureNotFound"); \
			}

		if (strcmp(fieldbuffer,"_BumpMap")==0) {
			MACRO_MATERIAL_READFILE(bumpmap)
		} else if (strcmp(fieldbuffer,"_DetailAlbedoMap")==0) {
			MACRO_MATERIAL_READFILE(detailalbedomap)
		} else if (strcmp(fieldbuffer,"_DetailMask")==0) {
			MACRO_MATERIAL_READFILE(detailmask)
		} else if (strcmp(fieldbuffer,"_DetailNormalMap")==0) {
			MACRO_MATERIAL_READFILE(detailnormalmap)
		} else if (strcmp(fieldbuffer,"_EmissionMap")==0) {
			MACRO_MATERIAL_READFILE(emissionmap)
		} else if (strcmp(fieldbuffer,"_MainTex")==0) {
			MACRO_MATERIAL_READFILE(maintex)
		} else if (strcmp(fieldbuffer,"_MetallicGlossMap")==0) {
			MACRO_MATERIAL_READFILE(metallicglossmap)
		} else if (strcmp(fieldbuffer,"_OcclusionMap")==0) {
			MACRO_MATERIAL_READFILE(occlusionmap)
		} else if (strcmp(fieldbuffer,"_ParallaxMap")==0) {
			MACRO_MATERIAL_READFILE(parallaxmap)
			parallaxmap_unk4 = ReadLong(f);
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
		} else if (strcmp(fieldbuffer,"_ALPHAPREMULTIPLY_ON")==0) {
			alphapremultiply_on_value1 = ReadLong(f);
			alphapremultiply_on_value2 = ReadLong(f);
			alphapremultiply_on_value3 = ReadLong(f);
			has_alphapremultiply_on = true;
		} else if (strcmp(fieldbuffer,"RenderType")==0) {
			rendertype_value = ReadLong(f);
			has_rendertype = true;
		} else if (strcmp(fieldbuffer,"Transparent")==0) {
			transparent_value = ReadLong(f);
			has_transparent = true;
		} else {
			break;
		}
		fieldlen = ReadLong(f);
	}
}

void ModelMaterialData::Write(fstream& f, UnityArchiveMetaData& metadata) {
	string fieldbuffer;
	WriteLong(f,shader_unk);
	WriteLongLong(f,shader_info);
	if (has_extheader) {
		WriteLong(f,0);
		WriteLong(f,extheader_value1);
		WriteLong(f,extheader_value2);
		WriteLong(f,extheader_value3);
		WriteLong(f,extheader_value4);
	}
	if (has_alphapremultiply_on) {
		fieldbuffer = "_ALPHAPREMULTIPLY_ON";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
		while (f.tellg()%4!=0) f.put(0);
		WriteLong(f,alphapremultiply_on_value1);
		WriteLong(f,alphapremultiply_on_value2);
		WriteLong(f,alphapremultiply_on_value3);
	}
	if (has_rendertype) {
		fieldbuffer = "RenderType";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
		while (f.tellg()%4!=0) f.put(0);
		WriteLong(f,rendertype_value);
	}
	if (has_transparent) {
		fieldbuffer = "Transparent";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
		while (f.tellg()%4!=0) f.put(0);
		WriteLong(f,transparent_value);
	}

	#define MACRO_MATERIAL_WRITEFILE(MAP) \
		WriteLong(f,MAP.unk1); \
		WriteLongLong(f,MAP.file_info); \
		WriteFloat(f,MAP.u); \
		WriteFloat(f,MAP.v); \
		WriteLong(f,MAP.unk2); \
		WriteLong(f,MAP.unk3);

	fieldbuffer = "_BumpMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(bumpmap)
	fieldbuffer = "_DetailAlbedoMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(detailalbedomap)
	fieldbuffer = "_DetailMask";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(detailmask)
	fieldbuffer = "_DetailNormalMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(detailnormalmap)
	fieldbuffer = "_EmissionMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(emissionmap)
	fieldbuffer = "_MainTex";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(maintex)
	fieldbuffer = "_MetallicGlossMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(metallicglossmap)
	fieldbuffer = "_OcclusionMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(occlusionmap)
	fieldbuffer = "_ParallaxMap";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	MACRO_MATERIAL_WRITEFILE(parallaxmap)
	WriteLong(f,parallaxmap_unk4);
	fieldbuffer = "_BumpScale";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,bumpscale_value);
	fieldbuffer = "_Cutoff";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,cutoff_value);
	fieldbuffer = "_DetailNormalMapScale";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,detailnormalmapscale_value);
	fieldbuffer = "_DstBlend";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,dstblend_value);
	fieldbuffer = "_Glossiness";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,glossiness_value);
	fieldbuffer = "_Metallic";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,metallic_value);
	fieldbuffer = "_Mode";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,mode_value);
	fieldbuffer = "_OcclusionStrength";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,occlusionstrength_value);
	fieldbuffer = "_Parallax";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,parallax_value);
	fieldbuffer = "_SrcBlend";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,srcblend_value);
	fieldbuffer = "_UVSec";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,uvsec_value);
	fieldbuffer = "_ZWrite";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,zwrite_factor);
	WriteLong(f,zwrite_mode);
	fieldbuffer = "_Color";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,color_red);
	WriteFloat(f,color_green);
	WriteFloat(f,color_blue);
	WriteFloat(f,color_alpha);
	fieldbuffer = "_EmissionColor";	WriteLong(f,fieldbuffer.size());	f.write(fieldbuffer.c_str(),fieldbuffer.size());
	while (f.tellg()%4!=0) f.put(0);
	WriteFloat(f,emissioncolor_red);
	WriteFloat(f,emissioncolor_green);
	WriteFloat(f,emissioncolor_blue);
	WriteFloat(f,emissioncolor_alpha);
}

int ModelMaterialData::GetDataSize() {
	return 0x2E0 // Header + fields that are always there
		+(has_extheader ? 0x14 : 0)
		+(has_alphapremultiply_on ? 0x24 : 0)+(has_rendertype ? 0x10 : 0)+(has_transparent ? 0x14 : 0);
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

void ModelAnimationData::Read(fstream& f, GameObjectHierarchy* gohier) {
	unsigned int i,j;
	name_len = ReadLong(f);
	name = "";
	for (i=0;i<name_len;i++)
		name.push_back(f.get());
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	num_unk1 = ReadShort(f);
	num_unk2 = ReadShort(f);

	#define MACRO_READTRANSFORM_BEGIN(LOCALTYPE) \
		LOCALTYPE ## _amount = ReadLong(f); \
		LOCALTYPE.resize(LOCALTYPE ## _amount); \
		for (i=0;i<LOCALTYPE ## _amount;i++) { \
			LOCALTYPE[i].transform_amount = ReadLong(f); \
			LOCALTYPE[i].transform.resize(LOCALTYPE[i].transform_amount); \
			for (j=0;j<LOCALTYPE[i].transform_amount;j++) {

	#define MACRO_READTRANSFORM_END(LOCALTYPE) \
			} \
			LOCALTYPE[i].unkint1 = ReadLong(f); \
			LOCALTYPE[i].unkint2 = ReadLong(f); \
			LOCALTYPE[i].object_name_len = ReadLong(f); \
			LOCALTYPE[i].object_name = ""; \
			for (j=0;j<LOCALTYPE[i].object_name_len;j++) \
				LOCALTYPE[i].object_name.push_back(f.get()); \
			f.seekg(GetAlignOffset(f.tellg()),ios::cur); \
		}

	MACRO_READTRANSFORM_BEGIN(localw)
		localw[i].transform[j].time = ReadFloat(f);
		localw[i].transform[j].rot.Read(f);
		localw[i].transform[j].rot1.Read(f);
		localw[i].transform[j].rot2.Read(f);
	MACRO_READTRANSFORM_END(localw)
	num_unk3 = ReadLong(f);
	MACRO_READTRANSFORM_BEGIN(localt)
		localt[i].transform[j].time = ReadFloat(f);
		ModelDataStruct::ReadCoordinates(f,localt[i].transform[j].transx,localt[i].transform[j].transy,localt[i].transform[j].transz);
		ModelDataStruct::ReadCoordinates(f,localt[i].transform[j].trans1x,localt[i].transform[j].trans1y,localt[i].transform[j].trans1z);
		ModelDataStruct::ReadCoordinates(f,localt[i].transform[j].trans2x,localt[i].transform[j].trans2y,localt[i].transform[j].trans2z);
	MACRO_READTRANSFORM_END(localt)
	MACRO_READTRANSFORM_BEGIN(locals)
		locals[i].transform[j].time = ReadFloat(f);
		ModelDataStruct::ReadCoordinates(f,locals[i].transform[j].scalex,locals[i].transform[j].scaley,locals[i].transform[j].scalez,false);
		ModelDataStruct::ReadCoordinates(f,locals[i].transform[j].scale1x,locals[i].transform[j].scale1y,locals[i].transform[j].scale1z,false);
		ModelDataStruct::ReadCoordinates(f,locals[i].transform[j].scale2x,locals[i].transform[j].scale2y,locals[i].transform[j].scale2z,false);
	MACRO_READTRANSFORM_END(locals)
	num_unk4 = ReadLong(f);
	num_unk5 = ReadLong(f);
	frame_rate = ReadFloat(f);
	num_unk6 = ReadLong(f);
	// ToDo: read the end
	num_unk7 = 0x9B;
	unk7.assign(num_unk7, -1);
}

void ModelAnimationData::Write(fstream& f) {
	unsigned int i,j;
	WriteLong(f,name_len);
	for (i=0;i<name_len;i++)
		f.put(name[i]);
	while (f.tellg()%4)
		f.put(0);
	WriteShort(f,num_unk1);
	WriteShort(f,num_unk2);

	#define MACRO_WRITETRANSFORM_BEGIN(LOCALTYPE) \
		WriteLong(f,LOCALTYPE ## _amount); \
		for (i=0;i<LOCALTYPE ## _amount;i++) { \
			WriteLong(f,LOCALTYPE[i].transform_amount); \
			for (j=0;j<LOCALTYPE[i].transform_amount;j++) {

	#define MACRO_WRITETRANSFORM_END(LOCALTYPE) \
			} \
			WriteLong(f,LOCALTYPE[i].unkint1); \
			WriteLong(f,LOCALTYPE[i].unkint2); \
			WriteLong(f,LOCALTYPE[i].object_name_len); \
			for (j=0;j<LOCALTYPE[i].object_name_len;j++) \
				f.put(LOCALTYPE[i].object_name[j]); \
			while (f.tellg()%4) \
				f.put(0); \
		}

	MACRO_WRITETRANSFORM_BEGIN(localw)
		WriteFloat(f,localw[i].transform[j].time);
		localw[i].transform[j].rot.Write(f);
		localw[i].transform[j].rot1.Write(f);
		localw[i].transform[j].rot2.Write(f);
	MACRO_WRITETRANSFORM_END(localw)
	WriteLong(f,num_unk3);
	MACRO_WRITETRANSFORM_BEGIN(localt)
		WriteFloat(f,localt[i].transform[j].time);
		ModelDataStruct::WriteCoordinates(f,localt[i].transform[j].transx,localt[i].transform[j].transy,localt[i].transform[j].transz);
		ModelDataStruct::WriteCoordinates(f,localt[i].transform[j].trans1x,localt[i].transform[j].trans1y,localt[i].transform[j].trans1z);
		ModelDataStruct::WriteCoordinates(f,localt[i].transform[j].trans2x,localt[i].transform[j].trans2y,localt[i].transform[j].trans2z);
	MACRO_WRITETRANSFORM_END(localt)
	MACRO_WRITETRANSFORM_BEGIN(locals)
		WriteFloat(f,locals[i].transform[j].time);
		ModelDataStruct::WriteCoordinates(f,locals[i].transform[j].scalex,locals[i].transform[j].scaley,locals[i].transform[j].scalez,false);
		ModelDataStruct::WriteCoordinates(f,locals[i].transform[j].scale1x,locals[i].transform[j].scale1y,locals[i].transform[j].scale1z,false);
		ModelDataStruct::WriteCoordinates(f,locals[i].transform[j].scale2x,locals[i].transform[j].scale2y,locals[i].transform[j].scale2z,false);
	MACRO_WRITETRANSFORM_END(locals)
	WriteLong(f,num_unk4);
	WriteLong(f,num_unk5);
	WriteFloat(f, frame_rate);
	WriteLong(f,num_unk6);

	#define MACRO_ANIM_END_WRITE(TYPE) \
		if (TYPE==2 || TYPE==3) WriteLong(f,0x14); \
		j = TYPE==1 ? 8 : TYPE==2 ? 29 : TYPE==3 ? 24 : TYPE==4 ? 7 : 12; \
		for (i=0;i<j;i++) WriteLong(f,0); \
		if (TYPE==1) { WriteLong(f,0x04); for (i=0;i<5;i++) WriteLong(f,0); } \
		else if (TYPE==4) { for (i=0;i<5;i++) WriteFloat(f,1.0); for (i=0;i<5;i++) WriteLong(f,0); } \
		j = TYPE==3 ? 0x34 : 0; \
		WriteLong(f,j); for (i=0;i<j;i++) WriteLong(f,0); \
		j = TYPE==3 ? 0x07 : 0; \
		WriteLong(f,j); for (i=0;i<5*j;i++) WriteLong(f,0); \
		WriteFloat(f,1.0); \
		for (i=0;i<4;i++) WriteFloat(f,TYPE==5 ? 0.0 : 1.0);

	MACRO_ANIM_END_WRITE(0)
	MACRO_ANIM_END_WRITE(1)
	MACRO_ANIM_END_WRITE(0)
	MACRO_ANIM_END_WRITE(0)
	MACRO_ANIM_END_WRITE(0)
	MACRO_ANIM_END_WRITE(0)
	MACRO_ANIM_END_WRITE(2)
	MACRO_ANIM_END_WRITE(3)
	MACRO_ANIM_END_WRITE(4)
	MACRO_ANIM_END_WRITE(5)
	WriteLong(f,num_unk7);
	for (i=0;i<num_unk7;i++)
		WriteLong(f,unk7[i]);
	WriteLong(f,0);	WriteLong(f,0);	WriteLong(f,0x1000000);
	WriteLong(f,0);	WriteLong(f,0);	WriteLong(f,0);	WriteLong(f,0);
}

void ModelAnimationData::WriteAsJSON(fstream& f) {
	unsigned int i, j, localindex;
	vector<string> bonelist = GetBoneHierarchyList();
	string space = "";
	f << space << "{\n";
	space += "    ";
	f << space << "\"name\": \"" + name + "\",\n";
	f << space << "\"frameRate\": " + to_string(frame_rate) + ",\n";
	f << space << "\"transform\": [\n";
	space += "    ";
	for (i = 0; i < bonelist.size(); i++) {
		bool addcomma = false;
		f << space << "{\n";
		space += "    ";
		f << space << "\"bone\": \"" + bonelist[i] + "\",\n";
		for (localindex = 0; localindex < localw_amount; localindex++)
			if (localw[localindex].object_name.compare(bonelist[i]) == 0)
				break;
		if (localindex < localw_amount) {
			f << space << "\"localRotation\": [\n";
			space += "    ";
			for (j = 0; j < localw[localindex].transform_amount; j++) {
				f << space << "{ \"time\": " + to_string(localw[localindex].transform[j].time)
					+ ", \"x\": " + to_string(localw[localindex].transform[j].rot.x)
					+ ", \"y\": " + to_string(localw[localindex].transform[j].rot.y)
					+ ", \"z\": " + to_string(localw[localindex].transform[j].rot.z)
					+ ", \"w\": " + to_string(localw[localindex].transform[j].rot.w)
					+ ", \"xInnerTangent\": " + to_string(localw[localindex].transform[j].rot1.x)
					+ ", \"yInnerTangent\": " + to_string(localw[localindex].transform[j].rot1.y)
					+ ", \"zInnerTangent\": " + to_string(localw[localindex].transform[j].rot1.z)
					+ ", \"wInnerTangent\": " + to_string(localw[localindex].transform[j].rot1.w)
					+ ", \"xOuterTangent\": " + to_string(localw[localindex].transform[j].rot2.x)
					+ ", \"yOuterTangent\": " + to_string(localw[localindex].transform[j].rot2.y)
					+ ", \"zOuterTangent\": " + to_string(localw[localindex].transform[j].rot2.z)
					+ ", \"wOuterTangent\": " + to_string(localw[localindex].transform[j].rot2.w)
					+ (j + 1 < localw[localindex].transform_amount ? " },\n" : " }\n");
			}
			space = space.substr(4);
			f << space << "]";
			addcomma = true;
		}
		for (localindex = 0; localindex < localt_amount; localindex++)
			if (localt[localindex].object_name.compare(bonelist[i]) == 0)
				break;
		if (localindex < localt_amount) {
			if (addcomma)
				f << ",\n";
			f << space << "\"localPosition\": [\n";
			space += "    ";
			for (j = 0; j < localt[localindex].transform_amount; j++) {
				f << space << "{ \"time\": " + to_string(localt[localindex].transform[j].time)
					+ ", \"x\": " + to_string(-localt[localindex].transform[j].transx)
					+ ", \"y\": " + to_string(-localt[localindex].transform[j].transy)
					+ ", \"z\": " + to_string(-localt[localindex].transform[j].transz)
					+ ", \"xInnerTangent\": " + to_string(-localt[localindex].transform[j].trans1x)
					+ ", \"yInnerTangent\": " + to_string(-localt[localindex].transform[j].trans1y)
					+ ", \"zInnerTangent\": " + to_string(-localt[localindex].transform[j].trans1z)
					+ ", \"xOuterTangent\": " + to_string(-localt[localindex].transform[j].trans2x)
					+ ", \"yOuterTangent\": " + to_string(-localt[localindex].transform[j].trans2y)
					+ ", \"zOuterTangent\": " + to_string(-localt[localindex].transform[j].trans2z)
					+ (j + 1 < localt[localindex].transform_amount ? " },\n" : " }\n");
			}
			space = space.substr(4);
			f << space << "]";
			addcomma = true;
		}
		for (localindex = 0; localindex < locals_amount; localindex++)
			if (locals[localindex].object_name.compare(bonelist[i]) == 0)
				break;
		if (localindex < locals_amount) {
			if (addcomma)
				f << ",\n";
			f << space << "\"localScale\": [\n";
			space += "    ";
			for (j = 0; j < locals[localindex].transform_amount; j++) {
				f << space << "{ \"time\": " + to_string(locals[localindex].transform[j].time)
					+ ", \"x\": " + to_string(locals[localindex].transform[j].scalex)
					+ ", \"y\": " + to_string(locals[localindex].transform[j].scaley)
					+ ", \"z\": " + to_string(locals[localindex].transform[j].scalez)
					+ ", \"xInnerTangent\": " + to_string(locals[localindex].transform[j].scale1x)
					+ ", \"yInnerTangent\": " + to_string(locals[localindex].transform[j].scale1y)
					+ ", \"zInnerTangent\": " + to_string(locals[localindex].transform[j].scale1z)
					+ ", \"xOuterTangent\": " + to_string(locals[localindex].transform[j].scale2x)
					+ ", \"yOuterTangent\": " + to_string(locals[localindex].transform[j].scale2y)
					+ ", \"zOuterTangent\": " + to_string(locals[localindex].transform[j].scale2z)
					+ (j + 1 < locals[localindex].transform_amount ? " },\n" : " }\n");
			}
			space = space.substr(4);
			f << space << "]";
			addcomma = true;
		}
		f << "\n";
		space = space.substr(4);
		f << space << (i + 1 < bonelist.size() ? "},\n" : "}\n");
	}
	space = space.substr(4);
	f << space << "]\n";
	space = space.substr(4);
	f << space << "}\n";
}

int ModelAnimationData::GetDataSize() {
	unsigned int i;
	int res = 1332+name_len+GetAlignOffset(name_len)+16*(localw_amount+localt_amount+locals_amount)+4*num_unk7;
	// static size for everything up to num_unk6: 40
	// static size (all except num_unk7 array) after num_unk6: 1292
	for (i=0;i<localw_amount;i++)	res += localw[i].object_name_len+GetAlignOffset(localw[i].object_name_len)+52*localw[i].transform_amount;
	for (i=0;i<localt_amount;i++)	res += localt[i].object_name_len+GetAlignOffset(localt[i].object_name_len)+40*localt[i].transform_amount;
	for (i=0;i<locals_amount;i++)	res += locals[i].object_name_len+GetAlignOffset(locals[i].object_name_len)+40*locals[i].transform_amount;
	return res;
}

vector<string> ModelAnimationData::GetBoneHierarchyList() {
	vector<string> bonehierarchylist;
	int i, j;
	for (i = 0; i < localt_amount; i++) {
		for (j = 0; j < bonehierarchylist.size(); j++)
			if (bonehierarchylist[j].compare(localt[i].object_name) == 0)
				break;
		if (j >= bonehierarchylist.size())
			bonehierarchylist.push_back(localt[i].object_name);
	}
	for (i = 0; i < locals_amount; i++) {
		for (j = 0; j < bonehierarchylist.size(); j++)
			if (bonehierarchylist[j].compare(locals[i].object_name) == 0)
				break;
		if (j >= bonehierarchylist.size())
			bonehierarchylist.push_back(locals[i].object_name);
	}
	for (i = 0; i < localw_amount; i++) {
		for (j = 0; j < bonehierarchylist.size(); j++)
			if (bonehierarchylist[j].compare(localw[i].object_name) == 0)
				break;
		if (j >= bonehierarchylist.size())
			bonehierarchylist.push_back(localw[i].object_name);
	}
	return bonehierarchylist;
}

bool ModelAnimationData::HasSameBoneHierarchy(vector<string>& bonelist, vector<string>& otherbonelist) {
	if (bonelist.size() != otherbonelist.size())
		return false;
	int i, j;
	for (i = 0; i < bonelist.size(); i++) {
		for (j = 0; j < otherbonelist.size(); j++)
			if (bonelist[i].compare(otherbonelist[j]) == 0)
				break;
		if (j >= otherbonelist.size())
			return false;
	}
	return true;
}

ModelAnimationData ModelAnimationData::GenerateFromTwoStances(ModelAnimationData& start, ModelAnimationData& end, float duration) {
	ModelAnimationData res = start;
	unsigned int bonecount = 0;
	vector<string> startbone = start.GetBoneHierarchyList();
	vector<string> endbone = end.GetBoneHierarchyList();
	vector<string> bone;
	int i, j;
	for (i = 0; i < startbone.size(); i++) {
		for (j = 0; j < bone.size(); j++)
			if (bone[j].compare(startbone[i]) == 0)
				break;
		if (j >= bone.size())
			bone.push_back(startbone[i]);
	}
	for (i = 0; i < endbone.size(); i++) {
		for (j = 0; j < bone.size(); j++)
			if (bone[j].compare(endbone[i]) == 0)
				break;
		if (j >= bone.size())
			bone.push_back(endbone[i]);
	}
	res.localw_amount = bone.size();
	res.localt_amount = bone.size();
	res.locals_amount = bone.size();
	res.localw.resize(res.localw_amount);
	res.localt.resize(res.localw_amount);
	res.locals.resize(res.localw_amount);
	for (i = 0; i < bone.size(); i++) {
		ModelAnimationTransformW transformw;
		ModelAnimationTransformT transformt;
		ModelAnimationTransformS transforms;

		#define MACRO_SETUP_DEFAULT_TRANSFORM() \
			transformw.rot.SetValue(0.0, 0.0, 0.0, 1.0); \
			transformw.rot1.SetValue(0.0, 0.0, 0.0, 1.0); \
			transformw.rot2.SetValue(0.0, 0.0, 0.0, 1.0); \
			transformt.transx = transformt.transy = transformt.transz = 0.0f; \
			transformt.trans1x = transformt.trans1y = transformt.trans1z = 0.0f; \
			transformt.trans2x = transformt.trans2y = transformt.trans2z = 0.0f; \
			transforms.scalex = transforms.scaley = transforms.scalez = 1.0f; \
			transforms.scale1x = transforms.scale1y = transforms.scale1z = 1.0f; \
			transforms.scale2x = transforms.scale2y = transforms.scale2z = 1.0f;

		#define MACRO_SETUP_START_TRANSFORM(TYPE, LETTER) \
			res.local ## LETTER[i].transform_amount = 2; \
			res.local ## LETTER[i].unkint1 = 2; \
			res.local ## LETTER[i].unkint2 = 2; \
			res.local ## LETTER[i].object_name_len = bone[i].length(); \
			res.local ## LETTER[i].object_name = bone[i]; \
			res.local ## LETTER[i].transform.resize(2); \
			for (j = 0; j < start.local ## LETTER ## _amount; j++) \
				if (bone[i].compare(start.local ## LETTER[j].object_name) == 0) { \
					if (start.local ## LETTER[j].transform_amount > 0 && start.local ## LETTER[j].transform.front().time == 0.0) \
						transform ## LETTER = start.local ## LETTER[j].transform.front(); \
					break; \
				} \
			transform ## LETTER.time = 0.0f; \
			res.local ## LETTER[i].transform[0] = transform ## LETTER;

		#define MACRO_SETUP_END_TRANSFORM(TYPE, LETTER) \
			res.local ## LETTER[i].transform_amount = 2; \
			res.local ## LETTER[i].unkint1 = 2; \
			res.local ## LETTER[i].unkint2 = 2; \
			res.local ## LETTER[i].object_name_len = bone[i].length(); \
			res.local ## LETTER[i].object_name = bone[i]; \
			res.local ## LETTER[i].transform.resize(2); \
			for (j = 0; j < end.local ## LETTER ## _amount; j++) \
				if (bone[i].compare(end.local ## LETTER[j].object_name) == 0) { \
					if (end.local ## LETTER[j].transform_amount > 0) \
						transform ## LETTER = end.local ## LETTER[j].transform.back(); \
					break; \
				} \
			transform ## LETTER.time = duration; \
			res.local ## LETTER[i].transform[1] = transform ## LETTER;

		MACRO_SETUP_DEFAULT_TRANSFORM()
		MACRO_SETUP_START_TRANSFORM(ModelAnimationTransformW, w)
		MACRO_SETUP_START_TRANSFORM(ModelAnimationTransformT, t)
		MACRO_SETUP_START_TRANSFORM(ModelAnimationTransformS, s)
		MACRO_SETUP_DEFAULT_TRANSFORM()
		MACRO_SETUP_END_TRANSFORM(ModelAnimationTransformW, w)
		MACRO_SETUP_END_TRANSFORM(ModelAnimationTransformT, t)
		MACRO_SETUP_END_TRANSFORM(ModelAnimationTransformS, s)
	}
	return res;
}

void ModelDataStruct::ReadCoordinates(fstream& f, float& x, float& y, float& z, bool swapsign) {
	int sign = swapsign ? -1 : 1;
	x = sign*ReadFloat(f);
	y = sign*ReadFloat(f);
	z = sign*ReadFloat(f);
}

void ModelDataStruct::WriteCoordinates(fstream& f, float x, float y, float z, bool swapsign) {
	int sign = swapsign ? -1 : 1;
	WriteFloat(f,sign*x);
	WriteFloat(f,sign*y);
	WriteFloat(f,sign*z);
}

bool ModelDataStruct::Read(fstream& f, GameObjectHierarchy* gohier) {
	hierarchy = gohier;
	UnityArchiveMetaData& meta = *hierarchy->meta_data;
	unsigned int i,j,k;
	vector<string> materialrelatedmeshname;
	for (i=0;i<hierarchy->node_list.size();i++) {
		if (hierarchy->node_list[i]->node_type==23) {
			// Read material list ; TODO: do not duplicates the same materials
			MeshRendererStruct* node = static_cast<MeshRendererStruct*>(hierarchy->node_list[i]);
			vector<ModelMaterialData> matdatalist;
			for (j=0;j<node->child_material_amount;j++) {
				ModelMaterialData matdata;
				if (!node->child_material[j]) return false;
				f.seekg(meta.GetFileOffsetByIndex(node->child_material[j]->file_index));
				matdata.Read(f,meta);
				matdata.name = meta.file_name[node->child_material[j]->file_index];
				matdatalist.push_back(matdata);
			}
			// Remember the name of the parent Game Object for the mesh-material link
			if (!node->parent_object) return false;
			if (node->parent_object->node_type==1)
				materialrelatedmeshname.push_back(static_cast<GameObjectStruct*>(node->parent_object)->GetScopedName());
			else
				materialrelatedmeshname.push_back("");
			material.push_back(matdatalist);
		} else if (hierarchy->node_list[i]->node_type==33) {
			// Read mesh
			MeshFilterStruct* node = static_cast<MeshFilterStruct*>(hierarchy->node_list[i]);
			ModelMeshData meshdata;
			if (!node->child_mesh) return false;
			f.seekg(meta.GetFileOffsetByIndex(node->child_mesh->file_index));
			meshdata.Read(f);
			if (!node->parent_object) return false;
			if (node->parent_object->node_type==1) {
				meshdata.name = static_cast<GameObjectStruct*>(node->parent_object)->name;
				meshdata.scoped_name = static_cast<GameObjectStruct*>(node->parent_object)->GetScopedName();
			}
			mesh.push_back(meshdata);
		} else if (hierarchy->node_list[i]->node_type==137) {
			// Read mesh + material list + node names
			SkinnedMeshRendererStruct* node = static_cast<SkinnedMeshRendererStruct*>(hierarchy->node_list[i]);
			ModelMeshData meshdata;
			vector<ModelMaterialData> matdatalist;
			if (!node->child_mesh) return false;
			f.seekg(meta.GetFileOffsetByIndex(node->child_mesh->file_index));
			meshdata.Read(f);
			if (!node->parent_object) return false;
			if (node->parent_object->node_type==1) {
				meshdata.name = static_cast<GameObjectStruct*>(node->parent_object)->name;
				meshdata.scoped_name = static_cast<GameObjectStruct*>(node->parent_object)->GetScopedName();
			}
			for (j=0;j<node->child_material_amount;j++) {
				ModelMaterialData matdata;
				if (!node->child_material[j]) return false;
				f.seekg(meta.GetFileOffsetByIndex(node->child_material[j]->file_index));
				matdata.Read(f,meta);
				matdata.name = meta.file_name[node->child_material[j]->file_index];
				matdatalist.push_back(matdata);
			}
			k = 0;
			for (j=0;j<node->child_bone_amount && k<meshdata.bone_amount;j++) {
				if (!node->child_bone[j]) return false;
				if (node->child_bone[j]->node_type==4) {
					TransformStruct* transfbone = static_cast<TransformStruct*>(node->child_bone[j]);
					if (!transfbone->child_object) return false;
					if (transfbone->child_object->node_type==1) {
						meshdata.bone[k].name = static_cast<GameObjectStruct*>(transfbone->child_object)->name;
						meshdata.bone[k].scoped_name = static_cast<GameObjectStruct*>(transfbone->child_object)->GetScopedName();
						k++;
					}
				}
			}
			mesh.push_back(meshdata);
			material.push_back(matdatalist);
			materialrelatedmeshname.push_back(meshdata.scoped_name);
		}
	}
	if (mesh.size()!=material.size()) return false;
	for (i=0;i<materialrelatedmeshname.size();i++)
		if (materialrelatedmeshname[i]!=mesh[i].scoped_name) {
			for (j=i+1;j<mesh.size();j++)
				if (materialrelatedmeshname[i]==mesh[j].scoped_name) {
					ModelMeshData tmp = mesh[i];
					mesh[i] = mesh[j];
					mesh[j] = tmp;
					break;
				}
			if (j>=mesh.size()) return false;
		}
	return true;
}

bool ModelDataStruct::Write(fstream& f) {
	UnityArchiveMetaData& meta = *hierarchy->meta_data;
	unsigned int i,j;
	unsigned int meshcounter = 0;
	hierarchy->OverwriteHierarchy(f);
	for (i=0;i<hierarchy->node_list.size();i++) {
		if (hierarchy->node_list[i]->node_type==23) {
			// Write material list ; TODO: do not duplicates the same materials
			MeshRendererStruct* node = static_cast<MeshRendererStruct*>(hierarchy->node_list[i]);
			for (j=0;j<node->child_material_amount;j++) {
				ModelMaterialData& matdata = material[meshcounter][j];
				if (!node->child_material[j]) return false;
				f.seekg(meta.GetFileOffsetByIndex(node->child_material[j]->file_index));
				matdata.Write(f,meta);
			}
			meshcounter++;
		} else if (hierarchy->node_list[i]->node_type==33) {
			// Write mesh
			MeshFilterStruct* node = static_cast<MeshFilterStruct*>(hierarchy->node_list[i]);
			ModelMeshData& meshdata = mesh[meshcounter];
			if (!node->child_mesh) return false;
			f.seekg(meta.GetFileOffsetByIndex(node->child_mesh->file_index));
			meshdata.Write(f);
		} else if (hierarchy->node_list[i]->node_type==137) {
			// Write mesh + material list + node names
			SkinnedMeshRendererStruct* node = static_cast<SkinnedMeshRendererStruct*>(hierarchy->node_list[i]);
			ModelMeshData& meshdata = mesh[meshcounter];
			if (!node->child_mesh) return false;
			f.seekg(meta.GetFileOffsetByIndex(node->child_mesh->file_index));
			meshdata.Write(f);
			for (j=0;j<node->child_material_amount;j++) {
				ModelMaterialData& matdata = material[meshcounter][j];
				if (!node->child_material[j]) return false;
				f.seekg(meta.GetFileOffsetByIndex(node->child_material[j]->file_index));
				matdata.Write(f,meta);
			}
			meshcounter++;
		}
	}
	return true;
}

int ModelDataStruct::Export(const char* outputname, int format) {
    FbxManager* sdkmanager = NULL;
    FbxScene* sdkscene = NULL;
    InitializeSdkObjects(sdkmanager, sdkscene);
    ConvertModelToFBX(*this, sdkmanager, sdkscene);
//	SetupAxisSystem(sdkscene);
	bool exportresult = SaveScene(outputname, sdkmanager, sdkscene, format);
	DestroySdkObjects(sdkmanager);
    if (!exportresult)
        return 1;
    return 0;
}

int ModelDataStruct::Import(const char* inputname, bool retrieveanims) {
	// ToDo: optimize importation of "animations only" -> don't load the whole fbx model
	FbxManager* sdkmanager = NULL;
	FbxScene* sdkscene = NULL;
	InitializeSdkObjects(sdkmanager, sdkscene);
	bool importresult = LoadScene(inputname, sdkmanager, sdkscene, retrieveanims);
	if (!importresult) {
		DestroySdkObjects(sdkmanager);
		return 1;
	}
	importresult = ConvertFBXToModel(*this, sdkmanager, sdkscene, retrieveanims);
	DestroySdkObjects(sdkmanager);
	if (!importresult)
		return 2;
	return 0;
}

void ModelDataStruct::SetupPostImportData(fstream& filebase, vector<unsigned int> folderfiles, UnityArchiveMetaData archivelist[UNITY_ARCHIVE_AMOUNT], vector<int64_t> additionalinfotoavoid, UnityArchiveAssetBundle* animbundle, GameObjectHierarchy* basehierarchy, int mergepolicy) {
	unsigned int i,j,k;
	if (basehierarchy)
		hierarchy->MergeHierarchy(archivelist,additionalinfotoavoid,basehierarchy,mergepolicy);
	else
		hierarchy->MergeHierarchy(archivelist,additionalinfotoavoid,NULL,2);
	vector<float> minmeshx,minmeshy,minmeshz;
	vector<float> maxmeshx,maxmeshy,maxmeshz;
	vector<float> meanmeshx,meanmeshy,meanmeshz;
	unsigned int meshcounter = 0;
	for (i=0;i<mesh.size();i++) {
		minmeshx.push_back(FLT_MAX);	minmeshy.push_back(FLT_MAX);	minmeshz.push_back(FLT_MAX);
		maxmeshx.push_back(-FLT_MAX);	maxmeshy.push_back(-FLT_MAX);	maxmeshz.push_back(-FLT_MAX);
		meanmeshx.push_back(0.0);		meanmeshy.push_back(0.0);		meanmeshz.push_back(0.0);
		for (j=0;j<mesh[i].vertice_amount;j++) {
			if (mesh[i].vert[j].x<minmeshx[i]) minmeshx[i] = mesh[i].vert[j].x;
			if (mesh[i].vert[j].x>maxmeshx[i]) maxmeshx[i] = mesh[i].vert[j].x;
			if (mesh[i].vert[j].y<minmeshy[i]) minmeshy[i] = mesh[i].vert[j].y;
			if (mesh[i].vert[j].y>maxmeshy[i]) maxmeshy[i] = mesh[i].vert[j].y;
			if (mesh[i].vert[j].z<minmeshz[i]) minmeshz[i] = mesh[i].vert[j].z;
			if (mesh[i].vert[j].z>maxmeshz[i]) maxmeshz[i] = mesh[i].vert[j].z;
			meanmeshx[i] += mesh[i].vert[j].x;
			meanmeshy[i] += mesh[i].vert[j].y;
			meanmeshz[i] += mesh[i].vert[j].z;
			mesh[i].vert[j].unkf = -1.0; // 1.0 or -1.0
			mesh[i].vert[j].unkuv1 = 0.0;
			mesh[i].vert[j].unkuv2 = 0.0;
		}
		meanmeshx[i] /= (float)mesh[i].vertice_amount;
		meanmeshy[i] /= (float)mesh[i].vertice_amount;
		meanmeshz[i] /= (float)mesh[i].vertice_amount;
		for (j=0;j<mesh[i].material_info_amount;j++) {
			float matminx = FLT_MAX, matminy = FLT_MAX, matminz = FLT_MAX;
			float matmaxx = -FLT_MAX, matmaxy = -FLT_MAX, matmaxz = -FLT_MAX;
			uint16_t vertid;
			for (k=0;k<mesh[i].mat_info[j].vert_list_amount;k++) {
				vertid = mesh[i].vert_list[mesh[i].mat_info[j].vert_list_start/2+k];
				if (mesh[i].vert[vertid].x<matminx) matminx = mesh[i].vert[vertid].x;
				if (mesh[i].vert[vertid].x>matmaxx) matmaxx = mesh[i].vert[vertid].x;
				if (mesh[i].vert[vertid].y<matminy) matminy = mesh[i].vert[vertid].y;
				if (mesh[i].vert[vertid].y>matmaxy) matmaxy = mesh[i].vert[vertid].y;
				if (mesh[i].vert[vertid].z<matminz) matminz = mesh[i].vert[vertid].z;
				if (mesh[i].vert[vertid].z>matmaxz) matmaxz = mesh[i].vert[vertid].z;
			}
			mesh[i].mat_info[j].unk0 = 0;
			mesh[i].mat_info[j].center_x = (matminx+matmaxx)/2;
			mesh[i].mat_info[j].center_y = (matminy+matmaxy)/2;
			mesh[i].mat_info[j].center_z = (matminz+matmaxz)/2;
			mesh[i].mat_info[j].radius_x = (matmaxx-matminx)/2;
			mesh[i].mat_info[j].radius_y = (matmaxy-matminy)/2;
			mesh[i].mat_info[j].radius_z = (matmaxz-matminz)/2;
		}
		mesh[i].unk_flag1 = 0;
		mesh[i].unk_flag2 = 1;
		mesh[i].unk_flag3 = 1;
		mesh[i].unk_flag4 = 1;
		// DEBUG: most of p0data4 format but not weapons/battlescene/worldmap
		mesh[i].vert_format0 = 0x8B;
		mesh[i].vert_format1 = 8;
		mesh[i].vert_format2 = 0x03000000;
		mesh[i].vert_format3 = 0x03000C00;
		mesh[i].vert_format4 = 0;
		mesh[i].vert_format5 = 0x02000001;
		mesh[i].vert_format6 = 0;
		mesh[i].vert_format7 = 0;
		mesh[i].vert_format8 = 0;
		mesh[i].vert_format9 = 0x04001800;
		mesh[i].vert_datasize = 12*4*mesh[i].vertice_amount+8*(mesh[i].vertice_amount%2);
		mesh[i].center_x = (minmeshx[i]+maxmeshx[i])/2;
		mesh[i].center_y = (minmeshy[i]+maxmeshy[i])/2;
		mesh[i].center_z = (minmeshz[i]+maxmeshz[i])/2;
		mesh[i].radius_x = (maxmeshx[i]-minmeshx[i])/2;
		mesh[i].radius_y = (maxmeshy[i]-minmeshy[i])/2;
		mesh[i].radius_z = (maxmeshz[i]-minmeshz[i])/2;
		mesh[i].unk_num1 = 1;
		mesh[i].unk_num2 = 0;
		mesh[i].unk_num3 = 0;
	}
	int64_t shaderinfo = 0; // DEBUG: Use the same shader for all materials
	for (k=0;k<folderfiles.size();k++)
		if (hierarchy->meta_data->file_type1[folderfiles[k]]==48) {
			shaderinfo = hierarchy->meta_data->file_info[folderfiles[k]];
			break;
		}
	if (shaderinfo==0)
		for (k=0;k<hierarchy->meta_data->header_file_amount;k++)
			if (hierarchy->meta_data->file_type1[k]==48) {
				shaderinfo = hierarchy->meta_data->file_info[k];
				break;
			}
	for (i=0;i<material.size();i++) {
		for (j=0;j<material[i].size();j++) {
			material[i][j].shader_unk = 0;
			material[i][j].shader_info = shaderinfo;

			#define MACRO_SETUP_FILEMAP(FMAP) \
				FMAP.unk1 = 0; \
				FMAP.unk2 = 0; \
				FMAP.unk3 = 0; \
				FMAP.file_info = 0; \
				if (FMAP.file_name!="") { \
					for (k=0;k<folderfiles.size();k++) \
						if (hierarchy->meta_data->file_name[folderfiles[k]]==FMAP.file_name && hierarchy->meta_data->file_type1[folderfiles[k]]==28) { \
							FMAP.file_info = hierarchy->meta_data->file_info[folderfiles[k]]; \
							break; \
						} \
				}

			MACRO_SETUP_FILEMAP(material[i][j].bumpmap)
			MACRO_SETUP_FILEMAP(material[i][j].detailalbedomap)
			MACRO_SETUP_FILEMAP(material[i][j].detailmask)
			MACRO_SETUP_FILEMAP(material[i][j].detailnormalmap)
			MACRO_SETUP_FILEMAP(material[i][j].emissionmap)
			MACRO_SETUP_FILEMAP(material[i][j].maintex)
			MACRO_SETUP_FILEMAP(material[i][j].metallicglossmap)
			MACRO_SETUP_FILEMAP(material[i][j].occlusionmap)
			MACRO_SETUP_FILEMAP(material[i][j].parallaxmap)
			material[i][j].parallaxmap_unk4 = 12;
		}
	}
	for (i=0;i<hierarchy->node_list.size();i++) {
		// ToDo: MeshRenderer & MeshFilter
		if (hierarchy->node_list[i]->node_type==111) {
			AnimationStruct* node = static_cast<AnimationStruct*>(hierarchy->node_list[i]);
			if (node->child_clip1!=NULL)
				continue;
			for (j=0;j<folderfiles.size();j++)
				if (hierarchy->meta_data->file_type1[folderfiles[j]]==74) {
					node->child_clip1 = new GameObjectNode(node,*hierarchy,74,0,hierarchy->meta_data->file_info[folderfiles[j]]);
					node->child_clip1->file_index = folderfiles[j];
					node->child_clip2 = node->child_clip1;
					node->flag2 = 1;
					hierarchy->node_list.insert(hierarchy->node_list.begin()+i+1,node->child_clip1);
					bool hastakeanim = false;
					for (k=0;k<animation.size();k++)
						if (animation[k].name.substr(0,4)=="Take") {
							hastakeanim = true;
							break;
						}
					if (!hastakeanim) {
						ModelAnimationData newanim;
						filebase.seekg(basehierarchy->meta_data->GetFileOffsetByIndex(folderfiles[j]));
						newanim.Read(filebase,basehierarchy);
						newanim.anim_id = 0xFFFFFFFF;
						newanim.file_id = folderfiles[j];
						if (newanim.name.substr(0,4)!="Take") {
							newanim.name = "Take 001";
							newanim.name_len = newanim.name.length();
						}
						animation.push_back(newanim);
					}
					break;
				}
		} else if (hierarchy->node_list[i]->node_type==137) {
			SkinnedMeshRendererStruct* node = static_cast<SkinnedMeshRendererStruct*>(hierarchy->node_list[i]);
			// DEBUG: unclear what all these actually mean
			node->center_x = meanmeshx[meshcounter];
			node->center_y = meanmeshy[meshcounter];
			node->center_z = meanmeshz[meshcounter];
			node->radius_x = max(abs(meanmeshx[meshcounter]-minmeshx[meshcounter]),abs(meanmeshx[meshcounter]-maxmeshx[meshcounter]));
			node->radius_y = max(abs(meanmeshy[meshcounter]-minmeshy[meshcounter]),abs(meanmeshy[meshcounter]-maxmeshy[meshcounter]));
			node->radius_z = max(abs(meanmeshz[meshcounter]-minmeshz[meshcounter]),abs(meanmeshz[meshcounter]-maxmeshz[meshcounter]));
			node->flag1 = 1;
			node->flags_unk = 0x101;
			node->max_unk = 0xFFFFFFFF;
			node->float_unk1 = 1.0;	node->float_unk2 = 1.0;	node->float_unk3 = 0.0;	node->float_unk4 = 0.0;
			node->float_unk5 = 1.0;	node->float_unk6 = 1.0;	node->float_unk7 = 0.0;	node->float_unk8 = 0.0;
			node->flag3 = 0;	node->flag4 = 0;	node->flag5 = 0;	node->flag6 = 0;
			node->flag7 = 1;	node->flag8 = 1;	node->flag9 = 0;	node->flag10 = 0;	node->flag11 = 0;
			node->flag12 = 0;	node->flag13 = 0;	node->flag14 = 0;	node->flag15 = 0;
			node->flag16 = 0;
			node->float_unk9 = 0.0;
			meshcounter++;
		}
	}
	string animsearchpath = "assets/resources/animations/"+steam_name+"/";
	string animsearchname;
	uint8_t modelcat = DATABASE_MODEL_CATEGORY_LIST_ALL;
	uint32_t modelid = atoi(steam_name.c_str());
	if (modelid>0 || steam_name=="0")
		for (i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++)
			if (modelid==HADES_STRING_MODEL_NAME[i].id)
				modelcat = DATABASE_MODEL_CATEGORY_TO_LIST(HADES_STRING_MODEL_NAME[i].category);
	for (i=0;i<animation.size();i++) {
		animation[i].anim_id = 0xFFFFFFFF;
		animation[i].file_id = -1;
		if (mergepolicy==2 || basehierarchy==NULL) // DEBUG: "Take" animations are imported in UNITY_ARCHIVE_DATA5 in these cases
			continue;
		if (animation[i].name.substr(0,4)=="Take") { // DEBUG: assuming there's only 1 such animation
			for (j=0;j<hierarchy->node_list.size();j++)
				if (hierarchy->node_list[j]->node_type==111) {
					AnimationStruct& nodeanim = *static_cast<AnimationStruct*>(hierarchy->node_list[j]);
					if (nodeanim.child_clip1!=NULL) {
						animation[i].anim_id = 0xFFFFFFFF;
						animation[i].file_id = nodeanim.child_clip1->file_index;
						break;
					}
				}
		}
		if (animation[i].file_id>=0)
			continue;
		if (animbundle) {
			int32_t animsearchindex = AnimationDatabase::GetIndexFromSteam(animation[i].name,modelcat);
			if (animsearchindex<0)
				continue;
			animsearchname = animsearchpath+to_string(AnimationDatabase::GetId(animsearchindex))+".anim";
			animation[i].anim_id = AnimationDatabase::GetId(animsearchindex);
			for (j=0;j<animbundle->amount;j++)
				if (animbundle->path[j]==animsearchname) {
					for (k=0;k<archivelist[UNITY_ARCHIVE_DATA5].header_file_amount;k++)
						if (animbundle->info[j]==archivelist[UNITY_ARCHIVE_DATA5].file_info[k]) {
							animation[i].file_id = k;
							j = animbundle->amount;
							break;
						}
					break;
				}
		}
	}
}

//=============================//
//       FBX SDK Common        //
//=============================//

inline FbxString GetFbxNameIndice(unsigned int meshid, unsigned int objid, unsigned int meshamount) {
	return meshamount == 1 ? FbxString((int)objid) : FbxString((int)meshid) + FbxString("_") + FbxString((int)objid);
}

bool ConvertModelToFBX(ModelDataStruct& model, FbxManager*& sdkmanager, FbxScene*& sdkscene) {
    unsigned int i,j,k,l;
    // Metadata
	FbxDocumentInfo* sceneInfo = FbxDocumentInfo::Create(sdkmanager, "SceneInfo");
	sceneInfo->mTitle = "Exported Model";
	sceneInfo->mSubject = "Final Fantasy IX Model";
	sceneInfo->mAuthor = "Hades Workshop";
	sceneInfo->mRevision = "";
	sceneInfo->mKeywords = "";
	sceneInfo->mComment = model.description.c_str();
	sdkscene->SetSceneInfo(sceneInfo);
	// Construct the basis for the skeleton
	FbxNode* lRootNode = sdkscene->GetRootNode();
	FbxSkeleton* lSingleSkeleton = NULL;
	FbxNode* lSingleSkeletonNode = NULL;
	vector<FbxSkeleton*> lMultiSkeleton;
	vector<string> lMultiSkeletonName;
	bool hasbones = false;
	for (i=0;i<model.mesh.size();i++)
		if (model.mesh[i].bone_amount>0) {
			hasbones = true;
			break;
		}
	if (model.animation.size()>0 && !hasbones) {
		lSingleSkeleton = FbxSkeleton::Create(sdkscene, "");
		lSingleSkeleton->SetSkeletonType(FbxSkeleton::eRoot);
		lSingleSkeleton->Size.Set(1.0);
		lSingleSkeletonNode = FbxNode::Create(sdkscene, "");
		lSingleSkeletonNode->SetNodeAttribute(lSingleSkeleton);
	}
	// Construct all the Fbx nodes
	vector<FbxNode*> lNodeList;
	vector<GameObjectNode*> hierarchynode = model.hierarchy->node_list;
	for (i=0;i<hierarchynode.size();i++) {
		if (hierarchynode[i]->node_type==4) {
			// Simple node
			string nodename = "";
			TransformStruct* nodespec = static_cast<TransformStruct*>(hierarchynode[i]);
			if (nodespec->child_object && nodespec->child_object->node_type==1)
				nodename = static_cast<GameObjectStruct*>(nodespec->child_object)->name;
			FbxNode* lNode = FbxNode::Create(sdkscene, FbxString(nodename.c_str()));
			lNode->LclRotation.Set(FbxDouble3(nodespec->rot.GetRoll(), nodespec->rot.GetPitch(), nodespec->rot.GetYaw()));
			lNode->LclTranslation.Set(FbxDouble3(nodespec->x, nodespec->y, nodespec->z));
			lNode->LclScaling.Set(FbxDouble3(nodespec->scale_x, nodespec->scale_y, nodespec->scale_z));
			if (hierarchynode[i]==model.hierarchy->root_node)
				lRootNode->AddChild(lNode);
			lNodeList.push_back(lNode);
		} else {
			lNodeList.push_back(NULL);
		}
	}
	// Link the nodes constructed so far and construct the bones
	for (i=0;i<hierarchynode.size();i++) {
		if (hierarchynode[i]->node_type==4 && lNodeList[i]) {
			TransformStruct* nodespec = static_cast<TransformStruct*>(hierarchynode[i]);
			if (nodespec->parent_transform && nodespec->parent_transform->node_type==4)
				for (j=0;j<hierarchynode.size();j++)
					if (nodespec->parent_transform==hierarchynode[j] && lNodeList[j]) {
						lNodeList[j]->AddChild(lNodeList[i]);
						break;
					}
		} else if (hierarchynode[i]->node_type==137 && hasbones) {
			SkinnedMeshRendererStruct* nodespec = static_cast<SkinnedMeshRendererStruct*>(hierarchynode[i]);
			for (j=0;j<nodespec->child_bone_amount;j++) {

				#define MACRO_CONSTRUCT_NEWBONE(BONENAME) \
					bool newbone = true; \
					for (k=0;k<lMultiSkeletonName.size();k++) \
						if (lMultiSkeletonName[k]==BONENAME) { \
							newbone = false; \
							break; \
						} \
					if (newbone) { \
						GameObjectStruct* boneobj = NULL; \
						TransformStruct* parenttransf = NULL; \
						for (k=0;k<hierarchynode.size();k++) \
							if (hierarchynode[k]->node_type==1 && static_cast<GameObjectStruct*>(hierarchynode[k])->GetScopedName()==BONENAME) { \
								boneobj = static_cast<GameObjectStruct*>(hierarchynode[k]); \
								parenttransf = boneobj->GetParentTransform(); \
								break; \
							} \
						if (boneobj) { \
							FbxSkeleton* lSkeleton = FbxSkeleton::Create(sdkscene, boneobj->name.c_str()); \
							if (parenttransf->parent_transform==NULL || parenttransf->parent_transform==model.hierarchy->root_node) \
								lSkeleton->SetSkeletonType(FbxSkeleton::eRoot); \
							else \
								lSkeleton->SetSkeletonType(FbxSkeleton::eLimbNode); \
							for (k=0;k<hierarchynode.size();k++) \
								if (parenttransf==hierarchynode[k] && lNodeList[k]) { \
									lNodeList[k]->SetNodeAttribute(lSkeleton); \
									break; \
								} \
							lSkeleton->Size.Set(1.0); \
							lMultiSkeleton.push_back(lSkeleton); \
							lMultiSkeletonName.push_back(BONENAME); \
						} \
					}

				string bonename = static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(nodespec->child_bone[j])->child_object)->GetScopedName();
				MACRO_CONSTRUCT_NEWBONE(bonename)
			}
		}
	}
	// Construct more bones
	if (hasbones)
		for (i=0;i<model.animation.size();i++) {
			for (j=0;j<model.animation[i].localw_amount;j++) {
				string bonename = model.animation[i].localw[j].object_name;
				MACRO_CONSTRUCT_NEWBONE(bonename)
			}
			for (j=0;j<model.animation[i].localt_amount;j++) {
				string bonename = model.animation[i].localt[j].object_name;
				MACRO_CONSTRUCT_NEWBONE(bonename)
			}
			for (j=0;j<model.animation[i].locals_amount;j++) {
				string bonename = model.animation[i].locals[j].object_name;
				MACRO_CONSTRUCT_NEWBONE(bonename)
			}
		}
	// Construct the meshes
	for (i=0;i<hierarchynode.size();i++) {
		if (hierarchynode[i]->node_type==1) {
			GameObjectStruct* nodespec = static_cast<GameObjectStruct*>(hierarchynode[i]);
			string nodescopedname = nodespec->GetScopedName();
			bool isskinnedmesh = false;
			bool hasmeshfilter = false;
			bool hasmeshrender = false;
			bool hasanimation = false;
			for (j=0;j<nodespec->child_amount;j++)
				if (nodespec->child[j]) {
					if (nodespec->child[j]->node_type==23)
						hasmeshrender = true;
					else if (nodespec->child[j]->node_type==33)
						hasmeshfilter = true;
					else if (nodespec->child[j]->node_type==111)
						hasanimation = true;
					else if (nodespec->child[j]->node_type==137)
						isskinnedmesh = true;
				}
			int meshindex = -1;
			if (isskinnedmesh || hasmeshfilter) {
				for (j=0;j<model.mesh.size();j++)
					if (model.mesh[j].scoped_name==nodescopedname) {
						meshindex = j;
						break;
					}
			}
			if (meshindex>=0) {
				// Mesh
				ModelMeshData& mesh = model.mesh[meshindex];
				FbxMesh* lMesh = FbxMesh::Create(sdkscene, mesh.name.c_str());
				FbxNode* lMeshNode = NULL;
				for (j=0;j<hierarchynode.size();j++)
					if (nodespec->GetParentTransform()==hierarchynode[j] && lNodeList[j]) {
						lMeshNode = lNodeList[j];
						break;
					}
				lMesh->InitControlPoints(mesh.vertice_amount);
				FbxVector4* lControlPoints = lMesh->GetControlPoints();
				FbxGeometryElementNormal* lGeometryElementNormal = lMesh->CreateElementNormal();
				lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
				lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
				FbxGeometryElementTangent* lGeometryElementTangent = lMesh->CreateElementTangent();
				lGeometryElementTangent->SetMappingMode(FbxGeometryElement::eByControlPoint);
				lGeometryElementTangent->SetReferenceMode(FbxGeometryElement::eDirect);
				FbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
				lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
				lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eDirect);
				FbxVector4 lNormal,lTangent;
				FbxVector2 lUVPoint;
				for (j = 0; j<mesh.vert.size(); j++) {
					lControlPoints[j].Set(mesh.vert[j].x, mesh.vert[j].y, mesh.vert[j].z);
					lNormal.Set(mesh.vert[j].nx, mesh.vert[j].ny, mesh.vert[j].nz);
					lTangent.Set(mesh.vert[j].tx, mesh.vert[j].ty, mesh.vert[j].tz);
					lGeometryElementNormal->GetDirectArray().Add(lNormal);
					lGeometryElementTangent->GetDirectArray().Add(lTangent);
					lUVPoint.Set(mesh.vert[j].u, mesh.vert[j].v);
					lUVDiffuseElement->GetDirectArray().Add(lUVPoint);
				}
				lMesh->InitMaterialIndices(FbxLayerElement::EMappingMode::eByPolygon);
				uint16_t vertid[3];
				for (j = 0; 3 * j<mesh.vert_list.size(); j++) {
					int mtlidcur = -1;
					for (k = 0; k <mesh.mat_info.size(); k++) {
						if (3 * j >= mesh.mat_info[k].vert_list_start / 2 && 3 * j < mesh.mat_info[k].vert_list_start / 2 + mesh.mat_info[k].vert_list_amount) {
							mtlidcur = k;
							break;
						}
					}
					vertid[0] = mesh.vert_list[3 * j];
					vertid[1] = mesh.vert_list[3 * j + 1];
					vertid[2] = mesh.vert_list[3 * j + 2];
					lMesh->BeginPolygon(mtlidcur);
					lMesh->AddPolygon(vertid[0]);
					lMesh->AddPolygon(vertid[2]);
					lMesh->AddPolygon(vertid[1]);
					lMesh->EndPolygon();
				}
				lMesh->GenerateNormals(false,true);
				lMeshNode->SetNodeAttribute(lMesh);
				lMeshNode->SetShadingMode(FbxNode::eTextureShading);
				// Materials and Textures
				vector<ModelMaterialData>& material = model.material[meshindex];
				for (j = 0; j < material.size(); j++) {
					FbxString lTextureName = material[j].maintex.file_name.c_str();
					FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(sdkscene, material[j].name.c_str());
					lMeshNode->AddMaterial(lMaterial);
					FbxFileTexture* lFileTex = FbxFileTexture::Create(sdkscene, lTextureName);
					lFileTex->SetFileName(lTextureName);
					lFileTex->SetRelativeFileName(lTextureName);
					lFileTex->SetTextureUse(FbxTexture::eStandard);
					lFileTex->SetMappingType(FbxTexture::eUV);
					lFileTex->SetMaterialUse(FbxFileTexture::eModelMaterial);
					lFileTex->UVSet.Set(FbxString("DiffuseUV"));
					FbxProperty lProp;
					// DEBUG: there is a "FbxMaterialBumpDT" and other similars... No idea of how to use them
					if (material[j].bumpmap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_BumpMap");
						lProp.Set(FbxString(material[j].bumpmap.file_name.c_str()));
					}
					if (material[j].detailalbedomap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_DetailAlbedoMap");
						lProp.Set(FbxString(material[j].detailalbedomap.file_name.c_str()));
					}
					if (material[j].detailmask.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_DetailMask");
						lProp.Set(FbxString(material[j].detailmask.file_name.c_str()));
					}
					if (material[j].detailnormalmap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_DetailNormalMap");
						lProp.Set(FbxString(material[j].detailnormalmap.file_name.c_str()));
					}
					if (material[j].emissionmap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_EmissionMap");
						lProp.Set(FbxString(material[j].emissionmap.file_name.c_str()));
					}
					if (material[j].metallicglossmap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_MetallicGlossMap");
						lProp.Set(FbxString(material[j].metallicglossmap.file_name.c_str()));
					}
					if (material[j].occlusionmap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_OcclusionMap");
						lProp.Set(FbxString(material[j].occlusionmap.file_name.c_str()));
					}
					if (material[j].parallaxmap.file_info!=0) {
						FbxProperty::Create(lMaterial, FbxStringDT, "_ParallaxMap");
						lProp.Set(FbxString(material[j].parallaxmap.file_name.c_str()));
					}
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_BumpScale");
					lProp.Set(material[j].bumpscale_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_Cutoff");
					lProp.Set(material[j].cutoff_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_DetailNormalMapScale");
					lProp.Set(material[j].detailnormalmapscale_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_DstBlend");
					lProp.Set(material[j].dstblend_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_Glossiness");
					lProp.Set(material[j].glossiness_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_Metallic");
					lProp.Set(material[j].metallic_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_Mode");
					lProp.Set(material[j].mode_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_OcclusionStrength");
					lProp.Set(material[j].occlusionstrength_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_Parallax");
					lProp.Set(material[j].parallax_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_SrcBlend");
					lProp.Set(material[j].srcblend_value);
					lProp = FbxProperty::Create(lMaterial, FbxFloatDT, "_UVSec");
					lProp.Set(material[j].uvsec_value);
					lProp = FbxProperty::Create(lMaterial, FbxDouble2DT, "_ZWrite");
					lProp.Set(FbxDouble2(material[j].zwrite_factor,material[j].zwrite_mode));
					lProp = FbxProperty::Create(lMaterial, FbxColor4DT, "_Color");
					lProp.Set(FbxColor(material[j].color_red,material[j].color_green,material[j].color_blue,material[j].color_alpha));
					lProp = FbxProperty::Create(lMaterial, FbxColor4DT, "_EmissionColor");
					lProp.Set(FbxColor(material[j].emissioncolor_red,material[j].emissioncolor_green,material[j].emissioncolor_blue,material[j].emissioncolor_alpha));
					if (material[j].has_alphapremultiply_on) {
						FbxProperty::Create(lMaterial, FbxColor3DT, "_ALPHAPREMULTIPLY_ON");
						lProp.Set(FbxColor(FbxDouble(material[j].alphapremultiply_on_value1)/0xFF,FbxDouble(material[j].alphapremultiply_on_value2)/0xFF,FbxDouble(material[j].alphapremultiply_on_value3)/0xFF));
					}
					if (material[j].has_rendertype) {
						FbxProperty::Create(lMaterial, FbxUIntDT, "RenderType");
						lProp.Set(material[j].rendertype_value);
					}
					if (material[j].has_transparent) {
						FbxProperty::Create(lMaterial, FbxUIntDT, "Transparent");
						lProp.Set(material[j].transparent_value);
					}
					lMaterial->Diffuse.ConnectSrcObject(lFileTex);
				}
				// Link points to Skeleton Clusters
				if (model.animation.size()>0 || hasbones) {
					FbxSkin* lSkin = FbxSkin::Create(sdkscene, "");
					FbxAMatrix lTransformMatrix = lMeshNode->EvaluateGlobalTransform();
					FbxAMatrix lTransformLinkMatrix;
					lTransformLinkMatrix.SetIdentity();
					lSkin->SetGeometry(lMesh);
					if (model.animation.size()>0 && !hasbones) {
						FbxCluster* lSingleSkeletonCluster = FbxCluster::Create(sdkscene, "");
						lSingleSkeletonCluster->SetLink(lSingleSkeletonNode);
						lSingleSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);
						for (j = 0; j<mesh.vert.size(); j++)
							lSingleSkeletonCluster->AddControlPointIndex(j, 1.0);
						lSkin->AddCluster(lSingleSkeletonCluster);
						lTransformLinkMatrix = lSingleSkeletonNode->EvaluateGlobalTransform();
						lSingleSkeletonCluster->SetTransformMatrix(lTransformMatrix);
						lSingleSkeletonCluster->SetTransformLinkMatrix(lTransformLinkMatrix);
					} else {
						for (j = 0; j < mesh.bone_amount; j++) {
							FbxCluster* lSkeletonCluster = FbxCluster::Create(sdkscene, "");
							for (k=0;k<lMultiSkeletonName.size();k++)
								if (lMultiSkeletonName[k]==mesh.bone[j].scoped_name) {
									lSkeletonCluster->SetLink(lMultiSkeleton[k]->GetNode());
									break;
								}
							lSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);
							for (k = 0; k < mesh.vertice_attachment_amount; k++)
								for (l = 0; l < 4; l++)
									if (mesh.vert_attachment[k].bone_id[l]==j && mesh.vert_attachment[k].bone_factor[l] > 0.0)
										lSkeletonCluster->AddControlPointIndex(k, mesh.vert_attachment[k].bone_factor[l]);
							lSkin->AddCluster(lSkeletonCluster);
							LinearTransformationMatrix& linkmatrix = mesh.bone[j].transform_matrix;
							lTransformMatrix.SetRow(0,FbxVector4(linkmatrix.value[0][0],linkmatrix.value[1][0],linkmatrix.value[2][0],linkmatrix.value[3][0]));
							lTransformMatrix.SetRow(1,FbxVector4(linkmatrix.value[0][1],linkmatrix.value[1][1],linkmatrix.value[2][1],linkmatrix.value[3][1]));
							lTransformMatrix.SetRow(2,FbxVector4(linkmatrix.value[0][2],linkmatrix.value[1][2],linkmatrix.value[2][2],linkmatrix.value[3][2]));
							lTransformMatrix.SetRow(3,FbxVector4(linkmatrix.value[0][3],linkmatrix.value[1][3],linkmatrix.value[2][3],linkmatrix.value[3][3]));
							lSkeletonCluster->SetTransformMatrix(lTransformMatrix);
							lSkeletonCluster->SetTransformLinkMatrix(lTransformLinkMatrix);
						}
					}
					lMesh->AddDeformer(lSkin);
				}
			}
		}
	}
	// Animations
	FbxTime lTime;
	int lKeyIndex;
	FbxAnimCurveFilterUnroll lUnrollFilter;
	lUnrollFilter.SetForceAutoTangents(true);
    for (i=0;i<model.animation.size();i++) {
		FbxAnimStack* lAnimStack = FbxAnimStack::Create(sdkscene, model.animation[i].name.c_str());
		FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(sdkscene, "Base Layer");
		FbxAnimCurve* lCurve;
		FbxTime lStartTime, lEndTime;
		lStartTime.SetSecondDouble(0.0);
		lEndTime.SetSecondDouble(0.0);
		lAnimStack->AddMember(lAnimLayer);
		for (j=0;j<model.animation[i].localw_amount;j++) {

			#define MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,COMPONENTNAME,VALUE,VALUE1,VALUE2) \
				lCurve = lSkeletonNode->LCLTYPE.GetCurve(lAnimLayer, COMPONENTNAME, true); \
				if (lCurve) { \
					lCurve->KeyModifyBegin(); \
					for (k=0;k<model.animation[i].LOCALTYPE[j].transform_amount;k++) { \
						lTime.SetSecondDouble(model.animation[i].LOCALTYPE[j].transform[k].time); \
						lKeyIndex = lCurve->KeyAdd(lTime); \
						lCurve->KeySet(lKeyIndex, lTime, model.animation[i].LOCALTYPE[j].transform[k].VALUE, FbxAnimCurveDef::eInterpolationCubic, FbxAnimCurveDef::eTangentGenericBreak, model.animation[i].LOCALTYPE[j].transform[k].VALUE1, model.animation[i].LOCALTYPE[j].transform[k].VALUE2); \
						if (lStartTime>lTime) lStartTime = lTime; \
						if (lEndTime<lTime) lEndTime = lTime; \
					} \
					lCurve->KeyModifyEnd(); \
				}

			#define MACRO_FBX_APPLY_TRANSFORMATION(LOCALTYPE,LCLTYPE,BASECOORD,X,Y,Z) \
				FbxNode* lSkeletonNode = NULL; \
				if (!hasbones) \
					lSkeletonNode = lSingleSkeletonNode; \
				else \
					for (k=0;k<lMultiSkeletonName.size();k++) \
						if (model.animation[i].LOCALTYPE[j].object_name==lMultiSkeletonName[k]) { \
							lSkeletonNode = lMultiSkeleton[k]->GetNode(); \
							break; \
						} \
				if (lSkeletonNode==NULL) \
					continue; \
				MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,FBXSDK_CURVENODE_COMPONENT_X,BASECOORD ## X,BASECOORD ## 1 ## X,BASECOORD ## 2 ## X) \
				MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,FBXSDK_CURVENODE_COMPONENT_Y,BASECOORD ## Y,BASECOORD ## 1 ## Y,BASECOORD ## 2 ## Y) \
				MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,FBXSDK_CURVENODE_COMPONENT_Z,BASECOORD ## Z,BASECOORD ## 1 ## Z,BASECOORD ## 2 ## Z)
			
			MACRO_FBX_APPLY_TRANSFORMATION(localw,LclRotation,rot,.GetRoll(),.GetPitch(),.GetYaw())
			FbxAnimCurve** lAnimEulerCurve = new FbxAnimCurve*[3];
			lAnimEulerCurve[0] = lSkeletonNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
			lAnimEulerCurve[1] = lSkeletonNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
			lAnimEulerCurve[2] = lSkeletonNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
			lUnrollFilter.Apply(lAnimEulerCurve,3);
			delete[] lAnimEulerCurve;
		}
		for (j=0;j<model.animation[i].localt_amount;j++) {
			MACRO_FBX_APPLY_TRANSFORMATION(localt,LclTranslation,trans,x,y,z)
		}
		for (j=0;j<model.animation[i].locals_amount;j++) {
			MACRO_FBX_APPLY_TRANSFORMATION(locals,LclScaling,scale,x,y,z)
		}
		lAnimStack->LocalStart = lStartTime;
		lAnimStack->LocalStop = lEndTime;
	}
    return true;
}

template<typename Tvect, typename Tgeoelement>
Tvect GetVertexGeometryElement(FbxMesh* lMesh, Tgeoelement* lGeometryElement, unsigned int vertindex) {
	if (lGeometryElement==NULL)
		return Tvect();
	int mappingid = vertindex;
	switch (lGeometryElement->GetMappingMode()) {
	case FbxLayerElement::eByControlPoint:
		if (lGeometryElement->GetReferenceMode()==FbxLayerElement::eIndex || lGeometryElement->GetReferenceMode()==FbxLayerElement::eIndexToDirect) {
			mappingid = lGeometryElement->GetIndexArray()[vertindex];
			if (mappingid<0)
				return Tvect();
		}
		return lGeometryElement->FbxLayerElementTemplate::GetDirectArray()[mappingid];
		break;
	case FbxLayerElement::eByPolygonVertex: {
		unsigned int i,j;
		for (i=0;i<lMesh->GetPolygonCount();i++)
			for (j=0;j<lMesh->GetPolygonSize(i);j++)
				if (lMesh->GetPolygonVertex(i,j)==vertindex)
					return lGeometryElement->FbxLayerElementTemplate::GetDirectArray()[lMesh->GetTextureUVIndex(i,j)];
		return Tvect();
		break;
	}
	case FbxLayerElement::eByPolygon:
		return Tvect();
		break;
	case FbxLayerElement::eByEdge:
		return Tvect();
		break;
	case FbxLayerElement::eAllSame:
		return lGeometryElement->FbxLayerElementTemplate::GetDirectArray()[0];
		break;
	}
	return Tvect();
}

bool ConvertFBXToModel(ModelDataStruct& model, FbxManager*& sdkmanager, FbxScene*& sdkscene, bool retrieveanims) {
	unsigned int i,j,k,l,m;
	// Construct the basis for the skeleton
	FbxNode* lRootNode = sdkscene->GetRootNode();
	FbxSkeleton* lSingleSkeleton = NULL;
	FbxNode* lSingleSkeletonNode = NULL;
	vector<FbxSkeleton*> lMultiSkeleton;
	vector<string> lMultiSkeletonName;
	bool hasbones = true; // DEBUG: maybe accept to construct non-boned models?
	model.hierarchy = new GameObjectHierarchy();
	// Construct the TransformStruct+GameObjectStruct hierarchy
	if (lRootNode==NULL && lRootNode->GetChildCount()<1)
		return false;
	// ToDo: RootNode is ignored, assuming it has exactly 1 child and no geometric transformation
	FbxNode* lCurrentNode = lRootNode->GetChild(0);
	vector<int> indexlist;
	vector<FbxNode*> lNodeList;
	vector<TransformStruct*> transf_list;
	vector<GameObjectStruct*> obj_list;
	int childcurrentindex = 0;
	indexlist.push_back(0);
	while (childcurrentindex>=0) {
		TransformStruct* newtransf = new TransformStruct(NULL,*model.hierarchy,4,0,0);
		newtransf->child_transform_amount = 0;
		Quaternion::EulerToQuaternion(newtransf->rot, lCurrentNode->LclRotation.Get()[0],lCurrentNode->LclRotation.Get()[1],lCurrentNode->LclRotation.Get()[2]);
		newtransf->x = lCurrentNode->LclTranslation.Get()[0];
		newtransf->y = lCurrentNode->LclTranslation.Get()[1];
		newtransf->z = lCurrentNode->LclTranslation.Get()[2];
		newtransf->scale_x = lCurrentNode->LclScaling.Get()[0];
		newtransf->scale_y = lCurrentNode->LclScaling.Get()[1];
		newtransf->scale_z = lCurrentNode->LclScaling.Get()[2];
		newtransf->parent_transform = NULL;
		for (i=0;i<lNodeList.size();i++)
			if (lCurrentNode->GetParent()==lNodeList[i]) {
				newtransf->parent = obj_list[i];
				newtransf->parent_transform = transf_list[i];
				transf_list[i]->child_transform.push_back(newtransf);
				transf_list[i]->child_transform_amount++;
				break;
			}
		GameObjectStruct* newobj = new GameObjectStruct(newtransf,*model.hierarchy,1,0,0);
		newobj->child_amount = 1;
		newobj->child.push_back(newtransf);
		newobj->name = lCurrentNode->GetNameOnly();
		newobj->name_len = newobj->name.length();
		newobj->unknown = 0;
		newobj->unk1 = 0;
		newobj->unk2 = 0;
		newobj->unk3 = 1;
		if (transf_list.size()==0) {
			AnimationStruct* newanim = new AnimationStruct(newobj,*model.hierarchy,111,0,0);
			newanim->parent_object = newobj;
			newanim->child_clip1 = NULL;
			newanim->child_clip2 = NULL;
			newanim->flag1 = 1;
			newanim->flag2 = 0;
			newanim->flag3 = 0;
			newanim->flag4 = 1;
			newanim->flag5 = 1;
			newobj->child.push_back(newanim);
			newobj->child_amount++;
		}
		newtransf->child_object = newobj;
		transf_list.push_back(newtransf);
		obj_list.push_back(newobj);
		lNodeList.push_back(lCurrentNode);
		while (childcurrentindex>=0 && indexlist[childcurrentindex]>=lCurrentNode->GetChildCount()) {
			childcurrentindex--;
			indexlist.pop_back();
			lCurrentNode = lCurrentNode->GetParent();
		}
		if (childcurrentindex>=0) {
			lCurrentNode = lCurrentNode->GetChild(indexlist[childcurrentindex]);
			indexlist[childcurrentindex]++;
			indexlist.push_back(0);
			childcurrentindex++;
		}
	}
	// Sequence the bone list of the whole model
	vector<FbxString> lSkeletonName;
	vector<FbxNode*> lSkeletonNode;
	indexlist.clear();
	for (i=0;i<lNodeList.size();i++) {
		lCurrentNode = lNodeList[i];
		if (lCurrentNode->GetNodeAttribute()!=NULL && lCurrentNode->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::EType::eSkeleton) {
			FbxSkeleton* lSkeleton =  static_cast<FbxSkeleton*>(lCurrentNode->GetNodeAttribute());
			lSkeletonName.push_back(lSkeleton->GetNameOnly());
			lSkeletonNode.push_back(lCurrentNode);
			indexlist.push_back(i);
		}
	}
	model.hierarchy->root_node = transf_list[0];
	// Construct the SkinnedMeshRenderer
	for (i=0;i<lNodeList.size();i++) {
		lCurrentNode = lNodeList[i];
		model.hierarchy->node_list.push_back(transf_list[i]);
		model.hierarchy->node_list.push_back(obj_list[i]);
		for (j=1;j<obj_list[i]->child_amount;j++)
			model.hierarchy->node_list.push_back(obj_list[i]->child[j]);
		// TODO: accept other geometry types in the future?
		if (lCurrentNode->GetNodeAttribute()!=NULL && lCurrentNode->GetNodeAttribute()->GetAttributeType()==FbxNodeAttribute::EType::eMesh) {
			SkinnedMeshRendererStruct* newskinmesh = new SkinnedMeshRendererStruct(obj_list[i],*model.hierarchy,137,0,0);
			model.hierarchy->node_list.push_back(newskinmesh);
			obj_list[i]->child.push_back(newskinmesh);
			obj_list[i]->child_amount++;
			newskinmesh->parent_object = obj_list[i];
			// Mesh
			FbxMesh* lMesh =  static_cast<FbxMesh*>(lCurrentNode->GetNodeAttribute());
			GameObjectNode* newmeshnode = new GameObjectNode(newskinmesh,*model.hierarchy,43,0,0);
			newskinmesh->child_mesh = newmeshnode;
			ModelMeshData newmesh;
			newmesh.name = obj_list[i]->name;
			newmesh.scoped_name = obj_list[i]->GetScopedName();
			newmesh.vertice_amount = lMesh->GetControlPointsCount();
			FbxVector4* lCtrlPts = lMesh->GetControlPoints();
			FbxGeometryElementNormal* lGeometryElementNormal = lMesh->GetElementNormal();
			FbxGeometryElementTangent* lGeometryElementTangent = lMesh->GetElementTangent();
			FbxGeometryElementUV* lGeometryElementUV = lMesh->GetElementUV();
			for (j=0;j<newmesh.vertice_amount;j++) {
				FbxVector4 lVertNormal = GetVertexGeometryElement<FbxVector4,FbxGeometryElementNormal>(lMesh,lGeometryElementNormal,j);
				FbxVector4 lVertTangent = GetVertexGeometryElement<FbxVector4,FbxGeometryElementTangent>(lMesh,lGeometryElementTangent,j);
				FbxVector2 lVertUV = GetVertexGeometryElement<FbxVector2,FbxGeometryElementUV>(lMesh,lGeometryElementUV,j);
				ModelMeshVertex newvert;
				newvert.x = lCtrlPts[j][0];
				newvert.y = lCtrlPts[j][1];
				newvert.z = lCtrlPts[j][2];
				newvert.nx = lVertNormal[0];
				newvert.ny = lVertNormal[1];
				newvert.nz = lVertNormal[2];
				newvert.tx = lVertTangent[0];
				newvert.ty = lVertTangent[1];
				newvert.tz = lVertTangent[2];
				newvert.u = lVertUV[0];
				newvert.v = lVertUV[1];
				newmesh.vert.push_back(newvert);
			}
			// Polygons are converted to triangles
			FbxGeometryElementMaterial* lGeometryElementMaterial = lMesh->GetElementMaterial();
			vector<FbxSurfaceMaterial*> lMaterialList;
			FbxSurfaceMaterial* lCurrentMaterial = NULL;
			newmesh.vertex_list_amount = 0;
			for (j=0;j<lMesh->GetPolygonCount();j++) {
				FbxSurfaceMaterial* lPolyMaterial;
				if (lGeometryElementMaterial->GetMappingMode()==FbxGeometryElement::eByPolygon)
					lPolyMaterial = lMesh->GetNode()->GetMaterial(lGeometryElementMaterial->GetIndexArray()[j]);
				else
					lPolyMaterial = lMesh->GetNode()->GetMaterial(lGeometryElementMaterial->GetIndexArray()[0]);
				if (lPolyMaterial!=lCurrentMaterial) {
					for (k=0;k<lMaterialList.size();k++)
						if (lMaterialList[k]==lPolyMaterial)
							break;
					if (k>=lMaterialList.size())
						lMaterialList.push_back(lPolyMaterial);
					ModelMeshMaterialInfo newmatinfo;
					newmatinfo.vert_list_start = newmesh.vertex_list_amount*2;
					newmesh.mat_info.push_back(newmatinfo);
					lCurrentMaterial = lPolyMaterial;
				}
				int polysize = lMesh->GetPolygonSize(j);
				newmesh.vertex_list_amount += 3*(polysize-2);
				for (k=0;k+2<polysize;k++) {
					// [0 1 2] [0 2 3] [0 3 -1] [-1 3 4] [-1 4 -2] [-2 4 5] [-2 5 -3] etc...
					if (k==0) {
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,0));
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,2));
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,1));
					} else if (k%2==1) {
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,(-((int)k-1)/2)%polysize));
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,(k+5)/2));
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,(k+4)/2));
					} else {
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,(-((int)k-1)/2)%polysize));
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,polysize-k/2));
						newmesh.vert_list.push_back(lMesh->GetPolygonVertex(j,(k+4)/2));
					}
				}
			}
			newmesh.material_info_amount = newmesh.mat_info.size();
			for (j=0;j<newmesh.material_info_amount;j++) {
				vector<uint16_t> matvertlist;
				if (j+1<newmesh.material_info_amount)
					newmesh.mat_info[j].vert_list_amount = (newmesh.mat_info[j+1].vert_list_start-newmesh.mat_info[j].vert_list_start)/2;
				else
					newmesh.mat_info[j].vert_list_amount = newmesh.vertex_list_amount-newmesh.mat_info[j].vert_list_start/2;
				for (k=0;k<newmesh.mat_info[j].vert_list_amount;k++) {
					for (l=0;l<matvertlist.size();l++)
						if (matvertlist[l]==newmesh.vert_list[newmesh.mat_info[j].vert_list_start/2+k])
							break;
					if (l>=matvertlist.size())
						matvertlist.push_back(newmesh.vert_list[newmesh.mat_info[j].vert_list_start/2+k]);
				}
				newmesh.mat_info[j].vert_amount = matvertlist.size();
				if (j==0)
					newmesh.mat_info[j].vert_start = 0;
				else
					newmesh.mat_info[j].vert_start = newmesh.mat_info[j-1].vert_start+newmesh.mat_info[j-1].vert_amount;
			}
			// Materials
			vector<ModelMaterialData> newmatlist;
			newskinmesh->child_material_amount = lMaterialList.size();
			for (j=0;j<lMaterialList.size();j++) {
				GameObjectNode* newmatnode = new GameObjectNode(newskinmesh,*model.hierarchy,21,0,0);
				model.hierarchy->node_list.push_back(newmatnode);
				newskinmesh->child_material.push_back(newmatnode);
				FbxSurfacePhong* lMaterial = static_cast<FbxSurfacePhong*>(lMaterialList[j]);
				FbxFileTexture* lFileTex = static_cast<FbxFileTexture*>(lMaterial->Diffuse.GetSrcObject(FbxCriteria::ObjectType(FbxFileTexture::ClassId)));
				ModelMaterialData newmat;
				newmat.name = lMaterial->GetNameOnly();
				if (lFileTex!=NULL) {
					newmat.maintex.file_name = RemoveFbxStringExtension(lFileTex->GetNameOnly());
				} else {
					newmat.maintex.file_name = "";
				}
				FbxProperty lProp = lMaterial->FindProperty("_BumpMap",FbxStringDT);
				if (lProp.IsValid()) newmat.bumpmap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_DetailAlbedoMap",FbxStringDT);
				if (lProp.IsValid()) newmat.detailalbedomap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_DetailMask",FbxStringDT);
				if (lProp.IsValid()) newmat.detailmask.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_DetailNormalMap",FbxStringDT);
				if (lProp.IsValid()) newmat.detailnormalmap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_EmissionMap",FbxStringDT);
				if (lProp.IsValid()) newmat.emissionmap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_MetallicGlossMap",FbxStringDT);
				if (lProp.IsValid()) newmat.metallicglossmap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_OcclusionMap",FbxStringDT);
				if (lProp.IsValid()) newmat.occlusionmap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_ParallaxMap",FbxStringDT);
				if (lProp.IsValid()) newmat.parallaxmap.file_name = RemoveFbxStringExtension(lProp.Get<FbxString>());
				lProp = lMaterial->FindProperty("_BumpScale",FbxFloatDT);
				if (lProp.IsValid()) newmat.bumpscale_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_Cutoff",FbxFloatDT);
				if (lProp.IsValid()) newmat.cutoff_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_DetailNormalMapScale",FbxFloatDT);
				if (lProp.IsValid()) newmat.detailnormalmapscale_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_DstBlend",FbxFloatDT);
				if (lProp.IsValid()) newmat.dstblend_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_Glossiness",FbxFloatDT);
				if (lProp.IsValid()) newmat.glossiness_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_Metallic",FbxFloatDT);
				if (lProp.IsValid()) newmat.metallic_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_Mode",FbxFloatDT);
				if (lProp.IsValid()) newmat.mode_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_OcclusionStrength",FbxFloatDT);
				if (lProp.IsValid()) newmat.occlusionstrength_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_Parallax",FbxFloatDT);
				if (lProp.IsValid()) newmat.parallax_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_SrcBlend",FbxFloatDT);
				if (lProp.IsValid()) newmat.srcblend_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_UVSec",FbxFloatDT);
				if (lProp.IsValid()) newmat.uvsec_value = lProp.Get<FbxFloat>();
				lProp = lMaterial->FindProperty("_ZWrite",FbxDouble2DT);
				if (lProp.IsValid()) {
					newmat.zwrite_factor = lProp.Get<FbxDouble2>()[0];
					newmat.zwrite_mode = lProp.Get<FbxDouble2>()[1];
				}
				lProp = lMaterial->FindProperty("_Color",FbxColor4DT);
				if (lProp.IsValid()) {
					newmat.color_red = lProp.Get<FbxColor>().mRed;
					newmat.color_green = lProp.Get<FbxColor>().mGreen;
					newmat.color_blue = lProp.Get<FbxColor>().mBlue;
					newmat.color_alpha = lProp.Get<FbxColor>().mAlpha;
				}
				lProp = lMaterial->FindProperty("_EmissionColor",FbxColor4DT);
				if (lProp.IsValid()) {
					newmat.emissioncolor_red = lProp.Get<FbxColor>().mRed;
					newmat.emissioncolor_green = lProp.Get<FbxColor>().mGreen;
					newmat.emissioncolor_blue = lProp.Get<FbxColor>().mBlue;
					newmat.emissioncolor_alpha = lProp.Get<FbxColor>().mAlpha;
				}
				lProp = lMaterial->FindProperty("_ALPHAPREMULTIPLY_ON",FbxColor3DT);
				if (lProp.IsValid()) {
					newmat.alphapremultiply_on_value1 = lProp.Get<FbxColor>().mRed*0xFF;
					newmat.alphapremultiply_on_value2 = lProp.Get<FbxColor>().mGreen*0xFF;
					newmat.alphapremultiply_on_value3 = lProp.Get<FbxColor>().mBlue*0xFF;
					newmat.has_alphapremultiply_on = true;
				}
				lProp = lMaterial->FindProperty("RenderType",FbxUIntDT);
				if (lProp.IsValid()) {
					newmat.rendertype_value = lProp.Get<FbxUInt>();
					newmat.has_rendertype = true;
				}
				lProp = lMaterial->FindProperty("Transparent",FbxUIntDT);
				if (lProp.IsValid()) {
					newmat.transparent_value = lProp.Get<FbxUInt>();
					newmat.has_transparent = true;
				}
				newmatlist.push_back(newmat);
			}
			model.hierarchy->node_list.push_back(newmeshnode);
			// Bones
			FbxSkin* lSkin = static_cast<FbxSkin*>(lMesh->GetDeformer(0,FbxDeformer::EDeformerType::eSkin));
			newskinmesh->child_bone_amount = 0;
			newskinmesh->child_bone_sample = NULL;
			if (lSkin!=NULL) {
				FbxAMatrix lTransformMatrix;
				newmesh.bone_amount = lSkin->GetClusterCount();
				newskinmesh->child_bone_amount = newmesh.bone_amount;
				for (j=0;j<newmesh.bone_amount;j++) {
					FbxCluster* lSkeletonCluster = lSkin->GetCluster(j);
					ModelMeshBone newbone;
					newbone.name = "";
					newbone.scoped_name = "";
					for (k=0;k<lSkeletonNode.size();k++) {
						if (lSkeletonNode[k]==lSkeletonCluster->GetLink()) {
							newbone.name = obj_list[indexlist[k]]->name;
							newbone.scoped_name = obj_list[indexlist[k]]->GetScopedName();
							newskinmesh->child_bone.push_back(transf_list[indexlist[k]]);
							newskinmesh->child_bone_sample = transf_list[indexlist[k]];
							break;
						}
					}
					if (k>=lSkeletonNode.size()) { // Should not happen
						TransformStruct* newbonenode = new TransformStruct(newskinmesh,*model.hierarchy,4,0,0);
						newbonenode->child_object = NULL;
						newbonenode->child_transform_amount = 0;
						newbonenode->parent_transform = NULL;
						newbonenode->rot.SetValue(0,0,0,1);
						newbonenode->x = 0;			newbonenode->y = 0;			newbonenode->z = 0;
						newbonenode->scale_x = 0;	newbonenode->scale_y = 0;	newbonenode->scale_z = 0;
						model.hierarchy->node_list.push_back(newbonenode);
						newskinmesh->child_bone.push_back(newbonenode);
					}
					lSkeletonCluster->GetTransformMatrix(lTransformMatrix);
					for (k=0;k<4;k++)
						for (l=0;l<4;l++)
							newbone.transform_matrix.value[k][l] = (l==3 && k==3 ? -lTransformMatrix.Get(l,k) : lTransformMatrix.Get(l,k));
					newmesh.bone.push_back(newbone);
				}
				newmesh.bone_unk_amount = newmesh.bone_amount;
				for (j=0;j<newmesh.bone_unk_amount;j++) {
					newmesh.bone_unk_list.push_back(j+1); // DEBUG
				}
				newmesh.bone_unk_main = newmesh.bone_unk_amount>0 ? newmesh.bone_unk_list[0] : 0;
				newmesh.vertice_attachment_amount = newmesh.vertice_amount;
				for (j=0;j<newmesh.vertice_attachment_amount;j++) {
					ModelMeshVertexAttachment newvertattach;
					for (k=0;k<4;k++) {
						newvertattach.bone_id[k] = 0;
						newvertattach.bone_factor[k] = 0.0;
					}
					for (k=0;k<lSkin->GetClusterCount();k++) {
						FbxCluster* lSkeletonCluster = lSkin->GetCluster(k);
						int* lPtsIndex = lSkeletonCluster->GetControlPointIndices();
						double* lPtsFactor = lSkeletonCluster->GetControlPointWeights();
						for (l=0;l<lSkeletonCluster->GetControlPointIndicesCount();l++) {
							if (lPtsIndex[l]==j) {
								for (m=0;m<4;m++)
									if (newvertattach.bone_factor[m]==0.0)
										break;
								if (m<4) {
									newvertattach.bone_id[m] = k;
									newvertattach.bone_factor[m] = lPtsFactor[l];
								}
							}
						}
					}
					newmesh.vert_attachment.push_back(newvertattach);
				}
			} else {
				// Default bone: take the whole mesh
				newmesh.bone_amount = 1;
				newskinmesh->child_bone_amount = 1;
				ModelMeshBone newbone;
				newbone.name = obj_list[i]->name;
				newbone.scoped_name = obj_list[i]->GetScopedName();
				newskinmesh->child_bone.push_back(transf_list[i]);
				newskinmesh->child_bone_sample = transf_list[i];
				for (k=0;k<4;k++)
					for (l=0;l<4;l++)
						newbone.transform_matrix.value[k][l] = (l==k ? 1.0 : 0.0);
				newmesh.bone.push_back(newbone);
				newmesh.bone_unk_amount = 1;
				newmesh.bone_unk_list.push_back(1); // DEBUG
				newmesh.bone_unk_main = newmesh.bone_unk_list[0];
				newmesh.vertice_attachment_amount = newmesh.vertice_amount;
				for (j=0;j<newmesh.vertice_attachment_amount;j++) {
					ModelMeshVertexAttachment newvertattach;
					newvertattach.bone_id[0] = 0;
					newvertattach.bone_factor[0] = 1.0;
					for (k=1;k<4;k++) {
						newvertattach.bone_id[k] = 0;
						newvertattach.bone_factor[k] = 0.0;
					}
					newmesh.vert_attachment.push_back(newvertattach);
				}
			}
			if (newskinmesh->child_bone_sample==NULL && newskinmesh->child_bone_amount>0)
				newskinmesh->child_bone_sample = newskinmesh->child_bone[0];
			model.material.push_back(newmatlist);
			model.mesh.push_back(newmesh);
		}
	}
	// Animations
	if (!retrieveanims)
		return true;
	model.animation.resize(sdkscene->GetSrcObjectCount(FbxCriteria::ObjectType(FbxAnimStack::ClassId)));
	for (i=0;i<model.animation.size();i++) {
		FbxAnimStack* lAnimStack = sdkscene->GetSrcObject<FbxAnimStack>(i);
		FbxAnimLayer* lAnimLayer = lAnimStack->GetMember<FbxAnimLayer>();
		float starttime = lAnimStack->LocalStart.Get().GetSecondDouble();
		FbxAnimCurve* lCurveX, *lCurveY, *lCurveZ;
		FbxTime lCurrentTime;
		int keyindexx, keyindexy, keyindexz, keycountx, keycounty, keycountz;
		/* Default Slopes:
		- Right Slope Value = Left Slope Value
		- Next Left Slope Value = (Next Value - Value) / (Next Time - Time)
		Exceptions:
		- Right Slope of 1st Transform (t=0) = Next Left Slope
		- Right Slope & Next Left Slope of of last Transform = 0
		Warning: For rotations, the formula is to be understood as angle interpolation
		*/

		#define MACRO_FBX_RETRIEVE_TRANSFORMATION(LOCALTYPE,ANIMLOCALTYPE,LCLTYPE,VALUE,VALUEFIELDX,VALUEFIELDY,VALUEFIELDZ) \
			for (j=0;j<lSkeletonNode.size();j++) { \
				lCurveX = lSkeletonNode[j]->LCLTYPE.GetCurve(lAnimLayer,FBXSDK_CURVENODE_COMPONENT_X); \
				lCurveY = lSkeletonNode[j]->LCLTYPE.GetCurve(lAnimLayer,FBXSDK_CURVENODE_COMPONENT_Y); \
				lCurveZ = lSkeletonNode[j]->LCLTYPE.GetCurve(lAnimLayer,FBXSDK_CURVENODE_COMPONENT_Z); \
				if (lCurveX || lCurveY || lCurveZ) { \
					lCurveX = lSkeletonNode[j]->LCLTYPE.GetCurve(lAnimLayer,FBXSDK_CURVENODE_COMPONENT_X,true); \
					lCurveY = lSkeletonNode[j]->LCLTYPE.GetCurve(lAnimLayer,FBXSDK_CURVENODE_COMPONENT_Y,true); \
					lCurveZ = lSkeletonNode[j]->LCLTYPE.GetCurve(lAnimLayer,FBXSDK_CURVENODE_COMPONENT_Z,true); \
					ModelAnimationLocal<ANIMLOCALTYPE> LOCALTYPE; \
					LOCALTYPE.object_name = obj_list[indexlist[j]]->GetScopedName(); \
					LOCALTYPE.object_name_len = LOCALTYPE.object_name.length(); \
					LOCALTYPE.unkint1 = 2; \
					LOCALTYPE.unkint2 = 2; \
					keyindexx = 0; \
					keyindexy = 0; \
					keyindexz = 0; \
					keycountx = lCurveX ? lCurveX->KeyGetCount() : 0; \
					keycounty = lCurveY ? lCurveY->KeyGetCount() : 0; \
					keycountz = lCurveZ ? lCurveZ->KeyGetCount() : 0; \
					while (keyindexx<keycountx || keyindexy<keycounty || keyindexz<keycountz) { \
						ANIMLOCALTYPE localtransf; \
						if (keyindexx<keycountx && (keyindexy>=keycounty || lCurveX->KeyGetTime(keyindexx)<=lCurveY->KeyGetTime(keyindexy)) && (keyindexz>=keycountz || lCurveX->KeyGetTime(keyindexx)<=lCurveZ->KeyGetTime(keyindexz))) { \
							lCurrentTime = lCurveX->KeyGetTime(keyindexx); \
						} else if (keyindexy<keycounty && (keyindexx>=keycountx || lCurveY->KeyGetTime(keyindexy)<=lCurveX->KeyGetTime(keyindexx)) && (keyindexz>=keycountz || lCurveY->KeyGetTime(keyindexy)<=lCurveZ->KeyGetTime(keyindexz))) { \
							lCurrentTime = lCurveY->KeyGetTime(keyindexy); \
						} else { \
							lCurrentTime = lCurveZ->KeyGetTime(keyindexz); \
						} \
						localtransf.time = lCurrentTime.GetSecondDouble()-starttime; \
						if (keyindexx<keycountx && lCurrentTime==lCurveX->KeyGetTime(keyindexx)) { \
							localtransf.VALUE ## VALUEFIELDX = lCurveX->KeyGetValue(keyindexx); \
/*							localtransf.VALUE ## 1 ## VALUEFIELDX = lCurveX->KeyGetLeftDerivative(keyindexx); \
							localtransf.VALUE ## 2 ## VALUEFIELDX = lCurveX->KeyGetRightDerivative(keyindexx); \
*/							keyindexx++; \
						} else { \
							localtransf.VALUE ## VALUEFIELDX = lCurveX->Evaluate(lCurrentTime); \
/*							localtransf.VALUE ## 1 ## VALUEFIELDX = lCurveX->EvaluateLeftDerivative(lCurrentTime,&keyindexx); \
							localtransf.VALUE ## 2 ## VALUEFIELDX = lCurveX->EvaluateRightDerivative(lCurrentTime,&keyindexx); \
*/						} \
						if (keyindexy<keycounty && lCurrentTime==lCurveY->KeyGetTime(keyindexy)) { \
							localtransf.VALUE ## VALUEFIELDY = lCurveY->KeyGetValue(keyindexy); \
/*							localtransf.VALUE ## 1 ## VALUEFIELDY = lCurveY->KeyGetLeftDerivative(keyindexy); \
							localtransf.VALUE ## 2 ## VALUEFIELDY = lCurveY->KeyGetRightDerivative(keyindexy); \
*/							keyindexy++; \
						} else { \
							localtransf.VALUE ## VALUEFIELDY = lCurveY->Evaluate(lCurrentTime); \
/*							localtransf.VALUE ## 1 ## VALUEFIELDY = lCurveY->EvaluateLeftDerivative(lCurrentTime,&keyindexy); \
							localtransf.VALUE ## 2 ## VALUEFIELDY = lCurveY->EvaluateRightDerivative(lCurrentTime,&keyindexy); \
*/						} \
						if (keyindexz<keycountz && lCurrentTime==lCurveZ->KeyGetTime(keyindexz)) { \
							localtransf.VALUE ## VALUEFIELDZ = lCurveZ->KeyGetValue(keyindexz); \
/*							localtransf.VALUE ## 1 ## VALUEFIELDZ = lCurveZ->KeyGetLeftDerivative(keyindexz); \
							localtransf.VALUE ## 2 ## VALUEFIELDZ = lCurveZ->KeyGetRightDerivative(keyindexz); \
*/							keyindexz++; \
						} else { \
							localtransf.VALUE ## VALUEFIELDZ = lCurveZ->Evaluate(lCurrentTime); \
/*							localtransf.VALUE ## 1 ## VALUEFIELDZ = lCurveZ->EvaluateLeftDerivative(lCurrentTime,&keyindexz); \
							localtransf.VALUE ## 2 ## VALUEFIELDZ = lCurveZ->EvaluateRightDerivative(lCurrentTime,&keyindexz); \
*/						} \
						LOCALTYPE.transform.push_back(localtransf); \
					} \
					LOCALTYPE.transform_amount = LOCALTYPE.transform.size(); \
					model.animation[i].LOCALTYPE.push_back(LOCALTYPE); \
				} \
			} \
			model.animation[i].LOCALTYPE ## _amount = model.animation[i].LOCALTYPE.size();

			#define MACRO_FBX_COMPUTE_TANGENT_TRANSFORMATION(LOCALTYPE,ANIMLOCALTYPE,VALUE,VALUEFIELDX,VALUEFIELDY,VALUEFIELDZ) \
				for (j=0;j<model.animation[i].LOCALTYPE.size();j++) \
					for (k=0;k<model.animation[i].LOCALTYPE[j].transform.size();k++) { \
						ANIMLOCALTYPE& localtransf = model.animation[i].LOCALTYPE[j].transform[k]; \
						if (k>0) { \
							ANIMLOCALTYPE& lasttransf = model.animation[i].LOCALTYPE[j].transform[k-1]; \
							lasttransf.VALUE ## 2 ## VALUEFIELDX = (double)(localtransf.VALUE ## VALUEFIELDX - lasttransf.VALUE ## VALUEFIELDX)/(double)(localtransf.time - lasttransf.time); \
							lasttransf.VALUE ## 2 ## VALUEFIELDY = (double)(localtransf.VALUE ## VALUEFIELDY - lasttransf.VALUE ## VALUEFIELDY)/(double)(localtransf.time - lasttransf.time); \
							lasttransf.VALUE ## 2 ## VALUEFIELDZ = (double)(localtransf.VALUE ## VALUEFIELDZ - lasttransf.VALUE ## VALUEFIELDZ)/(double)(localtransf.time - lasttransf.time); \
							localtransf.VALUE ## 1 ## VALUEFIELDX = lasttransf.VALUE ## 2 ## VALUEFIELDX; \
							localtransf.VALUE ## 1 ## VALUEFIELDY = lasttransf.VALUE ## 2 ## VALUEFIELDY; \
							localtransf.VALUE ## 1 ## VALUEFIELDZ = lasttransf.VALUE ## 2 ## VALUEFIELDZ; \
							if (k==1) { \
								lasttransf.VALUE ## 1 ## VALUEFIELDX = lasttransf.VALUE ## 2 ## VALUEFIELDX; \
								lasttransf.VALUE ## 1 ## VALUEFIELDY = lasttransf.VALUE ## 2 ## VALUEFIELDY; \
								lasttransf.VALUE ## 1 ## VALUEFIELDZ = lasttransf.VALUE ## 2 ## VALUEFIELDZ; \
							} \
						} \
						if (k+1==model.animation[i].LOCALTYPE[j].transform.size()) { \
							localtransf.VALUE ## 1 ## VALUEFIELDX = 0.0; \
							localtransf.VALUE ## 2 ## VALUEFIELDX = 0.0; \
							localtransf.VALUE ## 1 ## VALUEFIELDY = 0.0; \
							localtransf.VALUE ## 2 ## VALUEFIELDY = 0.0; \
							localtransf.VALUE ## 1 ## VALUEFIELDZ = 0.0; \
							localtransf.VALUE ## 2 ## VALUEFIELDZ = 0.0; \
						} \
					}


		MACRO_FBX_RETRIEVE_TRANSFORMATION(localw,ModelAnimationTransformW,LclRotation,rot,.x,.y,.z)
		for (j=0;j<model.animation[i].localw.size();j++)
			for (k=0;k<model.animation[i].localw[j].transform.size();k++) {
				ModelAnimationTransformW& transform = model.animation[i].localw[j].transform[k];
				Quaternion::EulerToQuaternion(transform.rot,transform.rot.x,transform.rot.y,transform.rot.z);
				if (k>0) {
					ModelAnimationTransformW& prevtransform = model.animation[i].localw[j].transform[k-1];
					FbxQuaternion fbxquat, prevfbxquat, interpolatequat;
					transform.rot.TakeContinuousRepresentative(prevtransform.rot);
					fbxquat.Set(transform.rot.x,transform.rot.y,transform.rot.z,transform.rot.w);
					prevfbxquat.Set(prevtransform.rot.x,prevtransform.rot.y,prevtransform.rot.z,prevtransform.rot.w);
/*					FbxQuaternion zeroquat;
					zeroquat.Set(0.0,0.0,0.0);
					prevfbxquat.Conjugate();
					interpolatequat = zeroquat.Slerp(prevfbxquat.Product(fbxquat),(double)(1.0)/(double)(transform.time-prevtransform.time));*/
					interpolatequat = (fbxquat-prevfbxquat)*(double)1.0/(double)(transform.time-prevtransform.time);
					prevtransform.rot2.x = interpolatequat[0];
					prevtransform.rot2.y = interpolatequat[1];
					prevtransform.rot2.z = interpolatequat[2];
					prevtransform.rot2.w = interpolatequat[3];
					transform.rot1.x = interpolatequat[0];
					transform.rot1.y = interpolatequat[1];
					transform.rot1.z = interpolatequat[2];
					transform.rot1.w = interpolatequat[3];
					if (k==1) {
						prevtransform.rot1.x = interpolatequat[0];
						prevtransform.rot1.y = interpolatequat[1];
						prevtransform.rot1.z = interpolatequat[2];
						prevtransform.rot1.w = interpolatequat[3];
					}
				}
				if (k+1==model.animation[i].localw[j].transform.size()) {
					transform.rot1.x = 0.0;
					transform.rot1.y = 0.0;
					transform.rot1.z = 0.0;
					transform.rot1.w = 0.0;
					transform.rot2.x = 0.0;
					transform.rot2.y = 0.0;
					transform.rot2.z = 0.0;
					transform.rot2.w = 0.0;
				}
			}
		MACRO_FBX_RETRIEVE_TRANSFORMATION(localt,ModelAnimationTransformT,LclTranslation,trans,x,y,z)
		MACRO_FBX_COMPUTE_TANGENT_TRANSFORMATION(localt,ModelAnimationTransformT,trans,x,y,z)
		MACRO_FBX_RETRIEVE_TRANSFORMATION(locals,ModelAnimationTransformS,LclScaling,scale,x,y,z)
		MACRO_FBX_COMPUTE_TANGENT_TRANSFORMATION(locals,ModelAnimationTransformS,scale,x,y,z)
		model.animation[i].name = lAnimStack->GetNameOnly();
		model.animation[i].name_len = model.animation[i].name.length();
		model.animation[i].num_unk1 = 1;
		model.animation[i].num_unk2 = 1;
		model.animation[i].num_unk3 = 0;
		model.animation[i].num_unk4 = 0;
		model.animation[i].num_unk5 = 0;
		model.animation[i].frame_rate = 30.0;
		model.animation[i].num_unk6 = 0;
		model.animation[i].num_unk7 = 0x9B;
		model.animation[i].unk7.assign(model.animation[i].num_unk7,-1);
	}
	return true;
}

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene) {
	pManager = FbxManager::Create();
	if (!pManager) {
		exit(1);
	}
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());
	pScene = FbxScene::Create(pManager, "Scene");
	if (!pScene) {
		exit(1);
	}
}

void DestroySdkObjects(FbxManager* pManager) {
	if (pManager) pManager->Destroy();
}

void SetupAxisSystem(FbxScene*& sdkscene) {
	FbxAxisSystem lAxisSytemBase = sdkscene->GetGlobalSettings().GetAxisSystem();
	int lUpVectorSign = 1;
	int lFrontVectorSign = -1;
	FbxAxisSystem lAxisSytem(lAxisSytemBase.GetUpVector(lUpVectorSign),lAxisSytemBase.GetFrontVector(lFrontVectorSign),lAxisSytemBase.GetCoorSystem());
	lAxisSytem.ConvertScene(sdkscene);
}

bool SaveScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene, int format, bool pEmbedMedia) {
	bool lStatus = true;
	int pFileFormat = -1;
	char* outputfullname = new char[strlen(pFilename) + 5];
	char* outputext = &outputfullname[strlen(pFilename)];
	strcpy(outputfullname, pFilename);
	switch (format) {
	case MODEL_FILE_FORMAT_FBX_ASCII:
		strcpy(outputext, ".fbx");
		break;
	case MODEL_FILE_FORMAT_FBX_BINARY:
		strcpy(outputext, ".fbx");
		pFileFormat = pManager->GetIOPluginRegistry()->GetNativeWriterFormat();
		break;
	case MODEL_FILE_FORMAT_AUTOCAD:
		strcpy(outputext, ".dxf");
		break;
	case MODEL_FILE_FORMAT_COLLADA:
		strcpy(outputext, ".dae");
		break;
	case MODEL_FILE_FORMAT_WAVEFRONT:
		strcpy(outputext, ".obj");
		break;
	case MODEL_FILE_FORMAT_3DS_MAX:
		strcpy(outputext, ".3ds");
		break;
	}
	FbxExporter* lExporter = FbxExporter::Create(pManager, "");
	if (format == MODEL_FILE_FORMAT_FBX_ASCII/* || format == MODEL_FILE_FORMAT_FBX_BINARY*/) {
		pFileFormat = pManager->GetIOPluginRegistry()->GetNativeWriterFormat();
		int lFormatIndex, lFormatCount = pManager->GetIOPluginRegistry()->GetWriterFormatCount();
		for (lFormatIndex = 0; lFormatIndex<lFormatCount; lFormatIndex++) {
			if (pManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex)) {
				FbxString lDesc = pManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char *lFormatStr = (format == MODEL_FILE_FORMAT_FBX_ASCII ? "ascii" : "binary");
//				const char *lVersionStr = "6.0";
				if (lDesc.Find(lFormatStr) >= 0/* && lDesc.Find(lVersionStr) >= 0*/) {
					pFileFormat = lFormatIndex;
					break;
				}
			}
		}
	}
    // Set the export states.
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_MATERIAL,        true);
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_TEXTURE,         true);
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED,        pEmbedMedia);
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_SHAPE,           true);
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_GOBO,            true);
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_ANIMATION,       true);
    pManager->GetIOSettings()->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);
    // Initialize the exporter by providing a filename.
	if (lExporter->Initialize(outputfullname, pFileFormat, pManager->GetIOSettings()) == false) {
		return false;
	}
    // Export the scene.
	lStatus = lExporter->Export(pScene);
	lExporter->Destroy();
	return lStatus;
}

bool LoadScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene, bool retrieveanims) {
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	bool lStatus;
	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");
	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
	if (!lImportStatus) {
		return false;
	}
	if (lImporter->IsFBX()) {
        // Set the import states.
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL,        true);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE,         true);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK,            true);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE,           true);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO,            true);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION,       retrieveanims);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }
    // Import the scene.
	lStatus = lImporter->Import(pScene);
    /* Passworded models not supported
    char lPassword[1024];
    if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError) {
        FBXSDK_printf("Please enter password: ");
        lPassword[0] = '\0';
        FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
        scanf("%s", lPassword);
        FBXSDK_CRT_SECURE_NO_WARNING_END
        FbxString lString(lPassword);
        pManager->GetIOSettings()->SetStringProp(IMP_FBX_PASSWORD,      lString);
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);
        lStatus = lImporter->Import(pScene);
        if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError) {
            FBXSDK_printf("\nPassword is wrong, import aborted.\n");
        }
    }*/
	lImporter->Destroy();
	return lStatus;
}

//=============================//
//           Maths             //
//=============================//

#define DEG_TO_RAD 0.0174532925199432958
#define RAD_TO_DEG 57.295779513082320877

void Quaternion::Read(fstream& f) {
	float fx,fy,fz;
	ModelDataStruct::ReadCoordinates(f,fx,fy,fz,false);
	x = fx; y = fy; z = fz;
	w = ReadFloat(f);
	apply_matrix_updated = false;
}

void Quaternion::Write(fstream& f) {
	ModelDataStruct::WriteCoordinates(f,x,y,z,false);
	WriteFloat(f,w);
}

void Quaternion::EulerToQuaternion(Quaternion& q, double roll, double pitch, double yaw) {
/*	FbxVector4 eulervect(fmod(roll+180,360)-180,fmod(pitch,180),fmod(yaw+180,360)-180);
	FbxQuaternion fbxquat;
	fbxquat.ComposeSphericalXYZ(eulervect);
	q.x = fbxquat[0];
	q.y = fbxquat[1];
	q.z = fbxquat[2];
	q.w = fbxquat[3];*/
	double r = DEG_TO_RAD * roll;
	double p = DEG_TO_RAD * pitch;
	double y = DEG_TO_RAD * yaw;
	double cy = cos(y * 0.5);
	double sy = sin(y * 0.5);
	double cr = cos(r * 0.5);
	double sr = sin(r * 0.5);
	double cp = cos(p * 0.5);
	double sp = sin(p * 0.5);
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;
	q.w = cr * cp * cy + sr * sp * sy;
	q.apply_matrix_updated = false;
}

void Quaternion::QuaternionToEuler(Quaternion& q, double& roll, double& pitch, double& yaw) {
	FbxQuaternion fbxquat(q.x,q.y,q.z,q.w);
	FbxVector4 fbxangle = fbxquat.DecomposeSphericalXYZ();
	roll = fbxangle[0];
	pitch = fbxangle[1];
	yaw = fbxangle[2];
/*	double ysqr = q.y * q.y;
	// roll (x-axis rotation)
	double t0 = +2.0 * (q.w * q.x + q.y * q.z);
	double t1 = +1.0 - 2.0 * (q.x * q.x + ysqr);
	roll = RAD_TO_DEG * atan2(t0, t1);
	// pitch (y-axis rotation)
	double t2 = +2.0 * (q.w * q.y - q.z * q.x);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	pitch = RAD_TO_DEG * asin(t2);
	// yaw (z-axis rotation)
	double t3 = +2.0 * (q.w * q.z + q.x * q.y);
	double t4 = +1.0 - 2.0 * (ysqr + q.z * q.z);  
	yaw = RAD_TO_DEG * atan2(t3, t4);*/
}

LinearTransformationMatrix Quaternion::QuaternionToMatrix(Quaternion& q) {
	LinearTransformationMatrix res;
	unsigned int i,j;
	q.UpdateMatrixIfNecessary();
	for (i=0;i<3;i++) {
		for (j=0;j<3;j++)
			res.value[i][j] = q.apply_matrix[3*i+j];
		res.value[3][i] = 0.0;
		res.value[i][3] = 0.0;
	}
	res.value[3][3] = 1.0;
	return res;
}

void Quaternion::UpdateMatrixIfNecessary() {
	if (!apply_matrix_updated) {
		double dx = w*w + x*x - y*y - z*z;
		double dy = w*w - x*x + y*y - z*z;
		double dz = w*w - x*x - y*y + z*z;
		double xy = 2*x*y;
		double xz = 2*x*z;
		double yz = 2*y*z;
		double wx = 2*w*x;
		double wy = 2*w*y;
		double wz = 2*w*z;
		apply_matrix[0] = dx;
		apply_matrix[1] = xy-wz;
		apply_matrix[2] = xz+wy;
		apply_matrix[3] = xy+wz;
		apply_matrix[4] = dy;
		apply_matrix[5] = yz-wx;
		apply_matrix[6] = xz-wy;
		apply_matrix[7] = yz+wx;
		apply_matrix[8] = dz;
		apply_matrix_updated = true;
	}
}

double Quaternion::GetRoll() {
	FbxQuaternion fbxquat(x,y,z,w);
	FbxVector4 fbxangle = fbxquat.DecomposeSphericalXYZ();
	return fbxangle[0];
/*	double ysqr = y*y;
	double t0 = +2.0 * (w*x + y*z);
	double t1 = +1.0 - 2.0 * (x*x + ysqr);
	return RAD_TO_DEG * atan2(t0, t1);*/
}

double Quaternion::GetPitch() {
	FbxQuaternion fbxquat(x,y,z,w);
	FbxVector4 fbxangle = fbxquat.DecomposeSphericalXYZ();
	return fbxangle[1];
/*	double t2 = +2.0 * (w*y - z*x);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	return RAD_TO_DEG * asin(t2);*/
}

double Quaternion::GetYaw() {
	FbxQuaternion fbxquat(x,y,z,w);
	FbxVector4 fbxangle = fbxquat.DecomposeSphericalXYZ();
	return fbxangle[2];
/*	double ysqr = y*y;
	double t3 = +2.0 * (w*z + x*y);
	double t4 = +1.0 - 2.0 * (ysqr + z*z);  
	return RAD_TO_DEG * atan2(t3, t4);*/
}

void Quaternion::SetValue(double newx, double newy, double newz, double neww) {
	x = newx;
	y = newy;
	z = newz;
	w = neww;
	apply_matrix_updated = false;
}

void Quaternion::Apply(double& posx, double& posy, double& posz) {
	double px = posx;
	double py = posy;
	double pz = posz;
	UpdateMatrixIfNecessary();
	posx = apply_matrix[0]*px + apply_matrix[1]*py + apply_matrix[2]*pz;
	posy = apply_matrix[3]*px + apply_matrix[4]*py + apply_matrix[5]*pz;
	posz = apply_matrix[6]*px + apply_matrix[7]*py + apply_matrix[8]*pz;
}

void Quaternion::TakeContinuousRepresentative(Quaternion closequaternion) {
	double norm1 = (closequaternion.x-x)*(closequaternion.x-x)+(closequaternion.y-y)*(closequaternion.y-y)+(closequaternion.z-z)*(closequaternion.z-z)+(closequaternion.w-w)*(closequaternion.w-w);
	double norm2 = (closequaternion.x+x)*(closequaternion.x+x)+(closequaternion.y+y)*(closequaternion.y+y)+(closequaternion.z+z)*(closequaternion.z+z)+(closequaternion.w+w)*(closequaternion.w+w);
	if (norm1>norm2) {
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}
}

Quaternion Quaternion::Product(Quaternion& lfactor, Quaternion& rfactor) {
	Quaternion res;
	res.w = lfactor.w*rfactor.w - lfactor.x*rfactor.x - lfactor.y*rfactor.y - lfactor.z*rfactor.z;
	res.x = lfactor.w*rfactor.x + lfactor.x*rfactor.w + lfactor.y*rfactor.z - lfactor.z*rfactor.y;
	res.y = lfactor.w*rfactor.y + lfactor.y*rfactor.w + lfactor.z*rfactor.x - lfactor.x*rfactor.z;
	res.z = lfactor.w*rfactor.z + lfactor.z*rfactor.w + lfactor.x*rfactor.y - lfactor.y*rfactor.x;
	res.apply_matrix_updated = false;
	return res;
}

LinearTransformationMatrix Quaternion::Product(LinearTransformationMatrix lfactor, Quaternion& rfactor) {
	LinearTransformationMatrix res;
	unsigned int i,j;
	rfactor.UpdateMatrixIfNecessary();
	for (i=0;i<4;i++) {
		for (j=0;j<3;j++)
			res.value[i][j] = lfactor.value[i][0]*rfactor.apply_matrix[j]+lfactor.value[i][1]*rfactor.apply_matrix[3+j]+lfactor.value[i][2]*rfactor.apply_matrix[6+j];
		res.value[i][3] = lfactor.value[i][3];
	}
	return res;
}

LinearTransformationMatrix Quaternion::Product(Quaternion& lfactor, LinearTransformationMatrix rfactor) {
	LinearTransformationMatrix res;
	unsigned int i,j;
	lfactor.UpdateMatrixIfNecessary();
	for (j=0;j<4;j++) {
		for (i=0;i<3;i++)
			res.value[i][j] = lfactor.apply_matrix[3*i]*rfactor.value[0][j]+lfactor.apply_matrix[3*i+1]*rfactor.value[1][j]+lfactor.apply_matrix[3*i+2]*rfactor.value[2][j];
		res.value[3][j] = rfactor.value[3][j];
	}
	return res;
}

LinearTransformationMatrix Quaternion::Product(LinearTransformationMatrix lfactor, LinearTransformationMatrix rfactor) {
	LinearTransformationMatrix res;
	unsigned int i,j,k;
	for (j=0;j<4;j++)
		for (i=0;i<j;i++) {
			res.value[i][j] = 0.0;
			for (k=0;k<4;k++)
				res.value[i][j] += lfactor.value[i][k]*rfactor.value[k][j];
		}
	return res;
}
