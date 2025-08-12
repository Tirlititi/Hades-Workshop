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
	SteamLanguage steam_language;
	bool steam_single_lang_mode;
	wxCheckBox* save_lang_box[STEAM_LANGUAGE_AMOUNT];
	bool save_lang[STEAM_LANGUAGE_AMOUNT];
	int background_resolution;
	bool prefer_export_as_patches;

	bool lang_change_allowed;
	
	PreferencesDialog(wxWindow* parent);
	int ShowModal(bool allowsteamlangchange = true, bool enablesinglemodeoption = true);
	bool SavePreferences();
	bool ReadConfiguration();
	static bool SaveMainFrameConfig(MainFrameBase* configwindow);
	static bool LoadMainFrameConfig(MainFrameBase* configwindow);
	static bool SaveToolUnityConfig(UnityViewerWindow* configwindow);
	static bool LoadToolUnityConfig(UnityViewerWindow* configwindow);
	static bool SaveToolBackgroundConfig(BackgroundEditorWindow* configwindow);
	static bool LoadToolBackgroundConfig(BackgroundEditorWindow* configwindow);
	static bool SaveToolRandomizerConfig(RandomizerWindow* configwindow);
	static bool LoadToolRandomizerConfig(RandomizerWindow* configwindow);
	static bool SaveToolCalculatorProfile(DamageCalculatorWindow* configwindow, wxString profileid, wxString profilename);
	static bool DeleteToolCalculatorProfile(DamageCalculatorWindow* configwindow, wxString profileid);
	static bool LoadToolCalculatorProfile(DamageCalculatorWindow* configwindow, wxString profileid);
	static bool GetToolCalculatorProfileList(wxArrayString* id = NULL, wxArrayString* name = NULL);

private:
	bool ReadCharmaps();
	wxArrayString CharmapNames;
	
	static bool GetBeforeAndAfterSection(wxString section, wxString& before, wxString& after);

	void UpdateSteamLanguageAvailability();

	void OnSteamLanguageChange(wxCommandEvent& event);
	void OnSingleLanguageMode(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
