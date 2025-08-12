﻿#ifndef _HADES_STRINGS_H
#define _HADES_STRINGS_H

#include "Database_Common.h"

// GUI Strings
#define HADES_STRING_HADES_WORKSHOP L"Hades Workshop"
#define HADES_STRING_ERROR L"Error"
#define HADES_STRING_WARNING L"Warning"
#define HADES_STRING_SUCCESS L"Success"
#define HADES_STRING_EMPTY_PROCEDURE L"Nothing to do!"
#define HADES_STRING_ERROR_UNKNOWN L"An unknown error has occured..."
#define HADES_STRING_GENERIC_ADD L"Add"
#define HADES_STRING_GENERIC_NEW L"New"
#define HADES_STRING_GENERIC_REMOVE L"Remove"
#define HADES_STRING_GENERIC_COPY L"Copy"
#define HADES_STRING_GENERIC_PASTE L"Paste"
#define HADES_STRING_GENERIC_PROPERTIES L"Properties..."
#define HADES_STRING_GENERIC_SELECT_ALL L"Select All"
#define HADES_STRING_GENERIC_DESELECT_ALL L"Deselect All"
#define HADES_STRING_GENERIC_EXPORT_SEL L"Export Selection"
#define HADES_STRING_GENERIC_IMPORT_SEL L"Import Selection"
#define HADES_STRING_GENERIC_UNUSED L"Unused"
#define HADES_STRING_NYI L"Not yet implemented"
#define HADES_STRING_NO_STEAM	L" This feature is disabled for the Steam version\n"\
								L" Open the archive p0data2 with the tool Unity Assets Viewer\n"\
								L"to access the Battle Scene 3D models"
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
#define HADES_STRING_OPEN_ERROR_FAIL_STEAM_MISS L"A key file is missing!"
#define HADES_STRING_OPEN_ERROR_BIN_ARCHIVE L"Select FF9_Launcher.exe for the Steam version, not an archive p0dataX.bin"
#define HADES_STRING_OPEN_ERROR_CREATE L"Cannot create file '%s'."
#define HADES_STRING_OPEN_ERROR_OVERWRITE L"Cannot overwrite file '%s'."
#define HADES_STRING_OPEN_ERROR_MEMORIA_WITHOUT_BACKUP	L"Hades Workshop cannot open an Assembly-CSharp.dll modded by the Memoria or Moguri mod. Moreover, the backup DLL is also unusable.\n"\
														L"Potential solutions:\n"\
														L"- Have a copy of the non-modded game,\n"\
														L"- Have a copy of the non-modded Assembly-CSharp.dll and put it in the game's folder \"x64/FF9_Data/Managed/\" under the name \"Assembly-CSharp_Vanilla.dll\",\n"\
														L"- Same as above with the proper non-modded DLL named \"Assembly-CSharp.bak\" instead,\n"\
														L"- Same as above with the proper non-modded DLL named \"Assembly-CSharp_Vanilla.dll\" and placed in Hades Workshop's folder instead.\n\n"\
														L"In order to uninstall properly the Memoria mod and thus retrieve a non-modded DLL, use Steam -> Library -> Final Fantasy IX -> right-click -> Properties -> Local files -> Verify inegrity of game files.\n\n"\
														L"Hades Workshop can generate mods compatible with Moguri/Memoria by selecting the options \"Spreadsheets (Memoria)\" and \"Raw assets\" when using \"Save Steam Mod\"."
#define HADES_STRING_OPEN_WARNING_UNKNOWN	L"Configuration file not found for this binary file.\n"\
											L"The program will perform a scan in order to open it.\n"\
											L"It should take about 1 minute.\n\n"\
											L"Do you want to scan the file?"
#define HADES_STRING_OPEN_WARNING_VERSION	L"The file's configuration is outdated.\n"\
											L"Click OK to update it."
#define HADES_STRING_OPEN_WARNING_DLL_USAGE	"Using the non-modded backup DLL '%s'."
#define HADES_STRING_CLOSE_WARNING_SAVE	L"There are unsaved changes.\n"\
										L"Do you really want to close?"
#define HADES_STRING_CONFIG_FIND_FAIL L"Can't scan '%s' as a Final Fantasy IX binary file."
#define HADES_STRING_CONFIG_FIND_FAIL_RNC L"'%s' has been compressed by a rnc patch and can't be read."
#define HADES_STRING_CONFIG_FIND_SUCCESS L"Successfully scanned!"
#define HADES_STRING_PREFERENCE_OPEN_FAIL L"Cannot open nor create a preference file."
#define HADES_STRING_SAVE_CONFIRM 	L"Be sure to have a backup of your binary file before saving!\n"\
									L"Exporting your changes as a .ppf file may be a safer solution."
#define HADES_STRING_PPF_SAVE_SUCCESS L"PPF file successfully created!"
#define HADES_STRING_STEAM_SAVE_DEFAULT L"HadesWorkshopMod\\"
#define HADES_STRING_STEAM_SAVE_SUCCESS L"Steam modded files successfully created!"
#define HADES_STRING_STEAM_SAVE_ERROR_OPENED_FILES L"You can't overwrite the currently opened Steam files"
#define HADES_STRING_STEAM_SAVE_ERROR_FAIL_READ L"Unable to open the file for reading"
#define HADES_STRING_STEAM_SAVE_ERROR_FAIL_WRITE L"Unable to create the file"
#define HADES_STRING_STEAM_SAVE_CSHARP_PIECES_HEADER	"/******************************************************* \n"\
														" *            Assembly-CSharp_Additions.cs             * \n"\
														" *      This file was generated by Hades Workshop      * \n"\
														" *                                                     * \n"\
														" *  This contains small pieces of C# code meant to     * \n"\
														" * replace parts of source codes of Final Fantasy IX.  * \n"\
														" * More specifically, parts of the source code of      * \n"\
														" * x64/FF9_Data/Managed/Assembly-CSharp.dll or         * \n"\
														" * x86/FF9_Data/Managed/Assembly-CSharp.dll            * \n"\
														" *                                                     * \n"\
														" *  For each piece of code, the method in which it     * \n"\
														" * should be placed (and eventually replace existing   * \n"\
														" * code) is given.                                     * \n"\
														" *  For instance, \"// Method: FF9.ff9item::.cctor\"     * \n"\
														" * means that the following piece of code should be    * \n"\
														" * placed in the method \".cctor\" (constructor) of the  * \n"\
														" * class \"ff9item\" which is itself in the C# directory * \n"\
														" * \"FF9\". Most of the time, it should replace existing * \n"\
														" * code similar to it.                                 * \n"\
														" *******************************************************/\n"
#define HADES_STRING_STEAM_SAVE_CSHARP_CIL_RAW	"// Error: There was raw CIL Code modifications done in Hades Workshop.\n"\
												"// They were ignored and not converted into C# code.\n"\
												"// The involved methods were:\n"
#define HADES_STRING_STEAM_SAVE_CSHARP_MEMORIA	L"Raw CIL Code modifications and macros were not exported\n"\
												L"They are better to be done using Memoria's options or C# modifications"
#define HADES_STRING_HWS_OPEN_SUCCESS L"Data successfully imported!"
#define HADES_STRING_HWS_OPEN_FAIL L"Cannot open '%s' as Final Fantasy IX save."
#define HADES_STRING_HWS_OPEN_WARNING L"Data has been imported with some errors:"
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
#define HADES_STRING_HWS_SAVE_SUCCESS L"Data successfully exported!"
#define HADES_STRING_TXT_SAVE_SUCCESS L"Text successfully exported!"
#define HADES_STRING_TEXTURE_SAVE_SUCCESS L"Texture successfully exported!"
#define HADES_STRING_FIELDTEXTURE_SAVE_SUCCESS L"Background successfully exported!"
#define HADES_STRING_SCENE_SAVE_SUCCESS L"Battle Scene model successfully exported!"
#define HADES_STRING_WALKMESH_SAVE_SUCCESS L"Walkmesh successfully exported!"
#define HADES_STRING_WALKMESH_LOAD_SUCCESS L"Walkmesh successfully imported!"
#define HADES_STRING_TEXT_TRANSLATE L"Auto-Translate"
#define HADES_STRING_ADD_OBJECT_SAME_ID L"An object with that ID already exists!"

extern vector<wxString> HADES_STRING_STEAM_LANGUAGE_SHORT_NAME;
extern vector<wxString> HADES_STRING_STEAM_LANGUAGE_SHORT_NAME_FIX;
extern vector<wxString> HADES_STRING_STEAM_LANGUAGE_LONG_NAME;


//-- HWS Frame
#define HADES_STRING_HWS_OPEN_TITLE L"Open hws..."
#define HADES_STRING_HWS_OPEN_MESSAGE L"Open..."
#define HADES_STRING_HWS_OPEN_CONFIRM L"Import"
#define HADES_STRING_HWS_SAVE_TITLE L"Save hws..."
#define HADES_STRING_HWS_SAVE_MESSAGE L"Save..."
#define HADES_STRING_HWS_SAVE_CONFIRM L"Save"

//-- Common
extern vector<wxString> HADES_STRING_PRINTABLE_CHARTABLE;
#define HADES_STRING_PRINTABLE_CHAR_CHARMAP	L"Á  À  Â  Ä  É  È  Ê  Ë  Í  Ì  Î  Ï  Ó  Ò  Ô  Ö  Ú  Ù  Û  Ü  á  à  â  ä  é  è  ê  ë  í  ì  î  ï  ó  ò  ô  ö  ú  ù  û  ü  Ç  Ñ  ç  ñ  Œ  ß\n"\
											L"“  ”  „  ‘  «  »  ¡  ¿  ×  ―  ∞  ∴  ∵  ∈  ∋  §  ♪  ♦  ♥  ★  ※  ☺  ♂  ♀  △  □  ↑  ↓  →  ←  ¶  \t"

#define HADES_STRING_TEXT_REACH_LIMIT	L"The text size has reached his limit.\n"\
										L"Maybe you can try to remove unused text\n"\
										L"to increase the limit here."

#define HADES_STRING_DATA_REACH_LIMIT	L"The data size has reached his limit."
#define HADES_STRING_SPECIAL_CHAR_VALIDATE		L"✔"
#define HADES_STRING_SPECIAL_CHAR_INVALIDATE	L"✘"

extern vector<wxString> HADES_STRING_ELEMENT_NAME;
extern vector<wxString> HADES_STRING_STATUS_NAME;

//-- Spell Panel
#define HADES_STRING_SPELL_TARGET_INFO_BASE L" Recommanded compatibility\nwith: "
extern vector<vector<wxString>> HADES_STRING_SPELL_TARGET_INFO;

//-- Command Panel
#define HADES_STRING_COMMAND_ADD_SPELL_ADD	L"Add Spell"
#define HADES_STRING_COMMAND_ADD_SPELL_EDIT	L"Set Spell"
#define HADES_STRING_COMMAND_REACH_LIMIT	L"You can't add spells to the list anymore.\n"\
											L"Maybe you can try to remove spells\n"\
											L"elsewhere or link commands together\n"\
											L"to increase the limit here."
#define HADES_STRING_COMMAND_CANT_CHANGE_LINK	L"Can only set link with another command verifying:\n"\
												L" - it uses the spell panel,\n"\
												L" - it is not linked to any command yet."

//-- Stat Panel
#define HADES_STRING_STAT_LEVEL	L"Level Progression"
extern vector<wxString> HADES_STRING_CHARACTER_DEFAULT_NAME;

//-- Party Special
extern vector<wxString> HADES_STRING_PARTY_SPECIAL_DATA;
#define HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SET			"Magic Sword %d"
#define HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_SUPPORTER	L"Caster: "
#define HADES_STRING_PARTY_SPECIAL_SWORD_MAGIC_BENEFICIARY	L"Knight: "

//-- Enemy Panel
#define HADES_STRING_NOGROUPEDIT	L"Warning: this type of data is not modified on similar enemies"
#define HADES_STRING_STAT_TOO_HIGH	L"Warning: speed and spirit should not be 60 or higher"
#define HADES_STRING_GROUPEDIT_ERROR_TEXT L"A similar battle is limited in size: failed to change the text."
#define HADES_STRING_GROUPEDIT_ERROR_DATA L"A similar battle is limited in size: failed to change some of its data."
#define HADES_STRING_STAT_WARN_LIMIT	L"A battle with more than 3 different enemy types will bug\n"\
										L"unless you increase the limit in the CIL method FF9StateBattleSystem::.ctor (Steam only)"
#define HADES_STRING_SPELL_WARN_LIMIT	L"A battle with more than 18 different attacks will bug\n"\
										L"unless you increase the limit in the CIL method FF9StateBattleSystem::.ctor (Steam only)"

//-- Field Panel
#define HADES_STRING_WALKMESH_PROPAGATE_FLAGS L"Propagate flags to walkpath"
#define HADES_STRING_WALKMESH_ERROR_NO_TRIANGLE L"Failed to add a triangle that way"
#define HADES_STRING_WALKMESH_INVALID_TRIANGLE L"That triangle is degenerate!"
#define HADES_STRING_WALKMESH_WRONG_CONNECTION L"A neighbour triangle already exists at that position"
#define HADES_STRING_WALKMESH_SIDE_1 L"1st side"
#define HADES_STRING_WALKMESH_SIDE_2 L"2nd side"
#define HADES_STRING_WALKMESH_SIDE_3 L"3rd side"
#define HADES_STRING_WALKMESH_IMPORT_FAIL L"Format error when parsing the object"
#define HADES_STRING_WALKMESH_IMPORT_SUCCESS	"Walkmesh successfully imported:\n"\
												" - %d walkpaths (different objects)\n"\
												" - %d triangles\n"\
												" - %d vertices"
#define HADES_STRING_WALKMESH_IMPORT_DEGEN_TRI "%d triangles were degenerate (remember that vertex positions are rounded to whole numbers)"
#define HADES_STRING_WALKMESH_IMPORT_ANIM_LOSS L"This field contained a walkmesh animation that got lost in the process"
#define HADES_STRING_WALKMESH_IMPORT_NORMALS L"Triangles can accept only 1 normal (not 1 per vertex); the first normal was used everytimes it conflicted"
#define HADES_STRING_WALKMESH_IMPORT_QUADS L"Only triangular faces are accepted; polygons with more than 3 vertices were cut down to 3 vertices"
#define HADES_STRING_WALKMESH_IMPORT_HINT L"You would better edit the imported walkmesh in order to check it and setup its pathing flags"

//-- Item Panel
#define HADES_STRING_ITEM_STAT_SET			L"Stat Set "
#define HADES_STRING_ITEM_STAT_SET_PERSO	L"Individual Stat Set"

//-- Shop Panel
#define	HADES_STRING_SHOP_CUSTOM_NAME			"Custom shop %d"
#define HADES_STRING_SHOP_REACH_LIMIT			L"Can't add more than 32 items to a shop."
#define HADES_STRING_SYNTH_REACH_LIMIT			"Synthesis Shops can't synth more than %d items.\n"
#define HADES_STRING_SYNTH_REACH_LIMIT_TOKEN	"\n - %s has %d items."
#define HADES_STRING_SYNTH_ADD_ITEM_ADD			L"Add Item"
#define HADES_STRING_SYNTH_ADD_ITEM_EDIT		L"Set Item"
extern vector<wxString> HADES_STRING_SYNTH_NAME;

//-- Scripts
#define HADES_STRING_ON_OFF_BUTTON				L"Check"
#define HADES_STRING_NULL_CHARACTER_SLOT		L"Null Character"
#define HADES_STRING_SCRIPT_NEW_ENTRY	 		L"New Entry"
#define HADES_STRING_ENTRY_ARG_LOST				"%u script lines used an entry that was deleted"
#define HADES_STRING_SCRIPT_NO_DELETE	 		L"Can't delete the Main_Init function"
#define HADES_STRING_SCRIPT_SAME_FUNCTYPE 		L"Better not create two functions with the same type for the same entry"
#define HADES_STRING_MISSING_SPACE				L"Not enough space available"
#define HADES_STRING_SCRIPT_SHOULDPARSE			L"Are you sure?\n"\
												L"Some function(s) have been modified but are not parsed."
#define HADES_STRING_LOGERROR_SPACE				" - Not enough space: data is %d bytes too heavy\n"
#define HADES_STRING_SCRIPT_RETURNMISSING		" - Line %u: Missing a 'return' or a 'loop'\n"
#define HADES_STRING_SCRIPT_BLOCKMISSING		" - Line %u: Missing %u block delimiter(s)\n"
#define HADES_STRING_SCRIPT_BLOCKTOOMANY		" - Line %u: Using a block delimiter out of any block\n"
#define HADES_STRING_SCRIPT_EXPECT				" - Line %u: Expected '%s'\n"
#define HADES_STRING_SCRIPT_UNEXPECT			" - Line %u: Unexpected '%s'\n"
#define HADES_STRING_SCRIPT_NUMBER				" - Line %u: Expected a number\n"
#define HADES_STRING_SCRIPT_EMPTY				" - Line %u: Empty argument\n"
#define HADES_STRING_SCRIPT_NOT_AN_OPCODE		" - Line %u: The code '%s' cannot be an opcode because its ID 0x%x ends with 0xff\n"
#define HADES_STRING_SCRIPT_ARGAMOUNT			" - Line %u: Opcode '%s' takes %u arguments; got %u\n"
#define HADES_STRING_SCRIPT_NOVARARG			" - Line %u: Opcode '%s' accepts numerical arguments only\n"
#define HADES_STRING_SCRIPT_UNKNOWN				" - Line %u: Unknown word '%s'\n"
#define HADES_STRING_SCRIPT_NOTSWITCH			" - Line %u: Using the keyword '%s' out of a switch\n"
#define HADES_STRING_SCRIPT_NOBREAK				" - Line %u: Using the keyword '%s' out of a breakable block\n"
#define HADES_STRING_SCRIPT_CASE				" - Line %u: Cases amount doesn't match\n"
#define HADES_STRING_SCRIPT_CASE_TWICE			" - Line %u: Case '%s' handled twice\n"
#define HADES_STRING_SCRIPT_CASE_RANGE			" - Line %u: Case is not in range\n"
#define HADES_STRING_SCRIPT_LOOP				" - Line %u: Looping functions can only be made in top-level blocks\n"
#define HADES_STRING_SCRIPT_FORWARD				" - Line %u: When the keyword '%s' is used, it must be used the function's only line\n"
#define HADES_STRING_SCRIPT_FORWARD_END			" - Line %u: There is no function next to forward to\n"
#define HADES_STRING_SCRIPT_VARARG_MAIN			" - Line %u: %s"
#define HADES_STRING_SCRIPT_VARARG_EMPTY		"Empty argument\n"
#define HADES_STRING_SCRIPT_VARARG_UNEXPECTED	"Unexpected '%s' in variable code\n"
#define HADES_STRING_SCRIPT_VARARG_TOO_PAR		"Too many ')' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_TOO_COMMA	"Too many arguments for '%s' in variable code\n"
#define HADES_STRING_SCRIPT_VARARG_TOO_VALUE	"Unexpected value '%s' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_MISS_PAR		"Missing ')' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_MISS_COMMA	"Missing an argument for '%s' in variable code\n"
#define HADES_STRING_SCRIPT_VARARG_MISS_VALUE	"Expected a value instead of '%s'\n"
#define HADES_STRING_SCRIPT_VARARG_OPEN_PAR		"Expected '(' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_GETENTRY		"Can only use numerical numbers inside a GetEntry call\n"
#define HADES_STRING_SCRIPT_VARARG_ARRAY		"Can only use numerical numbers or field names inside brackets\n"
#define HADES_STRING_SCRIPT_VARARG_BRACKETS		"Expected ']' in argument\n"
#define HADES_STRING_SCRIPT_VARARG_OPVAMISMATCH	"Amount of operations and values don't match\n"
#define HADES_STRING_SCRIPT_VARARG_TOO_HIGH		"Number '%s' exceeds the limit (%u) in argument\n"
#define HADES_STRING_SCRIPT_VARARG_UNKNOWN		"Unrecognized '%s' in argument\n"
#define HADES_STRING_SCRIPT_IGNORE				" - Line %u: Ignored trailing '%s'\n"
#define HADES_STRING_SCRIPT_IGNORE_POSTRET		" - Line %u: Unexpected code after the end of the function\n"
#define HADES_STRING_LSCRIPT_ALLOC_NUMBER		L" - Local Variable: Expected a number after 'allocate'\n"
#define HADES_STRING_LSCRIPT_MISS_NAME			L" - Local Variable: Expected a name for local variable\n"
#define HADES_STRING_LSCRIPT_UNEXPECTED			" - Local Variable: Unexpected '%s'\n"
#define HADES_STRING_LSCRIPT_MISS_ALLOC			" - Local Variable: Need to allocate '%u'\n"

//-- CIL Editor
#define HADES_STRING_CILSCRIPT_NOTINST			" - IL_%.4X: '%s' is not a CIL instruction; line ignored\n"
#define HADES_STRING_CILSCRIPT_NOTARG			" - IL_%.4X: Expected an argument instead of '%s'\n"
#define HADES_STRING_CILSCRIPT_WRONGTYPECHECK	" - IL_%.4X: Expected a list of check types, '%s', '%s' or '%s'\n"
#define HADES_STRING_CILSCRIPT_WRONGILPOS		" - IL_%.4X: '%s' is not a valid IL position\n"
#define HADES_STRING_CILSCRIPT_RANGEILPOS		" - IL_%.4X: '%s' is out of range for this instruction's short form\n"

//-- File Batching
#define HADES_STRING_BATCH_NOTHING					" - Nothing done\n"
#define HADES_STRING_BATCH_FIELD_NOT_LOADED			L" - It is a field script and fields are not loaded\n"
#define HADES_STRING_BATCH_WORLD_NOT_LOADED			L" - It is a world script and worlds are not loaded\n"
#define HADES_STRING_BATCH_ENEMY_NOT_LOADED			L" - It is a battle script and battles are not loaded\n"
#define HADES_STRING_BATCH_WRONG_FILETYPE			" - Line %u: Expected a %s filetype\n"
#define HADES_STRING_BATCH_INVALID_FILETYPE			" - Line %u: Invalid filetype\n"
#define HADES_STRING_BATCH_FILETYPE_TWICE			" - Line %u: Can't define typefile twice\n"
#define HADES_STRING_BATCH_FILETYPE_NOTFIRST		" - Line %u: First command must be the filetype\n"
#define HADES_STRING_BATCH_MISSING_INTEGER			" - Line %u: '%s' takes an integral argument\n"
#define HADES_STRING_BATCH_TOO_MANY					" - Line %u: Too many %s declarations\n"
#define HADES_STRING_BATCH_TEXT_WRONG_ID			" - Line %u: Text Block %u doesn't have a text n°%u\n"
#define HADES_STRING_BATCH_TEXT_REDEFINITION		" - Line %u: Redefinition of text n°%u\n"
#define HADES_STRING_BATCH_INVALID_LANGUAGE			" - Line %u: '%s' is not recognized as a language\n"
#define HADES_STRING_BATCH_TEXT_UNUSED				" - Text Block %u unused in this disc\n"
#define HADES_STRING_BATCH_TEXT_SIZE				" - Text Block %u capacity is %u bytes too short\n"
#define HADES_STRING_BATCH_TEXT_MISSMATCH_CODE		" - Text Block %u, Text %u: Expected %u opcode characters, got %u\n"
#define HADES_STRING_BATCH_FIELD_UNUSED				" - Line %u: Field %u is unknown\n"
#define HADES_STRING_BATCH_BATTLE_UNUSED			" - Line %u: Battle %u is unknown\n"
#define HADES_STRING_BATCH_WORLD_UNUSED				" - Line %u: World Map %u is unknown\n"
#define HADES_STRING_BATCH_SCRIPT_FUNCTION			"In %s of %s:\n"
#define HADES_STRING_BATCH_SCRIPT_NOGLOBALS			" - Line %u: Unexpected 'endglobals' out of a global variable block\n"
#define HADES_STRING_BATCH_SCRIPT_NOLOCALS			" - Line %u: Unexpected 'endlocals' out of a local variable block\n"

//-- World Maps
#define HADES_STRING_WORLD_BATTLE_NAME		"Battle Spot %d"
#define HADES_STRING_WORLD_FRIENDLY			"[Friendly] "
#define HADES_STRING_WORLD_FRIENDLY_HELP	L"Friendly monster battles should follow these rules:\n"\
											L"1) They should only be the 4th battle of a battle spot\n"\
											L"2) They cannot appear in more than 12 spots (including the different versions of these spots)"\

//-- Card Panel
#define HADES_STRING_CARD_SHARED	L"Common Datas"
#define HADES_STRING_CARD_NOCARD	L"No Card"

//-- TIM Panels
#define HADES_STRING_TIM_REACH_LIMIT	L"Can't add more texture data."

//-- Image Map Panels
#define HADES_STRING_IMAGE_MAP_REACH_LIMIT	L"Can't add more files to the preloading list."
extern vector<wxString> HADES_STRING_IMAGE_MAP_OBJECT;

//-- Spell Animations
#define HADES_STRING_SPELL_ANIMATION_SEQUENCE_FULL	L"Can't add more sequence code to the animation."

//-- Special Texts
extern vector<wxString> HADES_STRING_SPECIAL_TEXT_BLOCK;
extern vector<wxString> HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM;

//-- Mips & CIL
#define HADES_STRING_MIPS_FULL_CODE		L"Full Code"
#define HADES_STRING_MIPS_OOB			"RAM position is out of bounds [0x%X, 0x%X]"
#define HADES_STRING_CIL_NO_METHOD		L"Can't find a method body\n"\
										L"Method is external"
#define HADES_STRING_CIL_PROTECTED		L"This method contains some database specifically handled by Hades Workshop. You can't modify it."
#define HADES_STRING_CIL_APPLY_MACRO	L"Apply Macro"
#define HADES_STRING_CIL_UNAPPLY_MACRO	L"Unapply Macro"

//-- Tools
#define HADES_STRING_UNKNOWN_FIELD L"UNKNOWN_FIELD"
#define HADES_STRING_INVALID_FF9LAUNCHER L"Select a valid FF9_Launcher.exe"
#define HADES_STRING_INVALID_IMAGE_NAME_FORMAT L"The file name format must contain three '%%'"
#define HADES_STRING_CALCULATOR_LOAD_SECTIONS L"The tool needs to load the sections Spells, Stats, Items and Enemies"
#define HADES_STRING_CALCULATOR_PROFILE_DEFAULT "Character Profile "
#define HADES_STRING_CALCULATOR_LOAD_NONE L"There is no profile saved yet"
#define HADES_STRING_CALCULATOR_SAVE_ERROR L"Something went wrong!"
#define HADES_STRING_CALCULATOR_LOAD_ERROR L"Something went wrong!"
#define HADES_STRING_BACKGROUND_LOAD_SECTIONS L"The tool needs to load the Fields section"
#define HADES_STRING_IMPORT_BACKGROUND_RESULT	"%d background importations succeeded\n"\
												"%d background importations failed"
#define HADES_STRING_BACKGROUNDIMPORT_MISSING_LAYERS	" - Missing %d layers for the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_CREATE	" - Failed to convert the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_DIMENSIONS	" - The dimensions of the layers don't match for the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_ALPHA	" - One or several layer(s) have no alpha channel for the field %d (%s)\n"
#define HADES_STRING_BACKGROUNDIMPORT_ERROR_NO_BACKGROUND	"Pick a valid field!"
#define HADES_STRING_UNITYVIEWER_GAME_OPEN	L"This game folder is already opened in Hades Workshop's main module. Updating the assets is not possible in this situation."
#define HADES_STRING_UNITYVIEWER_IMPORT_ERROR_FORMAT	"Default format could not be retrieved for '%s'"
#define HADES_STRING_UNITYVIEWER_IMPORT_DEFAULT_FORMAT	"No compression format specified for '%s'.\n"\
														"File imported with RGBA format (no compression)"
#define HADES_STRING_UNITYVIEWER_MISSING_AKB	"Expected an AKB header in '%s' but none was found"
#define HADES_STRING_UNITYVIEWER_MISSING_OGG	"Expected an OGG format for '%s'"
#define HADES_STRING_UNITYVIEWER_IMPORT_SUCCESS	"Successfully imported %d asset(s)."
#define HADES_STRING_UNITYVIEWER_IMPORT_ERROR	"Successfully imported %d asset(s).\n"\
												"Failed to import %d asset(s)."
#define HADES_STRING_UNITYVIEWER_IMPORT_NO_REPLACE	"\nArchive not updated: saved the output as .tmp file."
#define HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL	L"\nImportation of the model animations canceled."
#define HADES_STRING_UNITYVIEWER_IMPORTANIM_SUCCESS	"Successfully imported %d animation(s) in p0data5.bin."
#define HADES_STRING_UNITYVIEWER_UNKNOWN_FORMAT	L"Format of '%s' not supported"
#define HADES_STRING_UNITYVIEWER_MODEL_BAD_HIERARCHY	L"Failed to read the asset's file hierarchy"
#define HADES_STRING_UNITYVIEWER_LINK_TEXTURE	L"The following Texture could not be automatically identified. Specify the corresponding file:\n"
#define HADES_STRING_UNITYVIEWER_IMPORT_MISSING	"%d nodes are new and thus not imported\n"\
												"Change the model importing options to allow the importation of new assets"
#define HADES_STRING_UNITYVIEWER_INVALID_FILE_INFO	L"Internal ID must be a 16-digit long hexadecimal number"
#define HADES_STRING_UNITYVIEWER_USED_FILE_INFO	L"Internal ID already used by another asset"
#define HADES_STRING_UNITYVIEWER_LOW_FILE_INFO	"Low Internal ID: the file will be inserted as #%d"
#define HADES_STRING_UNITYVIEWER_WARNING_FILE_INFO	L"Unpredicted behaviour will happen if two files use the same internal ID\n"\
													L"Confirm nonetheless?"
#define HADES_STRING_UNITYVIEWER_INVERT_SELECTION L"Invert selection"
#define HADES_STRING_UNITYVIEWER_EXPORT_HIERARCHY L"Export model hierarchy of selection"
#define HADES_STRING_UNITYVIEWER_HIERARCHY_HEADER "// Hierarchy dependancy of file #"
#define HADES_STRING_UNITYVIEWER_GENERATE_ANIMATION L"Generate animation transition from 2 animations"
#define HADES_STRING_UNITYVIEWER_GENERATE_TITLE L"Animation transition"
#define HADES_STRING_UNITYVIEWER_GENERATE_CHOICE L"Pick animation transition and duration"
#define HADES_STRING_UNITYVIEWER_REMOVE_ASSETS L"Delete assets"
#define HADES_STRING_UNITYVIEWER_WARNING_REMOVE	L"This will delete the selected assets from the archive.\n\n"\
												L"Do you want to generate the resulting archive as a duplicate (otherwise, the archive with fewer assets will replace the existing one)?"


//-- Memoria
#define HADES_STRING_MEMORIA_LOAD_SECTIONS "This action requires to load the %s"
#define HADES_STRING_MEMORIA_CUSTOM_SUCCESS "Successfully generated assets"
#define HADES_STRING_MEMORIA_CUSTOM_FAIL "Failed to generate assets"


//-- Others
#define HADES_STRING_FONT_ERROR L"Unsupported font."

#endif
