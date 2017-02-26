#include "UnityArchiver.h"

#define X64_FOLDER "x64\\"
#define X86_FOLDER "x86\\"
#define STREAMING_FILE_BASE "StreamingAssets\\p0data"
#define LEVEL_FILE_BASE "FF9_Data\\level"
#define SHARED_FILE_BASE "FF9_Data\\sharedassets"
#define MAINDATA_FILE_NAME "FF9_Data\\mainData"
#define RESOURCES_FILE_NAME "FF9_Data\\resources.assets"
#define STREAMING_FILE_EXT ".bin"
#define SHARED_FILE_EXT ".assets"

string UnityArchiveMetaData::GetArchiveName(UnityArchiveFile file, bool x86) {
	string result = "";
	if (file>=UNITY_ARCHIVE_DATA11 && file<=UNITY_ARCHIVE_DATA7) {
		result = STREAMING_FILE_BASE;
		if (file<=UNITY_ARCHIVE_DATA19)
			result += ConvertToString(10+file-UNITY_ARCHIVE_DATA11+1);
		else if (file>=UNITY_ARCHIVE_DATA2 && file<=UNITY_ARCHIVE_DATA5)
			result += ConvertToString(file-UNITY_ARCHIVE_DATA2+2);
		else if (file>=UNITY_ARCHIVE_DATA61 && file<=UNITY_ARCHIVE_DATA63)
			result += ConvertToString(60+file-UNITY_ARCHIVE_DATA61+1);
		else
			result += ConvertToString(7);
		result += STREAMING_FILE_EXT;
	} else if (file<UNITY_ARCHIVE_AMOUNT) {
		result = x86 ? X86_FOLDER : X64_FOLDER;
		if (file==UNITY_ARCHIVE_MAINDATA) {
			result += MAINDATA_FILE_NAME;
		} else if (file==UNITY_ARCHIVE_RESOURCES) {
			result += RESOURCES_FILE_NAME;
		} else if (file>=UNITY_ARCHIVE_LEVEL0 && file<=UNITY_ARCHIVE_LEVEL27) {
			result += LEVEL_FILE_BASE;
			result += ConvertToString(file-UNITY_ARCHIVE_LEVEL0);
		} else {
			result += SHARED_FILE_BASE;
			result += ConvertToString(file-UNITY_ARCHIVE_SHARED0);
			result += SHARED_FILE_EXT;
		}
	}
	return result;
}

string UnityArchiveMetaData::GetTypeName(uint32_t type) {
	switch (type) {
		case 1: return "GameObject";
		case 2: return "Component";
		case 3: return "LevelGameManager";
		case 4: return "Transform";
		case 5: return "TimeManager";
		case 6: return "GlobalGameManager";
		case 8: return "Behaviour";
		case 9: return "GameManager";
		case 11: return "AudioManager";
		case 12: return "ParticleAnimator";
		case 13: return "InputManager";
		case 15: return "EllipsoidParticleEmitter";
		case 17: return "Pipeline";
		case 18: return "EditorExtension";
		case 19: return "Physics2DSettings";
		case 20: return "Camera";
		case 21: return "Material";
		case 23: return "MeshRenderer";
		case 25: return "Renderer";
		case 26: return "ParticleRenderer";
		case 27: return "Texture";
		case 28: return "Texture2D";
		case 29: return "SceneSettings";
		case 30: return "GraphicsSettings";
		case 33: return "MeshFilter";
		case 41: return "OcclusionPortal";
		case 43: return "Mesh";
		case 45: return "Skybox";
		case 47: return "QualitySettings";
		case 48: return "Shader";
		case 49: return "TextAsset";
		case 50: return "Rigidbody2D";
		case 51: return "Physics2DManager";
		case 53: return "Collider2D";
		case 54: return "Rigidbody";
		case 55: return "PhysicsManager";
		case 56: return "Collider";
		case 57: return "Joint";
		case 58: return "CircleCollider2D";
		case 59: return "HingeJoint";
		case 60: return "PolygonCollider2D";
		case 61: return "BoxCollider2D";
		case 62: return "PhysicsMaterial2D";
		case 64: return "MeshCollider";
		case 65: return "BoxCollider";
		case 66: return "SpriteCollider2D";
		case 68: return "EdgeCollider2D";
		case 72: return "ComputeShader";
		case 74: return "AnimationClip";
		case 75: return "ConstantForce";
		case 76: return "WorldParticleCollider";
		case 78: return "TagManager";
		case 81: return "AudioListener";
		case 82: return "AudioSource";
		case 83: return "AudioClip";
		case 84: return "RenderTexture";
		case 87: return "MeshParticleEmitter";
		case 88: return "ParticleEmitter";
		case 89: return "Cubemap";
		case 90: return "Avatar";
		case 91: return "AnimatorController";
		case 92: return "GUILayer";
		case 93: return "RuntimeAnimatorController";
		case 94: return "ScriptMapper";
		case 95: return "Animator";
		case 96: return "TrailRenderer";
		case 98: return "DelayedCallManager";
		case 102: return "TextMesh";
		case 104: return "RenderSettings";
		case 108: return "Light";
		case 109: return "CGProgram";
		case 110: return "BaseAnimationTrack";
		case 111: return "Animation";
		case 114: return "MonoBehaviour";
		case 115: return "MonoScript";
		case 116: return "MonoManager";
		case 117: return "Texture3D";
		case 118: return "NewAnimationTrack";
		case 119: return "Projector";
		case 120: return "LineRenderer";
		case 121: return "Flare";
		case 122: return "Halo";
		case 123: return "LensFlare";
		case 124: return "FlareLayer";
		case 125: return "HaloLayer";
		case 126: return "NavMeshAreas";
		case 127: return "HaloManager";
		case 128: return "Font";
		case 129: return "PlayerSettings";
		case 130: return "NamedObject";
		case 131: return "GUITexture";
		case 132: return "GUIText";
		case 133: return "GUIElement";
		case 134: return "PhysicMaterial";
		case 135: return "SphereCollider";
		case 136: return "CapsuleCollider";
		case 137: return "SkinnedMeshRenderer";
		case 138: return "FixedJoint";
		case 140: return "RaycastCollider";
		case 141: return "BuildSettings";
		case 142: return "AssetBundle";
		case 143: return "CharacterController";
		case 144: return "CharacterJoint";
		case 145: return "SpringJoint";
		case 146: return "WheelCollider";
		case 147: return "ResourceManager";
		case 148: return "NetworkView";
		case 149: return "NetworkManager";
		case 150: return "PreloadData";
		case 152: return "MovieTexture";
		case 153: return "ConfigurableJoint";
		case 154: return "TerrainCollider";
		case 155: return "MasterServerInterface";
		case 156: return "TerrainData";
		case 157: return "LightmapSettings";
		case 158: return "WebCamTexture";
		case 159: return "EditorSettings";
		case 160: return "InteractiveCloth";
		case 161: return "ClothRenderer";
		case 162: return "EditorUserSettings";
		case 163: return "SkinnedCloth";
		case 164: return "AudioReverbFilter";
		case 165: return "AudioHighPassFilter";
		case 166: return "AudioChorusFilter";
		case 167: return "AudioReverbZone";
		case 168: return "AudioEchoFilter";
		case 169: return "AudioLowPassFilter";
		case 170: return "AudioDistortionFilter";
		case 171: return "SparseTexture";
		case 180: return "AudioBehaviour";
		case 181: return "AudioFilter";
		case 182: return "WindZone";
		case 183: return "Cloth";
		case 184: return "SubstanceArchive";
		case 185: return "ProceduralMaterial";
		case 186: return "ProceduralTexture";
		case 191: return "OffMeshLink";
		case 192: return "OcclusionArea";
		case 193: return "Tree";
		case 194: return "NavMeshObsolete";
		case 195: return "NavMeshAgent";
		case 196: return "NavMeshSettings";
		case 197: return "LightProbesLegacy";
		case 198: return "ParticleSystem";
		case 199: return "ParticleSystemRenderer";
		case 200: return "ShaderVariantCollection";
		case 205: return "LODGroup";
		case 206: return "BlendTree";
		case 207: return "Motion";
		case 208: return "NavMeshObstacle";
		case 210: return "TerrainInstance";
		case 212: return "SpriteRenderer";
		case 213: return "Sprite";
		case 214: return "CachedSpriteAtlas";
		case 215: return "ReflectionProbe";
		case 216: return "ReflectionProbes";
		case 220: return "LightProbeGroup";
		case 221: return "AnimatorOverrideController";
		case 222: return "CanvasRenderer";
		case 223: return "Canvas";
		case 224: return "RectTransform";
		case 225: return "CanvasGroup";
		case 226: return "BillboardAsset";
		case 227: return "BillboardRenderer";
		case 228: return "SpeedTreeWindAsset";
		case 229: return "AnchoredJoint2D";
		case 230: return "Joint2D";
		case 231: return "SpringJoint2D";
		case 232: return "DistanceJoint2D";
		case 233: return "HingeJoint2D";
		case 234: return "SliderJoint2D";
		case 235: return "WheelJoint2D";
		case 238: return "NavMeshData";
		case 240: return "AudioMixer";
		case 241: return "AudioMixerController";
		case 243: return "AudioMixerGroupController";
		case 244: return "AudioMixerEffectController";
		case 245: return "AudioMixerSnapshotController";
		case 246: return "PhysicsUpdateBehaviour2D";
		case 247: return "ConstantForce2D";
		case 248: return "Effector2D";
		case 249: return "AreaEffector2D";
		case 250: return "PointEffector2D";
		case 251: return "PlatformEffector2D";
		case 252: return "SurfaceEffector2D";
		case 258: return "LightProbes";
		case 271: return "SampleClip";
		case 272: return "AudioMixerSnapshot";
		case 273: return "AudioMixerGroup";
		case 290: return "AssetBundleManifest";
/*		case : return "Prefab";
		case : return "EditorExtensionImpl";
		case : return "AssetImporter";
		case : return "AssetDatabase";
		case : return "Mesh3DSImporter";
		case : return "TextureImporter";
		case : return "ShaderImporter";
		case : return "ComputeShaderImporter";
		case : return "AvatarMask";
		case : return "AudioImporter";
		case : return "HierarchyState";
		case : return "GUIDSerializer";
		case : return "AssetMetaData";
		case : return "DefaultAsset";
		case : return "DefaultImporter";
		case : return "TextScriptImporter";
		case : return "SceneAsset";
		case : return "NativeFormatImporter";
		case : return "MonoImporter";
		case : return "AssetServerCache";
		case : return "LibraryAssetImporter";
		case : return "ModelImporter";
		case : return "FBXImporter";
		case : return "TrueTypeFontImporter";
		case : return "MovieImporter";
		case : return "EditorBuildSettings";
		case : return "DDSImporter";
		case : return "InspectorExpandedState";
		case : return "AnnotationManager";
		case : return "PluginImporter";
		case : return "EditorUserBuildSettings";
		case : return "PVRImporter";
		case : return "ASTCImporter";
		case : return "KTXImporter";
		case : return "AnimatorStateTransition";
		case : return "AnimatorState";
		case : return "HumanTemplate";
		case : return "AnimatorStateMachine";
		case : return "PreviewAssetType";
		case : return "AnimatorTransition";
		case : return "SpeedTreeImporter";
		case : return "AnimatorTransitionBase";
		case : return "SubstanceImporter";
		case : return "LightmapParameters";
		case : return "LightmapSnapshot";*/
	}
	return "";
}

bool HasFileTypeName(uint32_t type) {
	return type==21 || type==28 || type==43 || type==48 || type==49 || type==109 || type==115 || type==213;
}

int UnityArchiveMetaData::Load(fstream& f) {
	unsigned int i,j;
	string mgc = "";
	uint32_t archivestart;
	for (i=0;i<8;i++)
		mgc.push_back(f.get());
	if (mgc.compare("UnityRaw")==0) {
		archivestart = 0x70;
		archive_type = 1;
	} else {
		archivestart = 0;
		archive_type = 0;
	}
	f.seekg(archivestart);
	header_size = ReadLongBE(f);
	header_file_size = ReadLongBE(f);
	header_id = ReadLongBE(f);
	header_file_offset = ReadLongBE(f);
	header_unknown1 = ReadLongBE(f);
	if (header_id!=0x0F)
		return 1;
	header_version = "";
	for (i=0;i<8;i++)
		header_version.push_back(f.get());
	header_unknown2 = ReadLong(f);
	header_unknown3 = f.get();
	header_file_info_amount = ReadLong(f);
	file_info_type = new int32_t[header_file_info_amount];
	if (header_unknown2==0x5) {
		file_info_unkstruct_amount = new uint32_t[header_file_info_amount];
		file_info_unkstruct_text_size = new uint32_t[header_file_info_amount];
	}
	for (i=0;i<header_file_info_amount;i++) {
		file_info_type[i] = ReadLong(f);
		if (file_info_type[i]>=0)
			f.seekg(0x10,ios::cur);
		else
			f.seekg(0x20,ios::cur);
		if (header_unknown2==0x5) {
			file_info_unkstruct_amount[i] = ReadLong(f);
			file_info_unkstruct_text_size[i] = ReadLong(f);
			f.seekg(file_info_unkstruct_amount[i]*0x18+file_info_unkstruct_text_size[i],ios::cur);
		}
	}
	header_file_amount = ReadLong(f);
	f.seekg(GetAlignOffset(f.tellg()),ios::cur);
	file_info = new uint64_t[header_file_amount];
	file_offset_start = new uint32_t[header_file_amount];
	file_size = new uint32_t[header_file_amount];
	file_type1 = new uint32_t[header_file_amount];
	file_type2 = new uint32_t[header_file_amount];
	file_unknown2 = new uint32_t[header_file_amount];
	file_name_len = new uint32_t[header_file_amount];
	file_name = new string[header_file_amount];
	for (i=0;i<header_file_amount;i++) {
		file_info[i] = ReadLongLong(f);
		file_offset_start[i] = ReadLong(f);
		file_size[i] = ReadLong(f);
		file_type1[i] = ReadLong(f);
		file_type2[i] = ReadLong(f);
		file_unknown2[i] = ReadLong(f);
		if (HasFileTypeName(file_type1[i])) {
			size_t curpos = f.tellg();
			f.seekg(archivestart+header_file_offset+file_offset_start[i]);
			file_name_len[i] = ReadLong(f);
			file_name[i] = "";
			for (j=0;j<file_name_len[i];j++)
				file_name[i].push_back(f.get());
			f.seekg(curpos);
		} else {
			file_name_len[i] = 0;
			file_name[i] = "";
		}
	}
	return 0;
}

uint32_t UnityArchiveMetaData::GetFileSizeById(unsigned int id) {
	if (id>=header_file_amount)
		return 0;
	if (HasFileTypeName(file_type1[id]))
		return file_size[id]-file_name_len[id]-4;
	return file_size[id];
}

uint32_t UnityArchiveMetaData::GetFileOffset(string filename, uint32_t filetype, unsigned int num, string folder) {
	int i = GetFileIndex(filename,filetype,num,folder);
	if (i>=0)
		return GetFileOffsetByIndex(i);
	return 0;
}

uint32_t UnityArchiveMetaData::GetFileOffsetByInfo(uint64_t info, uint32_t filetype, string folder) {
	int i = GetFileIndexByInfo(info,filetype,folder);
	if (i>=0)
		return GetFileOffsetByIndex(i);
	return 0;
}

uint32_t UnityArchiveMetaData::GetFileOffsetByIndex(unsigned int fileid, string folder) {
	if (fileid>=header_file_amount)
		return 0;
	uint32_t res = archive_type==1 ? 0x70 : 0;
	res += header_file_offset+file_offset_start[fileid];
	if (HasFileTypeName(file_type1[fileid]))
		res += file_name_len[fileid]+GetAlignOffset(file_name_len[fileid])+4;
	return res;
}

int32_t UnityArchiveMetaData::GetFileIndex(string filename, uint32_t filetype, unsigned int num, string folder) {
	unsigned int i;
	for (i=0;i<header_file_amount;i++)
		if (file_name[i].compare(filename)==0 && (filetype==0xFFFFFFFF || filetype==file_type1[i])) {
			if (num==0)
				return i;
			else
				num--;
		}
	return -1;
}

int32_t UnityArchiveMetaData::GetFileIndexByInfo(uint64_t info, uint32_t filetype, string folder) {
	unsigned int i;
	for (i=0;i<header_file_amount;i++)
		if (file_info[i]==info && (filetype==0xFFFFFFFF || filetype==file_type1[i]))
			return i;
	return -1;
}

uint32_t* UnityArchiveMetaData::Duplicate(fstream& fbase, fstream& fdest, bool* copylist, uint32_t* filenewsize) {
	uint32_t archivestart = archive_type==1 ? 0x70 : 0;
	uint32_t* res = new uint32_t[header_file_amount];
	uint32_t copysize, offstart, offtmp, filenewsizetmp;
	unsigned int i,j;
	char* buffer;
	copysize = archivestart+0x25; // up to file_info data
	for (i=0;i<header_file_info_amount;i++) {
		if (file_info_type[i]>=0)
			copysize += 0x14;
		else
			copysize += 0x24;
		if (header_unknown2==0x5)
			copysize += 0x18*file_info_unkstruct_amount[i]+file_info_unkstruct_text_size[i]+8;
	}
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	WriteLong(fdest,header_file_amount);
	fdest.seekp(GetAlignOffset(fdest.tellp()),ios::cur);
	offstart = 0;
	for (i=0;i<header_file_amount;i++) {
		filenewsizetmp = filenewsize[i];
		WriteLongLong(fdest,file_info[i]);
		WriteLong(fdest,offstart);
		if (copylist[i]) {
			WriteLong(fdest,file_size[i]);
		} else {
			if (HasFileTypeName(file_type1[i]))
				filenewsizetmp += 4+file_name_len[i]+GetAlignOffset(file_name_len[i]);
			filenewsizetmp++; // DEBUG : text files and some others use this kind of padding but not all files do
			filenewsizetmp += GetAlignOffset(filenewsizetmp,8);
			WriteLong(fdest,filenewsizetmp);
		}
		WriteLong(fdest,file_type1[i]);
		WriteLong(fdest,file_type2[i]);
		WriteLong(fdest,file_unknown2[i]);
		offtmp = fdest.tellp();
		fdest.seekp(archivestart+header_file_offset+offstart);
		if (HasFileTypeName(file_type1[i])) {
			WriteLong(fdest,file_name_len[i]);
			for (j=0;j<file_name_len[i];j++)
				fdest.put(file_name[i].at(j));
			fdest.seekp(GetAlignOffset(fdest.tellp()),ios::cur);
		}
		res[i] = fdest.tellp();
		if (copylist[i]) {
			copysize = file_size[i];
			if (HasFileTypeName(file_type1[i]))
				copysize -= 4+file_name_len[i]+GetAlignOffset(file_name_len[i]);
			fbase.seekg(GetFileOffsetByIndex(i));
			buffer = new char[copysize];
			fbase.read(buffer,copysize);
			fdest.write(buffer,copysize);
			delete[] buffer;
		}
		fdest.seekp(offtmp);
		if (copylist[i])
			offstart += file_size[i];
		else
			offstart += filenewsizetmp;
		offstart += GetAlignOffset(offstart,8);
	}
	if (offtmp<archivestart+header_file_offset) {
		copysize = archivestart+header_file_offset-offtmp;
		fbase.seekg(offtmp);
		buffer = new char[copysize];
		fbase.read(buffer,copysize);
		fdest.write(buffer,copysize);
		delete[] buffer;
	}
	fdest.seekp(0,ios::end);
	while (fdest.tellp()%4!=0)
		fdest.put(0);
	uint32_t fdestsize = fdest.tellp();
	if (archive_type==0) {
		fdest.seekp(4);
		WriteLongBE(fdest,fdestsize);
	} else {
		uint32_t size;
		fdest.seekp(0x1B);
		WriteLongBE(fdest,fdestsize);
		fdest.seekp(0x2B);
		size = fdestsize-0x3C;
		WriteLongBE(fdest,size);
		WriteLongBE(fdest,size);
		WriteLongBE(fdest,fdestsize);
		fdest.seekp(0x69);
		size = fdestsize-archivestart;
		WriteLongBE(fdest,size);
		fdest.seekp(archivestart+4);
		WriteLongBE(fdest,size);
	}
	return res;
}

int UnityArchiveIndexListData::Load(fstream& f) {
	uint32_t fnamelen;
	amount = ReadLong(f);
	path = new string[amount];
	unk1 = new uint32_t[amount];
	index = new uint32_t[amount];
	unk2 = new uint32_t[amount];
	unsigned int i,j;
	for (i=0;i<amount;i++) {
		fnamelen = ReadLong(f);
		path[i] = "";
		for (j=0;j<fnamelen;j++)
			path[i].push_back(f.get());
		f.seekg(GetAlignOffset(fnamelen),ios::cur);
		unk1[i] = ReadLong(f);
		index[i] = ReadLong(f);
		unk2[i] = ReadLong(f);
	}
	return 0;
}

uint32_t UnityArchiveIndexListData::GetFileIndex(string filepath) {
	for (unsigned int i=0;i<amount;i++)
		if (filepath.compare(path[i])==0)
			return index[i];
	return 0;
}

int UnityArchiveAssetBundle::Load(fstream& f) {
	uint32_t fnamelen;
	uint32_t unkinfoamount;
	ReadLong(f);
	unkinfoamount = ReadLong(f);
	f.seekg(unkinfoamount*12,ios::cur);
	amount = ReadLong(f);
	path = new string[amount];
	index = new uint32_t[amount];
	unk1 = new uint32_t[amount];
	unk2 = new uint32_t[amount];
	info = new uint64_t[amount];
	unsigned int i,j;
	for (i=0;i<amount;i++) {
		fnamelen = ReadLong(f);
		path[i] = "";
		for (j=0;j<fnamelen;j++)
			path[i].push_back(f.get());
		f.seekg(GetAlignOffset(fnamelen),ios::cur);
		index[i] = ReadLong(f);
		unk1[i] = ReadLong(f);
		unk2[i] = ReadLong(f);
		info[i] = ReadLong(f);
		info[i] |= ((uint64_t)ReadLong(f) << 32);
	}
	return 0;
}

uint32_t UnityArchiveAssetBundle::GetFileIndex(string filepath) {
	for (unsigned int i=0;i<amount;i++)
		if (filepath.compare(path[i])==0)
			return index[i];
	return 0;
}

uint64_t UnityArchiveAssetBundle::GetFileInfo(string filepath) {
	for (unsigned int i=0;i<amount;i++)
		if (filepath.compare(path[i])==0)
			return info[i];
	return 0;
}
