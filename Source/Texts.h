#ifndef _TEXTS_H
#define _TEXTS_H

struct TextFormatStruct;
struct TextDataStruct;
struct TextDataSet;

#define SPECIAL_TEXT_SPELL_NAMING_INDEX_PSX		6
#define SPECIAL_TEXT_SPELL_NAMING_INDEX_STEAM	2
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
	uint8_t* data;
};

struct TextDataStruct : public ChunkChild {
public:
	uint16_t amount;
	FF9String* text;
	// Followings are only used for PSX GameType
	uint16_t* offset;
	uint8_t* size_y;
	uint8_t* flag;
	// Followings are only used if has_format==true
	uint16_t* format_offset;
	uint16_t* size_x;
	uint16_t* format_amount;
	TextFormatStruct** format_data;
	bool has_format;
	
	wchar_t* extended_charmap;
	
	// Return 0 if success ; 1 if the value is too long
	int AddText(uint16_t id, FF9String& value);
	void RemoveText(uint16_t id);
	int SetText(uint16_t id, wstring& newvalue);
	int SetText(uint16_t id, FF9String& newvalue); // Copy the new value
	int AddFormatCode(uint16_t id, uint8_t opcode, uint8_t* arg); // arg must be malloc'ed and have the proper size depending of the opcode
	int RemoveFormatCode(uint16_t id, uint8_t formatid);
	void SetDialogBoxSize(uint16_t id, uint16_t x, uint16_t y, bool changey);
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void WriteSteam(fstream& f);
	void UpdateOffset();
};

struct TextDataSet {
public:
	uint16_t amount;
	uint16_t* cluster_id; // PSX only
	uint16_t* struct_id;
	uint16_t main_charmap_index; // PSX only
	wstring* name;
	uint16_t* tim_amount;
	TextDataStruct** text_data;
	CharmapDataStruct** charmap; // PSX only
	TIMImageDataStruct** chartim; // PSX only
	
	void Load(fstream& ffbin, ClusterSet& clusset);
	void Write(fstream& ffbin, ClusterSet& clusset);
	void WritePPF(fstream& ffbin, ClusterSet& clusset);
	// return value is int[3] to be deleted
	// {Number of oversized blocks, Number of unknown data, Number of unused blocks}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
};


struct SpecialTextDataStruct {
public:
	uint32_t amount;
	FF9String* text;
	uint16_t* offset; // PSX only
	uint16_t* size_x; // PSX only
	uint32_t space_total; // 4 bytes of "amount" not counted (PSX)
	uint32_t space_used; // Warning: for PSX, space is 16-bits long but for Steam, it's 32-bits long
	bool localizationfile; // Steam only
	
	// Return 0 if success ; 1 if the value is too long
	int AddText(uint16_t id, FF9String& value);
	void RemoveText(uint16_t id);
	int SetText(uint16_t id, wstring& newvalue);
	int SetText(uint16_t id, FF9String& newvalue); // Copy the new value
	void UpdateOffset();
	
private:
	SpecialTextDataSet* parent;
	friend SpecialTextDataSet;
};

struct SpecialTextDataSet {
public:
	unsigned int amount;
	SpecialTextDataStruct* text_block;
	bool modified;
	
	void Load(fstream& ffbin, ConfigurationSet& configset);
	void Write(fstream& ffbin, ConfigurationSet& configset);
	void WritePPF(fstream& ffbin, ConfigurationSet& configset);
	// return value is int[2] to be deleted
	// {Number of oversized blocks, Number of unknown data}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart& backup);
	void WriteSteam(fstream& ffbin, unsigned int blockid);
	
	static int GetSpellNamingIndex();
};

#endif
