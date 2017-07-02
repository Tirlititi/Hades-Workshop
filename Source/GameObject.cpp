#include "GameObject.h"

TransformStruct* GameObjectStruct::GetParentTransform() {
	for (unsigned int i=0;i<child_amount;i++)
		if (child[i] && child[i]->node_type==4)
			return static_cast<TransformStruct*>(child[i]);
	return NULL;
}

string GameObjectStruct::GetScopedName() {
	string res = name;
	TransformStruct* transf = GetParentTransform();
	GameObjectStruct* obj;
	while (true) {
		if (transf->parent_transform && transf->parent_transform->node_type==4)
			transf = static_cast<TransformStruct*>(transf->parent_transform);
		else
			break;
		if (transf==NULL || transf==root.root_node)
			break;
		if (transf->child_object && transf->child_object->node_type==1) {
			obj = static_cast<GameObjectStruct*>(transf->child_object);
			res = obj->name+"/"+res;
		}
	}
	return res;
}

GameObjectNode* BuildHierarchy_Rec(GameObjectNode* parent, GameObjectHierarchy& roothierarchy, uint32_t objtype, uint32_t objunk, uint64_t objinfo, fstream& f, UnityArchiveMetaData& meta, unsigned int fileindex) {
	GameObjectNode* res = NULL;
	unsigned int i;
	GameObjectNode* childobj = NULL;
	uint32_t ctype, cunk;
	uint64_t cinfo;
	int32_t objindex;
	size_t prevpos = f.tellg();
	f.seekg(meta.GetFileOffsetByIndex(fileindex));
	if (meta.file_type1[fileindex]==1) {
		GameObjectStruct* resobj = new GameObjectStruct(parent,roothierarchy,objtype,objunk,objinfo);
		res = resobj;
		roothierarchy.node_list.push_back(res);

		#define MACRO_READCHILDOBJ(PARENT,HASTYPE) \
			if (HASTYPE) ctype = ReadLong(f); \
			cunk = ReadLong(f); \
			cinfo = ReadLongLong(f); \
			if (cinfo==0) { \
				childobj = NULL; \
			} else { \
				childobj = roothierarchy.FindObjectByInfo(cinfo); \
				if (!childobj) { \
					objindex = meta.GetFileIndexByInfo(cinfo); \
					if (objindex>=0) \
						childobj = BuildHierarchy_Rec(PARENT,roothierarchy,HASTYPE ? ctype : meta.file_type1[objindex],cunk,cinfo,f,meta,objindex); \
					else \
						childobj = NULL; \
				} \
			}

		resobj->child_amount = ReadLong(f);
		for (i=0;i<resobj->child_amount;i++) {
			MACRO_READCHILDOBJ(resobj,true)
			resobj->child.push_back(childobj);
		}
		resobj->unknown = ReadLong(f);
		resobj->name_len = ReadLong(f);
		resobj->name = "";
		for (i=0;i<resobj->name_len;i++)
			resobj->name.push_back(f.get());
		f.seekg(GetAlignOffset(f.tellg()),ios::cur);
		resobj->unk1 = f.get();
		resobj->unk2 = f.get();
		resobj->unk3 = f.get();
		res = resobj;
	} else if (meta.file_type1[fileindex]==4) {
		TransformStruct* restransf = new TransformStruct(parent,roothierarchy,objtype,objunk,objinfo);
		res = restransf;
		roothierarchy.node_list.push_back(res);
		MACRO_READCHILDOBJ(restransf,false)
		restransf->child_object = childobj;
		restransf->rot.Read(f);
		ModelDataStruct::ReadCoordinates(f,restransf->x,restransf->y,restransf->z);
		ModelDataStruct::ReadCoordinates(f,restransf->scale_x,restransf->scale_y,restransf->scale_z,false);
		restransf->child_transform_amount = ReadLong(f);
		for (i=0;i<restransf->child_transform_amount;i++) {
			MACRO_READCHILDOBJ(restransf,false)
			restransf->child_transform.push_back(childobj);
		}
		MACRO_READCHILDOBJ(restransf,false)
		restransf->parent_transform = childobj;
		GameObjectNode* parentnoderec = restransf->parent_transform;
		Quaternion rotationquat = restransf->rot;
		while (parentnoderec && parentnoderec->node_type==4) {
			rotationquat = Quaternion::Product(static_cast<TransformStruct*>(parentnoderec)->rot,rotationquat);
			parentnoderec = static_cast<TransformStruct*>(parentnoderec)->parent_transform;
		}
		rotationquat.Apply(restransf->x,restransf->y,restransf->z);
		res = restransf;
	} else if (meta.file_type1[fileindex]==23) {
		MeshRendererStruct* resmeshren = new MeshRendererStruct(parent,roothierarchy,objtype,objunk,objinfo);
		res = resmeshren;
		roothierarchy.node_list.push_back(res);
		MACRO_READCHILDOBJ(resmeshren,false)
		resmeshren->parent_object = childobj;
		resmeshren->flag1 = ReadLong(f);
		resmeshren->flags_unk = ReadLong(f);
		resmeshren->max_unk = ReadLong(f);
		resmeshren->float_unk1 = ReadFloat(f);
		resmeshren->float_unk2 = ReadFloat(f);
		resmeshren->float_unk3 = ReadFloat(f);
		resmeshren->float_unk4 = ReadFloat(f);
		resmeshren->float_unk5 = ReadFloat(f);
		resmeshren->float_unk6 = ReadFloat(f);
		resmeshren->float_unk7 = ReadFloat(f);
		resmeshren->float_unk8 = ReadFloat(f);
		resmeshren->child_material_amount = ReadLong(f);
		for (i=0;i<resmeshren->child_material_amount;i++) {
			MACRO_READCHILDOBJ(resmeshren,false)
			resmeshren->child_material.push_back(childobj);
		}
		resmeshren->flag3 = ReadLong(f);
		resmeshren->flag4 = ReadLong(f);
		resmeshren->flag5 = ReadLong(f);
		resmeshren->flag6 = ReadLong(f);
		resmeshren->flag7 = ReadLong(f);
		resmeshren->flag8 = ReadLong(f);
		resmeshren->flag9 = ReadLong(f);
		resmeshren->flag10 = ReadLong(f);
		resmeshren->flag11 = ReadLong(f);
		resmeshren->flag12 = ReadLong(f);
		resmeshren->flag13 = ReadLong(f);
		resmeshren->flag14 = ReadLong(f);
		resmeshren->flag15 = ReadLong(f);
		resmeshren->flag16 = ReadLong(f);
		res = resmeshren;
	} else if (meta.file_type1[fileindex]==33) {
		MeshFilterStruct* resmeshfilter = new MeshFilterStruct(parent,roothierarchy,objtype,objunk,objinfo);
		res = resmeshfilter;
		roothierarchy.node_list.push_back(res);
		MACRO_READCHILDOBJ(resmeshfilter,false)
		resmeshfilter->parent_object = childobj;
		MACRO_READCHILDOBJ(resmeshfilter,false)
		resmeshfilter->child_mesh = childobj;
		res = resmeshfilter;
	} else if (meta.file_type1[fileindex]==111) {
		AnimationStruct* resanim = new AnimationStruct(parent,roothierarchy,objtype,objunk,objinfo);
		res = resanim;
		roothierarchy.node_list.push_back(res);
		MACRO_READCHILDOBJ(resanim,false)
		resanim->parent_object = childobj;
		resanim->flag1 = ReadLong(f);
		MACRO_READCHILDOBJ(resanim,false)
		resanim->child_clip1 = childobj;
		resanim->flag2 = ReadLong(f);
		if (cinfo!=0) {
			MACRO_READCHILDOBJ(resanim,false)
			resanim->child_clip2 = childobj;
		} else {
			resanim->child_clip2 = NULL;
		}
		resanim->flag3 = ReadLong(f);
		resanim->flag4 = ReadLong(f);
		resanim->flag5 = ReadLong(f);
	} else if (meta.file_type1[fileindex]==137) {
		SkinnedMeshRendererStruct* resskinmeshren = new SkinnedMeshRendererStruct(parent,roothierarchy,objtype,objunk,objinfo);
		res = resskinmeshren;
		roothierarchy.node_list.push_back(res);
		MACRO_READCHILDOBJ(resskinmeshren,false)
		resskinmeshren->parent_object = childobj;
		resskinmeshren->flag1 = ReadLong(f);
		resskinmeshren->flags_unk = ReadLong(f);
		resskinmeshren->max_unk = ReadLong(f);
		resskinmeshren->float_unk1 = ReadFloat(f);
		resskinmeshren->float_unk2 = ReadFloat(f);
		resskinmeshren->float_unk3 = ReadFloat(f);
		resskinmeshren->float_unk4 = ReadFloat(f);
		resskinmeshren->float_unk5 = ReadFloat(f);
		resskinmeshren->float_unk6 = ReadFloat(f);
		resskinmeshren->float_unk7 = ReadFloat(f);
		resskinmeshren->float_unk8 = ReadFloat(f);
		resskinmeshren->child_material_amount = ReadLong(f);
		for (i=0;i<resskinmeshren->child_material_amount;i++) {
			MACRO_READCHILDOBJ(resskinmeshren,false)
			resskinmeshren->child_material.push_back(childobj);
		}
		resskinmeshren->flag3 = ReadLong(f);
		resskinmeshren->flag4 = ReadLong(f);
		resskinmeshren->flag5 = ReadLong(f);
		resskinmeshren->flag6 = ReadLong(f);
		resskinmeshren->flag7 = ReadLong(f);
		resskinmeshren->flag8 = ReadLong(f);
		resskinmeshren->flag9 = ReadLong(f);
		resskinmeshren->flag10 = ReadLong(f);
		resskinmeshren->flag11 = ReadLong(f);
		resskinmeshren->flag12 = ReadLong(f);
		resskinmeshren->flag13 = ReadLong(f);
		resskinmeshren->flag14 = ReadLong(f);
		resskinmeshren->flag15 = ReadLong(f);
		MACRO_READCHILDOBJ(resskinmeshren,false)
		resskinmeshren->child_mesh = childobj;
		resskinmeshren->child_bone_amount = ReadLong(f);
		for (i=0;i<resskinmeshren->child_bone_amount;i++) {
			MACRO_READCHILDOBJ(resskinmeshren,false)
			resskinmeshren->child_bone.push_back(childobj);
		}
		resskinmeshren->flag16 = ReadLong(f);
		MACRO_READCHILDOBJ(resskinmeshren,false)
		resskinmeshren->child_bone_sample = childobj;
		resskinmeshren->float_unk9 = ReadFloat(f);
		resskinmeshren->float_unk10 = ReadFloat(f);
		resskinmeshren->float_unk11 = ReadFloat(f);
		resskinmeshren->float_unk12 = ReadFloat(f);
		resskinmeshren->float_unk13 = ReadFloat(f);
		resskinmeshren->float_unk14 = ReadFloat(f);
		resskinmeshren->float_unk15 = ReadFloat(f);
	} else {
		res = new GameObjectNode(parent,roothierarchy,objtype,objunk,objinfo);
		roothierarchy.node_list.push_back(res);
	}
	res->file_index = fileindex;
	f.seekg(prevpos);
	return res;
}

/*string DEBUGSpace = "";
vector<GameObjectNode*> DEBUGDisplayedNode;
void DEBUGDisplayHierarchy_Rec(fstream& fout, GameObjectNode* node) {
	if (!node) {
		fout << DEBUGSpace << "-> Invalid Node" << endl;
		return;
	}
	unsigned int i;
	fout << DEBUGSpace << "-> " << UnityArchiveMetaData::GetTypeName(node->node_type);
	if (node->node_type==1) {
		GameObjectStruct* nodeobj = static_cast<GameObjectStruct*>(node);
		fout << " (" << nodeobj->name << ")";
	}
	for (i=0;i<DEBUGDisplayedNode.size();i++)
		if (DEBUGDisplayedNode[i]==node) {
			fout << " - ALREADY DISPLAYED: " << (unsigned int)i << endl;
			return;
		}
	fout << " - " << (unsigned int)DEBUGDisplayedNode.size() << endl;
	DEBUGDisplayedNode.push_back(node);
	DEBUGSpace += "  ";
	if (node->node_type==1) {
		GameObjectStruct* nodespec = static_cast<GameObjectStruct*>(node);
		for (i=0;i<nodespec->child_amount;i++)
			DEBUGDisplayHierarchy_Rec(fout,nodespec->child[i]);
	} else if (node->node_type==4) {
		TransformStruct* nodespec = static_cast<TransformStruct*>(node);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->child_object);
		for (i=0;i<nodespec->child_transform_amount;i++)
			DEBUGDisplayHierarchy_Rec(fout,nodespec->child_transform[i]);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->parent_transform);
	} else if (node->node_type==23) {
		MeshRendererStruct* nodespec = static_cast<MeshRendererStruct*>(node);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->parent_object);
		for (i=0;i<nodespec->child_material_amount;i++)
			DEBUGDisplayHierarchy_Rec(fout,nodespec->child_material[i]);
	} else if (node->node_type==33) {
		MeshFilterStruct* nodespec = static_cast<MeshFilterStruct*>(node);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->parent_object);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->child_mesh);
	} else if (node->node_type==111) {
		AnimationStruct* nodespec = static_cast<AnimationStruct*>(node);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->parent_object);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->child_clip1);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->child_clip2);
	} else if (node->node_type==137) {
		SkinnedMeshRendererStruct* nodespec = static_cast<SkinnedMeshRendererStruct*>(node);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->parent_object);
		for (i=0;i<nodespec->child_material_amount;i++)
			DEBUGDisplayHierarchy_Rec(fout,nodespec->child_material[i]);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->child_mesh);
		for (i=0;i<nodespec->child_bone_amount;i++)
			DEBUGDisplayHierarchy_Rec(fout,nodespec->child_bone[i]);
		DEBUGDisplayHierarchy_Rec(fout,nodespec->child_bone_sample);
	}
	DEBUGSpace = DEBUGSpace.substr(2);
}*/

void GameObjectHierarchy::BuildHierarchy(fstream& archivefile, UnityArchiveMetaData& metadata, unsigned int rootfileindex) {
	uint32_t type = metadata.file_type1[rootfileindex];
	uint32_t unk = metadata.file_unknown2[rootfileindex];
	uint64_t info = metadata.file_info[rootfileindex];
	meta_data = &metadata;
	root_node = BuildHierarchy_Rec(NULL,*this,type,unk,info,archivefile,metadata,rootfileindex);
/*fstream fout("aaab.txt",ios::app|ios::out); fout << "DISPLAYING" << endl;
DEBUGDisplayedNode.clear();
DEBUGDisplayHierarchy_Rec(fout,root_node);
fout << endl; fout.close();*/
}

GameObjectHierarchy::~GameObjectHierarchy() {
	for (unsigned int i=0;i<node_list.size();i++)
		delete node_list[i];
	node_list.clear();
}

GameObjectNode* GameObjectHierarchy::FindObjectByInfo(uint64_t info) {
	for (unsigned int i=0;i<node_list.size();i++)
		if (node_list[i]->node_info==info)
			return node_list[i];
	return NULL;
}
