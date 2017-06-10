#include "Scripts.h"

#include <sstream>
#include "Hades_Strings.h"
#include "Database_Script.h"

uint8_t opcodetmp[0x800];
ScriptOperation scriptoptmp[0x8000];

bool ScriptArgument::SetValue(long long newvalue) {
//	if (is_var)
//		delete[] var;
	if (newvalue<0) {
		uint32_t sign = 0x1 << (typesize*8-1);
		value = ~(-newvalue-1 & ~sign) | sign;
	} else
		value = newvalue;
	size = typesize;
	is_var = false;
	return true;
}

void ScriptArgument::SetValueVar(uint8_t* newvaluevar, uint8_t newsize) {
//	if (is_var)
//		delete[] var;
	var = newvaluevar;
	size = newsize;
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
			uint16_t* num = (uint16_t*)&value;
			return *num;
		}
	}
	return value;
}

#define MACRO_SCRIPT_IOFUNCTION_ARGREAD(IO,SEEK) \
	unsigned int i; \
	typesize = sz; \
	if (is_var) { \
		uint8_t varbyte; \
		int vartype; \
		i = 0; \
		size = 0; \
		IO ## Char(f,opcodetmp[i]); size++; \
		varbyte = opcodetmp[i++]; \
		vartype = VarOpList[varbyte].type; \
		while (vartype!=-1) { \
			if (vartype==2) { \
			} else if (vartype==3) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} else if (vartype==5) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} else if (vartype==6) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} else if (vartype==7) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
				IO ## Char(f,opcodetmp[i++]); size++; \
				IO ## Char(f,opcodetmp[i++]); size++; \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} else if (vartype>=10 && vartype<20) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} else if (vartype>=20 && vartype<30) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} else if (vartype==55) { \
				IO ## Char(f,opcodetmp[i++]); size++; \
				IO ## Char(f,opcodetmp[i++]); size++; \
			} \
			IO ## Char(f,opcodetmp[i]); size++; \
			varbyte = opcodetmp[i++]; \
			vartype = VarOpList[varbyte].type; \
		} \
		var = new uint8_t[size]; \
		memcpy(var,opcodetmp,size); \
	} else { \
		size = typesize; \
		value = 0; \
		for (i=0;i<size;i++) { \
			IO ## Char(f,opcodetmp[i]); \
			value |= (opcodetmp[i] << i*8); \
		} \
	}

#define MACRO_SCRIPT_IOFUNCTION_ARGWRITE(IO,SEEK,PPF) \
	unsigned int i; \
	uint8_t buffer; \
	if (PPF) PPFInitScanStep(f); \
	if (is_var) \
		for (i=0;i<size;i++) \
			IO ## Char(f,var[i]); \
	else \
		for (i=0;i<size;i++) { \
			buffer = (value >> i*8) & 0xFF; \
			IO ## Char(f,buffer); \
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

ScriptArgument* NewScriptArgumentArray(unsigned int amount, ScriptOperation* p) {
	ScriptArgument* res = new ScriptArgument[amount];
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

void ScriptFunction::Read(fstream& f) {
	unsigned int len = 0;
	op_amount = 0;
	while (len<length) {
		scriptoptmp[op_amount++].Read(f);
		len += scriptoptmp[op_amount-1].size;
	}
	op = new ScriptOperation[op_amount];
	memcpy(op,scriptoptmp,op_amount*sizeof(ScriptOperation));
	for (unsigned int i=0;i<op_amount;i++)
		op[i].parent = this;
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
	while (len<length) {
		scriptoptmp[op_amount++].ReadHWS(f);
		len += scriptoptmp[op_amount-1].size;
	}
	op = new ScriptOperation[op_amount];
	memcpy(op,scriptoptmp,op_amount*sizeof(ScriptOperation));
	for (unsigned int i=0;i<op_amount;i++)
		op[i].parent = this;
}

void ScriptFunction::WriteHWS(fstream& f) {
	for (unsigned int i=0;i<op_amount;i++)
		op[i].WriteHWS(f);
}

int ScriptDataStruct::SetName(wstring newvalue) {
	FF9String tmp(name);
	tmp.SetValue(newvalue);
	if (tmp.length>SCRIPT_NAME_MAX_LENGTH)
		return 1;
	name.SetValue(newvalue);
	return 0;
}

int ScriptDataStruct::SetName(FF9String& newvalue) {
	if (newvalue.length>SCRIPT_NAME_MAX_LENGTH)
		return 1;
	name = newvalue;
	return 0;
}

void ScriptDataStruct::AddFunction(int entryid, int funcidpos, uint16_t functype) {
	entry_function_amount[entryid]++;
	ScriptFunction* newfunc = new ScriptFunction[entry_function_amount[entryid]];
	uint16_t* newfunctype = new uint16_t[entry_function_amount[entryid]];
	uint16_t* newfuncpoint = new uint16_t[entry_function_amount[entryid]];
	memcpy(newfunc,func[entryid],funcidpos*sizeof(ScriptFunction));
	memcpy(newfunctype,function_type[entryid],funcidpos*sizeof(uint16_t));
	memcpy(newfuncpoint,function_point[entryid],funcidpos*sizeof(uint16_t));
	newfunc[funcidpos].parent = this;
	newfunc[funcidpos].op_amount = 0;
	newfunc[funcidpos].op = new ScriptOperation[0];
	newfunc[funcidpos].length = 0;
	newfunctype[funcidpos] = functype;
	memcpy(newfunc+funcidpos+1,func[entryid]+funcidpos,(entry_function_amount[entryid]-funcidpos-1)*sizeof(ScriptFunction));
	memcpy(newfunctype+funcidpos+1,function_type[entryid]+funcidpos,(entry_function_amount[entryid]-funcidpos-1)*sizeof(uint16_t));
	memcpy(newfuncpoint+funcidpos+1,function_point[entryid]+funcidpos,(entry_function_amount[entryid]-funcidpos-1)*sizeof(uint16_t));
	delete[] func[entryid];
	delete[] function_type[entryid];
	delete[] function_point[entryid];
	func[entryid] = newfunc;
	function_type[entryid] = newfunctype;
	function_point[entryid] = newfuncpoint;
}

int ScriptDataStruct::RemoveFunction(int entryid, int funcid) {
	int res = max((unsigned int)4,func[entryid][funcid].length); // Don't count the extra 4 bytes because of padding
	entry_function_amount[entryid]--;
	ScriptFunction* newfunc = new ScriptFunction[entry_function_amount[entryid]];
	uint16_t* newfunctype = new uint16_t[entry_function_amount[entryid]];
	uint16_t* newfuncpoint = new uint16_t[entry_function_amount[entryid]];
	memcpy(newfunc,func[entryid],funcid*sizeof(ScriptFunction));
	memcpy(newfunctype,function_type[entryid],funcid*sizeof(uint16_t));
	memcpy(newfuncpoint,function_point[entryid],funcid*sizeof(uint16_t));
	memcpy(newfunc+funcid,func[entryid]+funcid+1,(entry_function_amount[entryid]-funcid)*sizeof(ScriptFunction));
	memcpy(newfunctype+funcid,function_type[entryid]+funcid+1,(entry_function_amount[entryid]-funcid)*sizeof(uint16_t));
	memcpy(newfuncpoint+funcid,function_point[entryid]+funcid+1,(entry_function_amount[entryid]-funcid)*sizeof(uint16_t));
	delete[] func[entryid];
	delete[] function_type[entryid];
	delete[] function_point[entryid];
	func[entryid] = newfunc;
	function_type[entryid] = newfunctype;
	function_point[entryid] = newfuncpoint;
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
	ScriptFunction** newfunc = new ScriptFunction*[entry_amount];
	uint16_t** newfunctype = new uint16_t*[entry_amount];
	uint16_t** newfuncpoint = new uint16_t*[entry_amount];
	uint16_t* newoffset = new uint16_t[entry_amount];
	uint16_t* newsize = new uint16_t[entry_amount];
	uint8_t* newlocalvar = new uint8_t[entry_amount];
	uint8_t* newflag = new uint8_t[entry_amount];
	uint8_t* newtype = new uint8_t[entry_amount];
	uint8_t* newfuncamount = new uint8_t[entry_amount];
	ScriptLocalVariableSet* newlocal = new ScriptLocalVariableSet[entry_amount];
	memcpy(newfunc,func,entrypos*sizeof(ScriptFunction*));
	memcpy(newfunctype,function_type,entrypos*sizeof(uint16_t*));
	memcpy(newfuncpoint,function_point,entrypos*sizeof(uint16_t*));
	memcpy(newoffset,entry_offset,entrypos*sizeof(uint16_t));
	memcpy(newsize,entry_size,entrypos*sizeof(uint16_t));
	memcpy(newlocalvar,entry_local_var,entrypos*sizeof(uint8_t));
	memcpy(newflag,entry_flag,entrypos*sizeof(uint8_t));
	memcpy(newtype,entry_type,entrypos*sizeof(uint8_t));
	memcpy(newfuncamount,entry_function_amount,entrypos*sizeof(uint8_t));
	memcpy(newlocal,local_data,entrypos*sizeof(ScriptLocalVariableSet));
	newfunc[entrypos] = new ScriptFunction[0];
	newfunctype[entrypos] = new uint16_t[0];
	newfuncpoint[entrypos] = new uint16_t[0];
	newoffset[entrypos] = 0;
	newsize[entrypos] = 0;
	newlocalvar[entrypos] = 0;
	newflag[entrypos] = 0;
	newtype[entrypos] = entrytype;
	newfuncamount[entrypos] = 0;
	newlocal[entrypos].allocate_amount = 0;
	newlocal[entrypos].amount = 0;
	memcpy(newfunc+entrypos+1,func+entrypos,(entry_amount-entrypos-1)*sizeof(ScriptFunction*));
	memcpy(newfunctype+entrypos+1,function_type+entrypos,(entry_amount-entrypos-1)*sizeof(uint16_t*));
	memcpy(newfuncpoint+entrypos+1,function_point+entrypos,(entry_amount-entrypos-1)*sizeof(uint16_t*));
	memcpy(newoffset+entrypos+1,entry_offset+entrypos,(entry_amount-entrypos-1)*sizeof(uint16_t));
	memcpy(newsize+entrypos+1,entry_size+entrypos,(entry_amount-entrypos-1)*sizeof(uint16_t));
	memcpy(newlocalvar+entrypos+1,entry_local_var+entrypos,(entry_amount-entrypos-1)*sizeof(uint8_t));
	memcpy(newflag+entrypos+1,entry_flag+entrypos,(entry_amount-entrypos-1)*sizeof(uint8_t));
	memcpy(newtype+entrypos+1,entry_type+entrypos,(entry_amount-entrypos-1)*sizeof(uint8_t));
	memcpy(newfuncamount+entrypos+1,entry_function_amount+entrypos,(entry_amount-entrypos-1)*sizeof(uint8_t));
	memcpy(newlocal+entrypos+1,local_data+entrypos,(entry_amount-entrypos-1)*sizeof(ScriptLocalVariableSet));
	delete[] func;
	delete[] function_type;
	delete[] function_point;
	delete[] entry_offset;
	delete[] entry_size;
	delete[] entry_local_var;
	delete[] entry_flag;
	delete[] entry_type;
	delete[] entry_function_amount;
	delete[] local_data;
	func = newfunc;
	function_type = newfunctype;
	function_point = newfuncpoint;
	entry_offset = newoffset;
	entry_size = newsize;
	entry_local_var = newlocalvar;
	entry_flag = newflag;
	entry_type = newtype;
	entry_function_amount = newfuncamount;
	local_data = newlocal;
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
	delete[] func[entrypos];
	delete[] function_type[entrypos];
	delete[] function_point[entrypos];
	entry_amount--;
	ScriptFunction** newfunc = new ScriptFunction*[entry_amount];
	uint16_t** newfunctype = new uint16_t*[entry_amount];
	uint16_t** newfuncpoint = new uint16_t*[entry_amount];
	uint16_t* newoffset = new uint16_t[entry_amount];
	uint16_t* newsize = new uint16_t[entry_amount];
	uint8_t* newlocalvar = new uint8_t[entry_amount];
	uint8_t* newflag = new uint8_t[entry_amount];
	uint8_t* newtype = new uint8_t[entry_amount];
	uint8_t* newfuncamount = new uint8_t[entry_amount];
	ScriptLocalVariableSet* newlocal = new ScriptLocalVariableSet[entry_amount];
	memcpy(newfunc,func,entrypos*sizeof(ScriptFunction*));
	memcpy(newfunctype,function_type,entrypos*sizeof(uint16_t*));
	memcpy(newfuncpoint,function_point,entrypos*sizeof(uint16_t*));
	memcpy(newoffset,entry_offset,entrypos*sizeof(uint16_t));
	memcpy(newsize,entry_size,entrypos*sizeof(uint16_t));
	memcpy(newlocalvar,entry_local_var,entrypos*sizeof(uint8_t));
	memcpy(newflag,entry_flag,entrypos*sizeof(uint8_t));
	memcpy(newtype,entry_type,entrypos*sizeof(uint8_t));
	memcpy(newfuncamount,entry_function_amount,entrypos*sizeof(uint8_t));
	memcpy(newlocal,local_data,entrypos*sizeof(ScriptLocalVariableSet));
	memcpy(newfunc+entrypos,func+entrypos+1,(entry_amount-entrypos)*sizeof(ScriptFunction*));
	memcpy(newfunctype+entrypos,function_type+entrypos+1,(entry_amount-entrypos)*sizeof(uint16_t*));
	memcpy(newfuncpoint+entrypos,function_point+entrypos+1,(entry_amount-entrypos)*sizeof(uint16_t*));
	memcpy(newoffset+entrypos,entry_offset+entrypos+1,(entry_amount-entrypos)*sizeof(uint16_t));
	memcpy(newsize+entrypos,entry_size+entrypos+1,(entry_amount-entrypos)*sizeof(uint16_t));
	memcpy(newlocalvar+entrypos,entry_local_var+entrypos+1,(entry_amount-entrypos)*sizeof(uint8_t));
	memcpy(newflag+entrypos,entry_flag+entrypos+1,(entry_amount-entrypos)*sizeof(uint8_t));
	memcpy(newtype+entrypos,entry_type+entrypos+1,(entry_amount-entrypos)*sizeof(uint8_t));
	memcpy(newfuncamount+entrypos,entry_function_amount+entrypos+1,(entry_amount-entrypos)*sizeof(uint8_t));
	memcpy(newlocal+entrypos,local_data+entrypos+1,(entry_amount-entrypos)*sizeof(ScriptLocalVariableSet));
	delete[] func;
	delete[] function_type;
	delete[] function_point;
	delete[] entry_offset;
	delete[] entry_size;
	delete[] entry_local_var;
	delete[] entry_flag;
	delete[] entry_type;
	delete[] entry_function_amount;
	delete[] local_data;
	func = newfunc;
	function_type = newfunctype;
	function_point = newfuncpoint;
	entry_offset = newoffset;
	entry_size = newsize;
	entry_local_var = newlocalvar;
	entry_flag = newflag;
	entry_type = newtype;
	entry_function_amount = newfuncamount;
	local_data = newlocal;
	return res;
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
		entry_offset = new uint16_t[entry_amount]; \
		entry_size = new uint16_t[entry_amount]; \
		entry_local_var = new uint8_t[entry_amount]; \
		entry_flag = new uint8_t[entry_amount]; \
		entry_type = new uint8_t[entry_amount]; \
		entry_function_amount = new uint8_t[entry_amount]; \
		function_type = new uint16_t*[entry_amount]; \
		function_point = new uint16_t*[entry_amount]; \
		func = new ScriptFunction*[entry_amount]; \
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
				function_type[i] = new uint16_t[entry_function_amount[i]]; \
				function_point[i] = new uint16_t[entry_function_amount[i]]; \
				func[i] = new ScriptFunction[entry_function_amount[i]]; \
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
			function_type[i] = new uint16_t[0]; \
			function_point[i] = new uint16_t[0]; \
			func[i] = new ScriptFunction[0]; \
		} \
	}


void ScriptDataStruct::Read(fstream& f) {
	if (loaded)
		return;
	if (GetGameType()==GAME_TYPE_PSX && parent_cluster->parent_cluster && parent_cluster->parent_cluster->clus_type==CLUSTER_TYPE_WORLD_MAP && object_id>=9100) {
		ChunkChild::Read(f);
	} else {
		if (GetGameType()==GAME_TYPE_PSX) {
			MACRO_SCRIPT_IOFUNCTION(FFIXRead,FFIXSeek,true,false,Read)
		} else {
			MACRO_SCRIPT_IOFUNCTION(SteamRead,SteamSeek,true,false,Read)
		}
		name.ReadFromChar(header_name);
		local_data = new ScriptLocalVariableSet[entry_amount];
		for (unsigned int i=0;i<entry_amount;i++) {
			local_data[i].allocate_amount = entry_local_var[i];
			local_data[i].amount = 0;
		}
		global_data.allocate_amount = 0;
		global_data.amount = 0;
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

void ScriptDataStruct::ReadHWS(fstream& f, bool usetext) {
	uint8_t oldentryamount = entry_amount;
	MACRO_SCRIPT_IOFUNCTION(HWSRead,HWSSeek,true,false,ReadHWS)
	if (usetext)
		name.ReadFromChar(header_name);
	if (oldentryamount!=entry_amount) {
		ScriptLocalVariableSet* newlocaldata = new ScriptLocalVariableSet[entry_amount];
		unsigned int i;
		memcpy(newlocaldata,local_data,min(oldentryamount,entry_amount)*sizeof(ScriptLocalVariableSet));
		for (i=0;i<entry_amount;i++)
			newlocaldata[i].allocate_amount = entry_local_var[i];
		for (i=oldentryamount;i<entry_amount;i++)
			newlocaldata[i].amount = 0;
		delete[] local_data;
		local_data = newlocaldata;
	}
	MarkDataModified();
}

void ScriptDataStruct::WriteHWS(fstream& f) {
	MACRO_SCRIPT_IOFUNCTION(HWSWrite,HWSSeek,false,false,WriteHWS)
}

void ScriptDataStruct::ReadLocalHWS(fstream& f) {
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
			local_data[i].local_type = new uint8_t[local_data[i].amount];
			local_data[i].type = new int16_t[local_data[i].amount];
			local_data[i].size = new uint8_t[local_data[i].amount];
			local_data[i].name = new wstring[local_data[i].amount];
			local_data[i].cat = new uint8_t[local_data[i].amount];
			local_data[i].id = new uint16_t[local_data[i].amount];
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
		global_data.local_type = new uint8_t[global_data.amount];
		global_data.type = new int16_t[global_data.amount];
		global_data.size = new uint8_t[global_data.amount];
		global_data.name = new wstring[global_data.amount];
		global_data.cat = new uint8_t[global_data.amount];
		global_data.id = new uint16_t[global_data.amount];
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
}

void ScriptDataStruct::WriteLocalHWS(fstream& f) {
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
}

void ScriptDataStruct::Copy(ScriptDataStruct& from, bool deleteold) {
	unsigned int i,j,k,l;
	if (deleteold) {
		for (i=0;i<entry_amount;i++) {
			for (j=0;j<entry_function_amount[i];j++) {
				for (k=0;k<func[i][j].op_amount;k++) {
					for (l=0;l<func[i][j].op[k].arg_amount;l++) {
						if (func[i][j].op[k].arg[l].is_var)
							delete[] func[i][j].op[k].arg[l].var;
					}
					delete[] func[i][j].op[k].arg;
				}
				delete[] func[i][j].op;
			}
			delete[] func[i];
			delete[] function_type[i];
			delete[] function_point[i];
		}
		delete[] entry_offset;
		delete[] entry_size;
		delete[] entry_flag;
		delete[] entry_type;
		delete[] entry_function_amount;
		delete[] local_data;
	}
	*this = from;
	entry_amount = from.entry_amount;
	entry_offset = new uint16_t[entry_amount];
	entry_size = new uint16_t[entry_amount];
	entry_local_var = new uint8_t[entry_amount];
	entry_flag = new uint8_t[entry_amount];
	entry_type = new uint8_t[entry_amount];
	entry_function_amount = new uint8_t[entry_amount];
	function_type = new uint16_t*[entry_amount];
	function_point = new uint16_t*[entry_amount];
	func = new ScriptFunction*[entry_amount];
	local_data = new ScriptLocalVariableSet[entry_amount];
	memcpy(entry_offset,from.entry_offset,entry_amount*sizeof(uint16_t));
	memcpy(entry_size,from.entry_size,entry_amount*sizeof(uint16_t));
	memcpy(entry_local_var,from.entry_local_var,entry_amount*sizeof(uint8_t));
	memcpy(entry_flag,from.entry_flag,entry_amount*sizeof(uint8_t));
	memcpy(entry_type,from.entry_type,entry_amount*sizeof(uint8_t));
	memcpy(entry_function_amount,from.entry_function_amount,entry_amount*sizeof(uint8_t));
	memcpy(local_data,from.local_data,entry_amount*sizeof(ScriptLocalVariableSet));
	for (i=0;i<entry_amount;i++) {
		function_type[i] = new uint16_t[entry_function_amount[i]];
		function_point[i] = new uint16_t[entry_function_amount[i]];
		func[i] = new ScriptFunction[entry_function_amount[i]];
		memcpy(function_type[i],from.function_type[i],entry_function_amount[i]*sizeof(uint16_t));
		memcpy(function_point[i],from.function_point[i],entry_function_amount[i]*sizeof(uint16_t));
		for (j=0;j<entry_function_amount[i];j++) {
			func[i][j].parent = this;
			func[i][j].op_amount = from.func[i][j].op_amount;
			func[i][j].length = from.func[i][j].length;
			func[i][j].op = new ScriptOperation[func[i][j].op_amount];
			for (k=0;k<func[i][j].op_amount;k++) {
				ScriptOperation& op = func[i][j].op[k];
				ScriptOperation& opfrom = from.func[i][j].op[k];
				op.parent = &func[i][j];
				op.opcode = opfrom.opcode;
				op.base_code = opfrom.base_code;
				op.ext_code = opfrom.ext_code;
				op.vararg_flag = opfrom.vararg_flag;
				op.size_byte = opfrom.size_byte;
				op.arg_amount = opfrom.arg_amount;
				op.size = opfrom.size;
				op.arg = new ScriptArgument[op.arg_amount];
				for (l=0;l<op.arg_amount;l++) {
					op.arg[l].parent = &op;
					op.arg[l].typesize = opfrom.arg[l].typesize;
					op.arg[l].size = opfrom.arg[l].size;
					op.arg[l].value = opfrom.arg[l].value;
					op.arg[l].is_var = opfrom.arg[l].is_var;
					op.arg[l].is_signed = opfrom.arg[l].is_signed;
					if (op.arg[l].is_var) {
						op.arg[l].var = new uint8_t[op.arg[l].size];
						memcpy(op.arg[l].var,opfrom.arg[l].var,op.arg[l].size*sizeof(uint8_t));
					}
				}
			}
		}
		local_data[i].local_type = new uint8_t[local_data[i].amount];
		local_data[i].type = new int16_t[local_data[i].amount];
		local_data[i].size = new uint8_t[local_data[i].amount];
		local_data[i].name = new wstring[local_data[i].amount];
		local_data[i].cat = new uint8_t[local_data[i].amount];
		local_data[i].id = new uint16_t[local_data[i].amount];
		memcpy(local_data[i].local_type,from.local_data[i].local_type,local_data[i].amount*sizeof(uint8_t));
		memcpy(local_data[i].type,from.local_data[i].type,local_data[i].amount*sizeof(int16_t));
		memcpy(local_data[i].size,from.local_data[i].size,local_data[i].amount*sizeof(uint8_t));
		memcpy(local_data[i].name,from.local_data[i].name,local_data[i].amount*sizeof(wstring));
		memcpy(local_data[i].cat,from.local_data[i].cat,local_data[i].amount*sizeof(uint8_t));
		memcpy(local_data[i].id,from.local_data[i].id,local_data[i].amount*sizeof(uint16_t));
	}
	global_data.amount = from.global_data.amount;
	global_data.allocate_amount = from.global_data.allocate_amount;
	global_data.local_type = new uint8_t[global_data.amount];
	global_data.type = new int16_t[global_data.amount];
	global_data.size = new uint8_t[global_data.amount];
	global_data.name = new wstring[global_data.amount];
	global_data.cat = new uint8_t[global_data.amount];
	global_data.id = new uint16_t[global_data.amount];
	memcpy(global_data.local_type,from.global_data.local_type,global_data.amount*sizeof(uint8_t));
	memcpy(global_data.type,from.global_data.type,global_data.amount*sizeof(int16_t));
	memcpy(global_data.size,from.global_data.size,global_data.amount*sizeof(uint8_t));
	memcpy(global_data.name,from.global_data.name,global_data.amount*sizeof(wstring));
	memcpy(global_data.cat,from.global_data.cat,global_data.amount*sizeof(uint8_t));
	memcpy(global_data.id,from.global_data.id,global_data.amount*sizeof(uint16_t));
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
