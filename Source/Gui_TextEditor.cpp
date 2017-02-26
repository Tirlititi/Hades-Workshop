#include "Gui_TextEditor.h"

#include <cstdarg>
#include "Database_Text.h"
#include "Hades_Strings.h"
#include "main.h"

#define DEFAULT_SPEED 5
#define NEW_PAGE_CHAR L'¶'

wxBitmap InitButton(wxBitmap bmp) {
	wxImage tmpimg = bmp.ConvertToImage();
	bmp = wxBitmap(tmpimg.Rescale(tmpimg.GetWidth()*0.4,tmpimg.GetHeight()*0.4,wxIMAGE_QUALITY_HIGH));
	return bmp;
}
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
void UnknownTextOp(TextEditDialog* dialog, uint8_t* args) {
}
void SpeedTextOp(TextEditDialog* dialog, uint8_t* args) {
	if (args[0]==0x02)
		dialog->wait = args[1]>0 ? 320/args[1] : 0xFFFF;
	else if (args[0]==0x0C)
		while (dialog->ProcessPreviewText()) {}
	else if (args[0]==0x41)
		Sleep(args[1]*40);
	else if (args[0]==0x42)
		dialog->wait = DEFAULT_SPEED;
	dialog->timer->Start(dialog->wait);
}
void PartyNTextOp(TextEditDialog* dialog, uint8_t* args) {
	static wxString GTE_PARTYNAMES[] = { L"Zidane", L"Vivi", L"Dagger", L"Steiner", L"Freya", L"Quina", L"Eiko", L"Tarask", L"Zidane", L"Vivi", L"Steiner", L"Dagger" };
	dialog->m_richtextctrl->WriteText(GTE_PARTYNAMES[args[0]-0x10]);
}
void ColorTextOp(TextEditDialog* dialog, uint8_t* args) {
	static uint8_t GTE_COLORS[][3] = { {255,255,255}, {220,220,220}, {127,127,127}, {255,0,0}, {255,255,0}, {0,127,255}, {255,0,127}, {0,255,0}, {0,0,0}, {255,0,0} };
	dialog->text_colour.Set(GTE_COLORS[args[0]-0x20][0],GTE_COLORS[args[0]-0x20][1],GTE_COLORS[args[0]-0x20][2]);
	dialog->m_richtextctrl->BeginTextColour(dialog->text_colour);
}
void VarTextOp(TextEditDialog* dialog, uint8_t* args) {
	if (args[0]==0x06)
		dialog->m_richtextctrl->WriteText(_("TOKEN"));
	else if (args[0]==0x0E)
		dialog->m_richtextctrl->WriteText(_("ITEM"));
	else if (args[0]==0x40)
		dialog->m_richtextctrl->WriteText(_("0"));
	else if (args[0]==0x68)
		dialog->m_richtextctrl->WriteText(_("0"));
	else if (args[0]==0x69)
		dialog->m_richtextctrl->WriteText(_("TEXT"));
	else if (args[0]==0x6C)
		dialog->m_richtextctrl->WriteText(_("0"));
}
void ButtonOp(TextEditDialog* dialog, uint8_t* args) {
	static wxBitmap GTE_BUTTONS[] = {
		InitButton(wxBITMAP(buttoncircle_image)), InitButton(wxBITMAP(buttoncross_image)), InitButton(wxBITMAP(buttontriangle_image)), InitButton(wxBITMAP(buttonsquare_image)),
		InitButton(wxBITMAP(buttonl1_image)), InitButton(wxBITMAP(buttonr1_image)), InitButton(wxBITMAP(buttonl2_image)), InitButton(wxBITMAP(buttonr2_image)),
		InitButton(wxBITMAP(buttonstart_image)), InitButton(wxBITMAP(buttonselect_image)),
		InitButton(wxBITMAP(buttonup_image)), InitButton(wxBITMAP(buttondown_image)), InitButton(wxBITMAP(buttonleft_image)), InitButton(wxBITMAP(buttonright_image)),
		InitButton(wxBITMAP(buttonl2_image)), InitButton(wxBITMAP(buttonr2_image)),
		InitButton(wxBITMAP(buttonpad_image))
	};
	static wxBitmap GTE_NEW = InitButton(wxBITMAP(indicatornew_image));
	if (args[0]==0x2F)
		dialog->m_richtextctrl->WriteImage(GTE_BUTTONS[0x10]);
	else if (args[0]==0x70)
		dialog->m_richtextctrl->WriteImage(GTE_NEW);
	else
		dialog->m_richtextctrl->WriteImage(GTE_BUTTONS[args[0]%0x30]);
}

void (*TextOp[256])(TextEditDialog* dialog, uint8_t* args) = {
	&UnknownTextOp,	&UnknownTextOp,	&SpeedTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&VarTextOp,		&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&SpeedTextOp,	&UnknownTextOp,	&VarTextOp,		&UnknownTextOp,
	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&PartyNTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&ColorTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&ButtonOp,
	&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,	
	&VarTextOp,		&SpeedTextOp,	&SpeedTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&ButtonOp,		&VarTextOp,		&VarTextOp,		&UnknownTextOp,	&UnknownTextOp,	&VarTextOp,		&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&ButtonOp,		&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,
	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp,	&UnknownTextOp
};

TextEditDialog::TextEditDialog(wxWindow* parent, FF9String& str, unsigned int extrasize, int style, CharmapDataStruct* chmap, CharmapDataStruct* chmapext, int sizex, int sizey, uint16_t formatamount, TextFormatStruct* format) :
	TextEditWindow(parent),
	text(str),
	charmap(chmap),
	charmap_Ext(chmapext),
	timer(new wxTimer(this)),
	text_style(style),
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
	m_richtextctrl->SetInsertionPointEnd();
	m_richtextctrl->BeginTextColour(text_colour);
	if (GetGameType()==GAME_TYPE_PSX) {
		while (str_pos<len && text.str[str_pos]==text.opcode_wchar) {
			str_pos++;
			TextOp[text.code_arg[code_pos][0]](this,text.code_arg[code_pos++]);
		}
	}
	if (str_pos<len) {
		if (text.str[str_pos]==NEW_PAGE_CHAR) {
			Sleep(500);
			m_richtextctrl->Clear();
			str_pos++;
		} else
			m_richtextctrl->WriteText(_(text.str[str_pos++]));
		if (str_pos>=len)
			timer->Stop();
	} else {
		timer->Stop();
	}
	return str_pos<len;
}

void TextEditDialog::PreviewText() {
	m_richtextctrl->Clear();
	if (text_style==TEXT_STYLE_DEFAULT)
		text_colour.Set(255,255,255);
	else
		text_colour.Set(0,0,0);
	str_pos = 0;
	choice_pos = 0;
	wait = DEFAULT_SPEED;
	code_pos = 0;
	timer->Start(wait);
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
		m_opcodelist->Append(_(HADES_STRING_TEXT_OPCODE[text.code_arg[i][0]].label));
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
			m_opcodelist->SetString(sel,_(HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount][0]].label));
			m_opcodelist->SetString(sel-1,_(HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount-1][0]].label));
			m_opcodelist->SetSelection(sel-1);
		}
	} else if (id==wxID_DOWN) {
		int sel = m_opcodelist->GetSelection();
		if (sel!=wxNOT_FOUND && sel!=text.code_amount+format_amount-1 && sel>=format_amount) {
			text.PermuteCode(sel-format_amount,sel-format_amount+1);
			m_opcodelist->SetString(sel,_(HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount][0]].label));
			m_opcodelist->SetString(sel+1,_(HADES_STRING_TEXT_OPCODE[text.code_arg[sel-format_amount+1][0]].label));
			m_opcodelist->SetSelection(sel+1);
		}
	} else if (id==wxID_ADD) {
		if (GetGameType()!=GAME_TYPE_PSX) // DEBUG
			return;
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
	ProcessPreviewText();
}

PreviewTextCtrl::PreviewTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, long style) :
	wxRichTextCtrl(parent,id,value,pos,size,style),
	parent_dialog((TextEditDialog*)parent) {
}

void PreviewTextCtrl::PaintBackground(wxDC& dc) {
	dc.SetBrush(wxBrush(wxColour(70,70,70),wxSOLID));
	dc.SetPen(wxPen(wxColour(0,0,0),0,wxSOLID));
	dc.DrawRectangle(0,0,1000,1000);
	if (parent_dialog->charmap) {
		if (parent_dialog->text_style==TEXT_STYLE_DEFAULT)
			dc.SetBrush(wxBrush(hades::TEXT_WINDOW_COLOR,wxSOLID));
		else if (parent_dialog->text_style==TEXT_STYLE_HELP)
			dc.SetBrush(wxBrush(wxColour(127,255,255),wxSOLID));
		else
			dc.SetBrush(wxBrush(wxColour(190,190,190),wxSOLID));
		dc.SetPen(wxPen(wxColour(255,255,255),1,wxSOLID));
		wxCoord width, height;
		width = (int)parent_dialog->m_sizex->GetValue()*1.1;
		height = parent_dialog->m_sizey->GetValue()*19+4;
		dc.DrawRoundedRectangle(2,2,width,height,5);
	}
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
	text = parent_dialog->text;
	str = _(text.str);
	token_str = _(text.str);
	token_str.Replace(_(L"\n"),_(L"•"));
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
		while (strp<code[2*i+2]+0x100*code[2*i+3]+2) {
			if (parent_dialog->text.raw[strp]==OPCODE_CHAR)
				strp += parent_dialog->text.code_arg_length[codep++];
			else if (parent_dialog->text.raw[strp]==CHARMAP_A_CODECHAR || parent_dialog->text.raw[strp]==CHARMAP_B_CODECHAR || parent_dialog->text.raw[strp]==CHARMAP_EXT_CODECHAR) {
				strp += 2;
				stri++;
			} else {
				strp++;
				stri++;
			}
		}
		int x = 6+dc.GetTextExtent(token_str.Mid(0,stri)).GetWidth();
		if (i==token_sel) {
			int selw = dc.GetTextExtent(token_str.Mid(stri,token_str.find(_(L"•"),stri)-stri)).GetWidth();
			dc.SetBrush(wxBrush(wxColour(255,0,0),wxSOLID));
			dc.SetPen(wxPen(wxColour(255,0,0),0,wxSOLID));
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
		int i;
		for (i=0;i<value;i++) {
			int newpos = max(0,code[2*i+2]+0x100*code[2*i+3]+2*(value-code[1]));
			code[2*i+2] = newpos % 0x100;
			code[2*i+3] = (newpos >> 8) % 0x100;
		}
		for (i=code[1];i<value;i++) {
			int newpos = 2*value;
			code[2*i+2] = newpos % 0x100;
			code[2*i+3] = (newpos >> 8) % 0x100;
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
				code[2*j+2] = strpprev%0x100;
				code[2*j+3] = strpprev/0x100;
				j--;
				strpprev = code[2*j]+0x100*code[2*j+1];
			}
			code[2*j+2] = strpmain%0x100;
			code[2*j+3] = strpmain/0x100;
		}
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
				while (strp<code[2*i+2]+0x100*code[2*i+3]+2) {
					if (parent_dialog->text.raw[strp]==OPCODE_CHAR)
						strp += parent_dialog->text.code_arg_length[codep++];
					else if (parent_dialog->text.raw[strp]==CHARMAP_A_CODECHAR || parent_dialog->text.raw[strp]==CHARMAP_B_CODECHAR || parent_dialog->text.raw[strp]==CHARMAP_EXT_CODECHAR) {
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
				if (parent_dialog->text.raw[strp]==OPCODE_CHAR)
					strp += parent_dialog->text.code_arg_length[codep++];
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
			newpos = max(2,(int)strp-2);// += 2*code[1];
			code[2*token_sel+2] = newpos % 0x100;
			code[2*token_sel+3] = (newpos >> 8) % 0x100;
		}
		wxClientDC dcp(m_tokenizetextpanel);
		DisplayTokens(dcp);
	}
}

void TextOpcodeDialog::OnTokenPaint(wxPaintEvent& event) {
	wxPaintDC dc(m_tokenizetextpanel);
	DisplayTokens(dc);
}

void TextExportDialog::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}


