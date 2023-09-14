#include "Shops.h"

#include <wx/wx.h>
#include "DllEditor.h"
#include "Database_Item.h"
#include "Database_CSV.h"
#include "CommonUtility.h"

#define SHOP_HWS_VERSION 3

const unsigned int steam_shop_field_size[] = { 16, 8, 8, 8, 8 };
const unsigned int steam_shop_field_array[] = { 0, 2, 0, 0, 0 };

#define MACRO_SHOP_IOFUNCTIONSHOP(IO,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < shopamount; i++) { \
		if (useextendedtype) { \
			IO ## FlexibleChar(ffbin, shop[i].item_amount, useextendedtype); \
			if (READ) shop[i].item_list.resize(max(SHOP_ITEM_AMOUNT, shop[i].item_amount)); \
			for (j = 0; j < shop[i].item_amount; j++) \
				IO ## FlexibleChar(ffbin, shop[i].item_list[j], useextendedtype); \
		} else { \
			if (READ) { \
				shop[i].item_amount = SHOP_ITEM_AMOUNT; \
				shop[i].item_list.resize(SHOP_ITEM_AMOUNT); \
			} \
			for (j = 0; j < SHOP_ITEM_AMOUNT; j++) { \
				IO ## FlexibleChar(ffbin, shop[i].item_list[j], useextendedtype); \
				if (READ && shop[i].item_list[j] == 0xFF && shop[i].item_amount == SHOP_ITEM_AMOUNT) \
					shop[i].item_amount = j; \
			} \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SHOP_IOFUNCTIONSYNTH(IO,READ,PPF) \
	uint8_t shopbits; \
	int vectorsize; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < synthamount; i++) { \
		IO ## FlexibleShort(ffbin, synthesis[i].price, useextendedtype); \
		if (useextendedtype) { \
			if (!READ) vectorsize = synthesis[i].recipe.size(); \
			IO ## FlexibleChar(ffbin, vectorsize, true); \
			if (READ) synthesis[i].recipe.resize(vectorsize); \
			for (j = 0; j < vectorsize; j++) \
				IO ## FlexibleChar(ffbin, synthesis[i].recipe[j], true); \
			IO ## FlexibleChar(ffbin, synthesis[i].synthesized, true); \
			if (!READ) vectorsize = synthesis[i].shops.size(); \
			IO ## FlexibleChar(ffbin, vectorsize, true); \
			if (READ) synthesis[i].shops.resize(vectorsize); \
			for (j = 0; j < vectorsize; j++) \
				IO ## FlexibleChar(ffbin, synthesis[i].shops[j], true); \
		} else { \
			if (READ) \
				synthesis[i].recipe.resize(2); \
			else \
				shopbits = synthesis[i].ShopsAsBitFlags(); \
			IO ## FlexibleChar(ffbin, synthesis[i].recipe[0], false); \
			IO ## FlexibleChar(ffbin, synthesis[i].recipe[1], false); \
			IO ## FlexibleChar(ffbin, synthesis[i].synthesized, false); \
			IO ## Char(ffbin, shopbits); \
			if (READ) { \
				synthesis[i].shops.clear(); \
				for (j = 0; j < 8; j++) \
					if (shopbits & (1 << j)) \
						synthesis[i].shops.push_back(32 + j); \
			} \
		} \
	} \
	if (PPF) PPFEndScanStep();


void ShopDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int shopamount = SHOP_AMOUNT;
	int synthamount = SYNTHESIS_AMOUNT;
	bool useextendedtype = false;
	int i, j;
	shop.resize(SHOP_AMOUNT);
	synthesis.resize(SYNTHESIS_AMOUNT);
	for (i = 0; i < SHOP_AMOUNT; i++) {
		shop[i].id = i;
		shop[i].item_list.resize(SHOP_ITEM_AMOUNT);
	}
	for (i = 0; i < SYNTHESIS_AMOUNT; i++) {
		synthesis[i].id = i;
		synthesis[i].recipe.resize(2);
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		ffbin.seekg(config.shop_data_offset);
		MACRO_SHOP_IOFUNCTIONSHOP(FFIXRead, true, false)
		ffbin.seekg(config.synthesis_data_offset);
		MACRO_SHOP_IOFUNCTIONSYNTH(FFIXRead, true, false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname;
		for (i = 0; i < SHOP_AMOUNT; i++) {
			if (config.dll_shop_field_id[i] < 0) {
				shop[i].item_list[0] = 0xFF;
				shop[i].item_amount = 0;
				steam_method_base_length_shop[i] = 0;
				j = 1;
			} else {
				dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_shop_field_id[i]));
				j = 0;
				do {
					SteamReadFlexibleChar(dlldata.dll_file, shop[i].item_list[j], false);
				} while (shop[i].item_list[j] != 0xFF && ++j < SHOP_ITEM_AMOUNT);
				shop[i].item_amount = j;
				if (j < SHOP_ITEM_AMOUNT)
					j++;
			}
			while (j < SHOP_ITEM_AMOUNT)
				shop[i].item_list[j++] = 0;
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_mix_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x1F, SYNTHESIS_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9MIX_DATA","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 3, initinst);
		steam_method_position_synthesis = dlldata.dll_file.tellg();
		uint8_t* rawsynthdata = dlldata.ConvertScriptToRaw_Object(SYNTHESIS_AMOUNT, 5, steam_shop_field_size, steam_shop_field_array);
		steam_method_base_length_synthesis = (unsigned int)dlldata.dll_file.tellg() - steam_method_position_synthesis;
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(), ios::out | ios::binary);
		ffbin.write((const char*)rawsynthdata, 6 * SYNTHESIS_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		MACRO_SHOP_IOFUNCTIONSYNTH(SteamRead, true, false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawsynthdata;
	}
}

DllMetaDataModification* ShopDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res;
	DllMetaData& dlldata = config.meta_dll;
	unsigned int fieldedshop = 0;
	unsigned int i, j, k;
	for (i = 0; i < SHOP_AMOUNT; i++)
		if (config.dll_shop_field_id[i] >= 0)
			fieldedshop++;
	res = new DllMetaDataModification[fieldedshop + 1];
	j = 0;
	for (i = 0; i < SHOP_AMOUNT; i++)
		if (config.dll_shop_field_id[i] >= 0) {
			res[j].position = config.meta_dll.GetStaticFieldOffset(config.dll_shop_field_id[i]);
			res[j].base_length = steam_method_base_length_shop[i];
			res[j].new_length = SHOP_ITEM_AMOUNT;
			res[j].value = new uint8_t[res[j].new_length];
			for (k = 0; k < SHOP_ITEM_AMOUNT; k++)
				res[j].value[k] = shop[i].item_list[k];
			j++;
		}
	uint32_t** argvalue = new uint32_t*[SYNTHESIS_AMOUNT];
	for (i = 0; i < SYNTHESIS_AMOUNT; i++) {
		argvalue[i] = new uint32_t[5];
		argvalue[i][0] = synthesis[i].price;
		argvalue[i][1] = synthesis[i].recipe.size() >= 1 ? synthesis[i].recipe[0] : 0xFF;
		argvalue[i][2] = synthesis[i].recipe.size() >= 2 ? synthesis[i].recipe[1] : 0xFF;
		argvalue[i][3] = synthesis[i].synthesized;
		argvalue[i][4] = synthesis[i].ShopsAsBitFlags();
	}
	res[fieldedshop] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position_synthesis, steam_method_base_length_synthesis, SYNTHESIS_AMOUNT, 5, steam_shop_field_size, steam_shop_field_array);
	for (i = 0; i < SYNTHESIS_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	*modifamount = fieldedshop + 1;
	return res;
}

void ShopDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i, j;
	stringstream shopdb;
	shopdb << "// Method: FF9.ff9buy::.cctor\n\n";
	shopdb << "\tff9buy._FF9Buy_Data = new byte[][] {\n";
	for (i = 0; i < SHOP_AMOUNT; i++) {
		shopdb << "\t\tnew byte[] { ";
		for (j = 0; j < SHOP_ITEM_AMOUNT && j < shop[i].item_list.size() && shop[i].item_list[j] != 0xFF; j++)
			shopdb << (int)shop[i].item_list[j] << (j + 1 == SHOP_ITEM_AMOUNT ? " " : ", ");
		if (j < SHOP_ITEM_AMOUNT)
			shopdb << "byte.MaxValue ";
		shopdb << (i + 1 == SHOP_AMOUNT ? "}" : "},") << " // " << HADES_STRING_SHOP_NAME[i].label.ToStdString() << "\n";
	}
	shopdb << "\t};\n";
	buffer.push_back(shopdb.str());
	stringstream synthdb;
	synthdb << "// Method: FF9.ff9mix::.cctor\n\n";
	synthdb << "\tff9mix._FF9MIX_DATA = new FF9MIX_DATA[] {\n";
	for (i = 0; i < SYNTHESIS_AMOUNT; i++)
		synthdb << "\t\tnew FF9MIX_DATA(" << (int)synthesis[i].price << ", new byte[]{ " << (synthesis[i].recipe.size() >= 1 ? synthesis[i].recipe[0] : 0xFF) << ", " << (synthesis[i].recipe.size() >= 2 ? synthesis[i].recipe[1] : 0xFF) << " }, " << (int)synthesis[i].synthesized << ", " << StreamAsHex(synthesis[i].ShopsAsBitFlags()) << (i + 1 == SYNTHESIS_AMOUNT ? ")\n" : "),\n");
	synthdb << "\t};\n";
	buffer.push_back(synthdb.str());
}

wxString CSV_ShopConstructor(ShopDataStruct& sh, int index) {
	wxString shopname = sh.id < G_V_ELEMENTS(HADES_STRING_SHOP_NAME) ? HADES_STRING_SHOP_NAME[sh.id].label : (_("Custom shop ") + to_string(sh.id));
	vector<int> short_item_list(sh.item_amount);
	for (int i = 0; i < sh.item_amount; i++)
		short_item_list[i] = sh.item_list[i];
	return wxString::Format(wxT("%s;%d;%s;# %s"),
		shopname,
		sh.id,
		ConcatenateStrings<int>(", ", short_item_list, [](int itid) {
			return itid == 0xFF ? "" : to_string(itid);
		}, true),
		shopname);
}

wxString CSV_SynthesisConstructor(SynthesisDataStruct& sy, int index) {
	wxString entry;
	if (GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_ITEM])
		entry = _(GetGameSaveSet()->itemset->GetItemById(sy.synthesized).name.str_nice) + _(L";");
	else
		entry = wxString::Format(wxT("Synthesis %d;"), sy.id);
	return entry + wxString::Format(wxT("%d;%s;%d;%d;%s"),
		sy.id,
		ConcatenateStrings<int>(", ", sy.shops, static_cast<string(*)(int)>(&to_string)),
		sy.price,
		sy.synthesized,
		ConcatenateStrings<int>(", ", sy.recipe, static_cast<string(*)(int)>(&to_string)));
}

bool ShopDataSet::GenerateCSV(string basefolder) {
	if (!MemoriaUtility::GenerateCSVGeneric<ShopDataStruct>(_(basefolder), _(HADES_STRING_CSV_SHOP_FILE), _(HADES_STRING_CSV_SHOP_HEADER), shop, &CSV_ShopConstructor, &MemoriaUtility::CSV_ComparerWithoutBoth, true))
		return false;
	if (!MemoriaUtility::GenerateCSVGeneric<SynthesisDataStruct>(_(basefolder), _(HADES_STRING_CSV_SYNTHESIS_FILE), _(HADES_STRING_CSV_SYNTHESIS_HEADER), synthesis, &CSV_SynthesisConstructor, &MemoriaUtility::CSV_ComparerWithoutStart, true))
		return false;
	return true;
}

void ShopDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	int shopamount = SHOP_AMOUNT;
	int synthamount = SYNTHESIS_AMOUNT;
	bool useextendedtype = false;
	int i, j;
	ffbin.seekg(config.shop_data_offset);
	MACRO_SHOP_IOFUNCTIONSHOP(FFIXWrite,false,false)
	ffbin.seekg(config.synthesis_data_offset);
	MACRO_SHOP_IOFUNCTIONSYNTH(FFIXWrite,false,false)
}

void ShopDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	int shopamount = SHOP_AMOUNT;
	int synthamount = SYNTHESIS_AMOUNT;
	bool useextendedtype = false;
	int i, j;
	ffbin.seekg(config.shop_data_offset);
	MACRO_SHOP_IOFUNCTIONSHOP(PPFStepAdd,false,true)
	ffbin.seekg(config.synthesis_data_offset);
	MACRO_SHOP_IOFUNCTIONSYNTH(PPFStepAdd,false,true)
}

void ShopDataSet::LoadHWS(fstream& ffbin, bool versionflag) {
	int shopamount = SHOP_AMOUNT;
	int synthamount = SYNTHESIS_AMOUNT;
	bool useextendedtype = false;
	vector<ShopDataStruct> nonmodifiedshop;
	vector<SynthesisDataStruct> nonmodifiedsynth;
	int i, j;
	uint16_t version = 1;
	if (versionflag)
		HWSReadShort(ffbin, version);
	if (version >= 3) {
		useextendedtype = true;
		vector<int> added;
		shopamount = PrepareHWSFlexibleList(ffbin, shop, nonmodifiedshop, added);
		synthamount = PrepareHWSFlexibleList(ffbin, synthesis, nonmodifiedsynth, added);
	}
	MACRO_SHOP_IOFUNCTIONSHOP(HWSRead, true, false)
	if (version >= 2) {
		MACRO_SHOP_IOFUNCTIONSYNTH(HWSRead, true, false)
	}
	for (i = 0; i < (int)nonmodifiedshop.size(); i++)
		InsertAtId(shop, nonmodifiedshop[i], nonmodifiedshop[i].id);
	for (i = 0; i < (int)nonmodifiedsynth.size(); i++)
		InsertAtId(synthesis, nonmodifiedsynth[i], nonmodifiedsynth[i].id);
}

void ShopDataSet::WriteHWS(fstream& ffbin) {
	int shopamount = shop.size();
	int synthamount = synthesis.size();
	bool useextendedtype = true;
	int i, j;
	HWSWriteShort(ffbin,SHOP_HWS_VERSION);
	HWSWriteFlexibleChar(ffbin, shopamount, true);
	for (i = 0; i < shopamount; i++)
		HWSWriteFlexibleChar(ffbin, shop[i].id, true);
	HWSWriteFlexibleChar(ffbin, synthamount, true);
	for (i = 0; i < synthamount; i++)
		HWSWriteFlexibleChar(ffbin, synthesis[i].id, true);
	MACRO_SHOP_IOFUNCTIONSHOP(HWSWrite,false,false)
	MACRO_SHOP_IOFUNCTIONSYNTH(HWSWrite,false,false)
}

int ShopDataSet::GetShopIndexById(int shopid) {
	if (shopid < SHOP_AMOUNT)
		return shopid;
	for (unsigned int i = SHOP_AMOUNT; i < shop.size(); i++)
		if (shop[i].id == shopid)
			return i;
	return -1;
}

ShopDataStruct dummyshop;
ShopDataStruct& ShopDataSet::GetShopById(int shopid) {
	int index = GetShopIndexById(shopid);
	if (index >= 0)
		return shop[index];
	dummyshop.id = -1;
	return dummyshop;
}

uint8_t SynthesisDataStruct::ShopsAsBitFlags() {
	uint8_t result = 0;
	for (unsigned int i = 0; i < shops.size(); i++)
		if (shops[i] >= 32 && shops[i] < 40)
			result |= 1 << (shops[i] - 32);
	return result;
}
