#ifndef _DATABASE_SPELL_H
#define _DATABASE_SPELL_H

#include "Database_Common.h"

#define HADES_STRING_SPELL_EFFECT_COUNT 128
#define HADES_STRING_SPELL_EFFECT_UNUSED_NAME "Unused %d"
#define HADES_STRING_SPELL_EFFECT_UNUSED_DESCRIPTION L"No effect."
extern vector<SortedChoiceItemWithHelp> HADES_STRING_SPELL_EFFECT;

#define HADES_STRING_STATUS_NONE	L"No Status"
extern vector<SortedChoiceItem> HADES_STRING_STATUS_PACK;
extern vector<wxString> HADES_STRING_STATUS;

#endif
