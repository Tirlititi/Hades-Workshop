#ifndef _SCRIPT_STRINGS_H
#define _SCRIPT_STRINGS_H

// ToDo: implement "command_tags.cs" infos

#define G_N_ELEMENTS(arr) ((sizeof(arr))/(sizeof(arr[0])))

//====================================//
//             Game Script            //
//====================================//

struct VariableOperation {
	// Operation types :
	// -1 : terminate
	// 0 : unary operator pre-fixed
	// 1 : unary operator post-fixed
	// 2 : binary operator
	// 3 : array operator
	// 5 : get constant char
	// 6 : get constant short
	// 7 : get constant long
	// 10 : get variable (unsigned char) (array byte : 1)
	// 11 : get variable (unsigned short) (array byte : 1)
	// 12 : get variable (unsigned long) (array byte : 1)
	// 13 : get variable (bit) (array byte : 1)
	// 19 : get variable (not tested) (array byte : 1)
	// 20 : get variable (unsigned char) (array byte : 2)
	// 21 : get variable (unsigned short) (array byte : 2)
	// 22 : get variable (unsigned long) (array byte : 2)
	// 23 : get variable (bit) (array byte : 2)
	// 29 : get variable (not tested) (array byte : 2)
	// 50 : function (1 arg)
	// 51 : function (2 args)
	// 100 : unknown/unused
	int type;
	wstring opstring;
	wstring description;
};

static VariableOperation VarOpList[] = {
	{ 50, L"Op0", L"" },	{ 50, L"Op1", L"" },	{ 50, L"Op2", L"" },	{ 50, L"Op3", L"" },
	{ 1, L"++", L"" },		{ 1, L"--", L"" },
	{ 0, L"++", L"Increase the variable by 1.\n Prefixed, it returns the value of the variable after the increment.\n Postfixed, it returns the value of the variable before the increment." },
	{ 0, L"--", L"Decrease the variable by 1.\n Prefixed, it returns the value of the variable after the decrement.\n Postfixed, it returns the value of the variable before the decrement." },
	{ 1, L"++$", L"" },		{ 1, L"--$", L"" },
	{ 0, L"++$", L"Increase each value of a list by 1.\n Prefixed, it returns the value of the last element of the list after the increment.\n Postfixed, it returns the value of the last element of the list before the increment." },
	{ 0, L"--$", L"Decrease each value of a list by 1.\n Prefixed, it returns the value of the last element of the list after the decrement.\n Postfixed, it returns the value of the last element of the list before the decrement." },
	{ 50, L"OpC", L"Single plus." },
	{ 50, L"OpD", L"Single minus." },
	{ 0, L"!", L"Boolean NOT operator." },
	{ 0, L"!$", L"Boolean list NOT operator." },
	
	{ 0, L"~", L"Bitwise NOT operator." },
	{ 2, L"*", L"Arithmetical multiplication." },
	{ 2, L"/", L"Arithmetical division." },
	{ 2, L"%", L"Arithmetical modulus." },
	{ 2, L"+", L"Arithmetical addition." },
	{ 2, L"-", L"Arithmetical subtraction." },
	{ 2, L"<<", L"Bitwise LEFT SHIFT operator." },
	{ 2, L">>", L"Bitwise RIGHT SHIFT operator." },
	{ 2, L"<", L"Numerical STRICTLY LESS THAN check." },
	{ 2, L">", L"Numerical STRICTLY GREATER THAN check." },
	{ 2, L"<=", L"Numerical LESS THAN check." },
	{ 2, L">=", L"Numerical GREATER THAN check." },
	{ 2, L"<$", L"List STRICTLY LESS THAN check." },
	{ 2, L">$", L"List STRICTLY GREATER THAN check." },
	{ 2, L"<=$", L"List LESS THAN check." },
	{ 2, L">=$", L"List GREATER THAN check." },
	
	{ 2, L"==", L"Numerical EQUAL check." },
	{ 2, L"!=", L"Numerical NOT EQUAL check." },
	{ 2, L"==$", L"List EQUAL check." },
	{ 2, L"!=$", L"List NOT EQUAL check." },
	{ 2, L"&", L"Bitwise AND operator." },
	{ 2, L"^", L"Bitwise XOR operator." },
	{ 2, L"|", L"Bitwise OR operator." },
	{ 2, L"&&", L"Boolean AND operator." },
	{ 2, L"||", L"Boolean OR operator." },
	{ 3, L"", L"Get the battle data of one or more characters in battle.\nIt returns a list with as many values as characters.\nFor instance, SV_PlayerTeam[HP] returns a list of all the HP of the player's team.\nUse the list operators $ to manipulate the list returned.\n\nKnown battle data fields :\n" },
	{ 0, L"#", L"Bitwise COUNT operator." },
	{ 50, L"FirstOf", L"Convert a list into the first of its values." },
	{ 2, L"=", L"Numerical variable assignment." },
	{ 2, L"=$", L"List variable assignment." },
	{ 2, L"!!=$", L"Boolean list variable NOT ZERO assignment." },
	{ 2, L"*=", L"Numerical variable multiplication assignment." },
	
	{ 2, L"/=", L"Numerical variable division assignment." },
	{ 2, L"%=", L"Numerical variable modulus assignment." },
	{ 2, L"+=", L"Numerical variable addition assignment." },
	{ 2, L"-=", L"Numerical variable subtraction assignment." },
	{ 2, L"<<=", L"Bitwise variable LEFT SHIFT assignment." },
	{ 2, L">>=", L"Bitwise variable RIGHT SHIFT assignment." },
	{ 2, L"*=$", L"Numerical list variable multiplication assignment." },
	{ 2, L"/=$", L"Numerical list variable division assignment." },
	{ 2, L"%=$", L"Numerical list variable modulus assignment." },
	{ 2, L"+=$", L"Numerical list variable addition assignment." },
	{ 2, L"-=$", L"Numerical list variable subtraction assignment." },
	{ 2, L"<<=$", L"Bitwise list variable LEFT SHIFT assignment." },
	{ 2, L">>=$", L"Bitwise list variable RIGHT SHIFT assignment." },
	{ 2, L"&=", L"Bitwise variable AND assignment." },
	{ 2, L"^=", L"Bitwise variable XOR assignment." },
	{ 2, L"|=", L"Bitwise variable OR assignment." },
	
	{ 2, L"&=$", L"Bitwise list variable AND assignment." },
	{ 2, L"^=$", L"Bitwise list variable XOR assignment." },
	{ 2, L"|=$", L"Bitwise list variable OR assignment." },
	{ 2, L"Op43", L"Boolean list variable AND assignment." },
	{ 2, L"Op44", L"Boolean list variable XOR assignment." },
	{ 2, L"Op45", L"Boolean list variable OR assignment." },
	{ 50, L"Char", L"Cast to an 8-bits value." },
	{ 50, L"UChar", L"Cast to an 8-bits unsigned value." },
	{ 50, L"Short", L"Cast to a 16-bits value." },
	{ 50, L"UShort", L"Cast to a 16-bits unsigned value." },
	{ 50, L"List", L"Cast to a list value." },
	{ 50, L"IndexMax", L"Return the bitmask corresponding to the index of the maximum value of a list." },
	{ 50, L"IndexMin", L"Return the bitmask corresponding to the index of the minimum value of a list." },
	{ 50, L"RandomInTeam", L"Return a random value from a bit list." },
	{ 2, L"Op4E", L"ObjSpec." },
	{ 50, L"IsButton", L"Check if a button of the controller was pressed right before the current frame." },
	
	{ 50, L"Op50", L"A sinus operator." },
	{ 50, L"Op51", L"A cosinus operator." },
	{ 50, L"GetHP", L"Return the HP of the character." },
	{ 50, L"GetMaxHP", L"Return the MAX HP of the character." },
	{ 51, L"Matching", L"Return the list of indexes matching at least one bit of a bitflag condition.\n\n 1st argument : list of bitflags.\n 2nd argument : bitflag condition." },
	{ 51, L"NotMatching", L"Return the list of indexes not matching any bit of a bitflag condition.\n\n 1st argument : list of bitflags.\n 2nd argument : bitflag condition." },
	{ 51, L"NotMatchingExact", L"Return the list of indexes that doesn't fit exactly the bitflag condition.\n\n 1st argument : list of bitflags.\n 2nd argument : bitflag condition." },
	{ 51, L"ListOR", L"Return the list of indexes that has at least one of the two values being non-zero.\n\n 1st and 2nd arguments : lists of values." },
	{ 50, L"IsUnbutton", L"Check if a button of the controller was released right before the current frame." },
	{ 50, L"IsButtonDown", L"Check if a button of the controller is currently pressed." },
	{ 50, L"Op5A", L"A button pressing check." },
	{ 50, L"Op5B", L"A button releasing check." },
	{ 50, L"Op5C", L"A button holding check." },
	{ 51, L"Angle", L"Calculate the angle between the entry's object position and the target position, in the XY plane.\n\n 1st argument : target position X.\n 2nd argument : target position Y." },
	{ 51, L"Distance", L"Calculate the distance between the entry's object position and the target position, in the XY plane.\n\n 1st argument : target position X.\n 2nd argument : target position Y." },
	{ 19, L"VAR_5F_", L"" },
	
	{ 50, L"Op60", L"An angle calculation." },
	{ 50, L"Op61", L"A distance calculation." },
	{ 50, L"Sin", L"Calculate the sinus of an angle and multiply it by 4096 to get an integer." },
	{ 50, L"Cos", L"Calculate the cosinus of an angle and multiply it by 4096 to get an integer." },
	{ 50, L"GetItemCount", L"Return the amount of items of an item type in the player's inventory." },
	{ 50, L"GetTileAnimFrame", L"Return the current frame of the specified tile animation." },
	{ 51, L"Op66", L"An angle calculation." },
	{ 100, L"Op67", L"" },	{ 100, L"Op68", L"" },	{ 100, L"Op69", L"" },
	{ 50, L"GetAnimDuration", L"Return the duration of the entry's object specified animation." },
	{ 50, L"IsInParty", L"Check if a character is in the player's party." },
	{ 100, L"Op6C", L"Sps information." },
	{ 50, L"AddParty", L"Add a character to the player's party." },
	{ 50, L"GetMP", L"Return the MP of the character." },
	{ 50, L"GetMaxMP", L"Return the MAX MP of the character." },
	
	{ 50, L"Op70", L"Maybe return the walkmesh triangle on which the specified entry's object is standing." },
	{ 50, L"Op71", L"Maybe return the walkpath on which the specified entry's object is standing." },
	{ 100, L"Op72", L"" },	{ 100, L"Op73", L"" },	{ 100, L"Op74", L"" },		{ 100, L"Op75", L"" },	{ 100, L"Op76", L"" },	{ 100, L"Op77", L"" },
	{ 29, L"MV_", L"" },	{ 19, L"SV_", L"" },	{ 19, L"GetData_", L"" },	{ 100, L"Op7B", L"" },	{ 100, L"Op7C", L"" },	{ 6, L"", L"" },	{ 7, L"", L"" },	{ -1, L"", L"" },
	
	{ 100, L"Op80", L"" },	{ 100, L"Op81", L"" },	{ 100, L"Op82", L"" },	{ 100, L"Op83", L"" },	{ 100, L"Op84", L"" },	{ 100, L"Op85", L"" },	{ 100, L"Op86", L"" },	{ 100, L"Op87", L"" },
	{ 100, L"Op88", L"" },	{ 100, L"Op89", L"" },	{ 100, L"Op8A", L"" },	{ 100, L"Op8B", L"" },	{ 100, L"Op8C", L"" },	{ 100, L"Op8D", L"" },	{ 100, L"Op8E", L"" },	{ 100, L"Op8F", L"" },
	
	{ 100, L"Op90", L"" },	{ 100, L"Op91", L"" },	{ 100, L"Op92", L"" },	{ 100, L"Op93", L"" },	{ 100, L"Op94", L"" },	{ 100, L"Op95", L"" },	{ 100, L"Op96", L"" },	{ 100, L"Op97", L"" },
	{ 100, L"Op98", L"" },	{ 100, L"Op99", L"" },	{ 100, L"Op9A", L"" },	{ 100, L"Op9B", L"" },	{ 100, L"Op9C", L"" },	{ 100, L"Op9D", L"" },	{ 100, L"Op9E", L"" },	{ 100, L"Op9F", L"" },
	
	{ 100, L"OpA0", L"" },	{ 100, L"OpA1", L"" },	{ 100, L"OpA2", L"" },	{ 100, L"OpA3", L"" },	{ 100, L"OpA4", L"" },	{ 100, L"OpA5", L"" },	{ 100, L"OpA6", L"" },	{ 100, L"OpA7", L"" },
	{ 100, L"OpA8", L"" },	{ 100, L"OpA9", L"" },	{ 100, L"OpAA", L"" },	{ 100, L"OpAB", L"" },	{ 100, L"OpAC", L"" },	{ 100, L"OpAD", L"" },	{ 100, L"OpAE", L"" },	{ 100, L"OpAF", L"" },
	
	{ 100, L"OpB0", L"" },	{ 100, L"OpB1", L"" },	{ 100, L"OpB2", L"" },	{ 100, L"OpB3", L"" },	{ 100, L"OpB4", L"" },	{ 100, L"OpB5", L"" },	{ 100, L"OpB6", L"" },	{ 100, L"OpB7", L"" },
	{ 100, L"OpB8", L"" },	{ 100, L"OpB9", L"" },	{ 100, L"OpBA", L"" },	{ 100, L"OpBB", L"" },	{ 100, L"OpBC", L"" },	{ 100, L"OpBD", L"" },	{ 100, L"OpBE", L"" },	{ 100, L"OpBF", L"" },
	
	{ 10, L"VAR_A1_", L"" },	{ 19, L"VAR_A2_", L"" },	{ 19, L"VAR_A3_", L"" },	{ 19, L"VAR_A4_", L"" },	{ 19, L"VAR_A5_", L"" },	{ 19, L"VAR_A6_", L"" },	{ 19, L"VAR_A7_", L"" },	{ 19, L"VAR_A8_", L"" },
	{ 19, L"VAR_A9_", L"" },	{ 19, L"VAR_A10_", L"" },	{ 12, L"VAR_A11_", L"" },	{ 19, L"VAR_A12_", L"" },	{ 19, L"VAR_A13_", L"" },	{ 19, L"VAR_A14_", L"" },	{ 19, L"VAR_A15_", L"" },	{ 19, L"VAR_A16_", L"" },
	
	{ 10, L"VAR_B1_", L"" },	{ 19, L"VAR_B2_", L"" },	{ 19, L"VAR_B3_", L"" },	{ 19, L"VAR_B4_", L"" },	{ 19, L"VAR_B5_", L"" },	{ 19, L"VAR_B6_", L"" },	{ 19, L"VAR_B7_", L"" },	{ 19, L"VAR_B8_", L"" },
	{ 19, L"VAR_B9_", L"" },	{ 19, L"VAR_B10_", L"" },	{ 19, L"VAR_B11_", L"" },	{ 19, L"VAR_B12_", L"" },	{ 19, L"VAR_B13_", L"" },	{ 19, L"VAR_B14_", L"" },	{ 19, L"VAR_B15_", L"" },	{ 19, L"VAR_B16_", L"" },
	
	{ 29, L"VAR_C1_", L"" },	{ 29, L"VAR_C2_", L"" },	{ 29, L"VAR_C3_", L"" },	{ 29, L"VAR_C4_", L"" },	{ 24, L"VAR_C5_", L"" },	{ 29, L"VAR_C6_", L"" },	{ 29, L"VAR_C7_", L"" },	{ 29, L"VAR_C8_", L"" },
	{ 29, L"VAR_C9_", L"" },	{ 29, L"VAR_C10_", L"" },	{ 29, L"VAR_C11_", L"" },	{ 29, L"VAR_C12_", L"" },	{ 29, L"VAR_C13_", L"" },	{ 29, L"VAR_C14_", L"" },	{ 29, L"VAR_C15_", L"" },	{ 29, L"VAR_C16_", L"" },
	
	{ 29, L"VAR_D1_", L"" },	{ 29, L"VAR_D2_", L"" },	{ 29, L"VAR_D3_", L"" },	{ 29, L"VAR_D4_", L"" },	{ 20, L"VAR_D5_", L"" },	{ 29, L"VAR_D6_", L"" },	{ 29, L"VAR_D7_", L"" },	{ 29, L"VAR_D8_", L"" },
	{ 29, L"VAR_D9_", L"" },	{ 29, L"VAR_D10_", L"" },	{ 29, L"VAR_D11_", L"" },	{ 29, L"VAR_D12_", L"" },	{ 29, L"VAR_D13_", L"" },	{ 29, L"VAR_D14_", L"" },	{ 29, L"VAR_D15_", L"" },	{ 29, L"VAR_D16_", L"" }
};

#define ARRAY_ADDITIONAL_INFO	L"\n\nAdditional informations :\n"\
								L"[PREVENT_ATTACK]\n"\
								L"If set to 1, it cancels all the character's awaiting attacks (selected using an Attack call or selected by the player). It doesn't empty the ATB.\n\n"\
								L"[MAX_HP/MP]\n"\
								L"The modifications of Max HP/MP are not reversed at the end of the battle for player's character. The modifications of the other stats (strength, magic...) are reversed.\n\n"\
								L"[ROW/COLUMN]\n"\
								L"The row is 0/1 for front/back row while the column is an integer from 0 to 3, sorting the characters from right to left.\n\n"\
								L"[MODEL_TYPE]\n"\
								L"Zidane : 0 or 1\n"\
								L"Vivi : 2\n"\
								L"Dagger : 3, 4, 5 or 6\n"\
								L"Steiner : 7 or 8\n"\
								L"Quina : 9\n"\
								L"Eiko : 10 or 11\n"\
								L"Freya : 12\n"\
								L"Amarant : 13\n"\
								L"Cinna : 14\n"\
								L"Marcus : 15\n"\
								L"Blank : 16 or 17\n"\
								L"Beatrix : 18\n\n"\
								L"[CATEGORY]\n"\
								L"1  : male\n"\
								L"2  : female\n"\
								L"4  : gaian\n"\
								L"8  : terran\n"\
								L"16 : temporary character\n\n"\
								L"[STAND_ANIMATION]\n"\
								L"This can be modified by the attack animation sequences.\n\n"\
								L"[MARTHYM_ON]\n"\
								L"Fill the ATB bar and order the character to cast Marthym. The set value is the target."

struct VariableName {
	uint8_t cat;
	uint16_t id;
	wstring name;
	wstring description;
};

// AttackType :
// -- Physical attack : 1,3,6,8,9,10,11,15,25,26,27,28,29,30,31,32,33,52
// -- Magic attack : 16,17,18,19,20,21,22,23,24,34

static VariableName VarNameList[] = {
	{ 0x78, 1,		L"MV_WorldPositionX1",		L"X position of the 1st entry's object." },
	{ 0x78, 2,		L"MV_WorldPositionX2",		L"X position of the 2nd entry's object." },
	{ 0x78, 3,		L"MV_WorldPositionX3",		L"X position of the 3rd entry's object." },
	{ 0x78, 4,		L"MV_WorldPositionX4",		L"X position of the 4th entry's object." },
	{ 0x78, 5,		L"MV_WorldPositionX5",		L"X position of the 5th entry's object." },
	{ 0x78, 6,		L"MV_WorldPositionX6",		L"X position of the 6th entry's object." },
	{ 0x78, 7,		L"MV_WorldPositionX7",		L"X position of the 7th entry's object." },
	{ 0x78, 8,		L"MV_WorldPositionX8",		L"X position of the 8th entry's object." },
	{ 0x78, 9,		L"MV_WorldPositionX9",		L"X position of the 9th entry's object." },
	{ 0x78, 10,		L"MV_WorldPositionX10",		L"X position of the 10th entry's object." },
	{ 0x78, 11,		L"MV_WorldPositionX11",		L"X position of the 11th entry's object." },
	{ 0x78, 12,		L"MV_WorldPositionX12",		L"X position of the 12th entry's object." },
	{ 0x78, 250,	L"MV_WorldPositionXPlayer",	L"X position of the player's character." },
	{ 0x78, 251,	L"MV_WorldPositionXParty1",	L"X position of the player's 1st party member." },
	{ 0x78, 252,	L"MV_WorldPositionXParty2",	L"X position of the player's 2nd party member." },
	{ 0x78, 253,	L"MV_WorldPositionXParty3",	L"X position of the player's 3rd party member." },
	{ 0x78, 254,	L"MV_WorldPositionXParty4",	L"X position of the player's 4th party member." },
	{ 0x78, 255,	L"MV_WorldPositionX",		L"X position of the entry's object." },
	{ 0x78, 257,	L"MV_WorldPositionZ1",		L"Z position of the 1st entry's object." },
	{ 0x78, 258,	L"MV_WorldPositionZ2",		L"Z position of the 2nd entry's object." },
	{ 0x78, 259,	L"MV_WorldPositionZ3",		L"Z position of the 3rd entry's object." },
	{ 0x78, 260,	L"MV_WorldPositionZ4",		L"Z position of the 4th entry's object." },
	{ 0x78, 261,	L"MV_WorldPositionZ5",		L"Z position of the 5th entry's object." },
	{ 0x78, 262,	L"MV_WorldPositionZ6",		L"Z position of the 6th entry's object." },
	{ 0x78, 263,	L"MV_WorldPositionZ7",		L"Z position of the 7th entry's object." },
	{ 0x78, 264,	L"MV_WorldPositionZ8",		L"Z position of the 8th entry's object." },
	{ 0x78, 265,	L"MV_WorldPositionZ9",		L"Z position of the 9th entry's object." },
	{ 0x78, 266,	L"MV_WorldPositionZ10",		L"Z position of the 10th entry's object." },
	{ 0x78, 267,	L"MV_WorldPositionZ11",		L"Z position of the 11th entry's object." },
	{ 0x78, 268,	L"MV_WorldPositionZ12",		L"Z position of the 12th entry's object." },
	{ 0x78, 506,	L"MV_WorldPositionZPlayer",	L"Z position of the player's character." },
	{ 0x78, 507,	L"MV_WorldPositionZParty1",	L"Z position of the player's 1st party member." },
	{ 0x78, 508,	L"MV_WorldPositionZParty2",	L"Z position of the player's 2nd party member." },
	{ 0x78, 509,	L"MV_WorldPositionZParty3",	L"Z position of the player's 3rd party member." },
	{ 0x78, 510,	L"MV_WorldPositionZParty4",	L"Z position of the player's 4th party member." },
	{ 0x78, 511,	L"MV_WorldPositionZ",		L"Z position of the entry's object." },
	{ 0x78, 513,	L"MV_WorldPositionY1",		L"Y position of the 1st entry's object." },
	{ 0x78, 514,	L"MV_WorldPositionY2",		L"Y position of the 2nd entry's object." },
	{ 0x78, 515,	L"MV_WorldPositionY3",		L"Y position of the 3rd entry's object." },
	{ 0x78, 516,	L"MV_WorldPositionY4",		L"Y position of the 4th entry's object." },
	{ 0x78, 517,	L"MV_WorldPositionY5",		L"Y position of the 5th entry's object." },
	{ 0x78, 518,	L"MV_WorldPositionY6",		L"Y position of the 6th entry's object." },
	{ 0x78, 519,	L"MV_WorldPositionY7",		L"Y position of the 7th entry's object." },
	{ 0x78, 520,	L"MV_WorldPositionY8",		L"Y position of the 8th entry's object." },
	{ 0x78, 521,	L"MV_WorldPositionY9",		L"Y position of the 9th entry's object." },
	{ 0x78, 522,	L"MV_WorldPositionY10",		L"Y position of the 10th entry's object." },
	{ 0x78, 523,	L"MV_WorldPositionY11",		L"Y position of the 11th entry's object." },
	{ 0x78, 524,	L"MV_WorldPositionY12",		L"Y position of the 12th entry's object." },
	{ 0x78, 762,	L"MV_WorldPositionYPlayer",	L"Y position of the player's character." },
	{ 0x78, 763,	L"MV_WorldPositionYParty1",	L"Y position of the player's 1st party member." },
	{ 0x78, 764,	L"MV_WorldPositionYParty2",	L"Y position of the player's 2nd party member." },
	{ 0x78, 765,	L"MV_WorldPositionYParty3",	L"Y position of the player's 3rd party member." },
	{ 0x78, 766,	L"MV_WorldPositionYParty4",	L"Y position of the player's 4th party member." },
	{ 0x78, 767,	L"MV_WorldPositionY",		L"Y position of the entry's object." },
	{ 0x78, 769,	L"MV_WorldPositionAngle1",	L"Facing angle of the 1st entry's object." },
	{ 0x78, 770,	L"MV_WorldPositionAngle2",	L"Facing angle of the 2nd entry's object." },
	{ 0x78, 771,	L"MV_WorldPositionAngle3",	L"Facing angle of the 3rd entry's object." },
	{ 0x78, 772,	L"MV_WorldPositionAngle4",	L"Facing angle of the 4th entry's object." },
	{ 0x78, 773,	L"MV_WorldPositionAngle5",	L"Facing angle of the 5th entry's object." },
	{ 0x78, 774,	L"MV_WorldPositionAngle6",	L"Facing angle of the 6th entry's object." },
	{ 0x78, 775,	L"MV_WorldPositionAngle7",	L"Facing angle of the 7th entry's object." },
	{ 0x78, 776,	L"MV_WorldPositionAngle8",	L"Facing angle of the 8th entry's object." },
	{ 0x78, 777,	L"MV_WorldPositionAngle9",	L"Facing angle of the 9th entry's object." },
	{ 0x78, 778,	L"MV_WorldPositionAngle10",	L"Facing angle of the 10th entry's object." },
	{ 0x78, 779,	L"MV_WorldPositionAngle11",	L"Facing angle of the 11th entry's object." },
	{ 0x78, 780,	L"MV_WorldPositionAngle12",	L"Facing angle of the 12th entry's object." },
	{ 0x78, 1018,	L"MV_WorldPositionAnglePlayer",	L"Facing angle of the player's character." },
	{ 0x78, 1019,	L"MV_WorldPositionAngleParty1",	L"Facing angle of the player's 1st party member." },
	{ 0x78, 1020,	L"MV_WorldPositionAngleParty2",	L"Facing angle of the player's 2nd party member." },
	{ 0x78, 1021,	L"MV_WorldPositionAngleParty3",	L"Facing angle of the player's 3rd party member." },
	{ 0x78, 1022,	L"MV_WorldPositionAngleParty4",	L"Facing angle of the player's 4th party member." },
	{ 0x78, 1023,	L"MV_WorldPositionAngle",	L"Facing angle of the entry's object." },
	{ 0x79, 0,		L"SV_Target",				L"Shared variable with the battle system.\nTarget of the enemy's attack for a further Attack call.\n\nThe target is a list of 8 bits, first 4 bits being the party members and last 4 bits being the enemies." },
	{ 0x79, 1,		L"SV_FunctionEnemy",		L"Shared variable with the battle system.\nReturn the enemy's identifying bit." },
	{ 0x79, 2,		L"SV_PlayerTeam",			L"Shared variable with the battle system.\nReturn bits identifying the fighting party characters." },
	{ 0x79, 3,		L"SV_EnemyTeam",			L"Shared variable with the battle system.\nReturn bits identifying the fighting enemies." },
	{ 0x7A, 0,		L"GetRandom",				L"Return a random number between 0 and 255." },
	{ 0x7A, 1,		L"GetFieldCamera",			L"Return the ID of the current field camera." },
	{ 0x7A, 2,		L"IsMovementEnabled",		L"Check if the player's movement is on." },
	{ 0x7A, 3,		L"SyncSounds",				L"Synchronise field sounds and return 0." },
	{ 0x7A, 4,		L"GetCollisionAngle",		L"Return the angle between the player's character and the NPC for collision events." },
	{ 0x7A, 5,		L"GetScriptCharacter",		L"Return the ID of the character whose script has called this. Return a negative value if it's not a playable character or if the current entry wasn't run by another function." },
	{ 0x7A, 6,		L"GetGil",					L"Return the player's Gil amount." },
	{ 0x7A, 7,		L"GetTotalSteps",			L"Return the total amount of steps walked." },
	{ 0x7A, 8,		L"GetDialogProgression",	L"Return the progression counter set by dialog's [PROGRESSION] code." },
	{ 0x7A, 9,		L"GetDialogChoice",			L"Return the choice selected in a multichoice dialog." },
	{ 0x7A, 10,		L"GetFieldExitX",			L"Return the field activated exit position." },
	{ 0x7A, 11,		L"GetFieldExitY",			L"Return the field activated exit position." },
//	{ 0x7A, 12,		L"",		L"." },		sSysX
//	{ 0x7A, 13,		L"",		L"." },
	{ 0x7A, 14,		L"GetCinematicFrame",		L"Return the frame of the currently running cinematic." },
	{ 0x7A, 15,		L"SyncCinematic",			L"Synchronise cinematic." },
	{ 0x7A, 16,		L"GetFrogAmount",			L"Return the amount of frogs catched." },
	{ 0x7A, 17,		L"GetTimerTime",			L"Return the time left for the timer of the timer window, in seconds." },
	{ 0x7A, 18,		L"GetTetraMasterResult",	L"Return the result of the last Tetra Master game.\n\n0 stands for a win.\n1 stands for a defeat.\n2 stands for a draw." },
	{ 0x7A, 19,		L"GetCardAmount",			L"Return the amount of cards owned by the player." },
	{ 0x7A, 20,		L"GetTime",					L"Return the time clock, in seconds." },
	{ 0x7A, 21,		L"IsTimerShown",			L"Check if the timer window is currently displayed." },
	{ 0x7A, 22,		L"GetSoundDistance",		L"Seem to return some distance about 3D sounds." },
	{ 0x7A, 23,		L"GetSoundVolume",			L"Seem to return the volume of some 3D sounds." },
	{ 0x7A, 24,		L"GetLastBattleId",			L"Return the last battle ID." },
	{ 0x7A, 25,		L"IsAttacking",				L"Check if the function's enemy is attacking." },
	{ 0x7A, 26,		L"IsBattleInitialized",		L"Check if the current running module is a battle." },
	{ 0x7A, 27,		L"GetBattleResult",			L"Return the result of the last battle.\n\n0 stands for a defeat.\n1 stands for a win.\n5 stands for an interrupted battle.\n7 stands for an enemy escape." },
	{ 0x7A, 28,		L"GetAttackCommandId",		L"Return the command ID of the current attack." },
	{ 0x7A, 29,		L"GetAttackId",				L"Return the attack ID of the current attack." },
	{ 0x7A, 30,		L"GetBattleState",			L"Return the battle's state.\n\n0 stands for system initialization.\n1 stands for frozen battle.\n2 stands for the starting camera movements.\n3 stands for a normal state.\n4 stands for state turning to the normal state.\n5 stands for a normal state.\n6 stands for the victory fanfare.\n7 stands for a game over state.\n8 stands for the closing screen fading." },
	{ 0x7A, 31,		L"GetBattleGroupId",		L"Return the chosen group ID of the current battle." },
	{ 0x7A, 32,		L"GetBattleStartType",		L"Return the back attack state of the start of the battle.\n\n0 stands for a back attack.\n1 stands for a preemptive strike.\n2 stands for a normal state." },
	{ 0x7A, 33,		L"GetBattleGil",			L"Return the player's Gil amount. Should be used in battles only." },
	{ 0x7A, 34,		L"GetCurrentBattleState",	L"Return the battle's current state. Should be used in battles only.\n\n0 stands for system initialization.\n1 stands for frozen battle.\n2 stands for the starting camera movements.\n3 stands for a normal state.\n4 stands for state turning to the normal state.\n5 stands for a normal state.\n6 stands for the victory fanfare.\n7 stands for a game over state.\n8 stands for the closing screen fading." },
//	{ 0x7A, 35,		L"",		L"." },		IsAttacking-like?
	{ 0x7A, 36,		L"GetAttacker",				L"Return the bit identifying the attacker." },
	{ 0x7A, 37,		L"GetTarget",				L"Return the bit identifying the target." },
	{ 0x7A, 38,		L"GetAttackerWeaponElement",L"Return the element of attacker's weapon for a standard attack." },
	{ 0x7A, 39,		L"GetAttackElement",		L"Return the element of the attack.\nWorks even when the effect doesn't take the element into account but return 0 if the caster has Element Null." },
	{ 0x7A, 40,		L"GetBattleLoadState",		L"Return the bitflags of battle loading state.\n\n8 stands for enemy data loaded.\n16 stands for player characters loaded." },
	{ 0x7A, 41,		L"GetExpGain",				L"Return the experience gain of the current battle so far." },
//	{ 0x7A, 42,		L"",		L"." },		Seem to check an Eiko disponibility
	{ 0x7A, 192,	L"GetWorldArea",			L"Return the area ID at player's position." },
	{ 0x7A, 193,	L"GetWorldTerrainType",		L"Return the terrain type at player's position." },
//	{ 0x7A, 194,	L"",		L"." },		Maybe GetHeight
//	{ 0x7A, 195,	L"",		L"." },		Some position ; maybe of the town left?
//	{ 0x7A, 196,	L"",		L"." },
//	{ 0x7A, 197,	L"",		L"." },
	{ 0x7A, 198,	L"GetCurrentChocograph",	L"Return the current selected chocograph." },
//	{ 0x7A, 199,	L"",		L"." },		Get camera rotation?
//	{ 0x7A, 200,	L"",		L"." },		Return 0 on Steam
	{ 0x7A, 201,	L"GetWorldRealPositionX",	L"Return the player's character position X." },
	{ 0x7A, 202,	L"GetWorldRealPositionZ",	L"Return the player's character position Z." },
	{ 0x7A, 203,	L"GetWorldRealPositionY",	L"Return the player's character position Y." },
//	{ 0x7A, 204,	L"",		L"." },		w_weatherDistanceEva
	{ 0x7A, 205,	L"IsMouseEncounterable",	L"Check if the player is walking in a forest, is on the World Map since more than 400 frames and if the encounter odds are favorable. Return 1 in this case and 0 else." },
	{ 0x7A, 207,	L"GetWorldZone",			L"Return the zone ID at player's position." },
//	{ 0x7A, 208,	L"",		L"." },		Maybe GetHeight on the floor
	{ 0x7A, 209,	L"IsWorldInitialized",		L"Check if the World Map event loop is already running." },
	{ 0x7A, 210,	L"GetWorldTitleDisplay",	L"Return the state of the continent's name display.\n\n0 stands for a title waiting for display.\n1 stands for a fading in title.\n2 stands for a fading out title.\n3 stands for nothing displayed." },
//	{ 0xC5, 184,	L"General_SaveFlag",		L"." },
	{ 0xCC, 182,	L"Chocobo_PeapperSpotFound",L"Bit flags for dead peapper spots already found." },
	{ 0xC8, 184,	L"Chocobo_ChocographFound",	L"Bit flags for chocographs already found." },
	{ 0xD8, 2,		L"General_FieldEntrance",	L"Variable shared between field transition to know where the player comes from." },
	{ 0xD4, 4,		L"Setting_OptionalQuina",	L"Flag checking if Quina is in the party.\nUsed for the part where s/he is an optional character." },
	{ 0xD4, 16,		L"Setting_TranceOn",		L"Flag enabling or disabling the trance system." },
	{ 0xD4, 17,		L"Setting_DaggerDepresses",	L"Flag enabling or disabling Dagger depression state." },
	{ 0xD4, 18,		L"Setting_MPx4",			L"Flag enabling or disabling the MP cost boost." },
//	{ 0xD5, 24,		L"World_CloseLocationFoot" },
//	{ 0xD5, 26,		L"World_CloseLocationChocobo" },
//	{ 0xD5, 28,		L"World_CloseLocationShip" },
//	{ 0xD5, 35,		L"World_MegamapShow" },
//	{ 0xD5, 38,		L"World_CloseLocationKnown" },
//	{ 0xD5, 39,		L"World_CloseLocation" },
	{ 0xDC, 92,		L"World_LocationKnown1",	L"Bit flags for World Map locations already known." },
	{ 0xDC, 94,		L"World_LocationKnown2",	L"Bit flags for World Map locations already known." },
	{ 0xDC, 96,		L"World_LocationKnown3",	L"Bit flags for World Map locations already known." },
	{ 0xDC, 98,		L"World_LocationKnown4",	L"Bit flags for World Map locations already known." },
	{ 0xD4, 100,	L"World_MinimapShow",		L"Flag enabling or disabling the World Minimap." },
	{ 0xD4, 103,	L"World_LocationSelection",	L"Location currently selected by the player on the World Minimap." },
	{ 0xDC, 136,	L"Chocobo_BeakExp",			L"Choco's total beak experience." },
	{ 0xD4, 139,	L"Chocobo_BeakLevel",		L"Choco's current beak level." },
	{ 0xD8, 140,	L"Chocobo_BeakNextLevel",	L"Choco's current beak experience needed to level up." },
	{ 0xD4, 190,	L"World_CurrentTransport",	L"Current transport used by the player on the World Map." },
	{ 0xD4, 191,	L"Chocobo_ChocoColor",		L"Choco's current evolution." },
	{ 0xD4, 192,	L"General_TonberryCounter",	L"Counter of twice the Tonberries killed.\nUsed for Grudge's damage formula :\nDamage = 2 ^ General_TonberryCounter." },
	{ 0xD4, 193,	L"General_SummonMadeen",	L"Flag checking if Madeen has already been summoned at least once." },
	{ 0xD4, 194,	L"FriendlyMonster_Progress",L"Bit flags for friendly monsters met and satisfied." },
	{ 0xD4, 195,	L"Ragtime_QuizzDone1",		L"Bit flags for Ragtime Mouse questions already answered." },
	{ 0xD4, 196,	L"Ragtime_QuizzDone2",		L"Bit flags for Ragtime Mouse questions already answered." },
	{ 0xD4, 197,	L"Ragtime_QuizzDone3",		L"Bit flags for Ragtime Mouse questions already answered." },
	{ 0xD4, 198,	L"Ragtime_QuizzSuccess",	L"Bit flags and integer mainly used for Ragtime Mouse questions already successfully answered.\n\n 2nd bit : Dagger reacted to Eiko summoning in the Mountain Path.\n 4th to 8th bits : amount of Ragtime Mouse questions rightfully answered." },
	{ 0xD4, 207,	L"General_SummonFlag",		L"Bit flags for each summon checking if it has already been summoned at least once." },
	{ 0xDC, 0,		L"General_ScenarioCounter",	L"Scenario counter, increasing at various point of the game." },
//	{ 0xE5, 324,	L"Setting_AllowPartySwitch",L"." },
	{ 0xE4, 1460,	L"Chocobo_FoundGarden",		L"Flag checking if the Flying Garden has been found." },
	{ 0xE4, 1584,	L"FriendlyMonster_Complete",L"Flag checking if the Friendly Monster quest has been totaly completed (Friendly Yan has been satisfied)." },
	{ 0xE4, 1816,	L"Setting_MagicOff",		L"Flag enabling or disabling the use of magic in the player's menu." },
	{ 0xF4, 509,	L"Stiltzkin_ItemBought",	L"Bit flags for Stiltzkin items bought." },
};

static wstring FunctionTypeName[] {
	L"Init",
	L"Loop",
	L"Range",
	L"SpeakBTN",
	L"Unk",
	L"ATB",
	L"Counter",
	L"CounterEx",
	L"CardBTN",
	L"Death",
	L"Reinit"
};

static wstring DiscName[] {
	L"Disc 1",
	L"Disc 2",
	L"Disc 3",
	L"Disc 4"
};

static wstring PlaystationButton[] {
	L"Select",
	L"???",
	L"???",
	L"Start",
	L"Up",
	L"Right",
	L"Down",
	L"Left",
	L"L2",
	L"R2",
	L"L1",
	L"R1",
	L"Triangle",
	L"Circle",
	L"Cross",
	L"Square",
	L"Cancel",
	L"Confirm",
	L"???",
	L"Moogle",
	L"L1 Ex",
	L"R1 Ex",
	L"L2 Ex",
	L"R2 Ex",
	L"Menu",
	L"Select Ex"
};

static wstring BattleState[] {
	L"FreezeATB",
	L"InitPhase",
	L"ShowATB",
	L"EndPhase",
	L"???",
	L"???",
	L"???",
	L"???"
};

static wstring MenuType[] {
	L"Player's Menu",
	L"Naming Menu",
	L"Shop Menu",
	L"???",
	L"Save Menu",
	L"Chocograph Menu"
};

static wstring WindowFlags[] {
	L"???",
	L"???",
	L"Disable Tail",
	L"Mognet",
	L"Hide Window",
	L"No Focus",
	L"ATE",
	L"Dialog"
};

static wstring AnimationFlags[] {
	L"Freeze at end",
	L"Loop",
	L"Loop forth and back",
	L"???",
	L"???",
	L"???",
	L"???",
	L"???"
};

static wstring AbilitySetName[] {
	L"Zidane's",
	L"Vivi's",
	L"Dagger's",
	L"Steiner's",
	L"Freya's",
	L"Quina's",
	L"Eiko's",
	L"Amarant's",
	L"Cinna's (1)",
	L"Cinna's (2)",
	L"Marcus's (1)",
	L"Marcus's (2)",
	L"Blank's (1)",
	L"Blank's (2)",
	L"Beatrix's (1)",
	L"Beatrix's (2)",
	L"Theater Zidane's",
	L"Theater Cinna's",
	L"Theater Marcus's",
	L"Theater Blank's"
};

static wstring BubbleSymbolName[] = {
	L"?",
	L"!",
	L"! - Card",
	L"Beach",
	L"! - Beach"
};

struct ConstantNames {
	uint16_t id;
	wstring name;
};

static ConstantNames ScriptFunctionType[] = {
	{ 0, L"main" },
	{ 1, L"loop" },
	{ 2, L"range" },
	{ 3, L"speak" },
	{ 5, L"atb" },
	{ 6, L"counterparty" },
	{ 7, L"counterenemy" },
	{ 8, L"card" },
	{ 9, L"death" },
	{ 10, L"routine1" },
	{ 11, L"routine2" },
	{ 12, L"routine3" },
	{ 13, L"routine4" },
	{ 14, L"routine5" },
	{ 15, L"routine6" },
	{ 16, L"routine7" },
	{ 17, L"routine8" },
	{ 18, L"meet" },
};

static ConstantNames ScriptCharacterField[] = {
	{ 32, L"PRESENCE_ON" },
	{ 33, L"PRESENCE_OFF" },
	{ 34, L"PREVENT_ATTACK" },
	{ 35, L"MAX_HP" },
	{ 36, L"HP" },
	{ 37, L"MAX_MP" },
	{ 38, L"MP" },
	{ 39, L"MAX_ATB" },
	{ 40, L"ATB" },
	{ 41, L"LEVEL" },
	{ 42, L"STATUS_IMMUNE_B" },
	{ 43, L"STATUS_IMMUNE_A" },
	{ 44, L"STATUS_AUTO_B" },
	{ 45, L"STATUS_AUTO_A" },
	{ 46, L"STATUS_CURRENT_B" },
	{ 47, L"STATUS_CURRENT_A" },
	{ 48, L"ELEMENT_IMMUNE" },
	{ 49, L"ELEMENT_ABSORB" },
	{ 50, L"ELEMENT_HALF" },
	{ 51, L"ELEMENT_WEAK" },
	{ 52, L"TARGETABLE" },
	{ 53, L"DISAPPEAR" },
	{ 54, L"SHADOW" },
	{ 55, L"MODEL_SIZE" },
	{ 56, L"RESET_SIZE" },
	{ 57, L"MODEL" },
	{ 58, L"MODEL_VISIBLE" },
	{ 59, L"MODEL_OFF" },
	{ 60, L"MODEL_ON" },
	{ 61, L"PLAY_ANIM" },
	{ 62, L"STOP_ANIM" },
	{ 63, L"PLAY_ANIM_ONCE" },
	{ 64, L"ROW" },
	{ 65, L"COLUMN" },
	{ 66, L"MODEL_TYPE" },
	{ 67, L"CATEGORY" },
	{ 68, L"CLASS" },
	{ 69, L"STAND_ANIMATION" },
	{ 70, L"ENEMY_ID" },
	{ 71, L"DEFEATED_ON" },
	{ 72, L"STRENGTH" },
	{ 73, L"MAGIC" },
	{ 74, L"DEFENCE" },
	{ 75, L"EVADE" },
	{ 76, L"MAGIC_DEFENCE" },
	{ 77, L"MAGIC_EVADE" },
	{ 78, L"MARTHYM_ON" }
};

static ConstantNames EquipSetName[] = {
	{ 0xFF, L"Don't change" },
	{ 0x00, L"Zidane's" },
	{ 0x01, L"Vivi's" },
	{ 0x02, L"Dagger's" },
	{ 0x03, L"Steiner's" },
	{ 0x04, L"Freya's" },
	{ 0x05, L"Quina's" },
	{ 0x06, L"Eiko's" },
	{ 0x07, L"Amarant's" },
	{ 0x08, L"Cinna's" },
	{ 0x09, L"Marcus's (1)" },
	{ 0x0A, L"Blank's (1)" },
	{ 0x0B, L"Beatrix's (1)" },
	{ 0x0C, L"Marcus's (2)" },
	{ 0x0D, L"Beatrix's (2)" },
	{ 0x0E, L"Blank's (2)" }
};

static ConstantNames FMVNameList[] = {
	{ 0x00,	L"Opening" },
	{ 0x03,	L"Intro" },
	{ 0x04,	L"Prima Vista reaches Alexandria" },
	{ 0x05,	L"Vivi falls" },
	{ 0x08,	L"Theatre Show begins" },
	{ 0x09,	L"Dagger jumps and Steiner crashes" },
	{ 0x0A,	L"Brahne attacks Prima Vista" },
	{ 0x0B,	L"Prima Vista flees" },
	{ 0x0C,	L"Prima Vista crashes" },
	{ 0x0D,	L"Ruined Prima Vista in Evil Forest" },
	{ 0x10,	L"Evil Forest gets stoned" },
	{ 0x11,	L"Cargo Ship takes off" },
	{ 0x14,	L"Black Waltz 3 incoming" },
	{ 0x15,	L"Black Waltz 3 kills Black Mages" },
	{ 0x16,	L"Cargo Ship crosses the Gate" },
	{ 0x17,	L"The Forest Guardian" },
	{ 0x1A,	L"Cargo Ship reaches Lindblum" },
	{ 0x1B,	L"Dagger is singing" },
	{ 0x1C,	L"Kuja takes off from Burmecia" }
};

static ConstantNames BattleCodeName[] = {
	{ 32, L"Disable ATB" },
	{ 33, L"End Battle" }, // 1 = victory, 5 = interuption, 7 = Enmy flee
	{ 34, L"Game Over" },
	{ 35, L"Enable ATB" },
	{ 36, L"Run Camera" },
	{ 37, L"Change Field" },
	{ 38, L"Add Gil" },
	{ 39, L"Dragon Counter++" },
	{ 40, L"Emphasize Music" }
};

static ConstantNames ModelCodeName[] = {
	{ 0, L"Texture Blend" },
	{ 4, L"Slice" },
	{ 8, L"Enable Mirror" },
	{ 9, L"Mirror Position" },
	{ 10, L"Mirror Normal" },
	{ 11, L"Mirror Color" },
	{ 16, L"Add Sound" },
	{ 17, L"Add Secondary Sound" },
	{ 18, L"Sound Random Pitch" },
	{ 19, L"Remove Sound" }
};

static ConstantNames SoundCodeName[] = {
	{ 0, L"Play Music" },
	{ 256, L"Stop Music" },
	{ 16897, L"Music Volume" },
	{ 16898, L"Music Tempo" },
	{ 16899, L"Music Pitch" },
	{ 16900, L"Music TempoPitch" },
	{ 33537, L"Music Adjust Volume" },
	{ 33538, L"Music Adjust Tempo" },
	{ 33539, L"Music Adjust Pitch" },
	{ 33540, L"Music Adjust TempoPitch" },
	{ 50177, L"Music Fade" },
	{ 1028, L"Music Fade TempoPitch" },
	{ 53248, L"Play Sound" },
	{ 20736, L"Stop Sound" },
	{ 37377, L"Sound Volume" },
	{ 37381, L"Sound Position" },
	{ 37379, L"Sound Pitch" },
	{ 54017, L"Sound Adjust Volume" },
	{ 54021, L"Sound Adjust Position" },
	{ 54019, L"Sound Adjust Pitch" },
	{ 21761, L"All Sounds Volume" },
	{ 21765, L"All Sounds Position" },
	{ 21763, L"All Sounds Pitch" },
	{ 38401, L"All Sounds Adjust Volume" },
	{ 38405, L"All Sounds Adjust Position" },
	{ 38403, L"All Sounds Adjust Pitch" }
};

//====================================//
//       Spell Animation Sequence     //
//====================================//

typedef unsigned int Spell_Animation_Argument_Type;
#define SAAT_NOTHING		0
#define SAAT_INTEGER		1
#define SAAT_CHARACTER		2
#define SAAT_EFFECT			3
#define SAAT_MODEL			4
#define SAAT_SOUND			5
#define SAAT_ANIMATION		6
#define SAAT_FADE_INOUT		7

#define SPELL_ANIMATION_SEQUENCE_ARG	2

struct SpellAnimSequenceCode {
	uint8_t id;
	wstring label;
	wstring help;
	Spell_Animation_Argument_Type arg_type[SPELL_ANIMATION_SEQUENCE_ARG];
};

static SpellAnimSequenceCode SPELLANIM_OPCODE[] = {
	{ 0x0, L"END", L"End of spell animation.\nIt must always be the last code.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_NOTHING, SAAT_NOTHING } }, // Must be the first of the list
	{ 0x01, L"Wait", L"Wait for some fixed time.\n - 1st arg : unknown\n - 2nd arg : time to wait", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x02, L"Play caster animation", L"Play an animation for the caster.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x03, L"0x03", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x04, L"0x04", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x05, L"0x05", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x06, L"0x06", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x07, L"0x07", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x08, L"0x08", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x09, L"0x09", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x0A, L"0x0A", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x0B, L"0x0B", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x0C, L"0x0C", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x0D, L"Change model", L"Change the character's model. Seem to work for the trance abilities only.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x0E, L"0x0E", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x0F, L"0x0F", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x23, L"Camera-related stuff", L"Some stuff about the different camera movements.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x24, L"Show/hide characters", L"Hide or show the characters and their attached models (such as weapons or status sfx).\n - 1st arg : fade time\n - 2nd arg : who and how", { SAAT_INTEGER, SAAT_FADE_INOUT } },
	{ 0x25, L"Effect point", L"Either apply the spell's effect (damage, status, etc...) or display them.\n - 1st arg : which character\n - 2nd arg : which effect", { SAAT_CHARACTER, SAAT_EFFECT } },
	{ 0x27, L"Channel toward target", L"Make the caster face the specified character and play its casting animation.\n - 1st arg : which character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x28, L"Put back in sleep mode", L"Make the character (the caster most of the time) face his enemies and play its usual standing animation.\n - 1st arg : which character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x29, L"Play camera", L"Play a camera movement.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x2A, L"Set battle scene transparency", L"Fade in or fade out the background scene.\n - 1st arg : alpha (0 hides it totally)\n - 2nd arg : fade time", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x2B, L"Wait for animation", L"Wait until the model's animation ends.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x2C, L"0x2C", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x2D, L"Play sound", L"Play a sound.\n - 1st arg : which sound\n - 2nd arg : unknown", { SAAT_SOUND, SAAT_INTEGER } },
	{ 0x2E, L"0x2E", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_SOUND, SAAT_INTEGER } },
	{ 0x50, L"0x50", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x51, L"Setup destination", L"Setup a destination toward the target for latter use.\n - 1st arg : distance from current position toward the target ?\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x55, L"Play miss sound", L"Play the miss sound of character's attack.\n - 1st arg : which character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x57, L"0x57", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x5A, L"Play character animation looping", L"Make a character play an animation by kind and make it loop.\n - 1st arg : which character\n - 2nd arg : kind of animation", { SAAT_CHARACTER, SAAT_ANIMATION } },
	{ 0x5B, L"Play character animation", L"Make a character play an animation by kind.\n - 1st arg : which character\n - 2nd arg : kind of animation", { SAAT_CHARACTER, SAAT_ANIMATION } },
	{ 0x5C, L"0x5C", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x5E, L"0x5E", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x5F, L"0x5F", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x61, L"0x61", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x62, L"Face back to normal", L"Make the character face his enemies.\n - 1st arg : which character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x63, L"Play raw animation", L"Make a character play an animation?\n - 1st arg : which character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x64, L"0x64", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x65, L"0x65", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x69, L"Move before", L"Instantatly move a character in front of another character.\n - 1st arg : character to move\n - 2nd arg : other character", { SAAT_CHARACTER, SAAT_CHARACTER } },
	{ 0x6A, L"0x6A", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x6B, L"0x6B", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x6C, L"0x6C", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x6D, L"0x6D", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x6E, L"0x6E", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x6F, L"Move caster", L"Smoothly moves the caster to a destination that has been set up.\n - 1st arg : unknown\n - 2nd arg : time to reach destination", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x70, L"0x70", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x71, L"0x71", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x72, L"0x72", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x73, L"0x73", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x79, L"0x79", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x7A, L"Unused 0x7A", L"Unknown effect.\n - 1st arg : some character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x7B, L"Unused 0x7B", L"Unknown effect.\n - 1st arg : some character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x7C, L"Unused 0x7C", L"Seems to shorten the sequence.\n - 1st arg : some character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x7F, L"Unused 0x7F", L"Unknown effect.\n - 1st arg : some character\n - 2nd arg : unknown", { SAAT_CHARACTER, SAAT_INTEGER } },
	{ 0x80, L"Play model on target v1", L"Display a spell model and run its animation.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x81, L"Play model on target v2", L"Display a spell model and run its animation.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x82, L"Play model on target v3", L"Display a spell model and run its animation.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x83, L"Play model on target v4", L"Display a spell model and run its animation.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x84, L"0x84", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x85, L"0x85", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x86, L"0x86", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0x87, L"0x87", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } },
	{ 0xFF, L"Unused code 0xFF", L"Unknown effect.\n - 1st arg : unknown\n - 2nd arg : unknown", { SAAT_INTEGER, SAAT_INTEGER } }
};

static SpellAnimSequenceCode& GetSpellAnimSequenceCode(uint8_t code) {
	for (unsigned int i=0;i<G_N_ELEMENTS(SPELLANIM_OPCODE);i++) {
		if (SPELLANIM_OPCODE[i].id==code)
			return SPELLANIM_OPCODE[i];
	}
	return SPELLANIM_OPCODE[G_N_ELEMENTS(SPELLANIM_OPCODE)-1];
}

struct SpellAnimSequenceArgumentString {
	Spell_Animation_Argument_Type type;
	uint8_t arg;
	wstring label;
};

static SpellAnimSequenceArgumentString SPELLANIM_ARGUMENT_STRING[] = {
	{ SAAT_CHARACTER, 0,	L"1st target" },
	{ SAAT_CHARACTER, 1,	L"2nd target" },
	{ SAAT_CHARACTER, 2,	L"3rd target" },
	{ SAAT_CHARACTER, 3,	L"4th target" },
	{ SAAT_CHARACTER, 4,	L"5th target" },
	{ SAAT_CHARACTER, 5,	L"6th target" },
	{ SAAT_CHARACTER, 6,	L"7th target" },
	{ SAAT_CHARACTER, 7,	L"8th target" },
	{ SAAT_CHARACTER, 0x10,	L"caster" },
	{ SAAT_CHARACTER, 0x11,	L"unknown" },
	{ SAAT_CHARACTER, 0xFF,	L"all targets" },
	{ SAAT_EFFECT,    0,	L"damage point" },
	{ SAAT_EFFECT,    1,	L"figure point" },
	{ SAAT_ANIMATION, 0,	L"stand" },
	{ SAAT_ANIMATION, 1,	L"stand weak" },
	{ SAAT_ANIMATION, 2,	L"hit" },
	{ SAAT_ANIMATION, 3,	L"hit fly" },
	{ SAAT_ANIMATION, 4,	L"stand KO" },
	{ SAAT_ANIMATION, 5,	L"weak -> stand" },
	{ SAAT_ANIMATION, 6,	L"KO -> stand" },
	{ SAAT_ANIMATION, 7,	L"hit hard" },
	{ SAAT_ANIMATION, 8,	L"KO" },
	{ SAAT_ANIMATION, 9,	L"stand ready" },
	{ SAAT_ANIMATION, 10,	L"ready" },
	{ SAAT_ANIMATION, 11,	L"KO -> stand weak" },
	{ SAAT_ANIMATION, 12,	L"defend" },
	{ SAAT_ANIMATION, 13,	L"stand defend" },
	{ SAAT_ANIMATION, 14,	L"defend -> stand" },
	{ SAAT_ANIMATION, 15,	L"???" },
	{ SAAT_ANIMATION, 16,	L"dodge" },
	{ SAAT_ANIMATION, 17,	L"flee" },
	{ SAAT_ANIMATION, 18,	L"victory" },
	{ SAAT_ANIMATION, 19,	L"stand victory" },
	{ SAAT_ANIMATION, 20,	L"run" },
	{ SAAT_ANIMATION, 21,	L"stand run" },
	{ SAAT_ANIMATION, 22,	L"attack 1" },
	{ SAAT_ANIMATION, 23,	L"attack 2" },
	{ SAAT_ANIMATION, 24,	L"jump back 1" },
	{ SAAT_ANIMATION, 25,	L"jump back 2" },
	{ SAAT_ANIMATION, 26,	L"cast" },
	{ SAAT_ANIMATION, 27,	L"stand cast" },
	{ SAAT_ANIMATION, 28,	L"cast -> stand" },
	{ SAAT_ANIMATION, 29,	L"move forward" },
	{ SAAT_ANIMATION, 30,	L"move backward" },
	{ SAAT_ANIMATION, 31,	L"item" },
	{ SAAT_ANIMATION, 32,	L"ready -> stand" },
	{ SAAT_ANIMATION, 33,	L"cast alternate" },
	{ SAAT_FADE_INOUT, 0,	L"show team" },
	{ SAAT_FADE_INOUT, 1,	L"show enemies" },
	{ SAAT_FADE_INOUT, 2,	L"show all" },
	{ SAAT_FADE_INOUT, 3,	L"???" },
	{ SAAT_FADE_INOUT, 4,	L"hide team" },
	{ SAAT_FADE_INOUT, 5,	L"hide enemies" },
	{ SAAT_FADE_INOUT, 6,	L"hide all" },
	{ SAAT_FADE_INOUT, 7,	L"???" },
	{ SAAT_FADE_INOUT, 8,	L"show team but caster" },
	{ SAAT_FADE_INOUT, 9,	L"show caster" },
	{ SAAT_FADE_INOUT, 10,	L"???" },
	{ SAAT_FADE_INOUT, 11,	L"???" },
	{ SAAT_FADE_INOUT, 12,	L"hide team but caster" },
	{ SAAT_FADE_INOUT, 13,	L"hide caster" },
	{ SAAT_FADE_INOUT, 14,	L"???" },
	{ SAAT_FADE_INOUT, 15,	L"???" }
};

//====================================//
//       Enemy Animation Sequence     //
//====================================//

typedef unsigned int Enemy_Animation_Argument_Type;
#define EAAT_INTEGER		0
#define EAAT_COORDINATE		1
#define EAAT_ANIMATION		2
#define EAAT_TEXT			3
#define EAAT_SPELL_ANIM		4
#define EAAT_TIME			5
#define EAAT_SOUND			6
struct EnemySequenceCode {
	uint8_t id;
	wstring label;
	wstring help;
	int arg_amount;
	int* arg_length;
	Enemy_Animation_Argument_Type* arg_type;
};

static EnemySequenceCode ENEMYANIM_OPCODE[] = {
	{ 0x01, L"Wait", L"Wait some time.\n - 1st arg : Time", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x07, L"Wait Animation", L"Wait until the animation is completed.", 0, NULL, NULL },
	{ 0x02, L"Effect Point", L"Apply the effect.", 0, NULL, NULL },
	{ 0x0E, L"Battle Text", L"Display battle text.\n - 1st arg : Text", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_TEXT } },
	{ 0x21, L"Battle Text 2", L"Display battle text.\n - 1st arg : Text", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_TEXT } },
	{ 0x08, L"Set Spell Animation + Channel", L"Define the next spell animation to run with channelling.\n - 1st arg : Animation\n - 2nd arg : Caster attachement point\n - 3rd arg : ???\n - 4th arg : ???", 4, new int[4]{ 2, 2, 2, 2 }, new Enemy_Animation_Argument_Type[4]{ EAAT_SPELL_ANIM, EAAT_INTEGER, EAAT_INTEGER, EAAT_INTEGER } },
	{ 0x1A, L"Set Spell Animation", L"Define the next spell animation to run.\n - 1st arg : Animation\n - 2nd arg : Caster attachement point\n - 3rd arg : ???\n - 4th arg : ???", 4, new int[4]{ 2, 2, 2, 2 }, new Enemy_Animation_Argument_Type[4]{ EAAT_SPELL_ANIM, EAAT_INTEGER, EAAT_INTEGER, EAAT_INTEGER } },
	{ 0x0A, L"Run Spell Animation", L"Play the spell animation previously set.", 0, NULL, NULL },
	{ 0x05, L"Play Animation", L"Play enemy animation.\n - 1st arg : animation", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_ANIMATION } },
	{ 0x11, L"Change Stand Animation", L"Define the new standing animation for the caster.\n - 1st arg : Animation", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x03, L"Walk Target", L"Make the caster walk toward the target.\n - 1st arg : Time\n - 2nd arg : Distance Y", 2, new int[2]{ 1, 2 }, new Enemy_Animation_Argument_Type[2]{ EAAT_TIME, EAAT_COORDINATE } },
	{ 0x1E, L"Walk Target 2", L"Make the caster walk toward the target.\n - 1st arg : Time\n - 2nd arg : Distance Y", 2, new int[2]{ 1, 2 }, new Enemy_Animation_Argument_Type[2]{ EAAT_TIME, EAAT_COORDINATE } },
	{ 0x1B, L"Walk Relative", L"Make the caster walk to relative position.\n - 1st arg : Time\n - 2nd arg : Distance X\n - 3rd arg : Distance Z\n - 4th arg : Distance Y", 4, new int[4]{ 1, 2, 2, 2 }, new Enemy_Animation_Argument_Type[4]{ EAAT_TIME, EAAT_COORDINATE, EAAT_COORDINATE, EAAT_COORDINATE } },
	{ 0x13, L"Walk Absolute", L"Make the caster walk to absolute position.\n - 1st arg : Time\n - 2nd arg : Position X\n - 3rd arg : Position Z\n - 4th arg : Position Y", 4, new int[4]{ 1, 2, 2, 2 }, new Enemy_Animation_Argument_Type[4]{ EAAT_TIME, EAAT_COORDINATE, EAAT_COORDINATE, EAAT_COORDINATE } },
	{ 0x0C, L"Resize", L"Resize the caster.\n - 1st arg : ???\n - 2nd arg : Size factor (16 keeps the size)\n - 3rd arg : Time", 3, new int[3]{ 1, 1, 1 }, new Enemy_Animation_Argument_Type[3]{ EAAT_INTEGER, EAAT_INTEGER, EAAT_TIME } },
	{ 0x20, L"Run Camera", L"Run a camera sequence.\n - 1st arg : Camera sequence", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x12, L"Run Camera 2", L"Run a camera sequence.\n - 1st arg : Camera sequence", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x19, L"Play Sound", L"Play a sound.\n - 1st arg : Sound\n - 2nd arg : ???\n - 3rd arg : ???", 3, new int[3]{ 2, 2, 1 }, new Enemy_Animation_Argument_Type[3]{ EAAT_SOUND, EAAT_INTEGER, EAAT_INTEGER } },
	{ 0x06, L"SFX", L"Seems to have various effects (display sfx, apply the effect and display damage...).\n - 1st arg : SFX\n - 2nd arg : ???\n - 3rd arg : Time to wait before running", 3, new int[3]{ 2, 1, 1 }, new Enemy_Animation_Argument_Type[3]{ EAAT_INTEGER, EAAT_INTEGER, EAAT_INTEGER } },
	{ 0x18, L"End", L"Terminate animation sequence?", 0, NULL, NULL }, // unsure ; followed by 0x00
	{ 0x00, L"END", L"Terminate animation sequence.", 0, NULL, NULL },
	{ 0x04, L"Return", L"Return to the normal position after walking toward a target.\n - 1st arg : Time", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_TIME } },
	{ 0x09, L"0x09", L"Unknown effect.", 0, NULL, NULL },
	{ 0x0B, L"0x0B", L"Unknown effect.", 0, NULL, NULL },
	{ 0x0D, L"0x0D", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 2 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x0F, L"0x0F", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 2 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x10, L"0x10", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x14, L"0x14", L"Unknown effect.\n - 1st arg : ???", 3, new int[3]{ 1, 2, 2 }, new Enemy_Animation_Argument_Type[3]{ EAAT_INTEGER, EAAT_INTEGER, EAAT_INTEGER } },
	{ 0x15, L"0x15", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x16, L"0x16", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x17, L"0x17", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x1C, L"0x1C", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x1D, L"0x1D", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } },
	{ 0x1F, L"0x1F", L"Unknown effect.\n - 1st arg : ???", 1, new int[1]{ 1 }, new Enemy_Animation_Argument_Type[1]{ EAAT_INTEGER } }
};

static EnemySequenceCode& GetEnemySequenceCode(uint8_t code) {
	for (unsigned int i=0;i<G_N_ELEMENTS(ENEMYANIM_OPCODE);i++) {
		if (ENEMYANIM_OPCODE[i].id==code)
			return ENEMYANIM_OPCODE[i];
	}
	return ENEMYANIM_OPCODE[0];
}

//====================================//
//             MIPS Script            //
//====================================//

struct MipsHelpField {
	wstring label;
	wstring help;
};

static MipsHelpField MIPS_HELP[] = {
	{ L"Introduction",	L"MIPS is the deepest coding language, shared by all the PSX games. As such, modifying it is extremely dangerous unless you know what you are doing.\n\n"
						L"This help only gives a few hints about modifying Final Fantasy IX's MIPS code. For a general documentation about MIPS, you can go to :\n"
						L"http://cgi.cse.unsw.edu.au/~cs3231/doc/R3000.pdf\n\n"
						L"You may find some basic operations done tediously. In particular, multiplying by constant numbers such as 100 is done by default with lines like these :\n"
						L" $2 = $1 << 1\n"
						L" $2 = $2 + $1\n"
						L" $2 = $2 << 3\n"
						L" $2 = $1 + $2\n"
						L" $1 = $2 << 2\n"
						L"This is because raw multiplications are considered slow and are avoided when possible. This kind of optimisation appears in scripts that are not looping (and thus don't cause slowness problems).\n"
						L"You may replace those by raw multiplications to get some space and don't bother doing this optimisation in your own code.\n\n"
						L"One other very important thing to note is that the jumps are always delayed by 1 instruction. That means the line following a jump is executed before actually jumping.\n" },
	{ L"Battle - Function Dependency",	L"Spell effects are parts of the whole battle code. It uses references to other parts of this code and sometimes to other spell effects.\n"
										L"Apart from the effects which are strictly the same, here is a list of the dependencies between spell effects :\n\n"
										L"%9 is partially used by %19 and %20\n"
										L"%11 is used by %108 and %109\n"
										L"%15 is partially used by %69\n"
										L"%16 is partially used by %26, %28 and %83\n"
										L"%17 is partially used by %91\n"
										L"%24 is partially used by %22 and %23\n"
										L"%26 is used by %28\n"
										L"%27 is partially used by %25\n"
										L"%32 is partially used by %17 and %91\n"
										L"%48 is partially used by %42\n"
										L"%50 is partially used by %14\n"
										L"%63 is partially used by %68\n"
										L"%65 is partially used by %51\n"
										L"%66 is partially used by %53\n"
										L"%69 is partially used by %10\n"
										L"%71 is partially used by %50\n"
										L"%72 is partially used by %13 and %71\n"
										L"%73 is used by %62 and %72\n"
										L"%93 is partially used by %76\n"
										L"%98 is partially used by %18, %21, %28, %41, %52, %54, %63 and %97\n"
										L"%99 is partially used by %74 and %75\n"
										L"%101 is partially used by %58\n"
										L"%102 is partially used by %101\n"
										L"%103 is partially used by %109\n"
										L"%104 is partially used by %38, %66, %67 and %68\n"
										L"%107 is partially used by %8 and %85\n\n"
										L"In addition to those, Apply Effect is called by most of spell effects."},
	{ L"Battle - Apply Effect",	L"Apply Effect allows to deal damage, add or remove a status or simply update the state of the battle situation.\n"
								L"Its arguments are set in the memory addresses $29[...]. Here is a list of those arguments, with the byte size in parentheses :\n\n"
								L"24(4) : Caster ID\n"
								L"28(4) : Target ID\n"
								L"32(4) : Maybe Spell Instance ID\n"
								L"38(2) : Damage Formula - Base\n"
								L"40(2) : Damage Formula - Defence\n"
								L"44(2) : Damage Formula - Bonus\n"
								L"48(2) : Effect Flags (Miss ; Dodge ; MP Attack ; Absorb ; ??? ; ??? ; Guard ; Set HP ; Add Status ; - last bits unused -)\n"
								L"50(1) : Caster Flags (HP Damage ; HP Heal ; Critical Message ; MP Damage ; MP Heal ; Miss Message ; Death Message ; Guard Message )\n"
								L"51(1) : Target Flags (HP Damage ; HP Heal ; Critical Message ; MP Damage ; MP Heal ; Miss Message ; Death Message ; Guard Message )\n"
								L"52(2) : Caster HP Damage\n"
								L"54(2) : Target HP Damage\n"
								L"56(2) : Caster MP Damage\n"
								L"58(2) : Target MP Damage\n" },
	{ L"Battle - Data Access",	L"MIPS code can always access to everything in the RAM... provided you know where to seek !\n"
								L"In the following list of useful datas for spell effects, the numbers in parentheses indicate the byte sizes and thus which mask you must use between 0xFF, 0xFFFF and 0xFFFFFFFF.\n\n"
								L"Spell Stats :\n"
								L" $1 = $17[8]\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by :\n"
								L" 1 for its animation(2)\n"
								L" 4 for its effect(1)\n"
								L" 5 for its power(1)\n"
								L" 6 for its element(1)\n"
								L" 7 for its accuracy(1)\n"
								L" 8 for its flags(1)\n"
								L" 9 for its status set(1)\n"
								L" 10 for its mp cost(1)\n"
								L" 12 for its alternate animation(2)\n\n"
								L"Caster Stats :\n"
								L" $1 = $21[N] & mask\n"
								L"Target Stats :\n"
								L" $1 = $19[N] & mask\n"
								L"with N replaced by :\n"
								L" 36 for max HP(2)\n"
								L" 38 for max MP(2)\n"
								L" 40 for HP(2)\n"
								L" 42 for MP(2)\n"
								L" 48 for speed(1)\n"
								L" 49 for strength(1)\n"
								L" 50 for magic(1)\n"
								L" 51 for spirit(1)\n"
								L" 60 for status(3)\n"
								L" 70 for category(1)\n"
								L" 108 for defence(1)\n"
								L" 109 for evade(1)\n"
								L" 110 for magic defence(1)\n"
								L" 111 for magic evade(1)\n"
								L" 112 for elemental immunity(1)\n"
								L" 113 for elemental absorption(1)\n"
								L" 114 for elemental resistance(1)\n"
								L" 115 for elemental weakness(1)\n"
								L" 116 for weapon ID(4)\n"
								L" 120 for trance gauge(1)\n"
								L" 121 for elemental boost(1)\n"
								L" 122 for level(1)\n"
								L" 196 for activate supporting abilities(8)\n\n"
								L"Weapon Stats :\n"
								L" $1 = $19[116] or $21[116]\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by :\n"
								L" 0 for its flags(1)\n"
								L" 1 for its status(1)\n"
								L" 2 for its model(2)\n"
								L" 4 for its damage formula(1)\n"
								L" 5 for its attack power(1)\n"
								L" 6 for its element(1)\n"
								L" 7 for its status accuracy(1)\n\n"
								L"Random Number (between 0 and 0xFFFFFFFF):\n"
								L" $16 = 32776 << 16\n"
								L" $1 = $16[-17448]\n"
								L" jump 0x4C600 (store return position to $31)\n"
								L" nothing\n\n"
								L"Other :\n"
								L" $16 : spell effect ID\n" },
	{ L"Generic - Data Access",	L"MIPS code can always access to everything in the RAM... provided you know where to seek !\n"
								L"In the following list of useful datas that are loaded in the RAM most of the time, the numbers in parentheses indicate the byte sizes.\n\n"
								L"General VAR_B variables :\n"
								L" $1 = 32776 << 16\n"
								L" $1 = $1 + 45992\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by the wanted variable ID (see the script help for a list of general variables)\n\n"
								L"Others :\n"
								L" $1 = 32775 << 16\n"
								L" $1 = $1[-32732]\n"
								L" $1 = $1[28]\n"
								L" $1 = $1[N] & mask\n"
								L"with N replaced by :\n"
								L" 1376 for gil amount(4)\n"
								L" 1384 for catched frog amount(2)\n"
								L" 1386 for successful steal amount(2)\n"
								L" 1388 for dragon slain amount(2)\n" },
};

#endif
