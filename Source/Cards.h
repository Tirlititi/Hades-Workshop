#ifndef _CARDS_H
#define _CARDS_H

#define CARD_AMOUNT 100
#define CARD_SET_AMOUNT 64
#define CARD_DECK_AMOUNT 256
#define CARD_SET_CAPACITY 16
struct CardDeckDataStruct;
struct CardSetDataStruct;
struct CardDataStruct;
struct CardDataSet;

#include <inttypes.h>
#include <fstream>
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

struct CardDeckDataStruct {
	uint8_t set;
	uint8_t difficulty;
};

struct CardSetDataStruct {
	uint8_t card[CARD_SET_CAPACITY];
};

struct CardDataStruct {
public:
	int id;
	FF9String name; // readonly
	uint16_t name_size_x;
	
	// In Steam version, only "player" stats are used for both purpose
	uint8_t player_attack;
	uint8_t player_type;
	uint8_t player_defence;
	uint8_t player_magicdefence;
	uint8_t npc_attack;
	uint8_t npc_type;
	uint8_t npc_defence;
	uint8_t npc_magicdefence;
	uint8_t points;
	
	// Return 0 if success ; 1 if the value is too long
	int SetName(wstring newvalue);
	int SetName(FF9String& newvalue);

private:
	uint16_t name_offset;
	CardDataSet* parent;
	friend CardDataSet;
};

struct CardDataSet {
public:
	CardDeckDataStruct deck[CARD_DECK_AMOUNT];
	CardSetDataStruct set[CARD_SET_AMOUNT];
	vector<CardDataStruct> card;
	uint32_t card_amount; // == CARD_AMOUNT
	uint16_t name_space_total;
	uint16_t name_space_used;
	
	void Load(fstream& ffbin, ConfigurationSet& config);
	void Write(fstream& ffbin, ConfigurationSet& config);
	void WritePPF(fstream& ffbin, ConfigurationSet& config);
	// Return 0 on success and 1 if names are too long
	int LoadHWS(fstream& ffhws, bool usetext);
	void WriteHWS(fstream& ffhws);
	void WriteSteamText(fstream& ffbin, bool onlymodified, bool asmes, SteamLanguage lang = GetSteamLanguage());
	// datatype: 0 for stats, 1 for decks, 2 for sets
	void WriteSteamData(fstream& ffbin, unsigned int datatype);
	void UpdateOffset();
};

#endif
