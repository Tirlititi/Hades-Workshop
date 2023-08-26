#ifndef _DATABASE_ANIMATION_H
#define _DATABASE_ANIMATION_H

#include "Database_Common.h"
#include "Database_Resource.h"

struct AnimationDatabaseItem {
	uint32_t model_id;
	uint32_t id;
	wxString steam_id;
	wxString label;
};

struct AnimationDatabaseDoubledItem {
	uint32_t model_id;
	uint32_t id;
	int32_t id2;
	wxString steam_id;
	wxString label;
};

extern vector<AnimationDatabaseItem> DATABASE_ANIMATION_SINGLEID;
extern vector<AnimationDatabaseDoubledItem> DATABASE_ANIMATION_DOUBLEID;

namespace AnimationDatabase {

	int32_t GetIndex(uint32_t animid, unsigned int categories = DATABASE_MODEL_CATEGORY_LIST_ALL);
	int32_t GetIndexFromSteam(wxString steamid, unsigned int categories = DATABASE_MODEL_CATEGORY_LIST_ALL);
	int32_t GetIndexFromModelIndex(uint32_t modelindex); // Warning: modelindex is index of HADES_STRING_MODEL_NAME, not model id
	int32_t GetIndexFromModelId(uint32_t modelid);
	int32_t GetId2(int32_t index);
	uint32_t GetId(int32_t index);
	uint32_t GetModelId(int32_t index);
	wxString GetSteamId(int32_t index);
	wxString GetDescription(int32_t index);

}

#endif
