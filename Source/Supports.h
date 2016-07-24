#ifndef _SUPPORTS_H
#define _SUPPORTS_H

#define SUPPORT_AMOUNT 64
struct SupportDataStruct;
struct SupportDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"
using namespace std;

struct SupportDataStruct {
public:
	FF9String name; // readonly
	FF9String help; // readonly
	uint16_t help_size_x;
	uint8_t category;
	uint8_t cost;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);
	int SetHelp(wstring newvalue);
	int SetHelp(FF9String& newvalue);

private:
	uint16_t name_offset;
	uint16_t help_offset;
	SupportDataSet* parent;
	friend SupportDataSet;
};

struct SupportDataSet {
public:
	SupportDataStruct support[SUPPORT_AMOUNT];
	uint16_t name_space_total;
	uint16_t help_space_total;
	uint16_t name_space_used;
	uint16_t help_space_used;
	
	uint32_t steam_method_position;
	uint32_t steam_method_base_length;
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return 0, 1, 2 or 3 for success, names too long, helps too long and both
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(fstream& ffbinbase, ConfigurationSet& config, unsigned int* modifamount);
	// texttype: 0 for name, 1 for help
	void WriteSteamText(fstream& ffbin, unsigned int texttype);
	void UpdateOffset();
};

#endif
