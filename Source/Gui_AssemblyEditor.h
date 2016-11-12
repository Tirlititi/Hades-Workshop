#ifndef _GUI_ASSEMBLYEDITOR_H
#define _GUI_ASSEMBLYEDITOR_H

class MipsScriptEditDialog;
class MipsScriptHelpDialog;
class MipsScriptSizer;
class CilScriptEditDialog;

#include "gui.h"
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

class MipsScriptSizer : public wxBoxSizer {
public:
	MipsScriptEditDialog* parent;
	MipsInstruction* codeline;
	unsigned int line_nb;
	unsigned int arg_amount;
	unsigned int* arg_type;
	wxChoice* m_code;
	wxSpinCtrl** m_arg;
	
	MipsScriptSizer(MipsScriptEditDialog* p, unsigned int l, MipsInstruction* c);
	void CreateArgumentControls();
};

class MipsScriptEditDialog : public MipsScriptEditWindow {
public:
	MipsFunction function;
	MipsScriptSizer** code_sizer;
	wxArrayString code_list;
	MipsScriptHelpDialog* help_dial;
	wxFlexGridSizer* m_parentsizer;
	
	MipsScriptEditDialog(wxWindow* parent, MipsFunction* func);
	~MipsScriptEditDialog();
	int ShowModal();
	void DisplayCode();
	void HighlightCode(bool removeprev = true);
	
	void OnAddressCheck(wxCommandEvent& event);
	void OnHelpClick(wxCommandEvent& event);
	void OnMipsScroll(wxScrollEvent& event);
	void OnChangeCode(wxCommandEvent& event);
	void OnChangeArgSpin(wxSpinEvent& event);
};

class CilScriptEditDialog : public CilScriptEditWindow {
public:
	CILDataSet* cilset;
	int error_type; // 0 = no error, 1 = file access, 2 = no method body
	unsigned int type_id;
	unsigned int method_id;
	wxString script_pos_string;
	
	uint32_t method_raw_length;
	uint8_t* method_raw;
	
	CilScriptEditDialog(wxWindow* parent, CILDataSet* cil, unsigned int tid, unsigned int mid);
	~CilScriptEditDialog();
	int ShowModal();
	// Apply the modifications parsed after a call of ShowModal that returned wxID_OK
	DllMetaDataModification* ComputeModification();
	
private:
	wxTimer* timer;
	size_t current_script_length;
	wxTextPos current_insertion_point;
	int current_insertion_point_line;
	int current_scroll_pos;
	bool delete_method_raw;
	
	void UpdateScriptPositions();
	bool WriteArgumentToRawCode(LogStruct& log, unsigned int ilcodeid, wxString argstr, uint8_t* raw, unsigned int rampos);
	LogStruct ParseMethod(wxString scriptstr);
	
	void OnButtonClick(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
};

#endif
