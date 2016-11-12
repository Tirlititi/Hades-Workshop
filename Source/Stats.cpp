#include "Stats.h"

#include "Hades_Strings.h"
#include "DllEditor.h"

#define STAT_HWS_VERSION 2

const unsigned int steam_statabil_field_size[] = { 8, 8 };
const unsigned int steam_stat_field_size[] = { 8, 8, 8, 8, 16 };
const unsigned int steam_lvlhpmp_field_size[] = { 16, 16 };

int InitialStatDataStruct::SetDefaultName(wstring newvalue) {
	FF9String tmp(default_name);
	tmp.SetValue(newvalue);
	int oldlen = default_name.length;
	int newlen = tmp.length;
	if (parent->default_name_space_used+newlen>parent->default_name_space_total+oldlen)
		return 1;
	default_name.SetValue(newvalue);
	parent->default_name_space_used += newlen-oldlen;
	return 0;
}

int InitialStatDataStruct::SetDefaultName(FF9String& newvalue) {
	int oldlen = default_name.length;
	int newlen = newvalue.length;
	if (parent->default_name_space_used+newlen>parent->default_name_space_total+oldlen)
		return 1;
	default_name = newvalue;
	parent->default_name_space_used += newlen-oldlen;
	return 0;
}

uint8_t character_equip_set[EQUIP_SET_AMOUNT];
uint8_t* StatDataSet::GetCharacterEquipmentsId(int charid, unsigned int* amount) {
	character_equip_set[0] = charid;
	if (charid<9) {
		if (amount)
			*amount = 1;
	} else if (charid==9) {
		character_equip_set[1] = 12;
		if (amount)
			*amount = 2;
	} else if (charid==10) {
		character_equip_set[1] = 14;
		if (amount)
			*amount = 2;
	} else if (charid==11) {
		character_equip_set[1] = 13;
		if (amount)
			*amount = 2;
	}
	return character_equip_set;
}

uint8_t character_command_set[COMMAND_SET_AMOUNT];
uint8_t* StatDataSet::GetCharacterCommandsId(int charid, unsigned int* amount) {
	if (charid==0) {
		character_command_set[0] = 0;
		character_command_set[1] = 16;
		if (amount)
			*amount = 2;
	} else if (charid<8) {
		character_command_set[0] = charid;
		if (amount)
			*amount = 1;
	} else if (charid==8) {
		character_command_set[0] = 8;
		character_command_set[1] = 9;
		character_command_set[2] = 17;
		if (amount)
			*amount = 3;
	} else if (charid==9) {
		character_command_set[0] = 10;
		character_command_set[1] = 11;
		character_command_set[2] = 18;
		if (amount)
			*amount = 3;
	} else if (charid==10) {
		character_command_set[0] = 12;
		character_command_set[1] = 13;
		character_command_set[2] = 19;
		if (amount)
			*amount = 3;
	} else if (charid==11) {
		character_command_set[0] = 14;
		character_command_set[1] = 15;
		if (amount)
			*amount = 2;
	}
	return character_command_set;
}

#define MACRO_STAT_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	uint16_t zero16 = 0; \
	uint32_t txtpos; \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Short(ffbin,amount); \
	IO ## Short(ffbin,zero16); \
	txtpos = ffbin.tellg(); \
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) { \
		IO ## Short(ffbin,initial_stat[i].default_name_offset); \
		IO ## Short(ffbin,initial_stat[i].default_name_size_x); \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,0); \
	if (READ) default_name_space_used = PLAYABLE_CHAR_AMOUNT*4; \
	if (PPF) PPFInitScanStep(ffbin,true,default_name_space_total); \
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,initial_stat[i].default_name_offset); \
		IO ## FF9String(ffbin,initial_stat[i].default_name); \
		if (READ) default_name_space_used += initial_stat[i].default_name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,default_name_space_total);

#define MACRO_STAT_IOFUNCTIONEQUIP(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<EQUIP_SET_AMOUNT;i++) { \
		IO ## Char(ffbin,initial_equip[i].weapon); \
		IO ## Char(ffbin,initial_equip[i].head); \
		IO ## Char(ffbin,initial_equip[i].wrist); \
		IO ## Char(ffbin,initial_equip[i].armor); \
		IO ## Char(ffbin,initial_equip[i].accessory); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONABIL(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ABILITY_SET_AMOUNT;i++) { \
		for (j=0;j<ABILITY_SET_CAPACITY;j++) { \
			IO ## Char(ffbin,ability_list[i].ability[j]); \
			IO ## Char(ffbin,ability_list[i].ap_cost[j]); \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONSTAT(IO,SEEK,READ,PPF) \
	uint8_t zero8 = 0; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) { \
		IO ## Char(ffbin,initial_stat[i].speed); \
		IO ## Char(ffbin,initial_stat[i].strength); \
		IO ## Char(ffbin,initial_stat[i].magic); \
		IO ## Char(ffbin,initial_stat[i].spirit); \
		IO ## Char(ffbin,initial_stat[i].magic_stone); \
		IO ## Char(ffbin,zero8); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONLEVEL(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<MAX_LEVEL;i++) { \
		IO ## Short(ffbin,level[i].hp_table); \
		IO ## Short(ffbin,level[i].mp_table); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONLEVELEXP(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<MAX_LEVEL;i++) { \
		IO ## Long(ffbin,level[i].exp_table); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	IO ## Char(ffbin,shared_command_attack); \
	IO ## Char(ffbin,shared_command_2); \
	IO ## Char(ffbin,shared_command_3); \
	IO ## Char(ffbin,shared_command_item); \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONCOMMAND(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<COMMAND_SET_AMOUNT;i++) { \
		IO ## Char(ffbin,command_list[i].first_command); \
		IO ## Char(ffbin,command_list[i].second_command); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_STAT_IOFUNCTIONTRANCECOMMAND(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<COMMAND_SET_AMOUNT;i++) { \
		IO ## Char(ffbin,command_list[i].first_command_trance); \
		IO ## Char(ffbin,command_list[i].second_command_trance); \
		IO ## Char(ffbin,command_list[i].trance_attack); \
	} \
	if (PPF) PPFEndScanStep();


void StatDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	default_name_space_total = config.stat_defaultname_space_total;
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++)
		initial_stat[i].parent = this;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.stat_defaultname_offset);
		MACRO_STAT_IOFUNCTIONNAME(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.stat_initialequip_offset);
		MACRO_STAT_IOFUNCTIONEQUIP(FFIXRead,FFIXSeek,true,false)
		MACRO_STAT_IOFUNCTIONABIL(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.stat_initialstat_offset);
		MACRO_STAT_IOFUNCTIONSTAT(FFIXRead,FFIXSeek,true,false)
		MACRO_STAT_IOFUNCTIONLEVEL(FFIXRead,FFIXSeek,true,false)
		MACRO_STAT_IOFUNCTIONLEVELEXP(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.stat_cmddata_offset[0]);
		MACRO_STAT_IOFUNCTIONCOMMAND(FFIXRead,FFIXSeek,true,false)
		MACRO_STAT_IOFUNCTIONTRANCECOMMAND(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.stat_cmddata_offset[2]);
		MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(FFIXRead,FFIXSeek,true,false)
	} else {
		for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) { // Steam default names are in Assembly-CSharp.dll, FF9TextTool's script...
			initial_stat[i].default_name.CreateEmpty();
			initial_stat[i].default_name.SetValue(HADES_STRING_CHARACTER_DEFAULT_NAME[i]);
		}
		DllMethodInfo methinfo;
		string fname = config.steam_dir_managed;
		fname += "Assembly-CSharp.dll";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<EQUIP_SET_AMOUNT;i++) {
			ffbin.seekg(config.meta_dll.GetStaticFieldOffset(config.dll_equipset_field_id[i]));
			SteamReadChar(ffbin,initial_equip[i].weapon);
			SteamReadChar(ffbin,initial_equip[i].head);
			SteamReadChar(ffbin,initial_equip[i].wrist);
			SteamReadChar(ffbin,initial_equip[i].armor);
			SteamReadChar(ffbin,initial_equip[i].accessory);
		}
		ffbin.seekg(config.meta_dll.GetMethodOffset(config.dll_ability_method_id));
		methinfo.ReadMethodInfo(ffbin);
		ILInstruction initinstabil[3] = {
			{ 0x1F, ABILITY_SET_CAPACITY },
			{ 0x8D, config.meta_dll.GetTypeTokenIdentifier("PA_DATA","FF9") },
			{ 0x25 }
		};
		uint8_t* rawabildata[ABILITY_SET_AMOUNT];
		for (i=0;i<ABILITY_SET_AMOUNT;i++) {
			methinfo.JumpToInstructions(ffbin,3,initinstabil);
			steam_method_position_abilset[i] = ffbin.tellg();
			rawabildata[i] = ConvertILScriptToRawData_Object(ffbin,ABILITY_SET_CAPACITY,2,steam_statabil_field_size);
			steam_method_base_length_abilset[i] = (unsigned int)ffbin.tellg()-steam_method_position_abilset[i];
		}
		ffbin.seekg(config.meta_dll.GetMethodOffset(config.dll_level_method_id));
		methinfo.ReadMethodInfo(ffbin);
		ILInstruction initinststat[3] = {
			{ 0x1F, PLAYABLE_CHAR_AMOUNT },
			{ 0x8D, config.meta_dll.GetTypeTokenIdentifier("FF9LEVEL_BASE") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(ffbin,3,initinststat);
		steam_method_position_initstat = ffbin.tellg();
		uint8_t* rawstatdata = ConvertILScriptToRawData_Object(ffbin,PLAYABLE_CHAR_AMOUNT,5,steam_stat_field_size);
		steam_method_base_length_initstat = (unsigned int)ffbin.tellg()-steam_method_position_initstat;
		ffbin.seekg(config.meta_dll.GetMethodOffset(config.dll_level_method_id));
		methinfo.ReadMethodInfo(ffbin);
		ILInstruction initinsthpmp[3] = {
			{ 0x1F, MAX_LEVEL },
			{ 0x8D, config.meta_dll.GetTypeTokenIdentifier("FF9LEVEL_HPMP") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(ffbin,3,initinsthpmp);
		steam_method_position_hpmp = ffbin.tellg();
		uint8_t* rawlvldata = ConvertILScriptToRawData_Object(ffbin,MAX_LEVEL,2,steam_lvlhpmp_field_size);
		steam_method_base_length_hpmp = (unsigned int)ffbin.tellg()-steam_method_position_hpmp;
		ffbin.seekg(config.meta_dll.GetStaticFieldOffset(config.dll_statexp_field_id));
		for (i=0;i<MAX_LEVEL;i++) {
			SteamReadLong(ffbin,level[i].exp_table);
			ffbin.seekg(4,ios::cur);
		}
		// ToDo : shared_commands
		ffbin.seekg(config.meta_dll.GetMethodOffset(config.dll_rdata_method_id));
		methinfo.ReadMethodInfo(ffbin);
		ILInstruction initinstcmd[4] = {
			{ 0x1F, COMMAND_SET_AMOUNT },
			{ 0x18 },
			{ 0x73, config.meta_dll.GetMemberTokenIdentifier("void valuetype FF9.command_tags[,]::.ctor( int, int )") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(ffbin,4,initinstcmd);
		steam_method_position_cmdset = ffbin.tellg();
		int64_t** rawcmddata = ConvertILScriptToRawData_Array2(ffbin,COMMAND_SET_AMOUNT,2);
		steam_method_base_length_cmdset = (unsigned int)ffbin.tellg()-steam_method_position_cmdset;
		for (i=0;i<COMMAND_SET_AMOUNT;i++) {
			command_list[i].first_command = rawcmddata[i][0];
			command_list[i].second_command = rawcmddata[i][1];
			delete[] rawcmddata[i];
		}
		delete[] rawcmddata;
		ffbin.seekg(config.meta_dll.GetStaticFieldOffset(config.dll_statcmdtrance_field_id));
		MACRO_STAT_IOFUNCTIONTRANCECOMMAND(SteamRead,SteamSeek,true,false)
		// ToDo : Update some "trance_attack" with ff9com.FF9COMMAND_TRANCE_NONE
		ffbin.close();
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		for (i=0;i<ABILITY_SET_AMOUNT;i++)
			ffbin.write((const char*)rawabildata[i],2*ABILITY_SET_CAPACITY);
		ffbin.write((const char*)rawstatdata,6*PLAYABLE_CHAR_AMOUNT);
		ffbin.write((const char*)rawlvldata,4*MAX_LEVEL);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_STAT_IOFUNCTIONABIL(SteamRead,SteamSeek,true,false)
		MACRO_STAT_IOFUNCTIONSTAT(SteamRead,SteamSeek,true,false)
		MACRO_STAT_IOFUNCTIONLEVEL(SteamRead,SteamSeek,true,false)
		ffbin.close();
		remove(fname.c_str());
		for (i=0;i<ABILITY_SET_AMOUNT;i++)
			delete[] rawabildata[i];
		delete[] rawstatdata;
		delete[] rawlvldata;
		default_name_space_used = 0;
	}
}

DllMetaDataModification* StatDataSet::ComputeSteamMod(fstream& ffbinbase, ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[EQUIP_SET_AMOUNT+ABILITY_SET_AMOUNT+5];
	unsigned int i,j,modcounter = 0;
	uint32_t** argvalue;
	for (i=0;i<EQUIP_SET_AMOUNT;i++) {
		res[modcounter].position = config.meta_dll.GetStaticFieldOffset(config.dll_equipset_field_id[i]);
		res[modcounter].base_length = 5;
		res[modcounter].new_length = 5;
		res[modcounter].value = new uint8_t[res[modcounter].new_length];
		res[modcounter].value[0] = initial_equip[i].weapon;
		res[modcounter].value[1] = initial_equip[i].head;
		res[modcounter].value[2] = initial_equip[i].wrist;
		res[modcounter].value[3] = initial_equip[i].armor;
		res[modcounter].value[4] = initial_equip[i].accessory;
		modcounter++;
	}
	argvalue = new uint32_t*[ABILITY_SET_CAPACITY];
	for (j=0;j<ABILITY_SET_CAPACITY;j++)
		argvalue[j] = new uint32_t[2];
	for (i=0;i<ABILITY_SET_AMOUNT;i++) {
		for (j=0;j<ABILITY_SET_CAPACITY;j++) {
			argvalue[j][0] = ability_list[i].ability[j];
			argvalue[j][1] = ability_list[i].ap_cost[j];
		}
		res[modcounter] = ModifyILScript_Object(ffbinbase,argvalue,steam_method_position_abilset[i],steam_method_base_length_abilset[i],ABILITY_SET_CAPACITY,2,steam_statabil_field_size);
		modcounter++;
	}
	for (j=0;j<ABILITY_SET_CAPACITY;j++)
		delete[] argvalue[j];
	delete[] argvalue;
	res[modcounter].position = config.meta_dll.GetStaticFieldOffset(config.dll_statcmdtrance_field_id);
	res[modcounter].base_length = 3*COMMAND_SET_AMOUNT;
	res[modcounter].new_length = 3*COMMAND_SET_AMOUNT;
	res[modcounter].value = new uint8_t[res[modcounter].new_length];
	for (i=0;i<COMMAND_SET_AMOUNT;i++) {
		res[modcounter].value[i*3] = command_list[i].first_command_trance;
		res[modcounter].value[i*3+1] = command_list[i].second_command_trance;
		res[modcounter].value[i*3+2] = command_list[i].trance_attack;
	}
	modcounter++;
	res[modcounter].position = config.meta_dll.GetStaticFieldOffset(config.dll_statexp_field_id);
	res[modcounter].base_length = 8*MAX_LEVEL;
	res[modcounter].new_length = 8*MAX_LEVEL;
	res[modcounter].value = new uint8_t[res[modcounter].new_length];
	BufferInitPosition();
	for (i=0;i<MAX_LEVEL;i++)
		BufferWriteLongLong(res[modcounter].value,level[i].exp_table);
	modcounter++;
	argvalue = new uint32_t*[PLAYABLE_CHAR_AMOUNT];
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) {
		argvalue[i] = new uint32_t[5];
		argvalue[i][0] = initial_stat[i].speed;
		argvalue[i][1] = initial_stat[i].strength;
		argvalue[i][2] = initial_stat[i].magic;
		argvalue[i][3] = initial_stat[i].spirit;
		argvalue[i][4] = initial_stat[i].magic_stone;
	}
	res[modcounter] = ModifyILScript_Object(ffbinbase,argvalue,steam_method_position_initstat,steam_method_base_length_initstat,PLAYABLE_CHAR_AMOUNT,5,steam_stat_field_size);
	for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	modcounter++;
	argvalue = new uint32_t*[MAX_LEVEL];
	for (i=0;i<MAX_LEVEL;i++) {
		argvalue[i] = new uint32_t[2];
		argvalue[i][0] = level[i].hp_table;
		argvalue[i][1] = level[i].mp_table;
	}
	res[modcounter] = ModifyILScript_Object(ffbinbase,argvalue,steam_method_position_hpmp,steam_method_base_length_hpmp,MAX_LEVEL,2,steam_lvlhpmp_field_size);
	for (i=0;i<MAX_LEVEL;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	modcounter++;
	argvalue = new uint32_t*[COMMAND_SET_AMOUNT];
	for (i=0;i<COMMAND_SET_AMOUNT;i++) {
		argvalue[i] = new uint32_t[2];
		argvalue[i][0] = command_list[i].first_command;
		argvalue[i][1] = command_list[i].second_command;
	}
	res[modcounter] = ModifyILScript_Array2(ffbinbase,argvalue,steam_method_position_cmdset,steam_method_base_length_cmdset,COMMAND_SET_AMOUNT,2);
	for (i=0;i<COMMAND_SET_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	modcounter++;
	*modifamount = EQUIP_SET_AMOUNT+ABILITY_SET_AMOUNT+5;
	return res;
}

void StatDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	UpdateOffset();
	ffbin.seekg(config.stat_defaultname_offset);
	MACRO_STAT_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_initialequip_offset);
	MACRO_STAT_IOFUNCTIONEQUIP(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONABIL(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_initialstat_offset);
	MACRO_STAT_IOFUNCTIONSTAT(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONLEVEL(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONLEVELEXP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_cmddata_offset[0]);
	MACRO_STAT_IOFUNCTIONCOMMAND(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONTRANCECOMMAND(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_cmddata_offset[1]);
	MACRO_STAT_IOFUNCTIONCOMMAND(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.stat_cmddata_offset[2]);
	MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(FFIXWrite,FFIXSeek,false,false)
	MACRO_STAT_IOFUNCTIONCOMMAND(FFIXWrite,FFIXSeek,false,false)
}

void StatDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i,j;
	UpdateOffset();
	ffbin.seekg(config.stat_defaultname_offset);
	MACRO_STAT_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_initialequip_offset);
	MACRO_STAT_IOFUNCTIONEQUIP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONABIL(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_initialstat_offset);
	MACRO_STAT_IOFUNCTIONSTAT(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONLEVEL(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONLEVELEXP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_cmddata_offset[0]);
	MACRO_STAT_IOFUNCTIONCOMMAND(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONTRANCECOMMAND(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_cmddata_offset[1]);
	MACRO_STAT_IOFUNCTIONCOMMAND(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.stat_cmddata_offset[2]);
	MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(PPFStepAdd,FFIXSeek,false,true)
	MACRO_STAT_IOFUNCTIONCOMMAND(PPFStepAdd,FFIXSeek,false,true)
}

int StatDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	unsigned int i,j;
	int res = 0;
	uint16_t version;
	uint16_t defnamesize = default_name_space_total;
	HWSReadShort(ffbin,version);
	HWSReadShort(ffbin,default_name_space_total);
	if (default_name_space_total<=defnamesize && usetext && GetGameType()==GAME_TYPE_PSX) { // DEBUG : character name can't be modified
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			MACRO_STAT_IOFUNCTIONNAME(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++)
					initial_stat[i].default_name.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=0xFF) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
/*				if (GetGameType()!=GAME_TYPE_PSX && lg==GetSteamLanguage()) { // DEBUG : need to make Steam language compatible with PSX versions
					for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) {
						SteamReadFF9String(ffbin,initial_stat[i].default_name);
						if (GetGameType()==GAME_TYPE_PSX)
							initial_stat[i].default_name.SteamToPSX();
					}
				}*/
				ffbin.seekg(tmppos+txtspace);
				HWSReadChar(ffbin,lg);
			}
		}
	} else {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			ffbin.seekg(default_name_space_total+4,ios::cur);
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
		if (usetext && GetGameType()==GAME_TYPE_PSX)
			res |= 1;
	}
	MACRO_STAT_IOFUNCTIONEQUIP(HWSRead,HWSSeek,true,false)
	MACRO_STAT_IOFUNCTIONABIL(HWSRead,HWSSeek,true,false)
	MACRO_STAT_IOFUNCTIONSTAT(HWSRead,HWSSeek,true,false)
	MACRO_STAT_IOFUNCTIONLEVEL(HWSRead,HWSSeek,true,false)
	MACRO_STAT_IOFUNCTIONLEVELEXP(HWSRead,HWSSeek,true,false)
	if (version>=2) {
		MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(HWSRead,HWSSeek,true,false)
		MACRO_STAT_IOFUNCTIONCOMMAND(HWSRead,HWSSeek,true,false)
		MACRO_STAT_IOFUNCTIONTRANCECOMMAND(HWSRead,HWSSeek,true,false)
	}
	default_name_space_total = defnamesize;
	UpdateOffset();
	return res;
}

void StatDataSet::WriteHWS(fstream& ffbin) {
	unsigned int i,j;
	UpdateOffset();
	HWSWriteShort(ffbin,STAT_HWS_VERSION);
	uint16_t defnamesize = default_name_space_total;
	default_name_space_total = default_name_space_used;
	HWSWriteShort(ffbin,default_name_space_total);
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_STAT_IOFUNCTIONNAME(HWSWrite,HWSSeek,false,false)
	} else {
		SteamLanguage lg = GetSteamLanguage();
/*		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,default_name_space_total);
		for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++)
			SteamWriteFF9String(ffbin,initial_stat[i].default_name);*/
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
	}
	MACRO_STAT_IOFUNCTIONEQUIP(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONABIL(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONSTAT(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONLEVEL(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONLEVELEXP(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONSHAREDCOMMAND(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONCOMMAND(HWSWrite,HWSSeek,false,false)
	MACRO_STAT_IOFUNCTIONTRANCECOMMAND(HWSWrite,HWSSeek,false,false)
	default_name_space_total = defnamesize;
}

void StatDataSet::UpdateOffset() {
	uint16_t j;
	unsigned int i;
	if (GetGameType()==GAME_TYPE_PSX) {
		j = PLAYABLE_CHAR_AMOUNT*4;
		for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++) {
			initial_stat[i].default_name_offset = j;
			j += initial_stat[i].default_name.length;
		}
		default_name_space_used = j;
	} else {
		j = 0;
		for (i=0;i<PLAYABLE_CHAR_AMOUNT;i++)
			j += initial_stat[i].default_name.length;
		default_name_space_used = j;
	}
}
