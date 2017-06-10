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
	int background_resolution;
	
	PreferencesDialog(wxWindow* parent);
	int ShowModal();
	bool SavePreferences();
	bool ReadConfiguration();
	static bool SaveMainFrameConfig(MainFrameBase* configwindow);
	static bool LoadMainFrameConfig(MainFrameBase* configwindow);
	static bool SaveToolUnityConfig(UnityViewerWindow* configwindow);
	static bool LoadToolUnityConfig(UnityViewerWindow* configwindow);
	static bool SaveToolBackgroundConfig(BackgroundEditorWindow* configwindow);
	static bool LoadToolBackgroundConfig(BackgroundEditorWindow* configwindow);

private:
	bool ReadCharmaps();
	wxArrayString CharmapNames;
	
	static bool GetBeforeAndAfterSection(wxString section, wxString& before, wxString& after);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
