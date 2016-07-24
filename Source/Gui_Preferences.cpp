#include "Gui_Preferences.h"

#include "Hades_Strings.h"

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

bool PreferencesDialog::SavePreferences() {
	wxFile configfilein(_(PREFERENCE_FILE_NAME),wxFile::read);
	if (!configfilein.IsOpened())
		return false;
	wxString configstring;
	if (!configfilein.ReadAll(&configstring))
		return false;
	configfilein.Close();
	wxString line,procline;
	wxString part1 = L"",part2 = L"";
	procline = GetWxStringLine(configstring);
	line = procline;
	if (procline.length()!=0)
		PreprocessLine(procline);
	while (procline!=_(L"[Preferences]") && !configstring.IsEmpty()) {
		part1 += line+_(L"\n");
		procline = GetWxStringLine(configstring);
		line = procline;
		if (procline.length()!=0)
			PreprocessLine(procline);
	}
	procline = GetWxStringLine(configstring);
	line = procline;
	if (procline.length()!=0)
		PreprocessLine(procline);
	while (procline[0]!=L'[' && !configstring.IsEmpty()) {
		procline = GetWxStringLine(configstring);
		line = procline;
		if (procline.length()!=0)
			PreprocessLine(procline);
	}
	part2 += line;
	while (!configstring.IsEmpty()) {
		line = GetWxStringLine(configstring);
		part2 += _(L"\n")+line;
	}
	wxFile configfileout(_(PREFERENCE_FILE_NAME),wxFile::write);
	configfileout.Write(part1);
	configfileout.Write(_(L"[Preferences]"));
	configfileout.Write(_(L"\nCharmap=")+CharmapNames[m_gamealphabet->GetSelection()]);
	configfileout.Write(_(L"\nOpcodeCharacter="));
	configfileout.Write(wxString(charmap_opchar));
	configfileout.Write(_(L"\nMenuColor=")+wxString::Format(wxT("%i"),menu_color));
	configfileout.Write(_(L"\nTextPreview=")+wxString::Format(wxT("%i"),text_preview_type));
	configfileout.Write(_(L"\n\n")+part2);
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
	}
	m_gamealphabet->Clear();
	m_gamealphabet->Append(CharmapNames);
	m_gamealphabet->SetSelection(chmapselected);
	m_opcodechar->ChangeValue(_(charmap_opchar));
	m_gamewindowcolor->SetSelection(menu_color);
	m_textpreviewtype->SetSelection(text_preview_type);
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
			charmap_default[254] = L'¶';
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
