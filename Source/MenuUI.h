#ifndef _MENUUI_H
#define _MENUUI_H

struct AtlasDataStruct;
struct MenuUIDataSet;

#include <inttypes.h>
#include <fstream>
#include <map>
#include <wx/bitmap.h>
#include "Texts.h"
#include "File_Manipulation.h"
#include "Configuration.h"
#include "UnityArchiver.h"
using namespace std;

struct AtlasDataStruct {
	map<wxString,wxBitmap> sprite;

	bool LoadAtlas(fstream& archive, UnityArchiveMetaData& meta, unsigned int img, unsigned int spt);
};

struct MenuUIDataSet {
public:
	SpecialTextDataSet* special_text;
	AtlasDataStruct steam_atlas[1];
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// return value is int[2] to be deleted
	// special_text: {Number of oversized blocks, Number of unknown data}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart* backup, bool usetext);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart* backup);
};

#endif
