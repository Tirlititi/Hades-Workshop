/*********************************************************************
 *  Hades Workshop
 *  by Tirliti
 * 
 *  Program reading and editing Final Fantasy IX binary images.
 * 
 *  The code source tends to clearly separate the part of the code that
 * uses the GUI api (wxWidgets) and the rest, focused mainly on the file
 * structure and handling data.
 * 
 * 
 *  main.h : init the main loop and declare the main frame's methods.
 * 
 *  gui.h : generated by wxFormBuilder. Declare the GUI's objects.
 * 
 *  Hades_Strings.h : declare most of the constant strings not declared
 * in gui.h. Also sort those strings and declare some of the attributs of
 * the objects linked to them, such as their ID.
 * 
 *  Script_Strings.h : same as Hades_Strings.h, focused on strings script-
 * related.
 * 
 *  Steam_Strings.h : same as Hades_Strings.h, focused on Steam file strings.
 * 
 *  DllEditor.h :  more like a "Assembly-CSharp.dll editor". Used to search,
 * convert, and patch informations contained in the Steam version's main DLL.
 * 
 *  UnityArchiver.h :  handle file extraction/reinsertion of Unity archives,
 * for the Steam version of the game.
 * 
 *  Gui_Preferences.h : define the "Preferences" window.
 * 
 *  Gui_Manipulation.h : mainly declare the CD panel's methods. Handles
 * most of the things related to the data display.
 * 
 *  Gui_GLWindow.h : declare GLWindow, a window specialized in 3D displaying
 * stuff.
 * 
 *  Gui_ScriptEditor.h : declare ScriptEditDialog, a window specialized in
 * displaying and editing the game script code.
 * 
 *  Gui_TextEditor.h : declare TextEditDialog, a window specialized in
 * Final Fantasy IX text displays and edits.
 * 
 *  Gui_TextureEditor.h : declare ManageTextureDialog, a window specialized
 * in simple edition of TIM textures.
 * 
 *  Gui_FieldTextureEditor.h : declare ManageFieldTextureDialog, a window
 * specialized in editing fields' backgrounds.
 * 
 *  Gui_SpellAnimationsEditor.h : declare AnimSequenceEditDialog and its
 * counterpart for enemies. Those are windows specialized in displaying
 * and editing spell animation sequences.
 * 
 *  Gui_CurveEditor.h : declare CurveEditorDialog, a window specialized in
 * displaying and drawing data curves.
 * 
 *  Gui_Tools.h : everything related to what's in the "Tools" menu.
 * 
 *  Gui_LoadingDialog.h : the only gui-related header included in data-
 * processing files. It's an API for displaying a progression bar.
 * 
 *  File_IOCommon.h : declare really basic IO functions used by the program.
 * 
 *  File_Manipulation.h : declare other IO functions used by the program.
 * Also declare the FF9 String struct.
 * 
 *  File_Batching.h : API for exporting and importing several files in
 * one go. Also declare the GUI windows for that.
 * 
 *  File_Recreation.h : API for expanding the size of a binary PSX file.
 * Not currently working and more or less abandoned.
 * 
 *  Configuration.h : handle reading, writing and creating HW files.
 * It scans the PSX and Steam format to create a Configuration set,
 * containing informations about the data offsets and such.
 * It also creates .hwf files for the PSX format, containing this
 * Configuration set. And finally, it handles the .hws format, the custom
 * Hades Workshop format for saving the game's modifications.
 * 
 *  ClusterData.h : this struct is a data-managing object. A lot of the
 * binary file's datas are stored in those.
 * 
 *  ImageMaps.h : this struct handles datas about preloading other datas.
 * A chunk can't be used if it is not preloaded beforehand.
 * 
 *  Texts.h : declare TextDataStruct, an object containing texts and being
 * handled by clusters.
 * 
 *  Charmaps.h : declare CharmapDataStruct, an object containing the positions
 * of letters, characters and UI parts in an image.
 * 
 *  Scripts.h : declare ScriptDataStruct, an object containing fields'
 * or battles' script and being handled by clusters.
 * 
 *  MIPS.h : declare MIPS-related structs. MIPS is the coding language of the
 * PlayStation's microprocessor architecture.
 * 
 *  TIMImages.h : declare TIMImageDataStruct, an object handling images
 * as they are written in FFIX. It provides methods to convert them in
 * raw image datas.
 * 
 *  Enemies.h : declare EnemyDataStruct, an object containing enemies'
 * statistics and being handled by clusters.
 * 
 *  WorldMaps.h : declare WorldMapDataStruct, an object containing yet
 * unknown datas about the world map.
 * 
 *  Fields.h : declare FieldDataSet, an object containing informations
 * about the fields.
 * 
 *  BattleScenes.h : declare BattleSceneDataStruct, an object containing
 * the battleground 3D models.
 * 
 *  Spells.h : declare spell-related types and SpellDataStruct, an
 * object containing attacks used by the party. Also declare most of
 * ability-related constants and types.
 * 
 *  SpellAnimations.h : declare SpellAnimationDataStruct, an object
 * containing datas about the shared attack SFX.
 * 
 *  Supports.h : declare SupportDataStruct, an object containing datas
 * about supporting skills.
 * 
 *  Commands.h : declare CommandDataStruct, an object containing
 * commands used in battle.
 * 
 *  Stats.h : declare structs about characters' statistics and datas.
 * 
 *  Items.h : declare item and key item-related structs.
 * 
 *  Shops.h : declare ShopDataStruct and SynthesisDataStruct, objects
 * containing datas about the items sold or synthsized by merchants.
 * 
 *  Cards.h : declare card and card deck-related structs.
 * 
 *  MenuUI.h : gather structs containing UI-related datas.
 * 
 *  PartySpecials.h : gather the datas specially handling party's spells
 * or commands.
 * 
 *  makeppf3.h : a library for creating PPF 3.0 files. The original
 * files (stand-alone Windows program and source code) can be found at
 * http://aminet.net/package.php?package=util/misc/makeppf_aos4.lha
 *********************************************************************/

#ifndef __main__
#define __main__

// Includes
#include <wx/wx.h>
#include "File_Manipulation.h"
#include "Gui_Manipulation.h"
#include "gui.h"

// Main constants
#define MAX_CD_PANELS 10
namespace hades {
	extern wxColour TEXT_WINDOW_COLOR;
	extern wxFont TEXT_DISPLAY_FONT;
	extern int TEXT_PREVIEW_TYPE;
	extern wchar_t* SPECIAL_STRING_CHARMAP_DEFAULT;
	extern wchar_t* SPECIAL_STRING_CHARMAP_A;
	extern wchar_t* SPECIAL_STRING_CHARMAP_B;
	extern ExtendedCharmap SPECIAL_STRING_CHARMAP_EXT;
	extern wchar_t SPECIAL_STRING_OPCODE_WCHAR;
}

////////////////////////////////////////////////////////////////////////////////
// application class declaration 
////////////////////////////////////////////////////////////////////////////////

class MainApp : public wxApp {
public:
	virtual bool OnInit();
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)

////////////////////////////////////////////////////////////////////////////////
// main application frame declaration 
////////////////////////////////////////////////////////////////////////////////

class MainFrame : public MainFrameBase {
public:
	MainFrame( wxWindow *parent );
	virtual ~MainFrame();
	PreferencesDialog* PreferenceWindow;
	CDDataStruct* CDPanel[MAX_CD_PANELS];
	string CDName[MAX_CD_PANELS];
	bool CDModifiedState[MAX_CD_PANELS];
	unsigned int CDPanelAmount;
	wxFileName SteamSaveDir;
	
	void MarkDataModified(); // Mark the selected CD in the cdnotebook as modified
	void PreferencesUpdate(); // Read and apply configurations from PreferenceWindow
	
protected:
	void RepaintBackground();
	
	// protected event handlers
	virtual void OnCloseFrame( wxCloseEvent& event );
	virtual void OnExitClick( wxCommandEvent& event );
	virtual void OnOpenClick( wxCommandEvent& event );
	virtual void OnOpenHWSClick( wxCommandEvent& event );
	virtual void OnCloseClick( wxCommandEvent& event );
	virtual void OnCloseAllClick( wxCommandEvent& event );
	virtual void OnSaveHWSClick( wxCommandEvent& event );
	virtual void OnSaveSteamClick( wxCommandEvent& event );
	virtual void OnExportPPFClick( wxCommandEvent& event );
	virtual void OnSaveBinClick( wxCommandEvent& event );
	virtual void OnSortSpellClick( wxCommandEvent& event );
	virtual void OnSortSupportClick( wxCommandEvent& event );
	virtual void OnSortCommandClick( wxCommandEvent& event );
	virtual void OnSortEnemyClick( wxCommandEvent& event );
	virtual void OnSortCardClick( wxCommandEvent& event );
	virtual void OnSortItemClick( wxCommandEvent& event );
	virtual void OnSortKeyItemClick( wxCommandEvent& event );
	virtual void OnSortTextClick( wxCommandEvent& event );
	virtual void OnSortWorldMapClick( wxCommandEvent& event );
	virtual void OnSortFieldClick( wxCommandEvent& event );
	virtual void OnSortBattleSceneClick( wxCommandEvent& event );
	virtual void OnSortSpellAnimationClick( wxCommandEvent& event );
	virtual void OnPreferencesClick( wxCommandEvent& event );
	virtual void OnBatchExportClick( wxCommandEvent& event );
	virtual void OnBatchImportClick( wxCommandEvent& event );
	virtual void OnToolClick( wxCommandEvent& event );
	virtual void OnAboutClick( wxCommandEvent& event );
	virtual void OnHelpClick( wxCommandEvent& event );
	virtual void OnPanelChanged( wxAuiNotebookEvent& event );
	virtual void OnFramePaint( wxPaintEvent& event );
	
	virtual void OnDebugClick( wxCommandEvent& event );
	
private:
	void UpdateMenuAvailability(int panel);
};

MainFrame* GetTopWindow();

#endif //__main__