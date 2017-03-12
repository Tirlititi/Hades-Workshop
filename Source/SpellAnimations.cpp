#include "SpellAnimations.h"

#include <cstring>
#include "Hades_Strings.h"

SpellAnimSequenceCodeLine seqcodetmp[0x400];

#define MACRO_SPELL_ANIM_IOFUNCTION_HEADER(IO,SEEK,READ,PPF) \
	if (PPF) PPFInitScanStep(f); \
	IO ## Short(f,chunk_amount); \
	if (READ) { \
		chunk_index = new uint16_t[chunk_amount]; \
		chunk_entry_amount = new uint16_t[chunk_amount]; \
		chunk_entry_id1 = new uint8_t*[chunk_amount]; \
		chunk_entry_id2 = new uint8_t*[chunk_amount]; \
		chunk_entry_size = new uint16_t*[chunk_amount]; \
	} \
	for (i=0;i<chunk_amount;i++) { \
		IO ## Short(f,chunk_index[i]); \
		IO ## Short(f,chunk_entry_amount[i]); \
		if (READ) { \
			chunk_entry_id1[i] = new uint8_t[chunk_entry_amount[i]]; \
			chunk_entry_id2[i] = new uint8_t[chunk_entry_amount[i]]; \
			chunk_entry_size[i] = new uint16_t[chunk_entry_amount[i]]; \
		} \
		for (j=0;j<chunk_entry_amount[i];j++) { \
			IO ## Char(f,chunk_entry_id1[i][j]); \
			IO ## Char(f,chunk_entry_id2[i][j]); \
			IO ## Short(f,chunk_entry_size[i][j]); \
			if (chunk_entry_id1[i][j]==2 && chunk_index[i]==0) { \
				IO ## Short(f,chunk_entry2_additional); \
			} \
		} \
	} \
	if (PPF) PPFEndScanStep();

#define MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(IO,SEEK) \
	unsigned int argi; \
	seq_code_amount = 0; \
	do { \
		IO ## Char(f,seqcodetmp[seq_code_amount].code); \
		for (argi=0;argi<SPELL_ANIMATION_SEQUENCE_ARG;argi++) \
			IO ## Char(f,seqcodetmp[seq_code_amount].arg[argi]); \
	} while (seqcodetmp[seq_code_amount++].code!=0); \
	seq_code = new SpellAnimSequenceCodeLine[seq_code_amount]; \
	memcpy(seq_code,seqcodetmp,seq_code_amount*sizeof(SpellAnimSequenceCodeLine)); \
	for (unsigned int i=0;i<seq_code_amount;i++) \
		seq_code[i].parent = this;

#define MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(IO,SEEK,PPF) \
	if (PPF) PPFInitScanStep(f); \
	for (i=0;i<seq_code_amount;i++) { \
		IO ## Char(f,seq_code[i].code); \
		for (j=0;j<SPELL_ANIMATION_SEQUENCE_ARG;j++) \
			IO ## Char(f,seq_code[i].arg[j]); \
	} \
	if (PPF) PPFEndScanStep();


void SpellAnimationDataStruct::Read(fstream& f) {
	unsigned int i,j;
	uint32_t headerpos = f.tellg();
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(FFIXRead,FFIXSeek,true,false)
		FFIXSeek(f,headerpos,0x400);
		MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(FFIXRead,FFIXSeek)
	} else {
		raw_data = new uint8_t[raw_size];
		f.read((char*)raw_data,raw_size);
		SteamSeek(f,headerpos,0);
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(SteamRead,SteamSeek,true,false)
		SteamSeek(f,headerpos,0x400);
		MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(SteamRead,SteamSeek)
	}
	modified_data = 0;
}

void SpellAnimationDataStruct::Write(fstream& f) {
	unsigned int i,j;
	uint32_t headerpos = f.tellg();
	if (GetGameType()==GAME_TYPE_PSX) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(FFIXWrite,FFIXSeek,false,false)
		FFIXSeek(f,headerpos,0x400);
		MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(FFIXWrite,FFIXSeek,false)
		modified_data = 0;
	} else {
		f.write((char*)raw_data,raw_size);
		f.seekg(headerpos);
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(SteamWrite,SteamSeek,false,false)
		f.seekg(headerpos+0x400);
		MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(SteamWrite,SteamSeek,false)
	}
}

void SpellAnimationDataStruct::WritePPF(fstream& f) {
	unsigned int i,j;
	uint32_t headerpos = f.tellg();
	MACRO_SPELL_ANIM_IOFUNCTION_HEADER(PPFStepAdd,FFIXSeek,false,true)
	FFIXSeek(f,headerpos,0x400);
	MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(PPFStepAdd,FFIXSeek,true)
}

void SpellAnimationDataStruct::ReadHWS(fstream& f) {
	unsigned int i,j;
	uint32_t headerpos = f.tellg();
	Spell_Animation_Data_Type hwsmodif;
	HWSReadLong(f,hwsmodif);
	if (hwsmodif & SPELL_ANIMATION_DATA_TYPE_HEADER) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(HWSRead,HWSSeek,true,false)
	}
	if (hwsmodif & SPELL_ANIMATION_DATA_TYPE_SEQUENCE) {
		MACRO_SPELL_ANIM_IOFUNCTION_SEQREAD(HWSRead,HWSSeek)
	}
	modified_data |= hwsmodif;
}

void SpellAnimationDataStruct::WriteHWS(fstream& f) {
	unsigned int i,j;
	uint32_t headerpos = f.tellg();
	HWSWriteLong(f,modified_data);
	if (modified_data & SPELL_ANIMATION_DATA_TYPE_HEADER) {
		MACRO_SPELL_ANIM_IOFUNCTION_HEADER(HWSWrite,HWSSeek,false,false)
	}
	if (modified_data & SPELL_ANIMATION_DATA_TYPE_SEQUENCE) {
		MACRO_SPELL_ANIM_IOFUNCTION_SEQWRITE(HWSWrite,HWSSeek,false)
	}
}

void SpellAnimationDataStruct::Copy(SpellAnimationDataStruct& from, bool deleteold) {
	unsigned int i;
	if (deleteold) {
		delete[] seq_code;
	}
	*this = from;
	seq_code = new SpellAnimSequenceCodeLine[seq_code_amount];
	memcpy(seq_code,from.seq_code,seq_code_amount*sizeof(SpellAnimSequenceCodeLine));
	for (i=0;i<seq_code_amount;i++) {
		memcpy(seq_code[i].arg,from.seq_code[i].arg,SPELL_ANIMATION_SEQUENCE_ARG*sizeof(uint8_t));
	}
}

bool SpellAnimationDataStruct::AddAnimationSequenceCode(unsigned int codeline, uint8_t newcode) {
	if (seq_code_amount>=0x155)
		return false;
	SpellAnimSequenceCodeLine* buffer = new SpellAnimSequenceCodeLine[seq_code_amount+1];
	memcpy(buffer,seq_code,codeline*sizeof(SpellAnimSequenceCodeLine));
	buffer[codeline].parent = this;
	buffer[codeline].code = newcode;
	for (unsigned int i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++)
		buffer[codeline].arg[i] = 0;
	memcpy(buffer+codeline+1,seq_code+codeline,(seq_code_amount-codeline)*sizeof(SpellAnimSequenceCodeLine));
	delete[] seq_code;
	seq_code = buffer;
	seq_code_amount++;
	return true;
}

void SpellAnimationDataStruct::DeleteAnimationSequenceCode(unsigned int codeline) {
	SpellAnimSequenceCodeLine* buffer = new SpellAnimSequenceCodeLine[seq_code_amount-1];
	memcpy(buffer,seq_code,codeline*sizeof(SpellAnimSequenceCodeLine));
	memcpy(buffer+codeline,seq_code+codeline+1,(seq_code_amount-codeline-1)*sizeof(SpellAnimSequenceCodeLine));
	delete[] seq_code;
	seq_code = buffer;
	seq_code_amount--;
}

void SpellAnimationDataStruct::ChangeAnimationSequenceCode(unsigned int codeline, uint8_t newcode) {
	seq_code[codeline].code = newcode;
	for (unsigned int i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++)
		seq_code[codeline].arg[i] = 0;
}

// Assume there's only 1 spell_dir
void SpellAnimationDataSet::Load(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap) {
	unsigned int i;
	amount = GetGameType()==GAME_TYPE_PSX ? globalmap.spell_dir->file_amount : SPELL_ANIMATION_AMOUNT;
	spell = new SpellAnimationDataStruct[amount];
	if (GetGameType()==GAME_TYPE_PSX) {
		for (i=0;i<amount;i++) {
			spell[i].id = i;
			spell[i].parent = this;
			if (globalmap.spell_dir->file_offset[i]!=0xFFFF) {
				spell[i].is_empty = false;
				ffbin.seekg(IMG_HEADER_OFFSET+FILE_IGNORE_DATA_PERIOD*(globalmap.spell_dir->first_file_offset+globalmap.spell_dir->file_offset[i]));
				spell[i].Read(ffbin);
			} else
				spell[i].is_empty = true;
		}
	} else {
		string fname = config.steam_dir_data;
		fname += "resources.assets";
		ffbin.open(fname.c_str(),ios::in | ios::binary);
		for (i=0;i<amount;i++) {
			spell[i].id = i;
			spell[i].parent = this;
			if (config.spellanim_steam_file[i]>=0) {
				spell[i].is_empty = false;
				ffbin.seekg(config.meta_res.GetFileOffsetByIndex(config.spellanim_steam_file[i]));
				spell[i].raw_size = config.meta_res.GetFileSizeByIndex(config.spellanim_steam_file[i]);
				spell[i].Read(ffbin);
			} else
				spell[i].is_empty = true;
		}
		ffbin.close();
	}
}

void SpellAnimationDataSet::Write(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap) {
	unsigned int i;
	for (i=0;i<amount;i++) {
		if (!spell[i].is_empty) {
			ffbin.seekg(IMG_HEADER_OFFSET+FILE_IGNORE_DATA_PERIOD*(globalmap.spell_dir->first_file_offset+globalmap.spell_dir->file_offset[i]));
			spell[i].Write(ffbin);
		}
	}
}

void SpellAnimationDataSet::WritePPF(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap) {
	unsigned int i;
	for (i=0;i<amount;i++) {
		if (!spell[i].is_empty) {
			ffbin.seekg(IMG_HEADER_OFFSET+FILE_IGNORE_DATA_PERIOD*(globalmap.spell_dir->first_file_offset+globalmap.spell_dir->file_offset[i]));
			spell[i].WritePPF(ffbin);
		}
	}
}

int* SpellAnimationDataSet::LoadHWS(fstream& ffhws) {
	uint32_t spam,spamsize,tmp32;
	uint16_t spamid,i;
	int* res = new int[1];
	res[0] = 0;
	HWSReadLong(ffhws,tmp32); // spell_dir amount
	HWSReadLong(ffhws,tmp32); // spell_dir ID
	HWSReadLong(ffhws,spam);
	for (i=0;i<spam;i++) {
		HWSReadShort(ffhws,spamid);
		spell[spamid].ReadHWS(ffhws);
	}
	return res;
}

void SpellAnimationDataSet::WriteHWS(fstream& ffhws) {
	uint32_t spam = 0;
	uint16_t i;
	HWSWriteLong(ffhws,1); // spell_dir amount
	HWSWriteLong(ffhws,0); // spell_dir ID
	uint32_t spampos = ffhws.tellg();
	HWSWriteLong(ffhws,0); // amount of spell animations
	for (i=0;i<amount;i++) {
		if (!spell[i].is_empty && spell[i].modified_data) {
			HWSWriteShort(ffhws,i);
			spell[i].WriteHWS(ffhws);
			spam++;
		}
	}
	uint32_t endpos = ffhws.tellg();
	HWSSeek(ffhws,spampos,0);
	HWSWriteLong(ffhws,spam);
	HWSSeek(ffhws,endpos,0);
}
