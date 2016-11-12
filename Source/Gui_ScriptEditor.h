#ifndef _GUI_SCRIPTEDITOR_H
#define _GUI_SCRIPTEDITOR_H

class ScriptEditHandler;
class ScriptEditDialog;
class ScriptHelpDialog;

#include <wx/clrpicker.h>
#include "gui.h"
#include "Gui_GLWindow.h"
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

class ScriptEditHandler {
public:
	ScriptDataStruct script;
	unsigned int entry_selection;
	unsigned int function_selection;
	wxString** func_str;
	wxString* localvar_str;
	wxString globalvar_str;
	wxArrayString entry_name;
	wxArrayString functionlist_str;
	wxArrayString modellist_str;
	uint16_t** functionlist_id;
	uint16_t** modellist_id;
	
	ScriptEditDialog* handler_dialog;
	
	ScriptEditHandler(ScriptDataStruct& scpt);
	~ScriptEditHandler();
	void GenerateFunctionStrings();
	void GenerateFunctionList(bool firsttime);
	void EntryChangeName(unsigned int entry, wxString newname);

private:
	bool GenerateFunctionStrings_Rec(wxString& str, ScriptFunction& func, unsigned int& funcpos, unsigned int& oppos, int endfuncpos = -1, unsigned int tabpos = 1, int blocktype = 0, int endblockpos = -1);
	wxString ConvertVarArgument(ScriptArgument& arg);
};

class ScriptEditDialog : public ScriptEditWindow, public ScriptEditHandler {
public:
	EnemyDataStruct* enemy;
	TextDataStruct* text;
	SaveSet* datas;
	int script_type;
	bool use_character;
	bool use_text;
	bool use_battle;
	bool use_attack;
	bool use_field;
	bool use_item;
	bool use_ability;
	bool use_command;
	bool use_card;
	wxArrayString defaultbool_str;
	wxArrayString text_str;
	wxArrayString battle_str;
	wxArrayString field_str;
	wxArrayString attack_str;
	wxArrayString item_str;
	wxArrayString disc_str;
	wxArrayString menutype_str;
	wxArrayString shop_str;
	wxArrayString character_str;
	wxArrayString abilityset_str;
	wxArrayString equipset_str;
	wxArrayString fmv_str;
	wxArrayString battlecode_str;
	wxArrayString modelcode_str;
	wxArrayString worldcode_str;
	wxArrayString soundcode_str;
	wxArrayString spscode_str;
	wxArrayString entrylist_str;
	wxArrayString bubblesymbol_str;
	wxArrayString worldmap_str;
	wxArrayString ability_str;
	wxArrayString command_str;
	wxArrayString deck_str;
	uint16_t** battle_id;
	uint16_t** field_id;
	uint16_t** item_id;
	uint16_t** character_id;
	uint16_t** equipset_id;
	uint16_t** fmv_id;
	uint16_t** battlecode_id;
	uint16_t** modelcode_id;
	uint16_t** worldcode_id;
	uint16_t** soundcode_id;
	uint16_t** spscode_id;
	uint16_t** entrylist_id;
	uint16_t** worldmap_id;
	
	long line_selection;
	long text_x_selection;
	unsigned int arg_amount;
	unsigned int* arg_position;
	bool* func_should_parse;
	uint16_t current_opcode;
	int* arg_control_type;
	wxStaticText** arg_label;
	wxWindow** arg_control;
	
	// dataload[] = { statset, enemyset, fieldset, itemset, spellset, commandset, cardset }
	ScriptEditDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype, SaveSet* sv, EnemyDataStruct* ed, TextDataStruct* td, bool* dataloaded);
	~ScriptEditDialog();
	int ShowModal();
	void DisplayFunctionList(bool firsttime, int newfunc=-1, int removedfunc=-1);
	LogStruct ParseFunction(wxString str, unsigned int entry, unsigned int function);
	void DisplayFunction(unsigned int entry, unsigned int function);
	void DisplayOperation(wxString line, bool refreshargcontrol = true, bool refresharg = true);
	void UpdateLineHelp(long x, long y);

private:
	uint32_t extra_size;
	unsigned int world_pos_type;
	int32_t world_pos_x;
	int32_t world_pos_y;
	bool refresh_control_disable;
	
	wxTimer* timer;
	wxMenu* func_popup_menu;
	GLWindow* gl_window;
	ScriptHelpDialog* help_dial;
	wxCheckBox* ArgCreateFlag(wxString& arg, unsigned int id);
	wxTextCtrl* ArgCreateText(wxString& arg, unsigned int id);
	wxSpinCtrl* ArgCreateSpin(wxString& arg, unsigned int id, int size, bool sign);
	wxChoice* ArgCreateChoice(wxString& arg, unsigned int id, uint16_t** choiceid, wxArrayString& choicestr);
	wxPanel* ArgCreateDiscFieldChoice(wxString& arg, unsigned int id, uint16_t** choiceid, wxArrayString& choicestr);
	wxPanel* ArgCreateFlags(wxString& arg, unsigned int id, unsigned int amount, wxArrayString& flagstr);
	wxPanel* ArgCreatePosition(wxArrayString& arg, unsigned int id);
	wxColourPickerCtrl* ArgCreateColorPicker(wxArrayString& arg, unsigned int id, bool rgb);
	
	void UpdateGlobalLocalStrings(int ignoreentry=-1);
	ScriptLocalVariableSet* ParseLocal(LogStruct& log, wxString str);
	void ScriptChangeArg(int argi, int64_t value, int argshift = 0);
	
	void OnFunctionChoose(wxListEvent& event);
	void OnIntValueText(wxCommandEvent& event);
	void OnFunctionRightClick(wxListEvent& event);
	void OnFunctionRightClickMenu(wxCommandEvent& event);
	void OnFunctionUpdate(wxCommandEvent& event);
	void OnFunctionNewLine(wxCommandEvent& event);
	void OnChoiceSelection(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnWorldMapPaint(wxPaintEvent& event);
	void OnShowHideLocalVar(wxMouseEvent& event);
	void OnButtonClick(wxCommandEvent& event);
	void OnTimer(wxTimerEvent& event);
	
	void OnArgFlag(wxCommandEvent& event);
	void OnArgSpin(wxSpinEvent& event);
	void OnArgChoice(wxCommandEvent& event);
	void OnArgFlags(wxCommandEvent& event);
	void OnArgField(wxCommandEvent& event);
	void OnArgDisc(wxCommandEvent& event);
	void OnArgPositionPaint(wxPaintEvent& event);
	void OnArgPositionMouseMove(wxMouseEvent& event);
	void OnArgPositionMouseUp(wxMouseEvent& event);
	void OnArgPositionMouseWheel(wxMouseEvent& event);
	void OnArgPositionKeyboard(wxKeyEvent& event);
	void OnArgColorPicker(wxColourPickerEvent& event);
	
	friend ScriptHelpDialog;
	friend ScriptEditHandler;
};

class ScriptEditEntryDialog : public ScriptEditEntryWindow {
public:
	int script_type;
	unsigned int base_entry_amount;
	unsigned int entry_amount;
	bool has_character_entry; // should always be true
	int* base_entry_id;
	uint8_t* entry_type;
	
	ScriptEditEntryDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype);
	~ScriptEditEntryDialog();
	int ShowModal();
	void ApplyModifications(ScriptDataStruct& scpt);

private:
	uint32_t extra_size;
	
	void DisplayEntry(int sel);
	
	void OnEntrySelect(wxCommandEvent& event);
	void OnSpinCtrl(wxSpinEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
