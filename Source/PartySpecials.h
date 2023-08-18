#ifndef _PARTY_SPECIALS_H
#define _PARTY_SPECIALS_H

#define MAGIC_SWORD_AMOUNT	13
struct MagicSwordDataStruct;
struct PartySpecialDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"
using namespace std;

struct MagicSwordDataStruct {
	int id;
	int supporter;
	int beneficiary;
	vector<int> requirement;
	vector<int> unlocked;

	void InitDefault();
};

struct PartySpecialDataSet {
public:
	vector<MagicSwordDataStruct> magic_sword;
	
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
