#include "Commands.h"

#include "DllEditor.h"

#define COMMAND_HWS_VERSION 1

const unsigned int steam_cmd_field_size[] = { 16, 16, 16, 8, 8, 32 };

int CommandDataStruct::SetName(wstring newvalue) {
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

int CommandDataStruct::SetName(FF9String& newvalue) {
	int oldlen = name.length;
	int newlen = newvalue.length;
	if (parent->name_space_used+newlen>parent->name_space_total+oldlen)
		return 1;
	name = newvalue;
	parent->name_space_used += newlen-oldlen;
	return 0;
}

int CommandDataStruct::SetHelp(wstring newvalue) {
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

int CommandDataStruct::SetHelp(FF9String& newvalue) {
	int oldlen = help.length;
	int newlen = newvalue.length;
	if (parent->help_space_used+newlen>parent->help_space_total+oldlen)
		return 1;
	help = newvalue;
	parent->help_space_used += newlen-oldlen;
	return 0;
}

void CommandDataStruct::SetPanel(uint8_t newvalue) {
	if (panel==COMMAND_PANEL_SPELL && newvalue!=COMMAND_PANEL_SPELL) {
		if (link==0xFF)
			parent->spell_space_remaining += spell_amount;
		else {
			parent->cmd[link].link = 0xFF;
			link = 0xFF;
		}
		spell_list[0] = 0;
		spell_amount = 0;
	} else if (panel!=COMMAND_PANEL_SPELL && newvalue==COMMAND_PANEL_SPELL)
		spell_amount = 0;
	panel = newvalue;
}

bool CommandDataStruct::AddSpell(uint8_t spellid, uint8_t spellpos) {
	if (parent->spell_space_remaining==0)
		return false;
	int i;
	parent->spell_space_remaining--;
	for (i=spell_amount++;i>spellpos;i--)
		spell_list[i] = spell_list[i-1];
	spell_list[spellpos] = spellid;
	if (link!=0xFF)
		for (i=parent->cmd[link].spell_amount++;i>=spellpos;i--)
			parent->cmd[link].spell_list[i] = spell_list[i];
	return true;
}

void CommandDataStruct::RemoveSpell(uint8_t spellpos) {
	int i;
	parent->spell_space_remaining++;
	spell_amount--;
	for (i=spellpos;i<spell_amount;i++)
		spell_list[i] = spell_list[i+1];
	if (link!=0xFF) {
		parent->cmd[link].spell_amount--;
		for (i=spellpos;i<spell_amount;i++)
			parent->cmd[link].spell_list[i] = spell_list[i];
	}
}

void CommandDataStruct::MoveSpell(uint8_t spellpos, uint8_t newpos) {
	uint8_t tmp = spell_list[spellpos];
	spell_list[spellpos] = spell_list[newpos];
	spell_list[newpos] = tmp;
	if (link!=0xFF) {
		parent->cmd[link].spell_list[spellpos] = spell_list[spellpos];
		parent->cmd[link].spell_list[newpos] = spell_list[newpos];
	}
}

void CommandDataStruct::AddLink(uint8_t cmdid) {
	parent->spell_space_remaining += spell_amount;
	link = cmdid;
	parent->cmd[cmdid].link = id;
	spell_amount = parent->cmd[cmdid].spell_amount;
	for (int i=0;i<spell_amount;i++)
		spell_list[i] = parent->cmd[cmdid].spell_list[i];
}

void CommandDataStruct::ChangeLink(uint8_t newlink) {
	parent->cmd[link].link = 0xFF;
	link = newlink;
	parent->cmd[newlink].link = id;
	spell_amount = parent->cmd[newlink].spell_amount;
	for (int i=0;i<spell_amount;i++)
		spell_list[i] = parent->cmd[newlink].spell_list[i];
}

void CommandDataStruct::BreakLink() {
	parent->cmd[link].link = 0xFF;
	link = 0xFF;
	if (parent->spell_space_remaining<spell_amount)
		spell_amount = 0;
	else
		parent->spell_space_remaining -= spell_amount;
}

#define MACRO_COMMAND_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<COMMAND_AMOUNT;i++) { \
		IO ## Short(ffbin,cmd[i].name_offset); \
		IO ## Short(ffbin,cmd[i].help_offset); \
		IO ## Short(ffbin,cmd[i].help_size_x); \
		IO ## Char(ffbin,cmd[i].panel); \
		IO ## Char(ffbin,cmd[i].spell_amount); \
		IO ## Char(ffbin,cmd[i].spell_index); \
		IO ## Char(ffbin,zero8); \
		IO ## Short(ffbin,zero16); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_COMMAND_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<COMMAND_AMOUNT;i++) { \
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
	for (i=0;i<COMMAND_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,cmd[i].help_offset); \
		IO ## FF9String(ffbin,cmd[i].help); \
		if (READ) help_space_used += cmd[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total);

#define MACRO_COMMAND_IOFUNCTIONLIST(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<COMMAND_SPELL_AMOUNT;i++) \
		IO ## Char(ffbin,spell_full_list[i]); \
	if (PPF) PPFEndScanStep(); \
	if (READ) SetupLinks();


void CommandDataSet::Load(fstream &ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	name_space_total = config.cmd_name_space_total;
	help_space_total = config.cmd_help_space_total;
	for (i=0;i<COMMAND_AMOUNT;i++) {
		cmd[i].parent = this;
		cmd[i].id = i;
	}
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.cmd_data_offset[0]);
		MACRO_COMMAND_IOFUNCTIONDATA(FFIXRead,FFIXSeek,true,false)
		MACRO_COMMAND_IOFUNCTIONNAME(FFIXRead,FFIXSeek,true,false)
		MACRO_COMMAND_IOFUNCTIONHELP(FFIXRead,FFIXSeek,true,false)
		MACRO_COMMAND_IOFUNCTIONLIST(FFIXRead,FFIXSeek,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.cmd_name_file[GetSteamLanguage()]));
		name_space_used = ReadLong(ffbin);
		for (i=0;i<COMMAND_AMOUNT;i++)
			SteamReadFF9String(ffbin,cmd[i].name);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.cmd_help_file[GetSteamLanguage()]));
		help_space_used = ReadLong(ffbin);
		for (i=0;i<COMMAND_AMOUNT;i++)
			SteamReadFF9String(ffbin,cmd[i].help);
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_rdata_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x1F, COMMAND_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9COMMAND") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinst);
		steam_method_position = dlldata.dll_file.tellg();
		uint8_t* rawcmddata = dlldata.ConvertScriptToRaw_Object(COMMAND_AMOUNT,6,steam_cmd_field_size);
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg()-steam_method_position;
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_cmdspelllist_field_id));
		for (i=0;i<COMMAND_SPELL_AMOUNT;i++) {
			SteamReadChar(dlldata.dll_file,spell_full_list[i]);
			dlldata.dll_file.seekg(3,ios::cur);
		}
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		ffbin.write((const char*)rawcmddata,0xC*COMMAND_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_COMMAND_IOFUNCTIONDATA(SteamRead,SteamSeek,true,false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawcmddata;
		SetupLinks();
	}
}

DllMetaDataModification* CommandDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[2];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t*[COMMAND_AMOUNT];
	UpdateSpellsDatas();
	unsigned int i;
	for (i=0;i<COMMAND_AMOUNT;i++) {
		argvalue[i] = new uint32_t[6];
		argvalue[i][0] = cmd[i].name_offset;
		argvalue[i][1] = cmd[i].help_offset;
		argvalue[i][2] = cmd[i].help_size_x;
		argvalue[i][3] = cmd[i].panel;
		argvalue[i][4] = cmd[i].spell_amount;
		argvalue[i][5] = cmd[i].spell_index;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position,steam_method_base_length,COMMAND_AMOUNT,6,steam_cmd_field_size);
	for (i=0;i<COMMAND_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	res[1].position = config.meta_dll.GetStaticFieldOffset(config.dll_cmdspelllist_field_id);
	res[1].base_length = 4*COMMAND_SPELL_AMOUNT;
	res[1].new_length = 4*COMMAND_SPELL_AMOUNT;
	res[1].value = new uint8_t[res[1].new_length];
	BufferInitPosition();
	for (i=0;i<COMMAND_SPELL_AMOUNT;i++)
		BufferWriteLong(res[1].value,spell_full_list[i]);
	*modifamount = 2;
	return res;
}

void CommandDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype) {
	unsigned int i;
	if (texttype==0) {
		for (i=0;i<COMMAND_AMOUNT;i++)
			SteamWriteFF9String(ffbin,cmd[i].name);
	} else {
		for (i=0;i<COMMAND_AMOUNT;i++)
			SteamWriteFF9String(ffbin,cmd[i].help);
	}
}

void CommandDataSet::Write(fstream &ffbin, ConfigurationSet& config) {
	unsigned int i,j;
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
	unsigned int i,j;
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

int CommandDataSet::LoadHWS(fstream &ffbin, bool usetext) {
	unsigned int i,j;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	HWSReadShort(ffbin,version);
	if (version>50) {
		name_space_total = version;
		version = 1;
	} else
		HWSReadShort(ffbin,name_space_total);
	HWSReadShort(ffbin,help_space_total);
	MACRO_COMMAND_IOFUNCTIONDATA(HWSRead,HWSSeek,true,false)
	if (name_space_total<=namesize && usetext) {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			MACRO_COMMAND_IOFUNCTIONNAME(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<COMMAND_AMOUNT;i++)
					cmd[i].name.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=0xFF) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType()!=GAME_TYPE_PSX && lg==GetSteamLanguage()) { // DEBUG : need to make Steam language compatible with PSX versions
					for (i=0;i<COMMAND_AMOUNT;i++) {
						SteamReadFF9String(ffbin,cmd[i].name);
						if (GetGameType()==GAME_TYPE_PSX)
							cmd[i].name.SteamToPSX();
					}
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
			while (lg!=0xFF) {
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
			MACRO_COMMAND_IOFUNCTIONHELP(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<COMMAND_AMOUNT;i++)
					cmd[i].help.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=0xFF) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType()!=GAME_TYPE_PSX && lg==GetSteamLanguage()) { // DEBUG : need to make Steam language compatible with PSX versions
					for (i=0;i<COMMAND_AMOUNT;i++) {
						SteamReadFF9String(ffbin,cmd[i].help);
						if (GetGameType()==GAME_TYPE_PSX)
							cmd[i].help.SteamToPSX();
					}
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
			while (lg!=0xFF) {
				HWSReadShort(ffbin,txtspace);
				ffbin.seekg(txtspace,ios::cur);
				HWSReadChar(ffbin,lg);
			}
		}
		if (usetext)
			res |= 2;
	}
	MACRO_COMMAND_IOFUNCTIONLIST(HWSRead,HWSSeek,true,false)
	name_space_total = namesize;
	help_space_total = helpsize;
	UpdateOffset();
	return res;
}

void CommandDataSet::WriteHWS(fstream &ffbin) {
	unsigned int i,j;
	uint32_t txtpos;
	uint8_t zero8 = 0;
	uint16_t zero16 = 0;
	UpdateOffset();
	UpdateSpellsDatas();
	HWSWriteShort(ffbin,COMMAND_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	HWSWriteShort(ffbin,name_space_total);
	HWSWriteShort(ffbin,help_space_total);
	MACRO_COMMAND_IOFUNCTIONDATA(HWSWrite,HWSSeek,false,false)
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_COMMAND_IOFUNCTIONNAME(HWSWrite,HWSSeek,false,false)
		MACRO_COMMAND_IOFUNCTIONHELP(HWSWrite,HWSSeek,false,false)
	} else {
		SteamLanguage lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,name_space_total);
		for (i=0;i<COMMAND_AMOUNT;i++)
			SteamWriteFF9String(ffbin,cmd[i].name);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
		lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,help_space_total);
		for (i=0;i<COMMAND_AMOUNT;i++)
			SteamWriteFF9String(ffbin,cmd[i].help);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
	}
	MACRO_COMMAND_IOFUNCTIONLIST(HWSWrite,HWSSeek,false,false)
	name_space_total = namesize;
	help_space_total = helpsize;
}

void CommandDataSet::UpdateOffset() {
	uint16_t j=0,k=0;
	unsigned int i;
	for (i=0;i<COMMAND_AMOUNT;i++) {
		cmd[i].name_offset = j;
		j += cmd[i].name.length;
		cmd[i].help_offset = k;
		k += cmd[i].help.length;
	}
	name_space_used = j;
	help_space_used = k;
}

void CommandDataSet::SetupLinks() {
	unsigned int i,j;
	spell_space_remaining = COMMAND_SPELL_AMOUNT;
	for (i=0;i<COMMAND_AMOUNT;i++)
		cmd[i].link = 0xFF;
	for (i=0;i<COMMAND_AMOUNT;i++)
		if (cmd[i].panel==COMMAND_PANEL_SPELL) {
			if (cmd[i].link==0xFF) {
				spell_space_remaining -= cmd[i].spell_amount;
				for (j=i+1;j<COMMAND_AMOUNT;j++)
					if (cmd[i].spell_index==cmd[j].spell_index && cmd[i].spell_amount==cmd[j].spell_amount) {
						cmd[i].link = j;
						cmd[j].link = i;
						break;
					}
			}
			for (j=0;j<cmd[i].spell_amount;j++)
				cmd[i].spell_list[j] = spell_full_list[cmd[i].spell_index+j];
		} else
			cmd[i].spell_list[0] = cmd[i].spell_index;
}

void CommandDataSet::UpdateSpellsDatas() {
	int i,j,k = 0;
	for (i=0;i<COMMAND_AMOUNT;i++)
		if (cmd[i].panel==COMMAND_PANEL_SPELL) {
			if (cmd[i].link>i) {
				cmd[i].spell_index = k;
				for (j=0;j<cmd[i].spell_amount;j++)
					spell_full_list[k++] = cmd[i].spell_list[j];
			} else
				cmd[i].spell_index = cmd[cmd[i].link].spell_index;
		} else
			cmd[i].spell_index = cmd[i].spell_list[0];
}
