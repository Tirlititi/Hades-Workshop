#ifndef _GUI_PREFERENCES_H
#define _GUI_PREFERENCES_H

class PreferencesDialog;

#include "File_Manipulation.h"
#include "gui.h"

class PreferencesDialog : public PreferencesWindow {
public:
	wchar_t charmap_default[256];
	wchar_t charmap_a[256];
	wchar_t charmap_b[256];
	ExtendedCharmap charmap_ext;
	wchar_t charmap_opchar;
	int menu_color;
	int text_preview_type;
	
	PreferencesDialog(wxWindow* parent);
	int ShowModal();
	bool ReadConfiguration();
	bool SavePreferences();

private:
	bool ReadCharmaps();
	wxArrayString CharmapNames;
	
	void OnButtonClick(wxCommandEvent& event);
};

#endif
