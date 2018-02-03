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
	while (transf) {
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

GameObjectNode* BuildHierarchy_Rec(GameObjectNode* parent, GameObjectHierarchy& roothierarchy, uint32_t objtype, uint32_t objflags, int64_t objinfo, fstream& f, UnityArchiveMetaData& meta, unsigned int fileindex) {
	GameObjectNode* res = NULL;
	unsigned int i;
	GameObjectNode* childobj = NULL;
	uint32_t ctype, cunk;
	int64_t cinfo;
	int32_t objindex;
	size_t prevpos = f.tellg();
	f.seekg(meta.GetFileOffsetByIndex(fileindex));
	if (meta.file_type1[fileindex]==1) {
		GameObjectStruct* resobj = new GameObjectStruct(parent,roothierarchy,objtype,objflags,objinfo);
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
	} else if (meta.file_type1[fileindex]==4) {
		TransformStruct* restransf = new TransformStruct(parent,roothierarchy,objtype,objflags,objinfo);
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
	} else if (meta.file_type1[fileindex]==23) {
		MeshRendererStruct* resmeshren = new MeshRendererStruct(parent,roothierarchy,objtype,objflags,objinfo);
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
	} else if (meta.file_type1[fileindex]==33) {
		MeshFilterStruct* resmeshfilter = new MeshFilterStruct(parent,roothierarchy,objtype,objflags,objinfo);
		res = resmeshfilter;
		roothierarchy.node_list.push_back(res);
		MACRO_READCHILDOBJ(resmeshfilter,false)
		resmeshfilter->parent_object = childobj;
		MACRO_READCHILDOBJ(resmeshfilter,false)
		resmeshfilter->child_mesh = childobj;
	} else if (meta.file_type1[fileindex]==111) {
		AnimationStruct* resanim = new AnimationStruct(parent,roothierarchy,objtype,objflags,objinfo);
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
		SkinnedMeshRendererStruct* resskinmeshren = new SkinnedMeshRendererStruct(parent,roothierarchy,objtype,objflags,objinfo);
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
		ModelDataStruct::ReadCoordinates(f,resskinmeshren->center_x,resskinmeshren->center_y,resskinmeshren->center_z); // DEBUG: Unsure if "swap sign" for both
		ModelDataStruct::ReadCoordinates(f,resskinmeshren->radius_x,resskinmeshren->radius_y,resskinmeshren->radius_z,false);
		resskinmeshren->float_unk9 = ReadFloat(f);
	} else {
		res = new GameObjectNode(parent,roothierarchy,objtype,objflags,objinfo);
		roothierarchy.node_list.push_back(res);
	}
	res->file_index = fileindex;
	f.seekg(prevpos);
	return res;
}

string DEBUGSpace = "";
vector<GameObjectNode*> DEBUGDisplayedNode;
void DEBUGDisplayHierarchy_Rec(fstream& fout, GameObjectNode* node) {
	if (!node) {
		fout << DEBUGSpace << "-> Invalid Node" << endl;
		return;
	}
	unsigned int i;
	fout << DEBUGSpace << "-> " << UnityArchiveMetaData::GetTypeName(node->node_type) << " " << (unsigned int)node->file_index+1;
	if (node->node_type==1) {
		GameObjectStruct* nodeobj = static_cast<GameObjectStruct*>(node);
		fout << " (" << nodeobj->name << ")";
	}
	for (i=0;i<DEBUGDisplayedNode.size();i++)
		if (DEBUGDisplayedNode[i]==node) {
			fout << " - ALREADY DISPLAYED: " << (unsigned int)i;
			GameObjectStruct* nodeobj = NULL;
			if (node->node_type==4)
				fout << " (" << static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(node)->child_object)->name << ")";
			fout << endl;
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
}

void GameObjectHierarchy::DEBUGDisplayHierarchy() {
	fstream fout("aaab.txt",ios::app|ios::out); fout << "DISPLAYING" << endl;
	DEBUGDisplayedNode.clear();
	DEBUGDisplayHierarchy_Rec(fout,root_node);
	fout << endl; fout.close();
}

void GameObjectHierarchy::BuildHierarchy(fstream& archivefile, UnityArchiveMetaData& metadata, unsigned int rootfileindex) {
	uint32_t type = metadata.file_type1[rootfileindex];
	uint32_t flags = metadata.file_flags[rootfileindex];
	int64_t info = metadata.file_info[rootfileindex];
	meta_data = &metadata;
	root_node = BuildHierarchy_Rec(NULL,*this,type,flags,info,archivefile,metadata,rootfileindex);
}

void GameObjectHierarchy::OverwriteHierarchy(fstream& archivefile) {
	unsigned int i,j;
	for (i=0;i<node_list.size();i++) {
//fstream fout("aaab.txt",ios::app|ios::out); fout << "HIERARCHY WRITE: " << i << " " << (unsigned int)node_list[i]->node_type << endl; fout.close();
		if (node_list[i]->node_type==1) {
			GameObjectStruct& nodeobj = *static_cast<GameObjectStruct*>(node_list[i]);
			archivefile.seekg(meta_data->GetFileOffsetByIndex(node_list[i]->file_index));

			#define MACRO_WRITECHILDOBJ(OBJ,HASTYPE) \
				if (OBJ==NULL) { \
					if (HASTYPE) WriteLong(archivefile,0); \
					WriteLong(archivefile,0); \
					WriteLongLong(archivefile,0); \
				} else { \
					if (HASTYPE) WriteLong(archivefile,OBJ->node_type); \
					WriteLong(archivefile,OBJ->node_unknown); \
					WriteLongLong(archivefile,OBJ->node_info); \
				}

			WriteLong(archivefile,nodeobj.child_amount);
			for (j=0;j<nodeobj.child_amount;j++) {
				MACRO_WRITECHILDOBJ(nodeobj.child[j],true)
			}
			WriteLong(archivefile,nodeobj.unknown);
			WriteLong(archivefile,nodeobj.name_len);
			for (j=0;j<nodeobj.name_len;j++)
				archivefile.put(nodeobj.name[j]);
			while (archivefile.tellg()%4) archivefile.put(0);
			archivefile.put(nodeobj.unk1);
			archivefile.put(nodeobj.unk2);
			archivefile.put(nodeobj.unk3);
		} else if (node_list[i]->node_type==4) {
			TransformStruct& nodetransf = *static_cast<TransformStruct*>(node_list[i]);
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY TRANSFORM: " << (int)node_list[i]->file_index << endl; fout.close();
			archivefile.seekg(meta_data->GetFileOffsetByIndex(node_list[i]->file_index));
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY 1" << endl; fout.close();
			MACRO_WRITECHILDOBJ(nodetransf.child_object,false)
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY 2" << endl; fout.close();
			nodetransf.rot.Write(archivefile);
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY 3" << endl; fout.close();
			ModelDataStruct::WriteCoordinates(archivefile,nodetransf.x,nodetransf.y,nodetransf.z);
			ModelDataStruct::WriteCoordinates(archivefile,nodetransf.scale_x,nodetransf.scale_y,nodetransf.scale_z,false);
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY CHILD: " << (unsigned int)nodetransf.child_transform_amount << endl; fout.close();
			WriteLong(archivefile,nodetransf.child_transform_amount);
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY 4" << endl; fout.close();
			for (j=0;j<nodetransf.child_transform_amount;j++) {
				MACRO_WRITECHILDOBJ(nodetransf.child_transform[j],false)
			}
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY 5" << endl; fout.close();
			MACRO_WRITECHILDOBJ(nodetransf.parent_transform,false)
//fout.open("aaab.txt",ios::app|ios::out); fout << "HIERARCHY 6" << endl; fout.close();
		} else if (node_list[i]->node_type==23) {
			MeshRendererStruct& nodemeshren = *static_cast<MeshRendererStruct*>(node_list[i]);
			archivefile.seekg(meta_data->GetFileOffsetByIndex(node_list[i]->file_index));
			MACRO_WRITECHILDOBJ(nodemeshren.parent_object,false)
			WriteLong(archivefile,nodemeshren.flag1);
			WriteLong(archivefile,nodemeshren.flags_unk);
			WriteLong(archivefile,nodemeshren.max_unk);
			WriteFloat(archivefile,nodemeshren.float_unk1);
			WriteFloat(archivefile,nodemeshren.float_unk2);
			WriteFloat(archivefile,nodemeshren.float_unk3);
			WriteFloat(archivefile,nodemeshren.float_unk4);
			WriteFloat(archivefile,nodemeshren.float_unk5);
			WriteFloat(archivefile,nodemeshren.float_unk6);
			WriteFloat(archivefile,nodemeshren.float_unk7);
			WriteFloat(archivefile,nodemeshren.float_unk8);
			WriteLong(archivefile,nodemeshren.child_material_amount);
			for (j=0;j<nodemeshren.child_material_amount;j++) {
				MACRO_WRITECHILDOBJ(nodemeshren.child_material[j],false)
			}
			WriteLong(archivefile,nodemeshren.flag3);
			WriteLong(archivefile,nodemeshren.flag4);
			WriteLong(archivefile,nodemeshren.flag5);
			WriteLong(archivefile,nodemeshren.flag5);
			WriteLong(archivefile,nodemeshren.flag6);
			WriteLong(archivefile,nodemeshren.flag7);
			WriteLong(archivefile,nodemeshren.flag8);
			WriteLong(archivefile,nodemeshren.flag9);
			WriteLong(archivefile,nodemeshren.flag10);
			WriteLong(archivefile,nodemeshren.flag11);
			WriteLong(archivefile,nodemeshren.flag12);
			WriteLong(archivefile,nodemeshren.flag13);
			WriteLong(archivefile,nodemeshren.flag14);
			WriteLong(archivefile,nodemeshren.flag15);
		} else if (node_list[i]->node_type==33) {
			MeshFilterStruct& nodemeshfilter = *static_cast<MeshFilterStruct*>(node_list[i]);
			archivefile.seekg(meta_data->GetFileOffsetByIndex(node_list[i]->file_index));
			MACRO_WRITECHILDOBJ(nodemeshfilter.parent_object,false)
			MACRO_WRITECHILDOBJ(nodemeshfilter.child_mesh,false)
		} else if (node_list[i]->node_type==111) {
			AnimationStruct& nodeanim = *static_cast<AnimationStruct*>(node_list[i]);
			archivefile.seekg(meta_data->GetFileOffsetByIndex(node_list[i]->file_index));
			MACRO_WRITECHILDOBJ(nodeanim.parent_object,false)
			WriteLong(archivefile,nodeanim.flag1);
			MACRO_WRITECHILDOBJ(nodeanim.child_clip1,false)
			WriteLong(archivefile,nodeanim.flag2);
			if (nodeanim.child_clip1!=NULL) {
				MACRO_WRITECHILDOBJ(nodeanim.child_clip2,false)
			}
			WriteLong(archivefile,nodeanim.flag3);
			WriteLong(archivefile,nodeanim.flag4);
			WriteLong(archivefile,nodeanim.flag5);
		} else if (node_list[i]->node_type==137) {
			SkinnedMeshRendererStruct& nodeskinmeshren = *static_cast<SkinnedMeshRendererStruct*>(node_list[i]);
			archivefile.seekg(meta_data->GetFileOffsetByIndex(node_list[i]->file_index));
			MACRO_WRITECHILDOBJ(nodeskinmeshren.parent_object,false)
			WriteLong(archivefile,nodeskinmeshren.flag1);
			WriteLong(archivefile,nodeskinmeshren.flags_unk);
			WriteLong(archivefile,nodeskinmeshren.max_unk);
			WriteFloat(archivefile,nodeskinmeshren.float_unk1);
			WriteFloat(archivefile,nodeskinmeshren.float_unk2);
			WriteFloat(archivefile,nodeskinmeshren.float_unk3);
			WriteFloat(archivefile,nodeskinmeshren.float_unk4);
			WriteFloat(archivefile,nodeskinmeshren.float_unk5);
			WriteFloat(archivefile,nodeskinmeshren.float_unk6);
			WriteFloat(archivefile,nodeskinmeshren.float_unk7);
			WriteFloat(archivefile,nodeskinmeshren.float_unk8);
			WriteLong(archivefile,nodeskinmeshren.child_material_amount);
			for (j=0;j<nodeskinmeshren.child_material_amount;j++) {
				MACRO_WRITECHILDOBJ(nodeskinmeshren.child_material[j],false)
			}
			WriteLong(archivefile,nodeskinmeshren.flag3);
			WriteLong(archivefile,nodeskinmeshren.flag4);
			WriteLong(archivefile,nodeskinmeshren.flag5);
			WriteLong(archivefile,nodeskinmeshren.flag6);
			WriteLong(archivefile,nodeskinmeshren.flag7);
			WriteLong(archivefile,nodeskinmeshren.flag8);
			WriteLong(archivefile,nodeskinmeshren.flag9);
			WriteLong(archivefile,nodeskinmeshren.flag10);
			WriteLong(archivefile,nodeskinmeshren.flag11);
			WriteLong(archivefile,nodeskinmeshren.flag12);
			WriteLong(archivefile,nodeskinmeshren.flag13);
			WriteLong(archivefile,nodeskinmeshren.flag14);
			WriteLong(archivefile,nodeskinmeshren.flag15);
			MACRO_WRITECHILDOBJ(nodeskinmeshren.child_mesh,false)
			WriteLong(archivefile,nodeskinmeshren.child_bone_amount);
			for (j=0;j<nodeskinmeshren.child_bone_amount;j++) {
				MACRO_WRITECHILDOBJ(nodeskinmeshren.child_bone[j],false)
			}
			WriteLong(archivefile,nodeskinmeshren.flag16);
			MACRO_WRITECHILDOBJ(nodeskinmeshren.child_bone_sample,false)
			ModelDataStruct::WriteCoordinates(archivefile,nodeskinmeshren.center_x,nodeskinmeshren.center_y,nodeskinmeshren.center_z);
			ModelDataStruct::WriteCoordinates(archivefile,nodeskinmeshren.radius_x,nodeskinmeshren.radius_y,nodeskinmeshren.radius_z,false);
			WriteFloat(archivefile,nodeskinmeshren.float_unk9);
		}
	}
}

GameObjectHierarchy::~GameObjectHierarchy() {
	for (unsigned int i=0;i<node_list.size();i++)
		delete node_list[i];
}

GameObjectNode* GameObjectHierarchy::FindObjectByInfo(int64_t info) {
	for (unsigned int i=0;i<node_list.size();i++)
		if (node_list[i]->node_info==info)
			return node_list[i];
	return NULL;
}

void GameObjectHierarchy::MergeHierarchy(UnityArchiveMetaData archivelist[UNITY_ARCHIVE_AMOUNT], vector<int64_t> additionalinfotoavoid, GameObjectHierarchy* base, int mergepolicy) {
	unsigned int i,j,macroi,newfileindex = meta_data->header_file_amount;
	int64_t newfileinfo = meta_data->file_info[meta_data->header_file_amount-1]+1;
	unsigned int archiveinfolist[UNITY_ARCHIVE_AMOUNT];
	if (mergepolicy>=1) {
		for (i=0;i<UNITY_ARCHIVE_AMOUNT;i++) {
			archiveinfolist[i] = archivelist[i].header_file_amount;
			do {
				archiveinfolist[i]--;
			} while (archivelist[i].file_info[archiveinfolist[i]]>=newfileinfo);
			archiveinfolist[i]++;
		}

		#define MACRO_VERIFY_INFO_AVAILABILITY(INFO) \
			macroi = 0; \
			while (macroi<UNITY_ARCHIVE_AMOUNT) { \
				if (macroi==0) { \
					while (macroi<additionalinfotoavoid.size()) { \
						if (additionalinfotoavoid[macroi]==INFO) { \
							INFO++; \
							macroi = 0; \
						} else { \
							macroi++; \
						} \
					} \
					macroi = 0; \
				} \
				if (archiveinfolist[macroi]<archivelist[macroi].header_file_amount && INFO==archivelist[macroi].file_info[archiveinfolist[macroi]]) { \
					archiveinfolist[macroi]++; \
					INFO++; \
					macroi = 0; \
				} else { \
					macroi++; \
				} \
			}

		MACRO_VERIFY_INFO_AVAILABILITY(newfileinfo)
	}

	#define MACRO_ASSIGN_NEW_INFO(NODE) \
		NODE->node_unknown = 0; \
		MACRO_VERIFY_INFO_AVAILABILITY(newfileinfo) \
		NODE->node_info = newfileinfo++; \
		NODE->file_index = newfileindex++;

	if (mergepolicy==2) {
		for (i=0;i<node_list.size();i++) {
			if (root_node->node_type!=4 || static_cast<TransformStruct*>(root_node)->child_object!=node_list[i]) {
				MACRO_ASSIGN_NEW_INFO(node_list[i])
			} else {
				node_list[i]->node_unknown = 0;
			}
		}
		return;
	}
	bool* node_found = new bool[base->node_list.size()];
	bool found;
	for (i=0;i<base->node_list.size();i++)
		node_found[i] = false;
	for (i=0;i<node_list.size();i++) {
		node_list[i]->node_unknown = 0;
		node_list[i]->node_info = 0;
		node_list[i]->file_index = 0;
	}
	for (i=0;i<node_list.size();i++) {
		if (node_list[i]->node_info!=0)
			continue;
		found = false;
		for (j=0;!found && j<base->node_list.size();j++) {
			if (!node_found[j] && node_list[i]->node_type==base->node_list[j]->node_type) {
				if (node_list[i]->node_type==4 && ((j==0 && i==0) || static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(node_list[i])->child_object)->name==static_cast<GameObjectStruct*>(static_cast<TransformStruct*>(base->node_list[j])->child_object)->name)) {
					node_list[i]->node_unknown = base->node_list[j]->node_unknown;
					node_list[i]->node_info = base->node_list[j]->node_info;
					node_list[i]->file_index = base->node_list[j]->file_index;
					static_cast<TransformStruct*>(node_list[i])->child_object->node_unknown = static_cast<TransformStruct*>(base->node_list[j])->child_object->node_unknown;
					static_cast<TransformStruct*>(node_list[i])->child_object->node_info = static_cast<TransformStruct*>(base->node_list[j])->child_object->node_info;
					static_cast<TransformStruct*>(node_list[i])->child_object->file_index = static_cast<TransformStruct*>(base->node_list[j])->child_object->file_index;
					node_found[j] = true;
					found = true;
				} else if (node_list[i]->parent && node_list[i]->parent->node_info!=0 && base->node_list[j]->parent && node_list[i]->parent->file_index==base->node_list[j]->parent->file_index) {
					node_list[i]->node_unknown = base->node_list[j]->node_unknown;
					node_list[i]->node_info = base->node_list[j]->node_info;
					node_list[i]->file_index = base->node_list[j]->file_index;
					node_found[j] = true;
					found = true;
				}
			}
		}
		if (!found && mergepolicy==1) {
			MACRO_ASSIGN_NEW_INFO(node_list[i])
		}
	}
	delete[] node_found;
}

int64_t GameObjectHierarchy::GetRootInfoFromObject(uint8_t * objbuffer) {
	uint32_t childamount;
	uint32_t childtype;
	int64_t rootinfo = 0;
	unsigned int i;
	BufferInitPosition();
	BufferReadLong(objbuffer,childamount);
	for (i=0;i<childamount;i++) {
		BufferReadLong(objbuffer,childtype);
		if (childtype==4) {
			BufferInitPosition(BufferGetPosition()+4);
			BufferReadLongLong(objbuffer,(uint64_t&)rootinfo);
			break;
		}
		BufferInitPosition(BufferGetPosition()+0xC);
	}
	return rootinfo;
}

int GameObjectNode::GetDataSize() {
	return 0;
}

int GameObjectStruct::GetDataSize() {
	return 15+16*child_amount+name_len+GetAlignOffset(name_len);
}

int TransformStruct::GetDataSize() {
	return 68+12*child_transform_amount;
}

int MeshRendererStruct::GetDataSize() {
	return 116+12*child_material_amount;
}

int MeshFilterStruct::GetDataSize() {
	return 24;
}

int AnimationStruct::GetDataSize() {
	return 44+(child_clip1!=NULL ? 12 : 0);
}

int SkinnedMeshRendererStruct::GetDataSize() {
	return 172+12*child_material_amount+12*child_bone_amount;
}
