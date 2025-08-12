#include "Shops.h"

#include <wx/wx.h>
#include "DllEditor.h"
#include "Database_Item.h"
#include "Database_CSV.h"
#include "CommonUtility.h"
#include "main.h"

#define SHOP_HWS_VERSION 4

#define SHOP_CSV_CHECK L"%id%;%item_list%"
#define SHOP_CSV_DEFAULT L"Shop %id%;" SHOP_CSV_CHECK L";# Shop %id% - %name%"
#define SYNTH_CSV_CHECK L"%id%;%shop_list%;%price%;%synthesized%;%recipe_list%"
#define SYNTH_CSV_DEFAULT L"%synthesized_name%;" SYNTH_CSV_CHECK

const unsigned int steam_shop_field_size[] = { 16, 8, 8, 8, 8 };
const unsigned int steam_shop_field_array[] = { 0, 2, 0, 0, 0 };

wxString ShopDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return id < G_V_ELEMENTS(HADES_STRING_SHOP_NAME) ? HADES_STRING_SHOP_NAME[id].label : (_("Custom shop ") + to_string(id));
	if (fieldname.IsSameAs("item_list")) {
		wxString result = wxEmptyString;
		bool addcomma = false;
		for (unsigned int i = 0; i < item_list.size(); i++) {
			if (item_list[i] == 0xFF)
				break;
			if (hades::PREFER_EXPORT_AS_PATCHES && item_list[i] >= 256)
				continue;
			result += (addcomma ? _(L", ") : _(L"")) + to_string(item_list[i]);
			addcomma = true;
		}
		return result;
	}
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_shop.find(fieldname); search != parent->custom_field_shop.end()) return search->second;
	return _(L"");
}

bool ShopDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(SHOP_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';').BeforeLast(L';'), rightcsv);
}

wxString SynthesisDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("synthesized_name")) {
		if (GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_ITEM])
			return _(GetGameSaveSet()->itemset->GetItemById(synthesized).name.str_nice);
		else
			return wxString::Format(wxT("Synthesis %d"), id);
	}
	if (fieldname.IsSameAs("price")) return wxString::Format(wxT("%d"), price);
	if (fieldname.IsSameAs("synthesized")) return wxString::Format(wxT("%d"), synthesized);
	if (fieldname.IsSameAs("shop_list")) return wxString::Format(wxT("%s"), ConcatenateStrings<int>(", ", shops, static_cast<string(*)(int)>(&to_string), true));
	if (fieldname.IsSameAs("recipe_list")) return wxString::Format(wxT("%s"), ConcatenateStrings<int>(", ", recipe, static_cast<string(*)(int)>(&to_string), true));
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_synthesis.find(fieldname); search != parent->custom_field_synthesis.end()) return search->second;
	return _(L"");
}

bool SynthesisDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(SYNTH_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';'), rightcsv);
}

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
		if (useextendedtype2) IO ## CSVFields(ffbin, shop[i].custom_field); \
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
		if (useextendedtype2) IO ## CSVFields(ffbin, synthesis[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();


void ShopDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int shopamount = SHOP_AMOUNT;
	int synthamount = SYNTHESIS_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i, j;
	csv_header_shop = _(HADES_STRING_CSV_SHOP_HEADER);
	csv_header_synthesis = _(HADES_STRING_CSV_SYNTHESIS_HEADER);
	csv_format_shop = _(SHOP_CSV_DEFAULT);
	csv_format_synthesis = _(SYNTH_CSV_DEFAULT);
	shop.resize(SHOP_AMOUNT);
	synthesis.resize(SYNTHESIS_AMOUNT);
	for (i = 0; i < SHOP_AMOUNT; i++) {
		shop[i].id = i;
		shop[i].parent = this;
		shop[i].item_list.resize(SHOP_ITEM_AMOUNT);
	}
	for (i = 0; i < SYNTHESIS_AMOUNT; i++) {
		synthesis[i].id = i;
		synthesis[i].parent = this;
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

bool ShopDataSet::GenerateCSV(string basefolder) {
	if (!MemoriaUtility::GenerateDatabaseGeneric<ShopDataStruct>(_(basefolder), _(HADES_STRING_CSV_SHOP_FILE), csv_header_shop, _(L"\n"), _(L"\n"), shop, csv_format_shop, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<SynthesisDataStruct>(_(basefolder), _(HADES_STRING_CSV_SYNTHESIS_FILE), csv_header_synthesis, _(L"\n"), _(L"\n"), synthesis, csv_format_synthesis, true))
		return false;
	if (hades::PREFER_EXPORT_AS_PATCHES && GetGameSaveSet() != NULL && GetGameSaveSet()->sectionloaded[DATA_SECTION_ITEM]) {
		ItemDataSet* itemset = GetGameSaveSet()->itemset;
		wxString shoppatchstr = _(L"");
		set<int> releventitem;
		unsigned int i, j;
		for (i = 0; i < shop.size(); i++)
			for (j = 0; j < shop[i].item_list.size(); j++)
				if (shop[i].item_list[j] >= 256)
					releventitem.insert(shop[i].item_list[j]);
		for (auto it = releventitem.begin(); it != releventitem.end(); it++) {
			ItemDataStruct& item = itemset->GetItemById(*it);
			shoppatchstr += wxString::Format(wxT("// %s\n%d Add"), item.name.str, item.id);
			for (i = 0; i < shop.size(); i++)
				for (j = 0; j < shop[i].item_list.size(); j++)
					if (shop[i].item_list[j] == *it)
						shoppatchstr += wxString::Format(wxT(" %d"), shop[i].id);
			shoppatchstr += _(L"\n\n");
		}
		if (!shoppatchstr.IsEmpty()) {
			wxFile shoptxtfile;
			if (!shoptxtfile.Open(_(basefolder) + _(HADES_STRING_SHOP_PATCH_FILE), wxFile::write))
				return false;
			if (!shoptxtfile.Write(shoppatchstr))
				return false;
			shoptxtfile.Close();
		}
	}
	return true;
}

void ShopDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	int shopamount = SHOP_AMOUNT;
	int synthamount = SYNTHESIS_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
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
	bool useextendedtype2 = false;
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
	bool useextendedtype2 = false;
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
		for (i = 0; i < (int)added.size(); i++)
			shop[added[i]].parent = this;
		synthamount = PrepareHWSFlexibleList(ffbin, synthesis, nonmodifiedsynth, added);
		for (i = 0; i < (int)added.size(); i++)
			synthesis[added[i]].parent = this;
	}
	if (version >= 4) {
		useextendedtype2 = true;
		HWSReadCSVFormatting(ffbin, csv_header_shop, csv_format_shop);
		HWSReadCSVFields(ffbin, custom_field_shop);
		HWSReadCSVFormatting(ffbin, csv_header_synthesis, csv_format_synthesis);
		HWSReadCSVFields(ffbin, custom_field_synthesis);
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
	bool useextendedtype2 = true;
	int i, j;
	HWSWriteShort(ffbin,SHOP_HWS_VERSION);
	HWSWriteFlexibleChar(ffbin, shopamount, true);
	for (i = 0; i < shopamount; i++)
		HWSWriteFlexibleChar(ffbin, shop[i].id, true);
	HWSWriteFlexibleChar(ffbin, synthamount, true);
	for (i = 0; i < synthamount; i++)
		HWSWriteFlexibleChar(ffbin, synthesis[i].id, true);
	HWSWriteCSVFormatting(ffbin, csv_header_shop, csv_format_shop);
	HWSWriteCSVFields(ffbin, custom_field_shop);
	HWSWriteCSVFormatting(ffbin, csv_header_synthesis, csv_format_synthesis);
	HWSWriteCSVFields(ffbin, custom_field_synthesis);
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
	dummyshop.parent = this;
	return dummyshop;
}

uint8_t SynthesisDataStruct::ShopsAsBitFlags() {
	uint8_t result = 0;
	for (unsigned int i = 0; i < shops.size(); i++)
		if (shops[i] >= 32 && shops[i] < 40)
			result |= 1 << (shops[i] - 32);
	return result;
}
