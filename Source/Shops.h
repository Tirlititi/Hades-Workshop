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
	uint16_t price;
	uint8_t recipe1;
	uint8_t recipe2;
	uint8_t synthesized;
	uint8_t shops;
};

struct ShopDataStruct {
public:
	// Quiet a simple struct ; list can be edited directly.
	// Just make sure either item_amount==SHOP_ITEM_AMOUNT
	// either item_list is 0xFF-terminated.
	uint8_t item_amount;
	uint8_t item_list[SHOP_ITEM_AMOUNT];
	
private:
	ShopDataSet* parent;
	friend ShopDataSet;
};

struct ShopDataSet {
public:
	ShopDataStruct shop[SHOP_AMOUNT];
	SynthesisDataStruct synthesis[SYNTHESIS_AMOUNT];
	
//	uint32_t steam_method_position_shop;
//	uint32_t steam_method_base_length_shop;
	uint32_t steam_method_base_length_shop[SHOP_AMOUNT];
	uint32_t steam_method_position_synthesis;
	uint32_t steam_method_base_length_synthesis;
	
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
