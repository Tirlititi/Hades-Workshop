#include "Spells.h"

#include "main.h"
#include "Database_CSV.h"

#define SPELL_HWS_VERSION 3

const unsigned int steam_spell_field_size[] = { 4, 1, 3, 9, 12, 1, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16 };

int SpellDataStruct::SetName(wstring newvalue) {
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

int SpellDataStruct::SetName(FF9String& newvalue) {
	int oldlen = name.length;
	int newlen = newvalue.length;
	if (parent->name_space_used+newlen>parent->name_space_total+oldlen)
		return 1;
	name = newvalue;
	parent->name_space_used += newlen-oldlen;
	return 0;
}

int SpellDataStruct::SetHelp(wstring newvalue) {
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

int SpellDataStruct::SetHelp(FF9String& newvalue) {
	int oldlen = help.length;
	int newlen = newvalue.length;
	if (parent->help_space_used+newlen>parent->help_space_total+oldlen)
		return 1;
	help = newvalue;
	parent->help_space_used += newlen-oldlen;
	return 0;
}

Spell_Panel SpellDataStruct::GetPanel() {
	return (target_type >> 5) & 0x7;
}

void SpellDataStruct::SetPanel(Spell_Panel newvalue) {
	target_type = (target_type & 0x1F) | (newvalue << 5);
}

Spell_Target_Priority SpellDataStruct::GetTargetPriority() {
	return (target_type >> 4) & 0x1;
}

void SpellDataStruct::SetTargetPriority(Spell_Target_Priority newvalue) {
	target_type = (target_type & 0xEF) | (newvalue << 4);
}

Spell_Target_Type SpellDataStruct::GetTargetType() {
	uint8_t val = target_type & 0xF;
	if (val == 0x0 || val == 0x3 || val == 0x6 || val == 0x9) // In Steam, 0x6 and 0x9 are SPELL_TARGET_TYPE_EVERYONE
		return SPELL_TARGET_TYPE_ANY;
	if (val == 0x1 || val == 0x4 || val == 0x7 || val == 0xA)
		return SPELL_TARGET_TYPE_ALLY;
	if (val == 0x2 || val == 0x5 || val == 0x8 || val == 0xB)
		return SPELL_TARGET_TYPE_ENEMY;
	if (val == 0xC)
		return SPELL_TARGET_TYPE_EVERYONE;
	if (val == 0xD)
		return SPELL_TARGET_TYPE_SELF;
	return SPELL_TARGET_TYPE_IRRELEVANT;
}

void SpellDataStruct::SetTargetType(Spell_Target_Type newvalue) {
	uint8_t ta = target_type & 0xF;
	ta = ta >= 0xC ? 0 : ta / 3;
	if (newvalue == SPELL_TARGET_TYPE_EVERYONE)
		target_type = (target_type & 0xF0) + 0xC;
	else if (newvalue == SPELL_TARGET_TYPE_IRRELEVANT)
		target_type = (target_type & 0xF0) + 0xE;
	else if (newvalue == SPELL_TARGET_TYPE_SELF)
		target_type = (target_type & 0xE0) + 0x10 + 0xD;
	else if (newvalue == SPELL_TARGET_TYPE_ANY)
		target_type = (target_type & 0xF0) + 3 * ta;
	else if (newvalue == SPELL_TARGET_TYPE_ALLY)
		target_type = (target_type & 0xE0) + 0x10 + 3 * ta + 1;
	else if (newvalue == SPELL_TARGET_TYPE_ENEMY)
		target_type = (target_type & 0xE0) + 3 * ta + 2;
}

Spell_Target_Amount SpellDataStruct::GetTargetAmount() {
	uint8_t val = target_type & 0xF;
	if (val < 0x3 || val == 0xD)
		return SPELL_TARGET_AMOUNT_ONE;
	if (val < 0x6)
		return SPELL_TARGET_AMOUNT_VARIABLE;
	if (val < 0x9 || val >= 0xC)
		return SPELL_TARGET_AMOUNT_GROUP;
	return SPELL_TARGET_AMOUNT_RANDOM;
}

void SpellDataStruct::SetTargetAmount(Spell_Target_Amount newvalue) {
	if (GetTargetType() == SPELL_TARGET_TYPE_SELF && newvalue != SPELL_TARGET_AMOUNT_ONE)
		SetTargetType(SPELL_TARGET_TYPE_ALLY);
	if (GetTargetType() == SPELL_TARGET_TYPE_EVERYONE && newvalue != SPELL_TARGET_AMOUNT_GROUP)
		SetTargetType(SPELL_TARGET_TYPE_ANY);
	if (GetTargetType() == SPELL_TARGET_TYPE_IRRELEVANT)
		SetTargetType(SPELL_TARGET_TYPE_ANY);
	uint8_t tt = target_type & 0xF;
	if (newvalue == SPELL_TARGET_AMOUNT_ONE)
		target_type = (target_type & 0xF0) + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_VARIABLE)
		target_type = (target_type & 0xF0) + 0x3 + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_GROUP)
		target_type = (target_type & 0xF0) + 0x6 + tt % 3;
	else if (newvalue == SPELL_TARGET_AMOUNT_RANDOM)
		target_type = (target_type & 0xF0) + 0x9 + tt % 3;
}

uint16_t SpellDataStruct::GetSound() {
	return (model >> 9) | ((target_flag & 0x1F) << 7);
}

void SpellDataStruct::SetSound(uint16_t newvalue) {
	model = (model & 0x1FF) | (newvalue & 0x7F << 9);
	target_flag = (target_flag & 0xE0) | ((newvalue & 0xF80) >> 7);
}

#define MACRO_SPELL_IOFUNCTIONDATA(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<SPELL_AMOUNT;i++) { \
		IO ## Char(ffbin,spell[i].target_type); \
		IO ## Short(ffbin,spell[i].model); \
		IO ## Char(ffbin,spell[i].target_flag); \
		IO ## Char(ffbin,spell[i].effect); \
		IO ## Char(ffbin,spell[i].power); \
		IO ## Char(ffbin,spell[i].element); \
		IO ## Char(ffbin,spell[i].accuracy); \
		IO ## Char(ffbin,spell[i].flag); \
		IO ## Char(ffbin,spell[i].status); \
		IO ## Char(ffbin,spell[i].mp); \
		IO ## Char(ffbin,spell[i].menu_flag); \
		IO ## Short(ffbin,spell[i].model_alt); \
		IO ## Short(ffbin,spell[i].name_offset); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_IOFUNCTIONNAME(IO,SEEK,TXTPOS,READ,PPF) \
	txtpos = TXTPOS; \
	if (READ) name_space_used = 0; \
	ffbin.seekg(txtpos); \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<SPELL_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,spell[i].name_offset); \
		IO ## FF9String(ffbin,spell[i].name); \
		if (READ) name_space_used += spell[i].name.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,name_space_total);

#define MACRO_SPELL_IOFUNCTIONHELP(IO,SEEK,READ,PPF) \
	txtpos = ffbin.tellg(); \
	SEEK(ffbin,txtpos,help_space_total); \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<SPELL_AMOUNT;i++) { \
		IO ## Short(ffbin,spell[i].help_offset); \
		IO ## Short(ffbin,spell[i].help_size_x); \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) help_space_used = 0; \
	ffbin.seekg(txtpos); \
	if (PPF) PPFInitScanStep(ffbin,true,help_space_total); \
	for (i=0;i<SPELL_AMOUNT;i++) { \
		SEEK(ffbin,txtpos,spell[i].help_offset); \
		IO ## FF9String(ffbin,spell[i].help); \
		if (READ) help_space_used += spell[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total+4*SPELL_AMOUNT);

#define MACRO_SPELL_IOFUNCTIONPERFNAME(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<SPELL_AMOUNT;i++) \
		IO ## Char(ffbin,spell[i].perform_name); \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_IOFUNCTIONSTATUS(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<STATUS_SET_AMOUNT;i++) \
		IO ## Long(ffbin,status_set[i]); \
	if (PPF) PPFEndScanStep();


void SpellDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	uint32_t txtpos;
	unsigned int i;
	name_space_total = config.spell_name_space_total;
	help_space_total = config.spell_help_space_total;
	for (i=0;i<SPELL_AMOUNT;i++)
		spell[i].parent = this;
	if (GetGameType()==GAME_TYPE_PSX) {
		ffbin.seekg(config.spell_data_offset[0]);
		MACRO_SPELL_IOFUNCTIONDATA(FFIXRead,FFIXSeek,true,false)
		MACRO_SPELL_IOFUNCTIONNAME(FFIXRead,FFIXSeek,ffbin.tellg(),true,false)
		MACRO_SPELL_IOFUNCTIONHELP(FFIXRead,FFIXSeek,true,false)
		ffbin.seekg(config.spell_naming_rules_offset);
		MACRO_SPELL_IOFUNCTIONPERFNAME(FFIXRead,FFIXSeek,true,false)
		FFIXSeek(ffbin,config.spell_data_offset[4],0x10*SPELL_AMOUNT); // ToDo : Should go to the .hwf
		MACRO_SPELL_IOFUNCTIONSTATUS(FFIXRead,FFIXSeek,true,false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (SteamLanguage lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang!=GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.spell_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.spell_name_file[lang]);
			for (i=0;i<SPELL_AMOUNT;i++)
				SteamReadFF9String(ffbin,spell[i].name,lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.spell_help_file[lang]));
			help_space_used = config.meta_res.GetFileSizeByIndex(config.spell_help_file[lang]);
			for (i=0;i<SPELL_AMOUNT;i++)
				SteamReadFF9String(ffbin,spell[i].help,lang);
		}
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_spellnaming_field_id));
		for (i=0;i<SPELL_AMOUNT;i++)
			SteamReadChar(dlldata.dll_file,spell[i].perform_name);
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_statusset_field_id));
		for (i=0;i<STATUS_SET_AMOUNT;i++)
			SteamReadLong(dlldata.dll_file,status_set[i]);
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_battledb_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x20, SPELL_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("AA_DATA") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file,3,initinst);
		steam_method_position = dlldata.dll_file.tellg();
		uint8_t* rawspelldata = dlldata.ConvertScriptToRaw_Object(SPELL_AMOUNT,18,steam_spell_field_size);
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg()-steam_method_position;
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(),ios::out | ios::binary);
		ffbin.write((const char*)rawspelldata,0x10*SPELL_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		MACRO_SPELL_IOFUNCTIONDATA(SteamRead,SteamSeek,true,false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawspelldata;
	}
}

DllMetaDataModification* SpellDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[3];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t*[SPELL_AMOUNT];
	unsigned int i;
	for (i=0;i<SPELL_AMOUNT;i++) {
		argvalue[i] = new uint32_t[18];
		argvalue[i][0] = spell[i].target_type & 0xF;
		argvalue[i][1] = (spell[i].target_type >> 4) & 0x1;
		argvalue[i][2] = (spell[i].target_type >> 5) & 0x7;
		argvalue[i][3] = spell[i].model & 0x1FF;
		argvalue[i][4] = spell[i].GetSound();
		argvalue[i][5] = (spell[i].target_flag >> 5) & 0x1;
		argvalue[i][6] = (spell[i].target_flag >> 6) & 0x1;
		argvalue[i][7] = (spell[i].target_flag >> 7) & 0x1;
		argvalue[i][8] = spell[i].effect;
		argvalue[i][9] = spell[i].power;
		argvalue[i][10] = spell[i].element;
		argvalue[i][11] = spell[i].accuracy;
		argvalue[i][12] = spell[i].flag;
		argvalue[i][13] = spell[i].status;
		argvalue[i][14] = spell[i].mp;
		argvalue[i][15] = spell[i].menu_flag;
		argvalue[i][16] = spell[i].model_alt;
		argvalue[i][17] = spell[i].name_offset;
	}
	res[0] = dlldata.ConvertRawToScript_Object(argvalue,steam_method_position,steam_method_base_length,SPELL_AMOUNT,18,steam_spell_field_size);
	for (i=0;i<SPELL_AMOUNT;i++)
		delete[] argvalue[i];
	delete[] argvalue;
	res[1].position = config.meta_dll.GetStaticFieldOffset(config.dll_spellnaming_field_id);
	res[1].base_length = SPELL_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_spellnaming_field_id);
	res[1].new_length = SPELL_AMOUNT;
	res[1].value = new uint8_t[res[1].new_length];
	for (i=0;i<SPELL_AMOUNT;i++)
		res[1].value[i] = spell[i].perform_name;
	res[2].position = config.meta_dll.GetStaticFieldOffset(config.dll_statusset_field_id);
	res[2].base_length = 8*STATUS_SET_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_statusset_field_id);
	res[2].new_length = 8*STATUS_SET_AMOUNT;
	res[2].value = new uint8_t[res[2].new_length];
	BufferInitPosition();
	for (i = 0; i<STATUS_SET_AMOUNT; i++)
		BufferWriteLong(res[2].value, status_set[i]);
	for (i = 0; i<STATUS_SET_AMOUNT; i++) // DEBUG: Steam version has 2x more status sets...
		BufferWriteLong(res[2].value, 0);
	*modifamount = 3;
	return res;
}

void SpellDataSet::GenerateCSharp(vector<string>& buffer) {
	unsigned int i;
	stringstream battledb;
	battledb << "// Method: FF9BattleDB::.cctor\n\n";
	battledb << "\tFF9BattleDB.aa_data = new AA_DATA[] {\n";
	for (i = 0; i < SPELL_AMOUNT; i++)
		battledb<< "\t\tnew AA_DATA(new CMD_INFO(" << (int)(spell[i].target_type & 0xF) << ", " << (int)((spell[i].target_type >> 4) & 0x1) << ", " << (int)((spell[i].target_type >> 5) & 0x7) << ", " << (int)(spell[i].model & 0x1FF) << ", " << (int)spell[i].GetSound() << ", " << (int)((spell[i].target_flag >> 5) & 0x1) << ", " << (int)((spell[i].target_flag >> 6) & 0x1) << ", " << (int)((spell[i].target_flag >> 7) & 0x1)
				<< "), new BTL_REF(" << (int)spell[i].effect << ", " << (int)spell[i].power << ", " << StreamAsHex(spell[i].element) << ", " << (int)spell[i].accuracy
				<< "), " << StreamAsHex(spell[i].flag) << ", " << (int)spell[i].status << ", " << (int)spell[i].mp << ", " << StreamAsHex(spell[i].menu_flag) << ", " << (int)spell[i].model_alt << ", " << (int)spell[i].name_offset << (i+1==SPELL_AMOUNT ? ")" : "),") << " // " << ConvertWStrToStr(spell[i].name.str_nice) << "\n";
	battledb << "\t};\n";
	battledb << "\t// ...\n";
	battledb << "\tFF9BattleDB.add_status = new uint[] { ";
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		battledb << StreamAsHex(status_set[i]) << "u, ";
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		battledb << 0 << (i+1==STATUS_SET_AMOUNT ? "u " : "u, ");
	battledb << "};\n";
	buffer.push_back(battledb.str());
	stringstream spellnaming;
	spellnaming << "// Method: BattleHUD::.cctor\n\n";
	spellnaming << "\tBattleHUD.CmdTitleTable = new byte[] { ";
	for (i = 0; i < SPELL_AMOUNT; i++)
		spellnaming << (int)spell[i].perform_name << (i+1==SPELL_AMOUNT ? " " : ", ");
	spellnaming << "};\n";
	buffer.push_back(spellnaming.str());
}

bool SpellDataSet::GenerateCSV(string basefolder) {
	static const wstring CSV_PANEL_NAME[] {
		L"None", L"HP", L"MP", L"BadStatus", L"GoodStatus", L"Empty", L"Card", L"Bug"
	};
	unsigned int i, j;
	string fname = basefolder + HADES_STRING_CSV_SPELL_FILE;
	wfstream csv(fname.c_str(), ios::out);
	if (!csv.is_open()) return false;
	csv << HADES_STRING_CSV_SPELL_HEADER;
	for (i=0; i<SPELL_AMOUNT; i++) {
		csv << ConvertWStrToStr(spell[i].name.str_nice).c_str() << L";" << i << L";";
		csv << CSV_PANEL_NAME[spell[i].GetPanel()] << L"(" << (int)spell[i].GetPanel() << L");" << (int)(spell[i].target_type & 0xF) << L";" << (int)((spell[i].target_type >> 4) & 0x1) << L";";
		csv << (int)((spell[i].target_flag >> 5) & 0x1) << L";" << (int)((spell[i].target_flag >> 7) & 0x1) << L";" << (int)((spell[i].target_flag >> 6) & 0x1) << L";" << (int)(spell[i].model & 0x1FF) << L";";
		csv << (int)spell[i].model_alt << L";" << (int)spell[i].effect << L";" << (int)spell[i].power << L";" << (int)spell[i].element << L";" << (int)spell[i].accuracy << L";";
		csv << (int)spell[i].flag << L";" << (int)spell[i].status << L";" << (int)spell[i].mp << L";" << (int)spell[i].menu_flag << L";# " << ConvertWStrToStr(spell[i].name.str_nice).c_str() << L"\n";
	}
	csv.close();
	fname = basefolder + HADES_STRING_CSV_STATUS_FILE;
	csv.open(fname.c_str(), ios::out);
	if (!csv.is_open()) return false;
	csv << HADES_STRING_CSV_STATUS_HEADER;
	for (i=0; i<STATUS_SET_AMOUNT; i++) {
		csv << L"Set " << i << L";" << i << L";";
		if (status_set[i]==0)
			csv << L"0;";
		else
			for (j=0; j<STATUS_AMOUNT; j++)
				if (status_set[i] & (1 << j))
					csv << j+1 << L";";
		csv << L"# " << status_set_name[i] << L"\n";
	}
	for (i=0; i<STATUS_SET_AMOUNT; i++)
		csv << L"Set " << STATUS_SET_AMOUNT+i << L";" << STATUS_SET_AMOUNT+i << L";0;# Set " << STATUS_SET_AMOUNT+i << L"\n";
	csv.close();
	fname = basefolder + HADES_STRING_CSV_SPELLTITLE_FILE;
	csv.open(fname.c_str(), ios::out);
	if (!csv.is_open()) return false;
	csv << HADES_STRING_CSV_SPELLTITLE_HEADER;
	for (i=0; i<SPELL_AMOUNT; i++)
		csv << ConvertWStrToStr(spell[i].name.str_nice).c_str() << L";" << i << L";" << (int)spell[i].perform_name << L"\n";
	csv.close();
	return true;
}

int SpellDataSet::GetSteamTextSize(unsigned int texttype, SteamLanguage lang) {
	unsigned int i;
	int res = 0;
	if (texttype==0)
		for (i=0;i<SPELL_AMOUNT;i++)
			res += spell[i].name.GetLength(lang);
	else
		for (i=0;i<SPELL_AMOUNT;i++)
			res += spell[i].help.GetLength(lang);
	return res;
}

void SpellDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype, SteamLanguage lang) {
	unsigned int i;
	if (texttype==0) {
		for (i=0;i<SPELL_AMOUNT;i++)
			SteamWriteFF9String(ffbin,spell[i].name,lang);
	} else {
		for (i=0;i<SPELL_AMOUNT;i++)
			SteamWriteFF9String(ffbin,spell[i].help,lang);
	}
}

void SpellDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	uint32_t txtpos;
	unsigned int i;
	UpdateOffset();
	ffbin.seekg(config.spell_data_offset[0]);
	MACRO_SPELL_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_SPELL_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,ffbin.tellg(),false,false)
	MACRO_SPELL_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.spell_data_offset[1]);
	MACRO_SPELL_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_SPELL_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,ffbin.tellg(),false,false)
	ffbin.seekg(config.spell_data_offset[2]);
	MACRO_SPELL_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_SPELL_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,ffbin.tellg(),false,false)
	ffbin.seekg(config.spell_data_offset[3]);
	MACRO_SPELL_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_SPELL_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,ffbin.tellg(),false,false)
	ffbin.seekg(config.spell_data_offset[4]);
	MACRO_SPELL_IOFUNCTIONDATA(FFIXWrite,FFIXSeek,false,false)
	MACRO_SPELL_IOFUNCTIONNAME(FFIXWrite,FFIXSeek,config.spell_text_offset,false,false)
	MACRO_SPELL_IOFUNCTIONHELP(FFIXWrite,FFIXSeek,false,false)
	FFIXSeek(ffbin,config.spell_data_offset[4],0x10*SPELL_AMOUNT);
	MACRO_SPELL_IOFUNCTIONSTATUS(FFIXWrite,FFIXSeek,false,false)
	ffbin.seekg(config.spell_naming_rules_offset);
	MACRO_SPELL_IOFUNCTIONPERFNAME(FFIXWrite,FFIXSeek,false,false)
}

void SpellDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	uint32_t txtpos;
	unsigned int i;
	UpdateOffset();
	ffbin.seekg(config.spell_data_offset[0]);
	MACRO_SPELL_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SPELL_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,ffbin.tellg(),false,true)
	MACRO_SPELL_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.spell_data_offset[1]);
	MACRO_SPELL_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SPELL_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,ffbin.tellg(),false,true)
	ffbin.seekg(config.spell_data_offset[2]);
	MACRO_SPELL_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SPELL_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,ffbin.tellg(),false,true)
	ffbin.seekg(config.spell_data_offset[3]);
	MACRO_SPELL_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SPELL_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,ffbin.tellg(),false,true)
	ffbin.seekg(config.spell_data_offset[4]);
	MACRO_SPELL_IOFUNCTIONDATA(PPFStepAdd,FFIXSeek,false,true)
	MACRO_SPELL_IOFUNCTIONNAME(PPFStepAdd,FFIXSeek,config.spell_text_offset,false,true)
	MACRO_SPELL_IOFUNCTIONHELP(PPFStepAdd,FFIXSeek,false,true)
	FFIXSeek(ffbin,config.spell_data_offset[4],0x10*SPELL_AMOUNT);
	MACRO_SPELL_IOFUNCTIONSTATUS(PPFStepAdd,FFIXSeek,false,true)
	ffbin.seekg(config.spell_naming_rules_offset);
	MACRO_SPELL_IOFUNCTIONPERFNAME(PPFStepAdd,FFIXSeek,false,true)
}

int SpellDataSet::LoadHWS(fstream& ffbin, bool usetext) {
	uint32_t txtpos;
	unsigned int i;
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
	MACRO_SPELL_IOFUNCTIONDATA(HWSRead,HWSSeek,true,false)
	if (name_space_total<=namesize && usetext) {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			MACRO_SPELL_IOFUNCTIONNAME(HWSRead,HWSSeek,ffbin.tellg(),true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<SPELL_AMOUNT;i++)
					spell[i].name.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType()!=GAME_TYPE_PSX)
					for (i=0;i<SPELL_AMOUNT;i++)
						SteamReadFF9String(ffbin,spell[i].name,lg);
				else if (lg==GetSteamLanguage()) // DEBUG: should somehow check if the total length of converted strings is low enough
					for (i=0;i<SPELL_AMOUNT;i++) {
						SteamReadFF9String(ffbin,spell[i].name);
						spell[i].name.SteamToPSX();
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
			MACRO_SPELL_IOFUNCTIONHELP(HWSRead,HWSSeek,true,false)
			if (GetGameType()!=GAME_TYPE_PSX)
				for (i=0;i<SPELL_AMOUNT;i++)
					spell[i].help.PSXToSteam();
		} else {
			SteamLanguage lg;
			uint16_t txtspace;
			uint32_t tmppos;
			HWSReadChar(ffbin,lg);
			while (lg!=STEAM_LANGUAGE_NONE) {
				HWSReadShort(ffbin,txtspace);
				tmppos = ffbin.tellg();
				if (GetGameType()!=GAME_TYPE_PSX)
					for (i=0;i<SPELL_AMOUNT;i++)
						SteamReadFF9String(ffbin,spell[i].help,lg);
				else if (lg==GetSteamLanguage())
					for (i=0;i<SPELL_AMOUNT;i++) {
						SteamReadFF9String(ffbin,spell[i].help,lg);
						spell[i].help.SteamToPSX();
					}
				ffbin.seekg(tmppos+txtspace);
				HWSReadChar(ffbin,lg);
			}
		}
	} else {
		if (GetHWSGameType()==GAME_TYPE_PSX) {
			ffbin.seekg(help_space_total+4*SPELL_AMOUNT,ios::cur);
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
	if (version>=2) {
		MACRO_SPELL_IOFUNCTIONPERFNAME(HWSRead,HWSSeek,true,false)
	}
	if (version>=3) {
		MACRO_SPELL_IOFUNCTIONSTATUS(HWSRead,HWSSeek,true,false)
	}
	name_space_total = namesize;
	help_space_total = helpsize;
	UpdateOffset();
	return res;
}

void SpellDataSet::WriteHWS(fstream& ffbin) {
	uint32_t txtpos;
	unsigned int i;
	UpdateOffset();
	HWSWriteShort(ffbin,SPELL_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	HWSWriteShort(ffbin,name_space_total);
	HWSWriteShort(ffbin,help_space_total);
	MACRO_SPELL_IOFUNCTIONDATA(HWSWrite,HWSSeek,false,false)
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SPELL_IOFUNCTIONNAME(HWSWrite,HWSSeek,ffbin.tellg(),false,false)
		MACRO_SPELL_IOFUNCTIONHELP(HWSWrite,HWSSeek,false,false)
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
	MACRO_SPELL_IOFUNCTIONPERFNAME(HWSWrite,HWSSeek,false,false)
	MACRO_SPELL_IOFUNCTIONSTATUS(HWSWrite,HWSSeek,false,false)
	name_space_total = namesize;
	help_space_total = helpsize;
}

void SpellDataSet::UpdateOffset() {
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	uint16_t j=0,k=0;
	unsigned int i;
	for (i=0;i<SPELL_AMOUNT;i++) {
		spell[i].name_offset = j;
		j += spell[i].name.length;
		spell[i].help_offset = k;
		k += spell[i].help.length;
	}
	name_space_used = j;
	help_space_used = k;
}
