#include "ModelMesh.h"

#include "fbxsdk.h"

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager);
void SetupAxisSystem(FbxScene*& sdkscene);
bool SaveScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene, int format = MODEL_FILE_FORMAT_FBX_ASCII, bool pEmbedMedia = false);
bool LoadScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene);
bool ConvertModelToFBX(ModelDataStruct& model, FbxManager*& sdkmanager, FbxScene*& sdkscene);

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
	unsigned int i;
	size_t headerpos = f.tellg();
//fstream fout("aaaa.txt",ios::out|ios::app);
	material_info_amount = ReadLong(f);
//fout << "material_info_amount: " << (unsigned int)material_info_amount << endl;
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
//fout << "bone_amount: " << (unsigned int)bone_amount << endl;
	bone.reserve(bone_amount);
	for (i=0;i<bone_amount;i++) {
		bonebuffer.unk_p1 = ReadFloat(f);
		bonebuffer.unk_p2 = ReadFloat(f);
		bonebuffer.unk_p3 = ReadFloat(f);
		bonebuffer.unk_f1 = ReadFloat(f);
		bonebuffer.unk_p4 = ReadFloat(f);
		bonebuffer.unk_p5 = ReadFloat(f);
		bonebuffer.unk_p6 = ReadFloat(f);
		bonebuffer.unk_f2 = ReadFloat(f);
		bonebuffer.unk_p7 = ReadFloat(f);
		bonebuffer.unk_p8 = ReadFloat(f);
		bonebuffer.unk_p9 = ReadFloat(f);
		bonebuffer.unk_f3 = ReadFloat(f);
		bonebuffer.unk_z1 = ReadLong(f);
		bonebuffer.unk_z2 = ReadLong(f);
		bonebuffer.unk_z3 = ReadLong(f);
		bonebuffer.unk_one = ReadFloat(f);
		bone.push_back(bonebuffer);
	}
	bone_unk_amount = ReadLong(f);
//fout << "bone_unk_amount: " << (unsigned int)bone_unk_amount << endl;
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
//fout << "vertex_list_amount: " << (unsigned int)vertex_list_amount << endl;
	vert_list.reserve(vertex_list_amount);
	for (i=0;i<vertex_list_amount;i++)
		vert_list.push_back(ReadShort(f));
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	vertice_attachment_amount = ReadLong(f);
//fout << "vertice_attachment_amount: " << (unsigned int)vertice_attachment_amount << endl;
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
//fout << "vertice_attachment_amount: " << (unsigned int)vertice_amount << endl;
	vert_format1 = ReadLong(f);
	vert_format2 = ReadLong(f);
	vert_format3 = ReadLong(f);
	vert_format4 = ReadLong(f);
	vert_format5 = ReadLong(f);
	vert_format6 = ReadLong(f);
	vert_format7 = ReadLong(f);
	vert_format8 = ReadLong(f);
	vert_format9 = ReadLong(f);
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
		vertbuffer.unkp1 = ReadFloat(f);
		vertbuffer.unkp2 = ReadFloat(f);
		vertbuffer.unkp3 = ReadFloat(f);
		vertbuffer.unkp4 = ReadFloat(f);
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
//fout.close();
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

void ModelAnimationData::Read(fstream& f, GameObjectHierarchy* gohier) {
	unsigned int i,j,k,t;
	name_len = ReadLong(f);
	name = "";
	for (i=0;i<name_len;i++)
		name.push_back(f.get());
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	num_unk1 = ReadShort(f);
	num_unk2 = ReadShort(f);
//fstream fout("aaaa.txt",ios::out|ios::app);

	#define MACRO_IOTRANSFORM_BEGIN(LOCALTYPE) \
		LOCALTYPE ## _amount = ReadLong(f); \
/*fout << #LOCALTYPE": " << (unsigned int)LOCALTYPE ## _amount << endl;*/ \
		LOCALTYPE.resize(LOCALTYPE ## _amount); \
		for (i=0;i<LOCALTYPE ## _amount;i++) { \
			LOCALTYPE[i].transform_amount = ReadLong(f); \
/*fout << #LOCALTYPE" transform: " << (unsigned int)LOCALTYPE[i].transform_amount << endl;*/ \
			LOCALTYPE[i].transform.resize(LOCALTYPE[i].transform_amount); \
			for (j=0;j<LOCALTYPE[i].transform_amount;j++) {

	#define MACRO_IOTRANSFORM_END(LOCALTYPE) \
			} \
			LOCALTYPE[i].unkint1 = ReadLong(f); \
			LOCALTYPE[i].unkint2 = ReadLong(f); \
			LOCALTYPE[i].object_name_len = ReadLong(f); \
			LOCALTYPE[i].object_name = ""; \
			for (j=0;j<LOCALTYPE[i].object_name_len;j++) \
				LOCALTYPE[i].object_name.push_back(f.get()); \
			f.seekg(GetAlignOffset(f.tellg()),ios::cur); \
/*fout << #LOCALTYPE" name: " << LOCALTYPE[i].object_name.c_str() << endl;*/ \
		}

	MACRO_IOTRANSFORM_BEGIN(localw)
		localw[i].transform[j].time = ReadFloat(f);
		localw[i].transform[j].rot.Read(f);
		localw[i].transform[j].rot1.Read(f);
		localw[i].transform[j].rot2.Read(f);
	MACRO_IOTRANSFORM_END(localw)
	num_unk3 = ReadLong(f);
	MACRO_IOTRANSFORM_BEGIN(localt)
		localt[i].transform[j].time = ReadFloat(f);
		ModelDataStruct::ReadCoordinates(f,localt[i].transform[j].transx,localt[i].transform[j].transy,localt[i].transform[j].transz);
		ModelDataStruct::ReadCoordinates(f,localt[i].transform[j].transx1,localt[i].transform[j].transy1,localt[i].transform[j].transz1);
		ModelDataStruct::ReadCoordinates(f,localt[i].transform[j].transx2,localt[i].transform[j].transy2,localt[i].transform[j].transz2);
	MACRO_IOTRANSFORM_END(localt)
	MACRO_IOTRANSFORM_BEGIN(locals)
		locals[i].transform[j].time = ReadFloat(f);
		ModelDataStruct::ReadCoordinates(f,locals[i].transform[j].scalex,locals[i].transform[j].scaley,locals[i].transform[j].scalez,false);
		ModelDataStruct::ReadCoordinates(f,locals[i].transform[j].scalex1,locals[i].transform[j].scaley1,locals[i].transform[j].scalez1,false);
		ModelDataStruct::ReadCoordinates(f,locals[i].transform[j].scalex2,locals[i].transform[j].scaley2,locals[i].transform[j].scalez2,false);
	MACRO_IOTRANSFORM_END(locals)
	num_unk4 = ReadLong(f);
	num_unk5 = ReadLong(f);
	float_unk = ReadFloat(f);
	num_unk6 = ReadLong(f);
//fout.close();
	// ToDo: read the end
	// In FBX, the translations are done using the global coordinates
	// In Steam assets, they are done using the node's orientation
	vector<int> nodeparentindex;
	vector<FbxString> nodename;
	vector<Quaternion> nodedefaultglobalrot;
	vector<Quaternion> nodeglobalrot;
	FbxString nodefullname, nodenewname;
	unsigned int tokenamount;
	bool foundflag;
	// 1st step: create the list of needed nodes and parent hierarchy
	// Note: a parent node is always before a child in the vector lists
	vector<unsigned int> localtnodeindex;
	localtnodeindex.resize(localt_amount);
	for (i=0;i<localt_amount;i++) {
		nodefullname = FbxString(localt[i].object_name.c_str());
		tokenamount = nodefullname.GetTokenCount("/");
		for (j=0;j<tokenamount;j++) {
			nodenewname = nodefullname.GetToken(j,"/");
			foundflag = false;
			for (k=0;k<nodename.size();k++)
				if (nodename[k]==nodenewname) {
					foundflag = true;
					if (j+1==tokenamount)
						localtnodeindex[i] = k;
					break;
				}
			if (!foundflag) {
				if (j+1==tokenamount)
					localtnodeindex[i] = nodename.size();
				nodename.push_back(nodenewname);
			}
		}
		if (tokenamount<=1) {
			nodeparentindex.push_back(-1);
		} else {
			nodenewname = nodefullname.GetToken(tokenamount-2,"/");
			foundflag = false;
			for (k=0;k<nodename.size();k++)
				if (nodenewname==nodename[k]) {
					nodeparentindex.push_back(k);
					foundflag = true;
					break;
				}
			if (!foundflag)
				nodeparentindex.push_back(-1);
		}
	}
	// 2nd step: retrieve the default orientation from GameObjectHierarchy
	nodedefaultglobalrot.resize(nodename.size());
	if (gohier) {
		for (i=0;i<nodedefaultglobalrot.size();i++) {
			foundflag = false;
			for (j=0;gohier->node_list.size();j++)
				if (gohier->node_list[j]->node_type==4 /*
				*/ && static_cast<TransformStruct*>(gohier->node_list[j])->child_object && static_cast<TransformStruct*>(gohier->node_list[j])->child_object->node_type==1 /*
				*/ && nodename[i]==FbxString(static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(gohier->node_list[j])->child_object)->name.c_str())) {
					nodedefaultglobalrot[i] = static_cast<TransformStruct*>(gohier->node_list[j])->rot;
					foundflag = true;
					break;
				}
			if (!foundflag) {
				nodedefaultglobalrot[i].x = 0.0;
				nodedefaultglobalrot[i].y = 0.0;
				nodedefaultglobalrot[i].z = 0.0;
				nodedefaultglobalrot[i].w = 1.0;
			}
		}
	} else {
		for (i=0;i<nodedefaultglobalrot.size();i++) {
			nodedefaultglobalrot[i].x = 0.0;
			nodedefaultglobalrot[i].y = 0.0;
			nodedefaultglobalrot[i].z = 0.0;
			nodedefaultglobalrot[i].w = 1.0;
		}
	}
	// 3rd step: compute the orientation at each time and apply it to the translation
	vector<float> timelist;
	float newtime;
	for (i=0;i<localt_amount;i++)
		for (j=0;j<localt[i].transform_amount;j++) {
			newtime = localt[i].transform[j].time;
			foundflag = false;
			for (k=0;k<timelist.size();k++)
				if (newtime==timelist[k]) {
					foundflag = true;
					break;
				}
			if (!foundflag)
				timelist.push_back(newtime);
		}
	for (t=0;t<timelist.size();t++) {
		nodeglobalrot.clear();
		nodeglobalrot.resize(nodename.size());
		for (k=0;k<nodeglobalrot.size();k++) {
			nodeglobalrot[k] = nodedefaultglobalrot[k];
			for (i=0;i<localw_amount;i++)
				if (FbxString(localw[i].object_name.c_str()).GetToken(FbxString(localw[i].object_name.c_str()).GetTokenCount("/"),"/")==nodename[k]) {
					for (j=0;j<localw[i].transform_amount;j++)
						if (localw[i].transform[j].time==timelist[t]) {
							nodeglobalrot[k] = localw[i].transform[j].rot;
							break;
						}
					break;
				}
			if (nodeparentindex[k]>=0)
				nodeglobalrot[k] = Quaternion::Product(nodeglobalrot[nodeparentindex[k]],nodeglobalrot[k]);
		}
		for (i=0;i<localt_amount;i++)
			for (j=0;j<localt[i].transform_amount;j++)
				if (localt[i].transform[j].time==timelist[t]) {
					nodeglobalrot[localtnodeindex[i]].Apply(localt[i].transform[j].transx,localt[i].transform[j].transy,localt[i].transform[j].transz);
					nodeglobalrot[localtnodeindex[i]].Apply(localt[i].transform[j].transx1,localt[i].transform[j].transy1,localt[i].transform[j].transz1);
					nodeglobalrot[localtnodeindex[i]].Apply(localt[i].transform[j].transx2,localt[i].transform[j].transy2,localt[i].transform[j].transz2);
				}
	}
/*	for (i=0;i<localt_amount;i++)
		for (k=0;k<localw_amount;k++)
			if (localw[k].object_name==localt[i].object_name) {
				for (j=0;j<localt[i].transform_amount;j++)
					for (l=0;l<localw[k].transform_amount;l++)
						if (localw[k].transform[l].time==localt[i].transform[j].time) {
							localw[k].transform[l].rot.Apply(localt[i].transform[j].transx,localt[i].transform[j].transy,localt[i].transform[j].transz);
							localw[k].transform[l].rot.Apply(localt[i].transform[j].transx1,localt[i].transform[j].transy1,localt[i].transform[j].transz1);
							localw[k].transform[l].rot.Apply(localt[i].transform[j].transx2,localt[i].transform[j].transy2,localt[i].transform[j].transz2);
							break;
						}
				break;
			}*/
}

void ModelDataStruct::ReadCoordinates(fstream& f, float& x, float& y, float& z, bool swapsign) {
	float sign = swapsign ? 1.0 : 1.0;
	x = sign*ReadFloat(f);
	y = sign*ReadFloat(f);
	z = sign*ReadFloat(f);
}

void ModelDataStruct::WriteCoordinates(fstream& f, float x, float y, float z, bool swapsign) {
	float sign = swapsign ? -1.0 : 1.0;
	WriteFloat(f,sign*x);
	WriteFloat(f,sign*z);
	WriteFloat(f,sign*y);
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
				matdata.Read(f);
				matdata.name = meta.file_name[node->child_material[j]->file_index];
				int32_t texfileid = meta.GetFileIndexByInfo(matdata.maintex_file_info);
				matdata.maintex_file_name = (texfileid>=0 ? meta.file_name[texfileid]+".png" : "TextureNotFound");
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
				matdata.Read(f);
				matdata.name = meta.file_name[node->child_material[j]->file_index];
				int32_t texfileid = meta.GetFileIndexByInfo(matdata.maintex_file_info);
				matdata.maintex_file_name = (texfileid>=0 ? meta.file_name[texfileid]+".png" : "TextureNotFound");
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
	FbxCluster* lSingleSkeletonCluster = NULL;
	FbxNode* lSingleSkeletonNode = NULL;
	vector<FbxSkin*> lMultiSkin;
	vector<FbxSkeleton*> lMultiSkeleton;
/*	vector<FbxCluster*> lMultiSkeletonCluster;*/
	vector<string> lMultiSkeletonName;
	bool hasbones = false;
	for (i=0;i<model.mesh.size();i++)
		if (model.mesh[i].bone_amount>0) {
			hasbones = true;
			break;
		}
	if (model.animation.size()>0 && !hasbones) {
		lSingleSkeleton = FbxSkeleton::Create(sdkscene, "");
		lSingleSkeletonNode = FbxNode::Create(sdkscene, "");
		lSingleSkeletonCluster = FbxCluster::Create(sdkscene, "");
		lSingleSkeleton->SetSkeletonType(FbxSkeleton::eRoot);
		lSingleSkeletonNode->SetNodeAttribute(lSingleSkeleton);
		lSingleSkeletonCluster->SetLink(lSingleSkeletonNode);
		lSingleSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);
	}
	// Construct all the Fbx nodes
	vector<FbxNode*> lNodeList;
	vector<GameObjectNode*> hierarchynode = model.hierarchy->node_list;
	for (i=0;i<hierarchynode.size();i++) {
		if (hierarchynode[i]==model.hierarchy->root_node) {
			if (hierarchynode[i]->node_type==4) { // Should be always true
				TransformStruct* nodespec = static_cast<TransformStruct*>(hierarchynode[i]);
//{fstream fout("aaaa.txt",ios::app|ios::out); fout << "ROOT NODE : " << (unsigned int)nodespec->file_index << endl; fout.close();}
				lRootNode->LclRotation.Set(FbxDouble3(nodespec->rot.GetRoll(), nodespec->rot.GetPitch(), nodespec->rot.GetYaw()));
				lRootNode->LclTranslation.Set(FbxDouble3(nodespec->x, nodespec->y, nodespec->z));
				lRootNode->LclScaling.Set(FbxDouble3(nodespec->scale_x, nodespec->scale_y, nodespec->scale_z));
			}
			lNodeList.push_back(lRootNode);
			continue;
		}
		if (hierarchynode[i]->node_type==4) {
			// Simple node
			string nodename = "";
			TransformStruct* nodespec = static_cast<TransformStruct*>(hierarchynode[i]);
			if (nodespec->child_object && nodespec->child_object->node_type==1)
				nodename = static_cast<GameObjectStruct*>(nodespec->child_object)->name;
//{fstream fout("aaaa.txt",ios::app|ios::out); fout << nodename.c_str() << " : " << (unsigned int)nodespec->file_index << endl; fout.close();}
			FbxNode* lNode = FbxNode::Create(sdkscene, FbxString(nodename.c_str()));
			lNode->LclRotation.Set(FbxDouble3(nodespec->rot.GetRoll(), nodespec->rot.GetPitch(), nodespec->rot.GetYaw()));
			lNode->LclTranslation.Set(FbxDouble3(nodespec->x, nodespec->y, nodespec->z));
			lNode->LclScaling.Set(FbxDouble3(nodespec->scale_x, nodespec->scale_y, nodespec->scale_z));
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
		} else if (hierarchynode[i]->node_type==137) {
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
							/*FbxCluster* lSkeletonCluster = FbxCluster::Create(sdkscene, "");*/ \
							if (parenttransf->parent_transform==NULL || parenttransf->parent_transform==model.hierarchy->root_node) \
								lSkeleton->SetSkeletonType(FbxSkeleton::eRoot); \
							else \
								lSkeleton->SetSkeletonType(FbxSkeleton::eLimbNode); \
							for (k=0;k<hierarchynode.size();k++) \
								if (parenttransf==hierarchynode[k] && lNodeList[k]) { \
									lNodeList[k]->SetNodeAttribute(lSkeleton); \
									/*lSkeletonCluster->SetLink(lNodeList[k]);*/ \
									break; \
								} \
							lSkeleton->Size.Set(1.0); \
							/*lSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);*/ \
							lMultiSkeleton.push_back(lSkeleton); \
							/*lMultiSkeletonCluster.push_back(lSkeletonCluster);*/ \
							lMultiSkeletonName.push_back(BONENAME); \
						} \
					}

				string bonename = static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(nodespec->child_bone[j])->child_object)->GetScopedName();
				MACRO_CONSTRUCT_NEWBONE(bonename)
			}
		}
	}
	// Construct more bones
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
				FbxVector4 lNormal;
				FbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
				lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
				lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eDirect);
				FbxVector2 lUVPoint;
				for (j = 0; j<mesh.vert.size(); j++) {
					lControlPoints[j].Set(mesh.vert[j].x, mesh.vert[j].y, mesh.vert[j].z);
					lNormal.Set(mesh.vert[j].nx, mesh.vert[j].ny, mesh.vert[j].nz);
					lGeometryElementNormal->GetDirectArray().Add(lNormal);
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
					lMesh->AddPolygon(vertid[0], vertid[0]);
					lMesh->AddPolygon(vertid[1], vertid[1]);
					lMesh->AddPolygon(vertid[2], vertid[2]);
					lMesh->EndPolygon();
				}
				lMesh->GenerateNormals(false,true);
				lMeshNode->SetNodeAttribute(lMesh);
				lMeshNode->SetShadingMode(FbxNode::eTextureShading);
				// Materials and Textures
				vector<ModelMaterialData>& material = model.material[meshindex];
				for (j = 0; j < material.size(); j++) {
					FbxString lTextureName = material[j].maintex_file_name.c_str();
					FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(sdkscene, material[j].name.c_str());
					lMeshNode->AddMaterial(lMaterial);
					FbxFileTexture* lFileTex = FbxFileTexture::Create(sdkscene, lTextureName);
					lFileTex->SetFileName(lTextureName);
					lFileTex->SetRelativeFileName(lTextureName);
					lFileTex->SetTextureUse(FbxTexture::eStandard);
					lFileTex->SetMappingType(FbxTexture::eUV);
					lFileTex->SetMaterialUse(FbxFileTexture::eModelMaterial);
					lFileTex->UVSet.Set(FbxString("DiffuseUV"));
					lMaterial->Diffuse.ConnectSrcObject(lFileTex);
//					FbxProperty lProp = FbxProperty::Create(lMaterial, FbxDouble3DT, "EnvSampler", "SamplerTexture");
//					FbxDouble3 lSampleVal(0, 0, 0);
//					lProp.Set(lSampleVal);
//					lFileTex->ConnectDstProperty(lProp);
//					lMaterial->ConnectSrcObject(lFileTex);
				}
				// Link points to Skeleton Clusters
				if (model.animation.size()>0 || hasbones) {
					FbxSkin* lSkin = FbxSkin::Create(sdkscene, "");
					lSkin->SetGeometry(lMesh);
					if (model.animation.size()>0 && !hasbones) {
						for (j = 0; j<mesh.vert.size(); j++)
							lSingleSkeletonCluster->AddControlPointIndex(j, 1.0);
						lSkin->AddCluster(lSingleSkeletonCluster);
					} else {
						for (j = 0; j < mesh.bone_amount; j++) {
/*							FbxCluster* lSkeletonCluster = NULL;
							for (k=0;k<lMultiSkeleton.size();k++)
								if (lMultiSkeletonName[k]==mesh.bone[j].scoped_name) {
									lSkeletonCluster = lMultiSkeletonCluster[k];
									break;
								}*/
							FbxCluster* lSkeletonCluster = FbxCluster::Create(sdkscene, ""); // DEBUG
							for (k=0;k<lMultiSkeletonName.size();k++) // DEBUG
								if (lMultiSkeletonName[k]==mesh.bone[j].scoped_name) { // DEBUG
									lSkeletonCluster->SetLink(lMultiSkeleton[k]->GetNode());
									break;
								}
							lSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne); // DEBUG
							if (lSkeletonCluster) {
								for (k = 0; k < mesh.vertice_attachment_amount; k++)
									for (l = 0; l < 4; l++)
										if (mesh.vert_attachment[k].bone_id[l]==j && mesh.vert_attachment[k].bone_factor[l] > 0.0)
											lSkeletonCluster->AddControlPointIndex(k, mesh.vert_attachment[k].bone_factor[l]);
								lSkin->AddCluster(lSkeletonCluster);
							}
						}
					}
					FbxAMatrix lTransformMatrix = lMeshNode->EvaluateGlobalTransform();
					for (j=0;j<lSkin->GetClusterCount();j++)
						lSkin->GetCluster(j)->SetTransformMatrix(lTransformMatrix);
					for (j=0;j<lSkin->GetClusterCount();j++) {
						lTransformMatrix = lSkin->GetCluster(j)->GetLink()->EvaluateGlobalTransform();
						lSkin->GetCluster(j)->SetTransformLinkMatrix(lTransformMatrix);
					}
					lMesh->AddDeformer(lSkin);
				}
			}
		}
	}
	// Animations
	FbxTime lTime;
	int lKeyIndex;
    for (i=0;i<model.animation.size();i++) {
		FbxAnimStack* lAnimStack = FbxAnimStack::Create(sdkscene, model.animation[i].name.c_str());
		FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(sdkscene, "Base Layer");
		FbxAnimCurve* lCurve;
		lAnimStack->AddMember(lAnimLayer);
		for (j=0;j<model.animation[i].localw_amount;j++) {

			#define MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,COMPONENTNAME,VALUE) \
				lCurve = lSkeletonNode->LCLTYPE.GetCurve(lAnimLayer, COMPONENTNAME, true); \
				if (lCurve) { \
					lCurve->KeyModifyBegin(); \
					for (k=0;k<model.animation[i].LOCALTYPE[j].transform_amount;k++) { \
						lTime.SetSecondDouble(model.animation[i].LOCALTYPE[j].transform[k].time); \
						lKeyIndex = lCurve->KeyAdd(lTime); \
						lCurve->KeySetValue(lKeyIndex, model.animation[i].LOCALTYPE[j].transform[k].VALUE); \
						lCurve->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear); \
					} \
					lCurve->KeyModifyEnd(); \
				}

			#define MACRO_FBX_APPLY_TRANSFORMATION(LOCALTYPE,LCLTYPE,X,Y,Z) \
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
				MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,FBXSDK_CURVENODE_COMPONENT_X,X) \
				MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,FBXSDK_CURVENODE_COMPONENT_Y,Y) \
				MACRO_FBX_APPLY_SINGLETRANSFORMATION(LOCALTYPE,LCLTYPE,FBXSDK_CURVENODE_COMPONENT_Z,Z)
			
			MACRO_FBX_APPLY_TRANSFORMATION(localw,LclRotation,rot.GetRoll(),rot.GetPitch(),rot.GetYaw())
		}
		for (j=0;j<model.animation[i].localt_amount;j++) {
			MACRO_FBX_APPLY_TRANSFORMATION(localt,LclTranslation,transx,transy,transz)
		}
		for (j=0;j<model.animation[i].locals_amount;j++) {
			MACRO_FBX_APPLY_TRANSFORMATION(locals,LclScaling,scalex,scaley,scalez)
		}
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

bool LoadScene(const char* pFilename, FbxManager* pManager, FbxDocument* pScene) {
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
        pManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION,       true);
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
	ModelDataStruct::ReadCoordinates(f,x,y,z);
	w = ReadFloat(f);
	apply_matrix_updated = false;
}

void Quaternion::Write(fstream& f) {
	ModelDataStruct::WriteCoordinates(f,x,y,z);
	WriteFloat(f,w);
}

Quaternion Quaternion::EulerToQuaternion(float roll, float pitch, float yaw) {
	double r = DEG_TO_RAD * roll;
	double p = DEG_TO_RAD * pitch;
	double y = DEG_TO_RAD * yaw;
	Quaternion q;
	double t0 = cos(y * 0.5);
	double t1 = sin(y * 0.5);
	double t2 = cos(r * 0.5);
	double t3 = sin(r * 0.5);
	double t4 = cos(p * 0.5);
	double t5 = sin(p * 0.5);
	q.w = t0 * t2 * t4 + t1 * t3 * t5;
	q.x = t0 * t3 * t4 - t1 * t2 * t5;
	q.y = t0 * t2 * t5 + t1 * t3 * t4;
	q.z = t1 * t2 * t4 - t0 * t3 * t5;
	return q;
}

void Quaternion::QuaternionToEuler(const Quaternion q, float& roll, float& pitch, float& yaw) {
	double ysqr = q.y * q.y;
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
	yaw = RAD_TO_DEG * atan2(t3, t4);
}

float Quaternion::GetRoll() {
	double ysqr = y*y;
	double t0 = +2.0 * (w*x + y*z);
	double t1 = +1.0 - 2.0 * (x*x + ysqr);
	return RAD_TO_DEG * atan2(t0, t1);
}

float Quaternion::GetPitch() {
	double t2 = +2.0 * (w*y - z*x);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	return RAD_TO_DEG * asin(t2);
}

float Quaternion::GetYaw() {
	double ysqr = y*y;
	double t3 = +2.0 * (w*z + x*y);
	double t4 = +1.0 - 2.0 * (ysqr + z*z);  
	return RAD_TO_DEG * atan2(t3, t4);
}

void Quaternion::SetValue(float newx, float newy, float newz, float neww) {
	x = newx;
	y = newy;
	z = newz;
	w = neww;
	apply_matrix_updated = false;
}

void Quaternion::Apply(float& posx, float& posy, float& posz) {
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
	float px = posx;
	float py = posy;
	float pz = posz;
	posx = apply_matrix[0]*px + apply_matrix[1]*py + apply_matrix[2]*pz;
	posy = apply_matrix[3]*px + apply_matrix[4]*py + apply_matrix[5]*pz;
	posz = apply_matrix[6]*px + apply_matrix[7]*py + apply_matrix[8]*pz;
}

Quaternion Quaternion::Product(Quaternion lfactor, Quaternion rfactor) {
	Quaternion res;
	res.w = lfactor.w*rfactor.w - lfactor.x*rfactor.x - lfactor.y*rfactor.y - lfactor.z*rfactor.z;
	res.x = lfactor.w*rfactor.x + lfactor.x*rfactor.w + lfactor.y*rfactor.z - lfactor.z*rfactor.y;
	res.y = lfactor.w*rfactor.y + lfactor.y*rfactor.w + lfactor.z*rfactor.x - lfactor.x*rfactor.z;
	res.z = lfactor.w*rfactor.z + lfactor.z*rfactor.w + lfactor.x*rfactor.y - lfactor.y*rfactor.x;
	res.apply_matrix_updated = false;
	return res;
}
