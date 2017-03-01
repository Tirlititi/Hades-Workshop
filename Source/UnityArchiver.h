#ifndef _UNITY_ARCHIVER_H
#define _UNITY_ARCHIVER_H

struct UnityArchiveMetaData;
struct UnityArchiveIndexListData;
struct UnityArchiveAssetBundle;

#include "File_IOCommon.h"

enum UnityArchiveFile {
	UNITY_ARCHIVE_DATA11 = 0,
	UNITY_ARCHIVE_DATA12,
	UNITY_ARCHIVE_DATA13,
	UNITY_ARCHIVE_DATA14,
	UNITY_ARCHIVE_DATA15,
	UNITY_ARCHIVE_DATA16,
	UNITY_ARCHIVE_DATA17,
	UNITY_ARCHIVE_DATA18,
	UNITY_ARCHIVE_DATA19,
	UNITY_ARCHIVE_DATA2,
	UNITY_ARCHIVE_DATA3,
	UNITY_ARCHIVE_DATA4,
	UNITY_ARCHIVE_DATA5,
	UNITY_ARCHIVE_DATA61,
	UNITY_ARCHIVE_DATA62,
	UNITY_ARCHIVE_DATA63,
	UNITY_ARCHIVE_DATA7,
	UNITY_ARCHIVE_MAINDATA,
	UNITY_ARCHIVE_RESOURCES,
	UNITY_ARCHIVE_LEVEL0,
	UNITY_ARCHIVE_LEVEL1,
	UNITY_ARCHIVE_LEVEL2,
	UNITY_ARCHIVE_LEVEL3,
	UNITY_ARCHIVE_LEVEL4,
	UNITY_ARCHIVE_LEVEL5,
	UNITY_ARCHIVE_LEVEL6,
	UNITY_ARCHIVE_LEVEL7,
	UNITY_ARCHIVE_LEVEL8,
	UNITY_ARCHIVE_LEVEL9,
	UNITY_ARCHIVE_LEVEL10,
	UNITY_ARCHIVE_LEVEL11,
	UNITY_ARCHIVE_LEVEL12,
	UNITY_ARCHIVE_LEVEL13,
	UNITY_ARCHIVE_LEVEL14,
	UNITY_ARCHIVE_LEVEL15,
	UNITY_ARCHIVE_LEVEL16,
	UNITY_ARCHIVE_LEVEL17,
	UNITY_ARCHIVE_LEVEL18,
	UNITY_ARCHIVE_LEVEL19,
	UNITY_ARCHIVE_LEVEL20,
	UNITY_ARCHIVE_LEVEL21,
	UNITY_ARCHIVE_LEVEL22,
	UNITY_ARCHIVE_LEVEL23,
	UNITY_ARCHIVE_LEVEL24,
	UNITY_ARCHIVE_LEVEL25,
	UNITY_ARCHIVE_LEVEL26,
	UNITY_ARCHIVE_LEVEL27,
	UNITY_ARCHIVE_SHARED0,
	UNITY_ARCHIVE_SHARED1,
	UNITY_ARCHIVE_SHARED2,
	UNITY_ARCHIVE_SHARED3,
	UNITY_ARCHIVE_SHARED4,
	UNITY_ARCHIVE_SHARED5,
	UNITY_ARCHIVE_SHARED6,
	UNITY_ARCHIVE_SHARED7,
	UNITY_ARCHIVE_SHARED8,
	UNITY_ARCHIVE_SHARED9,
	UNITY_ARCHIVE_SHARED10,
	UNITY_ARCHIVE_SHARED11,
	UNITY_ARCHIVE_SHARED12,
	UNITY_ARCHIVE_SHARED13,
	UNITY_ARCHIVE_SHARED14,
	UNITY_ARCHIVE_SHARED15,
	UNITY_ARCHIVE_SHARED16,
	UNITY_ARCHIVE_SHARED17,
	UNITY_ARCHIVE_SHARED18,
	UNITY_ARCHIVE_SHARED19,
	UNITY_ARCHIVE_SHARED20,
	UNITY_ARCHIVE_SHARED21,
	UNITY_ARCHIVE_SHARED22,
	UNITY_ARCHIVE_SHARED23,
	UNITY_ARCHIVE_SHARED24,
	UNITY_ARCHIVE_SHARED25,
	UNITY_ARCHIVE_SHARED26,
	UNITY_ARCHIVE_SHARED27,
	UNITY_ARCHIVE_SHARED28,
	UNITY_ARCHIVE_AMOUNT
};

struct UnityArchiveMetaData {
	uint8_t archive_type; // p0data[n].bin or .assets
	bool loaded;
	
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
	void Flush();
	uint32_t GetFileSizeByIndex(unsigned int fileid);
	uint32_t GetFileOffset(string filename, uint32_t filetype = 0xFFFFFFFF, unsigned int num = 0, string folder = "");
	uint32_t GetFileOffsetByInfo(uint64_t info, uint32_t filetype = 0xFFFFFFFF, string folder = "");
	uint32_t GetFileOffsetByIndex(unsigned int fileid, string folder = "");
	int32_t GetFileIndex(string filename, uint32_t filetype = 0xFFFFFFFF, unsigned int num = 0, string folder = "");
	int32_t GetFileIndexByInfo(uint64_t info, uint32_t filetype = 0xFFFFFFFF, string folder = "");
	
	// Return the starting offset of the files in the duplicate (must be deleted[])
	// Arrays must be of length header_file_amount
	uint32_t* Duplicate(fstream& fbase, fstream& fdest, bool* copylist, uint32_t* filenewsize);
	
	UnityArchiveMetaData() : loaded(false) {}
	~UnityArchiveMetaData() { Flush(); }
	
	static string GetArchiveName(UnityArchiveFile file, bool x86 = false);
	static string GetTypeName(uint32_t type);
};

struct UnityArchiveIndexListData {
	bool loaded;
	uint32_t amount;
	string* path;
	uint32_t* unk1;
	uint32_t* index;
	uint32_t* unk2;
	
	int Load(fstream& f);
	void Flush();
	uint32_t GetFileIndex(string filepath);
	
	UnityArchiveIndexListData() : loaded(false) {}
	~UnityArchiveIndexListData() { Flush(); }
};

struct UnityArchiveAssetBundle {
	bool loaded;
	uint32_t amount;
	string* path;
	uint32_t* index;
	uint32_t* unk1;
	uint32_t* unk2;
	uint64_t* info;
	
	int Load(fstream& f);
	void Flush();
	uint32_t GetFileIndex(string filepath);
	uint64_t GetFileInfo(string filepath);
	
	UnityArchiveAssetBundle() : loaded(false) {}
	~UnityArchiveAssetBundle() { Flush(); }
};

#endif
