#include "Scripts.h"

#include "main.h"
#include "Hades_Strings.h"
#include "Database_Script.h"

#define ENTRY_MAX_VALID_ID 128

#define SCRIPT_VERSION_VANILLA	2
#define SCRIPT_VERSION_PENTRY	3 // Player Character entries (beyond vanillas), script append mode and memoria id
#define SCRIPT_VERSION_CURRENT	3

int EntryIdShift = 0;
int EntryIdShift_Base = 0;

uint16_t CurrentScriptReadField = 0;
uint16_t CurrentScriptReadEntry = 0;
uint16_t CurrentScriptReadFunction = 0;

inline uint8_t LangBoolsAsBits(const bool* lang) {
	uint8_t langbits = 0;
	for (uint8_t i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
		if (lang[i])
			langbits |= 1 << i;
	return langbits;
}

inline void LangBitsAsBools(uint8_t langbits, bool* lang) {
	for (uint8_t i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
		lang[i] = (langbits & (1 << i)) != 0;
}

inline wxString CS_GetNextWord(wxString& str) {
	size_t pos = str.find_first_of(L" \t\r\n");
	if (pos == wxString::npos) {
		wxString strcpy = str;
		str = _(L"");
		return strcpy;
	}
	wxString res = str.Mid(0, pos);
	while (pos < str.Len() && (str[pos] == L' ' || str[pos] == L'\t' || str[pos] == L'\r' || str[pos] == L'\n'))
		pos++;
	str = str.Mid(pos);
	return res;
}

inline wxString CS_GetQuotedString(wxString& str) {
	if (str.IsEmpty() || str[0] != L'\"')
		return CS_GetNextWord(str);
	size_t pos = str.find_first_of(L"\"", 1);
	if (pos == wxString::npos) {
		wxString strcpy = str;
		str = _(L"");
		return strcpy;
	}
	wxString res = str.Mid(1, pos - 1);
	pos++;
	while (pos < str.Len() && (str[pos] == L' ' || str[pos] == L'\t' || str[pos] == L'\r' || str[pos] == L'\n'))
		pos++;
	str = str.Mid(pos);
	return res;
}

int GetDefaultPlayerLink(int entryindex, int entryamount, int fieldid) {
	if (entryindex + 1 == entryamount)
		return 11; // Beatrix
	if (fieldid >= 0) {
		if (entryindex + 2 == entryamount) {
			if ((fieldid < 1600 && fieldid != 956 && fieldid != 1250) || fieldid == 1816 || fieldid == 1851 || fieldid == 1854 || fieldid == 1864 || fieldid == 2054 || fieldid == 2105 || fieldid == 2114 || fieldid == 2161 || fieldid == 2168 || fieldid == 2453 || fieldid == 2755 || fieldid == 3008 || fieldid == 3009 || fieldid == 3010)
				return 10; // Blank
			else
				return 7; // Amarant
		}
		if (entryindex + 3 == entryamount) {
			if ((fieldid < 1450 && fieldid != 956) || fieldid == 1851 || fieldid == 1854 || fieldid == 1864 || fieldid == 2054 || fieldid == 2114 || fieldid == 2453 || fieldid == 2755 || fieldid == 3008)
				return 9; // Marcus
			else
				return 6; // Eiko
		}
		if (entryindex + 4 == entryamount) {
			if (fieldid < 157 || fieldid == 1851 || fieldid == 1864 || fieldid == 2450 || fieldid == 3008 || fieldid == 3011)
				return 8; // Cinna
			else
				return 5; // Quina
		}
	}
	if (entryindex + SCRIPT_FIXED_ENTRY_AMOUNT >= entryamount)
		return entryindex + SCRIPT_FIXED_ENTRY_AMOUNT - entryamount;
	return -1;
}

// That whole thing is meant only for importing .hws of v0.50b and older
struct EntryIdentifyingInfos {
	uint8_t entrytype = 255;
	int playerlink = -1;
	int modelid = -1;
	int regionx = 0;
	int regiony = 0;
	bool emptyentry = false;
	bool entryfollow = false;
	bool codesps = false;
	bool codetile = false;
	bool codeshared = false;
	bool coderegion = false;
	bool codevariable = true;

	string ToString() {
		return wxString::Format(wxT("entrytype %d, emptyentry %d, playerlink %d, modelid %d, coderegion %d, regionx %d, regiony %d, entryfollow %d, codesps %d, codetile %d, codeshared %d, codevariable %d"), entrytype, emptyentry, playerlink, modelid, coderegion, regionx, regiony, entryfollow, codesps, codetile, codeshared, codevariable).ToStdString();
	}

	bool CompareWith(EntryIdentifyingInfos& other) {
		//if (playerlink >= 0 || other.playerlink >= 0)
		//	return playerlink == other.playerlink;
		//if (entrytype != other.entrytype)
		//	return false;
		if (entrytype == other.entrytype) {
			if (emptyentry && other.emptyentry)
				return true;
			if (entrytype == 2) {
				if (modelid >= 0 || other.modelid >= 0)
					return modelid == other.modelid;
				return entryfollow == other.entryfollow;
			}
			if (entrytype == 1) {
				if (coderegion && other.coderegion)
					return abs(regionx - other.regionx) < 10 && abs(regiony - other.regiony) < 10;
				if (coderegion != other.coderegion)
					return false;
				if ((codeshared && other.codeshared) || (codetile && other.codetile) || (codesps && other.codesps))
					return true;
				return (codeshared == other.codeshared) && (codetile == other.codetile) && (codesps == other.codesps);
			}
			if (entrytype == 0) {
				if ((codeshared && other.codeshared) || (codetile && other.codetile) || (codesps && other.codesps) || (codevariable && other.codevariable))
					return true;
				return (codeshared == other.codeshared) && (codetile == other.codetile) && (codesps == other.codesps) && (codevariable == other.codevariable);
			}
			return true;
		}
		if (modelid >= 0 || other.modelid >= 0)
			return modelid == other.modelid;
		if (coderegion && other.coderegion)
			return abs(regionx - other.regionx) < 10 && abs(regiony - other.regiony) < 10;
		if (coderegion != other.coderegion)
			return false;
		if (emptyentry && other.emptyentry)
			return true;
		return false;
	}
};

EntryIdentifyingInfos GetEntryIdentifyingInfos(ScriptEntry& entry) {
	EntryIdentifyingInfos infos;
	infos.entrytype = entry.type;
	infos.playerlink = entry.player_link;
	infos.emptyentry = entry.function_amount == 0;
	if (entry.function_amount == 0)
		return infos;
	SteamLanguage lang;
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		if (!entry.func[0].indices[lang].empty())
			break;
	if (lang == STEAM_LANGUAGE_AMOUNT)
		return infos;
	unsigned int i, j, k;
	for (i = 0; i < entry.function_amount; i++) {
		for (j = 0; j < entry.func[i].indices[lang].size(); j++) {
			if (entry.func[i].indices[lang][j] >= 0) {
				ScriptOperation& op = entry.func[i].op[entry.func[i].indices[lang][j]];
				if (op.opcode == 0x2F && infos.modelid < 0)
					infos.modelid = op.arg[0].GetValue();
				else if (op.opcode == 0xA1 || op.opcode == 0xA2 || op.opcode == 0xA5 || op.opcode == 0x1D || op.opcode == 0x23 || op.opcode == 0x24)
					infos.entryfollow = true;
				else if (op.opcode == 0x29) {
					infos.coderegion = true;
					float x = 0.0f, y = 0.0f;
					for (k = 0; k < op.arg_amount; k++) {
						x += (int16_t)(op.arg[k].GetValue() & 0xFFFF);
						y += (int16_t)((op.arg[k].GetValue() >> 16) & 0xFFFF);
					}
					if (op.arg_amount > 0) {
						infos.regionx = x / op.arg_amount;
						infos.regiony = y / op.arg_amount;
					}
				} else if (op.opcode == 0xB3 || op.opcode == 0xDA)
					infos.codesps = true;
				else if ((op.opcode >= 0x59 && op.opcode <= 0x65 && op.opcode != 0x62) || op.opcode == 0xE4)
					infos.codetile = true;
				else if (op.opcode == 0x1C && op.arg[0].GetValue() == 255)
					infos.codeshared = true;
				if (op.opcode != 0x05 && op.opcode != 0x05)
					infos.codevariable = false;
			}
		}
	}
	return infos;
}

// Custom script utility externalizes to "ScriptAPI.txt" different functions and variables to be used in scripts
// It should only be updated according to EventEngine API additions made in the Memoria mod
bool CustomScriptsAreLoaded = false;
void LoadCustomScriptUtility() {
	if (CustomScriptsAreLoaded)
		return;
	if (!wxFile::Exists(SCRIPT_CUSTOM_API_FILENAME)) {
		CustomScriptsAreLoaded = true;
		return;
	}
	wxFile apifile(SCRIPT_CUSTOM_API_FILENAME);
	if (!apifile.IsOpened())
		return;
	wxString apistr, token;
	int i, helpi;
	apifile.ReadAll(&apistr);
	apifile.Close();
	while (!apistr.IsEmpty()) {
		token = CS_GetNextWord(apistr);
		if (token.IsSameAs(_(L"Function"))) {
			SortedChoiceItemScriptOpcode newopcode;
			newopcode.use_vararg = true;
			newopcode.jump_pos = 0;
			newopcode.id = wxAtoi(CS_GetNextWord(apistr));
			newopcode.label = CS_GetNextWord(apistr).ToStdWstring();
			newopcode.help = CS_GetQuotedString(apistr).ToStdWstring();
			newopcode.arg_amount = wxAtoi(CS_GetNextWord(apistr));
			newopcode.arg_length = new uint8_t[newopcode.arg_amount];
			newopcode.arg_help = new wstring[newopcode.arg_amount];
			newopcode.arg_type = new uint8_t[newopcode.arg_amount];
			helpi = 0;
			for (i = 0; i < newopcode.arg_amount; i++) {
				newopcode.arg_length[i] = 3;
				newopcode.arg_type[i] = SCRIPT_ARG_TYPE_MAP[CS_GetNextWord(apistr).ToStdWstring()];
				if (newopcode.arg_type[i] == AT_NONE)
					newopcode.arg_type[i] = AT_USPIN;
				if (newopcode.arg_type[i] != AT_NONE && /*
				 */ newopcode.arg_type[i] != AT_POSITION_Y && newopcode.arg_type[i] != AT_POSITION_Z && /*
				 */ newopcode.arg_type[i] != AT_COLOR_MAGENTA && newopcode.arg_type[i] != AT_COLOR_YELLOW && /*
				 */ newopcode.arg_type[i] != AT_COLOR_GREEN && newopcode.arg_type[i] != AT_COLOR_BLUE)
					newopcode.arg_help[helpi++] = CS_GetQuotedString(apistr).ToStdWstring();
			}
			HADES_STRING_SCRIPT_OPCODE[newopcode.id] = newopcode;
		} else if (token.IsSameAs(_(L"Variable"))) {
			VariableName newvar;
			token = CS_GetNextWord(apistr);
			if (token.IsSameAs(L"GETTER"))
				newvar.cat = VARCODE_CUSTOM_GETTER;
			else if (token.IsSameAs(L"ENGINE"))
				newvar.cat = VARCODE_ENGINE;
			else
				newvar.cat = wxAtoi(token);
			newvar.id = wxAtoi(CS_GetNextWord(apistr));
			newvar.name = CS_GetNextWord(apistr).ToStdWstring();
			newvar.description = CS_GetQuotedString(apistr).ToStdWstring();
			VarNameList.push_back(newvar);
		} else if (token.IsSameAs(_(L"VarCode"))) {
			FlexibleFunctionName newcode;
			newcode.id = wxAtoi(CS_GetNextWord(apistr));
			newcode.name = CS_GetNextWord(apistr).ToStdWstring();
			newcode.description = CS_GetQuotedString(apistr).ToStdWstring();
			newcode.argcount = wxAtoi(CS_GetNextWord(apistr));
			FlexibleFunctionNameList[newcode.id] = newcode;
		}
	}
	CustomScriptsAreLoaded = true;
}

bool ScriptLanguageLink::InitFromHWS(fstream& ffhws, SteamLanguage baselang) {
	bool shouldread = !hades::STEAM_SINGLE_LANGUAGE_MODE || baselang == GetSteamLanguage();
	uint32_t langdatasize;
	unsigned int i, j;
	lang_base = baselang;
	HWSReadChar(ffhws, lang_link_count);
	lang_link.resize(lang_link_count);
	id_count.resize(lang_link_count);
	id_base.resize(lang_link_count);
	id_link.resize(lang_link_count);
	for (i = 0; i < lang_link_count; i++) {
		HWSReadChar(ffhws, lang_link[i]);
		if (hades::STEAM_SINGLE_LANGUAGE_MODE && lang_link[i] == GetSteamLanguage())
			shouldread = true;
		HWSReadLong(ffhws, langdatasize);
		HWSReadShort(ffhws, id_count[i]);
		id_base[i].resize(id_count[i]);
		id_link[i].resize(id_count[i]);
		for (j = 0; j < id_count[i]; j++) {
			HWSReadShort(ffhws, id_base[i][j]);
			HWSReadShort(ffhws, id_link[i][j]);
		}
	}
	return shouldread;
}

void ScriptLanguageLink::Apply(vector<ScriptOperation>& baselangop, int linkindex) {
	unsigned int i, linkj;
	int j;
	modarg.clear();
	modbaseval.clear();
	for (i = 0; i < baselangop.size(); i++) {
		SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[baselangop[i].opcode];
		for (j = 0; j < scriptop.arg_amount; j++) {
			if (scriptop.arg_type[j] == AT_TEXT && !baselangop[i].arg[j].is_var) {
				for (linkj = 0; linkj < id_count[linkindex]; linkj++) {
					if (id_base[linkindex][linkj] == baselangop[i].arg[j].GetValue()) {
						baselangop[i].arg[j].SetValue(id_link[linkindex][linkj]);
						modarg.push_back(&baselangop[i].arg[j]);
						modbaseval.push_back(id_base[linkindex][linkj]);
						break;
					}
				}
			}
		}
	}
}

void ScriptLanguageLink::RevertLastApply() {
	for (unsigned int i = 0; i < modarg.size(); i++)
		modarg[i]->SetValue(modbaseval[i]);
	modarg.clear();
	modbaseval.clear();
}

bool ScriptArgument::SetValue(long long newvalue) {
	if (newvalue < 0) {
		uint32_t sign = 0x1 << (typesize * 8 - 1);
		value = ~(-newvalue - 1 & ~sign) | sign;
	} else {
		value = newvalue;
	}
	size = typesize;
	is_var = false;
	var.clear();
	return true;
}

void ScriptArgument::SetValueVar(vector<uint8_t> newvaluevar) {
	var = newvaluevar;
	size = newvaluevar.size();
	is_var = true;
}

int64_t ScriptArgument::GetValue() const {
	if (is_signed) {
		if (typesize == 1) {
			int8_t* num = (int8_t*)&value;
			return *num;
		} else if (typesize == 2) {
			int16_t* num = (int16_t*)&value;
			return *num;
		}
		int32_t* num = (int32_t*)&value;
		return *num;
	} else {
		if (typesize == 1) {
			uint8_t* num = (uint8_t*)&value;
			return *num;
		} else if (typesize == 2) {
			uint16_t* num = (uint16_t*)&value;
			return *num;
		}
	}
	return value;
}

#define MACRO_SCRIPT_IOFUNCTION_ARGREAD(IO, SEEK) \
	unsigned int i; \
	uint8_t varbyte; \
	typesize = sz; \
	if (is_var) { \
		int vartype; \
		size = 0; \
		IO ## Char(f, varbyte); var.push_back(varbyte); \
		vartype = VarOpList[varbyte].type; \
		while (vartype != -1) { \
			if (vartype == 2) { \
			} else if (vartype == 3) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype == 5) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype == 6) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype == 7) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype >= 10 && vartype < 20) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype >= 20 && vartype < 30) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype == 55) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} else if (vartype == 60) { \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
				IO ## Char(f, varbyte); var.push_back(varbyte); \
			} \
			IO ## Char(f, varbyte); var.push_back(varbyte); \
			vartype = VarOpList[varbyte].type; \
		} \
		size = var.size(); \
	} else { \
		size = typesize; \
		value = 0; \
		for (i = 0; i < size; i++) { \
			IO ## Char(f, varbyte); \
			value |= (varbyte << i*8); \
		} \
	}

#define MACRO_SCRIPT_IOFUNCTION_ARGWRITE(IO, SEEK, PPF) \
	unsigned int i; \
	uint8_t buffer; \
	if (PPF) PPFInitScanStep(f); \
	if (is_var) \
		for (i = 0; i < size; i++) \
			IO ## Char(f, var[i]); \
	else if (size != 2 || is_signed) \
		for (i = 0; i < size; i++) { \
			buffer = (value >> i*8) & 0xFF; \
			IO ## Char(f, buffer); \
		} \
	else { \
		uint16_t translatedval = GetValue(); \
		for (i = 0; i < size; i++) { \
			buffer = (translatedval >> i*8) & 0xFF; \
			IO ## Char(f, buffer); \
		} \
	} \
	if (PPF) PPFEndScanStep();


void ScriptArgument::Read(fstream& f, uint8_t sz, bool isvar, bool issign) {
	is_var = isvar;
	is_signed = issign;
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_SCRIPT_IOFUNCTION_ARGREAD(FFIXRead, FFIXSeek)
	} else {
		MACRO_SCRIPT_IOFUNCTION_ARGREAD(SteamRead, SteamSeek)
	}
}

void ScriptArgument::Write(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(FFIXWrite, FFIXSeek, false)
}

void ScriptArgument::WritePPF(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(PPFStepAdd, FFIXSeek, true)
}

void ScriptArgument::ReadHWS(fstream& f, uint8_t sz, bool isvar, bool issign) {
	is_var = isvar;
	is_signed = issign;
	MACRO_SCRIPT_IOFUNCTION_ARGREAD(HWSRead, HWSSeek)
}

void ScriptArgument::WriteHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(HWSWrite, HWSSeek, false)
}

void ScriptArgument::WriteSteam(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(SteamWrite, SteamSeek, false)
}

inline void ScriptOperation::InitialiseArgumentVector(unsigned int count) {
	arg.resize(count);
	for (unsigned int i = 0; i < count; i++) {
		arg[i].parent = this;
		arg[i].var.clear();
	}
}

inline bool IsScriptArgTypeSigned(uint8_t argtype) {
	return argtype == AT_JUMP || argtype == AT_SPIN || argtype == AT_POSITION_X || argtype == AT_POSITION_Y || argtype == AT_POSITION_Z;
}

inline bool IsScriptArgTypeEntry(const ScriptOperation& op, int argindex) {
	if (HADES_STRING_SCRIPT_OPCODE[op.opcode].arg_type[argindex] == AT_ENTRY)
		return true;
	if ((op.opcode == 0xB3 || op.opcode == 0xDA) && argindex == 2 && !op.arg[1].is_var && op.arg[1].value == 150)
		return true;
	return false;
}

#define MACRO_SCRIPT_IOFUNCTION_OPREAD(IO, SEEK, FUNC) \
	unsigned int i; \
	uint8_t opcodebyte; \
	opcode = 0; \
	size = 1; \
	IO ## Char(f, opcodebyte); \
	while (opcodebyte == 0xFF) { \
		opcode += 0x100; \
		size++; \
		IO ## Char(f, opcodebyte); \
	} \
	opcode += opcodebyte; \
	if (opcode == 0x06) { \
		IO ## Char(f, size_byte); \
		arg_amount = 1 + size_byte * 2; \
		InitialiseArgumentVector(arg_amount); \
		arg[0].FUNC(f, 2, false, true); \
		for (i = 0; i < size_byte; i++) { \
			arg[2 * i + 1].FUNC(f, 2, false, false); \
			arg[2 * i + 2].FUNC(f, 2, false, true); \
		} \
		size += 3 + size_byte * 4; \
	} else if (opcode == 0x0B) { \
		IO ## Char(f, size_byte); \
		arg_amount = 2 + size_byte; \
		InitialiseArgumentVector(arg_amount); \
		arg[0].FUNC(f, 2, false, false); \
		for (i = 1; i < arg_amount; i++) \
			arg[i].FUNC(f, 2, false, true); \
		size += 5 + size_byte * 2; \
	} else if (opcode == 0x29) { \
		uint8_t flag = 1; \
		IO ## Char(f, vararg_flag); \
		IO ## Char(f, size_byte); \
		arg_amount = size_byte; \
		InitialiseArgumentVector(arg_amount); \
		size += 2; \
		for (i = 0; i < arg_amount; i++) { \
			arg[i].FUNC(f, 4, flag & vararg_flag, true); \
			size += arg[i].size; \
			flag <<= 1; \
		} \
	} else { \
		SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[opcode]; \
		arg_amount = scriptop.arg_amount; \
		InitialiseArgumentVector(arg_amount); \
		if (scriptop.use_vararg) { \
			uint8_t flag = 1; \
			IO ## Char(f, vararg_flag); \
			size += 1; \
			for (i = 0; i < arg_amount; i++) { \
				arg[i].FUNC(f, scriptop.arg_length[i], flag & vararg_flag, IsScriptArgTypeSigned(scriptop.arg_type[i])); \
				size += arg[i].size; \
				flag <<= 1; \
			} \
		} else { \
			for (i = 0; i < arg_amount; i++) { \
				arg[i].FUNC(f, scriptop.arg_length[i], opcode == 0x05, IsScriptArgTypeSigned(scriptop.arg_type[i])); \
				size += arg[i].size; \
			} \
		} \
		if (EntryIdShift != 0) { \
			for (i = 0; i < arg_amount; i++) { \
				if (!arg[i].is_var && IsScriptArgTypeEntry(*this, i)) { \
					if ((int)arg[i].value >= EntryIdShift_Base && arg[i].value < ENTRY_MAX_VALID_ID) \
						arg[i].value += EntryIdShift; \
				} else if (arg[i].is_var) { \
					int varargtype; \
					for (unsigned int m = 0; m < arg[i].size; m++) { \
						varargtype = VarOpList[arg[i].var[m]].type; \
						if (varargtype == 55) { \
							if (arg[i].var[m + 1] > EntryIdShift_Base && arg[i].var[m + 1] < ENTRY_MAX_VALID_ID) \
								arg[i].var[m + 1] += EntryIdShift; \
							m += 2; \
						} else if (varargtype == 3 || varargtype == 5 || (varargtype >= 10 && varargtype < 20)) { \
							if (arg[i].var[m] == 0x5F && arg[i].var[m + 1] > EntryIdShift_Base && arg[i].var[m + 1] < ENTRY_MAX_VALID_ID) \
								arg[i].var[m + 1] += EntryIdShift; \
							m++; \
						} else if (varargtype == 6 || (varargtype >= 20 && varargtype < 30)) { \
							m += 2; \
						} else if (varargtype == 7) { \
							m += 4; \
						} else if (varargtype == 60) { \
							m += 3; \
						} \
					} \
				} \
			} \
		} \
	}

#define MACRO_SCRIPT_IOFUNCTION_OPWRITE(IO, SEEK, PPF, FUNC) \
	unsigned int i; \
	uint16_t opcodecopy = opcode; \
	if (PPF) PPFInitScanStep(f); \
	while (opcodecopy >= 0x100) { \
		IO ## Char(f, 0xFF); \
		opcodecopy -= 0x100; \
	} \
	IO ## Char(f, opcodecopy); \
	if (opcode == 0x06) { \
		IO ## Char(f, size_byte); \
		if (PPF) PPFEndScanStep(); \
		arg[0].FUNC(f); \
		for (i = 1; i < arg_amount; i++) \
			arg[i].FUNC(f); \
	} else if (opcode == 0x0B) { \
		IO ## Char(f, size_byte); \
		if (PPF) PPFEndScanStep(); \
		arg[0].FUNC(f); \
		arg[1].FUNC(f); \
		for (i = 2; i < arg_amount; i++) \
			arg[i].FUNC(f); \
	} else if (opcode == 0x29) { \
		IO ## Char(f, vararg_flag); \
		IO ## Char(f, size_byte); \
		if (PPF) PPFEndScanStep(); \
		for (i = 0; i < arg_amount; i++) \
			arg[i].FUNC(f); \
	} else { \
		if (HADES_STRING_SCRIPT_OPCODE[opcode].use_vararg) { \
			IO ## Char(f, vararg_flag); \
			if (PPF) PPFEndScanStep(); \
			for (i = 0; i < arg_amount; i++) \
				arg[i].FUNC(f); \
		} else { \
			if (PPF) PPFEndScanStep(); \
			for (i = 0; i < arg_amount; i++) \
				arg[i].FUNC(f); \
		} \
	}

inline bool AreVerySimilarOpcodes(uint16_t op1, uint16_t op2) {
	// "RunScript" opcodes are sometimes changed to other "RunScript" opcodes in different languages (field 1755, Function Zidane_13)
	if ((op1 == 0x10 || op1 == 0x12 || op1 == 0x14) && (op2 == 0x10 || op2 == 0x12 || op2 == 0x14))
		return true;
	// No other instances in vanilla, don't check for other very similar opcodes (like "Window(A)sync")
	return false;
}

inline bool IsDebugCutsceneSwitch(SteamLanguage lang) {
	// The way that bug is handled currently doesn't generate optimal codes (the whole function is duplicated for the JP language)

	// Function GrandpaA_Loop of field 457 (Mountain/Shack)
	//  switchex 10 ( VAR_GlobInt16_28 ) {
	//  [...] (same in all languages)
	//  default:
	//    [...]
	//    [jp] RunScriptSync( 4, 4, 22 )
	//    [~jp] if ( VAR_GlobInt8_34 == 0 ) { RunScriptSync( 4, 4, 22 ) }
	//    [...]
	//  }
	//  Wait( 1 )
	//  loop
	if (CurrentScriptReadField == 457 && CurrentScriptReadEntry == 4 && CurrentScriptReadFunction == 1)
		return lang == STEAM_LANGUAGE_JA;

	// Function Vivi_Loop of field 661 (Marsh/Master's House)
	//  switchex 6 ( VARL_GenUInt8_408 ) {
	//  [...]
	//  case 20:
	//    [...]
	//    [Some code, overall very similar but in different order in jp and ~jp]
	//    [...]
	//  }
	//  Wait( 1 )
	//  loop
	if (CurrentScriptReadField == 661 && CurrentScriptReadEntry == 8 && CurrentScriptReadFunction == 1)
		return lang == STEAM_LANGUAGE_JA;

	// Function Main_Init of field 1052 (Cleyra/Sandpit)
	//   [...]
	//   switch 3 ( General_ScenarioCounter ) from 4725 {
	//     [...]
	//   default:
	//     set VAR_GlobUInt8_24 = 0
	//     [jp] set VAR_GenInt16_239 = 1052
	//     [jp] InitCode(2, 0) // Entry2
	//     [jp] set VAR_GlobBool_153 = 1
	//     [...]
	//   }
	//   [jp] [...]
	//   [...]
	//   return
	if (CurrentScriptReadField == 1052 && CurrentScriptReadEntry == 0 && CurrentScriptReadFunction == 0)
		return lang == STEAM_LANGUAGE_JA;
	return false;
}

inline bool IsDebugIifaCutsceneSwitch() {
	// Function Dagger_Loop of field 1663 (Iifa Tree/Tree Trunk)
	//   switchex 15 ( VAR_GlobInt16_47 ) {
	//     [...]
	//   case 124:
	//     [...]
	//     [us,jp] WindowAsync( 3, 128, "Dagger: No... I can't... I can't let anything happen to my mother." )
	//     [us,jp] WaitAnimation(  )
	//     [uk,gr,fr,it,es] WaitAnimation(  )
	//     [uk,gr,fr,it,es] WindowAsync( 3, 128, "Dagger: No... I can't... I can't let anything happen to my mother." )
	//     [...]
	//   }
	//   [...]
	//   return
	if (CurrentScriptReadField == 1663 && CurrentScriptReadEntry == 20 && CurrentScriptReadFunction == 1)
		return true;
	return false;
}

int ScriptOperation::Compare(const ScriptOperation& other) const {
	if (opcode != other.opcode || arg_amount != other.arg_amount)
		return 3;
	unsigned int i, j;
	int cmp = 0;
	for (i = 0; i < arg_amount; i++) {
		const ScriptArgument& thisarg = arg[i];
		const ScriptArgument& otherarg = other.arg[i];
		if (thisarg.is_var != otherarg.is_var)
			return 3;
		if (thisarg.is_var) {
			if (thisarg.size != otherarg.size)
				return 3;
			for (j = 0; j < thisarg.size; j++)
				if (thisarg.var[j] != otherarg.var[j])
					cmp = 2;
		} else {
			if (cmp < 1 && thisarg.value != otherarg.value)
				cmp = 1;
		}
	}
	return cmp;
}

void ScriptOperation::Read(fstream& f) {
	if (GetGameType() == GAME_TYPE_PSX) {
		MACRO_SCRIPT_IOFUNCTION_OPREAD(FFIXRead, FFIXSeek, Read)
	} else {
		MACRO_SCRIPT_IOFUNCTION_OPREAD(SteamRead, SteamSeek, Read)
	}
}

void ScriptOperation::Write(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(FFIXWrite, FFIXSeek, false, Write)
}

void ScriptOperation::WritePPF(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(PPFStepAdd, FFIXSeek, true, WritePPF)
}

void ScriptOperation::ReadHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPREAD(HWSRead, HWSSeek, ReadHWS)
}

void ScriptOperation::WriteHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(HWSWrite, HWSSeek, false, WriteHWS)
}

void ScriptOperation::WriteSteam(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(SteamWrite, SteamSeek, false, WriteSteam)
}

ScriptOperation& ScriptOperation::operator=(const ScriptOperation& from) {
	parent = from.parent;
	opcode = from.opcode;
	vararg_flag = from.vararg_flag;
	size_byte = from.size_byte;
	arg_amount = from.arg_amount;
	size = from.size;
	arg = from.arg;
	for (unsigned int i = 0; i < arg_amount; i++)
		arg[i].parent = this;
	return *this;
}

ScriptOperation::ScriptOperation(const ScriptOperation& from) :
	parent(from.parent),
	opcode(from.opcode),
	vararg_flag(from.vararg_flag),
	size_byte(from.size_byte),
	arg_amount(from.arg_amount),
	size(from.size),
	arg(from.arg) {
	for (unsigned int i = 0; i < arg_amount; i++)
		arg[i].parent = this;
}

void ScriptFunction::SetDefaultScriptForced() {
	ScriptOperation returnop;
	returnop.parent = this;
	returnop.opcode = 0x04;
	returnop.vararg_flag = 0;
	returnop.size_byte = 0;
	returnop.arg_amount = 0;
	returnop.size = 1;
	returnop.arg.clear();
	op.clear();
	op.push_back(returnop);
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
		indices[lang].clear();
		if (parent->has_language & (1 << lang))
			indices[lang].push_back(0);
	}
}

uint16_t GetControlVarArgCode(const vector<ScriptOperation>& oplist, unsigned int pos, unsigned int nextpos) {
	if (oplist[pos].opcode != 0x05 || nextpos >= oplist.size())
		return 0;
	if (oplist[nextpos].opcode != 0x02 && oplist[nextpos].opcode != 0x03 && oplist[nextpos].opcode != 0x06 && oplist[nextpos].opcode != 0x0B)
		return 0;
	return oplist[nextpos].opcode;
}

bool dbgscript = wxFileName::FileExists("LogScripts.txt");
bool ScriptFunction::SearchOperationCorrespondance(const vector<ScriptOperation>& langop, SteamLanguage lang, unsigned int& langindex, unsigned int& searchindex, unsigned int& codecount, unsigned int codediff, set<unsigned int>& elsejumpoffsets, unsigned int& codeoffset) {
	unsigned int i, j, langsearchindex;
	SteamLanguage lg;
	uint16_t controlvararg = GetControlVarArgCode(langop, langindex, langindex + 1);
	if (controlvararg == 0x02 && langop[langindex].arg[0].GetValue() > 0)
		elsejumpoffsets.insert((unsigned int)(codeoffset + langop[langindex].arg[0].GetValue()));
	for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
		if (lg == lang || indices[lg].empty())
			continue;
		langsearchindex = searchindex;
		while (langsearchindex < codecount && indices[lg][langsearchindex] < 0)
			langsearchindex++;
		if (langsearchindex >= codecount)
			continue;
		int cmp = langop[langindex].Compare(op[indices[lg][langsearchindex]]);
		if (cmp == 0) {
			// Exact match
			if (controlvararg == 0 || langop[langindex + 1].Compare(op[indices[lg][langsearchindex + 1]]) == 0) {
				if (dbgscript) {
					GetDebugLog() << "Exact match: " << (int)lang << " " << (int)lg << " -> " << (int)langindex << " " << (int)langsearchindex << " [" << ConvertWStrToStr(HADES_STRING_SCRIPT_OPCODE[langop[langindex].opcode].label).c_str() << "]" << endl;
				}
				// TODO: Similar JUMP with different offsets currently don't match (more duplicated functions but easier "indices" structure)
				indices[lang].insert(indices[lang].end(), langsearchindex - searchindex, -1);
				indices[lang].push_back(indices[lg][langsearchindex]);
				searchindex = langsearchindex + 1;
				return true;
			}
		}
	}
	for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
		if (lg == lang || indices[lg].empty())
			continue;
		langsearchindex = searchindex;
		while (langsearchindex < codecount && indices[lg][langsearchindex] < 0)
			langsearchindex++;
		if (langsearchindex >= codecount)
			continue;
		int cmp = langop[langindex].Compare(op[indices[lg][langsearchindex]]);
		if (cmp == 1 || cmp == 2) {
			if (dbgscript) {
				GetDebugLog() << "SEARCH SIMILAR? " << langsearchindex << " " << codecount << " " << (int)indices[lg].size() << endl;
			}
			uint16_t othercontrolvararg = langsearchindex + 1 < codecount ? GetControlVarArgCode(op, indices[lg][langsearchindex], indices[lg][langsearchindex + 1]) : 0;
			if (controlvararg == othercontrolvararg) {
				// Variation of the same code
				if ((controlvararg == 0 || langop[langindex + 1].Compare(op[indices[lg][langsearchindex + 1]]) == 0) && (langop[langindex].opcode != 0x01 || elsejumpoffsets.find(codeoffset) != elsejumpoffsets.end())) {
					if (dbgscript) {
						GetDebugLog() << "Similar match: " << (int)lang << " " << (int)lg << " -> " << (int)langindex << " " << (int)langsearchindex << " [" << ConvertWStrToStr(HADES_STRING_SCRIPT_OPCODE[langop[langindex].opcode].label).c_str() << "]" << endl;
					}
					// TODO: Similar JUMP with different offsets currently don't match (more duplicated functions but easier "indices" structure), except "else" JUMP instructions and some cutscene switch debug ("switchex" opcode is same for all languages but the "break" inside are not because of the "default" case)
					indices[lang].insert(indices[lang].end(), langsearchindex - searchindex, -1);
					indices[lang].push_back(op.size());
					op.push_back(langop[langindex]);
					searchindex = langsearchindex + 1;
					return true;
				}
			}
		}
	}
	// Special gap: very similar opcodes (most likely the same arguments and sizes, but with different albeit similar opcodes)
	// Do it just for the one occurrence where it's effective in vanilla, although it might be extended
	if (CurrentScriptReadField == 1755) {
		for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (lg == lang || indices[lg].empty())
				continue;
			langsearchindex = searchindex;
			while (langsearchindex < codecount && indices[lg][langsearchindex] < 0)
				langsearchindex++;
			if (langsearchindex >= codecount)
				continue;
			if (langop[langindex].size == op[indices[lg][langsearchindex]].size && AreVerySimilarOpcodes(langop[langindex].opcode, op[indices[lg][langsearchindex]].opcode)) {
				if (dbgscript) {
					GetDebugLog() << "Very similar opcode match: " << (int)lang << " " << (int)lg << " -> " << (int)langop[langindex].opcode << "/" << (int)op[indices[lg][langsearchindex]].opcode << " (size = " << langop[langindex].size << ")" << endl;
				}
				indices[lang].insert(indices[lang].end(), langsearchindex - searchindex + 1, -1);
				indices[lang].push_back(op.size());
				op.push_back(langop[langindex]);
				for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
					if (lg != lang && !indices[lg].empty())
						indices[lg].insert(indices[lg].begin() + langsearchindex + 1, 1, -1);
				searchindex = langsearchindex + 2;
				codecount++;
				return true;
			}
		}
	}
	if (IsDebugIifaCutsceneSwitch() && langindex + 1 < langop.size() && controlvararg == 0) {
		for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (lg == lang || indices[lg].empty())
				continue;
			langsearchindex = searchindex;
			while (langsearchindex < codecount && indices[lg][langsearchindex] < 0)
				langsearchindex++;
			if (langsearchindex + 1 >= codecount)
				continue;
			int cmp1 = langop[langindex + 1].Compare(op[indices[lg][langsearchindex]]);
			int cmp2 = langop[langindex].Compare(op[indices[lg][langsearchindex + 1]]);
			if (cmp1 <= 2 && cmp2 <= 2) {
				if (dbgscript) {
					GetDebugLog() << "Debug Iifa switch match: " << (int)lang << " " << (int)lg << " -> " << (int)langop[langindex].opcode << " and " << (int)langop[langindex + 1].opcode << " at " << langindex << "/" << langsearchindex << endl;
				}
				indices[lang].insert(indices[lang].end(), langsearchindex - searchindex + 2, -1);
				indices[lang].push_back(op.size());
				op.push_back(langop[langindex++]);
				indices[lang].push_back(op.size());
				op.push_back(langop[langindex]);
				for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
					if (lg != lang && !indices[lg].empty())
						indices[lg].insert(indices[lg].begin() + langsearchindex + 2, 2, -1);
				searchindex = langsearchindex + 4;
				codecount += 2;
				return true;
			}
		}
	}
	return false; // TODO: never search for a gap in the code flow, mark the whole function as language-different instead
	// Search for a gap in a code flow (langop either lacks a part or has some more than the others)
	for (i = 1; i < codediff; i++) {
		for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (lg == lang || indices[lg].empty())
				continue;
			if (searchindex + i < codecount && indices[lg][searchindex + i] >= 0) {
				int cmp = langop[langindex].Compare(op[indices[lg][searchindex + i]]);
				if (cmp == 0) {
					// Assume langop lacks a code block of i operations that other languages have
					indices[lang].insert(indices[lang].end(), i, -1);
					indices[lang].push_back(indices[lg][searchindex + i]);
					searchindex += i + 1;
					return true;
				}
			}
			if (langindex + i < langop.size() && indices[lg][searchindex] >= 0) {
				int cmp = langop[langindex + i].Compare(op[indices[lg][searchindex]]);
				if (cmp == 0) {
					controlvararg = GetControlVarArgCode(langop, langindex + i, langindex + i + 1);
					uint16_t othercontrolvararg = searchindex + 1 < codecount ? GetControlVarArgCode(op, indices[lg][searchindex], indices[lg][searchindex + 1]) : 0;
					if (controlvararg == othercontrolvararg) {
						// Assume langop has a code block of i operations that other languages don't have
						for (j = 0; j < i; j++) {
							indices[lang].push_back(op.size());
							op.push_back(langop[langindex]);
							codeoffset += langop[langindex++].size;
						}
						indices[lang].push_back(indices[lg][searchindex]);
						for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
							if (lg != lang && !indices[lg].empty())
								indices[lg].insert(indices[lg].begin() + searchindex, i, -1);
						searchindex += i + 1;
						codecount += i;
						return true;
					}
				}
			}
		}
	}
	return false;
}

void ScriptFunction::Import(const vector<ScriptOperation>& langop, SteamLanguage lang) {
	unsigned int langindex;
	SteamLanguage lg;
	bool shouldflush = indices[lang].size() > 0;
	indices[lang].clear();
	if (dbgscript) {
		GetDebugLog() << "START IMPORT " << HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX[lang] << " (" << (int)langop.size() << ")" << endl;
		GetDebugLog() << "LANGS:";
		for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
			GetDebugLog() << " " << (int)indices[lg].size();
		GetDebugLog().flush();
	}
	unsigned int codecount = 0, validcodecount = 0, searchindex = 0, codeoffset = 0;
	set<unsigned int> elsejumpoffsets;
	for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
		if (lg != lang && !indices[lg].empty()) {
			unsigned int langvalidcodecount = 0;
			codecount = indices[lg].size();
			for (langindex = 0; langindex < indices[lg].size(); langindex++)
				if (indices[lg][langindex] >= 0)
					langvalidcodecount++;
			validcodecount = max(validcodecount, langvalidcodecount);
		}
	}
	int codediff = max(20, abs((int)validcodecount - (int)langop.size()));
	if (dbgscript) {
		GetDebugLog() << " -> " << codecount << " " << validcodecount << " " << codediff << endl;
	}
	if (shouldflush && codecount == 0) {
		op.clear();
		shouldflush = false;
	}
	if (IsDebugCutsceneSwitch(lang)) {
		langindex = 0;
	} else {
		for (langindex = 0; langindex < langop.size(); langindex++) {
			if (searchindex >= codecount)
				break;
			if (SearchOperationCorrespondance(langop, lang, langindex, searchindex, codecount, codediff, elsejumpoffsets, codeoffset)) {
				uint16_t controlvararg = GetControlVarArgCode(langop, langindex, langindex + 1);
				if (controlvararg != 0) { // Keep control codes next to their var_arg setup
					bool foundmatch = false;
					codeoffset += langop[langindex++].size;
					if (controlvararg == 0x02 && langop[langindex].arg[0].GetValue() > 0)
						elsejumpoffsets.insert((unsigned int)(codeoffset + langop[langindex].arg[0].GetValue()));
					if (searchindex < codecount) {
						for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
							if (lg != lang && !indices[lg].empty() && indices[lg][searchindex] >= 0) {
								uint16_t othercontrolvararg = op[indices[lg][searchindex]].opcode;
								if (controlvararg == othercontrolvararg && langop[langindex].Compare(op[indices[lg][searchindex]]) == 0) {
									indices[lang].push_back(indices[lg][searchindex]);
									foundmatch = true;
									break;
								}
							}
						}
					}
					if (!foundmatch) {
						indices[lang].push_back(op.size());
						op.push_back(langop[langindex]);
					}
					searchindex++;
				}
			} else {
				break; // TODO: never search for a gap in the code flow, mark the whole function as language-different instead
				// Cannot find any correspondance: treat as distinct operations
				uint16_t controlvararg = GetControlVarArgCode(langop, langindex, langindex + 1);
				unsigned int distinctcount = controlvararg != 0 ? 2 : 1; // If it is a controlvararg, treat the next one as distinct as well
				indices[lang].push_back(op.size());
				op.push_back(langop[langindex]);
				for (unsigned int dc = 1; dc < distinctcount; dc++) {
					codeoffset += langop[langindex++].size;
					indices[lang].push_back(op.size());
					op.push_back(langop[langindex]);
				}
				for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
					if (lg != lang && !indices[lg].empty())
						indices[lg].insert(indices[lg].begin() + searchindex, distinctcount, -1);
				searchindex += distinctcount;
				codecount += distinctcount;
				if (validcodecount >= langop.size()) {
					// Advance the searchindex if there are more opcodes in other languages than in lang
					// TODO: compare the *remaining* opcode counts instead?
					indices[lang].push_back(-1);
					searchindex++;
					uint16_t othercontrolvararg = 0;
					if (searchindex < codecount)
						for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
							if (lg != lang && !indices[lg].empty() && indices[lg][searchindex] >= 0)
								othercontrolvararg = op[indices[lg][searchindex]].opcode;
					if (othercontrolvararg == 0x02 || othercontrolvararg == 0x03 || othercontrolvararg == 0x06 || othercontrolvararg == 0x0B) {
						// Keep (already in place) control codes next to their var_arg setup
						indices[lang].push_back(-1);
						searchindex++;
					}
				}
			}
			codeoffset += langop[langindex].size;
		}
	}
	unsigned int addop = op.size();
	if (langindex < langop.size())
		op.insert(op.end(), langop.begin() + langindex, langop.end());
	if (searchindex < codecount)
		indices[lang].insert(indices[lang].end(), codecount - searchindex, -1);
	while (langindex < langop.size()) {
		for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (lg == lang)
				indices[lg].push_back(addop++);
			else if (!indices[lg].empty())
				indices[lg].push_back(-1);
		}
		codeoffset += langop[langindex++].size;
		codecount++;
	}
	if (codecount > 0) {
		while (codecount < indices[lang].size()) {
			for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
				if (lg != lang && !indices[lg].empty())
					indices[lg].push_back(-1);
			codecount++;
		}
	}
	if (dbgscript) {
		GetDebugLog() << "END IMPORT: " << langindex << " " << codecount << " " << (int)langop.size() << " " << (int)op.size() << endl;
	}
	if (shouldflush)
		FlushUnusedOperations();
	if (!shouldflush && dbgscript)
		GetDebugLog() << "FLUSH SKIPPED" << endl;
}

void ScriptFunction::FlushUnusedOperations() {
	unsigned int dbgflushcount = 0;
	SteamLanguage lg;
	int i, indexi;
	for (i = 0; i < (int)op.size(); i++) {
		bool opflush = true;
		for (lg = 0; opflush && lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			for (indexi = i; indexi < (int)indices[lg].size(); indexi++) {
				if (indices[lg][indexi] == i) {
					opflush = false;
					break;
				}
			}
			if (opflush) {
				for (indexi = 0; indexi < i && indexi < (int)indices[lg].size(); indexi++) {
					if (indices[lg][indexi] == i) {
						opflush = false;
						break;
					}
				}
			}
		}
		if (opflush) {
			dbgflushcount++;
			op.erase(op.begin() + i);
			for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
				for (indexi = 0; indexi < (int)indices[lg].size(); indexi++)
					if (indices[lg][indexi] >= i)
						indices[lg][indexi]--;
			i--;
		}
	}
	int indexcount = 0;
	for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
		if (!indices[lg].empty()) {
			indexcount = indices[lg].size();
			break;
		}
	}
	for (indexi = 0; indexi < indexcount; indexi++) {
		bool indexflush = true;
		for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++) {
			if (!indices[lg].empty() && indices[lg][indexi] >= 0) {
				indexflush = false;
				break;
			}
		}
		if (indexflush) {
			for (lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
				if (!indices[lg].empty())
					indices[lg].erase(indices[lg].begin() + indexi);
			indexi--;
			indexcount--;
		}
	}
	if (dbgscript)
		GetDebugLog() << "FLUSH DONE: " << dbgflushcount << endl;
}

void ScriptFunction::Read(fstream& f, unsigned int length, SteamLanguage lang) {
	if (dbgscript) {
		GetDebugLog() << "READ FIELD/ENTRY/FUNC " << (int)CurrentScriptReadField << "/" << (int)CurrentScriptReadEntry << "/" << (int)CurrentScriptReadFunction << endl;
	}
	vector<ScriptOperation> langop;
	unsigned int len = 0;
	while (len < length) {
		ScriptOperation newop;
		newop.parent = this;
		newop.Read(f);
		langop.push_back(newop);
		len += newop.size;
	}
	Import(langop, lang);
}

void ScriptFunction::Write(fstream& f, SteamLanguage lang) {
	vector<int>& routeindices = indices[lang];
	for (unsigned int i = 0; i < indices[lang].size(); i++)
		if (routeindices[i] >= 0)
			op[indices[lang][i]].Write(f);
}

void ScriptFunction::WritePPF(fstream& f, SteamLanguage lang) {
	vector<int>& routeindices = indices[lang];
	for (unsigned int i = 0; i < routeindices.size(); i++)
		if (routeindices[i] >= 0)
			op[routeindices[i]].WritePPF(f);
}

void ScriptFunction::ReadHWS(fstream& f, unsigned int length, SteamLanguage lang, ScriptLanguageLink* langlink) {
	if (dbgscript) {
		GetDebugLog() << "READHWS FUNC " << (int)function_type << endl;
	}
	if (lang == STEAM_LANGUAGE_AMOUNT) {
		uint32_t opcount, indicecount;
		SteamLanguage lang;
		uint8_t haslang;
		unsigned int i;
		HWSReadChar(f, haslang);
		HWSReadShort(f, function_type);
		HWSReadLong(f, opcount);
		HWSReadLong(f, indicecount);
		if (dbgscript) {
			GetDebugLog() << "READ FUNC at 0x" << std::hex << ((int)f.tellg() - 10) << std::dec << ": " << (int)function_type << ": " << (int)opcount << ": " << (int)indicecount << endl;
		}
		op.resize(opcount);
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
			indices[lang].resize((haslang & (1 << lang)) ? indicecount : 0);
		for (i = 0; i < opcount; i++) {
			op[i].parent = this;
			op[i].ReadHWS(f);
		}
		if (dbgscript) {
			GetDebugLog() << "READ INDICES at 0x" << std::hex << (int)f.tellg() << std::dec << endl;
		}
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
			if (haslang & (1 << lang))
				for (i = 0; i < indicecount; i++)
					HWSReadFlexibleChar(f, indices[lang][i], true);
		if (hades::STEAM_SINGLE_LANGUAGE_MODE) {
			if ((haslang & (1 << GetSteamLanguage())) == 0) {
				for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
					indices[lang].clear();
				op.clear();
			} else {
				for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
					if ((haslang & (1 << lang)) == 0)
						indices[lang].clear();
				FlushUnusedOperations();
			}
		}
	} else {
		vector<ScriptOperation> langop;
		unsigned int len = 0;
		while (len < length) {
			ScriptOperation newop;
			newop.parent = this;
			newop.Read(f);
			langop.push_back(newop);
			len += newop.size;
		}
		Import(langop, lang);
		if (langlink != NULL) {
			for (unsigned int linki = 0; linki < langlink->lang_link_count; linki++) {
				langlink->Apply(langop, linki);
				Import(langop, langlink->lang_link[linki]);
				langlink->RevertLastApply();
			}
		}
	}
}

void ScriptFunction::WriteHWS(fstream& f) {
	uint32_t opcount = op.size();
	uint32_t indicecount = 0;
	uint8_t haslang = 0;
	unsigned int i;
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
		if (!indices[lang].empty()) {
			haslang |= 1 << lang;
			indicecount = indices[lang].size();
		}
	}
	HWSWriteChar(f, haslang);
	HWSWriteShort(f, function_type);
	HWSWriteLong(f, opcount);
	HWSWriteLong(f, indicecount);
	for (i = 0; i < opcount; i++)
		op[i].WriteHWS(f);
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		if (haslang & (1 << lang))
			for (i = 0; i < indicecount; i++)
				HWSWriteFlexibleChar(f, indices[lang][i], true);
}

void ScriptFunction::WriteSteam(fstream& f, SteamLanguage lang) {
	vector<int>& routeindices = indices[lang];
	for (unsigned int i = 0; i < routeindices.size(); i++)
		if (routeindices[i] >= 0)
			op[routeindices[i]].WriteSteam(f);
}

unsigned int ScriptFunction::GetLength(SteamLanguage lang) {
	vector<int>& routeindices = indices[lang];
	unsigned int length = 0;
	for (unsigned int i = 0; i < routeindices.size(); i++)
		if (routeindices[i] >= 0)
			length += op[routeindices[i]].size;
	return length;
}

unsigned int ScriptFunction::GetSubLength(const vector<unsigned int>& indices) {
	unsigned int length = 0;
	for (unsigned int i = 0; i < indices.size(); i++)
		if (indices[i] >= 0)
			length += op[indices[i]].size;
	return length;
}

ScriptFunction& ScriptFunction::operator=(const ScriptFunction& from) {
	parent = from.parent;
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		indices[lang] = from.indices[lang];
	op = from.op;
	function_point = from.function_point;
	function_type = from.function_type;
	for (unsigned int i = 0; i < op.size(); i++)
		op[i].parent = this;
	return *this;
}

ScriptFunction::ScriptFunction(const ScriptFunction& from) :
	parent(from.parent),
	op(from.op),
	function_point(from.function_point),
	function_type(from.function_type) {
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++)
		indices[lang] = from.indices[lang];
	for (unsigned int i = 0; i < op.size(); i++)
		op[i].parent = this;
}

int ScriptDataStruct::SetName(wstring newvalue, SteamLanguage lang) {
	if (GetGameType() == GAME_TYPE_PSX) {
		FF9String tmp(name);
		tmp.SetValue(newvalue);
		if (tmp.length > SCRIPT_NAME_MAX_LENGTH)
			return 1;
	}
	name.SetValue(newvalue, lang);
	return 0;
}

int ScriptDataStruct::SetName(FF9String& newvalue) {
	if (GetGameType() == GAME_TYPE_PSX && newvalue.length > SCRIPT_NAME_MAX_LENGTH)
		return 1;
	name = newvalue;
	return 0;
}

void ScriptDataStruct::AddFunction(int entryid, int funcidpos, uint16_t functype) {
	ScriptFunction newfunc;
	newfunc.parent = this;
	newfunc.function_point = 0;
	newfunc.function_type = functype;
	entry[entryid].func.insert(entry[entryid].func.begin() + funcidpos, newfunc);
	entry[entryid].function_amount++;
}

void ScriptDataStruct::RemoveFunction(int entryid, int funcid) {
	entry[entryid].function_amount--;
	entry[entryid].func.erase(entry[entryid].func.begin() + funcid);
}

void ScriptDataStruct::AddEntry(int entrypos, uint8_t entrytype, int playerlink) {
	unsigned int i, j, k, l, m;
	int varargtype;
	for (i = 0; i < entry_amount; i++)
		for (j = 0; j < entry[i].function_amount; j++)
			for (k = 0; k < entry[i].func[j].op.size(); k++)
				if (entry[i].func[j].op[k].opcode != 0x06 && entry[i].func[j].op[k].opcode != 0x0B && entry[i].func[j].op[k].opcode != 0x29)
					for (l = 0; l < entry[i].func[j].op[k].arg_amount; l++) {
						if (!entry[i].func[j].op[k].arg[l].is_var && IsScriptArgTypeEntry(entry[i].func[j].op[k], l) && (int)entry[i].func[j].op[k].arg[l].value >= entrypos && entry[i].func[j].op[k].arg[l].value < ENTRY_MAX_VALID_ID) {
							entry[i].func[j].op[k].arg[l].value++;
						} else if (entry[i].func[j].op[k].arg[l].is_var) {
							for (m = 0; m < entry[i].func[j].op[k].arg[l].size; m++) {
								varargtype = VarOpList[entry[i].func[j].op[k].arg[l].var[m]].type;
								if (varargtype == 55) {
									if (entry[i].func[j].op[k].arg[l].var[m + 1] >= entrypos && entry[i].func[j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID)
										entry[i].func[j].op[k].arg[l].var[m + 1]++;
									m += 2;
								} else if (varargtype == 3 || varargtype == 5 || (varargtype >= 10 && varargtype < 20)) {
									if (entry[i].func[j].op[k].arg[l].var[m] == 0x5F && entry[i].func[j].op[k].arg[l].var[m + 1] >= entrypos && entry[i].func[j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID)
										entry[i].func[j].op[k].arg[l].var[m + 1]++;
									m++;
								} else if (varargtype == 6 || (varargtype >= 20 && varargtype < 30)) {
									m += 2;
								} else if (varargtype == 7) {
									m += 4;
								} else if (varargtype == 60) {
									m += 3;
								}
							}
						}
					}
	set<int> memexistingids;
	int memid = 1000;
	for (i = 0; i < entry_amount; i++)
		if (i != entrypos)
			memexistingids.insert(entry[i].memoria_id);
	while (memexistingids.find(memid) != memexistingids.end())
		memid++;
	entry_amount++;
	entry.insert(entry.begin() + entrypos, ScriptEntry());
	entry[entrypos].type = entrytype;
	entry[entrypos].memoria_id = memid;
	entry[entrypos].player_link = playerlink;
	entry[entrypos].append_mode = false;
	entry[entrypos].function_amount = 0;
	entry[entrypos].size = 0;
	entry[entrypos].flag = 0;
	local_data.insert(local_data.begin() + entrypos, ScriptLocalVariableSet());
	local_data[entrypos].allocate_amount = 0;
	local_data[entrypos].amount = 0;
}

int ScriptDataStruct::RemoveEntry(int entrypos) {
	unsigned int i, j, k, l, m;
	int varargtype;
	int res = 0;
	for (i = 0; i < entry_amount; i++)
		if (i != entrypos)
			for (j = 0; j < entry[i].function_amount; j++)
				for (k = 0; k < entry[i].func[j].op.size(); k++)
					if (entry[i].func[j].op[k].opcode != 0x06 && entry[i].func[j].op[k].opcode != 0x0B && entry[i].func[j].op[k].opcode != 0x29)
						for (l = 0; l < entry[i].func[j].op[k].arg_amount; l++) {
							if (!entry[i].func[j].op[k].arg[l].is_var && IsScriptArgTypeEntry(entry[i].func[j].op[k], l)) {
								if (entry[i].func[j].op[k].arg[l].value == entrypos) {
									entry[i].func[j].op[k].arg[l].value = 0;
									res++;
								} else if ((int)entry[i].func[j].op[k].arg[l].value > entrypos && entry[i].func[j].op[k].arg[l].value < ENTRY_MAX_VALID_ID) {
									entry[i].func[j].op[k].arg[l].value--;
								}
							} else if (entry[i].func[j].op[k].arg[l].is_var) {
								for (m = 0; m < entry[i].func[j].op[k].arg[l].size; m++) {
									varargtype = VarOpList[entry[i].func[j].op[k].arg[l].var[m]].type;
									if (varargtype == 55) {
										if (entry[i].func[j].op[k].arg[l].var[m + 1] == entrypos) {
											entry[i].func[j].op[k].arg[l].var[m + 1] = 0;
											res++;
										} else if (entry[i].func[j].op[k].arg[l].var[m + 1] > entrypos && entry[i].func[j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID) {
											entry[i].func[j].op[k].arg[l].var[m + 1]--;
										}
										m += 2;
									} else if (varargtype == 3 || varargtype == 5 || (varargtype >= 10 && varargtype < 20)) {
										if (entry[i].func[j].op[k].arg[l].var[m] == 0x5F) {
											if (entry[i].func[j].op[k].arg[l].var[m + 1] == entrypos) {
												entry[i].func[j].op[k].arg[l].var[m + 1] = 0;
												res++;
											} else if (entry[i].func[j].op[k].arg[l].var[m + 1] > entrypos && entry[i].func[j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID) {
												entry[i].func[j].op[k].arg[l].var[m + 1]--;
											}
										}
										m++;
									} else if (varargtype == 6 || (varargtype >= 20 && varargtype < 30)) {
										m += 2;
									} else if (varargtype == 7) {
										m += 4;
									} else if (varargtype == 60) {
										m += 3;
									}
								}
							}
						}
	entry_amount--;
	entry.erase(entry.begin() + entrypos);
	local_data.erase(local_data.begin() + entrypos);
	return res;
}

void ScriptDataStruct::GuaranteePlayerLinks() {
	bool foundzidane = false;
	bool forcelinks = false;
	unsigned int i;
	for (i = 0; i < entry_amount; i++) {
		if (entry[i].player_link == 0)
			foundzidane = true;
		if (i + SCRIPT_FIXED_ENTRY_AMOUNT >= entry_amount && entry[i].player_link < 0)
			forcelinks = true;
		if (forcelinks)
			entry[i].player_link = GetDefaultPlayerLink(i, entry_amount, is_field_script ? object_id : -1);
		else if (!foundzidane && entry[i].player_link >= 0)
			entry[i].player_link = -1;
	}
}

int ScriptDataStruct::ShiftArgument(int argtype, vector<pair<int, int>> shift) {
	unsigned int i, j;
	int count = 0;
	auto shifter = [](ScriptFunction& f, int argtype, vector<pair<int, int>>& sh) {
		unsigned int k, m, n;
		bool change;
		int c = 0;
		for (k = 0; k < f.op.size(); k++) {
			ScriptOperation& op = f.op[k];
			SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[op.opcode];
			for (m = 0; m < op.arg_amount; m++) {
				if (op.arg[m].is_var)
					continue;
				if ((int)m < scriptop.arg_amount && scriptop.arg_type[m] == argtype) {
					change = false;
					for (n = 0; n < sh.size(); n++)
						if ((int)op.arg[m].value >= sh[n].first) {
							change = true;
							op.arg[m].value += sh[n].second;
						}
					if (change)
						c++;
				}
			}
		}
		return c;
	};
	for (i = 0; i < entry_amount; i++)
		for (j = 0; j < entry[i].function_amount; j++)
			count += shifter(entry[i].func[j], argtype, shift);
	return count;
}

#define MACRO_SCRIPT_IOFUNCTION_PART1(IO, SEEK, READ, PPF) \
	unsigned int i, j, k, entryi, funcj, entryprevamount; \
	vector<uint16_t> funcpoint; \
	vector<int> funcreorder; \
	uint32_t entry_pos, local_entry_pos, function_pos; \
	uint16_t functype; \
	uint16_t zero16 = 0; \
	uint8_t zero8 = 0; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f, magic_number); \
	IO ## Char(f, version); \
	IO ## Char(f, entry_amount); \
	entryprevamount = entry.size(); \
	EntryIdShift = (READ && entryprevamount >= SCRIPT_FIXED_ENTRY_AMOUNT && entryprevamount > entry_amount) ? entryprevamount - entry_amount : 0; \
	EntryIdShift_Base = entry_amount - SCRIPT_FIXED_ENTRY_AMOUNT; \
	if (READ && entry_amount > entryprevamount) { \
		if (entryprevamount >= SCRIPT_FIXED_ENTRY_AMOUNT) { \
			uint8_t extra_count = entry_amount - entryprevamount; \
			entry_amount = entryprevamount; \
			for (i = 0; i < extra_count; i++) \
				AddEntry(entry.size() - SCRIPT_FIXED_ENTRY_AMOUNT, 0, -1); \
		} else { \
			entry.resize(entry_amount); \
			local_data.resize(max(local_data.size(), (size_t)entry_amount)); \
		} \
	} \
	for (i = 0; i < 20; i++) { \
		IO ## Char(f, header_unknown2[i]); \
		IO ## Char(f, header_unknown3[i]); \
	} \
	for (i = 0; i < SCRIPT_NAME_MAX_LENGTH; i++) \
		IO ## Char(f, header_name[lang][i]); \
	entry_pos = f.tellg(); \
	for (i = 0; i < entry_amount; i++) { \
		entryi = i + SCRIPT_FIXED_ENTRY_AMOUNT < entry_amount ? i : i + entry.size() - entry_amount; \
		IO ## Short(f, entry[entryi].offset); \
		IO ## Short(f, entry[entryi].size); \
		IO ## Char(f, local_data[entryi].allocate_amount); \
		IO ## Char(f, entry[entryi].flag); \
		IO ## Short(f, zero16); \
		if (READ) { \
			entry[entryi].player_link = GetDefaultPlayerLink(i, entry_amount, is_field_script ? object_id : -1); \
			entry[entryi].append_mode = 0; \
		} \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i = 0; i < entry_amount; i++) { \
		entryi = i + SCRIPT_FIXED_ENTRY_AMOUNT < entry_amount ? i : i + entry.size() - entry_amount; \
		if (entry[entryi].size > 0) { \
			CurrentScriptReadEntry = entryi; \
			SEEK(f, entry_pos, entry[entryi].offset); \
			local_entry_pos = f.tellg(); \
			if (PPF) PPFInitScanStep(f); \
			IO ## Char(f, entry[entryi].type); \
			IO ## Char(f, entry[entryi].function_amount); \
			funcreorder.clear(); \
			funcpoint.resize(entry[entryi].function_amount); \
			function_pos = f.tellg(); \
			for (j = 0; j < entry[entryi].function_amount; j++) { \
				if (READ) { \
					IO ## Short(f, functype); \
					IO ## Short(f, funcpoint[j]); \
					for (funcj = 0; funcj < entry[entryi].func.size(); funcj++) \
						if (entry[entryi].func[funcj].function_type == functype) \
							break; \
					if (funcj == entry[entryi].func.size()) \
						entry[entryi].func.resize(funcj + 1); \
					else \
						for (k = 0; k < funcreorder.size(); k++) \
							if (funcreorder[k] == funcj) \
								funcreorder[k] = -1; \
					funcreorder.push_back(funcj); \
					entry[entryi].func[funcj].function_type = functype; \
					entry[entryi].func[funcj].function_point = funcpoint[j]; \
					entry[entryi].func[funcj].parent = this; \
				} else { \
					IO ## Short(f, entry[entryi].func[j].function_type); \
					IO ## Short(f, entry[entryi].func[j].function_point); \
					funcpoint[j] = entry[entryi].func[j].function_point; \
				} \
			} \
			if (PPF) PPFEndScanStep(); \
			for (j = 0; j < entry[entryi].function_amount; j++) { \
				if (READ && funcreorder[j] < 0) \
					continue; \
				funcj = READ ? funcreorder[j] : j; \
				SEEK(f, function_pos, funcpoint[j]); \
				CurrentScriptReadFunction = entry[entryi].func[j].function_type; \
				uint32_t length; \
				if (j + 1 < entry[entryi].function_amount) \
					length = funcpoint[j + 1] - funcpoint[j]; \
				else \
					length = entry[entryi].size - funcpoint[j] - 2;

#define MACRO_SCRIPT_IOFUNCTION_PART2(IO, SEEK, READ, PPF) \
			} \
			if (READ) entry[entryi].function_amount = entry[entryi].func.size(); \
			while (f.tellg() < local_entry_pos + entry[entryi].size) \
				IO ## Char(f, zero8); \
		} else if (READ && entry[entryi].function_amount == 0) { \
			entry[entryi].type = 0xFF; \
		} \
	} \
	if (READ) { \
		entry_amount = entry.size(); \
		local_data.resize(max(local_data.size(), (size_t)entry_amount)); \
	}

#define MACRO_SCRIPT_IOFUNCTION_MULTILANG(IO, SEEK, READ) \
	wstring namestr; \
	unsigned int i, j; \
	uint16_t zero16 = 0; \
	EntryIdShift = EntryIdShift_Base = 0; \
	IO ## Char(f, has_language); \
	if (READ && hades::STEAM_SINGLE_LANGUAGE_MODE && (has_language & (1 << GetSteamLanguage())) == 0) { \
		has_language = 1 << GetSteamLanguage(); \
		return; \
	} \
	IO ## Short(f, magic_number); \
	IO ## Char(f, version); \
	if (version >= SCRIPT_VERSION_PENTRY) IO ## Char(f, append_mode); \
	IO ## Char(f, entry_amount); \
	if (READ) { \
		entry.resize(entry_amount); \
		unsigned int localprevamount = local_data.size(); \
		local_data.resize(max(localprevamount, (unsigned int)entry_amount)); \
		for (i = localprevamount; i < entry_amount; i++) \
			local_data[i].amount = 0; \
	} \
	for (i = 0; i < 20; i++) { \
		IO ## Char(f, header_unknown2[i]); \
		IO ## Char(f, header_unknown3[i]); \
	} \
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) { \
		if (has_language & (1 << lang)) { \
			if (!READ) namestr = lang == GetSteamLanguage() ? name.str : name.multi_lang_str[lang]; \
			IO ## WString(f, namestr); \
			if (usetext && READ && (!hades::STEAM_SINGLE_LANGUAGE_MODE || lang == GetSteamLanguage())) name.SetValue(namestr, lang); \
		} \
	} \
	for (i = 0; i < entry_amount; i++) { \
		if (version >= SCRIPT_VERSION_PENTRY) { \
			IO ## FlexibleChar(f, entry[i].memoria_id, true); \
			IO ## FlexibleChar(f, entry[i].player_link, true); \
			IO ## Char(f, entry[i].append_mode); \
		} else if (READ) { \
			entry[i].memoria_id = i; \
			entry[i].player_link = GetDefaultPlayerLink(i, entry_amount, is_field_script ? object_id : -1); \
			entry[i].append_mode = 0; \
		} \
		IO ## Char(f, local_data[i].allocate_amount); \
		IO ## Char(f, entry[i].flag); \
		IO ## Short(f, zero16); \
		IO ## Char(f, entry[i].type); \
		IO ## Char(f, entry[i].function_amount); \
		if (READ) { \
			entry[i].func.resize(entry[i].function_amount); \
			for (j = 0; j < entry[i].function_amount; j++) { \
				entry[i].func[j].parent = this; \
				entry[i].func[j].ReadHWS(f, 0, STEAM_LANGUAGE_AMOUNT); \
			} \
		} else { \
			for (j = 0; j < entry[i].function_amount; j++) \
				entry[i].func[j].WriteHWS(f); \
		} \
	} \
	for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) \
		if (has_language & (1 << lang)) \
			UpdateOffset(lang);

void ScriptDataStruct::Read(fstream& f, SteamLanguage lang) {
	dbgscript = wxFileName::FileExists("LogScripts.txt");
	LoadCustomScriptUtility();
	CurrentScriptReadField = object_id;
	if (dbgscript) {
		GetDebugLog() << "FIELD " << (int)object_id << endl;
	}
	if (lang < STEAM_LANGUAGE_AMOUNT)
		has_language |= 1 << lang;
	if (GetGameType() == GAME_TYPE_PSX && parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type == CLUSTER_TYPE_WORLD_MAP && object_id >= 9100) {
		ChunkChild::Read(f);
	} else {
		if (GetGameType() == GAME_TYPE_PSX) {
			MACRO_SCRIPT_IOFUNCTION_PART1(FFIXRead, FFIXSeek, true, false)
				entry[entryi].func[funcj].Read(f, length, lang);
			MACRO_SCRIPT_IOFUNCTION_PART2(FFIXRead, FFIXSeek, true, false)
			name.ReadFromChar(header_name[lang]);
		} else {
			MACRO_SCRIPT_IOFUNCTION_PART1(SteamRead, SteamSeek, true, false)
				entry[entryi].func[funcj].Read(f, length, lang);
			MACRO_SCRIPT_IOFUNCTION_PART2(SteamRead, SteamSeek, true, false)
			FF9String dummystr;
			dummystr.ReadFromChar(header_name[lang]);
			if (!name.created)
				name.CreateEmpty();
			name.SetValue(dummystr.str, lang);
		}
		local_data.resize(max(local_data.size(), (size_t)entry_amount));
		for (unsigned int i = 0; i < entry_amount; i++)
			local_data[i].amount = 0;
		global_data.allocate_amount = 0;
		global_data.amount = 0;
		for (unsigned int i = 0; i < entry_amount; i++)
			entry[i].memoria_id = i;
		loaded = true;
	}
	GuaranteePlayerLinks();
	loaded = true;
}

void ScriptDataStruct::Write(fstream& f) {
	if (parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type == CLUSTER_TYPE_WORLD_MAP && object_id >= 9100) {
		ChunkChild::Write(f);
	} else {
		SteamLanguage lang = GetSteamLanguage();
		MACRO_SCRIPT_IOFUNCTION_PART1(FFIXWrite, FFIXSeek, false, false)
			entry[entryi].func[funcj].Write(f, lang);
		MACRO_SCRIPT_IOFUNCTION_PART2(FFIXWrite, FFIXSeek, false, false)
	}
	modified = false;
}

void ScriptDataStruct::WritePPF(fstream& f) {
	if (parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type == CLUSTER_TYPE_WORLD_MAP && object_id >= 9100) {
		ChunkChild::WritePPF(f);
	} else {
		SteamLanguage lang = GetSteamLanguage();
		MACRO_SCRIPT_IOFUNCTION_PART1(PPFStepAdd, FFIXSeek, false, true)
			entry[entryi].func[funcj].WritePPF(f, lang);
		MACRO_SCRIPT_IOFUNCTION_PART2(PPFStepAdd, FFIXSeek, false, true)
	}
}

int oldhwsenemycount = 0;
void ScriptDataStruct::ReadHWS(fstream& f, bool usetext, SteamLanguage lang, ScriptLanguageLink* langlink) {
	dbgscript = wxFileName::FileExists("LogScripts.txt");
	if (dbgscript) {
		GetDebugLog() << "LOAD FIELD HWS " << ConvertWStrToStr(name.str_nice) << (lang < STEAM_LANGUAGE_AMOUNT ? " (old version)" : " (new version)") << endl;
	}
	if (lang < STEAM_LANGUAGE_AMOUNT) {
		CurrentScriptReadField = is_battle_script ? 0 : object_id;
		EntryIdShift = EntryIdShift_Base = 0;
		vector<EntryIdentifyingInfos> entryinfos;
		vector<uint8_t> hwsentryallocate;
		vector<ScriptEntry> hwsentry;
		vector<int> entrymoveindex;
		vector<bool> entryisnew;
		unsigned int entryextracount = 0;
		unsigned int i, j, k;
		uint32_t script_pos, entry_pos, function_pos;
		uint16_t functype, funcpoint;
		uint8_t hwsentrycount, entrytype, entryfunccount, entryfuncprevcount;
		script_pos = f.tellg();
		f.seekg(3, ios::cur);
		HWSReadChar(f, hwsentrycount);
		uint8_t hwsexactentrycount = hwsentrycount;
		bool rawimport = is_battle_script || hwsentrycount == entry_amount;
		if (is_battle_script) {
			while (entry_amount < oldhwsenemycount + SCRIPT_FIXED_ENTRY_AMOUNT + 1)
				AddEntry(max(1, entry_amount - SCRIPT_FIXED_ENTRY_AMOUNT), 2, -1);
			if (hwsentrycount > entry_amount)
				hwsentrycount = entry_amount;
		} else if (!rawimport) {
			for (i = 0; i + SCRIPT_FIXED_ENTRY_AMOUNT < entry_amount; i++)
				entryinfos.push_back(GetEntryIdentifyingInfos(entry[i]));
		}
		for (i = 0; i < 20; i++) {
			HWSReadChar(f, header_unknown2[i]);
			HWSReadChar(f, header_unknown3[i]);
		}
		if (usetext)
			for (i = 0; i < SCRIPT_NAME_MAX_LENGTH; i++)
				HWSReadChar(f, header_name[lang][i]);
		else
			f.seekg(SCRIPT_NAME_MAX_LENGTH, ios::cur);
		entry_pos = f.tellg();
		if (!rawimport) {
			hwsentry.resize(hwsentrycount);
			hwsentryallocate.resize(hwsentrycount);
		}
		vector<ScriptEntry>& readentry = rawimport ? entry : hwsentry;
		uint16_t lastentryoffset, lastentrysize, entrysize;
		for (i = 0; i < hwsexactentrycount; i++) {
			if (i < hwsentrycount) {
				HWSReadShort(f, readentry[i].offset);
				HWSReadShort(f, readentry[i].size);
				HWSReadChar(f, rawimport ? local_data[i].allocate_amount : hwsentryallocate[i]);
				HWSReadChar(f, readentry[i].flag);
				f.seekg(2, ios::cur);
				readentry[i].append_mode = 0;
				readentry[i].player_link = GetDefaultPlayerLink(i, rawimport ? entry_amount : hwsentrycount, is_field_script ? object_id : -1);
				lastentryoffset = readentry[i].offset;
				if (readentry[i].size > 0)
					lastentrysize = readentry[i].size;
			} else {
				HWSReadShort(f, lastentryoffset);
				HWSReadShort(f, entrysize);
				if (entrysize > 0)
					lastentrysize = entrysize;
				f.seekg(4, ios::cur);
			}
		}
		int entrylastindex = -1;
		for (i = 0; i < hwsentrycount; i++) {
			if (readentry[i].size > 0) {
				CurrentScriptReadEntry = i;
				HWSSeek(f, entry_pos, readentry[i].offset);
				HWSReadChar(f, entrytype);
				HWSReadChar(f, entryfunccount);
				if (is_battle_script && i >= 1 && entryfunccount <= 1)
					continue;
				entryfuncprevcount = readentry[i].function_amount;
				readentry[i].type = entrytype;
				readentry[i].function_amount = entryfunccount;
				readentry[i].func.resize(readentry[i].function_amount);
				function_pos = f.tellg();
				if (rawimport) {
					for (j = 0; j < readentry[i].function_amount; j++) {
						HWSReadShort(f, functype);
						HWSReadShort(f, funcpoint);
						for (k = 0; k < readentry[i].function_amount && k < entryfuncprevcount; k++) {
							if (readentry[i].func[k].function_type == functype) {
								if (j != k) {
									ScriptFunction ftmp = readentry[i].func[k];
									readentry[i].func[k] = readentry[i].func[j];
									readentry[i].func[j] = ftmp;
								}
								break;
							}
						}
					}
					f.seekg(function_pos);
				}
				for (j = 0; j < readentry[i].function_amount; j++) {
					HWSReadShort(f, readentry[i].func[j].function_type);
					HWSReadShort(f, readentry[i].func[j].function_point);
					readentry[i].func[j].parent = this;
					if (rawimport) {
						readentry[i].func[j].indices[lang].clear();
						if (langlink != NULL)
							for (k = 0; k < langlink->lang_link_count; k++)
								readentry[i].func[j].indices[langlink->lang_link[k]].clear();
					}
				}
				for (j = 0; j < readentry[i].function_amount; j++) {
					HWSSeek(f, function_pos, readentry[i].func[j].function_point);
					CurrentScriptReadFunction = readentry[i].func[j].function_type;
					uint32_t length;
					if (j + 1 < readentry[i].function_amount)
						length = readentry[i].func[j + 1].function_point - readentry[i].func[j].function_point;
					else
						length = readentry[i].size - readentry[i].func[j].function_point - 2;
					readentry[i].func[j].ReadHWS(f, length, lang, langlink);
					if (rawimport && hades::STEAM_SINGLE_LANGUAGE_MODE) {
						for (SteamLanguage sublang = 0; sublang < STEAM_LANGUAGE_AMOUNT; sublang++)
							if (sublang != GetSteamLanguage())
								readentry[i].func[j].indices[sublang].clear();
						readentry[i].func[j].FlushUnusedOperations();
					}
				}
			} else {
				if (rawimport)
					continue;
				readentry[i].type = 0xFF;
				readentry[i].function_amount = 0;
			}
			if (!rawimport) {
				bool foundentry = false;
				if (i == 0) {
					entrylastindex = i;
					foundentry = true;
				} else if (i + SCRIPT_FIXED_ENTRY_AMOUNT >= hwsentrycount) {
					entrylastindex = i + entry_amount - hwsentrycount;
					foundentry = true;
				} else {
					EntryIdentifyingInfos hwsinfo = GetEntryIdentifyingInfos(readentry[i]);
					for (j = entrylastindex + 1 - entryextracount; j < entryinfos.size(); j++) {
						if (hwsinfo.CompareWith(entryinfos[j])) {
							entrylastindex = j + entryextracount;
							foundentry = true;
							break;
						} else if (j + hwsentrycount >= entryinfos.size() + SCRIPT_FIXED_ENTRY_AMOUNT + i) {
							break;
						}
					}
				}
				if (foundentry) {
					entrymoveindex.push_back(entrylastindex);
					entryisnew.push_back(false);
				} else {
					AddEntry(++entrylastindex, readentry[i].type, -1);
					entrymoveindex.push_back(entrylastindex);
					entryisnew.push_back(true);
					entryextracount++;
				}
				local_data[entrylastindex].allocate_amount = hwsentryallocate[i];
			}
		}
		HWSSeek(f, entry_pos, lastentryoffset + lastentrysize);
		if (!rawimport) {
			unsigned int l, m;
			int varargtype;
			for (i = 0; i < hwsentrycount; i++) {
				for (j = 0; j < hwsentry[i].function_amount; j++)
					for (k = 0; k < hwsentry[i].func[j].op.size(); k++) {
						ScriptOperation& hwsop = hwsentry[i].func[j].op[k];
						if (hwsop.opcode != 0x06 && hwsop.opcode != 0x0B && hwsop.opcode != 0x29) {
							for (l = 0; l < hwsop.arg_amount; l++) {
								if (!hwsop.arg[l].is_var && IsScriptArgTypeEntry(hwsop, l)) {
									if (hwsop.arg[l].value < entrymoveindex.size())
										hwsop.arg[l].value = entrymoveindex[hwsop.arg[l].value];
								} else if (hwsop.arg[l].is_var) {
									for (m = 0; m < hwsop.arg[l].size; m++) {
										varargtype = VarOpList[hwsop.arg[l].var[m]].type;
										if (varargtype == 55) {
											if (hwsop.arg[l].var[m + 1] < entrymoveindex.size())
												hwsop.arg[l].var[m + 1] = entrymoveindex[hwsop.arg[l].var[m + 1]];
											m += 2;
										} else if (varargtype == 3 || varargtype == 5 || (varargtype >= 10 && varargtype < 20)) {
											if (hwsop.arg[l].var[m] == 0x5F) {
												if (hwsop.arg[l].var[m + 1] < entrymoveindex.size())
													hwsop.arg[l].var[m + 1] = entrymoveindex[hwsop.arg[l].var[m + 1]];
											}
											m++;
										} else if (varargtype == 6 || (varargtype >= 20 && varargtype < 30)) {
											m += 2;
										} else if (varargtype == 7) {
											m += 4;
										} else if (varargtype == 60) {
											m += 3;
										}
									}
								}
							}
						}
					}
			}
			for (i = 0; i < hwsentrycount; i++) {
				if (entryisnew[i]) {
					for (j = 0; j < hwsentry[i].function_amount; j++) {
						if (hades::STEAM_SINGLE_LANGUAGE_MODE)
							for (SteamLanguage sublang = 0; sublang < STEAM_LANGUAGE_AMOUNT; sublang++)
								if (sublang != GetSteamLanguage())
									hwsentry[i].func[j].indices[sublang].clear();
						hwsentry[i].func[j].FlushUnusedOperations();
					}
					entry[entrymoveindex[i]] = hwsentry[i];
				} else {
					ScriptEntry& mergedentry = entry[entrymoveindex[i]];
					for (j = 0; j < mergedentry.function_amount; j++) {
						mergedentry.func[j].indices[lang].clear();
						if (langlink != NULL)
							for (k = 0; k < langlink->lang_link_count; k++)
								mergedentry.func[j].indices[langlink->lang_link[k]].clear();
					}
					for (j = 0; j < hwsentry[i].function_amount; j++) {
						ScriptFunction& hwsfunc = hwsentry[i].func[j];
						int funcindex = -1;
						for (k = 0; k < mergedentry.function_amount; k++) {
							if (mergedentry.func[k].function_type == hwsfunc.function_type) {
								funcindex = k;
								break;
							}
						}
						if (funcindex < 0) {
							funcindex = mergedentry.function_amount;
							AddFunction(entrymoveindex[i], funcindex, hwsfunc.function_type);
						}
						vector<ScriptOperation> importop;
						for (k = 0; k < hwsfunc.indices[lang].size(); k++)
							if (hwsfunc.indices[lang][k] >= 0)
								importop.push_back(hwsfunc.op[hwsfunc.indices[lang][k]]);
						if (!hades::STEAM_SINGLE_LANGUAGE_MODE || lang == GetSteamLanguage())
							mergedentry.func[funcindex].Import(importop, lang);
						if (langlink != NULL) {
							for (l = 0; l < langlink->lang_link_count; l++) {
								if (hades::STEAM_SINGLE_LANGUAGE_MODE && langlink->lang_link[l] != GetSteamLanguage())
									continue;
								langlink->Apply(importop, l);
								mergedentry.func[funcindex].Import(importop, langlink->lang_link[l]);
								langlink->RevertLastApply();
							}
						}
						mergedentry.func[funcindex].FlushUnusedOperations();
						if (mergedentry.func[funcindex].op.size() == 0)
							RemoveFunction(entrymoveindex[i], funcindex);
					}
				}
			}
		}
	} else {
		MACRO_SCRIPT_IOFUNCTION_MULTILANG(HWSRead, HWSSeek, true)
	}
	GuaranteePlayerLinks();
	MarkDataModified();
}

void ScriptDataStruct::WriteHWS(fstream& f, bool uselatestversion) {
	SteamLanguage lang;
	bool usetext = true;
	version = SCRIPT_VERSION_CURRENT;
	MACRO_SCRIPT_IOFUNCTION_MULTILANG(HWSWrite, HWSSeek, false)
}

void ScriptDataStruct::WriteSteam(fstream& f, bool uselatestversion, SteamLanguage lang) {
	UpdateOffset(lang);
	unsigned int i, j, k;
	vector<uint16_t> offsets;
	uint32_t entry_pos, local_entry_pos, function_pos;
	uint16_t entry_offset = uselatestversion ? 0 : 8 * entry_amount;
	uint16_t zero16 = 0;
	uint8_t zero8 = 0;
	HWSWriteShort(f, magic_number);
	HWSWriteChar(f, uselatestversion ? SCRIPT_VERSION_CURRENT : SCRIPT_VERSION_VANILLA);
	if (uselatestversion)
		HWSWriteChar(f, append_mode);
	HWSWriteChar(f, entry_amount);
	if (!uselatestversion || append_mode == 0) {
		for (i = 0; i < 20; i++) {
			HWSWriteChar(f, header_unknown2[i]);
			HWSWriteChar(f, header_unknown3[i]);
		}
		for (i = 0; i < SCRIPT_NAME_MAX_LENGTH; i++)
			HWSWriteChar(f, header_name[lang][i]);
	}
	entry_pos = f.tellg();
	for (i = 0; i < entry_amount; i++) {
		offsets.push_back(entry_offset);
		if (uselatestversion) {
			if (dbgscript) {
				GetDebugLog() << "GENERATE SCRIPT ENTRY " << (int)object_id << ", " << i << ": " << entry[i].memoria_id << endl;
			}
			HWSWriteFlexibleChar(f, entry[i].memoria_id, true);
			HWSWriteFlexibleChar(f, entry[i].player_link, true);
			HWSWriteChar(f, entry[i].append_mode);
		}
		if (uselatestversion && append_mode != 0 && (entry[i].append_mode & 1) == 0) {
			HWSWriteShort(f, 0);
			HWSWriteShort(f, 0);
		} else {
			HWSWriteShort(f, entry_offset);
			HWSWriteShort(f, entry[i].size);
			entry_offset += entry[i].size;
		}
		HWSWriteChar(f, local_data[i].allocate_amount);
		HWSWriteChar(f, entry[i].flag);
		HWSWriteShort(f, zero16);
	}
	if (uselatestversion)
		entry_pos = f.tellg();
	for (i = 0; i < entry_amount; i++) {
		if (uselatestversion && append_mode != 0 && (entry[i].append_mode & 1) == 0)
			continue;
		if (entry[i].size > 0) {
			HWSSeek(f, entry_pos, offsets[i]);
			local_entry_pos = f.tellg();
			HWSWriteChar(f, entry[i].type);
			HWSWriteChar(f, entry[i].function_amount);
			function_pos = f.tellg();
			for (j = 0; j < entry[i].function_amount; j++) {
				HWSWriteShort(f, entry[i].func[j].function_type);
				HWSWriteShort(f, entry[i].func[j].function_point);
			}
			for (j = 0; j < entry[i].function_amount; j++) {
				vector<int>& langindices = entry[i].func[j].indices[lang];
				HWSSeek(f, function_pos, entry[i].func[j].function_point);
				for (k = 0; k < langindices.size(); k++)
					if (langindices[k] >= 0)
						entry[i].func[j].op[langindices[k]].WriteHWS(f);
			}
			while (f.tellg() < local_entry_pos + entry[i].size)
				HWSWriteChar(f, zero8);
		}
	}
}

void ScriptDataStruct::ReadLocalHWS(fstream& f) {
	uint32_t localpos, entrypos, vardatapos, localsize;
	uint16_t entryheadersize, vardatasize, tmp16;
	uint8_t entryam, vardatatype, allocatecount;
	unsigned int i, j;
	localpos = f.tellg();
	HWSReadLong(f, localsize);
	HWSReadChar(f, entryam);
	local_data.resize(max(local_data.size(), (size_t)entryam));
	for (i = 0; i < entryam; i++) {
		entrypos = f.tellg();
		HWSReadShort(f, entryheadersize);
		HWSReadChar(f, allocatecount);
		if (GetHWSGlobalVersion() > 100)
			local_data[i].allocate_amount = allocatecount;
		HWSReadChar(f, (uint8_t&)local_data[i].amount);
		HWSSeek(f, entrypos, entryheadersize);
		local_data[i].local_type.resize(local_data[i].amount);
		local_data[i].type.resize(local_data[i].amount);
		local_data[i].size.resize(local_data[i].amount);
		local_data[i].name.resize(local_data[i].amount);
		local_data[i].cat.resize(local_data[i].amount);
		local_data[i].id.resize(local_data[i].amount);
		for (j = 0; j < local_data[i].amount; j++) {
			HWSReadChar(f, local_data[i].local_type[j]);
			HWSReadShort(f, (uint16_t&)local_data[i].type[j]);
			HWSReadChar(f, local_data[i].size[j]);
			HWSReadChar(f, local_data[i].cat[j]);
			HWSReadShort(f, local_data[i].id[j]);
			local_data[i].name[j].clear();
			HWSReadShort(f, tmp16);
			while (tmp16 != 0) {
				local_data[i].name[j].append(1, tmp16);
				HWSReadShort(f, tmp16);
			}
			HWSReadChar(f, vardatatype);
			while (vardatatype != 0) {
				vardatapos = f.tellg();
				HWSReadShort(f, vardatasize);
				// Additional variable informations
				// may appear here in a future version
				HWSSeek(f, vardatapos, vardatasize);
				HWSReadChar(f, vardatatype);
			}
		}
	}
	if (localsize + localpos > f.tellg()) {
		entrypos = f.tellg();
		HWSReadShort(f, entryheadersize);
		HWSReadChar(f, global_data.allocate_amount);
		HWSReadChar(f, (uint8_t&)global_data.amount);
		HWSSeek(f, entrypos, entryheadersize);
		global_data.local_type.resize(global_data.amount);
		global_data.type.resize(global_data.amount);
		global_data.size.resize(global_data.amount);
		global_data.name.resize(global_data.amount);
		global_data.cat.resize(global_data.amount);
		global_data.id.resize(global_data.amount);
		for (j = 0; j < global_data.amount; j++) {
			HWSReadChar(f, global_data.local_type[j]);
			HWSReadShort(f, (uint16_t&)global_data.type[j]);
			HWSReadChar(f, global_data.size[j]);
			HWSReadChar(f, global_data.cat[j]);
			HWSReadShort(f, global_data.id[j]);
			global_data.name[j].clear();
			HWSReadShort(f, tmp16);
			while (tmp16 != 0) {
				global_data.name[j].append(1, tmp16);
				HWSReadShort(f, tmp16);
			}
			HWSReadChar(f, vardatatype);
			while (vardatatype != 0) {
				vardatapos = f.tellg();
				HWSReadShort(f, vardatasize);
				// Additional variable informations
				// may appear here in a future version
				HWSSeek(f, vardatapos, vardatasize);
				HWSReadChar(f, vardatatype);
			}
		}
	}
	HWSSeek(f, localpos, localsize);
	MarkDataModified();
}

void ScriptDataStruct::WriteLocalHWS(fstream& f) {
	unsigned int i, j, k;
	uint32_t localpos, localsize = 0;
	uint16_t zero16 = 0;
	uint8_t zero8 = 0;
	localpos = f.tellg();
	HWSWriteLong(f, localsize);
	HWSWriteChar(f, entry_amount);
	for (i = 0; i < entry_amount; i++) {
		HWSWriteShort(f, 0x4);
		HWSWriteChar(f, local_data[i].allocate_amount);
		HWSWriteChar(f, (uint8_t&)local_data[i].amount);
		for (j = 0; j < local_data[i].amount; j++) {
			HWSWriteChar(f, local_data[i].local_type[j]);
			HWSWriteShort(f, local_data[i].type[j]);
			HWSWriteChar(f, local_data[i].size[j]);
			HWSWriteChar(f, local_data[i].cat[j]);
			HWSWriteShort(f, local_data[i].id[j]);
			for (k = 0; k < local_data[i].name[j].length(); k++)
				HWSWriteShort(f, local_data[i].name[j][k]);
			HWSWriteShort(f, zero16);
			HWSWriteChar(f, zero8);
		}
	}
	if (global_data.amount > 0) {
		HWSWriteShort(f, 0x4);
		HWSWriteChar(f, global_data.allocate_amount);
		HWSWriteChar(f, (uint8_t&)global_data.amount);
		for (j = 0; j < global_data.amount; j++) {
			HWSWriteChar(f, global_data.local_type[j]);
			HWSWriteShort(f, global_data.type[j]);
			HWSWriteChar(f, global_data.size[j]);
			HWSWriteChar(f, global_data.cat[j]);
			HWSWriteShort(f, global_data.id[j]);
			for (k = 0; k < global_data.name[j].length(); k++)
				HWSWriteShort(f, global_data.name[j][k]);
			HWSWriteShort(f, zero16);
			HWSWriteChar(f, zero8);
		}
	}
	localsize = (long long)f.tellg() - localpos;
	f.seekg(localpos);
	HWSWriteLong(f, localsize);
	HWSSeek(f, localpos, localsize);
}

ScriptDataStruct& ScriptDataStruct::operator=(const ScriptDataStruct& from) {
	unsigned int i, j;
	parent_cluster = from.parent_cluster;
	parent_chunk = from.parent_chunk;
	type = from.type;
	size = from.size;
	object_id = from.object_id;
	loaded = from.loaded;
	modified = false;
	is_field_script = from.is_field_script;
	name = from.name;
	magic_number = from.magic_number;
	version = from.version;
	entry_amount = from.entry_amount;
	entry = from.entry;
	global_data = from.global_data;
	local_data = from.local_data;
	has_language = from.has_language;
	append_mode = from.append_mode;
	related_charmap_id = from.related_charmap_id;
	for (i = 0; i < 20; i++) {
		header_unknown2[i] = from.header_unknown2[i];
		header_unknown3[i] = from.header_unknown3[i];
	}
	for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
		for (j = 0; j < SCRIPT_NAME_MAX_LENGTH; j++)
			header_name[i][j] = from.header_name[i][j];
	for (i = 0; i < entry_amount; i++)
		for (j = 0; j < entry[i].function_amount; j++)
			entry[i].func[j].parent = this;
	return *this;
}

bool ScriptDataStruct::IsDataModified(SteamLanguage lang) {
	return modified;
}

int ScriptDataStruct::GetDataSize(SteamLanguage lang) {
	uint16_t funcpos, entrypos = 8 * entry_amount, lastentrypos = entrypos;
	unsigned int i, j;
	int res = 128;
	for (i = 0; i < entry_amount; i++) {
		if (entry[i].function_amount > 0) {
			entry[i].offset = entrypos;
			funcpos = 4 * entry[i].function_amount;
			for (j = 0; j < entry[i].function_amount; j++) {
				entry[i].func[j].function_point = funcpos;
				funcpos += entry[i].func[j].GetLength(lang);
			}
			entry[i].size = funcpos + 2;
			if (entry[i].size % 4)
				entry[i].size += 4 - entry[i].size % 4;
			lastentrypos = entrypos;
			entrypos += entry[i].size;
		} else {
			entry[i].offset = entrypos;
			entry[i].size = 0;
		}
	}
	if (entry_amount > 0) // Just for mimicking the game's behavior
		for (i = entry_amount - 1; entry[i].size == 0; i--)
			entry[i].offset = lastentrypos;
	res += entrypos;
	return res;
}

void ScriptDataStruct::UpdateOffset(SteamLanguage lang) {
	// Assume the size/length of ScriptFunction
	// and structures under it are properly set
	unsigned int i, j;
	if (GetGameType() == GAME_TYPE_PSX) // No modif for Steam as this name is unused
		for (i = 0; i < name.length && i < SCRIPT_NAME_MAX_LENGTH; i++)
			header_name[lang][i] = name.raw[i];
	uint16_t funcpos, entrypos = 8 * entry_amount, lastentrypos = entrypos;
	uint32_t totalsize = 128;
	for (i = 0; i < entry_amount; i++) {
		if (entry[i].function_amount > 0) {
			entry[i].offset = entrypos;
			funcpos = 4 * entry[i].function_amount;
			for (j = 0; j < entry[i].function_amount; j++) {
				entry[i].func[j].function_point = funcpos;
				funcpos += entry[i].func[j].GetLength(lang);
			}
			entry[i].size = funcpos + 2;
			if (entry[i].size % 4)
				entry[i].size += 4 - entry[i].size % 4;
			lastentrypos = entrypos;
			entrypos += entry[i].size;
		} else {
			entry[i].offset = entrypos;
			entry[i].size = 0;
		}
	}
	if (entry_amount > 0) // Just for mimicking the game's behavior
		for (i = entry_amount - 1; entry[i].size == 0; i--)
			entry[i].offset = lastentrypos;
	totalsize += entrypos;
	SetSize(totalsize);
}

void ScriptDataStruct::RegisterEnemyCountForOldHWS(int battleenemycount) {
	oldhwsenemycount = battleenemycount;
}
