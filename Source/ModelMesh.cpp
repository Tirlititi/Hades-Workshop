#include "ModelMesh.h"

#include "fbxsdk.h"

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager);
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
fstream fout("aaaa.txt",ios::out|ios::app);
	material_info_amount = ReadLong(f);
fout << "material_info_amount: " << (unsigned int)material_info_amount << endl;
	mat_info.reserve(material_info_amount);
	for (i=0;i<material_info_amount;i++) {
		matinfobuffer.vert_list_start = ReadLong(f);
		matinfobuffer.vert_list_amount = ReadLong(f);
		matinfobuffer.unk0 = ReadLong(f);
		matinfobuffer.vert_start = ReadLong(f);
		matinfobuffer.vert_amount = ReadLong(f);
		matinfobuffer.center_x = ReadFloat(f);
		matinfobuffer.center_y = ReadFloat(f);
		matinfobuffer.center_z = ReadFloat(f);
		matinfobuffer.radius_x = ReadFloat(f);
		matinfobuffer.radius_y = ReadFloat(f);
		matinfobuffer.radius_z = ReadFloat(f);
		mat_info.push_back(matinfobuffer);
	}
	f.seekg(0x10,ios::cur);
	bone_amount = ReadLong(f);
fout << "bone_amount: " << (unsigned int)bone_amount << endl;
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
fout << "bone_unk_amount: " << (unsigned int)bone_unk_amount << endl;
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
fout << "vertex_list_amount: " << (unsigned int)vertex_list_amount << endl;
	vert_list.reserve(vertex_list_amount);
	for (i=0;i<vertex_list_amount;i++)
		vert_list.push_back(ReadShort(f));
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	vertice_attachment_amount = ReadLong(f);
fout << "vertice_attachment_amount: " << (unsigned int)vertice_attachment_amount << endl;
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
fout << "vertice_attachment_amount: " << (unsigned int)vertice_amount << endl;
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
	center_x = ReadFloat(f);
	center_y = ReadFloat(f);
	center_z = ReadFloat(f);
	radius_x = ReadFloat(f);
	radius_y = ReadFloat(f);
	radius_z = ReadFloat(f);
	unk_num1 = ReadLong(f);
	unk_num2 = ReadLong(f);
	unk_num3 = ReadLong(f);
fout.close();
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

void ModelAnimationData::Read(fstream& f) {
	unsigned int i,j;
	name_len = ReadLong(f);
	name = "";
	for (i=0;i<name_len;i++)
		name.push_back(f.get());
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	num_unk1 = ReadShort(f);
	num_unk2 = ReadShort(f);
fstream fout("aaaa.txt",ios::out|ios::app);

	#define MACRO_IOTRANSFORM_BEGIN(LOCALTYPE) \
	LOCALTYPE ## _amount = ReadLong(f); \
fout << #LOCALTYPE": " << (unsigned int)LOCALTYPE ## _amount << endl; \
	LOCALTYPE.resize(LOCALTYPE ## _amount); \
	for (i=0;i<LOCALTYPE ## _amount;i++) { \
		LOCALTYPE[i].transform_amount = ReadLong(f); \
fout << #LOCALTYPE" transform: " << (unsigned int)LOCALTYPE[i].transform_amount << endl; \
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
fout << #LOCALTYPE" name: " << LOCALTYPE[i].object_name.c_str() << endl; \
	}

	MACRO_IOTRANSFORM_BEGIN(localw)
		localw[i].transform[j].time = ReadFloat(f);
		localw[i].transform[j].rot.x = ReadFloat(f);
		localw[i].transform[j].rot.y = ReadFloat(f);
		localw[i].transform[j].rot.z = ReadFloat(f);
		localw[i].transform[j].rot.w = ReadFloat(f);
		localw[i].transform[j].x1 = ReadFloat(f);
		localw[i].transform[j].y1 = ReadFloat(f);
		localw[i].transform[j].z1 = ReadFloat(f);
		localw[i].transform[j].w1 = ReadFloat(f);
		localw[i].transform[j].x2 = ReadFloat(f);
		localw[i].transform[j].y2 = ReadFloat(f);
		localw[i].transform[j].z2 = ReadFloat(f);
		localw[i].transform[j].w2 = ReadFloat(f);
	MACRO_IOTRANSFORM_END(localw)
	num_unk3 = ReadLong(f);
	MACRO_IOTRANSFORM_BEGIN(localt)
		localt[i].transform[j].time = ReadFloat(f);
		localt[i].transform[j].rotx = ReadFloat(f);
		localt[i].transform[j].transx = ReadFloat(f);
		localt[i].transform[j].unkx = ReadFloat(f);
		localt[i].transform[j].roty = ReadFloat(f);
		localt[i].transform[j].transy = ReadFloat(f);
		localt[i].transform[j].unky = ReadFloat(f);
		localt[i].transform[j].rotz = ReadFloat(f);
		localt[i].transform[j].transz = ReadFloat(f);
		localt[i].transform[j].unkz = ReadFloat(f);
	MACRO_IOTRANSFORM_END(localt)
	MACRO_IOTRANSFORM_BEGIN(locals)
		locals[i].transform[j].time = ReadFloat(f);
		locals[i].transform[j].scalex = ReadFloat(f);
		locals[i].transform[j].scaley = ReadFloat(f);
		locals[i].transform[j].scalez = ReadFloat(f);
		locals[i].transform[j].unkx1 = ReadFloat(f);
		locals[i].transform[j].unky1 = ReadFloat(f);
		locals[i].transform[j].unkz1 = ReadFloat(f);
		locals[i].transform[j].unkx2 = ReadFloat(f);
		locals[i].transform[j].unky2 = ReadFloat(f);
		locals[i].transform[j].unkz2 = ReadFloat(f);
	MACRO_IOTRANSFORM_END(locals)
	num_unk4 = ReadLong(f);
	num_unk5 = ReadLong(f);
	float_unk = ReadFloat(f);
	num_unk6 = ReadLong(f);
fout.close();
	// ToDo: read the end
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
	FbxSkin* lSkin = NULL;
	FbxSkeleton* lSingleSkeleton = NULL;
	FbxNode* lSingleSkeletonNode = NULL;
	FbxCluster* lSingleSkeletonCluster = NULL;
	vector<FbxSkeleton*> lMultiSkeleton;
	vector<FbxCluster*> lMultiSkeletonCluster;
	vector<string> lMultiSkeletonName;
	bool hasbones = false;
	for (i=0;i<model.mesh.size();i++)
		if (model.mesh[i].bone_amount>0) {
			hasbones = true;
			break;
		}
	if (model.animation.size()>0 || hasbones) {
		lSkin = FbxSkin::Create(sdkscene, "");
	}
	if (model.animation.size()>0 && !hasbones) {
		lSingleSkeleton = FbxSkeleton::Create(sdkscene, "");
		lSingleSkeletonNode = FbxNode::Create(sdkscene, "");
		lSingleSkeletonCluster = FbxCluster::Create(sdkscene, "");
		lSingleSkeleton->SetSkeletonType(FbxSkeleton::eRoot);
		lSingleSkeletonNode->SetNodeAttribute(lSingleSkeleton);
		lSingleSkeletonCluster->SetLink(lSingleSkeletonNode);
		lSingleSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);
		lSkin->AddCluster(lSingleSkeletonCluster);
	}
	// Construct all the Fbx nodes
	vector<FbxNode*> lNodeList;
	vector<GameObjectNode*> hierarchynode = model.hierarchy->node_list;
	for (i=0;i<hierarchynode.size();i++) {
		if (hierarchynode[i]==model.hierarchy->root_node) {
			if (hierarchynode[i]->node_type==4) { // Should be always true
				TransformStruct* nodespec = static_cast<TransformStruct*>(hierarchynode[i]);
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
				string bonename = static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(nodespec->child_bone[j])->child_object)->GetScopedName();
				bool newbone = true;
				for (k=0;k<lMultiSkeletonName.size();k++)
					if (lMultiSkeletonName[k]==bonename) {
						newbone = false;
						break;
					}
				if (newbone) {
					TransformStruct* parenttransf = static_cast<GameObjectStruct*>(nodespec->parent_object)->GetParentTransform();
					FbxSkeleton* lSkeleton = FbxSkeleton::Create(sdkscene, static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(nodespec->child_bone[j])->child_object)->name.c_str());
					FbxCluster* lSkeletonCluster = FbxCluster::Create(sdkscene, "");
					if (parenttransf->parent_transform==NULL || parenttransf->parent_transform==model.hierarchy->root_node)
						lSkeleton->SetSkeletonType(FbxSkeleton::eRoot);
					else
						lSkeleton->SetSkeletonType(FbxSkeleton::eLimbNode);
					for (k=0;k<hierarchynode.size();k++)
						if (parenttransf==hierarchynode[k] && lNodeList[k]) {
							lNodeList[k]->SetNodeAttribute(lSkeleton);
							lSkeletonCluster->SetLink(lNodeList[k]);
							break;
						}
					lSkeleton->Size.Set(1.0);
					lSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);
					lSkin->AddCluster(lSkeletonCluster);
					lMultiSkeleton.push_back(lSkeleton);
					lMultiSkeletonCluster.push_back(lSkeletonCluster);
					lMultiSkeletonName.push_back(bonename);
				}
			}
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
				if (model.animation.size()>0 && !hasbones) {
					for (j = 0; j<mesh.vert.size(); j++)
						lSingleSkeletonCluster->AddControlPointIndex(j, 1.0);
				} else {
					for (j = 0; j < mesh.bone_amount; j++) {
						FbxCluster* lSkeletonCluster = NULL;
						for (k=0;k<lMultiSkeleton.size();k++)
							if (lMultiSkeletonName[k]==mesh.bone[j].scoped_name) {
								lSkeletonCluster = lMultiSkeletonCluster[k];
								break;
							}
						if (lSkeletonCluster) {
							for (k = 0; k < mesh.vertice_attachment_amount; k++)
								for (l = 0; l < 4; l++)
									if (mesh.vert_attachment[k].bone_id[l]==j && mesh.vert_attachment[k].bone_factor[l] > 0.0)
										lSkeletonCluster->AddControlPointIndex(k, mesh.vert_attachment[k].bone_factor[l]);
						}
					}
				}
				FbxGeometry* lPatchAttribute = (FbxGeometry*)lMeshNode->GetNodeAttribute();
				lPatchAttribute->AddDeformer(lSkin);
			}
		}
	}
	/*
	int* lMeshNodeIndex = new int[model.mesh.size()];
	int* lSkeletonNodeIndex = new int[model.mesh.size()];
	for (i = 0; i<model.mesh.size(); i++) {
		// Meshes
		FbxString lMeshName = FbxString("mesh") + FbxString((int)i);
		FbxMesh* lMesh = FbxMesh::Create(sdkscene, lMeshName);
		lMesh->InitControlPoints(model.mesh[i].vertice_amount);
		FbxVector4* lControlPoints = lMesh->GetControlPoints();
		FbxGeometryElementNormal* lGeometryElementNormal = lMesh->CreateElementNormal();
		lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
		lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
		FbxVector4 lNormal;
		FbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
		lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
		lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eDirect);
		FbxVector2 lUVPoint;
		for (j = 0; j<model.mesh[i].vert.size(); j++) {
			lControlPoints[j].Set(model.mesh[i].vert[j].x, model.mesh[i].vert[j].y, model.mesh[i].vert[j].z);
			lNormal.Set(model.mesh[i].vert[j].nx, model.mesh[i].vert[j].ny, model.mesh[i].vert[j].nz);
			lGeometryElementNormal->GetDirectArray().Add(lNormal);
			lUVPoint.Set(model.mesh[i].vert[j].u, model.mesh[i].vert[j].v);
			lUVDiffuseElement->GetDirectArray().Add(lUVPoint);
		}
		lMesh->InitMaterialIndices(FbxLayerElement::EMappingMode::eByPolygon);
		uint16_t vertid[3];
		for (j = 0; 3 * j<model.mesh[i].vert_list.size(); j++) {
			int mtlidcur = -1;
			for (k = 0; k <model.mesh[i].mat_info.size(); k++) {
				if (3 * j >= model.mesh[i].mat_info[k].vert_list_start / 2 && 3 * j < model.mesh[i].mat_info[k].vert_list_start / 2 + model.mesh[i].mat_info[k].vert_list_amount) {
					mtlidcur = k;
					break;
				}
			}
			vertid[0] = model.mesh[i].vert_list[3 * j];
			vertid[1] = model.mesh[i].vert_list[3 * j + 1];
			vertid[2] = model.mesh[i].vert_list[3 * j + 2];
			lMesh->BeginPolygon(mtlidcur);
			lMesh->AddPolygon(vertid[0], vertid[0]);
			lMesh->AddPolygon(vertid[1], vertid[1]);
			lMesh->AddPolygon(vertid[2], vertid[2]);
			lMesh->EndPolygon();
		}
		lMesh->GenerateNormals(false,true);
		FbxNode* lMeshNode = FbxNode::Create(sdkscene, "nodemesh");
		lMeshNode->SetNodeAttribute(lMesh);
		lMeshNode->SetShadingMode(FbxNode::eTextureShading);
		// Materials and Textures
		for (j = 0; j < model.material[i].size(); j++) {
			FbxString lTextureName = model.material[i][j].maintex_file_name.c_str();
			FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(sdkscene, FbxString("mat") + GetFbxNameIndice(i, j, model.mesh.size()));
			lMeshNode->AddMaterial(lMaterial);
			FbxFileTexture* lFileTex = FbxFileTexture::Create(sdkscene, lTextureName);
			lFileTex->SetFileName(lTextureName);
			lFileTex->SetRelativeFileName(lTextureName);
			lFileTex->SetTextureUse(FbxTexture::eStandard);
			lFileTex->SetMappingType(FbxTexture::eUV);
			lFileTex->SetMaterialUse(FbxFileTexture::eModelMaterial);
			lFileTex->UVSet.Set(FbxString("DiffuseUV"));
			lMaterial->Diffuse.ConnectSrcObject(lFileTex);
//			FbxProperty lProp = FbxProperty::Create(lMaterial, FbxDouble3DT, "EnvSampler", "SamplerTexture");
//			FbxDouble3 lSampleVal(0, 0, 0);
//			lProp.Set(lSampleVal);
//			lFileTex->ConnectDstProperty(lProp);
//			lMaterial->ConnectSrcObject(lFileTex);
		}
		if (lRootNode->AddChild(lMeshNode))
			lMeshNodeIndex[i] = lRootNode->GetChildCount()-1;
		else
			lMeshNodeIndex[i] = -1;
		// Skeleton
		if (model.mesh[i].bone_amount > 0) {
			FbxNode* lSkeletonRootNode = NULL;
			FbxSkin* lSkin = FbxSkin::Create(sdkscene, "");
			for (j = 0; j < model.mesh[i].bone_amount; j++) {
				FbxSkeleton* lSkeletonBone = FbxSkeleton::Create(sdkscene, FbxString("bone") + GetFbxNameIndice(i, j, model.mesh.size()));
				FbxNode* lSkeletonNode = FbxNode::Create(sdkscene, "bonenode");
				if (j == 0) {
					lSkeletonBone->SetSkeletonType(FbxSkeleton::eRoot);
					lSkeletonRootNode = lSkeletonNode;
				} else {
					lSkeletonBone->SetSkeletonType(FbxSkeleton::eLimbNode);
					lSkeletonBone->Size.Set(1.0);
					lSkeletonRootNode->AddChild(lSkeletonNode);
				}
				lSkeletonNode->SetNodeAttribute(lSkeletonBone);
				lSkeletonNode->SetRotationPivot(FbxNode::eDestinationPivot, FbxVector4(model.mesh[i].bone[j].unk_f1, model.mesh[i].bone[j].unk_f2, model.mesh[i].bone[j].unk_f3));
				lSkeletonNode->SetScalingPivot(FbxNode::eDestinationPivot, FbxVector4(model.mesh[i].bone[j].unk_f1, model.mesh[i].bone[j].unk_f2, model.mesh[i].bone[j].unk_f3));
				FbxCluster *lSkeletonCluster = FbxCluster::Create(sdkscene, "");
				lSkeletonCluster->SetLink(lSkeletonNode);
				lSkeletonCluster->SetLinkMode(FbxCluster::eTotalOne);
				for (k = 0; k < model.mesh[i].vertice_attachment_amount; k++) {
					for (l = 0; l < 4; l++) {
						if (model.mesh[i].vert_attachment[k].bone_id[l]==j && model.mesh[i].vert_attachment[k].bone_factor[l] > 0.0) {
							lSkeletonCluster->AddControlPointIndex(k, model.mesh[i].vert_attachment[k].bone_factor[l]);
						}
					}
				}
				lSkin->AddCluster(lSkeletonCluster);
			}
			if (lRootNode->AddChild(lSkeletonRootNode))
				lSkeletonNodeIndex[i] = lRootNode->GetChildCount()-1;
			else
				lSkeletonNodeIndex[i] = -1;
			FbxGeometry* lPatchAttribute = (FbxGeometry*)lMeshNode->GetNodeAttribute();
			lPatchAttribute->AddDeformer(lSkin);
		}
	}*/
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
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene) {
		exit(1);
	}
}

void DestroySdkObjects(FbxManager* pManager) {
	if (pManager) pManager->Destroy();
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

Quaternion Quaternion::EulerToQuaternion(float roll, float pitch, float yaw) {
	Quaternion q;
	double t0 = cos(yaw * 0.5);
	double t1 = sin(yaw * 0.5);
	double t2 = cos(roll * 0.5);
	double t3 = sin(roll * 0.5);
	double t4 = cos(pitch * 0.5);
	double t5 = sin(pitch * 0.5);
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
	roll = atan2(t0, t1);
	// pitch (y-axis rotation)
	double t2 = +2.0 * (q.w * q.y - q.z * q.x);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	pitch = asin(t2);
	// yaw (z-axis rotation)
	double t3 = +2.0 * (q.w * q.z + q.x * q.y);
	double t4 = +1.0 - 2.0 * (ysqr + q.z * q.z);  
	yaw = atan2(t3, t4);
}

float Quaternion::GetRoll() {
	double ysqr = y*y;
	double t0 = +2.0 * (w*x + y*z);
	double t1 = +1.0 - 2.0 * (x*x + ysqr);
	return atan2(t0, t1);
}

float Quaternion::GetPitch() {
	double t2 = +2.0 * (w*y - z*x);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	return asin(t2);
}

float Quaternion::GetYaw() {
	double ysqr = y * y;
	double t3 = +2.0 * (w*z + x*y);
	double t4 = +1.0 - 2.0 * (ysqr + z*z);  
	return atan2(t3, t4);
}
