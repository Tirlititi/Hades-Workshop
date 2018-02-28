#ifndef _GUI_SPELLANIMATIONSEDITOR_H
#define _GUI_SPELLANIMATIONSEDITOR_H

class AnimSequenceCodeSizer;
class AnimSequenceEditDialog;
class EnemyAnimSequenceCodeSizer;
class EnemyAnimSequenceEditDialog;

#include <vector>
#include "gui.h"
#include "SpellAnimations.h"
#include "Enemies.h"
using namespace std;

class AnimSequenceCodeSizer : public wxBoxSizer {
public:
	AnimSequenceEditDialog* parent;
	SpellAnimSequenceCodeLine* codeline;
	unsigned int line_nb;
	wxButton* m_add;
	wxButton* m_remove;
	wxChoice* m_code;
	wxControl* m_arg[SPELL_ANIMATION_SEQUENCE_ARG];
	
	AnimSequenceCodeSizer(AnimSequenceEditDialog* p, unsigned int l, SpellAnimSequenceCodeLine* c);
	void DestroyLine();
	void CreateArgumentControls();
};

class AnimSequenceEditDialog : public AnimSequenceEditWindow {
public:
	SpellAnimationDataStruct anim;
	wxArrayString code_list;
	wxArrayString arg_character;
	wxArrayString arg_effect;
	wxArrayString arg_animation;
	wxArrayString arg_fadeinout;
//	wxArrayString arg_spellanim;
	uint8_t** arg_character_id;
	uint8_t** arg_effect_id;
	uint8_t** arg_animation_id;
	uint8_t** arg_fadeinout_id;
//	uint16_t** arg_spellanim_id;
	wxFlexGridSizer* m_sequencesizer;
	AnimSequenceCodeSizer** sequence_code_sizer;
	
	AnimSequenceEditDialog(wxWindow* parent, SpellAnimationDataStruct& a);

	void OnMouseMoveOver(wxMouseEvent& event);
	void OnAddCode(wxCommandEvent& event);
	void OnDeleteCode(wxCommandEvent& event);
	void OnChangeCode(wxCommandEvent& event);
	void OnChangeArgSpin(wxSpinEvent& event);
	void OnChangeArgChoice(wxCommandEvent& event);
};

class EnemyAnimSequenceCodeSizer : public wxBoxSizer {
public:
	EnemyAnimSequenceEditDialog* parent;
	EnemySequenceCodeLine* codeline;
	unsigned int line_nb;
	wxButton* m_add;
	wxButton* m_remove;
	wxChoice* m_code;
	wxControl** m_arg;
	
	EnemyAnimSequenceCodeSizer(EnemyAnimSequenceEditDialog* p, unsigned int l, EnemySequenceCodeLine* c);
	void DestroyLine();
	void CreateArgumentControls();
};

class EnemyAnimSequenceEditDialog : public AnimSequenceEditWindow {
public:
	BattleDataStruct& battle;
	uint16_t anim_id;
	unsigned int sequence_code_amount;
	vector<EnemySequenceCodeLine> sequence_code;
	wxArrayString code_list;
	wxArrayString arg_animation;
	wxArrayString arg_battletext;
	wxArrayString arg_spellanim;
	wxArrayString arg_sound;
	uint32_t** arg_animation_id;
	uint32_t** arg_battletext_id;
	uint32_t** arg_spellanim_id;
	uint32_t** arg_sound_id;
	wxFlexGridSizer* m_sequencesizer;
	vector<EnemyAnimSequenceCodeSizer*> sequence_code_sizer;
	
	EnemyAnimSequenceEditDialog(wxWindow* parent, BattleDataStruct& b, uint16_t animid);
	
	void OnMouseMoveOver(wxMouseEvent& event);
	void OnAddCode(wxCommandEvent& event);
	void OnDeleteCode(wxCommandEvent& event);
	void OnChangeCode(wxCommandEvent& event);
	void OnChangeArgSpin(wxSpinEvent& event);
	void OnChangeArgChoice(wxCommandEvent& event);
};

#endif
