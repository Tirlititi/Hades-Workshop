#ifndef _DATABASE_ANIMATION_H
#define _DATABASE_ANIMATION_H

#include "Database_Common.h"
#include "Database_Resource.h"

struct AnimationDatabaseItem {
	uint32_t model_id;
	uint32_t id;
	wxString steamid;
	wxString label;
};

static AnimationDatabaseItem DATABASE_ANIMATION[] = {
	{ 0,	72,		L"ANH_MON_B3_147_000",		L"Idle" },
	{ 0,	327,	L"ANH_MON_B3_147_001",		L"Idle Alternate" },
	{ 0,	60,		L"ANH_MON_B3_147_003",		L"Hit" },
	{ 0,	339,	L"ANH_MON_B3_147_004",		L"Hit Alternate" },
	{ 0,	343,	L"ANH_MON_B3_147_005",		L"Open" },
	{ 0,	331,	L"ANH_MON_B3_147_006",		L"Close" },
	{ 0,	345,	L"ANH_MON_B3_147_009",		L"Land" },
	{ 0,	74,		L"ANH_MON_B3_147_010",		L"Demon’s Claw" },
	{ 0,	68,		L"ANH_MON_B3_147_020",		L"Cast Init" },
	{ 0,	318,	L"ANH_MON_B3_147_021",		L"Cast Loop" },
	{ 0,	36,		L"ANH_MON_B3_147_022",		L"Cast Resume" },
	{ 0,	70,		L"ANH_MON_B3_147_030",		L"Cast Alternate Init" },
	{ 0,	322,	L"ANH_MON_B3_147_031",		L"Cast Alternate Loop" },
	{ 0,	39,		L"ANH_MON_B3_147_032",		L"Cast Alternate Resume" },
	{ 0,	63,		L"ANH_MON_B3_147_040",		L"Spin 1" },
	{ 0,	309,	L"ANH_MON_B3_147_041",		L"Spin 2" },
	{ 0,	32,		L"ANH_MON_B3_147_042",		L"Spin 3" },
	{ 0,	46,		L"ANH_MON_B3_147_043",		L"Spin 4" },
	{ 0,	335,	L"ANH_MON_B3_147_044",		L"Spin 5" },
	{ 0,	66,		L"ANH_MON_B3_147_050",		L"Death Init" },
	{ 0,	311,	L"ANH_MON_B3_147_051",		L"Death Init Alternate" },
	{ 0,	34,		L"ANH_MON_B3_147_052",		L"Death 2" },
	{ 0,	48,		L"ANH_MON_B3_147_053",		L"Death 3" },
	{ 0,	337,	L"ANH_MON_B3_147_054",		L"Death 4" },
	{ 0,	342,	L"ANH_MON_B3_147_055",		L"Death 5" },
	{ 0,	330,	L"ANH_MON_B3_147_056",		L"Death 6" },
	{ 0,	334,	L"ANH_MON_B3_147_057",		L"Unused" },
	{ 0,	14729,	L"ANH_MON_B3_147_B",		L"Dummy B" },
	{ 0,	307,	L"ANH_MON_B3_147_ILLUST",	L"Dummy Illust" },
	{ 0,	14731,	L"ANH_MON_B3_147_P",		L"Dummy P" },
	{ 0,	43,		L"ANH_MON_B3_147_TEST",		L"Test" },

};

#endif
