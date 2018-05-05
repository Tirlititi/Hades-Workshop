#ifndef _GUI_SCRIPTEDITOR_H
#define _GUI_SCRIPTEDITOR_H

class ScriptEditHandler;
class ScriptEditDialog;
class ScriptEditEntryDialog;
class ScriptEditLinkDialog;
class ScriptEditTextLinkDialog;
class ScriptHelpDialog;

#include <vector>
#include <wx/clrpicker.h>
#include "gui.h"
#include "Gui_GLWindow.h"
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

class ScriptEditHandler {
public:
	ScriptDataStruct script;
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
	bool use_support;
	bool use_command;
	bool use_card;
	unsigned int entry_selection;
	unsigned int function_selection;
	vector<wxArrayString> func_str;
	wxArrayString localvar_str;
	wxString globalvar_str;
	wxArrayString entry_name;
	wxArrayString functionlist_str;
	vector<uint16_t*> functionlist_id;
	vector<int32_t> entry_model_index;

	ScriptEditDialog* handler_dialog;
	
	// dataload[] = { statset, enemyset, fieldset, itemset, spellset, supportset, commandset, cardset }
	ScriptEditHandler(ScriptDataStruct& scpt, int scpttype, SaveSet* sv, EnemyDataStruct* ed, TextDataStruct* td, bool* dataloaded);
	~ScriptEditHandler();
	void GenerateFunctionStrings(bool appendcomment = false);
	void GenerateFunctionList();
	void EntryChangeName(unsigned int entry, wxString newname);
	wxString GetArgumentDescription(int64_t argvalue, uint8_t argtype);

private:
	bool GenerateFunctionStrings_Rec(wxString& str, ScriptFunction& func, unsigned int& funcpos, unsigned int& oppos, int endfuncpos = -1, unsigned int tabpos = 1, int blocktype = 0, int endblockpos = -1, bool appendcomment = false);
	wxString ConvertVarArgument(ScriptArgument& arg, wxArrayString* argcomment = NULL);
};

class ScriptEditDialog : public ScriptEditWindow, public ScriptEditHandler {
public:
	wxArrayString modellist_str;
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
	wxArrayString animlist_str;
	vector<uint16_t*> modellist_id;
	vector<uint16_t*> battle_id;
	vector<uint16_t*> field_id;
	vector<uint16_t*> item_id;
	vector<uint16_t*> character_id;
	vector<uint16_t*> equipset_id;
	vector<uint16_t*> fmv_id;
	vector<uint16_t*> battlecode_id;
	vector<uint16_t*> modelcode_id;
	vector<uint16_t*> worldcode_id;
	vector<uint16_t*> soundcode_id;
	vector<uint16_t*> spscode_id;
	vector<uint16_t*> entrylist_id;
	vector<uint16_t*> worldmap_id;
	vector<uint16_t*> animlist_id;
	
	long line_selection;
	long text_x_selection;
	unsigned int arg_amount;
	vector<unsigned int> arg_position;
	vector<bool> func_should_parse;
	uint16_t current_opcode;
	vector<int> arg_control_type;
	vector<wxStaticText*> arg_label;
	vector<wxWindow*> arg_control;
	
	// dataload[] = { statset, enemyset, fieldset, itemset, spellset, supportset, commandset, cardset }
	ScriptEditDialog(wxWindow* parent, ScriptDataStruct& scpt, int scpttype, SaveSet* sv, EnemyDataStruct* ed, TextDataStruct* td, bool* dataloaded);
	~ScriptEditDialog();
	int ShowModal();
	void DisplayFunctionList(int newfunc = -1, int removedfunc = -1);
	LogStruct ParseFunction(wxString str, unsigned int entry, unsigned int function);
	void DisplayFunction(unsigned int entry, unsigned int function);
	void DisplayOperation(wxString line, bool refreshargcontrol = true, bool refresharg = true);
	void UpdateLineHelp(long x, long y);
	void UpdateMultiLangDialogHelp(wxChoice* dialogchoice);

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
	wxChoice* ArgCreateChoice(wxString& arg, unsigned int id, vector<uint16_t*> choiceid, wxArrayString& choicestr);
	wxWindow* ArgCreateDialog(wxString& arg, unsigned int id, vector<uint16_t*> choiceid, wxArrayString& choicestr);
	wxPanel* ArgCreateDiscFieldChoice(wxString& arg, unsigned int id, vector<uint16_t*> choiceid, wxArrayString& choicestr);
	wxPanel* ArgCreateFlags(wxString& arg, unsigned int id, unsigned int amount, wxArrayString& flagstr);
	wxPanel* ArgCreatePosition(wxArrayString& arg, unsigned int id);
	wxColourPickerCtrl* ArgCreateColorPicker(wxArrayString& arg, unsigned int id, bool rgb);
	
	void UpdateGlobalLocalStrings(int ignoreentry = -1);
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
	void OnArgDialogLink(wxCommandEvent& event);
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
	vector<int> base_entry_id;
	vector<uint8_t> entry_type;
	
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

class ScriptEditLinkDialog : public ScriptEditLinkWindow {
public:
	TextDataStruct* text;
	vector<uint16_t> message_link[STEAM_LANGUAGE_AMOUNT];
	vector<uint16_t> message_link_base[STEAM_LANGUAGE_AMOUNT];
	wxChoice* lang_link[STEAM_LANGUAGE_AMOUNT];
	wxButton* lang_text[STEAM_LANGUAGE_AMOUNT];
	bool is_modified[STEAM_LANGUAGE_AMOUNT];

	ScriptEditLinkDialog(wxWindow* parent, ScriptDataStruct& scpt, TextDataStruct& td);
	~ScriptEditLinkDialog();
	void ApplyModifications(ScriptDataStruct& scpt);

private:
	void OnButtonClick(wxCommandEvent& event);
	void OnChangeLink(wxCommandEvent& event);
};

class ScriptEditTextLinkDialog : public ScriptEditTextLinkWindow {
public:
	TextDataStruct* text;
	SteamLanguage base_lang;
	SteamLanguage link[STEAM_LANGUAGE_AMOUNT];
	wxChoice* link_choice[STEAM_LANGUAGE_AMOUNT];
	vector<uint16_t> message_link[STEAM_LANGUAGE_AMOUNT];
	vector<uint16_t> message_link_base;

	ScriptEditTextLinkDialog(wxWindow* parent, SteamLanguage* linklang, TextDataStruct& td, SteamLanguage baselang = GetSteamLanguage());
	~ScriptEditTextLinkDialog();
	int ShowModal(int dialogid = -1);
	void UpdateDialogSelection();

private:
	void OnButtonClick(wxCommandEvent& event);
	void OnChooseDialog(wxCommandEvent& event);
};

#endif
