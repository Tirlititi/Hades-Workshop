/*********************************************************************
 * Hades Workshop
 * by Tirliti
 *********************************************************************/

#include "main.h"

#include <fstream>
#include <sstream>
#include <string>
#include "Gui_LoadingDialog.h"
#include "Gui_Preferences.h"
#include "Tool_ModManager.h"
#include "Tool_BackgroundEditor.h"
#include "Tool_UnityViewer.h"
#include "File_Batching.h"
#include "File_Manipulation.h"
#include "Configuration.h"
#include "Hades_Strings.h"
#include "makeppf3.h"

// initialize the application
IMPLEMENT_APP(MainApp);

namespace hades {
	wxColour TEXT_WINDOW_COLOR = wxColour(190,190,190);
	wxFont TEXT_DISPLAY_FONT = wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial"));
	int TEXT_PREVIEW_TYPE = 0;
	int FIELD_BACKGROUND_RESOLUTION = 32;
	wchar_t* SPECIAL_STRING_CHARMAP_DEFAULT = DEFAULT_CHARMAP;
	wchar_t* SPECIAL_STRING_CHARMAP_A = DEFAULT_SECONDARY_CHARMAP;
	wchar_t* SPECIAL_STRING_CHARMAP_B = DEFAULT_SECONDARY_CHARMAP;
	ExtendedCharmap SPECIAL_STRING_CHARMAP_EXT = ExtendedCharmap::CreateEmpty();
	wchar_t SPECIAL_STRING_OPCODE_WCHAR = OPCODE_WCHAR;
	SteamLanguage CURRENT_STEAM_LANGUAGE = STEAM_LANGUAGE_US;
	bool STEAM_LANGUAGE_SAVE_LIST[STEAM_LANGUAGE_AMOUNT] = { true, false, false, false, false, false, false };
	bool STEAM_SINGLE_LANGUAGE_MODE = false;
}

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

bool MainApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxTGAHandler);
	wxImage::AddHandler(new wxTIFFHandler);
	SetTopWindow(new MainFrame(NULL));
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(wxWindow *parent) :
	MainFrameBase(parent),
	CDPanelAmount(0) {
	SetIcon(wxICON(hadesworkshop_icon));
	wxClientDC dc(m_background);
	wxBitmap bmp = wxBITMAP_PNG(hadesworkshop_splash);
	wxImage img = bmp.ConvertToImage();
	img.Rescale(m_background->GetSize().GetWidth(),m_background->GetSize().GetHeight());
	dc.Clear();
	dc.DrawBitmap(wxBitmap(img),0,0);
	PreferenceWindow = new PreferencesDialog(this);
	PreferencesUpdate();
	PreferencesDialog::LoadMainFrameConfig(this);
	SetSteamLanguage(hades::CURRENT_STEAM_LANGUAGE);
	LoadingDialogCreate(this);
}

MainFrame::~MainFrame() {
	PreferencesDialog::SaveMainFrameConfig(this);
}

void MainFrame::MarkDataModified() {
	int currentpanel = m_cdbook->GetSelection();
	if (currentpanel==wxNOT_FOUND || CDModifiedState[currentpanel])
		return;
	CDModifiedState[currentpanel] = true;
	m_cdbook->SetPageText(currentpanel,'*'+m_cdbook->GetPageText(currentpanel));
	UpdateMenuAvailability(currentpanel);
}

void MainFrame::OnCloseFrame(wxCloseEvent& event) {
	Destroy();
}

void MainFrame::OnExitClick(wxCommandEvent& event) {
	Destroy();
}

void MainFrame::OnOpenClick(wxCommandEvent& event) {
	if (CDPanelAmount>=MAX_CD_PANELS) {
		wxLogError(HADES_STRING_OPEN_ERROR_LIMIT,MAX_CD_PANELS);
		return;
	}
	unsigned int i;
	bool success = false;
	wxFileDialog* openFileDialog = new wxFileDialog(this,HADES_STRING_OPEN_FRAME_NAME,"","",HADES_STRING_OPEN_FILTERS,wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if (openFileDialog->ShowModal() == wxID_CANCEL) {
		openFileDialog->Destroy();
		return;
	}
	ConfigurationSet* configalloc = new ConfigurationSet; // Debug : ConfigurationSet, much like fstream, must never be copied ; ToDo : delete it at some point
	ConfigurationSet& config = *configalloc;
	string filename = openFileDialog->GetPath().ToStdString();
	size_t dirsep = filename.find_last_of("/\\")+1;
	GameType gt = GAME_TYPE_PSX;
	if (filename.substr(filename.length()-4).compare(".exe")==0)
		gt = GAME_TYPE_STEAM;
	if (gt==GAME_TYPE_PSX)
		for (i=0;i<CDPanelAmount;i++)
			if (CDName[i]==filename) {
				m_cdbook->SetSelection(i);
				openFileDialog->Destroy();
				return;
			}
	else
		for (i=0;i<CDPanelAmount;i++)
			if (filename.substr(0,dirsep).compare(CDPanel[i]->filename)==0) {
				m_cdbook->SetSelection(i);
				openFileDialog->Destroy();
				return;
			}
	int res;
	if (gt==GAME_TYPE_PSX)
		res = FileCheckConfig(filename,config);
	else
		res = InitSteamConfiguration(filename.substr(0,dirsep),config);
	switch (res) {
	case -1: {
		wxLogError(HADES_STRING_OPEN_ERROR_FAIL,filename);
		break;
	}
	case -2: {
		wxMessageDialog popup(this,HADES_STRING_OPEN_WARNING_UNKNOWN,HADES_STRING_WARNING,wxYES_NO|wxSTAY_ON_TOP|wxCENTRE);
		if (popup.ShowModal()==wxID_YES) {
			int fres = FindConfiguration(filename,config);
			switch (fres) {
			case -1:
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,filename);
				break;
			case -2:
				wxLogError(HADES_STRING_CONFIG_FIND_FAIL,filename);
				break;
			case -3:
				wxLogError(HADES_STRING_CONFIG_FIND_FAIL_RNC,filename);
				break;
			default:
				wxMessageDialog popupsuccess(this,HADES_STRING_CONFIG_FIND_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxSTAY_ON_TOP|wxCENTRE);
				popupsuccess.ShowModal();
				WriteConfiguration(filename,config);
				success = true;
			}
		}
		break;
	}
	case 1: {
		wxMessageDialog popup(this,HADES_STRING_OPEN_WARNING_VERSION,HADES_STRING_WARNING,wxOK|wxCANCEL|wxSTAY_ON_TOP|wxCENTRE);
		if (popup.ShowModal()==wxID_OK) {
			int fres = FindConfiguration(filename,config);
			switch (fres) {
			case -1:
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,filename);
				break;
			case -2:
				wxLogError(HADES_STRING_CONFIG_FIND_FAIL,filename);
				break;
			default:
				wxMessageDialog popupsuccess(this,HADES_STRING_CONFIG_FIND_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxSTAY_ON_TOP|wxCENTRE);
				popupsuccess.ShowModal();
				WriteConfiguration(filename,config);
				success = true;
			}
		}
		break;
	}
	default:
		success = true;
	}
	if (success) {
		SetGameType(gt);
		if (gt==GAME_TYPE_PSX) {
			CDPanel[CDPanelAmount] = new CDDataStruct(m_cdbook,filename,config);
			CDName[CDPanelAmount] = filename.substr(dirsep);
		} else {
			CDPanel[CDPanelAmount] = new CDDataStruct(m_cdbook,filename.substr(0,dirsep),config);
			CDName[CDPanelAmount] = HADES_STRING_OPEN_STEAM_DEFAULT;
			SteamSaveDir = CDPanel[CDPanelAmount]->filename+_(HADES_STRING_STEAM_SAVE_DEFAULT);
		}
		CDModifiedState[CDPanelAmount] = false;
		m_cdbook->AddPage(CDPanel[CDPanelAmount],_(CDName[CDPanelAmount].c_str()),true,wxNullBitmap);
		m_openhws->Enable();
		m_close->Enable();
		m_closeall->Enable();
		m_modmanager->Enable();
		m_backgroundeditor->Enable(CDPanel[CDPanelAmount]->fieldloaded);
		m_background->Show(false);
		m_cdbook->Show(true);
		m_cdbook->Layout();
		m_cdbook->GetParent()->GetSizer()->Layout();
		m_cdbook->Refresh();
		CDPanelAmount++;
	}
	openFileDialog->Destroy();
}


IOHWSMessage* TheIOHWSMessageIn;
void MainFrame::OnOpenHWSClick(wxCommandEvent& event) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	if (!TheIOHWSMessageIn)
		TheIOHWSMessageIn = new IOHWSMessage(this);
	bool* section = new bool[DATA_SECTION_AMOUNT];
	bool* sectext = new bool[DATA_SECTION_AMOUNT];
	if (TheIOHWSMessageIn->ShowModal(false,section,sectext,NULL)==wxID_OK) {
		int hwslen = TheIOHWSMessageIn->m_hwspicker->GetPath().Length();
		char* hwsname = new char[hwslen+1];
		bool localsec[3];
		localsec[0] = TheIOHWSMessageIn->m_enemylocal->GetValue() && TheIOHWSMessageIn->m_enemylocal->IsEnabled();
		localsec[1] = TheIOHWSMessageIn->m_worldlocal->GetValue() && TheIOHWSMessageIn->m_worldlocal->IsEnabled();
		localsec[2] = TheIOHWSMessageIn->m_fieldlocal->GetValue() && TheIOHWSMessageIn->m_fieldlocal->IsEnabled();
		hwsname[hwslen] = 0;
		strncpy(hwsname, (const char*)TheIOHWSMessageIn->m_hwspicker->GetPath().mb_str(), hwslen);
		wstring* res = CDPanel[currentpanel]->ReadHWS(hwsname,section,sectext,localsec);
		delete[] hwsname;
		delete[] section;
		delete[] sectext;
		wxMessageDialog popupsuccess(this,_(res[0]),_(res[1]),wxOK|wxCENTRE);
		popupsuccess.ShowModal();
		delete[] res;
	}
}

void MainFrame::OnCloseClick(wxCommandEvent& event) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	if (CDPanelAmount>1) {
		for (unsigned int i=currentpanel;i+1<CDPanelAmount;i++) {
			CDName[i] = CDName[i+1];
			CDPanel[i] = CDPanel[i+1];
			CDModifiedState[i] = CDModifiedState[i+1];
		}
	} else {
		m_cdbook->Show(false);
		m_background->Show(true);
		m_background->Layout();
		m_background->GetParent()->GetSizer()->Layout();
		m_background->Update();
		RepaintBackground();
	}
	CDPanelAmount--;
	m_cdbook->DeletePage(currentpanel);
	UpdateMenuAvailability(m_cdbook->GetSelection());
}

void MainFrame::OnCloseAllClick(wxCommandEvent& event) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		m_cdbook->DeletePage(0);
	CDPanelAmount = 0;
	UpdateMenuAvailability(wxNOT_FOUND);
	m_cdbook->Show(false);
	m_background->Show(true);
	m_background->Layout();
	m_background->GetParent()->GetSizer()->Layout();
	m_background->Update();
	RepaintBackground();
}

IOHWSMessage* TheIOHWSMessageOut;
void MainFrame::OnSaveHWSClick(wxCommandEvent& event) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	unsigned int i;
	if (!TheIOHWSMessageOut)
		TheIOHWSMessageOut = new IOHWSMessage(this);
	bool* section = new bool[DATA_SECTION_AMOUNT];
	bool* sectext = new bool[DATA_SECTION_AMOUNT];
	bool* secton = new bool[DATA_SECTION_AMOUNT];
	for (i=0;i<DATA_SECTION_AMOUNT;i++) {
		section[i] =	i==DATA_SECTION_SPELL ? CDPanel[currentpanel]->spellloaded :
						i==DATA_SECTION_SUPPORT ? CDPanel[currentpanel]->supportloaded :
						i==DATA_SECTION_CMD ? CDPanel[currentpanel]->cmdloaded :
						i==DATA_SECTION_STAT ? CDPanel[currentpanel]->statloaded :
						i==DATA_SECTION_PARTY_SPECIAL ? CDPanel[currentpanel]->partyspecialloaded :
						i==DATA_SECTION_ENMY ? CDPanel[currentpanel]->enemyloaded :
						i==DATA_SECTION_ITEM ? CDPanel[currentpanel]->itemloaded :
						i==DATA_SECTION_SHOP ? CDPanel[currentpanel]->shoploaded :
						i==DATA_SECTION_CARD ? CDPanel[currentpanel]->cardloaded :
						i==DATA_SECTION_TEXT ? CDPanel[currentpanel]->textloaded :
						i==DATA_SECTION_WORLD_MAP ? CDPanel[currentpanel]->worldloaded :
						i==DATA_SECTION_FIELD ? CDPanel[currentpanel]->fieldloaded :
						i==DATA_SECTION_BATTLE_SCENE ? CDPanel[currentpanel]->sceneloaded :
						i==DATA_SECTION_SPELL_ANIM ? CDPanel[currentpanel]->spellanimloaded :
						i==DATA_SECTION_MENU_UI ? CDPanel[currentpanel]->ffuiloaded : 
						i==DATA_SECTION_ASSEMBLY ? CDPanel[currentpanel]->mipsloaded || CDPanel[currentpanel]->cilloaded : false;
	}
	for (i=0;i<DATA_SECTION_AMOUNT;i++) {
		secton[i] =		i==DATA_SECTION_SPELL ? CDPanel[currentpanel]->spellmodified :
						i==DATA_SECTION_SUPPORT ? CDPanel[currentpanel]->supportmodified :
						i==DATA_SECTION_CMD ? CDPanel[currentpanel]->cmdmodified :
						i==DATA_SECTION_STAT ? CDPanel[currentpanel]->statmodified :
						i==DATA_SECTION_PARTY_SPECIAL ? CDPanel[currentpanel]->partyspecialmodified :
						i==DATA_SECTION_ENMY ? CDPanel[currentpanel]->enemymodified :
						i==DATA_SECTION_ITEM ? CDPanel[currentpanel]->itemmodified :
						i==DATA_SECTION_SHOP ? CDPanel[currentpanel]->shopmodified :
						i==DATA_SECTION_CARD ? CDPanel[currentpanel]->cardmodified :
						i==DATA_SECTION_TEXT ? CDPanel[currentpanel]->textmodified :
						i==DATA_SECTION_WORLD_MAP ? CDPanel[currentpanel]->worldmodified :
						i==DATA_SECTION_FIELD ? CDPanel[currentpanel]->fieldmodified :
						i==DATA_SECTION_BATTLE_SCENE ? CDPanel[currentpanel]->scenemodified :
						i==DATA_SECTION_SPELL_ANIM ? CDPanel[currentpanel]->spellanimmodified :
						i==DATA_SECTION_MENU_UI ? CDPanel[currentpanel]->ffuimodified :
						i==DATA_SECTION_ASSEMBLY ? CDPanel[currentpanel]->mipsmodified || CDPanel[currentpanel]->cilmodified : false;
	}
	if (TheIOHWSMessageOut->ShowModal(true,section,sectext,secton)==wxID_OK) {
		int hwslen = TheIOHWSMessageOut->m_hwspicker->GetPath().Length();
		char* hwsname = new char[hwslen+1];
		hwsname[hwslen] = 0;
		strncpy(hwsname, (const char*)TheIOHWSMessageOut->m_hwspicker->GetPath().mb_str(), hwslen);
		bool localsec[3];
		localsec[0] = TheIOHWSMessageOut->m_enemylocal->GetValue() && TheIOHWSMessageOut->m_enemylocal->IsEnabled();
		localsec[1] = TheIOHWSMessageOut->m_worldlocal->GetValue() && TheIOHWSMessageOut->m_worldlocal->IsEnabled();
		localsec[2] = TheIOHWSMessageOut->m_fieldlocal->GetValue() && TheIOHWSMessageOut->m_fieldlocal->IsEnabled();
		if (WriteHWS(hwsname,section,localsec,CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->backupset)) {
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE,_(hwsname));
		} else {
			wxMessageDialog popupsuccess(this,HADES_STRING_HWS_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
			if (CDModifiedState[currentpanel]) {
				wxString title = m_cdbook->GetPageText(currentpanel);
				m_cdbook->SetPageText(currentpanel,title.Mid(1,title.Len()));
				CDModifiedState[currentpanel] = false;
			}
		}
		delete[] section;
		delete[] sectext;
		delete[] secton;
		delete[] hwsname;
	}
}

void MainFrame::OnSaveSteamClick(wxCommandEvent& event) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	unsigned int i;
	SaveSteamWindow dial(this);
	dial.m_dirpicker->SetDirName(SteamSaveDir);
	if (dial.ShowModal()==wxID_OK) {
		bool* modifiedsection = new bool[DATA_SECTION_AMOUNT];
		for (i=0;i<DATA_SECTION_AMOUNT;i++) {
			modifiedsection[i] =	i==DATA_SECTION_SPELL ? CDPanel[currentpanel]->spellmodified :
									i==DATA_SECTION_SUPPORT ? CDPanel[currentpanel]->supportmodified :
									i==DATA_SECTION_CMD ? CDPanel[currentpanel]->cmdmodified :
									i==DATA_SECTION_STAT ? CDPanel[currentpanel]->statmodified :
									i==DATA_SECTION_PARTY_SPECIAL ? CDPanel[currentpanel]->partyspecialmodified :
									i==DATA_SECTION_ENMY ? CDPanel[currentpanel]->enemymodified :
									i==DATA_SECTION_ITEM ? CDPanel[currentpanel]->itemmodified :
									i==DATA_SECTION_SHOP ? CDPanel[currentpanel]->shopmodified :
									i==DATA_SECTION_CARD ? CDPanel[currentpanel]->cardmodified :
									i==DATA_SECTION_TEXT ? CDPanel[currentpanel]->textmodified :
									i==DATA_SECTION_WORLD_MAP ? CDPanel[currentpanel]->worldmodified :
									i==DATA_SECTION_FIELD ? CDPanel[currentpanel]->fieldmodified :
									i==DATA_SECTION_BATTLE_SCENE ? CDPanel[currentpanel]->scenemodified :
									i==DATA_SECTION_SPELL_ANIM ? CDPanel[currentpanel]->spellanimmodified :
									i==DATA_SECTION_MENU_UI ? CDPanel[currentpanel]->ffuimodified :
									i==DATA_SECTION_CIL ? CDPanel[currentpanel]->cilmodified : false;
		}
		SteamSaveDir = dial.m_dirpicker->GetDirName();
		wxFileName::Mkdir(SteamSaveDir.GetPath()+_(L"\\StreamingAssets\\"),wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
		wxFileName::Mkdir(SteamSaveDir.GetPath()+_(L"\\x64\\FF9_Data\\"),wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
		wxFileName::Mkdir(SteamSaveDir.GetPath()+_(L"\\x64\\FF9_Data\\Managed\\"),wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
		wxArrayString dirname = SteamSaveDir.GetDirs();
		int res = CreateSteamMod(SteamSaveDir.GetPath().ToStdString(),modifiedsection,CDPanel[currentpanel]->config,CDPanel[currentpanel]->saveset,dirname.Last().compare(L"FINAL FANTASY IX")!=0);
		if (res==0) {
			wxMessageDialog popupsuccess(this,HADES_STRING_STEAM_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
		} else if (res==1) {
			wxMessageDialog popupfail(this,HADES_STRING_STEAM_SAVE_ERROR_OPENED_FILES,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popupfail.ShowModal();
		} else if (res==2) {
			wxMessageDialog popupfail(this,HADES_STRING_STEAM_SAVE_ERROR_FAIL_READ,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popupfail.ShowModal();
		} else if (res==3) {
			wxMessageDialog popupfail(this,HADES_STRING_STEAM_SAVE_ERROR_FAIL_WRITE,HADES_STRING_ERROR,wxOK|wxCENTRE);
			popupfail.ShowModal();
		}
		delete[] modifiedsection;
	}
}

ExportPPFMessage* TheExportPPFMessage;
void MainFrame::OnExportPPFClick(wxCommandEvent& event) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	if (!TheExportPPFMessage)
		TheExportPPFMessage = new ExportPPFMessage(this);
	if (TheExportPPFMessage->ShowModal()==wxID_OK) {
		int ppflen = TheExportPPFMessage->m_ppfpicker->GetPath().Length();
		int desclen = TheExportPPFMessage->m_description->GetValue().Length();
		char* ppfname = new char[ppflen+1];
		char* desc = new char[desclen+1];
		ppfname[ppflen] = 0;
		desc[desclen] = 0;
		strncpy(ppfname, (const char*)TheExportPPFMessage->m_ppfpicker->GetPath().mb_str(), ppflen);
		strncpy(desc, (const char*)TheExportPPFMessage->m_description->GetValue().mb_str(), desclen);
		if (!PPFOpenFilesForCreate(ppfname,TheExportPPFMessage->m_undo->IsChecked(),TheExportPPFMessage->m_blockcheck->IsChecked(),desc)) {
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE,_(ppfname));
			delete[] ppfname;
			delete[] desc;
			return;
		}
		if (TheExportPPFMessage->m_blockcheck->IsChecked()) {
			unsigned char binblock[1024];
			fstream ffbin(CDName[currentpanel].c_str(),ios::in | ios::binary);
			ffbin.seekg(0x9320);
			ffbin.read((char*)binblock,1024);
			ffbin.close();
			if (PPFCreateHeader(binblock)) {
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,CDName[currentpanel]);
				PPFCloseAllFiles();
				delete[] ppfname;
				delete[] desc;
				return;
			}
		} else {
			if (PPFCreateHeader(NULL)) {
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,CDName[currentpanel]);
				PPFCloseAllFiles();
				delete[] ppfname;
				delete[] desc;
				return;
			}
		}
		if (!CDPanel[currentpanel]->ExportPPF())
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,CDName[currentpanel]);
		else {
			wxMessageDialog popupsuccess(this,HADES_STRING_PPF_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
		}
		PPFCloseAllFiles();
		delete[] ppfname;
		delete[] desc;
	}
}

void MainFrame::OnSaveBinClick(wxCommandEvent& event) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	wxMessageDialog popup(this,HADES_STRING_SAVE_CONFIRM,HADES_STRING_WARNING,wxOK|wxCANCEL|wxCENTRE);
	if (popup.ShowModal()==wxID_OK) {
		if (!CDPanel[currentpanel]->OverwriteBinary()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,_(CDPanel[currentpanel]->filename));
		} else {
			m_exportppf->Enable(false);
			m_savebin->Enable(false);
			if (CDModifiedState[currentpanel]) {
				wxString title = m_cdbook->GetPageText(currentpanel);
				m_cdbook->SetPageText(currentpanel,title.Mid(1,title.Len()));
				CDModifiedState[currentpanel] = false;
			}
		}
	}
}

void MainFrame::OnSortSpellClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->spellloaded)
			CDPanel[i]->SpellDisplayNames();
}

void MainFrame::OnSortSupportClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->supportloaded)
			CDPanel[i]->SupportDisplayNames();
}

void MainFrame::OnSortCommandClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->cmdloaded)
			CDPanel[i]->CommandDisplayNames();
}

void MainFrame::OnSortEnemyClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->enemyloaded)
			CDPanel[i]->EnemyDisplayNames();
}

void MainFrame::OnSortCardClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->cardloaded)
			CDPanel[i]->CardDisplayNames();
}

void MainFrame::OnSortItemClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->itemloaded)
			CDPanel[i]->ItemDisplayNames();
}

void MainFrame::OnSortKeyItemClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->itemloaded)
			CDPanel[i]->KeyItemDisplayNames();
}

void MainFrame::OnSortTextClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->textloaded)
			CDPanel[i]->TextDisplayNames();
}

void MainFrame::OnSortWorldMapClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->worldloaded)
			CDPanel[i]->WorldMapDisplayNames();
}

void MainFrame::OnSortFieldClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->fieldloaded)
			CDPanel[i]->FieldDisplayNames();
}

void MainFrame::OnSortBattleSceneClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->sceneloaded)
			CDPanel[i]->BattleSceneDisplayNames();
}

void MainFrame::OnSortSpellAnimationClick( wxCommandEvent& event ) {
	for (unsigned int i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->spellanimloaded)
			CDPanel[i]->SpellAnimationDisplayNames();
}

wchar_t PrefCharmapDefault[256];
wchar_t PrefCharmapA[256];
wchar_t PrefCharmapB[256];

void MainFrame::PreferencesUpdate() {
	unsigned int i,j,k;
	bool changechmap = false;
	if (PreferenceWindow->m_gamealphabet->GetSelection()==0) {
		if (hades::SPECIAL_STRING_CHARMAP_DEFAULT!=DEFAULT_CHARMAP) {
			hades::SPECIAL_STRING_CHARMAP_DEFAULT = DEFAULT_CHARMAP;
			hades::SPECIAL_STRING_CHARMAP_A = DEFAULT_SECONDARY_CHARMAP;
			hades::SPECIAL_STRING_CHARMAP_B = DEFAULT_SECONDARY_CHARMAP;
			hades::SPECIAL_STRING_CHARMAP_EXT.Delete();
			changechmap = true;
		}
	} else {
		for (i=0;i<256;i++)
			if (hades::SPECIAL_STRING_CHARMAP_DEFAULT[i]!=PreferenceWindow->charmap_default[i]) {
				changechmap = true;
				break;
			}
		if (changechmap) {
			memcpy(PrefCharmapDefault,PreferenceWindow->charmap_default,256*sizeof(wchar_t));
			memcpy(PrefCharmapA,PreferenceWindow->charmap_a,256*sizeof(wchar_t));
			memcpy(PrefCharmapB,PreferenceWindow->charmap_b,256*sizeof(wchar_t));
			hades::SPECIAL_STRING_CHARMAP_DEFAULT = PrefCharmapDefault;
			hades::SPECIAL_STRING_CHARMAP_A = PrefCharmapA;
			hades::SPECIAL_STRING_CHARMAP_B = PrefCharmapB;
			hades::SPECIAL_STRING_CHARMAP_EXT = PreferenceWindow->charmap_ext.Copy();
		}
	}
	if (changechmap) {
		for (i=0;i<CDPanelAmount;i++)
			CDPanel[i]->ChangeFF9StringCharmap(hades::SPECIAL_STRING_CHARMAP_DEFAULT,hades::SPECIAL_STRING_CHARMAP_A,hades::SPECIAL_STRING_CHARMAP_B,hades::SPECIAL_STRING_CHARMAP_EXT);
	}
	hades::STEAM_SINGLE_LANGUAGE_MODE = PreferenceWindow->steam_single_lang_mode;
	if (hades::SPECIAL_STRING_OPCODE_WCHAR!=PreferenceWindow->charmap_opchar) {
		hades::SPECIAL_STRING_OPCODE_WCHAR = PreferenceWindow->charmap_opchar;
		for (i=0;i<CDPanelAmount;i++)
			CDPanel[i]->ChangeFF9StringOpcodeChar(hades::SPECIAL_STRING_OPCODE_WCHAR);
	}
	if (PreferenceWindow->menu_color==0)
		hades::TEXT_WINDOW_COLOR.Set(190,190,190);
	else
		hades::TEXT_WINDOW_COLOR.Set(0,60,255);
	hades::TEXT_PREVIEW_TYPE = PreferenceWindow->text_preview_type;
	if (hades::CURRENT_STEAM_LANGUAGE!=PreferenceWindow->steam_language) {
		hades::CURRENT_STEAM_LANGUAGE = PreferenceWindow->steam_language;
		SetSteamLanguage(hades::CURRENT_STEAM_LANGUAGE);
		for (i=0;i<CDPanelAmount;i++)
			CDPanel[i]->ChangeFF9StringSteamLanguage(hades::CURRENT_STEAM_LANGUAGE);
	}
	for (i=0;i<STEAM_LANGUAGE_AMOUNT;i++)
		hades::STEAM_LANGUAGE_SAVE_LIST[i] = PreferenceWindow->save_lang[i];
	if (hades::FIELD_BACKGROUND_RESOLUTION!=PreferenceWindow->background_resolution) {
		hades::FIELD_BACKGROUND_RESOLUTION = PreferenceWindow->background_resolution;
		for (i=0;i<CDPanelAmount;i++)
			if (CDPanel[i]->gametype!=GAME_TYPE_PSX && CDPanel[i]->fieldloaded) {
				CDPanel[i]->fieldset.tile_size = hades::FIELD_BACKGROUND_RESOLUTION;
				for (j=0;j<CDPanel[i]->fieldset.amount;j++)
					if (CDPanel[i]->fieldset.background_data[j]!=NULL)
						for (k=0;k<CDPanel[i]->fieldset.background_data[j]->camera_amount;k++)
							CDPanel[i]->fieldset.background_data[j]->camera[k].UpdateSize();
			}
	}
}

void MainFrame::OnPreferencesClick( wxCommandEvent& event ) {
	unsigned int i;
	bool steamloaded = false;
	for (i=0;i<CDPanelAmount;i++)
		if (CDPanel[i]->gametype!=GAME_TYPE_PSX) {
			steamloaded = true;
			break;
		}
	int res = PreferenceWindow->ShowModal(!hades::STEAM_SINGLE_LANGUAGE_MODE || !steamloaded,!steamloaded);
	if (res==wxID_OK) {
		PreferencesUpdate();
	} else if (res==wxID_EXIT) {
		wxLogError(HADES_STRING_PREFERENCE_OPEN_FAIL);
	}
}

void MainFrame::OnBatchExportClick( wxCommandEvent& event ) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	int id = event.GetId();
	bool dataloaded[8] = { CDPanel[currentpanel]->statloaded, CDPanel[currentpanel]->enemyloaded, CDPanel[currentpanel]->fieldloaded, CDPanel[currentpanel]->itemloaded, CDPanel[currentpanel]->spellloaded, CDPanel[currentpanel]->supportloaded, CDPanel[currentpanel]->cmdloaded, CDPanel[currentpanel]->cardloaded };
	if (id==wxID_TEXT) {
		BatchExportDialog dial(this);
		dial.ShowModal(1,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->m_textlist->GetStrings(),CDPanel[currentpanel]->textsorted,dataloaded);
	} else if (id==wxID_UITEXT) {
		BatchExportDialog dial(this);
		dial.ShowModal(2,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->m_specialtextlist->GetStrings(),NULL,dataloaded);
	} else if (id==wxID_ENMYSCRIPT) {
		BatchExportDialog dial(this);
		dial.ShowModal(3,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->m_enemylist->GetStrings(),CDPanel[currentpanel]->enemysorted,dataloaded);
	} else if (id==wxID_WORLDSCRIPT) {
		BatchExportDialog dial(this);
		dial.ShowModal(4,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->m_worldlist->GetStrings(),NULL,dataloaded);
	} else if (id==wxID_FIELDSCRIPT) {
		BatchExportDialog dial(this);
		dial.ShowModal(5,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->m_fieldlist->GetStrings(),CDPanel[currentpanel]->fieldsorted,dataloaded);
	} else if (id==wxID_BACKGROUND) {
		BatchExportDialog dial(this);
		dial.ShowModal(10,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->m_fieldlist->GetStrings(),CDPanel[currentpanel]->fieldsorted,dataloaded);
	}
}

void MainFrame::OnBatchImportClick( wxCommandEvent& event ) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	int id = event.GetId();
	if (id==wxID_IMPTEXT) {
		BatchImportDialog dial(this);
		if (dial.ShowModal(1,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->config.language & LANGUAGE_VERSION_JAPAN)==wxID_OK)
			CDPanel[currentpanel]->MarkDataTextModified(0,CHUNK_TYPE_TEXT);
	} else if (id==wxID_IMPUITEXT) {
		BatchImportDialog dial(this);
		if (dial.ShowModal(2,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->config.language & LANGUAGE_VERSION_JAPAN)==wxID_OK)
			CDPanel[currentpanel]->MarkDataMenuUIModified();
	} else if (id==wxID_IMPENMYSCRIPT) {
		BatchImportDialog dial(this);
		dial.ShowModal(3,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->config.language & LANGUAGE_VERSION_JAPAN);
	} else if (id==wxID_IMPWORLDSCRIPT) {
		BatchImportDialog dial(this);
		dial.ShowModal(4,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->config.language & LANGUAGE_VERSION_JAPAN);
	} else if (id==wxID_IMPFIELDSCRIPT) {
		BatchImportDialog dial(this);
		dial.ShowModal(5,&CDPanel[currentpanel]->saveset,CDPanel[currentpanel]->config.language & LANGUAGE_VERSION_JAPAN);
	}
}

void MainFrame::OnToolClick( wxCommandEvent& event ) {
	int currentpanel = m_cdbook->GetSelection();
	int id = event.GetId();
	if (id==wxID_TOOLMOD) {
		if (currentpanel==wxNOT_FOUND)
			return;
		ToolModManager dial(this);
		dial.ShowModal(CDPanel[currentpanel]);
		MarkDataModified();
	} else if (id==wxID_BACKEDIT) {
		ToolBackgroundEditor dial(this);
		dial.ShowModal(currentpanel!=wxNOT_FOUND ? CDPanel[currentpanel] : NULL);
	} else if (id==wxID_ASSETS) {
		ToolUnityViewer* unityviewer = new ToolUnityViewer(this);
		unityviewer->Show();
	}
}

void MainFrame::OnAboutClick( wxCommandEvent& event ) {
	AboutWindow dialog(this);
	dialog.ShowModal();
}

void MainFrame::OnHelpClick( wxCommandEvent& event ) {
	HelpWindow* dialog = new HelpWindow(this);
	dialog->Show();
}

void MainFrame::OnPanelChanged( wxAuiNotebookEvent& event ) {
	UpdateMenuAvailability(event.GetSelection());
}

void MainFrame::RepaintBackground() {
	wxClientDC dc(m_background);
	wxBitmap bmp = wxBITMAP_PNG(hadesworkshop_splash);
	wxImage img = bmp.ConvertToImage();
	img.Rescale(m_background->GetSize().GetWidth(),m_background->GetSize().GetHeight());
/*	unsigned char* data = img.GetData();
	int w = img.GetWidth(),h = img.GetHeight();
	double i,j,factor;
	for (i=0;i<w;i++)
		for (j=0;j<h;j++) {
			factor = max((i-w*0.5)*(i-w*0.5)/(w*w*0.25),(j-h*0.5)*(j-h*0.5)/(h*h*0.25));
			factor = 1-factor*factor*factor;
			data[int(j*w+i)*3] = data[int(j*w+i)*3]*factor+255*(1-factor);
			data[int(j*w+i)*3+1] = data[int(j*w+i)*3+1]*factor+255*(1-factor);
			data[int(j*w+i)*3+2] = data[int(j*w+i)*3+2]*factor+255*(1-factor);
		}*/
	bmp = wxBitmap(img);
	wxMemoryDC mdc(bmp);
	dc.Clear();
	dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
}

void MainFrame::OnFramePaint( wxPaintEvent& event ) {
	wxPaintDC udc(this);
	if (CDPanelAmount==0)
		RepaintBackground();
}

MainFrame* GetTopWindow() {
	return (MainFrame*)wxGetApp().GetTopWindow();
}

void MainFrame::UpdateMenuAvailability(int panel) {
	if (panel==wxNOT_FOUND) {
		m_openhws->Enable(false);
		m_close->Enable(false);
		m_closeall->Enable(false);
		m_savehws->Enable(false);
		m_savesteam->Enable(false);
		m_exportppf->Enable(false);
		m_savebin->Enable(false);
		m_exporttext->Enable(false);
		m_importtext->Enable(false);
		m_exportuitext->Enable(false);
		m_importuitext->Enable(false);
		m_exportenemyscript->Enable(false);
		m_importenemyscript->Enable(false);
		m_exportworldscript->Enable(false);
		m_importworldscript->Enable(false);
		m_exportfieldscript->Enable(false);
		m_importfieldscript->Enable(false);
		m_exportfieldbackground->Enable(false);
		m_modmanager->Enable(false);
		m_backgroundeditor->Enable(false);
		return;
	}
	SetGameType(CDPanel[panel]->gametype);
	m_openhws->Enable(true);
	m_close->Enable(true);
	m_closeall->Enable(true);
	m_modmanager->Enable(true);
	m_backgroundeditor->Enable(CDPanel[panel]->fieldloaded);
	m_savehws->Enable(CDModifiedState[panel]);
	m_savesteam->Enable(CDModifiedState[panel] && GetGameType()!=GAME_TYPE_PSX);
	m_exportppf->Enable(CDModifiedState[panel] && GetGameType()==GAME_TYPE_PSX);
	m_savebin->Enable(CDModifiedState[panel] && GetGameType()==GAME_TYPE_PSX);
	m_exporttext->Enable(CDPanel[panel]->textloaded);
	m_importtext->Enable(CDPanel[panel]->textloaded);
	m_exportuitext->Enable(CDPanel[panel]->ffuiloaded);
	m_importuitext->Enable(CDPanel[panel]->ffuiloaded);
	m_exportenemyscript->Enable(CDPanel[panel]->enemyloaded);
	m_importenemyscript->Enable(CDPanel[panel]->enemyloaded);
	m_exportworldscript->Enable(CDPanel[panel]->worldloaded);
	m_importworldscript->Enable(CDPanel[panel]->worldloaded);
	m_exportfieldscript->Enable(CDPanel[panel]->fieldloaded);
	m_importfieldscript->Enable(CDPanel[panel]->fieldloaded);
	m_exportfieldbackground->Enable(CDPanel[panel]->fieldloaded);
}

void MainFrame::OnDebugClick( wxCommandEvent& event ) {
	unsigned int currentpanel = m_cdbook->GetSelection();
	CDPanel[currentpanel]->DebugWrite();
}
