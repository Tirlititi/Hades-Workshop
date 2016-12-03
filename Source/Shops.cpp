#include "Shops.h"

#include "DllEditor.h"

#define SHOP_HWS_VERSION 2

const unsigned int steam_shop_field_size[] = { 16, 8, 8, 8, 8 };
const unsigned int steam_shop_field_array[] = { 0, 2, 0, 0, 0 };

#define MACRO_SHOP_IOFUNCTIONSHOP(IO,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<SHOP_AMOUNT;i++) { \
		if (READ) shop[i].parent = this; \
		if (READ) shop[i].item_amount = SHOP_ITEM_AMOUNT; \
		for (j=0;j<SHOP_ITEM_AMOUNT;j++) { \
			IO ## Char(ffbin,shop[i].item_list[j]); \
			if (READ && shop[i].item_list[j]==0xFF && shop[i].item_amount==SHOP_ITEM_AMOUNT) \
				shop[i].item_amount = j; \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SHOP_IOFUNCTIONSYNTH(IO,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<SYNTHESIS_AMOUNT;i++) { \
		IO ## Short(ffbin,synthesis[i].price); \
		IO ## Char(ffbin,synthesis[i].recipe1); \
		IO ## Char(ffbin,synthesis[i].recipe2); \
		IO ## Char(ffbin,synthesis[i].synthesized); \
		IO ## Char(ffbin,synthesis[i].shops); \
	} \
	if (PPF) PPFEndScanStep();


void ShopDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.shop_data_offset);
		MACRO_SHOP_IOFUNCTIONSHOP(FFIXRead,true,false)
		ffbin.seekg(config.synthesis_data_offset);
		MACRO_SHOP_IOFUNCTIONSYNTH(FFIXRead,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname;
		for (i=0;i<SHOP_AMOUNT;i++) {
			if (config.dll_shop_field_id[i]<0) {
				shop[i].item_list[0] = 0xFF;
				shop[i].item_amount = 0;
				steam_method_base_length_shop[i] = 0;
				j = 1;
			} else {
				dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_shop_field_id[i]));
				j = 0;
				do {
					SteamReadChar(dlldata.dll_file,shop[i].item_list[j]);
				} while (shop[i].item_list[j]!=0xFF && ++j<SHOP_ITEM_AMOUNT);
				shop[i].item_amount = j;
				if (j<SHOP_ITEM_AMOUNT)
					j++;
				steam_method_base_length_shop[i] = j+GetAlignOffset(j);
			}
			while (j<SHOP_ITEM_AMOUNT)
				shop[i].item_list[j++] = 0;
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_mix_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x1F, SYNTHESIS_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9MIX_DATA","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinst);
		steam_method_position_synthesis = dlldata.dll_file.tellg();
		uint8_t* rawsynthdata = dlldata.ConvertScriptToRaw_Object(SYNTHESIS_AMOUNT,5,steam_shop_field_size,steam_shop_field_array);
		steam_method_base_length_synthesis = (unsigned int)dlldata.dll_file.tellg()-steam_method_position_synthesis;
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		ffbin.write((const char*)rawsynthdata,6*SYNTHESIS_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_SHOP_IOFUNCTIONSYNTH(SteamRead,true,false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawsynthdata;
	}
}

DllMetaDataModification* ShopDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res;
	DllMetaData& dlldata = config.meta_dll;
	unsigned int fieldedshop = 0;
	unsigned int i,j,k;
	for (i=0;i<SHOP_AMOUNT;i++)
		if (config.dll_shop_field_id[i]>=0)
			fieldedshop++;
	res = new DllMetaDataModification[fieldedshop+1];
	j = 0;
	for (i=0;i<SHOP_AMOUNT;i++)
		if (config.dll_shop_field_id[i]>=0) {
			res[j].position = config.meta_dll.GetStaticFieldOffset(config.dll_shop_field_id[i]);
			res[j].base_length = steam_method_base_length_shop[i];
			res[j].new_length = SHOP_ITEM_AMOUNT;
			res[j].value = new uint8_t[res[j].new_length];
			for (k=0;k<SHOP_ITEM_AMOUNT;k++)
				res[j].value[k] = shop[i].item_list[k];
			j++;
		}
	uint32_t** argvalue = new uint32_t*[SYNTHESIS_AMOUNT];
	for (i=0;i<SYNTHESIS_AMOUNT;i++) {
		argvalue[i] = new uint32_t[5];
		argvalue[i][0] = synthesis[i].price;
		argvalue[i][1] = synthesis[i].recipe1;
		argvalue[i][2] = synthesis[i].recipe2;
		argvalue[i][3] = synthesis[i].synthesized;
		argvalue[i][4] = synthesis[i].shops;
	}
	res[fieldedshop] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position_synthesis,steam_method_base_length_synthesis,SYNTHESIS_AMOUNT,5,steam_shop_field_size,steam_shop_field_array);
	for (i=0;i<SYNTHESIS_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	*modifamount = fieldedshop+1;
	return res;
}

void ShopDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	ffbin.seekg(config.shop_data_offset);
	MACRO_SHOP_IOFUNCTIONSHOP(FFIXWrite,false,false)
	ffbin.seekg(config.synthesis_data_offset);
	MACRO_SHOP_IOFUNCTIONSYNTH(FFIXWrite,false,false)
}

void ShopDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	ffbin.seekg(config.shop_data_offset);
	MACRO_SHOP_IOFUNCTIONSHOP(PPFStepAdd,false,true)
	ffbin.seekg(config.synthesis_data_offset);
	MACRO_SHOP_IOFUNCTIONSYNTH(PPFStepAdd,false,true)
}

void ShopDataSet::LoadHWS(fstream& ffbin, bool versionflag) {
	unsigned int i,j;
	uint16_t version = 1;
	if (versionflag)
		HWSReadShort(ffbin,version);
	MACRO_SHOP_IOFUNCTIONSHOP(HWSRead,true,false)
	if (version>=2) {
		MACRO_SHOP_IOFUNCTIONSYNTH(HWSRead,true,false)
	}
}

void ShopDataSet::WriteHWS(fstream& ffbin) {
	unsigned int i,j;
	HWSWriteShort(ffbin,SHOP_HWS_VERSION);
	MACRO_SHOP_IOFUNCTIONSHOP(HWSWrite,false,false)
	MACRO_SHOP_IOFUNCTIONSYNTH(HWSWrite,false,false)
}
