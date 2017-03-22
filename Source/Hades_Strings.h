#ifndef _HADES_STRINGS_H
#define _HADES_STRINGS_H

#include "Database_Common.h"

// GUI Strings
#define HADES_STRING_HADES_WORKSHOP L"Hades Workshop"
#define HADES_STRING_ERROR L"Error"
#define HADES_STRING_WARNING L"Warning"
#define HADES_STRING_SUCCESS L"Success"
#define HADES_STRING_EMPTY_PROCEDURE L"Nothing to do !"
#define HADES_STRING_ERROR_UNKNOWN L"An unknown error has occured..."
#define HADES_STRING_GENERIC_ADD L"Add"
#define HADES_STRING_GENERIC_REMOVE L"Remove"
#define HADES_STRING_GENERIC_COPY L"Copy"
#define HADES_STRING_GENERIC_PASTE L"Paste"
#define HADES_STRING_GENERIC_PROPERTIES L"Properties..."
#define HADES_STRING_GENERIC_SELECT_ALL L"Select All"
#define HADES_STRING_GENERIC_DESELECT_ALL L"Deselect All"
#define HADES_STRING_GENERIC_EXPORT_SEL L"Export Selection"
#define HADES_STRING_GENERIC_IMPORT_SEL L"Import Selection"
#define HADES_STRING_NYI L"Not yet implemented"
#define HADES_STRING_NO_STEAM L"This feature is disabled for the Steam version"
#define HADES_STRING_VOID L"_"

//-- Main Frame
#define HADES_STRING_OPEN_FRAME_NAME L"Open Final Fantasy IX binary file"
#define HADES_STRING_OPEN_FILTERS L"Binary file (*.bin;Steam)|*.bin;FF9_Launcher.exe|All files|*"
#define HADES_STRING_OPEN_STEAM_FILTER L"Steam Launcher|FF9_Launcher.exe"
#define HADES_STRING_OPEN_STEAM_DEFAULT "Steam Game"
#define HADES_STRING_OPEN_ERROR_LIMIT L"Can't open more than %u files."
#define HADES_STRING_OPEN_ERROR_NONEXISTENT L"The file '%s' doesn't exist!"
#define HADES_STRING_OPEN_ERROR_FAIL L"Cannot open file '%s'."
#define HADES_STRING_OPEN_ERROR_FAIL_NF L"Cannot open the specified file."
#define HADES_STRING_OPEN_ERROR_FAIL_FORMAT L"Unsupported format for '%s'."
#define HADES_STRING_OPEN_ERROR_CREATE L"Cannot create file '%s'."
#define HADES_STRING_OPEN_WARNING_UNKNOWN	L"Configuration file not found for this binary file.\n"\
											L"The program will perform a scan in order to open it.\n"\
											L"It should take about 1 minute.\n\n"\
											L"Do you want to scan the file ?"
#define HADES_STRING_OPEN_WARNING_VERSION	L"The file's configuration is outdated.\n"\
											L"Click OK to update it."
#define HADES_STRING_CONFIG_FIND_FAIL L"Can't scan '%s' as a Final Fantasy IX binary file."
#define HADES_STRING_CONFIG_FIND_FAIL_RNC L"'%s' has been compressed by a rnc patch and can't be read."
#define HADES_STRING_CONFIG_FIND_SUCCESS L"Successfully scanned !"
#define HADES_STRING_PREFERENCE_OPEN_FAIL L"Cannot open nor create a preference file."
#define HADES_STRING_SAVE_CONFIRM 	L"Be sure to have a backup of your binary file before saving !\n"\
									L"Exporting your changes as a .ppf file may be a safer solution."
#define HADES_STRING_PPF_SAVE_SUCCESS L"PPF file successfully created !"
#define HADES_STRING_STEAM_SAVE_DEFAULT L"HadesWorkshopMod\\"
#define HADES_STRING_STEAM_SAVE_SUCCESS L"Steam modded files successfully created !"
#define HADES_STRING_STEAM_SAVE_ERROR_OPENED_FILES L"You can't overwrite the currently opened Steam files"
#define HADES_STRING_STEAM_SAVE_ERROR_FAIL_READ L"Unable to open the file for reading"
#define HADES_STRING_STEAM_SAVE_ERROR_FAIL_WRITE L"Unable to create the file"
#define HADES_STRING_HWS_OPEN_SUCCESS L"Data successfully imported !"
#define HADES_STRING_HWS_OPEN_FAIL L"Cannot open '%s' as Final Fantasy IX save."
#define HADES_STRING_HWS_OPEN_WARNING L"Data has been imported with some errors :"
#define HADES_STRING_HWS_OPEN_WARNING_COMMON L"\n - "
#define HADES_STRING_HWS_OPEN_WARNING_SPELL_NAME L"The spell names are too long."
#define HADES_STRING_HWS_OPEN_WARNING_SPELL_HELP L"The spell helps are too long."
#define HADES_STRING_HWS_OPEN_WARNING_CMD_NAME L"The command names are too long."
#define HADES_STRING_HWS_OPEN_WARNING_CMD_HELP L"The command helps are too long."
#define HADES_STRING_HWS_OPEN_WARNING_ITEM_NAME L"The item names are too long."
#define HADES_STRING_HWS_OPEN_WARNING_ITEM_HELP L"The item helps are too long."
#define HADES_STRING_HWS_OPEN_WARNING_ITEM_HELP2 L"The item in-battle helps are too long."
#define HADES_STRING_HWS_OPEN_WARNING_KEY_ITEM_NAME L"The key item names are too long."
#define HADES_STRING_HWS_OPEN_WARNING_KEY_ITEM_HELP L"The key item helps are too long."
#define HADES_STRING_HWS_OPEN_WARNING_KEY_ITEM_DESC L"The key item descriptions are too long."
#define HADES_STRING_HWS_OPEN_WARNING_SUPPORT_NAME L"The supporting ability names are too long."
#define HADES_STRING_HWS_OPEN_WARNING_SUPPORT_HELP L"The supporting ability helps are too long."
#define HADES_STRING_HWS_OPEN_WARNING_STAT_DEFAULTNAME L"The characters' name are too long."
#define HADES_STRING_HWS_OPEN_WARNING_CARD_NAME L"The card names are too long."
#define HADES_STRING_HWS_OPEN_WARNING_ENEMY_SIZE L" battle datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_ENEMY_UNKNOWN L" unknown battle data types."
#define HADES_STRING_HWS_OPEN_WARNING_ENEMY_UNUSED L" battles are unused in this disc."
#define HADES_STRING_HWS_OPEN_WARNING_ENEMY_PRELOAD_UNAVAILABLE L" battle preloading datas ignored."
#define HADES_STRING_HWS_OPEN_WARNING_TEXT_SIZE L" text block datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_TEXT_UNKNOWN L" unknown text data types."
#define HADES_STRING_HWS_OPEN_WARNING_TEXT_UNUSED L" text blocks are unused in this disc."
#define HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_SIZE L" world map datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_UNKNOWN L" unknown world map data types."
#define HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_UNUSED L" world maps are unused in this disc."
#define HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_PARENTSIZE L" world map meta datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_WORLD_MAP_PRELOAD_UNAVAILABLE L" world map preloading datas ignored."
#define HADES_STRING_HWS_OPEN_WARNING_FIELD_SIZE L" field datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_FIELD_UNKNOWN L" unknown field data types."
#define HADES_STRING_HWS_OPEN_WARNING_FIELD_UNUSED L" fields are unused in this disc."
#define HADES_STRING_HWS_OPEN_WARNING_FIELD_PARENTSIZE L" field meta datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_FIELD_PRELOAD_UNAVAILABLE L" field preloading datas ignored."
#define HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_SIZE L" battle scene model datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_UNKNOWN L" unknown battle scene data types."
#define HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_UNUSED L" battle scenes are unused in this disc."
#define HADES_STRING_HWS_OPEN_WARNING_BATTLE_SCENE_UNAVAILABLE L"Battle scene data can't be imported."
#define HADES_STRING_HWS_OPEN_WARNING_SPELL_ANIMATION_SIZE L" spell animation datas are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_SPELL_ANIMATION_UNAVAILABLE L"Spell animation data can't be imported."
#define HADES_STRING_HWS_OPEN_WARNING_MENU_UI_TEXT_SIZE L" UI text blocks are too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_MENU_UI_TEXT_UNKNOWN L" unknown UI text blocks."
#define HADES_STRING_HWS_OPEN_WARNING_MENU_UI_TEXT_UNAVAILABLE L"Special text format is not compatible."
#define HADES_STRING_HWS_OPEN_WARNING_MIPS_SIZE L"The MIPS code is too heavy."
#define HADES_STRING_HWS_OPEN_WARNING_MIPS_UNAVAILABLE L"MIPS code can't be imported to the Steam version."
#define HADES_STRING_HWS_OPEN_WARNING_CIL_UNAVAILABLE L"CIL code can't be imported to the PSX version."
#define HADES_STRING_HWS_OPEN_WARNING_CIL_RAW_NOT_FOUND L" methods from the modifications are not found."
#define HADES_STRING_HWS_OPEN_WARNING_CIL_RAW_NOT_MATCHING L" methods from the modifications don't match with the ones in the DLL."
#define HADES_STRING_HWS_OPEN_WARNING_CIL_MACRO_UNKNOWN L" macros are unknown."
#define HADES_STRING_HWS_OPEN_WARNING_CIL_MACRO_FAIL L" macros can't be activated."
#define HADES_STRING_HWS_SAVE_SUCCESS L"Data successfully exported !"
#define HADES_STRING_TXT_SAVE_SUCCESS L"Text successfully exported !"
#define HADES_STRING_TEXTURE_SAVE_SUCCESS L"Texture successfully exported !"
#define HADES_STRING_FIELDTEXTURE_SAVE_SUCCESS L"Background successfully exported !"
#define HADES_STRING_SCENE_SAVE_SUCCESS L"Battle Scene model successfully exported !"

//-- HWS Frame
#define HADES_STRING_HWS_OPEN_TITLE L"Open hws..."
#define HADES_STRING_HWS_OPEN_MESSAGE L"Open..."
#define HADES_STRING_HWS_OPEN_CONFIRM L"Import"
#define HADES_STRING_HWS_SAVE_TITLE L"Save hws..."
#define HADES_STRING_HWS_SAVE_MESSAGE L"Save..."
#define HADES_STRING_HWS_SAVE_CONFIRM L"Save"

//-- Common
static wxString HADES_STRING_PRINTABLE_CHARTABLE_CHAR[] = {	L"0",L"1",L"2",L"3",L"4",L"5",L"6",L"7",L"8",L"9",L"+",L"-",L"=",L"*",L"%",L" ",
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
#define HADES_STRING_PRINTABLE_CHAR_CHARMAP	L"Á  À  Â  Ä  É  È  Ê  Ë  Í  Ì  Î  Ï  Ó  Ò  Ô  Ö  Ú  Ù  Û  Ü  á  à  â  ä  é  è  ê  ë  í  ì  î  ï  ó  ò  ô  ö  ú  ù  û  ü  Ç  Ñ  ç  ñ  Œ  ß\n"\
											L"“  ”  „  ‘  «  »  ¡  ¿  ×  ―  ∞  ∴  ∵  ∈  ∋  §  ♪  ♦  ♥  ★  ※  ☺  ♂  ♀  △  □  ↑  ↓  →  ←  ¶  \t"
static wxArrayString HADES_STRING_PRINTABLE_CHARTABLE(G_N_ELEMENTS(HADES_STRING_PRINTABLE_CHARTABLE_CHAR),HADES_STRING_PRINTABLE_CHARTABLE_CHAR);
#define HADES_STRING_TEXT_REACH_LIMIT	L"The text size has reached his limit.\n"\
										L"Maybe you can try to remove unused text\n"\
										L"to increase the limit here."
#define HADES_STRING_DATA_REACH_LIMIT	L"The data size has reached his limit."

//-- Spell Panel
#define HADES_STRING_SPELL_TARGET_INFO_BASE L"Recommanded compatibility\n with : "
static wxString HADES_STRING_SPELL_TARGET_INFO[5][4] = {
	{ L"One target",	L"Multiple targets",L"",				L"" },
	{ L"One ally",		L"Multiple allies",	L"",				L"" },
	{ L"One enemy",		L"Multiple enemies",L"",				L"" },
	{ L"",				L"Everyone",		L"",				L"" },
	{ L"Self targeting",L"",				L"",				L"" }
};

//-- Command Panel
#define HADES_STRING_COMMAND_ADD_SPELL_ADD	L"Add Spell"
#define HADES_STRING_COMMAND_ADD_SPELL_EDIT	L"Set Spell"
#define HADES_STRING_COMMAND_REACH_LIMIT	L"You can't add spells to the list anymore.\n"\
											L"Maybe you can try to remove spells\n"\
											L"elsewhere or link commands together\n"\
											L"to increase the limit here."
#define HADES_STRING_COMMAND_CANT_CHANGE_LINK	L"Can only set link with another command verifying :\n"\
												L" - it uses the spell panel,\n"\
												L" - it is not linked to any command yet."

//-- Stat Panel
#define HADES_STRING_STAT_LEVEL	L"Level Progression"
static wstring HADES_STRING_CHARACTER_DEFAULT_NAME[] = {
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

//-- Party Special
static wxString HADES_STRING_PARTY_SPECIAL_DATA[] = {
	L"Ability Availabilities"
};

//-- Enemy Panel
#define HADES_STRING_GROUPEDIT_ERROR_TEXT L"A battle data is too short-spaced : failed to change the text."

//-- Shop Panel
#define HADES_STRING_SHOP_REACH_LIMIT			L"Can't add more than 32 items to a shop."
#define HADES_STRING_SYNTH_REACH_LIMIT			"Synthesis Shops can't synth more than %d items.\n"
#define HADES_STRING_SYNTH_REACH_LIMIT_TOKEN	"\n - %s has %d items."
static wxString HADES_STRING_SYNTH_NAME[8] = {
	L"Lindblum (Disc 1/2)",			L"Treno (Disc 2)",
	L"Lindblum (Disc 2)",			L"Black Mage Village (Disc 2/3)",
	L"Alex. Lind. Treno (Disc 3)",	L"Daguerreo (Disc 3/4)",
	L"Black Mage Village (Disc 4)",	L"Hades (Disc 4)"
};

//-- Scripts
#define HADES_STRING_ON_OFF_BUTTON				L"Check"
#define HADES_STRING_NULL_CHARACTER_SLOT		L"Null Character"
#define HADES_STRING_SCRIPT_NEW_ENTRY	 		L"New Entry"
#define HADES_STRING_ENTRY_ARG_LOST				"%u script lines used an entry that was deleted"
#define HADES_STRING_SCRIPT_NO_DELETE	 		L"Can't delete the Main_Init function"
#define HADES_STRING_MISSING_SPACE				L"Not enough space available"
#define HADES_STRING_SCRIPT_SHOULDPARSE			L"Are you sure ?\n"\
												L"Some function(s) have been modified but are not parsed."
#define HADES_STRING_LOGERROR_SPACE				" - Not enough space : data is %d bytes too heavy\n"
#define HADES_STRING_SCRIPT_RETURNMISSING		" - Line %u : Missing a 'return' or a 'loop'\n"
#define HADES_STRING_SCRIPT_BLOCKMISSING		" - Line %u : Missing %u block delimiter(s)\n"
#define HADES_STRING_SCRIPT_BLOCKTOOMANY		" - Line %u : Using a block delimiter out of any block\n"
#define HADES_STRING_SCRIPT_EXPECT				" - Line %u : Expected '%s'\n"
#define HADES_STRING_SCRIPT_UNEXPECT			" - Line %u : Unexpected '%s'\n"
#define HADES_STRING_SCRIPT_NUMBER				" - Line %u : Expected a number\n"
#define HADES_STRING_SCRIPT_ARGAMOUNT			" - Line %u : Opcode '%s' takes %u arguments ; got %u\n"
#define HADES_STRING_SCRIPT_NOVARARG			" - Line %u : Opcode '%s' accepts numerical arguments only\n"
#define HADES_STRING_SCRIPT_UNKNOWN				" - Line %u : Unknown word '%s'\n"
#define HADES_STRING_SCRIPT_NOTSWITCH			" - Line %u : Using the keyword '%s' out of a switch\n"
#define HADES_STRING_SCRIPT_NOBREAK				" - Line %u : Using the keyword '%s' out of a breakable block\n"
#define HADES_STRING_SCRIPT_CASE				" - Line %u : Cases amount doesn't match\n"
#define HADES_STRING_SCRIPT_CASE_TWICE			" - Line %u : Case '%s' handled twice\n"
#define HADES_STRING_SCRIPT_CASE_RANGE			" - Line %u : Case is not in range\n"
#define HADES_STRING_SCRIPT_LOOP				" - Line %u : Looping functions can only be made in top-level blocks\n"
#define HADES_STRING_SCRIPT_VARARG_MAIN			" - Line %u : %s"
#define HADES_STRING_SCRIPT_VARARG_EMPTY		"Empty argument\n"
#define HADES_STRING_SCRIPT_VARARG_TOO_PAR		"Too many ')' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_MISS_PAR		"Missing ')' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_MISS_COMMA	"Missing an argument in variable code\n"
#define HADES_STRING_SCRIPT_VARARG_TOO_COMMA	"Too many arguments in variable code\n"
#define HADES_STRING_SCRIPT_VARARG_OPEN_PAR		"Expected '(' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_GETENTRY		"Can only use numerical numbers inside a GetEntry call\n"
#define HADES_STRING_SCRIPT_VARARG_ARRAY		"Can only use numerical numbers or field names inside brackets\n"
#define HADES_STRING_SCRIPT_VARARG_BRACKETS		"Expected ']' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_OPVAMISMATCH	"Amount of operations and values don't match\n"
#define HADES_STRING_SCRIPT_VARARG_UNKNOWN		"Unrecognized '%s' in argument\n"
#define HADES_STRING_SCRIPT_IGNORE				" - Line %u : Ignored trailing '%s'\n"
#define HADES_STRING_SCRIPT_IGNORE_POSTRET		" - Line %u : Unexpected code after the end of the function\n"
#define HADES_STRING_LSCRIPT_ALLOC_NUMBER		L" - Local Variable : Expected a number after 'allocate'\n"
#define HADES_STRING_LSCRIPT_MISS_NAME			L" - Local Variable : Expected a name for local variable\n"
#define HADES_STRING_LSCRIPT_UNEXPECTED			" - Local Variable : Unexpected '%s'\n"
#define HADES_STRING_LSCRIPT_MISS_ALLOC			" - Local Variable : Need to allocate '%u'\n"

//-- CIL Editor
#define HADES_STRING_CILSCRIPT_NOTINST			" - IL_%.4X : '%s' is not a CIL instruction ; line ignored\n"
#define HADES_STRING_CILSCRIPT_NOTARG			" - IL_%.4X : Expected an argument instead of '%s'\n"
#define HADES_STRING_CILSCRIPT_WRONGTYPECHECK	" - IL_%.4X : Expected a list of check types, '%s', '%s' or '%s'\n"
#define HADES_STRING_CILSCRIPT_WRONGILPOS		" - IL_%.4X : '%s' is not a valid IL position\n"
#define HADES_STRING_CILSCRIPT_RANGEILPOS		" - IL_%.4X : '%s' is out of range for tbis instruction's short form\n"

//-- File Batching
#define HADES_STRING_BATCH_NOTHING				" - Nothing done\n"
#define HADES_STRING_BATCH_WRONG_FILETYPE		" - Line %u : Expected a %s filetype\n"
#define HADES_STRING_BATCH_FILETYPE_TWICE		" - Line %u : Can't define typefile twice\n"
#define HADES_STRING_BATCH_FILETYPE_NOTFIRST	" - Line %u : First command must be the filetype\n"
#define HADES_STRING_BATCH_MISSING_INTEGER		" - Line %u : '%s' takes an integral argument\n"
#define HADES_STRING_BATCH_TOO_MANY				" - Line %u : Too many %s declarations\n"
#define HADES_STRING_BATCH_TEXT_WRONG_ID		" - Line %u : Text Block %u doesn't have a text n°%u\n"
#define HADES_STRING_BATCH_TEXT_REDEFINITION	" - Line %u : Redefinition of text n°%u\n"
#define HADES_STRING_BATCH_TEXT_UNUSED			" - Text Block %u unused in this disc\n"
#define HADES_STRING_BATCH_TEXT_SIZE			" - Text Block %u capacity is %u bytes too short\n"
#define HADES_STRING_BATCH_TEXT_MISSMATCH_CODE	" - Text Block %u, Text %u : Expected %u opcode characters, got %u\n"

//-- World Maps
#define HADES_STRING_WORLD_BATTLE_NAME	"Battle Spot %d"

//-- Card Panel
#define HADES_STRING_CARD_SHARED	L"Common Datas"
#define HADES_STRING_CARD_NOCARD	L"No Card"

//-- TIM Panels
#define HADES_STRING_TIM_REACH_LIMIT	L"Can't add more texture data."

//-- Image Map Panels
#define HADES_STRING_IMAGE_MAP_REACH_LIMIT	L"Can't add more files to the preloading list."
static wxString HADES_STRING_IMAGE_MAP_OBJECT[] = {
	L"Battle ",
	L"Battle Scene ",
	L"Music ",
	L"Audio ",
	L"Field ",
	L"World Map ",
	L"Model "
};

//-- Spell Animations
#define HADES_STRING_SPELL_ANIMATION_SEQUENCE_FULL	L"Can't add more sequence code to the animation."

//-- Special Texts
static wxString HADES_STRING_SPECIAL_TEXT_BLOCK[] = {
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

static wxString HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[] = {
	L"Battle Infos",
	L"Battle Scan",
	L"Battle Spell Naming",
	L"Chocobo Menu",
	L"Card Menu",
	L"Tetra Master Game",
	L"Localization"
};

//-- Mips & CIL
#define HADES_STRING_MIPS_FULL_CODE		L"Full Code"
#define HADES_STRING_MIPS_OOB			"RAM position is out of bounds [0x%X, 0x%X]"
#define HADES_STRING_CIL_NO_METHOD		L"Can't find a method body\n"\
										L"Method is external"
#define HADES_STRING_CIL_PROTECTED		L"This method contains some database specially handled by Hades Workshop. You can't modify it."
#define HADES_STRING_CIL_APPLY_MACRO	L"Apply Macro"
#define HADES_STRING_CIL_UNAPPLY_MACRO	L"Unapply Macro"

//-- Tools
#define HADES_STRING_UNKNOWN_FIELD L"UNKNOWN_FIELD"
#define HADES_STRING_INVALID_FF9LAUNCHER L"Select a valid FF9_Launcher.exe"
#define HADES_STRING_INVALID_IMAGE_NAME_FORMAT L"The file name format must contain three '%%'"
#define HADES_STRING_IMPORT_BACKGROUND_RESULT	"%d background importations succeeded\n"\
												"%d background importations failed"
#define HADES_STRING_BACKGROUNDIMPORT_MISSING_LAYERS	" - Missing %d layers for the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_CREATE		" - Failed to convert the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_DIMENSIONS	" - The dimensions of the layers don't match for the field %d (%s)\n"
#define HADES_STRING_UNITYVIEWER_GAME_OPEN	L"This game folder is already opened in Hades Workshop's main module. Updating the assets is not possible in this situation."
#define HADES_STRING_UNITYVIEWER_IMPORT_ERROR_FORMAT	"Default format could not be retrieved for '%s'"
#define HADES_STRING_UNITYVIEWER_MISSING_AKB	"Expected an AKB header in '%s' but none was found"
#define HADES_STRING_UNITYVIEWER_MISSING_OGG	"Expected an OGG format for '%s'"
#define HADES_STRING_UNITYVIEWER_IMPORT_SUCCESS	"Successfully imported %d asset(s)."
#define HADES_STRING_UNITYVIEWER_IMPORT_ERROR	"Successfully imported %d asset(s).\n"\
												"Failed to import %d asset(s)."
#define HADES_STRING_UNITYVIEWER_IMPORT_NO_REPLACE	"\nArchive not updated: saved the output as tmp file."



//-- Others
#define HADES_STRING_FONT_ERROR L"Unsupported font."

#endif
