#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

struct GameObjectHierarchy;
struct GameObjectNode;
struct GameObjectStruct;			// 1
struct TransformStruct;				// 4
struct MeshRendererStruct;			// 23
struct MeshFilterStruct;			// 33
struct AnimationStruct;				// 111
struct SkinnedMeshRendererStruct;	// 137

//=====================
// Standard Hierarchies
//=====================
// Models in p0data4:
//  Root Object
//  -> Animation
//  Bone Hierarchy
//  Mesh
//  -> SkinnedMeshRenderer (contains Material+Mesh)
//  More meshes...
//=====================
// Models in p0data2:
//  Root Object
//  -> MeshFilter (contains Mesh)
//  -> MeshRenderer (contains Material)
//  -> Animation
//=====================
// BBG Models:
//  Root Object
//  -> Animation
//  Mesh
//  -> MeshFilter (contains Mesh)
//  -> MeshRenderer (contains Materials)
//  More meshes...
//=====================

#include <vector>
#include "UnityArchiver.h"
#include "ModelMesh.h"

struct GameObjectNode {
	GameObjectNode* parent;
	GameObjectHierarchy& root;
	uint32_t node_type;
	uint32_t node_unknown;
	uint64_t node_info;
	unsigned int file_index;

	GameObjectNode(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : parent(prt), root(rt), node_type(ndtype), node_unknown(unk), node_info(nfo) {}
};

struct GameObjectStruct : public GameObjectNode {
	uint32_t child_amount;
	vector<GameObjectNode*> child; // 1st child is the parent_transform
	uint32_t unknown;
	uint32_t name_len;
	string name;
	uint8_t unk1;
	uint8_t unk2;
	uint8_t unk3;

	GameObjectStruct(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : GameObjectNode(prt, rt, ndtype, unk, nfo) {}
	TransformStruct* GetParentTransform();
	string GetScopedName();
};

struct TransformStruct : public GameObjectNode {
	GameObjectNode* child_object;
	uint32_t child_transform_amount;
	vector<GameObjectNode*> child_transform;
	GameObjectNode* parent_transform;
	Quaternion rot;
	float x;
	float y;
	float z;
	float scale_x;
	float scale_y;
	float scale_z;

	TransformStruct(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : GameObjectNode(prt, rt, ndtype, unk, nfo) {}
};

struct MeshRendererStruct : public GameObjectNode {
	GameObjectNode* parent_object;
	uint32_t flag1;
	uint32_t flags_unk;
	uint32_t max_unk;
	float float_unk1;
	float float_unk2;
	float float_unk3;
	float float_unk4;
	float float_unk5;
	float float_unk6;
	float float_unk7;
	float float_unk8;
	uint32_t child_material_amount;
	vector<GameObjectNode*> child_material;
	uint32_t flag3;
	uint32_t flag4;
	uint32_t flag5;
	uint32_t flag6;
	uint32_t flag7;
	uint32_t flag8;
	uint32_t flag9;
	uint32_t flag10;
	uint32_t flag11;
	uint32_t flag12;
	uint32_t flag13;
	uint32_t flag14;
	uint32_t flag15;
	uint32_t flag16;

	MeshRendererStruct(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : GameObjectNode(prt, rt, ndtype, unk, nfo) {}
};

struct MeshFilterStruct : public GameObjectNode {
	GameObjectNode* parent_object;
	GameObjectNode* child_mesh;

	MeshFilterStruct(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : GameObjectNode(prt, rt, ndtype, unk, nfo) {}
};

struct AnimationStruct : public GameObjectNode {
	GameObjectNode* parent_object;
	GameObjectNode* child_clip1;
	GameObjectNode* child_clip2;
	uint32_t flag1;
	uint32_t flag2;
	uint32_t flag3;
	uint32_t flag4;
	uint32_t flag5;

	AnimationStruct(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : GameObjectNode(prt, rt, ndtype, unk, nfo) {}
};

struct SkinnedMeshRendererStruct : public GameObjectNode {
	GameObjectNode* parent_object;
	uint32_t flag1;
	uint32_t flags_unk;
	uint32_t max_unk;
	float float_unk1;
	float float_unk2;
	float float_unk3;
	float float_unk4;
	float float_unk5;
	float float_unk6;
	float float_unk7;
	float float_unk8;
	uint32_t child_material_amount;
	vector<GameObjectNode*> child_material;
	uint32_t flag3;
	uint32_t flag4;
	uint32_t flag5;
	uint32_t flag6;
	uint32_t flag7;
	uint32_t flag8;
	uint32_t flag9;
	uint32_t flag10;
	uint32_t flag11;
	uint32_t flag12;
	uint32_t flag13;
	uint32_t flag14;
	uint32_t flag15;
	GameObjectNode* child_mesh;
	uint32_t child_bone_amount;
	vector<GameObjectNode*> child_bone;
	uint32_t flag16;
	GameObjectNode* child_bone_sample;
	float center_x; // Weird coordinates ; maybe barycenter?
	float center_y;
	float center_z;
	float radius_x;
	float radius_y;
	float radius_z;
	float float_unk9;

	SkinnedMeshRendererStruct(GameObjectNode* prt, GameObjectHierarchy& rt, uint32_t ndtype, uint32_t unk, uint64_t nfo) : GameObjectNode(prt, rt, ndtype, unk, nfo) {}
};

struct GameObjectHierarchy {
	GameObjectNode* root_node = NULL;
	vector<GameObjectNode*> node_list;

	UnityArchiveMetaData* meta_data;

	void DEBUGDisplayHierarchy();

	void BuildHierarchy(fstream& archivefile, UnityArchiveMetaData& metadata, unsigned int rootfileindex);
	~GameObjectHierarchy();

	GameObjectNode* FindObjectByInfo(uint64_t info);

	// Setup new informations (node_info, file_index, ...) for this->node_list[]
	// Both "this" and "base" should share the same meta_data
	// mergepolicies:
	// 0=Only setup the nodes that are already existing (node_info==0 for the others)
	// 1=Setup the nodes that are already existing and assign new file informations for the others
	// 2=Assign new informations to all the nodes
	void MergeHierarchy(GameObjectHierarchy* base, int mergepolicy);

	static uint64_t GetRootInfoFromObject(uint8_t* objbuffer);
};

#endif
