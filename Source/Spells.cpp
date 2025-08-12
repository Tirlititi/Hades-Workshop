#include "Spells.h"

#include "main.h"
#include "Database_CSV.h"
#include "CommonUtility.h"

#define SPELL_HWS_VERSION 5

#define SPELL_CSV_CHECK L"%id%;%panel%;%target_type%;%target_default_ally%;%target_for_dead%;%target_default_on_dead%;%target_default_camera%;%model%;%model_alt%;%effect%;%power%;%element%;%accuracy%;%flag%;%status%;%mp%;%menu_flag%;%perform_name%"
#define SPELL_CSV_DEFAULT L"%name%;" SPELL_CSV_CHECK L";# %name%"
#define STATUS_CSV_CHECK L"%id%;%status_list%"
#define STATUS_CSV_DEFAULT L"Set %id%;" STATUS_CSV_CHECK L";# %description%"

const unsigned int steam_spell_field_size[] = { 4, 1, 3, 9, 12, 1, 1, 1, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16 };

int SpellDataStruct::SetName(wstring newvalue) {
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

int SpellDataStruct::SetName(FF9String& newvalue) {
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

int SpellDataStruct::SetHelp(wstring newvalue) {
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

int SpellDataStruct::SetHelp(FF9String& newvalue) {
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

wxString SpellDataStruct::GetFieldValue(wxString fieldname) {
	static const wxString CSV_PANEL_NAME[]{
		L"None", L"Hp", L"Mp", L"Debuffs", L"Buffs", L"Empty", L"Card", L"Special"
	};
	static const wxString CSV_TARGET_TYPE_NAME[]{
		L"SingleAny",	L"SingleAlly",	L"SingleEnemy",	L"ManyAny",		L"ManyAlly",	L"ManyEnemy",	L"All",			L"AllAlly",
		L"AllEnemy",	L"Random",		L"RandomAlly",	L"RandomEnemy",	L"Everyone",	L"Self",		L"Automatic",	L"Special"
	};
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("name")) return wxString::Format(wxT("%s"), name.str_nice);
	if (fieldname.IsSameAs("help")) return wxString::Format(wxT("%s"), help.str_nice);
	if (fieldname.IsSameAs("panel")) return wxString::Format(wxT("%s(%d)"), CSV_PANEL_NAME[GetPanel()], GetPanel());
	if (fieldname.IsSameAs("target_type_var")) return wxString::Format(wxT("%d"), target_type);
	if (fieldname.IsSameAs("target_type")) return wxString::Format(wxT("%s(%d)"), CSV_TARGET_TYPE_NAME[target_type & 0xF], target_type & 0xF);
	if (fieldname.IsSameAs("target_default_ally")) return wxString::Format(wxT("%d"), (target_type >> 4) & 0x1);
	if (fieldname.IsSameAs("target_for_dead")) return wxString::Format(wxT("%d"), (target_flag >> 5) & 0x1);
	if (fieldname.IsSameAs("target_default_on_dead")) return wxString::Format(wxT("%d"), (target_flag >> 7) & 0x1);
	if (fieldname.IsSameAs("target_default_camera")) return wxString::Format(wxT("%d"), (target_flag >> 6) & 0x1);
	if (fieldname.IsSameAs("model")) return wxString::Format(wxT("%d"), model);
	if (fieldname.IsSameAs("model_alt")) return wxString::Format(wxT("%d"), model_alt);
	if (fieldname.IsSameAs("effect")) return wxString::Format(wxT("%d"), effect);
	if (fieldname.IsSameAs("power")) return wxString::Format(wxT("%d"), power);
	if (fieldname.IsSameAs("element")) return wxString::Format(wxT("%d"), element);
	if (fieldname.IsSameAs("accuracy")) return wxString::Format(wxT("%d"), accuracy);
	if (fieldname.IsSameAs("flag")) return wxString::Format(wxT("%d"), flag);
	if (fieldname.IsSameAs("status")) return wxString::Format(wxT("%d"), status);
	if (fieldname.IsSameAs("mp")) return wxString::Format(wxT("%d"), mp);
	if (fieldname.IsSameAs("menu_flag")) return wxString::Format(wxT("%d"), menu_flag);
	if (fieldname.IsSameAs("perform_name")) return wxString::Format(wxT("%d"), perform_name);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field.find(fieldname); search != parent->custom_field.end()) return search->second;
	return _(L"");
}

bool SpellDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(SPELL_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';').BeforeLast(L';'), rightcsv);
}

wxString StatusSetStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("status_list")) return FormatStatusSet(status_list);
	if (fieldname.IsSameAs("description")) return wxString::Format(wxT("%s"), name);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field_status.find(fieldname); search != parent->custom_field_status.end()) return search->second;
	return _(L"");
}

bool StatusSetStruct::CompareWithCSV(wxArrayString entries) {
	if (!custom_field.empty())
		return false;
	if (status_list.size() == 0 && id >= 39)
		return true;
	if (id >= (int)entries.GetCount())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(STATUS_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';').BeforeLast(L';'), rightcsv);
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
	for (i=0;i<spellamount;i++) { \
		IO ## Char(ffbin,spell[i].target_type); \
		IO ## FlexibleShort(ffbin,spell[i].model, useextendedtype); \
		IO ## Char(ffbin,spell[i].target_flag); \
		IO ## FlexibleChar(ffbin,spell[i].effect, useextendedtype); \
		IO ## FlexibleChar(ffbin,spell[i].power, useextendedtype); \
		IO ## Char(ffbin,spell[i].element); \
		IO ## FlexibleChar(ffbin,spell[i].accuracy, useextendedtype); \
		IO ## Char(ffbin,spell[i].flag); \
		IO ## FlexibleChar(ffbin,spell[i].status, useextendedtype2); \
		IO ## FlexibleChar(ffbin,spell[i].mp, useextendedtype); \
		IO ## Char(ffbin,spell[i].menu_flag); \
		IO ## FlexibleShort(ffbin,spell[i].model_alt, useextendedtype); \
		IO ## Short(ffbin,spell[i].name_offset); \
		if (useextendedtype2) IO ## CSVFields(ffbin, spell[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_IOFUNCTIONNAME(IO,SEEK,TXTPOS,READ,PPF) \
	txtpos = TXTPOS; \
	if (READ) name_space_used = 0; \
	ffbin.seekg(txtpos); \
	if (PPF) PPFInitScanStep(ffbin,true,name_space_total); \
	for (i=0;i<spellamount;i++) { \
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
	for (i=0;i<spellamount;i++) { \
		IO ## Short(ffbin,spell[i].help_offset); \
		IO ## Short(ffbin,spell[i].help_size_x); \
	} \
	if (PPF) PPFEndScanStep(); \
	if (READ) help_space_used = 0; \
	ffbin.seekg(txtpos); \
	if (PPF) PPFInitScanStep(ffbin,true,help_space_total); \
	for (i=0;i<spellamount;i++) { \
		SEEK(ffbin,txtpos,spell[i].help_offset); \
		IO ## FF9String(ffbin,spell[i].help); \
		if (READ) help_space_used += spell[i].help.length; \
	} \
	if (PPF) PPFEndScanStep(); \
	SEEK(ffbin,txtpos,help_space_total+4*spellamount);

#define MACRO_SPELL_IOFUNCTIONPERFNAME(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<spellamount;i++) \
		IO ## Char(ffbin,spell[i].perform_name); \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_IOFUNCTIONSTATUS(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<statussetamount;i++) { \
		MACRO_IOFUNCTIONGENERIC_STATUS(ffbin, useextendedtype2, IO, READ, status_set[i].status_list) \
		if (useextendedtype2) IO ## CSVFields(ffbin, status_set[i].custom_field); \
	} \
	if (PPF) PPFEndScanStep();


void SpellDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	int spellamount = SPELL_AMOUNT;
	int statussetamount = STATUS_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	uint32_t txtpos;
	int i;
	csv_header = _(HADES_STRING_CSV_SPELL_HEADER);
	csv_header_status = _(HADES_STRING_CSV_STATUS_HEADER);
	csv_format = _(SPELL_CSV_DEFAULT);
	csv_format_status = _(STATUS_CSV_DEFAULT);
	name_space_total = config.spell_name_space_total;
	help_space_total = config.spell_help_space_total;
	spell.resize(SPELL_AMOUNT);
	status_set.resize(STATUS_SET_AMOUNT);
	for (i = 0; i < SPELL_AMOUNT; i++) {
		spell[i].parent = this;
		spell[i].id = i;
	}
	for (i = 0; i < STATUS_SET_AMOUNT; i++) {
		status_set[i].parent = this;
		status_set[i].id = i;
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		ffbin.seekg(config.spell_data_offset[0]);
		MACRO_SPELL_IOFUNCTIONDATA(FFIXRead, FFIXSeek, true, false)
		MACRO_SPELL_IOFUNCTIONNAME(FFIXRead, FFIXSeek, ffbin.tellg(), true, false)
		MACRO_SPELL_IOFUNCTIONHELP(FFIXRead, FFIXSeek, true, false)
		ffbin.seekg(config.spell_naming_rules_offset);
		MACRO_SPELL_IOFUNCTIONPERFNAME(FFIXRead, FFIXSeek, true, false)
		FFIXSeek(ffbin, config.spell_data_offset[4], 0x10 * SPELL_AMOUNT); // ToDo : Should go to the .hwf
		MACRO_SPELL_IOFUNCTIONSTATUS(FFIXRead, FFIXSeek, true, false)
	} else {
		DllMetaData& dlldata = config.meta_dll;
		DllMethodInfo methinfo;
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang != GetSteamLanguage())
				continue;
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.spell_name_file[lang]));
			name_space_used = config.meta_res.GetFileSizeByIndex(config.spell_name_file[lang]);
			for (i = 0; i < SPELL_AMOUNT; i++)
				SteamReadFF9String(ffbin, spell[i].name, lang);
			ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.spell_help_file[lang]));
			help_space_used = config.meta_res.GetFileSizeByIndex(config.spell_help_file[lang]);
			for (i = 0; i < SPELL_AMOUNT; i++)
				SteamReadFF9String(ffbin, spell[i].help, lang);
		}
		ffbin.close();
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_spellnaming_field_id));
		for (i = 0; i < SPELL_AMOUNT; i++)
			SteamReadChar(dlldata.dll_file, spell[i].perform_name);
		dlldata.dll_file.seekg(dlldata.GetStaticFieldOffset(config.dll_statusset_field_id));
		for (i = 0; i < STATUS_SET_AMOUNT; i++) {
			uint32_t statusraw;
			SteamReadLong(dlldata.dll_file, statusraw);
			SetupStatusList(status_set[i].status_list, statusraw);
		}
		dlldata.dll_file.seekg(dlldata.GetMethodOffset(config.dll_battledb_method_id));
		methinfo.ReadMethodInfo(dlldata.dll_file);
		ILInstruction initinst[3] = {
			{ 0x20, SPELL_AMOUNT },
			{ 0x8D, dlldata.GetTypeTokenIdentifier("AA_DATA") },
			{ 0x25 }
		};
		methinfo.JumpToInstructions(dlldata.dll_file, 3, initinst);
		steam_method_position = dlldata.dll_file.tellg();
		uint8_t* rawspelldata = dlldata.ConvertScriptToRaw_Object(SPELL_AMOUNT, 18, steam_spell_field_size);
		steam_method_base_length = (unsigned int)dlldata.dll_file.tellg() - steam_method_position;
		fname = tmpnam(NULL);
		ffbin.open(fname.c_str(), ios::out | ios::binary);
		ffbin.write((const char*)rawspelldata, 0x10 * SPELL_AMOUNT);
		ffbin.close();
		ffbin.open(fname.c_str(), ios::in | ios::binary);
		MACRO_SPELL_IOFUNCTIONDATA(SteamRead, SteamSeek, true, false)
		ffbin.close();
		remove(fname.c_str());
		delete[] rawspelldata;
		if (config.dll_usage != 0) {
			// Apply Memoria's default fixes for Life, Full-Life and Thunder Slash
			spell[5].target_flag |= TARGET_FLAG_CAMERA;
			spell[6].target_flag |= TARGET_FLAG_CAMERA;
			spell[149].effect = 20;
		}
	}
}

DllMetaDataModification* SpellDataSet::ComputeSteamMod(ConfigurationSet& config, unsigned int* modifamount) {
	DllMetaDataModification* res = new DllMetaDataModification[3];
	DllMetaData& dlldata = config.meta_dll;
	uint32_t** argvalue = new uint32_t*[SPELL_AMOUNT];
	int i;
	for (i = 0; i < SPELL_AMOUNT; i++) {
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
	res[0] = dlldata.ConvertRawToScript_Object(argvalue, steam_method_position, steam_method_base_length, SPELL_AMOUNT, 18, steam_spell_field_size);
	for (i = 0; i < SPELL_AMOUNT; i++)
		delete[] argvalue[i];
	delete[] argvalue;
	res[1].position = config.meta_dll.GetStaticFieldOffset(config.dll_spellnaming_field_id);
	res[1].base_length = SPELL_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_spellnaming_field_id);
	res[1].new_length = SPELL_AMOUNT;
	res[1].value = new uint8_t[res[1].new_length];
	for (i = 0; i < SPELL_AMOUNT; i++)
		res[1].value[i] = spell[i].perform_name;
	res[2].position = config.meta_dll.GetStaticFieldOffset(config.dll_statusset_field_id);
	res[2].base_length = 8 * STATUS_SET_AMOUNT; // config.meta_dll.GetStaticFieldRange(config.dll_statusset_field_id);
	res[2].new_length = 8 * STATUS_SET_AMOUNT;
	res[2].value = new uint8_t[res[2].new_length];
	BufferInitPosition();
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		BufferWriteLong(res[2].value, GetStatusBitList(status_set[i].status_list));
	for (i = 0; i < STATUS_SET_AMOUNT; i++) // DEBUG: Steam version has 2x more status sets...
		BufferWriteLong(res[2].value, 0);
	*modifamount = 3;
	return res;
}

void SpellDataSet::GenerateCSharp(vector<string>& buffer) {
	int i;
	stringstream battledb;
	battledb << "// Method: FF9BattleDB::.cctor\n\n";
	battledb << "\tFF9BattleDB.aa_data = new AA_DATA[] {\n";
	for (i = 0; i < SPELL_AMOUNT; i++)
		battledb << "\t\tnew AA_DATA(new CMD_INFO(" << (int)(spell[i].target_type & 0xF) << ", " << (int)((spell[i].target_type >> 4) & 0x1) << ", " << (int)((spell[i].target_type >> 5) & 0x7) << ", " << (int)(spell[i].model & 0x1FF) << ", " << (int)spell[i].GetSound() << ", " << (int)((spell[i].target_flag >> 5) & 0x1) << ", " << (int)((spell[i].target_flag >> 6) & 0x1) << ", " << (int)((spell[i].target_flag >> 7) & 0x1)
		<< "), new BTL_REF(" << (int)spell[i].effect << ", " << (int)spell[i].power << ", " << StreamAsHex(spell[i].element) << ", " << (int)spell[i].accuracy
		<< "), " << StreamAsHex(spell[i].flag) << ", " << (int)spell[i].status << ", " << (int)spell[i].mp << ", " << StreamAsHex(spell[i].menu_flag) << ", " << (int)spell[i].model_alt << ", " << (int)spell[i].name_offset << (i + 1 == SPELL_AMOUNT ? ")" : "),") << " // " << ConvertWStrToStr(spell[i].name.str_nice) << "\n";
	battledb << "\t};\n";
	battledb << "\t// ...\n";
	battledb << "\tFF9BattleDB.add_status = new uint[] { ";
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		battledb << StreamAsHex(GetStatusBitList(status_set[i].status_list)) << "u, ";
	for (i = 0; i < STATUS_SET_AMOUNT; i++)
		battledb << 0 << (i + 1 == STATUS_SET_AMOUNT ? "u " : "u, ");
	battledb << "};\n";
	buffer.push_back(battledb.str());
	stringstream spellnaming;
	spellnaming << "// Method: BattleHUD::.cctor\n\n";
	spellnaming << "\tBattleHUD.CmdTitleTable = new byte[] { ";
	for (i = 0; i < SPELL_AMOUNT; i++)
		spellnaming << (int)spell[i].perform_name << (i + 1 == SPELL_AMOUNT ? " " : ", ");
	spellnaming << "};\n";
	buffer.push_back(spellnaming.str());
}

bool SpellDataSet::GenerateCSV(string basefolder) {
	if (!MemoriaUtility::GenerateDatabaseGeneric<SpellDataStruct>(_(basefolder), _(HADES_STRING_CSV_SPELL_FILE), csv_header, _(L"\n"), _(L"\n"), spell, csv_format, true))
		return false;
	if (!MemoriaUtility::GenerateDatabaseGeneric<StatusSetStruct>(_(basefolder), _(HADES_STRING_CSV_STATUS_FILE), csv_header_status, _(L"\n"), _(L"\n"), status_set, csv_format_status, true))
		return false;
	return true;
}

void SpellDataSet::WriteSteamText(fstream& ffbin, unsigned int texttype, bool onlymodified, bool asmes, SteamLanguage lang) {
	vector<int> writesubset;
	if (texttype == 0) {
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<SpellDataStruct>(&writesubset, GetGameConfiguration()->spell_name_file, spell, [lang](SpellDataStruct& sp) { return sp.name.multi_lang_str[lang]; }, lang))
			WriteSteamTextGeneric(ffbin, spell, &SpellDataStruct::name, &writesubset, asmes, lang);
		else
			WriteSteamTextGeneric(ffbin, spell, &SpellDataStruct::name, NULL, asmes, lang);
	} else {
		if (onlymodified && MemoriaUtility::GetModifiedSteamTexts<SpellDataStruct>(&writesubset, GetGameConfiguration()->spell_help_file, spell, [lang](SpellDataStruct& sp) { return sp.help.multi_lang_str[lang]; }, lang))
			WriteSteamTextGeneric(ffbin, spell, &SpellDataStruct::help, &writesubset, asmes, lang);
		else
			WriteSteamTextGeneric(ffbin, spell, &SpellDataStruct::help, NULL, asmes, lang);
	}
}

void SpellDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	int spellamount = SPELL_AMOUNT;
	int statussetamount = STATUS_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	uint32_t txtpos;
	int i;
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
	int spellamount = SPELL_AMOUNT;
	int statussetamount = STATUS_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	uint32_t txtpos;
	int i;
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
	int spellamount = SPELL_AMOUNT;
	int statussetamount = STATUS_SET_AMOUNT;
	bool useextendedtype = false;
	bool useextendedtype2 = false;
	vector<SpellDataStruct> nonmodifiedspell;
	vector<StatusSetStruct> nonmodifiedstatusset;
	uint32_t txtpos;
	SteamLanguage lg;
	int txtspace;
	int i;
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
	if (version >= 4) {
		useextendedtype = true;
		vector<int> added;
		spellamount = PrepareHWSFlexibleList(ffbin, spell, nonmodifiedspell, added);
		for (i = 0; i < (int)added.size(); i++) {
			spell[added[i]].name.CreateEmpty(true);
			spell[added[i]].help.CreateEmpty(true);
			spell[added[i]].parent = this;
		}
	}
	if (version >= 5) {
		useextendedtype2 = true;
		vector<int> added;
		statussetamount = PrepareHWSFlexibleList(ffbin, status_set, nonmodifiedstatusset, added);
		for (i = 0; i < (int)added.size(); i++)
			status_set[added[i]].parent = this;
		HWSReadCSVFormatting(ffbin, csv_header, csv_format);
		HWSReadCSVFields(ffbin, custom_field);
		HWSReadCSVFormatting(ffbin, csv_header_status, csv_format_status);
		HWSReadCSVFields(ffbin, custom_field_status);
	}
	MACRO_SPELL_IOFUNCTIONDATA(HWSRead, HWSSeek, true, false)
	if (name_space_total <= namesize && usetext) {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			MACRO_SPELL_IOFUNCTIONNAME(HWSRead, HWSSeek, ffbin.tellg(), true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < spellamount; i++)
					spell[i].name.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < spellamount; i++)
						SteamReadFF9String(ffbin, spell[i].name, lg);
				else if (lg == GetSteamLanguage()) // DEBUG: should somehow check if the total length of converted strings is low enough
					for (i = 0; i < spellamount; i++) {
						SteamReadFF9String(ffbin, spell[i].name);
						spell[i].name.SteamToPSX();
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
			MACRO_SPELL_IOFUNCTIONHELP(HWSRead, HWSSeek, true, false)
			if (GetGameType() != GAME_TYPE_PSX)
				for (i = 0; i < spellamount; i++)
					spell[i].help.PSXToSteam();
		} else {
			uint32_t tmppos;
			HWSReadChar(ffbin, lg);
			while (lg != STEAM_LANGUAGE_NONE) {
				HWSReadFlexibleShort(ffbin, txtspace, useextendedtype);
				tmppos = ffbin.tellg();
				if (GetGameType() != GAME_TYPE_PSX)
					for (i = 0; i < spellamount; i++)
						SteamReadFF9String(ffbin, spell[i].help, lg);
				else if (lg == GetSteamLanguage())
					for (i = 0; i < spellamount; i++) {
						SteamReadFF9String(ffbin, spell[i].help, lg);
						spell[i].help.SteamToPSX();
					}
				ffbin.seekg(tmppos + txtspace);
				HWSReadChar(ffbin, lg);
			}
		}
	} else {
		if (GetHWSGameType() == GAME_TYPE_PSX) {
			ffbin.seekg(help_space_total + 4 * spellamount, ios::cur);
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
		MACRO_SPELL_IOFUNCTIONPERFNAME(HWSRead, HWSSeek, true, false)
	}
	if (version >= 3) {
		MACRO_SPELL_IOFUNCTIONSTATUS(HWSRead, HWSSeek, true, false)
	}
	for (i = 0; i < (int)nonmodifiedspell.size(); i++)
		InsertAtId(spell, nonmodifiedspell[i], nonmodifiedspell[i].id);
	for (i = 0; i < (int)nonmodifiedstatusset.size(); i++)
		InsertAtId(status_set, nonmodifiedstatusset[i], nonmodifiedstatusset[i].id);
	name_space_total = namesize;
	help_space_total = helpsize;
	UpdateOffset();
	return res;
}

void SpellDataSet::WriteHWS(fstream& ffbin) {
	int spellamount = spell.size();
	int statussetamount = status_set.size();
	bool useextendedtype = true;
	bool useextendedtype2 = true;
	uint32_t txtpos;
	int i;
	UpdateOffset();
	HWSWriteShort(ffbin, SPELL_HWS_VERSION);
	uint16_t namesize = name_space_total, helpsize = help_space_total;
	name_space_total = name_space_used;
	help_space_total = help_space_used;
	HWSWriteShort(ffbin, name_space_total);
	HWSWriteShort(ffbin, help_space_total);
	HWSWriteFlexibleChar(ffbin, spellamount, true);
	for (i = 0; i < spellamount; i++)
		HWSWriteFlexibleChar(ffbin, spell[i].id, true);
	HWSWriteFlexibleChar(ffbin, statussetamount, true);
	for (i = 0; i < statussetamount; i++)
		HWSWriteFlexibleChar(ffbin, status_set[i].id, true);
	HWSWriteCSVFormatting(ffbin, csv_header, csv_format);
	HWSWriteCSVFields(ffbin, custom_field);
	HWSWriteCSVFormatting(ffbin, csv_header_status, csv_format_status);
	HWSWriteCSVFields(ffbin, custom_field_status);
	MACRO_SPELL_IOFUNCTIONDATA(HWSWrite, HWSSeek, false, false)
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_SPELL_IOFUNCTIONNAME(HWSWrite, HWSSeek, ffbin.tellg(), false, false)
		MACRO_SPELL_IOFUNCTIONHELP(HWSWrite, HWSSeek, false, false)
	} else {
		SteamLanguage lg;
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(spell, &SpellDataStruct::name, false, lg), true);
				WriteSteamText(ffbin, 0, false, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
		for (lg = STEAM_LANGUAGE_US; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (hades::STEAM_LANGUAGE_SAVE_LIST[lg]) {
				HWSWriteChar(ffbin, lg);
				HWSWriteFlexibleShort(ffbin, GetSteamTextSizeGeneric(spell, &SpellDataStruct::help, false, lg), true);
				WriteSteamText(ffbin, 1, false, false, lg);
			}
		}
		HWSWriteChar(ffbin, STEAM_LANGUAGE_NONE);
	}
	MACRO_SPELL_IOFUNCTIONPERFNAME(HWSWrite, HWSSeek, false, false)
	MACRO_SPELL_IOFUNCTIONSTATUS(HWSWrite, HWSSeek, false, false)
	name_space_total = namesize;
	help_space_total = helpsize;
}

void SpellDataSet::UpdateOffset() {
	if (GetGameType() != GAME_TYPE_PSX)
		return;
	uint16_t j = 0, k = 0;
	int i;
	for (i = 0; i < SPELL_AMOUNT; i++) {
		spell[i].name_offset = j;
		j += spell[i].name.length;
		spell[i].help_offset = k;
		k += spell[i].help.length;
	}
	name_space_used = j;
	help_space_used = k;
}

wxArrayString SpellDataSet::GetModifiableSpellFields() {
	wxArrayString list;
	list.Add(_(L"name"));
	list.Add(_(L"help"));
	list.Add(_(L"target_type_var"));
	list.Add(_(L"target_for_dead"));
	list.Add(_(L"target_default_on_dead"));
	list.Add(_(L"target_default_camera"));
	list.Add(_(L"model"));
	list.Add(_(L"model_alt"));
	list.Add(_(L"effect"));
	list.Add(_(L"power"));
	list.Add(_(L"element"));
	list.Add(_(L"accuracy"));
	list.Add(_(L"flag"));
	list.Add(_(L"status"));
	list.Add(_(L"mp"));
	list.Add(_(L"menu_flag"));
	list.Add(_(L"perform_name"));
	for (auto f = custom_field.begin(); f != custom_field.end(); f++)
		list.Add(f->first);
	return list;
}

wxArrayString SpellDataSet::GetModifiableStatusSetFields() {
	wxArrayString list;
	list.Add(_(L"status_list"));
	for (auto f = custom_field.begin(); f != custom_field.end(); f++)
		list.Add(f->first);
	return list;
}

int SpellDataSet::GetSpellIndexById(int spellid) {
	if (spellid < SPELL_AMOUNT)
		return spellid;
	for (unsigned int i = SPELL_AMOUNT; i < spell.size(); i++)
		if (spell[i].id == spellid)
			return i;
	return -1;
}

SpellDataStruct dummyspell;
SpellDataStruct& SpellDataSet::GetSpellById(int spellid) {
	int index = GetSpellIndexById(spellid);
	if (index >= 0)
		return spell[index];
	dummyspell.id = -1;
	dummyspell.name.CreateEmpty();
	dummyspell.name.SetValue(L"[Invalid]");
	return dummyspell;
}

int SpellDataSet::GetStatusSetIndexById(int statusid) {
	if (statusid < STATUS_SET_AMOUNT)
		return statusid;
	for (unsigned int i = STATUS_SET_AMOUNT; i < status_set.size(); i++)
		if (status_set[i].id == statusid)
			return i;
	return -1;
}

StatusSetStruct dummystatusset;
StatusSetStruct& SpellDataSet::GetStatusSetById(int statusid) {
	int index = GetStatusSetIndexById(statusid);
	if (index >= 0)
		return status_set[index];
	dummystatusset.id = -1;
	dummystatusset.name = L"[Invalid]";
	return dummystatusset;
}
