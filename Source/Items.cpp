#include "Items.h"

#include "DllEditor.h"

#define ITEM_HWS_VERSION 1

const unsigned int steam_item_field_size[] = { 16, 16, 16, 16, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
const unsigned int steam_item_field_array[] = { 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0 };
const unsigned int steam_usable_field_size[] = { 4, 1, 3, 9, 12, 1, 1, 1, 8, 8, 8, 8, 32 };
const unsigned int steam_armor_field_size[] = { 8, 8, 8, 8 };
const unsigned int steam_stat_field_size[] = { 8, 8, 8, 8, 8, 8, 8, 8, 32 };
const unsigned int steam_weapon_field_size[] = { 8, 8, 16, 8, 8, 8, 8, 16, 16 };
const unsigned int steam_weapon_field_array[] = { 0, 0, 0, 0, 0, 0, 0, 2, 0 };

int ItemDataStruct::SetName(wstring newvalue) {
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

int ItemDataStruct::SetName(FF9String& newvalue) {
	int oldlen = name.length;
	int newlen = newvalue.length;
	if (parent->name_space_used+newlen>parent->name_space_total+oldlen)
		return 1;
	name = newvalue;
	parent->name_space_used += newlen-oldlen;
	return 0;
}

int ItemDataStruct::SetHelp(wstring newvalue) {
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

int ItemDataStruct::SetHelp(FF9String& newvalue) {
	int oldlen = help.length;
	int newlen = newvalue.length;
	if (parent->help_space_used+newlen>parent->help_space_total+oldlen)
		return 1;
	help = newvalue;
	parent->help_space_used += newlen-oldlen;
	return 0;
}

int ItemDataStruct::SetBattleHelp(wstring newvalue) {
	FF9String tmp(battle_help);
	tmp.SetValue(newvalue);
	int oldlen = battle_help.length;
	int newlen = tmp.length;
	if (parent->help2_space_used+newlen>parent->help2_space_total+oldlen)
		return 1;
	battle_help.SetValue(newvalue);
	parent->help2_space_used += newlen-oldlen;
	return 0;
}

int ItemDataStruct::SetBattleHelp(FF9String& newvalue) {
	int oldlen = battle_help.length;
	int newlen = newvalue.length;
	if (parent->help2_space_used+newlen>parent->help2_space_total+oldlen)
		return 1;
	battle_help = newvalue;
	parent->help2_space_used += newlen-oldlen;
	return 0;
}

int KeyItemDataStruct::SetName(wstring newvalue) {
	FF9String tmp(name);
	tmp.SetValue(newvalue);
	int oldlen = name.length;
	int newlen = tmp.length;
	if (parent->key_name_space_used+newlen>parent->key_name_space_total+oldlen)
		return 1;
	name.SetValue(newvalue);
	parent->key_name_space_used += newlen-oldlen;
	return 0;
}

int KeyItemDataStruct::SetName(FF9String& newvalue) {
	int oldlen = name.length;
	int newlen = newvalue.length;
	if (parent->key_name_space_used+newlen>parent->key_name_space_total+oldlen)
		return 1;
	name = newvalue;
	parent->key_name_space_used += newlen-oldlen;
	return 0;
}

int KeyItemDataStruct::SetHelp(wstring newvalue) {
	FF9String tmp(help);
	tmp.SetValue(newvalue);
	int oldlen = help.length;
	int newlen = tmp.length;
	if (parent->key_help_space_used+newlen>parent->key_help_space_total+oldlen)
		return 1;
	help.SetValue(newvalue);
	parent->key_help_space_used += newlen-oldlen;
	return 0;
}

int KeyItemDataStruct::SetHelp(FF9String& newvalue) {
	int oldlen = help.length;
	int newlen = newvalue.length;
	if (parent->key_help_space_used+newlen>parent->key_help_space_total+oldlen)
		return 1;
	help = newvalue;
	parent->key_help_space_used += newlen-oldlen;
	return 0;
}

int KeyItemDataStruct::SetDescription(wstring newvalue) {
	if (description_out_of_bounds)
		return 2;
	FF9String tmp(description);
	tmp.SetValue(newvalue);
	int oldlen = description.length;
	int newlen = tmp.length;
	if (parent->key_desc_space_used+newlen>parent->key_desc_space_total+oldlen)
		return 1;
	description.SetValue(newvalue);
	parent->key_desc_space_used += newlen-oldlen;
	return 0;
}

int KeyItemDataStruct::SetDescription(FF9String& newvalue) {
	if (description_out_of_bounds)
		return 2;
	int oldlen = description.length;
	int newlen = newvalue.length;
	if (parent->key_desc_space_used+newlen>parent->key_desc_space_total+oldlen)
		return 1;
	description = newvalue;
	parent->key_desc_space_used += newlen-oldlen;
	return 0;
}

Spell_Panel ItemUsableDataStruct::GetPanel() {
	return (target_type >> 5) & 0x7;
}

void ItemUsableDataStruct::SetPanel(Spell_Panel newvalue) {
	target_type = (target_type & 0x1F) + (newvalue << 5);
}

Spell_Target_Priority ItemUsableDataStruct::GetTargetPriority() {
	return (target_type >> 4) & 0x1;
}

void ItemUsableDataStruct::SetTargetPriority(Spell_Target_Priority newvalue) {
	target_type = (target_type & 0xEF) + (newvalue << 4);
}

Spell_Target_Type ItemUsableDataStruct::GetTargetType() {
	uint8_t val = target_type & 0xF;
	if (val==0xC)
		return SPELL_TARGET_TYPE_EVERYONE;
	if (val==0xD)
		return SPELL_TARGET_TYPE_SELF;
	if (val==0x0 || val==0x3 || val==0x6 || val==0x9)
		return SPELL_TARGET_TYPE_ANY;
	if (val==0x1 || val==0x4 || val==0x7 || val==0xA)
		return SPELL_TARGET_TYPE_ALLY;
	return SPELL_TARGET_TYPE_ENEMY;
}

void ItemUsableDataStruct::SetTargetType(Spell_Target_Type newvalue) {
	uint8_t ta = target_type & 0xF;
	ta = ta>=0xC ? 0 : ta/3;
	if (newvalue==SPELL_TARGET_TYPE_EVERYONE)
		target_type = (target_type & 0xF0) + 0xC;
	else if (newvalue==SPELL_TARGET_TYPE_SELF)
		target_type = (target_type & 0xE0) + 0x10 + 0xD;
	else if (newvalue==SPELL_TARGET_TYPE_ANY)
		target_type = (target_type & 0xF0) + 3*ta;
	else if (newvalue==SPELL_TARGET_TYPE_ALLY)
		target_type = (target_type & 0xE0) + 0x10 + 3*ta + 1;
	else if (newvalue==SPELL_TARGET_TYPE_ENEMY)
		target_type = (target_type & 0xE0) + 3*ta + 2;
}

Spell_Target_Amount ItemUsableDataStruct::GetTargetAmount() {
	uint8_t val = target_type & 0xF;
	if (val<0x3 || val >0xC)
		return SPELL_TARGET_AMOUNT_ONE;
	if (val<6)
		return SPELL_TARGET_AMOUNT_VARIABLE;
	return SPELL_TARGET_AMOUNT_GROUP;
}

void ItemUsableDataStruct::SetTargetAmount(Spell_Target_Amount newvalue) {
	uint8_t tt = target_type & 0xF;
	if (newvalue==SPELL_TARGET_AMOUNT_ONE)
		target_type = (target_type & 0xF0) + tt%3;
	else if (newvalue==SPELL_TARGET_AMOUNT_VARIABLE)
		target_type = (target_type & 0xF0) + 0x3 + tt%3;
	else if (newvalue==SPELL_TARGET_AMOUNT_GROUP)
		target_type = (target_type & 0xF0) + 0x6 + tt%3;
}

uint16_t ItemUsableDataStruct::GetSound() {
	return (model >> 9) | ((target_flag & 0x1F) << 7);
}

void ItemUsableDataStruct::SetSound(uint16_t newvalue) {
	model = (model & 0x1FF) | (newvalue & 0x7F << 9);
	target_flag = (target_flag & 0xE0) | ((newvalue & 0xF80) >> 7);
}

#define MACRO_ITEM_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	uint8_t zero8 = 0; \
	uint16_t zero16 = 0; \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ITEM_AMOUNT;i++) { \
		IO ## Short(ffbin,item[i].name_offset); \
		IO ## Short(ffbin,item[i].help_offset); \
		IO ## Short(ffbin,item[i].price); \
		IO ## Short(ffbin,item[i].char_availability); \
		IO ## Char(ffbin,item[i].icon); \
		IO ## Char(ffbin,item[i].icon_color); \
		IO ## Char(ffbin,item[i].equip_position); \
		IO ## Char(ffbin,item[i].stat_id); \
		IO ## Char(ffbin,item[i].skill[0]); \
		IO ## Char(ffbin,item[i].skill[1]); \
		IO ## Char(ffbin,item[i].skill[2]); \
		IO ## Char(ffbin,item[i].type); \
		IO ## Char(ffbin,item[i].menu_position); \
		IO ## Char(ffbin,zero8); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONNAME(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<ITEM_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,item[i].name_offset); \
		IO ## FF9String(ffbin,item[i].name); \
		if (READ) name_space_used += item[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,name_space_total); \
	txtpos = ffbin.tellg(); \
	SEEK(ffbin,txtpos,key_name_space_total);

#define MACRO_ITEM_IOFUNCTIONUSABLE(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ITEM_USABLE_AMOUNT;i++) { \
		IO ## Char(ffbin,usable[i].target_type); \
		IO ## Short(ffbin,usable[i].model); \
		IO ## Char(ffbin,usable[i].target_flag); \
		IO ## Char(ffbin,usable[i].effect); \
		IO ## Char(ffbin,usable[i].power); \
		IO ## Char(ffbin,usable[i].element); \
		IO ## Char(ffbin,usable[i].accuracy); \
		IO ## Long(ffbin,usable[i].status); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONKEY(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<KEY_ITEM_AMOUNT;i++) { \
		IO ## Short(ffbin,key_item[i].name_offset); \
		IO ## Short(ffbin,key_item[i].help_offset); \
		IO ## Short(ffbin,key_item[i].help_size_x); \
		IO ## Short(ffbin,key_item[i].description_offset); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONKEYNAME(IO,SEEK,READ,PPF) \
	SEEK(ffbin,txtpos,0); \
	if (READ) key_name_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,key_name_space_total); \
	for (i=0;i<KEY_ITEM_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,key_item[i].name_offset); \
		IO ## FF9String(ffbin,key_item[i].name); \
		if (READ) key_name_space_used += key_item[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,key_name_space_total+ITEM_USABLE_AMOUNT*12+KEY_ITEM_AMOUNT*8);

#define MACRO_ITEM_IOFUNCTIONARMOR(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ITEM_ARMOR_AMOUNT;i++) { \
		IO ## Char(ffbin,armor[i].defence); \
		IO ## Char(ffbin,armor[i].evade); \
		IO ## Char(ffbin,armor[i].magic_defence); \
		IO ## Char(ffbin,armor[i].magic_evade); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONSTAT(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ITEM_STAT_AMOUNT;i++) { \
		IO ## Char(ffbin,stat[i].speed); \
		IO ## Char(ffbin,stat[i].strength); \
		IO ## Char(ffbin,stat[i].magic); \
		IO ## Char(ffbin,stat[i].spirit); \
		IO ## Char(ffbin,stat[i].element_immune); \
		IO ## Char(ffbin,stat[i].element_absorb); \
		IO ## Char(ffbin,stat[i].element_half); \
		IO ## Char(ffbin,stat[i].element_weak); \
		IO ## Char(ffbin,stat[i].element_boost); \
		IO ## Char(ffbin,zero8); \
		IO ## Short(ffbin,zero16); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONWEAPONSTAT(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ITEM_WEAPON_AMOUNT;i++) { \
		IO ## Char(ffbin,weapon[i].flag); \
		IO ## Char(ffbin,weapon[i].status); \
		IO ## Short(ffbin,weapon[i].model); \
		IO ## Char(ffbin,weapon[i].damage_formula); \
		IO ## Char(ffbin,weapon[i].power); \
		IO ## Char(ffbin,weapon[i].element); \
		IO ## Char(ffbin,weapon[i].status_accuracy); \
		IO ## Short(ffbin,(uint16_t&)weapon[i].offset1); \
		IO ## Short(ffbin,(uint16_t&)weapon[i].offset2); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_ITEM_IOFUNCTIONBATTLEHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	SEEK(ffbin,txtpos,help2_space_total); \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<ITEM_AMOUNT;i++) \
		IO ## Short(ffbin,item[i].battle_help_offset); \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,0); \
	if (READ) help2_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,help2_space_total); \
	for (i=0;i<ITEM_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,item[i].battle_help_offset); \
		IO ## FF9String(ffbin,item[i].battle_help); \
		if (READ) help2_space_used += item[i].battle_help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help2_space_total+2*ITEM_AMOUNT);

#define MACRO_ITEM_IOFUNCTIONHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) help_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,help_space_total); \
	for (i=0;i<ITEM_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,item[i].help_offset); \
		IO ## FF9String(ffbin,item[i].help); \
		if (READ) help_space_used += item[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total);

#define MACRO_ITEM_IOFUNCTIONKEYHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) key_help_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,key_help_space_total); \
	for (i=0;i<KEY_ITEM_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,key_item[i].help_offset); \
		IO ## FF9String(ffbin,key_item[i].help); \
		if (READ) key_help_space_used += key_item[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,key_help_space_total);

#define MACRO_ITEM_IOFUNCTIONKEYDESC(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	if (READ) key_desc_space_used = 0; \
	if (PPF) PPFInitScanStep(ffbin,true,key_desc_space_total); \
	for (i=0;i<KEY_ITEM_AMOUNT;i++) { \
		if (key_item[i].description_offset<key_desc_space_total) { \
			SEEK(ffbin,txtpos,key_item[i].description_offset); \
			IO ## FF9String(ffbin,key_item[i].description); \
			if (READ) { \
				key_desc_space_used += key_item[i].description.length; \
				key_item[i].description_out_of_bounds = false; \
			} \
		} else if (READ) { \
			key_item[i].description.CreateEmpty(); \
			key_item[i].description_out_of_bounds = true; \
		} \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,key_desc_space_total);


void ItemDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	uint32_t txtpos;
	name_space_total = config.item_name_space_total;
	help_space_total = config.item_help_space_total;
	help2_space_total = config.item_help_space2_total;
	key_name_space_total = config.item_key_name_space_total;
	key_help_space_total = config.item_key_help_space_total;
	key_desc_space_total = config.item_key_desc_space_total;
	for (i=0;i<ITEM_AMOUNT;i++)
		item[i].parent = this;
	for (i=0;i<KEY_ITEM_AMOUNT;i++)
		key_item[i].parent = this;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.item_data_offset);
		MACRO_ITEM_IOFUNCTIONDATA(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONNAME(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONUSABLE(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEY(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEYNAME(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_stat_offset);
		MACRO_ITEM_IOFUNCTIONARMOR(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONSTAT(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_weaponstat_offset);
		MACRO_ITEM_IOFUNCTIONWEAPONSTAT(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_help_offset[0]);
		MACRO_ITEM_IOFUNCTIONBATTLEHELP(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.item_help_offset[2]);
		MACRO_ITEM_IOFUNCTIONHELP(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEYHELP(FFIXRead,FFIXSeek,true,false)
		MACRO_ITEM_IOFUNCTIONKEYDESC(FFIXRead,FFIXSeek,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.item_name_file[GetSteamLanguage()]));
		name_space_used = config.meta_res.GetFileSizeByIndex(config.item_name_file[GetSteamLanguage()]);
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamReadFF9String(ffbin,item[i].name);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.item_help_file[GetSteamLanguage()]));
		help_space_used = config.meta_res.GetFileSizeByIndex(config.item_help_file[GetSteamLanguage()]);
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamReadFF9String(ffbin,item[i].help);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.item_help2_file[GetSteamLanguage()]));
		help2_space_used = config.meta_res.GetFileSizeByIndex(config.item_help2_file[GetSteamLanguage()]);
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamReadFF9String(ffbin,item[i].battle_help);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.itemkey_name_file[GetSteamLanguage()]));
		key_name_space_used = config.meta_res.GetFileSizeByIndex(config.itemkey_name_file[GetSteamLanguage()]);
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamReadFF9String(ffbin,key_item[i].name);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.itemkey_help_file[GetSteamLanguage()]));
		key_help_space_used = config.meta_res.GetFileSizeByIndex(config.itemkey_help_file[GetSteamLanguage()]);
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamReadFF9String(ffbin,key_item[i].help);
		ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.itemkey_desc_file[GetSteamLanguage()]));
		key_desc_space_used = config.meta_res.GetFileSizeByIndex(config.itemkey_desc_file[GetSteamLanguage()]);
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamReadFF9String(ffbin,key_item[i].description);
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_item_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstit[3] = {
			{ 0x20, ITEM_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("FF9ITEM_DATA") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstit);
		steam_method_position[0] = dlldata.dll_file.tellg();
		uint8_t* rawitemdata = dlldata.ConvertScriptToRaw_Object(ITEM_AMOUNT,14,steam_item_field_size,steam_item_field_array);
		steam_method_base_length[0] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[0];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_item_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstuse[3] = {
			{ 0x1F, ITEM_USABLE_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("ITEM_DATA","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstuse);
		steam_method_position[1] = dlldata.dll_file.tellg();
		uint8_t* rawusabledata = dlldata.ConvertScriptToRaw_Object(ITEM_USABLE_AMOUNT,13,steam_usable_field_size);
		steam_method_base_length[1] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[1];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_armor_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstarmor[3] = {
			{ 0x20, ITEM_ARMOR_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("DEF_PARAMS") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstarmor);
		steam_method_position[2] = dlldata.dll_file.tellg();
		uint8_t* rawarmordata = dlldata.ConvertScriptToRaw_Object(ITEM_ARMOR_AMOUNT,4,steam_armor_field_size);
		steam_method_base_length[2] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[2];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_equip_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinststat[3] = {
			{ 0x20, ITEM_STAT_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("EQUIP_PRIVILEGE","FF9") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinststat);
		steam_method_position[3] = dlldata.dll_file.tellg();
		uint8_t* rawitstatdata = dlldata.ConvertScriptToRaw_Object(ITEM_STAT_AMOUNT,9,steam_stat_field_size);
		steam_method_base_length[3] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[3];
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_weapon_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinstweap[3] = {
			{ 0x1F, ITEM_WEAPON_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("WEAPON") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinstweap);
		steam_method_position[4] = dlldata.dll_file.tellg();
		uint8_t* rawweapdata = dlldata.ConvertScriptToRaw_Object(ITEM_WEAPON_AMOUNT,9,steam_weapon_field_size,steam_weapon_field_array);
		steam_method_base_length[4] = (unsigned int)dlldata.dll_file.tellg()-steam_method_position[4];
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		ffbin.write((const char*)rawitemdata,0x12*ITEM_AMOUNT);
		ffbin.write((const char*)rawusabledata,0xC*ITEM_USABLE_AMOUNT);
		ffbin.write((const char*)rawarmordata,4*ITEM_ARMOR_AMOUNT);
		ffbin.write((const char*)rawitstatdata,0xC*ITEM_STAT_AMOUNT);
		ffbin.write((const char*)rawweapdata,0xC*ITEM_WEAPON_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_ITEM_IOFUNCTIONDATA(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONUSABLE(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONARMOR(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONSTAT(SteamRead,SteamSeek,true,false)
		MACRO_ITEM_IOFUNCTIONWEAPONSTAT(SteamRead,SteamSeek,true,false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawitemdata;
		delete[] rawusabledata;
		delete[] rawarmordata;
		delete[] rawitstatdata;
		delete[] rawweapdata;
	}
	for (i=0;i<ITEM_WEAPON_AMOUNT;i++) {
		item[i].weapon_id = i;
		item[i].armor_id = -1;
		item[i].usable_id = -1;
	}
	for (i=0;i<ITEM_ARMOR_AMOUNT;i++) {
		item[i+ITEM_WEAPON_AMOUNT].weapon_id = -1;
		item[i+ITEM_WEAPON_AMOUNT].armor_id = i;
		item[i+ITEM_WEAPON_AMOUNT].usable_id = -1;
	}
	for (i=0;i<ITEM_USABLE_AMOUNT;i++) {
		item[i+ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT].weapon_id = -1;
		item[i+ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT].armor_id = -1;
		item[i+ITEM_WEAPON_AMOUNT+ITEM_ARMOR_AMOUNT].usable_id = i;
	}
}

DllMetaDataModification* ItemDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[5];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t*[ITEM_AMOUNT];
	unsigned int i;
	for (i=0;i<ITEM_AMOUNT;i++) {
		argvalue[i] = new uint32_t[14];
		argvalue[i][0] = item[i].name_offset;
		argvalue[i][1] = item[i].help_offset;
		argvalue[i][2] = item[i].price;
		argvalue[i][3] = item[i].char_availability;
		argvalue[i][4] = item[i].icon;
		argvalue[i][5] = item[i].icon_color;
		argvalue[i][6] = item[i].equip_position;
		argvalue[i][7] = item[i].stat_id;
		argvalue[i][8] = item[i].skill[0];
		argvalue[i][9] = item[i].skill[1];
		argvalue[i][10] = item[i].skill[2];
		argvalue[i][11] = item[i].type;
		argvalue[i][12] = item[i].menu_position;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position[0],steam_method_base_length[0],ITEM_AMOUNT,14,steam_item_field_size,steam_item_field_array);
	for (i=0;i<ITEM_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_USABLE_AMOUNT];
	for (i=0;i<ITEM_USABLE_AMOUNT;i++) {
		argvalue[i] = new uint32_t[13];
		argvalue[i][0] = usable[i].target_type & 0xF;
		argvalue[i][1] = (usable[i].target_type >> 4) & 0x1;
		argvalue[i][2] = (usable[i].target_type >> 5) & 0x7;
		argvalue[i][3] = usable[i].model & 0x1FF;
		argvalue[i][4] = usable[i].GetSound();
		argvalue[i][5] = (usable[i].target_flag >> 5) & 0x1;
		argvalue[i][6] = (usable[i].target_flag >> 6) & 0x1;
		argvalue[i][7] = (usable[i].target_flag >> 7) & 0x1;
		argvalue[i][8] = usable[i].effect;
		argvalue[i][9] = usable[i].power;
		argvalue[i][10] = usable[i].element;
		argvalue[i][11] = usable[i].accuracy;
		argvalue[i][12] = usable[i].status;
	}
	res[1] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position[1],steam_method_base_length[1],ITEM_USABLE_AMOUNT,13,steam_usable_field_size);
	for (i=0;i<ITEM_USABLE_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_ARMOR_AMOUNT];
	for (i=0;i<ITEM_ARMOR_AMOUNT;i++) {
		argvalue[i] = new uint32_t[4];
		argvalue[i][0] = armor[i].defence;
		argvalue[i][1] = armor[i].evade;
		argvalue[i][2] = armor[i].magic_defence;
		argvalue[i][3] = armor[i].magic_evade;
	}
	res[2] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position[2],steam_method_base_length[2],ITEM_ARMOR_AMOUNT,4,steam_armor_field_size);
	for (i=0;i<ITEM_ARMOR_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_STAT_AMOUNT];
	for (i=0;i<ITEM_STAT_AMOUNT;i++) {
		argvalue[i] = new uint32_t[9];
		argvalue[i][0] = stat[i].speed;
		argvalue[i][1] = stat[i].strength;
		argvalue[i][2] = stat[i].magic;
		argvalue[i][3] = stat[i].spirit;
		argvalue[i][4] = stat[i].element_immune;
		argvalue[i][5] = stat[i].element_absorb;
		argvalue[i][6] = stat[i].element_half;
		argvalue[i][7] = stat[i].element_weak;
		argvalue[i][8] = stat[i].element_boost;
	}
	res[3] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position[3],steam_method_base_length[3],ITEM_STAT_AMOUNT,9,steam_stat_field_size);
	for (i=0;i<ITEM_STAT_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	argvalue = new uint32_t*[ITEM_WEAPON_AMOUNT];
	for (i=0;i<ITEM_WEAPON_AMOUNT;i++) {
		argvalue[i] = new uint32_t[9];
		argvalue[i][0] = weapon[i].flag;
		argvalue[i][1] = weapon[i].status;
		argvalue[i][2] = weapon[i].model;
		argvalue[i][3] = weapon[i].damage_formula;
		argvalue[i][4] = weapon[i].power;
		argvalue[i][5] = weapon[i].element;
		argvalue[i][6] = weapon[i].status_accuracy;
		argvalue[i][7] = (uint16_t)weapon[i].offset1;
		argvalue[i][8] = (uint16_t)weapon[i].offset2;
	}
	res[4] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position[4],steam_method_base_length[4],ITEM_WEAPON_AMOUNT,9,steam_weapon_field_size,steam_weapon_field_array);
	for (i=0;i<ITEM_WEAPON_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	*modifamount = 5;
	return res;
}

void ItemDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype) {
	unsigned int i;
	if (texttype==0) {
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,item[i].name);
	} else if (texttype==1) {
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,item[i].help);
	} else if (texttype==2) {
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,item[i].battle_help);
	} else if (texttype==3) {
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,key_item[i].name);
	} else if (texttype==4) {
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,key_item[i].help);
	} else {
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,key_item[i].description);
	}
}

void ItemDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	uint32_t txtpos;
	UpdateOffset();
	ffbin.seekg(config.item_data_offset);
	MACRO_ITEM_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONUSABLE(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEY(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEYNAME(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_stat_offset);
	MACRO_ITEM_IOFUNCTIONARMOR(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONSTAT(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_weaponstat_offset);
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[0]);
	MACRO_ITEM_IOFUNCTIONBATTLEHELP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[1]);
	MACRO_ITEM_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[2]);
	MACRO_ITEM_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEYHELP(FFIXWrite,FFIXSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEYDESC(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.item_help_offset[3]);
	MACRO_ITEM_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
}

void ItemDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	uint32_t txtpos;
	UpdateOffset();
	ffbin.seekg(config.item_data_offset);
	MACRO_ITEM_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONUSABLE(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEY(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEYNAME(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_stat_offset);
	MACRO_ITEM_IOFUNCTIONARMOR(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONSTAT(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_weaponstat_offset);
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[0]);
	MACRO_ITEM_IOFUNCTIONBATTLEHELP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[1]);
	MACRO_ITEM_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[2]);
	MACRO_ITEM_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEYHELP(PPFStepAdd,FFIXSeek,false,true)
	MACRO_ITEM_IOFUNCTIONKEYDESC(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.item_help_offset[3]);
	MACRO_ITEM_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
}

int ItemDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	unsigned int i;
	uint32_t txtpos;
	int res = 0;
	uint16_t version;
	uint16_t namesize = name_space_total, helpsize = help_space_total, helpsize2 = help2_space_total;
	uint16_t knamesize = key_name_space_total, khelpsize = key_help_space_total, kdescsize = key_desc_space_total;
	HWSReadShort(ffbin,version);
	HWSReadShort(ffbin,name_space_total);
	HWSReadShort(ffbin,help_space_total);
	HWSReadShort(ffbin,help2_space_total);
	HWSReadShort(ffbin,key_name_space_total);
	HWSReadShort(ffbin,key_help_space_total);
	HWSReadShort(ffbin,key_desc_space_total);
	MACRO_ITEM_IOFUNCTIONDATA(HWSRead,HWSSeek,true,false)
	if (GetHWSGameType()==GAME_TYPE_PSX) {
		if (name_space_total<=namesize && usetext) {
			MACRO_ITEM_IOFUNCTIONNAME(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<ITEM_AMOUNT;i++)
					item[i].name.PSXToSteam();
		} else {
			ffbin.seekg(name_space_total,ios::cur);
			txtpos = ffbin.tellg();
			ffbin.seekg(key_name_space_total,ios::cur);
			if (usetext)
				res |= 0x1;
		}
	}
	MACRO_ITEM_IOFUNCTIONUSABLE(HWSRead,HWSSeek,true,false)
	MACRO_ITEM_IOFUNCTIONKEY(HWSRead,HWSSeek,true,false)
	if (GetHWSGameType()==GAME_TYPE_PSX) {
		if (key_name_space_total<=knamesize && usetext) {
			MACRO_ITEM_IOFUNCTIONKEYNAME(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<KEY_ITEM_AMOUNT;i++)
					key_item[i].name.PSXToSteam();
		} else if (usetext)
			res |= 0x8;
	}
	MACRO_ITEM_IOFUNCTIONARMOR(HWSRead,HWSSeek,true,false)
	MACRO_ITEM_IOFUNCTIONSTAT(HWSRead,HWSSeek,true,false)
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(HWSRead,HWSSeek,true,false)
	if (GetHWSGameType()==GAME_TYPE_PSX) {
		if (help_space_total<=helpsize && usetext) {
			MACRO_ITEM_IOFUNCTIONHELP(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<ITEM_AMOUNT;i++)
					item[i].help.PSXToSteam();
		} else {
			ffbin.seekg(help_space_total,ios::cur);
			if (usetext)
				res |= 0x2;
		}
		if (help2_space_total<=helpsize2 && usetext) {
			MACRO_ITEM_IOFUNCTIONBATTLEHELP(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<ITEM_AMOUNT;i++)
					item[i].battle_help.PSXToSteam();
		} else {
			ffbin.seekg(help2_space_total+2*ITEM_AMOUNT,ios::cur);
			if (usetext)
				res |= 0x4;
		}
		if (key_help_space_total<=khelpsize && usetext) {
			MACRO_ITEM_IOFUNCTIONKEYHELP(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<KEY_ITEM_AMOUNT;i++)
					key_item[i].help.PSXToSteam();
		} else {
			ffbin.seekg(key_help_space_total,ios::cur);
			if (usetext)
				res |= 0x10;
		}
		if (key_desc_space_total<=kdescsize && usetext) {
			MACRO_ITEM_IOFUNCTIONKEYDESC(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<KEY_ITEM_AMOUNT;i++)
					key_item[i].description.PSXToSteam();
		} else {
			ffbin.seekg(key_desc_space_total,ios::cur);
			if (usetext)
				res |= 0x20;
		}
	}
	name_space_total = namesize;
	help_space_total = helpsize;
	help2_space_total = helpsize2;
	key_name_space_total = knamesize;
	key_help_space_total = khelpsize;
	key_desc_space_total = kdescsize;
	if (GetHWSGameType()!=GAME_TYPE_PSX) {
		SteamLanguage lg;
		uint16_t txtspace;
		uint32_t tmppos;
		
		#define MACRO_ITEM_HWSSTEAMTEXT(STR,AMT,SPACEMAX) \
			HWSReadChar(ffbin,lg); \
			while (lg!=0xFF) { \
				HWSReadShort(ffbin,txtspace); \
				tmppos = ffbin.tellg(); \
				if (usetext && txtspace<=SPACEMAX && GetGameType()!=GAME_TYPE_PSX && lg==GetSteamLanguage()) { \
					for (i=0;i<AMT;i++) { \
						SteamReadFF9String(ffbin,STR); \
						if (GetGameType()==GAME_TYPE_PSX) \
							STR.SteamToPSX(); \
					} \
				} \
				ffbin.seekg(tmppos+txtspace); \
				HWSReadChar(ffbin,lg); \
			}
		
		MACRO_ITEM_HWSSTEAMTEXT(item[i].name,ITEM_AMOUNT,name_space_total)
		MACRO_ITEM_HWSSTEAMTEXT(item[i].help,ITEM_AMOUNT,help_space_total)
		MACRO_ITEM_HWSSTEAMTEXT(item[i].battle_help,ITEM_AMOUNT,help2_space_total)
		MACRO_ITEM_HWSSTEAMTEXT(key_item[i].name,KEY_ITEM_AMOUNT,key_name_space_total)
		MACRO_ITEM_HWSSTEAMTEXT(key_item[i].help,KEY_ITEM_AMOUNT,key_help_space_total)
		MACRO_ITEM_HWSSTEAMTEXT(key_item[i].description,KEY_ITEM_AMOUNT,key_desc_space_total)
	}
	UpdateOffset();
	return res;
}

void ItemDataSet::WriteHWS(fstream& ffbin) {
	unsigned int i;
	uint32_t txtpos;
	UpdateOffset();
	HWSWriteShort(ffbin,ITEM_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total, helpsize2 = help2_space_total;
	uint16_t knamesize = key_name_space_total, khelpsize = key_help_space_total, kdescsize = key_desc_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	help2_space_total = help2_space_used;
	key_name_space_total = key_name_space_used;
	key_help_space_total = key_help_space_used;
	key_desc_space_total = key_desc_space_used;
	HWSWriteShort(ffbin,name_space_total);
	HWSWriteShort(ffbin,help_space_total);
	HWSWriteShort(ffbin,help2_space_total);
	HWSWriteShort(ffbin,key_name_space_total);
	HWSWriteShort(ffbin,key_help_space_total);
	HWSWriteShort(ffbin,key_desc_space_total);
	MACRO_ITEM_IOFUNCTIONDATA(HWSWrite,HWSSeek,false,false)
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_ITEM_IOFUNCTIONNAME(HWSWrite,HWSSeek,false,false)
	}
	MACRO_ITEM_IOFUNCTIONUSABLE(HWSWrite,HWSSeek,false,false)
	MACRO_ITEM_IOFUNCTIONKEY(HWSWrite,HWSSeek,false,false)
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_ITEM_IOFUNCTIONKEYNAME(HWSWrite,HWSSeek,false,false)
	}
	MACRO_ITEM_IOFUNCTIONARMOR(HWSWrite,HWSSeek,false,false)
	MACRO_ITEM_IOFUNCTIONSTAT(HWSWrite,HWSSeek,false,false)
	MACRO_ITEM_IOFUNCTIONWEAPONSTAT(HWSWrite,HWSSeek,false,false)
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_ITEM_IOFUNCTIONHELP(HWSWrite,HWSSeek,false,false)
		MACRO_ITEM_IOFUNCTIONBATTLEHELP(HWSWrite,HWSSeek,false,false)
		MACRO_ITEM_IOFUNCTIONKEYHELP(HWSWrite,HWSSeek,false,false)
		MACRO_ITEM_IOFUNCTIONKEYDESC(HWSWrite,HWSSeek,false,false)
	}
	name_space_total = namesize;
	help_space_total = helpsize;
	help2_space_total = helpsize2;
	key_name_space_total = knamesize;
	key_help_space_total = khelpsize;
	key_desc_space_total = kdescsize;
	if (GetGameType()!=GAME_TYPE_PSX) {
		SteamLanguage lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,name_space_used);
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,item[i].name);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
		lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,help_space_used);
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,item[i].help);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
		lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,help2_space_used);
		for (i=0;i<ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,item[i].battle_help);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
		lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,key_name_space_used);
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,key_item[i].name);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
		lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,key_help_space_used);
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,key_item[i].help);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
		lg = GetSteamLanguage();
		HWSWriteChar(ffbin,lg);
		HWSWriteShort(ffbin,key_desc_space_used);
		for (i=0;i<KEY_ITEM_AMOUNT;i++)
			SteamWriteFF9String(ffbin,key_item[i].description);
		lg = 0xFF;
		HWSWriteChar(ffbin,lg);
	}
}

void ItemDataSet::UpdateOffset() {
	uint16_t j=0,k=0,l=0;
	unsigned int i;
	for (i=0;i<ITEM_AMOUNT;i++) {
		item[i].name_offset = j;
		j += item[i].name.length;
		item[i].help_offset = k;
		k += item[i].help.length;
		item[i].battle_help_offset = l;
		l += item[i].battle_help.length;
	}
	name_space_used = j;
	help_space_used = k;
	help2_space_used = l;
	j = 0;
	k = 0;
	l = 0;
	for (i=0;i<KEY_ITEM_AMOUNT;i++) {
		key_item[i].name_offset = j;
		j += key_item[i].name.length;
		key_item[i].help_offset = k;
		k += key_item[i].help.length;
		if (GetGameType()==GAME_TYPE_PSX && key_item[i].description_out_of_bounds)
			key_item[i].description_offset = key_desc_space_total;
		else {
			key_item[i].description_offset = l;
			l += key_item[i].description.length;
		}
	}
	key_name_space_used = j;
	key_help_space_used = k;
	key_desc_space_used = l;
}
