#pragma once

class SaveCustomFieldDialog;
class SaveCustomBattleDialog;

#include "gui.h"
#include "Configuration.h"
using namespace std;

class SaveCustomFieldDialog : public SaveCustomFieldWindow {
public:
	SaveCustomFieldDialog(wxWindow* parent, wxString defaultpath, SaveSet& saveset);
	int DoGenerate(ConfigurationSet& config, SaveSet& saveset);

private:
	SaveSet* svset;

	void UpdateDictionaryPatchCommand();
	void OnTextBlockChoice(wxCommandEvent& event);
	void OnUpdateArg(wxCommandEvent& event);
	void OnUpdateArgSpin(wxSpinEvent& event);
};

class SaveCustomBattleDialog : public SaveCustomBattleWindow {
public:
	SaveCustomBattleDialog(wxWindow* parent, wxString defaultpath, SaveSet& saveset);
	int DoGenerate(ConfigurationSet& config, SaveSet& saveset);

private:
	SaveSet* svset;

	void UpdateDictionaryPatchCommand();
	void OnBackgroundChoice(wxCommandEvent& event);
	void OnUpdateArg(wxCommandEvent& event);
	void OnUpdateArgSpin(wxSpinEvent& event);
};
