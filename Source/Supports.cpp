#include "Supports.h"

#include "main.h"
#include "DllEditor.h"

#define SUPPORT_HWS_VERSION 1

const unsigned int steam_support_field_size[] = { 8, 8, 16, 16, 16 };

int SupportDataStruct::SetName(wstring newvalue) {
	FF9String tmp(name);
	tmp.SetValue(newvalue);
	int oldlen = name.length;
	int newlen = tmp.length;
	if (parent->name_space_used+newlen>parent->name_space_total+oldlen)
		return 1;
	name.SetValue(newvalue);
	parent->name_space_used += newlen-oldlen;
	return 0;
}

int SupportDataStruct::SetName(FF9String& newvalue) {
	int oldlen = name.length;
	int newlen = newvalue.length;
	if (parent->name_space_used+newlen>parent->name_space_total+oldlen)
		return 1;
	name = newvalue;
	parent->name_space_used += newlen-oldlen;
	return 0;
}

int SupportDataStruct::SetHelp(wstring newvalue) {
	FF9String tmp(help);
	tmp.SetValue(newvalue);
	int oldlen = help.length;
	int newlen = tmp.length;
	if (parent->help_space_used+newlen>parent->help_space_total+oldlen)
		return 1;
	help.SetValue(newvalue);
	parent->help_space_used += newlen-oldlen;
	return 0;
}

int SupportDataStruct::SetHelp(FF9String& newvalue) {
	int oldlen = help.length;
	int newlen = newvalue.length;
	if (parent->help_space_used+newlen>parent->help_space_total+oldlen)
		return 1;
	help = newvalue;
	parent->help_space_used += newlen-oldlen;
	return 0;
}

#define MACRO_SUPPORT_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	uint32_t txtpos; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<SUPPORT_AMOUNT;i++) { \
		IO ## Char(ffbin,support[i].category); \
		IO ## Char(ffbin,support[i].cost); \
		IO ## Short(ffbin,support[i].name_offset); \
		IO ## Short(ffbin,support[i].help_offset); \
		IO ## Short(ffbin,support[i].help_size_x); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SUPPORT_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<SUPPORT_AMOUNT;i++) { \
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
	for (i=0;i<SUPPORT_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,support[i].help_offset); \
		IO ## FF9String(ffbin,support[i].help); \
		if (READ) help_space_used += support[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total);


void SupportDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	name_space_total = config.support_name_space_total;
	help_space_total = config.support_help_space_total;
	for (i=0;i<SUPPORT_AMOUNT;i++)
		support[i].parent = this;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.support_data_offset);
		MACRO_SUPPORT_IOFUNCTIONDATA(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.support_text_offset[0]);
		MACRO_SUPPORT_IOFUNCTIONNAME(FFIXRead,FFIXSeek,true,false)
		MACRO_SUPPORT_IOFUNCTIONHELP(FFIXRead,FFIXSeek,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.support_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.support_name_file[lang]);
			for (i=0;i<SUPPORT_AMOUNT;i++)
				SteamReadFF9String(ffbin,support[i].name,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.support_help_file[lang]));
			help_space_used = config.meta_res.GetFileSizeByIndex(config.support_help_file[lang]);
			for (i=0;i<SUPPORT_AMOUNT;i++)
				SteamReadFF9String(ffbin,support[i].help,lang);
		}
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_ability_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x1F, SUPPORT_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("SA_DATA","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinst);
		steam_method_position = dlldata.dll_file.tellg();
		uint8_t* rawsupportdata = dlldata.ConvertScriptToRaw_Object(SUPPORT_AMOUNT,5,steam_support_field_size);
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg()-steam_method_position;
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		ffbin.write((const char*)rawsupportdata,8*SUPPORT_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_SUPPORT_IOFUNCTIONDATA(SteamRead,SteamSeek,true,false)
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
	for (i=0;i<SUPPORT_AMOUNT;i++) {
		argvalue[i] = new uint32_t[5];
		argvalue[i][0] = support[i].category;
		argvalue[i][1] = support[i].cost;
		argvalue[i][2] = support[i].name_offset;
		argvalue[i][3] = support[i].help_offset;
		argvalue[i][4] = support[i].help_size_x;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position,steam_method_base_length,SUPPORT_AMOUNT,5,steam_support_field_size);
	for (i=0;i<SUPPORT_AMOUNT;i++)
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

int SupportDataSet::GetSteamTextSize(unsigned int texttype, SteamLanguage lang) {
	unsigned int i;
	int res = 0;
	if (texttype==0)
		for (i=0;i<SUPPORT_AMOUNT;i++)
			res += support[i].name.GetLength(lang);
	else
		for (i=0;i<SUPPORT_AMOUNT;i++)
			res += support[i].help.GetLength(lang);
	return res;
}

void SupportDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype, SteamLanguage lang) {
	unsigned int i;
	if (texttype==0) {
		for (i=0;i<SUPPORT_AMOUNT;i++)
			SteamWriteFF9String(ffbin,support[i].name,lang);
	} else {
		for (i=0;i<SUPPORT_AMOUNT;i++)
			SteamWriteFF9String(ffbin,support[i].help,lang);
	}
}

void SupportDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
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
	unsigned int i;
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
	unsigned int i;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	HWSReadShort(ffbin,version);
	HWSReadShort(ffbin,name_space_total);
	HWSReadShort(ffbin,help_space_total);
	MACRO_SUPPORT_IOFUNCTIONDATA(HWSRead,HWSSeek,true,false)
	if (name_space_total<=namesize && usetext) {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			MACRO_SUPPORT_IOFUNCTIONNAME(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<SUPPORT_AMOUNT;i++)
					support[i].name.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType()!=GAME_TYPE_PSX)
					for (i=0;i<SUPPORT_AMOUNT;i++)
						SteamReadFF9String(ffbin,support[i].name,lg);
				else if (lg==GetSteamLanguage())
					for (i=0;i<SUPPORT_AMOUNT;i++) {
						SteamReadFF9String(ffbin,support[i].name);
						support[i].name.SteamToPSX();
					}
				ffbin.seekg(tmppos+txtspace);
				HWSReadChar(ffbin,lg);
			}
		}
	} else {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			ffbin.seekg(name_space_total,ios::cur);
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			HWSReadChar(ffbin,lg);
			while (lg!=STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin,txtspace);
				ffbin.seekg(txtspace,ios::cur);
				HWSReadChar(ffbin,lg);
			}
		}
		if (usetext)
			res |= 1;
	}
	if (help_space_total<=helpsize && usetext) {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			MACRO_SUPPORT_IOFUNCTIONHELP(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<SUPPORT_AMOUNT;i++)
					support[i].help.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType()!=GAME_TYPE_PSX)
					for (i=0;i<SUPPORT_AMOUNT;i++)
						SteamReadFF9String(ffbin,support[i].help,lg);
				else if (lg==GetSteamLanguage())
					for (i=0;i<SUPPORT_AMOUNT;i++) {
						SteamReadFF9String(ffbin,support[i].help);
						support[i].help.SteamToPSX();
					}
				ffbin.seekg(tmppos+txtspace);
				HWSReadChar(ffbin,lg);
			}
		}
	} else {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			ffbin.seekg(help_space_total,ios::cur);
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			HWSReadChar(ffbin,lg);
			while (lg!=STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin,txtspace);
				ffbin.seekg(txtspace,ios::cur);
				HWSReadChar(ffbin,lg);
			}
		}
		if (usetext)
			res |= 2;
	}
	name_space_total = namesize;
	help_space_total = helpsize;
	UpdateOffset();
	return res;
}

void SupportDataSet::WriteHWS(fstream& ffbin) {
	unsigned int i;
	UpdateOffset();
	HWSWriteShort(ffbin,SUPPORT_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	HWSWriteShort(ffbin,name_space_total);
	HWSWriteShort(ffbin,help_space_total);
	MACRO_SUPPORT_IOFUNCTIONDATA(HWSWrite,HWSSeek,false,false)
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SUPPORT_IOFUNCTIONNAME(HWSWrite,HWSSeek,false,false)
		MACRO_SUPPORT_IOFUNCTIONHELP(HWSWrite,HWSSeek,false,false)
	} else {
		SteamLanguage lg;
		for (lg=STEAM_LANGUAGE_US;lg<STEAM_LANGUAGE_AMOUNT;lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin,lg);
				HWSWriteShort(ffbin,GetSteamTextSize(0,lg));
				WriteSteamText(ffbin,0,lg);
			}
		}
		HWSWriteChar(ffbin,STEAM_LANGUAGE_NONE);
		for (lg=STEAM_LANGUAGE_US;lg<STEAM_LANGUAGE_AMOUNT;lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin,lg);
				HWSWriteShort(ffbin,GetSteamTextSize(1,lg));
				WriteSteamText(ffbin,1,lg);
			}
		}
		HWSWriteChar(ffbin,STEAM_LANGUAGE_NONE);
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
