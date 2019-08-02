#ifndef _PARTY_SPECIALS_H
#define _PARTY_SPECIALS_H

#define MAGIC_SWORD_AMOUNT	13
struct PartySpecialDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"
using namespace std;

struct PartySpecialDataSet {
public:
	uint8_t magic_sword[MAGIC_SWORD_AMOUNT];
	uint8_t magic_sword_requirement[MAGIC_SWORD_AMOUNT];
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	void LoadHWS(fstream& ffhws);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	void GenerateCSharp(vector<string>& buffer);
	bool GenerateCSV(string basefolder);
};

#endif
