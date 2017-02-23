#ifndef _HADES_STRINGS_H
#define _HADES_STRINGS_H

#include <wx/arrstr.h>
#include "Spells.h"

#define G_N_ELEMENTS(arr) ((sizeof(arr))/(sizeof(arr[0])))

struct SortedChoiceItem {
	unsigned int id;
	wxString label;
};
struct SortedChoiceItemWithHelp {
	unsigned int id;
	wxString label;
	wxString help;
};
struct SortedChoiceItemTextOpcode {
	unsigned int id;
	wstring label;
	wstring converted;
	wstring help;
	int length;
	int type;
	wstring* arg_help;
};
struct SortedChoiceItemScriptOpcode {
	unsigned int id;
	wstring label;
	wstring help;
	bool use_vararg;
	int arg_amount;
	uint8_t* arg_length;
	wstring* arg_help;
	uint8_t* arg_type;
	unsigned int jump_pos;
};
struct SortedChoiceItemModel {
	unsigned int id;
	wxString label;
	Spell_Target_Type target_type;
	Spell_Target_Amount target_amount;
	uint8_t bug_rate;
};

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
#define HADES_STRING_NYI L"Not yet implemented"
#define HADES_STRING_NO_STEAM L"This feature is disabled for the Steam version"
#define HADES_STRING_VOID L"_"
//-- MainFrame
#define HADES_STRING_OPEN_FRAME_NAME L"Open Final Fantasy IX binary file"
#define HADES_STRING_OPEN_FILTERS L"Binary file (*.bin;Steam)|*.bin;FF9_Launcher.exe|All files|*"
#define HADES_STRING_OPEN_STEAM_FILTER L"Steam Launcher|FF9_Launcher.exe"
#define HADES_STRING_OPEN_STEAM_DEFAULT "Steam Game"
#define HADES_STRING_OPEN_ERROR_LIMIT L"Can't open more than %u files."
#define HADES_STRING_OPEN_ERROR_NONEXISTENT L"The file '%s' doesn't exist!"
#define HADES_STRING_OPEN_ERROR_FAIL L"Cannot open file '%s'."
#define HADES_STRING_OPEN_ERROR_FAIL_NF L"Cannot open the specified file."
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
#define HADES_STRING_INVALID_IMAGE_DIMENSIONS L"The dimensions of the layers don't match"
#define HADES_STRING_IMPORT_BACKGROUND_RESULT	"%d background importations succeeded\n"\
												"%d background importations failed"
#define HADES_STRING_BACKGROUNDIMPORT_MISSING_LAYERS	" - Missing %d layers for the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_CREATE		" - Failed to convert the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_DIMENSIONS	" - The dimensions of the layers don't match for the field %d (%s)\n"
#define HADES_STRING_UNITYVIEWER_GAME_OPEN	L"This game folder is already opened in Hades Workshop's main module. Updating the assets is not possible in this situation."

//-- Others
#define HADES_STRING_FONT_ERROR L"Unsupported font."

// Data Strings
//-- Spells
static SortedChoiceItemWithHelp HADES_STRING_SPELL_EFFECT[] = {
	{ 0, L"No Effect", L"No effect." },
	{ 1, L"Attack Standard", L"Standard weapons attack.\nDamage = (Attack - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Healer, Berserk, Trance, Mini, Back Row and Elemental Boost (Weapon)\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity (Weapon)\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble.\nAdd weapon's status with Add Status (use weapon's status accuracy)." },
	{ 2, L"Attack Thief Sword", L"Thief swords and Knight swords attack.\nDamage = (Attack - Defence) * (Random[Strength, Strength + (Level + Strength) / 8] + Spirit / 2)\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Healer, Berserk, Trance, Mini, Back Row and Elemental Boost (Weapon)\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity (Weapon)\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble.\nAdd weapon's status with Add Status (use weapon's status accuracy)." },
	{ 3, L"Attack Rackets", L"Rackets attack.\nDamage = (Attack - Defence) * (Random[Strength, Strength + (Level + Strength) / 8] + Speed / 2)\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Healer, Berserk, Trance, Mini, Back Row and Elemental Boost (Weapon)\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity (Weapon)\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble.\nAdd weapon's status with Add Status (use weapon's status accuracy)." },
	{ 4, L"Attack Scrolls", L"Magical weapons attack.\nDamage = (Attack - Defence) * (Random[Strength, Strength + (Level + Strength) / 8] + Magic / 2)\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Healer, Berserk, Trance, Mini, Back Row and Elemental Boost (Weapon)\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity (Weapon)\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble.\nAdd weapon's status with Add Status (use weapon's status accuracy)." },
	{ 5, L"Attack Forks", L"Forks and Hammers attack.\nDamage = (Attack - Defence) * Random[0, Random[Strength, Strength + (Level + Strength) / 8]]\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Healer, Berserk, Trance, Mini, Back Row and Elemental Boost (Weapon)\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity (Weapon)\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble.\nAdd weapon's status with Add Status (use weapon's status accuracy)." },
	{ 6, L"Attack Blood Sword", L"Blood Sword attack. Heal the caster by the damage amount, but reverse the process on Zombie.\nDamage = (Attack - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Berserk, Trance, Mini and Back Row\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row and Gamble Defence\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify (miss), Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble." },
	{ 7, L"Attack Save The Queen", L"Save The Queen attack.\nDamage = (Attack + Level - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\nHit Rate = 100\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Killer Abilities, MP Attack, Healer, Berserk, Trance, Mini, Back Row and Elemental Boost (Weapon)\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity (Weapon)\n Other : Ipsen's Curse\nHit Rate Modifiers\n Caster : Accuracy+, Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble.\nAdd weapon's status with Add Status (use weapon's status accuracy)." },
	{ 8, L"Enemy Attack", L"Physical attack used by enemies that can also add status.\nDamage = (Attack - Defence) * Random[Strength, Strength + (Level + Strength) / 4]\nHit Rate = 100\nStatus Hit Rate = SpellAccuracy\nCritical Rate = Spirit / 8\n\nDamage Modifiers\n Caster : Healer, Berserk, Trance, Mini, Back Row and Elemental Boost\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity\nHit Rate Modifiers\n Caster : Confuse, Blind, Trance and Vanish\n Target : Distract, Defend, Vanish, Petrify, Venom, Virus, Blind, Confuse, Stop, Sleep and Freeze\nUse physical evasion.\nKill Frozen targets.\nSpread damage if the target is under Trouble." },
	{ 9, L"Magic Attack", L"Basic magic attack.\nDamage = (Power - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8]\nStatus Hit Rate = SpellAccuracy\n\nDamage Modifiers\n Caster : Multi-targeting, Mini and Elemental Boost\n Target : Shell and Elemental Affinity\nMiss on flying targets if the spell is Earth-elemental." },
	{ 10, L"Magic Heal", L"Basic heal (usable out of battles), damaging Zombie targets.\nHeal = Power * Random[Magic, Magic + (Level + Magic) / 8]\n\nHeal Modifiers : Multi-targeting, Concentrate, Petrify (miss) and Mini" },
	{ 11, L"Bad Status", L"Add status if the target is not immune.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifiers : Multi-targeting and Shell\nUse magical evasion." },
	{ 12, L"Cure Status", L"Cure status (usable out of battles)." },
	{ 13, L"Life", L"Cure status and heal a player character (usable out of battles). On Zombie targets, the spell misses if the target is also KO and kill it instead else.\nHeal = (Spirit + Power) * MaxHP / 100\nHit Rate (Zombie) = Accuracy + Magic / 4 + Level - TargetLevel\n\nHeal Modifiers : Concentrate\nUse magical evasion." },
	{ 14, L"Death", L"Add status. On Zombie, the target is fully HP healed instead.\nHit Rate (Add Status) = Accuracy + Magic / 4 + Level - TargetLevel\n\nUse magical evasion.\nNever hit with the Easy Kill guard." },
	{ 15, L"Osmose", L"Magic attack stealing target's MP, but reverse the effect on Zombie.\nDamage MP = (Power - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8] / 4\n\nDamage MP Modifiers : Mini (Caster), Petrify (miss) and Shell" },
	{ 16, L"Drain", L"Magic attack stealing target's HP, but reverse the effect on Zombie.\nDamage = (Power - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8]\n\nDamage Modifiers : Mini (Caster), Petrify (miss) and Shell" },
	{ 17, L"Gravity", L"Magic attack depending on target's max HP.\nDamage = MaxHP * Power / 100\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nDamage Modifiers : Elemental Affinity and Boost\nHit Rate Modifiers : Multi-targeting, Shell and Easy Kill.\nUse magical evasion.\nMiss on flying targets if the spell is Earth-elemental." },
	{ 18, L"Random Magic", L"Magic attack with a wide range of damage that can also add status.\nExcept for a few spell slots, it never misses (see btl_calc::DecideMeteor).\nDamage = Power * Random[1, Level + Magic - 1]\nStatus Hit Rate = SpellAccuracy\n\nDamage Modifiers : Multi-targeting, Mini (Caster), Shell, Elemental Affinity and Boost\nMiss on flying targets if the spell is Earth-elemental." },
	{ 19, L"Physical Strike", L"Physical attack that can also add status.\nDamage = (Attaque * Power / 10 - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\nStatus Hit Rate = SpellAccuracy\n\nDamage Modifiers\n Caster : Berserk, Trance, Mini and Elemental Boost (Spell)\n Target : Defend, Protect, Sleep, Mini and Elemental Affinity (Weapon)" },
	{ 20, L"Magic Weapon", L"Magic attack using strength that can also add status.\nDamage = (Attaque * Power / 10 - MagicDefence) * Random[Strength, Strength + (Level + Strength) / 8]\nStatus Hit Rate = SpellAccuracy\n\nDamage Modifiers : Mini (half), Shell, Elemental Affinity and Boost (Spell)\nMiss on flying targets if the spell is Earth-elemental." },
	{ 21, L"Goblin Punch", L"Magic attack improved if target and caster are same level.\nBase Damage = (Power - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8]\nImproved Damage = (Power + Level) * Random[Magic, Magic + (Level + Magic) / 8]\n\nDamage Modifiers : Mini (half) and Shell" },
	{ 22, L"LV? Death", L"Bring HP to « Power » if target's level is a multiple of « Accuracy ».\nPass through any status immunity but miss if the target is under Petrify." },
	{ 23, L"LV? Attack", L"Magic attack if target's level is a multiple of « Accuracy ».\nDamage = (Power - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8]\n\nDamage Modifiers : Mini (Caster), Shell, Petrify (miss), Elemental Affinity and Boost" },
	{ 24, L"LV? Defless", L"Reduce both defence and magic defence if target's level is a multiple of « Accuracy » and not under Petrify.\nNew Defences = Random[0, Old Defences - 1]" },
	{ 25, L"Roulette", L"Bring HP to « Power » but fail if the target is under Easy Kill or Petrify.\nThe base Roulette spell slot randomizes the target but the others have a determinist target (see btl_cmd::CheckCommandCondition)." },
	{ 26, L"Pure Damage", L"Type free attack.\nDamage = Power * 100 + Accuracy" },
	{ 27, L"Matra Magic", L"Bring HP to « Power » but fail if the target is under Easy Kill or Petrify.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifier : Shell\nUse magical evasion." },
	{ 28, L"Limit Glove", L"Pure damage if caster's HP are exactly 1 but miss otherwise.\nDamage = Power * 100 + Accuracy" },
	{ 29, L"Minus Strike", L"Type free attack.\nDamage = CasterMaxHP - CasterHP" },
	{ 30, L"White Wind", L"Heal depending on caster's max HP. Damage Zombie instead of healing them.\nHeal = CasterMaxHP / 3" },
	{ 31, L"Magic Hammer", L"Reduce target's MP.\nDamage MP = Random[0, TargetMP - 1]" },
	{ 32, L"Darkside", L"Physical attack costing 1/8 of caster's max HP and ignoring weapon's element.\nDamage = (Attack * Power / 10 - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\n\nDamage Modifiers : Elemental Affinity and Boost (Spell)" },
	{ 33, L"Armor Break", L"Reduce target's defence by 50%.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifier : Shell\nUse magical evasion." },
	{ 34, L"Power Break", L"Reduce target's strength by 25%.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifier : Shell\nUse magical evasion." },
	{ 35, L"Mental Break", L"Reduce target's magic defence by 50%.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifier : Shell\nUse magical evasion." },
	{ 36, L"Magic Break", L"Reduce target's magic by 25%.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifier : Shell\nUse magical evasion." },
	{ 37, L"Chakra", L"Heal both HP and MP. The effect is doubled if the caster has Power Up.\nHeal = MaxHP * Power / 100\nMP Heal = MaxMP * Power / 100" },
	{ 38, L"Spare Change", L"Type free attack using gils.\nLost Gils = Level * Power.\nDamage = (LostGils * LostGils * Spirit) / (TotalGils * 10)\n\nDamage is spread between the targets." },
	{ 39, L"Lancer", L"Physical attack reducing both HP and MP.\nDamage = (Attack * Power / 10 - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\nDamage MP = Damage / 16\n\nDamage Modifiers\n Caster : Berserk, Trance and Mini\n Target : Defend, Protect, Sleep and Mini" },
	{ 40, L"Dragon Breath", L"Type free attack on all the enemies depending on target's HP.\nDamage = MaxHP - HP" },
	{ 41, L"White Draw", L"Heal all the allies' MP depending on target's level.\nMP Heal = Random[0, TargetLevel * 2 - 1]" },
	{ 42, L"Throw", L"Physical attack using a weapon.\nDamage = (WeaponAttack * 2 - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\n\nDamage Modifiers\n Caster : Healer, Berserk, Trance and Mini\n Target : Flying type, Defend, Protect, Sleep and Mini\nKill Frozen targets." },
	{ 43, L"Might", L"Increase target's stength.\nBonus = (100 / Power)%" },
	{ 44, L"Focus", L"Increase target's magic.\nBonus = (100 / Power)%" },
	{ 45, L"Sacrifice", L"Fully heal targets' HP and MP by consuming caster's HP and MP. Fail on Petrified targets." },
	{ 46, L"Soul Blade", L"Add status depending on caster's weapon.\nWork only if the equipped weapon is a thief sword (ID between 7 and 15)." },
	{ 47, L"Apply Effect", L"No effect by itself.\nUsed by the other spell effects to apply damage, status, etc..." },
	{ 48, L"Spear", L"Physical attack.\nDamage = 1.5 * (Attack - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\n\nDamage Modifiers : Healer, High Jump (factor changed from 1.5 to 2), Mini (Caster - half), Defend, Protect, Sleep and Mini (Target)" },
	{ 49, L"Eidolon Phoenix", L"Magic Attack on target enemies and Life on target allies.\nDamage = Magic Attack formula using Phoenix's spell datas\nHeal = Life formula using Rebirth Flame's spell datas" },
	{ 50, L"Six Dragons", L"Randomly change target's HP and/or MP but fail if the target is under Petrify.\nRestore both HP and MP : 10%\nRestore HP : 20%\nRestore MP : 20%\nReduce HP to 1 : 15%\nReduce MP to 1 : 15%\nReduce both HP and MP to 1 : 20%" },
	{ 51, L"Curse", L"Add an elemental weakness.\nThe base Curse spell slots randomize the element but the others have a determinist element (see btl_cmd::CheckCommandCondition)." },
	{ 52, L"Angel's Snack", L"Use an item specified by « Power » on each target." },
	{ 53, L"Lucky Seven", L"Type free attack improved if the last figure of caster's HP is 7.\nBase Damage = 1\nImproved Damage = Random{7, 77, 777, 7777}" },
	{ 54, L"What's That!?", L"Place the target enemies in a back attack state but fail if the « Scripted Start » flag of the battle is on.\nAlso put the target allies back to their usual row position." },
	{ 55, L"Change", L"Toggle between front row and back row." },
	{ 56, L"Flee", L"Escape from battle. Always miss during an enemy attack.\nSuccess Rate Per Second = 200 * TeamMeanLevel / EnemiesMeanLevel / 16" },
	{ 57, L"Flee Skill", L"Escape from battle but lose 10% of enemies carried gils." },
	{ 58, L"Steal", L"Try to steal an item from the target ennemies." },
	{ 59, L"Scan", L"Display the target's statistics but fail if it is under Easy Kill." },
	{ 60, L"Detect", L"Display target's owned items." },
	{ 61, L"Charge!", L"Make low HP allies (under 15% of their max HP) attack a random target. Those attacks are considered as counters and can't trigger another counter." },
	{ 62, L"Item : Soft", L"Cure Status (usable out of battles). Kill the target instead if it has the Stone type and is not under Petrify." },
	{ 63, L"Sword Magic", L"Perform a basic attack with improved damage and custom element.\nDamage = (Attack + Power - Defence) * Random[Strength, Strength + (Level + Strength) / 8]\n\nDamage Modifiers : Elemental Affinity and Boost (Spell)\nMiss on flying targets if the spell is Earth-elemental." },
	{ 64, L"Special", L"Used by enemies for various purposes.\nDoesn't do anything by itself." },
	{ 65, L"Eat", L"Eat the target if his HP are low enough but fail if it is under Easy Kill, Petrify or is Human-type.\nThreshold = MaxHP / Power" },
	{ 66, L"Frog Drop", L"Type free attack depending on frogs catched.\nDamage = Level * NbFrogs" },
	{ 67, L"Thievery", L"Type free attack depending on items stolen.\nDamage = Speed * NbSteals / 2" },
	{ 68, L"Dragon's Crest", L"Type free attack depending on dragons killed.\nDamage = NbDragons * NbDragons" },
	{ 69, L"Item : Heal", L"Item heal improved in battle (usable out of battles). Reverse the effect on Zombie.\nHeal = ItemPower * 10\nImproved Heal = ItemPower* 15\n\nHeal Modifier : Chemist" },
	{ 70, L"Item : MP Heal", L"Item MP heal improved in battle (usable out of battles). Reverse the effect on Zombie.\nMP Heal = ItemPower * 10\nImproved Heal = ItemPower * 15\n\nHeal Modifier : Chemist" },
	{ 71, L"Item : Full Heal", L"Set target's HP and MP at max (usable out of battles). Reverse the effect on Zombie." },
	{ 72, L"Item : Revive", L"Cure status and heal a player character (usable out of battles) but fail is the target is under Petrify. On Zombie targets, it misses if the target is also KO and set its HP to Random[0, 9] instead else.\nHeal = Random[1, 10]" },
	{ 73, L"Item : Cure Status", L"Cure Statuses using the item's status list (usable out of battles)." },
	{ 74, L"Item : Ore Heal", L"Heal depending on amount of the corresponding item in the player's inventory.\nHeal = NbItem * Power" },
	{ 75, L"Item : Pepper Damage", L"Type free attack depending on amount of the corresponding item in the player's inventory.\nDamage = NbItem * Power" },
	{ 76, L"Item : Tent", L"Heal 50% of target's max HP and MP. Also has 50% chances to add statuses using the item's status list." },
	{ 77, L"Item : Dark Matter", L"Type free attack dealing max damage but fail if the target is under Petrify.\nDamage = 9999" },
	{ 78, L"Unused 1", L"No effect." },
	{ 79, L"Unused 2", L"No effect." },
	{ 80, L"Unused 3", L"No effect." },
	{ 81, L"Unused 4", L"No effect." },
	{ 82, L"Unused 5", L"No effect." },
	{ 83, L"Trance Spear", L"Physical attack.\nDamage = 1.5 * (Attack - Defence) * Random[0, Random[Strength, Strength + (Level + Strength) / 8]]\n\nDamage Modifiers : High Jump (factor changed from 1.5 to 2), Mini (Caster - half), Defend, Protect, Sleep and Mini (Target)\nDamage is spread between the targets" },
	{ 84, L"Jewel", L"Add an ore to the inventory.\nSuccess Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\Success Rate Modifier : Shell\nUse magical evasion." },
	{ 85, L"Eidolon Common", L"Magic attack possibly weakened (unless the supporting ability Boost is on).\nDamage = (Power - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8]\nWeakened Damage = Damage * 2 / 3\n\nDamage Modifiers : Mini (Caster - half), Shell, Elemental Affinity and Boost\nFor several spell slots, the amount of jewels or the caster's level increase the Power.\nMiss on flying targets if the spell is Earth-elemental." },
	{ 86, L"Eidolon Atomos", L"Magic attack depending on target's max HP, possibly weakened (unless the supporting ability Boost is on), but fail if the target is under Easy Kill.\nDamage = HPMax * Power / 100\nWeakened Damage = Damage * 2 / 3\n\nDamage Modifiers : Amethyst amount, Elemental Affinity and Boost\nMiss on flying targets if the spell is Earth-elemental." },
	{ 87, L"Eidolon Odin", L"Add status but fail if the target is under Easy Kill. If the caster's Odin Sword is on, it also deals magic damage possibly weakened.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel + OreAmount / 2\nDamage = (Power + 100 - OreAmount - MagicDefence) * Random[Magic, Magic + (Level + Magic) / 8]\nWeakened Damage = Damage * 2 / 3\n\nUse magical evasion." },
	{ 88, L"Self-Destruct", L"Type free attack sacrifying the caster.\nDamage = CasterHP" },
	{ 89, L"HP Switching", L"Switch caster's and target's current HP but fail if the target is under Easy Kill or Petrify." },
	{ 90, L"Defless", L"Divide target's defence and magic defence by 2." },
	{ 91, L"Cannon", L"Magic attack depending on target's current HP.\nDamage = Power * HP / 100\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nDamage Modifiers : Elemental Affinity and Boost\nHit Rate Modifiers : Multi-targeting and Shell\nMiss on flying targets if the spell is Earth-elemental." },
	{ 92, L"Add Item", L"Give an item specified by « Power » to the player." },
	{ 93, L"Maelstrom", L"Bring HP to a random number between 1 and 10 and add status but fail if the target is under Easy Kill or Petrify." },
	{ 94, L"Absorb Magic", L"Divide by 2 the target's magic and multiply by 2 the caster's.\nCan break the magic limit up to 255." },
	{ 95, L"Absorb Strength", L"Divide by 2 the target's strength and multiply by 2 the caster's.\nCan break the strength limit up to 255." },
	{ 96, L"Trance", L"Turn the target on trance." },
	{ 97, L"Entice", L"Add status but fail on girls." },
	{ 98, L"Terran Strike", L"Physical attack that misses on terran people.\nDamage = (Power - Defence) * Random[Strength, Strength + (Level + Strength) / 4]" },
	{ 99, L"Flare Star", L"Type free attack depending on target's level.\nDamage = Power * TargetLevel\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifiers : Multi-targeting and Shell\nUse magical evasion." },
	{ 100, L"Enemy Accurate Attack", L"Physical attack used by enemies that never misses and can also add status.\nDamage = (Power - Defence) * Random[Strength, Strength + (Level + Strength) / 4]\n\nDamage Modifiers\n Caster : Berserk, Trance, Mini, Back Row and Elemental Boost\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity\nKill Frozen targets." },
	{ 101, L"Inventory Steal", L"Remove an item from the player's inventory specified by « Accuracy ».\nHit Rate = 100\n\nUse physical evasion." },
	{ 102, L"Inventory Mug", L"Physical attack.\nAlso Remove an item from the player's inventory specified by « Accuracy ».\nDamage = (Power - Defence) * Random[Strength, Strength + (Level + Strength) / 4]\n\nDamage Modifiers : Berserk, Trance, Mini (Caster and Target), Defend, Protect, Sleep, Back Attack, Back Row, Gamble Defence" },
	{ 103, L"Good Status", L"Add status with a 100% hit rate." },
	{ 104, L"Grudge", L"Type free attack depending on Tonberry killed.\nDamage = 4 ^ NbTonberry" },
	{ 105, L"Grand Cross", L"Add random statuses among the following, but fail if the target is under Petrify :\nPetrify, Silence, Darkness, Trouble, Zombie, Instant Death, Confuse, Berserk, Stop, Poison, Sleep, Heat, Freeze, Doom, Mini and HP reduce to 1-10.\nHit Rate (each status) = 1 / 8" },
	{ 106, L"Banish", L"Remove target from battle." },
	{ 107, L"Beat Back", L"Physical attack that can change the target's row (after damage) and can also add status.\nIf the target is a player character, he is moved to back row. Else, the target moves forward, preventing further front/back row modifiers.\nDamage = (Power - Defence) * Random[Strength, Strength + (Level + Strength) / 4]\n\nDamage Modifiers\n Caster : Berserk, Trance, Mini, Back Row and Elemental Boost\n Target : Defend, Protect, Sleep, Mini, Back Attack, Back Row, Gamble Defence and Elemental Affinity" },
	{ 108, L"Iai Strike", L"Add status but fail if the target is under Easy Kill.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifiers : Multi-targeting and Shell\nUse magical evasion." },
	{ 109, L"Mini", L"Add status. Has a 100% hit rate if target is under Mini.\nHit Rate = Accuracy + Magic / 4 + Level - TargetLevel\n\nHit Rate Modifiers : Multi-targeting and Shell\nUse magical evasion." }
};

#define HADES_STRING_STATUS_NONE	L"No Status"
static SortedChoiceItem HADES_STRING_STATUS_PACK[] = {
	{ 0xFFFFFFFF, L"Everything" },
	{ 0xBFFF3C7F, L"All Statuses" },
	{ 0x24EC2000, L"All Good" },
	{ 0x9B13147F, L"All Bad" },
	{ 0x93030418, L"Esuna" },
	{ 0x10110410, L"Bad Breath" },
	{ 0x00C00000, L"Mighty Guard" },
	{ 0x00042000, L"Aura" },
};
static wxString HADES_STRING_STATUS[] = {
	L"Petrify",
	L"Venom",
	L"Virus",
	L"Silence",
	L"Darkness",
	L"Trouble",
	L"Zombie",
	L"Easy Kill",
	L"Death",
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

#define SPELL_MODEL_ATTACK_ID 101
#define SPELL_MODEL_ATTACK_AMOUNT 18
#define SPELL_MODEL_WEAPON_ID 313
#define SPELL_MODEL_WEAPON_AMOUNT 65
static SortedChoiceItemModel HADES_STRING_SPELL_MODEL[] = {
	{ 0,   L"Fire (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 70,  L"Fire (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 2,   L"Fira (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 71,  L"Fira (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 7,   L"Firaga (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 72,  L"Firaga (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 26,  L"Sleep (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 78,  L"Sleep (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 6,   L"Blizzard (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 62,  L"Blizzard (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 4,   L"Blizzara (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 77,  L"Blizzara (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 151, L"Blizzaga (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 152, L"Blizzaga (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 126, L"Slow", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 3,   L"Thunder (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 66,  L"Thunder (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 5,   L"Thundera (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 73,  L"Thundara (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 24,  L"Thundaga (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 133, L"Thundaga (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 149, L"Stop", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 180, L"Aero", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 502, L"Aera-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 205, L"Aera-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 284, L"Aeraga", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 21,  L"Poison (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 202, L"Poison (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 93,  L"Bio (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 120, L"Bio (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 57,  L"Osmose", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 132, L"Drain", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 98,  L"Demi (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 99,  L"Demi (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 56,  L"Comet", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 94,  L"Death", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 153, L"Break", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 22,  L"Water (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 95,  L"Water (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 427, L"Waterga", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 134, L"Meteor (Success)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 143, L"Meteor (Fail)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 125, L"Flare", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 436, L"Flare Star", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 308, L"Doomsday", SPELL_TARGET_TYPE_EVERYONE, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 199, L"Focus", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 8,   L"Cure (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 9,   L"Cure (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 141, L"Cura (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 162, L"Cura (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 142, L"Curaga (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 144, L"Curaga (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 32,  L"Regen", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 17,  L"Life", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 15,  L"Full Life", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 131, L"Scan", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 12,  L"Panacea", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 13,  L"Stona", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 11,  L"Esuna", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 63,  L"Shell", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 123, L"Protect", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 89,  L"Haste", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 42,  L"Silence (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 127, L"Silence (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 25,  L"Mini (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 124, L"Mini (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 79,  L"Reflect", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 96,  L"Confuse (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 97,  L"Confuse (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 14,  L"Berserk (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 140, L"Berserk (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 158, L"Blind (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 157, L"Blind (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 27,  L"Float (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 76,  L"Float (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 128, L"Dispel", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 201, L"Jewel", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 90,  L"Might (Ally)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 100, L"Might (Enemy)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 87,  L"Holy", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 407, L"Shiva (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 38,  L"Shiva (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 445, L"Ifrit (Short)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 276, L"Ifrit (Full)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 415, L"Ramuh (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 186, L"Ramuh (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 446, L"Atomos (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 184, L"Atomos (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 424, L"Odin (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 261, L"Odin (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 406, L"Leviathan (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 179, L"Leviathan (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 405, L"Bahamut (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 227, L"Bahamut (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 447, L"Ark (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 381, L"Ark (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 504, L"Carbuncle-Ruby (Short)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 177, L"Carbuncle-Ruby (Full)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 505, L"Carbuncle-Pearl (Short)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 493, L"Carbuncle-Pearl (Full)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 506, L"Carbuncle-Emerald (Short)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 494, L"Carbuncle-Emerald (Full)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 507, L"Carbuncle-Diamond (Short)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 495, L"Carbuncle-Diamond (Full)", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 508, L"Fenrir-Earth (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 210, L"Fenrir-Earth (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 509, L"Fenrir-Wind (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 226, L"Fenrir-Wind (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 510, L"Phoenix (Short)", SPELL_TARGET_TYPE_EVERYONE, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 211, L"Phoenix (Full)", SPELL_TARGET_TYPE_EVERYONE, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 225, L"Phoenix-Rebirth Flame", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 378, L"Madeen (Short)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 251, L"Madeen (Full)", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 254, L"Eat", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 255, L"Cook", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 74,  L"Goblin Punch", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 154, L"LV5 Death", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 129, L"LV4 Holy", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 392, L"LV3 Def-less", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 236, L"Doom", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 237, L"Roulette", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 28,  L"Aqua Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 81,  L"Mighty Guard", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 88,  L"Matra Magic", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 44,  L"Bad Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 82,  L"Limit Glove", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 30,  L"1000 Needles", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 297, L"Pumpkin Head", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 501, L"Night", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 178, L"Twister", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 390, L"Earth Shake", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 45,  L"Angel's Snack", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 228, L"Frog Drop", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 41,  L"White Wind", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 500, L"Vanish", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 150, L"Frost", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 187, L"Mustard Bomb", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 130, L"Magic Hammer", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 138, L"Auto-Life", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 148, L"Trine", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 188, L"Darkside-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 409, L"Darkside-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 189, L"Minus Strike-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 410, L"Minus Strike-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 190, L"Iai Strike-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 411, L"Iai Strike-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 208, L"Power Break", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 92,  L"Armor Break", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 209, L"Mental Break", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 46,  L"Magic Break", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 398, L"Charge!", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 191, L"Thunder Slash-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 412, L"Thunder Slash-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 457, L"Thunder Slash-3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 207, L"Stock Break-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 414, L"Stock Break-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 459, L"Stock Break-3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 397, L"Climhazzard-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 417, L"Climhazzard-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 443, L"Climhazzard-3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 192, L"Shock-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 413, L"Shock-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 458, L"Shock-3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 212, L"Fire Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 213, L"Fira Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 214, L"Firaga Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 215, L"Thunder Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 216, L"Thundara Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 217, L"Thundaga Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 218, L"Blizzard Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 219, L"Blizzara Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 220, L"Blizzaga Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 221, L"Bio Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 222, L"Water Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 223, L"Flare Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 224, L"Doomsday Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 200, L"Steal-Main", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 19,  L"Steal-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 20,  L"Steal-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 119, L"Steal-3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 273, L"Steal-4", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 249, L"Flee-Skill", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 250, L"Detect", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 399, L"What's That!?", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 229, L"Soul Blade", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 233, L"Annoy", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 234, L"Sacrifice", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 312, L"Lucky Seven", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 231, L"Thievery", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 241, L"Free Energy", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 242, L"Tidal Flame", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 243, L"Scoop Art", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 244, L"Shift Break", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 245, L"Stellar Circle 5", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 246, L"Meo Twister", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 247, L"Solution 9", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 248, L"Grand Lethal", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 393, L"Jump", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 1 },
	{ 388, L"Spear", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 389, L"Spear-Trance", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 401, L"Lancer", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 168, L"Reis' Wind", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 296, L"Dragon Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 83,  L"White Draw", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 61,  L"Luna", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 491, L"Six Dragons", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 387, L"Cherry Blossom", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 490, L"Dragon's Crest", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 265, L"Throw-Racket", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 266, L"Throw-Thief Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 267, L"Throw-Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 268, L"Throw-Spear", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 269, L"Throw-Claw", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 270, L"Throw-Rod", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 271, L"Throw-Fork", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 272, L"Throw-Dagger", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 277, L"Throw-Disc", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 85,  L"Chakra (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 86,  L"Chakra (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 135, L"Spare Change (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 136, L"Spare Change (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 193, L"No Mercy (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 194, L"No Mercy (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 197, L"Aura (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 198, L"Aura (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 195, L"Curse (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 196, L"Curse (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 137, L"Revive (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 421, L"Revive (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 121, L"Demi Shock (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 122, L"Demi Shock (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 385, L"Countdown (Single)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 386, L"Countdown (Multi)", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 278, L"Ether", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 40,  L"Elixir", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 280, L"Echo Screen", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 279, L"Eye Drops", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 281, L"Magic Tag", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 282, L"Vaccine", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 283, L"Annoyntment", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 287, L"Dark Matter", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 289, L"Gysahl Greens", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 288, L"Dead Pepper", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 290, L"Tent", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 292, L"Ore", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 159, L"Pyro", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 293, L"Poly", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 377, L"Medeo", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	
	{ 10,  L"Poison", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 383, L"Poison", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 3 },
	{ 55,  L"Blind", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 16,  L"Instant", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 1 },
	{ 18,  L"Smoke", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 1 },
	{ 23,  L"Snowflakes", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 31,  L"Explosion", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 33,  L"Greater Smoke", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 1 },
	{ 35,  L"Psychokinesis", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 47,  L"Flame", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 48,  L"Pollen", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 49,  L"Glare", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 50,  L"Glowing Eyes", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 51,  L"Snort", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 52,  L"Cold Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 53,  L"Oil Shot", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 54,  L"Inferno", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 58,  L"Earthquake", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 59,  L"Trouble Juice", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 60,  L"Rainbow Storm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 64,  L"Bubbles", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 65,  L"Energy Blast", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 67,  L"Ink", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 68,  L"Ultra Sound Wave", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 75,  L"Sandstorm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 69,  L"Aerial Slash", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 29,  L"Aerial Slash-1", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 36,  L"Aerial Slash-2", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 43,  L"Aerial Slash-3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 139, L"Aerial Slash-4", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 145, L"Entice", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 146, L"Hypnotize", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 147, L"Cannon", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 155, L"Lava Gun x6", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 160, L"String", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 161, L"Blaster", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 163, L"Electrocute", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 164, L"Buzz", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 165, L"Pollen", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 166, L"Firestorm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 156, L"Starburst", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 169, L"Lightning", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 170, L"Snowstorm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 171, L"Maelstrom", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 172, L"Bio Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 173, L"Aero Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 174, L"Venom Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 175, L"Zombie Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 176, L"Autumn Leaves", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 181, L"Rainbow Wind", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 182, L"Scorch", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 183, L"Heavy", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 185, L"Sandstorm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 203, L"Nanoflare", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 204, L"Meteorite", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 206, L"Gradual Petrify", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 230, L"Twinkle", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 235, L"Psychokinesis", SPELL_TARGET_TYPE_ALLY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 238, L"Mirror", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 239, L"Red Clipper", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 240, L"Sand Breath", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 256, L"Solution", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 258, L"Rise", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 259, L"Mist", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 260, L"Soul Out", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 262, L"Slime", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 274, L"Devour", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 275, L"Light Shell", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 285, L"Forcefield", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 291, L"Raining Swords", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 294, L"Boomerang", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 295, L"Shimmer", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 298, L"Jet Fire", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 299, L"Photon", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 300, L"Magnitude 8", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 301, L"Sinkhole", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ZERO, 2 },
	{ 303, L"Paper Storm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 304, L"Propeller Wind", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 305, L"Accumulate", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ZERO, 0 },
	{ 306, L"Green Smoke", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 307, L"Red Smoke", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 309, L"Curse", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 310, L"Absorb", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 311, L"Flame", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 382, L"Chestnut", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 391, L"Fire Blades", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 395, L"Rippler", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 396, L"Rise", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 400, L"Smoke", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 402, L"Smoke (Self)", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ZERO, 0 },
	{ 403, L"Atomic Ray", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 404, L"Shockwave", SPELL_TARGET_TYPE_ENEMY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 408, L"Tidal Waves", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 416, L"Flaming Sword", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 419, L"Yellow Shot", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 167, L"Water Gun", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 422, L"Water Gun x3", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 423, L"Electricity", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 286, L"Virus Fly", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 425, L"Virus Powder", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 428, L"Virus Crunch", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 429, L"Green Sparkle", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 1 },
	{ 433, L"Dust Storm", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 434, L"Lapis Laser", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 498, L"Grand Cross", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 499, L"Neutron Ring", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 0 },
	{ 503, L"Blue Shockwave", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 486, L"Ragtime True", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 487, L"Ragtime False", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 437, L"Dummy", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ZERO, 0 },
	{ 101, L"[Attack-1...]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 313, L"[Weapon Attack-1...]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 253, L"[Prepare Attack]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 511, L"[Channel]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 252, L"[Flee]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 2 },
	{ 257, L"[Trance-Activate]", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ZERO, 0 },
	{ 489, L"[Trance-End]", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ZERO, 0 },
	{ 394, L"[Trance-Kuja]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 2 },
	{ 232, L"[Teleport]", SPELL_TARGET_TYPE_SELF, SPELL_TARGET_AMOUNT_ONE, 3 },
	{ 302, L"[Disappear]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 2 },
	{ 1,   L"[Special Curaga]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 2 },
	{ 418, L"[Steiner VS Blank]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 2 },
	{ 460, L"[Sealion-Engage]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 1 },
	{ 441, L"[Gizamaluke-Engage]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 2 },
	{ 440, L"[Gizamaluke-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 2 },
	{ 431, L"[Valia Pira-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 34,  L"[Kraken's Arm-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ONE, 0 },
	{ 384, L"[Ultima-Memoria]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 492, L"[Ultima-Terra]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 3 },
	{ 438, L"[Silver Dragon-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 439, L"[Nova Dragon-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 420, L"[Deathguise-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 435, L"[Necron-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 496, L"[Necron-Engage]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 2 },
	{ 497, L"[Necron-Teleport]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_GROUP, 2 },
	{ 432, L"[Ozma-Death]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 37,  L"[Lockup]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 39,  L"[Lockup]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 80,  L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 84,  L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 91,  L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 263, L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 264, L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 426, L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 430, L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 442, L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 },
	{ 444, L"[Game Crash]", SPELL_TARGET_TYPE_ANY, SPELL_TARGET_AMOUNT_ZERO, 3 }
};

//-- Shops
static SortedChoiceItem HADES_STRING_SHOP_NAME[] = {
	{ 0, L"Dali (Disc 1-4)" },
	{ 1, L"Lindblum (Disc 1)" },
	{ 2, L"Burmecia (Disc 1-4)" },
	{ 3, L"Summit Station (Disc 2)" },
	{ 4, L"Treno (Disc 2)" },
	{ 5, L"Cleyra (Disc 2)" },
	{ 6, L"Lindblum (Disc 2)" },
	{ 7, L"Fossile Roo (Disc 2)" },
	{ 8, L"Conde Petie (Disc 2-3)" },
	{ 9, L"Black Mage Village (Disc 2-3)" },
	{ 10, L"Madain Sari (Disc 2-4)" },
	{ 11, L"Alexandria (Disc 3-4)" },
	{ 12, L"Treno (Disc 3-4)" },
	{ 13, L"Lindblum (Disc 3-4)" },
	{ 14, L"Oeilvert (Disc 3)" },
	{ 15, L"Esto Gaza (Disc 3)" },
	{ 16, L"Ipsen's Castle (Disc 3-4)" },
	{ 17, L"Daguerreo (Disc 3)" },
	{ 18, L"Bran Bal (Disc 3)" },
	{ 19, L"Daguerreo (Disc 4)" },
	{ 20, L"Black Mage Village (Disc 4)" },
	{ 21, L"Esto Gaza (Disc 3)" },
	{ 22, L"Desert Palace (Disc 3)" },
	{ 23, L"Unused 1" },
	{ 24, L"Unused 2" },
	{ 25, L"Item Shop 1 (Disc 1-4)" },
	{ 26, L"Item Shop 2 (Disc 1-3)" },
	{ 27, L"Item Shop 3 (Disc 2-3)" },
	{ 28, L"Item Shop 4 (Disc 3-4)" },
	{ 29, L"Item Shop 5 (Disc 3-4)" },
	{ 30, L"Item Shop 6 (Disc 4)" },
	{ 31, L"Item Shop 7 (Zorn & Thorn)" },
	{ 32, L"Synth Shop : Lindblum (Disc 1-2)" },
	{ 33, L"Synth Shop : Treno (Disc 2)" },
	{ 34, L"Synth Shop : Lindblum (Disc 2)" },
	{ 35, L"Synth Shop : B. M. Village (Disc 2-3)" },
	{ 36, L"Synth Shop : Alex. Lind. Treno (Disc 3-4)" },
	{ 37, L"Synth Shop : Daguerreo (Disc 3-4)" },
	{ 38, L"Synth Shop : B. M. Village (Disc 4)" },
	{ 39, L"Synth Shop : Hades (Disc 4)" }
};

//-- Cards
static SortedChoiceItem HADES_STRING_DECK_NAME[] = {
	{ 0, L"Deck 0 - Alexandria..." }, // [Alexandria 1] Synthesist + Man (Alexandria 1) + Michelle (Alexandria 3) + Old Man (Treno)
	{ 1, L"Deck 1 - Alexandria" }, // [Alexandria 1] Mick + Synthesist's Wife (Alexandria 1)
	{ 2, L"Deck 2 - Alexandria..." }, // [Alexandria 1] Honorable Lord + Fish Man (Alexandria 1) + Young Nobleman (Treno)
	{ 3, L"Deck 3 - Alexandria..." }, // [Alexandria 1] Honorable Lady + Hippolady (Alexandria 1) + Noblewoman (Treno) + Tim (Lindblum 1)
	{ 4, L"Deck 4 - Alexandria..." }, // [Alexandria 1] Florist + Ashley (Alexandria 1) + Thomas (Lindblum 1)
	{ 5, L"Deck 5 - Alexandria..." }, // [Alexandria 1] Boatman + Maggie (Alexandria 1) + Rita (Lindblum 1)
	{ 6, L"Deck 6 - Alexandria..." }, // [Alexandria 1] Retired Boatman + Dante the Signmaker (Alexandria 1) + Nobleman (Treno) + Member n°56 (Lindblum 1)
	{ 7, L"Deck 7 - Alexandria..." }, // [Alexandria 1] Tom + Male Red Mage (Alexandria 1) + Adventurer (Treno) + Member n°29 (Lindblum 1)
	{ 8, L"Deck 8 - Alexandria" }, // [Alexandria 1] Ilia + Noble Lady (Alexandria 1)
	{ 9, L"Deck 9 - Alexandria" }, // [Alexandria 1] Noble Lord + Old Man (Alexandria 1)
	{ 10, L"Deck 10 - Alexandria..." }, // [Alexandria 1] Nikolai + Zenero (Alexandria 3) + Criminal (Treno)
	{ 11, L"Deck 11 - Alexandria..." }, // [Dali] Morrid + Fish Man (Alexandria 3) + Doug (Alexandria 1) + Female Adventurer (Treno)
	{ 12, L"Deck 12 - Alexandria..." }, // [Dali] Shopkeeper Eve + Shopkeeper (Alexandria 1)
	{ 13, L"Deck 13 - Alexandria" }, // [Alexandria 1] Ryan
	{ 14, L"Deck 14 - Alexandria" }, // [Alexandria 3] Benero + Hippaul (Alexandria 1)
	{ 15, L"Deck 15 - Alexandria..." }, // [Alexandria 3] Old Lady + Alleway Jack (Alexandria 1) + Father (Treno) + Gus (Lindblum 1) + Lilian (Lindblum 1)
	{ 16, L"Deck 16 - Lindblum..." }, // [Dali] Part-time Worker Mary + Card Freak Gon (Lindblum 1 & 3)
	{ 17, L"Deck 17 - Lindblum" }, // [Lindblum 1] Marsha
	{ 18, L"Deck 18 - Lindblum..." }, // [Alexandria 3] Genero + Old Margaret (Lindblum 1)
	{ 19, L"Deck 19 - Lindblum..." }, // [Alexandria 3] Hippolady + Drunk (Treno) + Female Red Mage (Lindblum 1)
	{ 20, L"Deck 20 - Lindblum" }, // [Lindblum 1] Wimpy Guy
	{ 21, L"Deck 21 - Lindblum" }, // [Lindblum 1] Lady with an Apron
	{ 22, L"Deck 22 - Lindblum..." }, // [Alexandria 3] Boatman + Yaup (Lindblum 1)
	{ 23, L"Deck 23 - Treno..." }, // [Alexandria 3] Man + Nobleman (Treno)
	{ 24, L"Deck 24 - Lindblum" }, // [Lindblum 1 & 3] Attendant
	{ 25, L"Deck 25 - Lindblum..." }, // [Chocobo Paradise] Yellow Chocobo + Marian (Lindblum 1)
	{ 26, L"Deck 26 - Lindblum..." }, // [Alexandria 3] Retired Boatman + Artist Michael (Lindblum 1 & 3)
	{ 27, L"Deck 27 - Lindblum..." }, // [Alexandria 3] Old Man + Noblewoman (Treno) + Pigeon Lover (Lindblum 1)
	{ 28, L"Deck 28 - South Gate" }, // [South Gate] Worker
	{ 29, L"Deck 29 - Treno" }, // [Treno] Natalie
	{ 30, L"Deck 30 - South Gate..." }, // [Alexandria 3] Tom + Shopkeeper (Treno) + Chef (South Gate)
	{ 31, L"Deck 31 - Treno..." }, // [Alexandria 3] Tour Guide + Thug (Treno)
	{ 32, L"Deck 32 - Treno" }, // [Treno] Carpenter
	{ 33, L"Deck 33 - Treno" }, // [Treno] Scholar
	{ 34, L"Deck 34 - Treno..." }, // [Dali] Part-time Worker Jeff + Onionmeister (Alexandria 3) + Shopkeeper (Treno)
	{ 35, L"Deck 35 - Treno..." }, // [Alexandria 3] Mick + Queen Stella (Treno)
	{ 36, L"Deck 36 - Treno" }, // [Treno] Servant
	{ 37, L"Deck 37 - Treno..." }, // [Chocobo Paradise] Blue Chocobo + Thug (Treno)
	{ 38, L"Deck 38 - Treno..." }, // [Alexandria 3] Ilia + Barkeep (Treno)
	{ 39, L"Deck 39 - Treno..." }, // [Alexandria 3] Doug + Mario (Treno)
	{ 40, L"Deck 40 - Treno" }, // [Treno] Waitress
	{ 41, L"Deck 41 - Treno..." }, // [Chocobo Paradise] Red Chocobo + Tot (Treno) + Dan (Cleyra)
	{ 42, L"Deck 42 - Treno..." }, // [Alexandria 3] Maggie + Thug (Treno) + Laerie (Cleyra)
	{ 43, L"Deck 43 - Alexandria" }, // [Alexandria 3] Honorable Lady
	{ 44, L"Deck 44 - Treno..." }, // [Treno] Thug (after throwing gils) + Jack (Cleyra)
	{ 45, L"Deck 45 - Treno..." }, // [Treno] Nobleman + Adam (Cleyra)
	{ 46, L"Deck 46 - Alexandria" }, // [Alexandria 3] Dante the Signmaker
	{ 47, L"Deck 47 - Treno..." }, // [Alexandria 3] Honorable Lord + Noblewoman (Treno)
	{ 48, L"Deck 48 - Treno" }, // [Treno] Girl
	{ 49, L"Deck 49 - Alexandria" }, // [Alexandria 3+] Boatman
	{ 50, L"Deck 50 - Mage Village..." }, // [Mage Village] Black Mage 24 + Retired Boatman (Alexandria 3+)
	{ 51, L"Deck 51 - Fossil Roo..." }, // [Mage Village] Black Mage 123 + Flower Girl (Alexandria) + Tom (Alexandria 3+) + Treasure Hunter (Fossil Roo)
	{ 52, L"Deck 52 - Fossil Roo..." }, // [Mage Village] Black Mage 234 + Flower Girl (Alexandria 3+) + Treasure Hunter (Fossil Roo)
	{ 53, L"Deck 53 - Condee Petie..." }, // [Mage Village] Black Mage 189 + John Fruitbringer (Condee Petie) + Weimar (Alexandria 3+)
	{ 54, L"Deck 54 - Condee Petie..." }, // [Mage Village] Black Mage 44 + Wendy Grocer (Condee Petie) + Old Lady (Alexandria 3+)
	{ 55, L"Deck 55 - Condee Petie..." }, // [Mage Village] Black Mage 192 + Kelley Fingerwaver (Condee Petie) + Synthesist (Alexandria 3+)
	{ 56, L"Deck 56 - Condee Petie..." }, // [Mage Village] Black Mage 56 + Shamis Gatekeeper (Condee Petie) + Ashley (Alexandria 3+)
	{ 57, L"Deck 57 - Condee Petie..." }, // [Mage Village] Black Mage 33 + Robert Dogherder (Condee Petie) + Tour Guide (Alexandria 3+)
	{ 58, L"Deck 58 - Condee Petie..." }, // [Mage Village] Black Mage 111 + Granin Miller (Condee Petie) + Tom's Mother (Alexandria 3+)
	{ 59, L"Deck 59 - Condee Petie..." }, // [Condee Petie] Barbara Gibgab + Ryan (Alexandria 3) + Man (Alexandria 3+)
	{ 60, L"Deck 60 - Alexandria" }, // [Alexandria 3] Synthesist + Shopkeeper (Alexandria 3+)
	{ 61, L"Deck 61 - Alexandria" }, // [Alexandria 3] Shopkeeper + Ryan (Alexandria 3+)
	{ 62, L"Deck 62 - Alexandria" }, // [Alexandria 3] Nikolai + Hippolady (Alexandria 3+)
	{ 63, L"Deck 63 - Alexandria..." }, // [Chocobo Paradise] Navi Chocobo + Hippaul (Alexandria 3 & 3+)
	{ 64, L"Deck 64 - Alexandria" }, // [Alexandria 3+] Fish Man
	{ 65, L"Deck 65 - Alexandria" }, // [Alexandria 3+] Red Mage
	{ 66, L"Deck 66 - Alexandria" }, // [Alexandria 3+] Red Mage
	{ 67, L"Deck 67 - Alexandria" }, // [Alexandria 3+] Ovenmeister
	{ 68, L"Deck 68 - Alexandria" }, // [Alexandria 3+] Nikolai
	{ 69, L"Deck 69 - Alexandria" }, // [Alexandria 3+] Michelle
	{ 70, L"Deck 70 - Alexandria" }, // [Alexandria 3+] Immigrant From Lindblum
	{ 71, L"Deck 71 - Alexandria" }, // [Alexandria 3+] Old Woman
	{ 72, L"Deck 72 - Alexandria" }, // [Alexandria 3+] Old Man
	{ 73, L"Deck 73 - Alexandria" }, // [Alexandria 3+] Ilia
	{ 74, L"Deck 74 - Lindblum" }, // [Lindblum 3] Craftsman
	{ 75, L"Deck 75 - Lindblum" }, // [Lindblum 3] Young Lady
	{ 76, L"Deck 76 - Lindblum" }, // [Lindblum 3] Olivier
	{ 77, L"Deck 77 - Lindblum" }, // [Lindblum 3] Joanna
	{ 78, L"Deck 78 - Lindblum" }, // [Lindblum 3] Carpenter's Apprentice 2
	{ 79, L"Deck 79 - Lindblum" }, // [Lindblum 3] Female Customer
	{ 80, L"Deck 80 - Lindblum" }, // [Lindblum 3] Industrial District Engineer
	{ 81, L"Deck 81 - Patient Fanni..." }, // [Treno] Patient Fanni + Card Boy (Lindblum 3)
	{ 82, L"Deck 82 - Daguerreo" }, // [Daguerreo] Adventurer
	{ 83, L"Deck 83 - Moody Lafrecia..." }, // [Treno] Moody Lafrecia + Card Freak (Lindblum 3) + Booklover (Daguerreo)
	{ 84, L"Deck 84 - Daguerreo..." }, // [Chocobo Paradise] Gold Chocobo + Bookworm (Daguerreo)
	{ 85, L"Deck 85 - Daguerreo" }, // [Daguerreo] Synthesist Expert
	{ 86, L"Deck 86 - Daguerreo" }, // [Daguerreo] Scholar
	{ 87, L"Deck 87 - Daguerreo" }, // [Daguerreo] Scholar
	{ 88, L"Deck 88 - Daguerreo" }, // [Daguerreo] Zebolt
	{ 89, L"Deck 89 - Daguerreo" }, // [Daguerreo] Librarian
	{ 90, L"Deck 90 - Daguerreo" }, // [Daguerreo] Old Man
	{ 91, L"Deck 91 - Daguerreo" }, // [Daguerreo] Weaponsmith
	{ 92, L"Deck 92 - Daguerreo" }, // [Daguerreo] Adventurer
	{ 93, L"Deck 93 - Mage Village" }, // [Mage Village 4] Black Mage 24
	{ 94, L"Deck 94 - Mage Village" }, // [Mage Village 4] Genome
	{ 95, L"Deck 95 - Mage Village" }, // [Mage Village 4] Black Mage 123
	{ 96, L"Deck 96 - Mage Village" }, // [Mage Village 4] Black Mage 234
	{ 97, L"Deck 97 - Mage Village" }, // [Mage Village 4] Genome
	{ 98, L"Deck 98 - Mage Village" }, // [Mage Village 4] Black Mage 189
	{ 99, L"Deck 99 - Mage Village" }, // [Mage Village 4] Black Mage 44
	{ 100, L"Deck 100 - Mage Village..." }, // [Madain Sari] Lani + Genome (Mage Village 4)
	{ 101, L"Deck 101 - Mage Village" }, // [Mage Village 4] Black Mage 192
	{ 102, L"Deck 102 - Mage Village" }, // [Mage Village 4] Black Mage 56
	{ 103, L"Deck 103 - Mage Village" }, // [Mage Village 4] Black Mage 33
	{ 104, L"Deck 104 - Lucious Ellie..." }, // + Genome (Mage Village 4)
	{ 105, L"Deck 105 - Card Freak Cil" },
	{ 106, L"Deck 106 - Gambler Grecchio" },
	{ 107, L"Deck 107 - Straight Shooter Shak" },
	{ 108, L"Deck 108 - Fat Chocobo" },
	{ 109, L"Deck 109 - Bomb Master Shannon" },
	{ 110, L"Deck 110 - Malboro Master Joe" },
	{ 111, L"Deck 111 - Worker #9" },
	{ 112, L"Deck 112 - Gilbert's Teacher" },
	{ 113, L"Deck 113 - Eidolon Master Leyra" },
	{ 114, L"Deck 114 - Beast Master Gilbert" },
	{ 115, L"Deck 115 - Magic Master Thalisa" },
	{ 116, L"Deck 116 - Daguerreo" }, // Gilgamesh
	{ 117, L"Deck 117 - Weapon Master Hunt" },
	{ 118, L"Deck 118 - Master Phantom" },
	{ 119, L"Deck 119 - Defense Phantom" },
	{ 120, L"Deck 120 - Strong Phantom" },
	{ 121, L"Deck 121 - Rare Phantom" },
	{ 122, L"Deck 122 - Dark Phantom" },
	{ 123, L"Deck 123 - Oglop Master Bon" },
	{ 124, L"Deck 124 - Attic Man Wake" },
	{ 125, L"Deck 125 - Mario of the Slums" },
	{ 126, L"Deck 126 - Cardona Bishop" },
	{ 127, L"Deck 127 - Sailor Erin" },
	{ 128, L"Deck 128" },
	{ 129, L"Deck 129" },
	{ 130, L"Deck 130" },
	{ 131, L"Deck 131" },
	{ 132, L"Deck 132" },
	{ 133, L"Deck 133" },
	{ 134, L"Deck 134" },
	{ 135, L"Deck 135" },
	{ 136, L"Deck 136" },
	{ 137, L"Deck 137" },
	{ 138, L"Deck 138" },
	{ 139, L"Deck 139" },
	{ 140, L"Deck 140" },
	{ 141, L"Deck 141" },
	{ 142, L"Deck 142" },
	{ 143, L"Deck 143" },
	{ 144, L"Deck 144" },
	{ 145, L"Deck 145" },
	{ 146, L"Deck 146" },
	{ 147, L"Deck 147" },
	{ 148, L"Deck 148" },
	{ 149, L"Deck 149" },
	{ 150, L"Deck 150" },
	{ 151, L"Deck 151" },
	{ 152, L"Deck 152" },
	{ 153, L"Deck 153" },
	{ 154, L"Deck 154" },
	{ 155, L"Deck 155" },
	{ 156, L"Deck 156" },
	{ 157, L"Deck 157" },
	{ 158, L"Deck 158" },
	{ 159, L"Deck 159" },
	{ 160, L"Deck 160" },
	{ 161, L"Deck 161" },
	{ 162, L"Deck 162" },
	{ 163, L"Deck 163" },
	{ 164, L"Deck 164" },
	{ 165, L"Deck 165" },
	{ 166, L"Deck 166" },
	{ 167, L"Deck 167" },
	{ 168, L"Deck 168" },
	{ 169, L"Deck 169" },
	{ 170, L"Deck 170" },
	{ 171, L"Deck 171" },
	{ 172, L"Deck 172" },
	{ 173, L"Deck 173" },
	{ 174, L"Deck 174" },
	{ 175, L"Deck 175" },
	{ 176, L"Deck 176" },
	{ 177, L"Deck 177" },
	{ 178, L"Deck 178" },
	{ 179, L"Deck 179" },
	{ 180, L"Deck 180" },
	{ 181, L"Deck 181" },
	{ 182, L"Deck 182" },
	{ 183, L"Deck 183" },
	{ 184, L"Deck 184" },
	{ 185, L"Deck 185" },
	{ 186, L"Deck 186" },
	{ 187, L"Deck 187" },
	{ 188, L"Deck 188" },
	{ 189, L"Deck 189" },
	{ 190, L"Deck 190" },
	{ 191, L"Deck 191" },
	{ 192, L"Deck 192" },
	{ 193, L"Deck 193" },
	{ 194, L"Deck 194" },
	{ 195, L"Deck 195" },
	{ 196, L"Deck 196" },
	{ 197, L"Deck 197" },
	{ 198, L"Deck 198" },
	{ 199, L"Deck 199" },
	{ 200, L"Deck 200" },
	{ 201, L"Deck 201" },
	{ 202, L"Deck 202" },
	{ 203, L"Deck 203" },
	{ 204, L"Deck 204" },
	{ 205, L"Deck 205" },
	{ 206, L"Deck 206" },
	{ 207, L"Deck 207" },
	{ 208, L"Deck 208" },
	{ 209, L"Deck 209" },
	{ 210, L"Deck 210" },
	{ 211, L"Deck 211" },
	{ 212, L"Deck 212" },
	{ 213, L"Deck 213" },
	{ 214, L"Deck 214" },
	{ 215, L"Deck 215" },
	{ 216, L"Deck 216" },
	{ 217, L"Deck 217" },
	{ 218, L"Deck 218" },
	{ 219, L"Deck 219" },
	{ 220, L"Deck 220" },
	{ 221, L"Deck 221" },
	{ 222, L"Deck 222" },
	{ 223, L"Deck 223" },
	{ 224, L"Deck 224" },
	{ 225, L"Deck 225" },
	{ 226, L"Deck 226" },
	{ 227, L"Deck 227" },
	{ 228, L"Deck 228" },
	{ 229, L"Deck 229" },
	{ 230, L"Deck 230" },
	{ 231, L"Deck 231" },
	{ 232, L"Deck 232" },
	{ 233, L"Deck 233" },
	{ 234, L"Deck 234" },
	{ 235, L"Deck 235" },
	{ 236, L"Deck 236" },
	{ 237, L"Deck 237" },
	{ 238, L"Deck 238" },
	{ 239, L"Deck 239" },
	{ 240, L"Deck 240" },
	{ 241, L"Deck 241" },
	{ 242, L"Deck 242" },
	{ 243, L"Deck 243" },
	{ 244, L"Deck 244" },
	{ 245, L"Deck 245" },
	{ 246, L"Deck 246" },
	{ 247, L"Deck 247" },
	{ 248, L"Deck 248" },
	{ 249, L"Deck 249" },
	{ 250, L"Deck 250" },
	{ 251, L"Deck 251" },
	{ 252, L"Deck 252" },
	{ 253, L"Deck 253" },
	{ 254, L"Deck 254" },
	{ 255, L"Deck 255" }
};

static SortedChoiceItem HADES_STRING_CARD_NAME[] = {
	{ 0, L"Goblin" },
	{ 1, L"Fang" },
	{ 2, L"Skeleton" },
	{ 3, L"Flan" },
	{ 4, L"Zaghnol" },
	{ 5, L"Lizard Man" },
	{ 6, L"Zombie" },
	{ 7, L"Bomb" },
	{ 8, L"Ironite" },
	{ 9, L"Sahagin" },
	{ 10, L"Yeti" },
	{ 11, L"Mimic" },
	{ 12, L"Wyerd" },
	{ 13, L"Mandragora" },
	{ 14, L"Crawler" },
	{ 15, L"Sand Scorpion" },
	{ 16, L"Nymph" },
	{ 17, L"Sand Golem" },
	{ 18, L"Zuu" },
	{ 19, L"Dragonfly" },
	{ 20, L"Carrion Worm" },
	{ 21, L"Cerberus" },
	{ 22, L"Antlion" },
	{ 23, L"Cactuar" },
	{ 24, L"Gimme Cat" },
	{ 25, L"Ragtime Mouse" },
	{ 26, L"Hedgehog Pie" },
	{ 27, L"Ralvuimago" },
	{ 28, L"Ochu" },
	{ 29, L"Troll" },
	{ 30, L"Blazer Beetle" },
	{ 31, L"Abomination" },
	{ 32, L"Zemzelett" },
	{ 33, L"Stroper" },
	{ 34, L"Tantarian" },
	{ 35, L"Grand Dragon" },
	{ 36, L"Feather Circle" },
	{ 37, L"Hecteyes" },
	{ 38, L"Ogre" },
	{ 39, L"Armstrong" },
	{ 40, L"Ash" },
	{ 41, L"Wraith" },
	{ 42, L"Gargoyle" },
	{ 43, L"Vepal" },
	{ 44, L"Grimlock" },
	{ 45, L"Tonberry" },
	{ 46, L"Veteran" },
	{ 47, L"Garuda" },
	{ 48, L"Malboro" },
	{ 49, L"Mover" },
	{ 50, L"Abadon" },
	{ 51, L"Behemoth" },
	{ 52, L"Iron Man" },
	{ 53, L"Nova Dragon" },
	{ 54, L"Ozma" },
	{ 55, L"Hades" },
	{ 56, L"Holy" },
	{ 57, L"Meteor" },
	{ 58, L"Flare" },
	{ 59, L"Shiva" },
	{ 60, L"Ifrit" },
	{ 61, L"Ramuh" },
	{ 62, L"Atomos" },
	{ 63, L"Odin" },
	{ 64, L"Leviathan" },
	{ 65, L"Bahamut" },
	{ 66, L"Ark" },
	{ 67, L"Fenrir" },
	{ 68, L"Madeen" },
	{ 69, L"Alexander" },
	{ 70, L"Excalibur II" },
	{ 71, L"Ultima Weapon" },
	{ 72, L"Masamune" },
	{ 73, L"Elixir" },
	{ 74, L"Dark Matter" },
	{ 75, L"Ribbon" },
	{ 76, L"Tiger Racket" },
	{ 77, L"Save the Queen" },
	{ 78, L"Genji" },
	{ 79, L"Mythril Sword" },
	{ 80, L"Blue Narciss" },
	{ 81, L"Hilda Garde III" },
	{ 82, L"Invincible" },
	{ 83, L"Cargo Ship" },
	{ 84, L"Hilda Garde I" },
	{ 85, L"Red Rose" },
	{ 86, L"Theater Ship" },
	{ 87, L"Viltgance" },
	{ 88, L"Chocobo" },
	{ 89, L"Fat Chocobo" },
	{ 90, L"Mog" },
	{ 91, L"Frog" },
	{ 92, L"Oglop" },
	{ 93, L"Alexandria" },
	{ 94, L"Lindblum" },
	{ 95, L"Twin Moons" },
	{ 96, L"Gargant" },
	{ 97, L"Namingway" },
	{ 98, L"Boco" },
	{ 99, L"Airship" }
};

//-- Texts
static SortedChoiceItem HADES_STRING_TEXT_BLOCK_NAME[] = {
	{ 0x0, L"Generic UI" },
	{ 0x1, L"Debug Room" },
	{ 0x2, L"Prima Vista" },
	{ 0x3, L"Alexandria Castle (1)" },
	{ 0x4, L"Evil Forest" },
	{ 0x7, L"Cleyra Trunk" },
	{ 0x8, L"Ice Cavern" },
	{ 0x12, L"Red Rose" },
	{ 0x16, L"Lindblum Castle (1)" },
	{ 0x17, L"Mist Gates" },
	{ 0x1E, L"Terra" },
	{ 0x1F, L"Mountain Path" },
	{ 0x20, L"Conde Petie" },
	{ 0x21, L"Alexandria (1)" },
	{ 0x25, L"Black Mage Village (1)" },
	{ 0x26, L"Mognet Central" },
	{ 0x28, L"Prima Vista (Ruin)" },
	{ 0x2A, L"Quan's Dwelling" },
	{ 0x2C, L"Cleyra" },
	{ 0x2F, L"Dali" },
	{ 0x32, L"Dali Underground" },
	{ 0x33, L"Gizamaluke Grotto" },
	{ 0x34, L"Bran Bal" },
	{ 0x35, L"Observatory Mountain" },
	{ 0x3F, L"Cargo Ship" },
	{ 0x44, L"World Map" },
	{ 0x46, L"Treno (1)" },
	{ 0x47, L"Qu's Marsh" },
	{ 0x4A, L"South Gate" },
	{ 0x4D, L"Burmecia" },
	{ 0x58, L"Alexandria Castle (3)" },
	{ 0x59, L"Alexander" },
	{ 0x5A, L"Alexandria (2)" },
	{ 0x5B, L"Alexandria (Night)" },
	{ 0x79, L"Shrines" },
	{ 0x7C, L"Oeilvert" },
	{ 0x86, L"Pinnacle Rocks" },
	{ 0xA6, L"Daguerreo" },
	{ 0xBA, L"Hilda Garde III" },
	{ 0xBB, L"Ending" },
	{ 0xBD, L"Invincible" },
	{ 0xDF, L"Treno & Supersoft" },
	{ 0x114, L"Lindblum (1)" },
	{ 0x121, L"Alexandria Castle (2)" },
	{ 0x122, L"Cleyra (Stormless)" },
	{ 0x158, L"Pandemonium" },
	{ 0x166, L"Madain Sari (1)" },
	{ 0x167, L"Gargan Roo" },
	{ 0x168, L"Madain Sari (2)" },
	{ 0x169, L"Fossil Roo" },
	{ 0x1E4, L"Mount Gulug" },
	{ 0x1E5, L"Lindblum (Occupied)" },
	{ 0x20D, L"Lindblum Castle (Occupied)" },
	{ 0x253, L"Lindblum (2)" },
	{ 0x2B6, L"Memoria" },
	{ 0x2E2, L"Iifa Tree Roots" },
	{ 0x2E3, L"Ipsen Castle" },
	{ 0x2E4, L"Desert Palace" },
	{ 0x2E5, L"Treno (2)" },
	{ 0x2F2, L"Alexandria Castle (Ruin)" },
	{ 0x38C, L"Esto Gaza" },
	{ 0x3AF, L"Lindblum Castle (2)" },
	{ 0x3B0, L"Iifa Tree" },
	{ 0x3B1, L"Chocobo Places" },
	{ 0x3B2, L"Alexandria (Ruin)" },
	{ 0x431, L"Black Mage Village (2)" },
	{ 0x1000, L"Charmap" }
};

// Should always be sorted by id
#define TEXT_OPCODE_FORMAT_TYPE 0x80 // 0x80 and next ones
static SortedChoiceItemTextOpcode HADES_STRING_TEXT_OPCODE[] = {
	{ 0x00, L"[0x00]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x01, L"[TIMED_END]", L"", L"Terminate the dialog after few seconds\nOnly use at the end of the string", 1, 1, new wstring[1]{ L"Time to wait" } },
	{ 0x02, L"[SLOW]", L"", L"Slow down the appearing rate\n1 is the slowest", 1, 1, new wstring[1]{ L"Slow ratio" } },
	{ 0x03, L"FORMAT:03", L"", L"Unknown Format Opcode", 3, TEXT_OPCODE_FORMAT_TYPE+1, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" } },
	{ 0x04, L"[TOKENIZE]", L"", L"Create tokens from the string to be used in other strings\nMust be the only opcode of the string", -1, 4, new wstring[2]{ L"Token amount", L"Tokens" } },
	{ 0x05, L"[0x05]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x06, L"[VAR_TOKEN]", L"Token", L"Replaced by a tokenized string from another string", 2, 2, new wstring[2]{ L"Text ID", L"Script ID" } },
	{ 0x07, L"FORMAT:07", L"", L"Unknown Format Opcode", 2, TEXT_OPCODE_FORMAT_TYPE+2, new wstring[2]{ L"Unknown", L"Unknown" } },
	{ 0x08, L"[INIT_MULTICHOICE]", L"", L"Each line becomes a choice inside the dialog", 0, 0, NULL },
	{ 0x09, L"[AUTO_END]", L"", L"Terminate the dialog after few seconds\nOnly use at the end of the string", 0, 0, NULL },
	{ 0x0A, L"[0x0A]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x0B, L"FORMAT:0B", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x0C, L"[DISPLAY_INSTANT]", L"", L"Instantatly display the whole string", 0, 0, NULL },
	{ 0x0D, L"FORMAT:0D", L"", L"Unknown Format Opcode", -1, TEXT_OPCODE_FORMAT_TYPE+4, NULL },
	{ 0x0E, L"[VAR_ITEM]", L"Item", L"Replaced by some item's name set by the game script", 1, 1, new wstring[1]{ L"Script ID" } },
	{ 0x0F, L"[PROGRESSION]", L"", L"Tell the dialog's step progression to the game script", 1, 1, new wstring[1]{ L"Step" } },
	{ 0x10, L"[ZIDANE]", L"Zidane", L"Replaced by Zidane's name", 0, 0, NULL },
	{ 0x11, L"[VIVI]", L"Vivi", L"Replaced by Vivi's name", 0, 0, NULL },
	{ 0x12, L"[DAGGER]", L"Dagger", L"Replaced by Dagger's name", 0, 0, NULL },
	{ 0x13, L"[STEINER]", L"Steiner", L"Replaced by Steiner's name", 0, 0, NULL },
	{ 0x14, L"[FREYA]", L"Freya", L"Replaced by Freya's name", 0, 0, NULL },
	{ 0x15, L"[QUINA]", L"Quina", L"Replaced by Quina's name", 0, 0, NULL },
	{ 0x16, L"[EIKO]", L"Eiko", L"Replaced by Eiko's name", 0, 0, NULL },
	{ 0x17, L"[AMARANT]", L"Amarant", L"Replaced by Amarant's name", 0, 0, NULL },
	{ 0x18, L"[PARTY_1]", L"Cinna", L"Replaced by the name of the external character", 0, 0, NULL },
	{ 0x19, L"[PARTY_2]", L"Marcus", L"Replaced by the name of the external character (2)", 0, 0, NULL },
	{ 0x1A, L"[PARTY_3]", L"Blank", L"Replaced by the name of the external character (3)", 0, 0, NULL },
	{ 0x1B, L"[PARTY_4]", L"Beatrix", L"Replaced by the name of the external character (4)", 0, 0, NULL },
	{ 0x1C, L"[0x1C]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x1D, L"[0x1D]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x1E, L"[0x1E]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x1F, L"[0x1F]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x20, L"[WHITE]", L"", L"Change the color to white", 0, 0, NULL },
	{ 0x21, L"[WHITE_NO_SHADOW]", L"", L"Change the color to white with no shadow", 0, 0, NULL },
	{ 0x22, L"[GREY]", L"", L"Change the color to grey", 0, 0, NULL },
	{ 0x23, L"[RED]", L"", L"Change the color to red", 0, 0, NULL },
	{ 0x24, L"[YELLOW]", L"", L"Change the color to yellow", 0, 0, NULL },
	{ 0x25, L"[CYAN]", L"", L"Change the color to cyan", 0, 0, NULL },
	{ 0x26, L"[MAUVE]", L"", L"Change the color to mauve", 0, 0, NULL },
	{ 0x27, L"[GREEN]", L"", L"Change the color to green", 0, 0, NULL },
	{ 0x28, L"[BLACK_HELP]", L"", L"Change the color to black\nHelp dialogs only", 0, 0, NULL },
	{ 0x29, L"[RED_HELP]", L"", L"Change the color to red\nHelp dialogs only", 0, 0, NULL },
	{ 0x2A, L"[0x2A]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2B, L"[0x2B]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2C, L"[0x2C]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2D, L"[0x2D]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2E, L"[0x2E]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2F, L"[BUTTON_PAD]", L"Pad", L"Replaced by a directional cross image", 0, 0, NULL },
	{ 0x30, L"[BUTTON_CIRCLE]", L"Circle", L"Replaced by a Circle button", 0, 0, NULL },
	{ 0x31, L"[BUTTON_CROSS]", L"Cross", L"Replaced by a Cross button", 0, 0, NULL },
	{ 0x32, L"[BUTTON_TRIANGLE]", L"Triangle", L"Replaced by a Triangle button", 0, 0, NULL },
	{ 0x33, L"[BUTTON_SQUARE]", L"Square", L"Replaced by a Square button", 0, 0, NULL },
	{ 0x34, L"[BUTTON_L1]", L"L1", L"Replaced by a L1 button", 0, 0, NULL },
	{ 0x35, L"[BUTTON_R1]", L"R1", L"Replaced by a R1 button", 0, 0, NULL },
	{ 0x36, L"[BUTTON_L2]", L"L2", L"Replaced by a L2 button", 0, 0, NULL },
	{ 0x37, L"[BUTTON_R2]", L"R2", L"Replaced by a R2 button", 0, 0, NULL },
	{ 0x38, L"[BUTTON_START]", L"Start", L"Replaced by a Start button", 0, 0, NULL },
	{ 0x39, L"[BUTTON_SELECT]", L"Select", L"Replaced by a Select button", 0, 0, NULL },
	{ 0x3A, L"[BUTTON_UP]", L"Up", L"Replaced by a Up button", 0, 0, NULL },
	{ 0x3B, L"[BUTTON_DOWN]", L"Down", L"Replaced by a Down button", 0, 0, NULL },
	{ 0x3C, L"[BUTTON_LEFT]", L"Left", L"Replaced by a Left button", 0, 0, NULL },
	{ 0x3D, L"[BUTTON_RIGHT]", L"Right", L"Replaced by a Right button", 0, 0, NULL },
	{ 0x3E, L"[BUTTON_L2]", L"L2", L"Replaced by a L2 button", 0, 0, NULL },
	{ 0x3F, L"[BUTTON_R2]", L"R2", L"Replaced by a R2 button", 0, 0, NULL },
	{ 0x40, L"[VAR_NUM]", L"0", L"Replaced by some number set by the game script", 1, 1, new wstring[1]{ L"Script ID" } },
	{ 0x41, L"[PAUSE]", L"", L"Stop the appearance for few seconds", 1, 1, new wstring[1]{ L"Frame to wait" } },
	{ 0x42, L"[RESUME_SPEED]", L"", L"Resume the appearing rate", 0, 0, NULL },
	{ 0x43, L"[CARET_POSITION_REL]", L"", L"Change the caret position in the line", 1, 1, new wstring[1]{ L"Relative position" } },
	{ 0x44, L"[CARET_POSITION_ABS]", L"", L"Change the caret position in the line", 1, 1, new wstring[1]{ L"Absolute position" } },
	{ 0x45, L"[0x45]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x46, L"[0x46]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x47, L"[0x47]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x48, L"[0x48]", L"", L"Unknown Opcode", -1, 5, new wstring[1]{ L"Amount" } },
	{ 0x49, L"[0x49]", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x4A, L"[0x4A]", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x4B, L"[IMAGE]", L"", L"Replaced by an image", 1, 1, new wstring[1]{ L"Image ID" } },
	{ 0x4C, L"[ICON_ITEM]", L"", L"Replaced by the icon of an item", 1, 1, new wstring[1]{ L"Item ID" } },
	{ 0x4D, L"[RAM_STATUS]", L"", L"Replaced by a status icon - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x4E, L"[RAM_EQUIPMENT]", L"", L"Replaced by an equipment icon - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x4F, L"FORMAT:4F", L"", L"Unknown Format Opcode", 2, TEXT_OPCODE_FORMAT_TYPE+2, new wstring[2]{ L"Unknown", L"Unknown" } },
	{ 0x50, L"FORMAT:50", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x51, L"FORMAT:51", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x52, L"FORMAT:52", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x53, L"FORMAT:53", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x54, L"FORMAT:54", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x55, L"FORMAT:55", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x56, L"[0x56]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x57, L"[0x57]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x58, L"FORMAT:Switch Up/Down", L"", L"Display the bubble over or under the talker", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x59, L"FORMAT:59", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5A, L"FORMAT:5A", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5B, L"FORMAT:5B", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5C, L"FORMAT:5C", L"", L"Unknown Format Opcode", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5D, L"[0x5D]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x5E, L"[0x5E]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x5F, L"FORMAT:Talker Gap", L"", L"Add a gap from the talker's position", 6, TEXT_OPCODE_FORMAT_TYPE+3, NULL },
	{ 0x60, L"[BUTTONEX_CIRCLE]", L"Circle", L"Replaced by the confirm button", 0, 0, NULL },
	{ 0x61, L"[BUTTONEX_CROSS]", L"Cross", L"Replaced by the cancel button", 0, 0, NULL },
	{ 0x62, L"[BUTTONEX_TRIANGLE]", L"Triangle", L"Replaced by the menu button", 0, 0, NULL },
	{ 0x63, L"[BUTTONEX_SQUARE]", L"Square", L"Replaced by the moogle button", 0, 0, NULL },
	{ 0x64, L"[BUTTONEX_L1]", L"L1", L"Replaced by the button associated to L1", 0, 0, NULL },
	{ 0x65, L"[BUTTONEX_R1]", L"R1", L"Replaced by the button associated to R1", 0, 0, NULL },
	{ 0x66, L"[BUTTONEX_L2]", L"L2", L"Replaced by the button associated to L2", 0, 0, NULL },
	{ 0x67, L"[BUTTONEX_R2]", L"R2", L"Replaced by the button associated to R2", 0, 0, NULL },
	{ 0x68, L"[RAM_NUM]", L"0", L"Replaced by a number - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x69, L"[RAM_TEXT]", L"Text", L"Replaced by some text - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6A, L"[0x6A]", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6B, L"[RAM_DATA]", L"", L"Replaced by a character's data value - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6C, L"[RAM_NUM_DESIGN]", L"0", L"Replaced by a designed number - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6D, L"[0x6D]", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6E, L"[0x6E]", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6F, L"[0x6F]", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x70, L"[IMAGE_NEW]", L"!New", L"Possibly replaced by a 'New' indicator", 1, 1, new wstring[1]{ L"Script ID" } },
	{ 0x71, L"[INDENT]", L"", L"Same as CARET_POSITION_ABS(18)", 0, 0, NULL },
	{ 0x72, L"[0x72]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x73, L"[0x73]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x74, L"[0x74]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x75, L"[0x75]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x76, L"[0x76]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x77, L"[0x77]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x78, L"[0x78]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x79, L"[0x79]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7A, L"[0x7A]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7B, L"[0x7B]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7C, L"[0x7C]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7D, L"[0x7D]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7E, L"[0x7E]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7F, L"[0x7F]", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x80, L"[0x80]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x81, L"[0x81]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x82, L"[0x82]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x83, L"[0x83]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x84, L"[0x84]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x85, L"[0x85]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x86, L"[0x86]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x87, L"[0x87]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x88, L"[0x88]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x89, L"[0x89]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x8A, L"[0x8A]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x8B, L"[0x8B]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x8C, L"[0x8C]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x8D, L"[0x8D]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x8E, L"[0x8E]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x8F, L"[0x8F]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x90, L"[0x90]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x91, L"[0x91]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x92, L"[0x92]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x93, L"[0x93]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x94, L"[0x94]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x95, L"[0x95]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x96, L"[0x96]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x97, L"[0x97]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x98, L"[0x98]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x99, L"[0x99]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x9A, L"[0x9A]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x9B, L"[0x9B]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x9C, L"[0x9C]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x9D, L"[0x9D]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x9E, L"[0x9E]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0x9F, L"[0x9F]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA0, L"[0xA0]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA1, L"[0xA1]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA2, L"[0xA2]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA3, L"[0xA3]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA4, L"[0xA4]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA5, L"[0xA5]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA6, L"[0xA6]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA7, L"[0xA7]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA8, L"[0xA8]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xA9, L"[0xA9]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xAA, L"[0xAA]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xAB, L"[0xAB]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xAC, L"[0xAC]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xAD, L"[0xAD]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xAE, L"[0xAE]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xAF, L"[0xAF]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB0, L"[0xB0]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB1, L"[0xB1]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB2, L"[0xB2]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB3, L"[0xB3]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB4, L"[0xB4]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB5, L"[0xB5]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB6, L"[0xB6]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB7, L"[0xB7]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB8, L"[0xB8]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xB9, L"[0xB9]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xBA, L"[0xBA]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xBB, L"[0xBB]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xBC, L"[0xBC]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xBD, L"[0xBD]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xBE, L"[0xBE]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xBF, L"[0xBF]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC0, L"[0xC0]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC1, L"[0xC1]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC2, L"[0xC2]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC3, L"[0xC3]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC4, L"[0xC4]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC5, L"[0xC5]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC6, L"[0xC6]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC7, L"[0xC7]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC8, L"[0xC8]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xC9, L"[0xC9]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xCA, L"[0xCA]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xCB, L"[0xCB]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xCC, L"[0xCC]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xCD, L"[0xCD]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xCE, L"[0xCE]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xCF, L"[0xCF]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD0, L"[0xD0]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD1, L"[0xD1]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD2, L"[0xD2]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD3, L"[0xD3]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD4, L"[0xD4]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD5, L"[0xD5]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD6, L"[0xD6]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD7, L"[0xD7]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD8, L"[0xD8]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xD9, L"[0xD9]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xDA, L"[0xDA]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xDB, L"[0xDB]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xDC, L"[0xDC]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xDD, L"[0xDD]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xDE, L"[0xDE]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xDF, L"[0xDF]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE0, L"[0xE0]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE1, L"[0xE1]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE2, L"[0xE2]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE3, L"[0xE3]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE4, L"[0xE4]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE5, L"[0xE5]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE6, L"[0xE6]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE7, L"[0xE7]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE8, L"[0xE8]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xE9, L"[0xE9]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xEA, L"[0xEA]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xEB, L"[0xEB]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xEC, L"[0xEC]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xED, L"[0xED]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xEE, L"[0xEE]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xEF, L"[0xEF]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF0, L"[0xF0]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF1, L"[0xF1]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF2, L"[0xF2]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF3, L"[0xF3]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF4, L"[0xF4]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF5, L"[0xF5]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF6, L"[0xF6]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF7, L"[0xF7]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF8, L"[0xF8]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xF9, L"[0xF9]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xFA, L"[0xFA]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xFB, L"[0xFB]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xFC, L"[0xFC]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xFD, L"[0xFD]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xFE, L"[0xFE]", L"", L"Unknown Opcode", 0, -1, NULL },
	{ 0xFF, L"[0xFF]", L"", L"Unknown Opcode", 0, -1, NULL }
};

//-- World Maps
static SortedChoiceItem HADES_STRING_WORLD_BLOCK_NAME[] = {
	{ 9000, L"World Map: No Transport" },
	{ 9001, L"World Map/Event: Cargo Ship" },
	{ 9002, L"World Map: Dagger" },
	{ 9003, L"World Map: Blue Narciss" },
	{ 9004, L"World Map/Event: Hilda Garde 1" },
	{ 9005, L"World Map: Hilda Garde 1" },
	{ 9006, L"World Map/Event: Track Kuja" },
	{ 9007, L"World Map: Hilda Garde 3" },
	{ 9008, L"World Map: Invincible" },
	{ 9009, L"World Map: ???" },
	{ 9010, L"World Map: Flying Airships" },
	{ 9011, L"World Map: Around Dali" },
	{ 9012, L"World Map/Event: Chocobo Treasure" },
	{ 9100, L"14th World Map" },
	{ 9101, L"15th World Map" }
};

//-- Fields
#define AT_NONE				0
#define AT_JUMP				1
#define AT_SPIN				2
#define AT_USPIN			3
#define AT_BOOL				4
#define AT_BOOLLIST			5
#define AT_TEXT				6
#define AT_BATTLE			7
#define AT_FIELD			8
#define AT_DISC_FIELD		9
#define AT_ATTACK			10
#define AT_ITEM				11
#define AT_MENUTYPE			12
#define AT_MENU				13
#define AT_CHARACTER		14
#define AT_LCHARACTER		15
#define AT_ABILITYSET		16
#define AT_EQUIPSET			17
#define AT_FMV				18
#define AT_ENTRY			19
#define AT_FUNCTION			20
#define AT_MODEL			21
#define AT_SOUND			22
#define AT_AUDIO			23
#define AT_BUBBLESYMBOL		24
#define AT_WORLDMAP			25
#define AT_TILE				26
#define AT_TILEANIM			27
#define AT_ABILITY			28
#define AT_BATTLECODE		29
#define AT_WALKPATH			30
#define AT_WALKTRIANGLE		31
#define AT_POSITION_X		32
#define AT_POSITION_Y		33
#define AT_POSITION_Z		34
#define AT_MODELCODE		35
#define AT_WORLDCODE		36
#define AT_SOUNDCODE		37
#define AT_SPSCODE			38
#define AT_ANIMATION		40
#define AT_ANIMFLAG			41
#define AT_DECK				42
#define AT_SCRIPTLVL		43
#define AT_COLOR_CYAN		50
#define AT_COLOR_MAGENTA	51
#define AT_COLOR_YELLOW		52
#define AT_COLOR_RED		53
#define AT_COLOR_GREEN		54
#define AT_COLOR_BLUE		55
// Should always be sorted by id
static SortedChoiceItemScriptOpcode HADES_STRING_SCRIPT_OPCODE[] = {
	{ 0x00, L"NOTHING", L"Do nothing.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x01, L"JMP", L"Skip some operations.\nWARNING : unsafe to use.", false, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Jump" }, new uint8_t[1]{ AT_JUMP }, 3 },
	{ 0x02, L"JMP_IFN", L"Skip some operations if the stack value is not 0.\nWARNING : unsafe to use.", false, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Jump" }, new uint8_t[1]{ AT_JUMP }, 3 },
	{ 0x03, L"JMP_IF", L"Skip some operations if the stack value is 0.\nWARNING : unsafe to use.", false, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Jump" }, new uint8_t[1]{ AT_JUMP }, 3 },
	{ 0x04, L"return", L"End the function.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x05, L"set", L"Perform variable operations and store the result in the stack.", false, 1, new uint8_t[1]{ 0 }, new wstring[1]{ L"Variable Code" }, new uint8_t[1]{ AT_NONE }, 0 },
	{ 0x06, L"JMP_SWITCHEX", L"Skip some operations depending on the stack value.\nWARNING : unsafe to use.", false, -1, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Default Jump", L"Case", L"Jump" }, new uint8_t[3]{ AT_JUMP, AT_SPIN, AT_JUMP }, 0 },
	{ 0x07, L"InitCode", L"Init a normal code (independant functions).\n\n1st argument : code entry to init.\n2nd argument : Unique ID (defaulted to entry's ID if 0).", false, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"UID" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x08, L"InitRegion", L"Init a region code (associated with a region).\n\n1st argument : code entry to init.\n2nd argument : Unique ID (defaulted to entry's ID if 0).", false, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"UID" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x09, L"InitObject", L"Init an object code (associated with a 3D model). Also load the related model into the RAM.\n\n1st argument : code entry to init.\n2nd argument : Unique ID (defaulted to entry's ID if 0).", false, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"UID" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x0A, L"0x0A", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x0B, L"JMP_SWITCH", L"Skip some operations depending on the stack value.\nWARNING : unsafe to use.", false, -1, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Starting Value", L"Default Jump", L"Jump" }, new uint8_t[3]{ AT_USPIN, AT_JUMP, AT_JUMP }, 1 },
	{ 0x0C, L"0x0C", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x0D, L"0x0D", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Steam seem to handle it like a JMP_SWITCH with a short instead of a char
	{ 0x0E, L"0x0E", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x0F, L"0x0F", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x10, L"RunScriptAsync", L"Run script function and continue executing the current one.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument : script level.\n2nd argument : entry of the function.\n3rd argument : function.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Script Level", L"Entry", L"Function" }, new uint8_t[3]{ AT_SCRIPTLVL, AT_ENTRY, AT_FUNCTION }, 0 },
	{ 0x11, L"0x11", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x12, L"RunScript", L"Wait until the entry's script level gets higher than the specified script level then run the script function and continue executing the current one.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument : script level.\n2nd argument : entry of the function.\n3rd argument : function.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Script Level", L"Entry", L"Function" }, new uint8_t[3]{ AT_SCRIPTLVL, AT_ENTRY, AT_FUNCTION }, 0 },
	{ 0x13, L"0x13", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x14, L"RunScriptSync", L"Wait until the entry's script level gets higher than the specified script level then run the script function and wait until it returns.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument : script level.\n2nd argument : entry of the function.\n3rd argument : function.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Script Level", L"Entry", L"Function" }, new uint8_t[3]{ AT_SCRIPTLVL, AT_ENTRY, AT_FUNCTION }, 0 },
	{ 0x15, L"0x15", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x16, L"RunScriptObjectAsync", L"Run script function and continue executing the current one. Must only be used in response to a function call ; the argument's entry is the one that called this function.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument : script level.\n2nd argument : function.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Script Level", L"Function" }, new uint8_t[2]{ AT_SCRIPTLVL, AT_FUNCTION }, 0 },
	{ 0x17, L"0x17", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x18, L"RunScriptObject", L"Wait until the entry's script level gets higher than the specified script level then run the script function and continue executing the current one. Must only be used in response to a function call ; the argument's entry is the one that called this function.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument : script level.\n2nd argument : function.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Script Level", L"Function" }, new uint8_t[2]{ AT_SCRIPTLVL, AT_FUNCTION }, 0 },
	{ 0x19, L"0x19", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x1A, L"RunScriptObjectSync", L"Wait until the entry's script level gets higher than the specified script level then run the script function and wait until it returns. Must only be used in response to a function call ; the argument's entry is the one that called this function.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument : script level.\n2nd argument : function.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Script Level", L"Function" }, new uint8_t[2]{ AT_SCRIPTLVL, AT_FUNCTION }, 0 },
	{ 0x1B, L"ContinueBattleMusic", L"Continue the music after the battle end.\n\n1st argument : flag continue/don't continue.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Continue" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0x1C, L"TerminateEntry", L"Stop the execution of an entry's code.\n\n1st argument : entry to terminate.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x1D, L"CreateObject", L"Place (or replace) the 3D model on the field.\n\n1st and 2nd arguments : position in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Position" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x1E, L"SetCameraBounds", L"Redefine the field camera boundaries (default value is part of the background's data).\n\n1st argument : camera ID.\n2nd to 5th arguments : Boundaries in (MinX, MaxX, MinY, MaxY) format.", true, 5, new uint8_t[5]{ 1, 2, 2, 2, 2 }, new wstring[5]{ L"Camera", L"Min X", L"Max X", L"Min Y", L"Max Y" }, new uint8_t[5]{ AT_USPIN, AT_SPIN, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x1F, L"WindowSync", L"Display a window with text inside and wait until it closes.\n\n1st argument : window ID.\n2nd argument : UI flag list.\n 3 : disable bubble tail\n 4 : mognet format\n 5 : hide window\n 7 : ATE window\n 8 : dialog window\n3rd argument : text to display.", true, 3, new uint8_t[3]{ 1, 1, 2 }, new wstring[3]{ L"Window ID", L"UI", L"Text" }, new uint8_t[3]{ AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x20, L"WindowAsync", L"Display a window with text inside and continue the execution of the script without waiting.\n\n1st argument : window ID.\n2nd argument : UI flag list.\n 3 : disable bubble tail\n 4 : mognet format\n 5 : hide window\n 7 : ATE window\n 8 : dialog window\n3rd argument : text to display.", true, 3, new uint8_t[3]{ 1, 1, 2 }, new wstring[3]{ L"Window ID", L"UI", L"Text" }, new uint8_t[3]{ AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x21, L"CloseWindow", L"Close a window.\n\n1st argument : window ID determined at its creation.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Window ID" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x22, L"Wait", L"Wait some time.\n\n1st argument : amount of frames to wait.\n For PAL, 1 frame is 0.04 seconds.\n For other versions, 1 frame is about 0.033 seconds.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Frame Amount" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x23, L"Walk", L"Make the character walk to destination. Make it synchronous if InitWalk is called before.\n\n1st and 2nd arguments : destination in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x24, L"WalkTowardObject", L"Make the character walk and follow an object.\n\n1st argument : object to walk toward.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x25, L"InitWalk", L"Make a further Walk call (or variations of Walk) synchronous.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x26, L"SetWalkSpeed", L"Change the walk speed.\n\n1st argument : speed (surely in unit/frame).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x27, L"0x27", L"Seems to do something with the background.\n\n1st argument : flags (unknown).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0x28, L"Cinematic", L"Run or setup a cinematic.\n\n1st argument : unknown.\n2nd argument : cinematic ID (may depends on 1st argument's value).\n3rd argument : unknown.\n4th argument : unknown.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Unknown", L"Cinematic ID", L"Unknown", L"Unknown" }, new uint8_t[4]{ AT_SPIN, AT_FMV, AT_SPIN, AT_SPIN }, 0 },
	{ 0x29, L"SetRegion", L"Define the polygonal region linked with the entry script.\n\nArguments are in the format (Vertice X, Vertice Y) and can be of any amount.", true, -1, new uint8_t[1]{ 4 }, new wstring[1]{ L"Polygon" }, new uint8_t[1]{ AT_NONE }, 0 },
	{ 0x2A, L"Battle", L"Start a battle (using a random enemy group).\n\n1st argument : rush type (unknown).\n2nd argument : gathered battle and Steiner's state (highest bit) informations.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Rush Type", L"Battle" }, new uint8_t[2]{ AT_SPIN, AT_BATTLE }, 0 },
	{ 0x2B, L"Field", L"Change the field scene.\n\n1st argument : field scene destination.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Field" }, new uint8_t[1]{ AT_FIELD }, 0 },
	{ 0x2C, L"DefinePlayerCharacter", L"Apply the player's control over the entry's object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x2D, L"DisableMove", L"Disable the player's movement control.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x2E, L"EnableMove", L"Enable the player's movement control.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x2F, L"SetModel", L"Set the model of the object and its head's height (used to set the dialog box's height).\n\n1st argument : model.\n2nd argument : head's height.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Model", L"Height" }, new uint8_t[2]{ AT_MODEL, AT_USPIN }, 0 },
	{ 0x30, L"0x30", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0x31, L"0x31", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0x32, L"0x32", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x33, L"SetStandAnimation", L"Change the standing animation.\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x34, L"SetWalkAnimation", L"Change the walking animation.\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x35, L"SetRunAnimation", L"Change the running animation.\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x36, L"TurnInstant", L"Make the character face an angle.\n\n1st argument : angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Angle" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x37, L"SetPitchAngle", L"Turn the model in the up/down direction.\n\n1st argument : angle (pitch axis).\n2nd argument : angle (XZ axis).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Pitch", L"XZ Angle" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x38, L"Attack", L"Make the enemy attack. The target(s) are to be set using the SV_Target variable.\nInside an ATB function, the attack is added to the queue.\nInside a counter function, the attack occurs directly.\n\n1st argument : attack to perform.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Attack" }, new uint8_t[1]{ AT_ATTACK }, 0 },
	{ 0x39, L"ShowObject", L"Show an object.\n\n1st argument : object.\n2nd argument : unknown.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Unknown" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x3A, L"HideObject", L"Hide an object.\n\n1st argument : object.\n2nd argument : unknown.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Unknown" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x3B, L"SetObjectIndex", L"Redefine the current object's index.\n\n1st argument : new index.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Index" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x3C, L"SetRandomBattles", L"Define random battles.\n\n1st argument : pattern, deciding the encounter chances and the topography (World Map only).\n 0 : {0.375, 0.28, 0.22, 0.125}\n 1 : {0.25, 0.25, 0.25, 0.25}\n 2 : {0.35, 0.3, 0.3, 0.05}\n 3 : {0.45, 0.4, 0.1, 0.05}\n2nd to 5th arguments : random battles possible.", true, 5, new uint8_t[5]{ 1, 2, 2, 2, 2 }, new wstring[5]{ L"Pattern", L"Battle 1", L"Battle 2", L"Battle 3", L"Battle 4" }, new uint8_t[5]{ AT_USPIN, AT_BATTLE, AT_BATTLE, AT_BATTLE, AT_BATTLE }, 0 },
	{ 0x3D, L"0x3D", L"Some animation flags.\n\n1st argument : unknown.\n2nd argument : unknown.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"In-Frame",  L"Out-Frame" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x3E, L"SetAnimationSpeed", L"Set the current object's animation speed.\n\n1st argument : speed.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x3F, L"SetAnimationFlags", L"Set the current object's next animation looping flags.\n\n1st argument : looping flag list.\n 1 : freeze at end\n 2 : loop\n 3 : loop back and forth\n2nd arguments : times to repeat.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Flag", L"Repeat" }, new uint8_t[2]{ AT_ANIMFLAG, AT_USPIN }, 0 },
	{ 0x40, L"RunAnimation", L"Make the character play an animation.\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x41, L"WaitAnimation", L"Wait until the current object's animation has ended.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x42, L"StopAnimation", L"Stop the character's animation.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x43, L"RunSharedScript", L"Run script passing the current object to it and continue executing the current function. If another shared script is already running for this object, it will be terminated.\n\n1st argument : entry (should be a one-function entry).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Entry" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x44, L"WaitSharedScript", L"Wait until the ran shared script has ended.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x45, L"StopSharedScript", L"Terminate the execution of the ran shared script.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x46, L"0x46", L"No use.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x47, L"EnableHeadFocus", L"Enable or disable the character turning his head toward an active object.\n\n1st argument : flags.\n 1 : unknown\n 2 : unknown\n 3 : turn toward talkers", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0x48, L"AddItem", L"Add item to the player's inventory. Only one copy of key items can be in the player's inventory.\n\n1st argument : item to add.\n2nd argument : amount to add.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Item", L"Amount" }, new uint8_t[2]{ AT_ITEM, AT_USPIN }, 0 },
	{ 0x49, L"RemoveItem", L"Remove item from the player's inventory.\n\n1st argument : item to remove.\n2nd argument : amount to remove.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Item", L"Amount" }, new uint8_t[2]{ AT_ITEM, AT_USPIN }, 0 },
	{ 0x4A, L"RunBattleCode", L"Run a battle code.\n\n1st argument : battle code.\n2nd argument : depends on the battle code.\n End Battle : 0 for a defeat (deprecated), 1 for a victory, 2 for a victory without victory pose, 3 for a defeat, 4 for an escape, 5 for an interrupted battle, 6 for a game over, 7 for an enemy escape.\n Run Camera : Camera ID.\n Change Field : ID of the destination field after the battle.\n Add Gil : amount to add.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Code", L"Argument" }, new uint8_t[2]{ AT_BATTLECODE, AT_SPIN }, 0 },
	{ 0x4B, L"SetObjectLogicalSize", L"Set different size informations of the object.\n\n1st argument : size for pathing.\n2nd argument : collision radius.\n3rd argument : talk distance.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Walk Radius", L"Collision Radius", L"Talk Distance" }, new uint8_t[3]{ AT_SPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0x4C, L"AttachObject", L"Attach an object to another one.\n\n1st argument : carried object.\n2nd argument : carrying object.\n3rd argument : attachment point (unknown format).", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Object", L"Carrier", L"Attachement Point" }, new uint8_t[3]{ AT_ENTRY, AT_ENTRY, AT_USPIN }, 0 },
	{ 0x4D, L"DetachObject", L"Stop attaching an object to another one.\n\n1st argument : carried object.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x4E, L"0x4E", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // WATCH & STOP
	{ 0x4F, L"0x4F", L"Unknown Opcode.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0x50, L"WaitTurn", L"Wait until the character has turned.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x51, L"TurnTowardObject", L"Turn the character toward an entry object (animated).\n\n1st argument : object.\n2nd argument : turn speed (1 is slowest).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Speed" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x52, L"SetInactiveAnimation", L"Change the animation played when inactive for a long time. The inaction time required is :\nFirst Time = 200 + 4 * Random[0, 255]\Following Times = 200 + 2 * Random[0, 255]\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x53, L"0x53", L"Seems to prevent new windows to close older ones.", false, 0, NULL, NULL, NULL, 0 }, // NOINITMES
	{ 0x54, L"WaitWindow", L"Wait until the window is closed.\n\n1st argument : window ID determined at its creation.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Window ID" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x55, L"SetWalkTurnSpeed", L"Change the turn speed of the object when it walks or runs (default is 16).\n\n1st argument : turn speed (with 0, the object doesn't turn while moving).\n\nSpecial treatments:\nVivi's in Iifa Tree/Eidolon Moun (field 1656) is initialized to 48.\nChoco's in Chocobo's Paradise (field 2954) is initialized to 96.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Turn Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x56, L"TimedTurn", L"Make the character face an angle (animated).\n\n1st argument : angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.\n2nd argument : turn speed (1 is slowest).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Angle", L"Speed" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x57, L"SetRandomBattleFrequency", L"Set the frequency of random battles.\n\n1st argument : frequency.\n 255 is the maximum frequency, corresponding to ~12 walking steps or ~7 running steps.\n 0 is the minimal frequency and disables random battles.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Frequency" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x58, L"SlideXZY", L"Make the character slide to destination (walk without using the walk animation and without changing the facing angle).\n\n1st to 3rd arguments : destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0x59, L"SetTileColor", L"Change the color of a field tile block.\n\n1st argument : background tile block.\n2nd to 4th arguments : color in (Cyan, Magenta, Yellow) format.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[2]{ L"Tile Block", L"Color" }, new uint8_t[4]{ AT_TILE, AT_COLOR_CYAN, AT_COLOR_MAGENTA, AT_COLOR_YELLOW }, 0 },
	{ 0x5A, L"SetTilePositionEx", L"Move a field tile block.\n\n1st argument : background tile block.\n2nd and 3rd argument : position in (X, Y) format.\n4th argument : closeness, defining whether 3D models are over or under that background tile.", true, 4, new uint8_t[4]{ 1, 2, 2, 2 }, new wstring[4]{ L"Tile Block", L"Position X", L"Position Y", L"Position Closeness" }, new uint8_t[4]{ AT_TILE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5B, L"ShowTile", L"Show or hide a field tile block.\n\n1st argument : background tile block.\n2nd argument : boolean show/hide.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Block", L"Show" }, new uint8_t[2]{ AT_TILE, AT_BOOL }, 0 },
	{ 0x5C, L"MoveTileLoop", L"Make the image of a field tile loop over space.\n\n1st argument : background tile block.\n2nd argument : unknown.\n3rd and 4th arguments : something to do with looping in the X and Y direction.", true, 4, new uint8_t[4]{ 1, 1, 2, 2 }, new wstring[4]{ L"Tile Block", L"Unknown", L"X Loop", L"Y Loop" }, new uint8_t[4]{ AT_TILE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5D, L"MoveTile", L"Make the field moves depending on the camera position.\n\n1st argument : background tile block.\n2nd argument : unknown.\n3rd and 4th arguments : movement in (X, Y) format.", true, 4, new uint8_t[4]{ 1, 1, 2, 2 }, new wstring[4]{ L"Tile Block", L"Unknown", L"Movement X", L"Movement Y" }, new uint8_t[4]{ AT_TILE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5E, L"SetTilePosition", L"Move a field tile block.\n\n1st argument : background tile block.\n2nd and 3rd argument : position in (X, Y) format.", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Tile Block", L"Position X", L"Position Y" }, new uint8_t[3]{ AT_TILE, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5F, L"RunTileAnimation", L"Run a field tile animation.\n\n1st argument : background animation.\n2nd argument : starting frame.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Field Animation", L"Frame" }, new uint8_t[2]{ AT_TILEANIM, AT_USPIN }, 0 },
	{ 0x60, L"ActivateTileAnimation", L"Make a field tile animation active.\n\n1st argument : background animation.\n2nd argument : boolean on/off.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Animation", L"Activate" }, new uint8_t[2]{ AT_TILEANIM, AT_BOOL }, 0 },
	{ 0x61, L"SetTileAnimationSpeed", L"Change the speed of a field tile animation.\n\n1st argument : background animation.\n2nd argument : speed.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Tile Animation", L"Speed" }, new uint8_t[2]{ AT_TILEANIM, AT_SPIN }, 0 },
	{ 0x62, L"SetRow", L"Change the battle row of a party member.\n\n1st argument : party member.\n2nd argument : boolean front/back.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Row" }, new uint8_t[2]{ AT_LCHARACTER, AT_BOOL }, 0 },
	{ 0x63, L"SetTileAnimationPause", L"Make a field tile animation pause at some frame.\n\n1st argument : background animation.\n2nd argument : frame.\n3rd argument : wait time.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Tile Animation", L"Frame ID", L"Time" }, new uint8_t[3]{ AT_TILEANIM, AT_USPIN, AT_USPIN }, 0 },
	{ 0x64, L"SetTileAnimationFlags", L"Change the flags of a field tile animation.\n\n1st argument : background animation.\n2nd argument : flags (unknown).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Animation", L"Flags" }, new uint8_t[2]{ AT_TILEANIM, AT_BOOLLIST }, 0 },
	{ 0x65, L"RunTileAnimationEx", L"Run a field tile animation and choose its frame range.\n\n1st argument : background animation.\n2nd argument : starting frame.\n3rd argument : ending frame.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Tile Animation", L"Start", L"End" }, new uint8_t[3]{ AT_TILEANIM, AT_USPIN, AT_USPIN }, 0 },
	{ 0x66, L"SetTextVariable", L"Set the value of a text number or item variable.\n\n1st argument : text variable's \"Script ID\".\n2nd argument : depends on which text opcode is related to the text variable.\n For [VAR_NUM] : integral value.\n For [VAR_ITEM] : item ID.\n For [VAR_TOKEN] : token number.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Variable ID", L"Value" }, new uint8_t[2]{ AT_USPIN, AT_ITEM }, 0 },
	{ 0x67, L"SetControlDirection", L"Set the angles for the player's movement control.\n\n1st argument : angle used for arrow movements.\n2nd argument : angle used for analogic stick movements.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Arrow Angle", L"Analogic Angle" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x68, L"Bubble", L"Display a speech bubble with a symbol inside over the head of player's character.\n\n1st argument : bubble symbol.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Symbol" }, new uint8_t[1]{ AT_BUBBLESYMBOL }, 0 },
	{ 0x69, L"ChangeTimerTime", L"Change the remaining time of the timer window.\n\n1st argument : time in seconds.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Time" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x6A, L"DisableRun", L"Make the player's character always walk.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6B, L"SetBackgroundColor", L"Change the default color, seen behind the field's tiles.\n\n1st to 3rd arguments : color in (Red, Green, Blue) format.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[1]{ L"Color" }, new uint8_t[3]{ AT_COLOR_RED, AT_COLOR_GREEN, AT_COLOR_BLUE }, 0 },
	{ 0x6C, L"0x6C", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6D, L"0x6D", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6E, L"0x6E", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6F, L"MoveCamera", L"Move camera over time.\n\n1st and 2nd arguments : destination in (X, Y) format.\n3nd argument : movement duration.\n4th argument : unknown.", true, 4, new uint8_t[4]{ 2, 2, 1, 1 }, new wstring[3]{ L"Destination", L"Time", L"Unknown" }, new uint8_t[4]{ AT_POSITION_X, AT_POSITION_Y, AT_SPIN, AT_SPIN }, 0 }, // screen size = 320?
	{ 0x70, L"ReleaseCamera", L"Unknown Opcode.\n\n1st arguments : unknown.\n2nd argument : unknown.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_USPIN }, 0 },
	{ 0x71, L"0x71", L"A camera movement.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x72, L"SetCameraFollowHeight", L"Define the standard height gap between the player's character position and the camera view.\n\n1st argument : height.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Height" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0x73, L"EnableCameraFollow", L"Make the camera follow the player's character.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x74, L"DisableCameraFollow", L"Stop making the camera follow the player's character.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x75, L"Menu", L"Open a menu.\n\n1st argument : menu type.\n2nd argument : depends on the menu type.\n Naming Menu : character to name.\n Shop Menu : shop ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Menu Type", L"Menu" }, new uint8_t[2]{ AT_MENUTYPE, AT_MENU }, 0 },
	{ 0x76, L"0x76", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x77, L"0x77", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x78, L"0x78", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // TRACKSTART, TRACK, TRACKADD and PRINTQUAD
	{ 0x79, L"0x79", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x7A, L"SetLeftAnimation", L"Change the left turning animation.\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x7B, L"SetRightAnimation", L"Change the right turning animation.\n\n1st argument : animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x7C, L"EnableDialogChoices", L"Define choices availability in dialogs using the [INIT_MULTICHOICE] text opcode.\n\n1st argument : boolean list for the different choices.\n2nd argument : default choice selected.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Choice List", L"Default Choice" }, new uint8_t[2]{ AT_BOOLLIST, AT_USPIN }, 0 },
	{ 0x7D, L"RunTimer", L"Run or pause the timer window.\n\n1st argument : boolean run/pause.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Run" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0x7E, L"SetFieldCamera", L"Change the field's background camera.\n\n1st argument : camera ID.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Camera ID" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x7F, L"EnableShadow", L"Enable the shadow for the entry's object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x80, L"DisableShadow", L"Disable the shadow for the entry's object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x81, L"SetShadowSize", L"Set the entry's object shadow size.\n\n1st argument : size X.\n2nd argument : size Y.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Size X", L"Size Y" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x82, L"SetShadowOffset", L"Change the offset between the entry's object and its shadow.\n\n1st argument : offset X.\n2nd argument : offset Y.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[2]{ L"Offset X", L"Offset Y" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x83, L"LockShadowRotation", L"Stop updating the shadow rotation by the object's rotation.\n\n1st argument : locked rotation.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Locked Rotation" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0x84, L"UnlockShadowRotation", L"Make the shadow rotate accordingly with its object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x85, L"SetShadowAmplifier", L"Amplify or reduce the shadow transparancy.\n\n1st argument : amplification factor.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Amplification Factor" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x86, L"SetAnimationStandSpeed", L"Change the standing animation speed.\n\n1st argument : unknown.\n2nd argument : unknown.\n3rd argument : unknown.\n4th argument : unknown.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Unknown", L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[4]{ AT_USPIN, AT_USPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0x87, L"0x87", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"Unknown" }, new uint8_t[2]{ AT_ENTRY, AT_SPIN }, 0 },
	{ 0x88, L"RunModelCode", L"Run a model code.\n\n1st argument : model code.\n2nd to 4th arguments : depends on the model code.\n Texture Blend (blend mode) with blend mode being 1 for screen, 2 for multiply, 4 for Soft and 255 for a solid texture\n Slice (boolean slice/unslice, value)\n Enable Mirror (boolean enable/disable)\n Mirror Position (X, Z, Y)\n Mirror Normal (X, Z, Y)\n Mirror Color (Red, Green, Blue)\n Sound codes (Animation, Frame, Value)\n  For Add (Secondary) Sound, the 3rd argument is the sound ID\n  For Sound Random Pitch, the 3rd argument is a boolean random/not random\n  For Remove Sound, the 3rd argument is unused", true, 4, new uint8_t[4]{ 1, 2, 2, 2 }, new wstring[4]{ L"Model Code", L"Argument 1", L"Argument 2", L"Argument 3" }, new uint8_t[4]{ AT_MODELCODE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x89, L"SetSoundPosition", L"Set the position of a 3D sound.\n\n1st to 3rd arguments : sound position.\n4th argument : sound volume.", true, 4, new uint8_t[4]{ 2, 2, 2, 1 }, new wstring[2]{ L"Position", L"Volume" }, new uint8_t[4]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y, AT_SPIN }, 0 },
	{ 0x8A, L"SetSoundObjectPosition", L"Set the position of a 3D sound to the object's position.\n\n1st argument : object.\n2nd argument : sound volume.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Volume" }, new uint8_t[2]{ AT_ENTRY, AT_SPIN }, 0 },
	{ 0x8B, L"SetHeadAngle", L"Maybe define the maximum angle and distance for the character to turn his head toward an active object.\n\n1st argument : unknown.\n2nd argument : unknown.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x8C, L"BattleEx", L"Start a battle and choose its battle group.\n\n1st argument : rush type (unknown).\n2nd argument : group.\n3rd argument : gathered battle and Steiner's state (highest bit) informations.", true, 3, new uint8_t[3]{ 1, 1, 2 }, new wstring[3]{ L"Rush Type", L"Battle Group", L"Battle" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_BATTLE }, 0 },
	{ 0x8D, L"ShowTimer", L"Activate the timer window.\n\n1st argument : boolean show/hide.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Enable" }, new uint8_t[1]{ AT_SPIN }, 0 }, // Surely AT_BOOL
	{ 0x8E, L"RaiseWindows", L"Make all the dialogs and windows display over the filters.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x8F, L"SetModelColor", L"Change a 3D model's color.\n\n1st argument : entry associated with the model.\n2nd to 4th arguments : color in (Red, Green, Blue) format.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[2]{ L"Object", L"Color" }, new uint8_t[4]{ AT_ENTRY, AT_COLOR_RED, AT_COLOR_GREEN, AT_COLOR_BLUE }, 0 },
	{ 0x90, L"DisableInactiveAnimation", L"Prevent player's character to play its inactive animation.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x91, L"0x91", L"Unknown Opcode.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_BOOL }, 0 }, // AUTOTURN: actor.turninst0 = (arg ? 167 : 4)
	{ 0x92, L"AttachTile", L"Make a part of the field background follow the player's movements. Also apply a color filter out of that tile block's range.\n\n1st argument : tile block.\n2nd and 3rd arguments : offset position in (X, Y) format.\n4th argument : filter mode ; use -1 for no filter effect.\n5th to 7th arguments : filter color in (Red, Green, Blue) format.", true, 7, new uint8_t[7]{ 1, 2, 1, 1, 1, 1, 1 }, new wstring[5]{ L"Tile Block", L"Position X", L"Position Y", L"Filter Mode", L"Filter Color" }, new uint8_t[7]{ AT_TILE, AT_SPIN, AT_SPIN, AT_SPIN, AT_COLOR_RED, AT_COLOR_GREEN, AT_COLOR_BLUE }, 0 },
	{ 0x93, L"SetObjectFlags", L"Change flags of the current entry's object.\n\n1st argument : object flags.\n 1 : show model\n 2 : unknown\n 4 : unknown\n 8 : unknown\n 16 : unknown\n 32 : unknown", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0x94, L"SetJumpAnimation", L"Change the jumping animation.\n\n1st argument : animation ID.\n2nd argument : unknown.\n3rd argument : unknown.", true, 3, new uint8_t[3]{ 2, 1, 1 }, new wstring[3]{ L"Animation", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_ANIMATION, AT_SPIN, AT_SPIN }, 0 },
	{ 0x95, L"WindowSyncEx", L"Display a window with text inside and wait until it closes.\n\n1st argument : talker's entry.\n2nd argument : window ID.\n3rd argument : UI flag list.\n 3 : disable bubble tail\n 4 : mognet format\n 5 : hide window\n 7 : ATE window\n 8 : dialog window\n4th argument : text to display.", true, 4, new uint8_t[4]{ 1, 1, 1, 2 }, new wstring[4]{ L"Talker", L"Window ID", L"UI", L"Text" }, new uint8_t[4]{ AT_ENTRY, AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x96, L"WindowAsyncEx", L"Display a window with text inside and continue the execution of the script without waiting.\n\n1st argument : talker's entry.\n2nd argument : window ID.\n3rd argument : UI flag list.\n 3 : disable bubble tail\n 4 : mognet format\n 5 : hide window\n 7 : ATE window\n 8 : dialog window\n4th argument : text to display.", true, 4, new uint8_t[4]{ 1, 1, 1, 2 }, new wstring[4]{ L"Talker", L"Window ID", L"UI", L"Text" }, new uint8_t[4]{ AT_ENTRY, AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x97, L"ReturnEntryFunctions", L"Make all the currently executed functions return for a given entry.\n\n1st argument : entry for which functions are returned.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Entry" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x98, L"MakeAnimationLoop", L"Make current object's currently playing animation loop.\n\n1st argument : loop amount.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Amount" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x99, L"SetTurnSpeed", L"Change the entry's object turn speed.\n\n1st argument : turn speed (1 is slowest).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x9A, L"EnablePathTriangle", L"Enable or disable a triangle of field pathing.\n\n1st argument : triangle ID.\n2nd argument : boolean enable/disable.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Triangle ID", L"Enable" }, new uint8_t[2]{ AT_WALKTRIANGLE, AT_BOOL }, 0 },
	{ 0x9B, L"TurnTowardPosition", L"Turn the character toward a position (animated). The object's turn speed is used (default to 16).\n\n1st and 2nd arguments : coordinates in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Coordinate" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x9C, L"RunJumpAnimation", L"Make the character play its jumping animation.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x9D, L"RunLandAnimation", L"Make the character play its landing animation (inverted jumping animation).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x9E, L"ExitField", L"Make the player's character walk to the field exit and prepare to flush the field datas.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x9F, L"SetObjectSize", L"Set the size of a 3D model.\n\n1st argument : entry of the 3D model.\n2nd to 4th arguments : size ratio in (Ratio X, Ratio Z, Ratio Y) format. A ratio of 64 is the default size.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Object", L"Size X", L"Size Z", L"Size Y" }, new uint8_t[4]{ AT_ENTRY, AT_USPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0xA0, L"WalkToExit", L"Make the entry's object walk to the field exit.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xA1, L"MoveInstantXZY", L"Instantatly move the object.\n\n1st to 3rd arguments : destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xA2, L"WalkXZY", L"Make the character walk to destination. Make it synchronous if InitWalk is called before.\n\n1st argument : destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xA3, L"0xA3", L"Unknown Opcode.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Unknown", L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[4]{ AT_SPIN, AT_SPIN, AT_SPIN, AT_SPIN }, 0 }, // Unused in Steam
	{ 0xA4, L"CalculateExitPosition", L"Calculate the field exit position based on the region's polygon.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xA5, L"Slide", L"Make the character slide to destination (walk without using the walk animation and without changing the facing angle).\n\n1st and 2nd arguments : destination in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0xA6, L"SetRunSpeedLimit", L"Change the speed at which the character uses his run animation instead of his walk animation (default is 31).\n\n1st argument : speed limit.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed Limit" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xA7, L"Turn", L"Make the character face an angle (animated). Speed is defaulted to 16.\n\n1st argument : angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Angle" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xA8, L"SetPathing", L"Change the pathing of the character.\n\n1st argument : boolean pathing on/off.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Pathing" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0xA9, L"0xA9", L"Unknown Opcode.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_ENTRY }, 0 }, // GETSCREEN ; change the sSys position
	{ 0xAA, L"EnableMenu", L"Enable menu access by the player.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xAB, L"DisableMenu", L"Disable menu access by the player.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xAC, L"ChangeDisc", L"Allow to save the game and change disc.\n\n1st argument : gathered field destination and disc destination.", true, 1, new uint8_t[1]{ 2 }, new wstring[2]{ L"Field Destination", L"Disc" }, new uint8_t[1]{ AT_DISC_FIELD }, 0 },
	{ 0xAD, L"MoveInstantXZYEx", L"Instantatly move an object.\n\n1st argument : object's entry.\n2nd to 4th arguments : destination in (X, Z, Y) format.", true, 4, new uint8_t[4]{ 1, 2, 2, 2 }, new wstring[4]{ L"Object", L"Destination" }, new uint8_t[4]{ AT_ENTRY, AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xAE, L"TetraMaster", L"Begin a card game.\n\n1st argument : card deck of the opponent.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Card Deck" }, new uint8_t[1]{ AT_DECK }, 0 },
	{ 0xAF, L"0xAF", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // DELETEALLCARD ; doesn't seem to work
	{ 0xB0, L"SetFieldName", L"Change the name of the field.\n\n1st argument : new name (unknown format).", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Name" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xB1, L"ResetFieldName", L"Reset the name of the field.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB2, L"Party", L"Allow the player to change the members of its party.\n\n1st argument : party size (if characters occupy slots beyond it, they are locked).\n2nd argument : list of locked characters.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Party Size", L"Locked Characters" }, new uint8_t[2]{ AT_USPIN, AT_CHARACTER }, 0 },
	{ 0xB3, L"RunSPSCode", L"Run Sps code, which seems to be special model effects on the field.\n\n1st argument : sps ID.\n2nd argument : sps code.\n3rd to 5th arguments : depend on the sps code.\n Load Sps (sps type)\n Enable Attribute (attribute list, boolean enable/disable)\n Set Position (X, -Z, Y)\n Set Rotation (angle X, angle Z, angle Y)\n Set Scale (scale factor)\n Attach (object's entry to attach, bone number)\n Set Fade (fade)\n Set Animation Rate (rate)\n Set Frame Rate (rate)\n Set Frame (value) where the value is factored by 16 to get the frame\n Set Position Offset (X, -Z, Y)\n Set Depth Offset (depth)", true, 5, new uint8_t[5]{ 1, 1, 2, 2, 2 }, new wstring[5]{ L"Sps", L"Code", L"Parameter 1", L"Parameter 2", L"Parameter 3" }, new uint8_t[5]{ AT_USPIN, AT_SPSCODE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xB4, L"SetPartyReserve", L"Define the party member availability for a future Party call.\n\n1st argument : list of available characters.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Characters available" }, new uint8_t[1]{ AT_CHARACTER }, 0 },
	{ 0xB5, L"0xB5", L"Seem to somehow make the object appropriate itself another entry's function list.\n\n1st argument : entry to get functions from.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Entry" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xB6, L"WorldMap", L"Change the scene to a world map.\n\n1st argument : world map destination.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"World Map" }, new uint8_t[1]{ AT_WORLDMAP }, 0 },
	{ 0xB7, L"0xB7", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB8, L"0xB8", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB9, L"AddControllerMask", L"Prevent the input to be processed by the game.\n\n1st argument : pad number (should only be 0 or 1).\n2nd argument : button list.\n1 : Select\n4 : Start\n5 : Up\n6 : Right\n7 : Down\n8 : Left\n9 : L2\n10 : R2\n11 : L1\n12 : R1\n13 : Triangle\n14 : Circle\n15 : Cross\n16 : Square\n17 : Cancel\n18 : Confirm\n20 : Moogle\n21 : L1 Ex\n22 : R1 Ex\n23 : L2 Ex\n24 : R2 Ex\n25 : Menu\n26 : Select Ex", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Pad", L"Buttons" }, new uint8_t[2]{ AT_USPIN, AT_BOOLLIST }, 0 },
	{ 0xBA, L"RemoveControllerMask", L"Enable back the controller's inputs.\n\n1st argument : pad number (should only be 0 or 1).\n2nd argument : button list.\n1 : Select\n4 : Start\n5 : Up\n6 : Right\n7 : Down\n8 : Left\n9 : L2\n10 : R2\n11 : L1\n12 : R1\n13 : Triangle\n14 : Circle\n15 : Cross\n16 : Square\n17 : Cancel\n18 : Confirm\n20 : Moogle\n21 : L1 Ex\n22 : R1 Ex\n23 : L2 Ex\n24 : R2 Ex\n25 : Menu\n26 : Select Ex", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Pad", L"Buttons" }, new uint8_t[2]{ AT_USPIN, AT_BOOLLIST }, 0 },
	{ 0xBB, L"TimedTurnEx", L"Make an object face an angle (animated).\n\n1st argument : object's entry.\n2nd argument : angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.\n3rd argument : turn speed (1 is slowest).", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Object", L"Angle", L"Speed" }, new uint8_t[3]{ AT_ENTRY, AT_USPIN, AT_USPIN }, 0 },
	{ 0xBC, L"WaitTurnEx", L"Wait until an object facing movement has ended.\n\n1st argument : object's entry.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xBD, L"RunAnimationEx", L"Play an object's animation.\n\n1st argument : object's entry.\n2nd argument : animation ID.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Object", L"Animation ID" }, new uint8_t[2]{ AT_ENTRY, AT_ANIMATION }, 0 },
	{ 0xBE, L"WaitAnimationEx", L"Wait until the object's animation has ended.\n\n1st argument : object's entry.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xBF, L"MoveInstantEx", L"Instantatly move an object.\n\n1st argument : object's entry.\n2nd and 3rd arguments : destination in (X, Y) format.", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Object", L"Destination" }, new uint8_t[3]{ AT_ENTRY, AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0xC0, L"EnableTextureAnimation", L"Run a model texture animation and make it loop.\n\n1st argument : model's entry.\n2nd argument : texture animation ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Texture Animation" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0xC1, L"RunTextureAnimation", L"Run once a model texture animation.\n\n1st argument : model's entry.\n2nd argument : texture animation ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Texture Animation" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0xC2, L"StopTextureAnimation", L"Stop playing the model texture animation.\n\n1st argument : model's entry.\n2nd argument : texture animation ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Texture Animation" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0xC3, L"0xC3", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // EBG_overlaySetViewport
	{ 0xC4, L"RunWorldCode", L"Run one of the World Map codes, which effects hava a large range. May modify the weather, the music, call the chocobo or enable the auto-pilot.\n\n1st argument : world code.\n2nd argument : depends on the code.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Code", L"Argument" }, new uint8_t[2]{ AT_WORLDCODE, AT_SPIN }, 0 },
	{ 0xC5, L"RunSoundCode", L"Same as RunSoundCode3( code, music, 0, 0, 0 ).", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[2]{ L"Code", L"Sound" }, new uint8_t[2]{ AT_SOUNDCODE, AT_SOUND }, 0 },
	{ 0xC6, L"RunSoundCode1", L"Same as RunSoundCode3( code, music, arg1, 0, 0 ).", true, 3, new uint8_t[3]{ 2, 2, 3 }, new wstring[3]{ L"Code", L"Sound", L"Argument" }, new uint8_t[3]{ AT_SOUNDCODE, AT_SOUND, AT_SPIN }, 0 },
	{ 0xC7, L"RunSoundCode2", L"Same as RunSoundCode3( code, music, arg1, arg2, 0 ).", true, 4, new uint8_t[4]{ 2, 2, 3, 1 }, new wstring[4]{ L"Code", L"Sound", L"Argument", L"Argument" }, new uint8_t[4]{ AT_SOUNDCODE, AT_SOUND, AT_SPIN, AT_SPIN }, 0 },
	{ 0xC8, L"RunSoundCode3", L"Run a sound code.\n\n1st argument : sound code.\n2nd argument : music or sound to process.\n3rd to 5th arguments : depend on the sound code.", true, 5, new uint8_t[5]{ 2, 2, 3, 1, 1 }, new wstring[5]{ L"Code", L"Sound", L"Argument", L"Argument", L"Argument" }, new uint8_t[5]{ AT_SOUNDCODE, AT_SOUND, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xC9, L"0xC9", L"Unknown Opcode.", true, 5, new uint8_t[5]{ 1, 2, 2, 2, 2 }, new wstring[5]{ L"Unknown", L"Unknown", L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[5]{ AT_USPIN, AT_USPIN, AT_USPIN, AT_USPIN, AT_USPIN }, 0 }, // EBG_overlayDefineViewport
	{ 0xCA, L"0xCA", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Visible" }, new uint8_t[2]{ AT_SPIN, AT_BOOL }, 0 }, // EBG_animSetVisible
	{ 0xCB, L"EnablePath", L"Enable a field path.\n\n1st argument : field path ID.\n2nd argument : boolean enable/disable.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Path", L"Enable" }, new uint8_t[2]{ AT_WALKPATH, AT_BOOL }, 0 },
	{ 0xCC, L"0xCC", L"Maybe make the character transparent.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Flag List" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0xCD, L"0xCD", L"Unknown Opcode.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Flag List" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0xCE, L"AddGil", L"Give gil to the player.\n\n1st argument : gil amount.", true, 1, new uint8_t[1]{ 3 }, new wstring[1]{ L"Amount" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xCF, L"RemoveGil", L"Remove gil from the player.\n\n1st argument : gil amount.", true, 1, new uint8_t[1]{ 3 }, new wstring[1]{ L"Amount" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xD0, L"BattleDialog", L"Display text in battle for 60 frames.\n\n1st argument : text to display.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Text" }, new uint8_t[1]{ AT_TEXT }, 0 },
	{ 0xD1, L"0xD1", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0xD2, L"0xD2", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0xD3, L"0xD3", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0xD4, L"0xD4", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_SPIN }, 0 }, // geoAttachOffset
	{ 0xD5, L"0xD5", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // PUSHHIDE
	{ 0xD6, L"0xD6", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // POPSHOW
	{ 0xD7, L"ATE", L"Enable or disable ATE.\n\n1st argument : maybe flags (unknown format).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xD8, L"SetWeather", L"Add a foreground effect.\n\n1st argument : unknown.\n2nd argument : unknown.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0xD9, L"CureStatus", L"Cure the status ailments of a party member.\n\n1st argument : character.\n2nd argument : status list.\n 1 : Petrified\n 2 : Venom\n 3 : Virus\n 4 : Silence\n 5 : Darkness\n 6 : Trouble\n 7 : Zombie", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Statuses" }, new uint8_t[2]{ AT_LCHARACTER, AT_BOOLLIST }, 0 },
	{ 0xDA, L"RunSPSCodeSimple", L"Run Sps code, which seems to be special model effects on the field.\n\n1st argument : sps ID.\n2nd argument : sps code.\n3rd to 5th arguments : depend on the sps code.\n Load Sps (sps type)\n Enable Attribute (attribute list, boolean enable/disable)\n Set Position (X, -Z, Y)\n Set Rotation (angle X, angle Z, angle Y)\n Set Scale (scale factor)\n Attach (object's entry to attach, bone number)\n Set Fade (fade)\n Set Animation Rate (rate)\n Set Frame Rate (rate)\n Set Frame (value) where the value is factored by 16 to get the frame\n Set Position Offset (X, -Z, Y)\n Set Depth Offset (depth)", true, 5, new uint8_t[5]{ 1, 1, 1, 2, 2 }, new wstring[5]{ L"Sps", L"Code", L"Parameter 1", L"Parameter 2", L"Parameter 3" }, new uint8_t[5]{ AT_USPIN, AT_SPSCODE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xDB, L"0xDB", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Unknown" }, new uint8_t[2]{ AT_LCHARACTER, AT_SPIN }, 0 }, // character.win_pose = value
	{ 0xDC, L"Jump", L"Perform a jumping animation. Must be used after a SetupJump call.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xDD, L"RemoveParty", L"Remove a character from the player's team.\n\n1st argument : character to remove.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Character" }, new uint8_t[1]{ AT_LCHARACTER }, 0 },
	{ 0xDE, L"SetName", L"Change the name of a party member. Clear the text opcodes from the chosen text.\n\n1st argument : character to rename.\n2nd argument : new name.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Character", L"Text" }, new uint8_t[2]{ AT_LCHARACTER, AT_TEXT }, 0 },
	{ 0xDF, L"0xDF", L"Unknown Opcode.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_USPIN }, 0 }, // posObj.ovalRatio
	{ 0xE0, L"AddFrog", L"Add one frog to the frog counter.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xE1, L"TerminateBattle", L"Return to the field (or world map) when the rewards are disabled.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xE2, L"SetupJump", L"Setup datas for a Jump call.\n\n1st to 3rd arguments : destination in (X, Z, Y) format.\n4th argument : number of steps for the jump.", true, 4, new uint8_t[4]{ 2, 2, 2, 1 }, new wstring[2]{ L"Destination", L"Steps" }, new uint8_t[4]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y, AT_USPIN }, 0 },
	{ 0xE3, L"SetDialogProgression", L"Change the dialog progression value.\n\n1st argument : new value.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Progression" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xE4, L"0xE4", L"Seem to move field tile while applying a loop effect to it.\n\n1st argument : background tile block.\n2nd argument : unknown.\n3rd and 4th arguments : seems to be the movement in (X/256, Y/256) format.\n5th argument : unknown.", true, 5, new uint8_t[5]{ 1, 1, 2, 2, 1 }, new wstring[5]{ L"Tile Block", L"Unknown", L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[5]{ AT_TILE, AT_USPIN, AT_SPIN, AT_SPIN, AT_USPIN }, 0 },
	{ 0xE5, L"AttackSpecial", L"Make the enemy instantatly use a special move. It doesn't use nor modify the battle state so it should be used when the battle is paused. The target(s) are to be set using the SV_Target variable.\n\n1st argument : attack to perform.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Attack" }, new uint8_t[1]{ AT_ATTACK }, 0 },
	{ 0xE6, L"0xE6", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // BGLLOOPTYPE
	{ 0xE7, L"SetTileAnimationFrame", L"Change the frame of a field tile animation.\n\n1st argument : background animation.\n2nd argument : animation frame to display.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Animation", L"Frame ID" }, new uint8_t[2]{ AT_TILEANIM, AT_USPIN }, 0 },
	{ 0xE8, L"SideWalkXZY", L"Make the character walk to destination without changing his facing angle. Make it synchronous if InitWalk is called before.\n\n1st to 3rd arguments : destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xE9, L"UpdatePartyUI", L"Update the party's menu icons and such.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEA, L"0xEA", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // EBG_sceneGetVRP ; modify sSys Position
	{ 0xEB, L"CloseAllWindows", L"Close all the dialogs and UI windows.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEC, L"FadeFilter", L"Apply a fade filter on the screen.\n\n1st argument : fade mode (0 to add, 2 to remove).\n2nd argument : fading time.\n3rd argument : unknown.\n4th to 6th arguments : color of the filter in (Cyan, Magenta, Yellow) format.", true, 6, new uint8_t[6]{ 1, 1, 1, 1, 1, 1 }, new wstring[4]{ L"Fade In/Out", L"Fading Time", L"Unknown", L"Color" }, new uint8_t[6]{ AT_USPIN, AT_USPIN, AT_SPIN, AT_COLOR_CYAN, AT_COLOR_MAGENTA, AT_COLOR_YELLOW }, 0 },
	{ 0xED, L"0xED", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_USPIN, AT_SPIN, AT_SPIN }, 0 }, // EBG_overlayDefineViewportAlpha
	{ 0xEE, L"EnableInactiveAnimation", L"Allow the player's character to play its inactive animation. The inaction time required is :\nFirst Time = 200 + 4 * Random[0, 255]\Following Times = 200 + 2 * Random[0, 255]", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEF, L"ShowHereIcon", L"Show the Here icon over player's chatacter.\n\n1st argument : display type (0 to hide, 3 to show unconditionally)", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Show" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xF0, L"EnableRun", L"Allow the player's character to run.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xF1, L"SetHP", L"Change the HP of a party's member.\n\n1st argument : character.\n2nd argument : new HP value.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Character", L"HP" }, new uint8_t[2]{ AT_LCHARACTER, AT_USPIN }, 0 },
	{ 0xF2, L"SetMP", L"Change the MP of a party's member.\n\n1st argument : character.\n2nd argument : new MP value.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Character", L"MP" }, new uint8_t[2]{ AT_LCHARACTER, AT_USPIN }, 0 },
	{ 0xF3, L"UnlearnAbility", L"Set an ability's AP back to 0.\n\n1st argument : character.\n2nd argument : ability to reset.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Ability" }, new uint8_t[2]{ AT_LCHARACTER, AT_ABILITY }, 0 },
	{ 0xF4, L"LearnAbility", L"Make character learn an ability.\n\n1st argument : character.\n2nd argument : ability to learn.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Ability" }, new uint8_t[2]{ AT_LCHARACTER, AT_ABILITY }, 0 },
	{ 0xF5, L"GameOver", L"Terminate the game with a Game Over screen.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xF6, L"VibrateController", L"Start the vibration lifespan.\n\n1st argument : frame to begin with.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Frame" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xF7, L"ActivateVibration", L"Make the controller's vibration active. If the current controller's frame is out of the vibration time range, the vibration lifespan is reinit.\n\n1st argument : boolean activate/deactivate.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Activate" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0xF8, L"RunVibrationTrack", L"Run a vibration track.\n\n1st argument : track ID.\n2nd argument : sample (0 or 1).\n3rd argument : boolean activate/deactivate.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Track", L"Sample", L"Activate" }, new uint8_t[3]{ AT_USPIN, AT_USPIN, AT_BOOL }, 0 },
	{ 0xF9, L"ActivateVibrationTrack", L"Activate a vibration track.\n\n1st argument : track ID.\n2nd argument : sample (0 or 1).\n3rd argument : boolean activate/deactivate.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Track", L"Sample", L"Activate" }, new uint8_t[3]{ AT_USPIN, AT_USPIN, AT_BOOL }, 0 },
	{ 0xFA, L"SetVibrationSpeed", L"Set the vibration frame rate.\n\n1st argument : frame rate.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Frame Rate" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xFB, L"SetVibrationFlags", L"Change the vibration flags.\n\n1st argument : flags.\n 8 : Loop\n 16 : Wrap", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0xFC, L"SetVibrationRange", L"Set the time range of vibration.\n\n1st and 2nd arguments : vibration range in (Start, End) format.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Start", L"End" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0xFD, L"PreloadField", L"Surely preload a field.\n\n1st argument : unknown.\n2nd argument : field to preload.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Unknown", L"Field" }, new uint8_t[2]{ AT_SPIN, AT_FIELD }, 0 },
	{ 0xFE, L"SetCharacterData", L"Init a party's member battle and menu datas.\n\n1st argument : character.\n2nd argument : boolean update level/don't update level.\n3rd argument : equipement set to use.\n4th argument : character categories ; doesn't change if all are enabled.\n 1 : male\n 2 : female\n 3 : gaian\n 4 : terran\n 5 : temporary character\n5th argument : ability and command set to use.", true, 5, new uint8_t[5]{ 1, 1, 1, 1, 1 }, new wstring[5]{ L"Character", L"Update Level", L"Equipement Set", L"Category", L"Ability Set" }, new uint8_t[5]{ AT_LCHARACTER, AT_BOOL, AT_EQUIPSET, AT_BOOLLIST, AT_ABILITYSET }, 0 },
	{ 0xFF, L"EXTENDED_CODE", L"Not an opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x100, L"0x100", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x101, L"0x101", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x102, L"0x102", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetActive
	{ 0x103, L"0x103", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetFlags
	{ 0x104, L"0x104", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetFloor
	{ 0x105, L"0x105", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetFrameRate
	{ 0x106, L"0x106", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetAlgorithm
	{ 0x107, L"0x107", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Unknown", L"Unknown" , L"Unknown" }, new uint8_t[3]{ AT_USPIN, AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetDelta
	{ 0x108, L"0x108", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_simSetAxis
	{ 0x109, L"0x109", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10A, L"0x10A", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 }, // walkMesh.BGI_animShowFrame
	{ 0x10B, L"0x10B", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10C, L"0x10C", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10D, L"0x10D", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10E, L"0x10E", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10F, L"0x10F", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x110, L"0x110", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x111, L"0x111", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 }
};

//-- Battle Scenes
static SortedChoiceItem HADES_STRING_BATTLE_SCENE_NAME[] = {
	{ 0x0,  L"Alexandria Castle, Hidden passage's lower level" },
	{ 0x4,  L"Alexandria Castle, Hidden passage's stairs" },
	{ 0x5,  L"Alexandria Castle, Extraction altar" },
	{ 0x6,  L"Alexandria Castle, Dungeon" },
	{ 0x2,  L"Alexandria Castle, Courtyard" },
	{ 0x1,  L"Alexandria Castle, Entrance Hall" },
	{ 0xA5, L"Alexandria Castle, Library" },
	{ 0x3,  L"Alexandria Castle, Hallway" },
	{ 0x7,  L"Alexandria Castle, Queen's Chamber" },
	{ 0x7F, L"Alexandria, Square (Night)" },
	{ 0x7E, L"Alexandria, Main Street (Night)" },
	{ 0xE,  L"Burmecia, Outside, Street" },
	{ 0xA,  L"Burmecia, Outside, Holy gate" },
	{ 0xD,  L"Burmecia, Outside, Uptown area" },
	{ 0xB,  L"Burmecia, Outside, Beatrix battle" },
	{ 0xF,  L"Burmecia, Inside, House" },
	{ 0xC,  L"Burmecia, Inside, Uptown area" },
	{ 0x12, L"Cargo Ship, Deck" },
	{ 0xB0, L"Chocobo's Air Garden" },
	{ 0x2E, L"Cleyra's Trunk, Outside, Rise (Tempest)" },
	{ 0x28, L"Cleyra's Trunk, Outside, Ladder (Tempest)" },
	{ 0x2B, L"Cleyra's Trunk, Outside, Ladder (Clear sky)" },
	{ 0x2A, L"Cleyra's Trunk, Inside, Sand-full room" },
	{ 0x2C, L"Cleyra's Trunk, Inside, Sandfall" },
	{ 0x2D, L"Cleyra's Trunk, Inside, Platform with less sand" },
	{ 0x29, L"Cleyra's Trunk, Inside, Trunk forest" },
	{ 0x19, L"Cleyra, Antlion battle" },
	{ 0x17, L"Cleyra, Entrance" },
	{ 0x16, L"Cleyra, Observation post" },
	{ 0x18, L"Cleyra, Front of the Cathedral" },
	{ 0xAD, L"Crystal World, Blue battlefied" },
	{ 0xAF, L"Crystal World, Red battlefield" },
	{ 0xA1, L"Crystal World, Deathguise battle" },
	{ 0xA2, L"Crystal World, Crystal" },
	{ 0xA3, L"Crystal World, Necron battle" },
	{ 0x8,  L"Dali, Landing ground" },
	{ 0x4B, L"Dali, Underground, Path" },
	{ 0x4C, L"Dali, Underground, Production area" },
	{ 0x11, L"Dead Forest" },
	{ 0x88, L"Desert Palace, Dock" },
	{ 0x8A, L"Desert Palace, Fire chamber" },
	{ 0x89, L"Desert Palace, Stairwell" },
	{ 0x8B, L"Desert Palace, Library" },
	{ 0x86, L"Desert Palace, Top level" },
	{ 0x87, L"Desert Palace, Sanctum" },
	{ 0x91, L"Earth Shrine" },
	{ 0x8C, L"Esto Gaza, Entrance" },
	{ 0x8D, L"Esto Gaza, Gulug's Gate" },
	{ 0x1A, L"Evil Forest, Light clearing" },
	{ 0x1D, L"Evil Forest, Dark way" },
	{ 0x1B, L"Evil Forest, Plant Brain battle" },
	{ 0x1C, L"Evil Forest, Dark clearing" },
	{ 0x20, L"Fossil Roo, Booby-Trapped road" },
	{ 0x1F, L"Fossil Roo, Upper level" },
	{ 0x21, L"Fossil Roo, Stone platform" },
	{ 0x22, L"Fossil Roo, Road accross water" },
	{ 0x23, L"Fossil Roo, Path of dirt with few scaffolding" },
	{ 0x24, L"Fossil Roo, Underground lake" },
	{ 0x27, L"Gargan Roo, Hall" },
	{ 0x26, L"Gargan Roo, Platform" },
	{ 0x25, L"Gargan Roo, Gargan path" },
	{ 0x30, L"Gizamaluke's Grotto, Hall" },
	{ 0x31, L"Gizamaluke's Grotto, Gizamaluke" },
	{ 0x33, L"Ice Cavern, Icicle fields" },
	{ 0x34, L"Ice Cavern, First fields" },
	{ 0x35, L"Ice Cavern, Road to waterfall" },
	{ 0x32, L"Ice Cavern, Waterfall" },
	{ 0x3D, L"Iifa Tree, Outside, Root" },
	{ 0x39, L"Iifa Tree, Outside, Root with Mist" },
	{ 0x37, L"Iifa Tree, Outside, Trunk" },
	{ 0x36, L"Iifa Tree, Outside, Slippery root" },
	{ 0x3C, L"Iifa Tree, Inside, Stone elevator" },
	{ 0x3B, L"Iifa Tree, Inside, Root" },
	{ 0x3A, L"Iifa Tree, Inside, Leaf elevator" },
	{ 0x38, L"Iifa Tree, Inside, Soulcage" },
	{ 0x8F, L"Ipsen's Castle, Hall" },
	{ 0x90, L"Ipsen's Castle, Stairwell" },
	{ 0x8E, L"Ipsen's Castle, Mirrors' room" },
	{ 0x3F, L"Lindblum, Industrial District" },
	{ 0x40, L"Lindblum, Business District" },
	{ 0x41, L"Lindblum, Theater District" },
	{ 0x42, L"Madain Sari, Eidolon wall" },
	{ 0xA0, L"Memoria, Nova Dragon battle" },
	{ 0xA4, L"Memoria, Entrance & Recollection" },
	{ 0x9D, L"Memoria, Stairs of Time" },
	{ 0x9A, L"Memoria, Outer Path" },
	{ 0x9C, L"Memoria, Oblivion" },
	{ 0x9E, L"Memoria, Recollection (Rainy)" },
	{ 0x98, L"Memoria, Time Interval" },
	{ 0xA6, L"Memoria, Ruins" },
	{ 0x9F, L"Memoria, Familiar Past" },
	{ 0xA7, L"Memoria, World Fusion" },
	{ 0xA9, L"Memoria, Portal" },
	{ 0xA8, L"Memoria, Birth" },
	{ 0xAC, L"Memoria, Hades & To the Origin" },
	{ 0x99, L"Memoria, Ocean" },
	{ 0xAA, L"Memoria, Time Warp" },
	{ 0xAB, L"Memoria, Gaia's Birth" },
	{ 0xAE, L"Memoria, Stairs" },
	{ 0x9B, L"Memoria, Gate to Space" },
	{ 0xB2, L"Memoria, To the Origin" },
	{ 0x13, L"Mountain Path, Opened ground" },
	{ 0x14, L"Mountain Path, Bridge" },
	{ 0x15, L"Mountain Path, Closed ground" },
	{ 0x81, L"Mount Gulug, Rectangular room" },
	{ 0x82, L"Mount Gulug, Ruined room with stairways" },
	{ 0x84, L"Mount Gulug, Interior" },
	{ 0x85, L"Mount Gulug, Room with a missing wall" },
	{ 0x83, L"Mount Gulug, Well's bottom" },
	{ 0x2F, L"Mount Gulug, Extraction circle" },
	{ 0x1E, L"Oeilvert, Outside" },
	{ 0x7C, L"Oeilvert, Hall" },
	{ 0x7D, L"Oeilvert, Grand Hall" },
	{ 0x7B, L"Oeilvert, Star display" },
	{ 0x97, L"Oeilvert, Ark battle" },
	{ 0x80, L"Pandemonium, Hall (Metallic gates)" },
	{ 0x93, L"Pandemonium, Hall (Stone gates)" },
	{ 0x94, L"Pandemonium, Elevator" },
	{ 0x95, L"Pandemonium, Teleportation maze" },
	{ 0x92, L"Pandemonium, Top of Pandemonium" },
	{ 0x43, L"Pinnacle Rocks" },
	{ 0x47, L"Prima Vista, Cargo room" },
	{ 0x9,  L"Prima Vista, Cargo room in ruins" },
	{ 0x48, L"Prima Vista, Storage room" },
	{ 0x49, L"Prima Vista, Stage (Evening)" },
	{ 0x46, L"Prima Vista, Stage (Night)" },
	{ 0x3E, L"Qu Marsh" },
	{ 0x44, L"Summit Station, Out of the Cable Car" },
	{ 0x45, L"Terra, Path" },
	{ 0x7A, L"Terra, Bridge" },
	{ 0x96, L"Terra, Tree" },
	{ 0x10, L"Terra, Bran Bal's stairways" },
	{ 0x4A, L"Treno, Battle arena" },
	{ 0x52, L"World Map, Mist Continent, Beach" },
	{ 0x5D, L"World Map, Mist Continent, Beach + Mist" },
	{ 0x53, L"World Map, Mist Continent, Dirt" },
	{ 0x59, L"World Map, Mist Continent, Dirt + Mist" },
	{ 0x4D, L"World Map, Mist Continent, Grassland (Day)" },
	{ 0x5A, L"World Map, Mist Continent, Grassland (Day) + Mist" },
	{ 0x54, L"World Map, Mist Continent, Grassland (Evening)" },
	{ 0x57, L"World Map, Mist Continent, Grassland (Evening) + Mist" },
	{ 0x55, L"World Map, Mist Continent, Grassland (Night)" },
	{ 0x56, L"World Map, Mist Continent, Grassland (Night) + Mist" },
	{ 0x4E, L"World Map, Mist Continent, Mountain" },
	{ 0x50, L"World Map, Mist Continent, Mountain + Mist" },
	{ 0x70, L"World Map, Outer Continent, Beach" },
	{ 0x6E, L"World Map, Outer Continent, Beach + Mist" },
	{ 0x72, L"World Map, Outer Continent, Dirt" },
	{ 0x6D, L"World Map, Outer Continent, Dirt + Mist" },
	{ 0x71, L"World Map, Outer Continent, Grassland" },
	{ 0x6F, L"World Map, Outer Continent, Grassland + Mist" },
	{ 0x68, L"World Map, Forgotten Continent, Dirt (Day)" },
	{ 0x62, L"World Map, Forgotten Continent, Dirt (Day) + Mist" },
	{ 0x69, L"World Map, Forgotten Continent, Dirt (Evening)" },
	{ 0x63, L"World Map, Forgotten Continent, Dirt (Evening) + Mist" },
	{ 0x6C, L"World Map, Forgotten Continent, Grassland (Day)" },
	{ 0x6B, L"World Map, Forgotten Continent, Grassland (Day) + Mist" },
	{ 0x66, L"World Map, Forgotten Continent, Grassland (Evening)" },
	{ 0x60, L"World Map, Forgotten Continent, Grassland (Evening) + Mist" },
	{ 0x6A, L"World Map, Forgotten Continent, Mountain (Day)" },
	{ 0x64, L"World Map, Forgotten Continent, Mountain (Day) + Mist" },
	{ 0x65, L"World Map, Forgotten Continent, Mountain (Evening)" },
	{ 0x5F, L"World Map, Forgotten Continent, Mountain (Evening) + Mist" },
	{ 0x74, L"World Map, Lost Continent, Beach" },
	{ 0x73, L"World Map, Lost Continent, Beach + Mist" },
	{ 0x75, L"World Map, Lost Continent, Dirt" },
	{ 0x76, L"World Map, Lost Continent, Dirt + Mist" },
	{ 0x78, L"World Map, Lost Continent, Snow" },
	{ 0x77, L"World Map, Lost Continent, Snow + Mist" },
	{ 0x51, L"World Map, Green forest" },
	{ 0x4F, L"World Map, Green forest + Mist" },
	{ 0x67, L"World Map, Brown forest" },
	{ 0x61, L"World Map, Black forest" },
	{ 0x58, L"World Map, Sand desert" },
	{ 0x5B, L"World Map, Sand desert + Mist" },
	{ 0x5E, L"World Map, Marsh" },
	{ 0x5C, L"World Map, Marsh + Mist" },
	{ 0xB1, L"??? (Empty battlefield)" }
};

//-- Music
static SortedChoiceItem HADES_STRING_MUSIC_NAME[] = {
	{ 60,	L"Ice Cavern" },
	{ 103,	L"Ipsen's Castle" },
	{ 108,	L"South Gate" },
	{ 110,	L"Mount Gulug" },
	{ 116,	L"Loss Of Me" },
	{ 125,	L"Protecting My Devotion" },
	{ 129,	L"Daguerreo" },
	{ 131,	L"Terra" },
	{ 133,	L"Lindblum Castle" },
	{ 136,	L"Wind Sound" },
	{ 144,	L"Esto Gaza" },
	{ 68,	L"Burmecia" },
	{ 82,	L"Black Mage Village" },
	{ 84,	L"Pandemonium" },
	{ 85,	L"Treno" },
	{ 86,	L"The Sneaky Frog" },
	{ 87,	L"Bran Bal" },
	{ 90,	L"Conde Petie" },
	{ 94,	L"Immoral Melody" },
	{ 96,	L"Kuja" }, // Desert Palace
	{ 74,	L"Lindblum" },
	{ 76,	L"Limited Time" },
	{ 78,	L"Vamo' Alla Flamenco" },
	{ 57,	L"Qu Marsh" },
	{ 2,	L"Amarant" },
	{ 3,	L"Dali" },
	{ 24,	L"Aloha De Chocobo" },
	{ 32,	L"Gizamaluke Grotto" },
	{ 35,	L"Boss Battle" },
	{ 36,	L"Oeilvert" },
	{ 48,	L"You're Not Alone" },
};

//-- Audio
static SortedChoiceItem HADES_STRING_AUDIO_NAME[] = {
	
};

//-- Model
static SortedChoiceItem HADES_STRING_MODEL_NAME[] = {
	{ 0,	L"Enemy_Deathguise" },
	{ 1,	L"Enemy_Ozma" },
	{ 3,	L"Terran_Elevator" },
	{ 4,	L"Silverdragon" },
	{ 5,	L"Enemy_Kuja" },
	{ 6,	L"Metallic_Ground" },
	{ 7,	L"Enemy_DummyA" },
	{ 8,	L"Vivi" },
	{ 9,	L"Enemy_TrickSparrow" },
	{ 10,	L"Grandma" },
	{ 11,	L"Man_BillyGoat" },
	{ 12,	L"Dali_Grandma" },
	{ 14,	L"Enemy_ZombieA" },
	{ 15,	L"Iifa_Elevator" },
	{ 20,	L"Alexandrian_SoldierA" },
	{ 22,	L"Ladder" },
	{ 23,	L"Theater_Ship_Miniature" },
	{ 25,	L"NobleladyA" },
	{ 26,	L"YoungNoblemanA" },
	{ 28,	L"Enemy_Hornet" },
	{ 31,	L"MiniBurmecia" },
	{ 32,	L"Alexandrian_BoyA" },
	{ 33,	L"Shadow" },
	{ 34,	L"Alexandrian_BoyB" },
	{ 36,	L"Enemy_SilverDragon" },
	{ 37,	L"Weapon_Hammer" },
	{ 38,	L"Enemy_Maliris" },
	{ 39,	L"Battle_Cinna" },
	{ 41,	L"World_Chest" },
	{ 42,	L"Battle_Blank" },
	{ 43,	L"Weapon_Fairy_Flute" },
	{ 44,	L"Weapon_Golem_Flute" },
	{ 45,	L"Battle_Marcus" },
	{ 46,	L"Enemy_GoblinMage" },
	{ 47,	L"Enemy_Mover" },
	{ 48,	L"Ruby" },
	{ 49,	L"Dali_ChildA" },
	{ 50,	L"Dali_ManA" },
	{ 52,	L"Hippaul" },
	{ 53,	L"Black_MageA" },
	{ 54,	L"Black_MageB" },
	{ 55,	L"Dali_GirlA" },
	{ 56,	L"NobleladyB" },
	{ 57,	L"Enemy_Yan" },
	{ 58,	L"Enemy_Dendrobium" },
	{ 59,	L"Enemy_HedgehogPie" },
	{ 60,	L"Enemy_Python" },
	{ 61,	L"Alexandrian_GirlA" },
	{ 62,	L"GenomeC" },
	{ 63,	L"Pigeon" },
	{ 64,	L"Dali_Woman" },
	{ 65,	L"Treno_Auctioneer" },
	{ 66,	L"Burmecian_SoldierA" },
	{ 67,	L"Housemaid" },
	{ 68,	L"Lindblum_Pub_Barman" },
	{ 69,	L"Owl" },
	{ 70,	L"Lindblum_Officer" },
	{ 71,	L"Stationmaster" },
	{ 72,	L"Mary" },
	{ 73,	L"Sailor" },
	{ 74,	L"Erin" },
	{ 75,	L"ChestA" },
	{ 76,	L"Dagger_Bis" },
	{ 77,	L"Enemy_Anemone" },
	{ 78,	L"Enemy_WhaleZombie" },
	{ 79,	L"Enemy_GiganOctopus" },
	{ 80,	L"Enemy_Zemzelett" },
	{ 81,	L"Enemy_SandScorpion" },
	{ 82,	L"Enemy_Garuda" },
	{ 83,	L"Enemy_Gargoyle" },
	{ 84,	L"Enemy_BlazerBeetle" },
	{ 85,	L"Enemy_Lamia" },
	{ 86,	L"Enemy_Axolotl" },
	{ 87,	L"Enemy_Zuu" },
	{ 88,	L"Enemy_Serpion" },
	{ 89,	L"Enemy_Cerberus" },
	{ 90,	L"Enemy_Griffin" },
	{ 91,	L"Chest_TerranA" },
	{ 92,	L"Enemy_Gnoll" },
	{ 93,	L"Enemy_Catoblepas" },
	{ 94,	L"Enemy_RingLeader" },
	{ 95,	L"Enemy_Ochu" },
	{ 96,	L"Enemy_Malboro" },
	{ 97,	L"Enemy_AxeBeak" },
	{ 98,	L"Zidane" },
	{ 100,	L"Chocobo_Blue" },
	{ 102,	L"Ramuh" },
	{ 103,	L"Daguerreo_Weaponsmith" },
	{ 104,	L"ThugA" },
	{ 105,	L"Enemy_ZombieB" },
	{ 106,	L"Queen_Brahne" },
	{ 107,	L"Cinna" },
	{ 108,	L"Lindblum_ManA" },
	{ 109,	L"MarcusA" },
	{ 110,	L"YoungMan" },
	{ 111,	L"ManA" },
	{ 112,	L"Old_Woman" },
	{ 113,	L"Conductor" },
	{ 114,	L"Oglop" },
	{ 115,	L"Cat" },
	{ 116,	L"Gilgamesh" },
	{ 117,	L"GrandpaA" },
	{ 118,	L"Alexandrian_SoldierB" },
	{ 119,	L"Red_MageA" },
	{ 120,	L"Red_MageB" },
	{ 121,	L"Puck" },
	{ 122,	L"Ticketmaster" },
	{ 123,	L"Hippolady" },
	{ 124,	L"FishMan" },
	{ 125,	L"Enemy_BlackWaltz2" },
	{ 126,	L"Dante" },
	{ 127,	L"Trumpeter" },
	{ 129,	L"Moogle_Female" },
	{ 130,	L"Cymbalist" },
	{ 131,	L"Drummer" },
	{ 132,	L"Violinist" },
	{ 133,	L"Moogle_Book" },
	{ 134,	L"Moogle_Quill" },
	{ 135,	L"Enemy_Vice" },
	{ 136,	L"Enemy_Ladybug" },
	{ 137,	L"Enemy_GiganToadA" },
	{ 138,	L"Enemy_Sahagin" },
	{ 141,	L"Enemy_Abomination" },
	{ 142,	L"Enemy_Stroper" },
	{ 143,	L"Enemy_Myconid" },
	{ 144,	L"Enemy_FeatherCircle" },
	{ 145,	L"Enemy_Bandersnatch" },
	{ 146,	L"Enemy_LandWorm" },
	{ 147,	L"Enemy_Ash" },
	{ 148,	L"Enemy_Drakan" },
	{ 149,	L"Enemy_WraithBlue" },
	{ 150,	L"Enemy_Clipper" },
	{ 151,	L"Enemy_Fang" },
	{ 152,	L"Enemy_Goblin" },
	{ 153,	L"Enemy_SeekerBat" },
	{ 154,	L"Enemy_Flan" },
	{ 155,	L"Enemy_Bomb" },
	{ 156,	L"Enemy_Ghost" },
	{ 157,	L"Enemy_Wyerd" },
	{ 158,	L"Enemy_Mu" },
	{ 159,	L"Enemy_Ironite" },
	{ 160,	L"Enemy_CarrionWorm" },
	{ 161,	L"Enemy_Mandragora" },
	{ 162,	L"Enemy_Skeleton" },
	{ 163,	L"Enemy_Zaghnol" },
	{ 164,	L"Enemy_Torama" },
	{ 165,	L"Enemy_Dracozombie" },
	{ 166,	L"Enemy_Hecteyes" },
	{ 167,	L"Black_Waltz3" },
	{ 168,	L"Black_Waltz2" },
	{ 169,	L"Black_Waltz1" },
	{ 170,	L"Hilda" },
	{ 171,	L"Doctor_Tot" },
	{ 172,	L"Zorn" },
	{ 173,	L"Pub_Waitress" },
	{ 174,	L"Frog_Baby" },
	{ 175,	L"Frog_Female" },
	{ 176,	L"Frog_Male" },
	{ 177,	L"NoblemanA" },
	{ 178,	L"Frog_Tadpole" },
	{ 179,	L"Dali_Innkeeper" },
	{ 180,	L"Dali_ChildB" },
	{ 181,	L"Burmecian_SoldierB" },
	{ 183,	L"ManB" },
	{ 184,	L"Enemy_Tonberry" },
	{ 185,	L"Dagger" },
	{ 186,	L"Baku_Masked" },
	{ 187,	L"Scholar" },
	{ 188,	L"Lindblum_Aviator" },
	{ 190,	L"Blank_Pluto_Knight" },
	{ 191,	L"Cid_Oglop" },
	{ 192,	L"Freya" },
	{ 193,	L"Dog" },
	{ 196,	L"Moogle_Child" },
	{ 198,	L"Mog" },
	{ 199,	L"Artemicion" },
	{ 200,	L"Air_Cab" },
	{ 202,	L"Garnet_HoodedA" },
	{ 203,	L"Zidane_Pluto_Knight" },
	{ 204,	L"Beatrix" },
	{ 205,	L"Garnet_HoodedB" },
	{ 206,	L"Burmecian_Girl" },
	{ 207,	L"Black_Waltz3_Broken" },
	{ 208,	L"SouthGate_Worker" },
	{ 209,	L"Burmecian_King" },
	{ 210,	L"Lowell" },
	{ 212,	L"Stiltzkin" },
	{ 213,	L"YoungNoblemanB" },
	{ 214,	L"Quan" },
	{ 215,	L"Quale" },
	{ 216,	L"Burmecian_Woman" },
	{ 217,	L"Lindblum_Soldier" },
	{ 218,	L"Lindblum_Elite_Soldier" },
	{ 219,	L"Minister_Artania" },
	{ 220,	L"Moogle_Male" },
	{ 221,	L"Enemy_Epitaph" },
	{ 222,	L"Ticket" },
	{ 224,	L"Jump_Rope" },
	{ 225,	L"Tent" },
	{ 227,	L"Purple_Bottle" },
	{ 232,	L"Madain_Sari_Boat" },
	{ 233,	L"Book" },
	{ 234,	L"Coffee_Cup" },
	{ 235,	L"Stew_Pot" },
	{ 236,	L"Cooked_Fish" },
	{ 237,	L"Stew_Plate" },
	{ 240,	L"Dali_Barrel" },
	{ 241,	L"Barrel" },
	{ 243,	L"Sword" },
	{ 244,	L"Enemy_Cactuar" },
	{ 245,	L"Zaghnol" },
	{ 246,	L"Bandersnatch" },
	{ 247,	L"Fang" },
	{ 248,	L"Mu" },
	{ 249,	L"Trick_Sparrow" },
	{ 250,	L"Ralvurahva" },
	{ 251,	L"Ralvuimago" },
	{ 252,	L"Armodullahan" },
	{ 254,	L"Griffin" },
	{ 255,	L"Fabryce" },
	{ 258,	L"Moogle_Letter" },
	{ 259,	L"Chocobo" },
	{ 260,	L"Boco" },
	{ 261,	L"Enemy_Veteran" },
	{ 262,	L"Dali_Cart" },
	{ 264,	L"Weapon_Hamelin" },
	{ 265,	L"Enemy_MagicVice" },
	{ 266,	L"Enemy_Mimic" },
	{ 267,	L"Kuja" },
	{ 268,	L"Conde_Priest" },
	{ 269,	L"Cleyran_Priest" },
	{ 270,	L"Cid_Human" },
	{ 272,	L"Cleyran_High_Priest" },
	{ 273,	L"Quina" },
	{ 274,	L"Thorn" },
	{ 275,	L"Bell" },
	{ 277,	L"Pickaxe" },
	{ 278,	L"Enemy_BlackWaltz3" },
	{ 279,	L"Fishing_Rod" },
	{ 281,	L"Battle_DaggerLongHairA" },
	{ 283,	L"Battle_DaggerLongHairB" },
	{ 284,	L"Battle_EikoA" },
	{ 285,	L"Battle_ZidaneA" },
	{ 286,	L"Battle_SteinerA" },
	{ 287,	L"Battle_DaggerShortHairA" },
	{ 288,	L"Battle_DaggerShortHairB" },
	{ 289,	L"Battle_Quina" },
	{ 290,	L"Battle_Freya" },
	{ 291,	L"Battle_EikoB" },
	{ 294,	L"Gargan_Car" },
	{ 295,	L"Enemy_Quina" },
	{ 296,	L"Enemy_SteinerB" },
	{ 297,	L"Enemy_Freya" },
	{ 298,	L"Enemy_SteinerA" },
	{ 299,	L"Enemy_Zenero" },
	{ 300,	L"Enemy_Thorn" },
	{ 301,	L"Enemy_Baku" },
	{ 302,	L"Enemy_Zorn" },
	{ 303,	L"Enemy_BlackMageB" },
	{ 304,	L"Enemy_BlackMageA" },
	{ 305,	L"Enemy_BlackMageC" },
	{ 306,	L"Gargant" },
	{ 307,	L"World_Moogle" },
	{ 308,	L"World_Chocobo" },
	{ 309,	L"World_Dagger" },
	{ 310,	L"World_Zidane" },
	{ 311,	L"World_Tent" },
	{ 312,	L"World_Hilda_Garde1" },
	{ 313,	L"World_Cargo_Ship" },
	{ 314,	L"World_AirshipA" },
	{ 315,	L"World_AirshipB" },
	{ 316,	L"World_AirshipC" },
	{ 317,	L"World_Invincible" },
	{ 318,	L"World_AirshipD" },
	{ 319,	L"World_AirshipE" },
	{ 320,	L"World_Hilda_Garde3" },
	{ 321,	L"World_Blue_Narciss" },
	{ 322,	L"Weapon_Angel_Flute" },
	{ 323,	L"Enemy_Nymph" },
	{ 324,	L"Enemy_Adamantoise" },
	{ 325,	L"Enemy_Amdusias" },
	{ 326,	L"Enemy_Troll" },
	{ 327,	L"Enemy_Ogre" },
	{ 328,	L"Enemy_Agares" },
	{ 330,	L"Enemy_Dragonfly" },
	{ 331,	L"Enemy_VepalBlue" },
	{ 332,	L"Enemy_Jabberwock" },
	{ 333,	L"Enemy_Grenade" },
	{ 334,	L"Enemy_Armstrong" },
	{ 335,	L"Enemy_RagtimeMouse" },
	{ 336,	L"Enemy_Behemoth" },
	{ 337,	L"Enemy_Chimera" },
	{ 338,	L"Enemy_ShellDragon" },
	{ 339,	L"Enemy_SandGolem" },
	{ 340,	L"Enemy_WormHydra" },
	{ 341,	L"Enemy_Abadon" },
	{ 342,	L"Enemy_Stilva" },
	{ 343,	L"Enemy_IronMan" },
	{ 344,	L"Enemy_Armodullahan" },
	{ 345,	L"Enemy_PlantSpider" },
	{ 346,	L"Enemy_Sealion" },
	{ 347,	L"Enemy_PlantBrain" },
	{ 348,	L"Enemy_Ralvurahva" },
	{ 349,	L"Enemy_Gizamaluke" },
	{ 350,	L"Enemy_Antlion" },
	{ 351,	L"Enemy_Lich" },
	{ 352,	L"Enemy_Ralvuimago" },
	{ 353,	L"Enemy_Tantarian" },
	{ 354,	L"Enemy_ValiaPira" },
	{ 355,	L"Enemy_GrandDragon" },
	{ 356,	L"Enemy_True" },
	{ 357,	L"Enemy_False" },
	{ 358,	L"Battle_Beatrix" },
	{ 359,	L"Enemy_KingLeo" },
	{ 362,	L"Enemy_Hilgigars" },
	{ 363,	L"Enemy_Soulcage" },
	{ 364,	L"Enemy_EarthGuardian" },
	{ 365,	L"Mill_Shovels" },
	{ 366,	L"Weapon_Broadsword" },
	{ 367,	L"Weapon_Ultima_Sword" },
	{ 368,	L"Lani" },
	{ 369,	L"Enemy_GimmeCat" },
	{ 370,	L"Enemy_Tiamat" },
	{ 371,	L"Enemy_Taharka" },
	{ 376,	L"GrandpaB" },
	{ 377,	L"Lani_Axe" },
	{ 378,	L"Enemy_Kraken" },
	{ 379,	L"Enemy_Grimlock" },
	{ 380,	L"Fratley" },
	{ 382,	L"Garland" },
	{ 383,	L"Weapon_Ragnarok" },
	{ 384,	L"Weapon_Mythril_Sword" },
	{ 385,	L"Weapon_Iron_Sword" },
	{ 386,	L"Weapon_Diamond_Sword" },
	{ 387,	L"Weapon_Coral_Sword" },
	{ 388,	L"Weapon_Ice_Brand" },
	{ 389,	L"Weapon_Blood_Sword" },
	{ 390,	L"Weapon_Save_The_Queen" },
	{ 391,	L"Weapon_Rune_Blade" },
	{ 392,	L"Weapon_Flame_Saber" },
	{ 398,	L"Enemy_FriendlyMu" },
	{ 399,	L"Enemy_WraithRed" },
	{ 400,	L"Enemy_FriendlyGhost" },
	{ 403,	L"Enemy_FriendlyYan" },
	{ 404,	L"Enemy_FriendlyGaruda" },
	{ 405,	L"Enemy_Ladybug" },
	{ 406,	L"Gargan_Plate" },
	{ 410,	L"Enemy_Lani" },
	{ 412,	L"ThugB" },
	{ 413,	L"Weapon_Dragon_Hair" },
	{ 414,	L"Daguerreo_ElevatorA" },
	{ 415,	L"Neptune_Statue" },
	{ 416,	L"Enemy_Meltigemini" },
	{ 417,	L"Queen_Stella" },
	{ 419,	L"Dagger_Queen" },
	{ 420,	L"Conde_ChildA" },
	{ 421,	L"Conde_FemaleA" },
	{ 422,	L"Conde_MaleA" },
	{ 423,	L"Frog_Gold" },
	{ 424,	L"Treno_Child" },
	{ 425,	L"Cid_Frog" },
	{ 426,	L"Treno_Guard" },
	{ 427,	L"Enemy_Beatrix" },
	{ 428,	L"Enemy_MaskedMan" },
	{ 429,	L"Enemy_BakuMaskRight" },
	{ 430,	L"Enemy_BakuMaskLeft" },
	{ 431,	L"Gaza_Priest" },
	{ 432,	L"Hooded_Mystery_Man" },
	{ 433,	L"Dagger_Weapon" },
	{ 434,	L"Earth_Mirror" },
	{ 436,	L"Lindblum_Truck" },
	{ 437,	L"Terran_Urn" },
	{ 440,	L"Weapon_Dragon_Claws" },
	{ 441,	L"Weapon_Zorlin_Shape" },
	{ 442,	L"Antlion" },
	{ 443,	L"Eiko" },
	{ 444,	L"Battle_Amarant" },
	{ 445,	L"Enemy_Quale" },
	{ 446,	L"Enemy_Garland" },
	{ 447,	L"Cleyran_Maiden" },
	{ 450,	L"Enemy_BlackWaltz1" },
	{ 451,	L"Enemy_Crawler" },
	{ 452,	L"World_Chocobo_Garden" },
	{ 453,	L"Weapon_Multina_Racket" },
	{ 454,	L"Weapon_Air_Racket" },
	{ 455,	L"Weapon_Rune_Claws" },
	{ 456,	L"Weapon_Duel_Claws" },
	{ 457,	L"Weapon_Tiger_Racket" },
	{ 458,	L"Weapon_Priest_Racket" },
	{ 459,	L"Weapon_Mythril_Racket" },
	{ 460,	L"Weapon_Magic_Racket" },
	{ 461,	L"Weapon_Rod" },
	{ 462,	L"Weapon_Wizard_Rod" },
	{ 463,	L"Weapon_Asura_Rod" },
	{ 464,	L"Weapon_Whale_Whisker" },
	{ 465,	L"Weapon_Javelin" },
	{ 466,	L"Weapon_Heavy_Lance" },
	{ 467,	L"Weapon_Trident" },
	{ 468,	L"Weapon_Partisan" },
	{ 469,	L"Weapon_Holy_Lance" },
	{ 470,	L"Weapon_Obelisk" },
	{ 471,	L"Weapon_Cat_Claws" },
	{ 472,	L"Weapon_Scissor_Fang" },
	{ 473,	L"Weapon_Kaiser_Knuckles" },
	{ 474,	L"Weapon_Avenger" },
	{ 475,	L"Weapon_Mage_Masher" },
	{ 476,	L"Weapon_Dagger" },
	{ 477,	L"Weapon_Gladius" },
	{ 478,	L"Weapon_Mage_Staff" },
	{ 479,	L"Weapon_Octagon_Rod" },
	{ 480,	L"Weapon_Cypress_Pile" },
	{ 481,	L"Weapon_Oak_Staff" },
	{ 482,	L"Weapon_Mace_Of_Zeus" },
	{ 483,	L"Weapon_High_Mage_Staff" },
	{ 484,	L"Weapon_Fork" },
	{ 485,	L"Weapon_Gastro_Fork" },
	{ 486,	L"Weapon_Bistro_Fork" },
	{ 487,	L"Ipsen_Pot" },
	{ 488,	L"Canal_Boat" },
	{ 489,	L"Funeral_Flowers" },
	{ 490,	L"Gysahl_Greens_Bag" },
	{ 491,	L"Weapon_Mythril_Rod" },
	{ 492,	L"Weapon_Healing_Rod" },
	{ 493,	L"Weapon_Stardust_Rod" },
	{ 494,	L"Weapon_Lamia_Flute" },
	{ 495,	L"Weapon_Siren_Flute" },
	{ 496,	L"Weapon_Mythril_Spear" },
	{ 497,	L"Weapon_Ice_Lance" },
	{ 498,	L"Weapon_Poison_Knuckles" },
	{ 499,	L"Weapon_Tiger_Fangs" },
	{ 500,	L"Weapon_Mythril_Claws" },
	{ 501,	L"Weapon_Mythril_Dagger" },
	{ 502,	L"Weapon_Ice_Staff" },
	{ 503,	L"Weapon_Flame_Staff" },
	{ 504,	L"Weapon_Lightning_Staff" },
	{ 505,	L"Weapon_Silver_Fork" },
	{ 506,	L"Weapon_Mythril_Fork" },
	{ 507,	L"Weapon_Needle_Fork" },
	{ 509,	L"Amarant" },
	{ 512,	L"Enemy_Ark" },
	{ 514,	L"Weapon_Sargatanas" },
	{ 515,	L"Weapon_Angel_Bless" },
	{ 516,	L"Weapon_Rune_Tooth" },
	{ 517,	L"Weapon_Exploda" },
	{ 518,	L"Weapon_Ultima_Weapon" },
	{ 519,	L"Weapon_The_Tower" },
	{ 520,	L"Weapon_Masamune" },
	{ 521,	L"Weapon_The_Ogre" },
	{ 522,	L"Weapon_Butterfly_Sword" },
	{ 527,	L"Weapon_Orichalcon" },
	{ 524,	L"Plant_Spider" },
	{ 528,	L"Gizamaluke" },
	{ 529,	L"Earth_Guardian" },
	{ 530,	L"Fire_Guardian" },
	{ 531,	L"Kuja_Trance" },
	{ 532,	L"Zidane_holding_Dagger" },
	{ 533,	L"Taharka" },
	{ 534,	L"Ramuh_Mighty" },
	{ 535,	L"Wind_Guardian" },
	{ 536,	L"Chocobo_Egg" },
	{ 537,	L"Gargan_Flower" },
	{ 539,	L"Mikoto" },
	{ 540,	L"GenomeA" },
	{ 541,	L"GenomeB" },
	{ 542,	L"Fat_Chocobo" },
	{ 543,	L"NobleladyC" },
	{ 545,	L"Enemy_NovaDragon" },
	{ 546,	L"Enemy_CaveImp" },
	{ 547,	L"Drunk" },
	{ 548,	L"Enemy_FriendlyNymph" },
	{ 549,	L"Conde_ChildB" },
	{ 550,	L"Conde_FemaleB" },
	{ 551,	L"Conde_MaleB" },
	{ 553,	L"Enemy_FriendlyJabberwock" },
	{ 555,	L"Save_The_Queen" },
	{ 556,	L"Enemy_Mistodon" },
	{ 557,	L"Enemy_Garnet" },
	{ 558,	L"Enemy_Vivi" },
	{ 564,	L"Conde_HighPriest" },
	{ 566,	L"Enemy_TranceKujaB" },
	{ 567,	L"Enemy_TranceKujaA" },
	{ 569,	L"Enemy_Zidane" },
	{ 570,	L"Enemy_Eiko" },
	{ 571,	L"Enemy_ViviB" },
	{ 572,	L"Enemy_Amarant" },
	{ 573,	L"Enemy_ScarletHair" },
	{ 574,	L"Mistodon" },
	{ 576,	L"Enemy_Hades" },
	{ 580,	L"Battle_TranceZidaneA" },
	{ 581,	L"Battle_TranceZidaneB" },
	{ 582,	L"Battle_TranceDaggerShortHairA" },
	{ 583,	L"Battle_TranceDaggerLongHairA" },
	{ 584,	L"Battle_TranceVivi" },
	{ 585,	L"Battle_TranceFreya" },
	{ 586,	L"PinnacleRocks_Stone" },
	{ 588,	L"FishA" },
	{ 589,	L"FishB" },
	{ 590,	L"FishC" },
	{ 591,	L"Enemy_Soldier" },
	{ 592,	L"Enemy_PlutoKnight" },
	{ 593,	L"Enemy_BlackWaltz3Broken" },
	{ 594,	L"Weapon_Excalibur_II" },
	{ 595,	L"Weapon_Excalibur" },
	{ 596,	L"Iifa_Leave" },
	{ 598,	L"Crier_Owl" },
	{ 600,	L"Lindblum_Worker" },
	{ 601,	L"Battle_TranceDaggerShortHairB" },
	{ 602,	L"Battle_TranceDaggerLongHairB" },
	{ 604,	L"Dali_GirlB" },
	{ 605,	L"Weapon_Kain_Lance" },
	{ 606,	L"Battle_TranceSteiner" },
	{ 607,	L"Battle_TranceQuina" },
	{ 608,	L"Battle_BlankAsPluto" },
	{ 609,	L"Wood_Staff" },
	{ 610,	L"Enemy_RedDragon" },
	{ 612,	L"YoungNoblemanC" },
	{ 613,	L"Chocobo_Red" },
	{ 614,	L"Chocobo_Navy" },
	{ 615,	L"Chocobo_Gold" },
	{ 616,	L"Red_Dragon" },
	{ 617,	L"Amduscias" },
	{ 618,	L"Enemy_KrakenCrystal" },
	{ 619,	L"Enemy_MalirisCrystal" },
	{ 620,	L"Enemy_LichCrystal" },
	{ 621,	L"Enemy_TiamatCrystal" },
	{ 622,	L"Daguerreo_ElevatorB" },
	{ 627,	L"Plank" },
	{ 629,	L"Weapon_Defender" },
	{ 633,	L"Water_Mirror" },
	{ 634,	L"Fire_Mirror" },
	{ 635,	L"Wind_Mirror" },
	{ 637,	L"Big_Eye" },
	{ 638,	L"Garnet_Stone" },
	{ 639,	L"Enemy_Blank" },
	{ 640,	L"Mythril_Spear" },
	{ 641,	L"Gulug_Stone" },
	{ 642,	L"Mog_Ribbon" },
	{ 644,	L"Battle_TranceEikoA" },
	{ 645,	L"Battle_TranceEikoB" },
	{ 647,	L"GrandpaC" },
	{ 648,	L"Battle_TranceAmarant" },
	{ 655,	L"Battle_SteinerB" },
	{ 658,	L"Steiner_Bis" },
	{ 660,	L"MarcusB" },
	{ 663,	L"Enemy_GiganToadB" },
	{ 664,	L"Enemy_Benero" },
	{ 667,	L"Kuja_Burned" },
	{ 671,	L"Enemy_Dagger" },
	{ 672,	L"Enemy_Necron" },
	{ 673,	L"Enemy_DummyB" },
	{ 676,	L"Enemy_VepalRed" },
	{ 677,	L"Treno_Hippo" },
	{ 681,	L"Broadsword" },
	{ 701,	L"ChestB" },
	{ 702,	L"Chest_TerranB" },
	{ 758,	L"Chest_TerranC" },
	{ 5414,	L"Battle_ZidaneB" },
	{ 5415,	L"Battle_Vivi" },
	{ 5458,	L"Enemy_Basilisk" },
	{ 5459,	L"Enemy_LizardMan" },
	{ 5460,	L"Enemy_CarveSpider" },
	{ 5461,	L"Enemy_PrisonCage" },
	{ 5464,	L"Baku" },
	{ 5467,	L"Blank" },
	{ 5468,	L"Black_MageD" },
	{ 5474,	L"Cook" },
	{ 5476,	L"OldNoblemanA" },
	{ 5478,	L"Lindblum_ManB" },
	{ 5488,	L"Pluto_KnightA" },
	{ 5489,	L"Steiner" },
	{ 5492,	L"Alexandrian_GirlB" },
	{ 5500,	L"Nero" },
	{ 5501,	L"King_Leo" },
	{ 5505,	L"Naked_KnightA" },
	{ 5509,	L"Kohel" },
	{ 5511,	L"Naked_KnightB" }
};

#endif
