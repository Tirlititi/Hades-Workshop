#ifndef _DATABASE_SPELLEFFECT_H
#define _DATABASE_SPELLEFFECT_H

#include "Database_Common.h"
#include "SpellAnimations.h"
#include "Spells.h"

typedef unsigned int Spell_Animation_Argument_Type;
#define SAAT_NOTHING		0
#define SAAT_INTEGER		1
#define SAAT_CHARACTER		2
#define SAAT_EFFECT			3
#define SAAT_MODEL			4
#define SAAT_SOUND			5
#define SAAT_ANIMATION		6
#define SAAT_FADE_INOUT		7

typedef unsigned int Enemy_Animation_Argument_Type;
#define EAAT_INTEGER		0
#define EAAT_COORDINATE		1
#define EAAT_ANIMATION		2
#define EAAT_TEXT			3
#define EAAT_SPELL_ANIM		4
#define EAAT_TIME			5
#define EAAT_SOUND			6

struct SortedChoiceItemModel {
	unsigned int id;
	wxString label;
	Spell_Target_Type target_type;
	Spell_Target_Amount target_amount;
	uint8_t bug_rate;
};

#define SPELL_MODEL_ATTACK_ID 100
#define SPELL_MODEL_ATTACK_AMOUNT 19
#define SPELL_MODEL_WEAPON_ID 313
#define SPELL_MODEL_WEAPON_AMOUNT 64
extern vector<SortedChoiceItemModel> HADES_STRING_SPELL_MODEL;
SortedChoiceItemModel& GetSpellModelInfo(uint16_t modelid);

//====================================//
//       Spell Animation Sequence     //
//====================================//

struct SpellAnimSequenceCode {
	uint8_t id;
	wstring label;
	wstring help;
	Spell_Animation_Argument_Type arg_type[SPELL_ANIMATION_SEQUENCE_ARG];
};

extern vector<SpellAnimSequenceCode> SPELLANIM_OPCODE;
SpellAnimSequenceCode& GetSpellAnimSequenceCode(uint8_t code);

struct SpellAnimSequenceArgumentString {
	Spell_Animation_Argument_Type type;
	uint8_t arg;
	wstring label;
};

extern vector<SpellAnimSequenceArgumentString> SPELLANIM_ARGUMENT_STRING;

//====================================//
//       Enemy Animation Sequence     //
//====================================//

struct EnemySequenceCode {
	uint8_t id;
	wstring label;
	wstring help;
	int arg_amount;
	int* arg_length;
	Enemy_Animation_Argument_Type* arg_type;
};

extern vector<EnemySequenceCode> ENEMYANIM_OPCODE;
EnemySequenceCode& GetEnemySequenceCode(uint8_t code);

#endif
