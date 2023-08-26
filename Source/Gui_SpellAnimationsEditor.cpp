#include "Gui_SpellAnimationsEditor.h"

#include "Database_SpellAnimation.h"
#include "Database_Animation.h"
#include "Hades_Strings.h"
#include "main.h"

#define REMOVE_CODE_LINE_TOOLTIP	L"Remove sequence code"
#define ADD_CODE_LINE_TOOLTIP		L"Add sequence code"

inline bool IsSAATChoice(Spell_Animation_Argument_Type saat) {
	return saat==SAAT_CHARACTER || saat==SAAT_EFFECT || saat==SAAT_ANIMATION || saat==SAAT_FADE_INOUT;
}

inline bool IsEAATChoice(Enemy_Animation_Argument_Type eaat) {
	return eaat==EAAT_ANIMATION || eaat==EAAT_TEXT || eaat==EAAT_SPELL_ANIM || eaat==EAAT_SOUND;
}

//====================================//
//       Spell Animation Sequence     //
//====================================//

AnimSequenceCodeSizer::AnimSequenceCodeSizer(AnimSequenceEditDialog* p, unsigned int l, SpellAnimSequenceCodeLine* c) :
	wxBoxSizer(wxHORIZONTAL),
	parent(p),
	codeline(c),
	line_nb(l) {
	m_add = new wxButton(parent->m_sequencewindow,SPELL_ANIMATION_SEQUENCE_ARG*line_nb,_(L"+"),wxDefaultPosition,wxSize(20,20),wxBU_EXACTFIT);
	m_add->SetToolTip(_(ADD_CODE_LINE_TOOLTIP));
	this->Add(m_add,0,wxALIGN_CENTER|wxALL,2);
	m_remove = new wxButton(parent->m_sequencewindow,SPELL_ANIMATION_SEQUENCE_ARG*line_nb,_(L"-"),wxDefaultPosition,wxSize(20,20),wxBU_EXACTFIT);
	m_remove->SetToolTip(_(REMOVE_CODE_LINE_TOOLTIP));
	this->Add(m_remove,0,wxALIGN_CENTER|wxALL,2);
	m_code = new wxChoice(parent->m_sequencewindow,SPELL_ANIMATION_SEQUENCE_ARG*line_nb,wxDefaultPosition,wxDefaultSize,parent->code_list,0);
	m_code->SetSelection(codeline->code);
	this->Add(m_code,0,wxALL,5);
	CreateArgumentControls();
	m_add->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnimSequenceEditDialog::OnAddCode ), NULL, parent );
	m_remove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnimSequenceEditDialog::OnDeleteCode ), NULL, parent );
	m_code->Connect( wxEVT_CHOICE, wxCommandEventHandler( AnimSequenceEditDialog::OnChangeCode ), NULL, parent );
	m_code->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( AnimSequenceEditDialog::OnMouseMoveOver ), NULL, parent );
}

void AnimSequenceCodeSizer::DestroyLine() {
	SpellAnimSequenceCode& seq = GetSpellAnimSequenceCode(codeline->code);
	unsigned int i;
	m_add->Destroy();
	m_remove->Destroy();
	m_code->Destroy();
	for (i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++) {
		m_arg[i]->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( AnimSequenceEditDialog::OnMouseMoveOver ), NULL, parent );
		if (IsSAATChoice(seq.arg_type[i]))
			m_arg[i]->Disconnect( wxEVT_CHOICE, wxCommandEventHandler( AnimSequenceEditDialog::OnChangeArgChoice ), NULL, parent );
		else if (seq.arg_type[i]!=SAAT_NOTHING)
			m_arg[i]->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AnimSequenceEditDialog::OnChangeArgSpin ), NULL, parent );
		m_arg[i]->Destroy();
	}
	parent->m_sequencesizer->Remove(this);
}

void AnimSequenceCodeSizer::CreateArgumentControls() {
	unsigned int i,j;
	SpellAnimSequenceCode& seqcode = GetSpellAnimSequenceCode(codeline->code);
	for (i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++) {
		if (seqcode.arg_type[i]==SAAT_NOTHING) {
			m_arg[i] = new wxStaticText(parent->m_sequencewindow,SPELL_ANIMATION_SEQUENCE_ARG*line_nb+i,_(L""),wxDefaultPosition,wxSize(0,-1));
			this->Add(m_arg[i],0,wxALIGN_CENTER,2);
		} else if (IsSAATChoice(seqcode.arg_type[i])) {
			m_arg[i] = new wxChoice(parent->m_sequencewindow,SPELL_ANIMATION_SEQUENCE_ARG*line_nb+i,wxDefaultPosition,wxSize(150,-1));
			if (seqcode.arg_type[i]==SAAT_CHARACTER) {
				((wxChoice*)m_arg[i])->Append(parent->arg_character,(void**)parent->arg_character_id);
				for (j=0;j<parent->arg_character.GetCount();j++)
					if (*parent->arg_character_id[j]==codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			} else if (seqcode.arg_type[i]==SAAT_EFFECT) {
				((wxChoice*)m_arg[i])->Append(parent->arg_effect,(void**)parent->arg_effect_id);
				for (j=0;j<parent->arg_effect.GetCount();j++)
					if (*parent->arg_effect_id[j]==codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			} else if (seqcode.arg_type[i]==SAAT_ANIMATION) {
				((wxChoice*)m_arg[i])->Append(parent->arg_animation,(void**)parent->arg_animation_id);
				for (j=0;j<parent->arg_animation.GetCount();j++)
					if (*parent->arg_animation_id[j]==codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			} else if (seqcode.arg_type[i]==SAAT_FADE_INOUT) {
				((wxChoice*)m_arg[i])->Append(parent->arg_fadeinout,(void**)parent->arg_fadeinout_id);
				for (j=0;j<parent->arg_fadeinout.GetCount();j++)
					if (*parent->arg_fadeinout_id[j]==codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
/*			} else if (seqcode.arg_type[i]==SAAT_SPELL_ANIM) {
				((wxChoice*)m_arg[i])->Append(parent->arg_spellanim,(void**)parent->arg_spellanim_id);
				for (j=0;j<parent->arg_spellanim.GetCount();j++)
					if ((((*parent->arg_spellanim_id[j]) & 0xFF)==codeline->arg[i]) && (((*parent->arg_spellanim_id[j]) >> 8)==codeline->arg[i+1])) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}*/
			}
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_CHOICE, wxCommandEventHandler( AnimSequenceEditDialog::OnChangeArgChoice ), NULL, parent );
		} else {
			m_arg[i] = new wxSpinCtrl(parent->m_sequencewindow,SPELL_ANIMATION_SEQUENCE_ARG*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(150,-1),wxSP_ARROW_KEYS,0,255,codeline->arg[i]);
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AnimSequenceEditDialog::OnChangeArgSpin ), NULL, parent );
		}
		m_arg[i]->Connect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( AnimSequenceEditDialog::OnMouseMoveOver ), NULL, parent );
	}
}

AnimSequenceEditDialog::AnimSequenceEditDialog(wxWindow* parent, SpellAnimationDataStruct& a) :
	AnimSequenceEditWindow(parent) {
	unsigned int i, j;
	anim.Copy(a);
	code_list.Alloc(0x100);
	for (i = 0; i < 0x100; i++) {
		for (j = 0; j < SPELLANIM_OPCODE.size(); j++)
			if (SPELLANIM_OPCODE[j].id == i) {
				code_list.Add(_(SPELLANIM_OPCODE[j].label));
				break;
			}
		if (j == SPELLANIM_OPCODE.size())
			code_list.Add(wxString::Format(wxT("Unused code 0x%.2X"), i));
	}
	unsigned int achamount = 0, aefamount = 0, aanamount = 0, afdamount = 0;
	for (i = 0; i < SPELLANIM_ARGUMENT_STRING.size(); i++) {
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_CHARACTER) achamount++;
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_EFFECT) aefamount++;
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_ANIMATION) aanamount++;
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_FADE_INOUT) afdamount++;
	}
	arg_character_id = new uint8_t * [achamount];
	arg_effect_id = new uint8_t * [aefamount];
	arg_animation_id = new uint8_t * [aanamount];
	arg_fadeinout_id = new uint8_t * [afdamount];
	arg_character.Alloc(achamount);
	arg_effect.Alloc(aefamount);
	arg_animation.Alloc(aanamount);
	arg_fadeinout.Alloc(afdamount);
	achamount = 0;
	aefamount = 0;
	aanamount = 0;
	afdamount = 0;
	for (i = 0; i < SPELLANIM_ARGUMENT_STRING.size(); i++) {
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_CHARACTER) {
			arg_character_id[achamount++] = new uint8_t(SPELLANIM_ARGUMENT_STRING[i].arg);
			arg_character.Add(_(SPELLANIM_ARGUMENT_STRING[i].label));
		}
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_EFFECT) {
			arg_effect_id[aefamount++] = new uint8_t(SPELLANIM_ARGUMENT_STRING[i].arg);
			arg_effect.Add(_(SPELLANIM_ARGUMENT_STRING[i].label));
		}
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_ANIMATION) {
			arg_animation_id[aanamount++] = new uint8_t(SPELLANIM_ARGUMENT_STRING[i].arg);
			arg_animation.Add(_(SPELLANIM_ARGUMENT_STRING[i].label));
		}
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_FADE_INOUT) {
			arg_fadeinout_id[afdamount++] = new uint8_t(SPELLANIM_ARGUMENT_STRING[i].arg);
			arg_fadeinout.Add(_(SPELLANIM_ARGUMENT_STRING[i].label));
		}
	}
/*	arg_spellanim_id = new uint16_t*[HADES_STRING_SPELL_MODEL.size()];
	arg_spellanim.Alloc(HADES_STRING_SPELL_MODEL.size());
	for (i=0;i<HADES_STRING_SPELL_MODEL.size();i++) {
		arg_spellanim_id[i] = new uint16_t(HADES_STRING_SPELL_MODEL[i].id);
		arg_spellanim.Add(_(HADES_STRING_SPELL_MODEL[i].label));
	}*/
	m_sequencesizer = new wxFlexGridSizer(0, 1, 0, 0);
	m_sequencesizer->SetFlexibleDirection(wxBOTH);
	m_sequencesizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sequence_code_sizer = new AnimSequenceCodeSizer * [anim.seq_code_amount];
	for (i = 0; i < anim.seq_code_amount; i++) {
		sequence_code_sizer[i] = new AnimSequenceCodeSizer(this, i, &anim.seq_code[i]);
		m_sequencesizer->Add(sequence_code_sizer[i], 1, wxEXPAND);
	}
	m_sequencewindow->SetSizer(m_sequencesizer);
	m_sequencewindow->Layout();
	m_sequencesizer->Fit(m_sequencewindow);
}

void AnimSequenceEditDialog::OnMouseMoveOver(wxMouseEvent& event) {
	AnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/SPELL_ANIMATION_SEQUENCE_ARG];
	SpellAnimSequenceCode& seqcode = GetSpellAnimSequenceCode(sizer->m_code->GetSelection());
	m_helptextctrl->ChangeValue(wxString::Format(wxT("Code 0x%.2X\n\n%s"),sizer->codeline->code,_(seqcode.help)));
}

void AnimSequenceEditDialog::OnAddCode(wxCommandEvent& event) {
	unsigned int i,j,l = event.GetId()/SPELL_ANIMATION_SEQUENCE_ARG;
	if (!anim.AddAnimationSequenceCode(l,1)) {
		wxMessageDialog popup(NULL,HADES_STRING_SPELL_ANIMATION_SEQUENCE_FULL,HADES_STRING_ERROR,wxOK|wxCENTRE);
		popup.ShowModal();
		return;
	}
	for (i=l;i+1<anim.seq_code_amount;i++) {
		SpellAnimSequenceCode& seq = GetSpellAnimSequenceCode(sequence_code_sizer[i]->codeline->code);
		for (j=0;j<SPELL_ANIMATION_SEQUENCE_ARG;j++) {
			sequence_code_sizer[i]->m_arg[j]->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( AnimSequenceEditDialog::OnMouseMoveOver ), NULL, this );
			if (IsSAATChoice(seq.arg_type[j]))
				sequence_code_sizer[i]->m_arg[j]->Disconnect( wxEVT_CHOICE, wxCommandEventHandler( AnimSequenceEditDialog::OnChangeArgChoice ), NULL, this );
			else if (seq.arg_type[j]!=SAAT_NOTHING)
				sequence_code_sizer[i]->m_arg[j]->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AnimSequenceEditDialog::OnChangeArgSpin ), NULL, this );
			sequence_code_sizer[i]->m_arg[j]->Destroy();
		}
	}
	AnimSequenceCodeSizer** newsizerlist = new AnimSequenceCodeSizer*[anim.seq_code_amount];
	memcpy(newsizerlist,sequence_code_sizer,(anim.seq_code_amount-1)*sizeof(AnimSequenceCodeSizer*));
	delete[] sequence_code_sizer;
	sequence_code_sizer = newsizerlist;
	for (i=0;i<l;i++)
		sequence_code_sizer[i]->codeline = &anim.seq_code[i];
	for (i=l;i+1<anim.seq_code_amount;i++) {
		sequence_code_sizer[i]->codeline = &anim.seq_code[i];
		sequence_code_sizer[i]->m_code->SetSelection(anim.seq_code[i].code);
		sequence_code_sizer[i]->CreateArgumentControls();
	}
	sequence_code_sizer[anim.seq_code_amount-1] = new AnimSequenceCodeSizer(this,anim.seq_code_amount-1,&anim.seq_code[anim.seq_code_amount-1]);
	m_sequencesizer->Add(sequence_code_sizer[anim.seq_code_amount-1],1,wxEXPAND);
	m_sequencewindow->FitInside();
	m_sequencewindow->Layout();
	m_sequencewindow->Refresh();
}

void AnimSequenceEditDialog::OnDeleteCode(wxCommandEvent& event) {
	unsigned int i,j,l = event.GetId()/SPELL_ANIMATION_SEQUENCE_ARG;
	for (i=l;i+1<anim.seq_code_amount;i++) {
		SpellAnimSequenceCode& seq = GetSpellAnimSequenceCode(sequence_code_sizer[i]->codeline->code);
		for (j=0;j<SPELL_ANIMATION_SEQUENCE_ARG;j++) {
			sequence_code_sizer[i]->m_arg[j]->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( AnimSequenceEditDialog::OnMouseMoveOver ), NULL, this );
			if (IsSAATChoice(seq.arg_type[j]))
				sequence_code_sizer[i]->m_arg[j]->Disconnect( wxEVT_CHOICE, wxCommandEventHandler( AnimSequenceEditDialog::OnChangeArgChoice ), NULL, this );
			else if (seq.arg_type[j]!=SAAT_NOTHING)
				sequence_code_sizer[i]->m_arg[j]->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AnimSequenceEditDialog::OnChangeArgSpin ), NULL, this );
			sequence_code_sizer[i]->m_arg[j]->Destroy();
		}
	}
	anim.DeleteAnimationSequenceCode(l);
	AnimSequenceCodeSizer** newsizerlist = new AnimSequenceCodeSizer*[anim.seq_code_amount];
	memcpy(newsizerlist,sequence_code_sizer,anim.seq_code_amount*sizeof(AnimSequenceCodeSizer*));
	sequence_code_sizer[anim.seq_code_amount]->DestroyLine();
	delete[] sequence_code_sizer;
	sequence_code_sizer = newsizerlist;
	for (i=0;i<l;i++)
		sequence_code_sizer[i]->codeline = &anim.seq_code[i];
	for (i=l;i<anim.seq_code_amount;i++) {
		sequence_code_sizer[i]->codeline = &anim.seq_code[i];
		sequence_code_sizer[i]->m_code->SetSelection(anim.seq_code[i].code);
		sequence_code_sizer[i]->CreateArgumentControls();
	}
	m_sequencewindow->FitInside();
	m_sequencewindow->Layout();
	m_sequencewindow->Refresh();
}

void AnimSequenceEditDialog::OnChangeCode(wxCommandEvent& event) {
	AnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/SPELL_ANIMATION_SEQUENCE_ARG];
	SpellAnimSequenceCode& oldseqcode = GetSpellAnimSequenceCode(sizer->codeline->code);
	uint8_t newcode = event.GetSelection();
	unsigned int i;
	for (i=0;i<SPELL_ANIMATION_SEQUENCE_ARG;i++) {
		sizer->m_arg[i]->Disconnect( wxEVT_ENTER_WINDOW, wxMouseEventHandler( AnimSequenceEditDialog::OnMouseMoveOver ), NULL, this );
		if (IsSAATChoice(oldseqcode.arg_type[i]))
			sizer->m_arg[i]->Disconnect( wxEVT_CHOICE, wxCommandEventHandler( AnimSequenceEditDialog::OnChangeArgChoice ), NULL, this );
		else if (oldseqcode.arg_type[i]!=SAAT_NOTHING)
			sizer->m_arg[i]->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AnimSequenceEditDialog::OnChangeArgSpin ), NULL, this );
		delete sizer->m_arg[i];
	}
	anim.ChangeAnimationSequenceCode(sizer->line_nb,newcode);
	sizer->CreateArgumentControls();
	m_sequencewindow->Layout();
	m_sequencewindow->Refresh();
}

void AnimSequenceEditDialog::OnChangeArgSpin(wxSpinEvent& event) {
	AnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/SPELL_ANIMATION_SEQUENCE_ARG];
	anim.seq_code[sizer->line_nb].arg[event.GetId()%SPELL_ANIMATION_SEQUENCE_ARG] = event.GetPosition();
}

void AnimSequenceEditDialog::OnChangeArgChoice(wxCommandEvent& event) {
	AnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/SPELL_ANIMATION_SEQUENCE_ARG];
	unsigned int argi = event.GetId()%SPELL_ANIMATION_SEQUENCE_ARG;
/*	if (SPELLANIM_OPCODE[anim.seq_code[sizer->line_nb].code].arg_type[argi]==SAAT_SPELL_ANIM) {
		uint16_t newvalue = *(uint16_t*)event.GetClientData();
		anim.seq_code[sizer->line_nb].arg[argi] = newvalue & 0xFF;
		anim.seq_code[sizer->line_nb].arg[argi+1] = newvalue >> 8;
	} else*/
		anim.seq_code[sizer->line_nb].arg[argi] = *(uint8_t*)event.GetClientData();
}

//====================================//
//       Enemy Animation Sequence     //
//====================================//

#define ENEMY_SEQUENCE_CODE_MAX_ARG	10 // only a top limit

EnemyAnimSequenceCodeSizer::EnemyAnimSequenceCodeSizer(EnemyAnimSequenceEditDialog* p, unsigned int l, EnemySequenceCodeLine* c) :
	wxBoxSizer(wxHORIZONTAL),
	parent(p),
	codeline(c),
	line_nb(l) {
	unsigned int i;
	m_add = new wxButton(parent->m_sequencewindow, ENEMY_SEQUENCE_CODE_MAX_ARG * line_nb, _(L"+"), wxDefaultPosition, wxSize(20, 20), wxBU_EXACTFIT);
	m_add->SetToolTip(_(ADD_CODE_LINE_TOOLTIP));
	this->Add(m_add, 0, wxALIGN_CENTER | wxALL, 2);
	m_remove = new wxButton(parent->m_sequencewindow, ENEMY_SEQUENCE_CODE_MAX_ARG * line_nb, _(L"-"), wxDefaultPosition, wxSize(20, 20), wxBU_EXACTFIT);
	m_remove->SetToolTip(_(REMOVE_CODE_LINE_TOOLTIP));
	this->Add(m_remove, 0, wxALIGN_CENTER | wxALL, 2);
	m_code = new wxChoice(parent->m_sequencewindow, ENEMY_SEQUENCE_CODE_MAX_ARG * line_nb, wxDefaultPosition, wxDefaultSize, parent->code_list, 0);
	for (i = 0; i < ENEMYANIM_OPCODE.size(); i++)
		if (ENEMYANIM_OPCODE[i].id == codeline->code) {
			m_code->SetSelection(i);
			break;
		}
	this->Add(m_code, 0, wxALL, 5);
	CreateArgumentControls();
	m_add->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnAddCode), NULL, parent);
	m_remove->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnDeleteCode), NULL, parent);
	m_code->Connect(wxEVT_CHOICE, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnChangeCode), NULL, parent);
	m_code->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EnemyAnimSequenceEditDialog::OnMouseMoveOver), NULL, parent);
}

void EnemyAnimSequenceCodeSizer::DestroyLine() {
	EnemySequenceCode& seq = GetEnemySequenceCode(codeline->code);
	int i;
	m_add->Destroy();
	m_remove->Destroy();
	m_code->Destroy();
	for (i = 0; i < seq.arg_amount; i++) {
		m_arg[i]->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EnemyAnimSequenceEditDialog::OnMouseMoveOver), NULL, parent);
		if (IsEAATChoice(seq.arg_type[i]))
			m_arg[i]->Disconnect(wxEVT_CHOICE, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgChoice), NULL, parent);
		else
			m_arg[i]->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgSpin), NULL, parent);
		m_arg[i]->Destroy();
	}
	delete[] m_arg;
	parent->m_sequencesizer->Remove(this);
}

void EnemyAnimSequenceCodeSizer::CreateArgumentControls() {
	unsigned int j;
	int i;
	EnemySequenceCode& seqcode = GetEnemySequenceCode(codeline->code);
	m_arg = new wxControl*[seqcode.arg_amount];
	for (i = 0; i < seqcode.arg_amount; i++) {
		if (IsEAATChoice(seqcode.arg_type[i])) {
			m_arg[i] = new wxChoice(parent->m_sequencewindow, ENEMY_SEQUENCE_CODE_MAX_ARG * line_nb + i, wxDefaultPosition, wxSize(seqcode.arg_type[i] == EAAT_SOUND ? 220 : 150, -1));
			if (seqcode.arg_type[i] == EAAT_ANIMATION) {
				((wxChoice*)m_arg[i])->Append(parent->arg_animation, (void**)parent->arg_animation_id);
				for (j = 0; j < parent->arg_animation.GetCount(); j++)
					if (*parent->arg_animation_id[j] == codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			} else if (seqcode.arg_type[i] == EAAT_TEXT) {
				((wxChoice*)m_arg[i])->Append(parent->arg_battletext, (void**)parent->arg_battletext_id);
				for (j = 0; j < parent->arg_battletext.GetCount(); j++)
					if (*parent->arg_battletext_id[j] == codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			} else if (seqcode.arg_type[i] == EAAT_SPELL_ANIM) {
				((wxChoice*)m_arg[i])->Append(parent->arg_spellanim, (void**)parent->arg_spellanim_id);
				for (j = 0; j < parent->arg_spellanim.GetCount(); j++)
					if (*parent->arg_spellanim_id[j] == codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			} else if (seqcode.arg_type[i] == EAAT_SOUND) {
				((wxChoice*)m_arg[i])->Append(parent->arg_sound, (void**)parent->arg_sound_id);
				for (j = 0; j < parent->arg_sound.GetCount(); j++)
					if (*parent->arg_sound_id[j] == codeline->arg[i]) {
						((wxChoice*)m_arg[i])->SetSelection(j);
						break;
					}
			}
			this->Add(m_arg[i], 0, wxALIGN_CENTER | wxALL, 2);
			m_arg[i]->Connect(wxEVT_CHOICE, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgChoice), NULL, parent);
		} else {
			if (seqcode.arg_type[i] == EAAT_COORDINATE)
				m_arg[i] = new wxSpinCtrl(parent->m_sequencewindow, ENEMY_SEQUENCE_CODE_MAX_ARG * line_nb + i, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxSP_ARROW_KEYS, -0x8000, 0x7FFF, codeline->arg[i] >= 0x8000 ? codeline->arg[i] - 0x10000 : codeline->arg[i]);
			else
				m_arg[i] = new wxSpinCtrl(parent->m_sequencewindow, ENEMY_SEQUENCE_CODE_MAX_ARG * line_nb + i, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxSP_ARROW_KEYS, seqcode.arg_type[i] == EAAT_TIME ? 1 : 0, (1 << min(seqcode.arg_length[i] * 8, 31)) - 1, codeline->arg[i]);
			this->Add(m_arg[i], 0, wxALIGN_CENTER | wxALL, 2);
			m_arg[i]->Connect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgSpin), NULL, parent);
		}
		m_arg[i]->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EnemyAnimSequenceEditDialog::OnMouseMoveOver), NULL, parent);
	}
}

EnemyAnimSequenceEditDialog::EnemyAnimSequenceEditDialog(wxWindow* parent, BattleDataStruct& b, uint16_t animid) :
	AnimSequenceEditWindow(parent),
	battle(b),
	anim_id(animid) {
	unsigned int i, j;
	sequence_code_amount = battle.sequence_code_amount[anim_id];
	sequence_code = battle.sequence_code[anim_id];
	for (i = 0; i < sequence_code_amount; i++) {
		EnemySequenceCode& seqcode = GetEnemySequenceCode(sequence_code[i].code);
		sequence_code[i].arg = new uint32_t[seqcode.arg_amount];
		memcpy(sequence_code[i].arg, battle.sequence_code[anim_id][i].arg, seqcode.arg_amount * sizeof(uint32_t));
	}
	code_list.Alloc(ENEMYANIM_OPCODE.size());
	for (i = 0; i < ENEMYANIM_OPCODE.size(); i++)
		code_list.Add(_(ENEMYANIM_OPCODE[i].label));
	unsigned int animamount = battle.animation_amount - battle.sequence_base_anim[anim_id];
	int32_t animdbindex;
	for (i = 0; i < battle.sequence_amount; i++)
		if (battle.sequence_base_anim[i] > battle.sequence_base_anim[anim_id] && battle.sequence_base_anim[i] - battle.sequence_base_anim[anim_id] < animamount)
			animamount = battle.sequence_base_anim[i] - battle.sequence_base_anim[anim_id];
	arg_animation_id = new uint32_t*[animamount + 1];
	arg_animation.Alloc(animamount + 1);
	for (i = 0; i < animamount; i++) {
		animdbindex = AnimationDatabase::GetIndex(battle.animation_id[battle.sequence_base_anim[anim_id] + i], DATABASE_MODEL_CATEGORY_TO_LIST(DATABASE_MODEL_CATEGORY_ENEMY));
		arg_animation_id[i] = new uint32_t(i);
		arg_animation.Add(AnimationDatabase::GetDescription(animdbindex));
	}
	arg_animation_id[animamount] = new uint32_t(0xFF);
	arg_animation.Add(_(L"Stand Animation"));
	unsigned int texti = battle.parent->battle[battle.id]->spell_amount + battle.parent->battle[battle.id]->stat_amount;
	for (i = 0; i < battle.parent->battle[battle.id]->stat_amount; i++)
		if (battle.parent->battle[battle.id]->stat[i].sequence_anim_base == battle.sequence_base_anim[anim_id]) {
			for (j = 0; j < i; j++)
				texti += battle.parent->battle[battle.id]->stat[j].text_amount;
			break;
		}
	unsigned int textamount = battle.parent->text[battle.id]->amount - texti + 1;
	arg_battletext_id = new uint32_t*[textamount];
	arg_battletext.Alloc(textamount);
	for (i = 0; i + 1 < textamount; i++) {
		arg_battletext_id[i] = new uint32_t(i);
		arg_battletext.Add(_(battle.parent->text[battle.id]->text[texti++].GetStr(hades::TEXT_PREVIEW_TYPE)));
	}
	arg_battletext_id[textamount - 1] = new uint32_t(0x81);
	arg_battletext.Add(_(battle.parent->text[battle.id]->text[battle.parent->battle[battle.id]->stat_amount + anim_id].GetStr(hades::TEXT_PREVIEW_TYPE)));
	arg_spellanim_id = new uint32_t*[HADES_STRING_SPELL_MODEL.size()];
	arg_spellanim.Alloc(HADES_STRING_SPELL_MODEL.size());
	for (i = 0; i < HADES_STRING_SPELL_MODEL.size(); i++) {
		arg_spellanim_id[i] = new uint32_t(HADES_STRING_SPELL_MODEL[i].id);
		arg_spellanim.Add(_(HADES_STRING_SPELL_MODEL[i].label));
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		if (battle.parent_cluster->SearchChunkType(CHUNK_TYPE_SOUND) >= 0) {
			ChunkData* soundchunk = &battle.parent_cluster->chunk[battle.parent_cluster->SearchChunkType(CHUNK_TYPE_SOUND)];
			arg_sound_id = new uint32_t*[soundchunk->object_amount];
			arg_sound.Alloc(soundchunk->object_amount);
			for (i = 0; i < soundchunk->object_amount; i++) {
				wxString sndlabel;
				sndlabel.Printf(wxT("Sound %d"), i + 1);
				for (j = 0; j < G_V_ELEMENTS(HADES_STRING_AUDIO_NAME); j++)
					if (soundchunk->object_id[i] == HADES_STRING_AUDIO_NAME[j].id) {
						sndlabel = HADES_STRING_AUDIO_NAME[j].label;
						break;
					}
				arg_sound.Add(sndlabel);
				arg_sound_id[i] = new uint32_t(soundchunk->object_id[i]);
			}
		}
	} else {
		arg_sound_id = new uint32_t*[G_V_ELEMENTS(HADES_STRING_AUDIO_NAME)];
		arg_sound.Alloc(G_V_ELEMENTS(HADES_STRING_AUDIO_NAME));
		for (i = 0; i < G_V_ELEMENTS(HADES_STRING_AUDIO_NAME); i++) {
			arg_sound_id[i] = new uint32_t(HADES_STRING_AUDIO_NAME[i].id);
			arg_sound.Add(_(HADES_STRING_AUDIO_NAME[i].label));
		}
	}
	m_sequencesizer = new wxFlexGridSizer(0, 1, 0, 0);
	m_sequencesizer->SetFlexibleDirection(wxBOTH);
	m_sequencesizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sequence_code_sizer.resize(sequence_code_amount);
	for (i = 0; i < sequence_code_amount; i++) {
		sequence_code_sizer[i] = new EnemyAnimSequenceCodeSizer(this, i, &sequence_code[i]);
		m_sequencesizer->Add(sequence_code_sizer[i], 1, wxEXPAND);
	}
	m_sequencewindow->SetSizer(m_sequencesizer);
	m_sequencewindow->Layout();
	m_sequencesizer->Fit(m_sequencewindow);
}

void EnemyAnimSequenceEditDialog::OnMouseMoveOver(wxMouseEvent& event) {
	EnemyAnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/ENEMY_SEQUENCE_CODE_MAX_ARG];
	EnemySequenceCode& seqcode = ENEMYANIM_OPCODE[sizer->m_code->GetSelection()];
	m_helptextctrl->ChangeValue(wxString::Format(wxT("Code 0x%.2X\n\n%s"),sizer->codeline->code,_(seqcode.help)));
}

void EnemyAnimSequenceEditDialog::OnAddCode(wxCommandEvent& event) {
	uint8_t newcode = 1;
	unsigned int i, j, l = event.GetId() / ENEMY_SEQUENCE_CODE_MAX_ARG;
	EnemySequenceCode& oldseq = GetEnemySequenceCode(sequence_code[l].code);
	EnemySequenceCode& newseq = GetEnemySequenceCode(newcode);
	uint16_t newlen = 1;
	for (i = 0; (int)i < newseq.arg_amount; i++)
		newlen += newseq.arg_length[i];
	if (newlen + 3 > (int)battle.GetExtraSize()) { // Secure 3 bytes for padding...
		wxMessageDialog popup(NULL, HADES_STRING_SPELL_ANIMATION_SEQUENCE_FULL, HADES_STRING_ERROR, wxOK | wxCENTRE);
		popup.ShowModal();
		return;
	}
	for (i = l; i < sequence_code_amount; i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(sequence_code[i].code);
		for (j = 0; (int)j < seq.arg_amount; j++) {
			sequence_code_sizer[i]->m_arg[j]->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EnemyAnimSequenceEditDialog::OnMouseMoveOver), NULL, this);
			if (IsEAATChoice(seq.arg_type[j]))
				sequence_code_sizer[i]->m_arg[j]->Disconnect(wxEVT_CHOICE, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgChoice), NULL, this);
			else
				sequence_code_sizer[i]->m_arg[j]->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgSpin), NULL, this);
			sequence_code_sizer[i]->m_arg[j]->Destroy();
		}
	}
	EnemySequenceCodeLine newseqcodeline;
	newseqcodeline.parent = &battle;
	newseqcodeline.code = newcode;
	newseqcodeline.arg = new uint32_t[newseq.arg_amount];
	for (i = 0; (int)i < newseq.arg_amount; i++) {
		if (newseq.arg_type[i] == EAAT_TIME)
			newseqcodeline.arg[i] = 1;
		else
			newseqcodeline.arg[i] = 0;
	}
	sequence_code_amount++;
	sequence_code.insert(sequence_code.begin() + l, newseqcodeline);
	battle.SetSize(battle.size + newlen);
	sequence_code_sizer.resize(sequence_code_amount);
	for (i = 0; i < l; i++)
		sequence_code_sizer[i]->codeline = &sequence_code[i];
	for (i = l; i + 1 < sequence_code_amount; i++) {
		sequence_code_sizer[i]->codeline = &sequence_code[i];
		for (j = 0; j < ENEMYANIM_OPCODE.size(); j++)
			if (ENEMYANIM_OPCODE[j].id == sequence_code[i].code) {
				sequence_code_sizer[i]->m_code->SetSelection(j);
				break;
			}
		sequence_code_sizer[i]->CreateArgumentControls();
	}
	sequence_code_sizer[sequence_code_amount - 1] = new EnemyAnimSequenceCodeSizer(this, sequence_code_amount - 1, &sequence_code[sequence_code_amount - 1]);
	m_sequencesizer->Add(sequence_code_sizer[sequence_code_amount - 1], 1, wxEXPAND);
	m_sequencewindow->FitInside();
	m_sequencewindow->Layout();
	m_sequencewindow->Refresh();
}

void EnemyAnimSequenceEditDialog::OnDeleteCode(wxCommandEvent& event) {
	unsigned int i, j, l = event.GetId() / ENEMY_SEQUENCE_CODE_MAX_ARG;
	EnemySequenceCode& oldseq = GetEnemySequenceCode(sequence_code[l].code);
	Enemy_Animation_Argument_Type argtype[ENEMY_SEQUENCE_CODE_MAX_ARG];
	for (i = 0; (int)i < oldseq.arg_amount; i++)
		argtype[i] = oldseq.arg_type[i];
	uint16_t oldlen = 1;
	for (i = 0; (int)i < oldseq.arg_amount; i++)
		oldlen += oldseq.arg_length[i];
	for (i = l; i + 1 < sequence_code_amount; i++) {
		EnemySequenceCode& seq = GetEnemySequenceCode(sequence_code[i].code);
		for (j = 0; (int)j < seq.arg_amount; j++) {
			sequence_code_sizer[i]->m_arg[j]->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EnemyAnimSequenceEditDialog::OnMouseMoveOver), NULL, this);
			if (IsEAATChoice(seq.arg_type[j]))
				sequence_code_sizer[i]->m_arg[j]->Disconnect(wxEVT_CHOICE, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgChoice), NULL, this);
			else
				sequence_code_sizer[i]->m_arg[j]->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgSpin), NULL, this);
			sequence_code_sizer[i]->m_arg[j]->Destroy();
		}
	}
	sequence_code_amount--;
	sequence_code.erase(sequence_code.begin() + l);
	battle.SetSize(battle.size - oldlen);
	sequence_code_sizer[sequence_code_amount]->DestroyLine();
	sequence_code_sizer.resize(sequence_code_amount);
	for (i = 0; i < l; i++)
		sequence_code_sizer[i]->codeline = &sequence_code[i];
	for (i = l; i < sequence_code_amount; i++) {
		sequence_code_sizer[i]->codeline = &sequence_code[i];
		for (j = 0; j < ENEMYANIM_OPCODE.size(); j++)
			if (ENEMYANIM_OPCODE[j].id == sequence_code[i].code) {
				sequence_code_sizer[i]->m_code->SetSelection(j);
				break;
			}
		sequence_code_sizer[i]->CreateArgumentControls();
	}
	m_sequencewindow->FitInside();
	m_sequencewindow->Layout();
	m_sequencewindow->Refresh();
}

void EnemyAnimSequenceEditDialog::OnChangeCode(wxCommandEvent& event) {
	EnemyAnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId() / ENEMY_SEQUENCE_CODE_MAX_ARG];
	uint8_t newcode = ENEMYANIM_OPCODE[event.GetSelection()].id;
	EnemySequenceCode& oldseq = GetEnemySequenceCode(sizer->codeline->code);
	EnemySequenceCode& newseq = GetEnemySequenceCode(newcode);
	uint16_t oldlen = 1;
	uint16_t newlen = 1;
	int i;
	for (i = 0; i < oldseq.arg_amount; i++)
		oldlen += oldseq.arg_length[i];
	for (i = 0; i < newseq.arg_amount; i++)
		newlen += newseq.arg_length[i];
	if (newlen + 3 > battle.GetExtraSize() + oldlen) { // Secure 3 bytes for padding...
		for (i = 0; i < (int)ENEMYANIM_OPCODE.size(); i++)
			if (ENEMYANIM_OPCODE[i].id == oldseq.id) {
				sizer->m_code->SetSelection(i);
				break;
			}
		wxMessageDialog popup(NULL, HADES_STRING_SPELL_ANIMATION_SEQUENCE_FULL, HADES_STRING_ERROR, wxOK | wxCENTRE);
		popup.ShowModal();
		return;
	}
	for (i = 0; i < oldseq.arg_amount; i++) {
		sizer->m_arg[i]->Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(EnemyAnimSequenceEditDialog::OnMouseMoveOver), NULL, this);
		if (IsEAATChoice(oldseq.arg_type[i]))
			sizer->m_arg[i]->Disconnect(wxEVT_CHOICE, wxCommandEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgChoice), NULL, this);
		else
			sizer->m_arg[i]->Disconnect(wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EnemyAnimSequenceEditDialog::OnChangeArgSpin), NULL, this);
		delete sizer->m_arg[i];
	}
	sequence_code[sizer->line_nb].code = newcode;
	delete[] sequence_code[sizer->line_nb].arg;
	sequence_code[sizer->line_nb].arg = new uint32_t[newseq.arg_amount];
	for (i = 0; i < newseq.arg_amount; i++) {
		if (newseq.arg_type[i] == EAAT_TIME)
			sequence_code[sizer->line_nb].arg[i] = 1;
		else
			sequence_code[sizer->line_nb].arg[i] = 0;
	}
	battle.SetSize(battle.size + newlen - oldlen);
	sizer->CreateArgumentControls();
	m_sequencewindow->Layout();
	m_sequencewindow->Refresh();
}

void EnemyAnimSequenceEditDialog::OnChangeArgSpin(wxSpinEvent& event) {
	EnemyAnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/ENEMY_SEQUENCE_CODE_MAX_ARG];
	uint32_t value = (GetEnemySequenceCode(sizer->codeline->code).arg_type[event.GetId()%ENEMY_SEQUENCE_CODE_MAX_ARG]==EAAT_COORDINATE && event.GetPosition()<0) ? 0x10000+event.GetPosition() : event.GetPosition();
	sequence_code[sizer->line_nb].arg[event.GetId()%ENEMY_SEQUENCE_CODE_MAX_ARG] = value;
}

void EnemyAnimSequenceEditDialog::OnChangeArgChoice(wxCommandEvent& event) {
	EnemyAnimSequenceCodeSizer* sizer = sequence_code_sizer[event.GetId()/ENEMY_SEQUENCE_CODE_MAX_ARG];
	unsigned int argi = event.GetId()%ENEMY_SEQUENCE_CODE_MAX_ARG;
	sequence_code[sizer->line_nb].arg[argi] = *(uint32_t*)event.GetClientData();
}
