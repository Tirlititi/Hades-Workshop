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
extern vector<MipsCode> MIPS_CODE_LIST;

struct MipsDataSet {
	uint32_t battle_ram_pos[MIPS_BATTLE_RAM_POS_AMOUNT];
	MipsSequence battle_code;
	MipsFunction battle_spell_effect[SPELL_EFFECT_AMOUNT];
	
	bool battle_modified;
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	int LoadHWS(fstream& ffhws);
	void WriteHWS(fstream& ffhws);
	
	// 1: Battle code
	void MarkAsModified(int datatype);
	void SetupBattleFunction();
};

#endif
