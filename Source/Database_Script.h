#ifndef _DATABASE_SCRIPT_H
#define _DATABASE_SCRIPT_H

// ToDo: implement "command_tags.cs" infos

#include "Database_Common.h"

struct SortedChoiceItemScriptOpcode {
	unsigned int id = 0;
	wstring label = L"";
	wstring help = L"";
	bool use_vararg = false;
	int arg_amount = 0;
	uint8_t* arg_length = NULL;
	wstring* arg_help = NULL;
	uint8_t* arg_type = NULL;
	unsigned int jump_pos = 0;
};

//====================================//
//             Game Script            //
//====================================//

#define FIELD_ENDING_ID		16000
#define FIELD_ENDING_NAME	L"SPECIAL: End Game"

extern map<wstring, int> SCRIPT_ARG_TYPE_MAP;

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
#define AT_BUTTONLIST		44
#define AT_COLOR_CYAN		50
#define AT_COLOR_MAGENTA	51
#define AT_COLOR_YELLOW		52
#define AT_COLOR_RED		53
#define AT_COLOR_GREEN		54
#define AT_COLOR_BLUE		55
#define AT_BATTLE_CHARACTER	100
extern map<unsigned int, SortedChoiceItemScriptOpcode> HADES_STRING_SCRIPT_OPCODE;

struct VariableOperation {
	// Operation types:
	// -1: terminate
	// 0: unary operator pre-fixed
	// 1: unary operator post-fixed
	// 2: binary operator
	// 3: array operator
	// 5: get constant char
	// 6: get constant short
	// 7: get constant long
	// 10: get variable (char) (array byte: 1)
	// 11: get variable (short) (array byte: 1)
	// 12: get variable (long) (array byte: 1)
	// 13: get variable (bit) (array byte: 1)
	// 19: get variable (null) (array byte: 1)
	// 20: get variable (char) (array byte: 2)
	// 21: get variable (short) (array byte: 2)
	// 22: get variable (long) (array byte: 2)
	// 23: get variable (bit) (array byte: 2)
	// 29: get variable (null) (array byte: 2)
	// 50: function (1 arg)
	// 51: function (2 args)
	// 55: GetEntry functions
	// 60: custom function
	// 100: unknown/unused
	int type;
	wstring opstring;
	wstring description;
};

extern vector<VariableOperation> VarOpList;
extern vector<VariableOperation> VarEntryPropList;
extern map<uint8_t, int> VarBinaryOpPrecedence;

struct VariableOperationArgType {
	uint8_t op;
	uint8_t type;
};

// For operations with 2 arguments, put 2nd argument type then 1st argument type
extern vector<VariableOperationArgType> VarOpTypeList;

#define ARRAY_ADDITIONAL_INFO	L"\n\nAdditional informations:\n"\
								L"[PREVENT_ATTACK]\n"\
								L"If set to 1, it cancels all the character's awaiting attacks (selected using an Attack call or selected by the player). It doesn't empty the ATB.\n\n"\
								L"[MAX_HP/MP]\n"\
								L"The modifications of Max HP/MP are not reversed at the end of the battle for player's character. The modifications of the other stats (strength, magic...) are reversed.\n\n"\
								L"[ROW/COLUMN]\n"\
								L"The row is 0/1 for front/back row while the column is an integer from 0 to 3, sorting the characters from right to left.\n\n"\
								L"[MODEL_TYPE]\n"\
								L"Zidane: 0 (dagger) or 1 (sword)\n"\
								L"Vivi: 2\n"\
								L"Dagger: 3 (long hair rod), 4 (long hair racket), 5 (short hair rod) or 6 (short hair racket)\n"\
								L"Steiner: 7 or 8 (\"indoor\", dummied)\n"\
								L"Quina: 9\n"\
								L"Eiko: 10 (flute) or 11 (racket)\n"\
								L"Freya: 12\n"\
								L"Amarant: 13\n"\
								L"Cinna: 14\n"\
								L"Marcus: 15\n"\
								L"Blank: 16 (normal) or 17 (pluto)\n"\
								L"Beatrix: 18\n\n"\
								L"[CATEGORY]\n"\
								L"1 : male\n"\
								L"2 : female\n"\
								L"4 : gaian\n"\
								L"8 : terran\n"\
								L"16: temporary character\n\n"\
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

struct FlexibleFunctionName {
	uint16_t id;
	uint8_t argcount;
	wstring name;
	wstring description;
};

// AttackType:
// -- Physical attack: 1,3,6,8,9,10,11,15,25,26,27,28,29,30,31,32,33,52
// -- Magic attack: 16,17,18,19,20,21,22,23,24,34

#define VARCODE_SHARED			0x79
#define VARCODE_ENGINE			0x7A
#define VARCODE_BOOL			0xC4
#define VARCODE_INT24			0xC8
#define VARCODE_UINT24			0xCC
#define VARCODE_INT8			0xD0
#define VARCODE_CUSTOM_FUNCTION	0xD3
#define VARCODE_UINT8			0xD4
#define VARCODE_INT16			0xD8
#define VARCODE_UINT16			0xDC
#define VARCODE_CUSTOM_GETTER	0xE3
#define VARCODE_LBOOL			0xE4
#define VARCODE_LINT24			0xE8
#define VARCODE_LUINT24			0xEC
#define VARCODE_LINT8			0xF0
#define VARCODE_LUINT8			0xF4
#define VARCODE_LINT16			0xF8
#define VARCODE_LUINT16			0xFC

extern vector<VariableName> VarNameList;

#define HADES_STRING_FLEXIBLE_FUNCTION_DEFAULT_NAME	"MemoriaVarCode_"
extern map<int, FlexibleFunctionName> FlexibleFunctionNameList;

extern vector<wstring> FunctionTypeName;
extern vector<wstring> WorldFunctionTriggerType;
extern vector<wstring> DiscName;
extern vector<wstring> PlaystationButton;
extern vector<wstring> BattleState;
extern vector<wstring> MenuType;
extern vector<wstring> WindowFlags;
extern vector<wstring> AnimationFlags;
extern vector<wstring> AbilitySetName;
extern vector<wstring> BattleParamName;
extern vector<wstring> BubbleSymbolName;
extern vector<wstring> CommandAddendaName;

struct ConstantNames {
	uint16_t id;
	wstring name;
};

extern vector<ConstantNames> ScriptFunctionType;
extern vector<ConstantNames> ScriptCharacterField;
extern vector<ConstantNames> EquipSetName;
extern vector<ConstantNames> FMVNameList;
extern vector<ConstantNames> BattleCodeName;
extern vector<ConstantNames> ModelCodeName;
extern vector<ConstantNames> WorldCodeName;
extern vector<ConstantNames> SoundCodeName;
extern vector<ConstantNames> SpsCodeName;

#endif
