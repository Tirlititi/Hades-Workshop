#include "Gui_Preferences.h"

#include <wx/tokenzr.h>
#include "Hades_Strings.h"
#include "Database_Text.h"

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

PreferencesDialog::PreferencesDialog(wxWindow* parent) :
	PreferencesWindow(parent),
	charmap_opchar(OPCODE_WCHAR),
	menu_color(0),
	text_preview_type(0),
	background_resolution(32),
	CharmapNames() {
	charmap_ext.id = new uint16_t[G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME)];
	charmap_ext.letter = new wchar_t*[G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME)];
	for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME);i++)
		charmap_ext.letter[i] = new wchar_t[256];
	ReadConfiguration();
	ReadCharmaps();
}

int PreferencesDialog::ShowModal() {
	if (!ReadConfiguration())
		return wxID_EXIT;
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

bool PreferencesDialog::SavePreferences() {
	wxString before, after;
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
		if (SearchField(cfgfield,_(L"BackgroundResolution"),TmpArgs,argcount))
			background_resolution = wxAtoi(TmpArgs[argcount]);
	}
	m_gamealphabet->Clear();
	m_gamealphabet->Append(CharmapNames);
	m_gamealphabet->SetSelection(chmapselected);
	m_opcodechar->ChangeValue(_(charmap_opchar));
	m_gamewindowcolor->SetSelection(menu_color);
	m_textpreviewtype->SetSelection(text_preview_type);
	m_backgroundresolution->SetValue(background_resolution);
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
	}
	return true;
}

bool PreferencesDialog::SaveToolUnityConfig(UnityViewerWindow* configwindow) {
	wxString before, after;
	if (!GetBeforeAndAfterSection(_(L"[UnityViewer]"), before, after))
		return false;
	wxFile configfileout(_(PREFERENCE_FILE_NAME), wxFile::write);
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
