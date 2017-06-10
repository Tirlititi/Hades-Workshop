#include "Gui_AssemblyEditor.h"

#include <wx/tokenzr.h>
#include "Gui_Manipulation.h"
#include "Hades_Strings.h"
#include "Database_Assembly.h"
#include "Database_Spell.h"

//=============================//
//            MIPS             //
//=============================//

#define MIPS_MAX_ARGUMENTS				4
#define MIPS_MAX_INSTRUCTION_DISPLAY	14

class MipsScriptHelpDialog : public MipsScriptHelpWindow {
public:
	MipsScriptHelpDialog(MipsScriptEditDialog* p);
	~MipsScriptHelpDialog();
	void DisplayHelp(unsigned int helpid);

private:
	MipsScriptEditDialog* parent;
	
	void OnListClick(wxCommandEvent& event);
};

MipsScriptHelpDialog::MipsScriptHelpDialog(MipsScriptEditDialog* p) :
	MipsScriptHelpWindow(p),
	parent(p) {
	unsigned int i;
	for (i=0;i<G_N_ELEMENTS(MIPS_HELP);i++)
		m_listfunction->Append(MIPS_HELP[i].label);
	m_listfunction->SetSelection(0);
	DisplayHelp(0);
}

MipsScriptHelpDialog::~MipsScriptHelpDialog() {
}

void MipsScriptHelpDialog::DisplayHelp(unsigned int helpid) {
	wxString help = MIPS_HELP[helpid].help;
	wxString effectname;
	int i,j;
	if (helpid==1) { // Battle - Function Dependency
		for (i=SPELL_EFFECT_AMOUNT-1;i>=0;i--) {
			effectname = _(L"");
			for (j=0;j<G_N_ELEMENTS(HADES_STRING_SPELL_EFFECT);j++)
				if (HADES_STRING_SPELL_EFFECT[j].id==i) {
					effectname = HADES_STRING_SPELL_EFFECT[j].label;
					break;
				}
			help.Replace(_(L"%")+wxString::Format(wxT("%d"),i),effectname);
		}
	}
	m_helptextctrl->SetValue(help);
}

void MipsScriptHelpDialog::OnListClick(wxCommandEvent& event) {
	DisplayHelp(m_listfunction->GetSelection());
}

MipsScriptSizer::MipsScriptSizer(MipsScriptEditDialog* p, unsigned int l, MipsInstruction* c) :
	wxBoxSizer(wxHORIZONTAL),
	parent(p),
	codeline(c),
	line_nb(l),
	m_arg(NULL) {
	m_code = new wxChoice(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb,wxDefaultPosition,wxDefaultSize,parent->code_list,0);
	m_code->SetSelection(codeline->mips_code_index);
	this->Add(m_code,0,wxALL,5);
	CreateArgumentControls();
	m_code->Connect( wxEVT_CHOICE, wxCommandEventHandler( MipsScriptEditDialog::OnChangeCode ), NULL, parent );
}

void MipsScriptSizer::CreateArgumentControls() {
	unsigned int i;
	if (m_arg!=NULL) {
		for (i=0;i<arg_amount;i++) {
			m_arg[i]->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			delete m_arg[i];
		}
		delete m_arg;
		delete arg_type;
	}
	wxString codestr = _(MIPS_CODE_LIST[codeline->mips_code_index].str);
	bool sdone = false, tdone = false, ddone = false;
	int pos = codestr.Find(L'%');
	wchar_t parameter;
	arg_amount = 0;
	while (pos!=wxNOT_FOUND) {
		parameter = codestr[pos+1];
		if (parameter==L'%')
			pos++;
		else {
			if (parameter==L's') {
				if (!sdone)
					arg_amount++;
				sdone = true;
			} else if (parameter==L't') {
				if (!tdone)
					arg_amount++;
				tdone = true;
			} else if (parameter==L'd') {
				if (!ddone)
					arg_amount++;
				ddone = true;
			} else {
				arg_amount++;
			}
		}
		pos = codestr.find(L'%',pos+1);
	}
	m_arg = new wxSpinCtrl*[arg_amount];
	arg_type = new unsigned int[arg_amount];
	pos = codestr.Find(L'%');
	sdone = false;
	tdone = false;
	ddone = false;
	i = 0;
	while (pos!=wxNOT_FOUND) {
		parameter = codestr[pos+1];
		switch (parameter) {
		case L's':
			if (sdone)
				break;
			sdone = true;
			m_arg[i] = new wxSpinCtrl(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(50,-1));
			m_arg[i]->SetRange(0,31);
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			arg_type[i] = 1;
			m_arg[i]->SetValue(codeline->regs_add);
			break;
		case L't':
			if (tdone)
				break;
			tdone = true;
			m_arg[i] = new wxSpinCtrl(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(50,-1));
			m_arg[i]->SetRange(0,31);
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			arg_type[i] = 2;
			m_arg[i]->SetValue(codeline->regt_add);
			break;
		case L'd':
			if (ddone)
				break;
			ddone = true;
			m_arg[i] = new wxSpinCtrl(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(50,-1));
			m_arg[i]->SetRange(0,31);
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			arg_type[i] = 3;
			m_arg[i]->SetValue(codeline->regd_add);
			break;
		case L'h':
			m_arg[i] = new wxSpinCtrl(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(50,-1));
			m_arg[i]->SetRange(0,31);
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			arg_type[i] = 4;
			m_arg[i]->SetValue(codeline->shift_val);
			break;
		case L'j':
			m_arg[i] = new wxSpinCtrl(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(150,-1));
			m_arg[i]->SetRange(0,0x3FFFFFF);
			m_arg[i]->SetValue(codeline->jump_off);
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			arg_type[i] = 5;
			break;
		case L'v':
			parameter = codestr[pos+2];
			m_arg[i] = new wxSpinCtrl(parent->m_mipswindow,MIPS_MAX_ARGUMENTS*line_nb+i,wxEmptyString,wxDefaultPosition,wxSize(100,-1));
			if (parameter==L's') {
				m_arg[i]->SetRange(-0x8000,0x7FFF);
				m_arg[i]->SetValue((int16_t)codeline->val);
			} else {
				m_arg[i]->SetRange(0,0xFFFF);
				m_arg[i]->SetValue(codeline->val);
			}
			this->Add(m_arg[i],0,wxALIGN_CENTER|wxALL,2);
			m_arg[i]->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( MipsScriptEditDialog::OnChangeArgSpin ), NULL, parent );
			arg_type[i] = 6;
			break;
		}
		if (parameter==L'%')
			pos++;
		else
			i++;
		pos = codestr.find(L'%',pos+1);
	}
}

MipsScriptEditDialog::MipsScriptEditDialog(wxWindow* parent, MipsFunction* func) :
	MipsScriptEditWindow(parent),
	help_dial(NULL) {
	unsigned int i,sizeramount;
	function.amount = func->amount;
	function.instruction = new MipsInstruction*[function.amount];
	for (i=0;i<function.amount;i++) {
		function.instruction[i] = new MipsInstruction[1];
		function.instruction[i]->CopyValue(*func->instruction[i]);
	}
	code_list.Alloc(G_N_ELEMENTS(MIPS_CODE_LIST)-1);
	for (i=0;i+1<G_N_ELEMENTS(MIPS_CODE_LIST);i++)
		code_list.Add(_(MIPS_CODE_LIST[i].name));
	m_mipsscroll->SetScrollbar(0,MIPS_MAX_INSTRUCTION_DISPLAY,function.amount,MIPS_MAX_INSTRUCTION_DISPLAY-1);
	m_parentsizer = new wxFlexGridSizer(0,1,0,0);
	m_parentsizer->SetFlexibleDirection(wxBOTH);
	m_parentsizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
	sizeramount = min((int)function.amount,MIPS_MAX_INSTRUCTION_DISPLAY);
	code_sizer = new MipsScriptSizer*[sizeramount];
	for (i=0;i<sizeramount;i++) {
		code_sizer[i] = new MipsScriptSizer(this,i,function.instruction[i]);
		m_parentsizer->Add(code_sizer[i],1,wxEXPAND);
	}
	m_mipswindow->SetSizer(m_parentsizer);
	m_mipswindow->Layout();
	m_parentsizer->Fit(m_mipswindow);
}

MipsScriptEditDialog::~MipsScriptEditDialog() {
	unsigned int i;
	for (i=0;i<function.amount;i++)
		delete[] function.instruction[i];
	delete[] function.instruction;
}

int MipsScriptEditDialog::ShowModal() {
	DisplayCode();
	return wxDialog::ShowModal();
}

void MipsScriptEditDialog::DisplayCode() {
	unsigned int i;
	wxString help = _(L"");
	if (m_displaypos->IsChecked()) {
		for (i=0;i<function.amount;i++) {
			help += wxString::Format(wxT("0x%X: "),function.instruction[i]->ram_pos & 0x7FFFFFFF);
			help += _(function.instruction[i]->GetDisplayStr())+_(L"\n");
		}
	} else {
		for (i=0;i<function.amount;i++)
			help += _(function.instruction[i]->GetDisplayStr())+_(L"\n");
	}
	int scrollpos = m_codetextctrl->GetScrollPos(wxVERTICAL);
	m_codetextctrl->Freeze();
	m_codetextctrl->ChangeValue(help);
	HighlightCode(false);
	m_codetextctrl->ScrollLines(scrollpos);
	m_codetextctrl->Thaw();
}

void MipsScriptEditDialog::HighlightCode(bool removeprev) {
	if (removeprev) {
		wxColour backcol = wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK);
		m_codetextctrl->SetStyle(0,m_codetextctrl->GetLastPosition(),wxTextAttr(wxNullColour,backcol));
	}
	long mipsposstart = m_codetextctrl->XYToPosition(0,m_mipsscroll->GetThumbPosition());
	long mipsposend = m_codetextctrl->XYToPosition(0,m_mipsscroll->GetThumbPosition()+MIPS_MAX_INSTRUCTION_DISPLAY);
	if (mipsposend==-1)
		mipsposend = m_codetextctrl->GetLastPosition();
	m_codetextctrl->SetStyle(mipsposstart,mipsposend,wxTextAttr(wxNullColour,*wxLIGHT_GREY));
}

void MipsScriptEditDialog::OnAddressCheck(wxCommandEvent& event) {
	DisplayCode();
}

void MipsScriptEditDialog::OnHelpClick(wxCommandEvent& event) {
	if (help_dial==NULL || !help_dial->IsShown()) {
		help_dial = new MipsScriptHelpDialog(this);
		help_dial->Show();
	} else {
		help_dial->SetFocus();
	}
}

void MipsScriptEditDialog::OnMipsScroll(wxScrollEvent& event) {
	unsigned int sizeramount = min((int)function.amount,MIPS_MAX_INSTRUCTION_DISPLAY);
	unsigned int i;
	for (i=0;i<sizeramount;i++) {
		code_sizer[i]->codeline = function.instruction[event.GetPosition()+i];
		code_sizer[i]->m_code->SetSelection(code_sizer[i]->codeline->mips_code_index);
		code_sizer[i]->CreateArgumentControls();
	}
	HighlightCode(true);
	m_mipswindow->Layout();
	m_mipswindow->Refresh();
}

void MipsScriptEditDialog::OnChangeCode(wxCommandEvent& event) {
	unsigned int newcode;
	MipsScriptSizer* sizer = code_sizer[event.GetId()/MIPS_MAX_ARGUMENTS];
	newcode = event.GetSelection();
	sizer->codeline->opcode = MIPS_CODE_LIST[newcode].opcode;
	sizer->codeline->function = MIPS_CODE_LIST[newcode].function;
	sizer->codeline->mips_code_index = newcode;
	sizer->codeline->regs_add = 0;
	sizer->codeline->regt_add = 0;
	sizer->codeline->regd_add = 0;
	sizer->codeline->shift_val = 0;
	sizer->codeline->val = 0;
	sizer->codeline->jump_off = 0;
	sizer->CreateArgumentControls();
	DisplayCode();
	m_mipswindow->Layout();
	m_mipswindow->Refresh();
}

void MipsScriptEditDialog::OnChangeArgSpin(wxSpinEvent& event) {
	MipsScriptSizer* sizer = code_sizer[event.GetId()/MIPS_MAX_ARGUMENTS];
	unsigned int argtype = sizer->arg_type[event.GetId()%MIPS_MAX_ARGUMENTS];
	if (argtype==1)
		sizer->codeline->regs_add = event.GetPosition();
	else if (argtype==2)
		sizer->codeline->regt_add = event.GetPosition();
	else if (argtype==3)
		sizer->codeline->regd_add = event.GetPosition();
	else if (argtype==4)
		sizer->codeline->shift_val = event.GetPosition();
	else if (argtype==5)
		sizer->codeline->jump_off = event.GetPosition();
	else if (argtype==6)
		sizer->codeline->val = event.GetPosition();
	DisplayCode();
}

//=============================//
//             CIL             //
//=============================//

#define CIL_NOCHECK_TYPECHECK	"typecheck"
#define CIL_NOCHECK_RANGECHECK	"rangecheck"
#define CIL_NOCHECK_NULLCHECK	"nullcheck"
#define CIL_REFRESH_TIMEOUT		0.02

wxString GetNextCILWord(wxString& scriptstr) {
	wxString res;
	size_t pos = scriptstr.find_first_not_of(L" \t");
	if (pos==string::npos) {
		scriptstr = wxEmptyString;
		return wxEmptyString;
	}
	size_t shift = pos;
	res = scriptstr.Mid(pos);
	if (res[0]==L'\n') {
		scriptstr = scriptstr.Mid(shift+1);
		return _(L"\n");
	}
	pos = res.find_first_of(L" \t\n");
	if (pos!=string::npos) {
		res = res.Mid(0,pos);
		shift += pos;
		scriptstr = scriptstr.Mid(shift);
	} else
		scriptstr = wxEmptyString;
	return res;
}

int GetLineCount(wxString str) {
	if (str.IsEmpty())
		return -1;
	int res = 0;
	for (unsigned int i=0;i<str.Len();i++)
		if (str[i]==L'\n')
			res++;
	return res;
}

CilScriptEditDialog::CilScriptEditDialog(wxWindow* parent, CILDataSet* cil, unsigned int tid, unsigned int mid) :
	CilScriptEditWindow(parent),
	cilset(cil),
	error_type(0),
	type_id(tid),
	method_id(mid),
	method_raw_length(0),
	method_raw(NULL),
	timer(new wxTimer(this)) {
	Connect(wxEVT_TIMER,wxTimerEventHandler(CilScriptEditDialog::OnTimer),NULL,this);
	method_raw = cilset->GetModifiedMethodRaw(type_id,method_id,&method_raw_length);
	if (method_raw==NULL) {
		error_type = 1;
	} else {
		string tname = cilset->data->GetTypeName(type_id,true);
		string mname = cilset->data->GetMethodName(type_id,method_id);
		if ((tname.compare("FF9BattleDB")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("rdata")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("ff9level")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("ff9item")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("FF9.ff9abil")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("FF9.ff9armor")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("FF9.ff9weap")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("FF9.ff9equip")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("FF9.ff9mix")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("FF9.ff9armor")==0 && mname.compare(".cctor")==0)
		 || (tname.compare("Assets.Sources.Scripts.UI.Common.FF9TextTool")==0 && mname.compare("CharacterDefaultName")==0)) {
			error_type = 2;
			m_buttonok->Enable(false);
		}
	}
	delete_method_raw = method_raw!=NULL;
}

CilScriptEditDialog::~CilScriptEditDialog() {
	Disconnect(wxEVT_TIMER,wxTimerEventHandler(CilScriptEditDialog::OnTimer),NULL,this);
	delete timer;
	if (delete_method_raw)
		delete[] method_raw;
}

int CilScriptEditDialog::ShowModal() {
	wxString script = _(L""), sposline;
	unsigned int i;
	ILCode code;
	uint32_t swam;
	uint64_t tmp64;
	uint32_t tmp32;
	uint16_t tmp16;
	uint8_t tmp8;
	size_t cilpos;
	script_pos_string = _(L"");
	BufferInitPosition();
	while (BufferGetPosition()<method_raw_length) {
		sposline.Printf(wxT("IL_%.4X:\n"),BufferGetPosition());
		script_pos_string += sposline;
		BufferReadChar(method_raw,tmp8);
		if (tmp8==0xFE) {
			tmp16 = (tmp8 << 8) | BufferReadChar(method_raw,tmp8);
			code = GetILCode(tmp16);
		} else
			code = GetILCode(tmp8);
		script += _(code.name);
		switch (code.size) {
		case 1:
			tmp32 = BufferReadChar(method_raw,tmp8);
			break;
		case 2:
			tmp32 = BufferReadShort(method_raw,tmp16);
			break;
		case 4:
			BufferReadLong(method_raw,tmp32);
			break;
		case 8:
			BufferReadLongLong(method_raw,tmp64);
			break;
		}
		switch (code.type) {
		case ILT_SWITCH:
			BufferReadLong(method_raw,swam);
			cilpos = BufferGetPosition()+swam*4;
			for (i=0;i<swam;i++)
				script += wxString::Format(wxT(" IL_%.4X"),cilpos+(int32_t)BufferReadLong(method_raw,tmp32));
			break;
		case ILT_INT:
			if (code.size==1)
				script += wxString::Format(wxT(" %hhi"),(int8_t)tmp8);
			else if (code.size==2)
				script += wxString::Format(wxT(" %hi"),(int16_t)tmp16);
			else if (code.size==4)
				script += wxString::Format(wxT(" %i"),(int32_t)tmp32);
			break;
		case ILT_UINT:
			script += wxString::Format(wxT(" %u"),tmp32);
			break;
		case ILT_FLOAT:
			if (code.size==4)
				script += wxString::Format(wxT(" %f"),*(float*)&tmp32);
			else if (code.size==8)
				script += wxString::Format(wxT(" %f"),*(double*)&tmp64);
			break;
		case ILT_OFFSET:
			if (code.size==1)
				script += wxString::Format(wxT(" IL_%.4X"),BufferGetPosition()+(int8_t)tmp8);
			else if (code.size==2)
				script += wxString::Format(wxT(" IL_%.4X"),BufferGetPosition()+(int16_t)tmp16);
			else if (code.size==4)
				script += wxString::Format(wxT(" IL_%.4X"),BufferGetPosition()+(int32_t)tmp32);
			break;
		case ILT_STRING:
			script += wxString::Format(wxT(" 0x%X\t// \"%s\""),tmp32,cilset->data->GetStringTokenDescription(tmp32).c_str());
			break;
		case ILT_METHOD:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_CLASS:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_CTOR:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_FIELD:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_TYPE:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_CALLSITE:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_TOKEN:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_ETYPE:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_VALUETYPE:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_TYPETOK:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_THISTYPE:
			script += wxString::Format(wxT(" 0x%X\t// %s"),tmp32,cilset->data->GetTokenDescription(tmp32).c_str());
			break;
		case ILT_UNALIGN:
			script += wxString::Format(wxT(" %u"),tmp32);
			break;
		case ILT_NOCHECK:
			if (tmp32 & 1)
				script += _(L" ") + _(CIL_NOCHECK_TYPECHECK);
			if (tmp32 & 2)
				script += ((tmp32 & 1) ? _(L"|") : _(L" ")) + _(CIL_NOCHECK_RANGECHECK);
			if (tmp32 & 4)
				script += ((tmp32 & 3) ? _(L"|") : _(L" ")) + _(CIL_NOCHECK_NULLCHECK);
			break;
		}
		script += _(L"\n");
	}
	m_scriptposctrl->ChangeValue(script_pos_string);
	m_scriptctrl->ChangeValue(script);
	current_scroll_pos = m_scriptctrl->GetScrollPos(wxVERTICAL);
	current_script_length = script.Len();
	current_insertion_point = 0;
	current_insertion_point_line = 0;
	m_scriptctrl->SetFocus();
	timer->Start(CIL_REFRESH_TIMEOUT);
	return wxDialog::ShowModal();
}

DllMetaDataModification* CilScriptEditDialog::ComputeModification() {
	unsigned int absmethid = cilset->data->cil_type_method_id[type_id]-1+method_id;
	DllMetaDataModification* res = new DllMetaDataModification[1];
	DllMethodInfo methinfo;
	fstream& dllfile = cilset->data->dll_file;
	dllfile.seekg(cilset->data->GetMethodOffset(absmethid));
	methinfo.ReadMethodInfo(dllfile);
	res->base_length = methinfo.code_size;
	res->method_id = absmethid;
	res->new_length = method_raw_length;
	res->position = dllfile.tellg();
	res->value = method_raw;
	delete_method_raw = false;
	return res;
}

void CilScriptEditDialog::UpdateScriptPositions() {
	wxString newscriptpos = _(L"");
	wxString scriptstr = m_scriptctrl->GetValue();
	wxString instrstr,sposline;
	size_t cilpos = 0;
	unsigned int i;
	while (!scriptstr.IsEmpty()) {
		instrstr = GetNextCILWord(scriptstr);
		sposline.Printf(wxT("IL_%.4X:\n"),cilpos);
		newscriptpos += sposline;
		for (i=0;i+1<G_N_ELEMENTS(ILCodeList);i++)
			if (instrstr.IsSameAs(ILCodeList[i].name)) {
				if (ILCodeList[i].size>=0)
					cilpos += ILCodeList[i].size+(ILCodeList[i].code>0xFF ? 2 : 1);
				else {
					cilpos += 5;
					instrstr = GetNextCILWord(scriptstr);
					while (!scriptstr.IsEmpty() && !instrstr.IsSameAs(L"\n") && !(instrstr.SubString(0,2).IsSameAs(L"//"))) {
						cilpos += 4;
						instrstr = GetNextCILWord(scriptstr);
					}
				}
				break;
			}
		while (!scriptstr.IsEmpty() && !instrstr.IsSameAs(L"\n"))
			instrstr = GetNextCILWord(scriptstr);
	}
	if (!newscriptpos.IsSameAs(script_pos_string)) {
		int linediff, curscroll = m_scriptctrl->GetScrollPos(wxVERTICAL);
		wxTextPos curselstart, curselend, curpos = m_scriptctrl->GetInsertionPoint();
		unsigned int linediffpos = 0;
		size_t scriptilpos,ilposline;
		linediff = newscriptpos.Len()-script_pos_string.Len();
		if (linediff!=0 && current_insertion_point_line>=0) {
			if (linediff>0)
				linediffpos = (current_insertion_point_line+1)*9;
			else
				linediffpos = current_insertion_point_line*9;
		}
		scriptstr = m_scriptctrl->GetValue();
		scriptilpos = scriptstr.find(L"IL_");
		while (scriptilpos!=string::npos) {
			instrstr = scriptstr.Mid(scriptilpos,7);
			ilposline = script_pos_string.find(instrstr);
			if (ilposline!=string::npos) {
				if (ilposline>=linediffpos)
					ilposline += linediff;
				scriptstr.replace(scriptilpos,7,newscriptpos.Mid(ilposline,7));
			}
			scriptilpos = scriptstr.find(L"IL_",scriptilpos+3);
		}
		m_scriptctrl->GetSelection(&curselstart,&curselend);
		m_scriptctrl->ChangeValue(scriptstr);
		m_scriptctrl->ScrollLines(curscroll);
		m_scriptctrl->SetSelection(curselstart,curselend);
		m_scriptctrl->SetInsertionPoint(curpos);
		script_pos_string = newscriptpos;
	}
}

bool CilScriptEditDialog::WriteArgumentToRawCode(LogStruct& log, unsigned int ilcodeid, wxString argstr, uint8_t* raw, unsigned int rampos) {
	wxString errstr, argstrbackup = argstr;
	ILCode code = ILCodeList[ilcodeid];
	bool isneg = false, ishexa = false;
	unsigned long arglong;
	unsigned int i;
	if (argstr.Mid(0,3).IsSameAs(L"IL_")) {
		if (script_pos_string.Find(argstr)!=wxNOT_FOUND) {
			int32_t argpos;
			argstr = argstr.Mid(3,4);
			argstr.ToCULong(&arglong,16);
			argpos = arglong-rampos;
			if (code.size==1) {
				if (argpos<-0x80 || argpos>0x7F) {
					errstr.Printf(wxT(HADES_STRING_CILSCRIPT_RANGEILPOS),BufferGetPosition(),argstrbackup);
					log.AddError(errstr.ToStdWstring());
					return false;
				}
				int8_t arg8 = argpos;
				BufferWriteChar(raw,arg8);
			} else if (code.size==2) {
				if (argpos<-0x8000 || argpos>0x7FFF) {
					errstr.Printf(wxT(HADES_STRING_CILSCRIPT_RANGEILPOS),BufferGetPosition(),argstrbackup);
					log.AddError(errstr.ToStdWstring());
					return false;
				}
				int16_t arg16 = argpos;
				BufferWriteShort(raw,arg16);
			} else {
				BufferWriteLong(raw,argpos);
			}
			return true;
		} else {
			errstr.Printf(wxT(HADES_STRING_CILSCRIPT_WRONGILPOS),BufferGetPosition(),argstrbackup);
			log.AddError(errstr.ToStdWstring());
			return false;
		}
	}
	if (code.type==ILT_NOCHECK) {
		wxArrayString checktok = wxStringTokenize(argstr,L"|",wxTOKEN_STRTOK);
		uint8_t arg8 = 0;
		for (i=0;i<checktok.Count();i++) {
			if (checktok[i].IsSameAs(CIL_NOCHECK_TYPECHECK))
				arg8 |= 1;
			else if (checktok[i].IsSameAs(CIL_NOCHECK_RANGECHECK))
				arg8 |= 2;
			else if (checktok[i].IsSameAs(CIL_NOCHECK_NULLCHECK))
				arg8 |= 4;
			else {
				errstr.Printf(wxT(HADES_STRING_CILSCRIPT_WRONGTYPECHECK),BufferGetPosition(),CIL_NOCHECK_TYPECHECK,CIL_NOCHECK_RANGECHECK,CIL_NOCHECK_NULLCHECK);
				log.AddError(errstr.ToStdWstring());
				return false;
			}
		}
		BufferWriteChar(raw,arg8);
		return true;
	}
	if (argstr[0]==L'-') {
		isneg = true;
		argstr = argstr.Mid(1);
	}
	if (argstr.Mid(0,2).IsSameAs(L"0x",false)) {
		ishexa = true;
		argstr = argstr.Mid(2);
	}
	if (code.type==ILT_FLOAT) {
		double argdouble;
		float argfloat;
		if (argstr.ToCDouble(&argdouble)) {
			switch (code.size) {
			case 4:
				argfloat = isneg ? -argdouble : argdouble;
				BufferWriteLong(raw,*(uint32_t*)&argfloat);
				return true;
			case 8:
				argdouble = isneg ? -argdouble : argdouble;
				BufferWriteLongLong(raw,*(uint64_t*)&argdouble);
				return true;
			}
		}
	} else {
		if (argstr.ToCULong(&arglong,ishexa ? 16 : 10)) {
			switch (code.size) {
			case 1:
				BufferWriteChar(raw,isneg ? -(int8_t)arglong : arglong);
				return true;
			case 2:
				BufferWriteShort(raw,isneg ? -(int16_t)arglong : arglong);
				return true;
			case 4:
				BufferWriteLong(raw,isneg ? -(int32_t)arglong : arglong);
				return true;
			}
		}
	}
	errstr.Printf(wxT(HADES_STRING_CILSCRIPT_NOTARG),BufferGetPosition(),argstrbackup);
	log.AddError(errstr.ToStdWstring());
	return false;
}

LogStruct CilScriptEditDialog::ParseMethod(wxString scriptstr) {
	wxString scriptword, errstr;
	uint8_t newraw[0x800];
	unsigned int i;
	LogStruct res = LogStruct();
	BufferInitPosition();
	while (!scriptstr.IsEmpty()) {
		scriptword = GetNextCILWord(scriptstr);
		for (i=0;i+1<G_N_ELEMENTS(ILCodeList);i++)
			if (scriptword.IsSameAs(ILCodeList[i].name)) {
				if (ILCodeList[i].code>0xFF) {
					BufferWriteChar(newraw,0xFE);
					BufferWriteChar(newraw,ILCodeList[i].code & 0xFF);
				} else
					BufferWriteChar(newraw,ILCodeList[i].code);
				if (ILCodeList[i].size>0) {
					scriptword = GetNextCILWord(scriptstr);
					if (!WriteArgumentToRawCode(res,i,scriptword,newraw,BufferGetPosition()+ILCodeList[i].size)) {
						BufferInitPosition(BufferGetPosition()+ILCodeList[i].size);
					}
				} else if (ILCodeList[i].size==-1) {
					wxString tmpline = scriptstr;
					uint32_t rampos,swam = 0;
					scriptword = GetNextCILWord(tmpline);
					while (!tmpline.IsEmpty() && !scriptword.IsSameAs(L"\n") && !scriptword.Mid(0,2).IsSameAs(L"//")) {
						scriptword = GetNextCILWord(tmpline);
						swam++;
					}
					BufferWriteLong(newraw,swam);
					rampos = BufferGetPosition()+4*swam;
					for (i=0;i<swam;i++) {
						scriptword = GetNextCILWord(scriptstr);
						if (!WriteArgumentToRawCode(res,i,scriptword,newraw,rampos)) {
							BufferInitPosition(BufferGetPosition()+4);
						}
					}
				}
				break;
			}
		if (i+1==G_N_ELEMENTS(ILCodeList) && !scriptword.IsSameAs(L"\n")) {
			errstr.Printf(wxT(HADES_STRING_CILSCRIPT_NOTINST),BufferGetPosition(),scriptword);
			res.AddWarning(errstr.ToStdWstring());
		}
		while (!scriptstr.IsEmpty() && !scriptword.IsSameAs(L"\n"))
			scriptword = GetNextCILWord(scriptstr);
	}
	if (res.ok) {
		method_raw_length = BufferGetPosition();
		delete[] method_raw;
		method_raw = new uint8_t[method_raw_length];
		memcpy(method_raw,newraw,method_raw_length*sizeof(uint8_t));
	}
	return res;
}

void CilScriptEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_OK) {
		LogStruct log = ParseMethod(m_scriptctrl->GetValue());
		if (log.error_amount>0 || log.warning_amount>0) {
			LogDialog dial(this,log);
			dial.ShowModal();
			if (!log.ok)
				return;
		}
	}
	EndModal(id);
}

void CilScriptEditDialog::OnTimer(wxTimerEvent& event) {
	bool updatepos = m_scriptctrl->GetValue().Len()!=current_script_length;
	if (updatepos) {
		current_script_length = m_scriptctrl->GetValue().Len();
		UpdateScriptPositions();
	}
	if (updatepos || current_scroll_pos!=m_scriptctrl->GetScrollPos(wxVERTICAL)) {
		current_scroll_pos = m_scriptctrl->GetScrollPos(wxVERTICAL);
		m_scriptposctrl->SetValue(script_pos_string.Mid(9*current_scroll_pos));
	}
	if (current_insertion_point!=m_scriptctrl->GetInsertionPoint()) {
		current_insertion_point = m_scriptctrl->GetInsertionPoint();
		current_insertion_point_line = GetLineCount(m_scriptctrl->GetRange(0,current_insertion_point));
	}
}
