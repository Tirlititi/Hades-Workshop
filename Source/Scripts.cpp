#include "Scripts.h"

#include "Hades_Strings.h"
#include "Database_Script.h"

#define ENTRY_MAX_VALID_ID 128

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

inline vector<ScriptArgument> NewScriptArgumentArray(unsigned int amount, ScriptOperation* p) {
	vector<ScriptArgument> res(amount);
	for (unsigned int i = 0; i < amount; i++)
		res[i].parent = p;
	return res;
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
		arg = NewScriptArgumentArray(arg_amount, this); \
		arg[0].FUNC(f, 2, false, true); \
		for (i = 0; i < size_byte; i++) { \
			arg[2 * i + 1].FUNC(f, 2, false, false); \
			arg[2 * i + 2].FUNC(f, 2, false, true); \
		} \
		size += 3 + size_byte * 4; \
	} else if (opcode == 0x0B) { \
		IO ## Char(f, size_byte); \
		arg_amount = 2 + size_byte; \
		arg = NewScriptArgumentArray(arg_amount, this); \
		arg[0].FUNC(f, 2, false, false); \
		for (i = 1; i < arg_amount; i++) \
			arg[i].FUNC(f, 2, false, true); \
		size += 5 + size_byte * 2; \
	} else if (opcode == 0x29) { \
		uint8_t flag = 1; \
		IO ## Char(f, vararg_flag); \
		IO ## Char(f, size_byte); \
		arg_amount = size_byte; \
		arg = NewScriptArgumentArray(arg_amount, this); \
		size += 2; \
		for (i = 0; i < arg_amount; i++) { \
			arg[i].FUNC(f, 4, flag & vararg_flag, true); \
			size += arg[i].size; \
			flag *= 2; \
		} \
	} else { \
		SortedChoiceItemScriptOpcode& scriptop = HADES_STRING_SCRIPT_OPCODE[opcode]; \
		arg_amount = scriptop.arg_amount; \
		arg = NewScriptArgumentArray(arg_amount, this); \
		if (scriptop.use_vararg) { \
			uint8_t flag = 1; \
			IO ## Char(f, vararg_flag); \
			size += 1; \
			for (i = 0; i < arg_amount; i++) { \
				arg[i].FUNC(f, scriptop.arg_length[i], flag & vararg_flag, IsScriptArgTypeSigned(scriptop.arg_type[i])); \
				size += arg[i].size; \
				flag *= 2; \
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

uint16_t GetControlVarArgCode(const vector<ScriptOperation>& oplist, unsigned int pos, unsigned int nextpos) {
	if (oplist[pos].opcode != 0x05 || nextpos >= oplist.size())
		return 0;
	if (oplist[nextpos].opcode != 0x02 && oplist[nextpos].opcode != 0x03 && oplist[nextpos].opcode != 0x06 && oplist[nextpos].opcode != 0x0B)
		return 0;
	return oplist[nextpos].opcode;
}

bool dbgscript = false;
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
				if (true || dbgscript) {
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
	indices[lang].clear();
	// TODO: flush previous
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
}

/*
void ScriptFunction::Import(const vector<ScriptOperation>& langop, SteamLanguage lang) {
	// TODO: flush previous
	// TODO: improve
	unsigned int i, j, k;
	for (i = 0; i < breaks.size(); i++) {
		vector<unsigned int> routeindices = GetRouteIndices(&breaks[i], breaks[i].GetFirstLang());
		if (abs((int)(routeindices.size() - langop.size())) > (int)langop.size() / 2)
			continue;
		unsigned int routeindex = 0;
		bool useasbasis = true;
		vector<ScriptLanguageBreakPoint> subbreaks;
		for (j = 0; j < langop.size(); j++) {
			if (routeindex >= routeindices.size()) {
				if (langop.size() - j > langop.size() / 5) {
					useasbasis = false;
				} else {
					// Add breakpoint for end
					ScriptLanguageBreakPoint br(false);
					br.lang[lang] = true;
					br.op_start_relative = j;
					br.op_amount = langop.size() - j;
					subbreaks.push_back(br);
				}
				break;
			}
			int cmp = langop[j].Compare(op[routeindices[routeindex]]);
			if (cmp == 0) {
				routeindex++;
			} else if (cmp == 1) {
				if (langop[j].opcode >= 0x10) {
					// Insert size 1 breakpoint
					ScriptLanguageBreakPoint br(false);
					br.lang[lang] = true;
					br.op_start_relative = j;
					br.op_amount = 1;
					subbreaks.push_back(br);
					for (SteamLanguage lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
						br.lang[lg] = breaks[i].lang[lg];
					br.op_start_relative = routeindex;
					br.op_amount = 1;
					subbreaks.push_back(br);
				} else {
					// TODO ??? (control block most likely) - same size
					useasbasis = false;
					break;
				}
				routeindex++;
			} else {
				bool shiftfound = false;
				for (k = 1; k < 10 && routeindex + k < routeindices.size(); k++) {
					cmp = langop[j].Compare(op[routeindices[routeindex + k]]);
					if (cmp == 0) {
						// Insert breakpoint: langop has less operations than breaks[i]
						ScriptLanguageBreakPoint br(false);
						for (SteamLanguage lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
							br.lang[lg] = breaks[i].lang[lg];
						br.op_start_relative = routeindex;
						br.op_amount = k;
						subbreaks.push_back(br);
						routeindex += k + 1;
						shiftfound = true;
						break;
					}
				}
				if (!shiftfound) {
					for (k = 1; k < 10 && j + k < langop.size(); k++) {
						cmp = langop[j + k].Compare(op[routeindices[routeindex]]);
						if (cmp == 0) {
							// Insert breakpoint: breaks[i] has less operations than langop
							ScriptLanguageBreakPoint br(false);
							br.lang[lang] = true;
							br.op_start_relative = j;
							br.op_amount = k;
							subbreaks.push_back(br);
							j += k;
							shiftfound = true;
							break;
						}
					}
				}
				if (!shiftfound) {
					useasbasis = false;
					break;
				}
			}
		}
		if (routeindex < routeindices.size()) {
			if (routeindices.size() - routeindex > routeindices.size() / 5) {
				useasbasis = false;
			} else {
				// Add breakpoint for end
				ScriptLanguageBreakPoint br(false);
				for (SteamLanguage lg = 0; lg < STEAM_LANGUAGE_AMOUNT; lg++)
					br.lang[lg] = breaks[i].lang[lg];
				br.op_start_relative = routeindex;
				br.op_amount = routeindices.size() - routeindex;
				subbreaks.push_back(br);
			}
		}
		if (useasbasis) {
			breaks[i].lang[lang] = true;
			for (k = 0; k < breaks[i].sub.size(); k++)
				if (breaks[i].sub[k].lang[breaks[i].GetFirstLang()])
					breaks[i].sub[k].lang[lang] = true;
			for (j = 0; j < subbreaks.size(); j++) {
				for (k = 0; k < breaks[i].sub.size(); k++) {
					if (breaks[i].sub[k].op_start_relative < subbreaks[j].op_start_relative) {
						subbreaks[j].op_start_relative += breaks[i].sub[k].op_amount;

					}
				}
				if (subbreaks[j].lang[lang]) {
					subbreaks[j].op_start_absolute = op.size();
					for (k = 0; k < subbreaks[j].op_amount; k++)
						op.push_back(langop[subbreaks[j].op_start_relative + k]);
				} else {

				}
				breaks[i].InsertSub(subbreaks[j]);
			}
			// TODO
			return;
		}
	}
	ScriptLanguageBreakPoint rootbreak(false);
	rootbreak.lang[lang] = true;
	rootbreak.op_start_absolute = op.size();
	rootbreak.op_start_relative = op.size();
	rootbreak.op_amount = langop.size();
	breaks.push_back(rootbreak);
	op.insert(op.end(), langop.begin(), langop.end());
}
*/

void ScriptFunction::Read(fstream& f, unsigned int length, SteamLanguage lang) {
	if (true) {
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
	vector<int32_t>& routeindices = indices[lang];
	for (unsigned int i = 0; i < indices[lang].size(); i++)
		if (routeindices[i] >= 0)
			op[indices[lang][i]].Write(f);
}

void ScriptFunction::WritePPF(fstream& f, SteamLanguage lang) {
	vector<int32_t>& routeindices = indices[lang];
	for (unsigned int i = 0; i < routeindices.size(); i++)
		if (routeindices[i] >= 0)
			op[routeindices[i]].WritePPF(f);
}

void ScriptFunction::ReadHWS(fstream& f, unsigned int length, SteamLanguage lang) {
	if (lang == STEAM_LANGUAGE_AMOUNT) {
		unsigned int i;
		uint32_t count;
		for (lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
			HWSReadLong(f, count);
			indices[lang].resize(count);
			for (i = 0; i < count; i++)
				HWSReadFlexibleChar(f, indices[lang][i], true);
		}
		HWSReadLong(f, count);
		op.resize(count);
		for (i = 0; i < op.size(); i++) {
			op[i].parent = this;
			op[i].ReadHWS(f);
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
	}
}

void ScriptFunction::WriteHWS(fstream& f) {
	unsigned int i;
	for (SteamLanguage lang = 0; lang < STEAM_LANGUAGE_AMOUNT; lang++) {
		HWSWriteLong(f, indices[lang].size());
		for (i = 0; i < indices[lang].size(); i++)
			HWSWriteFlexibleChar(f, indices[lang][i], true);
	}
	HWSWriteLong(f, op.size());
	for (i = 0; i < op.size(); i++)
		op[i].WriteHWS(f);
}

void ScriptFunction::WriteSteam(fstream& f, SteamLanguage lang) {
	vector<int32_t>& routeindices = indices[lang];
	for (unsigned int i = 0; i < routeindices.size(); i++)
		if (routeindices[i] >= 0)
			op[routeindices[i]].WriteSteam(f);
}

unsigned int ScriptFunction::GetLength(SteamLanguage lang) {
	vector<int32_t>& routeindices = indices[lang];
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
	func[entryid].insert(func[entryid].begin() + funcidpos, newfunc);
	entry_function_amount[entryid]++;
}

void ScriptDataStruct::RemoveFunction(int entryid, int funcid) {
	entry_function_amount[entryid]--;
	func[entryid].erase(func[entryid].begin() + funcid);
}

void ScriptDataStruct::AddEntry(int entrypos, uint8_t entrytype) {
	unsigned int i, j, k, l, m;
	int varargtype;
	for (i = 0; i < entry_amount; i++)
		for (j = 0; j < entry_function_amount[i]; j++)
			for (k = 0; k < func[i][j].op.size(); k++)
				if (func[i][j].op[k].opcode != 0x06 && func[i][j].op[k].opcode != 0x0B && func[i][j].op[k].opcode != 0x29)
					for (l = 0; l < func[i][j].op[k].arg_amount; l++) {
						if (!func[i][j].op[k].arg[l].is_var && IsScriptArgTypeEntry(func[i][j].op[k], l) && (int)func[i][j].op[k].arg[l].value >= entrypos && func[i][j].op[k].arg[l].value < ENTRY_MAX_VALID_ID) {
							func[i][j].op[k].arg[l].value++;
						} else if (func[i][j].op[k].arg[l].is_var) {
							for (m = 0; m < func[i][j].op[k].arg[l].size; m++) {
								varargtype = VarOpList[func[i][j].op[k].arg[l].var[m]].type;
								if (varargtype == 55) {
									if (func[i][j].op[k].arg[l].var[m + 1] >= entrypos && func[i][j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID)
										func[i][j].op[k].arg[l].var[m + 1]++;
									m += 2;
								} else if (varargtype == 3 || varargtype == 5 || (varargtype >= 10 && varargtype < 20)) {
									if (func[i][j].op[k].arg[l].var[m] == 0x5F && func[i][j].op[k].arg[l].var[m + 1] >= entrypos && func[i][j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID)
										func[i][j].op[k].arg[l].var[m + 1]++;
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
	entry_amount++;
	func.insert(func.begin() + entrypos, vector<ScriptFunction>());
	entry_offset.insert(entry_offset.begin() + entrypos, 0);
	entry_size.insert(entry_size.begin() + entrypos, 0);
	entry_local_var.insert(entry_local_var.begin() + entrypos, 0);
	entry_flag.insert(entry_flag.begin() + entrypos, 0);
	entry_type.insert(entry_type.begin() + entrypos, entrytype);
	entry_function_amount.insert(entry_function_amount.begin() + entrypos, 0);
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
			for (j = 0; j < entry_function_amount[i]; j++)
				for (k = 0; k < func[i][j].op.size(); k++)
					if (func[i][j].op[k].opcode != 0x06 && func[i][j].op[k].opcode != 0x0B && func[i][j].op[k].opcode != 0x29)
						for (l = 0; l < func[i][j].op[k].arg_amount; l++) {
							if (!func[i][j].op[k].arg[l].is_var && IsScriptArgTypeEntry(func[i][j].op[k], l)) {
								if (func[i][j].op[k].arg[l].value == entrypos) {
									func[i][j].op[k].arg[l].value = 0;
									res++;
								} else if ((int)func[i][j].op[k].arg[l].value > entrypos && func[i][j].op[k].arg[l].value < ENTRY_MAX_VALID_ID) {
									func[i][j].op[k].arg[l].value--;
								}
							} else if (func[i][j].op[k].arg[l].is_var) {
								for (m = 0; m < func[i][j].op[k].arg[l].size; m++) {
									varargtype = VarOpList[func[i][j].op[k].arg[l].var[m]].type;
									if (varargtype == 55) {
										if (func[i][j].op[k].arg[l].var[m + 1] == entrypos) {
											func[i][j].op[k].arg[l].var[m + 1] = 0;
											res++;
										} else if (func[i][j].op[k].arg[l].var[m + 1] > entrypos && func[i][j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID) {
											func[i][j].op[k].arg[l].var[m + 1]--;
										}
										m += 2;
									} else if (varargtype == 3 || varargtype == 5 || (varargtype >= 10 && varargtype < 20)) {
										if (func[i][j].op[k].arg[l].var[m] == 0x5F) {
											if (func[i][j].op[k].arg[l].var[m + 1] == entrypos) {
												func[i][j].op[k].arg[l].var[m + 1] = 0;
												res++;
											} else if (func[i][j].op[k].arg[l].var[m + 1] > entrypos && func[i][j].op[k].arg[l].var[m + 1] < ENTRY_MAX_VALID_ID) {
												func[i][j].op[k].arg[l].var[m + 1]--;
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
	func.erase(func.begin() + entrypos);
	entry_offset.erase(entry_offset.begin() + entrypos);
	entry_size.erase(entry_size.begin() + entrypos);
	entry_local_var.erase(entry_local_var.begin() + entrypos);
	entry_flag.erase(entry_flag.begin() + entrypos);
	entry_type.erase(entry_type.begin() + entrypos);
	entry_function_amount.erase(entry_function_amount.begin() + entrypos);
	local_data.erase(local_data.begin() + entrypos);
	return res;
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
		for (j = 0; j < entry_function_amount[i]; j++)
			count += shifter(func[i][j], argtype, shift);
	return count;
}

// TODO: apply entry shift to the incoming function.Read when "entryprevamount > entry_amount"
#define MACRO_SCRIPT_IOFUNCTION_PART1(IO, SEEK, READ, PPF) \
	unsigned int i, j, k, entryi, funcj, entryprevamount; \
	vector<uint16_t> funcpoint; \
	vector<int> funcreorder; \
	uint16_t functype; \
	uint32_t entry_pos, local_entry_pos, function_pos; \
	uint32_t zero32 = 0; \
	uint16_t zero16 = 0; \
	uint8_t zero8 = 0; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f, magic_number); \
	IO ## Char(f, header_unknown1); \
	IO ## Char(f, entry_amount); \
	entryprevamount = entry_offset.size(); \
	EntryIdShift = (READ && entryprevamount >= SCRIPT_FIXED_ENTRY_AMOUNT && entryprevamount > entry_amount) ? entryprevamount - entry_amount : 0; \
	EntryIdShift_Base = entry_amount - SCRIPT_FIXED_ENTRY_AMOUNT; \
	if (READ && entry_amount > entryprevamount) { \
		if (entryprevamount >= SCRIPT_FIXED_ENTRY_AMOUNT) { \
			uint8_t extra_count = entry_amount - entryprevamount; \
			entry_amount = entryprevamount; \
			for (i = 0; i < extra_count; i++) \
				AddEntry(entry_offset.size() - SCRIPT_FIXED_ENTRY_AMOUNT, 0); \
		} else { \
			entry_offset.resize(entry_amount); \
			entry_size.resize(entry_amount); \
			entry_local_var.resize(entry_amount); \
			entry_flag.resize(entry_amount); \
			entry_type.resize(entry_amount); \
			entry_function_amount.resize(entry_amount); \
			func.resize(entry_amount); \
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
		entryi = i + SCRIPT_FIXED_ENTRY_AMOUNT < entry_amount ? i : i + entry_offset.size() - entry_amount; \
		IO ## Short(f, entry_offset[entryi]); \
		IO ## Short(f, entry_size[entryi]); \
		IO ## Char(f, entry_local_var[entryi]); \
		IO ## Char(f, entry_flag[entryi]); \
		IO ## Short(f, zero16); \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i = 0; i < entry_amount; i++) { \
		entryi = i + SCRIPT_FIXED_ENTRY_AMOUNT < entry_amount ? i : i + entry_offset.size() - entry_amount; \
		if (entry_size[entryi] > 0) { \
			CurrentScriptReadEntry = entryi; \
			SEEK(f, entry_pos, entry_offset[entryi]); \
			local_entry_pos = f.tellg(); \
			if (PPF) PPFInitScanStep(f); \
			IO ## Char(f, entry_type[entryi]); \
			IO ## Char(f, entry_function_amount[entryi]); \
			funcreorder.clear(); \
			funcpoint.resize(entry_function_amount[entryi]); \
			function_pos = f.tellg(); \
			for (j = 0; j < entry_function_amount[entryi]; j++) { \
				if (READ) { \
					IO ## Short(f, functype); \
					IO ## Short(f, funcpoint[j]); \
					for (funcj = 0; funcj < func[entryi].size(); funcj++) \
						if (func[entryi][funcj].function_type == functype) \
							break; \
					if (funcj == func[entryi].size()) \
						func[entryi].resize(funcj + 1); \
					else \
						for (k = 0; k < funcreorder.size(); k++) \
							if (funcreorder[k] == funcj) \
								funcreorder[k] = -1; \
					funcreorder.push_back(funcj); \
					func[entryi][funcj].function_type = functype; \
					func[entryi][funcj].function_point = funcpoint[j]; \
					func[entryi][funcj].parent = this; \
				} else { \
					IO ## Short(f, func[entryi][j].function_type); \
					IO ## Short(f, func[entryi][j].function_point); \
					funcpoint[j] = func[entryi][j].function_point; \
				} \
			} \
			if (PPF) PPFEndScanStep(); \
			for (j = 0; j < entry_function_amount[entryi]; j++) { \
				if (READ && funcreorder[j] < 0) \
					continue; \
				funcj = READ ? funcreorder[j] : j; \
				SEEK(f, function_pos, funcpoint[j]); \
				CurrentScriptReadFunction = func[entryi][j].function_type; \
				uint32_t length; \
				if (j + 1 < entry_function_amount[entryi]) \
					length = funcpoint[j + 1] - funcpoint[j]; \
				else \
					length = entry_size[entryi] - funcpoint[j] - 2;

#define MACRO_SCRIPT_IOFUNCTION_PART2(IO, SEEK, READ, PPF) \
			} \
			if (READ) entry_function_amount[entryi] = func[entryi].size(); \
			while (f.tellg() < local_entry_pos + entry_size[entryi]) \
				IO ## Char(f, zero8); \
		} else if (READ && entry_function_amount[entryi] == 0) { \
			entry_type[entryi] = 0xFF; \
		} \
	} \
	if (READ) entry_amount = entry_offset.size(); 

int dbgscriptcounter = 0;
void ScriptDataStruct::Read(fstream& f, SteamLanguage lang) {
	LoadCustomScriptUtility();
	CurrentScriptReadField = object_id;
	//if (dbgscriptcounter++ > 800 * STEAM_LANGUAGE_AMOUNT) dbgscript = true;
	if (dbgscript) {
		GetDebugLog() << "FIELD " << (int)object_id << endl;
	}
	if (GetGameType() == GAME_TYPE_PSX && parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type == CLUSTER_TYPE_WORLD_MAP && object_id >= 9100) {
		ChunkChild::Read(f);
	} else {
		if (GetGameType() == GAME_TYPE_PSX) {
			MACRO_SCRIPT_IOFUNCTION_PART1(FFIXRead, FFIXSeek, true, false)
			func[entryi][funcj].Read(f, length, lang);
			MACRO_SCRIPT_IOFUNCTION_PART2(FFIXRead, FFIXSeek, true, false)
			name.ReadFromChar(header_name[lang]);
		} else {
			MACRO_SCRIPT_IOFUNCTION_PART1(SteamRead, SteamSeek, true, false)
			func[entryi][funcj].Read(f, length, lang);
			MACRO_SCRIPT_IOFUNCTION_PART2(SteamRead, SteamSeek, true, false)
			FF9String dummystr;
			dummystr.ReadFromChar(header_name[lang]);
			if (!name.created)
				name.CreateEmpty();
			name.SetValue(dummystr.str, lang);
		}
		local_data.resize(entry_amount);
		for (unsigned int i = 0; i < entry_amount; i++) {
			local_data[i].allocate_amount = entry_local_var[i];
			local_data[i].amount = 0;
		}
		global_data.allocate_amount = 0;
		global_data.amount = 0;
		loaded = true;
	}
	loaded = true;
}

void ScriptDataStruct::Write(fstream& f) {
	if (parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type == CLUSTER_TYPE_WORLD_MAP && object_id >= 9100) {
		ChunkChild::Write(f);
	} else {
		SteamLanguage lang = GetSteamLanguage();
		MACRO_SCRIPT_IOFUNCTION_PART1(FFIXWrite, FFIXSeek, false, false)
		func[entryi][funcj].Write(f, lang);
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
		func[entryi][funcj].WritePPF(f, lang);
		MACRO_SCRIPT_IOFUNCTION_PART2(PPFStepAdd, FFIXSeek, false, true)
	}
}

void ScriptDataStruct::ReadHWS(fstream& f, bool usetext, SteamLanguage lang) {
	// TODO: all langs
	MACRO_SCRIPT_IOFUNCTION_PART1(HWSRead, HWSSeek, true, false)
	if (lang == STEAM_LANGUAGE_AMOUNT)
		func[entryi][funcj].ReadHWS(f, length, lang);
	else
		func[entryi][funcj].ReadHWS(f, length, lang);
	MACRO_SCRIPT_IOFUNCTION_PART2(HWSRead, HWSSeek, true, false)
	//if (usetext)
	//	name.ReadFromChar(header_name);
	MarkDataModified();
}

void ScriptDataStruct::WriteHWS(fstream& f) {
	SteamLanguage lang = GetSteamLanguage();
	// TODO: all langs
	MACRO_SCRIPT_IOFUNCTION_PART1(HWSWrite, HWSSeek, false, false)
	func[entryi][funcj].WriteHWS(f);
	MACRO_SCRIPT_IOFUNCTION_PART2(HWSWrite, HWSSeek, false, false)
}

void ScriptDataStruct::WriteSteam(fstream& f, SteamLanguage lang) {
	UpdateOffset(lang);
	MACRO_SCRIPT_IOFUNCTION_PART1(HWSWrite, HWSSeek, false, false)
	vector<int32_t>& langindices = func[entryi][funcj].indices[lang];
	for (k = 0; k < langindices.size(); k++)
		if (langindices[k] >= 0)
			func[entryi][funcj].op[langindices[k]].WriteHWS(f);
	MACRO_SCRIPT_IOFUNCTION_PART2(HWSWrite, HWSSeek, false, false)
}

void ScriptDataStruct::ReadLocalHWS(fstream& f) {
	uint32_t localpos, entrypos, vardatapos, localsize;
	uint16_t entryheadersize, vardatasize, tmp16;
	uint8_t entryam, vardatatype, localam;
	unsigned int i, j;
	localpos = f.tellg();
	HWSReadLong(f, localsize);
	HWSReadChar(f, entryam);
	for (i = 0; i < entryam; i++) {
		entrypos = f.tellg();
		HWSReadShort(f, entryheadersize);
		if (i < entry_amount) {
			HWSReadChar(f, local_data[i].allocate_amount);
			HWSReadChar(f, (uint8_t&)local_data[i].amount);
			entry_local_var[i] = local_data[i].allocate_amount;
		} else {
			HWSReadChar(f, localam);
			HWSReadChar(f, localam);
		}
		HWSSeek(f, entrypos, entryheadersize);
		if (i < entry_amount) {
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
		} else {
			for (j = 0; j < localam; j++) {
				HWSSeek(f, f.tellg(), 7);
				HWSReadShort(f, tmp16);
				while (tmp16 != 0)
					HWSReadShort(f, tmp16);
				HWSReadChar(f, vardatatype);
				while (vardatatype != 0) {
					vardatapos = f.tellg();
					HWSReadShort(f, vardatasize);
					HWSSeek(f, vardatapos, vardatasize);
					HWSReadChar(f, vardatatype);
				}
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
	name = from.name;
	func = from.func;
	magic_number = from.magic_number;
	header_unknown1 = from.header_unknown1;
	entry_amount = from.entry_amount;
	entry_offset = from.entry_offset;
	entry_size = from.entry_size;
	entry_local_var = from.entry_local_var;
	entry_flag = from.entry_flag;
	entry_type = from.entry_type;
	entry_function_amount = from.entry_function_amount;
	global_data = from.global_data;
	local_data = from.local_data;
	related_charmap_id = from.related_charmap_id;
	for (i = 0; i < 20; i++) {
		header_unknown2[i] = from.header_unknown2[i];
		header_unknown3[i] = from.header_unknown3[i];
	}
	for (i = 0; i < STEAM_LANGUAGE_AMOUNT; i++)
		for (j = 0; j < SCRIPT_NAME_MAX_LENGTH; j++)
			header_name[i][j] = from.header_name[i][j];
	for (i = 0; i < entry_amount; i++)
		for (j = 0; j < entry_function_amount[i]; j++)
			func[i][j].parent = this;
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
		if (entry_function_amount[i] > 0) {
			entry_offset[i] = entrypos;
			funcpos = 4 * entry_function_amount[i];
			for (j = 0; j < entry_function_amount[i]; j++) {
				func[i][j].function_point = funcpos;
				funcpos += func[i][j].GetLength(lang);
			}
			entry_size[i] = funcpos + 2;
			if (entry_size[i] % 4)
				entry_size[i] += 4 - entry_size[i] % 4;
			lastentrypos = entrypos;
			entrypos += entry_size[i];
		} else {
			entry_offset[i] = entrypos;
			entry_size[i] = 0;
		}
	}
	if (entry_amount > 0) // Just for mimicking the game's behavior
		for (i = entry_amount - 1; entry_size[i] == 0; i--)
			entry_offset[i] = lastentrypos;
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
		if (entry_function_amount[i] > 0) {
			entry_offset[i] = entrypos;
			funcpos = 4 * entry_function_amount[i];
			for (j = 0; j < entry_function_amount[i]; j++) {
				func[i][j].function_point = funcpos;
				funcpos += func[i][j].GetLength(lang);
			}
			entry_size[i] = funcpos + 2;
			if (entry_size[i] % 4)
				entry_size[i] += 4 - entry_size[i] % 4;
			lastentrypos = entrypos;
			entrypos += entry_size[i];
		} else {
			entry_offset[i] = entrypos;
			entry_size[i] = 0;
		}
	}
	if (entry_amount > 0) // Just for mimicking the game's behavior
		for (i = entry_amount - 1; entry_size[i] == 0; i--)
			entry_offset[i] = lastentrypos;
	totalsize += entrypos;
	SetSize(totalsize);
}
