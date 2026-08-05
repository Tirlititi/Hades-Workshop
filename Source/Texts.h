#ifndef _TEXTS_H
#define _TEXTS_H

struct TextFormatStruct;
struct TextDataEntry;
struct TextDataStruct;
struct TextDataSet;

#define SPECIAL_TEXT_SPELL_NAMING_INDEX_PSX		6
#define SPECIAL_TEXT_SPELL_NAMING_INDEX_STEAM	2
#define SPECIAL_TEXT_LOCALIZATION_INDEX_STEAM	6
struct SpecialTextDataEntry;
struct SpecialTextDataStruct;
struct SpecialTextDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct TextFormatStruct {
	unsigned int length;
	uint8_t opcode;
	vector<uint8_t> data;
};

struct TextDataEntry {
	int id; // Used only for non-vanilla texts
	FF9String txt;
	// Followings are only used for PSX GameType
	uint16_t offset;
	uint8_t size_y;
	uint8_t flag;
	// Followings are only used if has_format == true
	uint16_t format_offset;
	uint16_t size_x;
	uint16_t format_amount;
	vector<TextFormatStruct> format_data;

	void SetDialogBoxSize(uint16_t x, uint16_t y, bool changey);
};

struct TextDataStruct : public ChunkChild {
public:
	uint16_t block_id;
	vector<TextDataEntry> text;
	bool has_format;
	unsigned int base_amount; // Steam only
	
	wchar_t* extended_charmap;

	// Return 0 if success ; 1 if the value is too long
	int AddText(int id, FF9String& value);
	void RemoveText(int index, bool shiftids = false);
	int SetText(int index, wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetText(int index, FF9String& newvalue); // Copy the new value
	int AddFormatCode(int index, uint8_t opcode, uint8_t* arg); // arg must be malloc'ed and have the proper size depending of the opcode
	int RemoveFormatCode(int index, uint8_t formatid);
	int GetTextIndexById(int id);
	FF9String& GetTextById(int id);
	wstring GetTextByFullListIndex(int index, int strtype); // When listing all texts, return the index-th current language's text
	FF9String GetTextByFullListIndex(int index); // Return a matchup with each index-th text for each language
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f, bool multilang = false);
	void WriteHWS(fstream& f, bool multilang = false);
	void WriteSteamPatch(fstream& f, wxArrayString basefile, SteamLanguage lang = GetSteamLanguage());
	void WriteSteam(fstream& f, SteamLanguage lang = GetSteamLanguage());
	int GetDataSize(SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();
};

struct TextDataSet {
public:
	uint16_t amount;
	vector<uint16_t> cluster_id; // PSX only
	vector<uint16_t> struct_id;
	uint16_t main_charmap_index; // PSX only
	vector<wstring> name;
	vector<uint16_t> tim_amount;
	vector<TextDataStruct*> text_data;
	vector<CharmapDataStruct*> charmap; // PSX only
	vector<TIMImageDataStruct*> chartim; // PSX only
	
	void Load(fstream& ffbin, ClusterSet& clusset);
	void Write(fstream& ffbin, ClusterSet& clusset);
	void WritePPF(fstream& ffbin, ClusterSet& clusset);
	// return value is int[3] to be deleted
	// {Number of oversized blocks, Number of unknown data, Number of unused blocks}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	int GetIndexById(uint16_t blockid);
};

struct SpecialTextDataEntry {
	int id;
	FF9String txt;
	uint16_t offset; // PSX only
	uint16_t size_x; // PSX only
	wstring localization_field; // Localization only
};

struct SpecialTextDataStruct {
public:
	vector<SpecialTextDataEntry> text;
	uint32_t space_total; // 4 bytes of "amount" not counted (PSX)
	uint32_t space_used; // Warning: for PSX, space is 16-bits long but for Steam, it's 32-bits long
	bool is_localization; // Steam only
	unsigned int base_amount; // Steam only

	static wstring LocalizationRawToNice(wstring entry);
	static wstring LocalizationNiceToRaw(wstring entry);

	// Return 0 if success ; 1 if the value is too long
	int AddText(int id, FF9String& value);
	int AddTextByKey(wstring key, FF9String& value);
	void RemoveText(int index);
	int SetText(int index, wstring newvalue, SteamLanguage lang = GetSteamLanguage());
	int SetText(int index, FF9String& newvalue); // Copy the new value
	int GetTextIndexById(int id);
	FF9String& GetTextById(int id);
	int GetTextIndexByKey(wstring key);
	FF9String& GetTextByKey(wstring key);
	void LocalizationRead(fstream& ffbin, uint32_t spaceused);
	int GetHWSDataSize(SteamLanguage lang = GetSteamLanguage());
	int GetDataSize(SteamLanguage lang = GetSteamLanguage());
	void UpdateOffset();
	
private:
	SpecialTextDataSet* parent;
	friend SpecialTextDataSet;
};

struct SpecialTextDataSet {
public:
	vector<SpecialTextDataStruct> text_block;
	bool modified;
	
	void Load(fstream& ffbin, ConfigurationSet& configset);
	void Write(fstream& ffbin, ConfigurationSet& configset);
	void WritePPF(fstream& ffbin, ConfigurationSet& configset);
	// return value is int[2] to be deleted
	// {Number of oversized blocks, Number of unknown data}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	void WriteSteamLocalizationPatch(fstream& fileout, fstream& baseresourcefile, ConfigurationSet& configset);
	void WriteSteamPatch(fstream& fileout, unsigned int blockid, wxArrayString basefile, SteamLanguage lang = GetSteamLanguage());
	void WriteSteamLocalization(fstream& fileout, bool alllang);
	void WriteSteam(fstream& fileout, unsigned int blockid, SteamLanguage lang = GetSteamLanguage());
	void WriteHWSSteam(fstream& fileout, unsigned int blockid, SteamLanguage lang = GetSteamLanguage());
	
	static int GetSpellNamingIndex();
};

#endif
