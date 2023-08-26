#include "Hades_Strings.h"

vector<wxString> HADES_STRING_STEAM_LANGUAGE_SHORT_NAME = {
	L"us",
	L"en",
	L"jp",
	L"de",
	L"fr",
	L"it",
	L"sp"
};

vector<wxString> HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX = {
	L"us",
	L"uk",
	L"jp",
	L"gr",
	L"fr",
	L"it",
	L"es"
};

vector<wxString> HADES_STRING_STEAM_LANGUAGE_LONG_NAME = {
	L"English (US)",
	L"English (UK)",
	L"Japanese",
	L"German",
	L"French",
	L"Italian",
	L"Spanish"
};

vector<wxString> HADES_STRING_PRINTABLE_CHARTABLE = { L"0",L"1",L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"+",L"-",L"=",L"*",L"%",L" ",
													  L"A",L"B",L"C",L"D",L"E",L"F",L"G",L"H",L"I",L"J",L"K",L"L",L"M",L"N",L"O",L"P",
													  L"Q",L"R",L"S",L"T",L"U",L"V",L"W",L"X",L"Y",L"Z",L"(",L"!",L"?",L"“",L":",L".",
													  L"a",L"b",L"c",L"d",L"e",L"f",L"g",L"h",L"i",L"j",L"k",L"l",L"m",L"n",L"o",L"p",
													  L"q",L"r",L"s",L"t",L"u",L"v",L"w",L"x",L"y",L"z",L")",L",",L"/",L"+",L"~",L"&",
													  L"Á",L"À",L"Â",L"Ä",L"É",L"È",L"Ê",L"Ë",L"Í",L"Ì",L"Î",L"Ï",L"Ó",L"Ò",L"Ô",L"Ö",
													  L"Ú",L"Ù",L"Û",L"Ü",L"á",L"à",L"â",L"ä",L"é",L"è",L"ê",L"ë",L"í",L"ì",L"î",L"ï",
													  L"ó",L"ò",L"ô",L"ö",L"ú",L"ù",L"û",L"ü",L"Ç",L"Ñ",L"ç",L"ñ",L"Œ",L"ß",L"\'",L"”",
													  L"_",L"}",L"{",L"∴",L"∵",L"♪",L"→",L"∈",L"×",L"♦",L"§",L"<",L">",L"←",L"∋",L"↑",
													  L"△",L"□",L"∞",L"♥",
													  L"«",L"»",L"↓",L"―",L"°",L"★",L"♂",L"♀",L"☺",L"„",L"‘",L"#",L"※",L";",
													  L"¡",L"¿",
													  L" ",L"\n",L"\t",L"µ",L" ",L" ",L"¶",L"\0" };

vector<wxString> HADES_STRING_ELEMENT_NAME = {
	L"Fire",
	L"Ice",
	L"Thunder",
	L"Earth",
	L"Water",
	L"Wind",
	L"Holy",
	L"Shadow"
};

vector<wxString> HADES_STRING_STATUS_NAME = {
	L"Petrify",
	L"Venom",
	L"Virus",
	L"Silence",
	L"Darkness",
	L"Trouble",
	L"Zombie",
	L"Easy Kill Proof",
	L"KO",
	L"Low HP",
	L"Confuse",
	L"Berserk",
	L"Stop",
	L"Auto-Life",
	L"Trance",
	L"Defend",
	L"Poison",
	L"Sleep",
	L"Regen",
	L"Haste",
	L"Slow",
	L"Float",
	L"Shell",
	L"Protect",
	L"Heat",
	L"Freeze",
	L"Vanish",
	L"Doom",
	L"Mini",
	L"Reflect",
	L"Jump",
	L"Gradual Petrify"
};

vector<vector<wxString>> HADES_STRING_SPELL_TARGET_INFO = {
	{ L"One target",		L"Multiple targets"	},
	{ L"One ally",			L"Multiple allies"	},
	{ L"One enemy",			L"Multiple enemies"	},
	{ L"",					L"Everyone"			},
	{ L"Self targeting",	L""					}
};

vector<wxString> HADES_STRING_CHARACTER_DEFAULT_NAME = {
	L"Zidane",
	L"Vivi",
	L"Dagger",
	L"Steiner",
	L"Freya",
	L"Quina",
	L"Eiko",
	L"Amarant",
	L"Cinna",
	L"Marcus",
	L"Blank",
	L"Beatrix"
};

vector<wxString> HADES_STRING_PARTY_SPECIAL_DATA = {
	L"Ability Availabilities"
};

vector<wxString> HADES_STRING_SYNTH_NAME = {
	L"Lindblum (Disc 1/2)",			L"Treno (Disc 2)",
	L"Lindblum (Disc 2)",			L"Black Mage Village (Disc 2/3)",
	L"Alex. Lind. Treno (Disc 3)",	L"Daguerreo (Disc 3/4)",
	L"Black Mage Village (Disc 4)",	L"Hades (Disc 4)"
};

vector<wxString> HADES_STRING_IMAGE_MAP_OBJECT = {
	L"Battle ",
	L"Battle Scene ",
	L"Music ",
	L"Audio ",
	L"Field ",
	L"World Map ",
	L"Model "
};

vector<wxString> HADES_STRING_SPECIAL_TEXT_BLOCK = {
	L"Game Menu",
	L"Save (Change Disc)",
	L"Game Menu Infos",
	L"Battle Commands",
	L"Battle Infos",
	L"Battle Scan",
	L"Battle Spell Naming",
	L"Unknown Menu Infos",
	L"Tetra Master Game",
	L"Melodies Of Life",
	L"Chocobo Menu",
	L"Ability Menu",
	L"Equip Menu",
	L"Config Menu",
	L"Card Menu",
	L"Item Menu",
	L"Status Menu",
	L"Save/Load Menu",
	L"Naming Character",
	L"Team Choice",
	L"Shop (Shared)",
	L"Shop (Buy)",
	L"Shop (Synthesis)",
	L"Shop (Sell)",
	L"Battle Rewards"
};

vector<wxString> HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM = {
	L"Battle Infos",
	L"Battle Scan",
	L"Battle Spell Naming",
	L"Chocobo Menu",
	L"Card Menu",
	L"Tetra Master Game",
	L"Localization"
};
