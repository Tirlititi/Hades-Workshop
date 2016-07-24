#ifndef _MENUUI_H
#define _MENUUI_H

struct MenuUIDataSet;

#include <inttypes.h>
#include <fstream>
#include "Texts.h"
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct MenuUIDataSet {
public:
	SpecialTextDataSet* special_text;
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// return value is int[2] to be deleted
	// special_text: {Number of oversized blocks, Number of unknown data}
	int* LoadHWS(fstream& ffhws, UnusedSaveBackupPart* backup, bool usetext);
	void WriteHWS(fstream& ffhws, UnusedSaveBackupPart* backup);
};

#endif
