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
public:
	int id;
	int supporter;
	int beneficiary;
	vector<int> requirement;
	vector<int> unlocked;
	set<int> supporter_status_blocker;
	set<int> beneficiary_status_blocker;

	map<wxString, wxString> custom_field;

	void InitDefault();

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

	PartySpecialDataSet* parent;
};

struct PartySpecialDataSet {
public:
	vector<MagicSwordDataStruct> magic_sword;

	wxString csv_header;
	wxString csv_format;
	map<wxString, wxString> custom_field;
	
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
