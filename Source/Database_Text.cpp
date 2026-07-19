#include "Database_Text.h"

map<unsigned int, TextBlockInfo> HADES_STRING_TEXT_BLOCK_NAME = {
	{ 0x0, { 0, L"Generic UI" } },
	{ 0x1, { 239, L"Debug Room" } },
	{ 0x2, { 387, L"Prima Vista" } },
	{ 0x3, { 338, L"Alexandria Castle (1)" } },
	{ 0x4, { 256, L"Evil Forest" } },
	{ 0x7, { 137, L"Cleyra Trunk" } },
	{ 0x8, { 211, L"Ice Cavern" } },
	{ 0x12, { 166, L"Red Rose" } },
	{ 0x16, { 688, L"Lindblum Castle (1)" } },
	{ 0x17, { 213, L"Mist Gates" } },
	{ 0x1E, { 118, L"Terra" } },
	{ 0x1F, { 168, L"Mountain Path" } },
	{ 0x20, { 396, L"Conde Petie" } },
	{ 0x21, { 430, L"Alexandria (1)" } },
	{ 0x25, { 484, L"Black Mage Village (1)" } },
	{ 0x26, { 96, L"Mognet Central" } },
	{ 0x28, { 412, L"Prima Vista (Ruin)" } },
	{ 0x2A, { 152, L"Quan's Dwelling" } },
	{ 0x2C, { 302, L"Cleyra (with Sandstorm)" } },
	{ 0x2F, { 612, L"Dali" } },
	{ 0x32, { 218, L"Dali Underground" } },
	{ 0x33, { 205, L"Gizamaluke Grotto" } },
	{ 0x34, { 422, L"Bran Bal" } },
	{ 0x35, { 340, L"Observatory Mountain" } },
	{ 0x3F, { 327, L"Cargo Ship" } },
    { TEXT_BLOCK_WORLD_MAP, { 92, L"World Map" } },
	{ 0x46, { 549, L"Treno (1)" } },
	{ 0x47, { 394, L"Qu's Marsh" } },
	{ 0x4A, { 552, L"South Gate" } },
	{ 0x4D, { 339, L"Burmecia" } },
	{ 0x58, { 508, L"Alexandria Castle (3)" } },
	{ 0x59, { 178, L"Alexander" } },
	{ 0x5A, { 557, L"Alexandria (2)" } },
	{ 0x5B, { 181, L"Alexandria (Night)" } },
	{ 0x79, { 166, L"Shrines" } },
	{ 0x7C, { 231, L"Oeilvert" } },
	{ 0x86, { 297, L"Pinnacle Rocks" } },
	{ 0xA6, { 255, L"Daguerreo" } },
	{ 0xBA, { 273, L"Hilda Garde III" } },
	{ 0xBB, { 194, L"Ending" } },
	{ 0xBD, { 175, L"Invincible" } },
	{ 0xDF, { 96, L"Treno & Supersoft" } },
	{ 0x114, { 705, L"Lindblum (1)" } },
	{ 0x121, { 457, L"Alexandria Castle (2)" } },
	{ 0x122, { 402, L"Cleyra (Stormless)" } },
	{ 0x158, { 493, L"Pandemonium" } },
	{ 0x166, { 920, L"Madain Sari (1)" } },
	{ 0x167, { 205, L"Gargan Roo" } },
	{ 0x168, { 127, L"Madain Sari (2)" } },
	{ 0x169, { 234, L"Fossil Roo" } },
	{ 0x1E4, { 253, L"Mount Gulug" } },
	{ 0x1E5, { 393, L"Lindblum (Occupied)" } },
	{ 0x20D, { 238, L"Lindblum Castle (Occupied)" } },
	{ 0x253, { 502, L"Lindblum (2)" } },
	{ 0x2B6, { 471, L"Memoria" } },
	{ 0x2E2, { 359, L"Iifa Tree Roots" } },
	{ 0x2E3, { 256, L"Ipsen Castle" } },
	{ 0x2E4, { 439, L"Desert Palace" } },
	{ 0x2E5, { 674, L"Treno (2)" } },
	{ 0x2F2, { 144, L"Alexandria Castle (Ruin)" } },
	{ 0x38C, { 191, L"Esto Gaza" } },
	{ 0x3AF, { 643, L"Lindblum Castle (2)" } },
	{ 0x3B0, { 310, L"Iifa Tree" } },
	{ 0x3B1, { 467, L"Chocobo Places" } },
	{ 0x3B2, { 277, L"Alexandria (Ruin)" } },
	{ 0x431, { 236, L"Black Mage Village (2)" } },
    { 0x1000, { 0, L"Charmap" } },
    { 0x2047, { 394, L"Qu's Marsh (mobile)" } }
};

vector<SortedChoiceItemTextOpcode> HADES_STRING_TEXT_OPCODE = {
	{ 0x00, L"0x00", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x01, L"TIMED_END", L"", L"Terminate the dialog after few seconds\nOnly use at the end of the string", 1, 1, new wstring[1]{ L"Time to wait" } },
	{ 0x02, L"SLOW", L"", L"Slow down the appearing rate\n1 is the slowest", 1, 1, new wstring[1]{ L"Slow ratio" } },
	{ 0x03, L"FORMAT:Bubble Position", L"", L"Configure the absolute position of the bubble", 3, TEXT_OPCODE_FORMAT_TYPE + 1, new wstring[3]{ L"X position", L"Z position", L"Y position" } },
	{ 0x04, L"TOKENIZE", L"", L"Create tokens from the string to be used in other strings\nMust be the only opcode of the string", -1, 4, new wstring[2]{ L"Token amount", L"Tokens" } },
	{ 0x05, L"0x05", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x06, L"VAR_TOKEN", L"Token", L"Replaced by a tokenized string from another string", 2, 2, new wstring[2]{ L"Text ID", L"Script ID" } },
	{ 0x07, L"FORMAT:Multichoice", L"", L"Setup a multichoice's number of choices and its 'Cancel' choice", 2, TEXT_OPCODE_FORMAT_TYPE + 2, new wstring[2]{ L"Number of choices", L"Cancel choice" } },
	{ 0x08, L"MULTICHOICE", L"", L"Each line becomes a choice inside the dialog", 0, 0, NULL },
	{ 0x09, L"AUTO_END", L"", L"Terminate the dialog after few seconds\nOnly use at the end of the string", 0, 0, NULL },
	{ 0x0A, L"DISPLAY_PROGRESSIVE", L"", L"Progressively display the text (default behavior)", 0, 0, NULL },
	{ 0x0B, L"FORMAT:Skip Closing Animation", L"", L"Instantly hide the text window instead of shrinking it on closure", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x0C, L"DISPLAY_INSTANT", L"", L"Instantly display the whole string", 0, 0, NULL },
	{ 0x0D, L"FORMAT:Width", L"", L"Setup variable bubble width: arguments are -1 terminated lists using the pattern (line number, default width, (opcode1, opcode1_args, ...), ...)", -1, TEXT_OPCODE_FORMAT_TYPE + 4, NULL },
	{ 0x0E, L"VAR_ITEM", L"Item", L"Replaced by some item's name set by the game script", 1, 1, new wstring[1]{ L"Script ID" } },
	{ 0x0F, L"PROGRESSION", L"", L"Update the dialog's step progression for use of the game script", 1, 1, new wstring[1]{ L"Step" } },
	{ 0x10, L"ZIDANE", L"Zidane", L"Replaced by Zidane's name", 0, 0, NULL },
	{ 0x11, L"VIVI", L"Vivi", L"Replaced by Vivi's name", 0, 0, NULL },
	{ 0x12, L"DAGGER", L"Dagger", L"Replaced by Dagger's name", 0, 0, NULL },
	{ 0x13, L"STEINER", L"Steiner", L"Replaced by Steiner's name", 0, 0, NULL },
	{ 0x14, L"FREYA", L"Freya", L"Replaced by Freya's name", 0, 0, NULL },
	{ 0x15, L"QUINA", L"Quina", L"Replaced by Quina's name", 0, 0, NULL },
	{ 0x16, L"EIKO", L"Eiko", L"Replaced by Eiko's name", 0, 0, NULL },
	{ 0x17, L"AMARANT", L"Amarant", L"Replaced by Amarant's name", 0, 0, NULL },
	{ 0x18, L"PARTY_1", L"Cinna", L"Replaced by the name of the external character", 0, 0, NULL },
	{ 0x19, L"PARTY_2", L"Marcus", L"Replaced by the name of the external character (2)", 0, 0, NULL },
	{ 0x1A, L"PARTY_3", L"Blank", L"Replaced by the name of the external character (3)", 0, 0, NULL },
	{ 0x1B, L"PARTY_4", L"Beatrix", L"Replaced by the name of the external character (4)", 0, 0, NULL },
	{ 0x1C, L"0x1C", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x1D, L"0x1D", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x1E, L"0x1E", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x1F, L"0x1F", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x20, L"WHITE", L"", L"Change the color to white", 0, 0, NULL },
	{ 0x21, L"WHITE_NO_SHADOW", L"", L"Change the color to white with no shadow", 0, 0, NULL },
	{ 0x22, L"GREY", L"", L"Change the color to grey", 0, 0, NULL },
	{ 0x23, L"RED", L"", L"Change the color to red", 0, 0, NULL },
	{ 0x24, L"YELLOW", L"", L"Change the color to yellow", 0, 0, NULL },
	{ 0x25, L"CYAN", L"", L"Change the color to cyan", 0, 0, NULL },
	{ 0x26, L"MAUVE", L"", L"Change the color to mauve", 0, 0, NULL },
	{ 0x27, L"GREEN", L"", L"Change the color to green", 0, 0, NULL },
	{ 0x28, L"BLACK_HELP", L"", L"Change the color to black\nHelp dialogs only", 0, 0, NULL },
	{ 0x29, L"RED_HELP", L"", L"Change the color to red\nHelp dialogs only", 0, 0, NULL },
	{ 0x2A, L"0x2A", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2B, L"0x2B", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2C, L"0x2C", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2D, L"0x2D", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2E, L"0x2E", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x2F, L"BUTTON_PAD", L"Pad", L"Replaced by a directional cross image", 0, 0, NULL },
	{ 0x30, L"BUTTON_CIRCLE", L"Circle", L"Replaced by a Circle button", 0, 0, NULL },
	{ 0x31, L"BUTTON_CROSS", L"Cross", L"Replaced by a Cross button", 0, 0, NULL },
	{ 0x32, L"BUTTON_TRIANGLE", L"Triangle", L"Replaced by a Triangle button", 0, 0, NULL },
	{ 0x33, L"BUTTON_SQUARE", L"Square", L"Replaced by a Square button", 0, 0, NULL },
	{ 0x34, L"BUTTON_L1", L"L1", L"Replaced by a L1 button", 0, 0, NULL },
	{ 0x35, L"BUTTON_R1", L"R1", L"Replaced by a R1 button", 0, 0, NULL },
	{ 0x36, L"BUTTON_L2", L"L2", L"Replaced by a L2 button", 0, 0, NULL },
	{ 0x37, L"BUTTON_R2", L"R2", L"Replaced by a R2 button", 0, 0, NULL },
	{ 0x38, L"BUTTON_START", L"Start", L"Replaced by a Start button", 0, 0, NULL },
	{ 0x39, L"BUTTON_SELECT", L"Select", L"Replaced by a Select button", 0, 0, NULL },
	{ 0x3A, L"BUTTON_UP", L"Up", L"Replaced by a Up button", 0, 0, NULL },
	{ 0x3B, L"BUTTON_DOWN", L"Down", L"Replaced by a Down button", 0, 0, NULL },
	{ 0x3C, L"BUTTON_LEFT", L"Left", L"Replaced by a Left button", 0, 0, NULL },
	{ 0x3D, L"BUTTON_RIGHT", L"Right", L"Replaced by a Right button", 0, 0, NULL },
	{ 0x3E, L"BUTTON_L2", L"L2", L"Replaced by a L2 button", 0, 0, NULL },
	{ 0x3F, L"BUTTON_R2", L"R2", L"Replaced by a R2 button", 0, 0, NULL },
	{ 0x40, L"VAR_NUM", L"0", L"Replaced by some number set by the game script", 1, 1, new wstring[1]{ L"Script ID" } },
	{ 0x41, L"PAUSE", L"", L"Stop the appearance for few seconds", 1, 1, new wstring[1]{ L"Frame to wait" } },
	{ 0x42, L"RESUME_SPEED", L"", L"Resume the appearing rate", 0, 0, NULL },
	{ 0x43, L"CARET_POSITION_REL", L"", L"Temporary change the caret horizontal position in the line", 1, 1, new wstring[1]{ L"Relative position" } },
	{ 0x44, L"CARET_POSITION_ABS", L"", L"Change the caret position", 1, 1, new wstring[1]{ L"Absolute position" } },
	{ 0x45, L"0x45", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x46, L"PROGRESSION_INCREASE", L"Increase the dialog's step progression by 1 for use of the game script", L"Unknown Opcode", 0, 0, NULL },
	{ 0x47, L"0x47", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x48, L"CENTER", L"", L"Center the text", -1, 5, new wstring[1]{ L"Arguments (usually 2: Width/Height)" } },
	{ 0x49, L"CARET_POSITION_Y_ADD", L"", L"Temporary change the caret vertical position", 1, 1, new wstring[1]{ L"Relative position" } },
	{ 0x4A, L"CARET_POSITION_Y_SUB", L"", L"Temporary change the caret vertical position", 1, 1, new wstring[1]{ L"Relative position" } },
	{ 0x4B, L"IMAGE", L"[IMG]", L"Replaced by an image", 1, 1, new wstring[1]{ L"Image ID" } },
	{ 0x4C, L"ICON_ITEM", L"[ICON]", L"Replaced by the icon of an item", 1, 1, new wstring[1]{ L"Item ID" } },
	{ 0x4D, L"RAM_STATUS", L"[ICON]", L"Replaced by a status icon - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x4E, L"RAM_EQUIPMENT", L"[ICON]", L"Replaced by an equipment icon - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x4F, L"FORMAT:Multichoice Masked", L"", L"Setup the number of choices and the 'Cancel' choice of a multichoice with potentially masked choices", 2, TEXT_OPCODE_FORMAT_TYPE + 2, new wstring[2]{ L"Number of choices", L"Cancel choice" } },
	{ 0x50, L"FORMAT:Tail Down Right", L"", L"Display the bubble over the talker, with the tail on the right", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x51, L"FORMAT:Tail Down Left", L"", L"Display the bubble over the talker, with the tail on the left", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x52, L"FORMAT:Tail Up Right", L"", L"Display the bubble under the talker, with the tail on the right", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x53, L"FORMAT:Tail Up Left", L"", L"Display the bubble under the talker, with the tail on the left", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x54, L"FORMAT:Tail Down Center", L"", L"Display the bubble over the talker, with the tail on the center", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x55, L"FORMAT:Tail Up Center", L"", L"Display the bubble under the talker, with the tail on the center", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x56, L"0x56", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x57, L"0x57", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x58, L"FORMAT:Tail Down Right!", L"", L"Force the display the bubble over the talker, with the tail on the right", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x59, L"FORMAT:Tail Down Left!", L"", L"Force the display the bubble over the talker, with the tail on the left", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5A, L"FORMAT:Tail Up Right!", L"", L"Force the display the bubble under the talker, with the tail on the right", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5B, L"FORMAT:Tail Up Left!", L"", L"Force the display the bubble under the talker, with the tail on the left", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5C, L"FORMAT:Tail Default", L"", L"Let the game decide where to display the bubble", 0, TEXT_OPCODE_FORMAT_TYPE, NULL },
	{ 0x5D, L"0x5D", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x5E, L"0x5E", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x5F, L"FORMAT:Offset", L"", L"Add an offset to the position of the bubble from the talker's position", 6, TEXT_OPCODE_FORMAT_TYPE + 3, NULL },
	{ 0x60, L"BUTTONEX_CIRCLE", L"Circle", L"Replaced by the confirm button", 0, 0, NULL },
	{ 0x61, L"BUTTONEX_CROSS", L"Cross", L"Replaced by the cancel button", 0, 0, NULL },
	{ 0x62, L"BUTTONEX_TRIANGLE", L"Triangle", L"Replaced by the menu button", 0, 0, NULL },
	{ 0x63, L"BUTTONEX_SQUARE", L"Square", L"Replaced by the moogle button", 0, 0, NULL },
	{ 0x64, L"BUTTONEX_L1", L"L1", L"Replaced by the button associated to L1", 0, 0, NULL },
	{ 0x65, L"BUTTONEX_R1", L"R1", L"Replaced by the button associated to R1", 0, 0, NULL },
	{ 0x66, L"BUTTONEX_L2", L"L2", L"Replaced by the button associated to L2", 0, 0, NULL },
	{ 0x67, L"BUTTONEX_R2", L"R2", L"Replaced by the button associated to R2", 0, 0, NULL },
	{ 0x68, L"RAM_NUM", L"0", L"Replaced by a number - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x69, L"RAM_TEXT", L"Text", L"Replaced by some text - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6A, L"0x6A", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6B, L"RAM_DATA", L"", L"Replaced by a character's data value - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6C, L"RAM_NUM_DESIGN", L"0", L"Replaced by a designed number - Use RAM setup", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6D, L"0x6D", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6E, L"0x6E", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x6F, L"0x6F", L"", L"Unknown Opcode", 1, 1, new wstring[1]{ L"Unknown" } },
	{ 0x70, L"IMAGE_NEW", L"!New", L"Possibly replaced by a 'New' indicator", 1, 1, new wstring[1]{ L"Script ID" } },
	{ 0x71, L"INDENT", L"", L"Same as CARET_POSITION_ABS(18)", 0, 0, NULL },
	{ 0x72, L"0x72", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x73, L"0x73", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x74, L"0x74", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x75, L"0x75", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x76, L"0x76", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x77, L"0x77", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x78, L"0x78", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x79, L"0x79", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7A, L"0x7A", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7B, L"0x7B", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7C, L"0x7C", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7D, L"0x7D", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7E, L"0x7E", L"", L"Unknown Opcode", 0, 0, NULL },
	{ 0x7F, L"0x7F", L"", L"Unknown Opcode", 0, 0, NULL }
};

vector<SortedChoiceItemTextSteamOpcode> HADES_STRING_TEXT_STEAM_OPCODE = {
	{ L"STRT",	2,	L"Setup the size of the dialog bubble\n - Size X\n - Size Y",	L"" },
	{ L"ID",	0,	L"DialogID (unused)",						L"" },
	{ L"CHOO",	0,	HADES_STRING_TEXT_OPCODE[8].help,			L"" },
	{ L"TIME",	1,	L"Use at the end of a text instead of [ENDN]\nPrevent the player to close the window\nMay also determine the time at which the window automatically closes\n - Signed integer N:\n -- If N>0, the window closes in N frames\n -- If N=-1, the window never closes by itself or by the player",	L"" },
	{ L"FLIM",	0,	HADES_STRING_TEXT_OPCODE[0xA].help,			L"" },
	{ L"NANI",	0,	HADES_STRING_TEXT_OPCODE[0xB].help,			L"" },
	{ L"IMME",	0,	HADES_STRING_TEXT_OPCODE[0xC].help,			L"" },
	{ L"SPED",	1,	L"Slow down the appearing rate\n - Speed factor (1 is the slowest)",			L"" },
	{ L"ZDNE",	0,	HADES_STRING_TEXT_OPCODE[0x10].help,		HADES_STRING_TEXT_OPCODE[0x10].converted },
	{ L"VIVI",	0,	HADES_STRING_TEXT_OPCODE[0x11].help,		HADES_STRING_TEXT_OPCODE[0x11].converted },
	{ L"DGGR",	0,	HADES_STRING_TEXT_OPCODE[0x12].help,		HADES_STRING_TEXT_OPCODE[0x12].converted },
	{ L"STNR",	0,	HADES_STRING_TEXT_OPCODE[0x13].help,		HADES_STRING_TEXT_OPCODE[0x13].converted },
	{ L"FRYA",	0,	HADES_STRING_TEXT_OPCODE[0x14].help,		HADES_STRING_TEXT_OPCODE[0x14].converted },
	{ L"QUIN",	0,	HADES_STRING_TEXT_OPCODE[0x15].help,		HADES_STRING_TEXT_OPCODE[0x15].converted },
	{ L"EIKO",	0,	HADES_STRING_TEXT_OPCODE[0x16].help,		HADES_STRING_TEXT_OPCODE[0x16].converted },
	{ L"AMRT",	0,	HADES_STRING_TEXT_OPCODE[0x17].help,		HADES_STRING_TEXT_OPCODE[0x17].converted },
	{ L"PTY1",	0,	HADES_STRING_TEXT_OPCODE[0x18].help,		HADES_STRING_TEXT_OPCODE[0x18].converted },
	{ L"PTY2",	0,	HADES_STRING_TEXT_OPCODE[0x19].help,		HADES_STRING_TEXT_OPCODE[0x19].converted },
	{ L"PTY3",	0,	HADES_STRING_TEXT_OPCODE[0x1A].help,		HADES_STRING_TEXT_OPCODE[0x1A].converted },
	{ L"PTY4",	0,	HADES_STRING_TEXT_OPCODE[0x1B].help,		HADES_STRING_TEXT_OPCODE[0x1B].converted },
	{ L"HSHD",	0,	L"Make the text have an understated shadow",	L"" },
	{ L"NSHD",	0,	L"Remove the understated shadow of the text",	L"" },
	{ L"DBTN",	1,	L"Replaced by a controller button without taking care of the player's settings (ignored on mobile platforms)\n - Button name among START/SELECT/PAD/UP/DOWN/LEFT/RIGHT/CROSS/SQUARE/CIRCLE/TRIANGLE/L1/L2/R1/R2",			L"[BTN]" },
	{ L"NFOC",	0,	L"Prevent the player to close the window",	L"" },
	{ L"INCS",	0,	HADES_STRING_TEXT_OPCODE[0x46].help,		L"" },
	{ L"CBTN",	1,	L"Replaced by a controller button taking care of the player's custom button settings (ignored on mobile platforms)\n - Button name among START/SELECT/PAD/UP/DOWN/LEFT/RIGHT/CROSS/SQUARE/CIRCLE/TRIANGLE/L1/L2/R1/R2",		L"[BTN]" },
	{ L"PNEW",	0,	HADES_STRING_TEXT_OPCODE[0x70].help,		HADES_STRING_TEXT_OPCODE[0x70].converted },
	{ L"MOVE",	2,	L"Change the caret temporary position in both directions\n - X offset to add\n - Y offset toward top to add",	L"" },
	{ L"ENDN",	0,	L"Must be appended at the end of most texts",	L"" },
	{ L"TEXT",	2,	HADES_STRING_TEXT_OPCODE[0x6].help + L"\n - ID of the tokenized text\n - ID of the variable token",	HADES_STRING_TEXT_OPCODE[0x6].converted },
	{ L"ITEM",	1,	HADES_STRING_TEXT_OPCODE[0xE].help + L"\n - ID of the variable item",		HADES_STRING_TEXT_OPCODE[0xE].converted },
	{ L"SIGL",	1,	HADES_STRING_TEXT_OPCODE[0xF].help + L"\n - Progression step",			L"" },
	{ L"NUMB",	1,	HADES_STRING_TEXT_OPCODE[0x40].help + L"\n - ID of the variable number",	HADES_STRING_TEXT_OPCODE[0x40].converted },
	{ L"WAIT",	1,	HADES_STRING_TEXT_OPCODE[0x41].help + L"\n - Frames to wait",				L"" },
	{ L"FEED",	1,	HADES_STRING_TEXT_OPCODE[0x43].help + L"\n - X offset to add",			L"" },
	{ L"XTAB",	1,	HADES_STRING_TEXT_OPCODE[0x44].help + L"\n - X absolute offset",			L"" },
	{ L"YADD",	1,	HADES_STRING_TEXT_OPCODE[0x49].help + L"\n - Y offset toward top to add",	L"" },
	{ L"YSUB",	1,	HADES_STRING_TEXT_OPCODE[0x4A].help + L"\n - Y offset toward bottom to add",	L"" },
	{ L"ICON",	1,	HADES_STRING_TEXT_OPCODE[0x4B].help + L"\n - Icon ID",					L"[IMG]" },
	{ L"PCHC",	2,	HADES_STRING_TEXT_OPCODE[0x7].help + L"\n - Number of choices\n - 'Cancel' choice",	L"" },
	{ L"PCHM",	2,	HADES_STRING_TEXT_OPCODE[0x4F].help + L"\n - Number of choices\n - 'Cancel' choice",	L"" },
	{ L"MPOS",	2,	HADES_STRING_TEXT_OPCODE[0x3].help + L"\n - X screen position\n - Y screen position",	L"" },
	{ L"OFFT",	3,	HADES_STRING_TEXT_OPCODE[0x5F].help + L"\n - X position\n - Z position\n - Y position",	L"" },
	{ L"TAIL",	1,	L"Set the bubble's tail position, effectively setting the bubble's position in the process\n - A position among: LOR/LOL/LOC/UPR/UPL/UPC/LORF/LOLF/UPRF/UPLF/DEFT",	L"" },
	{ L"TBLE",	-1,	L"Create tokens for an usage in other texts\n - Ignored arguments ; each token is separated by a new line",		L"" },
	{ L"WDTH",	-1,	L"Setup variable bubble width: arguments are -1 terminated lists using the pattern (line number, default width, (opcode1, opcode1_args, ...), ...)\nExample: [WDTH,0,50,16,14,0,-1] means that the line 0, of default size 50, contains the names [ZDNE] and [ITEM=0] which sizes are added\n"\
					L"List of opcodes:\n 6: [TEXT] token\n 14: [ITEM] item name\n From 16 to 27: names of party members\n 64: [NUMB] number\n 112: [PNEW] new icon",	L"" },
	{ L"CENT",	1,	HADES_STRING_TEXT_OPCODE[0x48].help + L"\n - Line width (ignored argument)",	L"" },
	{ L"PAGE",	0,	L"Add a new page of text, displaying after the previous one without redrawing the text window",	L"" },
	{ L"MOBI",	1,	L"Replaced by an icon on mobile platforms only\n - Mobile icon ID",		L"[IMG]" },
	{ L"SPAY",	0,	L"Spacing height (unused)",							L"" },
	{ L"KCBT",	1,	L"Replaced by a keyboard button icon (ignored on mobile platforms)\n - Button name among START/SELECT/PAD/UP/DOWN/LEFT/RIGHT/CROSS/SQUARE/CIRCLE/TRIANGLE/L1/L2/R1/R2",	L"[KEY]" },
	{ L"JCBT",	1,	L"Replaced by a joystick button icon (ignored on mobile platforms)\n - Button name among START/SELECT/PAD/UP/DOWN/LEFT/RIGHT/CROSS/SQUARE/CIRCLE/TRIANGLE/L1/L2/R1/R2",	L"[BTN]" },
	{ L"url",	1,	L"Add an internet URL inside the [url=...][/url] tag\nDoesn't work for most of the texts",	L"" },
	{ L"b",		0,	L"Bold the text inside the [b][/b] tag",			L"" },
	{ L"i",		0,	L"Italicise the text inside the [i][/i] tag",		L"" },
	{ L"u",		0,	L"Underline the text inside the [u][/u] tag",		L"" },
	{ L"s",		0,	L"Strike the text inside the [s][/s] tag",			L"" },
	{ L"c",		0,	L"Ignore the color inside the [c][/c] tag",			L"" },
	{ L"sub",	0,	L"Subscript the text inside the [sub][/sub] tag\nThe display of superscript and subscript is bugged inside the editor",	L"" },
	{ L"sup",	0,	L"Superscript the text inside the [sup][/sup] tag\nThe display of superscript and subscript is bugged inside the editor",	L"" }
};

struct double_map {
    map<int, int> forward;
    map<int, int> backward;

    void insert(int key, int val) {
        forward[key] = val;
        if (backward.find(val) == backward.end())
            backward[val] = key;
    }
};

double_map _dictNull;
double_map _dictShrinesES;
double_map _dictPinnacleRocksUSUKES;
double_map _dictPinnacleRocksJP;
double_map _dictHildaGarde3USUK;
double_map _dictEndingUK;
double_map _dictEndingGR;
double_map _dictPrimaVistaUSUK;
double_map _dictLindblumCastle1USUK;
double_map _dictLindblumCastle1GR;
double_map _dictLindblumCastle1FR;
double_map _dictLindblumCastle1ES;
double_map _dictMistGatesUSUK;
double_map _dictMistGatesJP;
double_map _dictMistGatesGR;
double_map _dictMistGatesFR;
double_map _dictMistGatesIT;
double_map _dictMistGatesES;
double_map _dictLindblum1USUK;
double_map _dictLindblum1FR;
double_map _dictLindblum1ES;
double_map _dictCleyraStormlessUSUK;
double_map _dictMadainSari1USUK;
double_map _dictMadainSari1JPFR;
double_map _dictMadainSari1GR;
double_map _dictMadainSari1IT;
double_map _dictMadainSari1ES;
double_map _dictGarganRooJP;
double_map _dictMadainSari2USUKES;
double_map _dictMadainSari2GR;
double_map _dictMadainSari2FRIT;
double_map _dictFossilRooUSUK;
double_map _dictFossilRooJP;
double_map _dictFossilRooFR;
double_map _dictFossilRooITES;
double_map _dictMognetCentralUSUK;
double_map _dictEvilForestJP;
double_map _dictEvilForestFR;
double_map _dictEvilForestIT;
double_map _dictPrimaVistaRuinJP;
double_map _dictPrimaVistaRuinFR;
double_map _dictCleyraUSUK;
double_map _dictCleyraJPFRES;
double_map _dictCleyraGR;
double_map _dictCleyraIT;
double_map _dictDaliUSUK;
double_map _dictDaliFR;
double_map _dictDaliES;
double_map _dictMountGulugUSUK;
double_map _dictMountGulugJP;
double_map _dictMountGulugGR;
double_map _dictMountGulugFR;
double_map _dictMountGulugIT;
double_map _dictMountGulugES;
double_map _dictLindblumOccupiedUSUK;
double_map _dictLindblumOccupiedJP;
double_map _dictLindblumOccupiedFR;
double_map _dictLindblumOccupiedIT;
double_map _dictLindblumOccupiedES;
double_map _dictDaliUndergroundUSUK;
double_map _dictDaliUndergroundFR;
double_map _dictDaliUndergroundES;
double_map _dictGizamalukeGrottoUSUK;
double_map _dictBranBalFR;
double_map _dictLindblumCastleOccupiedJPGRFRIT;
double_map _dictLindblumCastleOccupiedES;
double_map _dictObservatoryMountainUSUK;
double_map _dictLindblum2USUK;
double_map _dictLindblum2GR;
double_map _dictLindblum2FR;
double_map _dictLindblum2ES;
double_map _dictCargoShipUSUK;
double_map _dictCargoShipGR;
double_map _dictCargoShipFR;
double_map _dictCargoShipIT;
double_map _dictCargoShipES;
double_map _dictTreno1USUK;
double_map _dictTreno2USUK;
double_map _dictQuMarshUSUK;
double_map _dictQuMarshJPGRFR;
double_map _dictQuMarshIT;
double_map _dictQuMarshES;
double_map _dictIifaTreeRootsUSUKGR;
double_map _dictIifaTreeRootsFR;
double_map _dictIifaTreeRootsIT;
double_map _dictIifaTreeRootsES;
double_map _dictSouthGateUSUKES;
double_map _dictSouthGateGR;
double_map _dictSouthGateFR;
double_map _dictBurmeciaUSUK;
double_map _dictBurmeciaJPFRITES;
double_map _dictBurmeciaGR;
double_map _dictIceCavernES;
double_map _dictAlexandria2JPGRFRITES;
double_map _dictIifaTreeUSUKES;
double_map _dictIifaTreeGR;
double_map _dictIifaTreeFR;
double_map _dictIifaTreeIT;
double_map _dictChocoboPlacesUSUK;
double_map _dictChocoboPlacesJP;
double_map _dictChocoboPlacesGR;
double_map _dictChocoboPlacesFR;
double_map _dictChocoboPlacesIT;
double_map _dictChocoboPlacesES;
double_map _dictAlexandriaRuinUSUK;
double_map _dictWorldMapUSUKJPFR;

set<int> _setNull;
map<int, int> _mapNull;

double_map& SetupDictionary(double_map& dict, int max, set<int> missing = _setNull, map<int, int> swap = _mapNull, map<int, int> duplicate = _mapNull) {
    if (dict.forward.size() > 0)
        return dict;
    set<int> indexSkip;
    int indexKey = 0;
    // missings and duplicates are treated the same in HW: they get an index for them amongst the empty (or even non-initialised) texts at the end of the base text block
    for (auto kvp = duplicate.begin(); kvp != duplicate.end(); kvp++)
        missing.insert(kvp->first);
    if (swap.size() > 0) {
        for (auto kvp = swap.begin(); kvp != swap.end(); kvp++) {
            dict.insert(kvp->first, kvp->second);
            indexSkip.insert(kvp->second);
        }
        while (indexSkip.find(indexKey) != indexSkip.end())
            indexKey++;
    }
    for (int indexUniversal = 0; indexUniversal < max; indexUniversal++) {
        if (missing.find(indexUniversal) != missing.end() || swap.find(indexUniversal) != swap.end())
            continue;
        if (indexUniversal != indexKey)
            dict.insert(indexUniversal, indexKey);
        indexKey++;
        while (indexSkip.find(indexKey) != indexSkip.end())
            indexKey++;
    }
    for (auto kvp = missing.begin(); kvp != missing.end(); kvp++) {
        if (*kvp != indexKey)
            dict.insert(*kvp, indexKey);
        indexKey++;
        while (indexSkip.find(indexKey) != indexSkip.end())
            indexKey++;
    }
    return dict;
}

double_map& GetDictionary(SteamLanguage lang, int fieldTextArea) {
    switch (fieldTextArea) {
    case 121:
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictShrinesES, 166, {154});
        break;
    case 134:
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictPinnacleRocksJP, 297, _setNull, _mapNull, { {53, 52} });
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictPinnacleRocksUSUKES, 297, {143, 174, 223, 224, 225, 226, 252, 294, 295}, _mapNull, { {122, 121}, {142, 140} });
        break;
    case 186:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictHildaGarde3USUK, 273, _setNull, { {91, 266}, {95, 267}, {98, 268}, {207, 269}, {208, 270}, {209, 271}, {225, 272} });
        break;
    case 187:
        if (lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictEndingUK, 193, {138, 147, 174});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictEndingGR, 193, {138, 147, 173});
        break;
    case 2:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictPrimaVistaUSUK, 387, {264, 265});
        break;
    case 22:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictLindblumCastle1USUK, 688, _setNull, { {211, 686}, {677, 687} });
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictLindblumCastle1GR, 688, {389, 390});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictLindblumCastle1FR, 688, {201, 202, 483, 484, 485, 486, 505});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictLindblumCastle1ES, 688, {175, 201, 202, 389, 390, 401, 483, 484, 485, 486, 505, 584, 597, 625}, _mapNull, { {353, 343} });
        break;
    case 23:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictMistGatesUSUK, 213, {104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 153, 154, 160, 189, 198, 199, 200, 201, 205});
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictMistGatesJP, 213, _setNull, _mapNull, { {53, 52}, {125, 123} });
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictMistGatesGR, 213, {104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 153, 154, 160, 189, 198, 199, 200, 201, 205, 211}, _mapNull, { {174, 150} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictMistGatesFR, 213, {153, 154, 174, 189, 198, 199, 200, 201, 205, 211}, _mapNull, { {174, 171} });
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictMistGatesIT, 213, {118, 119, 120, 121, 122, 123, 205}, _mapNull, { {125, 205} });
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictMistGatesES, 213, {104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 153, 154, 205, 211});
        break;
    case 276:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictLindblum1USUK, 705, _setNull, { {389, 685}, {371, 686}, {372, 687}, {373, 688}, {374, 689}, {375, 690}, {376, 691}, {377, 692}, {378, 693}, {379, 694}, {380, 695}, {296, 696}, {229, 697}, {230, 698}, {231, 699}, {233, 700}, {234, 701}, {232, 702}, {236, 703}, {240, 704} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictLindblum1FR, 705, {220, 221, 270, 271, 518, 519, 520, 521, 522, 523});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictLindblum1ES, 705, {146, 220, 221, 270, 271, 343, 352, 559, 609});
        break;
    case 290:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictCleyraStormlessUSUK, 402, {92, 93, 94, 95, 96, 97, 98, 99});
        break;
    case 358:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictMadainSari1USUK, 920, {52, 109, 171, 172, 289, 357, 382, 450, 538, 544, 549, 550, 571, 609, 620, 623, 627, 636, 637, 638, 639, 640, 645, 646, 647, 648, 649, 652, 653, 654, 663, 687, 764, 765, 790, 864, 865, 866, 867, 868, 869, 879, 880, 881, 882, 883, 884, 892, 893, 894, 895, 896, 918, 919}, _mapNull, { {502, 493}, {671, 641} });
        if (lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictMadainSari1JPFR, 920, {192, 570, 571, 573, 574, 602, 607, 609, 620, 627, 636, 637, 638, 639, 640, 645, 646, 647, 648, 649, 652, 653, 654, 864, 865, 866, 867, 868, 869, 879, 880, 881, 882, 883, 884, 892, 893, 894, 895, 896, 918, 919}, { {599, 599}, {600, 600}, {601, 594}, {603, 595}, {604, 596}, {605, 597}, {606, 598} }, { {671, 650} });
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictMadainSari1GR, 920, {171, 172, 663, 892, 893, 894, 895, 896}, { {699, 878}, {700, 879}, {701, 880}, {561, 881}, {188, 882}, {189, 883}, {194, 884}, {806, 885}, {870, 886}, {871, 887}, {872, 888}, {873, 889}, {874, 890}, {875, 891}, {876, 892}, {885, 893}, {886, 894}, {887, 895}, {888, 896}, {889, 897}, {890, 898}, {891, 899}, {198, 900}, {199, 901}, {562, 902}, {563, 903}, {918, 904}, {641, 905}, {642, 906}, {650, 907}, {655, 908}, {656, 909}, {919, 910}, {657, 911} });
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictMadainSari1IT, 920, {573, 607, 609, 620, 627, 636, 637, 638, 639, 645, 648, 649, 653, 864, 865, 866, 867, 868, 869, 879, 880, 881, 882, 883, 884, 918, 919}, { {599, 604}, {600, 605}, {601, 598}, {602, 599}, {603, 600}, {604, 601}, {605, 602}, {606, 603}, {640, 633}, {646, 640}, {647, 641}, {650, 636}, {651, 637}, {652, 642}, {654, 643}, {655, 638}, {656, 639} }, { {671, 660} });
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictMadainSari1ES, 920, {52, 109, 171, 172, 192, 357, 382, 450, 512, 538, 544, 570, 571, 573, 574, 602, 609, 620, 627, 636, 637, 638, 639, 640, 645, 646, 647, 648, 649, 652, 653, 654, 663, 687, 764, 765, 790, 864, 865, 866, 867, 868, 869, 879, 880, 881, 882, 883, 884, 892, 893, 894, 895, 896, 918, 919}, { {599, 588}, {600, 589}, {601, 583}, {603, 584}, {604, 585}, {605, 586}, {606, 587} }, { {502, 493}, {671, 639} });
        break;
    case 359:
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictGarganRooJP, 205, _setNull, _mapNull, { {52, 51} });
        break;
    case 360:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictMadainSari2USUKES, 127, {76, 77, 78, 98});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictMadainSari2GR, 127, {77, 78});
        if (lang == STEAM_LANGUAGE_FR || lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictMadainSari2FRIT, 127, {76, 77, 78});
        break;
    case 361:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictFossilRooUSUK, 234, {223});
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictFossilRooJP, 234, _setNull, _mapNull, { {67, 66} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictFossilRooFR, 234, {178, 179, 180, 223});
        if (lang == STEAM_LANGUAGE_IT || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictFossilRooITES, 234, {178, 179, 180, 212, 223});
        break;
    case 38:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictMognetCentralUSUK, 96, {85}, { {84, 94} });
        break;
    case 4:
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictEvilForestJP, 256, _setNull, _mapNull, { {52, 51} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictEvilForestFR, 256, {155, 156, 157, 158, 159});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictEvilForestIT, 256, {107, 131, 133, 155, 156, 157, 158, 159, 183, 185});
        break;
    case 40:
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictPrimaVistaRuinJP, 412, _setNull, _mapNull, { {52, 51} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictPrimaVistaRuinFR, 412, {131, 245, 250, 257, 303, 304, 305, 306, 307, 308, 309, 320, 321, 344, 345, 346, 347, 355, 380, 381, 382, 383, 384, 385, 386, 387});
        break;
    case 44:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictCleyraUSUK, 302, {92, 93, 94, 95, 96, 97, 98, 99, 269}, { {271, 263}, {273, 262} }, { {272, 262} });
        if (lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_FR || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictCleyraJPFRES, 302, {269}, _mapNull, { {272, 271} });
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictCleyraGR, 302, _setNull, { {271, 273}, {273, 271} });
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictCleyraIT, 302, {269}, { {271, 271}, {273, 270} }, { {272, 270} });
        break;
    case 47:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictDaliUSUK, 612, {192, 207, 212, 303, 320, 360, 430, 481, 482, 483, 485, 486}, { {143, 582}, {146, 583}, {147, 584}, {148, 585}, {409, 586}, {410, 587}, {411, 588}, {412, 589}, {413, 590}, {142, 591}, {144, 592}, {523, 593}, {141, 594}, {258, 595}, {145, 596}, {609, 597}, {610, 598}, {611, 599} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictDaliFR, 612, {485, 486, 487});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictDaliES, 612, {192, 207, 212, 303, 320, 360, 430, 481, 482, 483, 485, 486, 487}, _mapNull, { {297, 296} });
        break;
    case 484:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictMountGulugUSUK, 253, {141, 142, 143, 144, 145, 146, 184, 187, 203, 204, 205, 240, 241}, { {88, 235}, {89, 236}, {90, 237}, {91, 238}, {92, 239} });
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictMountGulugJP, 253, {252});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictMountGulugGR, 253, {240, 252});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictMountGulugFR, 253, {141, 142, 143, 144, 145, 184, 187, 203, 204, 205, 240});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictMountGulugIT, 253, {141, 142, 143, 144, 145, 184, 187, 203, 204, 205, 240, 241, 252});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictMountGulugES, 253, {141, 142, 143, 144, 145, 184, 187, 203, 204, 205, 240, 241, 242, 252});
        break;
    case 485:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictLindblumOccupiedUSUK, 393, {258});
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictLindblumOccupiedJP, 393, {257, 258});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictLindblumOccupiedFR, 393, {152, 257, 258, 259, 260, 261, 266, 267, 268, 269, 270, 271, 272, 273});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictLindblumOccupiedIT, 393, {257, 258, 259});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictLindblumOccupiedES, 393, {152, 247, 250, 257, 258, 266, 267, 268, 269, 270, 271, 272, 273, 297, 381});
        break;
    case 50:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictDaliUndergroundUSUK, 218, {79, 80, 81, 82, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 182});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictDaliUndergroundFR, 218, {123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 182});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictDaliUndergroundES, 218, {80, 81, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137});
        break;
    case 51:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictGizamalukeGrottoUSUK, 205, _setNull, { {178, 201}, {179, 202}, {180, 203}, {181, 204} });
        break;
    case 52:
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictBranBalFR, 422, {156, 158});
        break;
    case 525:
        if (lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_GE || lang == STEAM_LANGUAGE_FR || lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictLindblumCastleOccupiedJPGRFRIT, 238, {202, 203, 204});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictLindblumCastleOccupiedES, 238, {129, 140, 141, 142, 202, 203, 204, 210, 219, 220});
        break;
    case 53:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictObservatoryMountainUSUK, 340, {226, 324});
        break;
    case 595:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictLindblum2USUK, 502, {161, 162, 171, 172, 178, 179, 180, 181, 182, 187, 188, 189, 190, 191, 192, 193});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictLindblum2GR, 502, {161, 162, 172});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictLindblum2FR, 502, {159, 160, 161, 162, 175, 176, 177, 178, 179, 180, 181, 182, 187, 188, 189, 190, 191, 192, 193, 199, 200, 205}, _mapNull, { {197, 177} });
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictLindblum2ES, 502, {130, 159, 160, 161, 162, 171, 172, 175, 176, 177, 178, 179, 180, 181, 182, 187, 188, 189, 190, 191, 192, 193, 195, 205}, _mapNull, { {197, 173} });
        break;
    case 63:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictCargoShipUSUK, 327, {76, 77, 78, 79, 80, 131, 134, 137, 152, 153, 154, 155, 165, 166, 167, 168, 170, 189, 190, 191, 203, 204, 205, 206, 207, 217, 218, 259}, _mapNull, { {110, 104} });
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictCargoShipGR, 327, {76, 77, 78, 79, 80, 87, 88, 131, 134, 137, 152, 153, 154, 155, 156, 165, 166, 167, 168, 170, 189, 190, 191, 207, 217, 218, 259}, _mapNull, { {110, 102} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictCargoShipFR, 327, {76, 77, 78, 79, 80, 131, 134, 137, 167, 168, 170, 189, 190, 191, 217, 218, 259});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictCargoShipIT, 327, {76, 77, 78, 79, 80, 165, 166, 167, 168, 170, 189, 190, 191, 207, 259});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictCargoShipES, 327, {77, 78, 165, 166, 167, 168, 170, 189, 190, 191, 207, 259});
        break;
    case 70:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictTreno1USUK, 549, _setNull, { {139, 248} });
        break;
    case 741:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictTreno2USUK, 674, _setNull, { {139, 248} });
        break;
    case 71:
    case 8263:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictQuMarshUSUK, 394, {365, 374, 390, 391}, { {212, 258} });
        if (lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_GE || lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictQuMarshJPGRFR, 394, {258});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictQuMarshIT, 394, {258, 365, 390, 391});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictQuMarshES, 394, {258, 365, 374, 390, 391});
        break;
    case 738:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN || lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictIifaTreeRootsUSUKGR, 359, {112, 135, 136, 138, 195, 210, 211, 223, 224, 225, 226, 227, 228, 229, 230, 231, 245, 265, 271, 279, 282, 336, 346, 347, 348});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictIifaTreeRootsFR, 359, {112, 138, 195, 210, 211, 223, 224, 225, 226, 227, 228, 229, 230, 231, 245, 279, 282});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictIifaTreeRootsIT, 359, {112, 135, 136, 138, 195, 211, 223, 224, 225, 226, 227, 228, 229, 230, 231, 245, 265, 271, 279, 282, 336, 346, 347, 348});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictIifaTreeRootsES, 359, {112, 135, 136, 138, 195, 210, 211, 224, 225, 226, 227, 228, 229, 230, 231, 245, 265, 271, 279, 282, 336, 346, 347, 348});
        break;
    case 74:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictSouthGateUSUKES, 552, {309, 373, 426, 480, 509, 511, 512, 513, 514, 537, 538}, _mapNull, { {498, 493} });
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictSouthGateGR, 552, {373, 480, 509, 511, 512, 513, 514}, _mapNull, { {498, 495} });
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictSouthGateFR, 552, {509, 511, 512, 513, 514, 537, 538});
        break;
    case 77:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictBurmeciaUSUK, 339, {295});
        if (lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_FR || lang == STEAM_LANGUAGE_IT || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictBurmeciaJPFRITES, 339, {295, 337, 338});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictBurmeciaGR, 339, {337, 338}, { {134, 335}, {213, 336} });
        break;
    case 8:
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictIceCavernES, 211, {97});
        break;
    case 90:
        if (lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_GE || lang == STEAM_LANGUAGE_FR || lang == STEAM_LANGUAGE_IT || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictAlexandria2JPGRFRITES, 557, {262});
        break;
    case 944:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN || lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictIifaTreeUSUKES, 310, {83, 84, 85, 90, 122, 123, 175, 204, 218, 220, 258, 259, 260, 278, 279, 280, 281, 282, 299, 306});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictIifaTreeGR, 310, {83, 84, 122, 123, 175, 204, 218, 220, 258, 259, 260, 278, 279, 280, 281, 282, 299, 306});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictIifaTreeFR, 310, {83, 84, 85, 90, 122, 123, 175, 204, 218, 220, 259, 260, 278, 279, 280, 281, 299, 306});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictIifaTreeIT, 310, {83, 84, 85, 90, 122, 123, 204, 218, 220, 258, 259, 260, 278, 279, 280, 281, 282, 299, 306});
        break;
    case 945:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictChocoboPlacesUSUK, 467, {183, 257, 280, 285, 289, 293, 297, 363, 432});
        if (lang == STEAM_LANGUAGE_JA)
            return SetupDictionary(_dictChocoboPlacesJP, 467, {89, 183});
        if (lang == STEAM_LANGUAGE_GE)
            return SetupDictionary(_dictChocoboPlacesGR, 467, {89, 432});
        if (lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictChocoboPlacesFR, 467, {183});
        if (lang == STEAM_LANGUAGE_IT)
            return SetupDictionary(_dictChocoboPlacesIT, 467, {89});
        if (lang == STEAM_LANGUAGE_SP)
            return SetupDictionary(_dictChocoboPlacesES, 467, {183}, { {89, 48} });
        break;
    case 946:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN)
            return SetupDictionary(_dictAlexandriaRuinUSUK, 277, _setNull, { {245, 270}, {246, 271}, {247, 272}, {248, 273}, {249, 274}, {256, 275}, {255, 276} });
        break;
    case TEXT_BLOCK_WORLD_MAP:
        if (lang == STEAM_LANGUAGE_US || lang == STEAM_LANGUAGE_EN || lang == STEAM_LANGUAGE_JA || lang == STEAM_LANGUAGE_FR)
            return SetupDictionary(_dictWorldMapUSUKJPFR, 92, {88, 89});
        break;
    }
    return _dictNull;
}

map<int, int>& GetUniversalTextMap(SteamLanguage lang, int fieldTextArea) {
    return GetDictionary(lang, fieldTextArea).forward;
}

int GetUniversalTextId(SteamLanguage lang, int fieldTextArea, int textId) {
	double_map& dict = GetDictionary(lang, fieldTextArea);
	auto universalId = dict.backward.find(textId);
	if (universalId != dict.backward.end())
		return universalId->second;
	return textId;
}

int GetTextIdFromUniversalId(SteamLanguage lang, int fieldTextArea, int universalId) {
    double_map& dict = GetDictionary(lang, fieldTextArea);
	auto textId = dict.forward.find(universalId);
	if (textId != dict.forward.end())
		return textId->second;
	return universalId;
}
