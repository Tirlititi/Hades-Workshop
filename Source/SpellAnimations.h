#ifndef _SPELL_ANIMATIONS_H
#define _SPELL_ANIMATIONS_H

#define SPELL_ANIMATION_AMOUNT			511
#define SPELL_ANIMATION_SEQUENCE_ARG	2

struct SpellAnimSequenceCodeLine;
struct SpellAnimImage;
struct SpellAnimResourceSet;
struct SpellAnimationDataStruct;
struct SpellAnimationDataSet;

#include <inttypes.h>
#include <fstream>
#include <array>
#include "Configuration.h"
#include "ImageMaps.h"
#include "Database_Script.h"
using namespace std;

typedef uint32_t Spell_Animation_Data_Type;
#define SPELL_ANIMATION_DATA_TYPE_WHOLE		0xFFFFFFFF // Unused yet
#define SPELL_ANIMATION_DATA_TYPE_HEADER	0x1
#define SPELL_ANIMATION_DATA_TYPE_SEQUENCE	0x2

enum struct SpellDataResourceType {
	AKAO,

	END = 0xFF
};

struct SpellAnimSequenceCodeLine {
	SpellAnimationDataStruct* parent;
	uint8_t code;
	uint8_t arg[SPELL_ANIMATION_SEQUENCE_ARG];
};

struct SpellAnimImage {
	uint32_t offset_list;
	uint32_t offset_palette;
	uint32_t palette_tim_amount;
	uint16_t small_palette_amount; // Small palette = 16 colors (RGBA16)
	uint16_t palette_amount; // Palette = 256 colors (RGBA16)
	vector<uint16_t> small_palette_key;
	vector<uint16_t> palette_key;
	uint16_t padding1;
	uint32_t offset_pixel;
	uint32_t tim_amount;
	vector<TIMImageDataStruct> tim;
	vector<TIMImageDataStruct> palette_tim;

	uint32_t linked_chunk_entry_pos;
	uint32_t tim_chunk_size;

	uint32_t* GetPaletteFromKey(uint16_t key, bool shortformat);
};

struct SpellAnimResourceAKAO {
	vector<uint8_t> raw;
};

struct SpellAnimResourceSet {
	unsigned int amount;
	vector<uint32_t> offset;
	vector<uint32_t> unknown;
	vector<SpellDataResourceType> type;
	vector<void*> data;

	void Flush() {
		amount = 0;
		offset.clear();
		unknown.clear();
		type.clear();
		for (unsigned int i=0;i+1<data.size();i++)
			delete[] data[i];
		data.clear();
	}
};

struct SpellAnimationDataStruct {
public:
	SpellAnimationDataSet* parent;
	Spell_Animation_Data_Type modified_data;
	uint16_t id;
	uint32_t size; // in term of 0x800 sized blocks
	bool is_empty; // empty animations don't have all the fields initialized

	// Header: chunks
	uint16_t chunk_amount;
	vector<uint16_t> chunk_index;
	vector<uint16_t> chunk_entry_amount;
	vector< vector<uint8_t> > chunk_entry_id;
	vector< vector<uint8_t> > chunk_entry_info;
	vector< vector<uint16_t> > chunk_entry_size; // 0x800 sized blocks
	vector< vector<uint32_t> > chunk_entry_pos; // in bytes
	uint16_t chunk_entry2_additional_size; // 0x800 sized blocks

	// Header: spell sequence
	unsigned int seq_code_amount;
	vector<SpellAnimSequenceCodeLine> seq_code;
	
	// Chunk 01, 04, 05: images
	vector<SpellAnimImage> image;

	// Chunk 15, 20, 21: resources (AKAO, ...)
	vector<SpellAnimResourceSet> resource;
	// 21 has 2 chunks X and Y, Y comes first
	// X = resources
	// 70: AKAO directly

	// Steam version only
	uint32_t raw_size;
	uint8_t* raw_data;
	
	void Read(fstream& f);
	void Write(fstream& f);
	void WritePPF(fstream& f);
	void ReadHWS(fstream& f);
	void WriteHWS(fstream& f);
	void Copy(SpellAnimationDataStruct& from);
	
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
