#ifndef _UNITY_ARCHIVER_H
#define _UNITY_ARCHIVER_H

struct UnityArchiveFileCreator;
struct UnityArchiveMetaData;
struct UnityArchiveIndexListData;
struct UnityArchiveAssetBundle;

#include <vector>
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

struct UnityArchiveFileCreator {
	UnityArchiveMetaData* meta_data;

	vector<int64_t> file_info;
	vector<uint32_t> file_size;
	vector<uint32_t> file_type;
	vector<uint32_t> file_unknown;
	vector<string> file_name;
	vector<uint32_t> file_index;

	UnityArchiveFileCreator(UnityArchiveMetaData* refdata) : meta_data(refdata) {}

	void Add(uint32_t type, uint32_t size, int64_t info, string name = "", uint32_t unk = 0);
};

struct UnityArchiveMetaData {
	uint8_t archive_type; // p0data[n].bin or .assets
	UnityArchiveFile archive_file;
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
	vector<int32_t> file_info_type;
	vector<uint32_t> file_info_unkstruct_amount;
	vector<uint32_t> file_info_unkstruct_text_size;
	// 0x10 or 0x20 info data
	// + unkstruct
	uint32_t header_file_amount;
	vector<int64_t> file_info;
	vector<uint32_t> file_offset_start;
	vector<uint32_t> file_size;
	vector<uint32_t> file_type1;
	vector<uint32_t> file_type2;
	vector<uint32_t> file_flags; // Often 0 (not always)
	vector<uint32_t> file_name_len;
	vector<string> file_name;
	
	vector<uint32_t> text_file_size;
	
	int Load(fstream& f, UnityArchiveFile file);
	void Flush();
	void Copy(UnityArchiveMetaData* base, bool copyfiles = true);
	uint32_t GetFileSizeByIndex(unsigned int fileid);
	uint32_t GetFileOffset(string filename, uint32_t filetype = 0x7FFFFFFF, unsigned int num = 0, string folder = "");
	uint32_t GetFileOffsetByInfo(int64_t info, uint32_t filetype = 0x7FFFFFFF, string folder = "");
	uint32_t GetFileOffsetByIndex(unsigned int fileid, string folder = "");
	int32_t GetFileIndex(string filename, uint32_t filetype = 0x7FFFFFFF, unsigned int num = 0, string folder = "");
	int32_t GetFileIndexByInfo(int64_t info, uint32_t filetype = 0x7FFFFFFF, string folder = "");
	string GetFileFullName(unsigned int fileid, UnityArchiveFile archivefile, UnityArchiveAssetBundle* bundle = NULL, UnityArchiveIndexListData* indexlist = NULL, bool* found = NULL);

	// Arrays must be of length header_file_amount
	// Return the starting offset of the files in the duplicate (must be deleted[] if newmetadata is not given)
	// If a newmetadata is given, an UnityArchiveMetaData compatible with the duplicate is computed and the offsets returned are file_offset_start instead (use GetFileOffsetByIndex instead)
	vector<uint32_t> Duplicate(fstream& fbase, fstream& fdest, vector<bool> copylist, vector<uint32_t> filenewsize, UnityArchiveFileCreator* addfile = NULL, UnityArchiveMetaData* newmetadata = NULL);
	// Array must be of length header_file_amount
	void DuplicateWithDeletion(fstream& fbase, fstream& fdest, vector<bool> filetokeep);

	UnityArchiveMetaData() : loaded(false) {}
	~UnityArchiveMetaData() { Flush(); }
	
	static string GetArchiveName(UnityArchiveFile file, bool x86 = false);
	static string GetTypeName(uint32_t type);
	static bool HasFileTypeName(uint32_t type);
};

struct UnityArchiveIndexListData {
	bool loaded;
	uint32_t amount;
	vector<string> path;
	vector<uint32_t> archive_file;
	vector<uint32_t> index;
	vector<uint32_t> unk;

	unsigned int data_part2_amount;
	vector<uint32_t> data_part2;
	
	int Load(fstream& f, uint32_t datasize);
	void Write(fstream& f);
	void Flush();
	uint32_t GetFileIndex(string filepath);
	
	UnityArchiveIndexListData() : loaded(false) {}
	~UnityArchiveIndexListData() { Flush(); }
	
	static UnityArchiveFile SteamToHWSArchiveFile(uint32_t type);
	static uint32_t HWSToSteamArchiveFile(UnityArchiveFile type);
};

struct UnityArchiveAssetBundle {
	uint32_t amount;
	vector<string> path;
	vector<uint32_t> index;
	vector<uint32_t> unk1;
	vector<uint32_t> unk2;
	vector<int64_t> info;

	uint32_t bundle_amount;
	vector<uint32_t> bundle_flag;
	vector<int64_t> bundle_info;

	uint32_t tail_unk1;
	uint32_t tail_unk2;
	uint32_t tail_unk3;
	uint32_t tail_unk4;
	uint32_t tail_unk5;
	uint32_t tail_unk6;
	string tail_archive_name;
	uint32_t tail_unk7;
	uint32_t tail_unk8;

	vector<uint32_t> bundle_index_start;
	vector<uint32_t> bundle_index_end;

	int Load(fstream& f);
	void Write(fstream& f);
	uint32_t GetFileIndex(string filepath);
	int64_t GetFileInfo(string filepath);

	int GetFileBundle(int64_t info);
	void AddFile(string filepath, uint32_t fileindex, int64_t fileinfo, uint32_t fileunk1 = 0, uint32_t fileunk2 = 0);
	void AddFileBundle(uint64_t type, vector<int64_t> fileinfolist);
	void AddFileToBundle(int64_t fileinfo, unsigned int bundleindex = 0);
	void RemoveFile(unsigned int fileindex);
	void RemoveFileBundle(unsigned int bundleindex);
	void RemoveFileFromBundle(int64_t fileinfo, unsigned int bundleindex = 0);

	int GetDataSize();
};

struct UnityArchiveDictionary {
	vector<string> parameter_name;
	vector< vector<string> > object;
	
	int GetObjectIndex(string keyparameter, string id); // return -1 if not found
	string GetObjectValue(string valueparameter, unsigned int index);
	void Load(fstream& f, bool append = false);
	void Flush() { parameter_name.clear(); object.clear(); }
};

#endif
