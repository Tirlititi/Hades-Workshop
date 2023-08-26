#include "MIPS.h"

#include <sstream>

#define MIPS_HWS_VERSION		1

#define MIPS_HWS_BATTLE_DATA	1

#define MIPS_BATTLE_RAM_OFFSET_GAP		0x16FCL
#define MIPS_BATTLE_RAM_OFFSET_GAP_JAP	0x174CL

wstring MipsInstruction::GetDisplayStr() {
	wstringstream resstream;
	size_t lastpos = 0;
	size_t pos = MIPS_CODE_LIST[mips_code_index].str.find_first_of(L"%");
	wchar_t c;
	while (pos!=string::npos) {
		resstream << MIPS_CODE_LIST[mips_code_index].str.substr(lastpos,pos-lastpos);
		lastpos = pos+2;
		c = MIPS_CODE_LIST[mips_code_index].str[pos+1];
		if (c==L'%')
			resstream << L"%";
		else if (c==L'd')
			resstream << L"$" << regd_add;
		else if (c==L's')
			resstream << L"$" << regs_add;
		else if (c==L't')
			resstream << L"$" << regt_add;
		else if (c==L'h')
			resstream << shift_val;
		else if (c==L'j')
			resstream << L"0x" << std::uppercase << std::hex << jump_off*4;
		else if (c==L'v') {
			c = MIPS_CODE_LIST[mips_code_index].str[pos+2];
			lastpos++;
			if (c==L's')
				resstream << (int16_t)val;
			else if (c==L'u')
				resstream << val;
			else if (c==L'h')
				resstream << L"0x" << std::uppercase << std::hex << val;
			else if (c==L'o')
				resstream << L"0x" << std::uppercase << std::hex << ((ram_pos & 0x7FFFFFFF)+(int16_t)val*4+4);
		}
		pos = MIPS_CODE_LIST[mips_code_index].str.find_first_of(L"%",lastpos);
	}
	resstream << MIPS_CODE_LIST[mips_code_index].str.substr(lastpos);
	return resstream.str();
}

void MipsInstruction::CopyValue(MipsInstruction& from) {
	opcode = from.opcode;
	function = from.function;
	regs_add = from.regs_add;
	regt_add = from.regt_add;
	regd_add = from.regd_add;
	shift_val = from.shift_val;
	val = from.val;
	jump_off = from.jump_off;
	mips_code_index = from.mips_code_index;
	ram_pos = from.ram_pos;
}

#define MACRO_MIPS_READ(IO,SEEK) \
	for (i=0;i<amount;i++) { \
		IO ## Long(ffbin,instr); \
		instruction[i].ram_pos = ram_pos+i*4; \
		instruction[i].opcode = (instr >> 26) & 0x3F; \
		instruction[i].mips_code_index = MIPS_CODE_UNK; \
		if (instr==0) { \
			instruction[i].mips_code_index = MIPS_CODE_NONE; \
		} else if (instruction[i].opcode==0) { \
			instruction[i].regs_add = (instr >> 21) & 0x1F; \
			instruction[i].regt_add = (instr >> 16) & 0x1F; \
			instruction[i].regd_add = (instr >> 11) & 0x1F; \
			instruction[i].shift_val = (instr >> 6) & 0x1F; \
			instruction[i].function = instr & 0x3F; \
			for (j=0;j<MIPS_CODE_UNK;j++) \
				if (MIPS_CODE_LIST[j].opcode==0 && MIPS_CODE_LIST[j].function==instruction[i].function) { \
					instruction[i].mips_code_index = j; \
					break; \
				} \
		} else { \
			for (j=0;j<MIPS_CODE_UNK;j++) { \
				if (MIPS_CODE_LIST[j].opcode==instruction[i].opcode) { \
					if (MIPS_CODE_LIST[j].type==MIPS_TYPE_J) { \
						instruction[i].jump_off = instr & 0x3FFFFFF; \
						instruction[i].mips_code_index = j; \
						break; \
					} else { \
						instruction[i].regs_add = (instr >> 21) & 0x1F; \
						instruction[i].regt_add = (instr >> 16) & 0x1F; \
						instruction[i].val = instr & 0xFFFF; \
						if (MIPS_CODE_LIST[j].opcode!=1 || MIPS_CODE_LIST[j].function==instruction[i].regt_add) { \
							instruction[i].mips_code_index = j; \
							break; \
						} \
					} \
				} \
			} \
		} \
	}

#define MACRO_MIPS_WRITE(IO,SEEK,PPF) \
	if (PPF) PPFInitScanStep(ffbin); \
	for (i=0;i<amount;i++) { \
		if (MIPS_CODE_LIST[instruction[i].mips_code_index].type==MIPS_TYPE_R) { \
			instr = (instruction[i].opcode & 0x3F) << 26; \
			instr |= (instruction[i].regs_add & 0x1F) << 21; \
			instr |= (instruction[i].regt_add & 0x1F) << 16; \
			instr |= (instruction[i].regd_add & 0x1F) << 11; \
			instr |= (instruction[i].shift_val & 0x1F) << 6; \
			instr |= instruction[i].function & 0x3F; \
		} else if (MIPS_CODE_LIST[instruction[i].mips_code_index].type==MIPS_TYPE_I) { \
			instr = (instruction[i].opcode & 0x3F) << 26; \
			instr |= (instruction[i].regs_add & 0x1F) << 21; \
			instr |= (instruction[i].regt_add & 0x1F) << 16; \
			instr |= instruction[i].val & 0xFFFF; \
		} else if (MIPS_CODE_LIST[instruction[i].mips_code_index].type==MIPS_TYPE_J) { \
			instr = (instruction[i].opcode & 0x3F) << 26; \
			instr |= instruction[i].jump_off & 0x3FFFFFF; \
		} else { \
			instr = 0; \
		} \
		IO ## Long(ffbin,instr); \
	} \
	if (PPF) PPFEndScanStep();


void MipsSequence::Read(fstream& ffbin, unsigned int am, uint32_t rampos) {
	unsigned int i,j;
	uint32_t instr;
	amount = am;
	ram_pos = rampos;
	instruction = new MipsInstruction[amount];
	MACRO_MIPS_READ(FFIXRead,FFIXSeek)
}

void MipsSequence::Write(fstream& ffbin) {
	unsigned int i;
	uint32_t instr;
	MACRO_MIPS_WRITE(FFIXWrite,FFIXSeek,false)
}

void MipsSequence::WritePPF(fstream& ffbin) {
	unsigned int i;
	uint32_t instr;
	MACRO_MIPS_WRITE(PPFStepAdd,FFIXSeek,true)
}

void MipsSequence::ReadHWS(fstream& ffbin) {
	unsigned int i,j;
	uint32_t instr;
	instruction = new MipsInstruction[amount];
	MACRO_MIPS_READ(HWSRead,HWSSeek)
}

void MipsSequence::WriteHWS(fstream& ffbin) {
	unsigned int i;
	uint32_t instr;
	MACRO_MIPS_WRITE(HWSWrite,HWSSeek,false)
}

void MipsDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	ffbin.seekg(config.mips_battle_offset_list);
	for (i=0;i<MIPS_BATTLE_RAM_POS_AMOUNT;i++)
		FFIXReadLong(ffbin,battle_ram_pos[i]);
	ffbin.seekg(config.mips_battle_code_offset);
	if (config.language & LANGUAGE_VERSION_JAPAN)
		battle_code.Read(ffbin,config.mips_battle_code_amount,battle_ram_pos[0]-MIPS_BATTLE_RAM_OFFSET_GAP_JAP);
	else
		battle_code.Read(ffbin,config.mips_battle_code_amount,battle_ram_pos[0]-MIPS_BATTLE_RAM_OFFSET_GAP);
	SetupBattleFunction();
	battle_modified = false;
}

void MipsDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	ffbin.seekg(config.mips_battle_offset_list);
	for (i=0;i<MIPS_BATTLE_RAM_POS_AMOUNT;i++)
		FFIXWriteLong(ffbin,battle_ram_pos[i]);
	ffbin.seekg(config.mips_battle_code_offset);
	battle_code.Write(ffbin);
	battle_modified = false;
}

void MipsDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config) {
	unsigned int i;
	PPFInitScanStep(ffbin);
	ffbin.seekg(config.mips_battle_offset_list);
	for (i=0;i<MIPS_BATTLE_RAM_POS_AMOUNT;i++)
		PPFStepAddLong(ffbin,battle_ram_pos[i]);
	PPFEndScanStep();
	ffbin.seekg(config.mips_battle_code_offset);
	battle_code.WritePPF(ffbin);
}

int MipsDataSet::LoadHWS(fstream& ffbin) {
	uint16_t version;
	uint8_t datatype;
	int res = 0;
	HWSReadShort(ffbin,version);
	HWSReadChar(ffbin,datatype);
	while (datatype!=0xFF) {
		if (datatype==MIPS_HWS_BATTLE_DATA) {
			uint32_t am;
			HWSReadLong(ffbin,am);
			if (am>battle_code.amount) {
				res |= 1;
				HWSSeek(ffbin,ffbin.tellg(),4*am);
			} else {
				battle_code.amount = am;
				battle_code.ReadHWS(ffbin);
				SetupBattleFunction();
				battle_modified = true;
			}
		}
		HWSReadChar(ffbin,datatype);
	}
	return 0;
}

void MipsDataSet::WriteHWS(fstream& ffbin) {
	HWSWriteShort(ffbin,MIPS_HWS_VERSION);
	HWSWriteChar(ffbin,MIPS_HWS_BATTLE_DATA);
	HWSWriteLong(ffbin,battle_code.amount);
	battle_code.WriteHWS(ffbin);
	HWSWriteChar(ffbin,0xFF);
}

void MipsDataSet::MarkAsModified(int datatype) {
	if (datatype==1 || datatype<0)
		battle_modified = true;
}

void MipsDataSet::SetupBattleFunction() {
	unsigned int i,j;
	uint32_t pos1,pos2;
	for (i=0;i<SPELL_EFFECT_AMOUNT;i++) {
		pos1 = battle_ram_pos[MIPS_BATTLE_SPELL_EFFECT_INDEX+i];
		pos2 = battle_code.ram_pos+(battle_code.amount << 2);
		for (j=0;j<MIPS_BATTLE_RAM_POS_AMOUNT;j++)
			if (battle_ram_pos[j]<pos2 && battle_ram_pos[j]>pos1)
				pos2 = battle_ram_pos[j];
		battle_spell_effect[i].amount = (pos2-pos1) >> 2;
		battle_spell_effect[i].instruction = new MipsInstruction*[battle_spell_effect[i].amount];
		for (j=0;j<battle_spell_effect[i].amount;j++)
			battle_spell_effect[i].instruction[j] = &battle_code.instruction[((pos1-battle_code.ram_pos) >> 2)+j];
	}
}

vector<MipsCode> MIPS_CODE_LIST = {
	{ 0x00, 0x20, MIPS_TYPE_R, L"add", L"%d = %s + %t" },
	{ 0x00, 0x21, MIPS_TYPE_R, L"addu", L"%d = %s + %t" },
	{ 0x08, 0x00, MIPS_TYPE_I, L"addi", L"%t = %s + %vs" },
	{ 0x09, 0x00, MIPS_TYPE_I, L"addiu", L"%t = %s + %vu" },
	{ 0x00, 0x22, MIPS_TYPE_R, L"sub", L"%d = %s - %t" },
	{ 0x00, 0x23, MIPS_TYPE_R, L"subu", L"%d = %s - %t" },
	{ 0x00, 0x18, MIPS_TYPE_R, L"mult", L"$LO = %s * %t (overflow value stored in $HI)" },
	{ 0x00, 0x19, MIPS_TYPE_R, L"multu", L"$LO = %s * %t (overflow value stored in $HI)" },
	{ 0x00, 0x1A, MIPS_TYPE_R, L"div", L"$LO = %s / %t ; $HI = %s %% %t" },
	{ 0x00, 0x1B, MIPS_TYPE_R, L"divu", L"$LO = %s / %t ; $HI = %s %% %t" },
	{ 0x00, 0x11, MIPS_TYPE_R, L"mthi", L"$HI = %s" },
	{ 0x00, 0x13, MIPS_TYPE_R, L"mtlo", L"$LO = %s" },
	{ 0x00, 0x10, MIPS_TYPE_R, L"mfhi", L"%d = $HI" },
	{ 0x00, 0x12, MIPS_TYPE_R, L"mflo", L"%d = $LO" },
	{ 0x00, 0x24, MIPS_TYPE_R, L"and", L"%d = %s & %t" },
	{ 0x0C, 0x00, MIPS_TYPE_I, L"andi", L"%t = %s & %vh" },
	{ 0x00, 0x25, MIPS_TYPE_R, L"or", L"%d = %s | %t" },
	{ 0x0D, 0x00, MIPS_TYPE_I, L"ori", L"%t = %s | %vh" },
	{ 0x00, 0x26, MIPS_TYPE_R, L"xor", L"%d = %s ^ %t" },
	{ 0x0E, 0x00, MIPS_TYPE_I, L"xori", L"%t = %s ^ %vh" },
	{ 0x00, 0x27, MIPS_TYPE_R, L"nor", L"%t = %s nor %vh" },
	{ 0x00, 0x00, MIPS_TYPE_R, L"sll", L"%d = %t << %h" },
	{ 0x00, 0x04, MIPS_TYPE_R, L"sllv", L"%d = %t << %s" },
	{ 0x00, 0x02, MIPS_TYPE_R, L"srl", L"%d = %t >> %h" },
	{ 0x00, 0x06, MIPS_TYPE_R, L"srlv", L"%d = %t >> %s" },
	{ 0x00, 0x03, MIPS_TYPE_R, L"sra", L"%d = %t >> %h (keep sign bit)" },
	{ 0x00, 0x07, MIPS_TYPE_R, L"srav", L"%d = %t >> %s (keep sign bit)" },
	{ 0x0F, 0x00, MIPS_TYPE_I, L"lui", L"%t = %vu << 16" },
	{ 0x20, 0x00, MIPS_TYPE_I, L"lb", L"%t = %s[%vs] & 0xFF" },
	{ 0x24, 0x00, MIPS_TYPE_I, L"lbu", L"%t = %s[%vs] & 0xFF" },
	{ 0x21, 0x00, MIPS_TYPE_I, L"lh", L"%t = %s[%vs] & 0xFFFF" },
	{ 0x25, 0x00, MIPS_TYPE_I, L"lhu", L"%t = %s[%vs] & 0xFFFF" },
	{ 0x23, 0x00, MIPS_TYPE_I, L"lw", L"%t = %s[%vs]" },
	{ 0x22, 0x00, MIPS_TYPE_I, L"lwl", L"%t = lshifted(%s,%vs)" },
	{ 0x26, 0x00, MIPS_TYPE_I, L"lwr", L"%t = rshifted(%s,%vs)" },
	{ 0x28, 0x00, MIPS_TYPE_I, L"sb", L"%s[%vs] = %t & 0xFF" },
	{ 0x29, 0x00, MIPS_TYPE_I, L"sh", L"%s[%vs] = %t & 0xFFFF" },
	{ 0x2B, 0x00, MIPS_TYPE_I, L"sw", L"%s[%vs] = %t" },
	{ 0x2D, 0x00, MIPS_TYPE_I, L"swl", L"%s[%vs] = lshifted(%t)" },
	{ 0x2E, 0x00, MIPS_TYPE_I, L"swr", L"%s[%vs] = rshifted(%t)" },
	{ 0x04, 0x00, MIPS_TYPE_I, L"beq", L"if (%s == %t), jump %vo" },
	{ 0x05, 0x00, MIPS_TYPE_I, L"bne", L"if (%s != %t), jump %vo" },
	{ 0x01, 0x01, MIPS_TYPE_I, L"bgez", L"if (%s >= 0), jump %vo" },
	{ 0x07, 0x00, MIPS_TYPE_I, L"bgtz", L"if (%s > 0), jump %vo" },
	{ 0x01, 0x21, MIPS_TYPE_I, L"bgezal", L"if (%s >= 0), $31 = jump %vo (store return position to $31)" },
	{ 0x06, 0x00, MIPS_TYPE_I, L"blez", L"if (%s <= 0), jump %vo" },
	{ 0x01, 0x00, MIPS_TYPE_I, L"bltz", L"if (%s < 0), jump %vo" },
	{ 0x01, 0x20, MIPS_TYPE_I, L"bltzal", L"if (%s < 0), jump %vo (store return position to $31)" },
	{ 0x00, 0x2A, MIPS_TYPE_R, L"slt", L"if (%s < %t) %d = 1 else %d = 0" },
	{ 0x00, 0x2B, MIPS_TYPE_R, L"sltu", L"if (%s < %t) %d = 1 else %d = 0" },
	{ 0x0A, 0x00, MIPS_TYPE_I, L"slti", L"if (%s < %vs) %t = 1 else %t = 0" },
	{ 0x0B, 0x00, MIPS_TYPE_I, L"sltiu", L"if (%s < %vu) %t = 1 else %t = 0" },
	{ 0x02, 0x00, MIPS_TYPE_J, L"j", L"jump %j" },
	{ 0x00, 0x08, MIPS_TYPE_R, L"jr", L"jump %s" },
	{ 0x03, 0x00, MIPS_TYPE_J, L"jal", L"jump %j (store return position to $31)" },
	{ 0x00, 0x09, MIPS_TYPE_R, L"jalr", L"jump %s (store return position to %d)" },
	{ 0x12, 0x00, MIPS_TYPE_I, L"unkn1", L"0x12 : %s %t %vu" }, // Unknown
	{ 0x2A, 0x00, MIPS_TYPE_I, L"unkn2", L"0x2A : %s %t %vu" }, // Unknown
	{ 0x32, 0x00, MIPS_TYPE_I, L"unkn3", L"0x32 : %s %t %vu" }, // Unknown
	{ 0x3A, 0x00, MIPS_TYPE_I, L"unkn4", L"0x3A : %s %t %vu" }, // Unknown
	{ 0x00, 0x0C, MIPS_TYPE_R, L"syscall", L"system call" }, // SPECIAL
	{ 0x00, 0x0D, MIPS_TYPE_R, L"break", L"break" }, // SPECIAL
	{ 0x00, 0x00, MIPS_TYPE_NOOP, L"noop", L"nothing" },
	{ 0xFF, 0xFF, MIPS_TYPE_NOOP, L"unk", L"unknown code" }
};
