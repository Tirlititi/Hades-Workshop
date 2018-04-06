///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 24 2017)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Gui_TextEditor.h"

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_menuBar = new wxMenuBar( 0 );
	m_menuFile = new wxMenu();
	m_open = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Open") ) + wxT('\t') + wxT("Ctrl+O"), _("Open Final Fantasy IX binary file"), wxITEM_NORMAL );
	m_menuFile->Append( m_open );
	
	m_openhws = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Open &Mod") ) + wxT('\t') + wxT("Ctrl+Shift+O"), _("Apply a previously created mod(ification) file"), wxITEM_NORMAL );
	m_menuFile->Append( m_openhws );
	m_openhws->Enable( false );
	
	m_close = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Close") ) + wxT('\t') + wxT("Ctrl+W"), _("Close current file"), wxITEM_NORMAL );
	m_menuFile->Append( m_close );
	m_close->Enable( false );
	
	m_closeall = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Close &All") ) + wxT('\t') + wxT("Ctrl+Shift+W"), _("Close all opened files"), wxITEM_NORMAL );
	m_menuFile->Append( m_closeall );
	m_closeall->Enable( false );
	
	m_menuFile->AppendSeparator();
	
	m_savehws = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Save Mod") ) + wxT('\t') + wxT("Ctrl+S"), _("Save the modifications as Hades Workshop save file"), wxITEM_NORMAL );
	m_menuFile->Append( m_savehws );
	m_savehws->Enable( false );
	
	m_savesteam = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Save S&team Mod") ) + wxT('\t') + wxT("Ctrl+M"), _("Create modded Steam files"), wxITEM_NORMAL );
	m_menuFile->Append( m_savesteam );
	m_savesteam->Enable( false );
	
	m_exportppf = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Export as PPF") ) + wxT('\t') + wxT("Ctrl+E"), _("Export the changes to a .ppf file"), wxITEM_NORMAL );
	m_menuFile->Append( m_exportppf );
	m_exportppf->Enable( false );
	
	m_savebin = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("Overwrite &Binary File") ) + wxT('\t') + wxT("Ctrl+B"), _("Directly save the file"), wxITEM_NORMAL );
	m_menuFile->Append( m_savebin );
	m_savebin->Enable( false );
	
	m_menuFile->AppendSeparator();
	
	m_preferences = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("&Preferences") ) + wxT('\t') + wxT("Ctrl+P"), _("Change the editor's preferences"), wxITEM_NORMAL );
	m_menuFile->Append( m_preferences );
	
	m_menuFile->AppendSeparator();
	
	m_exit = new wxMenuItem( m_menuFile, wxID_ANY, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+F4"), _("Close Hades Workshop"), wxITEM_NORMAL );
	m_menuFile->Append( m_exit );
	
	m_menuBar->Append( m_menuFile, _("&File") ); 
	
	m_menuOption = new wxMenu();
	m_sortspell = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Spells") ) , _("Sort spells by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortspell );
	
	m_sortsupport = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort Supporting &Abilities") ) , _("Sort supporting abilities by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortsupport );
	
	m_sortcmd = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Commands") ) , _("Sort commands by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortcmd );
	
	m_menuOption->AppendSeparator();
	
	m_sortitem = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Items") ) , _("Sort regular items by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortitem );
	
	m_sortkeyitem = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Key Items") ) , _("Sort key items by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortkeyitem );
	
	m_menuOption->AppendSeparator();
	
	m_sortenemy = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Enemies") ) , _("Sort enemies by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortenemy );
	
	m_enemyshowid = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Show Enem&y IDs") ) + wxT('\t') + wxT("Ctrl+I"), _("Display the battles identifiers"), wxITEM_CHECK );
	m_menuOption->Append( m_enemyshowid );
	
	m_editsimilarenemy = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Edit Si&milar Enemies") ) + wxT('\t') + wxT("Ctrl+D"), _("Edit stats and attacks of enemies sharing the same name and level"), wxITEM_CHECK );
	m_menuOption->Append( m_editsimilarenemy );
	
	m_menuOption->AppendSeparator();
	
	m_sortcard = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort Ca&rds") ) , _("Sort cards by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortcard );
	
	m_menuOption->AppendSeparator();
	
	m_sorttext = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Text Blocks") ) , _("Sort text blocks by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sorttext );
	
	m_sortworld = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &World Maps") ) , _("Sort world maps by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortworld );
	
	m_sortfield = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Fields") ) , _("Sort fields by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortfield );
	
	m_sortbattlescene = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Battle Scenes") ) , _("Sort battle scenes by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortbattlescene );
	
	m_sortspellanim = new wxMenuItem( m_menuOption, wxID_ANY, wxString( _("Sort &Spell Animations") ) , _("Sort spell animations by name or by ID"), wxITEM_CHECK );
	m_menuOption->Append( m_sortspellanim );
	
	m_menuBar->Append( m_menuOption, _("&Option") ); 
	
	m_menuBatch = new wxMenu();
	m_exporttext = new wxMenuItem( m_menuBatch, wxID_TEXT, wxString( _("Export Texts") ) , _("Export all the text blocks at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_exporttext );
	m_exporttext->Enable( false );
	
	m_importtext = new wxMenuItem( m_menuBatch, wxID_IMPTEXT, wxString( _("Import Texts") ) , _("Import all the text blocks at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_importtext );
	m_importtext->Enable( false );
	
	m_exportuitext = new wxMenuItem( m_menuBatch, wxID_UITEXT, wxString( _("Export UI Texts") ) , _("Export all the interface texts at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_exportuitext );
	m_exportuitext->Enable( false );
	
	m_importuitext = new wxMenuItem( m_menuBatch, wxID_IMPUITEXT, wxString( _("Import UI Texts") ) , _("Import all the interface texts at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_importuitext );
	m_importuitext->Enable( false );
	
	m_menuBatch->AppendSeparator();
	
	m_exportenemyscript = new wxMenuItem( m_menuBatch, wxID_ENMYSCRIPT, wxString( _("Export Enemy Scripts") ) , _("Export all the enemy AI scripts at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_exportenemyscript );
	m_exportenemyscript->Enable( false );
	
	m_importenemyscript = new wxMenuItem( m_menuBatch, wxID_IMPENMYSCRIPT, wxString( _("Import Enemy Scripts") ) , _("NOT YET IMPLEMENTED"), wxITEM_NORMAL );
	m_menuBatch->Append( m_importenemyscript );
	m_importenemyscript->Enable( false );
	
	m_exportworldscript = new wxMenuItem( m_menuBatch, wxID_WORLDSCRIPT, wxString( _("Export World Map Scripts") ) , _("Export all the World Map scripts at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_exportworldscript );
	m_exportworldscript->Enable( false );
	
	m_importworldscript = new wxMenuItem( m_menuBatch, wxID_IMPWORLDSCRIPT, wxString( _("Import World Map Scripts") ) , _("NOT YET IMPLEMENTED"), wxITEM_NORMAL );
	m_menuBatch->Append( m_importworldscript );
	m_importworldscript->Enable( false );
	
	m_exportfieldscript = new wxMenuItem( m_menuBatch, wxID_FIELDSCRIPT, wxString( _("Export Field Scripts") ) , _("Export all the field scripts at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_exportfieldscript );
	m_exportfieldscript->Enable( false );
	
	m_importfieldscript = new wxMenuItem( m_menuBatch, wxID_IMPFIELDSCRIPT, wxString( _("Import Field Scripts") ) , _("NOT YET IMPLEMENTED"), wxITEM_NORMAL );
	m_menuBatch->Append( m_importfieldscript );
	m_importfieldscript->Enable( false );
	
	m_menuBatch->AppendSeparator();
	
	m_exportfieldbackground = new wxMenuItem( m_menuBatch, wxID_BACKGROUND, wxString( _("Export Field Backgrounds") ) , _("Export all the field backgrounds at once"), wxITEM_NORMAL );
	m_menuBatch->Append( m_exportfieldbackground );
	m_exportfieldbackground->Enable( false );
	
	m_menuBar->Append( m_menuBatch, _("&Batch") ); 
	
	m_menuTools = new wxMenu();
	m_modmanager = new wxMenuItem( m_menuTools, wxID_TOOLMOD, wxString( _("Modifications Manager") ) , _("Set what must be saved"), wxITEM_NORMAL );
	m_menuTools->Append( m_modmanager );
	m_modmanager->Enable( false );
	
	m_backgroundeditor = new wxMenuItem( m_menuTools, wxID_BACKEDIT, wxString( _("Background Editor") ) , _("Convert images into field backgrounds for Steam"), wxITEM_NORMAL );
	m_menuTools->Append( m_backgroundeditor );
	m_backgroundeditor->Enable( false );
	
	m_unityviewer = new wxMenuItem( m_menuTools, wxID_ASSETS, wxString( _("Unity Assets Viewer") ) , _("Export and Import assets from the game's archives"), wxITEM_NORMAL );
	m_menuTools->Append( m_unityviewer );
	
	m_menuBar->Append( m_menuTools, _("&Tools") ); 
	
	m_menuHelp = new wxMenu();
	wxMenuItem* m_about;
	m_about = new wxMenuItem( m_menuHelp, wxID_ANY, wxString( _("&About") ) + wxT('\t') + wxT("F1"), _("About Hades Workshop..."), wxITEM_NORMAL );
	m_menuHelp->Append( m_about );
	
	wxMenuItem* m_showhelp;
	m_showhelp = new wxMenuItem( m_menuHelp, wxID_ANY, wxString( _("&Help") ) + wxT('\t') + wxT("F2"), _("Open the Hades Workshop's help"), wxITEM_NORMAL );
	m_menuHelp->Append( m_showhelp );
	
	m_menuBar->Append( m_menuHelp, _("&Help") ); 
	
	this->SetMenuBar( m_menuBar );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_cdbook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_FIXED_WIDTH|wxAUI_NB_TAB_SPLIT );
	m_cdbook->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_cdbook->Hide();
	
	
	mainSizer->Add( m_cdbook, 1, wxEXPAND, 5 );
	
	m_background = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mainSizer->Add( m_background, 1, wxEXPAND | wxALL, 0 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::OnFramePaint ) );
	this->Connect( m_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpenClick ) );
	this->Connect( m_openhws->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpenHWSClick ) );
	this->Connect( m_close->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnCloseClick ) );
	this->Connect( m_closeall->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnCloseAllClick ) );
	this->Connect( m_savehws->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSaveHWSClick ) );
	this->Connect( m_savesteam->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSaveSteamClick ) );
	this->Connect( m_exportppf->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExportPPFClick ) );
	this->Connect( m_savebin->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSaveBinClick ) );
	this->Connect( m_preferences->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnPreferencesClick ) );
	this->Connect( m_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	this->Connect( m_sortspell->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortSpellClick ) );
	this->Connect( m_sortsupport->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortSupportClick ) );
	this->Connect( m_sortcmd->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortCommandClick ) );
	this->Connect( m_sortitem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortItemClick ) );
	this->Connect( m_sortkeyitem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortKeyItemClick ) );
	this->Connect( m_sortenemy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortEnemyClick ) );
	this->Connect( m_enemyshowid->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortEnemyClick ) );
	this->Connect( m_sortcard->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortCardClick ) );
	this->Connect( m_sorttext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortTextClick ) );
	this->Connect( m_sortworld->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortWorldClick ) );
	this->Connect( m_sortfield->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortFieldClick ) );
	this->Connect( m_sortbattlescene->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortBattleSceneClick ) );
	this->Connect( m_sortspellanim->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortSpellAnimationClick ) );
	this->Connect( m_exporttext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Connect( m_importtext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Connect( m_exportuitext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Connect( m_importuitext->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Connect( m_exportenemyscript->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Connect( m_importenemyscript->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Connect( m_exportworldscript->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Connect( m_importworldscript->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Connect( m_exportfieldscript->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Connect( m_importfieldscript->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Connect( m_exportfieldbackground->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Connect( m_modmanager->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnToolClick ) );
	this->Connect( m_backgroundeditor->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnToolClick ) );
	this->Connect( m_unityviewer->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnToolClick ) );
	this->Connect( m_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnAboutClick ) );
	this->Connect( m_showhelp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnHelpClick ) );
	m_cdbook->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrameBase::OnPanelChanged ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::OnFramePaint ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpenClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnOpenHWSClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnCloseClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnCloseAllClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSaveHWSClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSaveSteamClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExportPPFClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSaveBinClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnPreferencesClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortSpellClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortSupportClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortCommandClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortItemClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortKeyItemClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortEnemyClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortEnemyClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortCardClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortTextClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortWorldClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortFieldClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortBattleSceneClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnSortSpellAnimationClick ) );
	this->Disconnect( wxID_TEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Disconnect( wxID_IMPTEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Disconnect( wxID_UITEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Disconnect( wxID_IMPUITEXT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Disconnect( wxID_ENMYSCRIPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Disconnect( wxID_IMPENMYSCRIPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Disconnect( wxID_WORLDSCRIPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Disconnect( wxID_IMPWORLDSCRIPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Disconnect( wxID_FIELDSCRIPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Disconnect( wxID_IMPFIELDSCRIPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchImportClick ) );
	this->Disconnect( wxID_BACKGROUND, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnBatchExportClick ) );
	this->Disconnect( wxID_TOOLMOD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnToolClick ) );
	this->Disconnect( wxID_BACKEDIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnToolClick ) );
	this->Disconnect( wxID_ASSETS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnToolClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnAboutClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnHelpClick ) );
	m_cdbook->Disconnect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrameBase::OnPanelChanged ), NULL, this );
	
}

CDPanel::CDPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookmain = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelparty = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookparty = new wxNotebook( m_panelparty, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel21 = new wxPanel( m_notebookparty, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spelllist = new wxListBox( m_panel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxVSCROLL ); 
	bSizer5->Add( m_spelllist, 0, wxALL|wxEXPAND, 5 );
	
	m_spellscrolledwindow = new wxScrolledWindow( m_panel21, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_spellscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 17, 2, 0, 0 );
	fgSizer7->AddGrowableCol( 1 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_spellnamelabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_spellnamelabel->Wrap( -1 );
	fgSizer7->Add( m_spellnamelabel, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spellname = new wxTextCtrl( m_spellscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer22->Add( m_spellname, 0, wxALL, 2 );
	
	m_spellnamebutton = new wxButton( m_spellscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_spellnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer22->Add( m_spellnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer7->Add( bSizer22, 1, wxEXPAND, 5 );
	
	m_staticText265 = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Casting Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText265->Wrap( -1 );
	fgSizer7->Add( m_staticText265, 0, wxALL, 5 );
	
	wxArrayString m_spellperformnameChoices;
	m_spellperformname = new wxChoice( m_spellscrolledwindow, wxID_PERFNAME, wxDefaultPosition, wxDefaultSize, m_spellperformnameChoices, 0 );
	m_spellperformname->SetSelection( 0 );
	m_spellperformname->SetToolTip( _("Name displayed at the casting time") );
	
	fgSizer7->Add( m_spellperformname, 0, wxALL, 2 );
	
	m_spellhelplabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Help"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_spellhelplabel->Wrap( -1 );
	fgSizer7->Add( m_spellhelplabel, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spellhelp = new wxTextCtrl( m_spellscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_spellhelp->SetToolTip( _("The spell's description shown with [Select]") );
	
	bSizer23->Add( m_spellhelp, 1, wxALL|wxEXPAND, 2 );
	
	m_spellhelpbutton = new wxButton( m_spellscrolledwindow, wxID_HELP, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_spellhelpbutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer23->Add( m_spellhelpbutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer7->Add( bSizer23, 1, wxEXPAND, 5 );
	
	m_spelleffectlabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Effect"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_spelleffectlabel->Wrap( -1 );
	fgSizer7->Add( m_spelleffectlabel, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_spelleffectChoices;
	m_spelleffect = new wxChoice( m_spellscrolledwindow, wxID_EFFECT, wxDefaultPosition, wxSize( -1,-1 ), m_spelleffectChoices, 0 );
	m_spelleffect->SetSelection( 0 );
	bSizer6->Add( m_spelleffect, 0, wxALL, 2 );
	
	m_spelleffecthelpwindow = new wxScrolledWindow( m_spellscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_spelleffecthelpwindow->SetScrollRate( 5, 5 );
	m_spelleffecthelpwindow->SetMinSize( wxSize( -1,60 ) );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );
	
	m_spelleffecthelp = new wxStaticText( m_spelleffecthelpwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_spelleffecthelp->Wrap( -1 );
	bSizer71->Add( m_spelleffecthelp, 0, wxALL, 2 );
	
	
	m_spelleffecthelpwindow->SetSizer( bSizer71 );
	m_spelleffecthelpwindow->Layout();
	bSizer71->Fit( m_spelleffecthelpwindow );
	bSizer6->Add( m_spelleffecthelpwindow, 1, wxEXPAND, 5 );
	
	
	fgSizer7->Add( bSizer6, 1, wxEXPAND, 5 );
	
	m_spellpowerlabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Power"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellpowerlabel->Wrap( -1 );
	fgSizer7->Add( m_spellpowerlabel, 0, wxALL|wxEXPAND, 5 );
	
	m_spellpower = new wxSpinCtrl( m_spellscrolledwindow, wxID_POWER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer7->Add( m_spellpower, 0, wxALL, 2 );
	
	m_spellelementlabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Element"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelementlabel->Wrap( -1 );
	fgSizer7->Add( m_spellelementlabel, 0, wxALL, 5 );
	
	wxGridSizer* m_spellelement;
	m_spellelement = new wxGridSizer( 2, 4, 0, 0 );
	
	m_spellelement1 = new wxCheckBox( m_spellscrolledwindow, wxID_SE1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement1, 0, wxALL, 5 );
	
	m_spellelement2 = new wxCheckBox( m_spellscrolledwindow, wxID_SE2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement2, 0, wxALL, 5 );
	
	m_spellelement3 = new wxCheckBox( m_spellscrolledwindow, wxID_SE3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement3, 0, wxALL, 5 );
	
	m_spellelement4 = new wxCheckBox( m_spellscrolledwindow, wxID_SE4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement4, 0, wxALL, 5 );
	
	m_spellelement5 = new wxCheckBox( m_spellscrolledwindow, wxID_SE5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement5, 0, wxALL, 5 );
	
	m_spellelement6 = new wxCheckBox( m_spellscrolledwindow, wxID_SE6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement6, 0, wxALL, 5 );
	
	m_spellelement7 = new wxCheckBox( m_spellscrolledwindow, wxID_SE7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement7, 0, wxALL, 5 );
	
	m_spellelement8 = new wxCheckBox( m_spellscrolledwindow, wxID_SE8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellelement->Add( m_spellelement8, 0, wxALL, 5 );
	
	
	fgSizer7->Add( m_spellelement, 1, wxEXPAND, 5 );
	
	m_spellstatuslabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellstatuslabel->Wrap( -1 );
	fgSizer7->Add( m_spellstatuslabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer147;
	bSizer147 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_spellstatusChoices;
	m_spellstatus = new wxChoice( m_spellscrolledwindow, wxID_STATUS, wxDefaultPosition, wxDefaultSize, m_spellstatusChoices, 0 );
	m_spellstatus->SetSelection( 0 );
	m_spellstatus->SetToolTip( _("Status to add or remove") );
	
	bSizer147->Add( m_spellstatus, 0, wxALL, 2 );
	
	m_spellstatusset = new wxButton( m_spellscrolledwindow, wxID_STATUS, _("Edit Status Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellstatusset->SetToolTip( _("Warning\nChanging a status set will modify it\nfor the every spells using the same set") );
	
	bSizer147->Add( m_spellstatusset, 0, wxALL, 2 );
	
	
	fgSizer7->Add( bSizer147, 1, wxEXPAND, 5 );
	
	m_spellaccuracylabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Accuracy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellaccuracylabel->Wrap( -1 );
	fgSizer7->Add( m_spellaccuracylabel, 0, wxALL, 5 );
	
	m_spellaccuracy = new wxSpinCtrl( m_spellscrolledwindow, wxID_ACCURACY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	m_spellaccuracy->SetToolTip( _("Pourcentage based accuracy") );
	
	fgSizer7->Add( m_spellaccuracy, 0, wxALL, 2 );
	
	m_spellmplabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("MP Cost"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmplabel->Wrap( -1 );
	fgSizer7->Add( m_spellmplabel, 0, wxALL, 5 );
	
	m_spellmp = new wxSpinCtrl( m_spellscrolledwindow, wxID_MP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer7->Add( m_spellmp, 0, wxALL, 2 );
	
	m_spellmenuflag = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Menu Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmenuflag->Wrap( -1 );
	fgSizer7->Add( m_spellmenuflag, 0, wxALL, 5 );
	
	wxGridSizer* gSizer212;
	gSizer212 = new wxGridSizer( 1, 3, 0, 0 );
	
	m_spellmenuflagmenuuse = new wxCheckBox( m_spellscrolledwindow, wxID_CAN_USE_IN_MENU, _("Can use in Menu"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmenuflagmenuuse->SetToolTip( _("The effect must be compatible") );
	
	gSizer212->Add( m_spellmenuflagmenuuse, 0, wxALL, 5 );
	
	m_spellmenuflaghideap = new wxCheckBox( m_spellscrolledwindow, wxID_HIDE_AP_PROGRESSION, _("Hide AP figures"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmenuflaghideap->SetToolTip( _("Hide the AP figures in menu but not the progression bar") );
	
	gSizer212->Add( m_spellmenuflaghideap, 0, wxALL, 5 );
	
	m_spellmenuflagmpboost = new wxCheckBox( m_spellscrolledwindow, wxID_MP_COST_BOOST, _("MP x 4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmenuflagmpboost->SetToolTip( _("Boost the MP cost early game") );
	
	gSizer212->Add( m_spellmenuflagmpboost, 0, wxALL, 5 );
	
	
	fgSizer7->Add( gSizer212, 1, wxEXPAND, 5 );
	
	m_spellmodellabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmodellabel->Wrap( -1 );
	fgSizer7->Add( m_spellmodellabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_spellmodel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmodel->Wrap( -1 );
	m_spellmodel->SetToolTip( _("For special effects, characters' animations, camera movements...\nAlso defines which and when characters are hit\nas well as when the spell ends (which can be never)") );
	
	gSizer3->Add( m_spellmodel, 0, wxALL|wxEXPAND, 5 );
	
	m_spellmodeledit = new wxButton( m_spellscrolledwindow, wxID_ANY, _("Change Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( m_spellmodeledit, 0, wxALL, 2 );
	
	
	fgSizer7->Add( gSizer3, 1, wxEXPAND, 5 );
	
	m_spellmodelaltlabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Animation 2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmodelaltlabel->Wrap( -1 );
	fgSizer7->Add( m_spellmodelaltlabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer4;
	gSizer4 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_spellmodelalt = new wxStaticText( m_spellscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_spellmodelalt->Wrap( -1 );
	m_spellmodelalt->SetToolTip( _("For single-multi target spells and eidolons mainly") );
	
	gSizer4->Add( m_spellmodelalt, 0, wxALL|wxEXPAND, 5 );
	
	m_spellmodelaltedit = new wxButton( m_spellscrolledwindow, wxID_ANY, _("Change Animation 2"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer4->Add( m_spellmodelaltedit, 0, wxALL, 2 );
	
	
	fgSizer7->Add( gSizer4, 1, wxEXPAND, 5 );
	
	m_spelltargettypelabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Target Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltargettypelabel->Wrap( -1 );
	fgSizer7->Add( m_spelltargettypelabel, 0, wxALL, 5 );
	
	wxString m_spelltargettypeChoices[] = { _("Any"), _("Ally"), _("Enemy"), _("Everyone"), _("Self") };
	int m_spelltargettypeNChoices = sizeof( m_spelltargettypeChoices ) / sizeof( wxString );
	m_spelltargettype = new wxChoice( m_spellscrolledwindow, wxID_TARGETTYPE, wxDefaultPosition, wxDefaultSize, m_spelltargettypeNChoices, m_spelltargettypeChoices, 0 );
	m_spelltargettype->SetSelection( 0 );
	fgSizer7->Add( m_spelltargettype, 0, wxALL, 2 );
	
	m_spelltargetamountlabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Target Amount"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltargetamountlabel->Wrap( -1 );
	fgSizer7->Add( m_spelltargetamountlabel, 0, wxALL, 5 );
	
	wxString m_spelltargetamountChoices[] = { _("Single"), _("Multiple"), _("Single-Multi") };
	int m_spelltargetamountNChoices = sizeof( m_spelltargetamountChoices ) / sizeof( wxString );
	m_spelltargetamount = new wxChoice( m_spellscrolledwindow, wxID_TARGETAMOUNT, wxDefaultPosition, wxDefaultSize, m_spelltargetamountNChoices, m_spelltargetamountChoices, 0 );
	m_spelltargetamount->SetSelection( 0 );
	fgSizer7->Add( m_spelltargetamount, 0, wxALL, 2 );
	
	m_spelltargetprioritylabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Target Priority"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltargetprioritylabel->Wrap( -1 );
	fgSizer7->Add( m_spelltargetprioritylabel, 0, wxALL, 5 );
	
	wxString m_spelltargetpriorityChoices[] = { _("Enemies"), _("Allies") };
	int m_spelltargetpriorityNChoices = sizeof( m_spelltargetpriorityChoices ) / sizeof( wxString );
	m_spelltargetpriority = new wxChoice( m_spellscrolledwindow, wxID_TARGETPRIORITY, wxDefaultPosition, wxDefaultSize, m_spelltargetpriorityNChoices, m_spelltargetpriorityChoices, 0 );
	m_spelltargetpriority->SetSelection( 0 );
	m_spelltargetpriority->SetToolTip( _("Which kind of target is selected first\nwhen the target is asked") );
	
	fgSizer7->Add( m_spelltargetpriority, 0, wxALL, 2 );
	
	m_spelltargetflaglabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("UI Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltargetflaglabel->Wrap( -1 );
	fgSizer7->Add( m_spelltargetflaglabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer21;
	gSizer21 = new wxGridSizer( 1, 3, 0, 0 );
	
	m_spelltargetflagdead = new wxCheckBox( m_spellscrolledwindow, wxID_CAN_TARGET_DEAD, _("Can target KO"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer21->Add( m_spelltargetflagdead, 0, wxALL, 5 );
	
	m_spelltargetflagdeadfirst = new wxCheckBox( m_spellscrolledwindow, wxID_TARGET_DEAD_FIRST, _("Target KO in priority"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer21->Add( m_spelltargetflagdeadfirst, 0, wxALL, 5 );
	
	m_spelltargetflagcamera = new wxCheckBox( m_spellscrolledwindow, wxID_TARGET_CAMERA, _("Alternate Camera"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltargetflagcamera->SetToolTip( _("Always use the alternative camera\nin spell animations using such a camera") );
	
	gSizer21->Add( m_spelltargetflagcamera, 0, wxALL, 5 );
	
	
	fgSizer7->Add( gSizer21, 1, wxEXPAND, 5 );
	
	m_spelltargetpanellabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Panel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltargetpanellabel->Wrap( -1 );
	fgSizer7->Add( m_spelltargetpanellabel, 0, wxALL, 5 );
	
	wxString m_spelltargetpanelChoices[] = { _("Normal"), _("HP"), _("MP"), _("Bad statuses"), _("Good statuses") };
	int m_spelltargetpanelNChoices = sizeof( m_spelltargetpanelChoices ) / sizeof( wxString );
	m_spelltargetpanel = new wxChoice( m_spellscrolledwindow, wxID_TARGETPANEL, wxDefaultPosition, wxDefaultSize, m_spelltargetpanelNChoices, m_spelltargetpanelChoices, 0 );
	m_spelltargetpanel->SetSelection( 0 );
	m_spelltargetpanel->SetToolTip( _("The panel type displayed when the target is asked\nOnly display informations on the allies") );
	
	fgSizer7->Add( m_spelltargetpanel, 0, wxALL, 2 );
	
	m_spellflaglabel = new wxStaticText( m_spellscrolledwindow, wxID_ANY, _("Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflaglabel->Wrap( -1 );
	fgSizer7->Add( m_spellflaglabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 2, 4, 0, 0 );
	
	m_spellflag1 = new wxCheckBox( m_spellscrolledwindow, wxID_SF1, _("Reflectable"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer6->Add( m_spellflag1, 0, wxALL, 5 );
	
	m_spellflag2 = new wxCheckBox( m_spellscrolledwindow, wxID_SF2, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag2->SetToolTip( _("Can't cast under Silence and Oeilvert's Curse") );
	
	gSizer6->Add( m_spellflag2, 0, wxALL, 5 );
	
	m_spellflag3 = new wxCheckBox( m_spellscrolledwindow, wxID_SF3, _("No M-Sword"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag3->SetToolTip( _("\"No Magic Sword\"\nUnknown effect") );
	
	gSizer6->Add( m_spellflag3, 0, wxALL, 5 );
	
	m_spellflag4 = new wxCheckBox( m_spellscrolledwindow, wxID_SF4, _("Is Physical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag4->SetToolTip( _("For removing the Confuse and Sleep statuses\nAlso triggers the party's counterattack") );
	
	gSizer6->Add( m_spellflag4, 0, wxALL, 5 );
	
	m_spellflag5 = new wxCheckBox( m_spellscrolledwindow, wxID_SF5, _("Is Magical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag5->SetToolTip( _("For removing the Vanish status") );
	
	gSizer6->Add( m_spellflag5, 0, wxALL, 5 );
	
	m_spellflag6 = new wxCheckBox( m_spellscrolledwindow, wxID_SF6, _("Short"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag6->SetToolTip( _("Unknown effect") );
	
	gSizer6->Add( m_spellflag6, 0, wxALL, 5 );
	
	m_spellflag7 = new wxCheckBox( m_spellscrolledwindow, wxID_SF7, _("Hit Anim Off"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag7->SetToolTip( _("Don't trigger the target's hit animation") );
	
	gSizer6->Add( m_spellflag7, 0, wxALL, 5 );
	
	m_spellflag8 = new wxCheckBox( m_spellscrolledwindow, wxID_SF8, _("Returnable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellflag8->SetToolTip( _("Return Magic only triggers when the caster is an enemy of the target") );
	
	gSizer6->Add( m_spellflag8, 0, wxALL, 5 );
	
	
	fgSizer7->Add( gSizer6, 1, wxEXPAND, 5 );
	
	
	m_spellscrolledwindow->SetSizer( fgSizer7 );
	m_spellscrolledwindow->Layout();
	fgSizer7->Fit( m_spellscrolledwindow );
	bSizer5->Add( m_spellscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel21->SetSizer( bSizer5 );
	m_panel21->Layout();
	bSizer5->Fit( m_panel21 );
	m_notebookparty->AddPage( m_panel21, _("Spells"), false );
	m_panel24 = new wxPanel( m_notebookparty, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxHORIZONTAL );
	
	m_supportlist = new wxListBox( m_panel24, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxVSCROLL ); 
	bSizer63->Add( m_supportlist, 0, wxALL|wxEXPAND, 5 );
	
	m_supportscrolledwindow = new wxScrolledWindow( m_panel24, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_supportscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText113 = new wxStaticText( m_supportscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113->Wrap( -1 );
	fgSizer22->Add( m_staticText113, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer221;
	bSizer221 = new wxBoxSizer( wxHORIZONTAL );
	
	m_supportname = new wxTextCtrl( m_supportscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer221->Add( m_supportname, 0, wxALL, 2 );
	
	m_supportnamebutton = new wxButton( m_supportscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_supportnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer221->Add( m_supportnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer22->Add( bSizer221, 1, wxEXPAND, 5 );
	
	m_staticText114 = new wxStaticText( m_supportscrolledwindow, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText114->Wrap( -1 );
	fgSizer22->Add( m_staticText114, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer231;
	bSizer231 = new wxBoxSizer( wxHORIZONTAL );
	
	m_supporthelp = new wxTextCtrl( m_supportscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_supporthelp->SetToolTip( _("The ability's description shown with [Select]") );
	m_supporthelp->SetMinSize( wxSize( 250,-1 ) );
	
	bSizer231->Add( m_supporthelp, 1, wxALL|wxEXPAND, 2 );
	
	m_supporthelpbutton = new wxButton( m_supportscrolledwindow, wxID_HELP, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_supporthelpbutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer231->Add( m_supporthelpbutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer22->Add( bSizer231, 1, wxEXPAND, 5 );
	
	m_staticText115 = new wxStaticText( m_supportscrolledwindow, wxID_ANY, _("Magic Stone Cost"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	fgSizer22->Add( m_staticText115, 0, wxALL|wxEXPAND, 5 );
	
	m_supportcost = new wxSpinCtrl( m_supportscrolledwindow, wxID_MAGICSTONE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer22->Add( m_supportcost, 0, wxALL, 2 );
	
	
	m_supportscrolledwindow->SetSizer( fgSizer22 );
	m_supportscrolledwindow->Layout();
	fgSizer22->Fit( m_supportscrolledwindow );
	bSizer63->Add( m_supportscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel24->SetSizer( bSizer63 );
	m_panel24->Layout();
	bSizer63->Fit( m_panel24 );
	m_notebookparty->AddPage( m_panel24, _("Supporting Abilities"), false );
	m_panel22 = new wxPanel( m_notebookparty, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cmdlist = new wxListBox( m_panel22, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer8->Add( m_cmdlist, 0, wxALL|wxEXPAND, 5 );
	
	m_cmdscrolledwindow = new wxScrolledWindow( m_panel22, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_cmdscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 6, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cmdnamelabl = new wxStaticText( m_cmdscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdnamelabl->Wrap( -1 );
	fgSizer2->Add( m_cmdnamelabl, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cmdname = new wxTextCtrl( m_cmdscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_PROCESS_ENTER );
	bSizer24->Add( m_cmdname, 0, wxALL, 2 );
	
	m_cmdnamebutton = new wxButton( m_cmdscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_cmdnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer24->Add( m_cmdnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( bSizer24, 1, wxEXPAND, 5 );
	
	m_cmdhelplabel = new wxStaticText( m_cmdscrolledwindow, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdhelplabel->Wrap( -1 );
	fgSizer2->Add( m_cmdhelplabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cmdhelp = new wxTextCtrl( m_cmdscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxTE_PROCESS_ENTER );
	m_cmdhelp->SetToolTip( _("The command's description shown with [Select]") );
	
	bSizer25->Add( m_cmdhelp, 1, wxALL|wxEXPAND, 2 );
	
	m_cmdhelpbutton = new wxButton( m_cmdscrolledwindow, wxID_HELP, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_cmdhelpbutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer25->Add( m_cmdhelpbutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( bSizer25, 1, wxEXPAND, 5 );
	
	m_cmdpanellabel = new wxStaticText( m_cmdscrolledwindow, wxID_ANY, _("Panel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdpanellabel->Wrap( -1 );
	fgSizer2->Add( m_cmdpanellabel, 0, wxALL, 5 );
	
	wxString m_cmdpanelChoices[] = { _("None"), _("Spells"), _("Items"), _("Weapons") };
	int m_cmdpanelNChoices = sizeof( m_cmdpanelChoices ) / sizeof( wxString );
	m_cmdpanel = new wxChoice( m_cmdscrolledwindow, wxID_PANEL, wxDefaultPosition, wxDefaultSize, m_cmdpanelNChoices, m_cmdpanelChoices, 0 );
	m_cmdpanel->SetSelection( 0 );
	m_cmdpanel->SetToolTip( _("Warning : changing this field\nresets the fields below") );
	
	fgSizer2->Add( m_cmdpanel, 0, wxALL, 2 );
	
	m_cmdlinklabel = new wxStaticText( m_cmdscrolledwindow, wxID_ANY, _("Linked Command"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdlinklabel->Wrap( -1 );
	fgSizer2->Add( m_cmdlinklabel, 0, wxALL, 5 );
	
	wxArrayString m_cmdlinkChoices;
	m_cmdlink = new wxChoice( m_cmdscrolledwindow, wxID_LINK, wxDefaultPosition, wxDefaultSize, m_cmdlinkChoices, 0 );
	m_cmdlink->SetSelection( 0 );
	m_cmdlink->SetToolTip( _("Linked commands share their spell list\nEditing this field may change this command's spell list") );
	
	fgSizer2->Add( m_cmdlink, 0, wxALL, 2 );
	
	m_cmdspelllabel = new wxStaticText( m_cmdscrolledwindow, wxID_ANY, _("Spell list"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdspelllabel->Wrap( -1 );
	fgSizer2->Add( m_cmdspelllabel, 0, wxALL, 5 );
	
	m_cmdspelllistpanel = new wxPanel( m_cmdscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cmdaddspell = new wxButton( m_cmdspelllistpanel, wxID_ADD, _("Add Spell"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_cmdaddspell, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_cmdremovespell = new wxButton( m_cmdspelllistpanel, wxID_REMOVE, _("Remove Spell"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_cmdremovespell, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText27 = new wxStaticText( m_cmdspelllistpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer3->Add( m_staticText27, 0, wxALL, 5 );
	
	m_cmdspellfulllist = new wxListBox( m_cmdspelllistpanel, wxID_ADD, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer3->Add( m_cmdspellfulllist, 0, wxALL, 5 );
	
	m_cmdspelllist = new wxListBox( m_cmdspelllistpanel, wxID_REMOVE, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer3->Add( m_cmdspelllist, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer89111;
	bSizer89111 = new wxBoxSizer( wxVERTICAL );
	
	m_cmdmovespellup = new wxBitmapButton( m_cmdspelllistpanel, wxID_UP, wxICON( moveup_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_cmdmovespellup->SetBitmapDisabled( wxICON( moveup_disicon ) );
	bSizer89111->Add( m_cmdmovespellup, 0, wxALL, 5 );
	
	m_cmdmovespelldown = new wxBitmapButton( m_cmdspelllistpanel, wxID_DOWN, wxICON( movedown_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_cmdmovespelldown->SetBitmapDisabled( wxICON( movedown_disicon ) );
	bSizer89111->Add( m_cmdmovespelldown, 0, wxALL, 5 );
	
	
	fgSizer3->Add( bSizer89111, 1, wxEXPAND, 5 );
	
	
	m_cmdspelllistpanel->SetSizer( fgSizer3 );
	m_cmdspelllistpanel->Layout();
	fgSizer3->Fit( m_cmdspelllistpanel );
	fgSizer2->Add( m_cmdspelllistpanel, 1, wxEXPAND | wxALL, 2 );
	
	m_cmdobjectslabel = new wxStaticText( m_cmdscrolledwindow, wxID_ANY, _("Object range"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdobjectslabel->Wrap( -1 );
	fgSizer2->Add( m_cmdobjectslabel, 0, wxALL, 5 );
	
	m_cmdobjectpanel = new wxPanel( m_cmdscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cmdobjectfirstlabel = new wxStaticText( m_cmdobjectpanel, wxID_ANY, _("From :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdobjectfirstlabel->Wrap( -1 );
	m_cmdobjectfirstlabel->SetToolTip( _("For \"Items\" and \"Weapons\" panels only") );
	
	fgSizer5->Add( m_cmdobjectfirstlabel, 0, wxALL, 5 );
	
	m_cmdobjectendlabel = new wxStaticText( m_cmdobjectpanel, wxID_ANY, _("To :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cmdobjectendlabel->Wrap( -1 );
	m_cmdobjectendlabel->SetToolTip( _("For \"Items\" and \"Weapons\" panels only") );
	
	fgSizer5->Add( m_cmdobjectendlabel, 0, wxALL, 5 );
	
	wxArrayString m_cmdobjectfirstChoices;
	m_cmdobjectfirst = new wxChoice( m_cmdobjectpanel, wxID_OBJECTFIRST, wxDefaultPosition, wxDefaultSize, m_cmdobjectfirstChoices, 0 );
	m_cmdobjectfirst->SetSelection( 0 );
	m_cmdobjectfirst->SetToolTip( _("First and last objects included") );
	
	fgSizer5->Add( m_cmdobjectfirst, 0, wxALL, 5 );
	
	wxArrayString m_cmdobjectlastChoices;
	m_cmdobjectlast = new wxChoice( m_cmdobjectpanel, wxID_OBJECTLAST, wxDefaultPosition, wxDefaultSize, m_cmdobjectlastChoices, 0 );
	m_cmdobjectlast->SetSelection( 0 );
	m_cmdobjectlast->SetToolTip( _("First and last objects included") );
	
	fgSizer5->Add( m_cmdobjectlast, 0, wxALL, 5 );
	
	
	m_cmdobjectpanel->SetSizer( fgSizer5 );
	m_cmdobjectpanel->Layout();
	fgSizer5->Fit( m_cmdobjectpanel );
	fgSizer2->Add( m_cmdobjectpanel, 1, wxEXPAND | wxALL, 5 );
	
	
	m_cmdscrolledwindow->SetSizer( fgSizer2 );
	m_cmdscrolledwindow->Layout();
	fgSizer2->Fit( m_cmdscrolledwindow );
	bSizer8->Add( m_cmdscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel22->SetSizer( bSizer8 );
	m_panel22->Layout();
	bSizer8->Fit( m_panel22 );
	m_notebookparty->AddPage( m_panel22, _("Commands"), false );
	m_panel25 = new wxPanel( m_notebookparty, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer64;
	bSizer64 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statlist = new wxListBox( m_panel25, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxVSCROLL ); 
	bSizer64->Add( m_statlist, 0, wxALL|wxEXPAND, 5 );
	
	m_statcharscrolledwindow = new wxScrolledWindow( m_panel25, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_statcharscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer23;
	fgSizer23 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer23->SetFlexibleDirection( wxBOTH );
	fgSizer23->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1131 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Default Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1131->Wrap( -1 );
	fgSizer23->Add( m_staticText1131, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer2211;
	bSizer2211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statchardefaultname = new wxTextCtrl( m_statcharscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	#ifdef __WXGTK__
	if ( !m_statchardefaultname->HasFlag( wxTE_MULTILINE ) )
	{
	m_statchardefaultname->SetMaxLength( 7 );
	}
	#else
	m_statchardefaultname->SetMaxLength( 7 );
	#endif
	m_statchardefaultname->SetToolTip( _("The character's name by default when naming a character\nUnused for temporary playable characters") );
	
	bSizer2211->Add( m_statchardefaultname, 0, wxALL, 2 );
	
	m_statchardefaultnamebutton = new wxButton( m_statcharscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_statchardefaultnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer2211->Add( m_statchardefaultnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer23->Add( bSizer2211, 1, wxEXPAND, 5 );
	
	m_staticText11311 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Base Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11311->Wrap( -1 );
	fgSizer23->Add( m_staticText11311, 0, wxALL, 5 );
	
	m_statcharspeed = new wxSpinCtrl( m_statcharscrolledwindow, wxID_SPEED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 50, 0 );
	fgSizer23->Add( m_statcharspeed, 0, wxALL, 2 );
	
	m_staticText113111 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Base Strength"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113111->Wrap( -1 );
	fgSizer23->Add( m_staticText113111, 0, wxALL, 5 );
	
	m_statcharstrength = new wxSpinCtrl( m_statcharscrolledwindow, wxID_STRENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0 );
	fgSizer23->Add( m_statcharstrength, 0, wxALL, 2 );
	
	m_staticText113112 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Base Magic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113112->Wrap( -1 );
	fgSizer23->Add( m_staticText113112, 0, wxALL, 5 );
	
	m_statcharmagic = new wxSpinCtrl( m_statcharscrolledwindow, wxID_MAGIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0 );
	fgSizer23->Add( m_statcharmagic, 0, wxALL, 2 );
	
	m_staticText113113 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Base Spirit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113113->Wrap( -1 );
	fgSizer23->Add( m_staticText113113, 0, wxALL, 5 );
	
	m_statcharspirit = new wxSpinCtrl( m_statcharscrolledwindow, wxID_SPIRIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 50, 0 );
	fgSizer23->Add( m_statcharspirit, 0, wxALL, 2 );
	
	m_staticText113114 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Base Magic Stones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113114->Wrap( -1 );
	fgSizer23->Add( m_staticText113114, 0, wxALL, 5 );
	
	m_statcharmagicstone = new wxSpinCtrl( m_statcharscrolledwindow, wxID_MAGICSTONE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0 );
	fgSizer23->Add( m_statcharmagicstone, 0, wxALL, 2 );
	
	m_staticText113115 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Ability Lists"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113115->Wrap( -1 );
	fgSizer23->Add( m_staticText113115, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer43->AddGrowableRow( 1 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_statcharabilitysetChoices;
	m_statcharabilityset = new wxChoice( m_statcharscrolledwindow, wxID_ABILITYSET, wxDefaultPosition, wxDefaultSize, m_statcharabilitysetChoices, 0 );
	m_statcharabilityset->SetSelection( 0 );
	fgSizer43->Add( m_statcharabilityset, 0, wxALL, 3 );
	
	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statcharabilitylist = new wxListBox( m_statcharscrolledwindow, wxID_ABILITY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_statcharabilitylist->SetToolTip( _("Both active and supporting abilities\ndisplayed in the Ability menu") );
	
	bSizer75->Add( m_statcharabilitylist, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer881;
	bSizer881 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_statcharabilitychoiceChoices;
	m_statcharabilitychoice = new wxChoice( m_statcharscrolledwindow, wxID_ABILITY, wxDefaultPosition, wxDefaultSize, m_statcharabilitychoiceChoices, 0 );
	m_statcharabilitychoice->SetSelection( 0 );
	bSizer881->Add( m_statcharabilitychoice, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText165 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("AP Requirement :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText165->Wrap( -1 );
	bSizer102->Add( m_staticText165, 0, wxALL, 0 );
	
	m_statcharabilitycost = new wxSpinCtrl( m_statcharscrolledwindow, wxID_ABILITYCOST, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	bSizer102->Add( m_statcharabilitycost, 0, wxALL, 5 );
	
	
	bSizer881->Add( bSizer102, 1, wxEXPAND, 5 );
	
	
	bSizer75->Add( bSizer881, 1, wxEXPAND, 5 );
	
	
	fgSizer43->Add( bSizer75, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1971 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1971->Wrap( -1 );
	fgSizer42->Add( m_staticText1971, 0, wxALL, 1 );
	
	m_staticText1981 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1981->Wrap( -1 );
	fgSizer42->Add( m_staticText1981, 0, wxALL, 1 );
	
	wxArrayString m_statcharcommand1Choices;
	m_statcharcommand1 = new wxChoice( m_statcharscrolledwindow, wxID_COMMAND1, wxDefaultPosition, wxDefaultSize, m_statcharcommand1Choices, 0 );
	m_statcharcommand1->SetSelection( 0 );
	fgSizer42->Add( m_statcharcommand1, 0, wxALL, 3 );
	
	wxArrayString m_statcharcommand2Choices;
	m_statcharcommand2 = new wxChoice( m_statcharscrolledwindow, wxID_COMMAND2, wxDefaultPosition, wxDefaultSize, m_statcharcommand2Choices, 0 );
	m_statcharcommand2->SetSelection( 0 );
	fgSizer42->Add( m_statcharcommand2, 0, wxALL, 3 );
	
	m_staticText1991 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Trance Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1991->Wrap( -1 );
	fgSizer42->Add( m_staticText1991, 0, wxALL, 1 );
	
	m_staticText2001 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2001->Wrap( -1 );
	fgSizer42->Add( m_staticText2001, 0, wxALL, 1 );
	
	wxArrayString m_statcharcommandtrance1Choices;
	m_statcharcommandtrance1 = new wxChoice( m_statcharscrolledwindow, wxID_TRANCE_COMMAND1, wxDefaultPosition, wxDefaultSize, m_statcharcommandtrance1Choices, 0 );
	m_statcharcommandtrance1->SetSelection( 0 );
	fgSizer42->Add( m_statcharcommandtrance1, 0, wxALL, 3 );
	
	wxArrayString m_statcharcommandtrance2Choices;
	m_statcharcommandtrance2 = new wxChoice( m_statcharscrolledwindow, wxID_TRANCE_COMMAND2, wxDefaultPosition, wxDefaultSize, m_statcharcommandtrance2Choices, 0 );
	m_statcharcommandtrance2->SetSelection( 0 );
	fgSizer42->Add( m_statcharcommandtrance2, 0, wxALL, 3 );
	
	
	fgSizer43->Add( fgSizer42, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer114;
	bSizer114 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText201 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Trance Link"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText201->Wrap( -1 );
	bSizer114->Add( m_staticText201, 0, wxALL, 5 );
	
	wxString m_statchartranceattackChoices[] = { _("No Trance"), _("None"), _("1st Command"), _("2nd Command") };
	int m_statchartranceattackNChoices = sizeof( m_statchartranceattackChoices ) / sizeof( wxString );
	m_statchartranceattack = new wxChoice( m_statcharscrolledwindow, wxID_TRANCE_ATTACK, wxDefaultPosition, wxDefaultSize, m_statchartranceattackNChoices, m_statchartranceattackChoices, 0 );
	m_statchartranceattack->SetSelection( 0 );
	m_statchartranceattack->SetToolTip( _("Commands in the specified slot (normal and trance)\nare linked together in term of ability availability.\nWarning! If the linked normal command contains spells,\nmake sure that they are learnable in the Ability Set.\nAlso, if there are more trance spells than normal ones,\nthe excess should be in the Ability Set as well.") );
	
	bSizer114->Add( m_statchartranceattack, 0, wxALL, 3 );
	
	
	fgSizer43->Add( bSizer114, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( fgSizer43, 1, wxEXPAND, 5 );
	
	m_staticText202 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Equipment Sets"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText202->Wrap( -1 );
	fgSizer23->Add( m_staticText202, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer115;
	bSizer115 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_statcharequipsetChoices;
	m_statcharequipset = new wxChoice( m_statcharscrolledwindow, wxID_EQUIPSET, wxDefaultPosition, wxDefaultSize, m_statcharequipsetChoices, 0 );
	m_statcharequipset->SetSelection( 0 );
	bSizer115->Add( m_statcharequipset, 0, wxALL, 3 );
	
	wxFlexGridSizer* fgSizer44;
	fgSizer44 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer44->SetFlexibleDirection( wxBOTH );
	fgSizer44->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText203 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Weapon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText203->Wrap( -1 );
	fgSizer44->Add( m_staticText203, 0, wxALL, 1 );
	
	m_staticText204 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText204->Wrap( -1 );
	fgSizer44->Add( m_staticText204, 0, 0, 5 );
	
	wxArrayString m_statcharweaponChoices;
	m_statcharweapon = new wxChoice( m_statcharscrolledwindow, wxID_WEAPON, wxDefaultPosition, wxDefaultSize, m_statcharweaponChoices, 0 );
	m_statcharweapon->SetSelection( 0 );
	fgSizer44->Add( m_statcharweapon, 0, wxALL, 3 );
	
	m_staticText209 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText209->Wrap( -1 );
	fgSizer44->Add( m_staticText209, 0, 0, 5 );
	
	m_staticText205 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Head"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText205->Wrap( -1 );
	fgSizer44->Add( m_staticText205, 0, wxALL, 1 );
	
	m_staticText206 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Wrist"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText206->Wrap( -1 );
	fgSizer44->Add( m_staticText206, 0, wxALL, 1 );
	
	wxArrayString m_statcharheadChoices;
	m_statcharhead = new wxChoice( m_statcharscrolledwindow, wxID_HEAD, wxDefaultPosition, wxDefaultSize, m_statcharheadChoices, 0 );
	m_statcharhead->SetSelection( 0 );
	fgSizer44->Add( m_statcharhead, 0, wxALL, 3 );
	
	wxArrayString m_statcharwristChoices;
	m_statcharwrist = new wxChoice( m_statcharscrolledwindow, wxID_WRIST, wxDefaultPosition, wxDefaultSize, m_statcharwristChoices, 0 );
	m_statcharwrist->SetSelection( 0 );
	fgSizer44->Add( m_statcharwrist, 0, wxALL, 3 );
	
	m_staticText207 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Armor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText207->Wrap( -1 );
	fgSizer44->Add( m_staticText207, 0, wxALL, 1 );
	
	m_staticText208 = new wxStaticText( m_statcharscrolledwindow, wxID_ANY, _("Accessory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText208->Wrap( -1 );
	fgSizer44->Add( m_staticText208, 0, wxALL, 1 );
	
	wxArrayString m_statchararmorChoices;
	m_statchararmor = new wxChoice( m_statcharscrolledwindow, wxID_ARMOR, wxDefaultPosition, wxDefaultSize, m_statchararmorChoices, 0 );
	m_statchararmor->SetSelection( 0 );
	fgSizer44->Add( m_statchararmor, 0, wxALL, 3 );
	
	wxArrayString m_statcharaccessoryChoices;
	m_statcharaccessory = new wxChoice( m_statcharscrolledwindow, wxID_ACCESSORY, wxDefaultPosition, wxDefaultSize, m_statcharaccessoryChoices, 0 );
	m_statcharaccessory->SetSelection( 0 );
	fgSizer44->Add( m_statcharaccessory, 0, wxALL, 3 );
	
	
	bSizer115->Add( fgSizer44, 1, wxEXPAND, 5 );
	
	
	fgSizer23->Add( bSizer115, 1, wxEXPAND, 5 );
	
	
	m_statcharscrolledwindow->SetSizer( fgSizer23 );
	m_statcharscrolledwindow->Layout();
	fgSizer23->Fit( m_statcharscrolledwindow );
	bSizer64->Add( m_statcharscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	m_statlvlscrolledwindow = new wxScrolledWindow( m_panel25, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_statlvlscrolledwindow->SetScrollRate( 5, 5 );
	m_statlvlscrolledwindow->Hide();
	
	wxFlexGridSizer* fgSizer231;
	fgSizer231 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer231->SetFlexibleDirection( wxBOTH );
	fgSizer231->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText11312 = new wxStaticText( m_statlvlscrolledwindow, wxID_ANY, _("Recquired Exp"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11312->Wrap( -1 );
	fgSizer231->Add( m_staticText11312, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer76;
	bSizer76 = new wxBoxSizer( wxVERTICAL );
	
	m_statlvlexpmanage = new wxButton( m_statlvlscrolledwindow, wxID_EXP, _("Curve Editing"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer76->Add( m_statlvlexpmanage, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer96;
	bSizer96 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statlvlexplist = new wxListBox( m_statlvlscrolledwindow, wxID_EXP, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_statlvlexplist->SetToolTip( _("Total experience required to reach the level") );
	
	bSizer96->Add( m_statlvlexplist, 0, wxALL, 5 );
	
	m_statlvlexpspin = new wxSpinCtrl( m_statlvlscrolledwindow, wxID_EXP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 2147483647, 0 );
	bSizer96->Add( m_statlvlexpspin, 0, wxALL, 5 );
	
	
	bSizer76->Add( bSizer96, 1, wxEXPAND, 5 );
	
	
	fgSizer231->Add( bSizer76, 1, wxEXPAND, 5 );
	
	m_staticText113121 = new wxStaticText( m_statlvlscrolledwindow, wxID_ANY, _("Base HP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113121->Wrap( -1 );
	fgSizer231->Add( m_staticText113121, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer761;
	bSizer761 = new wxBoxSizer( wxVERTICAL );
	
	m_statlvlhpmanage = new wxButton( m_statlvlscrolledwindow, wxID_HP, _("Curve Editing"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer761->Add( m_statlvlhpmanage, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer98;
	bSizer98 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statlvlhplist = new wxListBox( m_statlvlscrolledwindow, wxID_HP, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_statlvlhplist->SetToolTip( _("Max HP are calculated using the formula :\nMaxHP = BaseHP * Strength / 50\nStrength doesn't take equipement boosts in account.") );
	
	bSizer98->Add( m_statlvlhplist, 0, wxALL, 5 );
	
	m_statlvlhpspin = new wxSpinCtrl( m_statlvlscrolledwindow, wxID_HP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer98->Add( m_statlvlhpspin, 0, wxALL, 5 );
	
	
	bSizer761->Add( bSizer98, 1, wxEXPAND, 5 );
	
	
	fgSizer231->Add( bSizer761, 1, wxEXPAND, 5 );
	
	m_staticText113122 = new wxStaticText( m_statlvlscrolledwindow, wxID_ANY, _("Base MP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText113122->Wrap( -1 );
	fgSizer231->Add( m_staticText113122, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer762;
	bSizer762 = new wxBoxSizer( wxVERTICAL );
	
	m_statlvlmpmanage = new wxButton( m_statlvlscrolledwindow, wxID_MP, _("Curve Editing"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer762->Add( m_statlvlmpmanage, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer99;
	bSizer99 = new wxBoxSizer( wxHORIZONTAL );
	
	m_statlvlmplist = new wxListBox( m_statlvlscrolledwindow, wxID_MP, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_statlvlmplist->SetToolTip( _("Max MP are calculated using the formula :\nMaxMP = BaseMP * Magic / 100\nMagic doesn't take equipement boosts in account.") );
	
	bSizer99->Add( m_statlvlmplist, 0, wxALL, 5 );
	
	m_statlvlmpspin = new wxSpinCtrl( m_statlvlscrolledwindow, wxID_MP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer99->Add( m_statlvlmpspin, 0, wxALL, 5 );
	
	
	bSizer762->Add( bSizer99, 1, wxEXPAND, 5 );
	
	
	fgSizer231->Add( bSizer762, 1, wxEXPAND, 5 );
	
	
	m_statlvlscrolledwindow->SetSizer( fgSizer231 );
	m_statlvlscrolledwindow->Layout();
	fgSizer231->Fit( m_statlvlscrolledwindow );
	bSizer64->Add( m_statlvlscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel25->SetSizer( bSizer64 );
	m_panel25->Layout();
	bSizer64->Fit( m_panel25 );
	m_notebookparty->AddPage( m_panel25, _("Stats"), false );
	m_panel68 = new wxPanel( m_notebookparty, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxHORIZONTAL );
	
	m_partyspeciallist = new wxListBox( m_panel68, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer161->Add( m_partyspeciallist, 0, wxALL|wxEXPAND, 5 );
	
	m_partyspecialavailabilityscrolledwindow = new wxScrolledWindow( m_panel68, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_partyspecialavailabilityscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer74;
	fgSizer74 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer74->SetFlexibleDirection( wxBOTH );
	fgSizer74->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText473 = new wxStaticText( m_partyspecialavailabilityscrolledwindow, wxID_ANY, _("Magic Sword"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText473->Wrap( -1 );
	fgSizer74->Add( m_staticText473, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer75;
	fgSizer75 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer75->SetFlexibleDirection( wxBOTH );
	fgSizer75->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_partyspecialmagicswordlist = new wxListBox( m_partyspecialavailabilityscrolledwindow, wxID_MAGICSWD, wxDefaultPosition, wxSize( 180,120 ), 0, NULL, 0 ); 
	m_partyspecialmagicswordlist->SetToolTip( _("When Vivi learns the ability on the right,\nSteiner unlocks the ability on the left") );
	
	fgSizer75->Add( m_partyspecialmagicswordlist, 0, wxALL, 5 );
	
	wxArrayString m_partyspecialmagicswordspellChoices;
	m_partyspecialmagicswordspell = new wxChoice( m_partyspecialavailabilityscrolledwindow, wxID_MAGICSWD, wxDefaultPosition, wxDefaultSize, m_partyspecialmagicswordspellChoices, 0 );
	m_partyspecialmagicswordspell->SetSelection( 0 );
	m_partyspecialmagicswordspell->SetToolTip( _("When Vivi learns the ability on the right,\nSteiner unlocks the ability on the left") );
	
	fgSizer75->Add( m_partyspecialmagicswordspell, 0, wxALL, 5 );
	
	
	fgSizer74->Add( fgSizer75, 1, wxEXPAND, 5 );
	
	
	m_partyspecialavailabilityscrolledwindow->SetSizer( fgSizer74 );
	m_partyspecialavailabilityscrolledwindow->Layout();
	fgSizer74->Fit( m_partyspecialavailabilityscrolledwindow );
	bSizer161->Add( m_partyspecialavailabilityscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel68->SetSizer( bSizer161 );
	m_panel68->Layout();
	bSizer161->Fit( m_panel68 );
	m_notebookparty->AddPage( m_panel68, _("Special"), false );
	
	bSizer14->Add( m_notebookparty, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelparty->SetSizer( bSizer14 );
	m_panelparty->Layout();
	bSizer14->Fit( m_panelparty );
	m_notebookmain->AddPage( m_panelparty, _("Party"), false );
	m_panelinventory = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer105;
	bSizer105 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookinventory = new wxNotebook( m_panelinventory, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel26 = new wxPanel( m_notebookinventory, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer88;
	bSizer88 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookitem = new wxNotebook( m_panel26, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel27 = new wxPanel( m_notebookitem, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemlist = new wxListBox( m_panel27, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer111->Add( m_itemlist, 0, wxALL|wxEXPAND, 5 );
	
	m_itemscrolledwindow = new wxScrolledWindow( m_panel27, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_itemscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer511;
	fgSizer511 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer511->SetFlexibleDirection( wxBOTH );
	fgSizer511->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText155 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText155->Wrap( -1 );
	fgSizer511->Add( m_staticText155, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer2212;
	bSizer2212 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemname = new wxTextCtrl( m_itemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer2212->Add( m_itemname, 0, wxALL, 2 );
	
	m_itemnamebutton = new wxButton( m_itemscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_itemnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer2212->Add( m_itemnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer511->Add( bSizer2212, 1, wxEXPAND, 5 );
	
	m_staticText1551 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Menu Help"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1551->Wrap( -1 );
	fgSizer511->Add( m_staticText1551, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer2311;
	bSizer2311 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemhelp = new wxTextCtrl( m_itemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_itemhelp->SetToolTip( _("The ability's description shown with [Select] inside menus") );
	
	bSizer2311->Add( m_itemhelp, 1, wxALL|wxEXPAND, 2 );
	
	m_itemhelpbutton = new wxButton( m_itemscrolledwindow, wxID_HELP, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_itemhelpbutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer2311->Add( m_itemhelpbutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer511->Add( bSizer2311, 1, wxEXPAND, 5 );
	
	m_staticText15511 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Battle Help"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15511->Wrap( -1 );
	fgSizer511->Add( m_staticText15511, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer2312;
	bSizer2312 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemhelp2 = new wxTextCtrl( m_itemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_itemhelp2->SetToolTip( _("The ability's description shown with [Select] inside battles") );
	
	bSizer2312->Add( m_itemhelp2, 1, wxALL|wxEXPAND, 2 );
	
	m_itemhelp2button = new wxButton( m_itemscrolledwindow, wxID_BATTLEHELP, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_itemhelp2button->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer2312->Add( m_itemhelp2button, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer511->Add( bSizer2312, 1, wxEXPAND, 5 );
	
	m_staticText15521 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15521->Wrap( -1 );
	fgSizer511->Add( m_staticText15521, 0, wxALL, 5 );
	
	wxGridSizer* m_itemtype;
	m_itemtype = new wxGridSizer( 2, 4, 0, 0 );
	
	m_itemtype1 = new wxCheckBox( m_itemscrolledwindow, wxID_IT1, _("Consumable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype1->SetToolTip( _("Can be used out of battles") );
	
	m_itemtype->Add( m_itemtype1, 0, wxALL, 5 );
	
	m_itemtype2 = new wxCheckBox( m_itemscrolledwindow, wxID_IT2, _("Gil"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype2->SetToolTip( _("Unknown flag") );
	
	m_itemtype->Add( m_itemtype2, 0, wxALL, 5 );
	
	m_itemtype3 = new wxCheckBox( m_itemscrolledwindow, wxID_IT3, _("Usable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype3->SetToolTip( _("Can be used in battles") );
	
	m_itemtype->Add( m_itemtype3, 0, wxALL, 5 );
	
	m_itemtype4 = new wxCheckBox( m_itemscrolledwindow, wxID_IT4, _("Accessory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype->Add( m_itemtype4, 0, wxALL, 5 );
	
	m_itemtype5 = new wxCheckBox( m_itemscrolledwindow, wxID_IT5, _("Armor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype->Add( m_itemtype5, 0, wxALL, 5 );
	
	m_itemtype6 = new wxCheckBox( m_itemscrolledwindow, wxID_IT6, _("Head"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype->Add( m_itemtype6, 0, wxALL, 5 );
	
	m_itemtype7 = new wxCheckBox( m_itemscrolledwindow, wxID_IT7, _("Wrist"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype->Add( m_itemtype7, 0, wxALL, 5 );
	
	m_itemtype8 = new wxCheckBox( m_itemscrolledwindow, wxID_IT8, _("Weapon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtype->Add( m_itemtype8, 0, wxALL, 5 );
	
	
	fgSizer511->Add( m_itemtype, 1, wxEXPAND, 5 );
	
	m_staticText1552 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Price"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1552->Wrap( -1 );
	fgSizer511->Add( m_staticText1552, 0, wxALL, 5 );
	
	m_itemprice = new wxSpinCtrl( m_itemscrolledwindow, wxID_PRICE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_itemprice->SetToolTip( _("Price at which merchants sell the item\nThey buy it half this price") );
	
	fgSizer511->Add( m_itemprice, 0, wxALL, 2 );
	
	m_staticText15522 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Usability"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15522->Wrap( -1 );
	fgSizer511->Add( m_staticText15522, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer30;
	fgSizer30 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer30->SetFlexibleDirection( wxBOTH );
	fgSizer30->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_itemcharavailability16 = new wxCheckBox( m_itemscrolledwindow, wxID_IA16, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_itemcharavailability16->Hide();
	
	fgSizer30->Add( m_itemcharavailability16, 0, wxALL, 5 );
	
	m_itemcharavailability15 = new wxCheckBox( m_itemscrolledwindow, wxID_IA15, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_itemcharavailability15->Hide();
	
	fgSizer30->Add( m_itemcharavailability15, 0, wxALL, 5 );
	
	m_itemcharavailability14 = new wxCheckBox( m_itemscrolledwindow, wxID_IA14, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_itemcharavailability14->Hide();
	
	fgSizer30->Add( m_itemcharavailability14, 0, wxALL, 5 );
	
	m_itemcharavailability13 = new wxCheckBox( m_itemscrolledwindow, wxID_IA13, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_itemcharavailability13->Hide();
	
	fgSizer30->Add( m_itemcharavailability13, 0, wxALL, 5 );
	
	m_itemcharavailability12 = new wxCheckBox( m_itemscrolledwindow, wxID_IA12, _("Zidane"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability12, 0, wxALL, 5 );
	
	m_itemcharavailability11 = new wxCheckBox( m_itemscrolledwindow, wxID_IA11, _("Vivi"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability11, 0, wxALL, 5 );
	
	m_itemcharavailability10 = new wxCheckBox( m_itemscrolledwindow, wxID_IA10, _("Dagger"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability10, 0, wxALL, 5 );
	
	m_itemcharavailability9 = new wxCheckBox( m_itemscrolledwindow, wxID_IA9, _("Steiner"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability9, 0, wxALL, 5 );
	
	m_itemcharavailability8 = new wxCheckBox( m_itemscrolledwindow, wxID_IA8, _("Freya"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability8, 0, wxALL, 5 );
	
	m_itemcharavailability7 = new wxCheckBox( m_itemscrolledwindow, wxID_IA7, _("Quina"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability7, 0, wxALL, 5 );
	
	m_itemcharavailability6 = new wxCheckBox( m_itemscrolledwindow, wxID_IA6, _("Eiko"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability6, 0, wxALL, 5 );
	
	m_itemcharavailability5 = new wxCheckBox( m_itemscrolledwindow, wxID_IA5, _("Amarant"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability5, 0, wxALL, 5 );
	
	m_itemcharavailability4 = new wxCheckBox( m_itemscrolledwindow, wxID_IA4, _("Cinna"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability4, 0, wxALL, 5 );
	
	m_itemcharavailability3 = new wxCheckBox( m_itemscrolledwindow, wxID_IA3, _("Marcus"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability3, 0, wxALL, 5 );
	
	m_itemcharavailability2 = new wxCheckBox( m_itemscrolledwindow, wxID_IA2, _("Blank"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability2, 0, wxALL, 5 );
	
	m_itemcharavailability1 = new wxCheckBox( m_itemscrolledwindow, wxID_IA1, _("Beatrix"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer30->Add( m_itemcharavailability1, 0, wxALL, 5 );
	
	
	fgSizer511->Add( fgSizer30, 1, wxEXPAND, 5 );
	
	m_staticText15523 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Primary Stat Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15523->Wrap( -1 );
	fgSizer511->Add( m_staticText15523, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer87;
	bSizer87 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_itemstatidChoices;
	m_itemstatid = new wxChoice( m_itemscrolledwindow, wxID_STAT, wxDefaultPosition, wxDefaultSize, m_itemstatidChoices, 0 );
	m_itemstatid->SetSelection( 0 );
	bSizer87->Add( m_itemstatid, 0, wxALL, 2 );
	
	m_itemstatidbutton = new wxButton( m_itemscrolledwindow, wxID_STAT, _("Edit Stat Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemstatidbutton->SetToolTip( _("Warning\nChanging a stat set will modify it\nfor the every items using the same set") );
	
	bSizer87->Add( m_itemstatidbutton, 0, wxALL, 2 );
	
	m_itemstatinfo = new wxStaticText( m_itemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_itemstatinfo->Wrap( -1 );
	bSizer87->Add( m_itemstatinfo, 0, wxALL|wxEXPAND, 2 );
	
	
	fgSizer511->Add( bSizer87, 1, wxEXPAND, 5 );
	
	m_staticText15524 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Abilities"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15524->Wrap( -1 );
	fgSizer511->Add( m_staticText15524, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer116;
	bSizer116 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_itemskill1Choices;
	m_itemskill1 = new wxChoice( m_itemscrolledwindow, wxID_SKILL1, wxDefaultPosition, wxDefaultSize, m_itemskill1Choices, 0 );
	m_itemskill1->SetSelection( 0 );
	bSizer116->Add( m_itemskill1, 0, wxALL, 2 );
	
	wxArrayString m_itemskill2Choices;
	m_itemskill2 = new wxChoice( m_itemscrolledwindow, wxID_SKILL2, wxDefaultPosition, wxDefaultSize, m_itemskill2Choices, 0 );
	m_itemskill2->SetSelection( 0 );
	bSizer116->Add( m_itemskill2, 0, wxALL, 2 );
	
	wxArrayString m_itemskill3Choices;
	m_itemskill3 = new wxChoice( m_itemscrolledwindow, wxID_SKILL3, wxDefaultPosition, wxDefaultSize, m_itemskill3Choices, 0 );
	m_itemskill3->SetSelection( 0 );
	bSizer116->Add( m_itemskill3, 0, wxALL, 2 );
	
	
	fgSizer511->Add( bSizer116, 1, wxEXPAND, 5 );
	
	m_staticText15525 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Menu Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15525->Wrap( -1 );
	fgSizer511->Add( m_staticText15525, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer117;
	bSizer117 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemmenuposlist = new wxListBox( m_itemscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_itemmenuposlist->SetToolTip( _("Position inside the item menu") );
	
	bSizer117->Add( m_itemmenuposlist, 0, wxALL, 2 );
	
	wxBoxSizer* bSizer89;
	bSizer89 = new wxBoxSizer( wxVERTICAL );
	
	m_itemmenuposbuttonup = new wxBitmapButton( m_itemscrolledwindow, wxID_MENUPOSUP, wxICON( moveup_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_itemmenuposbuttonup->SetBitmapDisabled( wxICON( moveup_disicon ) );
	bSizer89->Add( m_itemmenuposbuttonup, 0, wxALL, 5 );
	
	m_itemmenuposbuttondown = new wxBitmapButton( m_itemscrolledwindow, wxID_MENUPOSDOWN, wxICON( movedown_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_itemmenuposbuttondown->SetBitmapDisabled( wxICON( movedown_disicon ) );
	bSizer89->Add( m_itemmenuposbuttondown, 0, wxALL, 5 );
	
	
	bSizer117->Add( bSizer89, 1, wxEXPAND, 5 );
	
	
	fgSizer511->Add( bSizer117, 1, 0, 5 );
	
	m_staticText15526 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Equip Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15526->Wrap( -1 );
	fgSizer511->Add( m_staticText15526, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer1171;
	bSizer1171 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemequipposlist = new wxListBox( m_itemscrolledwindow, wxID_ANY, wxDefaultPosition, wxSize( 300,-1 ), 0, NULL, wxLB_HSCROLL ); 
	m_itemequipposlist->SetToolTip( _("Position inside the equip menu\nAffects the \"Equip best stuff\" command") );
	m_itemequipposlist->SetMaxSize( wxSize( 300,-1 ) );
	
	bSizer1171->Add( m_itemequipposlist, 0, wxALL, 2 );
	
	wxBoxSizer* bSizer891;
	bSizer891 = new wxBoxSizer( wxVERTICAL );
	
	m_itemequipposbuttonup = new wxBitmapButton( m_itemscrolledwindow, wxID_EQUIPPOSUP, wxICON( moveup_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_itemequipposbuttonup->SetBitmapDisabled( wxICON( moveup_disicon ) );
	bSizer891->Add( m_itemequipposbuttonup, 0, wxALL, 5 );
	
	m_itemequipposbuttondown = new wxBitmapButton( m_itemscrolledwindow, wxID_EQUIPPOSDOWN, wxICON( movedown_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_itemequipposbuttondown->SetBitmapDisabled( wxICON( movedown_disicon ) );
	bSizer891->Add( m_itemequipposbuttondown, 0, wxALL, 5 );
	
	
	bSizer1171->Add( bSizer891, 1, wxEXPAND, 5 );
	
	
	fgSizer511->Add( bSizer1171, 1, 0, 5 );
	
	m_staticText15527 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15527->Wrap( -1 );
	fgSizer511->Add( m_staticText15527, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer122;
	bSizer122 = new wxBoxSizer( wxHORIZONTAL );
	
	m_itemicon = new wxSpinCtrl( m_itemscrolledwindow, wxID_ICON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 35, 0 );
	bSizer122->Add( m_itemicon, 0, wxALL, 2 );
	
	m_itemiconpreview = new wxStaticBitmap( m_itemscrolledwindow, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer122->Add( m_itemiconpreview, 0, wxALL, 5 );
	
	
	fgSizer511->Add( bSizer122, 1, wxEXPAND, 5 );
	
	m_staticText15528 = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Icon Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15528->Wrap( -1 );
	fgSizer511->Add( m_staticText15528, 0, wxALL, 5 );
	
	m_itemiconcolor = new wxSpinCtrl( m_itemscrolledwindow, wxID_ICONCOLOR, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 15, 0 );
	fgSizer511->Add( m_itemiconcolor, 0, wxALL, 2 );
	
	m_itemusablelabel = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Usable :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemusablelabel->Wrap( -1 );
	fgSizer511->Add( m_itemusablelabel, 0, wxALL, 5 );
	
	m_itemusablepanel = new wxPanel( m_itemscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText192 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Effect"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText192->Wrap( -1 );
	fgSizer33->Add( m_staticText192, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_itemusableeffectChoices;
	m_itemusableeffect = new wxChoice( m_itemusablepanel, wxID_EFFECT, wxDefaultPosition, wxSize( -1,-1 ), m_itemusableeffectChoices, 0 );
	m_itemusableeffect->SetSelection( 0 );
	bSizer62->Add( m_itemusableeffect, 0, wxALL, 2 );
	
	m_itemusableeffecthelpwindow = new wxScrolledWindow( m_itemusablepanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_itemusableeffecthelpwindow->SetScrollRate( 5, 5 );
	m_itemusableeffecthelpwindow->SetMinSize( wxSize( -1,60 ) );
	
	wxBoxSizer* bSizer712;
	bSizer712 = new wxBoxSizer( wxVERTICAL );
	
	m_itemusableeffecthelp = new wxStaticText( m_itemusableeffecthelpwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_itemusableeffecthelp->Wrap( -1 );
	bSizer712->Add( m_itemusableeffecthelp, 0, wxALL, 2 );
	
	
	m_itemusableeffecthelpwindow->SetSizer( bSizer712 );
	m_itemusableeffecthelpwindow->Layout();
	bSizer712->Fit( m_itemusableeffecthelpwindow );
	bSizer62->Add( m_itemusableeffecthelpwindow, 1, wxEXPAND, 5 );
	
	
	fgSizer33->Add( bSizer62, 1, wxEXPAND, 5 );
	
	m_staticText193 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Power"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText193->Wrap( -1 );
	fgSizer33->Add( m_staticText193, 0, wxALL, 5 );
	
	m_itemusablepower = new wxSpinCtrl( m_itemusablepanel, wxID_POWER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer33->Add( m_itemusablepower, 0, wxALL, 2 );
	
	m_staticText194 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText194->Wrap( -1 );
	fgSizer33->Add( m_staticText194, 0, wxALL, 5 );
	
	wxGridSizer* gSizer1911;
	gSizer1911 = new wxGridSizer( 8, 4, 0, 0 );
	
	m_itemusablestatus1 = new wxCheckBox( m_itemusablepanel, wxID_IS1, _("Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus1, 0, wxALL, 5 );
	
	m_itemusablestatus2 = new wxCheckBox( m_itemusablepanel, wxID_IS2, _("Venom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus2, 0, wxALL, 5 );
	
	m_itemusablestatus3 = new wxCheckBox( m_itemusablepanel, wxID_IS3, _("Virus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus3, 0, wxALL, 5 );
	
	m_itemusablestatus4 = new wxCheckBox( m_itemusablepanel, wxID_IS4, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus4, 0, wxALL, 5 );
	
	m_itemusablestatus5 = new wxCheckBox( m_itemusablepanel, wxID_IS5, _("Blind"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus5, 0, wxALL, 5 );
	
	m_itemusablestatus6 = new wxCheckBox( m_itemusablepanel, wxID_IS6, _("Trouble"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus6, 0, wxALL, 5 );
	
	m_itemusablestatus7 = new wxCheckBox( m_itemusablepanel, wxID_IS7, _("Zombie"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus7, 0, wxALL, 5 );
	
	m_itemusablestatus8 = new wxCheckBox( m_itemusablepanel, wxID_IS8, _("Easy Kill"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus8, 0, wxALL, 5 );
	
	m_itemusablestatus9 = new wxCheckBox( m_itemusablepanel, wxID_IS9, _("Death"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus9, 0, wxALL, 5 );
	
	m_itemusablestatus10 = new wxCheckBox( m_itemusablepanel, wxID_IS10, _("Low HP"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus10, 0, wxALL, 5 );
	
	m_itemusablestatus11 = new wxCheckBox( m_itemusablepanel, wxID_IS11, _("Confuse"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus11, 0, wxALL, 5 );
	
	m_itemusablestatus12 = new wxCheckBox( m_itemusablepanel, wxID_IS12, _("Berserk"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus12, 0, wxALL, 5 );
	
	m_itemusablestatus13 = new wxCheckBox( m_itemusablepanel, wxID_IS13, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus13, 0, wxALL, 5 );
	
	m_itemusablestatus14 = new wxCheckBox( m_itemusablepanel, wxID_IS14, _("Auto-Life"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus14, 0, wxALL, 5 );
	
	m_itemusablestatus15 = new wxCheckBox( m_itemusablepanel, wxID_IS15, _("Trance"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus15, 0, wxALL, 5 );
	
	m_itemusablestatus16 = new wxCheckBox( m_itemusablepanel, wxID_IS16, _("Defend"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus16, 0, wxALL, 5 );
	
	m_itemusablestatus17 = new wxCheckBox( m_itemusablepanel, wxID_IS17, _("Poison"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus17, 0, wxALL, 5 );
	
	m_itemusablestatus18 = new wxCheckBox( m_itemusablepanel, wxID_IS18, _("Sleep"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus18, 0, wxALL, 5 );
	
	m_itemusablestatus19 = new wxCheckBox( m_itemusablepanel, wxID_IS19, _("Regen"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus19, 0, wxALL, 5 );
	
	m_itemusablestatus20 = new wxCheckBox( m_itemusablepanel, wxID_IS20, _("Haste"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus20, 0, wxALL, 5 );
	
	m_itemusablestatus21 = new wxCheckBox( m_itemusablepanel, wxID_IS21, _("Slow"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus21, 0, wxALL, 5 );
	
	m_itemusablestatus22 = new wxCheckBox( m_itemusablepanel, wxID_IS22, _("Float"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus22, 0, wxALL, 5 );
	
	m_itemusablestatus23 = new wxCheckBox( m_itemusablepanel, wxID_IS23, _("Shell"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus23, 0, wxALL, 5 );
	
	m_itemusablestatus24 = new wxCheckBox( m_itemusablepanel, wxID_IS24, _("Protect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus24, 0, wxALL, 5 );
	
	m_itemusablestatus25 = new wxCheckBox( m_itemusablepanel, wxID_IS25, _("Heat"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus25, 0, wxALL, 5 );
	
	m_itemusablestatus26 = new wxCheckBox( m_itemusablepanel, wxID_IS26, _("Freeze"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus26, 0, wxALL, 5 );
	
	m_itemusablestatus27 = new wxCheckBox( m_itemusablepanel, wxID_IS27, _("Vanish"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus27, 0, wxALL, 5 );
	
	m_itemusablestatus28 = new wxCheckBox( m_itemusablepanel, wxID_IS28, _("Doom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus28, 0, wxALL, 5 );
	
	m_itemusablestatus29 = new wxCheckBox( m_itemusablepanel, wxID_IS29, _("Mini"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus29, 0, wxALL, 5 );
	
	m_itemusablestatus30 = new wxCheckBox( m_itemusablepanel, wxID_IS30, _("Reflect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus30, 0, wxALL, 5 );
	
	m_itemusablestatus31 = new wxCheckBox( m_itemusablepanel, wxID_IS31, _("Jump"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus31, 0, wxALL, 5 );
	
	m_itemusablestatus32 = new wxCheckBox( m_itemusablepanel, wxID_IS32, _("Gradual Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer1911->Add( m_itemusablestatus32, 0, wxALL, 5 );
	
	
	fgSizer33->Add( gSizer1911, 1, wxEXPAND, 5 );
	
	m_staticText195 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText195->Wrap( -1 );
	fgSizer33->Add( m_staticText195, 0, wxALL, 5 );
	
	wxGridSizer* gSizer32;
	gSizer32 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_itemusablemodel = new wxStaticText( m_itemusablepanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_itemusablemodel->Wrap( -1 );
	m_itemusablemodel->SetToolTip( _("For special effects, characters' animations, camera movements...\nAlso defines which and when characters are hit\nas well as when the spell ends (which can be never)") );
	
	gSizer32->Add( m_itemusablemodel, 0, wxALL|wxEXPAND, 5 );
	
	m_itemusablemodeledit = new wxButton( m_itemusablepanel, wxID_ANY, _("Change Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer32->Add( m_itemusablemodeledit, 0, wxALL, 2 );
	
	
	fgSizer33->Add( gSizer32, 1, wxEXPAND, 5 );
	
	m_staticText196 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Target Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText196->Wrap( -1 );
	fgSizer33->Add( m_staticText196, 0, wxALL, 5 );
	
	wxString m_itemusabletargettypeChoices[] = { _("Any"), _("Ally"), _("Enemy"), _("Everyone"), _("Self") };
	int m_itemusabletargettypeNChoices = sizeof( m_itemusabletargettypeChoices ) / sizeof( wxString );
	m_itemusabletargettype = new wxChoice( m_itemusablepanel, wxID_TARGETTYPE, wxDefaultPosition, wxDefaultSize, m_itemusabletargettypeNChoices, m_itemusabletargettypeChoices, 0 );
	m_itemusabletargettype->SetSelection( 0 );
	fgSizer33->Add( m_itemusabletargettype, 0, wxALL, 2 );
	
	m_staticText198 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Target Amount"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText198->Wrap( -1 );
	fgSizer33->Add( m_staticText198, 0, wxALL, 5 );
	
	wxString m_itemusabletargetamountChoices[] = { _("Single"), _("Multiple"), _("Single-Multi") };
	int m_itemusabletargetamountNChoices = sizeof( m_itemusabletargetamountChoices ) / sizeof( wxString );
	m_itemusabletargetamount = new wxChoice( m_itemusablepanel, wxID_TARGETAMOUNT, wxDefaultPosition, wxDefaultSize, m_itemusabletargetamountNChoices, m_itemusabletargetamountChoices, 0 );
	m_itemusabletargetamount->SetSelection( 0 );
	fgSizer33->Add( m_itemusabletargetamount, 0, wxALL, 2 );
	
	m_staticText197 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Target Priority"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText197->Wrap( -1 );
	fgSizer33->Add( m_staticText197, 0, wxALL, 5 );
	
	wxString m_itemusabletargetpriorityChoices[] = { _("Enemies"), _("Allies") };
	int m_itemusabletargetpriorityNChoices = sizeof( m_itemusabletargetpriorityChoices ) / sizeof( wxString );
	m_itemusabletargetpriority = new wxChoice( m_itemusablepanel, wxID_TARGETPRIORITY, wxDefaultPosition, wxDefaultSize, m_itemusabletargetpriorityNChoices, m_itemusabletargetpriorityChoices, 0 );
	m_itemusabletargetpriority->SetSelection( 0 );
	m_itemusabletargetpriority->SetToolTip( _("Which kind of target is selected first\nwhen the target is asked") );
	
	fgSizer33->Add( m_itemusabletargetpriority, 0, wxALL, 2 );
	
	m_staticText199 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("UI Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText199->Wrap( -1 );
	fgSizer33->Add( m_staticText199, 0, wxALL, 5 );
	
	wxGridSizer* gSizer211;
	gSizer211 = new wxGridSizer( 1, 3, 0, 0 );
	
	m_itemusabletargetflagdead = new wxCheckBox( m_itemusablepanel, wxID_CAN_TARGET_DEAD, _("Can target KO"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer211->Add( m_itemusabletargetflagdead, 0, wxALL, 5 );
	
	m_itemusabletargetflagdeadfirst = new wxCheckBox( m_itemusablepanel, wxID_TARGET_DEAD_FIRST, _("Target KO in priority"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer211->Add( m_itemusabletargetflagdeadfirst, 0, wxALL, 5 );
	
	m_itemusabletargetflagcamera = new wxCheckBox( m_itemusablepanel, wxID_TARGET_CAMERA, _("Alternate Camera"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemusabletargetflagcamera->SetToolTip( _("Always use the alternative camera\nin spell animations using such a camera") );
	
	gSizer211->Add( m_itemusabletargetflagcamera, 0, wxALL, 5 );
	
	
	fgSizer33->Add( gSizer211, 1, wxEXPAND, 5 );
	
	m_staticText200 = new wxStaticText( m_itemusablepanel, wxID_ANY, _("Panel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText200->Wrap( -1 );
	fgSizer33->Add( m_staticText200, 0, wxALL, 5 );
	
	wxString m_itemusabletargetpanelChoices[] = { _("Normal"), _("HP"), _("MP"), _("Bad statuses"), _("Good statuses") };
	int m_itemusabletargetpanelNChoices = sizeof( m_itemusabletargetpanelChoices ) / sizeof( wxString );
	m_itemusabletargetpanel = new wxChoice( m_itemusablepanel, wxID_TARGETPANEL, wxDefaultPosition, wxDefaultSize, m_itemusabletargetpanelNChoices, m_itemusabletargetpanelChoices, 0 );
	m_itemusabletargetpanel->SetSelection( 0 );
	m_itemusabletargetpanel->SetToolTip( _("The panel type displayed when the target is asked\nOnly display informations on the allies\nA panel is needed for using items out of battle") );
	
	fgSizer33->Add( m_itemusabletargetpanel, 0, wxALL, 2 );
	
	
	m_itemusablepanel->SetSizer( fgSizer33 );
	m_itemusablepanel->Layout();
	fgSizer33->Fit( m_itemusablepanel );
	fgSizer511->Add( m_itemusablepanel, 1, wxEXPAND | wxALL, 0 );
	
	m_itemweaponlabel = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Weapon :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponlabel->Wrap( -1 );
	fgSizer511->Add( m_itemweaponlabel, 0, wxALL, 5 );
	
	m_itemweaponpanel = new wxPanel( m_itemscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer29;
	fgSizer29 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer29->SetFlexibleDirection( wxBOTH );
	fgSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText156 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Formula"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText156->Wrap( -1 );
	fgSizer29->Add( m_staticText156, 0, wxALL, 5 );
	
	wxString m_itemweaponformulaChoices[] = { _("None"), _("Standard"), _("Spirit Bonus"), _("Speed Bonus"), _("Magic Bonus"), _("Random"), _("Blood Sword"), _("Level Bonus") };
	int m_itemweaponformulaNChoices = sizeof( m_itemweaponformulaChoices ) / sizeof( wxString );
	m_itemweaponformula = new wxChoice( m_itemweaponpanel, wxID_WEAPONFORMULA, wxDefaultPosition, wxDefaultSize, m_itemweaponformulaNChoices, m_itemweaponformulaChoices, 0 );
	m_itemweaponformula->SetSelection( 0 );
	m_itemweaponformula->SetToolTip( _("Defines the damage formula of regular attacks") );
	
	fgSizer29->Add( m_itemweaponformula, 0, wxALL, 2 );
	
	m_staticText157 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Power"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText157->Wrap( -1 );
	fgSizer29->Add( m_staticText157, 0, wxALL, 5 );
	
	m_itemweaponpower = new wxSpinCtrl( m_itemweaponpanel, wxID_WEAPONPOWER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer29->Add( m_itemweaponpower, 0, wxALL, 2 );
	
	m_staticText158 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Element"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText158->Wrap( -1 );
	fgSizer29->Add( m_staticText158, 0, wxALL, 5 );
	
	wxGridSizer* m_itemweaponelement;
	m_itemweaponelement = new wxGridSizer( 2, 4, 0, 0 );
	
	m_itemweaponelement1 = new wxCheckBox( m_itemweaponpanel, wxID_WE1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement1, 0, wxALL, 5 );
	
	m_itemweaponelement2 = new wxCheckBox( m_itemweaponpanel, wxID_WE2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement2, 0, wxALL, 5 );
	
	m_itemweaponelement3 = new wxCheckBox( m_itemweaponpanel, wxID_WE3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement3, 0, wxALL, 5 );
	
	m_itemweaponelement4 = new wxCheckBox( m_itemweaponpanel, wxID_WE4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement4, 0, wxALL, 5 );
	
	m_itemweaponelement5 = new wxCheckBox( m_itemweaponpanel, wxID_WE5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement5, 0, wxALL, 5 );
	
	m_itemweaponelement6 = new wxCheckBox( m_itemweaponpanel, wxID_WE6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement6, 0, wxALL, 5 );
	
	m_itemweaponelement7 = new wxCheckBox( m_itemweaponpanel, wxID_WE7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement7, 0, wxALL, 5 );
	
	m_itemweaponelement8 = new wxCheckBox( m_itemweaponpanel, wxID_WE8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponelement->Add( m_itemweaponelement8, 0, wxALL, 5 );
	
	
	fgSizer29->Add( m_itemweaponelement, 1, wxEXPAND, 5 );
	
	m_staticText159 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText159->Wrap( -1 );
	fgSizer29->Add( m_staticText159, 0, wxALL, 5 );
	
	wxArrayString m_itemweaponstatusChoices;
	m_itemweaponstatus = new wxChoice( m_itemweaponpanel, wxID_WEAPONSTATUS, wxDefaultPosition, wxDefaultSize, m_itemweaponstatusChoices, 0 );
	m_itemweaponstatus->SetSelection( 0 );
	fgSizer29->Add( m_itemweaponstatus, 0, wxALL, 2 );
	
	m_staticText2961 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Status Accuracy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2961->Wrap( -1 );
	fgSizer29->Add( m_staticText2961, 0, wxALL, 5 );
	
	m_itemweaponaccuracy = new wxSpinCtrl( m_itemweaponpanel, wxID_WEAPONACC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer29->Add( m_itemweaponaccuracy, 0, wxALL, 2 );
	
	m_staticText297 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText297->Wrap( -1 );
	fgSizer29->Add( m_staticText297, 0, wxALL, 5 );
	
	wxGridSizer* m_itemweaponflag;
	m_itemweaponflag = new wxGridSizer( 2, 4, 0, 0 );
	
	m_itemweaponflag1 = new wxCheckBox( m_itemweaponpanel, wxID_WF1, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag->Add( m_itemweaponflag1, 0, wxALL, 5 );
	
	m_itemweaponflag2 = new wxCheckBox( m_itemweaponpanel, wxID_WF2, _("Ignore Row"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag->Add( m_itemweaponflag2, 0, wxALL, 5 );
	
	m_itemweaponflag3 = new wxCheckBox( m_itemweaponpanel, wxID_WF3, _("Can Throw"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag->Add( m_itemweaponflag3, 0, wxALL, 5 );
	
	m_itemweaponflag4 = new wxCheckBox( m_itemweaponpanel, wxID_WF4, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag->Add( m_itemweaponflag4, 0, wxALL, 5 );
	
	m_itemweaponflag5 = new wxCheckBox( m_itemweaponpanel, wxID_WF5, _("Unused"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag5->Hide();
	
	m_itemweaponflag->Add( m_itemweaponflag5, 0, wxALL, 5 );
	
	m_itemweaponflag6 = new wxCheckBox( m_itemweaponpanel, wxID_WF6, _("Unused"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag6->Hide();
	
	m_itemweaponflag->Add( m_itemweaponflag6, 0, wxALL, 5 );
	
	m_itemweaponflag7 = new wxCheckBox( m_itemweaponpanel, wxID_WF7, _("Unused"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag7->Hide();
	
	m_itemweaponflag->Add( m_itemweaponflag7, 0, wxALL, 5 );
	
	m_itemweaponflag8 = new wxCheckBox( m_itemweaponpanel, wxID_WF8, _("Unused"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemweaponflag8->Hide();
	
	m_itemweaponflag->Add( m_itemweaponflag8, 0, wxALL, 5 );
	
	
	fgSizer29->Add( m_itemweaponflag, 1, wxEXPAND, 5 );
	
	m_staticText298 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText298->Wrap( -1 );
	fgSizer29->Add( m_staticText298, 0, wxALL, 5 );
	
	m_itemweaponunk1 = new wxSpinCtrl( m_itemweaponpanel, wxID_WEAPONUNK1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	fgSizer29->Add( m_itemweaponunk1, 0, wxALL, 2 );
	
	m_staticText299 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText299->Wrap( -1 );
	fgSizer29->Add( m_staticText299, 0, wxALL, 5 );
	
	m_itemweaponunk2 = new wxSpinCtrl( m_itemweaponpanel, wxID_WEAPONUNK2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	fgSizer29->Add( m_itemweaponunk2, 0, wxALL, 2 );
	
	m_staticText160 = new wxStaticText( m_itemweaponpanel, wxID_ANY, _("Model ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText160->Wrap( -1 );
	fgSizer29->Add( m_staticText160, 0, wxALL, 5 );
	
	m_itemweaponmodel = new wxSpinCtrl( m_itemweaponpanel, wxID_WEAPONMODEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_itemweaponmodel->SetToolTip( _("Use sparingly") );
	
	fgSizer29->Add( m_itemweaponmodel, 0, wxALL, 2 );
	
	
	m_itemweaponpanel->SetSizer( fgSizer29 );
	m_itemweaponpanel->Layout();
	fgSizer29->Fit( m_itemweaponpanel );
	fgSizer511->Add( m_itemweaponpanel, 1, wxEXPAND | wxALL, 0 );
	
	m_itemarmorlabel = new wxStaticText( m_itemscrolledwindow, wxID_ANY, _("Armor :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemarmorlabel->Wrap( -1 );
	fgSizer511->Add( m_itemarmorlabel, 0, wxALL, 5 );
	
	m_itemarmorpanel = new wxPanel( m_itemscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer291;
	fgSizer291 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer291->SetFlexibleDirection( wxBOTH );
	fgSizer291->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText161 = new wxStaticText( m_itemarmorpanel, wxID_ANY, _("Defence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	fgSizer291->Add( m_staticText161, 0, wxALL, 5 );
	
	m_itemarmordefence = new wxSpinCtrl( m_itemarmorpanel, wxID_DEFENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer291->Add( m_itemarmordefence, 0, wxALL, 2 );
	
	m_staticText162 = new wxStaticText( m_itemarmorpanel, wxID_ANY, _("Evade"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText162->Wrap( -1 );
	fgSizer291->Add( m_staticText162, 0, wxALL, 5 );
	
	m_itemarmorevade = new wxSpinCtrl( m_itemarmorpanel, wxID_EVADE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer291->Add( m_itemarmorevade, 0, wxALL, 2 );
	
	m_staticText163 = new wxStaticText( m_itemarmorpanel, wxID_ANY, _("Magic Defence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText163->Wrap( -1 );
	fgSizer291->Add( m_staticText163, 0, wxALL, 5 );
	
	m_itemarmormagicdefence = new wxSpinCtrl( m_itemarmorpanel, wxID_MDEFENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer291->Add( m_itemarmormagicdefence, 0, wxALL, 2 );
	
	m_staticText164 = new wxStaticText( m_itemarmorpanel, wxID_ANY, _("Magic Evade"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText164->Wrap( -1 );
	fgSizer291->Add( m_staticText164, 0, wxALL, 5 );
	
	m_itemarmormagicevade = new wxSpinCtrl( m_itemarmorpanel, wxID_MEVADE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer291->Add( m_itemarmormagicevade, 0, wxALL, 2 );
	
	
	m_itemarmorpanel->SetSizer( fgSizer291 );
	m_itemarmorpanel->Layout();
	fgSizer291->Fit( m_itemarmorpanel );
	fgSizer511->Add( m_itemarmorpanel, 1, wxEXPAND | wxALL, 0 );
	
	
	m_itemscrolledwindow->SetSizer( fgSizer511 );
	m_itemscrolledwindow->Layout();
	fgSizer511->Fit( m_itemscrolledwindow );
	bSizer111->Add( m_itemscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel27->SetSizer( bSizer111 );
	m_panel27->Layout();
	bSizer111->Fit( m_panel27 );
	m_notebookitem->AddPage( m_panel27, _("Regular Items"), false );
	m_panel28 = new wxPanel( m_notebookitem, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer112;
	bSizer112 = new wxBoxSizer( wxHORIZONTAL );
	
	m_keyitemlist = new wxListBox( m_panel28, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer112->Add( m_keyitemlist, 0, wxALL|wxEXPAND, 5 );
	
	m_keyitemscrolledwindow = new wxScrolledWindow( m_panel28, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_keyitemscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer512;
	fgSizer512 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer512->SetFlexibleDirection( wxBOTH );
	fgSizer512->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1553 = new wxStaticText( m_keyitemscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1553->Wrap( -1 );
	fgSizer512->Add( m_staticText1553, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer22121;
	bSizer22121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_keyitemname = new wxTextCtrl( m_keyitemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer22121->Add( m_keyitemname, 0, wxALL, 2 );
	
	m_keyitemnamebutton = new wxButton( m_keyitemscrolledwindow, wxID_KEYNAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_keyitemnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer22121->Add( m_keyitemnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer512->Add( bSizer22121, 1, wxEXPAND, 5 );
	
	m_staticText15512 = new wxStaticText( m_keyitemscrolledwindow, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15512->Wrap( -1 );
	fgSizer512->Add( m_staticText15512, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer23111;
	bSizer23111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_keyitemhelp = new wxTextCtrl( m_keyitemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_keyitemhelp->SetToolTip( _("The ability's description shown with [Select] inside menus") );
	m_keyitemhelp->SetMinSize( wxSize( 250,-1 ) );
	
	bSizer23111->Add( m_keyitemhelp, 1, wxALL|wxEXPAND, 2 );
	
	m_keyitemhelpbutton = new wxButton( m_keyitemscrolledwindow, wxID_KEYHELP, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_keyitemhelpbutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer23111->Add( m_keyitemhelpbutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer512->Add( bSizer23111, 1, wxEXPAND, 5 );
	
	m_staticText155111 = new wxStaticText( m_keyitemscrolledwindow, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText155111->Wrap( -1 );
	fgSizer512->Add( m_staticText155111, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer23121;
	bSizer23121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_keyitemdescription = new wxTextCtrl( m_keyitemscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_keyitemdescription->SetToolTip( _("The ability's description shown with [Select] inside battles") );
	m_keyitemdescription->SetMinSize( wxSize( 250,-1 ) );
	
	bSizer23121->Add( m_keyitemdescription, 1, wxALL|wxEXPAND, 2 );
	
	m_keyitemdescriptionbutton = new wxButton( m_keyitemscrolledwindow, wxID_KEYDESCRIPTION, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_keyitemdescriptionbutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer23121->Add( m_keyitemdescriptionbutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer512->Add( bSizer23121, 1, wxEXPAND, 5 );
	
	
	m_keyitemscrolledwindow->SetSizer( fgSizer512 );
	m_keyitemscrolledwindow->Layout();
	fgSizer512->Fit( m_keyitemscrolledwindow );
	bSizer112->Add( m_keyitemscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel28->SetSizer( bSizer112 );
	m_panel28->Layout();
	bSizer112->Fit( m_panel28 );
	m_notebookitem->AddPage( m_panel28, _("Key Items"), false );
	
	bSizer88->Add( m_notebookitem, 1, wxEXPAND | wxALL, 2 );
	
	
	m_panel26->SetSizer( bSizer88 );
	m_panel26->Layout();
	bSizer88->Fit( m_panel26 );
	m_notebookinventory->AddPage( m_panel26, _("Items"), false );
	m_panel9 = new wxPanel( m_notebookinventory, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	m_notebookshop = new wxNotebook( m_panel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel32 = new wxPanel( m_notebookshop, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer93;
	bSizer93 = new wxBoxSizer( wxHORIZONTAL );
	
	m_shoplist = new wxListBox( m_panel32, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer93->Add( m_shoplist, 0, wxALL|wxEXPAND, 5 );
	
	m_shopscrolledwindow = new wxScrolledWindow( m_panel32, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_shopscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer513;
	fgSizer513 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer513->SetFlexibleDirection( wxBOTH );
	fgSizer513->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_shopitemlistlabel = new wxStaticText( m_shopscrolledwindow, wxID_ANY, _("Items Sold"), wxDefaultPosition, wxDefaultSize, 0 );
	m_shopitemlistlabel->Wrap( -1 );
	fgSizer513->Add( m_shopitemlistlabel, 0, wxALL, 5 );
	
	m_shopitemlistpanel = new wxPanel( m_shopscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer312;
	fgSizer312 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer312->SetFlexibleDirection( wxBOTH );
	fgSizer312->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_shopitemadd = new wxButton( m_shopitemlistpanel, wxID_ADD, _("Add Item"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer312->Add( m_shopitemadd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_shopitemremove = new wxButton( m_shopitemlistpanel, wxID_REMOVE, _("Remove Item"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer312->Add( m_shopitemremove, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText2711 = new wxStaticText( m_shopitemlistpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2711->Wrap( -1 );
	fgSizer312->Add( m_staticText2711, 0, wxALL, 5 );
	
	m_shopitemfulllist = new wxListBox( m_shopitemlistpanel, wxID_ADD, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer312->Add( m_shopitemfulllist, 0, wxALL, 5 );
	
	m_shopitemlist = new wxListBox( m_shopitemlistpanel, wxID_REMOVE, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer312->Add( m_shopitemlist, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer89112;
	bSizer89112 = new wxBoxSizer( wxVERTICAL );
	
	m_shopmoveitemup = new wxBitmapButton( m_shopitemlistpanel, wxID_UP, wxICON( moveup_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_shopmoveitemup->SetBitmapDisabled( wxICON( moveup_disicon ) );
	bSizer89112->Add( m_shopmoveitemup, 0, wxALL, 5 );
	
	m_shopmoveitemdown = new wxBitmapButton( m_shopitemlistpanel, wxID_DOWN, wxICON( movedown_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_shopmoveitemdown->SetBitmapDisabled( wxICON( movedown_disicon ) );
	bSizer89112->Add( m_shopmoveitemdown, 0, wxALL, 5 );
	
	
	fgSizer312->Add( bSizer89112, 1, wxEXPAND, 5 );
	
	
	m_shopitemlistpanel->SetSizer( fgSizer312 );
	m_shopitemlistpanel->Layout();
	fgSizer312->Fit( m_shopitemlistpanel );
	fgSizer513->Add( m_shopitemlistpanel, 1, wxEXPAND | wxALL, 2 );
	
	
	m_shopscrolledwindow->SetSizer( fgSizer513 );
	m_shopscrolledwindow->Layout();
	fgSizer513->Fit( m_shopscrolledwindow );
	bSizer93->Add( m_shopscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel32->SetSizer( bSizer93 );
	m_panel32->Layout();
	bSizer93->Fit( m_panel32 );
	m_notebookshop->AddPage( m_panel32, _("Regular Shops"), false );
	m_panel34 = new wxPanel( m_notebookshop, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer95;
	bSizer95 = new wxBoxSizer( wxHORIZONTAL );
	
	m_synthshoplist = new wxListBox( m_panel34, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 ); 
	bSizer95->Add( m_synthshoplist, 0, wxALL|wxEXPAND, 5 );
	
	m_synthshopscrolledwindow = new wxScrolledWindow( m_panel34, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_synthshopscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer5141;
	fgSizer5141 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer5141->SetFlexibleDirection( wxBOTH );
	fgSizer5141->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText188 = new wxStaticText( m_synthshopscrolledwindow, wxID_ANY, _("Synthesized"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText188->Wrap( -1 );
	fgSizer5141->Add( m_staticText188, 0, wxALL, 5 );
	
	wxArrayString m_synthshopsynthitemChoices;
	m_synthshopsynthitem = new wxChoice( m_synthshopscrolledwindow, wxID_SYNTH, wxDefaultPosition, wxDefaultSize, m_synthshopsynthitemChoices, 0 );
	m_synthshopsynthitem->SetSelection( 0 );
	fgSizer5141->Add( m_synthshopsynthitem, 0, wxALL, 2 );
	
	m_staticText189 = new wxStaticText( m_synthshopscrolledwindow, wxID_ANY, _("Recipes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText189->Wrap( -1 );
	fgSizer5141->Add( m_staticText189, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer1021;
	bSizer1021 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_synthshoprecipe1Choices;
	m_synthshoprecipe1 = new wxChoice( m_synthshopscrolledwindow, wxID_RECIPE1, wxDefaultPosition, wxDefaultSize, m_synthshoprecipe1Choices, 0 );
	m_synthshoprecipe1->SetSelection( 0 );
	bSizer1021->Add( m_synthshoprecipe1, 0, wxALL, 2 );
	
	wxArrayString m_synthshoprecipe2Choices;
	m_synthshoprecipe2 = new wxChoice( m_synthshopscrolledwindow, wxID_RECIPE2, wxDefaultPosition, wxDefaultSize, m_synthshoprecipe2Choices, 0 );
	m_synthshoprecipe2->SetSelection( 0 );
	bSizer1021->Add( m_synthshoprecipe2, 0, wxALL, 2 );
	
	
	fgSizer5141->Add( bSizer1021, 1, wxEXPAND, 5 );
	
	m_staticText190 = new wxStaticText( m_synthshopscrolledwindow, wxID_ANY, _("Price"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText190->Wrap( -1 );
	fgSizer5141->Add( m_staticText190, 0, wxALL, 5 );
	
	m_synthshopprice = new wxSpinCtrl( m_synthshopscrolledwindow, wxID_PRICE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer5141->Add( m_synthshopprice, 0, wxALL, 2 );
	
	m_staticText191 = new wxStaticText( m_synthshopscrolledwindow, wxID_ANY, _("Synthesis Shops"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText191->Wrap( -1 );
	fgSizer5141->Add( m_staticText191, 0, wxALL, 5 );
	
	wxGridSizer* m_synthshopsynthshop;
	m_synthshopsynthshop = new wxGridSizer( 2, 4, 0, 0 );
	
	m_synthshopsynthshop1 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS1, _("Lindblum"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop1->SetToolTip( _("Disc 1 & 2") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop1, 0, wxALL, 5 );
	
	m_synthshopsynthshop2 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS2, _("Treno"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop2->SetToolTip( _("Disc 2") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop2, 0, wxALL, 5 );
	
	m_synthshopsynthshop3 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS3, _("Lindblum"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop3->SetToolTip( _("Disc 2") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop3, 0, wxALL, 5 );
	
	m_synthshopsynthshop4 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS4, _("Black Mage Village"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop4->SetToolTip( _("Disc 2 & 3") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop4, 0, wxALL, 5 );
	
	m_synthshopsynthshop5 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS5, _("Alex. Lind. Treno"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop5->SetToolTip( _("Disc 3 & 4") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop5, 0, wxALL, 5 );
	
	m_synthshopsynthshop6 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS6, _("Daguerreo"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop6->SetToolTip( _("Disc 3 & 4") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop6, 0, wxALL, 5 );
	
	m_synthshopsynthshop7 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS7, _("Black Mage Village"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop7->SetToolTip( _("Disc 4") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop7, 0, wxALL, 5 );
	
	m_synthshopsynthshop8 = new wxCheckBox( m_synthshopscrolledwindow, wxID_SS8, _("Hades"), wxDefaultPosition, wxDefaultSize, 0 );
	m_synthshopsynthshop8->SetToolTip( _("Disc 4") );
	
	m_synthshopsynthshop->Add( m_synthshopsynthshop8, 0, wxALL, 5 );
	
	
	fgSizer5141->Add( m_synthshopsynthshop, 1, wxEXPAND, 5 );
	
	
	m_synthshopscrolledwindow->SetSizer( fgSizer5141 );
	m_synthshopscrolledwindow->Layout();
	fgSizer5141->Fit( m_synthshopscrolledwindow );
	bSizer95->Add( m_synthshopscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel34->SetSizer( bSizer95 );
	m_panel34->Layout();
	bSizer95->Fit( m_panel34 );
	m_notebookshop->AddPage( m_panel34, _("Synthesis Shops"), false );
	
	bSizer11->Add( m_notebookshop, 1, wxEXPAND | wxALL, 2 );
	
	
	m_panel9->SetSizer( bSizer11 );
	m_panel9->Layout();
	bSizer11->Fit( m_panel9 );
	m_notebookinventory->AddPage( m_panel9, _("Shops"), false );
	
	bSizer105->Add( m_notebookinventory, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelinventory->SetSizer( bSizer105 );
	m_panelinventory->Layout();
	bSizer105->Fit( m_panelinventory );
	m_notebookmain->AddPage( m_panelinventory, _("Inventory"), false );
	m_panelbattle = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_enemylist = new wxListBox( m_panelbattle, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer10->Add( m_enemylist, 0, wxALL|wxEXPAND, 5 );
	
	m_enemyscrolledwindow = new wxScrolledWindow( m_panelbattle, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_enemyscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_enemystatlistlabel = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Enemy List"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatlistlabel->Wrap( -1 );
	fgSizer8->Add( m_enemystatlistlabel, 0, wxALL, 5 );
	
	m_enemystatlist = new wxListBox( m_enemyscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer8->Add( m_enemystatlist, 0, wxALL, 2 );
	
	m_enemyspelllistlabel = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Attacks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspelllistlabel->Wrap( -1 );
	fgSizer8->Add( m_enemyspelllistlabel, 0, wxALL, 5 );
	
	m_enemyspelllist = new wxListBox( m_enemyscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer8->Add( m_enemyspelllist, 0, wxALL, 2 );
	
	m_enemygrouplistlabel = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Enemy Groups"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygrouplistlabel->Wrap( -1 );
	fgSizer8->Add( m_enemygrouplistlabel, 0, wxALL, 5 );
	
	m_enemygrouplist = new wxListBox( m_enemyscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer8->Add( m_enemygrouplist, 0, wxALL, 2 );
	
	m_staticText86 = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Battle Texts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText86->Wrap( -1 );
	fgSizer8->Add( m_staticText86, 0, wxALL, 5 );
	
	m_enemytextlist = new wxListBox( m_enemyscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer8->Add( m_enemytextlist, 0, wxALL, 2 );
	
	m_staticText1912 = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Script"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1912->Wrap( -1 );
	fgSizer8->Add( m_staticText1912, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxHORIZONTAL );
	
	m_enemyscriptedit = new wxButton( m_enemyscrolledwindow, wxID_SCRIPT, _("Edit Script"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer191->Add( m_enemyscriptedit, 0, wxALL, 3 );
	
	m_enemyscriptentryedit = new wxButton( m_enemyscrolledwindow, wxID_ENTRY, _("Edit Entries"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer191->Add( m_enemyscriptentryedit, 0, wxALL, 3 );
	
	
	fgSizer8->Add( bSizer191, 1, wxEXPAND, 5 );
	
	m_staticText216 = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Battle Scene"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText216->Wrap( -1 );
	fgSizer8->Add( m_staticText216, 0, wxALL, 5 );
	
	wxArrayString m_enemysceneChoices;
	m_enemyscene = new wxChoice( m_enemyscrolledwindow, wxID_SCENE, wxDefaultPosition, wxDefaultSize, m_enemysceneChoices, 0 );
	m_enemyscene->SetSelection( 0 );
	fgSizer8->Add( m_enemyscene, 0, wxALL, 3 );
	
	m_staticText76 = new wxStaticText( m_enemyscrolledwindow, wxID_ANY, _("Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	fgSizer8->Add( m_staticText76, 0, wxALL, 5 );
	
	wxGridSizer* gSizer18;
	gSizer18 = new wxGridSizer( 8, 2, 0, 0 );
	
	m_enemyflag1 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF1, _("Scripted Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag1->SetToolTip( _("When enabled, the battle is put in a frozen state after the engaging camera movements") );
	
	gSizer18->Add( m_enemyflag1, 0, wxALL, 5 );
	
	m_enemyflag2 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF2, _("Always Back Attack"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag2, 0, wxALL, 5 );
	
	m_enemyflag3 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF3, _("No Game Over"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag3, 0, wxALL, 5 );
	
	m_enemyflag4 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF4, _("No Experience"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag4, 0, wxALL, 5 );
	
	m_enemyflag5 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF5, _("No Victory Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag5, 0, wxALL, 5 );
	
	m_enemyflag6 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF6, _("Can't Flee"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag6, 0, wxALL, 5 );
	
	m_enemyflag7 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF7, _("Out Of Reach"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag7->SetToolTip( _("Enemies can't be hitted by contact attacks") );
	
	gSizer18->Add( m_enemyflag7, 0, wxALL, 5 );
	
	m_enemyflag8 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF8, _("Oeilvert Curse"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag8->SetToolTip( _("Magic is disabled") );
	
	gSizer18->Add( m_enemyflag8, 0, wxALL, 5 );
	
	m_enemyflag9 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF9, _("Ipsen Curse"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag9->SetToolTip( _("Weapons' attack is transformed to :\nNew Attack = 60 - Attack") );
	
	gSizer18->Add( m_enemyflag9, 0, wxALL, 5 );
	
	m_enemyflag10 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF10, _("Fixed Camera 1"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag10, 0, wxALL, 5 );
	
	m_enemyflag11 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF11, _("Fixed Camera 2"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag11, 0, wxALL, 5 );
	
	m_enemyflag12 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF12, _("Disable Rewards"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag12->SetToolTip( _("Additionnal effect is that the script's main function\nis called again at the end of the battle\nThe script code TerminateBattle must be called at this occasion") );
	
	gSizer18->Add( m_enemyflag12, 0, wxALL, 5 );
	
	m_enemyflag13 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF13, _("Don't Stop Music"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag13->SetToolTip( _("Don't stop music after the fight\nDoesn't work for the Victory Fanfare") );
	
	gSizer18->Add( m_enemyflag13, 0, wxALL, 5 );
	
	m_enemyflag14 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF14, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( m_enemyflag14, 0, wxALL, 5 );
	
	m_enemyflag15 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF15, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag15->Hide();
	
	gSizer18->Add( m_enemyflag15, 0, wxALL, 5 );
	
	m_enemyflag16 = new wxCheckBox( m_enemyscrolledwindow, wxID_BF16, _("Unknown"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyflag16->Hide();
	
	gSizer18->Add( m_enemyflag16, 0, wxALL, 5 );
	
	
	fgSizer8->Add( gSizer18, 1, wxEXPAND, 5 );
	
	
	m_enemyscrolledwindow->SetSizer( fgSizer8 );
	m_enemyscrolledwindow->Layout();
	fgSizer8->Fit( m_enemyscrolledwindow );
	bSizer10->Add( m_enemyscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	m_enemystatscrolledwindow = new wxScrolledWindow( m_panelbattle, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_enemystatscrolledwindow->SetScrollRate( 5, 5 );
	m_enemystatscrolledwindow->Hide();
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_enemystatbook = new wxNotebook( m_enemystatscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatpanel1 = new wxPanel( m_enemystatbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 19, 2, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_enemystatnamelabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatnamelabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatnamelabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	m_enemystatname = new wxTextCtrl( m_enemystatpanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer26->Add( m_enemystatname, 0, wxALL, 2 );
	
	m_enemystatnamebutton = new wxButton( m_enemystatpanel1, wxID_STATNAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_enemystatnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer26->Add( m_enemystatnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer9->Add( bSizer26, 1, wxEXPAND, 5 );
	
	m_enemystatlvllabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Level"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatlvllabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatlvllabel, 0, wxALL, 5 );
	
	m_enemystatlvl = new wxSpinCtrl( m_enemystatpanel1, wxID_LVL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatlvl, 0, wxALL, 2 );
	
	m_enemystathplabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("HP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystathplabel->Wrap( -1 );
	fgSizer9->Add( m_enemystathplabel, 0, wxALL, 5 );
	
	m_enemystathp = new wxSpinCtrl( m_enemystatpanel1, wxID_HP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer9->Add( m_enemystathp, 0, wxALL, 2 );
	
	m_enemystatmplabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("MP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatmplabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatmplabel, 0, wxALL, 5 );
	
	m_enemystatmp = new wxSpinCtrl( m_enemystatpanel1, wxID_MP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer9->Add( m_enemystatmp, 0, wxALL, 2 );
	
	m_staticText69 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Attack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	fgSizer9->Add( m_staticText69, 0, wxALL, 5 );
	
	m_enemystatattack = new wxSpinCtrl( m_enemystatpanel1, wxID_ATTACK, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer9->Add( m_enemystatattack, 0, wxALL, 2 );
	
	m_enemystatspeedlabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatspeedlabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatspeedlabel, 0, wxALL, 5 );
	
	m_enemystatspeed = new wxSpinCtrl( m_enemystatpanel1, wxID_SPEED, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatspeed, 0, wxALL, 2 );
	
	m_enemystatstrengthlabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Strength"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatstrengthlabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatstrengthlabel, 0, wxALL, 5 );
	
	m_enemystatstrength = new wxSpinCtrl( m_enemystatpanel1, wxID_STRENGTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatstrength, 0, wxALL, 2 );
	
	m_enemystatmagiclabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Magic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatmagiclabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatmagiclabel, 0, wxALL, 5 );
	
	m_enemystatmagic = new wxSpinCtrl( m_enemystatpanel1, wxID_MAGIC, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatmagic, 0, wxALL, 2 );
	
	m_enemystatspiritlabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Spirit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatspiritlabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatspiritlabel, 0, wxALL, 5 );
	
	m_enemystatspirit = new wxSpinCtrl( m_enemystatpanel1, wxID_SPIRIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatspirit, 0, wxALL, 2 );
	
	m_staticText65 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Defence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText65->Wrap( -1 );
	fgSizer9->Add( m_staticText65, 0, wxALL, 5 );
	
	m_enemystatdefence = new wxSpinCtrl( m_enemystatpanel1, wxID_DEFENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatdefence, 0, wxALL, 2 );
	
	m_staticText66 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Evade"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText66->Wrap( -1 );
	fgSizer9->Add( m_staticText66, 0, wxALL, 5 );
	
	m_enemystatevade = new wxSpinCtrl( m_enemystatpanel1, wxID_EVADE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatevade, 0, wxALL, 2 );
	
	m_staticText67 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Magic Defence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText67->Wrap( -1 );
	fgSizer9->Add( m_staticText67, 0, wxALL, 5 );
	
	m_enemystatmagicdefence = new wxSpinCtrl( m_enemystatpanel1, wxID_MDEFENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatmagicdefence, 0, wxALL, 2 );
	
	m_staticText68 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Magic Evade"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText68->Wrap( -1 );
	fgSizer9->Add( m_staticText68, 0, wxALL, 5 );
	
	m_enemystatmagicevade = new wxSpinCtrl( m_enemystatpanel1, wxID_MEVADE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer9->Add( m_enemystatmagicevade, 0, wxALL, 2 );
	
	m_enemystatitemsteallabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Steal"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatitemsteallabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatitemsteallabel, 0, wxALL, 5 );
	
	wxGridSizer* m_enemystatitemsteal;
	m_enemystatitemsteal = new wxGridSizer( 4, 1, 0, 0 );
	
	wxArrayString m_enemystatitemsteal1Choices;
	m_enemystatitemsteal1 = new wxChoice( m_enemystatpanel1, wxID_STEAL1, wxDefaultPosition, wxDefaultSize, m_enemystatitemsteal1Choices, 0 );
	m_enemystatitemsteal1->SetSelection( 0 );
	m_enemystatitemsteal1->SetToolTip( _("The first steal slot :\n256/256 steal rate") );
	
	m_enemystatitemsteal->Add( m_enemystatitemsteal1, 0, wxALL, 2 );
	
	wxArrayString m_enemystatitemsteal2Choices;
	m_enemystatitemsteal2 = new wxChoice( m_enemystatpanel1, wxID_STEAL2, wxDefaultPosition, wxDefaultSize, m_enemystatitemsteal2Choices, 0 );
	m_enemystatitemsteal2->SetSelection( 0 );
	m_enemystatitemsteal2->SetToolTip( _("The second steal slot :\n64/256 steal rate") );
	
	m_enemystatitemsteal->Add( m_enemystatitemsteal2, 0, wxALL, 2 );
	
	wxArrayString m_enemystatitemsteal3Choices;
	m_enemystatitemsteal3 = new wxChoice( m_enemystatpanel1, wxID_STEAL3, wxDefaultPosition, wxDefaultSize, m_enemystatitemsteal3Choices, 0 );
	m_enemystatitemsteal3->SetSelection( 0 );
	m_enemystatitemsteal3->SetToolTip( _("The third steal slot :\n16/256 steal rate\n32/256 with Master Thief") );
	
	m_enemystatitemsteal->Add( m_enemystatitemsteal3, 0, wxALL, 2 );
	
	wxArrayString m_enemystatitemsteal4Choices;
	m_enemystatitemsteal4 = new wxChoice( m_enemystatpanel1, wxID_STEAL4, wxDefaultPosition, wxDefaultSize, m_enemystatitemsteal4Choices, 0 );
	m_enemystatitemsteal4->SetSelection( 0 );
	m_enemystatitemsteal4->SetToolTip( _("The fourth steal slot :\n1/256 steal rate\n32/256 with Master Thief") );
	
	m_enemystatitemsteal->Add( m_enemystatitemsteal4, 0, wxALL, 2 );
	
	
	fgSizer9->Add( m_enemystatitemsteal, 1, wxEXPAND, 5 );
	
	m_enemystatitemdroplabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Drop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatitemdroplabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatitemdroplabel, 0, wxALL, 5 );
	
	wxGridSizer* m_enemystatitemdrop;
	m_enemystatitemdrop = new wxGridSizer( 4, 1, 0, 0 );
	
	wxArrayString m_enemystatitemdrop1Choices;
	m_enemystatitemdrop1 = new wxChoice( m_enemystatpanel1, wxID_DROP1, wxDefaultPosition, wxDefaultSize, m_enemystatitemdrop1Choices, 0 );
	m_enemystatitemdrop1->SetSelection( 0 );
	m_enemystatitemdrop1->SetToolTip( _("The first drop slot :\n256/256 drop rate") );
	
	m_enemystatitemdrop->Add( m_enemystatitemdrop1, 0, wxALL, 2 );
	
	wxArrayString m_enemystatitemdrop2Choices;
	m_enemystatitemdrop2 = new wxChoice( m_enemystatpanel1, wxID_DROP2, wxDefaultPosition, wxDefaultSize, m_enemystatitemdrop2Choices, 0 );
	m_enemystatitemdrop2->SetSelection( 0 );
	m_enemystatitemdrop2->SetToolTip( _("The second drop slot :\n96/256 drop rate") );
	
	m_enemystatitemdrop->Add( m_enemystatitemdrop2, 0, wxALL, 2 );
	
	wxArrayString m_enemystatitemdrop3Choices;
	m_enemystatitemdrop3 = new wxChoice( m_enemystatpanel1, wxID_DROP3, wxDefaultPosition, wxDefaultSize, m_enemystatitemdrop3Choices, 0 );
	m_enemystatitemdrop3->SetSelection( 0 );
	m_enemystatitemdrop3->SetToolTip( _("The third drop slot :\n32/256 drop rate") );
	
	m_enemystatitemdrop->Add( m_enemystatitemdrop3, 0, wxALL, 2 );
	
	wxArrayString m_enemystatitemdrop4Choices;
	m_enemystatitemdrop4 = new wxChoice( m_enemystatpanel1, wxID_DROP4, wxDefaultPosition, wxDefaultSize, m_enemystatitemdrop4Choices, 0 );
	m_enemystatitemdrop4->SetSelection( 0 );
	m_enemystatitemdrop4->SetToolTip( _("The fourth drop slot :\n1/256 drop rate") );
	
	m_enemystatitemdrop->Add( m_enemystatitemdrop4, 0, wxALL, 2 );
	
	
	fgSizer9->Add( m_enemystatitemdrop, 1, wxEXPAND, 5 );
	
	m_enemystatdropcardlabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Card Drop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatdropcardlabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatdropcardlabel, 0, wxALL, 5 );
	
	wxArrayString m_enemystatdropcardChoices;
	m_enemystatdropcard = new wxChoice( m_enemystatpanel1, wxID_CARD, wxDefaultPosition, wxDefaultSize, m_enemystatdropcardChoices, 0 );
	m_enemystatdropcard->SetSelection( 0 );
	m_enemystatdropcard->SetToolTip( _("32/256 drop rate") );
	
	fgSizer9->Add( m_enemystatdropcard, 0, wxALL, 2 );
	
	m_enemystatexplabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Exp"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatexplabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatexplabel, 0, wxALL, 5 );
	
	m_enemystatexp = new wxSpinCtrl( m_enemystatpanel1, wxID_EXP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer9->Add( m_enemystatexp, 0, wxALL, 2 );
	
	m_enemystatgilslabel = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Gils"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatgilslabel->Wrap( -1 );
	fgSizer9->Add( m_enemystatgilslabel, 0, wxALL, 5 );
	
	m_enemystatgils = new wxSpinCtrl( m_enemystatpanel1, wxID_GILS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer9->Add( m_enemystatgils, 0, wxALL, 2 );
	
	m_staticText264 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Default Attack"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText264->Wrap( -1 );
	fgSizer9->Add( m_staticText264, 0, wxALL, 5 );
	
	wxArrayString m_enemystatdefaultattackChoices;
	m_enemystatdefaultattack = new wxChoice( m_enemystatpanel1, wxID_DEFATTACK, wxDefaultPosition, wxDefaultSize, m_enemystatdefaultattackChoices, 0 );
	m_enemystatdefaultattack->SetSelection( 0 );
	m_enemystatdefaultattack->SetToolTip( _("Attack used under the effects of\n\"Berserk\" and \"Confuse\"") );
	
	fgSizer9->Add( m_enemystatdefaultattack, 0, wxALL, 2 );
	
	m_staticText71 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Model ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer9->Add( m_staticText71, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer198;
	bSizer198 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_enemystatmodelChoices;
	m_enemystatmodel = new wxChoice( m_enemystatpanel1, wxID_MODEL, wxDefaultPosition, wxDefaultSize, m_enemystatmodelChoices, 0 );
	m_enemystatmodel->SetSelection( 0 );
	m_enemystatmodel->Enable( false );
	m_enemystatmodel->SetToolTip( _("Warning: if you change an enemy model,\nbe sure to modify the spell sequences accordingly") );
	
	bSizer198->Add( m_enemystatmodel, 0, wxALL|wxEXPAND, 2 );
	
	m_enemystatmodelid = new wxSpinCtrl( m_enemystatpanel1, wxID_MODEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer198->Add( m_enemystatmodelid, 0, wxALL, 2 );
	
	
	fgSizer9->Add( bSizer198, 1, wxEXPAND, 5 );
	
	m_staticText358 = new wxStaticText( m_enemystatpanel1, wxID_ANY, _("Resources"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText358->Wrap( -1 );
	fgSizer9->Add( m_staticText358, 0, wxALL, 5 );
	
	m_enemystatresources = new wxButton( m_enemystatpanel1, wxID_RESOURCES, _("Edit Resources"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_enemystatresources, 0, wxALL, 2 );
	
	
	m_enemystatpanel1->SetSizer( fgSizer9 );
	m_enemystatpanel1->Layout();
	fgSizer9->Fit( m_enemystatpanel1 );
	m_enemystatbook->AddPage( m_enemystatpanel1, _("Page 1"), true );
	m_enemystatpanel2 = new wxPanel( m_enemystatbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 9, 2, 0, 0 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_enemystatelementabsorblabel = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Absorb"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorblabel->Wrap( -1 );
	fgSizer92->Add( m_enemystatelementabsorblabel, 0, wxALL, 5 );
	
	wxGridSizer* m_enemystatelementabsorb;
	m_enemystatelementabsorb = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemystatelementabsorb1 = new wxCheckBox( m_enemystatpanel2, wxID_EA1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb1, 0, wxALL, 5 );
	
	m_enemystatelementabsorb2 = new wxCheckBox( m_enemystatpanel2, wxID_EA2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb2, 0, wxALL, 5 );
	
	m_enemystatelementabsorb3 = new wxCheckBox( m_enemystatpanel2, wxID_EA3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb3, 0, wxALL, 5 );
	
	m_enemystatelementabsorb4 = new wxCheckBox( m_enemystatpanel2, wxID_EA4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb4, 0, wxALL, 5 );
	
	m_enemystatelementabsorb5 = new wxCheckBox( m_enemystatpanel2, wxID_EA5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb5, 0, wxALL, 5 );
	
	m_enemystatelementabsorb6 = new wxCheckBox( m_enemystatpanel2, wxID_EA6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb6, 0, wxALL, 5 );
	
	m_enemystatelementabsorb7 = new wxCheckBox( m_enemystatpanel2, wxID_EA7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb7, 0, wxALL, 5 );
	
	m_enemystatelementabsorb8 = new wxCheckBox( m_enemystatpanel2, wxID_EA8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementabsorb->Add( m_enemystatelementabsorb8, 0, wxALL, 5 );
	
	
	fgSizer92->Add( m_enemystatelementabsorb, 1, wxEXPAND, 5 );
	
	m_enemystatelementimmunelabel = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Immune"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmunelabel->Wrap( -1 );
	fgSizer92->Add( m_enemystatelementimmunelabel, 0, wxALL, 5 );
	
	wxGridSizer* m_enemystatelementimmune;
	m_enemystatelementimmune = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemystatelementimmune1 = new wxCheckBox( m_enemystatpanel2, wxID_EI1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune1, 0, wxALL, 5 );
	
	m_enemystatelementimmune2 = new wxCheckBox( m_enemystatpanel2, wxID_EI2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune2, 0, wxALL, 5 );
	
	m_enemystatelementimmune3 = new wxCheckBox( m_enemystatpanel2, wxID_EI3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune3, 0, wxALL, 5 );
	
	m_enemystatelementimmune4 = new wxCheckBox( m_enemystatpanel2, wxID_EI4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune4, 0, wxALL, 5 );
	
	m_enemystatelementimmune5 = new wxCheckBox( m_enemystatpanel2, wxID_EI5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune5, 0, wxALL, 5 );
	
	m_enemystatelementimmune6 = new wxCheckBox( m_enemystatpanel2, wxID_EI6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune6, 0, wxALL, 5 );
	
	m_enemystatelementimmune7 = new wxCheckBox( m_enemystatpanel2, wxID_EI7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune7, 0, wxALL, 5 );
	
	m_enemystatelementimmune8 = new wxCheckBox( m_enemystatpanel2, wxID_EI8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementimmune->Add( m_enemystatelementimmune8, 0, wxALL, 5 );
	
	
	fgSizer92->Add( m_enemystatelementimmune, 1, wxEXPAND, 5 );
	
	m_enemystatelementhalflabel = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Half"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalflabel->Wrap( -1 );
	fgSizer92->Add( m_enemystatelementhalflabel, 0, wxALL, 5 );
	
	wxGridSizer* m_enemystatelementhalf;
	m_enemystatelementhalf = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemystatelementhalf1 = new wxCheckBox( m_enemystatpanel2, wxID_EH1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf1, 0, wxALL, 5 );
	
	m_enemystatelementhalf2 = new wxCheckBox( m_enemystatpanel2, wxID_EH2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf2, 0, wxALL, 5 );
	
	m_enemystatelementhalf3 = new wxCheckBox( m_enemystatpanel2, wxID_EH3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf3, 0, wxALL, 5 );
	
	m_enemystatelementhalf4 = new wxCheckBox( m_enemystatpanel2, wxID_EH4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf4, 0, wxALL, 5 );
	
	m_enemystatelementhalf5 = new wxCheckBox( m_enemystatpanel2, wxID_EH5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf5, 0, wxALL, 5 );
	
	m_enemystatelementhalf6 = new wxCheckBox( m_enemystatpanel2, wxID_EH6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf6, 0, wxALL, 5 );
	
	m_enemystatelementhalf7 = new wxCheckBox( m_enemystatpanel2, wxID_EH7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf7, 0, wxALL, 5 );
	
	m_enemystatelementhalf8 = new wxCheckBox( m_enemystatpanel2, wxID_EH8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementhalf->Add( m_enemystatelementhalf8, 0, wxALL, 5 );
	
	
	fgSizer92->Add( m_enemystatelementhalf, 1, wxEXPAND, 5 );
	
	m_enemystatelementweaklabel = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Weak"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweaklabel->Wrap( -1 );
	fgSizer92->Add( m_enemystatelementweaklabel, 0, wxALL, 5 );
	
	wxGridSizer* m_enemystatelementweak;
	m_enemystatelementweak = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemystatelementweak1 = new wxCheckBox( m_enemystatpanel2, wxID_EW1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak1, 0, wxALL, 5 );
	
	m_enemystatelementweak2 = new wxCheckBox( m_enemystatpanel2, wxID_EW2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak2, 0, wxALL, 5 );
	
	m_enemystatelementweak3 = new wxCheckBox( m_enemystatpanel2, wxID_EW3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak3, 0, wxALL, 5 );
	
	m_enemystatelementweak4 = new wxCheckBox( m_enemystatpanel2, wxID_EW4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak4, 0, wxALL, 5 );
	
	m_enemystatelementweak5 = new wxCheckBox( m_enemystatpanel2, wxID_EW5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak5, 0, wxALL, 5 );
	
	m_enemystatelementweak6 = new wxCheckBox( m_enemystatpanel2, wxID_EW6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak6, 0, wxALL, 5 );
	
	m_enemystatelementweak7 = new wxCheckBox( m_enemystatpanel2, wxID_EW7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak7, 0, wxALL, 5 );
	
	m_enemystatelementweak8 = new wxCheckBox( m_enemystatpanel2, wxID_EW8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatelementweak->Add( m_enemystatelementweak8, 0, wxALL, 5 );
	
	
	fgSizer92->Add( m_enemystatelementweak, 1, wxEXPAND, 5 );
	
	m_enemystatclasslabel = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Class"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemystatclasslabel->Wrap( -1 );
	fgSizer92->Add( m_enemystatclasslabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer161;
	gSizer161 = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemystatclass1 = new wxCheckBox( m_enemystatpanel2, wxID_EC1, _("Human"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass1, 0, wxALL, 5 );
	
	m_enemystatclass2 = new wxCheckBox( m_enemystatpanel2, wxID_EC2, _("Beast"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass2, 0, wxALL, 5 );
	
	m_enemystatclass3 = new wxCheckBox( m_enemystatpanel2, wxID_EC3, _("Demon"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass3, 0, wxALL, 5 );
	
	m_enemystatclass4 = new wxCheckBox( m_enemystatpanel2, wxID_EC4, _("Dragon"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass4, 0, wxALL, 5 );
	
	m_enemystatclass5 = new wxCheckBox( m_enemystatpanel2, wxID_EC5, _("Undead"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass5, 0, wxALL, 5 );
	
	m_enemystatclass6 = new wxCheckBox( m_enemystatpanel2, wxID_EC6, _("Stone"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass6, 0, wxALL, 5 );
	
	m_enemystatclass7 = new wxCheckBox( m_enemystatpanel2, wxID_EC7, _("Bug"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass7, 0, wxALL, 5 );
	
	m_enemystatclass8 = new wxCheckBox( m_enemystatpanel2, wxID_EC8, _("Flying"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer161->Add( m_enemystatclass8, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer161, 1, wxEXPAND, 5 );
	
	m_staticText79 = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Status Immunity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText79->Wrap( -1 );
	fgSizer92->Add( m_staticText79, 0, wxALL, 5 );
	
	wxGridSizer* gSizer19;
	gSizer19 = new wxGridSizer( 8, 4, 0, 0 );
	
	m_enemystatstatusimmune1 = new wxCheckBox( m_enemystatpanel2, wxID_SI1, _("Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune1, 0, wxALL, 5 );
	
	m_enemystatstatusimmune2 = new wxCheckBox( m_enemystatpanel2, wxID_SI2, _("Venom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune2, 0, wxALL, 5 );
	
	m_enemystatstatusimmune3 = new wxCheckBox( m_enemystatpanel2, wxID_SI3, _("Virus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune3, 0, wxALL, 5 );
	
	m_enemystatstatusimmune4 = new wxCheckBox( m_enemystatpanel2, wxID_SI4, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune4, 0, wxALL, 5 );
	
	m_enemystatstatusimmune5 = new wxCheckBox( m_enemystatpanel2, wxID_SI5, _("Blind"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune5, 0, wxALL, 5 );
	
	m_enemystatstatusimmune6 = new wxCheckBox( m_enemystatpanel2, wxID_SI6, _("Trouble"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune6, 0, wxALL, 5 );
	
	m_enemystatstatusimmune7 = new wxCheckBox( m_enemystatpanel2, wxID_SI7, _("Zombie"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune7, 0, wxALL, 5 );
	
	m_enemystatstatusimmune8 = new wxCheckBox( m_enemystatpanel2, wxID_SI8, _("Easy Kill"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune8, 0, wxALL, 5 );
	
	m_enemystatstatusimmune9 = new wxCheckBox( m_enemystatpanel2, wxID_SI9, _("Death"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune9, 0, wxALL, 5 );
	
	m_enemystatstatusimmune10 = new wxCheckBox( m_enemystatpanel2, wxID_SI10, _("Low HP"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune10, 0, wxALL, 5 );
	
	m_enemystatstatusimmune11 = new wxCheckBox( m_enemystatpanel2, wxID_SI11, _("Confuse"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune11, 0, wxALL, 5 );
	
	m_enemystatstatusimmune12 = new wxCheckBox( m_enemystatpanel2, wxID_SI12, _("Berserk"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune12, 0, wxALL, 5 );
	
	m_enemystatstatusimmune13 = new wxCheckBox( m_enemystatpanel2, wxID_SI13, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune13, 0, wxALL, 5 );
	
	m_enemystatstatusimmune14 = new wxCheckBox( m_enemystatpanel2, wxID_SI14, _("Auto-Life"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune14, 0, wxALL, 5 );
	
	m_enemystatstatusimmune15 = new wxCheckBox( m_enemystatpanel2, wxID_SI15, _("Trance"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune15, 0, wxALL, 5 );
	
	m_enemystatstatusimmune16 = new wxCheckBox( m_enemystatpanel2, wxID_SI16, _("Defend"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune16, 0, wxALL, 5 );
	
	m_enemystatstatusimmune17 = new wxCheckBox( m_enemystatpanel2, wxID_SI17, _("Poison"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune17, 0, wxALL, 5 );
	
	m_enemystatstatusimmune18 = new wxCheckBox( m_enemystatpanel2, wxID_SI18, _("Sleep"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune18, 0, wxALL, 5 );
	
	m_enemystatstatusimmune19 = new wxCheckBox( m_enemystatpanel2, wxID_SI19, _("Regen"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune19, 0, wxALL, 5 );
	
	m_enemystatstatusimmune20 = new wxCheckBox( m_enemystatpanel2, wxID_SI20, _("Haste"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune20, 0, wxALL, 5 );
	
	m_enemystatstatusimmune21 = new wxCheckBox( m_enemystatpanel2, wxID_SI21, _("Slow"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune21, 0, wxALL, 5 );
	
	m_enemystatstatusimmune22 = new wxCheckBox( m_enemystatpanel2, wxID_SI22, _("Float"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune22, 0, wxALL, 5 );
	
	m_enemystatstatusimmune23 = new wxCheckBox( m_enemystatpanel2, wxID_SI23, _("Shell"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune23, 0, wxALL, 5 );
	
	m_enemystatstatusimmune24 = new wxCheckBox( m_enemystatpanel2, wxID_SI24, _("Protect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune24, 0, wxALL, 5 );
	
	m_enemystatstatusimmune25 = new wxCheckBox( m_enemystatpanel2, wxID_SI25, _("Heat"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune25, 0, wxALL, 5 );
	
	m_enemystatstatusimmune26 = new wxCheckBox( m_enemystatpanel2, wxID_SI26, _("Freeze"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune26, 0, wxALL, 5 );
	
	m_enemystatstatusimmune27 = new wxCheckBox( m_enemystatpanel2, wxID_SI27, _("Vanish"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune27, 0, wxALL, 5 );
	
	m_enemystatstatusimmune28 = new wxCheckBox( m_enemystatpanel2, wxID_SI28, _("Doom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune28, 0, wxALL, 5 );
	
	m_enemystatstatusimmune29 = new wxCheckBox( m_enemystatpanel2, wxID_SI29, _("Mini"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune29, 0, wxALL, 5 );
	
	m_enemystatstatusimmune30 = new wxCheckBox( m_enemystatpanel2, wxID_SI30, _("Reflect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune30, 0, wxALL, 5 );
	
	m_enemystatstatusimmune31 = new wxCheckBox( m_enemystatpanel2, wxID_SI31, _("Jump"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune31, 0, wxALL, 5 );
	
	m_enemystatstatusimmune32 = new wxCheckBox( m_enemystatpanel2, wxID_SI32, _("Gradual Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer19->Add( m_enemystatstatusimmune32, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer19, 1, wxEXPAND, 5 );
	
	m_staticText78 = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Auto-status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText78->Wrap( -1 );
	fgSizer92->Add( m_staticText78, 0, wxALL, 5 );
	
	wxGridSizer* gSizer191;
	gSizer191 = new wxGridSizer( 8, 4, 0, 0 );
	
	m_enemystatstatusauto1 = new wxCheckBox( m_enemystatpanel2, wxID_SA1, _("Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto1, 0, wxALL, 5 );
	
	m_enemystatstatusauto2 = new wxCheckBox( m_enemystatpanel2, wxID_SA2, _("Venom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto2, 0, wxALL, 5 );
	
	m_enemystatstatusauto3 = new wxCheckBox( m_enemystatpanel2, wxID_SA3, _("Virus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto3, 0, wxALL, 5 );
	
	m_enemystatstatusauto4 = new wxCheckBox( m_enemystatpanel2, wxID_SA4, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto4, 0, wxALL, 5 );
	
	m_enemystatstatusauto5 = new wxCheckBox( m_enemystatpanel2, wxID_SA5, _("Blind"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto5, 0, wxALL, 5 );
	
	m_enemystatstatusauto6 = new wxCheckBox( m_enemystatpanel2, wxID_SA6, _("Trouble"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto6, 0, wxALL, 5 );
	
	m_enemystatstatusauto7 = new wxCheckBox( m_enemystatpanel2, wxID_SA7, _("Zombie"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto7, 0, wxALL, 5 );
	
	m_enemystatstatusauto8 = new wxCheckBox( m_enemystatpanel2, wxID_SA8, _("Easy Kill"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto8, 0, wxALL, 5 );
	
	m_enemystatstatusauto9 = new wxCheckBox( m_enemystatpanel2, wxID_SA9, _("Death"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto9, 0, wxALL, 5 );
	
	m_enemystatstatusauto10 = new wxCheckBox( m_enemystatpanel2, wxID_SA10, _("Low HP"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto10, 0, wxALL, 5 );
	
	m_enemystatstatusauto11 = new wxCheckBox( m_enemystatpanel2, wxID_SA11, _("Confuse"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto11, 0, wxALL, 5 );
	
	m_enemystatstatusauto12 = new wxCheckBox( m_enemystatpanel2, wxID_SA12, _("Berserk"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto12, 0, wxALL, 5 );
	
	m_enemystatstatusauto13 = new wxCheckBox( m_enemystatpanel2, wxID_SA13, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto13, 0, wxALL, 5 );
	
	m_enemystatstatusauto14 = new wxCheckBox( m_enemystatpanel2, wxID_SA14, _("Auto-Life"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto14, 0, wxALL, 5 );
	
	m_enemystatstatusauto15 = new wxCheckBox( m_enemystatpanel2, wxID_SA15, _("Trance"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto15, 0, wxALL, 5 );
	
	m_enemystatstatusauto16 = new wxCheckBox( m_enemystatpanel2, wxID_SA16, _("Defend"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto16, 0, wxALL, 5 );
	
	m_enemystatstatusauto17 = new wxCheckBox( m_enemystatpanel2, wxID_SA17, _("Poison"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto17, 0, wxALL, 5 );
	
	m_enemystatstatusauto18 = new wxCheckBox( m_enemystatpanel2, wxID_SA18, _("Sleep"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto18, 0, wxALL, 5 );
	
	m_enemystatstatusauto19 = new wxCheckBox( m_enemystatpanel2, wxID_SA19, _("Regen"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto19, 0, wxALL, 5 );
	
	m_enemystatstatusauto20 = new wxCheckBox( m_enemystatpanel2, wxID_SA20, _("Haste"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto20, 0, wxALL, 5 );
	
	m_enemystatstatusauto21 = new wxCheckBox( m_enemystatpanel2, wxID_SA21, _("Slow"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto21, 0, wxALL, 5 );
	
	m_enemystatstatusauto22 = new wxCheckBox( m_enemystatpanel2, wxID_SA22, _("Float"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto22, 0, wxALL, 5 );
	
	m_enemystatstatusauto23 = new wxCheckBox( m_enemystatpanel2, wxID_SA23, _("Shell"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto23, 0, wxALL, 5 );
	
	m_enemystatstatusauto24 = new wxCheckBox( m_enemystatpanel2, wxID_SA24, _("Protect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto24, 0, wxALL, 5 );
	
	m_enemystatstatusauto25 = new wxCheckBox( m_enemystatpanel2, wxID_SA25, _("Heat"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto25, 0, wxALL, 5 );
	
	m_enemystatstatusauto26 = new wxCheckBox( m_enemystatpanel2, wxID_SA26, _("Freeze"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto26, 0, wxALL, 5 );
	
	m_enemystatstatusauto27 = new wxCheckBox( m_enemystatpanel2, wxID_SA27, _("Vanish"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto27, 0, wxALL, 5 );
	
	m_enemystatstatusauto28 = new wxCheckBox( m_enemystatpanel2, wxID_SA28, _("Doom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto28, 0, wxALL, 5 );
	
	m_enemystatstatusauto29 = new wxCheckBox( m_enemystatpanel2, wxID_SA29, _("Mini"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto29, 0, wxALL, 5 );
	
	m_enemystatstatusauto30 = new wxCheckBox( m_enemystatpanel2, wxID_SA30, _("Reflect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto30, 0, wxALL, 5 );
	
	m_enemystatstatusauto31 = new wxCheckBox( m_enemystatpanel2, wxID_SA31, _("Jump"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto31, 0, wxALL, 5 );
	
	m_enemystatstatusauto32 = new wxCheckBox( m_enemystatpanel2, wxID_SA32, _("Gradual Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer191->Add( m_enemystatstatusauto32, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer191, 1, wxEXPAND, 5 );
	
	m_staticText80 = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Initial Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText80->Wrap( -1 );
	fgSizer92->Add( m_staticText80, 0, wxALL, 5 );
	
	wxGridSizer* gSizer192;
	gSizer192 = new wxGridSizer( 8, 4, 0, 0 );
	
	m_enemystatstatusinitial1 = new wxCheckBox( m_enemystatpanel2, wxID_ST1, _("Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial1, 0, wxALL, 5 );
	
	m_enemystatstatusinitial2 = new wxCheckBox( m_enemystatpanel2, wxID_ST2, _("Venom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial2, 0, wxALL, 5 );
	
	m_enemystatstatusinitial3 = new wxCheckBox( m_enemystatpanel2, wxID_ST3, _("Virus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial3, 0, wxALL, 5 );
	
	m_enemystatstatusinitial4 = new wxCheckBox( m_enemystatpanel2, wxID_ST4, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial4, 0, wxALL, 5 );
	
	m_enemystatstatusinitial5 = new wxCheckBox( m_enemystatpanel2, wxID_ST5, _("Blind"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial5, 0, wxALL, 5 );
	
	m_enemystatstatusinitial6 = new wxCheckBox( m_enemystatpanel2, wxID_ST6, _("Trouble"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial6, 0, wxALL, 5 );
	
	m_enemystatstatusinitial7 = new wxCheckBox( m_enemystatpanel2, wxID_ST7, _("Zombie"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial7, 0, wxALL, 5 );
	
	m_enemystatstatusinitial8 = new wxCheckBox( m_enemystatpanel2, wxID_ST8, _("Easy Kill"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial8, 0, wxALL, 5 );
	
	m_enemystatstatusinitial9 = new wxCheckBox( m_enemystatpanel2, wxID_ST9, _("Death"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial9, 0, wxALL, 5 );
	
	m_enemystatstatusinitial10 = new wxCheckBox( m_enemystatpanel2, wxID_ST10, _("Low HP"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial10, 0, wxALL, 5 );
	
	m_enemystatstatusinitial11 = new wxCheckBox( m_enemystatpanel2, wxID_ST11, _("Confuse"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial11, 0, wxALL, 5 );
	
	m_enemystatstatusinitial12 = new wxCheckBox( m_enemystatpanel2, wxID_ST12, _("Berserk"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial12, 0, wxALL, 5 );
	
	m_enemystatstatusinitial13 = new wxCheckBox( m_enemystatpanel2, wxID_ST13, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial13, 0, wxALL, 5 );
	
	m_enemystatstatusinitial14 = new wxCheckBox( m_enemystatpanel2, wxID_ST14, _("Auto-Life"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial14, 0, wxALL, 5 );
	
	m_enemystatstatusinitial15 = new wxCheckBox( m_enemystatpanel2, wxID_ST15, _("Trance"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial15, 0, wxALL, 5 );
	
	m_enemystatstatusinitial16 = new wxCheckBox( m_enemystatpanel2, wxID_ST16, _("Defend"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial16, 0, wxALL, 5 );
	
	m_enemystatstatusinitial17 = new wxCheckBox( m_enemystatpanel2, wxID_ST17, _("Poison"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial17, 0, wxALL, 5 );
	
	m_enemystatstatusinitial18 = new wxCheckBox( m_enemystatpanel2, wxID_ST18, _("Sleep"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial18, 0, wxALL, 5 );
	
	m_enemystatstatusinitial19 = new wxCheckBox( m_enemystatpanel2, wxID_ST19, _("Regen"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial19, 0, wxALL, 5 );
	
	m_enemystatstatusinitial20 = new wxCheckBox( m_enemystatpanel2, wxID_ST20, _("Haste"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial20, 0, wxALL, 5 );
	
	m_enemystatstatusinitial21 = new wxCheckBox( m_enemystatpanel2, wxID_ST21, _("Slow"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial21, 0, wxALL, 5 );
	
	m_enemystatstatusinitial22 = new wxCheckBox( m_enemystatpanel2, wxID_ST22, _("Float"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial22, 0, wxALL, 5 );
	
	m_enemystatstatusinitial23 = new wxCheckBox( m_enemystatpanel2, wxID_ST23, _("Shell"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial23, 0, wxALL, 5 );
	
	m_enemystatstatusinitial24 = new wxCheckBox( m_enemystatpanel2, wxID_ST24, _("Protect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial24, 0, wxALL, 5 );
	
	m_enemystatstatusinitial25 = new wxCheckBox( m_enemystatpanel2, wxID_ST25, _("Heat"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial25, 0, wxALL, 5 );
	
	m_enemystatstatusinitial26 = new wxCheckBox( m_enemystatpanel2, wxID_ST26, _("Freeze"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial26, 0, wxALL, 5 );
	
	m_enemystatstatusinitial27 = new wxCheckBox( m_enemystatpanel2, wxID_ST27, _("Vanish"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial27, 0, wxALL, 5 );
	
	m_enemystatstatusinitial28 = new wxCheckBox( m_enemystatpanel2, wxID_ST28, _("Doom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial28, 0, wxALL, 5 );
	
	m_enemystatstatusinitial29 = new wxCheckBox( m_enemystatpanel2, wxID_ST29, _("Mini"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial29, 0, wxALL, 5 );
	
	m_enemystatstatusinitial30 = new wxCheckBox( m_enemystatpanel2, wxID_ST30, _("Reflect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial30, 0, wxALL, 5 );
	
	m_enemystatstatusinitial31 = new wxCheckBox( m_enemystatpanel2, wxID_ST31, _("Jump"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial31, 0, wxALL, 5 );
	
	m_enemystatstatusinitial32 = new wxCheckBox( m_enemystatpanel2, wxID_ST32, _("Gradual Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer192->Add( m_enemystatstatusinitial32, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer192, 1, wxEXPAND, 5 );
	
	m_staticText701 = new wxStaticText( m_enemystatpanel2, wxID_ANY, _("Blue Magic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText701->Wrap( -1 );
	fgSizer92->Add( m_staticText701, 0, wxALL, 5 );
	
	wxArrayString m_enemystatbluemagicChoices;
	m_enemystatbluemagic = new wxChoice( m_enemystatpanel2, wxID_BLUEMAGIC, wxDefaultPosition, wxDefaultSize, m_enemystatbluemagicChoices, 0 );
	m_enemystatbluemagic->SetSelection( 0 );
	m_enemystatbluemagic->SetToolTip( _("Ability learned when eaten\nNote that supporting abilities can be learned this way\nbut their names are not properly displayed") );
	
	fgSizer92->Add( m_enemystatbluemagic, 0, wxALL, 2 );
	
	
	m_enemystatpanel2->SetSizer( fgSizer92 );
	m_enemystatpanel2->Layout();
	fgSizer92->Fit( m_enemystatpanel2 );
	m_enemystatbook->AddPage( m_enemystatpanel2, _("Page 2"), false );
	
	fgSizer12->Add( m_enemystatbook, 1, wxEXPAND, 5 );
	
	
	m_enemystatscrolledwindow->SetSizer( fgSizer12 );
	m_enemystatscrolledwindow->Layout();
	fgSizer12->Fit( m_enemystatscrolledwindow );
	bSizer10->Add( m_enemystatscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	m_enemyspellscrolledwindow = new wxScrolledWindow( m_panelbattle, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_enemyspellscrolledwindow->SetScrollRate( 5, 5 );
	m_enemyspellscrolledwindow->Hide();
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 9, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText74 = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer10->Add( m_staticText74, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	m_enemyspellname = new wxTextCtrl( m_enemyspellscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer27->Add( m_enemyspellname, 0, wxALL, 2 );
	
	m_enemyspellnamebutton = new wxButton( m_enemyspellscrolledwindow, wxID_SPELLNAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_enemyspellnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer27->Add( m_enemyspellnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer10->Add( bSizer27, 1, wxEXPAND, 5 );
	
	m_enemyspelleffectlabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Effect"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspelleffectlabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspelleffectlabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_enemyspelleffectChoices;
	m_enemyspelleffect = new wxChoice( m_enemyspellscrolledwindow, wxID_EFFECT, wxDefaultPosition, wxSize( -1,-1 ), m_enemyspelleffectChoices, 0 );
	m_enemyspelleffect->SetSelection( 0 );
	bSizer61->Add( m_enemyspelleffect, 0, wxALL, 2 );
	
	m_enemyspelleffecthelpwindow = new wxScrolledWindow( m_enemyspellscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_enemyspelleffecthelpwindow->SetScrollRate( 5, 5 );
	m_enemyspelleffecthelpwindow->SetMinSize( wxSize( -1,60 ) );
	
	wxBoxSizer* bSizer711;
	bSizer711 = new wxBoxSizer( wxVERTICAL );
	
	m_enemyspelleffecthelp = new wxStaticText( m_enemyspelleffecthelpwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_enemyspelleffecthelp->Wrap( -1 );
	bSizer711->Add( m_enemyspelleffecthelp, 0, wxALL, 2 );
	
	
	m_enemyspelleffecthelpwindow->SetSizer( bSizer711 );
	m_enemyspelleffecthelpwindow->Layout();
	bSizer711->Fit( m_enemyspelleffecthelpwindow );
	bSizer61->Add( m_enemyspelleffecthelpwindow, 1, wxEXPAND, 5 );
	
	
	fgSizer10->Add( bSizer61, 1, wxEXPAND, 5 );
	
	m_enemyspellpowerlabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Power"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellpowerlabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspellpowerlabel, 0, wxALL, 5 );
	
	m_enemyspellpower = new wxSpinCtrl( m_enemyspellscrolledwindow, wxID_POWER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer10->Add( m_enemyspellpower, 0, wxALL, 2 );
	
	m_enemyspellelementlabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Element"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellelementlabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspellelementlabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer22;
	gSizer22 = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemyspellelement1 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement1, 0, wxALL, 5 );
	
	m_enemyspellelement2 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement2, 0, wxALL, 5 );
	
	m_enemyspellelement3 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement3, 0, wxALL, 5 );
	
	m_enemyspellelement4 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement4, 0, wxALL, 5 );
	
	m_enemyspellelement5 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement5, 0, wxALL, 5 );
	
	m_enemyspellelement6 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement6, 0, wxALL, 5 );
	
	m_enemyspellelement7 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement7, 0, wxALL, 5 );
	
	m_enemyspellelement8 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SE8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer22->Add( m_enemyspellelement8, 0, wxALL, 5 );
	
	
	fgSizer10->Add( gSizer22, 1, wxEXPAND, 5 );
	
	m_enemyspellaccuracylabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Accuracy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellaccuracylabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspellaccuracylabel, 0, wxALL, 5 );
	
	m_enemyspellaccuracy = new wxSpinCtrl( m_enemyspellscrolledwindow, wxID_ACCURACY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer10->Add( m_enemyspellaccuracy, 0, wxALL, 2 );
	
	m_enemyspellstatuslabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellstatuslabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspellstatuslabel, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer148;
	bSizer148 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_enemyspellstatusChoices;
	m_enemyspellstatus = new wxChoice( m_enemyspellscrolledwindow, wxID_STATUS, wxDefaultPosition, wxDefaultSize, m_enemyspellstatusChoices, 0 );
	m_enemyspellstatus->SetSelection( 0 );
	bSizer148->Add( m_enemyspellstatus, 0, wxALL, 2 );
	
	m_enemyspellstatusset = new wxButton( m_enemyspellscrolledwindow, wxID_STATUS, _("Edit Status Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellstatusset->SetToolTip( _("Warning\nChanging a status set will modify it\nfor the every spells using the same set") );
	
	bSizer148->Add( m_enemyspellstatusset, 0, wxALL, 2 );
	
	
	fgSizer10->Add( bSizer148, 1, wxEXPAND, 5 );
	
	m_enemyspellmplabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("MP Cost"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellmplabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspellmplabel, 0, wxALL, 5 );
	
	m_enemyspellmp = new wxSpinCtrl( m_enemyspellscrolledwindow, wxID_SPELLMP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer10->Add( m_enemyspellmp, 0, wxALL, 2 );
	
	m_staticText75 = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer10->Add( m_staticText75, 0, wxALL, 5 );
	
	wxGridSizer* gSizer31;
	gSizer31 = new wxGridSizer( 1, 2, 0, 0 );
	
	m_enemyspellanimedit = new wxButton( m_enemyspellscrolledwindow, wxID_ANIM, _("Edit Animation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellanimedit->SetToolTip( _("Edit the animation sequence") );
	
	gSizer31->Add( m_enemyspellanimedit, 0, wxALL, 2 );
	
	wxArrayString m_enemyspellanimreflectChoices;
	m_enemyspellanimreflect = new wxChoice( m_enemyspellscrolledwindow, wxID_ANIM, wxDefaultPosition, wxDefaultSize, m_enemyspellanimreflectChoices, 0 );
	m_enemyspellanimreflect->SetSelection( 0 );
	m_enemyspellanimreflect->SetToolTip( _("Spell animation used on \"Reflect Magic\"") );
	
	gSizer31->Add( m_enemyspellanimreflect, 0, wxALL, 2 );
	
	
	fgSizer10->Add( gSizer31, 1, wxEXPAND, 5 );
	
	m_staticText244 = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText244->Wrap( -1 );
	fgSizer10->Add( m_staticText244, 0, wxALL, 5 );
	
	wxArrayString m_enemyspellbaseanimChoices;
	m_enemyspellbaseanim = new wxChoice( m_enemyspellscrolledwindow, wxID_SEQANIM, wxDefaultPosition, wxDefaultSize, m_enemyspellbaseanimChoices, 0 );
	m_enemyspellbaseanim->SetSelection( 0 );
	m_enemyspellbaseanim->SetToolTip( _("Enemy using this ability, so the animations match") );
	
	fgSizer10->Add( m_enemyspellbaseanim, 0, wxALL, 2 );
	
	m_staticText313 = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("UI Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText313->Wrap( -1 );
	fgSizer10->Add( m_staticText313, 0, wxALL, 5 );
	
	wxGridSizer* gSizer213;
	gSizer213 = new wxGridSizer( 1, 3, 0, 0 );
	
	m_enemyspelltargetflagdead = new wxCheckBox( m_enemyspellscrolledwindow, wxID_CAN_TARGET_DEAD, _("Can target KO"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer213->Add( m_enemyspelltargetflagdead, 0, wxALL, 5 );
	
	m_enemyspelltargetflagdeadfirst = new wxCheckBox( m_enemyspellscrolledwindow, wxID_TARGET_DEAD_FIRST, _("Target KO in priority"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer213->Add( m_enemyspelltargetflagdeadfirst, 0, wxALL, 5 );
	
	m_enemyspelltargetflagcamera = new wxCheckBox( m_enemyspellscrolledwindow, wxID_TARGET_CAMERA, _("Alternate Camera"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspelltargetflagcamera->SetToolTip( _("Always use the alternative camera\nin spell animations using such a camera") );
	
	gSizer213->Add( m_enemyspelltargetflagcamera, 0, wxALL, 5 );
	
	
	fgSizer10->Add( gSizer213, 1, wxEXPAND, 5 );
	
	m_enemyspellflaglabel = new wxStaticText( m_enemyspellscrolledwindow, wxID_ANY, _("Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflaglabel->Wrap( -1 );
	fgSizer10->Add( m_enemyspellflaglabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer62;
	gSizer62 = new wxGridSizer( 2, 4, 0, 0 );
	
	m_enemyspellflag1 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF1, _("Reflectable"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer62->Add( m_enemyspellflag1, 0, wxALL, 5 );
	
	m_enemyspellflag2 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF2, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag2->SetToolTip( _("Can't cast under Silence and Oeilvert's Curse") );
	
	gSizer62->Add( m_enemyspellflag2, 0, wxALL, 5 );
	
	m_enemyspellflag3 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF3, _("No M-Sword"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag3->SetToolTip( _("\"No Magic Sword\"\nUnknown effect") );
	
	gSizer62->Add( m_enemyspellflag3, 0, wxALL, 5 );
	
	m_enemyspellflag4 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF4, _("Is Physical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag4->SetToolTip( _("For removing the Confuse and Sleep statuses\nAlso triggers the party's counterattack") );
	
	gSizer62->Add( m_enemyspellflag4, 0, wxALL, 5 );
	
	m_enemyspellflag5 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF5, _("Is Magical"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag5->SetToolTip( _("For removing the Vanish status") );
	
	gSizer62->Add( m_enemyspellflag5, 0, wxALL, 5 );
	
	m_enemyspellflag6 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF6, _("Short"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag6->SetToolTip( _("Unknown effect") );
	
	gSizer62->Add( m_enemyspellflag6, 0, wxALL, 5 );
	
	m_enemyspellflag7 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF7, _("Hit Anim Off"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag7->SetToolTip( _("Don't trigger the target's hit animation") );
	
	gSizer62->Add( m_enemyspellflag7, 0, wxALL, 5 );
	
	m_enemyspellflag8 = new wxCheckBox( m_enemyspellscrolledwindow, wxID_SF8, _("Returnable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemyspellflag8->SetToolTip( _("Return Magic only triggers when the caster is an enemy of the target") );
	
	gSizer62->Add( m_enemyspellflag8, 0, wxALL, 5 );
	
	
	fgSizer10->Add( gSizer62, 1, wxEXPAND, 5 );
	
	
	m_enemyspellscrolledwindow->SetSizer( fgSizer10 );
	m_enemyspellscrolledwindow->Layout();
	fgSizer10->Fit( m_enemyspellscrolledwindow );
	bSizer10->Add( m_enemyspellscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	m_enemygroupscrolledwindow = new wxScrolledWindow( m_panelbattle, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_enemygroupscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer91;
	fgSizer91 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer91->SetFlexibleDirection( wxBOTH );
	fgSizer91->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_enemygroupfrequencelabel = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Frequency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygroupfrequencelabel->Wrap( -1 );
	fgSizer91->Add( m_enemygroupfrequencelabel, 0, wxALL, 5 );
	
	m_enemygroupfrequence = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_FREQUENCE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	m_enemygroupfrequence->SetToolTip( _("Chances for the group to appear in pourcentage") );
	
	fgSizer91->Add( m_enemygroupfrequence, 0, wxALL, 2 );
	
	m_enemygroupamountlabel = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Enemy Amount"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygroupamountlabel->Wrap( -1 );
	fgSizer91->Add( m_enemygroupamountlabel, 0, wxALL, 5 );
	
	m_enemygroupamount = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_AMOUNT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 4, 0 );
	m_enemygroupamount->SetToolTip( _("WARNING\nDon't forget to change the Script's main function accordingly") );
	
	fgSizer91->Add( m_enemygroupamount, 0, wxALL, 2 );
	
	m_enemygroupaplabel = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("AP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygroupaplabel->Wrap( -1 );
	fgSizer91->Add( m_enemygroupaplabel, 0, wxALL, 5 );
	
	m_enemygroupap = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_AP, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_enemygroupap->SetToolTip( _("Only 3 figures can be displayed in-game") );
	
	fgSizer91->Add( m_enemygroupap, 0, wxALL, 2 );
	
	m_staticText246 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Camera"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText246->Wrap( -1 );
	fgSizer91->Add( m_staticText246, 0, wxALL, 5 );
	
	m_enemygroupcamera = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_CAMERA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_enemygroupcamera->SetToolTip( _("Opening camera ID\nMust be one of the 3 first cameras (0, 1 or 2)\nFor other values, a random camera is chosen among the first 3") );
	
	fgSizer91->Add( m_enemygroupcamera, 0, wxALL, 2 );
	
	m_enemygroupidlabel = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Enemies"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygroupidlabel->Wrap( -1 );
	fgSizer91->Add( m_enemygroupidlabel, 0, wxALL, 5 );
	
	wxGridSizer* gSizer9;
	gSizer9 = new wxGridSizer( 4, 1, 0, 0 );
	
	wxGridBagSizer* gbSizer21;
	gbSizer21 = new wxGridBagSizer( 0, 0 );
	gbSizer21->SetFlexibleDirection( wxBOTH );
	gbSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_enemygroupid1Choices;
	m_enemygroupid1 = new wxChoice( m_enemygroupscrolledwindow, wxID_ENEMY1, wxDefaultPosition, wxDefaultSize, m_enemygroupid1Choices, 0 );
	m_enemygroupid1->SetSelection( 0 );
	gbSizer21->Add( m_enemygroupid1, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_enemygrouptargetable1 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_TARGETABLE1, _("Targetable"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer21->Add( m_enemygrouptargetable1, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_enemygrouplink1 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_LINK1, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygrouplink1->SetToolTip( _("Link the enemies together,\nsharing the counter reactions\nand scripted actions") );
	
	gbSizer21->Add( m_enemygrouplink1, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText222 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Position X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText222->Wrap( -1 );
	gbSizer21->Add( m_staticText222, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposx1 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSX1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposx1->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer21->Add( m_enemygroupposx1, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText223 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("PositionY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText223->Wrap( -1 );
	gbSizer21->Add( m_staticText223, wxGBPosition( 1, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposy1 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSY1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposy1->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer21->Add( m_enemygroupposy1, wxGBPosition( 2, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_staticText224 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText224->Wrap( -1 );
	gbSizer21->Add( m_staticText224, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposz1 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSZ1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32767, 32768, 0 );
	gbSizer21->Add( m_enemygroupposz1, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText225 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText225->Wrap( -1 );
	gbSizer21->Add( m_staticText225, wxGBPosition( 3, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupangle1 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_ANGLE1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -360, 360, 0 );
	gbSizer21->Add( m_enemygroupangle1, wxGBPosition( 4, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	
	gSizer9->Add( gbSizer21, 1, wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer211;
	gbSizer211 = new wxGridBagSizer( 0, 0 );
	gbSizer211->SetFlexibleDirection( wxBOTH );
	gbSizer211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_enemygroupid2Choices;
	m_enemygroupid2 = new wxChoice( m_enemygroupscrolledwindow, wxID_ENEMY2, wxDefaultPosition, wxDefaultSize, m_enemygroupid2Choices, 0 );
	m_enemygroupid2->SetSelection( 0 );
	gbSizer211->Add( m_enemygroupid2, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_enemygrouptargetable2 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_TARGETABLE2, _("Targetable"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer211->Add( m_enemygrouptargetable2, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_enemygrouplink2 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_LINK2, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygrouplink2->SetToolTip( _("Link the enemies together,\nsharing the counter reactions\nand scripted actions") );
	
	gbSizer211->Add( m_enemygrouplink2, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText2221 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Position X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2221->Wrap( -1 );
	gbSizer211->Add( m_staticText2221, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposx2 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSX2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposx2->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer211->Add( m_enemygroupposx2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText2231 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("PositionY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2231->Wrap( -1 );
	gbSizer211->Add( m_staticText2231, wxGBPosition( 1, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposy2 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSY2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposy2->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer211->Add( m_enemygroupposy2, wxGBPosition( 2, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_staticText2241 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2241->Wrap( -1 );
	gbSizer211->Add( m_staticText2241, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposz2 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSZ2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32767, 32768, 0 );
	gbSizer211->Add( m_enemygroupposz2, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText2251 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2251->Wrap( -1 );
	gbSizer211->Add( m_staticText2251, wxGBPosition( 3, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupangle2 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_ANGLE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -360, 360, 0 );
	gbSizer211->Add( m_enemygroupangle2, wxGBPosition( 4, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	
	gSizer9->Add( gbSizer211, 1, wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer212;
	gbSizer212 = new wxGridBagSizer( 0, 0 );
	gbSizer212->SetFlexibleDirection( wxBOTH );
	gbSizer212->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_enemygroupid3Choices;
	m_enemygroupid3 = new wxChoice( m_enemygroupscrolledwindow, wxID_ENEMY3, wxDefaultPosition, wxDefaultSize, m_enemygroupid3Choices, 0 );
	m_enemygroupid3->SetSelection( 0 );
	gbSizer212->Add( m_enemygroupid3, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_enemygrouptargetable3 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_TARGETABLE3, _("Targetable"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer212->Add( m_enemygrouptargetable3, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_enemygrouplink3 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_LINK3, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygrouplink3->SetToolTip( _("Link the enemies together,\nsharing the counter reactions\nand scripted actions") );
	
	gbSizer212->Add( m_enemygrouplink3, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText2222 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Position X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2222->Wrap( -1 );
	gbSizer212->Add( m_staticText2222, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposx3 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSX3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposx3->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer212->Add( m_enemygroupposx3, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText2232 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("PositionY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2232->Wrap( -1 );
	gbSizer212->Add( m_staticText2232, wxGBPosition( 1, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposy3 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSY3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposy3->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer212->Add( m_enemygroupposy3, wxGBPosition( 2, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_staticText2242 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2242->Wrap( -1 );
	gbSizer212->Add( m_staticText2242, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposz3 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSZ3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32767, 32768, 0 );
	gbSizer212->Add( m_enemygroupposz3, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText2252 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2252->Wrap( -1 );
	gbSizer212->Add( m_staticText2252, wxGBPosition( 3, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupangle3 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_ANGLE3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -360, 360, 0 );
	gbSizer212->Add( m_enemygroupangle3, wxGBPosition( 4, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	
	gSizer9->Add( gbSizer212, 1, wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer213;
	gbSizer213 = new wxGridBagSizer( 0, 0 );
	gbSizer213->SetFlexibleDirection( wxBOTH );
	gbSizer213->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_enemygroupid4Choices;
	m_enemygroupid4 = new wxChoice( m_enemygroupscrolledwindow, wxID_ENEMY4, wxDefaultPosition, wxDefaultSize, m_enemygroupid4Choices, 0 );
	m_enemygroupid4->SetSelection( 0 );
	gbSizer213->Add( m_enemygroupid4, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_enemygrouptargetable4 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_TARGETABLE4, _("Targetable"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer213->Add( m_enemygrouptargetable4, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_enemygrouplink4 = new wxCheckBox( m_enemygroupscrolledwindow, wxID_LINK4, _("Link"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemygrouplink4->SetToolTip( _("Link the enemies together,\nsharing the counter reactions\nand scripted actions") );
	
	gbSizer213->Add( m_enemygrouplink4, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText2223 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Position X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2223->Wrap( -1 );
	gbSizer213->Add( m_staticText2223, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposx4 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSX4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposx4->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer213->Add( m_enemygroupposx4, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText2233 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("PositionY"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2233->Wrap( -1 );
	gbSizer213->Add( m_staticText2233, wxGBPosition( 1, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposy4 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSY4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32768, 32767, 0 );
	m_enemygroupposy4->SetToolTip( _("Player's first character position is\n(948, -1560) for front row\n(948, -1960) for back row") );
	
	gbSizer213->Add( m_enemygroupposy4, wxGBPosition( 2, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	m_staticText2243 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2243->Wrap( -1 );
	gbSizer213->Add( m_staticText2243, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupposz4 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_POSZ4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -32767, 32768, 0 );
	gbSizer213->Add( m_enemygroupposz4, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL, 2 );
	
	m_staticText2253 = new wxStaticText( m_enemygroupscrolledwindow, wxID_ANY, _("Angle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2253->Wrap( -1 );
	gbSizer213->Add( m_staticText2253, wxGBPosition( 3, 2 ), wxGBSpan( 1, 2 ), 0, 5 );
	
	m_enemygroupangle4 = new wxSpinCtrl( m_enemygroupscrolledwindow, wxID_ANGLE4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -360, 360, 0 );
	gbSizer213->Add( m_enemygroupangle4, wxGBPosition( 4, 2 ), wxGBSpan( 1, 3 ), wxALL, 2 );
	
	
	gSizer9->Add( gbSizer213, 1, wxEXPAND, 5 );
	
	
	fgSizer91->Add( gSizer9, 1, wxEXPAND, 5 );
	
	
	m_enemygroupscrolledwindow->SetSizer( fgSizer91 );
	m_enemygroupscrolledwindow->Layout();
	fgSizer91->Fit( m_enemygroupscrolledwindow );
	bSizer10->Add( m_enemygroupscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelbattle->SetSizer( bSizer10 );
	m_panelbattle->Layout();
	bSizer10->Fit( m_panelbattle );
	m_notebookmain->AddPage( m_panelbattle, _("Enemies"), false );
	m_paneltetramaster = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	m_notebookcard = new wxNotebook( m_paneltetramaster, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel63 = new wxPanel( m_notebookcard, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer150;
	bSizer150 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cardlist = new wxListBox( m_panel63, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer150->Add( m_cardlist, 0, wxALL|wxEXPAND, 5 );
	
	m_cardscrolledwindow = new wxScrolledWindow( m_panel63, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_cardscrolledwindow->SetScrollRate( 5, 5 );
	wxGridBagSizer* gbSizer38;
	gbSizer38 = new wxGridBagSizer( 0, 0 );
	gbSizer38->SetFlexibleDirection( wxBOTH );
	gbSizer38->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText266 = new wxStaticText( m_cardscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText266->Wrap( -1 );
	gbSizer38->Add( m_staticText266, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxBoxSizer* bSizer222;
	bSizer222 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cardname = new wxTextCtrl( m_cardscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer222->Add( m_cardname, 0, wxALL, 2 );
	
	m_cardnamebutton = new wxButton( m_cardscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_cardnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer222->Add( m_cardnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	gbSizer38->Add( bSizer222, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_staticText461 = new wxStaticText( m_cardscrolledwindow, wxID_ANY, _("Points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText461->Wrap( -1 );
	gbSizer38->Add( m_staticText461, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_cardpoints = new wxSpinCtrl( m_cardscrolledwindow, wxID_POINTS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	m_cardpoints->SetToolTip( _("These points are given only for\nthe 1st card of this type") );
	
	gbSizer38->Add( m_cardpoints, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	wxBoxSizer* bSizer187;
	bSizer187 = new wxBoxSizer( wxVERTICAL );
	
	m_cardstatpsxpanel = new wxPanel( m_cardscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer732;
	fgSizer732 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer732->SetFlexibleDirection( wxBOTH );
	fgSizer732->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4622 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4622->Wrap( -1 );
	fgSizer732->Add( m_staticText4622, 0, 0, 5 );
	
	m_staticText4621 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4621->Wrap( -1 );
	fgSizer732->Add( m_staticText4621, 0, 0, 5 );
	
	m_staticText448 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, _("Attack Type (Player)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText448->Wrap( -1 );
	m_staticText448->SetToolTip( _("Player stats are used when the player finds\ncards on the field or as a battle drop") );
	
	fgSizer732->Add( m_staticText448, 0, wxALL, 5 );
	
	wxString m_cardplayertypeChoices[] = { _("P"), _("M"), _("X"), _("A") };
	int m_cardplayertypeNChoices = sizeof( m_cardplayertypeChoices ) / sizeof( wxString );
	m_cardplayertype = new wxChoice( m_cardstatpsxpanel, wxID_TYPEPLY, wxDefaultPosition, wxDefaultSize, m_cardplayertypeNChoices, m_cardplayertypeChoices, 0 );
	m_cardplayertype->SetSelection( 0 );
	fgSizer732->Add( m_cardplayertype, 0, wxALL, 2 );
	
	m_staticText4521 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, _("Min Stats (Player)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4521->Wrap( -1 );
	m_staticText4521->SetToolTip( _("Player stats are used when the player finds\ncards on the field or as a battle drop") );
	
	fgSizer732->Add( m_staticText4521, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer1522;
	bSizer1522 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cardplayerattack = new wxSpinCtrl( m_cardstatpsxpanel, wxID_ATTPLY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardplayerattack->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer1522->Add( m_cardplayerattack, 0, wxALL, 2 );
	
	m_staticText4511 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4511->Wrap( -1 );
	bSizer1522->Add( m_staticText4511, 0, wxLEFT, 20 );
	
	m_cardplayerdefence = new wxSpinCtrl( m_cardstatpsxpanel, wxID_DEFPLY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardplayerdefence->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer1522->Add( m_cardplayerdefence, 0, wxALL, 2 );
	
	m_cardplayermagicdefence = new wxSpinCtrl( m_cardstatpsxpanel, wxID_MDEFPLY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardplayermagicdefence->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer1522->Add( m_cardplayermagicdefence, 0, wxALL, 2 );
	
	m_cardplayerpowerlabel = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,30 ), 0 );
	m_cardplayerpowerlabel->Wrap( -1 );
	bSizer1522->Add( m_cardplayerpowerlabel, 0, wxLEFT, 5 );
	
	
	fgSizer732->Add( bSizer1522, 1, wxEXPAND, 5 );
	
	m_staticText462 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText462->Wrap( -1 );
	fgSizer732->Add( m_staticText462, 0, 0, 5 );
	
	m_staticText463 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText463->Wrap( -1 );
	fgSizer732->Add( m_staticText463, 0, 0, 5 );
	
	m_staticText4482 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, _("Attack Type (NPC)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4482->Wrap( -1 );
	m_staticText4482->SetToolTip( _("NPC stats are for cards used by\nNon-Playable Characters") );
	
	fgSizer732->Add( m_staticText4482, 0, wxALL, 5 );
	
	wxString m_cardnpctypeChoices[] = { _("P"), _("M"), _("X"), _("A") };
	int m_cardnpctypeNChoices = sizeof( m_cardnpctypeChoices ) / sizeof( wxString );
	m_cardnpctype = new wxChoice( m_cardstatpsxpanel, wxID_TYPENPC, wxDefaultPosition, wxDefaultSize, m_cardnpctypeNChoices, m_cardnpctypeChoices, 0 );
	m_cardnpctype->SetSelection( 0 );
	fgSizer732->Add( m_cardnpctype, 0, wxALL, 2 );
	
	m_staticText452 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, _("Min Stats (NPC)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText452->Wrap( -1 );
	m_staticText452->SetToolTip( _("NPC stats are for cards used by\nNon-Playable Characters") );
	
	fgSizer732->Add( m_staticText452, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer152;
	bSizer152 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cardnpcattack = new wxSpinCtrl( m_cardstatpsxpanel, wxID_ATTNPC, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardnpcattack->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer152->Add( m_cardnpcattack, 0, wxALL, 2 );
	
	m_staticText451 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText451->Wrap( -1 );
	bSizer152->Add( m_staticText451, 0, wxLEFT, 20 );
	
	m_cardnpcdefence = new wxSpinCtrl( m_cardstatpsxpanel, wxID_DEFNPC, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardnpcdefence->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer152->Add( m_cardnpcdefence, 0, wxALL, 2 );
	
	m_cardnpcmagicdefence = new wxSpinCtrl( m_cardstatpsxpanel, wxID_MDEFNPC, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardnpcmagicdefence->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer152->Add( m_cardnpcmagicdefence, 0, wxALL, 2 );
	
	m_cardnpcpowerlabel = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,30 ), 0 );
	m_cardnpcpowerlabel->Wrap( -1 );
	bSizer152->Add( m_cardnpcpowerlabel, 0, wxLEFT, 5 );
	
	
	fgSizer732->Add( bSizer152, 1, wxEXPAND, 5 );
	
	m_staticText464 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText464->Wrap( -1 );
	fgSizer732->Add( m_staticText464, 0, 0, 5 );
	
	m_staticText465 = new wxStaticText( m_cardstatpsxpanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText465->Wrap( -1 );
	fgSizer732->Add( m_staticText465, 0, 0, 5 );
	
	
	m_cardstatpsxpanel->SetSizer( fgSizer732 );
	m_cardstatpsxpanel->Layout();
	fgSizer732->Fit( m_cardstatpsxpanel );
	bSizer187->Add( m_cardstatpsxpanel, 1, wxEXPAND, 5 );
	
	m_cardstatsteampanel = new wxPanel( m_cardscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_cardstatsteampanel->Hide();
	
	wxFlexGridSizer* fgSizer7321;
	fgSizer7321 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7321->SetFlexibleDirection( wxBOTH );
	fgSizer7321->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4483 = new wxStaticText( m_cardstatsteampanel, wxID_ANY, _("Attack Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4483->Wrap( -1 );
	fgSizer7321->Add( m_staticText4483, 0, wxALL, 5 );
	
	wxString m_cardtypeChoices[] = { _("P"), _("M"), _("X"), _("A") };
	int m_cardtypeNChoices = sizeof( m_cardtypeChoices ) / sizeof( wxString );
	m_cardtype = new wxChoice( m_cardstatsteampanel, wxID_TYPEPLY, wxDefaultPosition, wxDefaultSize, m_cardtypeNChoices, m_cardtypeChoices, 0 );
	m_cardtype->SetSelection( 0 );
	fgSizer7321->Add( m_cardtype, 0, wxALL, 2 );
	
	m_staticText45211 = new wxStaticText( m_cardstatsteampanel, wxID_ANY, _("Min Stats"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45211->Wrap( -1 );
	fgSizer7321->Add( m_staticText45211, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer15221;
	bSizer15221 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cardattack = new wxSpinCtrl( m_cardstatsteampanel, wxID_ATTPLY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardattack->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer15221->Add( m_cardattack, 0, wxALL, 2 );
	
	m_staticText45111 = new wxStaticText( m_cardstatsteampanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45111->Wrap( -1 );
	bSizer15221->Add( m_staticText45111, 0, wxLEFT, 20 );
	
	m_carddefence = new wxSpinCtrl( m_cardstatsteampanel, wxID_DEFPLY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_carddefence->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer15221->Add( m_carddefence, 0, wxALL, 2 );
	
	m_cardmagicdefence = new wxSpinCtrl( m_cardstatsteampanel, wxID_MDEFPLY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	m_cardmagicdefence->SetToolTip( _("Stats are chosen randomly between\nMinStat and 2*MinStat - 1\n\nEvery 16 stat points give +1 to the actual power") );
	
	bSizer15221->Add( m_cardmagicdefence, 0, wxALL, 2 );
	
	m_cardpowerlabel = new wxStaticText( m_cardstatsteampanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,30 ), 0 );
	m_cardpowerlabel->Wrap( -1 );
	bSizer15221->Add( m_cardpowerlabel, 0, wxLEFT, 5 );
	
	
	fgSizer7321->Add( bSizer15221, 1, wxEXPAND, 5 );
	
	
	m_cardstatsteampanel->SetSizer( fgSizer7321 );
	m_cardstatsteampanel->Layout();
	fgSizer7321->Fit( m_cardstatsteampanel );
	bSizer187->Add( m_cardstatsteampanel, 1, wxEXPAND, 5 );
	
	
	gbSizer38->Add( bSizer187, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	
	gbSizer38->AddGrowableCol( 1 );
	
	m_cardscrolledwindow->SetSizer( gbSizer38 );
	m_cardscrolledwindow->Layout();
	gbSizer38->Fit( m_cardscrolledwindow );
	bSizer150->Add( m_cardscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel63->SetSizer( bSizer150 );
	m_panel63->Layout();
	bSizer150->Fit( m_panel63 );
	m_notebookcard->AddPage( m_panel63, _("Cards"), false );
	m_panel64 = new wxPanel( m_notebookcard, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer151;
	bSizer151 = new wxBoxSizer( wxHORIZONTAL );
	
	m_carddecklist = new wxListBox( m_panel64, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer151->Add( m_carddecklist, 0, wxALL|wxEXPAND, 5 );
	
	m_carddeckscrolledwindow = new wxScrolledWindow( m_panel64, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_carddeckscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer691;
	fgSizer691 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer691->SetFlexibleDirection( wxBOTH );
	fgSizer691->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText466 = new wxStaticText( m_carddeckscrolledwindow, wxID_ANY, _("Difficulty"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText466->Wrap( -1 );
	fgSizer691->Add( m_staticText466, 0, wxALL, 5 );
	
	m_carddeckdifficulty = new wxSpinCtrl( m_carddeckscrolledwindow, wxID_DIFFICULTY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	m_carddeckdifficulty->SetToolTip( _("May be the type of strategy used by the AI\nRanges from 0 to 3 by default") );
	
	fgSizer691->Add( m_carddeckdifficulty, 0, wxALL, 2 );
	
	m_staticText4481 = new wxStaticText( m_carddeckscrolledwindow, wxID_ANY, _("Card Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4481->Wrap( -1 );
	fgSizer691->Add( m_staticText4481, 0, wxALL, 5 );
	
	wxGridBagSizer* gbSizer39;
	gbSizer39 = new wxGridBagSizer( 0, 0 );
	gbSizer39->SetFlexibleDirection( wxBOTH );
	gbSizer39->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_carddecksetChoices;
	m_carddeckset = new wxChoice( m_carddeckscrolledwindow, wxID_DECKSET, wxDefaultPosition, wxDefaultSize, m_carddecksetChoices, 0 );
	m_carddeckset->SetSelection( 0 );
	gbSizer39->Add( m_carddeckset, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_carddecksetbutton = new wxButton( m_carddeckscrolledwindow, wxID_DECKSET, _("Edit Set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_carddecksetbutton->SetToolTip( _("Warning\nChanging the card set will modify it\nfor the other decks sharing the same set") );
	
	gbSizer39->Add( m_carddecksetbutton, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxBoxSizer* bSizer158;
	bSizer158 = new wxBoxSizer( wxHORIZONTAL );
	
	m_carddecksetinfo1 = new wxStaticText( m_carddeckscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 130,-1 ), 0 );
	m_carddecksetinfo1->Wrap( -1 );
	bSizer158->Add( m_carddecksetinfo1, 0, wxALL, 5 );
	
	m_carddecksetinfo2 = new wxStaticText( m_carddeckscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 130,-1 ), 0 );
	m_carddecksetinfo2->Wrap( -1 );
	bSizer158->Add( m_carddecksetinfo2, 0, wxALL, 5 );
	
	
	gbSizer39->Add( bSizer158, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	
	fgSizer691->Add( gbSizer39, 1, wxEXPAND, 5 );
	
	
	m_carddeckscrolledwindow->SetSizer( fgSizer691 );
	m_carddeckscrolledwindow->Layout();
	fgSizer691->Fit( m_carddeckscrolledwindow );
	bSizer151->Add( m_carddeckscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel64->SetSizer( bSizer151 );
	m_panel64->Layout();
	bSizer151->Fit( m_panel64 );
	m_notebookcard->AddPage( m_panel64, _("Decks"), false );
	
	bSizer101->Add( m_notebookcard, 1, wxEXPAND | wxALL, 5 );
	
	
	m_paneltetramaster->SetSizer( bSizer101 );
	m_paneltetramaster->Layout();
	bSizer101->Fit( m_paneltetramaster );
	m_notebookmain->AddPage( m_paneltetramaster, _("Tetra Master"), false );
	m_panelenvironment = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookenvironment = new wxNotebook( m_panelenvironment, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel131 = new wxPanel( m_notebookenvironment, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textlist = new wxListBox( m_panel131, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer17->Add( m_textlist, 0, wxALL|wxEXPAND, 5 );
	
	m_textscrolledwindow = new wxScrolledWindow( m_panel131, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_textscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_textdatalistlabel = new wxStaticText( m_textscrolledwindow, wxID_ANY, _("Texts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textdatalistlabel->Wrap( -1 );
	fgSizer14->Add( m_textdatalistlabel, 0, wxALL, 5 );
	
	m_textdatalist = new wxListBox( m_textscrolledwindow, wxID_TEXT, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer14->Add( m_textdatalist, 0, wxALL|wxEXPAND, 2 );
	
	m_textexportlabel = new wxStaticText( m_textscrolledwindow, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textexportlabel->Wrap( -1 );
	fgSizer14->Add( m_textexportlabel, 0, wxALL, 5 );
	
	m_textexport = new wxButton( m_textscrolledwindow, wxID_TEXT, _("Export Text Block"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textexport->SetToolTip( _("If you ever want to import a block of text back,\nconsider using File Batching instead") );
	
	fgSizer14->Add( m_textexport, 0, wxALL, 2 );
	
	m_textcharmaptexturelistlabel = new wxStaticText( m_textscrolledwindow, wxID_ANY, _("Charmap"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textcharmaptexturelistlabel->Wrap( -1 );
	fgSizer14->Add( m_textcharmaptexturelistlabel, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer211;
	fgSizer211 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer211->SetFlexibleDirection( wxBOTH );
	fgSizer211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_textmanagecharmapbutton = new wxButton( m_textscrolledwindow, wxID_TEXT, _("Manage Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer211->Add( m_textmanagecharmapbutton, 0, wxALIGN_CENTER|wxALL, 3 );
	
	m_textcharmappallabel = new wxStaticText( m_textscrolledwindow, wxID_ANY, _("Palette"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textcharmappallabel->Wrap( -1 );
	fgSizer211->Add( m_textcharmappallabel, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_textcharmaplist = new wxListBox( m_textscrolledwindow, wxID_TEXT, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer211->Add( m_textcharmaplist, 0, wxALL, 3 );
	
	wxArrayString m_textcharmappalchoiceChoices;
	m_textcharmappalchoice = new wxChoice( m_textscrolledwindow, wxID_TEXT, wxDefaultPosition, wxDefaultSize, m_textcharmappalchoiceChoices, 0 );
	m_textcharmappalchoice->SetSelection( 0 );
	fgSizer211->Add( m_textcharmappalchoice, 0, wxALL, 5 );
	
	
	fgSizer14->Add( fgSizer211, 1, wxEXPAND, 5 );
	
	m_textcharmapexportlabel = new wxStaticText( m_textscrolledwindow, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textcharmapexportlabel->Wrap( -1 );
	fgSizer14->Add( m_textcharmapexportlabel, 0, wxALL, 5 );
	
	m_textcharmapexport = new wxButton( m_textscrolledwindow, wxID_TEXT, _("Export Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer14->Add( m_textcharmapexport, 0, wxALL, 2 );
	
	m_textcharmapwindowlabel = new wxStaticText( m_textscrolledwindow, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textcharmapwindowlabel->Wrap( -1 );
	fgSizer14->Add( m_textcharmapwindowlabel, 0, wxALL, 5 );
	
	m_textcharmapwindow = new wxScrolledWindow( m_textscrolledwindow, wxID_TEXT, wxDefaultPosition, wxSize( 261,261 ), wxHSCROLL|wxSUNKEN_BORDER|wxVSCROLL );
	m_textcharmapwindow->SetScrollRate( 5, 5 );
	m_textcharmapwindow->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer14->Add( m_textcharmapwindow, 1, wxEXPAND | wxALL, 2 );
	
	
	m_textscrolledwindow->SetSizer( fgSizer14 );
	m_textscrolledwindow->Layout();
	fgSizer14->Fit( m_textscrolledwindow );
	bSizer17->Add( m_textscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel131->SetSizer( bSizer17 );
	m_panel131->Layout();
	bSizer17->Fit( m_panel131 );
	m_notebookenvironment->AddPage( m_panel131, _("Texts"), false );
	m_panel1311 = new wxPanel( m_notebookenvironment, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer172;
	bSizer172 = new wxBoxSizer( wxHORIZONTAL );
	
	m_notebookworld = new wxNotebook( m_panel1311, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel67 = new wxPanel( m_notebookworld, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1501;
	bSizer1501 = new wxBoxSizer( wxHORIZONTAL );
	
	m_worldlist = new wxListBox( m_panel67, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer1501->Add( m_worldlist, 0, wxALL|wxEXPAND, 5 );
	
	m_worldscrolledwindow = new wxScrolledWindow( m_panel67, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_worldscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer142;
	fgSizer142 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer142->SetFlexibleDirection( wxBOTH );
	fgSizer142->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText822 = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Script"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText822->Wrap( -1 );
	fgSizer142->Add( m_staticText822, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer189;
	bSizer189 = new wxBoxSizer( wxHORIZONTAL );
	
	m_worldscriptedit = new wxButton( m_worldscrolledwindow, wxID_SCRIPT, _("Edit Script"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer189->Add( m_worldscriptedit, 0, wxALL, 3 );
	
	m_worldscriptentryedit = new wxButton( m_worldscrolledwindow, wxID_ENTRY, _("Edit Entries"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer189->Add( m_worldscriptentryedit, 0, wxALL, 3 );
	
	
	fgSizer142->Add( bSizer189, 1, wxEXPAND, 5 );
	
	m_staticText217 = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Preloading"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText217->Wrap( -1 );
	fgSizer142->Add( m_staticText217, 0, wxALL, 5 );
	
	m_worldpreload = new wxButton( m_worldscrolledwindow, wxID_PRELOAD, _("Change"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_worldpreload, 0, wxALL, 3 );
	
	m_staticText1921 = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Texts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1921->Wrap( -1 );
	fgSizer142->Add( m_staticText1921, 0, wxALL, 5 );
	
	m_worldtextlist = new wxListBox( m_worldscrolledwindow, wxID_WORLD, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer142->Add( m_worldtextlist, 0, wxALL|wxEXPAND, 2 );
	
	m_staticText831 = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText831->Wrap( -1 );
	fgSizer142->Add( m_staticText831, 0, wxALL, 5 );
	
	m_worldtextexport = new wxButton( m_worldscrolledwindow, wxID_WORLD, _("Export Text Block"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_worldtextexport, 0, wxALL, 2 );
	
	m_worldtextcharmaptexturelistlabel = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Charmap"), wxDefaultPosition, wxDefaultSize, 0 );
	m_worldtextcharmaptexturelistlabel->Wrap( -1 );
	fgSizer142->Add( m_worldtextcharmaptexturelistlabel, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer2111;
	fgSizer2111 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2111->SetFlexibleDirection( wxBOTH );
	fgSizer2111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_worldtextmanagecharmapbutton = new wxButton( m_worldscrolledwindow, wxID_WORLD, _("Manage Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2111->Add( m_worldtextmanagecharmapbutton, 0, wxALIGN_CENTER|wxALL, 3 );
	
	m_worldtextcharmappallabel = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Palette"), wxDefaultPosition, wxDefaultSize, 0 );
	m_worldtextcharmappallabel->Wrap( -1 );
	fgSizer2111->Add( m_worldtextcharmappallabel, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_worldtextcharmaplist = new wxListBox( m_worldscrolledwindow, wxID_WORLD, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer2111->Add( m_worldtextcharmaplist, 0, wxALL, 3 );
	
	wxArrayString m_worldtextcharmappalchoiceChoices;
	m_worldtextcharmappalchoice = new wxChoice( m_worldscrolledwindow, wxID_WORLD, wxDefaultPosition, wxDefaultSize, m_worldtextcharmappalchoiceChoices, 0 );
	m_worldtextcharmappalchoice->SetSelection( 0 );
	fgSizer2111->Add( m_worldtextcharmappalchoice, 0, wxALL, 5 );
	
	
	fgSizer142->Add( fgSizer2111, 1, wxEXPAND, 5 );
	
	m_worldtextcharmapexportlabel = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	m_worldtextcharmapexportlabel->Wrap( -1 );
	fgSizer142->Add( m_worldtextcharmapexportlabel, 0, wxALL, 5 );
	
	m_worldtextcharmapexport = new wxButton( m_worldscrolledwindow, wxID_WORLD, _("Export Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer142->Add( m_worldtextcharmapexport, 0, wxALL, 2 );
	
	m_worldtextcharmapwindowlabel = new wxStaticText( m_worldscrolledwindow, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_worldtextcharmapwindowlabel->Wrap( -1 );
	fgSizer142->Add( m_worldtextcharmapwindowlabel, 0, wxALL, 5 );
	
	m_worldtextcharmapwindow = new wxScrolledWindow( m_worldscrolledwindow, wxID_WORLD, wxDefaultPosition, wxSize( 261,261 ), wxHSCROLL|wxSUNKEN_BORDER|wxVSCROLL );
	m_worldtextcharmapwindow->SetScrollRate( 5, 5 );
	m_worldtextcharmapwindow->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	fgSizer142->Add( m_worldtextcharmapwindow, 1, wxEXPAND | wxALL, 2 );
	
	
	m_worldscrolledwindow->SetSizer( fgSizer142 );
	m_worldscrolledwindow->Layout();
	fgSizer142->Fit( m_worldscrolledwindow );
	bSizer1501->Add( m_worldscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel67->SetSizer( bSizer1501 );
	m_panel67->Layout();
	bSizer1501->Fit( m_panel67 );
	m_notebookworld->AddPage( m_panel67, _("Global Map"), false );
	m_panel681 = new wxPanel( m_notebookworld, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1511;
	bSizer1511 = new wxBoxSizer( wxHORIZONTAL );
	
	m_worldplacelist = new wxListBox( m_panel681, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer1511->Add( m_worldplacelist, 0, wxALL|wxEXPAND, 5 );
	
	m_worldplacescrolledwindow = new wxScrolledWindow( m_panel681, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_worldplacescrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer1421;
	fgSizer1421 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1421->SetFlexibleDirection( wxBOTH );
	fgSizer1421->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText8221 = new wxStaticText( m_worldplacescrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8221->Wrap( -1 );
	fgSizer1421->Add( m_staticText8221, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer221221;
	bSizer221221 = new wxBoxSizer( wxHORIZONTAL );
	
	m_worldplacename = new wxTextCtrl( m_worldplacescrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer221221->Add( m_worldplacename, 0, wxALL, 2 );
	
	m_worldplacenamebutton = new wxButton( m_worldplacescrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_worldplacenamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer221221->Add( m_worldplacenamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer1421->Add( bSizer221221, 1, wxEXPAND, 5 );
	
	
	m_worldplacescrolledwindow->SetSizer( fgSizer1421 );
	m_worldplacescrolledwindow->Layout();
	fgSizer1421->Fit( m_worldplacescrolledwindow );
	bSizer1511->Add( m_worldplacescrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel681->SetSizer( bSizer1511 );
	m_panel681->Layout();
	bSizer1511->Fit( m_panel681 );
	m_notebookworld->AddPage( m_panel681, _("Places"), false );
	m_panel69 = new wxPanel( m_notebookworld, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1521;
	bSizer1521 = new wxBoxSizer( wxHORIZONTAL );
	
	m_worldbattlelist = new wxListBox( m_panel69, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 ); 
	bSizer1521->Add( m_worldbattlelist, 0, wxALL|wxEXPAND, 5 );
	
	m_worldbattlescrolledwindow = new wxScrolledWindow( m_panel69, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_worldbattlescrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer14211;
	fgSizer14211 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer14211->AddGrowableCol( 1 );
	fgSizer14211->SetFlexibleDirection( wxBOTH );
	fgSizer14211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText82211 = new wxStaticText( m_worldbattlescrolledwindow, wxID_ANY, _("Battles"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText82211->Wrap( -1 );
	fgSizer14211->Add( m_staticText82211, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer68;
	fgSizer68 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer68->AddGrowableCol( 2 );
	fgSizer68->SetFlexibleDirection( wxBOTH );
	fgSizer68->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_worldbattlepanelspin = new wxPanel( m_worldbattlescrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer157;
	bSizer157 = new wxBoxSizer( wxVERTICAL );
	
	m_worldbattlebattlespin1 = new wxSpinCtrl( m_worldbattlepanelspin, wxID_BATTLE1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer157->Add( m_worldbattlebattlespin1, 0, wxALL, 2 );
	
	m_worldbattlebattlespin2 = new wxSpinCtrl( m_worldbattlepanelspin, wxID_BATTLE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer157->Add( m_worldbattlebattlespin2, 0, wxALL, 2 );
	
	m_worldbattlebattlespin3 = new wxSpinCtrl( m_worldbattlepanelspin, wxID_BATTLE3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer157->Add( m_worldbattlebattlespin3, 0, wxALL, 2 );
	
	m_worldbattlebattlespin4 = new wxSpinCtrl( m_worldbattlepanelspin, wxID_BATTLE4, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	bSizer157->Add( m_worldbattlebattlespin4, 0, wxALL, 2 );
	
	
	m_worldbattlepanelspin->SetSizer( bSizer157 );
	m_worldbattlepanelspin->Layout();
	bSizer157->Fit( m_worldbattlepanelspin );
	fgSizer68->Add( m_worldbattlepanelspin, 1, 0, 5 );
	
	m_worldbattlepanelchoice = new wxPanel( m_worldbattlescrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_worldbattlepanelchoice->Hide();
	
	wxBoxSizer* bSizer1571;
	bSizer1571 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_worldbattlebattlechoice1Choices;
	m_worldbattlebattlechoice1 = new wxChoice( m_worldbattlepanelchoice, wxID_BATTLE1, wxDefaultPosition, wxDefaultSize, m_worldbattlebattlechoice1Choices, 0 );
	m_worldbattlebattlechoice1->SetSelection( 0 );
	bSizer1571->Add( m_worldbattlebattlechoice1, 0, wxALL, 2 );
	
	wxArrayString m_worldbattlebattlechoice2Choices;
	m_worldbattlebattlechoice2 = new wxChoice( m_worldbattlepanelchoice, wxID_BATTLE2, wxDefaultPosition, wxDefaultSize, m_worldbattlebattlechoice2Choices, 0 );
	m_worldbattlebattlechoice2->SetSelection( 0 );
	bSizer1571->Add( m_worldbattlebattlechoice2, 0, wxALL, 2 );
	
	wxArrayString m_worldbattlebattlechoice3Choices;
	m_worldbattlebattlechoice3 = new wxChoice( m_worldbattlepanelchoice, wxID_BATTLE3, wxDefaultPosition, wxDefaultSize, m_worldbattlebattlechoice3Choices, 0 );
	m_worldbattlebattlechoice3->SetSelection( 0 );
	bSizer1571->Add( m_worldbattlebattlechoice3, 0, wxALL, 2 );
	
	wxArrayString m_worldbattlebattlechoice4Choices;
	m_worldbattlebattlechoice4 = new wxChoice( m_worldbattlepanelchoice, wxID_BATTLE4, wxDefaultPosition, wxDefaultSize, m_worldbattlebattlechoice4Choices, 0 );
	m_worldbattlebattlechoice4->SetSelection( 0 );
	bSizer1571->Add( m_worldbattlebattlechoice4, 0, wxALL, 2 );
	
	
	m_worldbattlepanelchoice->SetSizer( bSizer1571 );
	m_worldbattlepanelchoice->Layout();
	bSizer1571->Fit( m_worldbattlepanelchoice );
	fgSizer68->Add( m_worldbattlepanelchoice, 1, 0, 5 );
	
	wxBoxSizer* bSizer155;
	bSizer155 = new wxBoxSizer( wxVERTICAL );
	
	m_worldbattlehelplabel1 = new wxStaticText( m_worldbattlescrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_worldbattlehelplabel1->Wrap( -1 );
	bSizer155->Add( m_worldbattlehelplabel1, 0, wxALL|wxEXPAND, 5 );
	
	m_worldbattlehelplabel2 = new wxStaticText( m_worldbattlescrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_worldbattlehelplabel2->Wrap( -1 );
	bSizer155->Add( m_worldbattlehelplabel2, 0, wxALL|wxEXPAND, 5 );
	
	m_worldbattlehelplabel3 = new wxStaticText( m_worldbattlescrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_worldbattlehelplabel3->Wrap( -1 );
	bSizer155->Add( m_worldbattlehelplabel3, 0, wxALL|wxEXPAND, 5 );
	
	m_worldbattlehelplabel4 = new wxStaticText( m_worldbattlescrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_worldbattlehelplabel4->Wrap( -1 );
	bSizer155->Add( m_worldbattlehelplabel4, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer68->Add( bSizer155, 1, wxEXPAND, 5 );
	
	
	fgSizer14211->Add( fgSizer68, 1, wxEXPAND, 5 );
	
	
	m_worldbattlescrolledwindow->SetSizer( fgSizer14211 );
	m_worldbattlescrolledwindow->Layout();
	fgSizer14211->Fit( m_worldbattlescrolledwindow );
	bSizer1521->Add( m_worldbattlescrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel69->SetSizer( bSizer1521 );
	m_panel69->Layout();
	bSizer1521->Fit( m_panel69 );
	m_notebookworld->AddPage( m_panel69, _("Battle Spots"), false );
	
	bSizer172->Add( m_notebookworld, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel1311->SetSizer( bSizer172 );
	m_panel1311->Layout();
	bSizer172->Fit( m_panel1311 );
	m_notebookenvironment->AddPage( m_panel1311, _("World Map"), false );
	m_panel41 = new wxPanel( m_notebookenvironment, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer108;
	bSizer108 = new wxBoxSizer( wxHORIZONTAL );
	
	m_fieldlist = new wxListBox( m_panel41, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer108->Add( m_fieldlist, 0, wxALL|wxEXPAND, 5 );
	
	m_fieldscrolledwindow = new wxScrolledWindow( m_panel41, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_fieldscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer39;
	fgSizer39 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer39->SetFlexibleDirection( wxBOTH );
	fgSizer39->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1901 = new wxStaticText( m_fieldscrolledwindow, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1901->Wrap( -1 );
	fgSizer39->Add( m_staticText1901, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer22122;
	bSizer22122 = new wxBoxSizer( wxHORIZONTAL );
	
	m_fieldname = new wxTextCtrl( m_fieldscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer22122->Add( m_fieldname, 0, wxALL, 2 );
	
	m_fieldnamebutton = new wxButton( m_fieldscrolledwindow, wxID_NAME, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_fieldnamebutton->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	bSizer22122->Add( m_fieldnamebutton, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer39->Add( bSizer22122, 1, wxEXPAND, 5 );
	
	m_staticText1911 = new wxStaticText( m_fieldscrolledwindow, wxID_ANY, _("Script"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1911->Wrap( -1 );
	fgSizer39->Add( m_staticText1911, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer190;
	bSizer190 = new wxBoxSizer( wxHORIZONTAL );
	
	m_fieldeditscript = new wxButton( m_fieldscrolledwindow, wxID_SCRIPT, _("Edit Script"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer190->Add( m_fieldeditscript, 0, wxALL, 3 );
	
	m_fieldscriptentryedit = new wxButton( m_fieldscrolledwindow, wxID_ENTRY, _("Edit Entries"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer190->Add( m_fieldscriptentryedit, 0, wxALL, 3 );
	
	
	fgSizer39->Add( bSizer190, 1, wxEXPAND, 5 );
	
	m_staticText218 = new wxStaticText( m_fieldscrolledwindow, wxID_ANY, _("Preloading"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText218->Wrap( -1 );
	fgSizer39->Add( m_staticText218, 0, wxALL, 5 );
	
	m_fieldpreload = new wxButton( m_fieldscrolledwindow, wxID_PRELOAD, _("Change"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer39->Add( m_fieldpreload, 0, wxALL, 3 );
	
	m_staticText211 = new wxStaticText( m_fieldscrolledwindow, wxID_ANY, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer39->Add( m_staticText211, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_fieldtexturechoiceChoices;
	m_fieldtexturechoice = new wxChoice( m_fieldscrolledwindow, wxID_TEXTURE, wxDefaultPosition, wxDefaultSize, m_fieldtexturechoiceChoices, 0 );
	m_fieldtexturechoice->SetSelection( 0 );
	bSizer124->Add( m_fieldtexturechoice, 0, wxALL, 3 );
	
	m_fieldtexturemanage = new wxButton( m_fieldscrolledwindow, wxID_TEXTURE, _("Manage"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_fieldtexturemanage, 0, wxALL, 2 );
	
	
	fgSizer39->Add( bSizer124, 1, wxEXPAND, 5 );
	
	m_staticText210 = new wxStaticText( m_fieldscrolledwindow, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText210->Wrap( -1 );
	fgSizer39->Add( m_staticText210, 0, wxALL, 5 );
	
	m_fieldtexturepreview = new wxScrolledWindow( m_fieldscrolledwindow, wxID_ANY, wxDefaultPosition, wxSize( 518,518 ), wxHSCROLL|wxSUNKEN_BORDER|wxVSCROLL );
	m_fieldtexturepreview->SetScrollRate( 5, 5 );
	fgSizer39->Add( m_fieldtexturepreview, 1, wxEXPAND | wxALL, 5 );
	
	
	m_fieldscrolledwindow->SetSizer( fgSizer39 );
	m_fieldscrolledwindow->Layout();
	fgSizer39->Fit( m_fieldscrolledwindow );
	bSizer108->Add( m_fieldscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel41->SetSizer( bSizer108 );
	m_panel41->Layout();
	bSizer108->Fit( m_panel41 );
	m_notebookenvironment->AddPage( m_panel41, _("Fields"), false );
	m_panel17 = new wxPanel( m_notebookenvironment, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxHORIZONTAL );
	
	m_battlescenelist = new wxListBox( m_panel17, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer171->Add( m_battlescenelist, 0, wxALL|wxEXPAND, 5 );
	
	m_battlescenescrolledwindow = new wxScrolledWindow( m_panel17, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_battlescenescrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer141;
	fgSizer141 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer141->SetFlexibleDirection( wxBOTH );
	fgSizer141->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText821 = new wxStaticText( m_battlescenescrolledwindow, wxID_ANY, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText821->Wrap( -1 );
	fgSizer141->Add( m_staticText821, 0, wxALL, 5 );
	
	m_battlesceneexport = new wxButton( m_battlescenescrolledwindow, wxID_ANY, _("Export Battle Scene"), wxDefaultPosition, wxDefaultSize, 0 );
	m_battlesceneexport->SetToolTip( _("Export Battle Scene model as .obj file") );
	
	fgSizer141->Add( m_battlesceneexport, 0, wxALL, 2 );
	
	m_staticText94 = new wxStaticText( m_battlescenescrolledwindow, wxID_ANY, _("Import Geometry"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText94->Wrap( -1 );
	fgSizer141->Add( m_staticText94, 0, wxALL, 5 );
	
	m_battlesceneimport = new wxButton( m_battlescenescrolledwindow, wxID_ANY, _("Import Battle Scene"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer141->Add( m_battlesceneimport, 0, wxALL, 2 );
	
	m_staticText93 = new wxStaticText( m_battlescenescrolledwindow, wxID_ANY, _("Textures"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText93->Wrap( -1 );
	fgSizer141->Add( m_staticText93, 0, wxALL, 5 );
	
	m_battlesceneobjectpanel = new wxPanel( m_battlescenescrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer311;
	fgSizer311 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer311->SetFlexibleDirection( wxBOTH );
	fgSizer311->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_battlescenemanagetexture = new wxButton( m_battlesceneobjectpanel, wxID_ANY, _("Manage Textures"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer311->Add( m_battlescenemanagetexture, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticText981 = new wxStaticText( m_battlesceneobjectpanel, wxID_ANY, _("Association"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText981->Wrap( -1 );
	fgSizer311->Add( m_staticText981, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_battlescenetexturelist = new wxListBox( m_battlesceneobjectpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer311->Add( m_battlescenetexturelist, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer60;
	bSizer60 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_battlescenetexturechoiceChoices;
	m_battlescenetexturechoice = new wxChoice( m_battlesceneobjectpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_battlescenetexturechoiceChoices, 0 );
	m_battlescenetexturechoice->SetSelection( 0 );
	bSizer60->Add( m_battlescenetexturechoice, 0, wxALL, 5 );
	
	wxArrayString m_battlescenepalettechoiceChoices;
	m_battlescenepalettechoice = new wxChoice( m_battlesceneobjectpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_battlescenepalettechoiceChoices, 0 );
	m_battlescenepalettechoice->SetSelection( 0 );
	bSizer60->Add( m_battlescenepalettechoice, 0, wxALL, 5 );
	
	
	fgSizer311->Add( bSizer60, 1, wxEXPAND, 5 );
	
	
	m_battlesceneobjectpanel->SetSizer( fgSizer311 );
	m_battlesceneobjectpanel->Layout();
	fgSizer311->Fit( m_battlesceneobjectpanel );
	fgSizer141->Add( m_battlesceneobjectpanel, 1, wxEXPAND | wxALL, 2 );
	
	m_staticText95 = new wxStaticText( m_battlescenescrolledwindow, wxID_ANY, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText95->Wrap( -1 );
	fgSizer141->Add( m_staticText95, 0, wxALL, 5 );
	
	m_battlescenetexture = new wxScrolledWindow( m_battlescenescrolledwindow, wxID_ANY, wxDefaultPosition, wxSize( 261,261 ), wxHSCROLL|wxSUNKEN_BORDER|wxVSCROLL );
	m_battlescenetexture->SetScrollRate( 5, 5 );
	fgSizer141->Add( m_battlescenetexture, 1, wxEXPAND | wxALL, 5 );
	
	
	m_battlescenescrolledwindow->SetSizer( fgSizer141 );
	m_battlescenescrolledwindow->Layout();
	fgSizer141->Fit( m_battlescenescrolledwindow );
	bSizer171->Add( m_battlescenescrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel17->SetSizer( bSizer171 );
	m_panel17->Layout();
	bSizer171->Fit( m_panel17 );
	m_notebookenvironment->AddPage( m_panel17, _("Battle Scenes"), false );
	m_panel171 = new wxPanel( m_notebookenvironment, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1711;
	bSizer1711 = new wxBoxSizer( wxHORIZONTAL );
	
	m_spellanimlist = new wxListBox( m_panel171, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer1711->Add( m_spellanimlist, 0, wxALL|wxEXPAND, 5 );
	
	m_spellanimscrolledwindow = new wxScrolledWindow( m_panel171, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_spellanimscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer1411;
	fgSizer1411 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer1411->SetFlexibleDirection( wxBOTH );
	fgSizer1411->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText8211 = new wxStaticText( m_spellanimscrolledwindow, wxID_ANY, _("Sequence"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8211->Wrap( -1 );
	fgSizer1411->Add( m_staticText8211, 0, wxALL, 5 );
	
	m_spellanimsequence = new wxButton( m_spellanimscrolledwindow, wxID_SEQUENCE, _("Edit Spell Sequencing"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1411->Add( m_spellanimsequence, 0, wxALL, 2 );
	
	
	m_spellanimscrolledwindow->SetSizer( fgSizer1411 );
	m_spellanimscrolledwindow->Layout();
	fgSizer1411->Fit( m_spellanimscrolledwindow );
	bSizer1711->Add( m_spellanimscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel171->SetSizer( bSizer1711 );
	m_panel171->Layout();
	bSizer1711->Fit( m_panel171 );
	m_notebookenvironment->AddPage( m_panel171, _("Spell Animations"), false );
	
	bSizer9->Add( m_notebookenvironment, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelenvironment->SetSizer( bSizer9 );
	m_panelenvironment->Layout();
	bSizer9->Fit( m_panelenvironment );
	m_notebookmain->AddPage( m_panelenvironment, _("Environment"), false );
	m_panelffui = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer159;
	bSizer159 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookinterface = new wxNotebook( m_panelffui, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel66 = new wxPanel( m_notebookinterface, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer160;
	bSizer160 = new wxBoxSizer( wxHORIZONTAL );
	
	m_specialtextlist = new wxListBox( m_panel66, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer160->Add( m_specialtextlist, 0, wxALL|wxEXPAND, 5 );
	
	m_specialtextscrolledwindow = new wxScrolledWindow( m_panel66, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_specialtextscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer73;
	fgSizer73 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer73->SetFlexibleDirection( wxBOTH );
	fgSizer73->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText472 = new wxStaticText( m_specialtextscrolledwindow, wxID_ANY, _("Texts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText472->Wrap( -1 );
	fgSizer73->Add( m_staticText472, 0, wxALL, 5 );
	
	m_specialtextdatalist = new wxListBox( m_specialtextscrolledwindow, wxID_ANY, wxDefaultPosition, wxSize( 350,150 ), 0, NULL, 0 ); 
	fgSizer73->Add( m_specialtextdatalist, 0, wxALL|wxEXPAND, 2 );
	
	
	m_specialtextscrolledwindow->SetSizer( fgSizer73 );
	m_specialtextscrolledwindow->Layout();
	fgSizer73->Fit( m_specialtextscrolledwindow );
	bSizer160->Add( m_specialtextscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel66->SetSizer( bSizer160 );
	m_panel66->Layout();
	bSizer160->Fit( m_panel66 );
	m_notebookinterface->AddPage( m_panel66, _("UI Texts"), false );
	
	bSizer159->Add( m_notebookinterface, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelffui->SetSizer( bSizer159 );
	m_panelffui->Layout();
	bSizer159->Fit( m_panelffui );
	m_notebookmain->AddPage( m_panelffui, _("Interface"), false );
	m_panelmips = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1591;
	bSizer1591 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookmips = new wxNotebook( m_panelmips, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel661 = new wxPanel( m_notebookmips, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1601;
	bSizer1601 = new wxBoxSizer( wxHORIZONTAL );
	
	m_mipsbattlelist = new wxListBox( m_panel661, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer1601->Add( m_mipsbattlelist, 0, wxALL|wxEXPAND, 5 );
	
	m_mipsbattlegenscrolledwindow = new wxScrolledWindow( m_panel661, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_mipsbattlegenscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer7311;
	fgSizer7311 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7311->AddGrowableCol( 1 );
	fgSizer7311->SetFlexibleDirection( wxBOTH );
	fgSizer7311->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText47212 = new wxStaticText( m_mipsbattlegenscrolledwindow, wxID_ANY, _("RAM Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47212->Wrap( -1 );
	fgSizer7311->Add( m_staticText47212, 0, wxALL, 5 );
	
	m_mipsbattleramposgen = new wxTextCtrl( m_mipsbattlegenscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxSIMPLE_BORDER );
	m_mipsbattleramposgen->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer7311->Add( m_mipsbattleramposgen, 0, wxALL, 2 );
	
	m_staticText296 = new wxStaticText( m_mipsbattlegenscrolledwindow, wxID_ANY, _("Instruction Amount"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText296->Wrap( -1 );
	fgSizer7311->Add( m_staticText296, 0, wxALL, 5 );
	
	m_mipsbattleramlengthgen = new wxSpinCtrl( m_mipsbattlegenscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 20 );
	fgSizer7311->Add( m_mipsbattleramlengthgen, 0, wxALL, 2 );
	
	m_staticText472111 = new wxStaticText( m_mipsbattlegenscrolledwindow, wxID_ANY, _("MIPS Code"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText472111->Wrap( -1 );
	fgSizer7311->Add( m_staticText472111, 0, wxALL, 5 );
	
	m_mipsbattlecodegen = new wxButton( m_mipsbattlegenscrolledwindow, wxID_SCRIPT, _("Edit Code"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7311->Add( m_mipsbattlecodegen, 0, wxALL, 2 );
	
	
	m_mipsbattlegenscrolledwindow->SetSizer( fgSizer7311 );
	m_mipsbattlegenscrolledwindow->Layout();
	fgSizer7311->Fit( m_mipsbattlegenscrolledwindow );
	bSizer1601->Add( m_mipsbattlegenscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	m_mipsbattlescrolledwindow = new wxScrolledWindow( m_panel661, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_mipsbattlescrolledwindow->SetScrollRate( 5, 5 );
	m_mipsbattlescrolledwindow->Hide();
	
	wxFlexGridSizer* fgSizer731;
	fgSizer731 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer731->AddGrowableCol( 1 );
	fgSizer731->SetFlexibleDirection( wxBOTH );
	fgSizer731->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText4721 = new wxStaticText( m_mipsbattlescrolledwindow, wxID_ANY, _("RAM Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4721->Wrap( -1 );
	fgSizer731->Add( m_staticText4721, 0, wxALL, 5 );
	
	m_mipsbattlerampos = new wxTextCtrl( m_mipsbattlescrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxSIMPLE_BORDER );
	m_mipsbattlerampos->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	fgSizer731->Add( m_mipsbattlerampos, 0, wxALL, 2 );
	
	m_staticText47211 = new wxStaticText( m_mipsbattlescrolledwindow, wxID_ANY, _("MIPS Code"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47211->Wrap( -1 );
	fgSizer731->Add( m_staticText47211, 0, wxALL, 5 );
	
	m_mipsbattlecode = new wxButton( m_mipsbattlescrolledwindow, wxID_SCRIPT, _("Edit Code"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer731->Add( m_mipsbattlecode, 0, wxALL, 2 );
	
	
	m_mipsbattlescrolledwindow->SetSizer( fgSizer731 );
	m_mipsbattlescrolledwindow->Layout();
	fgSizer731->Fit( m_mipsbattlescrolledwindow );
	bSizer1601->Add( m_mipsbattlescrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel661->SetSizer( bSizer1601 );
	m_panel661->Layout();
	bSizer1601->Fit( m_panel661 );
	m_notebookmips->AddPage( m_panel661, _("Battle"), false );
	
	bSizer1591->Add( m_notebookmips, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelmips->SetSizer( bSizer1591 );
	m_panelmips->Layout();
	bSizer1591->Fit( m_panelmips );
	m_notebookmain->AddPage( m_panelmips, _("MIPS Code"), false );
	m_panelcil = new wxPanel( m_notebookmain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15912;
	bSizer15912 = new wxBoxSizer( wxVERTICAL );
	
	m_notebookcil = new wxNotebook( m_panelcil, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel6611 = new wxPanel( m_notebookcil, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer15911;
	bSizer15911 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ciltypelist = new wxListBox( m_panel6611, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer15911->Add( m_ciltypelist, 0, wxALL|wxEXPAND, 5 );
	
	m_cilmethodlist = new wxListBox( m_panel6611, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer15911->Add( m_cilmethodlist, 0, wxALL|wxEXPAND, 5 );
	
	m_cilmethodscrolledwindow = new wxScrolledWindow( m_panel6611, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_cilmethodscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer733;
	fgSizer733 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer733->SetFlexibleDirection( wxBOTH );
	fgSizer733->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText346 = new wxStaticText( m_cilmethodscrolledwindow, wxID_ANY, _("CIL Code"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText346->Wrap( -1 );
	fgSizer733->Add( m_staticText346, 0, wxALL, 5 );
	
	m_cilmethodedit = new wxButton( m_cilmethodscrolledwindow, wxID_SCRIPT, _("Edit Code"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer733->Add( m_cilmethodedit, 0, wxALL, 2 );
	
	
	m_cilmethodscrolledwindow->SetSizer( fgSizer733 );
	m_cilmethodscrolledwindow->Layout();
	fgSizer733->Fit( m_cilmethodscrolledwindow );
	bSizer15911->Add( m_cilmethodscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel6611->SetSizer( bSizer15911 );
	m_panel6611->Layout();
	bSizer15911->Fit( m_panel6611 );
	m_notebookcil->AddPage( m_panel6611, _("Raw Code"), false );
	m_panel92 = new wxPanel( m_notebookcil, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1591111;
	bSizer1591111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_cilmacrolist = new wxListBox( m_panel92, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, 0 ); 
	bSizer1591111->Add( m_cilmacrolist, 0, wxALL|wxEXPAND, 5 );
	
	m_cilmacroscrolledwindow = new wxScrolledWindow( m_panel92, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_cilmacroscrolledwindow->SetScrollRate( 5, 5 );
	wxFlexGridSizer* fgSizer7331;
	fgSizer7331 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7331->AddGrowableCol( 1 );
	fgSizer7331->SetFlexibleDirection( wxBOTH );
	fgSizer7331->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText347 = new wxStaticText( m_cilmacroscrolledwindow, wxID_ANY, _("Apply/Unapply"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText347->Wrap( -1 );
	fgSizer7331->Add( m_staticText347, 0, wxALL, 5 );
	
	m_cilmacrobutton = new wxButton( m_cilmacroscrolledwindow, wxID_SCRIPT, _("Apply Macro"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer7331->Add( m_cilmacrobutton, 0, wxALL, 2 );
	
	m_staticText345 = new wxStaticText( m_cilmacroscrolledwindow, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText345->Wrap( -1 );
	fgSizer7331->Add( m_staticText345, 0, wxALL, 5 );
	
	m_cilmacrodescription = new wxTextCtrl( m_cilmacroscrolledwindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxSIMPLE_BORDER );
	m_cilmacrodescription->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	m_cilmacrodescription->SetMinSize( wxSize( -1,100 ) );
	
	fgSizer7331->Add( m_cilmacrodescription, 0, wxALL|wxEXPAND, 2 );
	
	m_staticText348 = new wxStaticText( m_cilmacroscrolledwindow, wxID_ANY, _("Methods Involved"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText348->Wrap( -1 );
	fgSizer7331->Add( m_staticText348, 0, wxALL, 5 );
	
	m_cilmacromethods = new wxListBox( m_cilmacroscrolledwindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_cilmacromethods->SetMinSize( wxSize( -1,100 ) );
	
	fgSizer7331->Add( m_cilmacromethods, 0, wxALL|wxEXPAND, 2 );
	
	m_staticText327 = new wxStaticText( m_cilmacroscrolledwindow, wxID_ANY, _("Parameters"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText327->Wrap( -1 );
	fgSizer7331->Add( m_staticText327, 0, wxALL, 5 );
	
	m_cilmacroparametersizer = new wxBoxSizer( wxVERTICAL );
	
	
	fgSizer7331->Add( m_cilmacroparametersizer, 1, wxEXPAND, 5 );
	
	
	m_cilmacroscrolledwindow->SetSizer( fgSizer7331 );
	m_cilmacroscrolledwindow->Layout();
	fgSizer7331->Fit( m_cilmacroscrolledwindow );
	bSizer1591111->Add( m_cilmacroscrolledwindow, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel92->SetSizer( bSizer1591111 );
	m_panel92->Layout();
	bSizer1591111->Fit( m_panel92 );
	m_notebookcil->AddPage( m_panel92, _("Macros"), false );
	
	bSizer15912->Add( m_notebookcil, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panelcil->SetSizer( bSizer15912 );
	m_panelcil->Layout();
	bSizer15912->Fit( m_panelcil );
	m_notebookmain->AddPage( m_panelcil, _("CIL Code"), false );
	
	bSizer7->Add( m_notebookmain, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer7 );
	this->Layout();
	
	// Connect Events
	m_notebookmain->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookMain ), NULL, this );
	m_notebookparty->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookParty ), NULL, this );
	m_spelllist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSpell ), NULL, this );
	m_spellname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSpellChangeName ), NULL, this );
	m_spellnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeButton ), NULL, this );
	m_spellperformname->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellhelp->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSpellChangeHelp ), NULL, this );
	m_spellhelpbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeButton ), NULL, this );
	m_spelleffect->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellpower->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSpellChangeSpin ), NULL, this );
	m_spellelement1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellstatus->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellstatusset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeButton ), NULL, this );
	m_spellaccuracy->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSpellChangeSpin ), NULL, this );
	m_spellmp->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSpellChangeSpin ), NULL, this );
	m_spellmenuflagmenuuse->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellmenuflaghideap->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellmenuflagmpboost->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellmodeledit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnButtonClickSpellModel ), NULL, this );
	m_spellmodelaltedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnButtonClickSpellModelAlt ), NULL, this );
	m_spelltargettype->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spelltargetamount->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spelltargetpriority->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spelltargetflagdead->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spelltargetflagdeadfirst->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spelltargetflagcamera->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spelltargetpanel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellflag1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_supportlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSupport ), NULL, this );
	m_supportname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSupportChangeName ), NULL, this );
	m_supportnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSupportChangeButton ), NULL, this );
	m_supporthelp->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSupportChangeHelp ), NULL, this );
	m_supporthelpbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSupportChangeButton ), NULL, this );
	m_supportcost->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSupportChangeSpin ), NULL, this );
	m_cmdlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCommand ), NULL, this );
	m_cmdname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnCommandChangeName ), NULL, this );
	m_cmdnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdhelp->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnCommandChangeHelp ), NULL, this );
	m_cmdhelpbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdpanel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_cmdlink->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_cmdaddspell->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdremovespell->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdspellfulllist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdspelllist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdmovespellup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdmovespelldown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdobjectfirst->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_cmdobjectlast->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_statlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxStat ), NULL, this );
	m_statchardefaultname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnStatChangeDefaultName ), NULL, this );
	m_statchardefaultnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statcharspeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharstrength->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharmagic->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharspirit->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharmagicstone->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharabilityset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharabilitylist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statcharabilitychoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharabilitycost->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharcommand1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharcommand2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharcommandtrance1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharcommandtrance2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statchartranceattack->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharequipset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharweapon->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharhead->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharwrist->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statchararmor->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharaccessory->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statlvlexpmanage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statlvlexplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statlvlexpspin->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statlvlhpmanage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statlvlhplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statlvlhpspin->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statlvlmpmanage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statlvlmplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statlvlmpspin->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_partyspeciallist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxPartySpecial ), NULL, this );
	m_partyspecialmagicswordlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnPartySpecialChangeList ), NULL, this );
	m_partyspecialmagicswordspell->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnPartySpecialChangeChoice ), NULL, this );
	m_notebookinventory->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookInventory ), NULL, this );
	m_notebookitem->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookNone ), NULL, this );
	m_itemlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxItem ), NULL, this );
	m_itemname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnItemChangeName ), NULL, this );
	m_itemnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemhelp->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnItemChangeHelp ), NULL, this );
	m_itemhelpbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemhelp2->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnItemChangeBattleHelp ), NULL, this );
	m_itemhelp2button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemtype1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemprice->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemcharavailability16->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability15->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability14->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability13->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability12->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability11->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability10->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability9->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemstatid->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemstatidbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemskill1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemskill2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemskill3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemmenuposlist->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CDPanel::OnItemPositionListClick ), NULL, this );
	m_itemmenuposbuttonup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemmenuposbuttondown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemequipposlist->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CDPanel::OnItemPositionListClick ), NULL, this );
	m_itemequipposbuttonup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemequipposbuttondown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemicon->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemiconcolor->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemusableeffect->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusablepower->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemusablestatus1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus9->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus10->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus11->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus12->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus13->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus14->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus15->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus16->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus17->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus18->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus19->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus20->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus21->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus22->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus23->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus24->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus25->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus26->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus27->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus28->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus29->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus30->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus31->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus32->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablemodeledit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnButtonClickItemModel ), NULL, this );
	m_itemusabletargettype->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusabletargetamount->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusabletargetpriority->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusabletargetflagdead->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusabletargetflagdeadfirst->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusabletargetflagcamera->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusabletargetpanel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemweaponformula->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemweaponpower->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponelement1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponstatus->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemweaponaccuracy->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponflag1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponunk1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponunk2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponmodel->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmordefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmorevade->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmormagicdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmormagicevade->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_keyitemlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxKeyItem ), NULL, this );
	m_keyitemname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnKeyItemChangeName ), NULL, this );
	m_keyitemnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_keyitemhelp->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnKeyItemChangeHelp ), NULL, this );
	m_keyitemhelpbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_keyitemdescription->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnKeyItemChangeDescription ), NULL, this );
	m_keyitemdescriptionbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_notebookshop->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookNone ), NULL, this );
	m_shoplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxShop ), NULL, this );
	m_shopitemadd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopitemremove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopitemfulllist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopitemlist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopmoveitemup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopmoveitemdown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_synthshoplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSynthesisShop ), NULL, this );
	m_synthshopsynthitem->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnShopChangeChoice ), NULL, this );
	m_synthshoprecipe1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnShopChangeChoice ), NULL, this );
	m_synthshoprecipe2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnShopChangeChoice ), NULL, this );
	m_synthshopprice->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnShopChangeSpin ), NULL, this );
	m_synthshopsynthshop1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_enemylist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemy ), NULL, this );
	m_enemystatlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemyStat ), NULL, this );
	m_enemystatlist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemyStatRightClick ), NULL, this );
	m_enemyspelllist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemySpell ), NULL, this );
	m_enemyspelllist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemySpellRightClick ), NULL, this );
	m_enemygrouplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemyGroup ), NULL, this );
	m_enemygrouplist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemyGroupRightClick ), NULL, this );
	m_enemytextlist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnListBoxEnemyText ), NULL, this );
	m_enemytextlist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemyTextRightClick ), NULL, this );
	m_enemyscriptedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyscriptentryedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyscene->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyflag1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag9->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag10->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag11->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag12->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag13->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag14->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag15->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag16->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnEnemyStatChangeName ), NULL, this );
	m_enemystatnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemystatlvl->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystathp->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmp->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatattack->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatspeed->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatstrength->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmagic->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatspirit->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatevade->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmagicdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmagicevade->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatitemsteal1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemsteal2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemsteal3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemsteal4->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop4->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatdropcard->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatexp->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatgils->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatdefaultattack->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatmodel->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatmodelid->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatresources->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemystatelementabsorb1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune9->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune10->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune11->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune12->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune13->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune14->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune15->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune16->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune17->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune18->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune19->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune20->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune21->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune22->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune23->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune24->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune25->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune26->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune27->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune28->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune29->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune30->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune31->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune32->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto9->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto10->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto11->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto12->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto13->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto14->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto15->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto16->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto17->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto18->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto19->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto20->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto21->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto22->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto23->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto24->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto25->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto26->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto27->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto28->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto29->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto30->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto31->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto32->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial9->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial10->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial11->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial12->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial13->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial14->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial15->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial16->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial17->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial18->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial19->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial20->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial21->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial22->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial23->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial24->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial25->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial26->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial27->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial28->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial29->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial30->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial31->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial32->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatbluemagic->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnEnemySpellChangeName ), NULL, this );
	m_enemyspellnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyspelleffect->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellpower->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemyspellelement1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellaccuracy->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemyspellstatus->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellstatusset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyspellmp->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemyspellanimedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyspellanimreflect->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellbaseanim->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspelltargetflagdead->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspelltargetflagdeadfirst->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspelltargetflagcamera->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag5->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag6->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag7->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag8->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupfrequence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupamount->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupap->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupcamera->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx3->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy3->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz3->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle3->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid4->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink4->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx4->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy4->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz4->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle4->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_notebookcard->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookCard ), NULL, this );
	m_cardlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCard ), NULL, this );
	m_cardname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnCardChangeName ), NULL, this );
	m_cardnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCardChangeButton ), NULL, this );
	m_cardpoints->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardplayertype->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_cardplayerattack->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardplayerdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardplayermagicdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardnpctype->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_cardnpcattack->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardnpcdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardnpcmagicdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardtype->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_cardattack->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_carddefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardmagicdefence->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_carddecklist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCardDeck ), NULL, this );
	m_carddeckdifficulty->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_carddeckset->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_carddecksetbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCardChangeButton ), NULL, this );
	m_notebookenvironment->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookEnvironment ), NULL, this );
	m_textlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxText ), NULL, this );
	m_textdatalist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnTextEditText ), NULL, this );
	m_textdatalist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnTextRightClick ), NULL, this );
	m_textexport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportText ), NULL, this );
	m_textmanagecharmapbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextManageCharmap ), NULL, this );
	m_textcharmaplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapListSelection ), NULL, this );
	m_textcharmappalchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapPaletteChoice ), NULL, this );
	m_textcharmapexport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportCharmap ), NULL, this );
	m_textcharmapwindow->Connect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnTextCharmapPaint ), NULL, this );
	m_notebookworld->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookNone ), NULL, this );
	m_worldlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxWorldMap ), NULL, this );
	m_worldscriptedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldscriptentryedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldpreload->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldtextlist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnTextEditText ), NULL, this );
	m_worldtextlist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnTextRightClick ), NULL, this );
	m_worldtextexport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportText ), NULL, this );
	m_worldtextmanagecharmapbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextManageCharmap ), NULL, this );
	m_worldtextcharmaplist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapListSelection ), NULL, this );
	m_worldtextcharmappalchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapPaletteChoice ), NULL, this );
	m_worldtextcharmapexport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportCharmap ), NULL, this );
	m_worldtextcharmapwindow->Connect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnTextCharmapPaint ), NULL, this );
	m_worldplacelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxWorldPlace ), NULL, this );
	m_worldplacename->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnWorldChangeName ), NULL, this );
	m_worldplacenamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldbattlelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxWorldBattle ), NULL, this );
	m_worldbattlebattlespin1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlespin2->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlespin3->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlespin4->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlechoice1->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_worldbattlebattlechoice2->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_worldbattlebattlechoice3->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_worldbattlebattlechoice4->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_fieldlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxField ), NULL, this );
	m_fieldname->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnFieldChangeName ), NULL, this );
	m_fieldnamebutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldeditscript->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldscriptentryedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldpreload->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldtexturechoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnFieldChangeChoice ), NULL, this );
	m_fieldtexturemanage->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldtexturepreview->Connect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnFieldTexturePaint ), NULL, this );
	m_battlescenelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxBattleScene ), NULL, this );
	m_battlesceneexport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnBattleSceneExportButton ), NULL, this );
	m_battlesceneimport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnBattleSceneImportButton ), NULL, this );
	m_battlescenemanagetexture->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnBattleSceneManageTexturesButton ), NULL, this );
	m_battlescenetexturelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnBattleSceneTextureListBox ), NULL, this );
	m_battlescenetexturelist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnBattleSceneTextureRightClick ), NULL, this );
	m_battlescenetexturechoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnBattleSceneTextureChoice ), NULL, this );
	m_battlescenepalettechoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnBattleScenePaletteChoice ), NULL, this );
	m_battlescenetexture->Connect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnBattleSceneTexturePaint ), NULL, this );
	m_spellanimlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSpellAnimation ), NULL, this );
	m_spellanimsequence->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellAnimationChangeButton ), NULL, this );
	m_notebookinterface->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookInterface ), NULL, this );
	m_specialtextlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSpecialText ), NULL, this );
	m_specialtextdatalist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnSpecialTextEditText ), NULL, this );
	m_specialtextdatalist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnSpecialTextRightClick ), NULL, this );
	m_notebookmips->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookMips ), NULL, this );
	m_mipsbattlelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxMipsBattle ), NULL, this );
	m_mipsbattlecodegen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnMipsBattleButton ), NULL, this );
	m_mipsbattlecode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnMipsBattleButton ), NULL, this );
	m_notebookcil->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookCil ), NULL, this );
	m_ciltypelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCilStruct ), NULL, this );
	m_cilmethodlist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCilMethod ), NULL, this );
	m_cilmethodedit->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCilMethodButton ), NULL, this );
	m_cilmacrolist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCilMacro ), NULL, this );
	m_cilmacrobutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCilMacroButton ), NULL, this );
}

CDPanel::~CDPanel()
{
	// Disconnect Events
	m_notebookmain->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookMain ), NULL, this );
	m_notebookparty->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookParty ), NULL, this );
	m_spelllist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSpell ), NULL, this );
	m_spellname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSpellChangeName ), NULL, this );
	m_spellnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeButton ), NULL, this );
	m_spellperformname->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellhelp->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSpellChangeHelp ), NULL, this );
	m_spellhelpbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeButton ), NULL, this );
	m_spelleffect->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellpower->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSpellChangeSpin ), NULL, this );
	m_spellelement1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellelement8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellstatus->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellstatusset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeButton ), NULL, this );
	m_spellaccuracy->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSpellChangeSpin ), NULL, this );
	m_spellmp->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSpellChangeSpin ), NULL, this );
	m_spellmenuflagmenuuse->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellmenuflaghideap->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellmenuflagmpboost->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellmodeledit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnButtonClickSpellModel ), NULL, this );
	m_spellmodelaltedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnButtonClickSpellModelAlt ), NULL, this );
	m_spelltargettype->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spelltargetamount->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spelltargetpriority->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spelltargetflagdead->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spelltargetflagdeadfirst->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spelltargetflagcamera->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spelltargetpanel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnSpellChangeChoice ), NULL, this );
	m_spellflag1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_spellflag8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnSpellChangeFlags ), NULL, this );
	m_supportlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSupport ), NULL, this );
	m_supportname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSupportChangeName ), NULL, this );
	m_supportnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSupportChangeButton ), NULL, this );
	m_supporthelp->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnSupportChangeHelp ), NULL, this );
	m_supporthelpbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSupportChangeButton ), NULL, this );
	m_supportcost->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnSupportChangeSpin ), NULL, this );
	m_cmdlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCommand ), NULL, this );
	m_cmdname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnCommandChangeName ), NULL, this );
	m_cmdnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdhelp->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnCommandChangeHelp ), NULL, this );
	m_cmdhelpbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdpanel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_cmdlink->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_cmdaddspell->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdremovespell->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdspellfulllist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdspelllist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdmovespellup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdmovespelldown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCommandChangeButton ), NULL, this );
	m_cmdobjectfirst->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_cmdobjectlast->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCommandChangeChoice ), NULL, this );
	m_statlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxStat ), NULL, this );
	m_statchardefaultname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnStatChangeDefaultName ), NULL, this );
	m_statchardefaultnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statcharspeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharstrength->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharmagic->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharspirit->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharmagicstone->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharabilityset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharabilitylist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statcharabilitychoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharabilitycost->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statcharcommand1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharcommand2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharcommandtrance1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharcommandtrance2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statchartranceattack->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharequipset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharweapon->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharhead->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharwrist->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statchararmor->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statcharaccessory->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeChoice ), NULL, this );
	m_statlvlexpmanage->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statlvlexplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statlvlexpspin->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statlvlhpmanage->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statlvlhplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statlvlhpspin->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_statlvlmpmanage->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnStatChangeButton ), NULL, this );
	m_statlvlmplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnStatChangeList ), NULL, this );
	m_statlvlmpspin->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnStatChangeSpin ), NULL, this );
	m_partyspeciallist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxPartySpecial ), NULL, this );
	m_partyspecialmagicswordlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnPartySpecialChangeList ), NULL, this );
	m_partyspecialmagicswordspell->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnPartySpecialChangeChoice ), NULL, this );
	m_notebookinventory->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookInventory ), NULL, this );
	m_notebookitem->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookNone ), NULL, this );
	m_itemlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxItem ), NULL, this );
	m_itemname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnItemChangeName ), NULL, this );
	m_itemnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemhelp->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnItemChangeHelp ), NULL, this );
	m_itemhelpbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemhelp2->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnItemChangeBattleHelp ), NULL, this );
	m_itemhelp2button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemtype1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemtype8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemprice->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemcharavailability16->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability15->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability14->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability13->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability12->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability11->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability10->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability9->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemcharavailability1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemstatid->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemstatidbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemskill1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemskill2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemskill3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemmenuposlist->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CDPanel::OnItemPositionListClick ), NULL, this );
	m_itemmenuposbuttonup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemmenuposbuttondown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemequipposlist->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CDPanel::OnItemPositionListClick ), NULL, this );
	m_itemequipposbuttonup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemequipposbuttondown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_itemicon->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemiconcolor->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemusableeffect->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusablepower->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemusablestatus1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus9->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus10->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus11->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus12->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus13->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus14->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus15->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus16->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus17->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus18->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus19->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus20->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus21->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus22->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus23->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus24->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus25->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus26->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus27->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus28->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus29->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus30->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus31->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablestatus32->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusablemodeledit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnButtonClickItemModel ), NULL, this );
	m_itemusabletargettype->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusabletargetamount->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusabletargetpriority->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemusabletargetflagdead->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusabletargetflagdeadfirst->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusabletargetflagcamera->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemusabletargetpanel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemweaponformula->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemweaponpower->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponelement1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponelement8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponstatus->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnItemChangeChoice ), NULL, this );
	m_itemweaponaccuracy->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponflag1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponflag8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeFlags ), NULL, this );
	m_itemweaponunk1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponunk2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemweaponmodel->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmordefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmorevade->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmormagicdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_itemarmormagicevade->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnItemChangeSpin ), NULL, this );
	m_keyitemlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxKeyItem ), NULL, this );
	m_keyitemname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnKeyItemChangeName ), NULL, this );
	m_keyitemnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_keyitemhelp->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnKeyItemChangeHelp ), NULL, this );
	m_keyitemhelpbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_keyitemdescription->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnKeyItemChangeDescription ), NULL, this );
	m_keyitemdescriptionbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnItemChangeButton ), NULL, this );
	m_notebookshop->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookNone ), NULL, this );
	m_shoplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxShop ), NULL, this );
	m_shopitemadd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopitemremove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopitemfulllist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopitemlist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopmoveitemup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_shopmoveitemdown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeButton ), NULL, this );
	m_synthshoplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSynthesisShop ), NULL, this );
	m_synthshopsynthitem->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnShopChangeChoice ), NULL, this );
	m_synthshoprecipe1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnShopChangeChoice ), NULL, this );
	m_synthshoprecipe2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnShopChangeChoice ), NULL, this );
	m_synthshopprice->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnShopChangeSpin ), NULL, this );
	m_synthshopsynthshop1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_synthshopsynthshop8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnShopChangeFlags ), NULL, this );
	m_enemylist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemy ), NULL, this );
	m_enemystatlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemyStat ), NULL, this );
	m_enemystatlist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemyStatRightClick ), NULL, this );
	m_enemyspelllist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemySpell ), NULL, this );
	m_enemyspelllist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemySpellRightClick ), NULL, this );
	m_enemygrouplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxEnemyGroup ), NULL, this );
	m_enemygrouplist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemyGroupRightClick ), NULL, this );
	m_enemytextlist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnListBoxEnemyText ), NULL, this );
	m_enemytextlist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnEnemyTextRightClick ), NULL, this );
	m_enemyscriptedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyscriptentryedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyscene->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyflag1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag9->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag10->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag11->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag12->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag13->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag14->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag15->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyflag16->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnEnemyStatChangeName ), NULL, this );
	m_enemystatnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemystatlvl->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystathp->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmp->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatattack->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatspeed->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatstrength->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmagic->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatspirit->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatevade->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmagicdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatmagicevade->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatitemsteal1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemsteal2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemsteal3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemsteal4->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatitemdrop4->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatdropcard->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatexp->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatgils->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatdefaultattack->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatmodel->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemystatmodelid->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemystatresources->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemystatelementabsorb1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementabsorb8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementimmune8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementhalf8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatelementweak8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatclass8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune9->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune10->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune11->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune12->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune13->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune14->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune15->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune16->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune17->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune18->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune19->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune20->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune21->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune22->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune23->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune24->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune25->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune26->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune27->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune28->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune29->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune30->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune31->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusimmune32->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto9->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto10->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto11->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto12->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto13->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto14->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto15->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto16->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto17->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto18->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto19->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto20->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto21->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto22->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto23->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto24->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto25->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto26->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto27->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto28->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto29->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto30->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto31->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusauto32->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial9->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial10->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial11->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial12->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial13->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial14->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial15->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial16->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial17->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial18->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial19->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial20->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial21->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial22->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial23->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial24->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial25->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial26->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial27->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial28->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial29->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial30->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial31->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatstatusinitial32->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemystatbluemagic->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnEnemySpellChangeName ), NULL, this );
	m_enemyspellnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyspelleffect->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellpower->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemyspellelement1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellelement8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellaccuracy->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemyspellstatus->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellstatusset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyspellmp->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemyspellanimedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeButton ), NULL, this );
	m_enemyspellanimreflect->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspellbaseanim->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemyspelltargetflagdead->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspelltargetflagdeadfirst->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspelltargetflagcamera->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag5->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag6->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag7->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemyspellflag8->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupfrequence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupamount->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupap->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupcamera->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx3->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy3->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz3->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle3->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupid4->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnEnemyChangeChoice ), NULL, this );
	m_enemygrouptargetable4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygrouplink4->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CDPanel::OnEnemyChangeFlags ), NULL, this );
	m_enemygroupposx4->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposy4->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupposz4->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_enemygroupangle4->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnEnemyChangeSpin ), NULL, this );
	m_notebookcard->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookCard ), NULL, this );
	m_cardlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCard ), NULL, this );
	m_cardname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnCardChangeName ), NULL, this );
	m_cardnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCardChangeButton ), NULL, this );
	m_cardpoints->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardplayertype->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_cardplayerattack->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardplayerdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardplayermagicdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardnpctype->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_cardnpcattack->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardnpcdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardnpcmagicdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardtype->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_cardattack->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_carddefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_cardmagicdefence->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_carddecklist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCardDeck ), NULL, this );
	m_carddeckdifficulty->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnCardChangeSpin ), NULL, this );
	m_carddeckset->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnCardChangeChoice ), NULL, this );
	m_carddecksetbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCardChangeButton ), NULL, this );
	m_notebookenvironment->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookEnvironment ), NULL, this );
	m_textlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxText ), NULL, this );
	m_textdatalist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnTextEditText ), NULL, this );
	m_textdatalist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnTextRightClick ), NULL, this );
	m_textexport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportText ), NULL, this );
	m_textmanagecharmapbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextManageCharmap ), NULL, this );
	m_textcharmaplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapListSelection ), NULL, this );
	m_textcharmappalchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapPaletteChoice ), NULL, this );
	m_textcharmapexport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportCharmap ), NULL, this );
	m_textcharmapwindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnTextCharmapPaint ), NULL, this );
	m_notebookworld->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookNone ), NULL, this );
	m_worldlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxWorldMap ), NULL, this );
	m_worldscriptedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldscriptentryedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldpreload->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldtextlist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnTextEditText ), NULL, this );
	m_worldtextlist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnTextRightClick ), NULL, this );
	m_worldtextexport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportText ), NULL, this );
	m_worldtextmanagecharmapbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextManageCharmap ), NULL, this );
	m_worldtextcharmaplist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapListSelection ), NULL, this );
	m_worldtextcharmappalchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnTextCharmapPaletteChoice ), NULL, this );
	m_worldtextcharmapexport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnTextExportCharmap ), NULL, this );
	m_worldtextcharmapwindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnTextCharmapPaint ), NULL, this );
	m_worldplacelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxWorldPlace ), NULL, this );
	m_worldplacename->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnWorldChangeName ), NULL, this );
	m_worldplacenamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnWorldChangeButton ), NULL, this );
	m_worldbattlelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxWorldBattle ), NULL, this );
	m_worldbattlebattlespin1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlespin2->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlespin3->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlespin4->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CDPanel::OnWorldChangeSpin ), NULL, this );
	m_worldbattlebattlechoice1->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_worldbattlebattlechoice2->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_worldbattlebattlechoice3->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_worldbattlebattlechoice4->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnWorldChangeChoice ), NULL, this );
	m_fieldlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxField ), NULL, this );
	m_fieldname->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CDPanel::OnFieldChangeName ), NULL, this );
	m_fieldnamebutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldeditscript->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldscriptentryedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldpreload->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldtexturechoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnFieldChangeChoice ), NULL, this );
	m_fieldtexturemanage->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnFieldChangeButton ), NULL, this );
	m_fieldtexturepreview->Disconnect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnFieldTexturePaint ), NULL, this );
	m_battlescenelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxBattleScene ), NULL, this );
	m_battlesceneexport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnBattleSceneExportButton ), NULL, this );
	m_battlesceneimport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnBattleSceneImportButton ), NULL, this );
	m_battlescenemanagetexture->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnBattleSceneManageTexturesButton ), NULL, this );
	m_battlescenetexturelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnBattleSceneTextureListBox ), NULL, this );
	m_battlescenetexturelist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnBattleSceneTextureRightClick ), NULL, this );
	m_battlescenetexturechoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnBattleSceneTextureChoice ), NULL, this );
	m_battlescenepalettechoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CDPanel::OnBattleScenePaletteChoice ), NULL, this );
	m_battlescenetexture->Disconnect( wxEVT_PAINT, wxPaintEventHandler( CDPanel::OnBattleSceneTexturePaint ), NULL, this );
	m_spellanimlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSpellAnimation ), NULL, this );
	m_spellanimsequence->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnSpellAnimationChangeButton ), NULL, this );
	m_notebookinterface->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookInterface ), NULL, this );
	m_specialtextlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxSpecialText ), NULL, this );
	m_specialtextdatalist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CDPanel::OnSpecialTextEditText ), NULL, this );
	m_specialtextdatalist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CDPanel::OnSpecialTextRightClick ), NULL, this );
	m_notebookmips->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookMips ), NULL, this );
	m_mipsbattlelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxMipsBattle ), NULL, this );
	m_mipsbattlecodegen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnMipsBattleButton ), NULL, this );
	m_mipsbattlecode->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnMipsBattleButton ), NULL, this );
	m_notebookcil->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CDPanel::OnNotebookCil ), NULL, this );
	m_ciltypelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCilStruct ), NULL, this );
	m_cilmethodlist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCilMethod ), NULL, this );
	m_cilmethodedit->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCilMethodButton ), NULL, this );
	m_cilmacrolist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CDPanel::OnListBoxCilMacro ), NULL, this );
	m_cilmacrobutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CDPanel::OnCilMacroButton ), NULL, this );
	
}

DiscardableMessage::DiscardableMessage( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer13->AddGrowableRow( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_label = new wxStaticText( this, wxID_ANY, _("Message"), wxDefaultPosition, wxDefaultSize, 0 );
	m_label->Wrap( -1 );
	fgSizer13->Add( m_label, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 10 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttondiscard = new wxButton( this, wxID_DISCARD, _("Don't display this\nmessage anymore"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_buttondiscard, 0, wxALL|wxEXPAND, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer15->Add( m_buttonok, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	
	fgSizer13->Add( bSizer15, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer13 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttondiscard->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscardableMessage::OnButtonClickDiscard ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscardableMessage::OnButtonClickOk ), NULL, this );
}

DiscardableMessage::~DiscardableMessage()
{
	// Disconnect Events
	m_buttondiscard->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscardableMessage::OnButtonClickDiscard ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiscardableMessage::OnButtonClickOk ), NULL, this );
	
}

SteamLanguageMessage::SteamLanguageMessage( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_label = new wxStaticText( this, wxID_ANY, _("Please select a language"), wxDefaultPosition, wxDefaultSize, 0 );
	m_label->Wrap( -1 );
	fgSizer13->Add( m_label, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP, 10 );
	
	wxString m_choiceChoices[] = { _("English (US)"), _("English (UK)"), _("Japanese"), _("German"), _("French"), _("Italian"), _("Spanish") };
	int m_choiceNChoices = sizeof( m_choiceChoices ) / sizeof( wxString );
	m_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceNChoices, m_choiceChoices, 0 );
	m_choice->SetSelection( 0 );
	fgSizer13->Add( m_choice, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer13->Add( m_buttonok, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	this->SetSizer( fgSizer13 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

SteamLanguageMessage::~SteamLanguageMessage()
{
}

SpellAnimationWindow::SpellAnimationWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer120;
	bSizer120 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* mainsizer;
	mainsizer = new wxGridBagSizer( 0, 0 );
	mainsizer->SetFlexibleDirection( wxBOTH );
	mainsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer117;
	bSizer117 = new wxBoxSizer( wxVERTICAL );
	
	m_panelmain = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer118;
	bSizer118 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_modelmainChoices;
	m_modelmain = new wxChoice( m_panelmain, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), m_modelmainChoices, 0 );
	m_modelmain->SetSelection( 0 );
	bSizer118->Add( m_modelmain, 0, wxALL, 5 );
	
	
	m_panelmain->SetSizer( bSizer118 );
	m_panelmain->Layout();
	bSizer118->Fit( m_panelmain );
	bSizer117->Add( m_panelmain, 1, wxEXPAND | wxALL, 2 );
	
	m_panelall = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelall->Hide();
	
	wxBoxSizer* bSizer119;
	bSizer119 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_modelallChoices;
	m_modelall = new wxChoice( m_panelall, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), m_modelallChoices, 0 );
	m_modelall->SetSelection( 0 );
	bSizer119->Add( m_modelall, 0, wxALL, 5 );
	
	
	m_panelall->SetSizer( bSizer119 );
	m_panelall->Layout();
	bSizer119->Fit( m_panelall );
	bSizer117->Add( m_panelall, 1, wxEXPAND | wxALL, 2 );
	
	
	mainsizer->Add( bSizer117, wxGBPosition( 0, 0 ), wxGBSpan( 2, 1 ), wxEXPAND, 5 );
	
	m_targetinfo = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_targetinfo->Wrap( -1 );
	mainsizer->Add( m_targetinfo, wxGBPosition( 0, 1 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_enableall = new wxCheckBox( this, wxID_ANY, _("Enable all animations"), wxDefaultPosition, wxDefaultSize, 0 );
	mainsizer->Add( m_enableall, wxGBPosition( 1, 1 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	mainsizer->Add( m_buttoncancel, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	mainsizer->Add( m_buttonok, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	
	mainsizer->AddGrowableCol( 0 );
	mainsizer->AddGrowableRow( 0 );
	
	bSizer120->Add( mainsizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer120 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_enableall->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SpellAnimationWindow::OnCheckBox ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellAnimationWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellAnimationWindow::OnButtonClick ), NULL, this );
}

SpellAnimationWindow::~SpellAnimationWindow()
{
	// Disconnect Events
	m_enableall->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SpellAnimationWindow::OnCheckBox ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellAnimationWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellAnimationWindow::OnButtonClick ), NULL, this );
	
}

CurveEditorWindow::CurveEditorWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_curvescrolled = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 300,300 ), wxHSCROLL|wxVSCROLL );
	m_curvescrolled->SetScrollRate( 5, 5 );
	fgSizer33->Add( m_curvescrolled, 1, wxEXPAND | wxALL, 5 );
	
	m_linkup = new wxCheckBox( this, wxID_ANY, _("Link up"), wxDefaultPosition, wxDefaultSize, 0 );
	m_linkup->SetValue(true); 
	fgSizer33->Add( m_linkup, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer98;
	bSizer98 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer98->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer98->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer33->Add( bSizer98, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer33 );
	this->Layout();
	fgSizer33->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_curvescrolled->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CurveEditorWindow::OnMouse ), NULL, this );
	m_curvescrolled->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( CurveEditorWindow::OnMouse ), NULL, this );
	m_curvescrolled->Connect( wxEVT_MOTION, wxMouseEventHandler( CurveEditorWindow::OnMouse ), NULL, this );
	m_curvescrolled->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CurveEditorWindow::OnMouseWheel ), NULL, this );
	m_curvescrolled->Connect( wxEVT_PAINT, wxPaintEventHandler( CurveEditorWindow::OnPaint ), NULL, this );
	m_linkup->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CurveEditorWindow::OnCheckBox ), NULL, this );
	m_linkup->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CurveEditorWindow::OnMouseWheel ), NULL, this );
}

CurveEditorWindow::~CurveEditorWindow()
{
	// Disconnect Events
	m_curvescrolled->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CurveEditorWindow::OnMouse ), NULL, this );
	m_curvescrolled->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( CurveEditorWindow::OnMouse ), NULL, this );
	m_curvescrolled->Disconnect( wxEVT_MOTION, wxMouseEventHandler( CurveEditorWindow::OnMouse ), NULL, this );
	m_curvescrolled->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CurveEditorWindow::OnMouseWheel ), NULL, this );
	m_curvescrolled->Disconnect( wxEVT_PAINT, wxPaintEventHandler( CurveEditorWindow::OnPaint ), NULL, this );
	m_linkup->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CurveEditorWindow::OnCheckBox ), NULL, this );
	m_linkup->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CurveEditorWindow::OnMouseWheel ), NULL, this );
	
}

SpellStatusWindow::SpellStatusWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxGridSizer* gSizer46;
	gSizer46 = new wxGridSizer( 0, 4, 0, 0 );
	
	m_statusbox1 = new wxCheckBox( this, wxID_ANY, _("Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox1, 0, wxALL, 5 );
	
	m_statusbox2 = new wxCheckBox( this, wxID_ANY, _("Venom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox2, 0, wxALL, 5 );
	
	m_statusbox3 = new wxCheckBox( this, wxID_ANY, _("Virus"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox3, 0, wxALL, 5 );
	
	m_statusbox4 = new wxCheckBox( this, wxID_ANY, _("Silence"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox4, 0, wxALL, 5 );
	
	m_statusbox5 = new wxCheckBox( this, wxID_ANY, _("Darkness"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox5, 0, wxALL, 5 );
	
	m_statusbox6 = new wxCheckBox( this, wxID_ANY, _("Trouble"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox6, 0, wxALL, 5 );
	
	m_statusbox7 = new wxCheckBox( this, wxID_ANY, _("Zombie"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox7, 0, wxALL, 5 );
	
	m_statusbox8 = new wxCheckBox( this, wxID_ANY, _("Easy Kill"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox8, 0, wxALL, 5 );
	
	m_statusbox9 = new wxCheckBox( this, wxID_ANY, _("Death"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox9, 0, wxALL, 5 );
	
	m_statusbox10 = new wxCheckBox( this, wxID_ANY, _("Low HP"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox10, 0, wxALL, 5 );
	
	m_statusbox11 = new wxCheckBox( this, wxID_ANY, _("Confuse"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox11, 0, wxALL, 5 );
	
	m_statusbox12 = new wxCheckBox( this, wxID_ANY, _("Berserk"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox12, 0, wxALL, 5 );
	
	m_statusbox13 = new wxCheckBox( this, wxID_ANY, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox13, 0, wxALL, 5 );
	
	m_statusbox14 = new wxCheckBox( this, wxID_ANY, _("Auto-Life"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox14, 0, wxALL, 5 );
	
	m_statusbox15 = new wxCheckBox( this, wxID_ANY, _("Trance"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox15, 0, wxALL, 5 );
	
	m_statusbox16 = new wxCheckBox( this, wxID_ANY, _("Defend"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox16, 0, wxALL, 5 );
	
	m_statusbox17 = new wxCheckBox( this, wxID_ANY, _("Poison"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox17, 0, wxALL, 5 );
	
	m_statusbox18 = new wxCheckBox( this, wxID_ANY, _("Sleep"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox18, 0, wxALL, 5 );
	
	m_statusbox19 = new wxCheckBox( this, wxID_ANY, _("Regen"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox19, 0, wxALL, 5 );
	
	m_statusbox20 = new wxCheckBox( this, wxID_ANY, _("Haste"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox20, 0, wxALL, 5 );
	
	m_statusbox21 = new wxCheckBox( this, wxID_ANY, _("Slow"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox21, 0, wxALL, 5 );
	
	m_statusbox22 = new wxCheckBox( this, wxID_ANY, _("Float"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox22, 0, wxALL, 5 );
	
	m_statusbox23 = new wxCheckBox( this, wxID_ANY, _("Shell"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox23, 0, wxALL, 5 );
	
	m_statusbox24 = new wxCheckBox( this, wxID_ANY, _("Protect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox24, 0, wxALL, 5 );
	
	m_statusbox25 = new wxCheckBox( this, wxID_ANY, _("Heat"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox25, 0, wxALL, 5 );
	
	m_statusbox26 = new wxCheckBox( this, wxID_ANY, _("Freeze"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox26, 0, wxALL, 5 );
	
	m_statusbox27 = new wxCheckBox( this, wxID_ANY, _("Vanish"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox27, 0, wxALL, 5 );
	
	m_statusbox28 = new wxCheckBox( this, wxID_ANY, _("Doom"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox28, 0, wxALL, 5 );
	
	m_statusbox29 = new wxCheckBox( this, wxID_ANY, _("Mini"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox29, 0, wxALL, 5 );
	
	m_statusbox30 = new wxCheckBox( this, wxID_ANY, _("Reflect"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox30, 0, wxALL, 5 );
	
	m_statusbox31 = new wxCheckBox( this, wxID_ANY, _("Jump"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox31, 0, wxALL, 5 );
	
	m_statusbox32 = new wxCheckBox( this, wxID_ANY, _("Gradual Petrify"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer46->Add( m_statusbox32, 0, wxALL, 5 );
	
	
	fgSizer33->Add( gSizer46, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer931;
	bSizer931 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer33->Add( bSizer931, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer33 );
	this->Layout();
	fgSizer33->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellStatusWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellStatusWindow::OnButtonClick ), NULL, this );
}

SpellStatusWindow::~SpellStatusWindow()
{
	// Disconnect Events
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellStatusWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SpellStatusWindow::OnButtonClick ), NULL, this );
	
}

ItemStatWindow::ItemStatWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxGridBagSizer* gbSizer121;
	gbSizer121 = new wxGridBagSizer( 0, 0 );
	gbSizer121->SetFlexibleDirection( wxBOTH );
	gbSizer121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1671 = new wxStaticText( this, wxID_ANY, _("Strength"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1671->Wrap( -1 );
	gbSizer121->Add( m_staticText1671, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_strength = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gbSizer121->Add( m_strength, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	m_staticText1681 = new wxStaticText( this, wxID_ANY, _("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1681->Wrap( -1 );
	gbSizer121->Add( m_staticText1681, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_speed = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gbSizer121->Add( m_speed, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	m_staticText1691 = new wxStaticText( this, wxID_ANY, _("Magic"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1691->Wrap( -1 );
	gbSizer121->Add( m_staticText1691, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_magic = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gbSizer121->Add( m_magic, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	m_staticText1701 = new wxStaticText( this, wxID_ANY, _("Spirit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1701->Wrap( -1 );
	gbSizer121->Add( m_staticText1701, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_spirit = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gbSizer121->Add( m_spirit, wxGBPosition( 1, 3 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	m_staticText1711 = new wxStaticText( this, wxID_ANY, _("Absorb"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1711->Wrap( -1 );
	gbSizer121->Add( m_staticText1711, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxGridSizer* m_elementabsorb;
	m_elementabsorb = new wxGridSizer( 2, 4, 0, 0 );
	
	m_elementabsorb1 = new wxCheckBox( this, wxID_EA1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb1, 0, wxALL, 5 );
	
	m_elementabsorb2 = new wxCheckBox( this, wxID_EA2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb2, 0, wxALL, 5 );
	
	m_elementabsorb3 = new wxCheckBox( this, wxID_EA3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb3, 0, wxALL, 5 );
	
	m_elementabsorb4 = new wxCheckBox( this, wxID_EA4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb4, 0, wxALL, 5 );
	
	m_elementabsorb5 = new wxCheckBox( this, wxID_EA5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb5, 0, wxALL, 5 );
	
	m_elementabsorb6 = new wxCheckBox( this, wxID_EA6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb6, 0, wxALL, 5 );
	
	m_elementabsorb7 = new wxCheckBox( this, wxID_EA7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb7, 0, wxALL, 5 );
	
	m_elementabsorb8 = new wxCheckBox( this, wxID_EA8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementabsorb->Add( m_elementabsorb8, 0, wxALL, 5 );
	
	
	gbSizer121->Add( m_elementabsorb, wxGBPosition( 2, 1 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	m_staticText1721 = new wxStaticText( this, wxID_ANY, _("Immune"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1721->Wrap( -1 );
	gbSizer121->Add( m_staticText1721, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxGridSizer* m_elementimmune;
	m_elementimmune = new wxGridSizer( 2, 4, 0, 0 );
	
	m_elementimmune1 = new wxCheckBox( this, wxID_EI1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune1, 0, wxALL, 5 );
	
	m_elementimmune2 = new wxCheckBox( this, wxID_EI2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune2, 0, wxALL, 5 );
	
	m_elementimmune3 = new wxCheckBox( this, wxID_EI3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune3, 0, wxALL, 5 );
	
	m_elementimmune4 = new wxCheckBox( this, wxID_EI4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune4, 0, wxALL, 5 );
	
	m_elementimmune5 = new wxCheckBox( this, wxID_EI5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune5, 0, wxALL, 5 );
	
	m_elementimmune6 = new wxCheckBox( this, wxID_EI6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune6, 0, wxALL, 5 );
	
	m_elementimmune7 = new wxCheckBox( this, wxID_EI7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune7, 0, wxALL, 5 );
	
	m_elementimmune8 = new wxCheckBox( this, wxID_EI8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementimmune->Add( m_elementimmune8, 0, wxALL, 5 );
	
	
	gbSizer121->Add( m_elementimmune, wxGBPosition( 3, 1 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	m_staticText1731 = new wxStaticText( this, wxID_ANY, _("Half"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1731->Wrap( -1 );
	gbSizer121->Add( m_staticText1731, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxGridSizer* m_elementhalf;
	m_elementhalf = new wxGridSizer( 2, 4, 0, 0 );
	
	m_elementhalf1 = new wxCheckBox( this, wxID_EH1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf1, 0, wxALL, 5 );
	
	m_elementhalf2 = new wxCheckBox( this, wxID_EH2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf2, 0, wxALL, 5 );
	
	m_elementhalf3 = new wxCheckBox( this, wxID_EH3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf3, 0, wxALL, 5 );
	
	m_elementhalf4 = new wxCheckBox( this, wxID_EH4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf4, 0, wxALL, 5 );
	
	m_elementhalf5 = new wxCheckBox( this, wxID_EH5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf5, 0, wxALL, 5 );
	
	m_elementhalf6 = new wxCheckBox( this, wxID_EH6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf6, 0, wxALL, 5 );
	
	m_elementhalf7 = new wxCheckBox( this, wxID_EH7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf7, 0, wxALL, 5 );
	
	m_elementhalf8 = new wxCheckBox( this, wxID_EH8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementhalf->Add( m_elementhalf8, 0, wxALL, 5 );
	
	
	gbSizer121->Add( m_elementhalf, wxGBPosition( 4, 1 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	m_staticText1741 = new wxStaticText( this, wxID_ANY, _("Weak"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1741->Wrap( -1 );
	gbSizer121->Add( m_staticText1741, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxGridSizer* m_elementweak;
	m_elementweak = new wxGridSizer( 2, 4, 0, 0 );
	
	m_elementweak1 = new wxCheckBox( this, wxID_EW1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak1, 0, wxALL, 5 );
	
	m_elementweak2 = new wxCheckBox( this, wxID_EW2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak2, 0, wxALL, 5 );
	
	m_elementweak3 = new wxCheckBox( this, wxID_EW3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak3, 0, wxALL, 5 );
	
	m_elementweak4 = new wxCheckBox( this, wxID_EW4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak4, 0, wxALL, 5 );
	
	m_elementweak5 = new wxCheckBox( this, wxID_EW5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak5, 0, wxALL, 5 );
	
	m_elementweak6 = new wxCheckBox( this, wxID_EW6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak6, 0, wxALL, 5 );
	
	m_elementweak7 = new wxCheckBox( this, wxID_EW7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak7, 0, wxALL, 5 );
	
	m_elementweak8 = new wxCheckBox( this, wxID_EW8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementweak->Add( m_elementweak8, 0, wxALL, 5 );
	
	
	gbSizer121->Add( m_elementweak, wxGBPosition( 5, 1 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	m_staticText1751 = new wxStaticText( this, wxID_ANY, _("Boost"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1751->Wrap( -1 );
	gbSizer121->Add( m_staticText1751, wxGBPosition( 6, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxGridSizer* m_elementboost;
	m_elementboost = new wxGridSizer( 2, 4, 0, 0 );
	
	m_elementboost1 = new wxCheckBox( this, wxID_EB1, _("Fire"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost1, 0, wxALL, 5 );
	
	m_elementboost2 = new wxCheckBox( this, wxID_EB2, _("Ice"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost2, 0, wxALL, 5 );
	
	m_elementboost3 = new wxCheckBox( this, wxID_EB3, _("Thunder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost3, 0, wxALL, 5 );
	
	m_elementboost4 = new wxCheckBox( this, wxID_EB4, _("Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost4, 0, wxALL, 5 );
	
	m_elementboost5 = new wxCheckBox( this, wxID_EB5, _("Water"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost5, 0, wxALL, 5 );
	
	m_elementboost6 = new wxCheckBox( this, wxID_EB6, _("Wind"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost6, 0, wxALL, 5 );
	
	m_elementboost7 = new wxCheckBox( this, wxID_EB7, _("Holy"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost7, 0, wxALL, 5 );
	
	m_elementboost8 = new wxCheckBox( this, wxID_EB8, _("Shadow"), wxDefaultPosition, wxDefaultSize, 0 );
	m_elementboost->Add( m_elementboost8, 0, wxALL, 5 );
	
	
	gbSizer121->Add( m_elementboost, wxGBPosition( 6, 1 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	
	fgSizer33->Add( gbSizer121, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer931;
	bSizer931 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer33->Add( bSizer931, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer33 );
	this->Layout();
	fgSizer33->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ItemStatWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ItemStatWindow::OnButtonClick ), NULL, this );
}

ItemStatWindow::~ItemStatWindow()
{
	// Disconnect Events
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ItemStatWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ItemStatWindow::OnButtonClick ), NULL, this );
	
}

CardSetWindow::CardSetWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxGridSizer* gSizer49;
	gSizer49 = new wxGridSizer( 0, 2, 0, 0 );
	
	wxArrayString m_choicecard1Choices;
	m_choicecard1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard1Choices, 0 );
	m_choicecard1->SetSelection( 0 );
	gSizer49->Add( m_choicecard1, 0, wxALL, 5 );
	
	wxArrayString m_choicecard2Choices;
	m_choicecard2 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard2Choices, 0 );
	m_choicecard2->SetSelection( 0 );
	gSizer49->Add( m_choicecard2, 0, wxALL, 5 );
	
	wxArrayString m_choicecard3Choices;
	m_choicecard3 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard3Choices, 0 );
	m_choicecard3->SetSelection( 0 );
	gSizer49->Add( m_choicecard3, 0, wxALL, 5 );
	
	wxArrayString m_choicecard4Choices;
	m_choicecard4 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard4Choices, 0 );
	m_choicecard4->SetSelection( 0 );
	gSizer49->Add( m_choicecard4, 0, wxALL, 5 );
	
	wxArrayString m_choicecard5Choices;
	m_choicecard5 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard5Choices, 0 );
	m_choicecard5->SetSelection( 0 );
	gSizer49->Add( m_choicecard5, 0, wxALL, 5 );
	
	wxArrayString m_choicecard6Choices;
	m_choicecard6 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard6Choices, 0 );
	m_choicecard6->SetSelection( 0 );
	gSizer49->Add( m_choicecard6, 0, wxALL, 5 );
	
	wxArrayString m_choicecard7Choices;
	m_choicecard7 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard7Choices, 0 );
	m_choicecard7->SetSelection( 0 );
	gSizer49->Add( m_choicecard7, 0, wxALL, 5 );
	
	wxArrayString m_choicecard8Choices;
	m_choicecard8 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard8Choices, 0 );
	m_choicecard8->SetSelection( 0 );
	gSizer49->Add( m_choicecard8, 0, wxALL, 5 );
	
	wxArrayString m_choicecard9Choices;
	m_choicecard9 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard9Choices, 0 );
	m_choicecard9->SetSelection( 0 );
	gSizer49->Add( m_choicecard9, 0, wxALL, 5 );
	
	wxArrayString m_choicecard10Choices;
	m_choicecard10 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard10Choices, 0 );
	m_choicecard10->SetSelection( 0 );
	gSizer49->Add( m_choicecard10, 0, wxALL, 5 );
	
	wxArrayString m_choicecard11Choices;
	m_choicecard11 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard11Choices, 0 );
	m_choicecard11->SetSelection( 0 );
	gSizer49->Add( m_choicecard11, 0, wxALL, 5 );
	
	wxArrayString m_choicecard12Choices;
	m_choicecard12 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard12Choices, 0 );
	m_choicecard12->SetSelection( 0 );
	gSizer49->Add( m_choicecard12, 0, wxALL, 5 );
	
	wxArrayString m_choicecard13Choices;
	m_choicecard13 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard13Choices, 0 );
	m_choicecard13->SetSelection( 0 );
	gSizer49->Add( m_choicecard13, 0, wxALL, 5 );
	
	wxArrayString m_choicecard14Choices;
	m_choicecard14 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard14Choices, 0 );
	m_choicecard14->SetSelection( 0 );
	gSizer49->Add( m_choicecard14, 0, wxALL, 5 );
	
	wxArrayString m_choicecard15Choices;
	m_choicecard15 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard15Choices, 0 );
	m_choicecard15->SetSelection( 0 );
	gSizer49->Add( m_choicecard15, 0, wxALL, 5 );
	
	wxArrayString m_choicecard16Choices;
	m_choicecard16 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicecard16Choices, 0 );
	m_choicecard16->SetSelection( 0 );
	gSizer49->Add( m_choicecard16, 0, wxALL, 5 );
	
	
	fgSizer33->Add( gSizer49, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer931;
	bSizer931 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer33->Add( bSizer931, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer33 );
	this->Layout();
	fgSizer33->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CardSetWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CardSetWindow::OnButtonClick ), NULL, this );
}

CardSetWindow::~CardSetWindow()
{
	// Disconnect Events
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CardSetWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CardSetWindow::OnButtonClick ), NULL, this );
	
}

EnemyResourceWindow::EnemyResourceWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer33->AddGrowableCol( 0 );
	fgSizer33->AddGrowableRow( 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer96;
	fgSizer96 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer96->AddGrowableCol( 2 );
	fgSizer96->AddGrowableRow( 0 );
	fgSizer96->SetFlexibleDirection( wxBOTH );
	fgSizer96->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer92;
	fgSizer92 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer92->AddGrowableCol( 1 );
	fgSizer92->SetFlexibleDirection( wxBOTH );
	fgSizer92->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3591 = new wxStaticText( this, wxID_ANY, _("Text Amount"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3591->Wrap( -1 );
	fgSizer92->Add( m_staticText3591, 0, wxALL, 5 );
	
	m_textamount = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 127, 0 );
	m_textamount->SetToolTip( _("Number of texts reserved for the Attack animations of the enemy") );
	
	fgSizer92->Add( m_textamount, 0, wxALL, 5 );
	
	m_staticText355 = new wxStaticText( this, wxID_ANY, _("Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText355->Wrap( -1 );
	fgSizer92->Add( m_staticText355, 0, wxALL, 5 );
	
	m_radius = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_radius->SetToolTip( _("A hint for the enemy's size\nUsed for instance to determine the size\nused by Scan's special effect") );
	
	fgSizer92->Add( m_radius, 0, wxALL, 5 );
	
	m_staticText356 = new wxStaticText( this, wxID_ANY, _("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText356->Wrap( -1 );
	fgSizer92->Add( m_staticText356, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxFlexGridSizer* fgSizer95;
	fgSizer95 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer95->SetFlexibleDirection( wxBOTH );
	fgSizer95->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText371 = new wxStaticText( this, wxID_ANY, _("Hidden"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText371->Wrap( -1 );
	fgSizer95->Add( m_staticText371, 0, wxLEFT, 5 );
	
	m_staticText372 = new wxStaticText( this, wxID_ANY, _("Vanish"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText372->Wrap( -1 );
	fgSizer95->Add( m_staticText372, 0, wxLEFT, 5 );
	
	m_mesh = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_mesh->SetToolTip( _("List of meshes hidden by default") );
	
	fgSizer95->Add( m_mesh, 0, wxLEFT|wxRIGHT, 5 );
	
	m_meshvanish = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_meshvanish->SetToolTip( _("List of meshes still displayed under the \"Vanish\" ailment") );
	
	fgSizer95->Add( m_meshvanish, 0, wxLEFT|wxRIGHT, 5 );
	
	
	fgSizer92->Add( fgSizer95, 1, wxEXPAND, 5 );
	
	m_staticText358 = new wxStaticText( this, wxID_ANY, _("Target Bone"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText358->Wrap( -1 );
	fgSizer92->Add( m_staticText358, 0, wxALL, 5 );
	
	m_bonetarget = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer92->Add( m_bonetarget, 0, wxALL, 5 );
	
	m_staticText359 = new wxStaticText( this, wxID_ANY, _("Camera Bones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText359->Wrap( -1 );
	fgSizer92->Add( m_staticText359, 0, wxALL, 5 );
	
	wxGridSizer* gSizer49;
	gSizer49 = new wxGridSizer( 0, 3, 0, 0 );
	
	m_bonecamera1 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer49->Add( m_bonecamera1, 0, wxALL, 5 );
	
	m_bonecamera2 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer49->Add( m_bonecamera2, 0, wxALL, 5 );
	
	m_bonecamera3 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer49->Add( m_bonecamera3, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer49, 1, wxEXPAND, 5 );
	
	m_staticText360 = new wxStaticText( this, wxID_ANY, _("Icon Bones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText360->Wrap( -1 );
	fgSizer92->Add( m_staticText360, 0, wxALL, 5 );
	
	wxGridSizer* gSizer50;
	gSizer50 = new wxGridSizer( 0, 3, 0, 0 );
	
	m_boneselection1 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer50->Add( m_boneselection1, 0, wxALL, 5 );
	
	m_boneselection2 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer50->Add( m_boneselection2, 0, wxALL, 5 );
	
	m_boneselection3 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer50->Add( m_boneselection3, 0, wxALL, 5 );
	
	m_boneselection4 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer50->Add( m_boneselection4, 0, wxALL, 5 );
	
	m_boneselection5 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer50->Add( m_boneselection5, 0, wxALL, 5 );
	
	m_boneselection6 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer50->Add( m_boneselection6, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer50, 1, wxEXPAND, 5 );
	
	m_staticText367 = new wxStaticText( this, wxID_ANY, _("Shadow Bones"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText367->Wrap( -1 );
	fgSizer92->Add( m_staticText367, 0, wxALL, 5 );
	
	wxGridSizer* gSizer51;
	gSizer51 = new wxGridSizer( 0, 3, 0, 0 );
	
	m_boneshadow1 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer51->Add( m_boneshadow1, 0, wxALL, 5 );
	
	m_boneshadow2 = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gSizer51->Add( m_boneshadow2, 0, wxALL, 5 );
	
	
	fgSizer92->Add( gSizer51, 1, wxEXPAND, 5 );
	
	m_staticText361 = new wxStaticText( this, wxID_ANY, _("Shadow Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText361->Wrap( -1 );
	fgSizer92->Add( m_staticText361, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxFlexGridSizer* fgSizer93;
	fgSizer93 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer93->SetFlexibleDirection( wxBOTH );
	fgSizer93->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText369 = new wxStaticText( this, wxID_ANY, _("x"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText369->Wrap( -1 );
	fgSizer93->Add( m_staticText369, 0, wxLEFT, 10 );
	
	m_staticText370 = new wxStaticText( this, wxID_ANY, _("y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText370->Wrap( -1 );
	fgSizer93->Add( m_staticText370, 0, wxLEFT, 10 );
	
	m_shadowsizex = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer93->Add( m_shadowsizex, 0, wxLEFT|wxRIGHT, 5 );
	
	m_shadowsizey = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer93->Add( m_shadowsizey, 0, wxLEFT|wxRIGHT, 5 );
	
	
	fgSizer92->Add( fgSizer93, 1, wxEXPAND, 5 );
	
	m_staticText363 = new wxStaticText( this, wxID_ANY, _("Engage Sound"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText363->Wrap( -1 );
	fgSizer92->Add( m_staticText363, 0, wxALL, 5 );
	
	m_soundengage = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_soundengage->SetToolTip( _("Sound played at the start of the fight") );
	
	fgSizer92->Add( m_soundengage, 0, wxALL, 5 );
	
	m_staticText362 = new wxStaticText( this, wxID_ANY, _("Death Sound"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText362->Wrap( -1 );
	fgSizer92->Add( m_staticText362, 0, wxALL, 5 );
	
	m_sounddeath = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_sounddeath->SetToolTip( _("Sound played when dealing the killing blow") );
	
	fgSizer92->Add( m_sounddeath, 0, wxALL, 5 );
	
	m_staticText364 = new wxStaticText( this, wxID_ANY, _("Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText364->Wrap( -1 );
	fgSizer92->Add( m_staticText364, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer94;
	fgSizer94 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer94->SetFlexibleDirection( wxBOTH );
	fgSizer94->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText373 = new wxStaticText( this, wxID_ANY, _("Idle"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText373->Wrap( -1 );
	fgSizer94->Add( m_staticText373, 0, wxLEFT, 5 );
	
	m_staticText374 = new wxStaticText( this, wxID_ANY, _("Hit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText374->Wrap( -1 );
	fgSizer94->Add( m_staticText374, 0, wxLEFT, 5 );
	
	m_staticText375 = new wxStaticText( this, wxID_ANY, _("Death"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText375->Wrap( -1 );
	fgSizer94->Add( m_staticText375, 0, wxLEFT, 5 );
	
	wxArrayString m_animidlechoiceChoices;
	m_animidlechoice = new wxChoice( this, wxID_IDLE, wxDefaultPosition, wxDefaultSize, m_animidlechoiceChoices, 0 );
	m_animidlechoice->SetSelection( 0 );
	fgSizer94->Add( m_animidlechoice, 0, wxLEFT|wxRIGHT, 5 );
	
	wxArrayString m_animhitchoiceChoices;
	m_animhitchoice = new wxChoice( this, wxID_HIT, wxDefaultPosition, wxDefaultSize, m_animhitchoiceChoices, 0 );
	m_animhitchoice->SetSelection( 0 );
	fgSizer94->Add( m_animhitchoice, 0, wxLEFT|wxRIGHT, 5 );
	
	wxArrayString m_animdeathchoiceChoices;
	m_animdeathchoice = new wxChoice( this, wxID_DEATH, wxDefaultPosition, wxDefaultSize, m_animdeathchoiceChoices, 0 );
	m_animdeathchoice->SetSelection( 0 );
	fgSizer94->Add( m_animdeathchoice, 0, wxLEFT|wxRIGHT, 5 );
	
	m_animidle = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer94->Add( m_animidle, 0, wxLEFT|wxRIGHT, 5 );
	
	m_animhit = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer94->Add( m_animhit, 0, wxLEFT|wxRIGHT, 5 );
	
	m_animdeath = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer94->Add( m_animdeath, 0, wxLEFT|wxRIGHT, 5 );
	
	m_staticText376 = new wxStaticText( this, wxID_ANY, _("Idle Alt."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText376->Wrap( -1 );
	fgSizer94->Add( m_staticText376, 0, wxLEFT, 5 );
	
	m_staticText377 = new wxStaticText( this, wxID_ANY, _("Hit Alt."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText377->Wrap( -1 );
	fgSizer94->Add( m_staticText377, 0, wxLEFT, 5 );
	
	m_staticText378 = new wxStaticText( this, wxID_ANY, _("Death Alt."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText378->Wrap( -1 );
	fgSizer94->Add( m_staticText378, 0, wxLEFT, 5 );
	
	wxArrayString m_animidlealtchoiceChoices;
	m_animidlealtchoice = new wxChoice( this, wxID_IDLEALT, wxDefaultPosition, wxDefaultSize, m_animidlealtchoiceChoices, 0 );
	m_animidlealtchoice->SetSelection( 0 );
	fgSizer94->Add( m_animidlealtchoice, 0, wxLEFT|wxRIGHT, 5 );
	
	wxArrayString m_animhitaltchoiceChoices;
	m_animhitaltchoice = new wxChoice( this, wxID_HITALT, wxDefaultPosition, wxDefaultSize, m_animhitaltchoiceChoices, 0 );
	m_animhitaltchoice->SetSelection( 0 );
	fgSizer94->Add( m_animhitaltchoice, 0, wxLEFT|wxRIGHT, 5 );
	
	wxArrayString m_animdeathaltchoiceChoices;
	m_animdeathaltchoice = new wxChoice( this, wxID_DEATHALT, wxDefaultPosition, wxDefaultSize, m_animdeathaltchoiceChoices, 0 );
	m_animdeathaltchoice->SetSelection( 0 );
	fgSizer94->Add( m_animdeathaltchoice, 0, wxLEFT|wxRIGHT, 5 );
	
	m_animidlealt = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer94->Add( m_animidlealt, 0, wxLEFT|wxRIGHT, 5 );
	
	m_animhitalt = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer94->Add( m_animhitalt, 0, wxLEFT|wxRIGHT, 5 );
	
	m_animdeathalt = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	fgSizer94->Add( m_animdeathalt, 0, wxLEFT|wxRIGHT, 5 );
	
	
	fgSizer92->Add( fgSizer94, 1, wxEXPAND, 5 );
	
	
	fgSizer96->Add( fgSizer92, 1, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	fgSizer96->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer97;
	fgSizer97 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer97->AddGrowableCol( 0 );
	fgSizer97->AddGrowableRow( 1 );
	fgSizer97->SetFlexibleDirection( wxBOTH );
	fgSizer97->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	fgSizer97->SetMinSize( wxSize( 310,-1 ) ); 
	m_staticText379 = new wxStaticText( this, wxID_ANY, _("Attack Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText379->Wrap( -1 );
	fgSizer97->Add( m_staticText379, 0, wxALL, 5 );
	
	m_animscrolled = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_animscrolled->SetScrollRate( 5, 5 );
	m_animsizer = new wxFlexGridSizer( 0, 3, 0, 0 );
	m_animsizer->AddGrowableCol( 0 );
	m_animsizer->SetFlexibleDirection( wxBOTH );
	m_animsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	m_animscrolled->SetSizer( m_animsizer );
	m_animscrolled->Layout();
	m_animsizer->Fit( m_animscrolled );
	fgSizer97->Add( m_animscrolled, 1, wxEXPAND | wxALL, 5 );
	
	m_attackanimadd = new wxButton( this, wxID_ANIM, _("Add Animation"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	fgSizer97->Add( m_attackanimadd, 0, wxALL, 5 );
	
	
	fgSizer96->Add( fgSizer97, 1, wxEXPAND, 5 );
	
	
	fgSizer33->Add( fgSizer96, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer931;
	bSizer931 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer931->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer33->Add( bSizer931, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer33 );
	this->Layout();
	fgSizer33->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_animidlechoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animhitchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animdeathchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animidlealtchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animhitaltchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animdeathaltchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_attackanimadd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnemyResourceWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnemyResourceWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnemyResourceWindow::OnButtonClick ), NULL, this );
}

EnemyResourceWindow::~EnemyResourceWindow()
{
	// Disconnect Events
	m_animidlechoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animhitchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animdeathchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animidlealtchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animhitaltchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_animdeathaltchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EnemyResourceWindow::OnAnimChoice ), NULL, this );
	m_attackanimadd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnemyResourceWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnemyResourceWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( EnemyResourceWindow::OnButtonClick ), NULL, this );
	
}

PreferencesWindow::PreferencesWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );
	
	m_auinotebook2 = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_FIXED_WIDTH );
	m_panel17 = new wxPanel( m_auinotebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer18->AddGrowableCol( 1 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText90 = new wxStaticText( m_panel17, wxID_ANY, _("Game Window Color"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText90->Wrap( -1 );
	fgSizer18->Add( m_staticText90, 0, wxALL, 5 );
	
	wxString m_gamewindowcolorChoices[] = { _("Normal"), _("Classic") };
	int m_gamewindowcolorNChoices = sizeof( m_gamewindowcolorChoices ) / sizeof( wxString );
	m_gamewindowcolor = new wxChoice( m_panel17, wxID_WINCOLOR, wxDefaultPosition, wxDefaultSize, m_gamewindowcolorNChoices, m_gamewindowcolorChoices, 0 );
	m_gamewindowcolor->SetSelection( 0 );
	fgSizer18->Add( m_gamewindowcolor, 1, wxALL, 2 );
	
	m_staticText218 = new wxStaticText( m_panel17, wxID_ANY, _("Text Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText218->Wrap( -1 );
	fgSizer18->Add( m_staticText218, 0, wxALL, 5 );
	
	wxString m_textpreviewtypeChoices[] = { _("Raw"), _("Extended"), _("Nice") };
	int m_textpreviewtypeNChoices = sizeof( m_textpreviewtypeChoices ) / sizeof( wxString );
	m_textpreviewtype = new wxChoice( m_panel17, wxID_TEXTPREVIEW, wxDefaultPosition, wxDefaultSize, m_textpreviewtypeNChoices, m_textpreviewtypeChoices, 0 );
	m_textpreviewtype->SetSelection( 0 );
	fgSizer18->Add( m_textpreviewtype, 0, wxALL, 2 );
	
	
	bSizer42->Add( fgSizer18, 1, wxEXPAND, 5 );
	
	
	m_panel17->SetSizer( bSizer42 );
	m_panel17->Layout();
	bSizer42->Fit( m_panel17 );
	m_auinotebook2->AddPage( m_panel17, _("Common"), false, wxNullBitmap );
	m_panel172 = new wxPanel( m_auinotebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer422;
	bSizer422 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer182;
	fgSizer182 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer182->AddGrowableCol( 1 );
	fgSizer182->SetFlexibleDirection( wxBOTH );
	fgSizer182->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText334 = new wxStaticText( m_panel172, wxID_ANY, _("Game Language"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText334->Wrap( -1 );
	fgSizer182->Add( m_staticText334, 0, wxALL, 5 );
	
	wxString m_steamlanguageChoices[] = { _("English (US)"), _("English (UK)"), _("Japanese"), _("German"), _("French"), _("Italian"), _("Spanish") };
	int m_steamlanguageNChoices = sizeof( m_steamlanguageChoices ) / sizeof( wxString );
	m_steamlanguage = new wxChoice( m_panel172, wxID_STEAMLANG, wxDefaultPosition, wxDefaultSize, m_steamlanguageNChoices, m_steamlanguageChoices, 0 );
	m_steamlanguage->SetSelection( 0 );
	fgSizer182->Add( m_steamlanguage, 0, wxALL, 2 );
	
	m_staticText336 = new wxStaticText( m_panel172, wxID_ANY, _("Single Language Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText336->Wrap( -1 );
	fgSizer182->Add( m_staticText336, 0, wxALL, 5 );
	
	m_steamsinglelanguage = new wxCheckBox( m_panel172, wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsinglelanguage->SetToolTip( _("Warning!\nWhen this option is enabled, Steam games will load faster\nHowever, you can not change the language setting anymore\nas long as the game is opened") );
	
	fgSizer182->Add( m_steamsinglelanguage, 0, wxALL, 5 );
	
	m_staticText335 = new wxStaticText( m_panel172, wxID_ANY, _("Saved Languages"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText335->Wrap( -1 );
	fgSizer182->Add( m_staticText335, 0, wxALL, 5 );
	
	wxGridSizer* gSizer38;
	gSizer38 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_steamsaveus = new wxCheckBox( m_panel172, wxID_ANY, _("English (US)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsaveus->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsaveus, 0, wxALL, 2 );
	
	m_steamsaveuk = new wxCheckBox( m_panel172, wxID_ANY, _("English (UK)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsaveuk->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsaveuk, 0, wxALL, 2 );
	
	m_steamsavejap = new wxCheckBox( m_panel172, wxID_ANY, _("Japanese"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsavejap->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsavejap, 0, wxALL, 2 );
	
	m_steamsaveger = new wxCheckBox( m_panel172, wxID_ANY, _("German"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsaveger->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsaveger, 0, wxALL, 2 );
	
	m_steamsavefr = new wxCheckBox( m_panel172, wxID_ANY, _("French"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsavefr->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsavefr, 0, wxALL, 2 );
	
	m_steamsaveit = new wxCheckBox( m_panel172, wxID_ANY, _("Italian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsaveit->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsaveit, 0, wxALL, 2 );
	
	m_steamsavesp = new wxCheckBox( m_panel172, wxID_ANY, _("Spanish"), wxDefaultPosition, wxDefaultSize, 0 );
	m_steamsavesp->SetToolTip( _("Languages to save in .hws mods, file batches\nand when generating Steam Mod") );
	
	gSizer38->Add( m_steamsavesp, 0, wxALL, 2 );
	
	
	fgSizer182->Add( gSizer38, 1, wxEXPAND, 5 );
	
	m_staticText901 = new wxStaticText( m_panel172, wxID_ANY, _("Background Resolution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText901->Wrap( -1 );
	fgSizer182->Add( m_staticText901, 0, wxALL, 5 );
	
	m_backgroundresolution = new wxSpinCtrl( m_panel172, wxID_RESOLUTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 16, 123, 32 );
	m_backgroundresolution->SetToolTip( _("Expected resolution of the Field backgrounds\n32 is the default Steam resolution\nOnly a background mod can increase it") );
	
	fgSizer182->Add( m_backgroundresolution, 0, wxALL, 2 );
	
	
	bSizer422->Add( fgSizer182, 1, wxEXPAND, 5 );
	
	
	m_panel172->SetSizer( bSizer422 );
	m_panel172->Layout();
	bSizer422->Fit( m_panel172 );
	m_auinotebook2->AddPage( m_panel172, _("Steam"), false, wxNullBitmap );
	m_panel171 = new wxPanel( m_auinotebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer421;
	bSizer421 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer181;
	fgSizer181 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer181->AddGrowableCol( 1 );
	fgSizer181->SetFlexibleDirection( wxBOTH );
	fgSizer181->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText892 = new wxStaticText( m_panel171, wxID_ANY, _("Game Alphabet"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText892->Wrap( -1 );
	fgSizer181->Add( m_staticText892, 0, wxALL, 5 );
	
	wxArrayString m_gamealphabetChoices;
	m_gamealphabet = new wxChoice( m_panel171, wxID_ALPHABET, wxDefaultPosition, wxDefaultSize, m_gamealphabetChoices, 0 );
	m_gamealphabet->SetSelection( 0 );
	fgSizer181->Add( m_gamealphabet, 1, wxALL, 2 );
	
	m_staticText8911 = new wxStaticText( m_panel171, wxID_ANY, _("Opcode Character"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8911->Wrap( -1 );
	fgSizer181->Add( m_staticText8911, 0, wxALL, 5 );
	
	m_opcodechar = new wxTextCtrl( m_panel171, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_opcodechar->HasFlag( wxTE_MULTILINE ) )
	{
	m_opcodechar->SetMaxLength( 1 );
	}
	#else
	m_opcodechar->SetMaxLength( 1 );
	#endif
	fgSizer181->Add( m_opcodechar, 0, wxALL, 2 );
	
	
	bSizer421->Add( fgSizer181, 1, wxEXPAND, 5 );
	
	
	m_panel171->SetSizer( bSizer421 );
	m_panel171->Layout();
	bSizer421->Fit( m_panel171 );
	m_auinotebook2->AddPage( m_panel171, _("PSX"), false, wxNullBitmap );
	
	bSizer34->Add( m_auinotebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer62;
	bSizer62 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer62->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer62->Add( m_buttonok, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizer34->Add( bSizer62, 0, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( bSizer34 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_steamlanguage->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PreferencesWindow::OnSteamLanguageChange ), NULL, this );
	m_steamsinglelanguage->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PreferencesWindow::OnSingleLanguageMode ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesWindow::OnButtonClick ), NULL, this );
}

PreferencesWindow::~PreferencesWindow()
{
	// Disconnect Events
	m_steamlanguage->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PreferencesWindow::OnSteamLanguageChange ), NULL, this );
	m_steamsinglelanguage->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PreferencesWindow::OnSingleLanguageMode ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesWindow::OnButtonClick ), NULL, this );
	
}

IOHWSWindow::IOHWSWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer3;
	gbSizer3 = new wxGridBagSizer( 0, 0 );
	gbSizer3->SetFlexibleDirection( wxBOTH );
	gbSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer( wxVERTICAL );
	
	m_hwspicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("Hades Workshop Save (*.hws)|*.hws"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_USE_TEXTCTRL );
	bSizer52->Add( m_hwspicker, 0, wxALL|wxEXPAND, 5 );
	
	
	gbSizer3->Add( bSizer52, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_spell = new wxCheckBox( this, wxID_SPELL, _("Spells"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spell->SetValue(true); 
	bSizer31->Add( m_spell, 0, wxALL, 3 );
	
	m_support = new wxCheckBox( this, wxID_SUPPORT, _("Supporting Abilities"), wxDefaultPosition, wxDefaultSize, 0 );
	m_support->SetValue(true); 
	bSizer31->Add( m_support, 0, wxALL, 3 );
	
	m_command = new wxCheckBox( this, wxID_CMD, _("Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	m_command->SetValue(true); 
	bSizer31->Add( m_command, 0, wxALL, 3 );
	
	m_stat = new wxCheckBox( this, wxID_STAT, _("Stats"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stat->SetValue(true); 
	bSizer31->Add( m_stat, 0, wxALL, 3 );
	
	m_partyspecial = new wxCheckBox( this, wxID_PARTY_SPECIAL, _("Party Special"), wxDefaultPosition, wxDefaultSize, 0 );
	m_partyspecial->SetValue(true); 
	bSizer31->Add( m_partyspecial, 0, wxALL, 3 );
	
	m_item = new wxCheckBox( this, wxID_ITEM, _("Items"), wxDefaultPosition, wxDefaultSize, 0 );
	m_item->SetValue(true); 
	bSizer31->Add( m_item, 0, wxALL, 3 );
	
	m_shop = new wxCheckBox( this, wxID_SHOP, _("Shops"), wxDefaultPosition, wxDefaultSize, 0 );
	m_shop->SetValue(true); 
	bSizer31->Add( m_shop, 0, wxALL, 3 );
	
	m_enemy = new wxCheckBox( this, wxID_ENMY, _("Enemies"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemy->SetValue(true); 
	bSizer31->Add( m_enemy, 0, wxALL, 3 );
	
	m_card = new wxCheckBox( this, wxID_CARD, _("Tetra Master"), wxDefaultPosition, wxDefaultSize, 0 );
	m_card->SetValue(true); 
	bSizer31->Add( m_card, 0, wxALL, 3 );
	
	m_text = new wxCheckBox( this, wxID_TEXT, _("Texts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_text->SetValue(true); 
	bSizer31->Add( m_text, 0, wxALL, 3 );
	
	m_world = new wxCheckBox( this, wxID_WORLD, _("World Maps"), wxDefaultPosition, wxDefaultSize, 0 );
	m_world->SetValue(true); 
	bSizer31->Add( m_world, 0, wxALL, 3 );
	
	m_field = new wxCheckBox( this, wxID_FIELD, _("Fields"), wxDefaultPosition, wxDefaultSize, 0 );
	m_field->SetValue(true); 
	bSizer31->Add( m_field, 0, wxALL, 3 );
	
	m_scene = new wxCheckBox( this, wxID_SCENE, _("Battle Scenes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_scene->SetValue(true); 
	bSizer31->Add( m_scene, 0, wxALL, 3 );
	
	m_spellanim = new wxCheckBox( this, wxID_SPELL_ANIM, _("Spell Animations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spellanim->SetValue(true); 
	bSizer31->Add( m_spellanim, 0, wxALL, 3 );
	
	m_ffui = new wxCheckBox( this, wxID_MENU_UI, _("Menu UI"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ffui->SetValue(true); 
	bSizer31->Add( m_ffui, 0, wxALL, 3 );
	
	m_assembly = new wxCheckBox( this, wxID_ASSEMBLY, _("Assembly Code"), wxDefaultPosition, wxDefaultSize, 0 );
	m_assembly->SetValue(true); 
	bSizer31->Add( m_assembly, 0, wxALL, 3 );
	
	
	gbSizer3->Add( bSizer31, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer54;
	bSizer54 = new wxBoxSizer( wxVERTICAL );
	
	m_spelltext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_spelltext->SetValue(true); 
	bSizer54->Add( m_spelltext, 0, wxALL, 3 );
	
	m_supporttext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_supporttext->SetValue(true); 
	bSizer54->Add( m_supporttext, 0, wxALL, 3 );
	
	m_commandtext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_commandtext->SetValue(true); 
	bSizer54->Add( m_commandtext, 0, wxALL, 3 );
	
	m_stattext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stattext->SetValue(true); 
	bSizer54->Add( m_stattext, 0, wxALL, 3 );
	
	m_staticText10514 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10514->Wrap( -1 );
	bSizer54->Add( m_staticText10514, 0, wxALL, 3 );
	
	m_itemtext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_itemtext->SetValue(true); 
	bSizer54->Add( m_itemtext, 0, wxALL, 3 );
	
	m_staticText1051 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1051->Wrap( -1 );
	bSizer54->Add( m_staticText1051, 0, wxALL, 3 );
	
	m_enemytext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemytext->SetValue(true); 
	bSizer54->Add( m_enemytext, 0, wxALL, 3 );
	
	m_cardtext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cardtext->SetValue(true); 
	bSizer54->Add( m_cardtext, 0, wxALL, 3 );
	
	m_staticText10511 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10511->Wrap( -1 );
	bSizer54->Add( m_staticText10511, 0, wxALL, 3 );
	
	m_worldtext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_worldtext->SetValue(true); 
	bSizer54->Add( m_worldtext, 0, wxALL, 3 );
	
	m_fieldtext = new wxCheckBox( this, wxID_ANY, _("including text"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fieldtext->SetValue(true); 
	bSizer54->Add( m_fieldtext, 0, wxALL, 3 );
	
	m_staticText10512 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10512->Wrap( -1 );
	bSizer54->Add( m_staticText10512, 0, wxALL, 3 );
	
	m_staticText10513 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10513->Wrap( -1 );
	bSizer54->Add( m_staticText10513, 0, wxALL, 3 );
	
	m_staticText10515 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10515->Wrap( -1 );
	bSizer54->Add( m_staticText10515, 0, wxALL, 3 );
	
	
	gbSizer3->Add( bSizer54, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer146;
	bSizer146 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText273 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText273->Wrap( -1 );
	bSizer146->Add( m_staticText273, 0, wxALL, 3 );
	
	m_staticText274 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText274->Wrap( -1 );
	bSizer146->Add( m_staticText274, 0, wxALL, 3 );
	
	m_staticText275 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText275->Wrap( -1 );
	bSizer146->Add( m_staticText275, 0, wxALL, 3 );
	
	m_staticText276 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText276->Wrap( -1 );
	bSizer146->Add( m_staticText276, 0, wxALL, 3 );
	
	m_staticText277 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText277->Wrap( -1 );
	bSizer146->Add( m_staticText277, 0, wxALL, 3 );
	
	m_staticText278 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText278->Wrap( -1 );
	bSizer146->Add( m_staticText278, 0, wxALL, 3 );
	
	m_staticText279 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText279->Wrap( -1 );
	bSizer146->Add( m_staticText279, 0, wxALL, 1 );
	
	m_enemylocal = new wxCheckBox( this, wxID_ANY, _("including locals"), wxDefaultPosition, wxDefaultSize, 0 );
	m_enemylocal->SetValue(true); 
	bSizer146->Add( m_enemylocal, 0, wxALL, 3 );
	
	m_staticText280 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText280->Wrap( -1 );
	bSizer146->Add( m_staticText280, 0, wxBOTTOM, 5 );
	
	m_staticText281 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText281->Wrap( -1 );
	bSizer146->Add( m_staticText281, 0, wxALL, 2 );
	
	m_worldlocal = new wxCheckBox( this, wxID_ANY, _("including locals"), wxDefaultPosition, wxDefaultSize, 0 );
	m_worldlocal->SetValue(true); 
	bSizer146->Add( m_worldlocal, 0, wxALL, 3 );
	
	m_fieldlocal = new wxCheckBox( this, wxID_ANY, _("including locals"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fieldlocal->SetValue(true); 
	bSizer146->Add( m_fieldlocal, 0, wxALL, 3 );
	
	
	gbSizer3->Add( bSizer146, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxPoint( -1,-1 ), wxSize( 100,30 ), 0 );
	bSizer33->Add( m_buttoncancel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, wxEmptyString, wxDefaultPosition, wxSize( 100,30 ), 0 );
	bSizer33->Add( m_buttonok, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	gbSizer3->Add( bSizer33, wxGBPosition( 2, 1 ), wxGBSpan( 1, 2 ), wxALIGN_RIGHT, 5 );
	
	
	bSizer29->Add( gbSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer29 );
	this->Layout();
	bSizer29->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_hwspicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( IOHWSWindow::OnChangeFile ), NULL, this );
	m_spell->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_support->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_command->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_stat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_item->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_enemy->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_card->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_world->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_field->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
}

IOHWSWindow::~IOHWSWindow()
{
	// Disconnect Events
	m_hwspicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( IOHWSWindow::OnChangeFile ), NULL, this );
	m_spell->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_support->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_command->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_stat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_item->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_enemy->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_card->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_world->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	m_field->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( IOHWSWindow::OnCheckSection ), NULL, this );
	
}

ExportPPFWindow::ExportPPFWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer3;
	gbSizer3 = new wxGridBagSizer( 10, 0 );
	gbSizer3->SetFlexibleDirection( wxBOTH );
	gbSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	bSizer31->SetMinSize( wxSize( 340,-1 ) ); 
	m_ppfpicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Save..."), wxT("Playstation Patch (*.ppf)|*.ppf"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	bSizer31->Add( m_ppfpicker, 0, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_undo = new wxCheckBox( this, wxID_ANY, _("Add an undo feature"), wxDefaultPosition, wxDefaultSize, 0 );
	m_undo->SetToolTip( _("Make so the patch can be unapplied\nbut quiet double the ppf's size") );
	
	bSizer31->Add( m_undo, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_blockcheck = new wxCheckBox( this, wxID_ANY, _("Add a block check for safety"), wxDefaultPosition, wxDefaultSize, 0 );
	m_blockcheck->SetValue(true); 
	m_blockcheck->SetToolTip( _("This prevents the patch to be mistakenly\napplied to a wrong file") );
	
	bSizer31->Add( m_blockcheck, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	
	gbSizer3->Add( bSizer31, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Description") ), wxVERTICAL );
	
	m_description = new wxTextCtrl( sbSizer1->GetStaticBox(), wxID_ANY, _("Generated by Hades Workshop"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_description->HasFlag( wxTE_MULTILINE ) )
	{
	m_description->SetMaxLength( 50 );
	}
	#else
	m_description->SetMaxLength( 50 );
	#endif
	sbSizer1->Add( m_description, 0, wxALL|wxEXPAND, 0 );
	
	
	gbSizer3->Add( sbSizer1, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxPoint( -1,-1 ), wxSize( 100,30 ), 0 );
	bSizer33->Add( m_buttoncancel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Export"), wxDefaultPosition, wxSize( 100,30 ), 0 );
	bSizer33->Add( m_buttonok, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	gbSizer3->Add( bSizer33, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	bSizer29->Add( gbSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer29 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

ExportPPFWindow::~ExportPPFWindow()
{
}

SaveSteamWindow::SaveSteamWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridBagSizer* gbSizer3;
	gbSizer3 = new wxGridBagSizer( 10, 0 );
	gbSizer3->SetFlexibleDirection( wxBOTH );
	gbSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	m_dirpicker = new wxDirPickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL );
	bSizer31->Add( m_dirpicker, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText309 = new wxStaticText( this, wxID_ANY, _("Choose or create the folder where to create modded files. Then replace the game's Steam files by the corresponding modded files created."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText309->Wrap( 260 );
	bSizer31->Add( m_staticText309, 0, wxALL, 5 );
	
	m_staticText3051 = new wxStaticText( this, wxID_ANY, _("Warning: Never modify or replace Steam files in the folders opened by Hades Workshop while it is running."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3051->Wrap( 260 );
	m_staticText3051->SetForegroundColour( wxColour( 99, 20, 20 ) );
	
	bSizer31->Add( m_staticText3051, 0, wxALL, 5 );
	
	
	gbSizer3->Add( bSizer31, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), 0, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxPoint( -1,-1 ), wxSize( 100,30 ), 0 );
	bSizer33->Add( m_buttoncancel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxSize( 100,30 ), 0 );
	bSizer33->Add( m_buttonok, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	gbSizer3->Add( bSizer33, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( gbSizer3 );
	this->Layout();
	gbSizer3->Fit( this );
	
	this->Centre( wxBOTH );
}

SaveSteamWindow::~SaveSteamWindow()
{
}

AboutWindow::AboutWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,390 ), wxDefaultSize );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl13 = new wxTextCtrl( this, wxID_ANY, _("Hades Workshop v0.39b\nMade by Tirlititi\n\nThe newer versions are available at\nhttp://forums.qhimm.com/index.php?topic=14315\n\nCredits and Thanks :\nIcarus/Paradox for ppf support\nZidane_2 for PSX model and texture exporter\nyaz0r for informations and ideas on scripts\nFroggy25 for informations about MIPS\nCecil-Master's team for informations about CIL\n\nThe Qhimm's forum members, especially\n - LandonRayW -\n - JBedford128 -\n - Zande -\n - Thisguyaresick2 -\n - Yugisokubodai -\n - Maki -\n - Satoh -\nThe Final Fantasy Wikia\nand some Gamefaqs's contributors, especially\n - Rebirth Flame -\n - S. Volo -\n\nLoading Screen by Maxa'\nhttp://maxa-art.deviantart.com/\n\nYou can e-mail me at\nlaroche.clement1@gmail.com"), wxDefaultPosition, wxSize( -1,330 ), wxTE_CENTRE|wxTE_MULTILINE|wxTE_READONLY|wxSIMPLE_BORDER );
	m_textCtrl13->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	m_textCtrl13->SetMinSize( wxSize( -1,330 ) );
	
	bSizer16->Add( m_textCtrl13, 1, wxALL|wxEXPAND, 5 );
	
	m_button18 = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_button18, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	this->SetSizer( bSizer16 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

AboutWindow::~AboutWindow()
{
}

HelpWindow::HelpWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	m_helpbook = new wxListbook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT );
	m_panel171 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer45321;
	bSizer45321 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl174 = new wxTextCtrl( m_panel171, wxID_ANY, _("Hades Workshop is a tool that allows you to edit a Final Fantasy IX Playstation CD image and Steam files. Playstation CD images can be retrieved with an ISO extractor but the manipulation for this won't be explained here.\n\nNote that the edition of Steam files is recent. Several features don't work with them and this help has not been updated. Several facts presented here only apply to the PSX version of the game.\n\nTo edit a file, select \"Open\" in the \"File\" menu and choose a Final Fantasy IX file. If you open that file for the first time, the program will ask you to perform a scan : it will make sure the file is correct and create another file with the same name and the extension \".hwf\" containing informations on the Final Fantasy IX file.\nThis scan takes some time, usually less than 1 minute.\n\nOnce the scan has been done, the file datas are read and displayed in several sections. Note that everytime you get to a new section, the Final Fantasy IX file is read (some sections take quite a bit of time). It is extremely dangerous to edit the file with another program at the same time.\n\nTo save the file, you have 3 different options :\n1) You can save a mod file (\"Save Mod\"). This file, with the extension \".hws\" contains every modifications you made with the program. They are not version-dependant and can be used to transfer modification from a disc to another disc. This is the most prefered way of saving modifications when you don't want to apply them at once.\n2) Create a Playstation Patch (\"Export as PPF\"). This file, with the extension \".ppf\" also contains the modifications you made with the program. The difference is that patches are more standard and can be applied on the Final Fantasy IX file by programs like ppf-o-matic. This is the most prefered way of saving when you want to share your modifications.\n3) Directly overwrite the file (\"Overwrite Binary File\"). The file you opened is overwritten with the changes you made. It allows you to directly apply the modifications to your game, but the process can't be undone. This is the most prefered way of saving when you want to test your modifications and have a backup of the original file.\n\nNote that, depending on what you edited, emulators' save states may not work properly."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl174->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer45321->Add( m_textCtrl174, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel171->SetSizer( bSizer45321 );
	m_panel171->Layout();
	bSizer45321->Fit( m_panel171 );
	m_helpbook->AddPage( m_panel171, _("Getting Started"), true );
	m_panel17 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4532;
	bSizer4532 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl17 = new wxTextCtrl( m_panel17, wxID_ANY, _("\"Animation\" is the name of a set used for spells.\nThis set includes :\n- the spell's special effects,\n- a sequence of animations for the caster and the targets,\n- a sequence of camera movements,\n- the \"damage point\", the moment the target is dealt damage or status alteration,\n- the \"number point\", the moment the damage are displayed by numbers floating over the target,\n- the \"return point\", the moment when the battle resumes so the next action may happen.\n\nDepending on what the animation is intended to do, some of the previous properties may be incomplete or missing.\n\nMost of the boss' ending moves are meant to be the final moves of a battle. Thus there is no \"return point\" for most of them (that unfortunatly includes Kuja's Ultima). When these animations are run under normal circumstances, the battle never resumes and the player will be stuck.\n\nSome of the enemies' animations are only meant to be used by enemies. Thus there may be camera movements or special effects not displayed properly. For instance, Trolls are moving in front of their target and give it an attack when doing their Solution spell, but that won't be executed if it's not a Troll using that animation.\n\nAnd last but not least, animations are usually meant for either 1, multiple target or global targeting. Those meant for global targeting (such as Night) may be used for anything else, even though the target won't always be identified by the special effect. However, animations made for 1 target cannot be used for more of them. They'll just won't affect more than 1 of the spell's target. About multi-targeting animations, they all may be used for a single-targeting use and some may be used for a global use, but the others are limited to up to 4 targets at a time. It is the case, for instance, of Amarant's No Mercy (the Trance version) : the caster will only fire up to 4 projectils. Note that Reflect refreshes this countdown."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl17->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer4532->Add( m_textCtrl17, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel17->SetSizer( bSizer4532 );
	m_panel17->Layout();
	bSizer4532->Fit( m_panel17 );
	m_helpbook->AddPage( m_panel17, _("Spell Animations"), false );
	m_panel18 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer45312;
	bSizer45312 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl171 = new wxTextCtrl( m_panel18, wxID_ANY, _("The \"Effect\" is a spell's or attack's action made on its target.\nIt is also the field that determines how some of the other fields are handled.\n\nA list of effects that support elemental attacks and status alteration will be available here as soon as possible."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl171->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer45312->Add( m_textCtrl171, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel18->SetSizer( bSizer45312 );
	m_panel18->Layout();
	bSizer45312->Fit( m_panel18 );
	m_helpbook->AddPage( m_panel18, _("Spell Effects"), false );
	m_panel19 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer453112;
	bSizer453112 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl172 = new wxTextCtrl( m_panel19, wxID_ANY, _("Initial and Auto statuses are not used extensively on enemies in the normal version of the game. There are some weird things happening when enemies are given one.\n\n[General]\nWith the exceptions of \"Doom\" and \"Gradual Petrify\", the battles are not won because of an auto/initial status.\nWhen a status can't be cured, it means it can't be cured by any way (spells, items, time, physical attacks...). They might be cured when the enemy dies but I didn't investigate it (Sand Golem is the only enemy I know that can actually be revived).\nColor changing is quiet bugged. It doesn't change in the right color most of the time.\n\n[Blind - Reflect - Silence]\nInit : status is applied at the beginning, works fine and can be cured.\nAuto : status is applied at the beginning, works fine and can't be cured.\n\n[Trouble - Confuse - Stop - Defend - Zombie - Virus (remove Evasion)]\nInit : status is applied at the beginning, works fine and can be cured.\nInit + Auto : status is applied at the beginning, works fine and can't be cured.\n\n[Shell - Protect]\nInit + Auto : status is applied at the beginning, works fine and can't be cured.\n\n[Haste - Slow]\nAuto : displays the SFX but doesn't do anything else.\n\n[Sleep - Float]\nInit : status applied but cured immediately.\nAuto : status is applied at the beginning, works fine and can't be cured.\n\n[Berserk]\nInit : status is applied at the beginning, works fine and can be cured.\nAuto : status is applied at the beginning, works fine, can't be cured but doesn't change the color.\nInit + Auto : status is applied at the beginning, works fine and can't be cured.\n\n[Regen]\nInit : status applied but cured immediately (regen HP once).\nAuto : status is applied at the beginning, works fine and can't be cured.\n\n[Poison]\nInit : status applied but cured immediately (deals damage once).\nAuto : displays the SFX but doesn't do anything else.\nInit + Auto : status is applied at the beginning, deals damage faster than usual and can't be cured.\n\n[Venom]\nInit : status is applied at the beginning, works fine and can be cured.\nAuto : status can't be cured but only disables the attacks.\nInit + Auto : status is applied at the beginning, deals damage faster than usual and can't be cured.\n\n[Vanish]\nInit + Auto : status is applied at the beginning, works fine, can't be cured but doesn't make invisible.\n\n[Mini]\nInit : status is applied at the beginning, works fine and can be cured. The enemy is however still normal-sized until it is hit.\nInit + Auto : status is applied at the beginning, works fine and can't be cured. The enemy is however still normal-sized until it is hit.\n\n[Doom]\nInit or Auto : does instant death (with \"Death\" message displayed).\n\n[Gradual Petrify]\nInit : status is applied with a very fast countdown. Petrify in less than 5 seconds with any spirit value.\n\n[Trance]\nInit : makes the enemy shine and uses an attack countdown. The enemy stops shining and the battle bugs when the trance ends (the fighter who should attack next is stucked and can't do anything until he dies but the others keep fighting).\nInit + Auto : makes the enemy shine and can't be cured.\n\n[Heat]\nInit : status applied (color change) but cured immediately.\nAuto : displays the SFX without changing the color but doesn't do anything else.\nInit + Auto : status is applied at the beginning, works fine and can't be cured.\n\n[Freeze]\nInit : status applied (color change) but cured immediately.\nAuto : displays the SFX without changing the color and only disables the attacks.\nInit + Auto : status is applied at the beginning, works fine and can't be cured.\n\n[Death]\nInit : the enemy has greyed name and can't be targeted (or cancel the spells targeting it). Game crashs if the battle ends (if you run away...).\nAuto : the enemy has greyed name and stops attacking when its HP reach 0 (and keep displaying its death animation everytime it's dealt damage) until it's healed.\n\n[Unknown (may be KO)]\nSimilar to \"Death\" but doesn't grey the name and disables the enemy's animations when it's hitted."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl172->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer453112->Add( m_textCtrl172, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel19->SetSizer( bSizer453112 );
	m_panel19->Layout();
	bSizer453112->Fit( m_panel19 );
	m_helpbook->AddPage( m_panel19, _("Enemies' Statuses"), false );
	m_panel26 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl28 = new wxTextCtrl( m_panel26, wxID_ANY, _("Battle Scene's models can be exported and replaced by new models. It is, however, not that simple.\n\nExporting a model creates a Wavefront .obj geometry, several Targa .tga textures and a material library .mtl. External programs like Blender can be used to edit those.\n\nImporting a model is more complex. I recommand you to follow these steps :\n1) When you create/edit your model, only use 256x256 textures (merge them if needed ; you'll be limited to 3 of those textures),\n2) In Hades Workshop, select the scene you want to replace with your custom model,\n3) Click on the \"Manage Textures\" button,\n4) Browse for a texture your custom model uses,\n5) Place it at the left of the picture,\n6) Click on the \"Import\" button,\n7) Do steps 4 to 6 for each textures you need, placing them on the middle and on the right of the picture,\n8) Delete the palettes you don't use (right-click on them then \"Remove\"),\n9) Click on the \"Ok\" button,\n10) Edit the textures such as you end up with as many as you use for your custom model, with the right palette associated to the right painting (right-click on them to add or remove some),\n11) Click on the \"Import Battle Scene\" button and choose your geometry file .obj,\n12) Once it is imported, link the model texture's identifiers to the textures you just set up,\n13) You are done, congrats !\n\nNote that, due to the format limitation, models you import may not be far more bigger than the standart model."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl28->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer71->Add( m_textCtrl28, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel26->SetSizer( bSizer71 );
	m_panel26->Layout();
	bSizer71->Fit( m_panel26 );
	m_helpbook->AddPage( m_panel26, _("Models"), false );
	m_panel201 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer45311111;
	bSizer45311111 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl1731 = new wxTextCtrl( m_panel201, wxID_ANY, _("The script in this section is the game's code, that handles scripted events, enemy AIs and other related stuffs.\n\nThe script is presented with a kind of C-like syntax. There are several differences though.\n\n[General]\nThe language accepts only one instruction per line.\nLines are not terminated by any punctuation character.\nFor flow control statements, the braces are mandatory.\nYou can't use braces out of a flow control statement.\nThere is no such thing as real values. Everything is done using integers (sometimes signed).\n\nOnce you have modified a function's script, you must parse it to check eventual errors.\n\n[Flow Control]\nThe different keywords for controlling the script's flow are described here.\n\nif : usual If/Then statement with an optional Else. The syntax is\n\nif ( CONDITION ) {\n    CODE\n}\n\nor\n\nif ( CONDITION ) {\n    CODE_A\n} else {\n    CODE_B\n}\n\nNote that the opening braces must be on the same line as the keywords \"if\" and \"else\".\n\nifnot : opposite control of if. It works similarly though. The syntax is\n\nifnot ( CONDITION ) {\n    CODE\n}\n\nwhile : usual While statement. You can use the keyword \"break\" to leave the control but there is no such thing as a \"continue\" instruction. The syntax is\n\nwhile ( CONDITION ) {\n    CODE\n}\n\ndo/while : usual Do/While statement. You can also use the keyword \"break\" to leave the control. The syntax is\n\ndo {\n    CODE\n} while ( CONDITION )\n\nswitch : a Switch statement with consecutive cases only. The first case's value is specified in the control, as well as the maximal amount of cases. The syntax is\n\nswitch NB ( VALUE ) from FIRST {\ncase +INC:\n    CODE_A\ncase +INC ; +INC:\n    CODE_B\ndefault:\n    CODE_C\n}\n\nwhere\nNB is the maximal amount of cases,\nVALUE is the control expression,\nFIRST is the \"+0\" case,\nINC are numbers strictly lower than NB.\nNote that, like in C, you have to use the keyword \"break\" if you want only one branch to be run and that the \"default\" case is optional.\nThe semicolon allows to make several values lead to the same code branch.\n\nswitchex : a more usual Switch statement. The exact amount of cases (default case excluded) must be provided in the control. The syntax is\n\nswitchex NB ( VALUE ) {\ncase X:\n    CODE_A\ncase Y ; Z:\n    CODE_B\ndefault:\n    CODE_C\n}\n\nNB being the amount of cases and X, Y and Z representing numbers. The keywords \"break\", \"default\" and the semicolon have the same purpose than for the \"switch\" statement.\n\nloop : rerun the whole function. It works as an alternative to a last \"return\" instruction and can only be used out of any other control block.\n\n[Variables]\nVariables are splitted into 3 categories (locals, globals and generals) and can be of 9 different types (Bool, SBool, Int8, UInt8, Int16, UInt16, Int24, UInt24 and Null).\n1) Local variables must be allocated for each entry of the script codes. They are local to that entry and are initialized to 0 when the script is executed (at the start of a battle or when the player enters a field),\n2) Global variables are shared by all the entries of a single script. They are thus not shared between several fields' scripts or battles. They are also initialized to 0 when the script is executed.\n3) General variables are permanent and can be referred in any script of the game. In adition, several of them (maybe all of them) are saved in memory card saves.\n\nThose three categories of variables each use their own RAM allocated space, and two variables of the same category use the same RAM space. The offset (or adress) of the variables are appended to their name.\nSo \"VAR_LocInt8_10\" and \"VAR_LocUInt8_10\" are the same variable (typecasted into an int of size 1 byte or an unsigned int of size 1 byte).\nHowever, \"VAR_LocInt8_10\", \"VAR_LocInt8_11\" and \"VAR_GlobInt8_10\" are all different variables.\nAlso, VAR_LocUInt16_10 == VAR_LocUInt8_10 | (VAR_LocUInt16_11 << 8).\nFinally, the offsets of boolean variables are their bit-offset and not byte-offset. For instance, VAR_LocBool_80 == VAR_LocUInt8_10 & 1.\n\nVariables are renamed with more suitable names when it's possible.\nMod files (.hws) can also save the names of local and global variables.\n\nAbout the types, note that SBool and Bool are synonyms, as well as UInt24 and Int24 (they both are signed because of an oversight from the developpers).\nAccessing to a Null type variables always return 0 and setting a Null type variable does nothing.\n\nAdditionally, some variables belong to special categories, used by the deep game's mechanics. \"SV\" variables, for instance, are extensively used by the battle mechanics.\nTo finish, there are also Getters to retrieve values from the other game's mechanics and those can't be directly modified (such as the player's gils amount).\n\nIn order to manipulate variables, you must use the instruction \"set\" followed by a series of operations over the variables. A few of those operations are still unknown. Informations about them can be found in the script's help window.\nThe brackets allow to access a structure's field. The result they send is a list (see the list operators in the function list).\n\nWARNINGS :\n1) At the moment, the operation priorities are not respected. By default, the operations will be executed from right to left. It is advised to write all the parentheses,\n2) Note that the minus operator can't be read as an unary operator yet,\n3) Only positive integers can be used in variable manipulation codes,\n4) When dealing with numbers larger than 32767 in variable manipulation codes, you must append 'L' to it ; otherwise it will be converted to a negative integer.\n\nVariable manipulation codes can also be used in most of instructions's arguments."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl1731->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer45311111->Add( m_textCtrl1731, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel201->SetSizer( bSizer45311111 );
	m_panel201->Layout();
	bSizer45311111->Fit( m_panel201 );
	m_helpbook->AddPage( m_panel201, _("Game Script"), false );
	m_panel20 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4531111;
	bSizer4531111 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl173 = new wxTextCtrl( m_panel20, wxID_ANY, _("Editing the binary file is limited in many ways. Here is an overview of these limitations.\n\n[Texts]\nTexts are usually limited to a certain amount of characters. This amount is shared by all the texts of the same kind, meaning you can shorten some texts in order to make others longer.\n\nAbility, Command, Stat and Item texts : there are really few free characters for them (up to 3 depending on the version you have). If you're lucky, you've got a version with non-empty names for the dummy spells and these can be erased to gain space. Note that names and helps use different limitations and you can't clear text in names to add some other text in helps.\n\nEnemies texts : all the datas related to a single battle use the same limitation. If you write a longer name for an enemy's attack, its name character limit will decrease by the same amount. Fortunatly, there is much more initial free characters for them by default (from 0 to 2044 by battle).\n\nDialogs and such : same as enemies. There is one limit per text block with a random amount of initial free characters.\n\n[Supporting Abilities]\nYou can't change the effects of the supporting abilities for now.\n\n[Commands]\nCommand is surely the less working panel at the moment. There are 192 slots in total for spell sets and you can't add more spells to a command once the limit is reached.\nHowever :\n- you may link two commands together. They'll share the same spell set but that will only count half in that limitation,\n- only commands using the \"Spell\" panel have their spells count in this limit.\nSo, in order to add spells to a command, you must first either remove spells from other commands either use links. You can link Dagger's Eidolon and Summon commands as well as Beatrix's Seiken commands to have 12 slots by default.\nNote that if a character is given an ability s/he doesn't have in his/her ability list (in the \"Stats\" panel), s/he won't need AP nor equipments and can use it directly.\n\nAlso, there are a lot of special commands. Trying to edit \"Defend\" or \"Attack\", for instance, will end up with strange results. Most of the time, only names and helps can be edited safely. Editing \"Item\" and \"Throw\" object range will have no effect for now, as the list displayed there is wrongly-ordered.\n\nFinally, editing the spell list of a command won't change the spell list displayed in the game menu, only those available in battle. You have to change the ability list in the stats panel for that.\n\n[Stats]\nYou can not change the name of temporary characters.\n\n[Items]\nType of items is kind of forced. The first 88 items are meant to be weapons, then there are 136 armors and finally 32 usable items. The items have statistics set to 0 for the datas not related to their type.\n\n[Shops]\nYou can add up to 32 items at each shop and synthesis shop.\n\n[Enemies]\nThe \"Model ID\" should never be edited as modifying an enemy's model is not working yet.\nYou can't change the enemies groups (or \"Battle formations\") apart from the AP gain and the group frequency (in pourcentage). Note that the frequency is cumulative starting from \"Group 1\" to the last. For example, if the \"Group 1\" has a 80% appearance frequency and the \"Group 2\" has 50%, the \"Group 1\" has priority and will only let to the \"Group 2\" the remaining 20% chances to appear. The next groups will never appear in this configuration.\nFinally, enemies attack animations can't be edited as they are mixed within a more complex structure than for party's spells.\n\n[Scripts]\nYou can't edit the World Maps' scripts in the Japanese version of the game."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl173->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer4531111->Add( m_textCtrl173, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel20->SetSizer( bSizer4531111 );
	m_panel20->Layout();
	bSizer4531111->Fit( m_panel20 );
	m_helpbook->AddPage( m_panel20, _("Modding Limitations"), false );
	m_panel202 = new wxPanel( m_helpbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer45311112;
	bSizer45311112 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrl1732 = new wxTextCtrl( m_panel202, wxID_ANY, _("In this section, you'll find informations about how modding a game can mess up with your saves.\n\n[Memory Card Saves]\n- Modifying the stats given by the items or the attacks of the weapons matters if you have a character equiped with it. It goes back to normal (to the newest version) when you un-equip then re-equip them.\n- Modifying the party's stats, both the \"Level Progression\" and the individual stats bug. The party's stats are only used as an initial value so it will have no impact on saved games (at least once the characters have join your team).\n- Abilities learned are replaced by the new ones and it has the same amount of AP than for the previous one. Let's say I have a save with Zidane's Thievery at 80/100 and I modify the game so he needs 200 to learn Flare instead of Thievery, then he will have 80/200 AP for learning flare. Also, you keep gaining AP once you're done learning an ability (but you can't see it).\n- If you modify the Magic Stones requirement of supporting abilities and if a character has it activated when you saved, it will totally mess up with the total amount of magic stones available for that character.\n\n[Save States]\nIn addition, for save states, some modifications don't take effect until they are re-loaded in the RAM (which may be never for some of them).\n\n- Spells : the spell datas are loaded at the beginning of each battles or when the player access to his menu.\n- Commands : the command datas are loaded in the RAM once when the game starts and are never reloaded.\n- Enemies : the enemy datas and AI are loaded at the beginning of each encounters.\n- Fields : the script's field is loaded in the RAM everytime you enter the field. Its text, however, is loaded once when the game needs it and is never reloaded.\n\nThese informations will be completed eventually."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	m_textCtrl1732->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	bSizer45311112->Add( m_textCtrl1732, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panel202->SetSizer( bSizer45311112 );
	m_panel202->Layout();
	bSizer45311112->Fit( m_panel202 );
	m_helpbook->AddPage( m_panel202, _("About Mods and Saves"), false );
	#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_helpbookListView = m_helpbook->GetListView();
	long m_helpbookFlags = m_helpbookListView->GetWindowStyleFlag();
	if( m_helpbookFlags & wxLC_SMALL_ICON )
	{
		m_helpbookFlags = ( m_helpbookFlags & ~wxLC_SMALL_ICON ) | wxLC_ICON;
	}
	m_helpbookListView->SetWindowStyleFlag( m_helpbookFlags );
	#endif
	
	bSizer43->Add( m_helpbook, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( bSizer43 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

HelpWindow::~HelpWindow()
{
}

LogWindow::LogWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_errornum = new wxTextCtrl( this, wxID_ANY, _("Error(s) : "), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxNO_BORDER );
	m_errornum->SetForegroundColour( wxColour( 255, 0, 0 ) );
	m_errornum->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	gbSizer2->Add( m_errornum, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxLEFT|wxTOP, 4 );
	
	m_errorctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,80 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxDOUBLE_BORDER );
	m_errorctrl->SetForegroundColour( wxColour( 255, 0, 0 ) );
	m_errorctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ) );
	
	gbSizer2->Add( m_errorctrl, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_warningnum = new wxTextCtrl( this, wxID_ANY, _("Warning(s) : "), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxNO_BORDER );
	m_warningnum->SetForegroundColour( wxColour( 255, 150, 0 ) );
	m_warningnum->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	gbSizer2->Add( m_warningnum, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxLEFT|wxTOP, 4 );
	
	m_warningctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,80 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxDOUBLE_BORDER );
	m_warningctrl->SetForegroundColour( wxColour( 255, 150, 0 ) );
	m_warningctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ) );
	
	gbSizer2->Add( m_warningctrl, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer2->Add( m_buttonok, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxALL, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	gbSizer2->AddGrowableRow( 3 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	bSizer18->Fit( this );
	
	this->Centre( wxBOTH );
}

LogWindow::~LogWindow()
{
}

TextEditWindow::TextEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_textctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE );
	gbSizer2->Add( m_textctrl, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 0 );
	
	m_panel14 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer6;
	gbSizer6 = new wxGridBagSizer( 0, 0 );
	gbSizer6->SetFlexibleDirection( wxBOTH );
	gbSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_opcodeadd = new wxButton( m_panel14, wxID_ADD, _("Add"), wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gbSizer6->Add( m_opcodeadd, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_opcoderemove = new wxButton( m_panel14, wxID_REMOVE, _("Remove"), wxDefaultPosition, wxSize( 60,-1 ), 0 );
	gbSizer6->Add( m_opcoderemove, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_opcodelist = new wxListBox( m_panel14, wxID_ANY, wxDefaultPosition, wxSize( 132,164 ), 0, NULL, 0 ); 
	gbSizer6->Add( m_opcodelist, wxGBPosition( 1, 0 ), wxGBSpan( 2, 2 ), wxALL|wxEXPAND, 2 );
	
	m_opcodeup = new wxBitmapButton( m_panel14, wxID_UP, wxICON( moveup_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	gbSizer6->Add( m_opcodeup, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	m_opcodedown = new wxBitmapButton( m_panel14, wxID_DOWN, wxICON( movedown_icon ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	gbSizer6->Add( m_opcodedown, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	
	gbSizer6->AddGrowableRow( 2 );
	
	bSizer36->Add( gbSizer6, 1, wxEXPAND, 5 );
	
	
	m_panel14->SetSizer( bSizer36 );
	m_panel14->Layout();
	bSizer36->Fit( m_panel14 );
	gbSizer2->Add( m_panel14, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 2 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonset = new wxButton( this, wxID_SET, _("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer28->Add( m_buttonset, 0, wxALL, 2 );
	
	m_buttonpreview = new wxButton( this, wxID_PREVIEW, _("Preview"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer28->Add( m_buttonpreview, 0, wxALL, 2 );
	
	
	gbSizer2->Add( bSizer28, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer281;
	bSizer281 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonbubble = new wxButton( this, wxID_BUBBLE, _("Bubble size :"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	bSizer281->Add( m_buttonbubble, 0, wxALL, 2 );
	
	m_sizex = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 4095, 0 );
	bSizer281->Add( m_sizex, 0, wxALL, 2 );
	
	m_sizey = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, 0, 127, 0 );
	bSizer281->Add( m_sizey, 0, wxALL, 2 );
	
	
	gbSizer2->Add( bSizer281, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	m_richtextctrl = new PreviewTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,75 ), wxTE_READONLY|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	m_richtextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	gbSizer2->Add( m_richtextctrl, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxEXPAND | wxALL, 2 );
	
	m_textcharmap = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY );
	m_textcharmap->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_3DLIGHT ) );
	
	gbSizer2->Add( m_textcharmap, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 0 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 0 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_textctrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TextEditWindow::OnTextEdit ), NULL, this );
	m_opcodeadd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_opcoderemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_opcodelist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( TextEditWindow::OnOpcodeEdit ), NULL, this );
	m_opcodeup->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_opcodedown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonpreview->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonbubble->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
}

TextEditWindow::~TextEditWindow()
{
	// Disconnect Events
	m_textctrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TextEditWindow::OnTextEdit ), NULL, this );
	m_opcodeadd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_opcoderemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_opcodelist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( TextEditWindow::OnOpcodeEdit ), NULL, this );
	m_opcodeup->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_opcodedown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonpreview->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonbubble->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextEditWindow::OnButtonClick ), NULL, this );
	
}

TextSteamEditWindow::TextSteamEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 620,600 ), wxDefaultSize );
	
	wxFlexGridSizer* fgSizer98;
	fgSizer98 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer98->AddGrowableCol( 0 );
	fgSizer98->AddGrowableRow( 0 );
	fgSizer98->SetFlexibleDirection( wxBOTH );
	fgSizer98->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_textctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,200 ), wxHSCROLL|wxTE_MULTILINE );
	gbSizer2->Add( m_textctrl, wxGBPosition( 0, 0 ), wxGBSpan( 2, 2 ), wxALL|wxEXPAND, 0 );
	
	wxGridBagSizer* gbSizer49;
	gbSizer49 = new wxGridBagSizer( 0, 0 );
	gbSizer49->SetFlexibleDirection( wxBOTH );
	gbSizer49->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer101->AddGrowableCol( 2 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_multilangshowimg = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 16,16 ), 0 );
	fgSizer101->Add( m_multilangshowimg, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_multilangtitle = new wxStaticText( this, wxID_ANY, _("Multi-Language Panel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_multilangtitle->Wrap( -1 );
	fgSizer101->Add( m_multilangtitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtranslateall = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate All"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_langtranslateall, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	gbSizer49->Add( fgSizer101, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	m_langname1 = new wxStaticText( this, wxID_ANY, _("English"), wxDefaultPosition, wxDefaultSize, 0 );
	m_langname1->Wrap( -1 );
	gbSizer49->Add( m_langname1, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtext1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_langtext1->SetMinSize( wxSize( 400,40 ) );
	
	gbSizer49->Add( m_langtext1, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_langtranslate1 = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer49->Add( m_langtranslate1, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_langname2 = new wxStaticText( this, wxID_ANY, _("Japanese"), wxDefaultPosition, wxDefaultSize, 0 );
	m_langname2->Wrap( -1 );
	gbSizer49->Add( m_langname2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtext2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_langtext2->SetMinSize( wxSize( 400,40 ) );
	
	gbSizer49->Add( m_langtext2, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_langtranslate2 = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer49->Add( m_langtranslate2, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_langname3 = new wxStaticText( this, wxID_ANY, _("German"), wxDefaultPosition, wxDefaultSize, 0 );
	m_langname3->Wrap( -1 );
	gbSizer49->Add( m_langname3, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtext3 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_langtext3->SetMinSize( wxSize( 400,40 ) );
	
	gbSizer49->Add( m_langtext3, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_langtranslate3 = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer49->Add( m_langtranslate3, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_langname4 = new wxStaticText( this, wxID_ANY, _("French"), wxDefaultPosition, wxDefaultSize, 0 );
	m_langname4->Wrap( -1 );
	gbSizer49->Add( m_langname4, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtext4 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_langtext4->SetMinSize( wxSize( 400,40 ) );
	
	gbSizer49->Add( m_langtext4, wxGBPosition( 4, 1 ), wxGBSpan( 1, 1 ), wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_langtranslate4 = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer49->Add( m_langtranslate4, wxGBPosition( 4, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_langname5 = new wxStaticText( this, wxID_ANY, _("Italian"), wxDefaultPosition, wxDefaultSize, 0 );
	m_langname5->Wrap( -1 );
	gbSizer49->Add( m_langname5, wxGBPosition( 5, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtext5 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_langtext5->SetMinSize( wxSize( 400,40 ) );
	
	gbSizer49->Add( m_langtext5, wxGBPosition( 5, 1 ), wxGBSpan( 1, 1 ), wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_langtranslate5 = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer49->Add( m_langtranslate5, wxGBPosition( 5, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_langname6 = new wxStaticText( this, wxID_ANY, _("Spanish"), wxDefaultPosition, wxDefaultSize, 0 );
	m_langname6->Wrap( -1 );
	gbSizer49->Add( m_langname6, wxGBPosition( 6, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_langtext6 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	m_langtext6->SetMinSize( wxSize( 400,40 ) );
	
	gbSizer49->Add( m_langtext6, wxGBPosition( 6, 1 ), wxGBSpan( 1, 1 ), wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_langtranslate6 = new wxButton( this, wxID_TRANSLATE, _("Auto-Translate"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer49->Add( m_langtranslate6, wxGBPosition( 6, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	
	gbSizer49->AddGrowableCol( 1 );
	gbSizer49->AddGrowableRow( 1 );
	gbSizer49->AddGrowableRow( 2 );
	gbSizer49->AddGrowableRow( 3 );
	gbSizer49->AddGrowableRow( 4 );
	gbSizer49->AddGrowableRow( 5 );
	gbSizer49->AddGrowableRow( 6 );
	
	gbSizer2->Add( gbSizer49, wxGBPosition( 2, 0 ), wxGBSpan( 1, 3 ), wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer100;
	fgSizer100 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer100->SetFlexibleDirection( wxBOTH );
	fgSizer100->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_buttonset = new wxButton( this, wxID_SET, _("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer100->Add( m_buttonset, 0, wxALL, 2 );
	
	m_buttonbubble = new wxButton( this, wxID_BUBBLE, _("Auto-Size"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	fgSizer100->Add( m_buttonbubble, 0, wxALL, 2 );
	
	m_buttonopcodelist = new wxButton( this, wxID_HELP, _("Opcode List"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer100->Add( m_buttonopcodelist, 0, wxALL, 2 );
	
	
	gbSizer2->Add( fgSizer100, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALIGN_BOTTOM|wxALIGN_LEFT, 5 );
	
	m_richtextctrl = new PreviewTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,160 ), wxTE_READONLY|wxHSCROLL|wxNO_BORDER|wxVSCROLL|wxWANTS_CHARS );
	m_richtextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	gbSizer2->Add( m_richtextctrl, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	
	gbSizer2->AddGrowableCol( 2 );
	gbSizer2->AddGrowableRow( 0 );
	
	fgSizer98->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer98->Add( bSizer20, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer98 );
	this->Layout();
	fgSizer98->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_textctrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TextSteamEditWindow::OnTextEdit ), NULL, this );
	m_multilangshowimg->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextSteamEditWindow::OnShowHideMultiLang ), NULL, this );
	m_multilangtitle->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextSteamEditWindow::OnShowHideMultiLang ), NULL, this );
	m_langtranslateall->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonbubble->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonopcodelist->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
}

TextSteamEditWindow::~TextSteamEditWindow()
{
	// Disconnect Events
	m_textctrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TextSteamEditWindow::OnTextEdit ), NULL, this );
	m_multilangshowimg->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextSteamEditWindow::OnShowHideMultiLang ), NULL, this );
	m_multilangtitle->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextSteamEditWindow::OnShowHideMultiLang ), NULL, this );
	m_langtranslateall->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_langtranslate6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonset->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonbubble->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonopcodelist->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextSteamEditWindow::OnButtonClick ), NULL, this );
	
}

TextOpcodeWindow::TextOpcodeWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxGridBagSizer* gbSizer13;
	gbSizer13 = new wxGridBagSizer( 0, 0 );
	gbSizer13->SetFlexibleDirection( wxBOTH );
	gbSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer103;
	bSizer103 = new wxBoxSizer( wxHORIZONTAL );
	
	wxArrayString m_opcodeChoices;
	m_opcode = new wxChoice( this, wxID_OPCODE, wxDefaultPosition, wxDefaultSize, m_opcodeChoices, 0 );
	m_opcode->SetSelection( 0 );
	bSizer103->Add( m_opcode, 0, wxALL, 5 );
	
	m_opcodehelp = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_opcodehelp->Wrap( -1 );
	bSizer103->Add( m_opcodehelp, 0, wxALL, 5 );
	
	
	gbSizer13->Add( bSizer103, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1011;
	bSizer1011 = new wxBoxSizer( wxVERTICAL );
	
	m_opcodepaneldefault1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_opcodepaneldefault1->Hide();
	
	wxFlexGridSizer* fgSizer361;
	fgSizer361 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer361->SetFlexibleDirection( wxBOTH );
	fgSizer361->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_defaultarglabel11 = new wxStaticText( m_opcodepaneldefault1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultarglabel11->Wrap( -1 );
	fgSizer361->Add( m_defaultarglabel11, 0, wxALL, 5 );
	
	m_defaultarg11 = new wxSpinCtrl( m_opcodepaneldefault1, wxID_DEFAULTARG1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer361->Add( m_defaultarg11, 0, wxALL, 5 );
	
	
	m_opcodepaneldefault1->SetSizer( fgSizer361 );
	m_opcodepaneldefault1->Layout();
	fgSizer361->Fit( m_opcodepaneldefault1 );
	bSizer1011->Add( m_opcodepaneldefault1, 1, wxEXPAND | wxALL, 5 );
	
	m_opcodepaneldefault2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_opcodepaneldefault2->Hide();
	
	wxFlexGridSizer* fgSizer38;
	fgSizer38 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer38->SetFlexibleDirection( wxBOTH );
	fgSizer38->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_defaultarglabel21 = new wxStaticText( m_opcodepaneldefault2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultarglabel21->Wrap( -1 );
	fgSizer38->Add( m_defaultarglabel21, 0, wxALL, 5 );
	
	m_defaultarg21 = new wxSpinCtrl( m_opcodepaneldefault2, wxID_DEFAULTARG1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer38->Add( m_defaultarg21, 0, wxALL, 5 );
	
	m_defaultarglabel22 = new wxStaticText( m_opcodepaneldefault2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultarglabel22->Wrap( -1 );
	fgSizer38->Add( m_defaultarglabel22, 0, wxALL, 5 );
	
	m_defaultarg22 = new wxSpinCtrl( m_opcodepaneldefault2, wxID_DEFAULTARG2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer38->Add( m_defaultarg22, 0, wxALL, 5 );
	
	
	m_opcodepaneldefault2->SetSizer( fgSizer38 );
	m_opcodepaneldefault2->Layout();
	fgSizer38->Fit( m_opcodepaneldefault2 );
	bSizer1011->Add( m_opcodepaneldefault2, 1, wxEXPAND | wxALL, 5 );
	
	m_opcodepaneldefault3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_opcodepaneldefault3->Hide();
	
	wxFlexGridSizer* fgSizer39;
	fgSizer39 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer39->SetFlexibleDirection( wxBOTH );
	fgSizer39->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_defaultarglabel31 = new wxStaticText( m_opcodepaneldefault3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultarglabel31->Wrap( -1 );
	fgSizer39->Add( m_defaultarglabel31, 0, wxALL, 5 );
	
	m_defaultarg31 = new wxSpinCtrl( m_opcodepaneldefault3, wxID_DEFAULTARG1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer39->Add( m_defaultarg31, 0, wxALL, 5 );
	
	m_defaultarglabel32 = new wxStaticText( m_opcodepaneldefault3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultarglabel32->Wrap( -1 );
	fgSizer39->Add( m_defaultarglabel32, 0, wxALL, 5 );
	
	m_defaultarg32 = new wxSpinCtrl( m_opcodepaneldefault3, wxID_DEFAULTARG2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer39->Add( m_defaultarg32, 0, wxALL, 5 );
	
	m_defaultarglabel33 = new wxStaticText( m_opcodepaneldefault3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_defaultarglabel33->Wrap( -1 );
	fgSizer39->Add( m_defaultarglabel33, 0, wxALL, 5 );
	
	m_defaultarg33 = new wxSpinCtrl( m_opcodepaneldefault3, wxID_DEFAULTARG3, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	fgSizer39->Add( m_defaultarg33, 0, wxALL, 5 );
	
	
	m_opcodepaneldefault3->SetSizer( fgSizer39 );
	m_opcodepaneldefault3->Layout();
	fgSizer39->Fit( m_opcodepaneldefault3 );
	bSizer1011->Add( m_opcodepaneldefault3, 1, wxEXPAND | wxALL, 5 );
	
	m_opcodepaneltokenize = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_opcodepaneltokenize->Hide();
	
	wxGridBagSizer* gbSizer14;
	gbSizer14 = new wxGridBagSizer( 0, 0 );
	gbSizer14->SetFlexibleDirection( wxBOTH );
	gbSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tokenizearglabel = new wxStaticText( m_opcodepaneltokenize, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tokenizearglabel->Wrap( -1 );
	gbSizer14->Add( m_tokenizearglabel, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_tokenizearg = new wxSpinCtrl( m_opcodepaneltokenize, wxID_TOKENIZE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	gbSizer14->Add( m_tokenizearg, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_tokenizetextpanel = new wxScrolledWindow( m_opcodepaneltokenize, wxID_ANY, wxDefaultPosition, wxSize( -1,50 ), wxHSCROLL|wxVSCROLL );
	m_tokenizetextpanel->SetScrollRate( 5, 5 );
	gbSizer14->Add( m_tokenizetextpanel, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxEXPAND | wxALL, 5 );
	
	
	m_opcodepaneltokenize->SetSizer( gbSizer14 );
	m_opcodepaneltokenize->Layout();
	gbSizer14->Fit( m_opcodepaneltokenize );
	bSizer1011->Add( m_opcodepaneltokenize, 1, wxEXPAND | wxALL, 5 );
	
	m_opcodepanel0x48 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_opcodepanel0x48->Hide();
	
	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer( wxHORIZONTAL );
	
	m_0x48arglabel1 = new wxStaticText( m_opcodepanel0x48, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_0x48arglabel1->Wrap( -1 );
	bSizer107->Add( m_0x48arglabel1, 0, wxALL, 5 );
	
	m_0x48arg1 = new wxSpinCtrl( m_opcodepanel0x48, wxID_48, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10, 0 );
	bSizer107->Add( m_0x48arg1, 0, wxALL, 5 );
	
	
	bSizer102->Add( bSizer107, 1, wxEXPAND, 5 );
	
	m_0x48sizer = new wxGridSizer( 0, 4, 0, 0 );
	
	
	bSizer102->Add( m_0x48sizer, 1, wxEXPAND, 5 );
	
	
	m_opcodepanel0x48->SetSizer( bSizer102 );
	m_opcodepanel0x48->Layout();
	bSizer102->Fit( m_opcodepanel0x48 );
	bSizer1011->Add( m_opcodepanel0x48, 1, wxEXPAND | wxALL, 5 );
	
	
	gbSizer13->Add( bSizer1011, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer104->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer104->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer13->Add( bSizer104, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer13->AddGrowableCol( 0 );
	gbSizer13->AddGrowableRow( 0 );
	
	this->SetSizer( gbSizer13 );
	this->Layout();
	gbSizer13->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_opcode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TextOpcodeWindow::OnOpcodeChange ), NULL, this );
	m_defaultarg11->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg21->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg22->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg31->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg32->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg33->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_tokenizearg->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_tokenizetextpanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextOpcodeWindow::OnTokenMouse ), NULL, this );
	m_tokenizetextpanel->Connect( wxEVT_MOTION, wxMouseEventHandler( TextOpcodeWindow::OnTokenMouse ), NULL, this );
	m_tokenizetextpanel->Connect( wxEVT_PAINT, wxPaintEventHandler( TextOpcodeWindow::OnTokenPaint ), NULL, this );
	m_0x48arg1->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextOpcodeWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextOpcodeWindow::OnButtonClick ), NULL, this );
}

TextOpcodeWindow::~TextOpcodeWindow()
{
	// Disconnect Events
	m_opcode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TextOpcodeWindow::OnOpcodeChange ), NULL, this );
	m_defaultarg11->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg21->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg22->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg31->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg32->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_defaultarg33->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_tokenizearg->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_tokenizetextpanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextOpcodeWindow::OnTokenMouse ), NULL, this );
	m_tokenizetextpanel->Disconnect( wxEVT_MOTION, wxMouseEventHandler( TextOpcodeWindow::OnTokenMouse ), NULL, this );
	m_tokenizetextpanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( TextOpcodeWindow::OnTokenPaint ), NULL, this );
	m_0x48arg1->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TextOpcodeWindow::OnArgumentChange ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextOpcodeWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextOpcodeWindow::OnButtonClick ), NULL, this );
	
}

TextExportWindow::TextExportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Save..."), wxT("Text files (*.txt)|*.txt"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxVERTICAL );
	
	wxString m_strtypeChoices[] = { _("Raw Text"), _("Extended Opcodes"), _("Nice Text") };
	int m_strtypeNChoices = sizeof( m_strtypeChoices ) / sizeof( wxString );
	m_strtype = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_strtypeNChoices, m_strtypeChoices, 0 );
	m_strtype->SetSelection( 0 );
	m_strtype->SetToolTip( _("What the text opcodes must be replaced by") );
	
	bSizer34->Add( m_strtype, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer34, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Text Header") ), wxVERTICAL );
	
	m_textheader = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, _("//======================================\n//\tFinal Fantasy IX Text Block : %s\n//======================================\n"), wxDefaultPosition, wxSize( -1,90 ), wxHSCROLL|wxTE_MULTILINE );
	#ifdef __WXGTK__
	if ( !m_textheader->HasFlag( wxTE_MULTILINE ) )
	{
	m_textheader->SetMaxLength( 1900 );
	}
	#else
	m_textheader->SetMaxLength( 1900 );
	#endif
	m_textheader->SetToolTip( _("This text will be added at the beginning of the file") );
	
	sbSizer2->Add( m_textheader, 0, wxALL|wxEXPAND, 0 );
	
	
	gbSizer2->Add( sbSizer2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Text Separator") ), wxVERTICAL );
	
	m_textseparator = new wxTextCtrl( sbSizer6->GetStaticBox(), wxID_ANY, _("\n// New Text : %d"), wxDefaultPosition, wxSize( -1,60 ), wxHSCROLL|wxTE_MULTILINE );
	#ifdef __WXGTK__
	if ( !m_textseparator->HasFlag( wxTE_MULTILINE ) )
	{
	m_textseparator->SetMaxLength( 1900 );
	}
	#else
	m_textseparator->SetMaxLength( 1900 );
	#endif
	m_textseparator->SetToolTip( _("This text will be added before every text unit") );
	
	sbSizer6->Add( m_textseparator, 0, wxALL|wxEXPAND, 0 );
	
	
	gbSizer2->Add( sbSizer6, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 4, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	gbSizer2->AddGrowableRow( 2 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextExportWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextExportWindow::OnButtonClick ), NULL, this );
}

TextExportWindow::~TextExportWindow()
{
	// Disconnect Events
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextExportWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextExportWindow::OnButtonClick ), NULL, this );
	
}

BatchExportWindow::BatchExportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Save..."), wxT("File Batch|*"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer162;
	bSizer162 = new wxBoxSizer( wxVERTICAL );
	
	m_splitfilepanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer159;
	bSizer159 = new wxBoxSizer( wxVERTICAL );
	
	m_splitfile = new wxCheckBox( m_splitfilepanel, wxID_ANY, _("Split Files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_splitfile->SetValue(true); 
	m_splitfile->SetToolTip( _("When unchecked, all the datas\nare written in one big file") );
	
	bSizer159->Add( m_splitfile, 0, wxALL, 5 );
	
	
	m_splitfilepanel->SetSizer( bSizer159 );
	m_splitfilepanel->Layout();
	bSizer159->Fit( m_splitfilepanel );
	bSizer162->Add( m_splitfilepanel, 1, wxEXPAND | wxALL, 0 );
	
	m_scriptpanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_scriptpanel->Hide();
	
	wxBoxSizer* bSizer198;
	bSizer198 = new wxBoxSizer( wxHORIZONTAL );
	
	m_scriptsplitfile = new wxCheckBox( m_scriptpanel, wxID_ANY, _("Split Files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_scriptsplitfile->SetValue(true); 
	m_scriptsplitfile->SetToolTip( _("When unchecked, all the datas\nare written in one big file") );
	
	bSizer198->Add( m_scriptsplitfile, 0, wxALL, 5 );
	
	m_scriptcomment = new wxCheckBox( m_scriptpanel, wxID_ANY, _("Readability Comments"), wxDefaultPosition, wxDefaultSize, 0 );
	m_scriptcomment->SetValue(true); 
	m_scriptcomment->SetToolTip( _("Generate comments in the script for a better readability") );
	
	bSizer198->Add( m_scriptcomment, 0, wxALL, 5 );
	
	
	m_scriptpanel->SetSizer( bSizer198 );
	m_scriptpanel->Layout();
	bSizer198->Fit( m_scriptpanel );
	bSizer162->Add( m_scriptpanel, 1, wxEXPAND | wxALL, 0 );
	
	m_backgroundpanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_backgroundpanel->Hide();
	
	wxBoxSizer* bSizer1591;
	bSizer1591 = new wxBoxSizer( wxVERTICAL );
	
	m_mergetile = new wxCheckBox( m_backgroundpanel, wxID_ANY, _("Merge Tilesets"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mergetile->SetValue(true); 
	m_mergetile->SetToolTip( _("When unchecked, all the datas\nare written in one big file") );
	
	bSizer1591->Add( m_mergetile, 0, wxALL, 2 );
	
	m_exportorder = new wxCheckBox( m_backgroundpanel, wxID_ANY, _("Sort Tilesets by Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_exportorder->SetValue(true); 
	m_exportorder->SetToolTip( _("If disabled, the tilesets are exported using their internal ordering\nIgnored if the tilesets are merged") );
	
	bSizer1591->Add( m_exportorder, 0, wxALL, 2 );
	
	wxString m_languagetitleChoices[] = { _("All Titles"), _("English (US)"), _("English (UK)"), _("Japanese"), _("German"), _("French"), _("Italian"), _("Spanish") };
	int m_languagetitleNChoices = sizeof( m_languagetitleChoices ) / sizeof( wxString );
	m_languagetitle = new wxChoice( m_backgroundpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_languagetitleNChoices, m_languagetitleChoices, 0 );
	m_languagetitle->SetSelection( 0 );
	bSizer1591->Add( m_languagetitle, 0, wxALL, 5 );
	
	
	m_backgroundpanel->SetSizer( bSizer1591 );
	m_backgroundpanel->Layout();
	bSizer1591->Fit( m_backgroundpanel );
	bSizer162->Add( m_backgroundpanel, 1, wxEXPAND | wxALL, 0 );
	
	
	gbSizer2->Add( bSizer162, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Export List") ), wxVERTICAL );
	
	wxArrayString m_exportlistChoices;
	m_exportlist = new wxCheckListBox( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 230,140 ), m_exportlistChoices, wxLB_HSCROLL|wxLB_SINGLE );
	sbSizer2->Add( m_exportlist, 0, wxALL|wxEXPAND, 5 );
	
	
	gbSizer2->Add( sbSizer2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 2 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_filepicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BatchExportWindow::OnFilePick ), NULL, this );
	m_splitfile->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BatchExportWindow::OnSplitFileCheck ), NULL, this );
	m_scriptsplitfile->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BatchExportWindow::OnSplitFileCheck ), NULL, this );
	m_exportlist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BatchExportWindow::OnListRightClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchExportWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchExportWindow::OnButtonClick ), NULL, this );
}

BatchExportWindow::~BatchExportWindow()
{
	// Disconnect Events
	m_filepicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BatchExportWindow::OnFilePick ), NULL, this );
	m_splitfile->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BatchExportWindow::OnSplitFileCheck ), NULL, this );
	m_scriptsplitfile->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BatchExportWindow::OnSplitFileCheck ), NULL, this );
	m_exportlist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BatchExportWindow::OnListRightClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchExportWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchExportWindow::OnButtonClick ), NULL, this );
	
}

BatchImportWindow::BatchImportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 275,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Load..."), wxT("File Batch|*"), wxDefaultPosition, wxDefaultSize, wxFLP_OPEN|wxFLP_USE_TEXTCTRL );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer162;
	bSizer162 = new wxBoxSizer( wxVERTICAL );
	
	m_fatalwarning = new wxCheckBox( this, wxID_ANY, _("Make warnings fatal"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fatalwarning->SetValue(true); 
	m_fatalwarning->SetToolTip( _("If on, text blocks won't be imported if there is a warning\nIf off, text blocks will be imported if there are warnings but no errors") );
	
	bSizer162->Add( m_fatalwarning, 0, wxALL, 2 );
	
	m_adjustsizepanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer159;
	bSizer159 = new wxBoxSizer( wxVERTICAL );
	
	m_adjustsize = new wxCheckBox( m_adjustsizepanel, wxID_ANY, _("Adjust Dialog Box Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_adjustsize->SetValue(true); 
	bSizer159->Add( m_adjustsize, 0, wxALL, 2 );
	
	
	m_adjustsizepanel->SetSizer( bSizer159 );
	m_adjustsizepanel->Layout();
	bSizer159->Fit( m_adjustsizepanel );
	bSizer162->Add( m_adjustsizepanel, 1, wxEXPAND | wxALL, 0 );
	
	
	gbSizer2->Add( bSizer162, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Import List") ), wxVERTICAL );
	
	wxArrayString m_importlistChoices;
	m_importlist = new wxCheckListBox( sbSizer2->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 230,150 ), m_importlistChoices, wxLB_HSCROLL|wxLB_SINGLE );
	sbSizer2->Add( m_importlist, 0, wxALL, 5 );
	
	
	gbSizer2->Add( sbSizer2, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 2 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	bSizer18->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_filepicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BatchImportWindow::OnFilePick ), NULL, this );
	m_importlist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BatchImportWindow::OnListRightClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchImportWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchImportWindow::OnButtonClick ), NULL, this );
}

BatchImportWindow::~BatchImportWindow()
{
	// Disconnect Events
	m_filepicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BatchImportWindow::OnFilePick ), NULL, this );
	m_importlist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( BatchImportWindow::OnListRightClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchImportWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BatchImportWindow::OnButtonClick ), NULL, this );
	
}

CharmapTextureExportWindow::CharmapTextureExportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Save..."), wxT("Image (*.tga)|*.tga"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

CharmapTextureExportWindow::~CharmapTextureExportWindow()
{
}

FieldTextureExportWindow::FieldTextureExportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Save..."), wxT("Image (*.tiff)|*.tiff"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer48;
	fgSizer48 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer48->SetFlexibleDirection( wxBOTH );
	fgSizer48->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_mergetiles = new wxCheckBox( this, wxID_ANY, _("Merge Tilesets"), wxDefaultPosition, wxDefaultSize, 0 );
	m_mergetiles->SetToolTip( _("If enabled, the picture is exported in one layer\nIf disabled, each tileset is exported in one layer") );
	
	fgSizer48->Add( m_mergetiles, 0, wxALL, 5 );
	
	m_exportorder = new wxCheckBox( this, wxID_ANY, _("Sort Tilesets by Depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_exportorder->SetValue(true); 
	m_exportorder->SetToolTip( _("If disabled, the tilesets are exported using their internal ordering\nIgnored if the tilesets are merged") );
	
	fgSizer48->Add( m_exportorder, 0, wxALL, 5 );
	
	m_onlyselected = new wxCheckBox( this, wxID_ANY, _("Export Selected Only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_onlyselected->SetToolTip( _("Export only the tilesets enabled in the preview") );
	
	fgSizer48->Add( m_onlyselected, 0, wxALL, 5 );
	
	m_staticText328 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText328->Wrap( -1 );
	fgSizer48->Add( m_staticText328, 0, 0, 5 );
	
	m_staticText326 = new wxStaticText( this, wxID_ANY, _("Language Titles"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText326->Wrap( -1 );
	fgSizer48->Add( m_staticText326, 0, wxALL, 5 );
	
	wxString m_languagetitleChoices[] = { _("All Languages"), _("English (US)"), _("English (UK)"), _("Japanese"), _("German"), _("French"), _("Italian"), _("Spanish") };
	int m_languagetitleNChoices = sizeof( m_languagetitleChoices ) / sizeof( wxString );
	m_languagetitle = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_languagetitleNChoices, m_languagetitleChoices, 0 );
	m_languagetitle->SetSelection( 0 );
	fgSizer48->Add( m_languagetitle, 0, wxALL, 2 );
	
	
	gbSizer2->Add( fgSizer48, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

FieldTextureExportWindow::~FieldTextureExportWindow()
{
}

BattleSceneExportWindow::BattleSceneExportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Save..."), wxT("Object file (*.obj)|*"), wxDefaultPosition, wxDefaultSize, wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_usequads = new wxCheckBox( this, wxID_ANY, _("Export Quads"), wxDefaultPosition, wxDefaultSize, 0 );
	m_usequads->SetValue(true); 
	m_usequads->SetToolTip( _("Not all the gfx cards support quads\nIf this option is checked off, quads are converted into triangles") );
	
	gbSizer2->Add( m_usequads, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 3 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

BattleSceneExportWindow::~BattleSceneExportWindow()
{
}

BattleSceneImportWindow::BattleSceneImportWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Import..."), wxT("Object file (*.obj)|*.obj"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	gbSizer2->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	
	bSizer18->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

BattleSceneImportWindow::~BattleSceneImportWindow()
{
}

BattleSceneImportLinkTextureWindow::BattleSceneImportLinkTextureWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	m_sizerroot = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	m_linkscrolled = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxVSCROLL );
	m_linkscrolled->SetScrollRate( 5, 5 );
	m_linksizer = new wxFlexGridSizer( 2, 3, 0, 0 );
	m_linksizer->AddGrowableCol( 2 );
	m_linksizer->SetFlexibleDirection( wxBOTH );
	m_linksizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText96 = new wxStaticText( m_linkscrolled, wxID_ANY, _("Texture Ref"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText96->Wrap( -1 );
	m_linksizer->Add( m_staticText96, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticText97 = new wxStaticText( m_linkscrolled, wxID_ANY, _("------------>"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText97->Wrap( -1 );
	m_linksizer->Add( m_staticText97, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_staticText98 = new wxStaticText( m_linkscrolled, wxID_ANY, _("Texture"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText98->Wrap( -1 );
	m_linksizer->Add( m_staticText98, 0, wxALIGN_LEFT|wxALL, 5 );
	
	
	m_linkscrolled->SetSizer( m_linksizer );
	m_linkscrolled->Layout();
	m_linksizer->Fit( m_linkscrolled );
	gbSizer2->Add( m_linkscrolled, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );
	
	m_texturepanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxHSCROLL|wxSUNKEN_BORDER|wxVSCROLL );
	m_texturepanel->SetScrollRate( 5, 5 );
	m_texturepanel->SetMinSize( wxSize( 261,261 ) );
	
	gbSizer2->Add( m_texturepanel, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer2->Add( m_buttonok, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALIGN_CENTER|wxALL, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 0 );
	
	m_sizerroot->Add( gbSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( m_sizerroot );
	this->Layout();
	
	this->Centre( wxBOTH );
}

BattleSceneImportLinkTextureWindow::~BattleSceneImportLinkTextureWindow()
{
}

ManageTextureWindow::ManageTextureWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,420 ), wxDefaultSize );
	
	wxBoxSizer* bSizer55;
	bSizer55 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer10;
	gbSizer10 = new wxGridBagSizer( 0, 0 );
	gbSizer10->SetFlexibleDirection( wxBOTH );
	gbSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_texturelist = new wxListBox( this, wxID_TEXTURE, wxDefaultPosition, wxSize( 90,-1 ), 0, NULL, 0 ); 
	gbSizer10->Add( m_texturelist, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_palettelist = new wxListBox( this, wxID_PALETTE, wxDefaultPosition, wxSize( 90,-1 ), 0, NULL, 0 ); 
	gbSizer10->Add( m_palettelist, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText1021 = new wxStaticText( this, wxID_ANY, _("Palette Selection :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1021->Wrap( -1 );
	gbSizer10->Add( m_staticText1021, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALIGN_BOTTOM|wxLEFT, 5 );
	
	m_texturepaletteselection = new wxSpinCtrl( this, wxID_PALSEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, 0 );
	gbSizer10->Add( m_texturepaletteselection, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALL, 3 );
	
	m_texturewindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 280,70 ), wxHSCROLL|wxVSCROLL );
	m_texturewindow->SetScrollRate( 5, 5 );
	gbSizer10->Add( m_texturewindow, wxGBPosition( 0, 2 ), wxGBSpan( 2, 2 ), wxEXPAND | wxALL, 5 );
	
	wxString m_modifytyperadioChoices[] = { _("Draw"), _("Import") };
	int m_modifytyperadioNChoices = sizeof( m_modifytyperadioChoices ) / sizeof( wxString );
	m_modifytyperadio = new wxRadioBox( this, wxID_MODIFY, _("Modify"), wxDefaultPosition, wxDefaultSize, m_modifytyperadioNChoices, m_modifytyperadioChoices, 1, wxRA_SPECIFY_ROWS );
	m_modifytyperadio->SetSelection( 0 );
	gbSizer10->Add( m_modifytyperadio, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 2 );
	
	wxBoxSizer* bSizer112;
	bSizer112 = new wxBoxSizer( wxVERTICAL );
	
	m_modifypaneldraw = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridBagSizer* gbSizer16;
	gbSizer16 = new wxGridBagSizer( 0, 0 );
	gbSizer16->SetFlexibleDirection( wxBOTH );
	gbSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_drawpanelcolor = new wxPanel( m_modifypaneldraw, wxID_ANY, wxDefaultPosition, wxSize( 170,50 ), wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	gbSizer16->Add( m_drawpanelcolor, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );
	
	
	m_modifypaneldraw->SetSizer( gbSizer16 );
	m_modifypaneldraw->Layout();
	gbSizer16->Fit( m_modifypaneldraw );
	bSizer112->Add( m_modifypaneldraw, 1, wxEXPAND | wxALL, 2 );
	
	m_modifypanelimport = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_modifypanelimport->Hide();
	
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );
	
	m_texturepicker = new wxFilePickerCtrl( m_modifypanelimport, wxID_ANY, wxEmptyString, _("Import Image"), wxT("Supported Image (*.tga)|*.tga"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer111->Add( m_texturepicker, 0, wxALL, 5 );
	
	m_buttonimport = new wxButton( m_modifypanelimport, wxID_IMPORT, _("Import"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer111->Add( m_buttonimport, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	m_modifypanelimport->SetSizer( bSizer111 );
	m_modifypanelimport->Layout();
	bSizer111->Fit( m_modifypanelimport );
	bSizer112->Add( m_modifypanelimport, 1, wxEXPAND | wxALL, 2 );
	
	
	gbSizer10->Add( bSizer112, wxGBPosition( 4, 0 ), wxGBSpan( 2, 2 ), wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText99 = new wxStaticText( this, wxID_ANY, _("Position X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	fgSizer45->Add( m_staticText99, 0, wxALL, 1 );
	
	m_staticText100 = new wxStaticText( this, wxID_ANY, _("Position Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	fgSizer45->Add( m_staticText100, 0, wxALL, 1 );
	
	m_textureposx = new wxSpinCtrl( this, wxID_POSX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer45->Add( m_textureposx, 0, wxALL, 5 );
	
	m_textureposy = new wxSpinCtrl( this, wxID_POSY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer45->Add( m_textureposy, 0, wxALL, 5 );
	
	m_staticText101 = new wxStaticText( this, wxID_ANY, _("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	fgSizer45->Add( m_staticText101, 0, wxALL, 1 );
	
	m_staticText102 = new wxStaticText( this, wxID_ANY, _("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	fgSizer45->Add( m_staticText102, 0, wxALL, 1 );
	
	m_texturewidth = new wxSpinCtrl( this, wxID_SIZEX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer45->Add( m_texturewidth, 0, wxALL, 5 );
	
	m_textureheight = new wxSpinCtrl( this, wxID_SIZEY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer45->Add( m_textureheight, 0, wxALL, 5 );
	
	
	gbSizer10->Add( fgSizer45, wxGBPosition( 2, 2 ), wxGBSpan( 4, 2 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer56;
	bSizer56 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer56->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer56->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer10->Add( bSizer56, wxGBPosition( 6, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	
	gbSizer10->AddGrowableCol( 2 );
	gbSizer10->AddGrowableRow( 0 );
	
	bSizer55->Add( gbSizer10, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer55 );
	this->Layout();
	bSizer55->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_texturelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ManageTextureWindow::OnListBoxSelect ), NULL, this );
	m_texturelist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnListBoxRightClick ), NULL, this );
	m_palettelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ManageTextureWindow::OnListBoxSelect ), NULL, this );
	m_palettelist->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnListBoxRightClick ), NULL, this );
	m_texturepaletteselection->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_texturewindow->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Connect( wxEVT_MOTION, wxMouseEventHandler( ManageTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( ManageTextureWindow::OnTextureMouseWheel ), NULL, this );
	m_texturewindow->Connect( wxEVT_PAINT, wxPaintEventHandler( ManageTextureWindow::OnPaintTexture ), NULL, this );
	m_modifytyperadio->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ManageTextureWindow::OnModifyRadio ), NULL, this );
	m_drawpanelcolor->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Connect( wxEVT_MOTION, wxMouseEventHandler( ManageTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Connect( wxEVT_PAINT, wxPaintEventHandler( ManageTextureWindow::OnPaintPalette ), NULL, this );
	m_texturepicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ManageTextureWindow::OnChooseFileImage ), NULL, this );
	m_buttonimport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageTextureWindow::OnButtonClick ), NULL, this );
	m_textureposx->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_textureposy->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_texturewidth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_textureheight->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageTextureWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageTextureWindow::OnButtonClick ), NULL, this );
}

ManageTextureWindow::~ManageTextureWindow()
{
	// Disconnect Events
	m_texturelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ManageTextureWindow::OnListBoxSelect ), NULL, this );
	m_texturelist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnListBoxRightClick ), NULL, this );
	m_palettelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ManageTextureWindow::OnListBoxSelect ), NULL, this );
	m_palettelist->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnListBoxRightClick ), NULL, this );
	m_texturepaletteselection->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_MOTION, wxMouseEventHandler( ManageTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( ManageTextureWindow::OnTextureMouseWheel ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ManageTextureWindow::OnPaintTexture ), NULL, this );
	m_modifytyperadio->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ManageTextureWindow::OnModifyRadio ), NULL, this );
	m_drawpanelcolor->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Disconnect( wxEVT_MOTION, wxMouseEventHandler( ManageTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ManageTextureWindow::OnPaintPalette ), NULL, this );
	m_texturepicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ManageTextureWindow::OnChooseFileImage ), NULL, this );
	m_buttonimport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageTextureWindow::OnButtonClick ), NULL, this );
	m_textureposx->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_textureposy->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_texturewidth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_textureheight->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageTextureWindow::OnSpinPosition ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageTextureWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageTextureWindow::OnButtonClick ), NULL, this );
	
}

ManageFieldTextureWindow::ManageFieldTextureWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,560 ), wxDefaultSize );
	
	wxBoxSizer* bSizer55;
	bSizer55 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer10;
	gbSizer10 = new wxGridBagSizer( 0, 0 );
	gbSizer10->SetFlexibleDirection( wxBOTH );
	gbSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_tilechecklistChoices;
	m_tilechecklist = new wxCheckListBox( this, wxID_TILE, wxDefaultPosition, wxDefaultSize, m_tilechecklistChoices, 0 );
	gbSizer10->Add( m_tilechecklist, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_animlist = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 135,160 ), wxLC_SINGLE_SEL|wxLC_SMALL_ICON );
	gbSizer10->Add( m_animlist, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_staticText1021 = new wxStaticText( this, wxID_ANY, _("Palette Selection :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1021->Wrap( -1 );
	gbSizer10->Add( m_staticText1021, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALIGN_BOTTOM|wxLEFT, 5 );
	
	m_texturepaletteselection = new wxSpinCtrl( this, wxID_PALSEL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1, 0 );
	m_texturepaletteselection->Enable( false );
	
	gbSizer10->Add( m_texturepaletteselection, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALL, 3 );
	
	m_texturewindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 280,120 ), wxHSCROLL|wxVSCROLL );
	m_texturewindow->SetScrollRate( 5, 5 );
	gbSizer10->Add( m_texturewindow, wxGBPosition( 0, 2 ), wxGBSpan( 2, 2 ), wxEXPAND | wxALL, 5 );
	
	wxString m_modifytyperadioChoices[] = { _("Draw"), _("Import") };
	int m_modifytyperadioNChoices = sizeof( m_modifytyperadioChoices ) / sizeof( wxString );
	m_modifytyperadio = new wxRadioBox( this, wxID_MODIFY, _("Modify"), wxDefaultPosition, wxDefaultSize, m_modifytyperadioNChoices, m_modifytyperadioChoices, 1, wxRA_SPECIFY_ROWS );
	m_modifytyperadio->SetSelection( 0 );
	m_modifytyperadio->Enable( false );
	
	gbSizer10->Add( m_modifytyperadio, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 2 );
	
	wxBoxSizer* bSizer112;
	bSizer112 = new wxBoxSizer( wxVERTICAL );
	
	m_modifypaneldraw = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridBagSizer* gbSizer16;
	gbSizer16 = new wxGridBagSizer( 0, 0 );
	gbSizer16->SetFlexibleDirection( wxBOTH );
	gbSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_drawpanelcolor = new wxPanel( m_modifypaneldraw, wxID_ANY, wxDefaultPosition, wxSize( 170,50 ), wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	gbSizer16->Add( m_drawpanelcolor, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );
	
	
	m_modifypaneldraw->SetSizer( gbSizer16 );
	m_modifypaneldraw->Layout();
	gbSizer16->Fit( m_modifypaneldraw );
	bSizer112->Add( m_modifypaneldraw, 1, wxEXPAND | wxALL, 2 );
	
	m_modifypanelimport = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_modifypanelimport->Hide();
	
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );
	
	m_texturepicker = new wxFilePickerCtrl( m_modifypanelimport, wxID_ANY, wxEmptyString, _("Import Image"), wxT("Supported Image (*.tga)|*.tga"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	bSizer111->Add( m_texturepicker, 0, wxALL, 5 );
	
	m_buttonimport = new wxButton( m_modifypanelimport, wxID_IMPORT, _("Import"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer111->Add( m_buttonimport, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	m_modifypanelimport->SetSizer( bSizer111 );
	m_modifypanelimport->Layout();
	bSizer111->Fit( m_modifypanelimport );
	bSizer112->Add( m_modifypanelimport, 1, wxEXPAND | wxALL, 2 );
	
	
	gbSizer10->Add( bSizer112, wxGBPosition( 4, 0 ), wxGBSpan( 2, 2 ), wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText99 = new wxStaticText( this, wxID_ANY, _("Position X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	fgSizer45->Add( m_staticText99, 0, wxALL, 1 );
	
	m_staticText100 = new wxStaticText( this, wxID_ANY, _("Position Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	fgSizer45->Add( m_staticText100, 0, wxALL, 1 );
	
	m_textureposx = new wxSpinCtrl( this, wxID_POSX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer45->Add( m_textureposx, 0, wxALL, 5 );
	
	m_textureposy = new wxSpinCtrl( this, wxID_POSY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 0 );
	fgSizer45->Add( m_textureposy, 0, wxALL, 5 );
	
	
	gbSizer10->Add( fgSizer45, wxGBPosition( 2, 2 ), wxGBSpan( 4, 2 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer56;
	bSizer56 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonexport = new wxButton( this, wxID_EXPORT, _("Export"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer56->Add( m_buttonexport, 0, wxALL, 5 );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer56->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonok->Enable( false );
	
	bSizer56->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer10->Add( bSizer56, wxGBPosition( 6, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	
	gbSizer10->AddGrowableCol( 2 );
	gbSizer10->AddGrowableRow( 0 );
	
	bSizer55->Add( gbSizer10, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer55 );
	this->Layout();
	bSizer55->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_tilechecklist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ManageFieldTextureWindow::OnTileSelection ), NULL, this );
	m_tilechecklist->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( ManageFieldTextureWindow::OnTileButton ), NULL, this );
	m_animlist->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ManageFieldTextureWindow::OnAnimClick ), NULL, this );
	m_animlist->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageFieldTextureWindow::OnAnimClick ), NULL, this );
	m_texturepaletteselection->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageFieldTextureWindow::OnSpinPosition ), NULL, this );
	m_texturewindow->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageFieldTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Connect( wxEVT_MOTION, wxMouseEventHandler( ManageFieldTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( ManageFieldTextureWindow::OnTextureMouseWheel ), NULL, this );
	m_texturewindow->Connect( wxEVT_PAINT, wxPaintEventHandler( ManageFieldTextureWindow::OnPaintTexture ), NULL, this );
	m_modifytyperadio->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ManageFieldTextureWindow::OnModifyRadio ), NULL, this );
	m_drawpanelcolor->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageFieldTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Connect( wxEVT_MOTION, wxMouseEventHandler( ManageFieldTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Connect( wxEVT_PAINT, wxPaintEventHandler( ManageFieldTextureWindow::OnPaintPalette ), NULL, this );
	m_texturepicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ManageFieldTextureWindow::OnChooseFileImage ), NULL, this );
	m_buttonimport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	m_textureposx->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageFieldTextureWindow::OnSpinPosition ), NULL, this );
	m_textureposy->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageFieldTextureWindow::OnSpinPosition ), NULL, this );
	m_buttonexport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
}

ManageFieldTextureWindow::~ManageFieldTextureWindow()
{
	// Disconnect Events
	m_tilechecklist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ManageFieldTextureWindow::OnTileSelection ), NULL, this );
	m_tilechecklist->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( ManageFieldTextureWindow::OnTileButton ), NULL, this );
	m_animlist->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ManageFieldTextureWindow::OnAnimClick ), NULL, this );
	m_animlist->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageFieldTextureWindow::OnAnimClick ), NULL, this );
	m_texturepaletteselection->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageFieldTextureWindow::OnSpinPosition ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageFieldTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_MOTION, wxMouseEventHandler( ManageFieldTextureWindow::OnTextureMouseMove ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( ManageFieldTextureWindow::OnTextureMouseWheel ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ManageFieldTextureWindow::OnPaintTexture ), NULL, this );
	m_modifytyperadio->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ManageFieldTextureWindow::OnModifyRadio ), NULL, this );
	m_drawpanelcolor->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ManageFieldTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Disconnect( wxEVT_MOTION, wxMouseEventHandler( ManageFieldTextureWindow::OnPaletteMouseMove ), NULL, this );
	m_drawpanelcolor->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ManageFieldTextureWindow::OnPaintPalette ), NULL, this );
	m_texturepicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ManageFieldTextureWindow::OnChooseFileImage ), NULL, this );
	m_buttonimport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	m_textureposx->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageFieldTextureWindow::OnSpinPosition ), NULL, this );
	m_textureposy->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ManageFieldTextureWindow::OnSpinPosition ), NULL, this );
	m_buttonexport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ManageFieldTextureWindow::OnButtonClick ), NULL, this );
	
}

ScriptEditWindow::ScriptEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	topsizer = new wxGridBagSizer( 0, 0 );
	topsizer->SetFlexibleDirection( wxBOTH );
	topsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	wxFlexGridSizer* fgSizer60;
	fgSizer60 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer60->AddGrowableCol( 1 );
	fgSizer60->AddGrowableRow( 0 );
	fgSizer60->SetFlexibleDirection( wxBOTH );
	fgSizer60->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer65;
	fgSizer65 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer65->AddGrowableRow( 0 );
	fgSizer65->SetFlexibleDirection( wxBOTH );
	fgSizer65->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer66;
	fgSizer66 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer66->AddGrowableCol( 0 );
	fgSizer66->AddGrowableRow( 0 );
	fgSizer66->SetFlexibleDirection( wxBOTH );
	fgSizer66->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_functionlist = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLC_REPORT|wxLC_SINGLE_SEL );
	fgSizer66->Add( m_functionlist, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText291 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText291->Wrap( -1 );
	fgSizer66->Add( m_staticText291, 0, wxLEFT, 30 );
	
	
	fgSizer65->Add( fgSizer66, 1, wxEXPAND, 5 );
	
	m_panelnotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_panelcommon = new wxPanel( m_panelnotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer59;
	fgSizer59 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer59->SetFlexibleDirection( wxBOTH );
	fgSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_intvaluesignedintlabel = new wxStaticText( m_panelcommon, wxID_ANY, _("Signed Int"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluesignedintlabel->Wrap( -1 );
	fgSizer59->Add( m_intvaluesignedintlabel, 0, wxALL, 5 );
	
	m_intvaluesignedint = new wxTextCtrl( m_panelcommon, wxID_INT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer59->Add( m_intvaluesignedint, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvaluesignedlonglabel = new wxStaticText( m_panelcommon, wxID_ANY, _("Signed Long"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluesignedlonglabel->Wrap( -1 );
	fgSizer59->Add( m_intvaluesignedlonglabel, 0, wxALL, 5 );
	
	m_intvaluesignedlong = new wxTextCtrl( m_panelcommon, wxID_LONG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer59->Add( m_intvaluesignedlong, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvaluebase16label = new wxStaticText( m_panelcommon, wxID_ANY, _("Hexadecimal"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluebase16label->Wrap( -1 );
	fgSizer59->Add( m_intvaluebase16label, 0, wxALL, 5 );
	
	m_intvaluebase16 = new wxTextCtrl( m_panelcommon, wxID_HEXA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer59->Add( m_intvaluebase16, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvalueitemlabel = new wxStaticText( m_panelcommon, wxID_ANY, _("Item"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvalueitemlabel->Wrap( -1 );
	fgSizer59->Add( m_intvalueitemlabel, 0, wxALL, 5 );
	
	m_intvalueitem = new wxTextCtrl( m_panelcommon, wxID_ITEM, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer59->Add( m_intvalueitem, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvaluebuttonlabel = new wxStaticText( m_panelcommon, wxID_ANY, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluebuttonlabel->Wrap( -1 );
	fgSizer59->Add( m_intvaluebuttonlabel, 0, wxALL, 5 );
	
	m_intvaluebutton = new wxTextCtrl( m_panelcommon, wxID_BUTTON, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer59->Add( m_intvaluebutton, 0, wxLEFT|wxRIGHT, 5 );
	
	
	m_panelcommon->SetSizer( fgSizer59 );
	m_panelcommon->Layout();
	fgSizer59->Fit( m_panelcommon );
	m_panelnotebook->AddPage( m_panelcommon, _("Common"), false );
	m_panelbattle = new wxPanel( m_panelnotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer591;
	fgSizer591 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer591->SetFlexibleDirection( wxBOTH );
	fgSizer591->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_intvaluebase64label = new wxStaticText( m_panelbattle, wxID_ANY, _("4-tuple"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluebase64label->Wrap( -1 );
	fgSizer591->Add( m_intvaluebase64label, 0, wxALL, 5 );
	
	m_intvaluebase64 = new wxTextCtrl( m_panelbattle, wxID_LIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer591->Add( m_intvaluebase64, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvalueattacklabel = new wxStaticText( m_panelbattle, wxID_ANY, _("Attack List"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvalueattacklabel->Wrap( -1 );
	fgSizer591->Add( m_intvalueattacklabel, 0, wxALL, 5 );
	
	m_intvalueattack = new wxTextCtrl( m_panelbattle, wxID_SPLIST, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer591->Add( m_intvalueattack, 0, wxLEFT|wxRIGHT, 5 );
	
	m_staticText305 = new wxStaticText( m_panelbattle, wxID_ANY, _("Status List A"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText305->Wrap( -1 );
	fgSizer591->Add( m_staticText305, 0, wxALL, 5 );
	
	m_intvaluestatusa = new wxTextCtrl( m_panelbattle, wxID_STATUSA, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer591->Add( m_intvaluestatusa, 0, wxLEFT|wxRIGHT, 5 );
	
	m_staticText3051 = new wxStaticText( m_panelbattle, wxID_ANY, _("Status List B"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3051->Wrap( -1 );
	fgSizer591->Add( m_staticText3051, 0, wxALL, 5 );
	
	m_intvaluestatusb = new wxTextCtrl( m_panelbattle, wxID_STATUSB, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer591->Add( m_intvaluestatusb, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvaluespelllabel = new wxStaticText( m_panelbattle, wxID_ANY, _("Spell"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluespelllabel->Wrap( -1 );
	fgSizer591->Add( m_intvaluespelllabel, 0, wxALL, 5 );
	
	m_intvaluespell = new wxTextCtrl( m_panelbattle, wxID_SPELL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer591->Add( m_intvaluespell, 0, wxLEFT|wxRIGHT, 5 );
	
	m_intvaluecmdlabel = new wxStaticText( m_panelbattle, wxID_ANY, _("Command"), wxDefaultPosition, wxDefaultSize, 0 );
	m_intvaluecmdlabel->Wrap( -1 );
	fgSizer591->Add( m_intvaluecmdlabel, 0, wxALL, 5 );
	
	m_intvaluecmd = new wxTextCtrl( m_panelbattle, wxID_CMD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer591->Add( m_intvaluecmd, 0, wxLEFT|wxRIGHT, 5 );
	
	
	m_panelbattle->SetSizer( fgSizer591 );
	m_panelbattle->Layout();
	fgSizer591->Fit( m_panelbattle );
	m_panelnotebook->AddPage( m_panelbattle, _("Battle"), false );
	m_panelfield = new wxPanel( m_panelnotebook, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer63;
	fgSizer63 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer63->SetFlexibleDirection( wxBOTH );
	fgSizer63->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer146;
	bSizer146 = new wxBoxSizer( wxHORIZONTAL );
	
	m_fielddisplaybackground = new wxCheckBox( m_panelfield, wxID_FBACK, _("Background"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer146->Add( m_fielddisplaybackground, 0, wxALL, 5 );
	
	m_fielddisplaymesh = new wxCheckBox( m_panelfield, wxID_FWALK, _("Walkmesh"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fielddisplaymesh->SetValue(true); 
	bSizer146->Add( m_fielddisplaymesh, 0, wxALL, 5 );
	
	
	fgSizer63->Add( bSizer146, 1, wxEXPAND, 5 );
	
	m_fielddisplaypanel = new wxPanel( m_panelfield, wxID_ANY, wxDefaultPosition, wxSize( 256,256 ), wxTAB_TRAVERSAL );
	m_fielddisplaypanel->SetMinSize( wxSize( 256,256 ) );
	
	fgSizer63->Add( m_fielddisplaypanel, 1, wxEXPAND, 5 );
	
	wxString m_fieldcoordchoiceChoices[] = { _("Disabled"), _("Coordinate X"), _("Coordinate Y"), _("Coordinate Z") };
	int m_fieldcoordchoiceNChoices = sizeof( m_fieldcoordchoiceChoices ) / sizeof( wxString );
	m_fieldcoordchoice = new wxChoice( m_panelfield, wxID_FIELD, wxDefaultPosition, wxDefaultSize, m_fieldcoordchoiceNChoices, m_fieldcoordchoiceChoices, 0 );
	m_fieldcoordchoice->SetSelection( 0 );
	fgSizer63->Add( m_fieldcoordchoice, 0, wxALL, 5 );
	
	
	m_panelfield->SetSizer( fgSizer63 );
	m_panelfield->Layout();
	fgSizer63->Fit( m_panelfield );
	m_panelnotebook->AddPage( m_panelfield, _("Field"), false );
	m_panelworld = new wxPanel( m_panelnotebook, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer64;
	fgSizer64 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer64->SetFlexibleDirection( wxBOTH );
	fgSizer64->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_worlddisplaypanel = new wxPanel( m_panelworld, wxID_ANY, wxDefaultPosition, wxSize( 256,216 ), wxTAB_TRAVERSAL );
	m_worlddisplaypanel->SetMinSize( wxSize( 256,216 ) );
	
	fgSizer64->Add( m_worlddisplaypanel, 1, wxEXPAND, 5 );
	
	wxString m_worldcoordchoiceChoices[] = { _("Disabled"), _("Coordinate X"), _("Coordinate Y") };
	int m_worldcoordchoiceNChoices = sizeof( m_worldcoordchoiceChoices ) / sizeof( wxString );
	m_worldcoordchoice = new wxChoice( m_panelworld, wxID_WORLD, wxDefaultPosition, wxDefaultSize, m_worldcoordchoiceNChoices, m_worldcoordchoiceChoices, 0 );
	m_worldcoordchoice->SetSelection( 0 );
	fgSizer64->Add( m_worldcoordchoice, 0, wxALL, 5 );
	
	
	m_panelworld->SetSizer( fgSizer64 );
	m_panelworld->Layout();
	fgSizer64->Fit( m_panelworld );
	m_panelnotebook->AddPage( m_panelworld, _("World Map"), false );
	
	fgSizer65->Add( m_panelnotebook, 1, wxEXPAND|wxTOP, 20 );
	
	
	fgSizer60->Add( fgSizer65, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer61->AddGrowableCol( 0 );
	fgSizer61->AddGrowableRow( 2 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer148;
	bSizer148 = new wxBoxSizer( wxHORIZONTAL );
	
	m_localvarshowimg = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 16,16 ), 0 );
	bSizer148->Add( m_localvarshowimg, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText274 = new wxStaticText( this, wxID_ANY, _("Local Variable Panel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText274->Wrap( -1 );
	bSizer148->Add( m_staticText274, 0, wxALL, 5 );
	
	
	fgSizer61->Add( bSizer148, 1, wxEXPAND, 5 );
	
	m_localvartext = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,150 ), wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE );
	fgSizer61->Add( m_localvartext, 0, wxALL|wxEXPAND, 5 );
	
	m_scripttext = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_RICH2 );
	fgSizer61->Add( m_scripttext, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer60->Add( fgSizer61, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer39;
	fgSizer39 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer39->AddGrowableRow( 3 );
	fgSizer39->SetFlexibleDirection( wxBOTH );
	fgSizer39->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_functionlistbutton = new wxButton( this, wxID_HELP, _("Function List"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer39->Add( m_functionlistbutton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_helptextctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 260,150 ), wxTE_MULTILINE|wxTE_READONLY|wxSIMPLE_BORDER );
	m_helptextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	fgSizer39->Add( m_helptextctrl, 0, wxALL, 5 );
	
	m_staticText197 = new wxStaticText( this, wxID_ANY, _("Arguments"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText197->Wrap( -1 );
	fgSizer39->Add( m_staticText197, 0, wxALL, 5 );
	
	m_argpanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 260,-1 ), wxHSCROLL|wxVSCROLL );
	m_argpanel->SetScrollRate( 5, 5 );
	argsizer = new wxFlexGridSizer( 0, 1, 0, 0 );
	argsizer->AddGrowableCol( 0 );
	argsizer->SetFlexibleDirection( wxBOTH );
	argsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	
	m_argpanel->SetSizer( argsizer );
	m_argpanel->Layout();
	fgSizer39->Add( m_argpanel, 1, wxEXPAND | wxALL, 5 );
	
	
	fgSizer60->Add( fgSizer39, 1, wxEXPAND, 5 );
	
	
	topsizer->Add( fgSizer60, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer113;
	bSizer113 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttoncancel, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_buttonparse = new wxButton( this, wxID_PARSE, _("Parse"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttonparse, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttonok, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	topsizer->Add( bSizer113, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	topsizer->AddGrowableCol( 0 );
	topsizer->AddGrowableRow( 0 );
	
	this->SetSizer( topsizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_functionlist->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( ScriptEditWindow::OnFunctionRightClick ), NULL, this );
	m_functionlist->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ScriptEditWindow::OnFunctionChoose ), NULL, this );
	m_intvaluesignedint->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluesignedlong->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluebase16->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvalueitem->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluebutton->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluebase64->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvalueattack->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluestatusa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluestatusb->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluespell->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluecmd->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_fielddisplaybackground->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnCheckBox ), NULL, this );
	m_fielddisplaymesh->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnCheckBox ), NULL, this );
	m_fieldcoordchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ScriptEditWindow::OnChoiceSelection ), NULL, this );
	m_worlddisplaypanel->Connect( wxEVT_PAINT, wxPaintEventHandler( ScriptEditWindow::OnWorldMapPaint ), NULL, this );
	m_worldcoordchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ScriptEditWindow::OnChoiceSelection ), NULL, this );
	m_localvarshowimg->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ScriptEditWindow::OnShowHideLocalVar ), NULL, this );
	m_staticText274->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ScriptEditWindow::OnShowHideLocalVar ), NULL, this );
	m_scripttext->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ScriptEditWindow::OnFunctionUpdate ), NULL, this );
	m_scripttext->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnFunctionNewLine ), NULL, this );
	m_functionlistbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonparse->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
}

ScriptEditWindow::~ScriptEditWindow()
{
	// Disconnect Events
	m_functionlist->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( ScriptEditWindow::OnFunctionRightClick ), NULL, this );
	m_functionlist->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ScriptEditWindow::OnFunctionChoose ), NULL, this );
	m_intvaluesignedint->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluesignedlong->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluebase16->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvalueitem->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluebutton->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluebase64->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvalueattack->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluestatusa->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluestatusb->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluespell->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_intvaluecmd->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnIntValueText ), NULL, this );
	m_fielddisplaybackground->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnCheckBox ), NULL, this );
	m_fielddisplaymesh->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnCheckBox ), NULL, this );
	m_fieldcoordchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ScriptEditWindow::OnChoiceSelection ), NULL, this );
	m_worlddisplaypanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ScriptEditWindow::OnWorldMapPaint ), NULL, this );
	m_worldcoordchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ScriptEditWindow::OnChoiceSelection ), NULL, this );
	m_localvarshowimg->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ScriptEditWindow::OnShowHideLocalVar ), NULL, this );
	m_staticText274->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ScriptEditWindow::OnShowHideLocalVar ), NULL, this );
	m_scripttext->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ScriptEditWindow::OnFunctionUpdate ), NULL, this );
	m_scripttext->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptEditWindow::OnFunctionNewLine ), NULL, this );
	m_functionlistbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonparse->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditWindow::OnButtonClick ), NULL, this );
	
}

ScriptEditPropertiesWindow::ScriptEditPropertiesWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridBagSizer* gbSizer21;
	gbSizer21 = new wxGridBagSizer( 0, 0 );
	gbSizer21->SetFlexibleDirection( wxBOTH );
	gbSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	wxFlexGridSizer* fgSizer501;
	fgSizer501 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer501->SetFlexibleDirection( wxBOTH );
	fgSizer501->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3071 = new wxStaticText( this, wxID_ANY, _("Entry"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3071->Wrap( -1 );
	fgSizer501->Add( m_staticText3071, 0, wxALL, 5 );
	
	wxArrayString m_entryctrlChoices;
	m_entryctrl = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), m_entryctrlChoices, 0 );
	m_entryctrl->SetSelection( 0 );
	fgSizer501->Add( m_entryctrl, 0, wxALL, 5 );
	
	m_staticText2461 = new wxStaticText( this, wxID_ANY, _("Entry Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2461->Wrap( -1 );
	fgSizer501->Add( m_staticText2461, 0, wxALL, 5 );
	
	m_entrytypectrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	m_entrytypectrl->SetToolTip( _("0 : normal\n1 : region or shared function\n2 : object with model") );
	
	fgSizer501->Add( m_entrytypectrl, 0, wxALL, 2 );
	
	m_staticText2451 = new wxStaticText( this, wxID_ANY, _("Function Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2451->Wrap( -1 );
	fgSizer501->Add( m_staticText2451, 0, wxALL, 5 );
	
	m_typectrl = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, 0 );
	m_typectrl->SetToolTip( _("0 : main function\n1 : looping function\n2 : running when in range\n3 : running on speak button\n5 : running on full atb\n6 : counter party function\n7 : counter enemy function\n8 : running on card button\n9 : running on death (not working well)\n10 : running after a battle") );
	
	fgSizer501->Add( m_typectrl, 0, wxALL, 2 );
	
	
	gbSizer21->Add( fgSizer501, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer201->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer21->Add( bSizer201, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer21->AddGrowableCol( 0 );
	gbSizer21->AddGrowableRow( 1 );
	
	this->SetSizer( gbSizer21 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

ScriptEditPropertiesWindow::~ScriptEditPropertiesWindow()
{
}

ScriptEditEntryWindow::ScriptEditEntryWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_NONE );
	
	wxBoxSizer* bSizer188;
	bSizer188 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonadd = new wxButton( this, wxID_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer188->Add( m_buttonadd, 0, wxALL, 5 );
	
	m_buttonremove = new wxButton( this, wxID_REMOVE, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer188->Add( m_buttonremove, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer188, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxALIGN_CENTER, 5 );
	
	m_entrylist = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), 0, NULL, wxLB_SINGLE ); 
	gbSizer2->Add( m_entrylist, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxFlexGridSizer* fgSizer79;
	fgSizer79 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer79->SetFlexibleDirection( wxBOTH );
	fgSizer79->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText324 = new wxStaticText( this, wxID_ANY, _("Entry Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText324->Wrap( -1 );
	fgSizer79->Add( m_staticText324, 0, wxALL, 5 );
	
	m_entrytype = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 255, 0 );
	m_entrytype->SetToolTip( _("0 : normal\n1 : region or shared function\n2 : object with model") );
	
	fgSizer79->Add( m_entrytype, 0, wxALL, 2 );
	
	
	gbSizer2->Add( fgSizer79, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer2->Add( bSizer20, wxGBPosition( 2, 0 ), wxGBSpan( 1, 2 ), wxALIGN_RIGHT, 5 );
	
	
	gbSizer2->AddGrowableCol( 0 );
	gbSizer2->AddGrowableRow( 1 );
	
	this->SetSizer( gbSizer2 );
	this->Layout();
	gbSizer2->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonadd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	m_buttonremove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	m_entrylist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptEditEntryWindow::OnEntrySelect ), NULL, this );
	m_entrytype->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ScriptEditEntryWindow::OnSpinCtrl ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
}

ScriptEditEntryWindow::~ScriptEditEntryWindow()
{
	// Disconnect Events
	m_buttonadd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	m_buttonremove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	m_entrylist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptEditEntryWindow::OnEntrySelect ), NULL, this );
	m_entrytype->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ScriptEditEntryWindow::OnSpinCtrl ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ScriptEditEntryWindow::OnButtonClick ), NULL, this );
	
}

ScriptHelpWindow::ScriptHelpWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridBagSizer* gbSizer29;
	gbSizer29 = new wxGridBagSizer( 0, 0 );
	gbSizer29->SetFlexibleDirection( wxBOTH );
	gbSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_searchtextctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gbSizer29->Add( m_searchtextctrl, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_searchbtn = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 16,16 ), 0 );
	gbSizer29->Add( m_searchbtn, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER_VERTICAL, 5 );
	
	m_listbook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( 165,-1 ), wxAUI_NB_TOP );
	m_listbook->SetFont( wxFont( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_listbook->SetMinSize( wxSize( 165,-1 ) );
	
	m_panel94 = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel94->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	wxGridBagSizer* gbSizer30;
	gbSizer30 = new wxGridBagSizer( 0, 0 );
	gbSizer30->SetFlexibleDirection( wxBOTH );
	gbSizer30->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_listfunction = new wxListBox( m_panel94, wxID_FUNCTION, wxDefaultPosition, wxSize( 155,-1 ), 0, NULL, wxLB_HSCROLL|wxLB_SINGLE|wxLB_SORT ); 
	gbSizer30->Add( m_listfunction, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	
	gbSizer30->AddGrowableCol( 0 );
	gbSizer30->AddGrowableRow( 0 );
	
	m_panel94->SetSizer( gbSizer30 );
	m_panel94->Layout();
	gbSizer30->Fit( m_panel94 );
	m_listbook->AddPage( m_panel94, _("Function"), false, wxNullBitmap );
	m_panel95 = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel95->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	wxGridBagSizer* gbSizer301;
	gbSizer301 = new wxGridBagSizer( 0, 0 );
	gbSizer301->SetFlexibleDirection( wxBOTH );
	gbSizer301->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_listvariable = new wxListBox( m_panel95, wxID_VARIABLE, wxDefaultPosition, wxSize( 155,-1 ), 0, NULL, wxLB_HSCROLL|wxLB_SINGLE|wxLB_SORT ); 
	gbSizer301->Add( m_listvariable, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	
	gbSizer301->AddGrowableCol( 0 );
	gbSizer301->AddGrowableRow( 0 );
	
	m_panel95->SetSizer( gbSizer301 );
	m_panel95->Layout();
	gbSizer301->Fit( m_panel95 );
	m_listbook->AddPage( m_panel95, _("Variable"), false, wxNullBitmap );
	m_panel951 = new wxPanel( m_listbook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel951->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	
	wxGridBagSizer* gbSizer302;
	gbSizer302 = new wxGridBagSizer( 0, 0 );
	gbSizer302->SetFlexibleDirection( wxBOTH );
	gbSizer302->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_listvarcode = new wxListBox( m_panel951, wxID_VARCODE, wxDefaultPosition, wxSize( 155,-1 ), 0, NULL, wxLB_HSCROLL|wxLB_SINGLE|wxLB_SORT ); 
	gbSizer302->Add( m_listvarcode, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	
	gbSizer302->AddGrowableCol( 0 );
	gbSizer302->AddGrowableRow( 0 );
	
	m_panel951->SetSizer( gbSizer302 );
	m_panel951->Layout();
	gbSizer302->Fit( m_panel951 );
	m_listbook->AddPage( m_panel951, _("Var Code"), false, wxNullBitmap );
	
	gbSizer29->Add( m_listbook, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxEXPAND | wxALL, 0 );
	
	m_helptextctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxTE_READONLY|wxSIMPLE_BORDER );
	m_helptextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	gbSizer29->Add( m_helptextctrl, wxGBPosition( 0, 2 ), wxGBSpan( 2, 1 ), wxALL|wxEXPAND, 5 );
	
	
	gbSizer29->AddGrowableCol( 2 );
	gbSizer29->AddGrowableRow( 1 );
	
	this->SetSizer( gbSizer29 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_searchtextctrl->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptHelpWindow::OnSearch ), NULL, this );
	m_searchbtn->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ScriptHelpWindow::OnSearchButton ), NULL, this );
	m_listfunction->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptHelpWindow::OnListClick ), NULL, this );
	m_listfunction->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptHelpWindow::OnListDoubleClick ), NULL, this );
	m_listvariable->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptHelpWindow::OnListClick ), NULL, this );
	m_listvariable->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptHelpWindow::OnListDoubleClick ), NULL, this );
	m_listvarcode->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptHelpWindow::OnListClick ), NULL, this );
	m_listvarcode->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptHelpWindow::OnListDoubleClick ), NULL, this );
}

ScriptHelpWindow::~ScriptHelpWindow()
{
	// Disconnect Events
	m_searchtextctrl->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ScriptHelpWindow::OnSearch ), NULL, this );
	m_searchbtn->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ScriptHelpWindow::OnSearchButton ), NULL, this );
	m_listfunction->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptHelpWindow::OnListClick ), NULL, this );
	m_listfunction->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptHelpWindow::OnListDoubleClick ), NULL, this );
	m_listvariable->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptHelpWindow::OnListClick ), NULL, this );
	m_listvariable->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptHelpWindow::OnListDoubleClick ), NULL, this );
	m_listvarcode->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ScriptHelpWindow::OnListClick ), NULL, this );
	m_listvarcode->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ScriptHelpWindow::OnListDoubleClick ), NULL, this );
	
}

AnimSequenceEditWindow::AnimSequenceEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	topsizer = new wxGridBagSizer( 0, 0 );
	topsizer->SetFlexibleDirection( wxBOTH );
	topsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_sequencewindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_sequencewindow->SetScrollRate( 5, 5 );
	topsizer->Add( m_sequencewindow, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxEXPAND | wxALL, 5 );
	
	m_helptextctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY|wxSIMPLE_BORDER );
	m_helptextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	topsizer->Add( m_helptextctrl, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer113;
	bSizer113 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttoncancel, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttonok, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	topsizer->Add( bSizer113, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	
	topsizer->AddGrowableCol( 0 );
	topsizer->AddGrowableRow( 0 );
	
	this->SetSizer( topsizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnimSequenceEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnimSequenceEditWindow::OnButtonClick ), NULL, this );
}

AnimSequenceEditWindow::~AnimSequenceEditWindow()
{
	// Disconnect Events
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnimSequenceEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AnimSequenceEditWindow::OnButtonClick ), NULL, this );
	
}

MipsScriptEditWindow::MipsScriptEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	topsizer = new wxGridBagSizer( 0, 0 );
	topsizer->SetFlexibleDirection( wxBOTH );
	topsizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_mipswindow = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	topsizer->Add( m_mipswindow, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND | wxALL, 5 );
	
	m_mipsscroll = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	topsizer->Add( m_mipsscroll, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxGridBagSizer* gbSizer48;
	gbSizer48 = new wxGridBagSizer( 0, 0 );
	gbSizer48->SetFlexibleDirection( wxBOTH );
	gbSizer48->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_helpbutton = new wxButton( this, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer48->Add( m_helpbutton, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_displaypos = new wxCheckBox( this, wxID_ANY, _("Display Address"), wxDefaultPosition, wxDefaultSize, 0 );
	gbSizer48->Add( m_displaypos, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER|wxALL, 5 );
	
	m_codetextctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 250,-1 ), wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH );
	m_codetextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	gbSizer48->Add( m_codetextctrl, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );
	
	
	gbSizer48->AddGrowableRow( 1 );
	
	topsizer->Add( gbSizer48, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	wxBoxSizer* bSizer113;
	bSizer113 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttoncancel, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer113->Add( m_buttonok, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	topsizer->Add( bSizer113, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALIGN_RIGHT, 5 );
	
	
	topsizer->AddGrowableCol( 0 );
	topsizer->AddGrowableRow( 0 );
	
	this->SetSizer( topsizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_mipsscroll->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_helpbutton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnHelpClick ), NULL, this );
	m_displaypos->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnAddressCheck ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnButtonClick ), NULL, this );
}

MipsScriptEditWindow::~MipsScriptEditWindow()
{
	// Disconnect Events
	m_mipsscroll->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_mipsscroll->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MipsScriptEditWindow::OnMipsScroll ), NULL, this );
	m_helpbutton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnHelpClick ), NULL, this );
	m_displaypos->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnAddressCheck ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MipsScriptEditWindow::OnButtonClick ), NULL, this );
	
}

MipsScriptHelpWindow::MipsScriptHelpWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxGridBagSizer* gbSizer29;
	gbSizer29 = new wxGridBagSizer( 0, 0 );
	gbSizer29->SetFlexibleDirection( wxBOTH );
	gbSizer29->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_listfunction = new wxListBox( this, wxID_FUNCTION, wxDefaultPosition, wxSize( 155,-1 ), 0, NULL, wxLB_HSCROLL|wxLB_SINGLE ); 
	gbSizer29->Add( m_listfunction, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	m_helptextctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE|wxTE_READONLY|wxSIMPLE_BORDER );
	m_helptextctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	gbSizer29->Add( m_helptextctrl, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	
	gbSizer29->AddGrowableCol( 1 );
	gbSizer29->AddGrowableRow( 0 );
	
	this->SetSizer( gbSizer29 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_listfunction->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( MipsScriptHelpWindow::OnListClick ), NULL, this );
}

MipsScriptHelpWindow::~MipsScriptHelpWindow()
{
	// Disconnect Events
	m_listfunction->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( MipsScriptHelpWindow::OnListClick ), NULL, this );
	
}

CilScriptEditWindow::CilScriptEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer45->AddGrowableCol( 0 );
	fgSizer45->AddGrowableRow( 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer78;
	fgSizer78 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer78->AddGrowableCol( 1 );
	fgSizer78->AddGrowableRow( 0 );
	fgSizer78->SetFlexibleDirection( wxBOTH );
	fgSizer78->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_scriptposctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY|wxNO_BORDER );
	m_scriptposctrl->SetForegroundColour( wxColour( 166, 0, 129 ) );
	m_scriptposctrl->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	fgSizer78->Add( m_scriptposctrl, 0, wxALL|wxEXPAND, 5 );
	
	m_scriptctrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE );
	fgSizer78->Add( m_scriptctrl, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer45->Add( fgSizer78, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer45->Add( bSizer124, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer45 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CilScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CilScriptEditWindow::OnButtonClick ), NULL, this );
}

CilScriptEditWindow::~CilScriptEditWindow()
{
	// Disconnect Events
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CilScriptEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CilScriptEditWindow::OnButtonClick ), NULL, this );
	
}

ImageMapEditWindow::ImageMapEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxString m_objecttypechoiceChoices[] = { _("Battle"), _("Battle Scene"), _("Music"), _("Audio"), _("Field"), _("World Map"), _("Model") };
	int m_objecttypechoiceNChoices = sizeof( m_objecttypechoiceChoices ) / sizeof( wxString );
	m_objecttypechoice = new wxChoice( this, wxID_TYPE, wxDefaultPosition, wxDefaultSize, m_objecttypechoiceNChoices, m_objecttypechoiceChoices, 0 );
	m_objecttypechoice->SetSelection( 0 );
	fgSizer45->Add( m_objecttypechoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxFlexGridSizer* fgSizer46;
	fgSizer46 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer46->SetFlexibleDirection( wxBOTH );
	fgSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer123;
	bSizer123 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonadd = new wxButton( this, wxID_ADD, _("Add file"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer123->Add( m_buttonadd, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_fulllist = new wxListBox( this, wxID_FULLLIST, wxDefaultPosition, wxSize( 135,200 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer123->Add( m_fulllist, 0, wxALL, 5 );
	
	
	fgSizer46->Add( bSizer123, 1, wxEXPAND, 5 );
	
	m_staticline = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	fgSizer46->Add( m_staticline, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer126;
	bSizer126 = new wxBoxSizer( wxVERTICAL );
	
	m_buttonremove = new wxButton( this, wxID_REMOVE, _("Remove file"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer126->Add( m_buttonremove, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_maplist = new wxListBox( this, wxID_MAPLIST, wxDefaultPosition, wxSize( 135,200 ), 0, NULL, wxLB_HSCROLL ); 
	bSizer126->Add( m_maplist, 0, wxALL, 5 );
	
	
	fgSizer46->Add( bSizer126, 1, wxEXPAND, 5 );
	
	
	fgSizer45->Add( fgSizer46, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer45->Add( bSizer124, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer45 );
	this->Layout();
	fgSizer45->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_objecttypechoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ImageMapEditWindow::OnChoice ), NULL, this );
	m_buttonadd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	m_fulllist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ImageMapEditWindow::OnList ), NULL, this );
	m_buttonremove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	m_maplist->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ImageMapEditWindow::OnList ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
}

ImageMapEditWindow::~ImageMapEditWindow()
{
	// Disconnect Events
	m_objecttypechoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ImageMapEditWindow::OnChoice ), NULL, this );
	m_buttonadd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	m_fulllist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ImageMapEditWindow::OnList ), NULL, this );
	m_buttonremove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	m_maplist->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ImageMapEditWindow::OnList ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ImageMapEditWindow::OnButtonClick ), NULL, this );
	
}

ImageMapAddModelWindow::ImageMapAddModelWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer176;
	bSizer176 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText295 = new wxStaticText( this, wxID_ANY, _("Model's Entry"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText295->Wrap( -1 );
	bSizer176->Add( m_staticText295, 0, wxALL, 5 );
	
	m_entryvalue = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 127, 0 );
	m_entryvalue->SetToolTip( _("ID of the script entry\nusing this model") );
	
	bSizer176->Add( m_entryvalue, 0, wxALL, 2 );
	
	
	fgSizer45->Add( bSizer176, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer45->Add( bSizer124, 1, wxALIGN_CENTER, 5 );
	
	
	this->SetSizer( fgSizer45 );
	this->Layout();
	fgSizer45->Fit( this );
	
	this->Centre( wxBOTH );
}

ImageMapAddModelWindow::~ImageMapAddModelWindow()
{
}

SteamImageMapEditWindow::SteamImageMapEditWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer45->AddGrowableCol( 0 );
	fgSizer45->AddGrowableRow( 1 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxArrayString m_objecttypechoiceChoices;
	m_objecttypechoice = new wxChoice( this, wxID_TYPE, wxDefaultPosition, wxDefaultSize, m_objecttypechoiceChoices, 0 );
	m_objecttypechoice->SetSelection( 0 );
	fgSizer45->Add( m_objecttypechoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_objectlist = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_MULTILINE );
	fgSizer45->Add( m_objectlist, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttoncancel, 0, wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonok, 0, wxALL, 5 );
	
	
	fgSizer45->Add( bSizer124, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer45 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_objecttypechoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SteamImageMapEditWindow::OnChoice ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SteamImageMapEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SteamImageMapEditWindow::OnButtonClick ), NULL, this );
}

SteamImageMapEditWindow::~SteamImageMapEditWindow()
{
	// Disconnect Events
	m_objecttypechoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SteamImageMapEditWindow::OnChoice ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SteamImageMapEditWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SteamImageMapEditWindow::OnButtonClick ), NULL, this );
	
}

ModManagerWindow::ModManagerWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 350,450 ), wxDefaultSize );
	
	wxGridBagSizer* gbSizer34;
	gbSizer34 = new wxGridBagSizer( 0, 0 );
	gbSizer34->SetFlexibleDirection( wxBOTH );
	gbSizer34->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_textCtrl55 = new wxTextCtrl( this, wxID_ANY, _("Here you can check and define which datas are labelled as \"modified\" and should be saved."), wxDefaultPosition, wxSize( -1,35 ), wxTE_MULTILINE|wxTE_NO_VSCROLL|wxTE_READONLY|wxSIMPLE_BORDER );
	m_textCtrl55->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	gbSizer34->Add( m_textCtrl55, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	m_listtree = new wxTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE | wxTL_CHECKBOX | wxTL_NO_HEADER );
	gbSizer34->Add( m_listtree, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonok, 0, wxALL, 5 );
	
	
	gbSizer34->Add( bSizer124, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALIGN_CENTER, 5 );
	
	
	gbSizer34->AddGrowableCol( 0 );
	gbSizer34->AddGrowableRow( 1 );
	
	this->SetSizer( gbSizer34 );
	this->Layout();
	gbSizer34->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModManagerWindow::OnButtonClick ), NULL, this );
}

ModManagerWindow::~ModManagerWindow()
{
	// Disconnect Events
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ModManagerWindow::OnButtonClick ), NULL, this );
	
}

BackgroundEditorWindow::BackgroundEditorWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer45;
	fgSizer45 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer45->AddGrowableCol( 0 );
	fgSizer45->AddGrowableRow( 0 );
	fgSizer45->SetFlexibleDirection( wxBOTH );
	fgSizer45->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_auinotebook = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TOP );
	m_panelconverter = new wxPanel( m_auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridBagSizer* gbSizer41;
	gbSizer41 = new wxGridBagSizer( 0, 0 );
	gbSizer41->SetFlexibleDirection( wxBOTH );
	gbSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer81;
	fgSizer81 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer81->AddGrowableCol( 1 );
	fgSizer81->SetFlexibleDirection( wxBOTH );
	fgSizer81->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText317 = new wxStaticText( m_panelconverter, wxID_ANY, _("Background Image"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText317->Wrap( -1 );
	fgSizer81->Add( m_staticText317, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_imagepicker = new wxFilePickerCtrl( m_panelconverter, wxID_ANY, wxEmptyString, _("Select a file"), wxT("Supported Image|*.tiff;*.tif;*.bmp;*.tga|All files|*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	m_imagepicker->SetToolTip( _("The file must be the first layer of the background image\nAll the layers must be in the same directory, named\n[name]0.bmp\n[name]1.bmp\n[name]2.bmp\n...\n\nFor multi-background fields, use the names\n[name]0_0.bmp\n[name]0_1.bmp\n...\n[name]1_0.bmp\n[name]1_1.bmp\n...") );
	
	fgSizer81->Add( m_imagepicker, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText318 = new wxStaticText( m_panelconverter, wxID_ANY, _("Destination Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText318->Wrap( -1 );
	fgSizer81->Add( m_staticText318, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_exportdir = new wxDirPickerCtrl( m_panelconverter, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer81->Add( m_exportdir, 0, wxALL|wxEXPAND, 5 );
	
	
	gbSizer41->Add( fgSizer81, wxGBPosition( 0, 0 ), wxGBSpan( 1, 4 ), wxEXPAND, 5 );
	
	m_usegametilebtn = new wxRadioButton( m_panelconverter, wxID_ANY, _("Use Game Tiling"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_usegametilebtn->SetValue( true ); 
	gbSizer41->Add( m_usegametilebtn, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_customtilebtn = new wxRadioButton( m_panelconverter, wxID_ANY, _("Custom Tiling"), wxDefaultPosition, wxDefaultSize, 0 );
	m_customtilebtn->Enable( false );
	
	gbSizer41->Add( m_customtilebtn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxArrayString m_fieldchoiceChoices;
	m_fieldchoice = new wxChoice( m_panelconverter, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_fieldchoiceChoices, 0 );
	m_fieldchoice->SetSelection( 0 );
	gbSizer41->Add( m_fieldchoice, wxGBPosition( 1, 2 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 5 );
	
	m_texturewindow = new wxScrolledWindow( m_panelconverter, wxID_ANY, wxDefaultPosition, wxSize( 400,400 ), wxHSCROLL|wxVSCROLL );
	m_texturewindow->SetScrollRate( 5, 5 );
	gbSizer41->Add( m_texturewindow, wxGBPosition( 2, 0 ), wxGBSpan( 1, 3 ), wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer186;
	bSizer186 = new wxBoxSizer( wxVERTICAL );
	
	m_sortlayer = new wxCheckBox( m_panelconverter, wxID_SORTLAYER, _("Sort layers by depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sortlayer->SetValue(true); 
	bSizer186->Add( m_sortlayer, 0, wxALL, 5 );
	
	m_revertlayer = new wxCheckBox( m_panelconverter, wxID_REVERTLAYER, _("Revert layer ordering"), wxDefaultPosition, wxDefaultSize, 0 );
	m_revertlayer->SetValue(true); 
	bSizer186->Add( m_revertlayer, 0, wxALL, 5 );
	
	m_tilelist = new wxListBox( m_panelconverter, wxID_ANY, wxDefaultPosition, wxSize( -1,200 ), 0, NULL, wxLB_MULTIPLE ); 
	bSizer186->Add( m_tilelist, 0, wxALL, 5 );
	
	m_staticText314 = new wxStaticText( m_panelconverter, wxID_ANY, _("Resolution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText314->Wrap( -1 );
	bSizer186->Add( m_staticText314, 0, wxALL, 5 );
	
	m_resolution = new wxSpinCtrl( m_panelconverter, wxID_RESOLUTION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 16, 123, 32 );
	bSizer186->Add( m_resolution, 0, wxALL, 5 );
	
	m_staticText328 = new wxStaticText( m_panelconverter, wxID_ANY, _("Compression"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText328->Wrap( -1 );
	bSizer186->Add( m_staticText328, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer189;
	bSizer189 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString m_convertformatChoices[] = { _("RGB"), _("RGBA"), _("ARGB"), _("DXT1"), _("DXT5") };
	int m_convertformatNChoices = sizeof( m_convertformatChoices ) / sizeof( wxString );
	m_convertformat = new wxChoice( m_panelconverter, wxID_FORMAT, wxDefaultPosition, wxDefaultSize, m_convertformatNChoices, m_convertformatChoices, 0 );
	m_convertformat->SetSelection( 4 );
	m_convertformat->SetToolTip( _("Compression Method\nDXT5 is the default\nRGBA and ARGB are not compressed") );
	
	bSizer189->Add( m_convertformat, 0, wxALL, 5 );
	
	wxString m_dxtflagchoiceChoices[] = { _("Low"), _("Medium"), _("High") };
	int m_dxtflagchoiceNChoices = sizeof( m_dxtflagchoiceChoices ) / sizeof( wxString );
	m_dxtflagchoice = new wxChoice( m_panelconverter, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_dxtflagchoiceNChoices, m_dxtflagchoiceChoices, 0 );
	m_dxtflagchoice->SetSelection( 2 );
	m_dxtflagchoice->SetToolTip( _("Compression Quality\nOnly change the algorithm speed\nnot the compressed image size") );
	
	bSizer189->Add( m_dxtflagchoice, 0, wxALL, 5 );
	
	
	bSizer186->Add( bSizer189, 1, wxEXPAND, 5 );
	
	
	gbSizer41->Add( bSizer186, wxGBPosition( 2, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );
	
	
	gbSizer41->AddGrowableCol( 2 );
	gbSizer41->AddGrowableRow( 2 );
	
	m_panelconverter->SetSizer( gbSizer41 );
	m_panelconverter->Layout();
	gbSizer41->Fit( m_panelconverter );
	m_auinotebook->AddPage( m_panelconverter, _("Converter"), false, wxNullBitmap );
	m_panelmassconverter = new wxPanel( m_auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxGridBagSizer* gbSizer411;
	gbSizer411 = new wxGridBagSizer( 0, 0 );
	gbSizer411->SetFlexibleDirection( wxBOTH );
	gbSizer411->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer811;
	fgSizer811 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer811->AddGrowableCol( 1 );
	fgSizer811->SetFlexibleDirection( wxBOTH );
	fgSizer811->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3171 = new wxStaticText( m_panelmassconverter, wxID_ANY, _("Image Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3171->Wrap( -1 );
	fgSizer811->Add( m_staticText3171, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_massimageimporter = new wxDirPickerCtrl( m_panelmassconverter, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer811->Add( m_massimageimporter, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText325 = new wxStaticText( m_panelmassconverter, wxID_ANY, _("File Name Format"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText325->Wrap( -1 );
	fgSizer811->Add( m_staticText325, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_massimageformat = new wxTextCtrl( m_panelmassconverter, wxID_ANY, _("Field%\\Background%_%.tif"), wxDefaultPosition, wxDefaultSize, 0 );
	m_massimageformat->SetToolTip( _("Formatted file name from Image Folder\nIt must contains '%' thrice which stand for numbers in the file names :\n1) Field ID,\n2) Background (multi-background fields),\n3) Layer.") );
	
	fgSizer811->Add( m_massimageformat, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3181 = new wxStaticText( m_panelmassconverter, wxID_ANY, _("Destination Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3181->Wrap( -1 );
	fgSizer811->Add( m_staticText3181, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_massexportdir = new wxDirPickerCtrl( m_panelmassconverter, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer811->Add( m_massexportdir, 0, wxALL|wxEXPAND, 5 );
	
	
	gbSizer411->Add( fgSizer811, wxGBPosition( 0, 0 ), wxGBSpan( 1, 4 ), wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer84;
	fgSizer84 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer84->SetFlexibleDirection( wxBOTH );
	fgSizer84->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_massfieldid = new wxCheckBox( m_panelmassconverter, wxID_ANY, _("Use the script Field ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_massfieldid->SetToolTip( _("When on, the ID in the file format refers to the script Field ID\nWhen off, the ID is the order of field in the editor, starting from 0") );
	
	fgSizer84->Add( m_massfieldid, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer187;
	bSizer187 = new wxBoxSizer( wxHORIZONTAL );
	
	m_masssortlayer = new wxCheckBox( m_panelmassconverter, wxID_ANY, _("Sort layers by depth"), wxDefaultPosition, wxDefaultSize, 0 );
	m_masssortlayer->SetValue(true); 
	bSizer187->Add( m_masssortlayer, 0, wxALL, 5 );
	
	m_massrevertlayer = new wxCheckBox( m_panelmassconverter, wxID_ANY, _("Revert layer ordering"), wxDefaultPosition, wxDefaultSize, 0 );
	m_massrevertlayer->SetValue(true); 
	bSizer187->Add( m_massrevertlayer, 0, wxALL, 5 );
	
	
	fgSizer84->Add( bSizer187, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer85;
	fgSizer85 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer85->SetFlexibleDirection( wxBOTH );
	fgSizer85->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText3141 = new wxStaticText( m_panelmassconverter, wxID_ANY, _("Resolution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3141->Wrap( -1 );
	fgSizer85->Add( m_staticText3141, 0, wxALL, 5 );
	
	m_massresolution = new wxSpinCtrl( m_panelmassconverter, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 16, 123, 32 );
	fgSizer85->Add( m_massresolution, 0, wxALL, 3 );
	
	m_staticText31411 = new wxStaticText( m_panelmassconverter, wxID_ANY, _("Compression"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31411->Wrap( -1 );
	fgSizer85->Add( m_staticText31411, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer190;
	bSizer190 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString m_massconvertformatChoices[] = { _("RGB"), _("RGBA"), _("ARGB"), _("DXT1"), _("DXT5") };
	int m_massconvertformatNChoices = sizeof( m_massconvertformatChoices ) / sizeof( wxString );
	m_massconvertformat = new wxChoice( m_panelmassconverter, wxID_MASSFORMAT, wxDefaultPosition, wxDefaultSize, m_massconvertformatNChoices, m_massconvertformatChoices, 0 );
	m_massconvertformat->SetSelection( 4 );
	m_massconvertformat->SetToolTip( _("Compression Method\nDXT5 is the default\nRGBA and ARGB are not compressed") );
	
	bSizer190->Add( m_massconvertformat, 0, wxALL, 3 );
	
	wxString m_massdxtflagchoiceChoices[] = { _("Low"), _("Medium"), _("High") };
	int m_massdxtflagchoiceNChoices = sizeof( m_massdxtflagchoiceChoices ) / sizeof( wxString );
	m_massdxtflagchoice = new wxChoice( m_panelmassconverter, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_massdxtflagchoiceNChoices, m_massdxtflagchoiceChoices, 0 );
	m_massdxtflagchoice->SetSelection( 2 );
	m_massdxtflagchoice->SetToolTip( _("Compression Quality\nOnly change the algorithm speed\nnot the compressed image size") );
	
	bSizer190->Add( m_massdxtflagchoice, 0, wxALL, 3 );
	
	
	fgSizer85->Add( bSizer190, 1, wxEXPAND, 5 );
	
	
	fgSizer84->Add( fgSizer85, 1, wxEXPAND, 5 );
	
	
	gbSizer411->Add( fgSizer84, wxGBPosition( 1, 0 ), wxGBSpan( 1, 4 ), wxEXPAND, 5 );
	
	
	gbSizer411->AddGrowableCol( 2 );
	gbSizer411->AddGrowableRow( 2 );
	
	m_panelmassconverter->SetSizer( gbSizer411 );
	m_panelmassconverter->Layout();
	gbSizer411->Fit( m_panelmassconverter );
	m_auinotebook->AddPage( m_panelmassconverter, _("Mass Converter"), false, wxNullBitmap );
	m_panelimporter = new wxPanel( m_auinotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer80;
	fgSizer80 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer80->AddGrowableCol( 1 );
	fgSizer80->SetFlexibleDirection( wxBOTH );
	fgSizer80->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText316 = new wxStaticText( m_panelimporter, wxID_ANY, _("FF9 Launcher"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText316->Wrap( -1 );
	fgSizer80->Add( m_staticText316, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_importlauncher = new wxFilePickerCtrl( m_panelimporter, wxID_ANY, wxEmptyString, _("Select a file"), wxT("Steam game|FF9_Launcher.exe"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	fgSizer80->Add( m_importlauncher, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText319 = new wxStaticText( m_panelimporter, wxID_ANY, _("Converted Files Folder"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText319->Wrap( -1 );
	fgSizer80->Add( m_staticText319, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_importdir = new wxDirPickerCtrl( m_panelimporter, wxID_ANY, wxEmptyString, _("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	fgSizer80->Add( m_importdir, 0, wxALL|wxEXPAND, 5 );
	
	
	m_panelimporter->SetSizer( fgSizer80 );
	m_panelimporter->Layout();
	fgSizer80->Fit( m_panelimporter );
	m_auinotebook->AddPage( m_panelimporter, _("Mass Importer"), false, wxNullBitmap );
	
	fgSizer45->Add( m_auinotebook, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer124;
	bSizer124 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonapply = new wxButton( this, wxID_APPLY, _("Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonapply, 0, wxALL, 5 );
	
	m_buttonclose = new wxButton( this, wxID_CLOSE, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer124->Add( m_buttonclose, 0, wxALL, 5 );
	
	
	fgSizer45->Add( bSizer124, 1, wxALIGN_RIGHT, 5 );
	
	
	this->SetSizer( fgSizer45 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_imagepicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnFilePick ), NULL, this );
	m_exportdir->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnDirPick ), NULL, this );
	m_usegametilebtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnRadioClick ), NULL, this );
	m_customtilebtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnRadioClick ), NULL, this );
	m_fieldchoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnFieldChoice ), NULL, this );
	m_texturewindow->Connect( wxEVT_PAINT, wxPaintEventHandler( BackgroundEditorWindow::OnPaintImage ), NULL, this );
	m_sortlayer->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnCheckBox ), NULL, this );
	m_revertlayer->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnCheckBox ), NULL, this );
	m_tilelist->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnTileSelect ), NULL, this );
	m_resolution->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BackgroundEditorWindow::OnSpinChange ), NULL, this );
	m_convertformat->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnChoice ), NULL, this );
	m_massexportdir->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnDirPick ), NULL, this );
	m_massconvertformat->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnChoice ), NULL, this );
	m_importdir->Connect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnDirPick ), NULL, this );
	m_buttonapply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnButtonClick ), NULL, this );
	m_buttonclose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnButtonClick ), NULL, this );
}

BackgroundEditorWindow::~BackgroundEditorWindow()
{
	// Disconnect Events
	m_imagepicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnFilePick ), NULL, this );
	m_exportdir->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnDirPick ), NULL, this );
	m_usegametilebtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnRadioClick ), NULL, this );
	m_customtilebtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnRadioClick ), NULL, this );
	m_fieldchoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnFieldChoice ), NULL, this );
	m_texturewindow->Disconnect( wxEVT_PAINT, wxPaintEventHandler( BackgroundEditorWindow::OnPaintImage ), NULL, this );
	m_sortlayer->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnCheckBox ), NULL, this );
	m_revertlayer->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnCheckBox ), NULL, this );
	m_tilelist->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnTileSelect ), NULL, this );
	m_resolution->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( BackgroundEditorWindow::OnSpinChange ), NULL, this );
	m_convertformat->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnChoice ), NULL, this );
	m_massexportdir->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnDirPick ), NULL, this );
	m_massconvertformat->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BackgroundEditorWindow::OnChoice ), NULL, this );
	m_importdir->Disconnect( wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler( BackgroundEditorWindow::OnDirPick ), NULL, this );
	m_buttonapply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnButtonClick ), NULL, this );
	m_buttonclose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BackgroundEditorWindow::OnButtonClick ), NULL, this );
	
}

UnityViewerWindow::UnityViewerWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_menubar = new wxMenuBar( 0 );
	m_menufile = new wxMenu();
	wxMenuItem* m_menuopen;
	m_menuopen = new wxMenuItem( m_menufile, wxID_OPEN, wxString( _("Open") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuopen );
	
	m_menureopen = new wxMenuItem( m_menufile, wxID_REOPEN, wxString( _("Reload") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menureopen );
	
	m_menufile->AppendSeparator();
	
	wxMenuItem* m_menuclose;
	m_menuclose = new wxMenuItem( m_menufile, wxID_CLOSE, wxString( _("Close") ) + wxT('\t') + wxT("Alt+F4"), wxEmptyString, wxITEM_NORMAL );
	m_menufile->Append( m_menuclose );
	
	m_menubar->Append( m_menufile, _("File") ); 
	
	m_menuassets = new wxMenu();
	m_submenudata = new wxMenu();
	wxMenuItem* m_submenudataItem = new wxMenuItem( m_menuassets, wxID_ANY, _("Streaming Assets"), wxEmptyString, wxITEM_NORMAL, m_submenudata );
	m_submenufield = new wxMenu();
	wxMenuItem* m_submenufieldItem = new wxMenuItem( m_submenudata, wxID_ANY, _("p0data1"), wxEmptyString, wxITEM_NORMAL, m_submenufield );
	wxMenuItem* m_menupdata11;
	m_menupdata11 = new wxMenuItem( m_submenufield, wxID_PDATA11, wxString( _("p0data11") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata11 );
	
	wxMenuItem* m_menupdata12;
	m_menupdata12 = new wxMenuItem( m_submenufield, wxID_PDATA12, wxString( _("p0data12") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata12 );
	
	wxMenuItem* m_menupdata13;
	m_menupdata13 = new wxMenuItem( m_submenufield, wxID_PDATA13, wxString( _("p0data13") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata13 );
	
	wxMenuItem* m_menupdata14;
	m_menupdata14 = new wxMenuItem( m_submenufield, wxID_PDATA14, wxString( _("p0data14") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata14 );
	
	wxMenuItem* m_menupdata15;
	m_menupdata15 = new wxMenuItem( m_submenufield, wxID_PDATA15, wxString( _("p0data15") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata15 );
	
	wxMenuItem* m_menupdata16;
	m_menupdata16 = new wxMenuItem( m_submenufield, wxID_PDATA16, wxString( _("p0data16") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata16 );
	
	wxMenuItem* m_menupdata17;
	m_menupdata17 = new wxMenuItem( m_submenufield, wxID_PDATA17, wxString( _("p0data17") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata17 );
	
	wxMenuItem* m_menupdata18;
	m_menupdata18 = new wxMenuItem( m_submenufield, wxID_PDATA18, wxString( _("p0data18") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata18 );
	
	wxMenuItem* m_menupdata19;
	m_menupdata19 = new wxMenuItem( m_submenufield, wxID_PDATA19, wxString( _("p0data19") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenufield->Append( m_menupdata19 );
	
	m_submenudata->Append( m_submenufieldItem );
	
	wxMenuItem* m_menupdata2;
	m_menupdata2 = new wxMenuItem( m_submenudata, wxID_PDATA2, wxString( _("p0data2") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenudata->Append( m_menupdata2 );
	
	wxMenuItem* m_menupdata3;
	m_menupdata3 = new wxMenuItem( m_submenudata, wxID_PDATA3, wxString( _("p0data3") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenudata->Append( m_menupdata3 );
	
	wxMenuItem* m_menupdata4;
	m_menupdata4 = new wxMenuItem( m_submenudata, wxID_PDATA4, wxString( _("p0data4") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenudata->Append( m_menupdata4 );
	
	wxMenuItem* m_menupdata5;
	m_menupdata5 = new wxMenuItem( m_submenudata, wxID_PDATA5, wxString( _("p0data5") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenudata->Append( m_menupdata5 );
	
	m_submenuaudio = new wxMenu();
	wxMenuItem* m_submenuaudioItem = new wxMenuItem( m_submenudata, wxID_ANY, _("p0data6"), wxEmptyString, wxITEM_NORMAL, m_submenuaudio );
	wxMenuItem* m_menupdata61;
	m_menupdata61 = new wxMenuItem( m_submenuaudio, wxID_PDATA61, wxString( _("p0data61") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuaudio->Append( m_menupdata61 );
	
	wxMenuItem* m_menupdata62;
	m_menupdata62 = new wxMenuItem( m_submenuaudio, wxID_PDATA62, wxString( _("p0data62") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuaudio->Append( m_menupdata62 );
	
	wxMenuItem* m_menupdata63;
	m_menupdata63 = new wxMenuItem( m_submenuaudio, wxID_PDATA63, wxString( _("p0data63") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenuaudio->Append( m_menupdata63 );
	
	m_submenudata->Append( m_submenuaudioItem );
	
	wxMenuItem* m_menupdata7;
	m_menupdata7 = new wxMenuItem( m_submenudata, wxID_PDATA7, wxString( _("p0data7") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenudata->Append( m_menupdata7 );
	
	m_menuassets->Append( m_submenudataItem );
	
	m_submenulevel = new wxMenu();
	wxMenuItem* m_submenulevelItem = new wxMenuItem( m_menuassets, wxID_ANY, _("Level"), wxEmptyString, wxITEM_NORMAL, m_submenulevel );
	wxMenuItem* m_menulevel0;
	m_menulevel0 = new wxMenuItem( m_submenulevel, wxID_LEVEL0, wxString( _("level0") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel0 );
	
	wxMenuItem* m_menulevel1;
	m_menulevel1 = new wxMenuItem( m_submenulevel, wxID_LEVEL1, wxString( _("level1") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel1 );
	
	wxMenuItem* m_menulevel2;
	m_menulevel2 = new wxMenuItem( m_submenulevel, wxID_LEVEL2, wxString( _("level2") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel2 );
	
	wxMenuItem* m_menulevel3;
	m_menulevel3 = new wxMenuItem( m_submenulevel, wxID_LEVEL3, wxString( _("level3") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel3 );
	
	wxMenuItem* m_menulevel4;
	m_menulevel4 = new wxMenuItem( m_submenulevel, wxID_LEVEL4, wxString( _("level4") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel4 );
	
	wxMenuItem* m_menulevel5;
	m_menulevel5 = new wxMenuItem( m_submenulevel, wxID_LEVEL5, wxString( _("level5") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel5 );
	
	wxMenuItem* m_menulevel6;
	m_menulevel6 = new wxMenuItem( m_submenulevel, wxID_LEVEL6, wxString( _("level6") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel6 );
	
	wxMenuItem* m_menulevel7;
	m_menulevel7 = new wxMenuItem( m_submenulevel, wxID_LEVEL7, wxString( _("level7") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel7 );
	
	wxMenuItem* m_menulevel8;
	m_menulevel8 = new wxMenuItem( m_submenulevel, wxID_LEVEL8, wxString( _("level8") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel8 );
	
	wxMenuItem* m_menulevel9;
	m_menulevel9 = new wxMenuItem( m_submenulevel, wxID_LEVEL9, wxString( _("level9") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel9 );
	
	wxMenuItem* m_menulevel10;
	m_menulevel10 = new wxMenuItem( m_submenulevel, wxID_LEVEL10, wxString( _("level10") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel10 );
	
	wxMenuItem* m_menulevel11;
	m_menulevel11 = new wxMenuItem( m_submenulevel, wxID_LEVEL11, wxString( _("level11") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel11 );
	
	wxMenuItem* m_menulevel12;
	m_menulevel12 = new wxMenuItem( m_submenulevel, wxID_LEVEL12, wxString( _("level12") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel12 );
	
	wxMenuItem* m_menulevel13;
	m_menulevel13 = new wxMenuItem( m_submenulevel, wxID_LEVEL13, wxString( _("level13") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel13 );
	
	wxMenuItem* m_menulevel14;
	m_menulevel14 = new wxMenuItem( m_submenulevel, wxID_LEVEL14, wxString( _("level14") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel14 );
	
	wxMenuItem* m_menulevel15;
	m_menulevel15 = new wxMenuItem( m_submenulevel, wxID_LEVEL15, wxString( _("level15") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel15 );
	
	wxMenuItem* m_menulevel16;
	m_menulevel16 = new wxMenuItem( m_submenulevel, wxID_LEVEL16, wxString( _("level16") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel16 );
	
	wxMenuItem* m_menulevel17;
	m_menulevel17 = new wxMenuItem( m_submenulevel, wxID_LEVEL17, wxString( _("level17") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel17 );
	
	wxMenuItem* m_menulevel18;
	m_menulevel18 = new wxMenuItem( m_submenulevel, wxID_LEVEL18, wxString( _("level18") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel18 );
	
	wxMenuItem* m_menulevel19;
	m_menulevel19 = new wxMenuItem( m_submenulevel, wxID_LEVEL19, wxString( _("level19") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel19 );
	
	wxMenuItem* m_menulevel20;
	m_menulevel20 = new wxMenuItem( m_submenulevel, wxID_LEVEL20, wxString( _("level20") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel20 );
	
	wxMenuItem* m_menulevel21;
	m_menulevel21 = new wxMenuItem( m_submenulevel, wxID_LEVEL21, wxString( _("level21") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel21 );
	
	wxMenuItem* m_menulevel22;
	m_menulevel22 = new wxMenuItem( m_submenulevel, wxID_LEVEL22, wxString( _("level22") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel22 );
	
	wxMenuItem* m_menulevel23;
	m_menulevel23 = new wxMenuItem( m_submenulevel, wxID_LEVEL23, wxString( _("level23") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel23 );
	
	wxMenuItem* m_menulevel24;
	m_menulevel24 = new wxMenuItem( m_submenulevel, wxID_LEVEL24, wxString( _("level24") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel24 );
	
	wxMenuItem* m_menulevel25;
	m_menulevel25 = new wxMenuItem( m_submenulevel, wxID_LEVEL25, wxString( _("level25") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel25 );
	
	wxMenuItem* m_menulevel26;
	m_menulevel26 = new wxMenuItem( m_submenulevel, wxID_LEVEL26, wxString( _("level26") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel26 );
	
	wxMenuItem* m_menulevel27;
	m_menulevel27 = new wxMenuItem( m_submenulevel, wxID_LEVEL27, wxString( _("level27") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenulevel->Append( m_menulevel27 );
	
	m_menuassets->Append( m_submenulevelItem );
	
	m_submenushared = new wxMenu();
	wxMenuItem* m_submenusharedItem = new wxMenuItem( m_menuassets, wxID_ANY, _("Shared Assets"), wxEmptyString, wxITEM_NORMAL, m_submenushared );
	wxMenuItem* m_menuresources;
	m_menuresources = new wxMenuItem( m_submenushared, wxID_RESOURCES, wxString( _("resources") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menuresources );
	
	wxMenuItem* m_menumaindata;
	m_menumaindata = new wxMenuItem( m_submenushared, wxID_MAINDATA, wxString( _("mainData") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menumaindata );
	
	wxMenuItem* m_menushared0;
	m_menushared0 = new wxMenuItem( m_submenushared, wxID_SHARED0, wxString( _("sharedassets0") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared0 );
	
	wxMenuItem* m_menushared1;
	m_menushared1 = new wxMenuItem( m_submenushared, wxID_SHARED1, wxString( _("sharedassets1") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared1 );
	
	wxMenuItem* m_menushared2;
	m_menushared2 = new wxMenuItem( m_submenushared, wxID_SHARED2, wxString( _("sharedassets2") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared2 );
	
	wxMenuItem* m_menushared3;
	m_menushared3 = new wxMenuItem( m_submenushared, wxID_SHARED3, wxString( _("sharedassets3") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared3 );
	
	wxMenuItem* m_menushared4;
	m_menushared4 = new wxMenuItem( m_submenushared, wxID_SHARED4, wxString( _("sharedassets4") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared4 );
	
	wxMenuItem* m_menushared5;
	m_menushared5 = new wxMenuItem( m_submenushared, wxID_SHARED5, wxString( _("sharedassets5") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared5 );
	
	wxMenuItem* m_menushared6;
	m_menushared6 = new wxMenuItem( m_submenushared, wxID_SHARED6, wxString( _("sharedassets6") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared6 );
	
	wxMenuItem* m_menushared7;
	m_menushared7 = new wxMenuItem( m_submenushared, wxID_SHARED7, wxString( _("sharedassets7") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared7 );
	
	wxMenuItem* m_menushared8;
	m_menushared8 = new wxMenuItem( m_submenushared, wxID_SHARED8, wxString( _("sharedassets8") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared8 );
	
	wxMenuItem* m_menushared9;
	m_menushared9 = new wxMenuItem( m_submenushared, wxID_SHARED9, wxString( _("sharedassets9") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared9 );
	
	wxMenuItem* m_menushared10;
	m_menushared10 = new wxMenuItem( m_submenushared, wxID_SHARED10, wxString( _("sharedassets10") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared10 );
	
	wxMenuItem* m_menushared11;
	m_menushared11 = new wxMenuItem( m_submenushared, wxID_SHARED11, wxString( _("sharedassets11") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared11 );
	
	wxMenuItem* m_menushared12;
	m_menushared12 = new wxMenuItem( m_submenushared, wxID_SHARED12, wxString( _("sharedassets12") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared12 );
	
	wxMenuItem* m_menushared13;
	m_menushared13 = new wxMenuItem( m_submenushared, wxID_SHARED13, wxString( _("sharedassets13") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared13 );
	
	wxMenuItem* m_menushared14;
	m_menushared14 = new wxMenuItem( m_submenushared, wxID_SHARED14, wxString( _("sharedassets14") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared14 );
	
	wxMenuItem* m_menushared15;
	m_menushared15 = new wxMenuItem( m_submenushared, wxID_SHARED15, wxString( _("sharedassets15") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared15 );
	
	wxMenuItem* m_menushared16;
	m_menushared16 = new wxMenuItem( m_submenushared, wxID_SHARED16, wxString( _("sharedassets16") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared16 );
	
	wxMenuItem* m_menushared17;
	m_menushared17 = new wxMenuItem( m_submenushared, wxID_SHARED17, wxString( _("sharedassets17") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared17 );
	
	wxMenuItem* m_menushared18;
	m_menushared18 = new wxMenuItem( m_submenushared, wxID_SHARED18, wxString( _("sharedassets18") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared18 );
	
	wxMenuItem* m_menushared19;
	m_menushared19 = new wxMenuItem( m_submenushared, wxID_SHARED19, wxString( _("sharedassets19") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared19 );
	
	wxMenuItem* m_menushared20;
	m_menushared20 = new wxMenuItem( m_submenushared, wxID_SHARED20, wxString( _("sharedassets20") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared20 );
	
	wxMenuItem* m_menushared21;
	m_menushared21 = new wxMenuItem( m_submenushared, wxID_SHARED21, wxString( _("sharedassets21") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared21 );
	
	wxMenuItem* m_menushared22;
	m_menushared22 = new wxMenuItem( m_submenushared, wxID_SHARED22, wxString( _("sharedassets22") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared22 );
	
	wxMenuItem* m_menushared23;
	m_menushared23 = new wxMenuItem( m_submenushared, wxID_SHARED23, wxString( _("sharedassets23") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared23 );
	
	wxMenuItem* m_menushared24;
	m_menushared24 = new wxMenuItem( m_submenushared, wxID_SHARED24, wxString( _("sharedassets24") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared24 );
	
	wxMenuItem* m_menushared25;
	m_menushared25 = new wxMenuItem( m_submenushared, wxID_SHARED25, wxString( _("sharedassets25") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared25 );
	
	wxMenuItem* m_menushared26;
	m_menushared26 = new wxMenuItem( m_submenushared, wxID_SHARED26, wxString( _("sharedassets26") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared26 );
	
	wxMenuItem* m_menushared27;
	m_menushared27 = new wxMenuItem( m_submenushared, wxID_SHARED27, wxString( _("sharedassets27") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared27 );
	
	wxMenuItem* m_menushared28;
	m_menushared28 = new wxMenuItem( m_submenushared, wxID_SHARED28, wxString( _("sharedassets28") ) , wxEmptyString, wxITEM_NORMAL );
	m_submenushared->Append( m_menushared28 );
	
	m_menuassets->Append( m_submenusharedItem );
	
	m_menubar->Append( m_menuassets, _("Archive") ); 
	
	m_menuoptions = new wxMenu();
	m_menuexportpath = new wxMenuItem( m_menuoptions, wxID_PATH, wxString( _("Export/Import with Full Path") ) , wxEmptyString, wxITEM_CHECK );
	m_menuoptions->Append( m_menuexportpath );
	m_menuexportpath->Check( true );
	
	m_menuoptions->AppendSeparator();
	
	m_menuconvertimg = new wxMenu();
	wxMenuItem* m_menuconvertimgItem = new wxMenuItem( m_menuoptions, wxID_ANY, _("Automatically Convert Images"), wxEmptyString, wxITEM_NORMAL, m_menuconvertimg );
	m_menuconvertimgnone = new wxMenuItem( m_menuconvertimg, wxID_ANY, wxString( _("Don't Convert") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimg->Append( m_menuconvertimgnone );
	
	m_menuconvertimgbmp = new wxMenuItem( m_menuconvertimg, wxID_ANY, wxString( _("BMP") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimg->Append( m_menuconvertimgbmp );
	
	m_menuconvertimgpng = new wxMenuItem( m_menuconvertimg, wxID_ANY, wxString( _("PNG") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimg->Append( m_menuconvertimgpng );
	m_menuconvertimgpng->Check( true );
	
	m_menuconvertimgtga = new wxMenuItem( m_menuconvertimg, wxID_ANY, wxString( _("TGA") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimg->Append( m_menuconvertimgtga );
	
	m_menuconvertimgtiff = new wxMenuItem( m_menuconvertimg, wxID_ANY, wxString( _("TIFF") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimg->Append( m_menuconvertimgtiff );
	
	m_menuoptions->Append( m_menuconvertimgItem );
	
	m_menuconvertimgquality = new wxMenu();
	wxMenuItem* m_menuconvertimgqualityItem = new wxMenuItem( m_menuoptions, wxID_ANY, _("Image Quality Conversion"), wxEmptyString, wxITEM_NORMAL, m_menuconvertimgquality );
	m_menuconvertimgqualitysame = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("Default Compression Method") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualitysame );
	m_menuconvertimgqualitysame->Check( true );
	
	m_menuconvertimgqualityalpha = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("Alpha Only") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualityalpha );
	
	m_menuconvertimgqualityrgb = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("RGB") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualityrgb );
	
	m_menuconvertimgqualityrgba = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("RGBA") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualityrgba );
	
	m_menuconvertimgqualityargb = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("ARGB") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualityargb );
	
	m_menuconvertimgqualitydxt1 = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("DXT1") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualitydxt1 );
	
	m_menuconvertimgqualitydxt5 = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("DXT5") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualitydxt5 );
	
	m_menuconvertimgquality->AppendSeparator();
	
	m_menuconvertimgqualitylow = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("Low Quality Algorithm") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualitylow );
	
	m_menuconvertimgqualitymedium = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("Medium Quality Algorithm") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualitymedium );
	
	m_menuconvertimgqualityhigh = new wxMenuItem( m_menuconvertimgquality, wxID_ANY, wxString( _("High Quality Algorithm") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertimgquality->Append( m_menuconvertimgqualityhigh );
	m_menuconvertimgqualityhigh->Check( true );
	
	m_menuoptions->Append( m_menuconvertimgqualityItem );
	
	m_menuoptions->AppendSeparator();
	
	m_menuconvertaudio = new wxMenu();
	wxMenuItem* m_menuconvertaudioItem = new wxMenuItem( m_menuoptions, wxID_ANY, _("Automatically Convert Audio"), wxEmptyString, wxITEM_NORMAL, m_menuconvertaudio );
	m_menuconvertaudionone = new wxMenuItem( m_menuconvertaudio, wxID_ANY, wxString( _("Don't Convert") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertaudio->Append( m_menuconvertaudionone );
	
	m_menuconvertaudioakb = new wxMenuItem( m_menuconvertaudio, wxID_ANY, wxString( _("Remove AKB Header") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertaudio->Append( m_menuconvertaudioakb );
	m_menuconvertaudioakb->Check( true );
	
	m_menuoptions->Append( m_menuconvertaudioItem );
	
	m_menuoptions->AppendSeparator();
	
	m_menuconvertmodel = new wxMenu();
	wxMenuItem* m_menuconvertmodelItem = new wxMenuItem( m_menuoptions, wxID_ANY, _("Automatically Convert 3D Models"), wxEmptyString, wxITEM_NORMAL, m_menuconvertmodel );
	m_menuconvertmodelnone = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Don't Convert") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuconvertmodelnone );
	
	m_menuconvertmodelfbxbin = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Convert as Binary FBX") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuconvertmodelfbxbin );
	m_menuconvertmodelfbxbin->Check( true );
	
	m_menuconvertmodelfbxtext = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Convert as ASCII FBX") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuconvertmodelfbxtext );
	
	m_menuconvertmodelautocad = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Convert as AutoCAD DXF") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuconvertmodelautocad );
	
	m_menuconvertmodelcollada = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Convert as Collada") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuconvertmodelcollada );
	
	m_menuconvertmodelwave = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Convert as Wavefront OBJ") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuconvertmodelwave );
	
	m_menuconvertmodel->AppendSeparator();
	
	m_menuimportmodelexistingfiles = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Only Update Exisiting Nodes") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuimportmodelexistingfiles );
	
	m_menuimportmodelmerge = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Update Existing Nodes and Import New Nodes") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuimportmodelmerge );
	m_menuimportmodelmerge->Check( true );
	
	m_menuimportmodelimportall = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Import All the Nodes in New Files") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuimportmodelimportall );
	
	m_menuconvertmodel->AppendSeparator();
	
	m_menuimportmodeldontflush = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Keep Unused Node Files") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuimportmodeldontflush );
	m_menuimportmodeldontflush->Check( true );
	
	m_menuimportmodelflush = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Delete Unused Node Files") ) , wxEmptyString, wxITEM_RADIO );
	m_menuconvertmodel->Append( m_menuimportmodelflush );
	m_menuimportmodelflush->Enable( false );
	
	m_menuconvertmodel->AppendSeparator();
	
	m_menuimportmodelmesh = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Import Meshes/Materials") ) , wxEmptyString, wxITEM_CHECK );
	m_menuconvertmodel->Append( m_menuimportmodelmesh );
	m_menuimportmodelmesh->Check( true );
	
	m_menuimportmodelanims = new wxMenuItem( m_menuconvertmodel, wxID_ANY, wxString( _("Import Animations") ) , wxEmptyString, wxITEM_CHECK );
	m_menuconvertmodel->Append( m_menuimportmodelanims );
	m_menuimportmodelanims->Check( true );
	
	m_menuoptions->Append( m_menuconvertmodelItem );
	
	m_menuoptions->AppendSeparator();
	
	m_menufolderx64 = new wxMenuItem( m_menuoptions, wxID_FOLDER64, wxString( _("Folder x64") ) , wxEmptyString, wxITEM_RADIO );
	m_menuoptions->Append( m_menufolderx64 );
	m_menufolderx64->Check( true );
	
	m_menufolderx86 = new wxMenuItem( m_menuoptions, wxID_FOLDER86, wxString( _("Folder x86") ) , wxEmptyString, wxITEM_RADIO );
	m_menuoptions->Append( m_menufolderx86 );
	
	m_menubar->Append( m_menuoptions, _("Options") ); 
	
	this->SetMenuBar( m_menubar );
	
	wxFlexGridSizer* fgSizer85;
	fgSizer85 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer85->AddGrowableCol( 0 );
	fgSizer85->AddGrowableRow( 1 );
	fgSizer85->SetFlexibleDirection( wxBOTH );
	fgSizer85->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_loadgauge = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL|wxGA_SMOOTH );
	m_loadgauge->SetValue( 0 ); 
	m_loadgauge->SetForegroundColour( wxColour( 28, 153, 255 ) );
	m_loadgauge->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	fgSizer85->Add( m_loadgauge, 0, wxALL|wxEXPAND, 5 );
	
	m_assetlist = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxNO_BORDER );
	fgSizer85->Add( m_assetlist, 0, wxALL|wxEXPAND, 1 );
	
	
	this->SetSizer( fgSizer85 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_menuopen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menureopen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menuclose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata12->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata15->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata16->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata17->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata18->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata19->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata61->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata62->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata63->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menupdata7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel0->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel12->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel15->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel16->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel17->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel18->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel19->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel20->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel21->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel22->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel23->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel24->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel25->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel26->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menulevel27->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menuresources->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menumaindata->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared0->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared6->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared8->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared9->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared12->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared15->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared16->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared17->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared18->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared19->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared20->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared21->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared22->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared23->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared24->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared25->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared26->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared27->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menushared28->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menufolderx64->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Connect( m_menufolderx86->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	m_assetlist->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( UnityViewerWindow::OnSortColumn ), NULL, this );
	m_assetlist->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( UnityViewerWindow::OnAssetRightClick ), NULL, this );
}

UnityViewerWindow::~UnityViewerWindow()
{
	// Disconnect Events
	this->Disconnect( wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_REOPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA11, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA12, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA13, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA14, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA15, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA16, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA17, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA18, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA19, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA2, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA3, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA4, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA5, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA61, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA62, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA63, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_PDATA7, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL0, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL2, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL3, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL4, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL5, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL6, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL7, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL8, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL10, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL11, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL12, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL13, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL14, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL15, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL16, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL17, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL18, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL19, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL20, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL21, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL22, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL23, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL24, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL25, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL26, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_LEVEL27, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_RESOURCES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_MAINDATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED0, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED2, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED3, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED4, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED5, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED6, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED7, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED8, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED9, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED10, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED11, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED12, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED13, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED14, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED15, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED16, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED17, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED18, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED19, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED20, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED21, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED22, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED23, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED24, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED25, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED26, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED27, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_SHARED28, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_FOLDER64, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	this->Disconnect( wxID_FOLDER86, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( UnityViewerWindow::OnMenuSelection ) );
	m_assetlist->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( UnityViewerWindow::OnSortColumn ), NULL, this );
	m_assetlist->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( UnityViewerWindow::OnAssetRightClick ), NULL, this );
	
}

UnityLinkFileWindow::UnityLinkFileWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer197;
	bSizer197 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer46;
	gbSizer46 = new wxGridBagSizer( 0, 0 );
	gbSizer46->SetFlexibleDirection( wxBOTH );
	gbSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_message = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 525,80 ), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxSIMPLE_BORDER );
	m_message->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INFOBK ) );
	
	gbSizer46->Add( m_message, wxGBPosition( 0, 0 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	wxArrayString m_existinglistChoices;
	m_existinglist = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_existinglistChoices, 0 );
	m_existinglist->SetSelection( 0 );
	gbSizer46->Add( m_existinglist, wxGBPosition( 1, 0 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_staticText329 = new wxStaticText( this, wxID_ANY, _("File Internal ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText329->Wrap( -1 );
	gbSizer46->Add( m_staticText329, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_fileinfo = new wxTextCtrl( this, wxID_ANY, _("FFFFFFFFFFFFFFFF"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
	#ifdef __WXGTK__
	if ( !m_fileinfo->HasFlag( wxTE_MULTILINE ) )
	{
	m_fileinfo->SetMaxLength( 16 );
	}
	#else
	m_fileinfo->SetMaxLength( 16 );
	#endif
	gbSizer46->Add( m_fileinfo, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL, 3 );
	
	
	gbSizer46->AddGrowableCol( 1 );
	
	bSizer197->Add( gbSizer46, 1, wxEXPAND, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer197->Add( m_buttonok, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	this->SetSizer( bSizer197 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_existinglist->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( UnityLinkFileWindow::OnFileSelect ), NULL, this );
	m_fileinfo->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UnityLinkFileWindow::OnFileInfoEdit ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityLinkFileWindow::OnButtonClick ), NULL, this );
}

UnityLinkFileWindow::~UnityLinkFileWindow()
{
	// Disconnect Events
	m_existinglist->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( UnityLinkFileWindow::OnFileSelect ), NULL, this );
	m_fileinfo->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UnityLinkFileWindow::OnFileInfoEdit ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityLinkFileWindow::OnButtonClick ), NULL, this );
	
}

UnityAddFileWindow::UnityAddFileWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer197;
	bSizer197 = new wxBoxSizer( wxVERTICAL );
	
	m_filebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer197->Add( m_filebook, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer89;
	fgSizer89 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer89->AddGrowableCol( 1 );
	fgSizer89->SetFlexibleDirection( wxBOTH );
	fgSizer89->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_buttonadd = new wxButton( this, wxID_ADD, _("Add Another File"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer89->Add( m_buttonadd, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer196;
	bSizer196 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttoncancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer196->Add( m_buttoncancel, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_buttonok = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer196->Add( m_buttonok, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	fgSizer89->Add( bSizer196, 1, wxALIGN_RIGHT, 5 );
	
	
	bSizer197->Add( fgSizer89, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer197 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_buttonadd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFileWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFileWindow::OnButtonClick ), NULL, this );
	m_buttonok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFileWindow::OnButtonClick ), NULL, this );
}

UnityAddFileWindow::~UnityAddFileWindow()
{
	// Disconnect Events
	m_buttonadd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFileWindow::OnButtonClick ), NULL, this );
	m_buttoncancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFileWindow::OnButtonClick ), NULL, this );
	m_buttonok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFileWindow::OnButtonClick ), NULL, this );
	
}

UnityAddFilePanel::UnityAddFilePanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxGridBagSizer* gbSizer46;
	gbSizer46 = new wxGridBagSizer( 0, 0 );
	gbSizer46->SetFlexibleDirection( wxBOTH );
	gbSizer46->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_filepicker = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, _("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	gbSizer46->Add( m_filepicker, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxALL|wxEXPAND, 5 );
	
	m_staticText333 = new wxStaticText( this, wxID_ANY, _("Type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText333->Wrap( -1 );
	gbSizer46->Add( m_staticText333, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	wxArrayString m_filetypeChoices;
	m_filetype = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), m_filetypeChoices, 0 );
	m_filetype->SetSelection( 0 );
	gbSizer46->Add( m_filetype, wxGBPosition( 1, 1 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_staticText332 = new wxStaticText( this, wxID_ANY, _("Internal Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText332->Wrap( -1 );
	gbSizer46->Add( m_staticText332, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_fileinternalname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	m_fileinternalname->SetToolTip( _("Only a few file types can have an internal name") );
	
	gbSizer46->Add( m_fileinternalname, wxGBPosition( 2, 1 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_staticText334 = new wxStaticText( this, wxID_ANY, _("File Internal ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText334->Wrap( -1 );
	gbSizer46->Add( m_staticText334, wxGBPosition( 3, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_fileinfo = new wxTextCtrl( this, wxID_ANY, _("FFFFFFFFFFFFFFFF"), wxDefaultPosition, wxSize( 140,-1 ), 0 );
	#ifdef __WXGTK__
	if ( !m_fileinfo->HasFlag( wxTE_MULTILINE ) )
	{
	m_fileinfo->SetMaxLength( 16 );
	}
	#else
	m_fileinfo->SetMaxLength( 16 );
	#endif
	m_fileinfo->SetToolTip( _("ID is valid and unused") );
	
	gbSizer46->Add( m_fileinfo, wxGBPosition( 3, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_generateinfo = new wxButton( this, wxID_ANY, _("Generate unused ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_generateinfo->SetToolTip( _("Generate a valid internal file ID") );
	
	gbSizer46->Add( m_generateinfo, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	m_addbundleinfo = new wxCheckBox( this, wxID_ANY, _("Add Bundle Info"), wxDefaultPosition, wxDefaultSize, 0 );
	m_addbundleinfo->SetToolTip( _("Bundle info can be added if the archive\ncontains a file of type AssetBundle") );
	
	gbSizer46->Add( m_addbundleinfo, wxGBPosition( 4, 0 ), wxGBSpan( 1, 2 ), wxALL, 5 );
	
	m_panel90 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer88;
	fgSizer88 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer88->SetFlexibleDirection( wxBOTH );
	fgSizer88->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_filenamelabel = new wxStaticText( m_panel90, wxID_ANY, _("Full Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_filenamelabel->Wrap( -1 );
	fgSizer88->Add( m_filenamelabel, 0, wxALL, 5 );
	
	m_filename = new wxTextCtrl( m_panel90, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 330,-1 ), 0 );
	m_filename->SetToolTip( _("Full names are used by the game's engine") );
	
	fgSizer88->Add( m_filename, 0, wxALL, 5 );
	
	
	m_panel90->SetSizer( fgSizer88 );
	m_panel90->Layout();
	fgSizer88->Fit( m_panel90 );
	gbSizer46->Add( m_panel90, wxGBPosition( 5, 0 ), wxGBSpan( 1, 3 ), wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( gbSizer46 );
	this->Layout();
	gbSizer46->Fit( this );
	
	// Connect Events
	m_filepicker->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( UnityAddFilePanel::OnFilePick ), NULL, this );
	m_filetype->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( UnityAddFilePanel::OnTypeChoice ), NULL, this );
	m_fileinfo->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UnityAddFilePanel::OnFileInfoEdit ), NULL, this );
	m_generateinfo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFilePanel::OnButtonClick ), NULL, this );
	m_addbundleinfo->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UnityAddFilePanel::OnCheckBox ), NULL, this );
}

UnityAddFilePanel::~UnityAddFilePanel()
{
	// Disconnect Events
	m_filepicker->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( UnityAddFilePanel::OnFilePick ), NULL, this );
	m_filetype->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( UnityAddFilePanel::OnTypeChoice ), NULL, this );
	m_fileinfo->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UnityAddFilePanel::OnFileInfoEdit ), NULL, this );
	m_generateinfo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( UnityAddFilePanel::OnButtonClick ), NULL, this );
	m_addbundleinfo->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UnityAddFilePanel::OnCheckBox ), NULL, this );
	
}

MenuDEBUG::MenuDEBUG( long style ) : wxMenuBar( style )
{
	m_menu6 = new wxMenu();
	wxMenuItem* debug;
	debug = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("DEBUG") ) + wxT('\t') + wxT("F5"), _("Does some debugging action"), wxITEM_NORMAL );
	m_menu6->Append( debug );
	
	Append( m_menu6, _("MyMenu") ); 
	
	
	// Connect Events
	this->Connect( debug->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuDEBUG::OnDebugClick ) );
}

MenuDEBUG::~MenuDEBUG()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuDEBUG::OnDebugClick ) );
	
}
