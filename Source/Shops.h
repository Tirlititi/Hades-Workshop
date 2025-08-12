#ifndef _SHOPS_H
#define _SHOPS_H

#define SHOP_AMOUNT 32
#define SHOP_ITEM_AMOUNT 32
#define SYNTHESIS_AMOUNT 64
#define SYNTHESIS_MAX_ITEMS 32
struct SynthesisDataStruct;
struct ShopDataStruct;
struct ShopDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
#include "DllEditor.h"
using namespace std;

struct SynthesisDataStruct {
public:
	int id;
	int price;
	vector<int> recipe;
	int synthesized;
	vector<int> shops;

	map<wxString, wxString> custom_field;

	uint8_t ShopsAsBitFlags();

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	ShopDataSet* parent;
	friend ShopDataSet;
};

struct ShopDataStruct {
public:
	int id;
	int item_amount;
	vector<int> item_list;

	map<wxString, wxString> custom_field;

	wxString GetFieldValue(wxString fieldname);
	bool CompareWithCSV(wxArrayString entries);

private:
	ShopDataSet* parent;
	friend ShopDataSet;
};

struct ShopDataSet {
public:
	vector<ShopDataStruct> shop;
	vector<SynthesisDataStruct> synthesis;
	
//	uint32_t steam_method_position_shop;
//	uint32_t steam_method_base_length_shop;
	uint32_t steam_method_base_length_shop[SHOP_AMOUNT];
	uint32_t steam_method_position_synthesis;
	uint32_t steam_method_base_length_synthesis;

	int GetShopIndexById(int shopid);
	ShopDataStruct& GetShopById(int shopid);

	wxString csv_header_shop;
	wxString csv_header_synthesis;
	wxString csv_format_shop;
	wxString csv_format_synthesis;
	map<wxString, wxString> custom_field_shop;
	map<wxString, wxString> custom_field_synthesis;
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	void LoadHWS(fstream& ffhws, bool versionflag);
	void WriteHWS(fstream& ffhws);
	// Return a modifamount-long pointer, to be deleted[]
	DllMetaDataModification* ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount);
	void GenerateCSharp(vector<string>& buffer);
	bool GenerateCSV(string basefolder);
};

#endif
