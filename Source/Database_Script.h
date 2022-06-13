#ifndef _DATABASE_SCRIPT_H
#define _DATABASE_SCRIPT_H

// ToDo: implement "command_tags.cs" infos

#include "Database_Common.h"

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

//====================================//
//             Game Script            //
//====================================//

#define FIELD_ENDING_ID		16000
#define FIELD_ENDING_NAME	L"SPECIAL: End Game"

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
// Should always be sorted by id
static SortedChoiceItemScriptOpcode HADES_STRING_SCRIPT_OPCODE[] = {
	{ 0x00, L"NOTHING", L"Do nothing.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x01, L"JMP", L"Skip some operations.\nWARNING: unsafe to use.", false, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Jump" }, new uint8_t[1]{ AT_JUMP }, 3 },
	{ 0x02, L"JMP_IFN", L"Skip some operations if the stack value is not 0.\nWARNING: unsafe to use.", false, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Jump" }, new uint8_t[1]{ AT_JUMP }, 3 },
	{ 0x03, L"JMP_IF", L"Skip some operations if the stack value is 0.\nWARNING: unsafe to use.", false, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Jump" }, new uint8_t[1]{ AT_JUMP }, 3 },
	{ 0x04, L"return", L"End the function.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x05, L"set", L"Perform variable operations and store the result in the stack.", false, 1, new uint8_t[1]{ 0 }, new wstring[1]{ L"Variable Code" }, new uint8_t[1]{ AT_NONE }, 0 },
	{ 0x06, L"JMP_SWITCHEX", L"Skip some operations depending on the stack value.\nWARNING: unsafe to use.", false, -1, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Default Jump", L"Case", L"Jump" }, new uint8_t[3]{ AT_JUMP, AT_SPIN, AT_JUMP }, 0 },
	{ 0x07, L"InitCode", L"Init a normal code (independant functions).\n\n1st argument: code entry to init.\n2nd argument: Unique ID (defaulted to entry's ID if 0).", false, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"UID" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x08, L"InitRegion", L"Init a region code (associated with a region).\n\n1st argument: code entry to init.\n2nd argument: Unique ID (defaulted to entry's ID if 0).", false, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"UID" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x09, L"InitObject", L"Init an object code (associated with a 3D model). Also load the related model into the RAM.\n\n1st argument: code entry to init.\n2nd argument: Unique ID (defaulted to entry's ID if 0).", false, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"UID" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x0A, L"0x0A", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x0B, L"JMP_SWITCH", L"Skip some operations depending on the stack value.\nWARNING: unsafe to use.", false, -1, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Starting Value", L"Default Jump", L"Jump" }, new uint8_t[3]{ AT_USPIN, AT_JUMP, AT_JUMP }, 1 },
	{ 0x0C, L"0x0C", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x0D, L"0x0D", L"Unknown Opcode.", false, 0, NULL, NULL, NULL, 0 }, // Steam seems to handle it like a JMP_SWITCH with a short instead of a char (number of cases)
	{ 0x0E, L"0x0E", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x0F, L"0x0F", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x10, L"RunScriptAsync", L"Run script function and continue executing the current one.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument: script level.\n2nd argument: entry of the function.\n3rd argument: function.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Script Level", L"Entry", L"Function" }, new uint8_t[3]{ AT_SCRIPTLVL, AT_ENTRY, AT_FUNCTION }, 0 },
	{ 0x11, L"0x11", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x12, L"RunScript", L"Wait until the entry's script level gets higher than the specified script level then run the script function and continue executing the current one.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument: script level.\n2nd argument: entry of the function.\n3rd argument: function.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Script Level", L"Entry", L"Function" }, new uint8_t[3]{ AT_SCRIPTLVL, AT_ENTRY, AT_FUNCTION }, 0 },
	{ 0x13, L"0x13", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x14, L"RunScriptSync", L"Wait until the entry's script level gets higher than the specified script level then run the script function and wait until it returns.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument: script level.\n2nd argument: entry of the function.\n3rd argument: function.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Script Level", L"Entry", L"Function" }, new uint8_t[3]{ AT_SCRIPTLVL, AT_ENTRY, AT_FUNCTION }, 0 },
	{ 0x15, L"0x15", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x16, L"RunScriptObjectAsync", L"Run script function and continue executing the current one. Must only be used in response to a function call ; the argument's entry is the one that called this function.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument: script level.\n2nd argument: function.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Script Level", L"Function" }, new uint8_t[2]{ AT_SCRIPTLVL, AT_FUNCTION }, 0 },
	{ 0x17, L"0x17", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x18, L"RunScriptObject", L"Wait until the entry's script level gets higher than the specified script level then run the script function and continue executing the current one. Must only be used in response to a function call ; the argument's entry is the one that called this function.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument: script level.\n2nd argument: function.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Script Level", L"Function" }, new uint8_t[2]{ AT_SCRIPTLVL, AT_FUNCTION }, 0 },
	{ 0x19, L"0x19", L"Unused Opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x1A, L"RunScriptObjectSync", L"Wait until the entry's script level gets higher than the specified script level then run the script function and wait until it returns. Must only be used in response to a function call ; the argument's entry is the one that called this function.\n\nEntry's script level is 0 until its main function returns, then it becomes 7. If the specified script level is higher than the entry's script level, the function is not run. Otherwise, the entry's script level is set to the specified script level until the function returns.\n\n1st argument: script level.\n2nd argument: function.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Script Level", L"Function" }, new uint8_t[2]{ AT_SCRIPTLVL, AT_FUNCTION }, 0 },
	{ 0x1B, L"ContinueBattleMusic", L"Continue the music after the battle end.\n\n1st argument: flag continue/don't continue.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Continue" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0x1C, L"TerminateEntry", L"Stop the execution of an entry's code.\n\n1st argument: entry to terminate.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x1D, L"CreateObject", L"Place (or replace) the 3D model on the field.\n\n1st and 2nd arguments: position in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Position" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x1E, L"SetCameraBounds", L"Redefine the field camera boundaries (default value is part of the background's data).\n\n1st argument: camera ID.\n2nd to 5th arguments: Boundaries in (MinX, MaxX, MinY, MaxY) format.", true, 5, new uint8_t[5]{ 1, 2, 2, 2, 2 }, new wstring[5]{ L"Camera", L"Min X", L"Max X", L"Min Y", L"Max Y" }, new uint8_t[5]{ AT_USPIN, AT_SPIN, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x1F, L"WindowSync", L"Display a window with text inside and wait until it closes.\n\n1st argument: window ID.\n2nd argument: UI flag list.\n 3: disable bubble tail\n 4: mognet format\n 5: hide window\n 7: ATE window\n 8: dialog window\n3rd argument: text to display.", true, 3, new uint8_t[3]{ 1, 1, 2 }, new wstring[3]{ L"Window ID", L"UI", L"Text" }, new uint8_t[3]{ AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x20, L"WindowAsync", L"Display a window with text inside and continue the execution of the script without waiting.\n\n1st argument: window ID.\n2nd argument: UI flag list.\n 3: disable bubble tail\n 4: mognet format\n 5: hide window\n 7: ATE window\n 8: dialog window\n3rd argument: text to display.", true, 3, new uint8_t[3]{ 1, 1, 2 }, new wstring[3]{ L"Window ID", L"UI", L"Text" }, new uint8_t[3]{ AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x21, L"CloseWindow", L"Close a window.\n\n1st argument: window ID determined at its creation.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Window ID" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x22, L"Wait", L"Wait some time.\n\n1st argument: amount of frames to wait.\n For PAL, 1 frame is 0.04 seconds.\n For other versions, 1 frame is about 0.033 seconds.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Frame Amount" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x23, L"Walk", L"Make the character walk to destination. Make it synchronous if InitWalk is called before.\n\n1st and 2nd arguments: destination in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x24, L"WalkTowardObject", L"Make the character walk and follow an object.\n\n1st argument: object to walk toward.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x25, L"InitWalk", L"Make a further Walk call (or variations of Walk) synchronous.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x26, L"SetWalkSpeed", L"Change the walk speed.\n\n1st argument: speed (surely in unit/frame).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x27, L"SetTriangleFlagMask", L"Set a bitmask for some of the walkmesh triangle flags.\n\n1st argument: flag mask.\n 7: disable restricted triangles\n 8: disable player-restricted triangles", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 }, // BGIMASK
	{ 0x28, L"Cinematic", L"Run or setup a cinematic.\n\n1st argument: unknown.\n2nd argument: cinematic ID (may depends on 1st argument's value).\n3rd argument: unknown.\n4th argument: unknown.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Unknown", L"Cinematic ID", L"Unknown", L"Unknown" }, new uint8_t[4]{ AT_SPIN, AT_FMV, AT_SPIN, AT_SPIN }, 0 },
	{ 0x29, L"SetRegion", L"Define the polygonal region linked with the entry script. If the polygon is not convex, its convex hull is used instead.\n\nArguments are in the format (Vertex X, Vertex Y) and there can be any number of them.", true, -1, new uint8_t[1]{ 4 }, new wstring[1]{ L"Polygon" }, new uint8_t[1]{ AT_NONE }, 0 },
	{ 0x2A, L"Battle", L"Start a battle (using a random enemy group).\n\n1st argument: rush type (unknown).\n2nd argument: gathered battle and Steiner's state (highest bit) informations.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Rush Type", L"Battle" }, new uint8_t[2]{ AT_SPIN, AT_BATTLE }, 0 },
	{ 0x2B, L"Field", L"Change the field scene.\n\n1st argument: field scene destination.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Field" }, new uint8_t[1]{ AT_FIELD }, 0 },
	{ 0x2C, L"DefinePlayerCharacter", L"Apply the player's control over the entry's object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x2D, L"DisableMove", L"Disable the player's movement control.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x2E, L"EnableMove", L"Enable the player's movement control.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x2F, L"SetModel", L"Set the model of the object and its head's height (used to set the dialog box's height).\n\n1st argument: model.\n2nd argument: head's height.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Model", L"Height" }, new uint8_t[2]{ AT_MODEL, AT_USPIN }, 0 },
	{ 0x30, L"0x30", L"Unknown Opcode (PRINT1); ignored in the non-PSX versions.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0x31, L"0x31", L"Unknown Opcode (PRINTF); ignored in the non-PSX versions.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0x32, L"0x32", L"Unused Opcode (LOCATE).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x33, L"SetStandAnimation", L"Change the standing animation.\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x34, L"SetWalkAnimation", L"Change the walking animation.\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x35, L"SetRunAnimation", L"Change the running animation.\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x36, L"TurnInstant", L"Make the character face an angle.\n\n1st argument: angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Angle" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x37, L"SetPitchAngle", L"Turn the model in the up/down direction.\n\n1st argument: angle (pitch axis).\n2nd argument: angle (XZ axis).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Pitch", L"XZ Angle" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x38, L"Attack", L"Make the enemy attack. The target(s) are to be set using the SV_Target variable.\nInside an ATB function, the attack is added to the queue.\nInside a counter function, the attack occurs directly.\n\n1st argument: attack to perform.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Attack" }, new uint8_t[1]{ AT_ATTACK }, 0 },
	{ 0x39, L"ShowObject", L"Show specific meshes of an object.\n\n1st argument: object.\n2nd argument: mesh list to show.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Mesh list" }, new uint8_t[2]{ AT_ENTRY, AT_BOOLLIST }, 0 },
	{ 0x3A, L"HideObject", L"Hide specific meshes of an object.\n\n1st argument: object.\n2nd argument: mesh list to hide.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Mesh list" }, new uint8_t[2]{ AT_ENTRY, AT_BOOLLIST }, 0 },
	{ 0x3B, L"SetObjectIndex", L"Redefine the current object's index.\n\n1st argument: new index.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Index" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x3C, L"SetRandomBattles", L"Define random battles.\n\n1st argument: pattern, deciding the encounter chances and the topography (World Map only).\n 0: {0.375, 0.28, 0.22, 0.125}\n 1: {0.25, 0.25, 0.25, 0.25}\n 2: {0.35, 0.3, 0.3, 0.05}\n 3: {0.45, 0.4, 0.1, 0.05}\n2nd to 5th arguments: possible random battles.", true, 5, new uint8_t[5]{ 1, 2, 2, 2, 2 }, new wstring[5]{ L"Pattern", L"Battle 1", L"Battle 2", L"Battle 3", L"Battle 4" }, new uint8_t[5]{ AT_USPIN, AT_BATTLE, AT_BATTLE, AT_BATTLE, AT_BATTLE }, 0 },
	{ 0x3D, L"SetAnimationInOut", L"Specify the starting and ending animation frames of the character.\n\n1st argument: starting frame.\n2nd argument: ending frame (capped to the animation's duration).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"In-Frame",  L"Out-Frame" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x3E, L"SetAnimationSpeed", L"Set the current object's animation speed.\n\n1st argument: speed.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x3F, L"SetAnimationFlags", L"Set the current object's next animation looping flags.\n\n1st argument: looping flag list.\n 1: freeze at end\n 2: loop\n 3: loop back and forth\n2nd arguments: times to repeat.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Flag", L"Repeat" }, new uint8_t[2]{ AT_ANIMFLAG, AT_USPIN }, 0 },
	{ 0x40, L"RunAnimation", L"Make the character play an animation.\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x41, L"WaitAnimation", L"Wait until the current object's animation has ended.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x42, L"StopAnimation", L"Stop the character's animation.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x43, L"RunSharedScript", L"Run script passing the current object to it and continue executing the current function. If another shared script is already running for this object, it will be terminated.\n\n1st argument: entry (should be a one-function entry).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Entry" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x44, L"WaitSharedScript", L"Wait until the ran shared script has ended.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x45, L"StopSharedScript", L"Terminate the execution of the ran shared script.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x46, L"0x46", L"No use (DEBUGCC).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x47, L"EnableHeadFocus", L"Enable or disable the character turning his head toward an active object.\n\n1st argument: flags.\n 1: enable\n 2: turn toward close characters\n 3: turn toward talkers", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0x48, L"AddItem", L"Add item to the player's inventory. Only one copy of key items can be in the player's inventory.\n\n1st argument: item to add.\n2nd argument: amount to add.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Item", L"Amount" }, new uint8_t[2]{ AT_ITEM, AT_USPIN }, 0 },
	{ 0x49, L"RemoveItem", L"Remove item from the player's inventory.\n\n1st argument: item to remove.\n2nd argument: amount to remove.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Item", L"Amount" }, new uint8_t[2]{ AT_ITEM, AT_USPIN }, 0 },
	{ 0x4A, L"RunBattleCode", L"Run a battle code.\n\n1st argument: battle code.\n2nd argument: depends on the battle code.\n End Battle: 0 for a defeat (deprecated), 1 for a victory, 2 for a victory without victory pose, 3 for a defeat, 4 for an escape, 5 for an interrupted battle, 6 for a game over, 7 for an enemy escape.\n Run Camera: Camera ID.\n Change Field: ID of the destination field after the battle.\n Add Gil: amount to add.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Code", L"Argument" }, new uint8_t[2]{ AT_BATTLECODE, AT_SPIN }, 0 },
	{ 0x4B, L"SetObjectLogicalSize", L"Set different size informations of the object.\n\n1st argument: size for pathing.\n2nd argument: collision radius.\n3rd argument: talk distance.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Walk Radius", L"Collision Radius", L"Talk Distance" }, new uint8_t[3]{ AT_SPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0x4C, L"AttachObject", L"Attach an object to another one.\n\n1st argument: carried object.\n2nd argument: carrying object.\n3rd argument: attachment point (unknown format).", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Object", L"Carrier", L"Attachement Point" }, new uint8_t[3]{ AT_ENTRY, AT_ENTRY, AT_USPIN }, 0 },
	{ 0x4D, L"DetachObject", L"Stop attaching an object to another one.\n\n1st argument: carried object.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x4E, L"0x4E", L"Unknown Opcode (WATCH).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x4F, L"0x4F", L"Unknown Opcode (STOP).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0x50, L"WaitTurn", L"Wait until the character has turned.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x51, L"TurnTowardObject", L"Turn the character toward an entry object (animated).\n\n1st argument: object.\n2nd argument: turn speed (1 is slowest).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Speed" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x52, L"SetInactiveAnimation", L"Change the animation played when inactive for a long time. The inaction time required is:\nFirst Time = 200 + 4 * Random[0, 255]\nSubsequent Times = 200 + 2 * Random[0, 255]\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x53, L"PreventWindowInit", L"Seems to prevent new dialog windows to close older ones.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x54, L"WaitWindow", L"Wait until the window is closed.\n\n1st argument: window ID determined at its creation.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Window ID" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x55, L"SetWalkTurnSpeed", L"Change the turn speed of the object when it walks or runs (default is 16).\n\n1st argument: turn speed (with 0, the object doesn't turn while moving).\n\nSpecial treatments:\nVivi's in Iifa Tree/Eidolon Moun (field 1656) is initialized to 48.\nChoco's in Chocobo's Paradise (field 2954) is initialized to 96.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Turn Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x56, L"TimedTurn", L"Make the character face an angle (animated).\n\n1st argument: angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.\n2nd argument: turn speed (1 is slowest).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Angle", L"Speed" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x57, L"SetRandomBattleFrequency", L"Set the frequency of random battles.\n\n1st argument: frequency.\n 255 is the maximum frequency, corresponding to ~12 walking steps or ~7 running steps.\n 0 is the minimal frequency and disables random battles.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Frequency" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x58, L"SlideXZY", L"Make the character slide to destination (walk without using the walk animation and without changing the facing angle).\n\n1st to 3rd arguments: destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0x59, L"SetTileColor", L"Change the color of a field tile block.\n\n1st argument: background tile block.\n2nd to 4th arguments: color in (Cyan, Magenta, Yellow) format.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[2]{ L"Tile Block", L"Color" }, new uint8_t[4]{ AT_TILE, AT_COLOR_CYAN, AT_COLOR_MAGENTA, AT_COLOR_YELLOW }, 0 },
	{ 0x5A, L"SetTilePositionEx", L"Move a field tile block.\n\n1st argument: background tile block.\n2nd and 3rd argument: movement in (dX, dY) format.\n4th argument: depth, with higher value being further away from camera.", true, 4, new uint8_t[4]{ 1, 2, 2, 2 }, new wstring[4]{ L"Tile Block", L"Position X", L"Position Y", L"Position Closeness" }, new uint8_t[4]{ AT_TILE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5B, L"ShowTile", L"Show or hide a field tile block.\n\n1st argument: background tile block.\n2nd argument: boolean show/hide.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Block", L"Show" }, new uint8_t[2]{ AT_TILE, AT_BOOL }, 0 },
	{ 0x5C, L"MoveTileLoop", L"Make the image of a field tile loop over space.\n\n1st argument: background tile block.\n2nd argument: boolean on/off.\n3rd and 4th arguments: speed in the X and Y directions.", true, 4, new uint8_t[4]{ 1, 1, 2, 2 }, new wstring[4]{ L"Tile Block", L"Activate", L"X Loop", L"Y Loop" }, new uint8_t[4]{ AT_TILE, AT_BOOL, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5D, L"MoveTile", L"Make the field moves depending on the camera position.\n\n1st argument: background tile block.\n2nd argument: boolean on/off.\n3rd and 4th arguments: parallax movement in (X, Y) format.", true, 4, new uint8_t[4]{ 1, 1, 2, 2 }, new wstring[4]{ L"Tile Block", L"Activate", L"Movement X", L"Movement Y" }, new uint8_t[4]{ AT_TILE, AT_BOOL, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5E, L"SetTilePosition", L"Move a field tile block.\n\n1st argument: background tile block.\n2nd and 3rd argument: movement in (dX, dY) format.", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Tile Block", L"Position X", L"Position Y" }, new uint8_t[3]{ AT_TILE, AT_SPIN, AT_SPIN }, 0 },
	{ 0x5F, L"RunTileAnimation", L"Run a field tile animation.\n\n1st argument: background animation.\n2nd argument: starting frame.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Field Animation", L"Frame" }, new uint8_t[2]{ AT_TILEANIM, AT_USPIN }, 0 },
	{ 0x60, L"ActivateTileAnimation", L"Make a field tile animation active.\n\n1st argument: background animation.\n2nd argument: boolean on/off.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Animation", L"Activate" }, new uint8_t[2]{ AT_TILEANIM, AT_BOOL }, 0 },
	{ 0x61, L"SetTileAnimationSpeed", L"Change the speed of a field tile animation.\n\n1st argument: background animation.\n2nd argument: speed (256 = 1 tile/frame).", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Tile Animation", L"Speed" }, new uint8_t[2]{ AT_TILEANIM, AT_SPIN }, 0 },
	{ 0x62, L"SetRow", L"Change the battle row of a party member.\n\n1st argument: party member.\n2nd argument: boolean front/back.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Row" }, new uint8_t[2]{ AT_LCHARACTER, AT_BOOL }, 0 },
	{ 0x63, L"SetTileAnimationPause", L"Make a field tile animation pause at some frame in addition to its normal animation speed.\n\n1st argument: background animation.\n2nd argument: animation frame.\n3rd argument: wait time.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Tile Animation", L"Frame ID", L"Time" }, new uint8_t[3]{ AT_TILEANIM, AT_USPIN, AT_USPIN }, 0 },
	{ 0x64, L"SetTileAnimationFlags", L"Add flags of a field tile animation.\n\n1st argument: background animation.\n2nd argument: flags (only the flags 5 and 6 can be added).\n 5: unknown\n 6: loop back and forth", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Animation", L"Flags" }, new uint8_t[2]{ AT_TILEANIM, AT_BOOLLIST }, 0 },
	{ 0x65, L"RunTileAnimationEx", L"Run a field tile animation and choose its frame range.\n\n1st argument: background animation.\n2nd argument: starting frame.\n3rd argument: ending frame.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Tile Animation", L"Start", L"End" }, new uint8_t[3]{ AT_TILEANIM, AT_USPIN, AT_USPIN }, 0 },
	{ 0x66, L"SetTextVariable", L"Set the value of a text number or item variable.\n\n1st argument: text variable's \"Script ID\".\n2nd argument: depends on which text opcode is related to the text variable.\n For [VAR_NUM]: integral value.\n For [VAR_ITEM]: item ID.\n For [VAR_TOKEN]: token number.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Variable ID", L"Value" }, new uint8_t[2]{ AT_USPIN, AT_ITEM }, 0 },
	{ 0x67, L"SetControlDirection", L"Set the angles for the player's movement control.\n\n1st argument: angle used for arrow movements.\n2nd argument: angle used for analogic stick movements.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Arrow Angle", L"Analogic Angle" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x68, L"Bubble", L"Display a speech bubble with a symbol inside over the head of player's character.\n\n1st argument: bubble symbol.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Symbol" }, new uint8_t[1]{ AT_BUBBLESYMBOL }, 0 },
	{ 0x69, L"ChangeTimerTime", L"Change the remaining time of the timer window.\n\n1st argument: time in seconds.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Time" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x6A, L"DisableRun", L"Make the player's character always walk.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6B, L"SetBackgroundColor", L"Change the default color, seen behind the field's tiles.\n\n1st to 3rd arguments: color in (Red, Green, Blue) format.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[1]{ L"Color" }, new uint8_t[3]{ AT_COLOR_RED, AT_COLOR_GREEN, AT_COLOR_BLUE }, 0 },
	{ 0x6C, L"0x6C", L"Unknown Opcode (PPRINT).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6D, L"0x6D", L"Unknown Opcode (PPRINTF).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6E, L"0x6E", L"Unknown Opcode (MAPID).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x6F, L"MoveCamera", L"Move camera over time.\n\n1st and 2nd arguments: destination in (X, Y) format.\n3nd argument: movement duration.\n4th argument: scrolling type (8 for sinusoidal, other values for linear interpolation).", true, 4, new uint8_t[4]{ 2, 2, 1, 1 }, new wstring[3]{ L"Destination", L"Time", L"Smoothness" }, new uint8_t[4]{ AT_POSITION_X, AT_POSITION_Y, AT_USPIN, AT_USPIN }, 0 }, // screen size = 320?
	{ 0x70, L"ReleaseCamera", L"Release camera movement, getting back to its normal behaviour.\n\n1st argument: duration of the repositioning.\n2nd argument: scrolling type (8 for sinusoidal, other values for linear interpolation).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Time", L"Smoothness" }, new uint8_t[2]{ AT_SPIN, AT_USPIN }, 0 },
	{ 0x71, L"EnableCameraServices", L"Enable or disable camera services. When disabling, the 2nd and 3rd arguments are ignored.\n\n1st argument: boolean activate/deactivate.\n2nd argument: duration of the repositioning when activating (defaulted to 30 if -1 is given).\n3rd argument: scrolling type of the repositioning when activating (8 for sinusoidal, other values for linear interpolation).", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Enable", L"Time", L"Smoothness" }, new uint8_t[3]{ AT_BOOL, AT_SPIN, AT_USPIN }, 0 },
	{ 0x72, L"SetCameraFollowHeight", L"Define the standard height gap between the player's character position and the camera view.\n\n1st argument: height.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Height" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0x73, L"EnableCameraFollow", L"Make the camera follow the player's character.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x74, L"DisableCameraFollow", L"Stop making the camera follow the player's character.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x75, L"Menu", L"Open a menu.\n\n1st argument: menu type.\n2nd argument: depends on the menu type.\n Naming Menu: character to name.\n Shop Menu: shop ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Menu Type", L"Menu" }, new uint8_t[2]{ AT_MENUTYPE, AT_MENU }, 0 },
	{ 0x76, L"DrawRegionStart", L"Start drawing the convex polygonal region linked with the entry script: two starting points are placed at the same position.\n\n1st and 2nd arguments: starting vertex position in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Vertex Position" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x77, L"DrawRegionSetLast", L"Change the position of the convex polygonal region's ending vertex.\n\n1st and 2nd arguments: new position in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Vertex Position" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x78, L"DrawRegionPushNew", L"Add a new vertex to the convex polygonal region linked with the entry script (defaulting its position to be the same as the current ending vertex's position). This method cannot exceed 8 vertices.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x79, L"0x79", L"Unknown Opcode (PRINTQUAD).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x7A, L"SetLeftAnimation", L"Change the left turning animation.\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x7B, L"SetRightAnimation", L"Change the right turning animation.\n\n1st argument: animation ID.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Animation" }, new uint8_t[1]{ AT_ANIMATION }, 0 },
	{ 0x7C, L"EnableDialogChoices", L"Define choices availability in dialogs using the [INIT_MULTICHOICE] text opcode.\n\n1st argument: boolean list for the different choices.\n2nd argument: default choice selected.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Choice List", L"Default Choice" }, new uint8_t[2]{ AT_BOOLLIST, AT_USPIN }, 0 },
	{ 0x7D, L"RunTimer", L"Run or pause the timer window.\n\n1st argument: boolean run/pause.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Run" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0x7E, L"SetFieldCamera", L"Change the field's background camera.\n\n1st argument: camera ID.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Camera ID" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x7F, L"EnableShadow", L"Enable the shadow for the entry's object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x80, L"DisableShadow", L"Disable the shadow for the entry's object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x81, L"SetShadowSize", L"Set the entry's object shadow size.\n\n1st argument: size X.\n2nd argument: size Y.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Size X", L"Size Y" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x82, L"SetShadowOffset", L"Change the offset between the entry's object and its shadow.\n\n1st argument: offset X.\n2nd argument: offset Y.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[2]{ L"Offset X", L"Offset Y" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x83, L"LockShadowRotation", L"Stop updating the shadow rotation by the object's rotation.\n\n1st argument: locked rotation.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Locked Rotation" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0x84, L"UnlockShadowRotation", L"Make the shadow rotate accordingly with its object.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x85, L"SetShadowAmplifier", L"Amplify or reduce the shadow transparancy.\n\n1st argument: amplification factor.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Amplification Factor" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x86, L"SetAnimationStandSpeed", L"Change the standing animation speed.\n\n1st to 4th arguments: different animation speeds (default is 16), picked at random.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"1st speed", L"2nd speed", L"3rd speed", L"4th speed" }, new uint8_t[4]{ AT_USPIN, AT_USPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0x87, L"TurnInstantEx", L"Make the specified character face an angle.\n\n1st argument: object to turn.\n2nd argument: angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Entry", L"Angle" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0x88, L"RunModelCode", L"Run a model code.\n\n1st argument: model code.\n2nd to 4th arguments: depends on the model code.\n Texture Blend (blend mode) with blend mode being 1 for screen, 2 for multiply, 4 for Soft and 255 for a solid texture\n Slice (boolean slice/unslice, value)\n Enable Mirror (boolean enable/disable)\n Mirror Position (X, Z, Y)\n Mirror Normal (X, Z, Y)\n Mirror Color (Red, Green, Blue)\n Sound codes (Animation, Frame, Value)\n  For Add (Secondary) Sound, the 3rd argument is the sound ID\n  For Sound Random Pitch, the 3rd argument is a boolean random/not random\n  For Remove Sound, the 3rd argument is unused", true, 4, new uint8_t[4]{ 1, 2, 2, 2 }, new wstring[4]{ L"Model Code", L"Argument 1", L"Argument 2", L"Argument 3" }, new uint8_t[4]{ AT_MODELCODE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x89, L"SetSoundPosition", L"Set the position of a 3D sound.\n\n1st to 3rd arguments: sound position.\n4th argument: sound volume.", true, 4, new uint8_t[4]{ 2, 2, 2, 1 }, new wstring[2]{ L"Position", L"Volume" }, new uint8_t[4]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y, AT_SPIN }, 0 },
	{ 0x8A, L"SetSoundObjectPosition", L"Set the position of a 3D sound to the object's position.\n\n1st argument: object.\n2nd argument: sound volume.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Volume" }, new uint8_t[2]{ AT_ENTRY, AT_SPIN }, 0 },
	{ 0x8B, L"SetHeadFocusMask", L"Define a mask for characters to turn their head toward close characters: if this character's TargetMask has a common bit with another character's SelfMask, then he can turn to that other character.\nAlso enables the two EnableHeadFocus flags \"enable\" and \"turn toward close characters\".\nCharacters must be at least 31 units away to have head focus and at most 2000 units away (the closest character is determined both by the distance and the angle).\n\n1st argument: SelfMask.\n2nd argument: TargetMask.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"SelfMask", L"TargetMask" }, new uint8_t[2]{ AT_BOOLLIST, AT_BOOLLIST }, 0 },
	{ 0x8C, L"BattleEx", L"Start a battle and choose its battle group.\n\n1st argument: rush type (unknown).\n2nd argument: group.\n3rd argument: gathered battle and Steiner's state (highest bit) informations.", true, 3, new uint8_t[3]{ 1, 1, 2 }, new wstring[3]{ L"Rush Type", L"Battle Group", L"Battle" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_BATTLE }, 0 },
	{ 0x8D, L"ShowTimer", L"Activate the timer window.\n\n1st argument: boolean show/hide.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Enable" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0x8E, L"RaiseWindows", L"Make all the dialogs and windows display over the filters.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x8F, L"SetModelColor", L"Change a 3D model's color.\n\n1st argument: entry associated with the model.\n2nd to 4th arguments: color in (Red, Green, Blue) format.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[2]{ L"Object", L"Color" }, new uint8_t[4]{ AT_ENTRY, AT_COLOR_RED, AT_COLOR_GREEN, AT_COLOR_BLUE }, 0 },
	{ 0x90, L"DisableInactiveAnimation", L"Prevent player's character to play its inactive animation.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x91, L"FollowFocus", L"Automatically turn toward the character that is stared at when needed.\n\n1st argument: boolean on/off", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Enable" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0x92, L"AttachTile", L"Make a part of the field background follow the player's movements. Also apply a color filter out of that tile block's range.\n\n1st argument: tile block.\n2nd and 3rd arguments: offset position in (X, Y) format.\n4th argument: filter mode ; use -1 for no filter effect.\n5th to 7th arguments: filter color in (Red, Green, Blue) format.", true, 7, new uint8_t[7]{ 1, 2, 1, 1, 1, 1, 1 }, new wstring[5]{ L"Tile Block", L"Position X", L"Position Y", L"Filter Mode", L"Filter Color" }, new uint8_t[7]{ AT_TILE, AT_SPIN, AT_SPIN, AT_SPIN, AT_COLOR_RED, AT_COLOR_GREEN, AT_COLOR_BLUE }, 0 },
	{ 0x93, L"SetObjectFlags", L"Change flags of the current entry's object.\n\n1st argument: object flags.\n 1: show model\n 2: collision with player character\n 4: collision with NPC\n 8: disable talk\n 16: can't walk through by insisting\n 32: don't hide all\n 64: unknown/unused (can't change with this)\n 128: is turning (can't change with this)", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0x94, L"SetJumpAnimation", L"Change the jumping animation.\n\n1st argument: animation ID.\n2nd argument: unknown.\n3rd argument: unknown.", true, 3, new uint8_t[3]{ 2, 1, 1 }, new wstring[3]{ L"Animation", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_ANIMATION, AT_SPIN, AT_SPIN }, 0 },
	{ 0x95, L"WindowSyncEx", L"Display a window with text inside and wait until it closes.\n\n1st argument: talker's entry.\n2nd argument: window ID.\n3rd argument: UI flag list.\n 3: disable bubble tail\n 4: mognet format\n 5: hide window\n 7: ATE window\n 8: dialog window\n4th argument: text to display.", true, 4, new uint8_t[4]{ 1, 1, 1, 2 }, new wstring[4]{ L"Talker", L"Window ID", L"UI", L"Text" }, new uint8_t[4]{ AT_ENTRY, AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x96, L"WindowAsyncEx", L"Display a window with text inside and continue the execution of the script without waiting.\n\n1st argument: talker's entry.\n2nd argument: window ID.\n3rd argument: UI flag list.\n 3: disable bubble tail\n 4: mognet format\n 5: hide window\n 7: ATE window\n 8: dialog window\n4th argument: text to display.", true, 4, new uint8_t[4]{ 1, 1, 1, 2 }, new wstring[4]{ L"Talker", L"Window ID", L"UI", L"Text" }, new uint8_t[4]{ AT_ENTRY, AT_USPIN, AT_BOOLLIST, AT_TEXT }, 0 },
	{ 0x97, L"ReturnEntryFunctions", L"Make all the currently executed functions return for a given entry.\n\n1st argument: entry for which functions are returned.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Entry" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0x98, L"MakeAnimationLoop", L"Make current object's currently playing animation loop.\n\n1st argument: loop amount.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Amount" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x99, L"SetTurnSpeed", L"Change the entry's object turn speed.\n\n1st argument: turn speed (1 is slowest).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0x9A, L"EnablePathTriangle", L"Enable or disable a triangle of field pathing.\n\n1st argument: triangle ID.\n2nd argument: boolean enable/disable.", true, 2, new uint8_t[2]{ 2, 1 }, new wstring[2]{ L"Triangle ID", L"Enable" }, new uint8_t[2]{ AT_WALKTRIANGLE, AT_BOOL }, 0 },
	{ 0x9B, L"TurnTowardPosition", L"Turn the character toward a position (animated). The object's turn speed is used (default to 16).\n\n1st and 2nd arguments: coordinates in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Coordinate" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0x9C, L"RunJumpAnimation", L"Make the character play its jumping animation.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x9D, L"RunLandAnimation", L"Make the character play its landing animation (inverted jumping animation).", false, 0, NULL, NULL, NULL, 0 },
	{ 0x9E, L"ExitField", L"Make the player's character walk to the field exit and prepare to flush the field datas.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x9F, L"SetObjectSize", L"Set the size of a 3D model.\n\n1st argument: entry of the 3D model.\n2nd to 4th arguments: size ratio in (Ratio X, Ratio Z, Ratio Y) format. A ratio of 64 is the default size.", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Object", L"Size X", L"Size Z", L"Size Y" }, new uint8_t[4]{ AT_ENTRY, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xA0, L"WalkToExit", L"Make the entry's object walk to the field exit.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xA1, L"MoveInstantXZY", L"Instantatly move the object.\n\n1st to 3rd arguments: destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xA2, L"WalkXZY", L"Make the character walk to destination. Make it synchronous if InitWalk is called before.\n\n1st argument: destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xA3, L"0xA3", L"Unknown Opcode (DRADIUS).", true, 4, new uint8_t[4]{ 1, 1, 1, 1 }, new wstring[4]{ L"Unknown", L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[4]{ AT_SPIN, AT_SPIN, AT_SPIN, AT_SPIN }, 0 }, // Unused in Steam
	{ 0xA4, L"CalculateExitPosition", L"Calculate the field exit position based on the region's polygon.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xA5, L"Slide", L"Make the character slide to destination (walk without using the walk animation and without changing the facing angle).\n\n1st and 2nd arguments: destination in (X, Y) format.", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[1]{ L"Destination" }, new uint8_t[2]{ AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0xA6, L"SetRunSpeedLimit", L"Change the speed at which the character uses his run animation instead of his walk animation (default is 31).\n\n1st argument: speed limit.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Speed Limit" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xA7, L"Turn", L"Make the character face an angle (animated). Speed is defaulted to 16.\n\n1st argument: angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Angle" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xA8, L"SetPathing", L"Change the pathing of the character.\n\n1st argument: boolean pathing on/off.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Pathing" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0xA9, L"CalculateScreenPosition", L"Calculate the object's position in screen coordinates and store it in \"GetScreenCalculatedX\" and \"GetScreenCalculatedY\".", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xAA, L"EnableMenu", L"Enable menu access by the player.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xAB, L"DisableMenu", L"Disable menu access by the player.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xAC, L"ChangeDisc", L"Allow to save the game and change disc.\n\n1st argument: gathered field destination and disc destination.", true, 1, new uint8_t[1]{ 2 }, new wstring[2]{ L"Field Destination", L"Disc" }, new uint8_t[1]{ AT_DISC_FIELD }, 0 },
	{ 0xAD, L"MoveInstantXZYEx", L"Instantatly move an object.\n\n1st argument: object's entry.\n2nd to 4th arguments: destination in (X, Z, Y) format.", true, 4, new uint8_t[4]{ 1, 2, 2, 2 }, new wstring[4]{ L"Object", L"Destination" }, new uint8_t[4]{ AT_ENTRY, AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xAE, L"TetraMaster", L"Begin a card game.\n\n1st argument: card deck of the opponent.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Card Deck" }, new uint8_t[1]{ AT_DECK }, 0 },
	{ 0xAF, L"DeleteAllCards", L"Clear the player's Tetra Master's deck.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB0, L"SetFieldName", L"Change the name of the field.\n\n1st argument: new name (unknown format).", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Name" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xB1, L"ResetFieldName", L"Reset the name of the field.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB2, L"Party", L"Allow the player to change the members of its party.\n\n1st argument: party size (if characters occupy slots beyond it, they are locked).\n2nd argument: list of locked characters.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Party Size", L"Locked Characters" }, new uint8_t[2]{ AT_USPIN, AT_CHARACTER }, 0 },
	{ 0xB3, L"RunSPSCode", L"Run Sps code, which seems to be special model effects on the field.\n\n1st argument: sps ID.\n2nd argument: sps code.\n3rd to 5th arguments: depend on the sps code.\n Load Sps (sps type)\n Enable Attribute (attribute list, boolean enable/disable)\n Set Position (X, -Z, Y)\n Set Rotation (angle X, angle Z, angle Y)\n Set Scale (scale factor)\n Attach (object's entry to attach, bone number)\n Set Fade (fade)\n Set Animation Rate (rate)\n Set Frame Rate (rate)\n Set Frame (value) where the value is factored by 16 to get the frame\n Set Position Offset (X, -Z, Y)\n Set Depth Offset (depth)", true, 5, new uint8_t[5]{ 1, 1, 2, 2, 2 }, new wstring[5]{ L"Sps", L"Code", L"Parameter 1", L"Parameter 2", L"Parameter 3" }, new uint8_t[5]{ AT_USPIN, AT_SPSCODE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xB4, L"SetPartyReserve", L"Define the party member availability for a future Party call.\n\n1st argument: list of available characters.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Characters available" }, new uint8_t[1]{ AT_CHARACTER }, 0 },
	{ 0xB5, L"PretendToBe", L"Link the object to another object, forcing this object to follow the linked object's position and logical animations.\n\n1st argument: object to pretend to be.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Entry" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xB6, L"WorldMap", L"Change the scene to a world map.\n\n1st argument: world map destination.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"World Map" }, new uint8_t[1]{ AT_WORLDMAP }, 0 },
	{ 0xB7, L"0xB7", L"Unknown World Map Opcode (EYE).", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB8, L"0xB8", L"Unknown World Map Opcode (AIM).", false, 0, NULL, NULL, NULL, 0 },
	{ 0xB9, L"AddControllerMask", L"Prevent the input to be processed by the game.\n\n1st argument: pad number (should only be 0 or 1).\n2nd argument: button list.\n1: Select\n4: Start\n5: Up\n6: Right\n7: Down\n8: Left\n9: L2\n10: R2\n11: L1\n12: R1\n13: Triangle\n14: Circle\n15: Cross\n16: Square\n17: Cancel\n18: Confirm\n20: Moogle\n21: L1 Ex\n22: R1 Ex\n23: L2 Ex\n24: R2 Ex\n25: Menu\n26: Select Ex", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Pad", L"Buttons" }, new uint8_t[2]{ AT_USPIN, AT_BUTTONLIST }, 0 },
	{ 0xBA, L"RemoveControllerMask", L"Enable back the controller's inputs.\n\n1st argument: pad number (should only be 0 or 1).\n2nd argument: button list.\n1: Select\n4: Start\n5: Up\n6: Right\n7: Down\n8: Left\n9: L2\n10: R2\n11: L1\n12: R1\n13: Triangle\n14: Circle\n15: Cross\n16: Square\n17: Cancel\n18: Confirm\n20: Moogle\n21: L1 Ex\n22: R1 Ex\n23: L2 Ex\n24: R2 Ex\n25: Menu\n26: Select Ex", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Pad", L"Buttons" }, new uint8_t[2]{ AT_USPIN, AT_BUTTONLIST }, 0 },
	{ 0xBB, L"TimedTurnEx", L"Make an object face an angle (animated).\n\n1st argument: object's entry.\n2nd argument: angle.\n0 faces south, 64 faces west, 128 faces north and 192 faces east.\n3rd argument: turn speed (1 is slowest).", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Object", L"Angle", L"Speed" }, new uint8_t[3]{ AT_ENTRY, AT_USPIN, AT_USPIN }, 0 },
	{ 0xBC, L"WaitTurnEx", L"Wait until an object facing movement has ended.\n\n1st argument: object's entry.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xBD, L"RunAnimationEx", L"Play an object's animation.\n\n1st argument: object's entry.\n2nd argument: animation ID.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Object", L"Animation ID" }, new uint8_t[2]{ AT_ENTRY, AT_ANIMATION }, 0 },
	{ 0xBE, L"WaitAnimationEx", L"Wait until the object's animation has ended.\n\n1st argument: object's entry.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Object" }, new uint8_t[1]{ AT_ENTRY }, 0 },
	{ 0xBF, L"MoveInstantEx", L"Instantatly move an object.\n\n1st argument: object's entry.\n2nd and 3rd arguments: destination in (X, Y) format.", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Object", L"Destination" }, new uint8_t[3]{ AT_ENTRY, AT_POSITION_X, AT_POSITION_Y }, 0 },
	{ 0xC0, L"EnableTextureAnimation", L"Run a model texture animation and make it loop.\n\n1st argument: model's entry.\n2nd argument: texture animation ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Texture Animation" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0xC1, L"RunTextureAnimation", L"Run once a model texture animation.\n\n1st argument: model's entry.\n2nd argument: texture animation ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Texture Animation" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0xC2, L"StopTextureAnimation", L"Stop playing the model texture animation.\n\n1st argument: model's entry.\n2nd argument: texture animation ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Object", L"Texture Animation" }, new uint8_t[2]{ AT_ENTRY, AT_USPIN }, 0 },
	{ 0xC3, L"SetTileCamera", L"Link a tile block to a specific field camera (useful for looping movement bounds).\n\n1st argument: background tile block.\n2nd argument: camera ID.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Block", L"Camera ID" }, new uint8_t[2]{ AT_TILE, AT_USPIN }, 0 },
	{ 0xC4, L"RunWorldCode", L"Run one of the World Map codes, which effects have a large range. May modify the weather, the music, call the chocobo or enable the auto-pilot.\n\n1st argument: world code.\n2nd argument: depends on the code.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Code", L"Argument" }, new uint8_t[2]{ AT_WORLDCODE, AT_SPIN }, 0 },
	{ 0xC5, L"RunSoundCode", L"Same as RunSoundCode3( code, music, 0, 0, 0 ).", true, 2, new uint8_t[2]{ 2, 2 }, new wstring[2]{ L"Code", L"Sound" }, new uint8_t[2]{ AT_SOUNDCODE, AT_SOUND }, 0 },
	{ 0xC6, L"RunSoundCode1", L"Same as RunSoundCode3( code, music, arg1, 0, 0 ).", true, 3, new uint8_t[3]{ 2, 2, 3 }, new wstring[3]{ L"Code", L"Sound", L"Argument" }, new uint8_t[3]{ AT_SOUNDCODE, AT_SOUND, AT_SPIN }, 0 },
	{ 0xC7, L"RunSoundCode2", L"Same as RunSoundCode3( code, music, arg1, arg2, 0 ).", true, 4, new uint8_t[4]{ 2, 2, 3, 1 }, new wstring[4]{ L"Code", L"Sound", L"Argument", L"Argument" }, new uint8_t[4]{ AT_SOUNDCODE, AT_SOUND, AT_SPIN, AT_SPIN }, 0 },
	{ 0xC8, L"RunSoundCode3", L"Run a sound code.\n\n1st argument: sound code.\n2nd argument: music or sound to process.\n3rd to 5th arguments: depend on the sound code.", true, 5, new uint8_t[5]{ 2, 2, 3, 1, 1 }, new wstring[5]{ L"Code", L"Sound", L"Argument", L"Argument", L"Argument" }, new uint8_t[5]{ AT_SOUNDCODE, AT_SOUND, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xC9, L"SetupTileLoopingWindow", L"Define the rectangular area for a looping tile block movement.\n\n1st argument: background tile block.\n2nd to 5th arguments: area that loops in local position (default position is (0, 0)).", true, 5, new uint8_t[5]{ 1, 2, 2, 2, 2 }, new wstring[5]{ L"Tile Block", L"Position X", L"Position Y", L"Width", L"Height" }, new uint8_t[5]{ AT_USPIN, AT_USPIN, AT_USPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0xCA, L"ResetTileAnimation", L"Stop the background tile animation and optionally show its first frame's tile block.\n\n1st argument: background tile animation.\n2nd argument: show/hide the 1st frame's tile block.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Animation", L"Visible" }, new uint8_t[2]{ AT_TILEANIM, AT_BOOL }, 0 },
	{ 0xCB, L"EnablePath", L"Enable a field path.\n\n1st argument: field path ID.\n2nd argument: boolean enable/disable.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Path", L"Enable" }, new uint8_t[2]{ AT_WALKPATH, AT_BOOL }, 0 },
	{ 0xCC, L"AddCharacterAttribute", L"Add specific attributes for the player character corresponding to the current's entry (should only be used by the entries of player characters).\n\n1st argument: attribute flags.\n 3: use a ladder.\n 5: hide shadow.\n 6: lock spin angle.\n 7: enable animation sounds.\n others: unknown.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Attributes" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0xCD, L"RemoveCharacterAttribute", L"Remove specific attributes for the player character corresponding to the current's entry (should only be used by the entries of player characters).\n\n1st argument: attribute flags.\n 3: use a ladder.\n 5: hide shadow.\n 6: lock spin angle.\n 7: enable animation sounds.\n others: unknown.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Attributes" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0xCE, L"AddGil", L"Give gil to the player.\n\n1st argument: gil amount.", true, 1, new uint8_t[1]{ 3 }, new wstring[1]{ L"Amount" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xCF, L"RemoveGil", L"Remove gil from the player.\n\n1st argument: gil amount.", true, 1, new uint8_t[1]{ 3 }, new wstring[1]{ L"Amount" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xD0, L"BattleDialog", L"Display text in battle for 60 frames.\n\n1st argument: text to display.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Text" }, new uint8_t[1]{ AT_TEXT }, 0 },
	{ 0xD1, L"0xD1", L"Unknown Opcode (GLOBALCLEAR); ignored in the non-PSX versions.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0xD2, L"0xD2", L"Unknown Opcode (DEBUGSAVE); ignored in the non-PSX versions.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0xD3, L"0xD3", L"Unknown Opcode (DEBUGLOAD); ignored in the non-PSX versions.", false, 0, NULL, NULL, NULL, 0 }, // Return 0 on Steam
	{ 0xD4, L"AttachObjectOffset", L"Add an offset to the attachment point.\n\n1st argument: offset in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[1]{ L"Offset" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xD5, L"HideAllObjects", L"Hide all the objects except those flagged with \"don't hide all\" (6th object flag).", false, 0, NULL, NULL, NULL, 0 },
	{ 0xD6, L"ShowAllObjects", L"Show all the (initialized) objects.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xD7, L"ATE", L"Enable or disable ATE.\n\n1st argument: maybe flags (unknown format).", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Unknown" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xD8, L"SetWeather", L"Add a raining effect (works on the world maps, fields and in battles).\n\n1st argument: strength of rain.\n2nd argument: speed of rain (unused for a battle rain).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Strength", L"Speed" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0xD9, L"CureStatus", L"Cure the status ailments of a party member.\n\n1st argument: character.\n2nd argument: status list.\n 1: Petrified\n 2: Venom\n 3: Virus\n 4: Silence\n 5: Darkness\n 6: Trouble\n 7: Zombie", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Statuses" }, new uint8_t[2]{ AT_LCHARACTER, AT_BOOLLIST }, 0 },
	{ 0xDA, L"RunSPSCodeSimple", L"Run Sps code, which seems to be special model effects on the field.\n\n1st argument: sps ID.\n2nd argument: sps code.\n3rd to 5th arguments: depend on the sps code.\n Load Sps (sps type)\n Enable Attribute (attribute list, boolean enable/disable)\n Set Position (X, -Z, Y)\n Set Rotation (angle X, angle Z, angle Y)\n Set Scale (scale factor)\n Attach (object's entry to attach, bone number)\n Set Fade (fade)\n Set Animation Rate (rate)\n Set Frame Rate (rate)\n Set Frame (value) where the value is factored by 16 to get the frame\n Set Position Offset (X, -Z, Y)\n Set Depth Offset (depth)", true, 5, new uint8_t[5]{ 1, 1, 1, 2, 2 }, new wstring[5]{ L"Sps", L"Code", L"Parameter 1", L"Parameter 2", L"Parameter 3" }, new uint8_t[5]{ AT_USPIN, AT_SPSCODE, AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0xDB, L"EnableVictoryPose", L"Enable or disable the victory pose at the end of battles for a specific character.\n\n1st argument: which character.\n2nd argument: boolean activate/deactivate.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Enable" }, new uint8_t[2]{ AT_LCHARACTER, AT_BOOL }, 0 },
	{ 0xDC, L"Jump", L"Perform a jumping animation. Must be used after a SetupJump call.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xDD, L"RemoveParty", L"Remove a character from the player's team.\n\n1st argument: character to remove.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Character" }, new uint8_t[1]{ AT_LCHARACTER }, 0 },
	{ 0xDE, L"SetName", L"Change the name of a party member. Clear the text opcodes from the chosen text.\n\n1st argument: character to rename.\n2nd argument: new name.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Character", L"Text" }, new uint8_t[2]{ AT_LCHARACTER, AT_TEXT }, 0 },
	{ 0xDF, L"SetObjectOvalRatio", L"Define a stretching factor for the object's collisions (seems to only work on world maps). The collisions' shape is not exactly an oval but consists of two discs patched together.\n\n1st argument: increase of the collision in the facing direction of the object.\n 0 removes the feature (both circles are merged)\n 48 makes the object's collision twice higher toward the facing direction than on its sides\n 100 corresponds to a factor of about 2.7 in the facing direction\n 255 corresponds to a factor of about 4", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Stretching Factor" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xE0, L"AddFrog", L"Add one frog to the frog counter.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xE1, L"TerminateBattle", L"Return to the field (or world map) when the rewards are disabled.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xE2, L"SetupJump", L"Setup datas for a Jump call.\n\n1st to 3rd arguments: destination in (X, Z, Y) format.\n4th argument: number of steps for the jump.", true, 4, new uint8_t[4]{ 2, 2, 2, 1 }, new wstring[2]{ L"Destination", L"Steps" }, new uint8_t[4]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y, AT_USPIN }, 0 },
	{ 0xE3, L"SetDialogProgression", L"Change the dialog progression value.\n\n1st argument: new value.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Progression" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xE4, L"MoveTileLoopWithOffset", L"Make the image of a field tile loop over space.\n\n1st argument: background tile block.\n2nd argument: boolean activate/deactivate.\n3rd argument: movement speed.\n4th argument: offset.\n5th argument: move along the X axis or Y axis.", true, 5, new uint8_t[5]{ 1, 1, 2, 2, 1 }, new wstring[5]{ L"Tile Block", L"Enable", L"Speed", L"Offset", L"Is X Movement" }, new uint8_t[5]{ AT_TILE, AT_BOOL, AT_SPIN, AT_SPIN, AT_BOOL }, 0 },
	{ 0xE5, L"AttackSpecial", L"Make the enemy instantatly use a special move. It doesn't use nor modify the battle state so it should be used when the battle is paused. The target(s) are to be set using the SV_Target variable.\n\n1st argument: attack to perform.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Attack" }, new uint8_t[1]{ AT_ATTACK }, 0 },
	{ 0xE6, L"SetTileLoopType", L"Let tile be screen anchored or not.\n\n1st argument: background tile block.\n2nd argument: boolean on/off.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Tile Block", L"Screen Anchored" }, new uint8_t[2]{ AT_TILE, AT_BOOL }, 0 },
	{ 0xE7, L"SetTileAnimationFrame", L"Change the frame of a field tile animation (can be used to hide them all if the given frame is out of range, eg. 255).\n\n1st argument: background animation.\n2nd argument: animation frame to display.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Animation", L"Frame ID" }, new uint8_t[2]{ AT_TILEANIM, AT_USPIN }, 0 },
	{ 0xE8, L"SideWalkXZY", L"Make the character walk to destination without changing his facing angle. Make it synchronous if InitWalk is called before.\n\n1st to 3rd arguments: destination in (X, Z, Y) format.", true, 3, new uint8_t[3]{ 2, 2, 2 }, new wstring[3]{ L"Destination" }, new uint8_t[3]{ AT_POSITION_X, AT_POSITION_Z, AT_POSITION_Y }, 0 },
	{ 0xE9, L"UpdatePartyUID", L"Update the party's entry list (Team Character 1-4 entries) for RunScript and InitObject calls. Should always be used after a Party call (it is automatic when using RemoveParty or AddParty).", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEA, L"CalculateScreenOrigin", L"Calculate the position of the top-left corner of the current camera view in screen coordinates and store it in \"GetScreenCalculatedX\" and \"GetScreenCalculatedY\".", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEB, L"CloseAllWindows", L"Close all the dialogs and UI windows.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEC, L"FadeFilter", L"Apply a fade filter on the screen.\n\n1st argument: filter mode (0 for ADD, 2 for SUBTRACT).\n2nd argument: fading time.\n3rd argument: unknown.\n4th to 6th arguments: color of the filter in (Cyan, Magenta, Yellow) format.", true, 6, new uint8_t[6]{ 1, 1, 1, 1, 1, 1 }, new wstring[4]{ L"Fade In/Out", L"Fading Time", L"Unknown", L"Color" }, new uint8_t[6]{ AT_USPIN, AT_USPIN, AT_SPIN, AT_COLOR_CYAN, AT_COLOR_MAGENTA, AT_COLOR_YELLOW }, 0 },
	{ 0xED, L"SetTileLoopAlpha", L"Unknown opcode about tile looping movements (EBG_overlayDefineViewportAlpha).\n\n1st argument: background tile block.\n2nd and 3rd arguments: unknown factors (X, Y).", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Tile Block", L"Unknown X", L"Unknown Y" }, new uint8_t[3]{ AT_TILE, AT_SPIN, AT_SPIN }, 0 },
	{ 0xEE, L"EnableInactiveAnimation", L"Allow the player's character to play its inactive animation. The inaction time required is:\nFirst Time = 200 + 4 * Random[0, 255]\nFollowing Times = 200 + 2 * Random[0, 255]", false, 0, NULL, NULL, NULL, 0 },
	{ 0xEF, L"ShowHereIcon", L"Show the Here icon over player's chatacter.\n\n1st argument: display type (0 to hide, 3 to show unconditionally)", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Show" }, new uint8_t[1]{ AT_SPIN }, 0 },
	{ 0xF0, L"EnableRun", L"Allow the player's character to run.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xF1, L"SetHP", L"Change the HP of a party's member.\n\n1st argument: character.\n2nd argument: new HP value.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Character", L"HP" }, new uint8_t[2]{ AT_LCHARACTER, AT_USPIN }, 0 },
	{ 0xF2, L"SetMP", L"Change the MP of a party's member.\n\n1st argument: character.\n2nd argument: new MP value.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Character", L"MP" }, new uint8_t[2]{ AT_LCHARACTER, AT_USPIN }, 0 },
	{ 0xF3, L"UnlearnAbility", L"Set an ability's AP back to 0.\n\n1st argument: character.\n2nd argument: ability to reset.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Ability" }, new uint8_t[2]{ AT_LCHARACTER, AT_ABILITY }, 0 },
	{ 0xF4, L"LearnAbility", L"Make character learn an ability.\n\n1st argument: character.\n2nd argument: ability to learn.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Character", L"Ability" }, new uint8_t[2]{ AT_LCHARACTER, AT_ABILITY }, 0 },
	{ 0xF5, L"GameOver", L"Terminate the game with a Game Over screen.", false, 0, NULL, NULL, NULL, 0 },
	{ 0xF6, L"VibrateController", L"Start the vibration lifespan.\n\n1st argument: frame to begin with.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Frame" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xF7, L"ActivateVibration", L"Make the controller's vibration active. If the current controller's frame is out of the vibration time range, the vibration lifespan is reinit.\n\n1st argument: boolean activate/deactivate.", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Activate" }, new uint8_t[1]{ AT_BOOL }, 0 },
	{ 0xF8, L"RunVibrationTrack", L"Run a vibration track.\n\n1st argument: track ID.\n2nd argument: sample (0 or 1).\n3rd argument: boolean activate/deactivate.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Track", L"Sample", L"Activate" }, new uint8_t[3]{ AT_USPIN, AT_USPIN, AT_BOOL }, 0 },
	{ 0xF9, L"ActivateVibrationTrack", L"Activate a vibration track.\n\n1st argument: track ID.\n2nd argument: sample (0 or 1).\n3rd argument: boolean activate/deactivate.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Track", L"Sample", L"Activate" }, new uint8_t[3]{ AT_USPIN, AT_USPIN, AT_BOOL }, 0 },
	{ 0xFA, L"SetVibrationSpeed", L"Set the vibration frame rate.\n\n1st argument: frame rate.", true, 1, new uint8_t[1]{ 2 }, new wstring[1]{ L"Frame Rate" }, new uint8_t[1]{ AT_USPIN }, 0 },
	{ 0xFB, L"SetVibrationFlags", L"Change the vibration flags.\n\n1st argument: flags.\n 8: Loop\n 16: Wrap", true, 1, new uint8_t[1]{ 1 }, new wstring[1]{ L"Flags" }, new uint8_t[1]{ AT_BOOLLIST }, 0 },
	{ 0xFC, L"SetVibrationRange", L"Set the time range of vibration.\n\n1st and 2nd arguments: vibration range in (Start, End) format.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Start", L"End" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0xFD, L"PreloadField", L"Surely preload a field; ignored in the non-PSX versions.\n\n1st argument: unknown.\n2nd argument: field to preload.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Unknown", L"Field" }, new uint8_t[2]{ AT_SPIN, AT_FIELD }, 0 },
	{ 0xFE, L"SetCharacterData", L"Init a party's member battle and menu datas.\n\n1st argument: character.\n2nd argument: boolean update level/don't update level.\n3rd argument: equipement set to use.\n4th argument: character categories ; doesn't change if all are enabled.\n 1: male\n 2: female\n 3: gaian\n 4: terran\n 5: temporary character\n5th argument: ability and command set to use.", true, 5, new uint8_t[5]{ 1, 1, 1, 1, 1 }, new wstring[5]{ L"Character", L"Update Level", L"Equipement Set", L"Category", L"Ability Set" }, new uint8_t[5]{ AT_LCHARACTER, AT_BOOL, AT_EQUIPSET, AT_BOOLLIST, AT_ABILITYSET }, 0 },
	{ 0xFF, L"EXTENDED_CODE", L"Not an opcode.", false, 0, NULL, NULL, NULL, 0 },
	{ 0x100, L"0x100", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x101, L"0x101", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x102, L"0x102", L"Unknown Opcode (BSACTIVE: walkMesh.BGI_simSetActive).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x103, L"0x103", L"Unknown Opcode (BSFLAG: walkMesh.BGI_simSetFlags).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x104, L"0x104", L"Unknown Opcode (BSFLOOR: walkMesh.BGI_simSetFloor).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x105, L"0x105", L"Unknown Opcode (BSRATE: walkMesh.BGI_simSetFrameRate).", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x106, L"0x106", L"Unknown Opcode (BSALGO: walkMesh.BGI_simSetAlgorithm).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x107, L"0x107", L"Unknown Opcode (BSDELTA: walkMesh.BGI_simSetDelta).", true, 3, new uint8_t[3]{ 1, 2, 2 }, new wstring[3]{ L"Unknown", L"Unknown" , L"Unknown" }, new uint8_t[3]{ AT_USPIN, AT_USPIN, AT_USPIN }, 0 },
	{ 0x108, L"0x108", L"Unknown Opcode (BSAXIS: walkMesh.BGI_simSetAxis).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x109, L"0x109", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10A, L"0x10A", L"Unknown Opcode (BAFRAME: walkMesh.BGI_animShowFrame).", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_USPIN, AT_USPIN }, 0 },
	{ 0x10B, L"0x10B", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10C, L"0x10C", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10D, L"0x10D", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 2 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10E, L"0x10E", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 },
	{ 0x10F, L"0x10F", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x110, L"0x110", L"Unknown Opcode.", true, 3, new uint8_t[3]{ 1, 1, 1 }, new wstring[3]{ L"Unknown", L"Unknown", L"Unknown" }, new uint8_t[3]{ AT_SPIN, AT_SPIN, AT_SPIN }, 0 },
	{ 0x111, L"0x111", L"Unknown Opcode.", true, 2, new uint8_t[2]{ 1, 1 }, new wstring[2]{ L"Unknown", L"Unknown" }, new uint8_t[2]{ AT_SPIN, AT_SPIN }, 0 }
};

/* Opcodes used in scripts and still unnamed:
0xB7 // Worlds
0xB8 // Worlds
0x102 // Fields
0x103 // Fields
0x104 // Fields
0x105 // Fields
0x106 // Fields
0x107 // Fields
0x108 // Fields
0x10A // Fields
*/

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
	// 100: unknown/unused
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
	{ 50, L"Plus", L"Single plus (do nothing)." },
	{ 50, L"Minus", L"Single minus." },
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
	{ 3, L"", L"Get the battle data of one or more characters in battle.\nIt returns a list with as many values as characters.\nFor instance, SV_PlayerTeam[HP] returns a list of all the HP of the player's team.\nUse the list operators $ to manipulate the list returned.\n\nKnown battle data fields:\n" },
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
	{ 2, L"&&=$", L"Boolean list variable AND assignment." },
	{ 2, L"^^=$", L"Boolean list variable XOR assignment." },
	{ 2, L"||=$", L"Boolean list variable OR assignment." },
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
	
	{ 50, L"Sin2", L"A sinus operator (the range 0-4096 corresponds to the range 0-360 in degrees)." },
	{ 50, L"Cos2", L"A cosinus operator (the range 0-4096 corresponds to the range 0-360 in degrees)." },
	{ 50, L"GetHP", L"Return the HP of the character." },
	{ 50, L"GetMaxHP", L"Return the MAX HP of the character." },
	{ 51, L"Matching", L"Return the list of indexes matching at least one bit of a bitflag condition.\n\n 1st argument: list of bitflags.\n 2nd argument: bitflag condition." },
	{ 51, L"NotMatching", L"Return the list of indexes not matching any bit of a bitflag condition.\n\n 1st argument: list of bitflags.\n 2nd argument: bitflag condition." },
	{ 51, L"NotMatchingExact", L"Return the list of indexes that doesn't fit exactly the bitflag condition.\n\n 1st argument: list of bitflags.\n 2nd argument: bitflag condition." },
	{ 51, L"ListOR", L"Return the list of indexes that has at least one of the two values being non-zero.\n\n 1st and 2nd arguments: lists of values." },
	{ 50, L"IsUnbutton", L"Check if a button of the controller was released right before the current frame." },
	{ 50, L"IsButtonDown", L"Check if a button of the controller is currently pressed." },
	{ 50, L"Op5A", L"A button pressing check." },
	{ 50, L"Op5B", L"A button releasing check." },
	{ 50, L"Op5C", L"A button holding check." },
	{ 51, L"Angle", L"Calculate the angle between the entry's object position and the target position, in the XY plane.\n\n 1st argument: target position X.\n 2nd argument: target position Y." },
	{ 51, L"Distance", L"Calculate the distance between the entry's object position and the target position, in the XY plane.\n\n 1st argument: target position X.\n 2nd argument: target position Y." },
	{ 19, L"ObjectUID_", L"Retrieve an object's UID or 0 if not found.\nOnly useful for (1) checking if an object exists with the UID or (2) convert special entry IDs (250 to 255) to a valid UID." },
	
	{ 50, L"AngleWithEntry", L"Calculate the angle between the entry's object position and the target's position, in the XY plane." },
	{ 50, L"DistanceWithEntry", L"Calculate the distance between the entry's object position and the target's position, in the XY plane." },
	{ 50, L"Sin", L"Calculate the sinus of an angle and multiply it by 4096 to get an integer." },
	{ 50, L"Cos", L"Calculate the cosinus of an angle and multiply it by 4096 to get an integer." },
	{ 50, L"GetItemCount", L"Return the amount of items of an item type in the player's inventory." },
	{ 50, L"GetTileAnimFrame", L"Return the current frame of the specified tile animation." },
	{ 51, L"AngleOfVector", L"Calculate the angle of the given vector.\n\n 1st argument: vector X coordinate.\n 2nd argument: vector Y coordinate." },
	{ 100, L"Op67", L"" },	{ 100, L"Op68", L"" },	{ 100, L"Op69", L"" },
	{ 50, L"GetAnimDuration", L"Return the duration of the entry's object specified animation." },
	{ 50, L"IsInParty", L"Check if a character is in the player's party." },
	{ 100, L"Op6C", L"Sps information." },
	{ 50, L"AddParty", L"Add a character to the player's party." },
	{ 50, L"GetMP", L"Return the MP of the character." },
	{ 50, L"GetMaxMP", L"Return the MAX MP of the character." },
	
	{ 50, L"GetWalkpathTriangle", L"Return the walkmesh triangle on which the specified entry's object is standing." },
	{ 50, L"GetWalkpath", L"Return the walkpath on which the specified entry's object is standing." },
	{ 100, L"Op72", L"" },				{ 100, L"Op73", L"" },	{ 100, L"Op74", L"" },		{ 100, L"Op75", L"" },	{ 100, L"Op76", L"" },	{ 100, L"Op77", L"" },
	{ 55, L"GetEntryProperty", L"Return one of the specified entry's object properties.\nProperty types 8 and more are the same as the [DATA_ACCESS]." },
	{ 19, L"SV_", L"" },	{ 19, L"GetData_", L"" },	{ 100, L"Op7B", L"" },	{ 100, L"Op7C", L"" },	{ 6, L"", L"" },	{ 7, L"", L"" },	{ -1, L"", L"" },
	
	{ 100, L"Op80", L"" },	{ 100, L"Op81", L"" },	{ 100, L"Op82", L"" },	{ 100, L"Op83", L"" },	{ 100, L"Op84", L"" },	{ 100, L"Op85", L"" },	{ 100, L"Op86", L"" },	{ 100, L"Op87", L"" },
	{ 100, L"Op88", L"" },	{ 100, L"Op89", L"" },	{ 100, L"Op8A", L"" },	{ 100, L"Op8B", L"" },	{ 100, L"Op8C", L"" },	{ 100, L"Op8D", L"" },	{ 100, L"Op8E", L"" },	{ 100, L"Op8F", L"" },
	
	{ 100, L"Op90", L"" },	{ 100, L"Op91", L"" },	{ 100, L"Op92", L"" },	{ 100, L"Op93", L"" },	{ 100, L"Op94", L"" },	{ 100, L"Op95", L"" },	{ 100, L"Op96", L"" },	{ 100, L"Op97", L"" },
	{ 100, L"Op98", L"" },	{ 100, L"Op99", L"" },	{ 100, L"Op9A", L"" },	{ 100, L"Op9B", L"" },	{ 100, L"Op9C", L"" },	{ 100, L"Op9D", L"" },	{ 100, L"Op9E", L"" },	{ 100, L"Op9F", L"" },
	
	{ 100, L"OpA0", L"" },	{ 100, L"OpA1", L"" },	{ 100, L"OpA2", L"" },	{ 100, L"OpA3", L"" },	{ 100, L"OpA4", L"" },	{ 100, L"OpA5", L"" },	{ 100, L"OpA6", L"" },	{ 100, L"OpA7", L"" },
	{ 100, L"OpA8", L"" },	{ 100, L"OpA9", L"" },	{ 100, L"OpAA", L"" },	{ 100, L"OpAB", L"" },	{ 100, L"OpAC", L"" },	{ 100, L"OpAD", L"" },	{ 100, L"OpAE", L"" },	{ 100, L"OpAF", L"" },
	
	{ 100, L"OpB0", L"" },	{ 100, L"OpB1", L"" },	{ 100, L"OpB2", L"" },	{ 100, L"OpB3", L"" },	{ 100, L"OpB4", L"" },	{ 100, L"OpB5", L"" },	{ 100, L"OpB6", L"" },	{ 100, L"OpB7", L"" },
	{ 100, L"OpB8", L"" },	{ 100, L"OpB9", L"" },	{ 100, L"OpBA", L"" },	{ 100, L"OpBB", L"" },	{ 100, L"OpBC", L"" },	{ 100, L"OpBD", L"" },	{ 100, L"OpBE", L"" },	{ 100, L"OpBF", L"" },
	
	{ 13, L"VAR_GenSBool_", L"" },	{ 13, L"VAR_GlobSBool_", L"" },	{ 13, L"VAR_LocSBool_", L"" },	{ 19, L"VAR_Null1_", L"" },	{ 13, L"VAR_GenBool_", L"" },	{ 13, L"VAR_GlobBool_", L"" },		{ 13, L"VAR_LocBool_", L"" },	{ 19, L"VAR_Null2_", L"" },
	{ 12, L"VAR_GenInt24_", L"" },	{ 12, L"VAR_GlobInt24_", L"" },	{ 12, L"VAR_LocInt24_", L"" },	{ 19, L"VAR_Null3_", L"" },	{ 12, L"VAR_GenUInt24_", L"" },	{ 12, L"VAR_GlobUInt24_", L"" },	{ 12, L"VAR_LocUInt24_", L"" },	{ 19, L"VAR_Null4_", L"" },
	
	{ 10, L"VAR_GenInt8_", L"" },	{ 10, L"VAR_GlobInt8_", L"" },	{ 10, L"VAR_LocInt8_", L"" },	{ 19, L"VAR_Null5_", L"" },	{ 10, L"VAR_GenUInt8_", L"" },	{ 10, L"VAR_GlobUInt8_", L"" },		{ 10, L"VAR_LocUInt8_", L"" },	{ 19, L"VAR_Null6_", L"" },
	{ 11, L"VAR_GenInt16_", L"" },	{ 11, L"VAR_GlobInt16_", L"" },	{ 11, L"VAR_LocInt16_", L"" },	{ 19, L"VAR_Null7_", L"" },	{ 11, L"VAR_GenUInt16_", L"" },	{ 11, L"VAR_GlobUInt16_", L"" },	{ 11, L"VAR_LocUInt16_", L"" },	{ 19, L"VAR_Null8_", L"" },
	
	{ 23, L"VARL_GenSBool_", L"" },	{ 23, L"VARL_GlobSBool_", L"" },	{ 23, L"VARL_LocSBool_", L"" },	{ 29, L"VARL_Null1_", L"" },	{ 23, L"VARL_GenBool_", L"" },		{ 23, L"VARL_GlobBool_", L"" },		{ 23, L"VARL_LocBool_", L"" },		{ 29, L"VARL_Null2_", L"" },
	{ 22, L"VARL_GenInt24_", L"" },	{ 22, L"VARL_GlobInt24_", L"" },	{ 22, L"VARL_LocInt24_", L"" },	{ 29, L"VARL_Null3_", L"" },	{ 22, L"VARL_GenUInt24_", L"" },	{ 22, L"VARL_GlobUInt24_", L"" },	{ 22, L"VARL_LocUInt24_", L"" },	{ 29, L"VARL_Null4_", L"" },
	
	{ 20, L"VARL_GenInt8_", L"" },	{ 20, L"VARL_GlobInt8_", L"" },		{ 20, L"VARL_LocInt8_", L"" },	{ 29, L"VARL_Null5_", L"" },	{ 20, L"VARL_GenUInt8_", L"" },		{ 20, L"VARL_GlobUInt8_", L"" },	{ 20, L"VARL_LocUInt8_", L"" },		{ 29, L"VARL_Null6_", L"" },
	{ 21, L"VARL_GenInt16_", L"" },	{ 21, L"VARL_GlobInt16_", L"" },	{ 21, L"VARL_LocInt16_", L"" },	{ 29, L"VARL_Null7_", L"" },	{ 21, L"VARL_GenUInt16_", L"" },	{ 21, L"VARL_GlobUInt16_", L"" },	{ 21, L"VARL_LocUInt16_", L"" },	{ 29, L"VARL_Null8_", L"" }
};

static VariableOperation VarEntryPropList[] = {
	{ 1000, L"GetEntryPosX", L"Return the specified entry's X position." },
	{ 1001, L"GetEntryPosZ", L"Return the specified entry's Z position." },
	{ 1002, L"GetEntryPosY", L"Return the specified entry's Y position." },
	{ 1003, L"GetEntryAngle", L"Return the specified entry's facing angle." },
	{ 1004, L"GetEntryFlags", L"Return the specified object's model flags.\n 1: show model\n 2: collision with player character\n 4: collision with NPC\n 8: talk\n 16: can't walk through by insisting\n 32: don't hide all\n 64: unknown/unused\n 128: is turning" },
	{ 1005, L"GetEntryUniqueID", L"Return the specified entry's unique ID." },
	{ 1006, L"GetEntryScriptLevel", L"Return the specified entry's current script level." },
	{ 1007, L"GetEntryAnimFrame", L"Return the current animation frame of the specified entry." }
};

struct VariableOperationArgType {
	uint8_t op;
	uint8_t type;
};

// For operations with 2 arguments, put 2nd argument type then 1st argument type
static VariableOperationArgType VarOpTypeList[] = {
	{ 79,	AT_BUTTONLIST }, // IsButton
	{ 82,	AT_LCHARACTER }, // GetHP
	{ 83,	AT_LCHARACTER }, // GetMaxHP
	{ 88,	AT_BUTTONLIST }, // IsUnbutton
	{ 89,	AT_BUTTONLIST }, // IsButtonDown
	{ 100,	AT_ITEM }, // GetItemCount
	{ 101,	AT_TILEANIM }, // GetTileAnimFrame
	{ 106,	AT_ANIMATION }, // GetAnimDuration
	{ 107,	AT_LCHARACTER }, // IsInParty
	{ 109,	AT_LCHARACTER }, // AddParty
	{ 110,	AT_LCHARACTER }, // GetMP
	{ 111,	AT_LCHARACTER }, // GetMaxMP
	{ 112,	AT_ENTRY }, // GetWalkpathTriangle
	{ 113,	AT_ENTRY } // GetWalkpath
};

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

// AttackType:
// -- Physical attack: 1,3,6,8,9,10,11,15,25,26,27,28,29,30,31,32,33,52
// -- Magic attack: 16,17,18,19,20,21,22,23,24,34

#define VARCODE_SHARED	0x79
#define VARCODE_ENGINE	0x7A
#define VARCODE_BOOL	0xC4
#define VARCODE_INT24	0xC8
#define VARCODE_UINT24	0xCC
#define VARCODE_INT8	0xD0
#define VARCODE_UINT8	0xD4
#define VARCODE_INT16	0xD8
#define VARCODE_UINT16	0xDC
#define VARCODE_LBOOL	0xE4
#define VARCODE_LINT24	0xE8
#define VARCODE_LUINT24	0xEC
#define VARCODE_LINT8	0xF0
#define VARCODE_LUINT8	0xF4
#define VARCODE_LINT16	0xF8
#define VARCODE_LUINT16	0xFC

static VariableName VarNameList[] = {
	{ VARCODE_SHARED,    0,	L"SV_Target",				L"Shared variable with the battle system.\nTarget of the enemy's attack for a further Attack call.\n\nThe target is a list of 8 bits, first 4 bits being the party members and last 4 bits being the enemies." },
	{ VARCODE_SHARED,    1,	L"SV_FunctionEnemy",		L"Shared variable with the battle system.\nReturn the enemy's identifying bit." },
	{ VARCODE_SHARED,    2,	L"SV_PlayerTeam",			L"Shared variable with the battle system.\nReturn bits identifying the fighting party characters." },
	{ VARCODE_SHARED,    3,	L"SV_EnemyTeam",			L"Shared variable with the battle system.\nReturn bits identifying the fighting enemies." },
	{ VARCODE_SHARED,    4,	L"SV_Everyone",				L"Shared variable with the battle system.\nSame as (SV_PlayerTeam | SV_EnemyTeam)." },
	{ VARCODE_ENGINE,    0,	L"GetRandom",				L"Return a random number between 0 and 255." },
	{ VARCODE_ENGINE,    1,	L"GetFieldCamera",			L"Return the ID of the current field camera." },
	{ VARCODE_ENGINE,    2,	L"IsMovementEnabled",		L"Check if the player's movement is on." },
	{ VARCODE_ENGINE,    3,	L"SyncSounds",				L"Synchronise field sounds and return 0." },
	{ VARCODE_ENGINE,    4,	L"GetCollisionAngle",		L"Return the angle between the player's character and the NPC for collision events." },
	{ VARCODE_ENGINE,    5,	L"GetScriptCharacter",		L"Return the ID of the character whose script has called this. Return a negative value if it's not a playable character or if the current entry wasn't run by another function." },
	{ VARCODE_ENGINE,    6,	L"GetGil",					L"Return the player's Gil amount." },
	{ VARCODE_ENGINE,    7,	L"GetTotalSteps",			L"Return the total amount of steps walked." },
	{ VARCODE_ENGINE,    8,	L"GetDialogProgression",	L"Return the progression counter set by dialog's [PROGRESSION] code." },
	{ VARCODE_ENGINE,    9,	L"GetDialogChoice",			L"Return the choice selected in a multichoice dialog." },
	{ VARCODE_ENGINE,   10,	L"GetFieldExitX",			L"Return the field activated exit position." },
	{ VARCODE_ENGINE,   11,	L"GetFieldExitY",			L"Return the field activated exit position." },
	{ VARCODE_ENGINE,   12,	L"GetScreenCalculatedX",	L"Return the X position (in screen coordinates) computed by either \"CalculateScreenOrigin\" or \"CalculateScreenPosition\"." },
	{ VARCODE_ENGINE,   13,	L"GetScreenCalculatedY",	L"Return the Y position (in screen coordinates) computed by either \"CalculateScreenOrigin\" or \"CalculateScreenPosition\"." },
	{ VARCODE_ENGINE,   14,	L"GetCinematicFrame",		L"Return the frame of the currently running cinematic." },
	{ VARCODE_ENGINE,   15,	L"SyncCinematic",			L"Synchronise cinematic." },
	{ VARCODE_ENGINE,   16,	L"GetFrogAmount",			L"Return the amount of frogs catched." },
	{ VARCODE_ENGINE,   17,	L"GetTimerTime",			L"Return the time left for the timer of the timer window, in seconds." },
	{ VARCODE_ENGINE,   18,	L"GetTetraMasterResult",	L"Return the result of the last Tetra Master game.\n\n0 stands for a win.\n1 stands for a defeat.\n2 stands for a draw." },
	{ VARCODE_ENGINE,   19,	L"GetCardAmount",			L"Return the amount of cards owned by the player." },
	{ VARCODE_ENGINE,   20,	L"GetTime",					L"Return the time clock, in seconds." },
	{ VARCODE_ENGINE,   21,	L"IsTimerShown",			L"Check if the timer window is currently displayed." },
	{ VARCODE_ENGINE,   22,	L"GetSoundDistance",		L"Seem to return some distance about 3D sounds." },
	{ VARCODE_ENGINE,   23,	L"GetSoundVolume",			L"Seem to return the volume of some 3D sounds." },
	{ VARCODE_ENGINE,   24,	L"GetLastBattleId",			L"Return the last battle ID." },
	{ VARCODE_ENGINE,   25,	L"IsAttacking",				L"Check if an attack sequence is running.\nUnlike other informations about the current attack (like GetAttacker or GetTarget), this is sure to turn to false for at least one frame between each attack." },
	{ VARCODE_ENGINE,   26,	L"IsBattleInitialized",		L"Check if the current running module is a battle." },
	{ VARCODE_ENGINE,   27,	L"GetBattleResult",			L"Return the result of the last battle.\n\n0 stands for a defeat.\n1 stands for a win.\n5 stands for an interrupted battle.\n7 stands for an enemy escape." },
	{ VARCODE_ENGINE,   28,	L"GetAttackCommandId",		L"Return the command ID of the current attack." },
	{ VARCODE_ENGINE,   29,	L"GetAttackId",				L"Return the attack ID of the current attack.\nOnly works in counter functions." },
	{ VARCODE_ENGINE,   30,	L"GetBattleState",			L"Return the battle's state.\n\n0 stands for system initialization.\n1 stands for frozen battle.\n2 stands for the starting camera movements.\n3 stands for a normal state.\n4 stands for state turning to the normal state.\n5 stands for a normal state.\n6 stands for the victory fanfare.\n7 stands for a game over state.\n8 stands for the closing screen fading." },
	{ VARCODE_ENGINE,   31,	L"GetBattleGroupId",		L"Return the chosen group ID of the current battle." },
	{ VARCODE_ENGINE,   32,	L"GetBattleStartType",		L"Return the back attack state of the start of the battle.\n\n0 stands for a back attack.\n1 stands for a preemptive strike.\n2 stands for a normal state." },
	{ VARCODE_ENGINE,   33,	L"GetBattleGil",			L"Return the player's Gil amount. Should be used in battles only." },
	{ VARCODE_ENGINE,   34,	L"GetCurrentBattleState",	L"Return the battle's current state. Should be used in battles only.\n\n0 stands for system initialization.\n1 stands for frozen battle.\n2 stands for the starting camera movements.\n3 stands for a normal state.\n4 stands for state turning to the normal state.\n5 stands for a normal state.\n6 stands for the victory fanfare.\n7 stands for a game over state.\n8 stands for the closing screen fading." },
//	{ VARCODE_ENGINE,   35,	L"",		L"." },		btl_seq
	{ VARCODE_ENGINE,   36,	L"GetAttacker",				L"Return the bit identifying the attacker." },
	{ VARCODE_ENGINE,   37,	L"GetTarget",				L"Return the bit identifying the target." },
	{ VARCODE_ENGINE,   38,	L"GetAttackerWeaponElement",L"Return the element of attacker's weapon for a standard attack." },
	{ VARCODE_ENGINE,   39,	L"GetAttackElement",		L"Return the element of the attack.\nWorks even when the effect doesn't take the element into account but return 0 if the caster has Element Null." },
	{ VARCODE_ENGINE,   40,	L"GetBattleLoadState",		L"Return the bitflags of battle loading state.\n\n8 stands for enemy data loaded.\n16 stands for player characters loaded." },
	{ VARCODE_ENGINE,   41,	L"GetExpGain",				L"Return the experience gain of the current battle so far." },
	{ VARCODE_ENGINE,   42,	L"IsRebirthFlameCasted",	L"Check if Eiko is in the party and if she casts the automatic Rebirth Flame." },
	{ VARCODE_ENGINE,  192,	L"GetWorldArea",			L"Return the area ID at player's position." },
	{ VARCODE_ENGINE,  193,	L"GetWorldTerrainType",		L"Return the terrain type at player's position." },
//	{ VARCODE_ENGINE,  194,	L"",		L"." },		Maybe GetHeight
//	{ VARCODE_ENGINE,  195,	L"",		L"." },		Some position ; maybe of the town left?
//	{ VARCODE_ENGINE,  196,	L"",		L"." },
//	{ VARCODE_ENGINE,  197,	L"",		L"." },
	{ VARCODE_ENGINE,  198,	L"GetCurrentChocograph",	L"Return the currently selected chocograph." },
//	{ VARCODE_ENGINE,  199,	L"",		L"." },		Get camera rotation?
//	{ VARCODE_ENGINE,  200,	L"",		L"." },		Return 0 on Steam
	{ VARCODE_ENGINE,  201,	L"GetWorldRealPositionX",	L"Return the player's character position X." },
	{ VARCODE_ENGINE,  202,	L"GetWorldRealPositionZ",	L"Return the player's character position Z." },
	{ VARCODE_ENGINE,  203,	L"GetWorldRealPositionY",	L"Return the player's character position Y." },
//	{ VARCODE_ENGINE,  204,	L"",		L"." },		w_weatherDistanceEva
	{ VARCODE_ENGINE,  205,	L"IsMouseEncounterable",	L"Check if the player is walking in a forest, is on the World Map since more than 400 frames and if the encounter odds are favorable. Return 1 in this case and 0 else." },
	{ VARCODE_ENGINE,  207,	L"GetWorldZone",			L"Return the zone ID at player's position." },
//	{ VARCODE_ENGINE,  208,	L"",		L"." },		Maybe GetHeight on the floor
	{ VARCODE_ENGINE,  209,	L"IsWorldInitialized",		L"Check if the World Map event loop is already running." },
	{ VARCODE_ENGINE,  210,	L"GetWorldTitleDisplay",	L"Return the state of the continent's name display.\n\n0 stands for a title waiting for display.\n1 stands for a fading in title.\n2 stands for a fading out title.\n3 stands for nothing displayed." },
	{ VARCODE_UINT16,    0,	L"General_ScenarioCounter",	L"Scenario counter, increasing at various point of the game.\n\nSome key values:\n"
														L"1000: Player gets control of Zidane\n1500: Blank wears Pluto armor\n1600: Blank wears off Pluto armor\n2400: Exited Evil Forest\n2990: South Gate destroyed\n"
														L"3900: Disc 2\n4990: Cleyra destroyed\n5598: Lindblum destroyed\n5990: Reached Exterior Continent\n6200: Black Mage Village entered\n6880: Amarant joined\n6990: South Gate recovered\n"
														L"7010: Disc 3 (Tantalus ran out of Pub)\n8800: Alexandria destroyed\n9450: Desert Palace accessible through sand pit\n9600: Boarded Hilda Garde I\n9890: Desert Palace done\n"
														L"10400: Hilda Garde III obtained\n10500: Ipsen Castle entered\n10600: Ipsen Castle done\n10620: Eiko and Dagger gone to Shrine\n10640: Amarant and Freya gone to Shrine\n10660: Steiner and Vivi gone to Shrine\n10700: Shrines done\n10830: Mikoto sighted\n"
														L"11100: Disc 4\n11760: Crystal World reached\n12000: *spoilers* Kuja dies" },
	{ VARCODE_INT16,     2,	L"General_FieldEntrance",	L"Variable shared between field transition to know where the player comes from." },
	{ VARCODE_UINT8,     4,	L"Setting_OptionalQuina",	L"Flag checking if Quina is in the party.\nUsed for the part where s/he is an optional character." },
	{ VARCODE_UINT8,     8,	L"Field_MusicVolume",		L"Volume of the Field music." },
	{ VARCODE_UINT8,    16,	L"Setting_TranceOn",		L"Flag enabling or disabling the trance system." },
	{ VARCODE_UINT8,    17,	L"Setting_DaggerDepresses",	L"Flag enabling or disabling Dagger depression state." },
	{ VARCODE_UINT8,    18,	L"Setting_MPx4",			L"Flag enabling or disabling the MP cost boost." },
	{ VARCODE_UINT16,   19,	L"Setting_PartyInitialized",L"Bit flags checking which characters were initialized with \"SetCharacterData\"." },
	{ VARCODE_UINT16,   21,	L"Setting_PartyReserve",	L"Bit flags memorizing the current Party Reserve." },
//	{ VARCODE_UINT8,    24,	L"World_CloseLocationFoot" },
//	{ VARCODE_UINT8,    26,	L"World_CloseLocationChocobo" },
//	{ VARCODE_UINT8,    28,	L"World_CloseLocationShip" },
//	{ VARCODE_UINT8,    35,	L"World_MegamapShow" },
//	{ VARCODE_UINT8,    38,	L"World_CloseLocationKnown" },
//	{ VARCODE_UINT8,    39,	L"World_CloseLocation" },
	{ VARCODE_UINT24,   64,	L"World_PlayerPositionX",	L"X coordinate of the Player's character position in the World Map." },
	{ VARCODE_UINT16,   67,	L"World_PlayerPositionZ",	L"Z coordinate of the Player's character position in the World Map." },
	{ VARCODE_UINT24,   69,	L"World_PlayerPositionY",	L"Y coordinate of the Player's character position in the World Map." },
	{ VARCODE_UINT8,    72,	L"World_PlayerAngle",		L"Facing angle of the Player's character in the World Map." },
	{ VARCODE_UINT16,   92,	L"World_LocationKnown1",	L"Bit flags for World Map locations already known." },
	{ VARCODE_UINT16,   94,	L"World_LocationKnown2",	L"Bit flags for World Map locations already known." },
	{ VARCODE_UINT16,   96,	L"World_LocationKnown3",	L"Bit flags for World Map locations already known." },
	{ VARCODE_UINT16,   98,	L"World_LocationKnown4",	L"Bit flags for World Map locations already known." },
	{ VARCODE_UINT8,   100,	L"World_MinimapShow",		L"Flag enabling or disabling the World Minimap." },
	{ VARCODE_UINT8,   102,	L"World_WorldMapID",		L"ID of the last visited World Map." },
	{ VARCODE_UINT8,   103,	L"World_LocationSelection",	L"Location currently selected by the player on the World Minimap." },
	{ VARCODE_UINT8,   104,	L"World_MusicVolume",		L"Volume of the World Map music." },
	{ VARCODE_INT16,   107,	L"Chocobo_BeachVisited1",	L"Bit flags for the visited world beaches." }, // Also exist in VARCODE_LBOOL format (16 of them)
	{ VARCODE_INT16,   109,	L"Chocobo_BeachVisited2",	L"Bit flags for the visited world beaches." }, // Also exist in VARCODE_LBOOL format (5 of them)
	{ VARCODE_INT24,   112,	L"Chocobo_HotColdPoints",	L"Point score of the player in Hot & Cold." },
	{ VARCODE_UINT8,   129,	L"Chocobo_GoodTreasureBonus", L"Bonus chance for digging high-valued treasures." },
	{ VARCODE_UINT8,   131,	L"Chocobo_ChocoPieceAmount",L"Number of chocograph pieces collected." },
	{ VARCODE_UINT8,   134,	L"Chocobo_CurrentField",	L"Index of the current Hot & Cold field among Forest (1), Lagoon (2) and Air Garden (3)." },
	{ VARCODE_UINT16,  136,	L"Chocobo_BeakExp",			L"Choco's total beak experience." },
	{ VARCODE_UINT8,   138,	L"Chocobo_BeakLevel64",		L"Choco's current beak level factored by 0.646." },
	{ VARCODE_UINT8,   139,	L"Chocobo_BeakLevel",		L"Choco's current beak level." },
	{ VARCODE_INT16,   140,	L"Chocobo_BeakNextLevel",	L"Choco's current beak experience needed to level up." },
	{ VARCODE_UINT16,  142,	L"Chocobo_LastTreasureTime",L"Time in seconds (as displayed by the timer window) of the last Hot & Cold treasure found." },
	{ VARCODE_UINT16,  146,	L"Chocobo_GardenGamePlayed",L"Number of Hot & Cold games played in the Air Garden." },
	{ VARCODE_UINT16,  148,	L"Chocobo_LagoonGamePlayed",L"Number of Hot & Cold games played in the Lagoon." },
	{ VARCODE_UINT16,  150,	L"Chocobo_ForestGamePlayed",L"Number of Hot & Cold games played in the Forest." },
	{ VARCODE_UINT16,  152,	L"Chocobo_GardenHighScore",	L"Highest score obtained during a Hot & Cold game in the Air Garden." },
	{ VARCODE_UINT16,  154,	L"Chocobo_LagoonHighScore",	L"Highest score obtained during a Hot & Cold game in the Lagoon." },
	{ VARCODE_UINT16,  156,	L"Chocobo_ForestHighScore",	L"Highest score obtained during a Hot & Cold game in the Forest." },
	{ VARCODE_UINT8,   166,	L"Chocobo_ColorPeckBonus",	L"Bonus added to the beak level that depends on Choco's color evolution." },
	{ VARCODE_UINT8,   167,	L"Chocobo_ForestChocographCount", L"Number of chocographs found in the Forest (excluding pieces)." },
	{ VARCODE_UINT8,   168,	L"Chocobo_LagoonChocographCount", L"Number of chocographs found in the Lagoon (excluding pieces)." },
	{ VARCODE_UINT8,   171,	L"Chocobo_GardenChocographCount", L"Number of chocographs found in the Air Garden (excluding pieces)." },
	{ VARCODE_INT24,   172,	L"Chocobo_AvailableChocograph",	L"Bit flags of chocographs that are available to be dug." },
	{ VARCODE_UINT8,   181,	L"Chocobo_EatingGreens",	L"Flag enabled between the moment the player selects a Gysahl Green and the moment Choco arrives." },
	{ VARCODE_UINT16,  182,	L"Chocobo_PepperSpotFound", L"Bit flags for dead pepper spots already found." }, // Chocobo_FoundGarden is part of it
	{ VARCODE_INT24,   184,	L"Chocobo_WorldChestFound",	L"Bit flags for chocograph treasures found on the World Map." },
	{ VARCODE_INT24,   187,	L"Chocobo_ChocographFound",	L"Bit flags for chocographs found in the Hot and Cold minigame." },
	{ VARCODE_UINT8,   190,	L"World_CurrentTransport",	L"Current transport used by the player on the World Map." },
	{ VARCODE_UINT8,   191,	L"Chocobo_ChocoColor",		L"Choco's current evolution." },
	{ VARCODE_UINT8,   192,	L"General_TonberryCounter",	L"Counter of twice the Tonberries killed.\nUsed for Grudge's damage formula:\nDamage = 2 ^ General_TonberryCounter." },
	{ VARCODE_UINT8,   193,	L"General_SummonMadeen",	L"Flag checking if Madeen has already been summoned at least once." },
	{ VARCODE_UINT8,   194,	L"FriendlyMonster_Progress",L"Bit flags for friendly monsters met and satisfied. Determine whether the related battle can be randomly encountered or not in their natural World Map areas." },
	{ VARCODE_UINT8,   195,	L"Ragtime_QuizzDone1",		L"Bit flags for Ragtime Mouse questions already answered." },
	{ VARCODE_UINT8,   196,	L"Ragtime_QuizzDone2",		L"Bit flags for Ragtime Mouse questions already answered." },
	{ VARCODE_UINT8,   197,	L"Ragtime_QuizzDone3",		L"Bit flags for Ragtime Mouse questions already answered." },
	{ VARCODE_UINT8,   198,	L"Ragtime_QuizzSuccess",	L"Bit flags and integer mainly used for Ragtime Mouse questions already successfully answered.\n\n 1st bit: Friendly monster quest completed (determine if Friendly Yan can be encountered).\n 2nd bit: Dagger reacted to Eiko summoning in the Mountain Path.\n 4th to 8th bits: amount of Ragtime Mouse questions rightfully answered." },
	{ VARCODE_UINT8,   207,	L"General_SummonFlag",		L"Bit flags for each summon checking if it has already been summoned at least once." },
	{ VARCODE_INT16,   220,	L"Treasure_PositionX",		L"X coordinate of the treasure being picked up." },
	{ VARCODE_INT16,   222,	L"Treasure_PositionY",		L"Y coordinate of the treasure being picked up." },
	{ VARCODE_INT16,   224,	L"Treasure_Item",			L"Item ID of the treasure being picked up." },
	{ VARCODE_UINT8,   226,	L"Treasure_PickupFlag",		L"Flag memorizing if a treasure has been picked up." },
	{ VARCODE_INT16,   228,	L"Treasure_Gil",			L"Gil amount of the treasure being picked up." },
//	{ VARCODE_LBOOL,   324,	L"Setting_AllowPartySwitch",L"." },
	{ VARCODE_LUINT8,  475,	L"Minigame_BlankFightScore",L"Best score (number of acclaiming nobles) obtained for the Blank Sword Fight minigame." },
	{ VARCODE_LUINT8,  479,	L"General_GizaBellCount",	L"Number of Gizamaluke bells possessed by the player." },
	{ VARCODE_LUINT8,  483,	L"ATE_CleyraVivi",			L"Progression counter of Cleyra's Active Time Events involving Vivi." },
	{ VARCODE_LUINT8,  483,	L"ATE_CleyraQuina",			L"Progression counter of Cleyra's Active Time Events involving Quina." },
	{ VARCODE_LUINT8,  489,	L"ATE_AlexandriaBaku",		L"Progression counter of Alexandria's Active Time Events involving Baku." },
	{ VARCODE_LUINT8,  490,	L"ATE_AlexandriaTantalus",	L"Progression counter of Alexandria's Active Time Events involving the Nero brothers, Blank and Marcus." },
	{ VARCODE_LUINT8,  491,	L"Minigame_HippaulLevel",	L"Hippaul level for the Hippaul Race minigame." },
	{ VARCODE_LUINT8,  492,	L"Minigame_QueenGarnetScore", L"Number of Pluto Knights assigned to a fitting job by the Queen Garnet." },
	{ VARCODE_LINT24,  496,	L"Minigame_RopeBestScore",	L"Best score done in the Jump Rope minigame." },
	{ VARCODE_LINT24,  499,	L"Minigame_RopeSecondScore",L"Second best score done in the Jump Rope minigame." },
	{ VARCODE_LINT24,  502,	L"Minigame_RopeThirdScore",	L"Third best score done in the Jump Rope minigame." },
	{ VARCODE_LUINT8,  509,	L"Stiltzkin_ItemBought",	L"Bit flags for Stiltzkin items bought." },
	{ VARCODE_BOOL,    184,	L"General_LoadedGame",		L"Flag checking if the game was just loaded from a save." },
	{ VARCODE_LBOOL,   808,	L"World_WhiteTransition",	L"Flag stating whether the World Map fading transition is a black or white screen." },
	{ VARCODE_LBOOL,   814,	L"Chocobo_ParadiseFound",	L"Flag checking if the Chocobo's Paradise has been found. Determine its entrance's appearance on the World Maps." },
	{ VARCODE_LBOOL,   815,	L"Chocobo_MognetFound",		L"Flag checking if the Mognet has been found. Determine its entrance's appearance on the World Maps." },
	{ VARCODE_LBOOL,   888,	L"Chocobo_Lv99",			L"Flag checking if Choco's beak level has reached maximum." },
	{ VARCODE_LBOOL,   889,	L"Chocobo_NoMoreChocograph",L"Flag checking if all the chocographs (and pieces) are found in the current location." },
	{ VARCODE_LBOOL,   890,	L"Chocobo_NoFindChocograph",L"Flag checking if all the available chocographs (and pieces) are found in the current location." },
	{ VARCODE_LBOOL,   891,	L"Chocobo_GardenExhaustChocograph",	L"Flag checking if Mene told the player about all the chocographs being found in the Air Garden." },
	{ VARCODE_LBOOL,   892,	L"Chocobo_LagoonExhaustChocograph",	L"Flag checking if Mene told the player about all the chocographs being found in the Lagoon." },
	{ VARCODE_LBOOL,   893,	L"Chocobo_ForestExhaustChocograph",	L"Flag checking if Mene told the player about all the chocographs being found in the Forest." },
	{ VARCODE_LBOOL,   894,	L"Chocobo_LagoonTempExhaustChocograph",	L"Flag checking if Mene told the player about all the available chocographs being found in the Lagoon." },
	{ VARCODE_LBOOL,   895,	L"Chocobo_ForestTempExhaustChocograph",	L"Flag checking if Mene told the player about all the available chocographs being found in the Forest." },
	{ VARCODE_LBOOL,  1042,	L"Chocobo_BeachAvailable",	L"Flag enabling or disabling the quest to visit the world beaches." },
	{ VARCODE_LBOOL,  1064,	L"Chocobo_PepperKnown",		L"Flag checking if Mene told the player about Dead Peppers." },
	{ VARCODE_LBOOL,  1065,	L"Chocobo_ChocoPieceKnown",	L"Flag checking if Mene told the player about chocograph pieces." },
	{ VARCODE_LBOOL,  1066,	L"Chocobo_PepperFound",		L"Flag checking if a Dead Pepper was found during the Hot and Cold minigame." },
	{ VARCODE_LBOOL,  1084,	L"Chocobo_ChocoPieceComplete", L"Flag checking if all the chocograph pieces are found." },
	{ VARCODE_LBOOL,  1417,	L"Chocobo_ChocographKnown",	L"Flag checking if Mene told the player about chocographs." },
	{ VARCODE_LBOOL,  1460,	L"Chocobo_FoundGarden",		L"Flag checking if the Flying Garden has been found." },
	{ VARCODE_LBOOL,  1584,	L"FriendlyMonster_Complete",L"Flag checking if the Friendly Monster quest has been totaly completed (Friendly Yan has been satisfied)." },
	{ VARCODE_LBOOL,  1816,	L"Setting_MagicOff",		L"Flag enabling or disabling the use of magic in the player's menu." },
	{ VARCODE_LBOOL,  7627,	L"Mognet_SuperslickFound",	L"Flag checking if the Superslick was given to the player." },
	{ VARCODE_LBOOL,  8495,	L"Mognet_SuperslickKnown",	L"Flag checking if the Superslick is heard of, and available for picking." },
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

static wstring CommandAddendaName[] = {
	L"Enemy Command",
	L"Boundary Check",
	L"Party Counter",
	L"Return Magic",
	L"Auto-Potion",
	L"Charge! Command",
	L"Enemy Counter",
	L"Enemy Death",
	L"Enemy Reaction",
	L"Flee Command",
	L"Eidolon Phantom",
	L"Rebirth Flame",
	L"Trance Command",
	L"Dead Command",
	L"Reraise Command",
	L"Stone Command"
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
	{ 0x0E, L"Blank's (2)" },
	{ 0x0F, L"Remove all" }
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
	{ 33, L"End Battle" }, // 1 = victory, 5 = interuption, 7 = Enemy flee
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

static ConstantNames WorldCodeName[] = {
	{ 0, L"Weather Stuff" },
	{ 1, L"Change Movement" },
	{ 2, L"Enable Minimap" },
	{ 3, L"Unknown" },
	{ 4, L"Set Navi Mode" },
	{ 5, L"Enable Camera Fix Mode" },
	{ 6, L"Set Effect Regist Parameter 0" },
	{ 7, L"Set Effect Regist Parameter 1" },
	{ 8, L"Set Effect Regist Parameter 2" },
	{ 9, L"Set Effect Regist Parameter 3" },
	{ 10, L"Effect Regist" },
	{ 11, L"Increment Navi Fade Time" },
	{ 12, L"Unknown" },
	{ 13, L"Unknown" },
	{ 14, L"Set Parameter 0" },
	{ 15, L"Enable Weather Auto-Change" },
	{ 16, L"Set Music SE Parameter 0" },
	{ 17, L"Set Music SE Parameter 1" },
	{ 18, L"Set Music SE Parameter 2" },
	{ 19, L"Set Music SE Parameter 3" },
	{ 20, L"Music SE Play" },
	{ 21, L"Music SE Volume" },
	{ 22, L"Music SE Volume Intpl" },
	{ 23, L"Music SE Stop" },
	{ 24, L"Music SE Volume Intpl All" },
	{ 25, L"Set Camera Smooth On" },
	{ 26, L"Set Frame Battle Probability" },
	{ 27, L"Enable Navi Cursol Move" },
	{ 28, L"Get Offset Position" },
	{ 29, L"Call Chocobo" },
	{ 30, L"Change Movement SP1" },
	{ 31, L"Change Movement SP2" },
	{ 32, L"Set Music Fading" },
	{ 33, L"Music SE Volume Intpl All To 0" },
	{ 34, L"Set Auto-Pilot On" },
	{ 35, L"Enable Mog Choice" },
	{ 36, L"Enable Navi Cursol Draw" },
	{ 37, L"Adjust XZ Alpha Control" },
	{ 38, L"Set Auto-Pilot Off" },
	{ 39, L"Set CD Use 3" },
	{ 40, L"Set CD Use 0" },
	{ 41, L"Unknown" },
	{ 500, L"Change Character Control Status" },
	{ 501, L"Set World Map Scenario Counter" },
	{ 502, L"Set World Map Disc To 1" }
};

static ConstantNames SoundCodeName[] = {
	{ 0, L"Play Music" },
	{ 256, L"Stop Music" },
	{ 265, L"Stop Current Music" },
	{ 1792, L"Load Music" },
	{ 2048, L"Suspend Music" },
	{ 2304, L"Restore Music" },
	{ 2566, L"Set Music Position" },
	{ 16903, L"Skip Music Phrase" },
	{ 3072, L"Sync Music" },
	{ 16897, L"Music Volume" },
	{ 16898, L"Music Tempo" },
	{ 16899, L"Music Pitch" },
	{ 16900, L"Music TempoPitch" },
	{ 33537, L"Music Adjust Volume" },
	{ 33538, L"Music Adjust Tempo" },
	{ 33539, L"Music Adjust Pitch" },
	{ 33540, L"Music Adjust TempoPitch" },
	{ 50177, L"Music Fade" },
	{ 50179, L"Music Fade Pitch" },
	{ 1028, L"Music Fade TempoPitch" },
	{ 34305, L"All Musics Volume Intpl" },
	{ 51969, L"All Musics Fade" },
	{ 53248, L"Play Sound" },
	{ 20736, L"Stop Sound" },
	{ 4489, L"Stop Current Sound" },
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
	{ 38403, L"All Sounds Adjust Pitch" },
	{ 53376, L"Play Sound Res" },
	{ 20864, L"Stop Sound Res" },
	{ 6272, L"Suspend Sound Res" },
	{ 6528, L"Restore Sound Res" },
	{ 37505, L"Sound Res Volume" },
	{ 54145, L"Sound Res Volume Intpl" },
	{ 21889, L"All Sounds Res Volume" },
	{ 38529, L"All Sounds Res Volume Intpl" },
	{ 40960, L"Play Stream" },
	{ 8448, L"Stop Stream" },
	{ 25100, L"Reverb Stream" },
	{ 25089, L"Stream Volume" },
	{ 25093, L"Stream Position" },
	{ 25098, L"Stream FMV Volume" },
	{ 30464, L"Load Instr" },
	{ 54147, L"SFX Res Unknown" },
	{ 54149, L"SFX Res Unknown" },
	{ 38531, L"SFX Res Unknown" },
	{ 38533, L"SFX Res Unknown" },
	{ 37507, L"SFX Res Unknown" },
	{ 37509, L"SFX Res Unknown" },
	{ 21891, L"SFX Res Unknown" },
	{ 21893, L"SFX Res Unknown" },
	{ 520, L"Music Null" },
	{ 4616, L"Sound Null" },
	{ 8712, L"Stream Null" },
	{ 16901, L"Unknown" },
	{ 16902, L"Unknown" },
	{ 21762, L"Unknown" },
	{ 21764, L"Unknown" },
	{ 21890, L"Unknown" },
	{ 21892, L"Unknown" },
	{ 25099, L"Unknown" },
	{ 37378, L"Unknown" },
	{ 37380, L"Unknown" },
	{ 37506, L"Unknown" },
	{ 37508, L"Unknown" },
	{ 38402, L"Unknown" },
	{ 38404, L"Unknown" },
	{ 38530, L"Unknown" },
	{ 38532, L"Unknown" },
	{ 50178, L"Unknown" },
	{ 54018, L"Unknown" },
	{ 54020, L"Unknown" },
	{ 54146, L"Unknown" },
	{ 54148, L"Unknown" }
};

static ConstantNames SpsCodeName[] = {
	{ 130, L"Load Sps" },
	{ 131, L"Enable Attribute" },
	{ 135, L"Set Position" },
	{ 140, L"Set Rotation" },
	{ 145, L"Set Scale" },
	{ 150, L"Attach" },
	{ 155, L"Set Fade" },
	{ 156, L"Set Animation Rate" },
	{ 160, L"Set Frame Rate" },
	{ 161, L"Set Frame" },
	{ 165, L"Set Position Offset" },
	{ 170, L"Set Depth Offset" }
};

#endif
