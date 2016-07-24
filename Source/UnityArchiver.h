#ifndef _UNITY_ARCHIVER_H
#define _UNITY_ARCHIVER_H

struct UnityArchiveMetaData;
struct UnityArchiveIndexListData;
struct UnityArchiveAssetBundle;

#include "File_IOCommon.h"

struct UnityArchiveMetaData {
	uint8_t archive_type; // p0data[n].bin or .assets
	
	uint32_t header_size;
	uint32_t header_file_size;
	uint32_t header_id; // must be 0x0f
	uint32_t header_file_offset;
	uint32_t header_unknown1;
	string header_version;
	uint32_t header_unknown2;
	uint8_t header_unknown3;
	uint32_t header_file_info_amount;
	int32_t* file_info_type;
	uint32_t* file_info_unkstruct_amount;
	uint32_t* file_info_unkstruct_text_size;
	// 0x10 or 0x20 info data
	// + unkstruct
	uint32_t header_file_amount;
	uint64_t* file_info;
	uint32_t* file_offset_start;
	uint32_t* file_size;
	uint32_t* file_type1;
	uint32_t* file_type2;
	uint32_t* file_unknown2;
	uint32_t* file_name_len;
	string* file_name;
	
	int Load(fstream& f);
	uint32_t GetFileOffset(string filename, uint32_t filetype = 0xFFFFFFFF, unsigned int num = 0, string folder = "");
	uint32_t GetFileOffsetByInfo(uint64_t info, uint32_t filetype = 0xFFFFFFFF, string folder = "");
	uint32_t GetFileOffsetByIndex(unsigned int fileid, string folder = "");
	int32_t GetFileIndex(string filename, uint32_t filetype = 0xFFFFFFFF, unsigned int num = 0, string folder = "");
	int32_t GetFileIndexByInfo(uint64_t info, uint32_t filetype = 0xFFFFFFFF, string folder = "");
	
	// Return the starting offset of the files in the duplicate (must be deleted[])
	// Arrays must be of length header_file_amount ; they are deleled[] in the process
	uint32_t* Duplicate(fstream& fbase, fstream& fdest, bool* copylist, uint32_t* filenewsize);
};

struct UnityArchiveIndexListData {
	uint32_t amount;
	string* path;
	uint32_t* unk1;
	uint32_t* index;
	uint32_t* unk2;
	
	int Load(fstream& f);
	uint32_t GetFileIndex(string filepath);
};

struct UnityArchiveAssetBundle {
	uint32_t amount;
	string* path;
	uint32_t* index;
	uint32_t* unk1;
	uint32_t* unk2;
	uint64_t* info;
	
	int Load(fstream& f);
	uint32_t GetFileIndex(string filepath);
	uint64_t GetFileInfo(string filepath);
};

#endif
