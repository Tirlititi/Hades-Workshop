#ifndef _DATABASE_TEXT_H
#define _DATABASE_TEXT_H

#include "Database_Common.h"

struct SortedChoiceItemTextOpcode {
	unsigned int id;
	wstring label;
	wstring converted;
	wstring help;
	int length;
	int type;
	wstring* arg_help;
};

struct SortedChoiceItemTextSteamOpcode {
	wstring id;
	int arg_amount;
	wstring help;
	wstring converted;
};

extern vector<SortedChoiceItem> HADES_STRING_TEXT_BLOCK_NAME;

// Should always be sorted by id
#define TEXT_OPCODE_FORMAT_TYPE 0x80 // 0x80 and next ones
extern vector<SortedChoiceItemTextOpcode> HADES_STRING_TEXT_OPCODE;

#define TEXT_STEAM_OPCODE_CLOSING_TAG 52
extern vector<SortedChoiceItemTextSteamOpcode> HADES_STRING_TEXT_STEAM_OPCODE;



#endif
