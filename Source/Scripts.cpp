#include "Scripts.h"

#include <sstream>
#include "Hades_Strings.h"
#include "Database_Script.h"

bool ScriptArgument::SetValue(long long newvalue) {
	if (typesize==2 && !is_signed) {
		SteamLanguage curlang = parent->parent->parent->current_language;
		if (curlang!=STEAM_LANGUAGE_NONE && parent->parent->parent->multi_lang_script!=NULL && parent->parent->parent->multi_lang_script->base_script_lang[curlang]!=curlang)
			for (unsigned int i=0;i<parent->arg_amount;i++)
				if (this==&parent->arg[i] && (int)i<HADES_STRING_SCRIPT_OPCODE[parent->opcode].arg_amount && HADES_STRING_SCRIPT_OPCODE[parent->opcode].arg_type[i]==AT_TEXT) {
					MultiLanguageScriptDataStruct* multilangdata = parent->parent->parent->multi_lang_script;
					SteamLanguage baselang = multilangdata->base_script_lang[curlang];
					for (unsigned int j=0;j<multilangdata->lang_script_text_id[curlang].size();j++)
						if (newvalue==multilangdata->lang_script_text_id[curlang][j]) {
							newvalue = multilangdata->base_script_text_id[curlang][j];
							break;
						}
					break;
				}
	}
	if (newvalue<0) {
		uint32_t sign = 0x1 << (typesize*8-1);
		value = ~(-newvalue-1 & ~sign) | sign;
	} else
		value = newvalue;
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

int64_t ScriptArgument::GetValue() {
	if (is_signed) {
		if (typesize==1) {
			int8_t* num = (int8_t*)&value;
			return *num;
		} else if (typesize==2) {
			int16_t* num = (int16_t*)&value;
			return *num;
		}
		int32_t* num = (int32_t*)&value;
		return *num;
	} else {
		if (typesize==1) {
			uint8_t* num = (uint8_t*)&value;
			return *num;
		} else if (typesize==2) {
			uint16_t num = *(uint16_t*)&value;
			SteamLanguage curlang = parent->parent->parent->current_language;
			if (curlang!=STEAM_LANGUAGE_NONE && parent->parent->parent->multi_lang_script!=NULL && parent->parent->parent->multi_lang_script->base_script_lang[curlang]!=curlang)
				for (unsigned int i=0;i<parent->arg_amount;i++)
					if (this==&parent->arg[i] && (int)i<HADES_STRING_SCRIPT_OPCODE[parent->opcode].arg_amount && HADES_STRING_SCRIPT_OPCODE[parent->opcode].arg_type[i]==AT_TEXT) {
						MultiLanguageScriptDataStruct* multilangdata = parent->parent->parent->multi_lang_script;
						SteamLanguage baselang = multilangdata->base_script_lang[curlang];
						for (unsigned int j=0;j<multilangdata->base_script_text_id[curlang].size();j++)
							if (num==multilangdata->base_script_text_id[curlang][j]) {
								num = multilangdata->lang_script_text_id[curlang][j];
								break;
							}
						break;
					}
			return num;
		}
	}
	return value;
}

#define MACRO_SCRIPT_IOFUNCTION_ARGREAD(IO,SEEK) \
	unsigned int i; \
	uint8_t varbyte; \
	typesize = sz; \
	if (is_var) { \
		int vartype; \
		size = 0; \
		IO ## Char(f,varbyte); var.push_back(varbyte); \
		vartype = VarOpList[varbyte].type; \
		while (vartype!=-1) { \
			if (vartype==2) { \
			} else if (vartype==3) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} else if (vartype==5) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} else if (vartype==6) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} else if (vartype==7) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} else if (vartype>=10 && vartype<20) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} else if (vartype>=20 && vartype<30) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} else if (vartype==55) { \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
				IO ## Char(f,varbyte); var.push_back(varbyte); \
			} \
			IO ## Char(f,varbyte); var.push_back(varbyte); \
			vartype = VarOpList[varbyte].type; \
		} \
		size = var.size(); \
	} else { \
		size = typesize; \
		value = 0; \
		for (i=0;i<size;i++) { \
			IO ## Char(f,varbyte); \
			value |= (varbyte << i*8); \
		} \
	}

#define MACRO_SCRIPT_IOFUNCTION_ARGWRITE(IO,SEEK,PPF) \
	unsigned int i; \
	uint8_t buffer; \
	if (PPF) PPFInitScanStep(f); \
	if (is_var) \
		for (i=0;i<size;i++) \
			IO ## Char(f,var[i]); \
	else if (size!=2 || !is_signed) \
		for (i=0;i<size;i++) { \
			buffer = (value >> i*8) & 0xFF; \
			IO ## Char(f,buffer); \
		} \
	else { \
		uint16_t translatedval = GetValue(); \
		for (i=0;i<size;i++) { \
			buffer = (translatedval >> i*8) & 0xFF; \
			IO ## Char(f,buffer); \
		} \
	} \
	if (PPF) PPFEndScanStep();


void ScriptArgument::Read(fstream& f, uint8_t sz, bool isvar, bool issign) {
	is_var = isvar;
	is_signed = issign;
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SCRIPT_IOFUNCTION_ARGREAD(FFIXRead,FFIXSeek)
	} else {
		MACRO_SCRIPT_IOFUNCTION_ARGREAD(SteamRead,SteamSeek)
	}
}

void ScriptArgument::Write(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(FFIXWrite,FFIXSeek,false)
}

void ScriptArgument::WritePPF(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(PPFStepAdd,FFIXSeek,true)
}

void ScriptArgument::ReadHWS(fstream& f, uint8_t sz, bool isvar, bool issign) {
	is_var = isvar;
	is_signed = issign;
	MACRO_SCRIPT_IOFUNCTION_ARGREAD(HWSRead,HWSSeek)
}

void ScriptArgument::WriteHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_ARGWRITE(HWSWrite,HWSSeek,false)
}

vector<ScriptArgument> NewScriptArgumentArray(unsigned int amount, ScriptOperation* p) {
	vector<ScriptArgument> res(amount);
	for (unsigned int i=0;i<amount;i++)
		res[i].parent = p;
	return res;
}

bool IsScriptArgTypeSigned(uint8_t argtype) {
	return argtype==AT_JUMP || argtype==AT_SPIN || argtype==AT_POSITION_X || argtype==AT_POSITION_Y || argtype==AT_POSITION_Z;
}

#define MACRO_SCRIPT_IOFUNCTION_OPREAD(IO,SEEK,FUNC) \
	unsigned int i; \
	IO ## Char(f,base_code); \
	if (base_code==0xFF) { \
		IO ## Char(f,ext_code); \
		opcode = 0x100+ext_code; \
		size = 2; \
	} else { \
		opcode = base_code; \
		size = 1; \
	} \
	if (opcode==0x06) { \
		IO ## Char(f,size_byte); \
		arg_amount = 1+size_byte*2; \
		arg = NewScriptArgumentArray(arg_amount,this); \
		arg[0].FUNC(f,2,false,true); \
		for (i=0;i<size_byte;i++) { \
			arg[2*i+1].FUNC(f,2,false,false); \
			arg[2*i+2].FUNC(f,2,false,true); \
		} \
		size += 3+size_byte*4; \
	} else if (opcode==0x0B) { \
		IO ## Char(f,size_byte); \
		arg_amount = 2+size_byte; \
		arg = NewScriptArgumentArray(arg_amount,this); \
		arg[0].FUNC(f,2,false,false); \
		arg[1].FUNC(f,2,false,true); \
		for (i=2;i<arg_amount;i++) \
			arg[i].FUNC(f,2,false,true); \
		size += 5+size_byte*2; \
	} else if (opcode==0x29) { \
		uint8_t flag = 1; \
		IO ## Char(f,vararg_flag); \
		IO ## Char(f,size_byte); \
		arg_amount = size_byte; \
		arg = NewScriptArgumentArray(arg_amount,this); \
		size += 2; \
		for (i=0;i<arg_amount;i++) { \
			arg[i].FUNC(f,4,flag & vararg_flag,true); \
			size += arg[i].size; \
			flag *= 2; \
		} \
	} else { \
		arg_amount = HADES_STRING_SCRIPT_OPCODE[opcode].arg_amount; \
		arg = NewScriptArgumentArray(arg_amount,this); \
		if (HADES_STRING_SCRIPT_OPCODE[opcode].use_vararg) { \
			uint8_t flag = 1; \
			IO ## Char(f,vararg_flag); \
			size += 1; \
			for (i=0;i<arg_amount;i++) { \
				arg[i].FUNC(f,HADES_STRING_SCRIPT_OPCODE[opcode].arg_length[i],flag & vararg_flag,IsScriptArgTypeSigned(HADES_STRING_SCRIPT_OPCODE[opcode].arg_type[i])); \
				size += arg[i].size; \
				flag *= 2; \
			} \
		} else { \
			for (i=0;i<arg_amount;i++) { \
				arg[i].FUNC(f,HADES_STRING_SCRIPT_OPCODE[opcode].arg_length[i],opcode==0x05,IsScriptArgTypeSigned(HADES_STRING_SCRIPT_OPCODE[opcode].arg_type[i])); \
				size += arg[i].size; \
			} \
		} \
	}

#define MACRO_SCRIPT_IOFUNCTION_OPWRITE(IO,SEEK,PPF,FUNC) \
	unsigned int i; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Char(f,base_code); \
	if (base_code==0xFF) \
		IO ## Char(f,ext_code); \
	if (opcode==0x06) { \
		IO ## Char(f,size_byte); \
		if (PPF) PPFEndScanStep(); \
		arg[0].FUNC(f); \
		for (i=1;i<arg_amount;i++) \
			arg[i].FUNC(f); \
	} else if (opcode==0x0B) { \
		IO ## Char(f,size_byte); \
		if (PPF) PPFEndScanStep(); \
		arg[0].FUNC(f); \
		arg[1].FUNC(f); \
		for (i=2;i<arg_amount;i++) \
			arg[i].FUNC(f); \
	} else if (opcode==0x29) { \
		IO ## Char(f,vararg_flag); \
		IO ## Char(f,size_byte); \
		if (PPF) PPFEndScanStep(); \
		for (i=0;i<arg_amount;i++) \
			arg[i].FUNC(f); \
	} else { \
		if (HADES_STRING_SCRIPT_OPCODE[opcode].use_vararg) { \
			IO ## Char(f,vararg_flag); \
			if (PPF) PPFEndScanStep(); \
			for (i=0;i<arg_amount;i++) \
				arg[i].FUNC(f); \
		} else { \
			if (PPF) PPFEndScanStep(); \
			for (i=0;i<arg_amount;i++) \
				arg[i].FUNC(f); \
		} \
	}

void ScriptOperation::Read(fstream& f) {
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SCRIPT_IOFUNCTION_OPREAD(FFIXRead,FFIXSeek,Read)
	} else {
		MACRO_SCRIPT_IOFUNCTION_OPREAD(SteamRead,SteamSeek,Read)
	}
}

void ScriptOperation::Write(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(FFIXWrite,FFIXSeek,false,Write)
}

void ScriptOperation::WritePPF(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(PPFStepAdd,FFIXSeek,true,WritePPF)
}

void ScriptOperation::ReadHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPREAD(HWSRead,HWSSeek,ReadHWS)
}

void ScriptOperation::WriteHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION_OPWRITE(HWSWrite,HWSSeek,false,WriteHWS)
}

ScriptOperation& ScriptOperation::operator=(const ScriptOperation& from) {
	parent = from.parent;
	opcode = from.opcode;
	base_code = from.base_code;
	ext_code = from.ext_code;
	vararg_flag = from.vararg_flag;
	size_byte = from.size_byte;
	arg_amount = from.arg_amount;
	size = from.size;
	arg = from.arg;
	for (unsigned int i=0;i<arg_amount;i++)
		arg[i].parent = this;
	return *this;
}

ScriptOperation::ScriptOperation(const ScriptOperation& from) :
	parent(from.parent),
	opcode(from.opcode),
	base_code(from.base_code),
	ext_code(from.ext_code),
	vararg_flag(from.vararg_flag),
	size_byte(from.size_byte),
	arg_amount(from.arg_amount),
	size(from.size),
	arg(from.arg) {
	for (unsigned int i=0;i<arg_amount;i++)
		arg[i].parent = this;
}

void ScriptFunction::Read(fstream& f) {
	unsigned int len = 0;
	op_amount = 0;
	op.clear();
	while (len<length) {
		op.resize(op_amount+1);
		op[op_amount].parent = this;
		op[op_amount].Read(f);
		len += op[op_amount++].size;
	}
}

void ScriptFunction::Write(fstream& f) {
	for (unsigned int i=0;i<op_amount;i++)
		op[i].Write(f);
}

void ScriptFunction::WritePPF(fstream& f) {
	for (unsigned int i=0;i<op_amount;i++)
		op[i].WritePPF(f);
}

void ScriptFunction::ReadHWS(fstream& f) {
	unsigned int len = 0;
	op_amount = 0;
	op.clear();
	while (len<length) {
		op.resize(op_amount+1);
		op[op_amount].parent = this;
		op[op_amount].ReadHWS(f);
		len += op[op_amount++].size;
	}
}

void ScriptFunction::WriteHWS(fstream& f) {
	for (unsigned int i=0;i<op_amount;i++)
		op[i].WriteHWS(f);
}

ScriptFunction& ScriptFunction::operator=(const ScriptFunction& from) {
	parent = from.parent;
	op_amount = from.op_amount;
	length = from.length;
	op = from.op;
	for (unsigned int i=0;i<op_amount;i++)
		op[i].parent = this;
	return *this;
}

ScriptFunction::ScriptFunction(const ScriptFunction& from) :
	parent(from.parent),
	op_amount(from.op_amount),
	length(from.length),
	op(from.op) {
	for (unsigned int i=0;i<op_amount;i++)
		op[i].parent = this;
}

int ScriptDataStruct::SetName(wstring newvalue, SteamLanguage lang) {
	if (GetGameType()==GAME_TYPE_PSX) {
		FF9String tmp(name);
		tmp.SetValue(newvalue);
		if (tmp.length>SCRIPT_NAME_MAX_LENGTH)
			return 1;
	}
	name.SetValue(newvalue,lang);
	return 0;
}

int ScriptDataStruct::SetName(FF9String& newvalue) {
	if (GetGameType()==GAME_TYPE_PSX && newvalue.length>SCRIPT_NAME_MAX_LENGTH)
		return 1;
	name = newvalue;
	return 0;
}

void ScriptDataStruct::AddFunction(int entryid, int funcidpos, uint16_t functype) {
	ScriptFunction newfunc;
	newfunc.parent = this;
	newfunc.op_amount = 0;
	newfunc.length = 0;
	func[entryid].insert(func[entryid].begin()+funcidpos,newfunc);
	function_type[entryid].insert(function_type[entryid].begin()+funcidpos,functype);
	function_point[entryid].insert(function_point[entryid].begin()+funcidpos,0);
	entry_function_amount[entryid]++;
}

int ScriptDataStruct::RemoveFunction(int entryid, int funcid) {
	int res = max((unsigned int)4,func[entryid][funcid].length); // Don't count the extra 4 bytes because of padding
	entry_function_amount[entryid]--;
	func[entryid].erase(func[entryid].begin()+funcid);
	function_type[entryid].erase(function_type[entryid].begin()+funcid);
	function_point[entryid].erase(function_point[entryid].begin()+funcid);
	return res;
}

void ScriptDataStruct::AddEntry(int entrypos, uint8_t entrytype) {
	unsigned int i,j,k,l,m;
	bool isargentry;
	int varargtype;
	for (i=0;i<entry_amount;i++)
		for (j=0;j<entry_function_amount[i];j++)
			for (k=0;k<func[i][j].op_amount;k++)
				if (func[i][j].op[k].opcode!=0x06 && func[i][j].op[k].opcode!=0x0B && func[i][j].op[k].opcode!=0x29)
					for (l=0;l<func[i][j].op[k].arg_amount;l++) {
						isargentry = !func[i][j].op[k].arg[l].is_var && HADES_STRING_SCRIPT_OPCODE[func[i][j].op[k].opcode].arg_type[l]==AT_ENTRY;
						isargentry = isargentry || (func[i][j].op[k].opcode==0xB3 || func[i][j].op[k].opcode==0xDA) && l==2 && !func[i][j].op[k].arg[l].is_var && !func[i][j].op[k].arg[1].is_var && func[i][j].op[k].arg[1].value==150;
						if (isargentry && func[i][j].op[k].arg[l].value>=entrypos && func[i][j].op[k].arg[l].value<entry_amount) {
							func[i][j].op[k].arg[l].value++;
						} else if (func[i][j].op[k].arg[l].is_var) {
							for (m=0;m<func[i][j].op[k].arg[l].size;m++) {
								varargtype = VarOpList[func[i][j].op[k].arg[l].var[m]].type;
								if (varargtype==55) {
									if (func[i][j].op[k].arg[l].var[m+1]>=entrypos && func[i][j].op[k].arg[l].var[m+1]<entry_amount) {
										func[i][j].op[k].arg[l].var[m+1]++;
									}
								} else if (varargtype==3 || varargtype==5 || (varargtype>=10 && varargtype<20)) {
									m++;
								} else if (varargtype==6 || (varargtype>=20 && varargtype<30)) {
									m += 2;
								} else if (varargtype==7) {
									m += 4;
								}
							}
						}
					}
	entry_amount++;
	func.insert(func.begin()+entrypos,vector<ScriptFunction>());
	function_type.insert(function_type.begin()+entrypos,vector<uint16_t>());
	function_point.insert(function_point.begin()+entrypos,vector<uint16_t>());
	entry_offset.insert(entry_offset.begin()+entrypos,0);
	entry_size.insert(entry_size.begin()+entrypos,0);
	entry_local_var.insert(entry_local_var.begin()+entrypos,0);
	entry_flag.insert(entry_flag.begin()+entrypos,0);
	entry_type.insert(entry_type.begin()+entrypos,entrytype);
	entry_function_amount.insert(entry_function_amount.begin()+entrypos,0);
	local_data.insert(local_data.begin()+entrypos,ScriptLocalVariableSet());
	local_data[entrypos].allocate_amount = 0;
	local_data[entrypos].amount = 0;
}

int ScriptDataStruct::RemoveEntry(int entrypos, int* modifiedargamount) {
	unsigned int i,j,k,l,m;
	bool isargentry;
	int varargtype;
	int res = 0;
	for (i=0;i<entry_amount;i++)
		if (i!=entrypos)
			for (j=0;j<entry_function_amount[i];j++)
				for (k=0;k<func[i][j].op_amount;k++)
					if (func[i][j].op[k].opcode!=0x06 && func[i][j].op[k].opcode!=0x0B && func[i][j].op[k].opcode!=0x29)
						for (l=0;l<func[i][j].op[k].arg_amount;l++) {
							isargentry = !func[i][j].op[k].arg[l].is_var && HADES_STRING_SCRIPT_OPCODE[func[i][j].op[k].opcode].arg_type[l]==AT_ENTRY;
							isargentry = isargentry || (func[i][j].op[k].opcode==0xB3 || func[i][j].op[k].opcode==0xDA) && l==2 && !func[i][j].op[k].arg[l].is_var && !func[i][j].op[k].arg[1].is_var && func[i][j].op[k].arg[1].value==150;
							if (isargentry) {
								if (func[i][j].op[k].arg[l].value==entrypos) {
									func[i][j].op[k].arg[l].value = 0;
									if (modifiedargamount)
										(*modifiedargamount)++;
								} else if (func[i][j].op[k].arg[l].value>entrypos && func[i][j].op[k].arg[l].value<entry_amount) {
									func[i][j].op[k].arg[l].value--;
								}
							} else if (func[i][j].op[k].arg[l].is_var) {
								for (m=0;m<func[i][j].op[k].arg[l].size;m++) {
									varargtype = VarOpList[func[i][j].op[k].arg[l].var[m]].type;
									if (varargtype==55) {
										if (func[i][j].op[k].arg[l].var[m+1]==entrypos) {
											func[i][j].op[k].arg[l].var[m+1] = 0;
											if (modifiedargamount)
												(*modifiedargamount)++;
										} else if (func[i][j].op[k].arg[l].var[m+1]>entrypos && func[i][j].op[k].arg[l].var[m+1]<entry_amount) {
											func[i][j].op[k].arg[l].var[m+1]--;
										}
									} else if (varargtype==3 || varargtype==5 || (varargtype>=10 && varargtype<20)) {
										m++;
									} else if (varargtype==6 || (varargtype>=20 && varargtype<30)) {
										m += 2;
									} else if (varargtype==7) {
										m += 4;
									}
								}
							}
						}
	for (i=0;i<entry_function_amount[entrypos];i++)
		res += max((unsigned int)4,func[entrypos][i].length);
	res += 0x10;
	entry_amount--;
	func.erase(func.begin()+entrypos);
	function_type.erase(function_type.begin()+entrypos);
	function_point.erase(function_point.begin()+entrypos);
	entry_offset.erase(entry_offset.begin()+entrypos);
	entry_size.erase(entry_size.begin()+entrypos);
	entry_local_var.erase(entry_local_var.begin()+entrypos);
	entry_flag.erase(entry_flag.begin()+entrypos);
	entry_type.erase(entry_type.begin()+entrypos);
	entry_function_amount.erase(entry_function_amount.begin()+entrypos);
	local_data.erase(local_data.begin()+entrypos);
	return res;
}

void ScriptDataStruct::ChangeSteamLanguage(SteamLanguage newlang) {
	if (multi_lang_script==NULL)
		return;
	unsigned int i;
	for (i=0;i<SCRIPT_NAME_MAX_LENGTH;i++)
		multi_lang_script->header_name[current_language][i] = header_name[i];
	for (i=0;i<SCRIPT_NAME_MAX_LENGTH;i++)
		header_name[i] = multi_lang_script->header_name[newlang][i];
	SteamLanguage oldlangbase = multi_lang_script->base_script_lang[current_language];
	SteamLanguage newlangbase = multi_lang_script->base_script_lang[newlang];
	multi_lang_script->func[oldlangbase] = func;
	multi_lang_script->magic_number[oldlangbase] = magic_number;
	multi_lang_script->header_unknown1[oldlangbase] = header_unknown1;
	multi_lang_script->entry_amount[oldlangbase] = entry_amount;
	for (i=0;i<20;i++) {
		multi_lang_script->header_unknown2[oldlangbase][i] = header_unknown2[i];
		multi_lang_script->header_unknown3[oldlangbase][i] = header_unknown3[i];
	}
	multi_lang_script->entry_offset[oldlangbase] = entry_offset;
	multi_lang_script->entry_size[oldlangbase] = entry_size;
	multi_lang_script->entry_local_var[oldlangbase] = entry_local_var;
	multi_lang_script->entry_flag[oldlangbase] = entry_flag;
	multi_lang_script->entry_type[oldlangbase] = entry_type;
	multi_lang_script->entry_function_amount[oldlangbase] = entry_function_amount;
	multi_lang_script->function_type[oldlangbase] = function_type;
	multi_lang_script->function_point[oldlangbase] = function_point;
	multi_lang_script->global_data[oldlangbase] = global_data;
	multi_lang_script->local_data[oldlangbase] = local_data;
	multi_lang_script->is_loaded[oldlangbase] = loaded;
	multi_lang_script->is_modified[oldlangbase] = modified;
	func = multi_lang_script->func[newlangbase];
	magic_number = multi_lang_script->magic_number[newlangbase];
	header_unknown1 = multi_lang_script->header_unknown1[newlangbase];
	entry_amount = multi_lang_script->entry_amount[newlangbase];
	for (i=0;i<20;i++) {
		header_unknown2[i] = multi_lang_script->header_unknown2[newlangbase][i];
		header_unknown3[i] = multi_lang_script->header_unknown3[newlangbase][i];
	}
	entry_offset = multi_lang_script->entry_offset[newlangbase];
	entry_size = multi_lang_script->entry_size[newlangbase];
	entry_local_var = multi_lang_script->entry_local_var[newlangbase];
	entry_flag = multi_lang_script->entry_flag[newlangbase];
	entry_type = multi_lang_script->entry_type[newlangbase];
	entry_function_amount = multi_lang_script->entry_function_amount[newlangbase];
	function_type = multi_lang_script->function_type[newlangbase];
	function_point = multi_lang_script->function_point[newlangbase];
	global_data = multi_lang_script->global_data[newlangbase];
	local_data = multi_lang_script->local_data[newlangbase];
	loaded = multi_lang_script->is_loaded[newlangbase];
	modified = multi_lang_script->is_modified[newlangbase];
	current_language = newlang;
}

void ScriptDataStruct::UpdateSteamMultiLanguage() {
	if (multi_lang_script==NULL)
		return;
	unsigned int i;
	for (i=0;i<SCRIPT_NAME_MAX_LENGTH;i++)
		multi_lang_script->header_name[current_language][i] = header_name[i];
	SteamLanguage curlangbase = multi_lang_script->base_script_lang[current_language];
	multi_lang_script->func[curlangbase] = func;
	multi_lang_script->magic_number[curlangbase] = magic_number;
	multi_lang_script->header_unknown1[curlangbase] = header_unknown1;
	multi_lang_script->entry_amount[curlangbase] = entry_amount;
	for (i=0;i<20;i++) {
		multi_lang_script->header_unknown2[curlangbase][i] = header_unknown2[i];
		multi_lang_script->header_unknown3[curlangbase][i] = header_unknown3[i];
	}
	multi_lang_script->entry_offset[curlangbase] = entry_offset;
	multi_lang_script->entry_size[curlangbase] = entry_size;
	multi_lang_script->entry_local_var[curlangbase] = entry_local_var;
	multi_lang_script->entry_flag[curlangbase] = entry_flag;
	multi_lang_script->entry_type[curlangbase] = entry_type;
	multi_lang_script->entry_function_amount[curlangbase] = entry_function_amount;
	multi_lang_script->function_type[curlangbase] = function_type;
	multi_lang_script->function_point[curlangbase] = function_point;
	multi_lang_script->global_data[curlangbase] = global_data;
	multi_lang_script->local_data[curlangbase] = local_data;
	multi_lang_script->is_loaded[curlangbase] = loaded;
	multi_lang_script->is_modified[curlangbase] = modified;
}

bool ScriptDataStruct::CheckLanguageSimilarity(SteamLanguage lang, SteamLanguage baselang, vector<uint16_t>* langtextid, vector<uint16_t>* baselangtextid) {
	if (multi_lang_script==NULL || !multi_lang_script->is_loaded[lang] || !multi_lang_script->is_loaded[baselang])
		return false;
	baselang = multi_lang_script->base_script_lang[baselang];
	if (multi_lang_script->base_script_lang[lang]==baselang) {
		if (langtextid!=NULL)
			*langtextid = multi_lang_script->lang_script_text_id[lang];
		if (baselangtextid!=NULL)
			*baselangtextid = multi_lang_script->base_script_text_id[lang];
		return true;
	}
	if (multi_lang_script->base_script_lang[lang]!=lang)
		return false;
	if (multi_lang_script->entry_amount[lang]!=multi_lang_script->entry_amount[baselang])
		return false;
	unsigned int ei,fi,oi,ai,vai,tli;
	vector<uint16_t> linktextid;
	vector<uint16_t> linktextbaseid;
	for (ei=0;ei<multi_lang_script->entry_amount[lang];ei++) {
		if (multi_lang_script->entry_size[lang][ei]!=multi_lang_script->entry_size[baselang][ei])
			return false;
		if (multi_lang_script->entry_function_amount[lang][ei]!=multi_lang_script->entry_function_amount[baselang][ei])
			return false;
		for (fi=0;fi<multi_lang_script->entry_function_amount[lang][ei];fi++) {
			ScriptFunction& f = multi_lang_script->func[lang][ei][fi];
			ScriptFunction& bf = multi_lang_script->func[baselang][ei][fi];
			if (f.op_amount!=bf.op_amount)
				return false;
			for (oi=0;oi<f.op_amount;oi++) {
				ScriptOperation& op = f.op[oi];
				ScriptOperation& bop = bf.op[oi];
				if (op.opcode!=bop.opcode || op.arg_amount!=bop.arg_amount)
					return false;
				for (ai=0;ai<op.arg_amount;ai++) {
					ScriptArgument& a = op.arg[ai];
					ScriptArgument& ba = bop.arg[ai];
					if (a.is_var!=ba.is_var)
						return false;
					if (a.is_var) {
						if (a.size!=ba.size)
							return false;
						for (vai=0;vai<a.size;vai++)
							if (a.var[vai]!=ba.var[vai])
								return false;
					} else if ((int)ai>=HADES_STRING_SCRIPT_OPCODE[op.opcode].arg_amount || HADES_STRING_SCRIPT_OPCODE[op.opcode].arg_type[ai]!=AT_TEXT) {
						if (a.value!=ba.value)
							return false;
					} else {
						for (tli=0;tli<linktextid.size();tli++)
							if (linktextid[tli]==a.value) {
								if (linktextbaseid[tli]!=ba.value)
									return false;
								break;
							}
						if (tli>=linktextid.size()) {
							linktextid.push_back(a.value);
							linktextbaseid.push_back(ba.value);
						}
					}
				}
			}
		}
	}
	for (tli=0;tli<linktextid.size();)
		if (linktextbaseid[tli]==linktextid[tli]) {
			linktextbaseid.erase(linktextbaseid.begin()+tli);
			linktextid.erase(linktextid.begin()+tli);
		} else {
			tli++;
		}
	if (langtextid!=NULL)
		*langtextid = linktextid;
	if (baselangtextid!=NULL)
		*baselangtextid = linktextbaseid;
	return true;
}

void ScriptDataStruct::LinkLanguageScripts(SteamLanguage lang, SteamLanguage baselang, vector<uint16_t> langtextid, vector<uint16_t> baselangtextid) {
	if (multi_lang_script==NULL || !multi_lang_script->is_loaded[lang] || !multi_lang_script->is_loaded[baselang])
		return;
	unsigned int i,j;
	SteamLanguage oldbaselang = multi_lang_script->base_script_lang[lang];
	if (lang==baselang) {
		if (oldbaselang==baselang)
			return;
		multi_lang_script->base_script_lang[lang] = lang;
		multi_lang_script->func[lang] = multi_lang_script->func[oldbaselang];
		multi_lang_script->function_type[lang] = multi_lang_script->function_type[oldbaselang];
		multi_lang_script->function_point[lang] = multi_lang_script->function_point[oldbaselang];
		multi_lang_script->entry_offset[lang] = multi_lang_script->entry_offset[oldbaselang];
		multi_lang_script->entry_size[lang] = multi_lang_script->entry_size[oldbaselang];
		multi_lang_script->entry_flag[lang] = multi_lang_script->entry_flag[oldbaselang];
		multi_lang_script->entry_type[lang] = multi_lang_script->entry_type[oldbaselang];
		multi_lang_script->entry_function_amount[lang] = multi_lang_script->entry_function_amount[oldbaselang];
		multi_lang_script->local_data[lang] = multi_lang_script->local_data[oldbaselang];
		multi_lang_script->lang_script_text_id[lang].clear();
		multi_lang_script->base_script_text_id[lang].clear();
	} else {
		if (multi_lang_script->base_script_lang[lang]==lang) {
			multi_lang_script->func[lang].clear();
			multi_lang_script->function_type[lang].clear();
			multi_lang_script->function_point[lang].clear();
			multi_lang_script->entry_offset[lang].clear();
			multi_lang_script->entry_size[lang].clear();
			multi_lang_script->entry_flag[lang].clear();
			multi_lang_script->entry_type[lang].clear();
			multi_lang_script->entry_function_amount[lang].clear();
			multi_lang_script->local_data[lang].clear();
		}
		multi_lang_script->base_script_lang[lang] = multi_lang_script->base_script_lang[baselang];
		if (baselang==multi_lang_script->base_script_lang[baselang]) {
			multi_lang_script->lang_script_text_id[lang] = langtextid;
			multi_lang_script->base_script_text_id[lang] = baselangtextid;
		} else {
			multi_lang_script->lang_script_text_id[lang].clear();
			multi_lang_script->base_script_text_id[lang].clear();
			for (i=0;i<langtextid.size();i++)
				for (j=0;j<multi_lang_script->lang_script_text_id[baselang].size();j++)
					if (baselangtextid[j]==multi_lang_script->lang_script_text_id[baselang][j]) {
						multi_lang_script->lang_script_text_id[lang].push_back(langtextid[i]);
						multi_lang_script->base_script_text_id[lang].push_back(multi_lang_script->base_script_text_id[baselang][j]);
						break;
					}
		}
	}
	multi_lang_script->is_modified[lang] = true;
	if (oldbaselang==baselang)
		return;
	if (current_language==lang) {
		SteamLanguage newlangbase = multi_lang_script->base_script_lang[lang];
		unsigned int i;
		func = multi_lang_script->func[newlangbase];
		magic_number = multi_lang_script->magic_number[newlangbase];
		header_unknown1 = multi_lang_script->header_unknown1[newlangbase];
		entry_amount = multi_lang_script->entry_amount[newlangbase];
		for (i=0;i<20;i++) {
			header_unknown2[i] = multi_lang_script->header_unknown2[newlangbase][i];
			header_unknown3[i] = multi_lang_script->header_unknown3[newlangbase][i];
		}
		entry_offset = multi_lang_script->entry_offset[newlangbase];
		entry_size = multi_lang_script->entry_size[newlangbase];
		entry_local_var = multi_lang_script->entry_local_var[newlangbase];
		entry_flag = multi_lang_script->entry_flag[newlangbase];
		entry_type = multi_lang_script->entry_type[newlangbase];
		entry_function_amount = multi_lang_script->entry_function_amount[newlangbase];
		function_type = multi_lang_script->function_type[newlangbase];
		function_point = multi_lang_script->function_point[newlangbase];
		global_data = multi_lang_script->global_data[newlangbase];
		local_data = multi_lang_script->local_data[newlangbase];
		loaded = multi_lang_script->is_loaded[newlangbase];
		modified = multi_lang_script->is_modified[newlangbase];
	}
}

void ScriptDataStruct::LinkSimilarLanguageScripts() {
	if (multi_lang_script==NULL)
		return;
	SteamLanguage lang,baselang;
	vector<uint16_t> langtextid;
	vector<uint16_t> baselangtextid;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
		if (!multi_lang_script->is_loaded[lang])
			continue;
		for (baselang=0;baselang<lang;baselang++) {
			if (!multi_lang_script->is_loaded[baselang] || multi_lang_script->base_script_lang[baselang]!=baselang)
				continue;
			if (CheckLanguageSimilarity(lang,baselang,&langtextid,&baselangtextid)) {
				LinkLanguageScripts(lang,baselang,langtextid,baselangtextid);
				break;
			}
		}
	}
}

void ScriptDataStruct::ApplyDialogLink(vector<uint16_t> langtextid, vector<uint16_t> baselangtextid) {
	unsigned int i,j,k,l,m;
	for (i=0;i<entry_amount;i++)
		for (j=0;j<entry_function_amount[i];j++)
			for (k=0;k<func[i][j].op_amount;k++)
				for (l=0;l<func[i][j].op[k].arg_amount;l++) {
					ScriptArgument& arg = func[i][j].op[k].arg[l];
					if (arg.typesize==2 && !arg.is_signed && !arg.is_var && l<HADES_STRING_SCRIPT_OPCODE[arg.parent->opcode].arg_amount && HADES_STRING_SCRIPT_OPCODE[arg.parent->opcode].arg_type[l]==AT_TEXT)
						for (m=0;m<baselangtextid.size();m++)
							if (arg.value==baselangtextid[m]) {
								arg.value = langtextid[m];
								break;
							}
				}
}

#define MACRO_SCRIPT_IOFUNCTION(IO,SEEK,READ,PPF,FUNC) \
	unsigned int i,j; \
	uint32_t entry_pos, function_pos; \
	uint32_t zero32 = 0; \
	uint16_t zero16 = 0; \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,magic_number); \
	IO ## Char(f,header_unknown1); \
	IO ## Char(f,entry_amount); \
	if (READ) { \
		entry_offset.resize(entry_amount); \
		entry_size.resize(entry_amount); \
		entry_local_var.resize(entry_amount); \
		entry_flag.resize(entry_amount); \
		entry_type.resize(entry_amount); \
		entry_function_amount.resize(entry_amount); \
		function_type.resize(entry_amount); \
		function_point.resize(entry_amount); \
		func.resize(entry_amount); \
	} \
	for (i=0;i<20;i++) { \
		IO ## Char(f,header_unknown2[i]); \
		IO ## Char(f,header_unknown3[i]); \
	} \
	for (i=0;i<SCRIPT_NAME_MAX_LENGTH;i++) \
		IO ## Char(f,header_name[i]); \
	entry_pos = f.tellg(); \
	for (i=0;i<entry_amount;i++) { \
		IO ## Short(f,entry_offset[i]); \
		IO ## Short(f,entry_size[i]); \
		IO ## Char(f,entry_local_var[i]); \
		IO ## Char(f,entry_flag[i]); \
		IO ## Short(f,zero16); \
	} \
	if (PPF) PPFEndScanStep(); \
	for (i=0;i<entry_amount;i++) { \
		if (entry_size[i]>0) { \
			SEEK(f,entry_pos,entry_offset[i]); \
			if (PPF) PPFInitScanStep(f); \
			IO ## Char(f,entry_type[i]); \
			IO ## Char(f,entry_function_amount[i]); \
			if (READ) { \
				function_type[i].resize(entry_function_amount[i]); \
				function_point[i].resize(entry_function_amount[i]); \
				func[i].resize(entry_function_amount[i]); \
			} \
			function_pos = f.tellg(); \
			for (j=0;j<entry_function_amount[i];j++) { \
				IO ## Short(f,function_type[i][j]); \
				IO ## Short(f,function_point[i][j]); \
			} \
			if (PPF) PPFEndScanStep(); \
			if (READ) { \
				for (j=0;j+1<entry_function_amount[i];j++) \
					func[i][j].length = function_point[i][j+1]-function_point[i][j]; \
				func[i][j].length = entry_size[i]-function_point[i][j]-2; \
			} \
			for (j=0;j<entry_function_amount[i];j++) { \
				if (READ) func[i][j].parent = this; \
				SEEK(f,function_pos,function_point[i][j]); \
				func[i][j].FUNC(f); \
			} \
		} else if (READ) { \
			entry_type[i] = 0xFF; \
			entry_function_amount[i] = 0; \
			function_type[i].clear(); \
			function_point[i].clear(); \
			func[i].clear(); \
		} \
	}


void ScriptDataStruct::Read(fstream& f, SteamLanguage lang) {
	if (loaded && lang==STEAM_LANGUAGE_NONE)
		return;
	if (lang!=STEAM_LANGUAGE_NONE && multi_lang_script==NULL) {
		multi_lang_script = new MultiLanguageScriptDataStruct();
		for (SteamLanguage lg=0;lg<STEAM_LANGUAGE_AMOUNT;lg++) {
			multi_lang_script->is_loaded[lg] = false;
			multi_lang_script->is_modified[lg] = false;
			multi_lang_script->base_script_lang[lg] = lg;
		}
	}
	if (GetGameType()==GAME_TYPE_PSX && parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type==CLUSTER_TYPE_WORLD_MAP && object_id>=9100) {
		ChunkChild::Read(f);
	} else {
		if (GetGameType()==GAME_TYPE_PSX) {
			MACRO_SCRIPT_IOFUNCTION(FFIXRead,FFIXSeek,true,false,Read)
			name.ReadFromChar(header_name);
		} else {
			MACRO_SCRIPT_IOFUNCTION(SteamRead,SteamSeek,true,false,Read)
			if (!name.created)
				name.CreateEmpty();
		}
		local_data.resize(entry_amount);
		for (unsigned int i=0;i<entry_amount;i++) {
			local_data[i].allocate_amount = entry_local_var[i];
			local_data[i].amount = 0;
		}
		global_data.allocate_amount = 0;
		global_data.amount = 0;
		current_language = lang;
		loaded = true;
		if (lang!=STEAM_LANGUAGE_NONE)
			ChangeSteamLanguage(lang); // Setup the values and pointers of multi_lang_script's language
	}
	loaded = true;
}

void ScriptDataStruct::Write(fstream& f) {
	if (parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type==CLUSTER_TYPE_WORLD_MAP && object_id>=9100)
		ChunkChild::Write(f);
	else {
		MACRO_SCRIPT_IOFUNCTION(FFIXWrite,FFIXSeek,false,false,Write)
	}
	modified = false;
}

void ScriptDataStruct::WritePPF(fstream& f) {
	if (parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type==CLUSTER_TYPE_WORLD_MAP && object_id>=9100)
		ChunkChild::WritePPF(f);
	else {
		MACRO_SCRIPT_IOFUNCTION(PPFStepAdd,FFIXSeek,false,true,WritePPF)
	}
}

void ScriptDataStruct::ReadHWS(fstream& f, bool usetext, SteamLanguage lang) {
	SteamLanguage oldlang = current_language;
	LinkLanguageScripts(lang,lang);
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(lang);
	uint8_t oldentryamount = entry_amount;
	MACRO_SCRIPT_IOFUNCTION(HWSRead,HWSSeek,true,false,ReadHWS)
	if (usetext)
		name.ReadFromChar(header_name);
	if (oldentryamount!=entry_amount) {
		unsigned int i;
		local_data.resize(entry_amount);
		for (i=0;i<entry_amount;i++)
			local_data[i].allocate_amount = entry_local_var[i];
		for (i=oldentryamount;i<entry_amount;i++)
			local_data[i].amount = 0;
	}
	MarkDataModified();
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(oldlang);
}

void ScriptDataStruct::WriteHWS(fstream& f, SteamLanguage lang) {
	SteamLanguage oldlang = current_language;
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(lang);
	MACRO_SCRIPT_IOFUNCTION(HWSWrite,HWSSeek,false,false,WriteHWS)
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(oldlang);
}

void ScriptDataStruct::ReadLocalHWS(fstream& f, SteamLanguage lang) {
	SteamLanguage oldlang = current_language;
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(lang);
	unsigned int i,j;
	uint32_t localpos, entrypos, vardatapos, localsize;
	uint16_t entryheadersize, vardatasize, tmp16;
	uint8_t entryam, vardatatype, localam;
	localpos = f.tellg();
	HWSReadLong(f,localsize);
	HWSReadChar(f,entryam);
	for (i=0;i<entryam;i++) {
		entrypos = f.tellg();
		HWSReadShort(f,entryheadersize);
		if (i<entry_amount) {
			HWSReadChar(f,local_data[i].allocate_amount);
			HWSReadChar(f,(uint8_t&)local_data[i].amount);
			entry_local_var[i] = local_data[i].allocate_amount;
		} else {
			HWSReadChar(f,localam);
			HWSReadChar(f,localam);
		}
		HWSSeek(f,entrypos,entryheadersize);
		if (i<entry_amount) {
			local_data[i].local_type.resize(local_data[i].amount);
			local_data[i].type.resize(local_data[i].amount);
			local_data[i].size.resize(local_data[i].amount);
			local_data[i].name.resize(local_data[i].amount);
			local_data[i].cat.resize(local_data[i].amount);
			local_data[i].id.resize(local_data[i].amount);
			for (j=0;j<local_data[i].amount;j++) {
				HWSReadChar(f,local_data[i].local_type[j]);
				HWSReadShort(f,(uint16_t&)local_data[i].type[j]);
				HWSReadChar(f,local_data[i].size[j]);
				HWSReadChar(f,local_data[i].cat[j]);
				HWSReadShort(f,local_data[i].id[j]);
				local_data[i].name[j].clear();
				HWSReadShort(f,tmp16);
				while (tmp16!=0) {
					local_data[i].name[j].append(1,tmp16);
					HWSReadShort(f,tmp16);
				}
				HWSReadChar(f,vardatatype);
				while (vardatatype!=0) {
					vardatapos = f.tellg();
					HWSReadShort(f,vardatasize);
					// Additional variable informations
					// may appear here in a future version
					HWSSeek(f,vardatapos,vardatasize);
					HWSReadChar(f,vardatatype);
				}
			}
		} else {
			for (j=0;j<localam;j++) {
				HWSSeek(f,f.tellg(),7);
				HWSReadShort(f,tmp16);
				while (tmp16!=0)
					HWSReadShort(f,tmp16);
				HWSReadChar(f,vardatatype);
				while (vardatatype!=0) {
					vardatapos = f.tellg();
					HWSReadShort(f,vardatasize);
					HWSSeek(f,vardatapos,vardatasize);
					HWSReadChar(f,vardatatype);
				}
			}
		}
	}
	if (localsize+localpos>f.tellg()) {
		entrypos = f.tellg();
		HWSReadShort(f,entryheadersize);
		HWSReadChar(f,global_data.allocate_amount);
		HWSReadChar(f,(uint8_t&)global_data.amount);
		HWSSeek(f,entrypos,entryheadersize);
		global_data.local_type.resize(global_data.amount);
		global_data.type.resize(global_data.amount);
		global_data.size.resize(global_data.amount);
		global_data.name.resize(global_data.amount);
		global_data.cat.resize(global_data.amount);
		global_data.id.resize(global_data.amount);
		for (j=0;j<global_data.amount;j++) {
			HWSReadChar(f,global_data.local_type[j]);
			HWSReadShort(f,(uint16_t&)global_data.type[j]);
			HWSReadChar(f,global_data.size[j]);
			HWSReadChar(f,global_data.cat[j]);
			HWSReadShort(f,global_data.id[j]);
			global_data.name[j].clear();
			HWSReadShort(f,tmp16);
			while (tmp16!=0) {
				global_data.name[j].append(1,tmp16);
				HWSReadShort(f,tmp16);
			}
			HWSReadChar(f,vardatatype);
			while (vardatatype!=0) {
				vardatapos = f.tellg();
				HWSReadShort(f,vardatasize);
				// Additional variable informations
				// may appear here in a future version
				HWSSeek(f,vardatapos,vardatasize);
				HWSReadChar(f,vardatatype);
			}
		}
	}
	HWSSeek(f,localpos,localsize);
	MarkDataModified();
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(oldlang);
}

void ScriptDataStruct::WriteLocalHWS(fstream& f, SteamLanguage lang) {
	SteamLanguage oldlang = current_language;
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(lang);
	unsigned int i,j,k;
	uint32_t localpos, localsize = 0;
	uint16_t zero16 = 0;
	uint8_t zero8 = 0;
	localpos = f.tellg();
	HWSWriteLong(f,localsize);
	HWSWriteChar(f,entry_amount);
	for (i=0;i<entry_amount;i++) {
		HWSWriteShort(f,0x4);
		HWSWriteChar(f,local_data[i].allocate_amount);
		HWSWriteChar(f,(uint8_t&)local_data[i].amount);
		for (j=0;j<local_data[i].amount;j++) {
			HWSWriteChar(f,local_data[i].local_type[j]);
			HWSWriteShort(f,local_data[i].type[j]);
			HWSWriteChar(f,local_data[i].size[j]);
			HWSWriteChar(f,local_data[i].cat[j]);
			HWSWriteShort(f,local_data[i].id[j]);
			for (k=0;k<local_data[i].name[j].length();k++)
				HWSWriteShort(f,local_data[i].name[j][k]);
			HWSWriteShort(f,zero16);
			HWSWriteChar(f,zero8);
		}
	}
	if (global_data.amount>0) {
		HWSWriteShort(f,0x4);
		HWSWriteChar(f,global_data.allocate_amount);
		HWSWriteChar(f,(uint8_t&)global_data.amount);
		for (j=0;j<global_data.amount;j++) {
			HWSWriteChar(f,global_data.local_type[j]);
			HWSWriteShort(f,global_data.type[j]);
			HWSWriteChar(f,global_data.size[j]);
			HWSWriteChar(f,global_data.cat[j]);
			HWSWriteShort(f,global_data.id[j]);
			for (k=0;k<global_data.name[j].length();k++)
				HWSWriteShort(f,global_data.name[j][k]);
			HWSWriteShort(f,zero16);
			HWSWriteChar(f,zero8);
		}
	}
	localsize = (long long)f.tellg()-localpos;
	f.seekg(localpos);
	HWSWriteLong(f,localsize);
	HWSSeek(f,localpos,localsize);
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(oldlang);
}

ScriptDataStruct& ScriptDataStruct::operator=(const ScriptDataStruct& from) {
	unsigned int i,j;
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
	function_type = from.function_type;
	function_point = from.function_point;
	global_data = from.global_data;
	local_data = from.local_data;
	multi_lang_script = from.multi_lang_script;
	current_language = from.current_language;
	related_charmap_id = from.related_charmap_id;
	for (i=0;i<20;i++) {
		header_unknown2[i] = from.header_unknown2[i];
		header_unknown3[i] = from.header_unknown3[i];
	}
	for (i=0;i<SCRIPT_NAME_MAX_LENGTH;i++)
		header_name[i] = from.header_name[i];
	for (i=0;i<entry_amount;i++)
		for (j=0;j<entry_function_amount[i];j++)
			func[i][j].parent = this;
	return *this;
}

bool ScriptDataStruct::IsDataModified(SteamLanguage lang) {
	if (multi_lang_script==NULL || lang==current_language)
		return modified;
	return multi_lang_script->is_modified[lang];
}

int ScriptDataStruct::GetDataSize(SteamLanguage lang) {
	SteamLanguage oldlang = current_language;
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(lang);
	uint16_t funcpos, entrypos = 8*entry_amount, lastentrypos = entrypos;
	unsigned int i,j;
	int res = 128;
	for (i=0;i<entry_amount;i++) {
		if (entry_function_amount[i]>0) {
			entry_offset[i] = entrypos;
			funcpos = 4*entry_function_amount[i];
			for (j=0;j<entry_function_amount[i];j++) {
				function_point[i][j] = funcpos;
				funcpos += func[i][j].length;
			}
			entry_size[i] = funcpos+2;
			if (entry_size[i]%4)
				entry_size[i] += 4-entry_size[i]%4;
			lastentrypos = entrypos;
			entrypos += entry_size[i];
		} else {
			entry_offset[i] = entrypos;
			entry_size[i] = 0;
		}
	}
	if (entry_amount>0) // Just for mimicking the game's behavior
		for (i=entry_amount-1;entry_size[i]==0;i--)
			entry_offset[i] = lastentrypos;
	res += entrypos;
	if (lang!=STEAM_LANGUAGE_NONE && oldlang!=STEAM_LANGUAGE_NONE)
		ChangeSteamLanguage(oldlang);
	return res;
}

void ScriptDataStruct::UpdateOffset() {
	// Assume the size/length of ScriptFunction
	// and structures under it are properly set
	unsigned int i,j;
	if (GetGameType()==GAME_TYPE_PSX) // No modif for Steam as this name is unused
		for (i=0;i<name.length && i<SCRIPT_NAME_MAX_LENGTH;i++)
			header_name[i] = name.raw[i];
	uint16_t funcpos, entrypos = 8*entry_amount, lastentrypos = entrypos;
	uint32_t totalsize = 128;
	for (i=0;i<entry_amount;i++) {
		if (entry_function_amount[i]>0) {
			entry_offset[i] = entrypos;
			funcpos = 4*entry_function_amount[i];
			for (j=0;j<entry_function_amount[i];j++) {
				function_point[i][j] = funcpos;
				funcpos += func[i][j].length;
			}
			entry_size[i] = funcpos+2;
			if (entry_size[i]%4)
				entry_size[i] += 4-entry_size[i]%4;
			lastentrypos = entrypos;
			entrypos += entry_size[i];
		} else {
			entry_offset[i] = entrypos;
			entry_size[i] = 0;
		}
	}
	if (entry_amount>0) // Just for mimicking the game's behavior
		for (i=entry_amount-1;entry_size[i]==0;i--)
			entry_offset[i] = lastentrypos;
	totalsize += entrypos;
	SetSize(totalsize);
}
