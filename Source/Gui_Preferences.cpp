#include "Gui_Preferences.h"

#include <wx/tokenzr.h>
#include "Hades_Strings.h"
#include "Database_Text.h"
#include "Tool_Randomizer.h"

#define PREFERENCE_FILE_NAME "HadesWorkshop.conf"
#define UNKNOWN_CHAR L'?'

wxString TmpArgs[100];

void PreprocessLine(wxString& str);
bool GoToSection(wxString& configfile, wxString secname);
bool SearchField(wxString& configfile, wxString fieldname, wxString* args, unsigned int& argcount);

inline wxString GetWxStringLine(wxString& str) {
	wxString tmpstr;
	wxString res = str.BeforeFirst(L'\n',&tmpstr);
	str = tmpstr;
	return res;
}

#include "Database_Resource.h" // DEBUG
#include "Database_Animation.h" // DEBUG

PreferencesDialog::PreferencesDialog(wxWindow* parent) :
	PreferencesWindow(parent),
	charmap_opchar(OPCODE_WCHAR),
	menu_color(0),
	text_preview_type(0),
	steam_language(STEAM_LANGUAGE_US),
	steam_single_lang_mode(false),
	background_resolution(32),
	lang_change_allowed(true),
	CharmapNames() {
	unsigned int i;
	save_lang_box[STEAM_LANGUAGE_US] = m_steamsaveus;
	save_lang_box[STEAM_LANGUAGE_EN] = m_steamsaveuk;
	save_lang_box[STEAM_LANGUAGE_JA] = m_steamsavejap;
	save_lang_box[STEAM_LANGUAGE_GE] = m_steamsaveger;
	save_lang_box[STEAM_LANGUAGE_FR] = m_steamsavefr;
	save_lang_box[STEAM_LANGUAGE_IT] = m_steamsaveit;
	save_lang_box[STEAM_LANGUAGE_SP] = m_steamsavesp;
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
		save_lang[i] = false;
	save_lang[STEAM_LANGUAGE_US] = true;
	charmap_ext.id = new uint16_t[G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME)];
	charmap_ext.letter = new wchar_t*[G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME)];
	for (i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME);i++)
		charmap_ext.letter[i] = new wchar_t[256];
	ReadConfiguration();
	ReadCharmaps();
}

int PreferencesDialog::ShowModal(bool allowsteamlangchange, bool enablesinglemodeoption) {
	lang_change_allowed = allowsteamlangchange;
	m_steamsinglelanguage->Enable(enablesinglemodeoption);
	if (!ReadConfiguration())
		return wxID_EXIT;
	if (!enablesinglemodeoption && allowsteamlangchange && steam_single_lang_mode) { // Someone messed up with the .conf
		steam_single_lang_mode = false;
		m_steamsinglelanguage->SetValue(false);
	}
	return wxDialog::ShowModal();
}

bool PreferencesDialog::GetBeforeAndAfterSection(wxString section, wxString& before, wxString& after) {
	wxFile configfilein(_(PREFERENCE_FILE_NAME), wxFile::read);
	if (!configfilein.IsOpened())
		return false;
	wxString configstring;
	if (!configfilein.ReadAll(&configstring))
		return false;
	configfilein.Close();
	wxString line, procline;
	before = _(L"");
	after = _(L"");
	procline = GetWxStringLine(configstring);
	line = procline;
	if (procline.length() != 0)
		PreprocessLine(procline);
	while (procline != section && !configstring.IsEmpty()) {
		before += line + _(L"\n");
		procline = GetWxStringLine(configstring);
		line = procline;
		if (procline.length() != 0)
			PreprocessLine(procline);
	}
	procline = GetWxStringLine(configstring);
	line = procline;
	if (procline.length() != 0)
		PreprocessLine(procline);
	while (procline[0] != L'[' && !configstring.IsEmpty()) {
		procline = GetWxStringLine(configstring);
		line = procline;
		if (procline.length() != 0)
			PreprocessLine(procline);
	}
	after += line;
	while (!configstring.IsEmpty()) {
		line = GetWxStringLine(configstring);
		after += _(L"\n") + line;
	}
	return true;
}

void PreferencesDialog::UpdateSteamLanguageAvailability() {
	SteamLanguage lang;
	if (!lang_change_allowed) {
		m_steamsinglelanguage->SetValue(true);
		m_steamlanguage->Enable(false);
	} else {
		m_steamlanguage->Enable(true);
	}
	if (m_steamsinglelanguage->IsChecked()) {
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++) {
			save_lang[lang] = lang==m_steamlanguage->GetSelection();
			save_lang_box[lang]->SetValue(save_lang[lang]);
			save_lang_box[lang]->Enable(false);
		}
	} else {
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			save_lang_box[lang]->Enable(true);
	}
}

bool PreferencesDialog::SavePreferences() {
	unsigned int i;
	wxString before, after, comma = _(L"");
	if (!GetBeforeAndAfterSection(_(L"[Preferences]"), before, after))
		return false;
	wxFile configfileout(_(PREFERENCE_FILE_NAME),wxFile::write);
	configfileout.Write(before);
	configfileout.Write(_(L"[Preferences]"));
	configfileout.Write(_(L"\nCharmap=")+CharmapNames[m_gamealphabet->GetSelection()]);
	configfileout.Write(_(L"\nOpcodeCharacter="));
	configfileout.Write(wxString(charmap_opchar));
	configfileout.Write(_(L"\nMenuColor=")+wxString::Format(wxT("%i"),menu_color));
	configfileout.Write(_(L"\nTextPreview=")+wxString::Format(wxT("%i"),text_preview_type));
	configfileout.Write(_(L"\nSteamLanguage=")+wxString::Format(wxT("%s"),HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[steam_language]));
	configfileout.Write(_(L"\nSteamSingleLanguage=")+wxString::Format(wxT("%i"),steam_single_lang_mode ? 1 : 0));
	configfileout.Write(_(L"\nSteamSaveLanguage="));
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
		if (save_lang[i])	{ configfileout.Write(comma+wxString::Format(wxT("%s"),HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i])); comma = _(L","); }
	configfileout.Write(_(L"\nBackgroundResolution=")+wxString::Format(wxT("%i"),background_resolution));
	configfileout.Write(_(L"\n\n") + after);
	configfileout.Close();
	return true;
}

bool PreferencesDialog::ReadConfiguration() {
	wxFile configfile(_(PREFERENCE_FILE_NAME),wxFile::read);
	if (!configfile.IsOpened()) {
		configfile.Open(_(PREFERENCE_FILE_NAME),wxFile::write);
		if (configfile.IsOpened())
			configfile.Close();
		configfile.Open(_(PREFERENCE_FILE_NAME),wxFile::read);
		if (!configfile.IsOpened())
			return false;
	}
	wxString cfgall, cfgsection, cfgfield;
	if (!configfile.ReadAll(&cfgall))
		return false;
	configfile.Close();
	cfgsection = cfgall;
	unsigned int i,argcount;
	unsigned int chmapselected = 0;
	CharmapNames.Empty();
	CharmapNames.Add(_(L"Default"));
	if (GoToSection(cfgsection,_(L"Text"))) {
		cfgfield = cfgsection;
		bool newchmap;
		while (SearchField(cfgfield,_(L"Charmap"),TmpArgs,argcount)) {
			if (argcount==0)
				continue;
			newchmap = true;
			for (i=1;i<CharmapNames.GetCount();i++)
				if (TmpArgs[0]==CharmapNames[i])
					newchmap = false;
			if (newchmap)
				CharmapNames.Add(TmpArgs[0]);
		}
	}
	cfgsection = cfgall;
	if (GoToSection(cfgsection,_(L"Preferences"))) {
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"Charmap"),TmpArgs,argcount))
			for (i=0;i<CharmapNames.GetCount();i++)
				if (TmpArgs[argcount]==CharmapNames[i])
					chmapselected = i;
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"OpcodeCharacter"),TmpArgs,argcount))
			charmap_opchar = TmpArgs[argcount][0];
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"MenuColor"),TmpArgs,argcount))
			menu_color = wxAtoi(TmpArgs[argcount]);
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"TextPreview"),TmpArgs,argcount))
			text_preview_type = wxAtoi(TmpArgs[argcount]);
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"SteamLanguage"),TmpArgs,argcount))
			for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
				if (TmpArgs[argcount].IsSameAs(HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i])) {
					steam_language = i;
					break;
				}
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"SteamSingleLanguage"),TmpArgs,argcount))
			steam_single_lang_mode = wxAtoi(TmpArgs[argcount])>0;
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"SteamSaveLanguage"),TmpArgs,argcount)) {
			wxStringTokenizer savelist(TmpArgs[argcount],L",");
			wxString savetoken;
			for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
				save_lang[i] = false;
			while (savelist.HasMoreTokens()) {
				savetoken = savelist.GetNextToken();
				for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
					if (savetoken.IsSameAs(HADES_STRING_STEAM_LANGUAGE_SHORT_NAME[i])) {
						save_lang[i] = true;
						break;
					}
			}
		}
		cfgfield = cfgsection;
		if (SearchField(cfgfield,_(L"BackgroundResolution"),TmpArgs,argcount))
			background_resolution = wxAtoi(TmpArgs[argcount]);
	}
	m_gamealphabet->Clear();
	m_gamealphabet->Append(CharmapNames);
	m_gamealphabet->SetSelection(chmapselected);
	m_opcodechar->ChangeValue(_(charmap_opchar));
	m_gamewindowcolor->SetSelection(menu_color);
	m_textpreviewtype->SetSelection(text_preview_type);
	m_steamlanguage->SetSelection(steam_language);
	m_steamsinglelanguage->SetValue(steam_single_lang_mode);
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
		save_lang_box[i]->SetValue(save_lang[i]);
	m_backgroundresolution->SetValue(background_resolution);
	UpdateSteamLanguageAvailability();
	return true;
}

bool PreferencesDialog::SaveMainFrameConfig(MainFrameBase* configwindow) {
	wxString before, after;
	if (!GetBeforeAndAfterSection(_(L"[Options]"), before, after))
		return false;
	wxFile configfileout(_(PREFERENCE_FILE_NAME), wxFile::write);
	wxString comma = _(L"");
	configfileout.Write(before);
	configfileout.Write(_(L"[Options]"));
	configfileout.Write(_(L"\nSort="));
	if (configwindow->m_sortspell->IsChecked())			{ configfileout.Write(comma+_(L"spell")); comma = _(L","); }
	if (configwindow->m_sortsupport->IsChecked())		{ configfileout.Write(comma+_(L"support")); comma = _(L","); }
	if (configwindow->m_sortcmd->IsChecked())			{ configfileout.Write(comma+_(L"command")); comma = _(L","); }
	if (configwindow->m_sortitem->IsChecked())			{ configfileout.Write(comma+_(L"item")); comma = _(L","); }
	if (configwindow->m_sortkeyitem->IsChecked())		{ configfileout.Write(comma+_(L"key")); comma = _(L","); }
	if (configwindow->m_sortenemy->IsChecked())			{ configfileout.Write(comma+_(L"enemy")); comma = _(L","); }
	if (configwindow->m_sortcard->IsChecked())			{ configfileout.Write(comma+_(L"card")); comma = _(L","); }
	if (configwindow->m_sorttext->IsChecked())			{ configfileout.Write(comma+_(L"text")); comma = _(L","); }
	if (configwindow->m_sortworld->IsChecked())			{ configfileout.Write(comma+_(L"world")); comma = _(L","); }
	if (configwindow->m_sortfield->IsChecked())			{ configfileout.Write(comma+_(L"field")); comma = _(L","); }
	if (configwindow->m_sortbattlescene->IsChecked())	{ configfileout.Write(comma+_(L"scene")); comma = _(L","); }
	if (configwindow->m_sortspellanim->IsChecked())		{ configfileout.Write(comma+_(L"spellanim")); comma = _(L","); }
	configfileout.Write(_(L"\nEnemyID=") + wxString::Format(wxT("%i"), configwindow->m_enemyshowid->IsChecked()));
	configfileout.Write(_(L"\nEnemySimilar=") + wxString::Format(wxT("%i"), configwindow->m_editsimilarenemy->IsChecked()));
	configfileout.Write(_(L"\nFieldID=") + wxString::Format(wxT("%i"), configwindow->m_fieldshowid->IsChecked()));
	configfileout.Write(_(L"\n\n") + after);
	configfileout.Close();
	return true;
}

bool PreferencesDialog::LoadMainFrameConfig(MainFrameBase* configwindow) {
	wxFile configfile(_(PREFERENCE_FILE_NAME), wxFile::read);
	if (!configfile.IsOpened())
		return false;
	wxString cfgstr, cfgfield;
	if (!configfile.ReadAll(&cfgstr))
		return false;
	configfile.Close();
	unsigned int argcount;
	if (GoToSection(cfgstr, _(L"Options"))) {
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"Sort"), TmpArgs, argcount)) {
			wxStringTokenizer sortlist(TmpArgs[argcount],L",");
			wxString sorttoken;
			while (sortlist.HasMoreTokens()) {
				sorttoken = sortlist.GetNextToken();
				if (sorttoken.IsSameAs(_(L"spell")))			configwindow->m_sortspell->Check();
				else if (sorttoken.IsSameAs(_(L"support")))		configwindow->m_sortsupport->Check();
				else if (sorttoken.IsSameAs(_(L"command")))		configwindow->m_sortcmd->Check();
				else if (sorttoken.IsSameAs(_(L"item")))		configwindow->m_sortitem->Check();
				else if (sorttoken.IsSameAs(_(L"key")))			configwindow->m_sortkeyitem->Check();
				else if (sorttoken.IsSameAs(_(L"enemy")))		configwindow->m_sortenemy->Check();
				else if (sorttoken.IsSameAs(_(L"card")))		configwindow->m_sortcard->Check();
				else if (sorttoken.IsSameAs(_(L"text")))		configwindow->m_sorttext->Check();
				else if (sorttoken.IsSameAs(_(L"world")))		configwindow->m_sortworld->Check();
				else if (sorttoken.IsSameAs(_(L"field")))		configwindow->m_sortfield->Check();
				else if (sorttoken.IsSameAs(_(L"scene")))		configwindow->m_sortbattlescene->Check();
				else if (sorttoken.IsSameAs(_(L"spellanim")))	configwindow->m_sortspellanim->Check();
			}
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"EnemyID"), TmpArgs, argcount))
			configwindow->m_enemyshowid->Check(!TmpArgs[argcount].IsSameAs(_(L"0")));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"EnemySimilar"), TmpArgs, argcount))
			configwindow->m_editsimilarenemy->Check(!TmpArgs[argcount].IsSameAs(_(L"0")));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"FieldID"), TmpArgs, argcount))
			configwindow->m_fieldshowid->Check(!TmpArgs[argcount].IsSameAs(_(L"0")));
	}
	return true;
}

bool PreferencesDialog::SaveToolUnityConfig(UnityViewerWindow* configwindow) {
	wxString before, after;
	if (!GetBeforeAndAfterSection(_(L"[UnityViewer]"), before, after))
		return false;
	wxFile configfileout(_(PREFERENCE_FILE_NAME), wxFile::write);
	wxString comma = _(L"");
	wxString token;
	configfileout.Write(before);
	configfileout.Write(_(L"[UnityViewer]"));
	configfileout.Write(_(L"\nFullPath=") + wxString::Format(wxT("%i"), configwindow->m_menuexportpath->IsChecked()));
	token = configwindow->m_menuconvertimgnone->IsChecked() ? _(L"none") :
		configwindow->m_menuconvertimgbmp->IsChecked() ? _(L"bmp") :
		configwindow->m_menuconvertimgpng->IsChecked() ? _(L"png") :
		configwindow->m_menuconvertimgtga->IsChecked() ? _(L"tga") :
		configwindow->m_menuconvertimgtiff->IsChecked() ? _(L"tiff") : _(L"bmp");
	configfileout.Write(_(L"\nImgFormat=") + token);
	token = configwindow->m_menuconvertimgqualitysame->IsChecked() ? _(L"same") :
		configwindow->m_menuconvertimgqualityalpha->IsChecked() ? _(L"alpha") :
		configwindow->m_menuconvertimgqualityrgb->IsChecked() ? _(L"rgb") :
		configwindow->m_menuconvertimgqualityrgba->IsChecked() ? _(L"rgba") :
		configwindow->m_menuconvertimgqualityargb->IsChecked() ? _(L"argb") :
		configwindow->m_menuconvertimgqualitydxt1->IsChecked() ? _(L"dxt1") :
		configwindow->m_menuconvertimgqualitydxt5->IsChecked() ? _(L"dxt5") : _(L"same");
	configfileout.Write(_(L"\nImgCompression=") + token);
	token = configwindow->m_menuconvertimgqualitylow->IsChecked() ? _(L"low") :
		configwindow->m_menuconvertimgqualitymedium->IsChecked() ? _(L"medium") :
		configwindow->m_menuconvertimgqualityhigh->IsChecked() ? _(L"high") : _(L"medium");
	configfileout.Write(_(L"\nImgQuality=") + token);
	token = configwindow->m_menuconvertaudionone->IsChecked() ? _(L"raw") :
		configwindow->m_menuconvertaudioakb->IsChecked() ? _(L"akbheader") : _(L"akbheader");
	configfileout.Write(_(L"\nAudioFormat=") + token);
	token = configwindow->m_menuconvertmodelnone->IsChecked() ? _(L"none") :
		configwindow->m_menuconvertmodelfbxbin->IsChecked() ? _(L"fbx_bin") :
		configwindow->m_menuconvertmodelfbxtext->IsChecked() ? _(L"fbx_ascii") :
		configwindow->m_menuconvertmodelautocad->IsChecked() ? _(L"dxf") :
		configwindow->m_menuconvertmodelcollada->IsChecked() ? _(L"dae") :
		configwindow->m_menuconvertmodelwave->IsChecked() ? _(L"obj") : _(L"fbx_bin");
	configfileout.Write(_(L"\nModelFormat=") + token);
	token = configwindow->m_menuimportmodelexistingfiles->IsChecked() ? _(L"existingonly") :
		configwindow->m_menuimportmodelmerge->IsChecked() ? _(L"merge") :
		configwindow->m_menuimportmodelimportall->IsChecked() ? _(L"allnew") : _(L"merge");
	configfileout.Write(_(L"\nModelImportPolicy=") + token);
	token = configwindow->m_menuimportmodeldontflush->IsChecked() ? _(L"no") :
		configwindow->m_menuimportmodelflush->IsChecked() ? _(L"yes") : _(L"yes");
	configfileout.Write(_(L"\nModelFlush=") + token);
	configfileout.Write(_(L"\nModelImportType="));
	if (configwindow->m_menuimportmodelmesh->IsChecked())		{ configfileout.Write(comma+_(L"meshmat")); comma = _(L","); }
	if (configwindow->m_menuimportmodelanims->IsChecked())		{ configfileout.Write(comma+_(L"anim")); comma = _(L","); }
	token = configwindow->m_menuconvertanimnone->IsChecked() ? _(L"none") :
		configwindow->m_menuconvertanimjson->IsChecked() ? _(L"json") : _(L"json");
	configfileout.Write(_(L"\nAnimFormat=") + token);
	token = configwindow->m_menufolderx86->IsChecked() ? _(L"x86") :
		configwindow->m_menufolderx64->IsChecked() ? _(L"x64") : _(L"x86");
	configfileout.Write(_(L"\nArchFolder=") + token);
	configfileout.Write(_(L"\n\n") + after);
	configfileout.Close();
	return true;
}

bool PreferencesDialog::LoadToolUnityConfig(UnityViewerWindow* configwindow) {
	wxFile configfile(_(PREFERENCE_FILE_NAME), wxFile::read);
	if (!configfile.IsOpened())
		return false;
	wxString cfgstr, cfgfield;
	if (!configfile.ReadAll(&cfgstr))
		return false;
	configfile.Close();
	unsigned int argcount;
	if (GoToSection(cfgstr, _(L"UnityViewer"))) {
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"FullPath"), TmpArgs, argcount))
			configwindow->m_menuexportpath->Check(!TmpArgs[argcount].IsSameAs(_(L"0")));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ImgFormat"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"none")))			configwindow->m_menuconvertimgnone->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"bmp")))		configwindow->m_menuconvertimgbmp->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"png")))		configwindow->m_menuconvertimgpng->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"tga")))		configwindow->m_menuconvertimgtga->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"tiff")))	configwindow->m_menuconvertimgtiff->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ImgCompression"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"same")))			configwindow->m_menuconvertimgqualitysame->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"alpha")))	configwindow->m_menuconvertimgqualityalpha->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"rgb")))		configwindow->m_menuconvertimgqualityrgb->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"rgba")))	configwindow->m_menuconvertimgqualityrgba->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"argb")))	configwindow->m_menuconvertimgqualityargb->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"dxt1")))	configwindow->m_menuconvertimgqualitydxt1->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"dxt5")))	configwindow->m_menuconvertimgqualitydxt5->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ImgQuality"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"low")))			configwindow->m_menuconvertimgqualitylow->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"medium")))	configwindow->m_menuconvertimgqualitymedium->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"high")))	configwindow->m_menuconvertimgqualityhigh->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"AudioFormat"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"raw")))				configwindow->m_menuconvertaudionone->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"akbheader")))	configwindow->m_menuconvertaudioakb->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ModelFormat"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"none")))				configwindow->m_menuconvertmodelnone->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"fbx_bin")))		configwindow->m_menuconvertmodelfbxbin->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"fbx_ascii")))	configwindow->m_menuconvertmodelfbxtext->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"dxf")))			configwindow->m_menuconvertmodelautocad->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"dae")))			configwindow->m_menuconvertmodelcollada->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"obj")))			configwindow->m_menuconvertmodelwave->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ModelImportPolicy"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"existingonly")))		configwindow->m_menuimportmodelexistingfiles->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"merge")))		configwindow->m_menuimportmodelmerge->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"allnew")))		configwindow->m_menuimportmodelimportall->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ModelFlush"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"no")))				configwindow->m_menuimportmodeldontflush->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"yes")))			configwindow->m_menuimportmodelflush->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ModelImportType"), TmpArgs, argcount)) {
			wxStringTokenizer sortlist(TmpArgs[argcount],L",");
			wxString sorttoken;
			configwindow->m_menuimportmodelmesh->Check(false);
			configwindow->m_menuimportmodelanims->Check(false);
			while (sortlist.HasMoreTokens()) {
				sorttoken = sortlist.GetNextToken();
				if (sorttoken.IsSameAs(_(L"meshmat")))				configwindow->m_menuimportmodelmesh->Check();
				else if (sorttoken.IsSameAs(_(L"anim")))			configwindow->m_menuimportmodelanims->Check();
			}
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"AnimFormat"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"none")))			configwindow->m_menuconvertanimnone->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"json")))	configwindow->m_menuconvertanimjson->Check();
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ArchFolder"), TmpArgs, argcount)) {
			if (TmpArgs[argcount].IsSameAs(_(L"x86")))		configwindow->m_menufolderx86->Check();
			else if (TmpArgs[argcount].IsSameAs(_(L"x64")))	configwindow->m_menufolderx64->Check();
		}
	}
	return true;
}

bool PreferencesDialog::SaveToolBackgroundConfig(BackgroundEditorWindow* configwindow) {
	wxString before, after;
	if (!GetBeforeAndAfterSection(_(L"[BackgroundEditor]"), before, after))
		return false;
	wxFile configfileout(_(PREFERENCE_FILE_NAME), wxFile::write);
	configfileout.Write(before);
	configfileout.Write(_(L"[BackgroundEditor]"));
	if (configwindow->m_exportdir->GetPath()!=configwindow->m_massexportdir->GetPath() || configwindow->m_exportdir->GetPath()!=configwindow->m_importdir->GetPath()) {
		configfileout.Write(_(L"\nAtlasDir[Converter]=") + configwindow->m_exportdir->GetPath());
		configfileout.Write(_(L"\nAtlasDir[MassConverter]=") + configwindow->m_massexportdir->GetPath());
		configfileout.Write(_(L"\nAtlasDir[Importer]=") + configwindow->m_importdir->GetPath());
	} else {
		configfileout.Write(_(L"\nAtlasDir=") + configwindow->m_exportdir->GetPath());
	}
	if (configwindow->m_sortlayer->IsChecked()!=configwindow->m_masssortlayer->IsChecked()) {
		configfileout.Write(_(L"\nSortLayers[Converter]=") + wxString::Format(wxT("%i"), configwindow->m_sortlayer->IsChecked()));
		configfileout.Write(_(L"\nSortLayers[MassConverter]=") + wxString::Format(wxT("%i"), configwindow->m_masssortlayer->IsChecked()));
	} else {
		configfileout.Write(_(L"\nSortLayers=") + wxString::Format(wxT("%i"), configwindow->m_sortlayer->IsChecked()));
	}
	if (configwindow->m_revertlayer->IsChecked()!=configwindow->m_massrevertlayer->IsChecked()) {
		configfileout.Write(_(L"\nRevertLayers[Converter]=") + wxString::Format(wxT("%i"), configwindow->m_revertlayer->IsChecked()));
		configfileout.Write(_(L"\nRevertLayers[MassConverter]=") + wxString::Format(wxT("%i"), configwindow->m_massrevertlayer->IsChecked()));
	} else {
		configfileout.Write(_(L"\nRevertLayers=") + wxString::Format(wxT("%i"), configwindow->m_revertlayer->IsChecked()));
	}
	if (configwindow->m_resolution->GetValue()!=configwindow->m_massresolution->GetValue()) {
		configfileout.Write(_(L"\nResolution[Converter]=") + wxString::Format(wxT("%i"), configwindow->m_resolution->GetValue()));
		configfileout.Write(_(L"\nResolution[MassConverter]=") + wxString::Format(wxT("%i"), configwindow->m_massresolution->GetValue()));
	} else {
		configfileout.Write(_(L"\nResolution=") + wxString::Format(wxT("%i"), configwindow->m_resolution->GetValue()));
	}
	if (configwindow->m_convertformat->GetSelection()!=configwindow->m_massconvertformat->GetSelection() || configwindow->m_dxtflagchoice->GetSelection()!=configwindow->m_massdxtflagchoice->GetSelection()) {
		if (configwindow->m_convertformat->GetSelection()<3) configfileout.Write(_(L"\nCompression[Converter]=") + configwindow->m_convertformat->GetStringSelection());
		else configfileout.Write(_(L"\nCompression[Converter]=") + wxString::Format(wxT("%s,%s"), configwindow->m_convertformat->GetStringSelection(), configwindow->m_dxtflagchoice->GetStringSelection()));
		if (configwindow->m_massconvertformat->GetSelection()<3) configfileout.Write(_(L"\nCompression[MassConverter]=") + configwindow->m_massconvertformat->GetStringSelection());
		else configfileout.Write(_(L"\nCompression[MassConverter]=") + wxString::Format(wxT("%s,%s"), configwindow->m_massconvertformat->GetStringSelection(), configwindow->m_massdxtflagchoice->GetStringSelection()));
	} else {
		if (configwindow->m_convertformat->GetSelection()<3) configfileout.Write(_(L"\nCompression=") + configwindow->m_convertformat->GetStringSelection());
		else configfileout.Write(_(L"\nCompression=") + wxString::Format(wxT("%s,%s"), configwindow->m_convertformat->GetStringSelection(), configwindow->m_dxtflagchoice->GetStringSelection()));
	}
	configfileout.Write(_(L"\nImgFile=") + configwindow->m_imagepicker->GetPath());
	configfileout.Write(_(L"\nMassImgFile=") + configwindow->m_massimageimporter->GetPath());
	configfileout.Write(_(L"\nMassImgFileFormat=") + configwindow->m_massimageformat->GetValue());
	configfileout.Write(_(L"\nMassFieldID=") + wxString::Format(wxT("%i"), configwindow->m_massfieldid->IsChecked()));
	configfileout.Write(_(L"\nImportLauncher=") + configwindow->m_importlauncher->GetPath());
	configfileout.Write(_(L"\n\n") + after);
	configfileout.Close();
	return true;
}

bool PreferencesDialog::LoadToolBackgroundConfig(BackgroundEditorWindow* configwindow) {
	wxFile configfile(_(PREFERENCE_FILE_NAME), wxFile::read);
	if (!configfile.IsOpened())
		return false;
	wxString cfgstr, cfgfield;
	if (!configfile.ReadAll(&cfgstr))
		return false;
	configfile.Close();
	unsigned int argcount;
	if (GoToSection(cfgstr, _(L"BackgroundEditor"))) {
		cfgfield = cfgstr;
		while (SearchField(cfgfield, _(L"AtlasDir"), TmpArgs, argcount)) {
			if (argcount==0) {
				configwindow->m_exportdir->SetPath(TmpArgs[argcount]);
				configwindow->m_massexportdir->SetPath(TmpArgs[argcount]);
				configwindow->m_importdir->SetPath(TmpArgs[argcount]);
			} else if (TmpArgs[0].IsSameAs(_(L"Converter")))	configwindow->m_exportdir->SetPath(TmpArgs[argcount]);
			else if (TmpArgs[0].IsSameAs(_(L"MassConverter")))	configwindow->m_massexportdir->SetPath(TmpArgs[argcount]);
			else if (TmpArgs[0].IsSameAs(_(L"Importer")))		configwindow->m_importdir->SetPath(TmpArgs[argcount]);
		}
		cfgfield = cfgstr;
		while (SearchField(cfgfield, _(L"SortLayers"), TmpArgs, argcount)) {
			if (argcount==0) {
				configwindow->m_sortlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
				configwindow->m_masssortlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
			} else if (TmpArgs[0].IsSameAs(_(L"Converter")))	configwindow->m_sortlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
			else if (TmpArgs[0].IsSameAs(_(L"MassConverter")))	configwindow->m_masssortlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
		}
		cfgfield = cfgstr;
		while (SearchField(cfgfield, _(L"RevertLayers"), TmpArgs, argcount)) {
			if (argcount==0) {
				configwindow->m_revertlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
				configwindow->m_massrevertlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
			} else if (TmpArgs[0].IsSameAs(_(L"Converter")))	configwindow->m_revertlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
			else if (TmpArgs[0].IsSameAs(_(L"MassConverter")))	configwindow->m_massrevertlayer->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
		}
		cfgfield = cfgstr;
		while (SearchField(cfgfield, _(L"Resolution"), TmpArgs, argcount)) {
			if (argcount==0) {
				configwindow->m_resolution->SetValue(wxAtoi(TmpArgs[argcount]));
				configwindow->m_massresolution->SetValue(wxAtoi(TmpArgs[argcount]));
			} else if (TmpArgs[0].IsSameAs(_(L"Converter")))	configwindow->m_resolution->SetValue(wxAtoi(TmpArgs[argcount]));
			else if (TmpArgs[0].IsSameAs(_(L"MassConverter")))	configwindow->m_massresolution->SetValue(wxAtoi(TmpArgs[argcount]));
		}
		cfgfield = cfgstr;
		while (SearchField(cfgfield, _(L"Compression"), TmpArgs, argcount)) {
			wxString cvtformat = TmpArgs[argcount].BeforeFirst(L',');
			wxString cvtquality = TmpArgs[argcount].AfterLast(L',');
			if (argcount==0) {
				configwindow->m_convertformat->SetStringSelection(cvtformat);
				configwindow->m_dxtflagchoice->SetStringSelection(cvtquality);
				configwindow->m_dxtflagchoice->Enable(configwindow->m_convertformat->GetSelection()>=3);
				configwindow->m_massconvertformat->SetStringSelection(cvtformat);
				configwindow->m_massdxtflagchoice->SetStringSelection(cvtquality);
				configwindow->m_massdxtflagchoice->Enable(configwindow->m_massconvertformat->GetSelection()>=3);
			} else if (TmpArgs[0].IsSameAs(_(L"Converter"))) {
				configwindow->m_convertformat->SetStringSelection(cvtformat);
				configwindow->m_dxtflagchoice->SetStringSelection(cvtquality);
				configwindow->m_dxtflagchoice->Enable(configwindow->m_convertformat->GetSelection()>=3);
			} else if (TmpArgs[0].IsSameAs(_(L"MassConverter"))) {
				configwindow->m_massconvertformat->SetStringSelection(cvtformat);
				configwindow->m_massdxtflagchoice->SetStringSelection(cvtquality);
				configwindow->m_massdxtflagchoice->Enable(configwindow->m_massconvertformat->GetSelection()>=3);
			}
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ImgFile"), TmpArgs, argcount))
			configwindow->m_imagepicker->SetPath(TmpArgs[argcount]);
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"MassImgFile"), TmpArgs, argcount))
			configwindow->m_massimageimporter->SetPath(TmpArgs[argcount]);
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"MassImgFileFormat"), TmpArgs, argcount))
			configwindow->m_massimageformat->SetValue(TmpArgs[argcount]);
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"MassFieldID"), TmpArgs, argcount))
			configwindow->m_massfieldid->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ImportLauncher"), TmpArgs, argcount))
			configwindow->m_importlauncher->SetPath(TmpArgs[argcount]);
	}
	return true;
}

bool PreferencesDialog::SaveToolRandomizerConfig(RandomizerWindow* configwindowuncast) {
	ToolRandomizer* configwindow = static_cast<ToolRandomizer*>(configwindowuncast);
	wxString before, after;
	if (!GetBeforeAndAfterSection(_(L"[Randomizer]"), before, after))
		return false;
	wxFile configfileout(_(PREFERENCE_FILE_NAME), wxFile::write);
	unsigned int i;
	wxString line;
	configfileout.Write(before);
	configfileout.Write(_(L"[Randomizer]"));
	configfileout.Write(_(L"\nUseRandomSeed=") + wxString::Format(wxT("%i"), configwindow->m_enableseed->IsChecked()));
	if (configwindow->used_seed>=0)
		configfileout.Write(_(L"\nLastUsedRandomSeed=") + wxString::Format(wxT("%i"), configwindow->used_seed));
	configfileout.Write(_(L"\nSafeAbilities[SpellStats]="));
	for (i = 0; i<configwindow->safe_abil_scramble.size(); i++)
		configfileout.Write(wxString::Format(wxT("%i"), configwindow->safe_abil_scramble[i]) + (i+1==configwindow->safe_abil_scramble.size() ? _(L"") : _(L",")));
	configfileout.Write(_(L"\nSafeAbilities[Spells]="));
	for (i = 0; i<configwindow->safe_abil_spell.size(); i++)
		configfileout.Write(wxString::Format(wxT("%i"), configwindow->safe_abil_spell[i]) + (i+1==configwindow->safe_abil_spell.size() ? _(L"") : _(L",")));
	configfileout.Write(_(L"\nSafeAbilities[Supports]="));
	for (i = 0; i<configwindow->safe_abil_support.size(); i++)
		configfileout.Write(wxString::Format(wxT("%i"), configwindow->safe_abil_support[i]) + (i+1==configwindow->safe_abil_support.size() ? _(L"") : _(L",")));
	configfileout.Write(_(L"\nSafeAbilities[Weapons]="));
	for (i = 0; i<configwindow->safe_abil_weap.size(); i++)
		configfileout.Write(wxString::Format(wxT("%i"), configwindow->safe_abil_weap[i]) + (i+1==configwindow->safe_abil_weap.size() ? _(L"") : _(L",")));
	configfileout.Write(_(L"\nSafeAbilities[Armors]="));
	for (i = 0; i<configwindow->safe_abil_armor.size(); i++)
		configfileout.Write(wxString::Format(wxT("%i"), configwindow->safe_abil_armor[i]) + (i+1==configwindow->safe_abil_armor.size() ? _(L"") : _(L",")));
	configfileout.Write(_(L"\nQuinaAP=") + wxString::Format(wxT("%i"), configwindow->m_charquinaap->GetValue()));
	configfileout.Write(_(L"\nWeaponSlotProp=") + wxString::Format(wxT("%i"), configwindow->m_weapslot->GetValue()));
	configfileout.Write(_(L"\nWeaponSupportProp=") + wxString::Format(wxT("%i"), configwindow->m_weapsupport->GetValue()));
	configfileout.Write(_(L"\nArmorSlotProp=") + wxString::Format(wxT("%i"), configwindow->m_armorslot->GetValue()));
	configfileout.Write(_(L"\nArmorSupportProp=") + wxString::Format(wxT("%i"), configwindow->m_armorsupport->GetValue()));
	configfileout.Write(_(L"\nEnemyMPScaling=") + wxString::Format(wxT("%i"), configwindow->m_battlempfactor->GetValue()));
	line = _(L"\nOptions[Characters]=");
	if (configwindow->m_charspellpower->GetSelection()==1)			line += _(L"shuffle_power,");
	else if (configwindow->m_charspellpower->GetSelection()==2)		line += _(L"randomize_power,");
	if (configwindow->m_charspellstatus->GetSelection()==1)			line += _(L"shuffle_status,");
	else if (configwindow->m_charspellstatus->GetSelection()==2)	line += _(L"randomize_status,");
	if (configwindow->m_charspellstatsafe->IsChecked())				line += _(L"safe_spell_stat,");
	if (configwindow->m_charspell->IsChecked())						line += _(L"shuffle_spell,");
	if (configwindow->m_charmpboost->GetSelection()==1)				line += _(L"mpx4_sumspell,");
	else if (configwindow->m_charmpboost->GetSelection()==2)		line += _(L"mpx4_sumcmd,");
	if (configwindow->m_charsummon->IsChecked())					line += _(L"summon_apart,");
	if (configwindow->m_charelan->IsChecked())						line += _(L"elan_principle,");
	if (configwindow->m_charspellsafe->IsChecked())					line += _(L"safe_spell,");
	if (configwindow->m_charsupport->IsChecked())					line += _(L"randomize_support,");
	if (configwindow->m_charsupportsafe->IsChecked())				line += _(L"safe_support,");
	if (line.Right(1).IsSameAs(L",")) line.RemoveLast();
	configfileout.Write(line);
	line = _(L"\nOptions[Weapons]=");
	if (configwindow->m_weapabil->GetSelection()==1)		line += _(L"shuffle_abil,");
	else if (configwindow->m_weapabil->GetSelection()==2)	line += _(L"randomize_abil,");
	if (configwindow->m_weapall->IsChecked())				line += _(L"ensure_all,");
	if (configwindow->m_weapsafe->IsChecked())				line += _(L"safe_weapon,");
	if (line.Right(1).IsSameAs(L",")) line.RemoveLast();
	configfileout.Write(line);
	line = _(L"\nOptions[Armors]=");
	if (configwindow->m_armorabil->GetSelection()==1)		line += _(L"shuffle_abil,");
	else if (configwindow->m_armorabil->GetSelection()==2)	line += _(L"randomize_abil,");
	if (configwindow->m_armorall->IsChecked())				line += _(L"ensure_all,");
	if (configwindow->m_armorsafe->IsChecked())				line += _(L"safe_armor,");
	if (line.Right(1).IsSameAs(L",")) line.RemoveLast();
	configfileout.Write(line);
	line = _(L"\nOptions[Battles]=");
	if (configwindow->m_battlespell->IsChecked())	line += _(L"shuffle_enemy_spell,");
	if (configwindow->m_battleprop->IsChecked())	line += _(L"same_proportion,");
	if (configwindow->m_battlemp->IsChecked())		line += _(L"scale_mp,");
	if (line.Right(1).IsSameAs(L",")) line.RemoveLast();
	configfileout.Write(line);
	line = _(L"\nOptions[Cards]=");
	if (configwindow->m_carddrop->IsChecked())				line += _(L"randomize_drop,");
	if (configwindow->m_carddeck->GetSelection()==1)		line += _(L"shuffle_deck,");
	else if (configwindow->m_carddeck->GetSelection()==2)	line += _(L"randomize_deck,");
	if (line.Right(1).IsSameAs(L",")) line.RemoveLast();
	configfileout.Write(line);
	configfileout.Write(_(L"\n\n") + after);
	configfileout.Close();
	return true;
}

bool PreferencesDialog::LoadToolRandomizerConfig(RandomizerWindow* configwindowuncast) {
	ToolRandomizer* configwindow = static_cast<ToolRandomizer*>(configwindowuncast);
	wxFile configfile(_(PREFERENCE_FILE_NAME), wxFile::read);
	if (!configfile.IsOpened())
		return false;
	wxString cfgstr, cfgfield;
	if (!configfile.ReadAll(&cfgstr))
		return false;
	configfile.Close();
	unsigned int argcount;
	unsigned int i, j;
	if (GoToSection(cfgstr, _(L"Randomizer"))) {
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"UseRandomSeed"), TmpArgs, argcount))
			configwindow->m_enableseed->SetValue(!TmpArgs[argcount].IsSameAs(_(L"0")));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"LastUsedRandomSeed"), TmpArgs, argcount))
			configwindow->used_seed = wxAtoi(TmpArgs[argcount]);
		cfgfield = cfgstr;
		bool clearsafe[5], safe[5];
		vector<uint8_t>* safelist[5] = { &configwindow->safe_abil_scramble, &configwindow->safe_abil_spell, &configwindow->safe_abil_support, &configwindow->safe_abil_weap, &configwindow->safe_abil_armor };
		for (i=0; i<5; i++)
			clearsafe[i] = true;
		while (SearchField(cfgfield, _(L"SafeAbilities"), TmpArgs, argcount)) {
			wxStringTokenizer abillist(TmpArgs[argcount],L",");
			wxString abiltoken;
			uint8_t abilid;
			bool duplicate;
			safe[0] = argcount==0 || TmpArgs[0].IsSameAs(L"Characters") || TmpArgs[0].IsSameAs(L"SpellStats");
			safe[1] = argcount==0 || TmpArgs[0].IsSameAs(L"Characters") || TmpArgs[0].IsSameAs(L"Spells");
			safe[2] = argcount==0 || TmpArgs[0].IsSameAs(L"Characters") || TmpArgs[0].IsSameAs(L"Supports");
			safe[3] = argcount==0 || TmpArgs[0].IsSameAs(L"Equipments") || TmpArgs[0].IsSameAs(L"Weapons");
			safe[4] = argcount==0 || TmpArgs[0].IsSameAs(L"Equipments") || TmpArgs[0].IsSameAs(L"Armors");
			for (i=0; i<5; i++)
				if (safe[i] && clearsafe[i]) {
					safelist[i]->clear();
					clearsafe[i] = false;
				}
			while (abillist.HasMoreTokens()) {
				abiltoken = abillist.GetNextToken();
				if (!abiltoken.IsNumber())
					continue;
				abilid = wxAtoi(abiltoken);
				for (i=0; i<5; i++)
					if (safe[i]) {
						duplicate = false;
						for (j=0; j<safelist[i]->size(); j++)
							if ((*safelist[i])[j]==abilid) {
								duplicate = true;
								break;
							}
						if (!duplicate) safelist[i]->push_back(abilid);
					}
			}
		}
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"QuinaAP"), TmpArgs, argcount))
			configwindow->m_charquinaap->SetValue(wxAtoi(TmpArgs[argcount]));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"WeaponSlotProp"), TmpArgs, argcount))
			configwindow->m_weapslot->SetValue(wxAtoi(TmpArgs[argcount]));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"WeaponSupportProp"), TmpArgs, argcount))
			configwindow->m_weapsupport->SetValue(wxAtoi(TmpArgs[argcount]));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ArmorSlotProp"), TmpArgs, argcount))
			configwindow->m_armorslot->SetValue(wxAtoi(TmpArgs[argcount]));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"ArmorSupportProp"), TmpArgs, argcount))
			configwindow->m_armorsupport->SetValue(wxAtoi(TmpArgs[argcount]));
		cfgfield = cfgstr;
		if (SearchField(cfgfield, _(L"EnemyMPScaling"), TmpArgs, argcount))
			configwindow->m_battlempfactor->SetValue(wxAtoi(TmpArgs[argcount]));
		cfgfield = cfgstr;
		while (SearchField(cfgfield, _(L"Options"), TmpArgs, argcount)) {
			wxStringTokenizer optionlist(TmpArgs[argcount],L",");
			wxString optiontoken;
			bool sectionchar = argcount==0 || TmpArgs[0].IsSameAs(L"Characters");
			bool sectionweap = argcount==0 || TmpArgs[0].IsSameAs(L"Equipments") || TmpArgs[0].IsSameAs(L"Weapons");
			bool sectionarmor = argcount==0 || TmpArgs[0].IsSameAs(L"Equipments") || TmpArgs[0].IsSameAs(L"Armors");
			bool sectionbattle = argcount==0 || TmpArgs[0].IsSameAs(L"Battles");
			bool sectioncard = argcount==0 || TmpArgs[0].IsSameAs(L"Cards");
			if (sectionchar) {
				configwindow->m_charspellpower->SetSelection(0);
				configwindow->m_charspellstatus->SetSelection(0);
				configwindow->m_charspellstatsafe->SetValue(false);
				configwindow->m_charspell->SetValue(false);
				configwindow->m_charmpboost->SetSelection(0);
				configwindow->m_charsummon->SetValue(false);
				configwindow->m_charelan->SetValue(false);
				configwindow->m_charspellsafe->SetValue(false);
				configwindow->m_charsupport->SetValue(false);
				configwindow->m_charsupportsafe->SetValue(false);
			}
			if (sectionweap) {
				configwindow->m_weapabil->SetSelection(0);
				configwindow->m_weapall->SetValue(false);
				configwindow->m_weapsafe->SetValue(false);
			}
			if (sectionarmor) {
				configwindow->m_armorabil->SetSelection(0);
				configwindow->m_armorall->SetValue(false);
				configwindow->m_armorsafe->SetValue(false);
			}
			if (sectionbattle) {
				configwindow->m_battlespell->SetValue(false);
				configwindow->m_battleprop->SetValue(false);
				configwindow->m_battlemp->SetValue(false);
			}
			if (sectioncard) {
				configwindow->m_carddrop->SetValue(false);
				configwindow->m_carddeck->SetSelection(0);
			}
			while (optionlist.HasMoreTokens()) {
				optiontoken = optionlist.GetNextToken();
				if (sectionchar && optiontoken.IsSameAs(L"shuffle_power")) configwindow->m_charspellpower->SetSelection(1);
				if (sectionchar && optiontoken.IsSameAs(L"randomize_power")) configwindow->m_charspellpower->SetSelection(2);
				if (sectionchar && optiontoken.IsSameAs(L"shuffle_status")) configwindow->m_charspellstatus->SetSelection(1);
				if (sectionchar && optiontoken.IsSameAs(L"randomize_status")) configwindow->m_charspellstatus->SetSelection(2);
				if (sectionchar && optiontoken.IsSameAs(L"safe_spell_stat")) configwindow->m_charspellstatsafe->SetValue(true);
				if (sectionchar && optiontoken.IsSameAs(L"shuffle_spell")) configwindow->m_charspell->SetValue(true);
				if (sectionchar && optiontoken.IsSameAs(L"mpx4_sumspell")) configwindow->m_charmpboost->SetSelection(1);
				if (sectionchar && optiontoken.IsSameAs(L"mpx4_sumcmd")) configwindow->m_charmpboost->SetSelection(2);
				if (sectionchar && optiontoken.IsSameAs(L"summon_apart")) configwindow->m_charsummon->SetValue(true);
				if (sectionchar && optiontoken.IsSameAs(L"elan_principle")) configwindow->m_charelan->SetValue(true);
				if (sectionchar && optiontoken.IsSameAs(L"safe_spell")) configwindow->m_charspellsafe->SetValue(true);
				if (sectionchar && optiontoken.IsSameAs(L"randomize_support")) configwindow->m_charsupport->SetValue(true);
				if (sectionchar && optiontoken.IsSameAs(L"safe_support")) configwindow->m_charsupportsafe->SetValue(true);
				if (sectionweap && optiontoken.IsSameAs(L"shuffle_abil")) configwindow->m_weapabil->SetSelection(1);
				if (sectionweap && optiontoken.IsSameAs(L"randomize_abil")) configwindow->m_weapabil->SetSelection(2);
				if (sectionweap && optiontoken.IsSameAs(L"ensure_all")) configwindow->m_weapall->SetValue(true);
				if (sectionweap && optiontoken.IsSameAs(L"safe_weapon")) configwindow->m_weapsafe->SetValue(true);
				if (sectionarmor && optiontoken.IsSameAs(L"shuffle_abil")) configwindow->m_armorabil->SetSelection(1);
				if (sectionarmor && optiontoken.IsSameAs(L"randomize_abil")) configwindow->m_armorabil->SetSelection(2);
				if (sectionarmor && optiontoken.IsSameAs(L"ensure_all")) configwindow->m_armorall->SetValue(true);
				if (sectionarmor && optiontoken.IsSameAs(L"safe_armor")) configwindow->m_armorsafe->SetValue(true);
				if (sectionbattle && optiontoken.IsSameAs(L"shuffle_enemy_spell")) configwindow->m_battlespell->SetValue(true);
				if (sectionbattle && optiontoken.IsSameAs(L"same_proportion")) configwindow->m_battleprop->SetValue(true);
				if (sectionbattle && optiontoken.IsSameAs(L"scale_mp")) configwindow->m_battlemp->SetValue(true);
				if (sectioncard && optiontoken.IsSameAs(L"randomize_drop")) configwindow->m_carddrop->SetValue(true);
				if (sectioncard && optiontoken.IsSameAs(L"shuffle_deck")) configwindow->m_carddeck->SetSelection(1);
				if (sectioncard && optiontoken.IsSameAs(L"randomize_deck")) configwindow->m_carddeck->SetSelection(2);
			}
		}
	}
	return true;
}

bool PreferencesDialog::ReadCharmaps() {
	if (m_gamealphabet->GetSelection()>0) {
		wxString chmapselectedstr = m_gamealphabet->GetStringSelection();
		wxFile configfile(_(PREFERENCE_FILE_NAME),wxFile::read);
		if (!configfile.IsOpened()) {
			m_gamealphabet->SetSelection(0);
			return false;
		}
		wxString cfgall, cfgsection, cfgfield;
		if (!configfile.ReadAll(&cfgall))
			return false;
		configfile.Close();
		cfgsection = cfgall;
		if (!GoToSection(cfgsection,_(L"Text"))) {
			m_gamealphabet->SetSelection(0);
			return false;
		}
		cfgfield = cfgsection;
		bool chmapread[4] = { false, false, false, false };
		charmap_ext.amount = 0;
		unsigned int i,argcount,exti;
		wxString::const_iterator it;
		while (SearchField(cfgfield,_(L"Charmap"),TmpArgs,argcount)) {
			if (argcount>0) {
				if (TmpArgs[0]==chmapselectedstr) {
					if (argcount==1 || TmpArgs[1]==_(L"Default")) {
						chmapread[0] = true;
						for (i=0;i<TmpArgs[argcount].length();i++)
							charmap_default[i] = TmpArgs[argcount][i];
						while (i<247)
							charmap_default[i++] = UNKNOWN_CHAR;
					} else if (TmpArgs[1]==_(L"A")) {
						chmapread[1] = true;
						for (i=0;i<TmpArgs[argcount].length();i++)
							charmap_a[i] = TmpArgs[argcount][i];
						while (i<256)
							charmap_a[i++] = UNKNOWN_CHAR;
					} else if (TmpArgs[1]==_(L"B")) {
						chmapread[2] = true;
						for (i=0;i<TmpArgs[argcount].length();i++)
							charmap_b[i] = TmpArgs[argcount][i];
						while (i<256)
							charmap_b[i++] = UNKNOWN_CHAR;
					} else if (TmpArgs[1]==_(L"Ext")) {
						chmapread[3] = true;
						uint16_t extid;
						bool newtable = true;
						if (argcount<3)
							extid = 0;
						else
							extid = wxAtoi(TmpArgs[2]);
						for (i=0;i<charmap_ext.amount;i++)
							if (extid==charmap_ext.id[i]) {
								newtable = false;
								exti = i;
								break;
							}
						if (newtable) {
							exti = charmap_ext.amount;
							charmap_ext.id[exti] = extid;
							charmap_ext.amount++;
						}
						for (i=0;i<TmpArgs[argcount].length();i++)
							charmap_ext.letter[exti][i] = TmpArgs[argcount][i];
						while (i<256)
							charmap_ext.letter[exti][i++] = UNKNOWN_CHAR;
					}
				}
			}
		}
		if (!chmapread[0]) {
			m_gamealphabet->SetSelection(0);
			return false;
		} else {
			if (!chmapread[1])
				for (i=0;i<256;i++)
					charmap_a[i] = UNKNOWN_CHAR;
			if (!chmapread[2])
				for (i=0;i<256;i++)
					charmap_b[i] = UNKNOWN_CHAR;
			charmap_default[247] = L'\n';
			charmap_default[248] = L'\t';
			charmap_default[249] = charmap_opchar;
			charmap_default[250] = UNKNOWN_CHAR;
			charmap_default[251] = UNKNOWN_CHAR;
			charmap_default[252] = UNKNOWN_CHAR;
			charmap_default[253] = UNKNOWN_CHAR;
			charmap_default[254] = L'\xB6'; // ¶
			charmap_default[255] = L'\0';
		}
	}
	return true;
}

void PreferencesDialog::OnSteamLanguageChange(wxCommandEvent& event) {
	SteamLanguage lang, newlang = event.GetSelection();
	if (m_steamsinglelanguage->IsChecked()) {
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			save_lang_box[lang]->SetValue(lang==newlang);
	} else {
		SteamLanguage lastlang = STEAM_LANGUAGE_AMOUNT;
		bool unchecklast = false;
		for (lang=0;lang<STEAM_LANGUAGE_AMOUNT;lang++)
			if (save_lang_box[lang]->IsChecked()) {
				if (lastlang==STEAM_LANGUAGE_AMOUNT) {
					lastlang = lang;
					unchecklast = true;
				} else {
					unchecklast = false;
					break;
				}
			}
		if (unchecklast)
			save_lang_box[lastlang]->SetValue(false);
		save_lang_box[newlang]->SetValue(true);
	}
}

void PreferencesDialog::OnSingleLanguageMode(wxCommandEvent & event) {
	UpdateSteamLanguageAvailability();
}

void PreferencesDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_OK) {
		wxString opchar = m_opcodechar->GetValue();
		if (opchar.length()==0)
			charmap_opchar = OPCODE_WCHAR;
		else
			charmap_opchar = opchar[0];
		ReadCharmaps();
		menu_color = m_gamewindowcolor->GetSelection();
		text_preview_type = m_textpreviewtype->GetSelection();
		steam_language = m_steamlanguage->GetSelection();
		steam_single_lang_mode = m_steamsinglelanguage->IsChecked();
		for (unsigned int i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
			save_lang[i] = save_lang_box[i]->IsChecked();
		background_resolution = m_backgroundresolution->GetValue();
		SavePreferences();
		return EndModal(id);
	} else if (id==wxID_CANCEL) {
		
		return EndModal(id);
	}
}

void PreprocessLine(wxString& str) {
	str = str.substr(str.find_first_not_of(L" \t"),str.find_last_not_of(L" \t")+1);
	if (str[0]==L'#')
		str = L"";
}

bool GoToSection(wxString& cfgstr, wxString secname) {
	wxString section = _(L"[")+wxString(secname)+_(L"]");
	wxString line;
	while (!cfgstr.IsEmpty()) {
		line = GetWxStringLine(cfgstr);
		if (line.length()==0)
			continue;
		PreprocessLine(line);
		if (line==section)
			return true;
	}
	return false;
}

bool SearchField(wxString& cfgstr, wxString fieldname, wxString* args, unsigned int& argcount) {
	wxString line, field;
	argcount = 0;
	while (!cfgstr.IsEmpty()) {
		line = GetWxStringLine(cfgstr);
		if (line.length()==0)
			continue;
		PreprocessLine(line);
		if (line[0]==L'[')
			break;
		field = line.substr(0,line.find_first_of(L"=[ "));
		if (field==fieldname) {
			line = line.substr(field.length());
			while (line[0]==L'[') {
				args[argcount++] = line.substr(1,line.find_first_of(L"]")-1);
				line = line.substr(line.find_first_of(L"]")+1);
			}
			args[argcount] = line.substr(line.find_first_of(L"=")+1);
			return true;
		}
	}
	return false;
}
