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
