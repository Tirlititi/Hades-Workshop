#include "Commands.h"

#include "DllEditor.h"
#include "main.h"
#include "Database_CSV.h"
#include "CommonUtility.h"

#define COMMAND_HWS_VERSION 2

const unsigned int steam_cmd_field_size[] = { 16, 16, 16, 8, 8, 32 };

int CommandDataStruct::SetName(wstring newvalue) {
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

int CommandDataStruct::SetName(FF9String& newvalue) {
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

int CommandDataStruct::SetHelp(wstring newvalue) {
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

int CommandDataStruct::SetHelp(FF9String& newvalue) {
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

void CommandDataStruct::SetPanel(uint8_t newvalue) {
	if (panel == COMMAND_PANEL_SPELL && newvalue != COMMAND_PANEL_SPELL) {
		if (link < 0)
			parent->spell_space_remaining += spell_amount;
		else {
			parent->GetCommandById(link).link = -1;
			link = -1;
		}
		spell_list[0] = 0;
		spell_amount = 0;
		spell_list.resize(1);
	} else if (panel != COMMAND_PANEL_SPELL && newvalue == COMMAND_PANEL_SPELL) {
		spell_amount = 0;
		spell_list.clear();
	}
	panel = newvalue;
}

bool CommandDataStruct::AddSpell(int spellid, int spellpos, bool uselimit) {
	if (uselimit && parent->spell_space_remaining == 0)
		return false;
	if (uselimit)
		parent->spell_space_remaining--;
	spell_list.insert(spell_list.begin() + spellpos, spellid);
	spell_amount++;
	if (link >= 0) {
		parent->GetCommandById(link).spell_list.insert(parent->GetCommandById(link).spell_list.begin() + spellpos, spellid);
		parent->GetCommandById(link).spell_amount++;
	}
	return true;
}

void CommandDataStruct::RemoveSpell(int spellpos, bool uselimit) {
	if (uselimit)
		parent->spell_space_remaining++;
	spell_amount--;
	spell_list.erase(spell_list.begin() + spellpos);
	if (link >= 0) {
		parent->GetCommandById(link).spell_list.erase(parent->GetCommandById(link).spell_list.begin() + spellpos);
		parent->GetCommandById(link).spell_amount--;
	}
}

void CommandDataStruct::MoveSpell(int spellpos, int newpos) {
	int tmp = spell_list[spellpos];
	spell_list[spellpos] = spell_list[newpos];
	spell_list[newpos] = tmp;
	if (link >= 0) {
		parent->GetCommandById(link).spell_list[spellpos] = spell_list[spellpos];
		parent->GetCommandById(link).spell_list[newpos] = spell_list[newpos];
	}
}

void CommandDataStruct::AddLink(int cmdid) {
	parent->spell_space_remaining += spell_amount;
	link = parent->cmd[cmdid].id;
	parent->cmd[cmdid].link = id;
	spell_amount = parent->cmd[cmdid].spell_amount;
	spell_list.resize(spell_amount);
	for (int i = 0; i < spell_amount; i++)
		spell_list[i] = parent->cmd[cmdid].spell_list[i];
}

void CommandDataStruct::ChangeLink(int newlink) {
	parent->GetCommandById(link).link = -1;
	link = parent->cmd[newlink].id;
	parent->cmd[newlink].link = id;
	spell_amount = parent->cmd[newlink].spell_amount;
	spell_list.resize(spell_amount);
	for (int i = 0; i < spell_amount; i++)
		spell_list[i] = parent->cmd[newlink].spell_list[i];
}

void CommandDataStruct::BreakLink() {
	parent->GetCommandById(link).link = -1;
	link = -1;
	if (parent->spell_space_remaining < spell_amount) {
		spell_amount = 0;
		spell_list.clear();
	} else {
		parent->spell_space_remaining -= spell_amount;
	}
}

#define MACRO_COMMAND_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<cmdamount;i++) { \
		IO ## Short(ffbin,cmd[i].name_offset); \
		IO ## Short(ffbin,cmd[i].help_offset); \
		IO ## Short(ffbin,cmd[i].help_size_x); \
		IO ## Char(ffbin,cmd[i].panel); \
		IO ## FlexibleChar(ffbin,cmd[i].spell_amount, useextendedtype); \
		IO ## FlexibleChar(ffbin,cmd[i].spell_index, useextendedtype); \
		IO ## Char(ffbin,zero8); \
		IO ## Short(ffbin,zero16); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_COMMAND_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<cmdamount;i++) { \
		SEEK(ffbin,txtpos,cmd[i].name_offset); \
		IO ## FF9String(ffbin,cmd[i].name); \
		if (READ) name_space_used += cmd[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,name_space_total);

#define MACRO_COMMAND_IOFUNCTIONHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) help_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,help_space_total); \
	for (i=0;i<cmdamount;i++) { \
		SEEK(ffbin,txtpos,cmd[i].help_offset); \
		IO ## FF9String(ffbin,cmd[i].help); \
		if (READ) help_space_used += cmd[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total);

#define MACRO_COMMAND_IOFUNCTIONLIST(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	if (useextendedtype) { \
		int fullistsize; \
		if (!READ) fullistsize = spell_full_list.size(); \
		IO ## FlexibleChar(ffbin, fullistsize, useextendedtype); \
		if (READ) spell_full_list.resize(fullistsize); \
		for (i = 0; i < fullistsize; i++) \
			IO ## FlexibleChar(ffbin, spell_full_list[i], useextendedtype); \
	} else { \
		if (READ) spell_full_list.resize(COMMAND_SPELL_AMOUNT); \
		for (i = 0; i < COMMAND_SPELL_AMOUNT; i++) \
			IO ## FlexibleChar(ffbin, spell_full_list[i], useextendedtype); \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) SetupLinks();


void CommandDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int cmdamount = COMMAND_AMOUNT;
	bool useextendedtype = false;
	int i;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	name_space_total = config.cmd_name_space_total;
	help_space_total = config.cmd_help_space_total;
	cmd.resize(COMMAND_AMOUNT);
	spell_full_list.resize(COMMAND_SPELL_AMOUNT);
	for (i = 0; i < COMMAND_AMOUNT; i++) {
		cmd[i].parent = this;
		cmd[i].id = i;
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		ffbin.seekg(config.cmd_data_offset[0]);
		MACRO_COMMAND_IOFUNCTIONDATA(FFIXRead, FFIXSeek, true, false)
		MACRO_COMMAND_IOFUNCTIONNAME(FFIXRead, FFIXSeek, true, false)
		MACRO_COMMAND_IOFUNCTIONHELP(FFIXRead, FFIXSeek, true, false)
		MACRO_COMMAND_IOFUNCTIONLIST(FFIXRead, FFIXSeek, true, false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.cmd_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.cmd_name_file[lang]);
			for (i = 0; i < COMMAND_AMOUNT; i++)
				SteamReadFF9String(ffbin, cmd[i].name, lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.cmd_help_file[lang]));
			help_space_used = config.meta_res.GetFileSizeByIndex(config.cmd_help_file[lang]);
			for (i = 0; i < COMMAND_AMOUNT; i++)
				SteamReadFF9String(ffbin, cmd[i].help, lang);
		}
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_rdata_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x1F, COMMAND_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9COMMAND") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 3, initinst);
		steam_method_position = dlldata.dll_file.tellg();
		uint8_t* rawcmddata = dlldata.ConvertScriptToRaw_Object(COMMAND_AMOUNT, 6, steam_cmd_field_size);
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg() - steam_method_position;
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_cmdspelllist_field_id));
		for (i = 0; i < COMMAND_SPELL_AMOUNT; i++) {
			SteamReadFlexibleChar(dlldata.dll_file, spell_full_list[i], false);
			dlldata.dll_file.seekg(3, ios::cur);
		}
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(), ios::out | ios::binary);
		ffbin.write((const char*)rawcmddata, 0xC * COMMAND_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		MACRO_COMMAND_IOFUNCTIONDATA(SteamRead, SteamSeek, true, false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawcmddata;
		SetupLinks();
	}
}

DllMetaDataModification* CommandDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[2];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t * [COMMAND_AMOUNT];
	UpdateSpellsDatas();
	unsigned int i;
	for (i = 0; i < COMMAND_AMOUNT; i++) {
		argvalue[i] = new uint32_t[6];
		argvalue[i][0] = cmd[i].name_offset;
		argvalue[i][1] = cmd[i].help_offset;
		argvalue[i][2] = cmd[i].help_size_x;
		argvalue[i][3] = cmd[i].panel;
		argvalue[i][4] = cmd[i].spell_amount;
		argvalue[i][5] = cmd[i].spell_index;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position, steam_method_base_length, COMMAND_AMOUNT, 6, steam_cmd_field_size);
	for (i = 0; i < COMMAND_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	res[1].position = config.meta_dll.GetStaticFieldOffset(config.dll_cmdspelllist_field_id);
	res[1].base_length = 4 * COMMAND_SPELL_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_cmdspelllist_field_id);
	res[1].new_length = 4 * COMMAND_SPELL_AMOUNT;
	res[1].value = new uint8_t[res[1].new_length];
	BufferInitPosition();
	for (i = 0; i < COMMAND_SPELL_AMOUNT; i++)
		BufferWriteLong(res[1].value, i < spell_full_list.size() ? spell_full_list[i] : 0);
	*modifamount = 2;
	return res;
}

void CommandDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i;
	stringstream cmddb;
	cmddb << "// Method: rdata::.cctor\n\n";
	cmddb << "\trdata._FF9FAbil_ComData = new rdata.FF9COMMAND[] {\n";
	for (i = 0; i < COMMAND_AMOUNT; i++)
		cmddb << "\t\tnew rdata.FF9COMMAND(" << (int)cmd[i].name_offset << ", " << (int)cmd[i].help_offset << ", " << (int)cmd[i].help_size_x << ", " << (int)cmd[i].panel << ", " << (int)cmd[i].spell_amount << ", " << (unsigned long)cmd[i].spell_index << (i + 1 == COMMAND_AMOUNT ? "UL)" : "UL),") << " // " << ConvertWStrToStr(cmd[i].name.str_nice) << "\n";
	cmddb << "\t};\n";
	cmddb << "\trdata._FF9BMenu_ComData = rdata._FF9FAbil_ComData;\n";
	cmddb << "\trdata._FF9BMenu_ComAbil = new int[] { ";
	for (i = 0; i < COMMAND_SPELL_AMOUNT; i++)
		cmddb << (i < spell_full_list.size() ? spell_full_list[i] : 0) << (i + 1 == COMMAND_SPELL_AMOUNT ? " " : ", ");
	cmddb << "};\n";
	cmddb << "\trdata._FF9FAbil_ComAbil = rdata._FF9BMenu_ComAbil;\n";
	buffer.push_back(cmddb.str());
}

wxString CSV_CommandConstructor(CommandDataStruct& cm, int index) {
	if ((cm.id == 35 || cm.id == 37 || cm.id == 39) && cm.panel == COMMAND_PANEL_SPELL && cm.spell_amount == 0)
		return wxEmptyString;
	if (cm.id == 47 && cm.panel == COMMAND_PANEL_NONE && (cm.spell_list.size() == 0 || cm.spell_list[0] == 0))
		return wxEmptyString;
	wxString csventry = wxString::Format(wxT("%d;%d;%d;"), cm.id, cm.panel, cm.spell_list.size() > 0 ? cm.spell_list[0] : 0);
	if (cm.panel == COMMAND_PANEL_SPELL)
		csventry += ConcatenateStrings<int>(", ", cm.spell_list, static_cast<string(*)(int)>(&to_string));
	csventry += wxString::Format(wxT(";# %s"), cm.name.str_nice);
	return csventry;
}

bool CSV_CommandComparer(wxString left, wxString right) {
	wxArrayString leftentries = MemoriaUtility::LoadCSVEntry(left);
	if (leftentries.GetCount() >= 4 && wxAtoi(leftentries[1]) != COMMAND_PANEL_SPELL)
		return left.BeforeLast(L';').BeforeLast(L';').IsSameAs(right.BeforeLast(L';').BeforeLast(L';'));
	return left.BeforeLast(L';').IsSameAs(right.BeforeLast(L';'));
}

bool CommandDataSet::GenerateCSV(string basefolder) {
	return MemoriaUtility::GenerateCSVGeneric<CommandDataStruct>(_(basefolder), _(HADES_STRING_CSV_COMMAND_FILE), _(HADES_STRING_CSV_COMMAND_HEADER), cmd, &CSV_CommandConstructor, &CSV_CommandComparer, true);
}

void CommandDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype, bool onlymodified, bool asmes, SteamLanguage lang) {
	vector<int> writesubset;
	if (texttype == 0) {
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<CommandDataStruct>(&writesubset, GetGameConfiguration()->cmd_name_file, cmd, [lang](CommandDataStruct& cm) { return cm.name.multi_lang_str[lang]; }, lang))
			WriteSteamTextGeneric(ffbin, cmd, &CommandDataStruct::name, &writesubset, asmes, lang);
		else
			WriteSteamTextGeneric(ffbin, cmd, &CommandDataStruct::name, NULL, asmes, lang);
	} else {
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<CommandDataStruct>(&writesubset, GetGameConfiguration()->cmd_help_file, cmd, [lang](CommandDataStruct& cm) { return cm.help.multi_lang_str[lang]; }, lang))
			WriteSteamTextGeneric(ffbin, cmd, &CommandDataStruct::help, &writesubset, asmes, lang);
		else
			WriteSteamTextGeneric(ffbin, cmd, &CommandDataStruct::help, NULL, asmes, lang);
	}
}

void CommandDataSet::Write(fstream &ffbin, ConfigurationSet& config) {
	int cmdamount = COMMAND_AMOUNT;
	bool useextendedtype = false;
	int i;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	UpdateOffset();
	UpdateSpellsDatas();
	ffbin.seekg(config.cmd_data_offset[0]);
	MACRO_COMMAND_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_COMMAND_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	MACRO_COMMAND_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	MACRO_COMMAND_IOFUNCTIONLIST(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.cmd_data_offset[1]);
	MACRO_COMMAND_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_COMMAND_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	MACRO_COMMAND_IOFUNCTIONLIST(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.cmd_data_offset[2]);
	MACRO_COMMAND_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_COMMAND_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
}

void CommandDataSet::WritePPF(fstream &ffbin, ConfigurationSet& config) {
	int cmdamount = COMMAND_AMOUNT;
	bool useextendedtype = false;
	int i;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	UpdateOffset();
	UpdateSpellsDatas();
	ffbin.seekg(config.cmd_data_offset[0]);
	MACRO_COMMAND_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_COMMAND_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	MACRO_COMMAND_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_COMMAND_IOFUNCTIONLIST(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.cmd_data_offset[1]);
	MACRO_COMMAND_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_COMMAND_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	MACRO_COMMAND_IOFUNCTIONLIST(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.cmd_data_offset[2]);
	MACRO_COMMAND_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_COMMAND_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
}

int CommandDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	int cmdamount = COMMAND_AMOUNT;
	bool useextendedtype = false;
	vector<CommandDataStruct> nonmodified;
	SteamLanguage lg;
	int txtspace;
	int i;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	HWSReadShort(ffbin, version);
	if (version > 50) {
		name_space_total = version;
		version = 1;
	} else {
		HWSReadShort(ffbin, name_space_total);
	}
	HWSReadShort(ffbin, help_space_total);
	if (version >= 2) {
		useextendedtype = true;
		vector<int> added;
		cmdamount = PrepareHWSFlexibleList(ffbin, cmd, nonmodified, added);
		for (i = 0; i < (int)added.size(); i++) {
			cmd[added[i]].name.CreateEmpty(true);
			cmd[added[i]].help.CreateEmpty(true);
			cmd[added[i]].parent = this;
		}
	}
	MACRO_COMMAND_IOFUNCTIONDATA(HWSRead, HWSSeek, true, false)
	if (name_space_total <= namesize && usetext) {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			MACRO_COMMAND_IOFUNCTIONNAME(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < cmdamount; i++)
					cmd[i].name.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < cmdamount; i++)
						SteamReadFF9String(ffbin, cmd[i].name, lg);
				else if (lg == GetSteamLanguage())
					for (i = 0; i < cmdamount; i++) {
						SteamReadFF9String(ffbin, cmd[i].name);
						cmd[i].name.SteamToPSX();
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
			MACRO_COMMAND_IOFUNCTIONHELP(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < cmdamount; i++)
					cmd[i].help.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < cmdamount; i++)
						SteamReadFF9String(ffbin, cmd[i].help, lg);
				else if (lg == GetSteamLanguage())
					for (i = 0; i < cmdamount; i++) {
						SteamReadFF9String(ffbin, cmd[i].help);
						cmd[i].help.SteamToPSX();
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
	MACRO_COMMAND_IOFUNCTIONLIST(HWSRead, HWSSeek, true, false)
	for (i = 0; i < (int)nonmodified.size(); i++)
		InsertAtId(cmd, nonmodified[i], nonmodified[i].id);
	name_space_total = namesize;
	help_space_total = helpsize;
	UpdateOffset();
	return res;
}

void CommandDataSet::WriteHWS(fstream& ffbin) {
	int cmdamount = cmd.size();
	bool useextendedtype = true;
	int i;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	UpdateOffset();
	UpdateSpellsDatas();
	HWSWriteShort(ffbin, COMMAND_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	HWSWriteShort(ffbin, name_space_total);
	HWSWriteShort(ffbin, help_space_total);
	HWSWriteFlexibleChar(ffbin, cmdamount, true);
	for (i = 0; i < cmdamount; i++)
		HWSWriteFlexibleChar(ffbin, cmd[i].id, true);
	MACRO_COMMAND_IOFUNCTIONDATA(HWSWrite, HWSSeek, false, false)
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_COMMAND_IOFUNCTIONNAME(HWSWrite, HWSSeek, false, false)
		MACRO_COMMAND_IOFUNCTIONHELP(HWSWrite, HWSSeek, false, false)
	} else {
		SteamLanguage lg;
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(cmd, &CommandDataStruct::name, false, lg), true);
				WriteSteamText(ffbin, 0, false, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(cmd, &CommandDataStruct::help, false, lg), true);
				WriteSteamText(ffbin, 1, false, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
	}
	MACRO_COMMAND_IOFUNCTIONLIST(HWSWrite, HWSSeek, false, false)
	name_space_total = namesize;
	help_space_total = helpsize;
}

void CommandDataSet::UpdateOffset() {
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	uint16_t j = 0, k = 0;
	int i;
	for (i = 0; i < COMMAND_AMOUNT; i++) {
		cmd[i].name_offset = j;
		j += cmd[i].name.length;
		cmd[i].help_offset = k;
		k += cmd[i].help.length;
	}
	name_space_used = j;
	help_space_used = k;
}

void CommandDataSet::SetupLinks() {
	unsigned int i, j;
	spell_space_remaining = COMMAND_SPELL_AMOUNT;
	for (i = 0; i < cmd.size(); i++)
		cmd[i].link = -1;
	for (i = 0; i < cmd.size(); i++)
		if (cmd[i].panel == COMMAND_PANEL_SPELL) {
			if (cmd[i].link < 0) {
				spell_space_remaining -= cmd[i].spell_amount;
				for (j = i + 1; j < cmd.size(); j++)
					if (cmd[i].spell_index == cmd[j].spell_index && cmd[i].spell_amount == cmd[j].spell_amount) {
						cmd[i].link = cmd[j].id;
						cmd[j].link = cmd[i].id;
						break;
					}
			}
			cmd[i].spell_list.resize(cmd[i].spell_amount);
			for (j = 0; (int)j < cmd[i].spell_amount; j++)
				cmd[i].spell_list[j] = spell_full_list[cmd[i].spell_index + j];
		} else {
			cmd[i].spell_list.resize(1);
			cmd[i].spell_list[0] = cmd[i].spell_index;
		}
}

void CommandDataSet::UpdateSpellsDatas() {
	int i, j, k = 0;
	for (i = 0; i < (int)cmd.size(); i++)
		if (cmd[i].panel == COMMAND_PANEL_SPELL) {
			if (cmd[i].link < 0 || cmd[i].link > i) {
				cmd[i].spell_index = k;
				if ((int)spell_full_list.size() < cmd[i].spell_index + cmd[i].spell_amount)
					spell_full_list.resize(cmd[i].spell_index + cmd[i].spell_amount);
				for (j = 0; j < cmd[i].spell_amount; j++)
					spell_full_list[k++] = cmd[i].spell_list[j];
			} else {
				cmd[i].spell_index = GetCommandById(cmd[i].link).spell_index;
			}
		} else {
			cmd[i].spell_index = cmd[i].spell_list[0];
		}
}

int CommandDataSet::GetCommandIndexById(int cmdid) {
	if (cmdid < COMMAND_AMOUNT)
		return cmdid;
	for (unsigned int i = COMMAND_AMOUNT; i < cmd.size(); i++)
		if (cmd[i].id == cmdid)
			return i;
	return -1;
}

CommandDataStruct dummycmd;
CommandDataStruct& CommandDataSet::GetCommandById(int cmdid) {
	int index = GetCommandIndexById(cmdid);
	if (index >= 0)
		return cmd[index];
	dummycmd.id = -1;
	dummycmd.name.CreateEmpty();
	dummycmd.name.SetValue(L"[Invalid]");
	return dummycmd;
}
