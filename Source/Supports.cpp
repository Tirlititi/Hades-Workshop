#include "Supports.h"

#include "main.h"
#include "DllEditor.h"
#include "Database_CSV.h"
#include "CommonUtility.h"

#define SUPPORT_HWS_VERSION 3

#define SUPPORT_CSV_CHECK L"%id%;%cost%;%boosted%"
#define SUPPORT_CSV_DEFAULT L"%name%;" SUPPORT_CSV_CHECK

const unsigned int steam_support_field_size[] = { 8, 8, 16, 16, 16 };

int SupportDataStruct::SetName(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(name);
		tmp.SetValue(newvalue);
		int oldlen = name.length;
		int newlen = tmp.length;
		if (parent->name_space_used + newlen > parent->name_space_total + oldlen)
			return 1;
		parent->name_space_used += newlen - oldlen;
	}
	name.SetValue(newvalue);
	return 0;
}

int SupportDataStruct::SetName(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = name.length;
		int newlen = newvalue.length;
		if (parent->name_space_used + newlen > parent->name_space_total + oldlen)
			return 1;
		parent->name_space_used += newlen - oldlen;
	}
	name = newvalue;
	return 0;
}

int SupportDataStruct::SetHelp(wstring newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(help);
		tmp.SetValue(newvalue);
		int oldlen = help.length;
		int newlen = tmp.length;
		if (parent->help_space_used + newlen > parent->help_space_total + oldlen)
			return 1;
		parent->help_space_used += newlen - oldlen;
	}
	help.SetValue(newvalue);
	return 0;
}

int SupportDataStruct::SetHelp(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX) {
		int oldlen = help.length;
		int newlen = newvalue.length;
		if (parent->help_space_used + newlen > parent->help_space_total + oldlen)
			return 1;
		parent->help_space_used += newlen - oldlen;
	}
	help = newvalue;
	return 0;
}

wxString SupportDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("%s"), name.str_nice);
	if (fieldname.IsSameAs("help")) return wxString::Format(wxT("%s"), help.str_nice);
	if (fieldname.IsSameAs("category")) return wxString::Format(wxT("%d"), category);
	if (fieldname.IsSameAs("cost")) return wxString::Format(wxT("%d"), cost);
	if (fieldname.IsSameAs("boosted")) wxString::Format(wxT("%s"), ConcatenateStrings<int>(", ", boosted_support, static_cast<string(*)(int)>(&to_string)));
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field.find(fieldname); search != parent->custom_field.end()) return search->second;
	return _(L"");
}

bool SupportDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(SUPPORT_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';'), rightcsv);
}

#define MACRO_SUPPORT_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i = 0; i < supportamount; i++) { \
		IO ## Char(ffbin, support[i].category); \
		IO ## FlexibleChar(ffbin, support[i].cost, useextendedtype2); \
		IO ## Short(ffbin, support[i].name_offset); \
		IO ## Short(ffbin, support[i].help_offset); \
		IO ## Short(ffbin, support[i].help_size_x); \
		if (useextendedtype2) IO ## CSVFields(ffbin, support[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SUPPORT_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<supportamount;i++) { \
		SEEK(ffbin,txtpos,support[i].name_offset); \
		IO ## FF9String(ffbin,support[i].name); \
		if (READ) name_space_used += support[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,name_space_total);

#define MACRO_SUPPORT_IOFUNCTIONHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) help_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,help_space_total); \
	for (i=0;i<supportamount;i++) { \
		SEEK(ffbin,txtpos,support[i].help_offset); \
		IO ## FF9String(ffbin,support[i].help); \
		if (READ) help_space_used += support[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total);


void SupportDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int supportamount = SUPPORT_AMOUNT;
	bool useextendedtype2 = false;
	int i;
	csv_header = _(HADES_STRING_CSV_SUPPORT_HEADER);
	csv_format = _(SUPPORT_CSV_DEFAULT);
	name_space_total = config.support_name_space_total;
	help_space_total = config.support_help_space_total;
	support.resize(SUPPORT_AMOUNT);
	for (i = 0; i < SUPPORT_AMOUNT; i++) {
		support[i].parent = this;
		support[i].id = i;
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		uint32_t txtpos;
		ffbin.seekg(config.support_data_offset);
		MACRO_SUPPORT_IOFUNCTIONDATA(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.support_text_offset[0]);
		MACRO_SUPPORT_IOFUNCTIONNAME(FFIXRead, FFIXSeek, true, false)
		MACRO_SUPPORT_IOFUNCTIONHELP(FFIXRead, FFIXSeek, true, false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.support_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.support_name_file[lang]);
			for (i = 0; i < SUPPORT_AMOUNT; i++)
				SteamReadFF9String(ffbin, support[i].name, lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.support_help_file[lang]));
			help_space_used = config.meta_res.GetFileSizeByIndex(config.support_help_file[lang]);
			for (i = 0; i < SUPPORT_AMOUNT; i++)
				SteamReadFF9String(ffbin, support[i].help, lang);
		}
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_ability_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x1F, SUPPORT_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("SA_DATA","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 3, initinst);
		steam_method_position = dlldata.dll_file.tellg();
		uint8_t* rawsupportdata = dlldata.ConvertScriptToRaw_Object(SUPPORT_AMOUNT, 5, steam_support_field_size);
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg() - steam_method_position;
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(), ios::out | ios::binary);
		ffbin.write((const char*)rawsupportdata, 8 * SUPPORT_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		MACRO_SUPPORT_IOFUNCTIONDATA(SteamRead, SteamSeek, true, false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawsupportdata;
	}
}

DllMetaDataModification* SupportDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[1];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t*[SUPPORT_AMOUNT];
	unsigned int i;
	for (i = 0; i < SUPPORT_AMOUNT; i++) {
		argvalue[i] = new uint32_t[5];
		argvalue[i][0] = support[i].category;
		argvalue[i][1] = support[i].cost;
		argvalue[i][2] = support[i].name_offset;
		argvalue[i][3] = support[i].help_offset;
		argvalue[i][4] = support[i].help_size_x;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position, steam_method_base_length, SUPPORT_AMOUNT, 5, steam_support_field_size);
	for (i = 0; i < SUPPORT_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	*modifamount = 1;
	return res;
}

void SupportDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i;
	stringstream supportdb;
	supportdb << "// Method: FF9.ff9abil::.cctor\n\n";
	supportdb << "\tff9abil._FF9Abil_SaData = new SA_DATA[] {\n";
	for (i = 0; i < SUPPORT_AMOUNT; i++)
		supportdb << "\t\tnew SA_DATA(" << (int)support[i].category << ", " << (int)support[i].cost << ", " << (int)support[i].name_offset << ", " << (int)support[i].help_offset << ", " << (int)support[i].help_size_x << (i+1==SUPPORT_AMOUNT ? ")" : "),") << " // " << ConvertWStrToStr(support[i].name.str_nice) << "\n";
	supportdb << "\t};\n";
	buffer.push_back(supportdb.str());
}

bool SupportDataSet::GenerateCSV(string basefolder) {
	return MemoriaUtility::GenerateDatabaseGeneric<SupportDataStruct>(_(basefolder), _(HADES_STRING_CSV_SUPPORT_FILE), csv_header, _(L"\n"), _(L"\n"), support, csv_format, true);
}

void SupportDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype, bool onlymodified, bool asmes, SteamLanguage lang) {
	vector<int> writesubset;
	if (texttype == 0) {
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<SupportDataStruct>(&writesubset, GetGameConfiguration()->support_name_file, support, [lang](SupportDataStruct& sp) { return sp.name.multi_lang_str[lang]; }, lang))
			WriteSteamTextGeneric(ffbin, support, &SupportDataStruct::name, &writesubset, asmes, lang);
		else
			WriteSteamTextGeneric(ffbin, support, &SupportDataStruct::name, NULL, asmes, lang);
	} else {
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<SupportDataStruct>(&writesubset, GetGameConfiguration()->support_help_file, support, [lang](SupportDataStruct& sp) { return sp.help.multi_lang_str[lang]; }, lang))
			WriteSteamTextGeneric(ffbin, support, &SupportDataStruct::help, &writesubset, asmes, lang);
		else
			WriteSteamTextGeneric(ffbin, support, &SupportDataStruct::help, NULL, asmes, lang);
	}
}

void SupportDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	int supportamount = SUPPORT_AMOUNT;
	bool useextendedtype2 = false;
	uint32_t txtpos;
	int i;
	UpdateOffset();
	ffbin.seekg(config.support_data_offset);
	MACRO_SUPPORT_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.support_text_offset[0]);
	MACRO_SUPPORT_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	MACRO_SUPPORT_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.support_text_offset[1]);
	MACRO_SUPPORT_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.support_text_offset[2]);
	MACRO_SUPPORT_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.support_text_offset[3]);
	MACRO_SUPPORT_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
}

void SupportDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	int supportamount = SUPPORT_AMOUNT;
	bool useextendedtype2 = false;
	uint32_t txtpos;
	int i;
	UpdateOffset();
	ffbin.seekg(config.support_data_offset);
	MACRO_SUPPORT_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.support_text_offset[0]);
	MACRO_SUPPORT_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SUPPORT_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.support_text_offset[1]);
	MACRO_SUPPORT_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.support_text_offset[2]);
	MACRO_SUPPORT_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.support_text_offset[3]);
	MACRO_SUPPORT_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
}

int SupportDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	int supportamount = SUPPORT_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	vector<SupportDataStruct> nonmodified;
	SteamLanguage lg;
	int txtspace;
	int i;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	HWSReadShort(ffbin, version);
	HWSReadShort(ffbin, name_space_total);
	HWSReadShort(ffbin, help_space_total);
	if (version >= 2) {
		useextendedtype = true;
		vector<int> added;
		supportamount = PrepareHWSFlexibleList(ffbin, support, nonmodified, added);
		for (i = 0; i < (int)added.size(); i++) {
			support[added[i]].name.CreateEmpty(true);
			support[added[i]].help.CreateEmpty(true);
			support[added[i]].parent = this;
		}
	}
	if (version >= 3) {
		useextendedtype2 = true;
		HWSReadCSVFormatting(ffbin, csv_header, csv_format);
		HWSReadCSVFields(ffbin, custom_field);
	}
	MACRO_SUPPORT_IOFUNCTIONDATA(HWSRead, HWSSeek, true, false)
	if (name_space_total <= namesize && usetext) {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			uint32_t txtpos;
			MACRO_SUPPORT_IOFUNCTIONNAME(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < supportamount; i++)
					support[i].name.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < supportamount; i++)
						SteamReadFF9String(ffbin, support[i].name, lg);
				else if (lg == GetSteamLanguage())
					for (i = 0; i < supportamount; i++) {
						SteamReadFF9String(ffbin, support[i].name);
						support[i].name.SteamToPSX();
					}
				ffbin.seekg(tmppos + txtspace);
				HWSReadChar(ffbin, lg);
			}
		}
	} else {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			ffbin.seekg(name_space_total, ios::cur);
		} else {
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				ffbin.seekg(txtspace, ios::cur);
				HWSReadChar(ffbin, lg);
			}
		}
		if (usetext)
			res |= 1;
	}
	if (help_space_total <= helpsize && usetext) {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			uint32_t txtpos;
			MACRO_SUPPORT_IOFUNCTIONHELP(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < supportamount; i++)
					support[i].help.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < supportamount; i++)
						SteamReadFF9String(ffbin, support[i].help, lg);
				else if (lg == GetSteamLanguage())
					for (i = 0; i < supportamount; i++) {
						SteamReadFF9String(ffbin, support[i].help);
						support[i].help.SteamToPSX();
					}
				ffbin.seekg(tmppos + txtspace);
				HWSReadChar(ffbin, lg);
			}
		}
	} else {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			ffbin.seekg(help_space_total, ios::cur);
		} else {
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				ffbin.seekg(txtspace, ios::cur);
				HWSReadChar(ffbin, lg);
			}
		}
		if (usetext)
			res |= 2;
	}
	if (version >= 2) {
		for (i = 0; i < supportamount; i++) {
			int boostedsize;
			HWSReadFlexibleChar(ffbin, boostedsize, true);
			support[i].boosted_support.resize(boostedsize);
			for (unsigned int j = 0; j < support[i].boosted_support.size(); j++)
				HWSReadFlexibleChar(ffbin, support[i].boosted_support[j], true);
		}
	}
	for (i = 0; i < (int)nonmodified.size(); i++)
		InsertAtId(support, nonmodified[i], nonmodified[i].id);
	name_space_total = namesize;
	help_space_total = helpsize;
	UpdateOffset();
	return res;
}

void SupportDataSet::WriteHWS(fstream& ffbin) {
	bool useextendedtype2 = true;
	int supportamount = support.size();
	int i;
	UpdateOffset();
	HWSWriteShort(ffbin, SUPPORT_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	HWSWriteShort(ffbin, name_space_total);
	HWSWriteShort(ffbin, help_space_total);
	HWSWriteFlexibleChar(ffbin, supportamount, true);
	for (i = 0; i < supportamount; i++)
		HWSWriteFlexibleChar(ffbin, support[i].id, true);
	HWSWriteCSVFormatting(ffbin, csv_header, csv_format);
	HWSWriteCSVFields(ffbin, custom_field);
	MACRO_SUPPORT_IOFUNCTIONDATA(HWSWrite, HWSSeek, false, false)
	if (GetGameType() == GAME_TYPE_PSX) {
		uint32_t txtpos;
		MACRO_SUPPORT_IOFUNCTIONNAME(HWSWrite, HWSSeek, false, false)
		MACRO_SUPPORT_IOFUNCTIONHELP(HWSWrite, HWSSeek, false, false)
	} else {
		SteamLanguage lg;
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(support, &SupportDataStruct::name, false, lg), true);
				WriteSteamText(ffbin, 0, false, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(support, &SupportDataStruct::help, false, lg), true);
				WriteSteamText(ffbin, 1, false, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
	}
	for (i = 0; i < supportamount; i++) {
		HWSWriteFlexibleChar(ffbin, support[i].boosted_support.size(), true);
		for (unsigned int j = 0; j < support[i].boosted_support.size(); j++)
			HWSWriteFlexibleChar(ffbin, support[i].boosted_support[j], true);
	}
	name_space_total = namesize;
	help_space_total = helpsize;
}

void SupportDataSet::UpdateOffset() {
	if (GetGameType()!=GAME_TYPE_PSX)
		return;
	uint16_t j=0,k=0;
	unsigned int i;
	for (i=0;i<SUPPORT_AMOUNT;i++) {
		support[i].name_offset = j;
		j += support[i].name.length;
		support[i].help_offset = k;
		k += support[i].help.length;
	}
	name_space_used = j;
	help_space_used = k;
}

int SupportDataSet::GetSupportIndexById(int supportid) {
	if (supportid < SUPPORT_AMOUNT)
		return supportid;
	for (unsigned int i = SUPPORT_AMOUNT; i < support.size(); i++)
		if (support[i].id == supportid)
			return i;
	return -1;
}

SupportDataStruct dummysupport;
SupportDataStruct& SupportDataSet::GetSupportById(int supportid) {
	int index = GetSupportIndexById(supportid);
	if (index >= 0)
		return support[index];
	dummysupport.id = -1;
	dummysupport.name.CreateEmpty();
	dummysupport.name.SetValue(L"[Invalid]");
	return dummysupport;
}
