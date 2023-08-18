#include "Gui_TextEditor.h"

#include <wx/richtext/richtextstyles.h>
#include <wx/tokenzr.h>
#include <cstdarg>
#include <sstream>
#include "YandexTranslation.h"
#include "Database_Text.h"
#include "Hades_Strings.h"
#include "main.h"

#define DEFAULT_SPEED 5 // 16 for Steam Fields
#define PAGE_BREAK_WAIT 2000
#define PIXEL_TO_SIZE 1.15
#define LINE_HEIGHT 19
#define TOKENIZE_SHOW_SEPARATOR L"\x2022"

wxBitmap InitButton(wxBitmap bmp, bool steam = false) {
	float scale = steam ? 0.25 : 0.4;
	wxImage tmpimg = bmp.ConvertToImage();
	bmp = wxBitmap(tmpimg.Rescale(tmpimg.GetWidth()*scale,tmpimg.GetHeight()*scale,wxIMAGE_QUALITY_HIGH));
	return bmp;
}


//==================================//
//              PSX                 //
//==================================//

#define NEW_PAGE_CHAR L'\xB6' // ¶

long StrFindN(wxString str, wchar_t c, int n) {
	size_t pos = -1;
	for (int i=0;i<=n;i++)
		pos = str.find(c,pos+1);
	if (pos==string::npos)
		return -1;
	size_t line = str.find(L'\n',0);
	long nbline = 0;
	while (line<pos && line!=string::npos) {
		line = str.find(L'\n',line+1);
		nbline++;
	}
	return pos+nbline;
}

void UnknownTextOp(TextEditDialogBase* dialog, uint8_t* args) {
}
void SpeedTextOp(TextEditDialogBase* dialog, uint8_t* args) {
	if (args[0]==0x0A) {
		dialog->display_immediate = false;
	} else if (args[0]==0x0C) {
		dialog->display_immediate = true;
		while (dialog->ProcessPreviewText() && dialog->display_immediate) {}
	} else if (args[0]==0x41) {
		dialog->timer->Start(args[1]*40);
		dialog->must_reset_timer = true;
	} else {
		if (args[0]==0x02)
			dialog->wait = args[1]>0 ? 320/args[1] : 0xFFFF;
		else if (args[0]==0x42)
			dialog->wait = DEFAULT_SPEED;
		dialog->timer->Start(dialog->wait);
	}
}
void PartyNTextOp(TextEditDialogBase* dialog, uint8_t* args) {
	dialog->preview_ctrl->WriteText(HADES_STRING_CHARACTER_DEFAULT_NAME[args[0]-0x10]);
}
void ColorTextOp(TextEditDialogBase* dialog, uint8_t* args) {
	static uint8_t GTE_COLORS[][3] = { {255,255,255}, {220,220,220}, {127,127,127}, {255,0,0}, {255,255,0}, {0,127,255}, {255,0,127}, {0,255,0}, {0,0,0}, {255,0,0} };
	dialog->preview_style.SetTextColour(wxColour(GTE_COLORS[args[0]-0x20][0],GTE_COLORS[args[0]-0x20][1],GTE_COLORS[args[0]-0x20][2]));
	dialog->preview_ctrl->BeginStyle(dialog->preview_style);
}
void VarTextOp(TextEditDialogBase* dialog, uint8_t* args) {
	if (args[0]==0x06)			dialog->preview_ctrl->WriteText(_("Token"));
	else if (args[0]==0x0E)		dialog->preview_ctrl->WriteText(_("Item"));
	else if (args[0]==0x40)		dialog->preview_ctrl->WriteText(_("0"));
	else if (args[0]==0x68)		dialog->preview_ctrl->WriteText(_("0"));
	else if (args[0]==0x69)		dialog->preview_ctrl->WriteText(_("Text"));
	else if (args[0]==0x6C)		dialog->preview_ctrl->WriteText(_("0"));
}
void ButtonOp(TextEditDialogBase* dialog, uint8_t* args) {
	static wxBitmap GTE_BUTTONS[] = {
		InitButton(wxBITMAP(buttoncircle_image)), InitButton(wxBITMAP(buttoncross_image)), InitButton(wxBITMAP(buttontriangle_image)), InitButton(wxBITMAP(buttonsquare_image)),
		InitButton(wxBITMAP(buttonl1_image)), InitButton(wxBITMAP(buttonr1_image)), InitButton(wxBITMAP(buttonl2_image)), InitButton(wxBITMAP(buttonr2_image)),
		InitButton(wxBITMAP(buttonstart_image)), InitButton(wxBITMAP(buttonselect_image)),
		InitButton(wxBITMAP(buttonup_image)), InitButton(wxBITMAP(buttondown_image)), InitButton(wxBITMAP(buttonleft_image)), InitButton(wxBITMAP(buttonright_image)),
		InitButton(wxBITMAP(buttonl2_image)), InitButton(wxBITMAP(buttonr2_image)),
		InitButton(wxBITMAP(buttonpad_image))
	};
	static wxBitmap GTE_NEW = InitButton(wxBITMAP(indicatornew_image));
	if (args[0]==0x2F)			dialog->preview_ctrl->WriteImage(GTE_BUTTONS[0x10]);
	else if (args[0]==0x70)		dialog->preview_ctrl->WriteImage(GTE_NEW);
	else						dialog->preview_ctrl->WriteImage(GTE_BUTTONS[args[0] & 0xF]);
}

void (*TextOp[128])(TextEditDialogBase* dialog, uint8_t* args) = {
	&UnknownTextOp,	&UnknownTextOp,	&SpeedTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&VarTextOp,		&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&SpeedTextOp,	&UnknownTextOp,	&SpeedTextOp,	&UnknownTextOp,	&VarTextOp,		&UnknownTextOp,
	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&ButtonOp,
	&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,	
	&VarTextOp,		&SpeedTextOp,	&SpeedTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&VarTextOp,		&VarTextOp,		&UnknownTextOp,	&UnknownTextOp,	&VarTextOp,		&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&ButtonOp,		&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp
};


TextEditDialog::TextEditDialog(wxWindow* parent, FF9String& str, unsigned int extrasize, int style, CharmapDataStruct* chmap, CharmapDataStruct* chmapext, int sizex, int sizey, uint16_t formatamount, TextFormatStruct* format) :
	TextEditWindow(parent),
	TextEditDialogBase(str,new wxTimer(this),m_richtextctrl,style),
	charmap(chmap),
	charmap_Ext(chmapext),
	op_dial(NULL) {
	max_length = str.length+extrasize;
	format_amount = formatamount;
	format_data = new TextFormatStruct[format_amount];
	format_removed = new bool[format_amount];
	memcpy(format_data,format,format_amount*sizeof(TextFormatStruct));
	for (unsigned int i=0;i<format_amount;i++)
		format_removed[i] = false;
	m_opcodeadd->Enable(text.length<max_length);
	m_textcharmap->ChangeValue(HADES_STRING_PRINTABLE_CHAR_CHARMAP);
	if (sizex>=0)
		m_sizex->SetValue(sizex);
	else
		m_sizex->Enable(false);
	if (sizey>=0)
		m_sizey->SetValue(sizey);
	else
		m_sizey->Enable(false);
	CalculateBestSize(sizex<0,sizey<0);
	m_buttonbubble->Enable(m_sizex->IsEnabled() || m_sizey->IsEnabled());
	Connect(wxEVT_TIMER,wxTimerEventHandler(TextEditDialog::OnTimer),NULL,this);
}

TextEditDialog::~TextEditDialog() {
	Disconnect(wxEVT_TIMER,wxTimerEventHandler(TextEditDialog::OnTimer),NULL,this);
	delete timer;
}

int TextEditDialog::ShowModal() {
	m_buttonok->SetFocus();
	m_textctrl->ChangeValue(_(text.str));
	RefreshOpcodeList();
	PreviewText();
	return wxDialog::ShowModal();
}

bool TextEditDialog::ProcessPreviewText() {
	wstring::size_type len = text.str.length();
	if (str_pos>0 && text.str[str_pos-1]==NEW_PAGE_CHAR)
		preview_ctrl->Clear();
	preview_ctrl->SetInsertionPointEnd();
	UpdatePreviewStyle();
	preview_ctrl->BeginStyle(preview_style);
	while (str_pos<len && text.str[str_pos]==text.opcode_wchar) {
		str_pos++;
		TextOp[text.code_arg[code_pos][0]](this,text.code_arg[code_pos]);
		code_pos++;
	}
	if (must_reset_timer)
		return str_pos<len;
	if (str_pos<len) {
		if (text.str[str_pos]==NEW_PAGE_CHAR) {
			timer->Start(PAGE_BREAK_WAIT);
			must_reset_timer = true;
			str_pos++;
		} else
			preview_ctrl->WriteText(_(text.str[str_pos++]));
		if (str_pos>=len)
			timer->Stop();
	} else {
		timer->Stop();
	}
	return str_pos<len;
}

void TextEditDialog::PreviewText() {
	preview_ctrl->Clear();
	preview_align = wxTEXT_ALIGNMENT_LEFT;
	preview_fontweight = wxFONTWEIGHT_NORMAL;
	preview_fontstyle = wxFONTSTYLE_NORMAL;
	preview_underlined = false;
	preview_striked = false;
	preview_has_url = false;
	preview_attrflags = wxTEXT_ATTR_TEXT_COLOUR | wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_ITALIC | wxTEXT_ATTR_FONT_UNDERLINE | wxTEXT_ATTR_FONT_STRIKETHROUGH | wxTEXT_ATTR_ALIGNMENT | wxTEXT_ATTR_EFFECTS;
	preview_effectflags = wxTEXT_ATTR_EFFECT_NONE;
	preview_effects = wxTEXT_ATTR_EFFECT_NONE;
	if (text_style==TEXT_STYLE_DEFAULT)
		preview_style.SetTextColour(wxColour(255,255,255));
	else
		preview_style.SetTextColour(wxColour(0,0,0));
	str_pos = 0;
	choice_pos = 0;
	choice_line = -1;
	line_num = 0;
	wait = DEFAULT_SPEED;
	code_pos = 0;
	timer->Start(wait);
}

int TextEditDialog::GetBubbleSizeX() {
	if (m_sizex->IsEnabled())
		return (int)m_sizex->GetValue()*PIXEL_TO_SIZE;
	return -1;
}

int TextEditDialog::GetBubbleSizeY() {
	if (m_sizey->IsEnabled())
		return m_sizey->GetValue()*LINE_HEIGHT+4;
	return -1;
}

void TextEditDialog::RefreshOpcodeList() {
	unsigned int i,j = 0;
	m_opcodelist->Clear();
	for (i=0;j<format_amount;i++)
		if (!format_removed[i]) {
			m_opcodelist->Append(_(HADES_STRING_TEXT_OPCODE[format_data[i].opcode].label));
			j++;
		}
	for (i=0;i<text.code_amount;i++)
		m_opcodelist->Append(_(L"["+HADES_STRING_TEXT_OPCODE[text.code_arg[i][0]].label+L"]"));
}

void TextEditDialog::CalculateBestSize(bool x, bool y) {
	if (charmap==NULL || (!x && !y))
		return;
	uint16_t sizex, sizey;
	charmap->CalculateTextSize(&text,charmap_Ext,&sizex,&sizey);
	if (x)
		m_sizex->SetValue(sizex);
	if (y)
		m_sizey->SetValue(sizey);
}

void TextEditDialog::OnTextEdit(wxCommandEvent& event) {
	FF9String tmp(text);
	wstring newvalue = m_textctrl->GetValue().ToStdWstring();
	unsigned int i = newvalue.length();
	tmp.SetValue(newvalue);
	if (tmp.length>max_length) {
		wxTextPos ip = m_textctrl->GetInsertionPoint();
		while (tmp.length>max_length)
			tmp.SetValue(newvalue.substr(0,--i));
		m_textctrl->ChangeValue(_(tmp.str));
		m_textctrl->SetInsertionPoint(ip);
	}
	m_opcodeadd->Enable(tmp.length<max_length);
}

void TextEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_PREVIEW)
		PreviewText();
	else if (id==wxID_SET) {
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
		m_textctrl->ChangeValue(_(text.str));
		RefreshOpcodeList();
		CalculateBestSize(!m_sizex->IsEnabled(),!m_sizey->IsEnabled());
		PreviewText();
	} else if (id==wxID_BUBBLE) {
		if (charmap==NULL)
			return;
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
		RefreshOpcodeList();
		CalculateBestSize();
	} else if (id==wxID_UP) {
		int sel = m_opcodelist->GetSelection();
		if (sel!=wxNOT_FOUND && sel>format_amount) {
			text.PermuteCode(sel-format_amount,sel-format_amount-1);
			m_opcodelist->SetString(sel,_(L"["+HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount][0]].label+L"]"));
			m_opcodelist->SetString(sel-1,_(L"["+HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount-1][0]].label+L"]"));
			m_opcodelist->SetSelection(sel-1);
		}
	} else if (id==wxID_DOWN) {
		int sel = m_opcodelist->GetSelection();
		if (sel!=wxNOT_FOUND && sel!=text.code_amount+format_amount-1 && sel>=format_amount) {
			text.PermuteCode(sel-format_amount,sel-format_amount+1);
			m_opcodelist->SetString(sel,_(L"["+HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount][0]].label+L"]"));
			m_opcodelist->SetString(sel+1,_(L"["+HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount+1][0]].label+L"]"));
			m_opcodelist->SetSelection(sel+1);
		}
	} else if (id==wxID_ADD) {
		if (!op_dial)
			op_dial = new TextOpcodeDialog(this);
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
		if (op_dial->ShowModal(0)==wxID_OK) {
			text.AddCode(op_dial->code,op_dial->length,0);
			text.SetValue(text.opcode_wchar+m_textctrl->GetValue().ToStdWstring());
			m_textctrl->ChangeValue(_(text.str));
			RefreshOpcodeList();
			PreviewText();
			m_opcodeadd->Enable(text.length<max_length);
		}
	} else if (id==wxID_REMOVE) {
		if (GetGameType()!=GAME_TYPE_PSX) {
			EndModal(id);
			return;
		}
		int sel = m_opcodelist->GetSelection();
		if (sel!=wxNOT_FOUND) {
			if (sel>=format_amount) {
				text.RemoveCode(sel-format_amount);
				m_opcodelist->Delete(sel);
				if (text.code_amount+format_amount>0)
					m_opcodelist->SetSelection(sel!=text.code_amount+format_amount ? sel : sel-1);
				int pos = StrFindN(m_textctrl->GetValue(),text.opcode_wchar,sel-format_amount);
				if (pos>=0)
					m_textctrl->Remove(pos,pos+1);
			} else {
				unsigned int i,j = 0;
				for (i=0;j<sel || format_removed[i];i++) {
					if (!format_removed[i])
						j++;
				}
				format_removed[i] = true;
				m_opcodelist->Delete(sel);
				format_amount--;
				if (text.code_amount+format_amount>0)
					m_opcodelist->SetSelection(sel!=text.code_amount+format_amount ? sel : sel-1);
			}
		}
	} else {
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
		EndModal(id);
	}
}

void TextEditDialog::OnOpcodeEdit(wxCommandEvent& event) {
	int sel = event.GetInt();
	if (!op_dial)
		op_dial = new TextOpcodeDialog(this);
	if (sel>=format_amount && op_dial->ShowModal(sel-format_amount+1)==wxID_OK) {
		text.SetCode(sel-format_amount,op_dial->code,op_dial->length);
		RefreshOpcodeList();
		PreviewText();
	}
}

void TextEditDialog::OnTimer(wxTimerEvent& event) {
	if (must_reset_timer) {
		must_reset_timer = false;
		timer->Start(wait);
	}
	ProcessPreviewText();
}

TextOpcodeDialog::TextOpcodeDialog(TextEditDialog* parent) :
	TextOpcodeWindow(parent),
	parent_dialog(parent) {
	for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_OPCODE);i++)
		if (HADES_STRING_TEXT_OPCODE[i].type>=0)
			m_opcode->Append(_(HADES_STRING_TEXT_OPCODE[i].label),(void*)&HADES_STRING_TEXT_OPCODE[i]);
}

TextOpcodeDialog::~TextOpcodeDialog() {
}

int TextOpcodeDialog::ShowModal(int codeindex) {
	bool init0x48 = true;
	code_index = codeindex;
	text = parent_dialog->text;
	str = _(text.str);
	token_str = _(text.str);
	token_str.Replace(_(L"\n"), _(TOKENIZE_SHOW_SEPARATOR));
	token_str.Replace(_(text.opcode_wchar),_(L""));
	if (codeindex==0) {
		code[0] = 0;
		length = 1;
		code_pos = 0;
		max_length = parent_dialog->max_length-text.length;
	} else if (codeindex>0) {
		codeindex--;
		memcpy(code,text.code_arg[codeindex],text.code_arg_length[codeindex]);
		length = text.code_arg_length[codeindex];
		code_pos = StrFindN(str,text.opcode_wchar,codeindex)-codeindex;
		max_length = parent_dialog->max_length+length-text.length;
	} else {
		/* TODO
		codeindex = -codeindex-1;
		memcpy(code,parent_dialog->format_data[codeindex].data+1,parent_dialog->format_data[codeindex].length-1);
		length = parent_dialog->format_data[codeindex].length-1;
		code_pos = 0;
		max_length = parent_dialog->max_length+length-text.length;*/
	}
	str.Replace(_(text.opcode_wchar),_(L""));
	for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_OPCODE);i++)
		if (code[0]==HADES_STRING_TEXT_OPCODE[i].id) {
			m_opcode->SetSelection(i);
			SetOpcodeSelection();
			if (HADES_STRING_TEXT_OPCODE[i].type==1) {
				m_defaultarg11->SetValue(code[1]);
				m_defaultarglabel11->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[0]);
			} else if (HADES_STRING_TEXT_OPCODE[i].type==2) {
				m_defaultarg21->SetValue(code[1]);
				m_defaultarg22->SetValue(code[2]);
				m_defaultarglabel21->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[0]);
				m_defaultarglabel22->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[1]);
			} else if (HADES_STRING_TEXT_OPCODE[i].type==3) {
				m_defaultarg31->SetValue(code[1]);
				m_defaultarg32->SetValue(code[2]);
				m_defaultarg33->SetValue(code[3]);
				m_defaultarglabel31->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[0]);
				m_defaultarglabel32->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[1]);
				m_defaultarglabel33->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[2]);
			} else if (HADES_STRING_TEXT_OPCODE[i].type==4) {
				m_tokenizearg->SetValue(code[1]);
				m_tokenizearglabel->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[0]);
				token_sel = 0;
				wxClientDC dc(m_tokenizetextpanel);
				DisplayTokens(dc);
			} else if (HADES_STRING_TEXT_OPCODE[i].type==5) {
				m_0x48arg1->SetValue(length-2);
				m_0x48arglabel1->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[0]);
				m_0x48sizer->Clear(true);
				for (unsigned int j=1;j+1<length;j++) {
					var_spin[j-1] = new wxSpinCtrl(m_opcodepanel0x48,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,255,0);
					var_spin[j-1]->SetValue(code[j]);
					m_0x48sizer->Add(var_spin[j-1],0,wxALL,1);
				}
				init0x48 = false;
			}/* else if (HADES_STRING_TEXT_OPCODE[i].type==TEXT_OPCODE_FORMAT_TYPE+1) {
			 m_formatarg31->SetValue(code[1]);
			 m_formatarg32->SetValue(code[2]);
			 m_formatarg33->SetValue(code[3]);
			 m_defaultarglabel31->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[0]);
			 m_defaultarglabel32->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[1]);
			 m_defaultarglabel33->SetLabel(HADES_STRING_TEXT_OPCODE[i].arg_help[2]);
			 }*/
			break;
		}
	if (init0x48) {
		m_0x48arg1->SetValue(1);
		m_0x48sizer->Clear(true);
		var_spin[0] = new wxSpinCtrl(m_opcodepanel0x48,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,255,0);
		var_spin[0]->SetValue(0);
		m_0x48sizer->Add(var_spin[0],0,wxALL,1);
	}
	Layout();
	Refresh();
	Fit();
	return wxDialog::ShowModal();
}

void TextOpcodeDialog::SetOpcodeSelection() {
	int sel = m_opcode->GetSelection();
	SortedChoiceItemTextOpcode* item = (SortedChoiceItemTextOpcode*)m_opcode->GetClientData(sel);
	m_opcodehelp->SetLabel(_(item->help));
	m_opcodepaneldefault1->Show(item->type==1);
	m_opcodepaneldefault2->Show(item->type==2);
	m_opcodepaneldefault3->Show(item->type==3);
	m_opcodepaneltokenize->Show(item->type==4);
	m_opcodepanel0x48->Show(item->type==5);
}

void TextOpcodeDialog::DisplayTokens(wxDC& dc) {
	static wxBitmap selectorpurple = InitButton(wxBITMAP(textcaretpurple_image));
	static wxBitmap selectorred = InitButton(wxBITMAP(textcaretred_image));
	dc.Clear();
	m_tokenizetextpanel->DoPrepareDC(dc);
	m_tokenizetextpanel->SetVirtualSize(dc.GetTextExtent(token_str)+wxSize(20,5));
	for (unsigned int i=0;i<code[1];i++) {
		unsigned int strp = 0, codep = 0;
		int stri = 0;
		while (strp<code[2*i+2]+0x100*code[2*i+3]) {
			if (parent_dialog->text.raw[strp] == OPCODE_CHAR) {
				if (code_index > 0 && codep + 1 == code_index)
					strp += 2 * code[1];
				else
					strp += parent_dialog->text.code_arg_length[codep];
				codep++;
			} else if (parent_dialog->text.raw[strp] == CHARMAP_A_CODECHAR || parent_dialog->text.raw[strp] == CHARMAP_B_CODECHAR || parent_dialog->text.raw[strp] == CHARMAP_EXT_CODECHAR) {
				strp += 2;
				stri++;
			} else {
				strp++;
				stri++;
			}
		}
		int x = 6+dc.GetTextExtent(token_str.Mid(0,stri)).GetWidth();
		if (i==token_sel) {
			int selw = dc.GetTextExtent(token_str.Mid(stri,token_str.find(_(TOKENIZE_SHOW_SEPARATOR),stri)-stri)).GetWidth();
			dc.SetBrush(wxBrush(wxColour(255,0,0)));
			dc.SetPen(wxPen(wxColour(255,0,0),0));
			dc.DrawRectangle(x+4,7,selw,13);
			dc.SetBrush(wxBrush(wxColour(255,255,255)));
			dc.SetPen(wxPen(wxColour(255,255,255)));
			dc.DrawBitmap(selectorred,x,0);
		} else
			dc.DrawBitmap(selectorpurple,x,0);
	}
	dc.DrawText(token_str,10,5);
}

void TextOpcodeDialog::OnOpcodeChange(wxCommandEvent& event) {
	int newsel = event.GetInt();
	SetOpcodeSelection();
	code[0] = HADES_STRING_TEXT_OPCODE[newsel].id;
	if (HADES_STRING_TEXT_OPCODE[newsel].length>=0) {
		length = HADES_STRING_TEXT_OPCODE[newsel].length+1;
		if (HADES_STRING_TEXT_OPCODE[newsel].type==1) {
			code[1] = m_defaultarg11->GetValue();
			m_defaultarglabel11->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[0]);
		} else if (HADES_STRING_TEXT_OPCODE[newsel].type==2) {
			code[1] = m_defaultarg21->GetValue();
			code[2] = m_defaultarg22->GetValue();
			m_defaultarglabel21->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[0]);
			m_defaultarglabel22->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[1]);
		} else if (HADES_STRING_TEXT_OPCODE[newsel].type==3) {
			code[1] = m_defaultarg31->GetValue();
			code[2] = m_defaultarg32->GetValue();
			code[3] = m_defaultarg33->GetValue();
			m_defaultarglabel31->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[0]);
			m_defaultarglabel32->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[1]);
			m_defaultarglabel33->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[2]);
		}
	} else if (code[0]==0x04) {
		length = 2+2*m_tokenizearg->GetValue();
		m_tokenizearglabel->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[0]);
		token_sel = 0;
		wxClientDC dc(m_tokenizetextpanel);
		DisplayTokens(dc);
	} else if (code[0]==0x48) {
		length = 2+m_0x48arg1->GetValue();
		m_0x48arglabel1->SetLabel(HADES_STRING_TEXT_OPCODE[newsel].arg_help[0]);
		for (unsigned int j=1;j+1<length;j++)
			code[j] = var_spin[j-1]->GetValue();
		code[length-1] = 0xFF;
	}
	m_buttonok->Enable(length<max_length && HADES_STRING_TEXT_OPCODE[newsel].type<TEXT_OPCODE_FORMAT_TYPE);
	Layout();
	Refresh();
	Fit();
}

void TextOpcodeDialog::OnArgumentChange(wxSpinEvent& event) {
	int id = event.GetId();
	int value = event.GetPosition();
	if (id==wxID_DEFAULTARG1) {
		code[1] = value;
	} else if (id==wxID_DEFAULTARG2) {
		code[2] = value;
	} else if (id==wxID_DEFAULTARG3) {
		code[3] = value;
	} else if (id==wxID_TOKENIZE) {
		uint16_t newpos;
		int i;
		for (i = 0; i<code[1]; i++) {
			newpos = code[2 * i + 2] + 0x100 * code[2 * i + 3] + 2 * (value - code[1]);
			code[2 * i + 2] = newpos & 0xFF;
			code[2 * i + 3] = (newpos >> 8) & 0xFF;
		}
		if (code[1] > 0)
			newpos = code[2 * code[1]] + 0x100 * code[2 * code[1] + 1];
		else
			newpos = 2 * value;
		for (i=code[1];i<value;i++) {
			code[2*i+2] = newpos & 0xFF;
			code[2*i+3] = (newpos >> 8) & 0xFF;
		}
		if (value>code[1] || token_sel>=value)
			token_sel = value-1;
		code[1] = value;
		length = 2+2*value;
		wxClientDC dc(m_tokenizetextpanel);
		DisplayTokens(dc);
		m_buttonok->Enable(length<max_length);
	} else if (id==wxID_48) {
		if (value>length-2) {
			for (int i=length-2;i<value;i++) {
				var_spin[i] = new wxSpinCtrl(m_opcodepanel0x48,wxID_ANY,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,255,0);
				var_spin[i]->SetValue(0);
				m_0x48sizer->Add(var_spin[i],0,wxALL,1);
				code[i+1] = 0;
			}
		} else {
			for (int i=value;i<length-2;i++) {
				m_0x48sizer->Detach(var_spin[i]);
				var_spin[i]->Destroy();
			}
		}
		length = 2+value;
		code[length-1] = 0xFF;
		m_buttonok->Enable(length<max_length);
		Layout();
		Refresh();
		Fit();
	}
}

void TextOpcodeDialog::OnButtonClick(wxCommandEvent& event) {
	if (code[0]==0x04) {
		unsigned int i,j;
		uint16_t strpmain,strpprev;
		for (i=1;i<code[1];i++) {
			strpmain = code[2*i+2]+0x100*code[2*i+3];
			j = i;
			strpprev = code[2*j]+0x100*code[2*j+1];
			while (j>0 && strpmain<strpprev) {
				code[2*j+2] = strpprev & 0xFF;
				code[2*j+3] = (strpprev >> 8) & 0xFF;
				j--;
				strpprev = code[2*j]+0x100*code[2*j+1];
			}
			code[2*j+2] = strpmain & 0xFF;
			code[2*j+3] = (strpmain >> 8) & 0xFF;
		}
	} else if (code[0] == 0x48) {
		for (unsigned int j = 1; j + 1<length; j++)
			code[j] = var_spin[j - 1]->GetValue();
		code[length - 1] = 0xFF;
	}
	EndModal(event.GetId());
}

void TextOpcodeDialog::OnTokenMouse(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
		wxClientDC dc(m_tokenizetextpanel);
		m_tokenizetextpanel->DoPrepareDC(dc);
		wxPoint mpos = event.GetLogicalPosition(dc);
		bool movecaret = true;
		if (event.LeftDown() && abs(mpos.y-3)<=4) {
			for (unsigned int i=0;i<code[1];i++) {
				unsigned int strp = 0, codep = 0;
				int stri = 0;
				while (strp<code[2*i+2]+0x100*code[2*i+3]) {
					if (parent_dialog->text.raw[strp] == OPCODE_CHAR) {
						if (code_index > 0 && codep + 1 == code_index)
							strp += 2 * code[1];
						else
							strp += parent_dialog->text.code_arg_length[codep];
						codep++;
					} else if (parent_dialog->text.raw[strp] == CHARMAP_A_CODECHAR || parent_dialog->text.raw[strp] == CHARMAP_B_CODECHAR || parent_dialog->text.raw[strp] == CHARMAP_EXT_CODECHAR) {
						strp += 2;
						stri++;
					} else {
						strp++;
						stri++;
					}
				}
				int caretx = 10+dc.GetTextExtent(token_str.Mid(0,stri)).GetWidth();
				if (abs(mpos.x-caretx)<=5) {
					token_sel = i;
					movecaret = false;
					break;
				}
			}
		}
		if (movecaret) {
			unsigned int strp = 0, codep = 0;
			unsigned int newpos;
			int caretx = 10;
			int minerr = abs(mpos.x-caretx);
			int minerrtmp;
			for (newpos=0;newpos+1<token_str.length();newpos++) {
				if (parent_dialog->text.raw[strp] == OPCODE_CHAR) {
					if (code_index > 0 && codep + 1 == code_index)
						strp += 2 * code[1];
					else
						strp += parent_dialog->text.code_arg_length[codep];
					codep++;
				}
				caretx += dc.GetTextExtent(token_str.Mid(newpos,1)).GetWidth();
				minerrtmp = abs(mpos.x-caretx);
				if (minerrtmp<minerr)
					minerr = minerrtmp;
				else
					break;
				if (parent_dialog->text.raw[strp]==CHARMAP_A_CODECHAR || parent_dialog->text.raw[strp]==CHARMAP_B_CODECHAR || parent_dialog->text.raw[strp]==CHARMAP_EXT_CODECHAR)
					strp += 2;
				else
					strp++;
			}
			newpos = max(2 * code[1], (int)strp);
			code[2*token_sel+2] = newpos & 0xFF;
			code[2*token_sel+3] = (newpos >> 8) & 0xFF;
		}
		wxClientDC dcp(m_tokenizetextpanel);
		DisplayTokens(dcp);
	}
}

void TextOpcodeDialog::OnTokenPaint(wxPaintEvent& event) {
	wxPaintDC dc(m_tokenizetextpanel);
	DisplayTokens(dc);
}

//==================================//
//             Steam                //
//==================================//
static map<wxString,wxString> GTE_BUTTONMAPJOY = { // From FF9UIDataTool
	{ L"START", L"joystick_start" },
	{ L"SELECT", L"joystick_select" },
	{ L"UP", L"ps_dpad_up" },
	{ L"DOWN", L"ps_dpad_down" },
	{ L"LEFT", L"ps_dpad_left" },
	{ L"RIGHT", L"ps_dpad_right" },
	{ L"CROSS", L"joystick_button_a" },
	{ L"SQUARE", L"joystick_button_x" },
	{ L"CIRCLE", L"joystick_button_b" },
	{ L"TRIANGLE", L"joystick_button_y" },
	{ L"L1", L"joystick_l1" },
	{ L"L2", L"joystick_l2" },
	{ L"R1", L"joystick_r1" },
	{ L"R2", L"joystick_r2" },
	{ L"PAD", L"ps_dpad" }
};
static map<wxString,wxString> GTE_BUTTONMAPPS = {
	{ L"START", L"ps_start" },
	{ L"SELECT", L"ps_select" },
	{ L"UP", L"ps_dpad_up" },
	{ L"DOWN", L"ps_dpad_down" },
	{ L"LEFT", L"ps_dpad_left" },
	{ L"RIGHT", L"ps_dpad_right" },
	{ L"CROSS", L"ps_cross" },
	{ L"SQUARE", L"ps_square" },
	{ L"CIRCLE", L"ps_circle" },
	{ L"TRIANGLE", L"ps_triangle" },
	{ L"L1", L"ps_l1" },
	{ L"L2", L"ps_l2" },
	{ L"R1", L"ps_r1" },
	{ L"R2", L"ps_r2" },
	{ L"PAD", L"ps_dpad" }
};
static map<int,wxString> GTE_ICONSPECIALTEXT = { // From NGUIText
	{ 34, L"0" },
	{ 35, L"1" },
	{ 39, L"5" },
	{ 45, L"/" },
	{ 159, L"Miss" },
	{ 160, L"Death" },
	{ 161, L"Guard" },
	{ 163, L"MP" },
	{ 173, L"9" },
	{ 174, L"/" },
	{ 179, L"Critical" }
};

void UnkSteamTextOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
}
void StrtOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (args.CountTokens()<2) {
		dialog->bubble_size_x = -1;
		dialog->bubble_size_y = -1;
	} else {
		dialog->bubble_size_x = wxAtoi(args.GetNextToken());
		dialog->bubble_size_y = wxAtoi(args.GetNextToken());
	}
	dialog->preview_ctrl->Refresh();
}
void ChooOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (op==2) {
		dialog->choice_line = dialog->line_num;
		dialog->choice_pos = dialog->choice_line;
		dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[_(L"cursor_hand_choice")],true));
	}
}
void EndnOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
}
void ImmeOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	dialog->display_immediate = op==6;
	while (dialog->display_immediate && dialog->ProcessPreviewText()) {}
}
void SpedOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	int speed = wxAtoi(args.GetNextToken());
	if (speed<0)	dialog->wait = DEFAULT_SPEED;
	else			dialog->wait = speed!=0 ? 320/speed : 0xFFFF;
	dialog->timer->Start(dialog->wait);
}
void PartySteamOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	dialog->preview_ctrl->WriteText(HADES_STRING_CHARACTER_DEFAULT_NAME[op-8]);
}
void HshdOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
//	dialog->preview_style.SetFlags(dialog->preview_style.GetFlags() | wxTEXT_ATTR_EFFECTS);
	if (op==20) {
		dialog->preview_effectflags |= wxTEXT_ATTR_EFFECT_SHADOW; // Not working in the wx API
		dialog->preview_effects |= wxTEXT_ATTR_EFFECT_SHADOW;
	} else {
		dialog->preview_effectflags &= ~wxTEXT_ATTR_EFFECT_SHADOW;
		dialog->preview_effects &= ~wxTEXT_ATTR_EFFECT_SHADOW;
	}
	dialog->UpdatePreviewStyle();
	dialog->preview_ctrl->BeginStyle(dialog->preview_style);
}
void DbtnOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (op==22) {
		wxString buttonid = args.GetNextToken();
		dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[GTE_BUTTONMAPJOY[buttonid]],true));
	} else if (op==25) {
		wxString buttonid = args.GetNextToken();
		dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[GTE_BUTTONMAPJOY[buttonid]],true));
	} else if (op==50) {
	} else if (op==51) {
		wxString buttonid = args.GetNextToken();
		dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[GTE_BUTTONMAPJOY[buttonid]],true));
	}
}
void IconOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (op==26) {
		dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[_(L"icon_new_exclamation")],true));
		dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[_(L"icon_new_text")],true));
	} else if (op==38) {
		int iconid = wxAtoi(args.GetNextToken());
		unsigned int i;
		for (i=0;i<G_N_ELEMENTS(SteamIconAtlas);i++)
			if (iconid==SteamIconAtlas[i].id) {
				dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[_(SteamIconAtlas[i].name)],true));
				break;
			}
		if (i>=G_N_ELEMENTS(SteamIconAtlas)) {
			auto it = GTE_ICONSPECIALTEXT.find(iconid);
			if (it!=GTE_ICONSPECIALTEXT.end())
				dialog->preview_ctrl->WriteText(it->second);
		}
	} else if (op==48) {
		int iconid = wxAtoi(args.GetNextToken());
		for (unsigned int i=0;i<G_N_ELEMENTS(SteamIconAtlas);i++)
			if (iconid==SteamIconAtlas[i].id) {
				dialog->preview_ctrl->WriteImage(InitButton(dialog->menuui->steam_atlas[0].sprite[_(SteamIconAtlas[i].name)],true));
				break;
			}
	}
}
void MoveOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	int i, val; // A normal space is of the size of [FEED=4] ; consider using hair space? (L'\x200A')
	if (op==27) {
		val = wxAtoi(args.GetNextToken());
		for (i=0;i*4<val;i++)
			dialog->preview_ctrl->WriteText(_(L" "));
	} else if (op==34) {
		val = wxAtoi(args.GetNextToken());
		for (i=0;i*4<val;i++)
			dialog->preview_ctrl->WriteText(_(L" "));
	} else if (op==35) {
		wxString lastline = dialog->preview_ctrl->GetValue().AfterLast(L'\n');
		wxFont textfont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _(L"Arial") );
		wxClientDC dc(dialog);
		dc.SetFont(textfont);
		int curpos = dc.GetTextExtent(lastline).GetWidth();
		val = wxAtoi(args.GetNextToken());
		while (curpos<val) {
			dialog->preview_ctrl->WriteText(_(L" "));
			curpos += 4;
		}
	} else if (op==36) {
	} else if (op==37) {}
}
void TokenTextOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (op==29)			dialog->preview_ctrl->WriteText(_(L"Token"));
	else if (op==44)	{}
}
void VarSteamOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (op==30)			dialog->preview_ctrl->WriteText(_(L"Item"));
	else if (op==32)	dialog->preview_ctrl->WriteText(_(L"0"));
}
void WaitOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	dialog->timer->Start(wxAtoi(args.GetNextToken())*40);
	dialog->must_reset_timer = true;
}
void TailOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
}
void WidthOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
}
void CentOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	dialog->preview_align = wxTEXT_ALIGNMENT_CENTER;
	dialog->UpdatePreviewStyle();
	dialog->preview_ctrl->BeginStyle(dialog->preview_style);
}
void PageOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	dialog->timer->Start(PAGE_BREAK_WAIT);
	dialog->must_reset_timer = true;
	dialog->must_clear_text = true;
}
void TextTagOp(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) {
	if (op==52)			{
		dialog->preview_url = args.GetNextToken();
		dialog->preview_has_url = true;
	} else if (op==53)	dialog->preview_fontweight = wxFONTWEIGHT_BOLD;
	else if (op==54)	dialog->preview_fontstyle = wxFONTSTYLE_ITALIC;
	else if (op==55)	dialog->preview_underlined = true;
	else if (op==56)	dialog->preview_striked = true;
	else if (op==57)	dialog->ignore_color = true;
	else if (op==58)	{
//		dialog->preview_ctrl->WriteText(_(L"_"));
//		dialog->preview_style.SetFlags(dialog->preview_style.GetFlags() | wxTEXT_ATTR_EFFECTS);
		dialog->preview_effectflags |= wxTEXT_ATTR_EFFECT_SUBSCRIPT;
		dialog->preview_effects |= wxTEXT_ATTR_EFFECT_SUBSCRIPT;
	} else if (op==59)	{
//		dialog->preview_ctrl->WriteText(_(L"^"));
//		dialog->preview_style.SetFlags(dialog->preview_style.GetFlags() | wxTEXT_ATTR_EFFECTS);
		dialog->preview_effectflags |= wxTEXT_ATTR_EFFECT_SUPERSCRIPT;
		dialog->preview_effects |= wxTEXT_ATTR_EFFECT_SUPERSCRIPT;
	} else if (op==60)	dialog->preview_has_url = false;
	else if (op==61)	dialog->preview_fontweight = wxFONTWEIGHT_NORMAL;
	else if (op==62)	dialog->preview_fontstyle = wxFONTSTYLE_NORMAL;
	else if (op==63)	dialog->preview_underlined = false;
	else if (op==64)	dialog->preview_striked = false;
	else if (op==65)	dialog->ignore_color = false;
	else if (op==66)	{
//		dialog->preview_style.SetFlags(dialog->preview_style.GetFlags() | wxTEXT_ATTR_EFFECTS);
		dialog->preview_effectflags &= ~wxTEXT_ATTR_EFFECT_SUBSCRIPT;
		dialog->preview_effects &= ~wxTEXT_ATTR_EFFECT_SUBSCRIPT;
	} else if (op==67)	{
//		dialog->preview_style.SetFlags(dialog->preview_style.GetFlags() | wxTEXT_ATTR_EFFECTS);
		dialog->preview_effectflags &= ~wxTEXT_ATTR_EFFECT_SUPERSCRIPT;
		dialog->preview_effects &= ~wxTEXT_ATTR_EFFECT_SUPERSCRIPT;
	}
	dialog->UpdatePreviewStyle();
	dialog->preview_ctrl->BeginStyle(dialog->preview_style);
}

void (*TextSteamOp[60])(TextSteamEditDialog* dialog, int op, wxStringTokenizer& args) = {
	/*STRT*/ &StrtOp,		&UnkSteamTextOp,&ChooOp,		&EndnOp,		&ImmeOp,		&UnkSteamTextOp,&ImmeOp,		&SpedOp,
	/*ZDNE*/ &PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,	&PartySteamOp,
	/*HSHD*/ &HshdOp,		&HshdOp,		&DbtnOp,		&UnkSteamTextOp,&UnkSteamTextOp,&DbtnOp,		&IconOp,		&MoveOp,		&EndnOp,
	/*TEXT*/ &TokenTextOp,	&VarSteamOp,	&UnkSteamTextOp,&VarSteamOp,	&WaitOp,		&MoveOp,		&MoveOp,		&MoveOp,		&MoveOp,		&IconOp,
	/*PCHC*/ &ChooOp,		&ChooOp,		&UnkSteamTextOp,&UnkSteamTextOp,&TailOp,		&TokenTextOp,	&WidthOp,		&CentOp,		&PageOp,		&IconOp,		&UnkSteamTextOp,&DbtnOp,		&DbtnOp,
	/*url */ &TextTagOp,	&TextTagOp,		&TextTagOp,		&TextTagOp,		&TextTagOp,		&TextTagOp,		&TextTagOp,		&TextTagOp
};

TextSteamEditDialog::TextSteamEditDialog(wxWindow* parent, MenuUIDataSet* ui, FF9String& str, int style) :
	TextSteamEditWindow(parent),
	TextEditDialogBase(str,new wxTimer(this),m_richtextctrl,style),
	menuui(ui),
	must_clear_text(false),
	ignore_color(false),
	bubble_size_x(-1),
	bubble_size_y(-1),
	help_dial(NULL) {
	SteamLanguage lang;
	unsigned int i = 0;
	for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
		if (lang!=GetSteamLanguage())
			multilang[lang<GetSteamLanguage() ? lang : lang-1] = lang;
	has_multilang = !hades::STEAM_SINGLE_LANGUAGE_MODE;
	multilangname[0] = m_langname1;		multilangname[1] = m_langname2;		multilangname[2] = m_langname3;
	multilangname[3] = m_langname4;		multilangname[4] = m_langname5;		multilangname[5] = m_langname6;
	multilangctrl[0] = m_langtext1;		multilangctrl[1] = m_langtext2;		multilangctrl[2] = m_langtext3;
	multilangctrl[3] = m_langtext4;		multilangctrl[4] = m_langtext5;		multilangctrl[5] = m_langtext6;
	multilangbtn[0] = m_langtranslate1;	multilangbtn[1] = m_langtranslate2;	multilangbtn[2] = m_langtranslate3;
	multilangbtn[3] = m_langtranslate4;	multilangbtn[4] = m_langtranslate5;	multilangbtn[5] = m_langtranslate6;
	m_langtranslateall->Enable(false); // ToDo
	if (has_multilang) {
		for (i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++) {
			multilangname[i]->SetLabelText(HADES_STRING_STEAM_LANGUAGE_LONG_NAME[multilang[i]]);
			if (text.multi_lang_init[multilang[i]]) {
				multilangbtn[i]->SetLabelText(_(HADES_STRING_TEXT_TRANSLATE));
				multilangctrl[i]->ChangeValue(text.multi_lang_str[multilang[i]]);
				multilangbtn[i]->Enable(false); // ToDo
			} else {
				multilangbtn[i]->SetLabelText(_(HADES_STRING_GENERIC_ADD));
				multilangname[i]->Enable(false);
				multilangctrl[i]->Enable(false);
			}
		}
		wxImage tmpimg = wxBITMAP(bulletdown_image).ConvertToImage();
		m_multilangshowimg->SetBitmap(wxBitmap(tmpimg));
	} else {
		m_langtranslateall->Hide();
		for (i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++) {
			multilangname[i]->Hide();
			multilangctrl[i]->Hide();
			multilangbtn[i]->Hide();
		}
		wxImage tmpimg = wxBITMAP(bulletright_image).ConvertToImage();
		m_multilangshowimg->SetBitmap(wxBitmap(tmpimg));
		m_multilangshowimg->Enable(false);
		m_multilangtitle->Enable(false);
	}
/*	wxRichTextCharacterStyleDefinition urlcharstyle;
	wxRichTextAttr urltextattrib = preview_ctrl->GetDefaultStyle();
	wxRichTextStyleSheet* previewstylesheet = preview_ctrl->GetStyleSheet();
	if (previewstylesheet==NULL) {
		previewstylesheet = new wxRichTextStyleSheet();
		preview_ctrl->SetStyleSheet(previewstylesheet);
	}
	urltextattrib.SetFontUnderlined(true);
//	urltextattrib.SetTextColour(*wxBLUE);
	urlcharstyle.SetStyle(urltextattrib);
	urlcharstyle.SetName(L"URL_Style");
	previewstylesheet->AddCharacterStyle(&urlcharstyle);*/
	Connect(wxEVT_TIMER,wxTimerEventHandler(TextSteamEditDialog::OnTimer),NULL,this);
}

TextSteamEditDialog::~TextSteamEditDialog() {
	Disconnect(wxEVT_TIMER,wxTimerEventHandler(TextSteamEditDialog::OnTimer),NULL,this);
	delete timer;
	if (help_dial!=NULL)
		help_dial->Destroy();
}

int TextSteamEditDialog::ShowModal() {
	m_buttonok->SetFocus();
	m_textctrl->ChangeValue(_(text.str));
	PreviewText();
	return wxDialog::ShowModal();
}

bool TextSteamEditDialog::ProcessPreviewText() {
	wstring::size_type len = text.str.length();
	bool printnextchar = true;
	if (must_clear_text) {
		preview_ctrl->Clear();
		must_clear_text = false;
	}
	preview_ctrl->SetInsertionPointEnd();
	UpdatePreviewStyle();
	preview_ctrl->BeginStyle(preview_style);
	while (str_pos<len && text.str[str_pos]==L'[') {
		wstring::size_type opcodelen;
		str_pos++;
		for (opcodelen=0;str_pos+opcodelen<len && text.str[str_pos+opcodelen]!=L'\n';opcodelen++)
			if (text.str[str_pos+opcodelen]==L']')
				break;
		if (str_pos+opcodelen<len && text.str[str_pos+opcodelen]!=L'\n') {
			wxStringTokenizer opcodeargs(_(text.str.substr(str_pos,opcodelen)),_(L"=,"));
			wxString opcode = opcodeargs.GetNextToken();
			bool iscolor = false;
			unsigned int i;
			str_pos += opcodelen+1;
			if (opcodeargs.CountTokens()==0) {
				if (opcode.Len()==6 || opcode.Len()==8) {
					for (i=0;i<opcode.Len();i++)
						if (!(opcode[i]>=L'0' && opcode[i]<=L'9') && !(opcode[i]>=L'A' && opcode[i]<=L'F') && !(opcode[i]>=L'a' && opcode[i]<=L'f'))
							break;
					iscolor = i>=opcode.Len();
					if (iscolor) {
						stringstream hexa;
						unsigned char r,g,b;
						unsigned char a = 0xFF;
						r = wxHexToDec(opcode.Mid(0,2));
						g = wxHexToDec(opcode.Mid(2,2));
						b = wxHexToDec(opcode.Mid(4,2));
						if (opcode.Len()==8)
							a = wxHexToDec(opcode.Mid(6,2));
						preview_style.SetTextColour(wxColour(r,g,b,a));
						preview_ctrl->BeginStyle(preview_style);
					}
				}
			}
			if (!iscolor)
				for (i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_STEAM_OPCODE);i++)
					if (opcode.IsSameAs(_(HADES_STRING_TEXT_STEAM_OPCODE[i].id))) {
						TextSteamOp[i](this,i,opcodeargs);
						break;
					} else if (i>=TEXT_STEAM_OPCODE_CLOSING_TAG && opcode.IsSameAs(_(L"/")+_(HADES_STRING_TEXT_STEAM_OPCODE[i].id))) {
						TextSteamOp[i](this,i-TEXT_STEAM_OPCODE_CLOSING_TAG+G_N_ELEMENTS(HADES_STRING_TEXT_STEAM_OPCODE),opcodeargs);
						break;
					}
			printnextchar = !must_reset_timer;
		} else
			preview_ctrl->WriteText(_(text.str[str_pos-1]));
	}
	if (str_pos>=len) {
		if (!must_reset_timer)
			timer->Stop();
		return false;
	}
	if (printnextchar) {
		preview_ctrl->WriteText(_(text.str[str_pos++]));
		if (text.str[str_pos-1]==L'\n') {
			preview_align = wxTEXT_ALIGNMENT_LEFT;
			line_num++;
		}
	}
	preview_ctrl->EndAllStyles();
	if (str_pos>=len && !must_reset_timer)
		timer->Stop();
	return str_pos<len;
}

void TextSteamEditDialog::PreviewText() {
	preview_ctrl->Clear();
	preview_align = wxTEXT_ALIGNMENT_LEFT;
	preview_fontweight = wxFONTWEIGHT_NORMAL;
	preview_fontstyle = wxFONTSTYLE_NORMAL;
	preview_underlined = false;
	preview_striked = false;
	preview_has_url = false;
	preview_attrflags = wxTEXT_ATTR_TEXT_COLOUR | wxTEXT_ATTR_FONT_WEIGHT | wxTEXT_ATTR_FONT_ITALIC | wxTEXT_ATTR_FONT_UNDERLINE | wxTEXT_ATTR_FONT_STRIKETHROUGH | wxTEXT_ATTR_ALIGNMENT | wxTEXT_ATTR_EFFECTS;
	preview_effectflags = wxTEXT_ATTR_EFFECT_NONE;
	preview_effects = wxTEXT_ATTR_EFFECT_NONE;
	if (text_style==TEXT_STYLE_DEFAULT)
		preview_style.SetTextColour(wxColour(255,255,255));
	else
		preview_style.SetTextColour(wxColour(0,0,0));
	str_pos = 0;
	choice_pos = 0;
	choice_line = -1;
	line_num = 0;
	wait = DEFAULT_SPEED;
	timer->Start(wait);
}

int TextSteamEditDialog::GetBubbleSizeX() {
	if (bubble_size_x<0)
		return -1;
	return (int)bubble_size_x*PIXEL_TO_SIZE;
}

int TextSteamEditDialog::GetBubbleSizeY() {
	if (bubble_size_y<0)
		return -1;
	return bubble_size_y*LINE_HEIGHT+4;
}

void TextSteamEditDialog::CalculateBestSize(SteamLanguage lang) {
	unsigned int i, j, multilangindex;
	bool addwdth = false;
	wxString textstr;
	if (lang==GetSteamLanguage()) {
		textstr = m_textctrl->GetValue();
	} else {
		for (i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++)
			if (multilang[i]==lang) {
				multilangindex = i;
				break;
			}
		if (i+1>=STEAM_LANGUAGE_AMOUNT)
			return;
		textstr = multilangctrl[multilangindex]->GetValue();
	}
	unsigned int linenum = 0, linecount = 0;
	vector<uint16_t> sizex(1,0);
	vector< vector<int> > sizedop(1);
	uint16_t currentsizex = 0;
	size_t opcodelen;
	// DEBUG: Arial font size is not a very good approximation of FF9 font, especially for japanese characters
	wxFont textfont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, _(L"Arial") );
	wxClientDC dc(this);
	dc.SetFont(textfont);
	for (i=0;i<textstr.Len();i++) {
		if (textstr[i]==L'[') {
			i++;
			for (opcodelen=0;i+opcodelen<textstr.Len();opcodelen++)
				if (textstr[i+opcodelen]==L']' || textstr[i+opcodelen]==L'\n')
					break;
			if (i+opcodelen<textstr.Len() && textstr[i+opcodelen]==L']') {
				wxStringTokenizer opcodeargs(textstr.Mid(i,opcodelen),_(L"=,"));
				wxString opcode = opcodeargs.GetNextToken();
				i += opcodelen;
				for (j=0;j<G_N_ELEMENTS(HADES_STRING_TEXT_STEAM_OPCODE);j++)
					if (opcode.IsSameAs(_(HADES_STRING_TEXT_STEAM_OPCODE[j].id))) {
						if (j>=8 && j<=19) {
							sizedop[linenum].push_back(8+j);
						} else if (j==22 || j==25 || j==50 || j==51) {
							// ToDo: BTN
						} else if (j==26) {
							sizedop[linenum].push_back(112);
						} else if (j==27 || j==34) {
							currentsizex += wxAtoi(opcodeargs.GetNextToken());
						} else if (j==29) {
							opcodeargs.GetNextToken();
							sizedop[linenum].push_back(6);
							sizedop[linenum].push_back(wxAtoi(opcodeargs.GetNextToken()));
						} else if (j==30) {
							sizedop[linenum].push_back(14);
							sizedop[linenum].push_back(wxAtoi(opcodeargs.GetNextToken()));
						} else if (j==32) {
							sizedop[linenum].push_back(64);
							sizedop[linenum].push_back(wxAtoi(opcodeargs.GetNextToken()));
						} else if (j==35) {
							sizex[linenum] = max(sizex[linenum],currentsizex);
							currentsizex = wxAtoi(opcodeargs.GetNextToken());
						} else if (j==38 || j==48) {
							// ToDo: ICON/MOBI
						} else if (j==45) {
							addwdth = true;
						} else if (j==47) {
							sizex[linenum] = max(sizex[linenum],currentsizex);
							currentsizex = 0;
							linenum = 0;
						} else if (j==53) {
							textfont.SetWeight(wxFONTWEIGHT_BOLD);
						} else if (j==54) {
							textfont.SetStyle(wxFONTSTYLE_ITALIC);
						}
						break;
					} else if (j>=TEXT_STEAM_OPCODE_CLOSING_TAG && opcode.IsSameAs(_(L"/")+_(HADES_STRING_TEXT_STEAM_OPCODE[j].id))) {
						if (j==53)
							textfont.SetWeight(wxFONTWEIGHT_NORMAL);
						else if (j==54)
							textfont.SetStyle(wxFONTSTYLE_NORMAL);
						break;
					}
			} else {
				i--;
				currentsizex += dc.GetTextExtent(textstr[i]).GetWidth();
			}
		} else if (textstr[i]==L'\n') {
			sizex[linenum] = max(sizex[linenum],currentsizex);
			currentsizex = 0;
			linenum++;
			linecount = max(linenum,linecount);
			sizex.resize(linecount+1,0);
			sizedop.resize(linecount+1);
		} else {
			currentsizex += dc.GetTextExtent(textstr[i]).GetWidth();
		}
	}
	sizex[linenum] = max(sizex[linenum],currentsizex);
	linecount = max(linenum+1,linecount);
	size_t posstr;
	if (textstr.Len()>=5 && textstr.Mid(0,5).IsSameAs(_(L"[STRT"))) {
		size_t endofsizeop = 5;
		for (posstr=5;posstr<textstr.Len();posstr++)
			if (textstr[posstr]==L']') {
				endofsizeop = posstr+1;
				break;
			}
		textstr = textstr.Mid(endofsizeop);
	}
	if (addwdth) {
		for (i=0;i+5<textstr.Len();i++)
			if (textstr.Mid(i,5).IsSameAs(_(L"[WDTH"))) {
				size_t endofsizeop = 5;
				for (posstr=i+5;posstr<textstr.Len();posstr++)
					if (textstr[posstr]==L']') {
						endofsizeop = posstr+1;
						break;
					}
				textstr = textstr.Mid(0,i)+textstr.Mid(endofsizeop);
				break;
			}
		wxString wdthcode = _(L"[WDTH=");
		bool emptywdth = true;
		for (linenum=0;linenum<linecount;linenum++)
			if (sizedop[linenum].size()>0) {
				if (emptywdth)
					emptywdth = false;
				else
					wdthcode += _(L",");
				wdthcode += wxString::Format(wxT("%d,%d,"),linenum,sizex[linenum]);
				for (i=0;i<sizedop[linenum].size();i++)
					wdthcode += wxString::Format(wxT("%d,"),sizedop[linenum][i]);
				wdthcode += _(L"-1");
			}
		if (!emptywdth)
			textstr = wdthcode+_(L"]")+textstr;
	}
	for (linenum=0;linenum<linecount;linenum++)
		currentsizex = max(currentsizex,sizex[linenum]);
	textstr.Printf(wxT("[STRT=%d,%d]%s"),currentsizex,linecount,textstr); // DEBUG: If the x size is too small, the game computes the right size anyway...
	if (lang==GetSteamLanguage())
		m_textctrl->SetValue(textstr);
	else
		multilangctrl[multilangindex]->SetValue(textstr);
}

void TextSteamEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_SET) {
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
//		m_textctrl->ChangeValue(_(text.str));
		PreviewText();
	} else if (id==wxID_BUBBLE) {
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
		CalculateBestSize();
		if (has_multilang && m_langtranslateall->IsShown())
			for (unsigned int i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++)
				if (multilangctrl[i]->IsEnabled())
					CalculateBestSize(multilang[i]);
	} else if (id==wxID_HELP) {
		if (help_dial==NULL || !help_dial->IsShown()) {
			help_dial = new TextSteamHelpDialog(this);
			help_dial->Show();
		} else {
			help_dial->SetFocus();
		}
	} else if (id==wxID_TRANSLATE) {
		wxButton* clicked = static_cast<wxButton*>(event.GetEventObject());
		for (unsigned int i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++)
			if (clicked==multilangbtn[i] || (clicked==m_langtranslateall && multilangctrl[i]->IsEnabled())) {
				SteamLanguage lang = multilang[i];
				if (!multilangctrl[i]->IsEnabled()) {
					multilangbtn[i]->SetLabelText(_(HADES_STRING_TEXT_TRANSLATE));
					multilangbtn[i]->Refresh();
					multilangbtn[i]->Layout();
					multilangbtn[i]->Enable(false); // ToDo
					multilangctrl[i]->ChangeValue(_(L"[STRT=0,1]"));
					multilangctrl[i]->Enable(true);
					multilangname[i]->Enable(true);
					text.SetValue(L"[STRT=0,1]",lang);
				} else {
/*					wxString translatestr = _(L"");
					int yandexresult = Yandex::Translate(m_textctrl->GetValue(),GetSteamLanguage(),lang,&translatestr);
					wxMessageDialog popup(NULL,translatestr,wxString::Format(wxT("Result: %d"),yandexresult),wxOK|wxCENTRE);
					popup.ShowModal();*/
				}
			}
	} else {
		unsigned int i;
		text.SetValue(m_textctrl->GetValue().ToStdWstring());
		for (i=0;i+1<STEAM_LANGUAGE_AMOUNT && multilang[i]!=STEAM_LANGUAGE_NONE;i++)
			text.multi_lang_str[multilang[i]] = multilangctrl[i]->GetValue().ToStdWstring();
		EndModal(id);
	}
}

void TextSteamEditDialog::OnShowHideMultiLang(wxMouseEvent& event) {
	if (!m_multilangtitle->IsEnabled()) {
		event.Skip();
		return;
	}
	unsigned int i;
	if (m_langtranslateall->IsShown()) {
		m_langtranslateall->Hide();
		for (i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++) {
			multilangname[i]->Hide();
			multilangctrl[i]->Hide();
			multilangbtn[i]->Hide();
		}
		wxImage tmpimg = wxBITMAP(bulletright_image).ConvertToImage();
		m_multilangshowimg->SetBitmap(wxBitmap(tmpimg));
	} else {
		m_langtranslateall->Show();
		for (i=0;i+1<STEAM_LANGUAGE_AMOUNT;i++) {
			multilangname[i]->Show();
			multilangctrl[i]->Show();
			multilangbtn[i]->Show();
		}
		wxImage tmpimg = wxBITMAP(bulletdown_image).ConvertToImage();
		m_multilangshowimg->SetBitmap(tmpimg);
	}
	Layout();
	Refresh();
	event.Skip();
}

void TextSteamEditDialog::OnTimer(wxTimerEvent& event) {
	if (must_reset_timer) {
		must_reset_timer = false;
		timer->Start(wait);
	}
	ProcessPreviewText();
}

TextSteamHelpDialog::TextSteamHelpDialog(TextSteamEditDialog * p) :
	TextSteamHelpWindow(p),
	parent(p) {
	wxArrayString funclist;
	funclist.Alloc(G_N_ELEMENTS(HADES_STRING_TEXT_STEAM_OPCODE));
	for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_STEAM_OPCODE);i++)
		funclist.Add(_(HADES_STRING_TEXT_STEAM_OPCODE[i].id));
	m_listfunction->Append(funclist);
}

TextSteamHelpDialog::~TextSteamHelpDialog() {
	parent->help_dial = NULL;
}

void TextSteamHelpDialog::OnListClick(wxCommandEvent& event) {
	m_helptextctrl->ChangeValue(HADES_STRING_TEXT_STEAM_OPCODE[event.GetSelection()].help);
}

void TextSteamHelpDialog::OnListDoubleClick(wxCommandEvent& event) {
	SortedChoiceItemTextSteamOpcode& opitem = HADES_STRING_TEXT_STEAM_OPCODE[event.GetSelection()];
	wxString opstr = _(L"[")+_(opitem.id);
	if (opitem.arg_amount==-1) {
		if (_(L"WDTH").IsSameAs(opitem.id))
			opstr += _(L"=0,0,-1]");
		else
			opstr += _(L"]");
	} else if (opitem.arg_amount==0) {
		opstr += _(L"]");
	} else {
		opstr += _(L"=");
		if (_(L"TAIL").IsSameAs(opitem.id))
			opstr += _(L"DEFT");
		else if (_(L"DBTN").IsSameAs(opitem.id) || _(L"CBTN").IsSameAs(opitem.id) || _(L"KCBT").IsSameAs(opitem.id) || _(L"JCBT").IsSameAs(opitem.id))
			opstr += _(L"START");
		else if (_(L"url").IsSameAs(opitem.id))
			opstr += _(L"http://www.example.com");
		else
			for (unsigned int i=0;i<opitem.arg_amount;i++)
				opstr += (i>0 ? _(L",0") : _(L"0"));
		opstr += _(L"]");
	}
	if (event.GetSelection()>=TEXT_STEAM_OPCODE_CLOSING_TAG) {
		opstr += _(L"[/")+_(opitem.id)+_(L"]");
	}
	parent->m_textctrl->WriteText(opstr);
}

//==================================//
//             Shared               //
//==================================//
PreviewTextCtrl::PreviewTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, long style) :
	wxRichTextCtrl(parent,id,value,pos,size,style),
	parent_dialog(NULL) {
}

void PreviewTextCtrl::PaintBackground(wxDC& dc) {
	unsigned int r = 128, g = 128, b = 128;
	if (parent_dialog!=NULL) {
		if (parent_dialog->text_style==TEXT_STYLE_DEFAULT) {
			r = 48;  g = 48;  b = 48;
		} else if (parent_dialog->text_style==TEXT_STYLE_HELP) {
			r = 208; g = 208; b = 208;
		} else {
			r = 128; g = 128; b = 128;
		}
	}
	dc.SetBrush(wxBrush(wxColour(r,g,b)));
	dc.SetPen(wxPen(wxColour(r,g,b),0));
	dc.DrawRectangle(0,0,1500,1500);
	if (parent_dialog==NULL)
		return;
	int bubblex = parent_dialog->GetBubbleSizeX();
	int bubbley = parent_dialog->GetBubbleSizeY();
	if (bubblex>0 && bubbley>0) {
		if (parent_dialog->text_style==TEXT_STYLE_DEFAULT)
			dc.SetBrush(wxBrush(hades::TEXT_WINDOW_COLOR));
		else if (parent_dialog->text_style==TEXT_STYLE_HELP)
			dc.SetBrush(wxBrush(wxColour(40,60,130)));
		else
			dc.SetBrush(wxBrush(wxColour(90,90,90)));
		dc.SetPen(wxPen(wxColour(255,255,255),1));
		dc.DrawRoundedRectangle(2,2,2+bubblex,2+bubbley,5);
	}
}

void TextExportDialog::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}
