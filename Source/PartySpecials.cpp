#include "PartySpecials.h"

#include <wx/wx.h>
#include "CommonUtility.h"
#include "Database_CSV.h"

#define PARTY_SPECIAL_HWS_VERSION 3

#define MAGIC_SWORD_CSV_CHECK L"%id%;%supporter%;%beneficiary%;%requirement%;%unlocked%;%supporter_status_blocker%;%beneficiary_status_blocker%"
#define MAGIC_SWORD_CSV_DEFAULT MAGIC_SWORD_CSV_CHECK L";# %name%"

void MagicSwordDataStruct::InitDefault() {
	id = 0;
	supporter = 1;
	beneficiary = 3;
	requirement.resize(MAGIC_SWORD_AMOUNT);
	unlocked.resize(MAGIC_SWORD_AMOUNT);
	unlocked[0] = 0x32;		unlocked[1] = 0x34;		unlocked[2] = 0x36;
	unlocked[3] = 0x38;		unlocked[4] = 0x39;		unlocked[5] = 0x3B;
	unlocked[6] = 0x3D;		unlocked[7] = 0x3F;		unlocked[8] = 0x41;
	unlocked[9] = 0x4B;		unlocked[10] = 0x4C;
	unlocked[11] = 0xBD;	unlocked[12] = 0xBF;
	supporter_status_blocker = { 3, 10, 11, 17, 24, 28 };
	beneficiary_status_blocker = { 17, 28 };
}

wxString MagicSwordDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("Magic Sword %d"), id);
	if (fieldname.IsSameAs("supporter")) return wxString::Format(wxT("%d"), supporter);
	if (fieldname.IsSameAs("beneficiary")) return wxString::Format(wxT("%d"), beneficiary);
	if (fieldname.IsSameAs("requirement")) return wxString::Format(wxT("%s"), ConcatenateStrings<int>(", ", requirement, [](int spellid) { return "AA:" + to_string(spellid); }));
	if (fieldname.IsSameAs("unlocked")) return wxString::Format(wxT("%s"), ConcatenateStrings<int>(", ", unlocked, [](int spellid) { return "AA:" + to_string(spellid); }));
	if (fieldname.IsSameAs("supporter_status_blocker")) return FormatStatusSet(supporter_status_blocker);
	if (fieldname.IsSameAs("beneficiary_status_blocker")) return FormatStatusSet(beneficiary_status_blocker);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field.find(fieldname); search != parent->custom_field.end()) return search->second;
	return _(L"");
}

bool MagicSwordDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(MAGIC_SWORD_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].BeforeLast(L';'), rightcsv);
}

#define MACRO_MAGICSWORD_IOFUNCTION(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	if (useextendedtype) { \
		if (!READ) swdamount = mgswd.unlocked.size(); \
		IO ## FlexibleChar(ffbin, swdamount, useextendedtype); \
		if (READ) { \
			mgswd.unlocked.resize(swdamount); \
			mgswd.requirement.resize(swdamount); \
		} \
	} \
	for (i = 0; i < swdamount; i++) { \
		if (useextendedtype) IO ## FlexibleChar(ffbin, mgswd.unlocked[i], useextendedtype); \
		IO ## FlexibleChar(ffbin, mgswd.requirement[i], useextendedtype); \
	} \
	MACRO_IOFUNCTIONGENERIC_STATUS(ffbin, useextendedtype2, IO, READ, mgswd.supporter_status_blocker) \
	MACRO_IOFUNCTIONGENERIC_STATUS(ffbin, useextendedtype2, IO, READ, mgswd.beneficiary_status_blocker) \
	if (useextendedtype2) IO ## CSVFields(ffbin, mgswd.custom_field); \
	if (PPF) PPFEndScanStep();


void PartySpecialDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int swdamount = MAGIC_SWORD_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	int i;
	csv_header = _(HADES_STRING_CSV_MGCSWORD_HEADER);
	csv_format = _(MAGIC_SWORD_CSV_DEFAULT);
	magic_sword.resize(1);
	MagicSwordDataStruct& mgswd = magic_sword[0];
	mgswd.parent = this;
	mgswd.InitDefault();
	if (GetGameType() == GAME_TYPE_PSX) {
		ffbin.seekg(config.party_magicsword_offset);
		MACRO_MAGICSWORD_IOFUNCTION(FFIXRead, FFIXSeek, true, false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_magicsword_field_id));
		for (i = 0; i < MAGIC_SWORD_AMOUNT; i++) {
			SteamReadFlexibleChar(dlldata.dll_file, mgswd.requirement[i], false);
			dlldata.dll_file.seekg(3, ios::cur);
		}
	}
}

DllMetaDataModification* PartySpecialDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[1];
	DllMetaData& dlldata = config.meta_dll;
	unsigned int i;
	res[0].position = dlldata.GetStaticFieldOffset(config.dll_magicsword_field_id);
	res[0].base_length = 4 * MAGIC_SWORD_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_magicsword_field_id);
	res[0].new_length = 4 * MAGIC_SWORD_AMOUNT;
	res[0].value = new uint8_t[res[0].new_length];
	BufferInitPosition();
	for (i = 0; i < MAGIC_SWORD_AMOUNT; i++) {
		if (magic_sword.size() > 0 && i < magic_sword[0].requirement.size())
			BufferWriteLong(res[0].value, magic_sword[0].requirement[i]);
		else
			BufferWriteLong(res[0].value, 0);
	}
	*modifamount = 1;
	return res;
}

void PartySpecialDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i;
	stringstream mgcswddb;
	mgcswddb << "// Method: BattleHUD::SetAbilityMagic\n\n";
	mgcswddb << "\t// ...\n";
	mgcswddb << "\tint[] array2 = new int[] {\n";
	for (i = 0; i < MAGIC_SWORD_AMOUNT; i++) {
		if (magic_sword.size() > 0 && i < magic_sword[0].requirement.size())
			mgcswddb << "\t\t" << (int)magic_sword[0].requirement[i];
		else
			mgcswddb << "\t\t" << 0;
		mgcswddb << (i + 1 == MAGIC_SWORD_AMOUNT ? "" : ",") << "\n";
	}
	mgcswddb << "\t};\n";
	mgcswddb << "\t// ...\n";
	buffer.push_back(mgcswddb.str());
}

bool PartySpecialDataSet::GenerateCSV(string basefolder) {
	return MemoriaUtility::GenerateDatabaseGeneric<MagicSwordDataStruct>(_(basefolder), _(HADES_STRING_CSV_MGCSWORD_FILE), csv_header, _(L"\n"), _(L"\n"), magic_sword, csv_format, true);
}

void PartySpecialDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	if (magic_sword.size() == 0)
		return;
	int swdamount = MAGIC_SWORD_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	MagicSwordDataStruct& mgswd = magic_sword[0];
	int i;
	ffbin.seekg(config.party_magicsword_offset);
	MACRO_MAGICSWORD_IOFUNCTION(FFIXWrite, FFIXSeek, false, false)
}

void PartySpecialDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	if (magic_sword.size() == 0)
		return;
	int swdamount = MAGIC_SWORD_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	MagicSwordDataStruct& mgswd = magic_sword[0];
	int i;
	ffbin.seekg(config.party_magicsword_offset);
	MACRO_MAGICSWORD_IOFUNCTION(PPFStepAdd, FFIXSeek, false, true)
}

void PartySpecialDataSet::LoadHWS(fstream& ffbin) {
	int swdamount = MAGIC_SWORD_AMOUNT;
	int swdsetamount = 1;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	uint16_t version;
	int i;
	HWSReadShort(ffbin, version);
	if (version >= 3) {
		useextendedtype2 = true;
		HWSReadCSVFormatting(ffbin, csv_header, csv_format);
		HWSReadCSVFields(ffbin, custom_field);
	}
	if (version >= 2) {
		useextendedtype = true;
		HWSReadFlexibleChar(ffbin, swdsetamount, true);
		magic_sword.resize(swdsetamount);
		for (int seti = 0; seti < swdsetamount; seti++) {
			MagicSwordDataStruct& mgswd = magic_sword[seti];
			mgswd.parent = this;
			HWSReadFlexibleChar(ffbin, mgswd.supporter, true);
			HWSReadFlexibleChar(ffbin, mgswd.beneficiary, true);
			MACRO_MAGICSWORD_IOFUNCTION(HWSRead, HWSSeek, true, false)
		}
	} else {
		magic_sword.resize(1);
		MagicSwordDataStruct& mgswd = magic_sword[0];
		mgswd.parent = this;
		mgswd.InitDefault();
		MACRO_MAGICSWORD_IOFUNCTION(HWSRead, HWSSeek, true, false)
	}
}

void PartySpecialDataSet::WriteHWS(fstream& ffbin) {
	bool useextendedtype = true;
	bool useextendedtype2 = true;
	int swdsetamount = magic_sword.size();
	int i, seti;
	HWSWriteShort(ffbin, PARTY_SPECIAL_HWS_VERSION);
	HWSWriteCSVFormatting(ffbin, csv_header, csv_format);
	HWSWriteCSVFields(ffbin, custom_field);
	HWSWriteFlexibleChar(ffbin, swdsetamount, true);
	for (seti = 0; seti < swdsetamount; seti++) {
		MagicSwordDataStruct& mgswd = magic_sword[seti];
		int swdamount = mgswd.unlocked.size();
		HWSWriteFlexibleChar(ffbin, mgswd.supporter, true);
		HWSWriteFlexibleChar(ffbin, mgswd.beneficiary, true);
		MACRO_MAGICSWORD_IOFUNCTION(HWSWrite, HWSSeek, false, false)
	}
}
