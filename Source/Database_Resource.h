#ifndef _DATABASE_RESOURCE_H
#define _DATABASE_RESOURCE_H

#include "Database_Common.h"

#define DATABASE_MODEL_CATEGORY_NONE			0
#define DATABASE_MODEL_CATEGORY_ACCESSORY		1
#define DATABASE_MODEL_CATEGORY_MAIN			2
#define DATABASE_MODEL_CATEGORY_ENEMY			3
#define DATABASE_MODEL_CATEGORY_NPC				4
#define DATABASE_MODEL_CATEGORY_SUB				5
#define DATABASE_MODEL_CATEGORY_WEAPON			6
#define DATABASE_MODEL_CATEGORY_LIST_ALL		0x3F
#define DATABASE_MODEL_CATEGORY_TO_LIST(CAT)	(1 << (CAT-1))

struct SortedChoiceSteamItem {
	unsigned int id;
	wxString steamid;
	wxString label;
};

struct ModelDatabaseItem {
	uint32_t id;
	uint8_t category;
	uint8_t module;
	uint8_t version; // Not sure what it is
	wxString steamid;
	wxString label;
};

//-- Battle Scenes
extern vector<SortedChoiceSteamItem> HADES_STRING_BATTLE_SCENE_NAME;

//-- World Maps
#define WORLD_BLOCK_VALID_AMOUNT 13
extern vector<SortedChoiceSteamItem> HADES_STRING_WORLD_BLOCK_NAME;

//-- Shared Audio
#define AUDIO_TYPE_MUSIC	0
#define AUDIO_TYPE_SONG		1
#define AUDIO_TYPE_SFX		2
#define AUDIO_TYPE_MOVIE	3
extern vector<wxString> HADES_STRING_AUDIO_TYPE;
extern vector<SortedChoiceItem> HADES_STRING_MUSIC_NAME; // Music
extern vector<SortedChoiceItem> HADES_STRING_AUDIO_NAME; // Audio (SFX plus others...)

//-- Model
extern vector<wxString> DATABASE_MODEL_TYPE;
extern vector<ModelDatabaseItem> HADES_STRING_MODEL_NAME;

#endif
