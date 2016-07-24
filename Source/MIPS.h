#ifndef _MIPS_H
#define _MIPS_H

struct MipsCode; // MISP opcode informations
struct MipsInstruction; // a single MISP instruction
struct MipsFunction; // part of a MISP sequence
struct MipsSequence; // whole block of MISP code
struct MipsDataSet;

#include "Configuration.h"
using namespace std;

#define MIPS_BATTLE_RAM_POS_AMOUNT		589
#define MIPS_BATTLE_SPELL_EFFECT_INDEX	273

typedef uint8_t Mips_Type;
#define MIPS_TYPE_NOOP	0
#define MIPS_TYPE_R		1
#define MIPS_TYPE_I		2
#define MIPS_TYPE_J		3

struct MipsCode {
	uint8_t opcode;
	uint8_t function;
	Mips_Type type;
	wstring name;
	wstring str;
};

struct MipsInstruction {
	uint8_t opcode; // 6 bits
	uint8_t function; // 6 bits
	uint8_t regs_add; // 5 bits
	uint8_t regt_add; // 5 bits
	uint8_t regd_add; // 5 bits
	uint8_t shift_val; // 5 bits
	uint16_t val; // 16 bits
	uint32_t jump_off; // 26 bits
	unsigned int mips_code_index;
	
	uint32_t ram_pos;
	
	wstring GetDisplayStr();
	void CopyValue(MipsInstruction& from);
};

struct MipsFunction {
	unsigned int amount;
	MipsInstruction** instruction;
};

struct MipsSequence {
	uint32_t amount;
	MipsInstruction* instruction;
	uint32_t ram_pos;
	
	void Read(fstream& f, unsigned int am, uint32_t rampos);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
};

#define MIPS_CODE_NONE	62
#define MIPS_CODE_UNK	63
static MipsCode MIPS_CODE_LIST[] = {
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

struct MipsDataSet {
	uint32_t battle_ram_pos[MIPS_BATTLE_RAM_POS_AMOUNT];
	MipsSequence battle_code;
	MipsFunction battle_spell_effect[SPELL_EFFECT_AMOUNT];
	
	bool battle_modified;
	
	void Load(fstream& ffbin, ConfigurationSet config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	int LoadHWS(fstream& ffhws);
	void WriteHWS(fstream& ffhws);
	
	// 1: Battle code
	void MarkAsModified(int datatype);
	void SetupBattleFunction();
};

#endif
