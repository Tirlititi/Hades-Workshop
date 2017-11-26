#include "UnityArchiver.h"

#include <algorithm>

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
	if (header_unknown3==1) {
		file_info_unkstruct_amount = new uint32_t[header_file_info_amount];
		file_info_unkstruct_text_size = new uint32_t[header_file_info_amount];
	}
	for (i=0;i<header_file_info_amount;i++) {
		file_info_type[i] = ReadLong(f);
		if (file_info_type[i]>=0)
			f.seekg(0x10,ios::cur);
		else
			f.seekg(0x20,ios::cur);
		if (header_unknown3==1) {
			file_info_unkstruct_amount[i] = ReadLong(f);
			file_info_unkstruct_text_size[i] = ReadLong(f);
			f.seekg(file_info_unkstruct_amount[i]*0x18+file_info_unkstruct_text_size[i],ios::cur);
		}
	}
	header_file_amount = ReadLong(f);
	f.seekg(GetAlignOffset(f.tellg()), ios::cur);
	file_info = new uint64_t[header_file_amount];
	file_offset_start = new uint32_t[header_file_amount];
	file_size = new uint32_t[header_file_amount];
	file_type1 = new uint32_t[header_file_amount];
	file_type2 = new uint32_t[header_file_amount];
	file_unknown2 = new uint32_t[header_file_amount];
	file_name_len = new uint32_t[header_file_amount];
	file_name = new string[header_file_amount];
	text_file_size = new uint32_t[header_file_amount];
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
			if (file_type1[i]==49) {
				f.seekg(GetAlignOffset(f.tellg()),ios::cur);
				text_file_size[i] = ReadLong(f);
			} else {
				text_file_size[i] = 0;
			}
			f.seekg(curpos);
		} else {
			file_name_len[i] = 0;
			file_name[i] = "";
			text_file_size[i] = 0;
		}
	}
	loaded = true;
	return 0;
}

void UnityArchiveMetaData::Flush() {
	if (loaded) {
		delete[] file_info_type;
		if (header_unknown3==1) {
			delete[] file_info_unkstruct_amount;
			delete[] file_info_unkstruct_text_size;
		}
		delete[] file_info;
		delete[] file_offset_start;
		delete[] file_size;
		delete[] file_type1;
		delete[] file_type2;
		delete[] file_unknown2;
		delete[] file_name_len;
		delete[] file_name;
		delete[] text_file_size;
		loaded = false;
	}
}

void UnityArchiveMetaData::Copy(UnityArchiveMetaData* base, bool copyfiles) {
	unsigned int i;
	archive_type = base->archive_type;
	loaded = base->loaded;
	header_size = base->header_size;
	header_file_size = base->header_file_size;
	header_id = base->header_id;
	header_file_offset = base->header_file_offset;
	header_unknown1 = base->header_unknown1;
	header_version = base->header_version;
	header_unknown2 = base->header_unknown2;
	header_unknown3 = base->header_unknown3;
	header_file_info_amount = base->header_file_info_amount;
	file_info_type = new int32_t[header_file_info_amount];
	memcpy(file_info_type,base->file_info_type,header_file_info_amount*sizeof(int32_t));
	if (header_unknown3==1) {
		file_info_unkstruct_amount = new uint32_t[header_file_info_amount];
		file_info_unkstruct_text_size = new uint32_t[header_file_info_amount];
		memcpy(file_info_unkstruct_amount,base->file_info_unkstruct_amount,header_file_info_amount*sizeof(uint32_t));
		memcpy(file_info_unkstruct_text_size,base->file_info_unkstruct_text_size,header_file_info_amount*sizeof(uint32_t));
	}
	if (copyfiles) {
		header_file_amount = base->header_file_amount;
		file_info = new uint64_t[header_file_amount];
		file_offset_start = new uint32_t[header_file_amount];
		file_size = new uint32_t[header_file_amount];
		file_type1 = new uint32_t[header_file_amount];
		file_type2 = new uint32_t[header_file_amount];
		file_unknown2 = new uint32_t[header_file_amount];
		file_name_len = new uint32_t[header_file_amount];
		file_name = new string[header_file_amount];
		text_file_size = new uint32_t[header_file_amount];
		memcpy(file_info,base->file_info,header_file_amount*sizeof(uint64_t));
		memcpy(file_offset_start,base->file_offset_start,header_file_amount*sizeof(uint32_t));
		memcpy(file_size,base->file_size,header_file_amount*sizeof(uint32_t));
		memcpy(file_type1,base->file_type1,header_file_amount*sizeof(uint32_t));
		memcpy(file_type2,base->file_type2,header_file_amount*sizeof(uint32_t));
		memcpy(file_unknown2,base->file_unknown2,header_file_amount*sizeof(uint32_t));
		memcpy(file_name_len,base->file_name_len,header_file_amount*sizeof(uint32_t));
		memcpy(text_file_size,base->text_file_size,header_file_amount*sizeof(uint32_t));
		for (i=0;i<header_file_amount;i++)
			file_name[i] = base->file_name[i];
	}
}

uint32_t UnityArchiveMetaData::GetFileSizeByIndex(unsigned int fileid) {
	if (fileid>=header_file_amount)
		return 0;
	if (file_type1[fileid]==49)
		return text_file_size[fileid];
	if (HasFileTypeName(file_type1[fileid]))
		return file_size[fileid]-file_name_len[fileid]-GetAlignOffset(file_name_len[fileid])-4;
	return file_size[fileid];
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
	if (file_type1[fileid]==49)
		res += 4;
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

uint32_t* UnityArchiveMetaData::Duplicate(fstream& fbase, fstream& fdest, bool* copylist, uint32_t* filenewsize, UnityArchiveMetaData* newmetadata) {
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
		if (header_unknown3==1)
			copysize += 0x18*file_info_unkstruct_amount[i]+file_info_unkstruct_text_size[i]+8;
	}
	buffer = new char[copysize];
	fbase.read(buffer,copysize);
	fdest.write(buffer,copysize);
	delete[] buffer;
	WriteLong(fdest,header_file_amount);
	if (newmetadata) {
		newmetadata->Copy(this,false);
		newmetadata->header_file_amount = header_file_amount;
		newmetadata->file_info = new uint64_t[newmetadata->header_file_amount];
		newmetadata->file_offset_start = new uint32_t[newmetadata->header_file_amount];
		newmetadata->file_size = new uint32_t[newmetadata->header_file_amount];
		newmetadata->file_type1 = new uint32_t[newmetadata->header_file_amount];
		newmetadata->file_type2 = new uint32_t[newmetadata->header_file_amount];
		newmetadata->file_unknown2 = new uint32_t[newmetadata->header_file_amount];
		newmetadata->file_name_len = new uint32_t[newmetadata->header_file_amount];
		newmetadata->text_file_size = new uint32_t[newmetadata->header_file_amount];
		newmetadata->file_name = new string[newmetadata->header_file_amount];
		newmetadata->file_offset_start = res;
	}
	fdest.seekp(GetAlignOffset(fdest.tellp()),ios::cur);
	offstart = 0;
	for (i=0;i<header_file_amount;i++) {
		filenewsizetmp = filenewsize[i];
		if (file_type1[i]==49)
			filenewsizetmp += 4;
		WriteLongLong(fdest,file_info[i]);
		WriteLong(fdest,offstart);
		if (copylist[i]) {
			WriteLong(fdest,file_size[i]);
		} else {
			if (HasFileTypeName(file_type1[i]))
				filenewsizetmp += 4+file_name_len[i]+GetAlignOffset(file_name_len[i]);
			if (file_type1[i]==49) { // DEBUG : verify that text files are the only ones using this kind of padding
				filenewsizetmp++;
				filenewsizetmp += GetAlignOffset(filenewsizetmp,8);
			}
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
			if (file_type1[i]==49)
				WriteLong(fdest,copylist[i] ? text_file_size[i] : filenewsize[i]);
		}
		res[i] = fdest.tellp();
		if (copylist[i]) {
			copysize = GetFileSizeByIndex(i);
			fbase.seekg(GetFileOffsetByIndex(i));
			buffer = new char[copysize];
			fbase.read(buffer,copysize);
			fdest.write(buffer,copysize);
			delete[] buffer;
		} else {
			fdest.seekp(res[i]+filenewsize[i]-1);
			fdest.put(0);
		}
		fdest.seekp(offtmp);
		if (newmetadata) {
			newmetadata->file_info[i] = file_info[i];
			newmetadata->file_type1[i] = file_type1[i];
			newmetadata->file_type2[i] = file_type2[i];
			newmetadata->file_unknown2[i] = file_unknown2[i];
			newmetadata->file_name_len[i] = file_name_len[i];
			newmetadata->file_name[i] = file_name[i];
			if (copylist[i]) {
				newmetadata->file_size[i] = file_size[i];
				newmetadata->text_file_size[i] = text_file_size[i];
			} else {
				newmetadata->file_size[i] = filenewsizetmp;
				if (file_type1[i]==49)
					newmetadata->text_file_size[i] = filenewsize[i];
				else
					newmetadata->text_file_size[i] = 0;
			}
			res[i] = offstart;
		}
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
	if (copylist[header_file_amount-1] && file_type1[header_file_amount-1]==49)
		fdest.put(0);
	uint32_t fdestfullfilesize = fdest.tellp();
	while (fdest.tellp()%8!=0)
		fdest.put(0);
	if (archive_type==0) {
		fdest.seekp(4);
		WriteLongBE(fdest,fdestfullfilesize);
	} else {
		uint32_t fdestsize = fdest.tellp();
		uint32_t size;
		fdest.seekp(0x1B);
		WriteLongBE(fdest,fdestsize);
		fdest.seekp(0x2B);
		size = fdestsize-0x3C;
		WriteLongBE(fdest,size);
		WriteLongBE(fdest,size);
		WriteLongBE(fdest,fdestsize);
		fdest.seekp(0x69);
		size = fdestfullfilesize-archivestart;
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
	loaded = true;
	return 0;
}

void UnityArchiveIndexListData::Flush() {
	if (loaded) {
		delete[] path;
		delete[] unk1;
		delete[] index;
		delete[] unk2;
		loaded = false;
	}
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
	loaded = true;
	return 0;
}

void UnityArchiveAssetBundle::Flush() {
	if (loaded) {
		delete[] path;
		delete[] index;
		delete[] unk1;
		delete[] unk2;
		delete[] info;
		loaded = false;
	}
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

int UnityArchiveDictionary::GetObjectIndex(string keyparameter, string id) {
	unsigned int i,paramindex;
	transform(keyparameter.begin(),keyparameter.end(),keyparameter.begin(),::tolower);
	for (i=0;i<parameter_name.size();i++)
		if (keyparameter.compare(parameter_name[i])==0) {
			paramindex = i;
			break;
		}
	if (i==parameter_name.size())
		return -1;
	transform(id.begin(),id.end(),id.begin(),::tolower);
	for (i=0;i<object.size();i++)
		if (id.compare(object[i][paramindex])==0)
			return i;
	return -1;
}

string UnityArchiveDictionary::GetObjectValue(string valueparameter, unsigned int index) {
	unsigned int i,paramindex;
	transform(valueparameter.begin(),valueparameter.end(),valueparameter.begin(),::tolower);
	for (i=0;i<parameter_name.size();i++)
		if (valueparameter.compare(parameter_name[i])==0) {
			paramindex = i;
			break;
		}
	if (i==parameter_name.size())
		return "";
	return object[index][paramindex];
}

void UnityArchiveDictionary::Load(fstream& f, bool append) {
	bool insideobject = false;
	int curvalueindex = -1;
	unsigned int i;
	vector<string> cur;
	string bufferstr;
	char buffer[100];
	char c = 0;
	if (!append)
		Flush();
	while (f.get()!='[') {}
	while (c!=']') {
		if (c=='{') {
			cur.assign(parameter_name.size(),"");
			curvalueindex = -1;
			insideobject = true;
		} else if (c=='}') {
			object.push_back(cur);
			insideobject = false;
		} else if (insideobject && c=='\"') {
			f.get(buffer,100,'\"');
			if (f.gcount()<100)
				f.get();
			bufferstr = buffer;
			transform(bufferstr.begin(),bufferstr.end(),bufferstr.begin(),::tolower);
			if (curvalueindex>=0) {
				cur[curvalueindex] = bufferstr;
				curvalueindex = -1;
			} else if (f.gcount()<100 && f.get()==':') {
				for (i=0;i<parameter_name.size();i++)
					if (bufferstr.compare(parameter_name[i])==0) {
						curvalueindex = i;
						break;
					}
				if (i==parameter_name.size()) {
					curvalueindex = parameter_name.size();
					parameter_name.push_back(bufferstr);
					for (i=0;i<object.size();i++)
						object[i].push_back("");
					cur.push_back("");
				}
			}
		}
		c = f.get();
	}
}
