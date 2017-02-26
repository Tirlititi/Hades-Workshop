#ifndef _SPELL_ANIMATIONS_H
#define _SPELL_ANIMATIONS_H

#define SPELL_ANIMATION_AMOUNT			511
#define SPELL_ANIMATION_SEQUENCE_ARG	2

struct SpellAnimSequenceCodeLine;
struct SpellAnimationDataStruct;
struct SpellAnimationDataSet;

#include <inttypes.h>
#include <fstream>
#include "Configuration.h"
#include "ImageMaps.h"
#include "Database_Script.h"
using namespace std;

typedef uint32_t Spell_Animation_Data_Type;
#define SPELL_ANIMATION_DATA_TYPE_WHOLE		0xFFFFFFFF // Unused yet
#define SPELL_ANIMATION_DATA_TYPE_HEADER	0x1
#define SPELL_ANIMATION_DATA_TYPE_SEQUENCE	0x2

struct SpellAnimSequenceCodeLine {
	SpellAnimationDataStruct* parent;
	uint8_t code;
	uint8_t arg[SPELL_ANIMATION_SEQUENCE_ARG];
};

struct SpellAnimationDataStruct {
public:
	SpellAnimationDataSet* parent;
	uint16_t id;
	uint32_t size; // in term of 0x800 sized blocks
	bool is_empty; // empty animations don't have all the fields initialized
	uint16_t chunk_amount;
	uint16_t* chunk_index;
	uint16_t* chunk_entry_amount;
	uint8_t** chunk_entry_id1;
	uint8_t** chunk_entry_id2;
	uint16_t** chunk_entry_size;
	uint16_t chunk_entry2_additional;
	unsigned int seq_code_amount;
	SpellAnimSequenceCodeLine* seq_code;
	
	Spell_Animation_Data_Type modified_data;
	
	// Steam version only
	uint32_t raw_size;
	uint8_t* raw_data;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void Copy(SpellAnimationDataStruct& from, bool deleteold = false);
	
	// Return false if there is no space
	// Arguments are defaulted
	bool AddAnimationSequenceCode(unsigned int codeline, uint8_t newcode);
	void DeleteAnimationSequenceCode(unsigned int codeline);
	void ChangeAnimationSequenceCode(unsigned int codeline, uint8_t newcode);
};

struct SpellAnimationDataSet {
public:
	unsigned int amount;
	SpellAnimationDataStruct* spell;
	
	GlobalMapSpellDirStruct** map_directory;
	
	void Load(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap);
	void Write(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap);
	void WritePPF(fstream& ffbin, ConfigurationSet& config, GlobalImageMapStruct& globalmap);
	// return value is int[1] to be deleted
	// {Number of oversized animations}
	int* LoadHWS(fstream& ffhws);
	void WriteHWS(fstream& ffhws);
};

#endif
