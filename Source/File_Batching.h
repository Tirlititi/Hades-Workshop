#ifndef _FILE_BATCHING_H
#define _FILE_BATCHING_H

struct BatchImportLog;
class BatchExportDialog;
class BatchImportDialog;

#include "Configuration.h"
#include "gui.h"

#define BATCHING_SCRIPT_INFO_FILENAME	1
#define BATCHING_SCRIPT_INFO_ARGUMENT	2
#define BATCHING_SCRIPT_INFO_TEXT_LINK	4
#define BATCHING_SCRIPT_INFO_ALL		7

class BatchExportDialog : public BatchExportWindow {
public:
	int datatype;
	unsigned int* sortlist;
	SaveSet* dataset;
	bool* dataloaded;
	
	BatchExportDialog(wxWindow* p);
	~BatchExportDialog();
	int ShowModal(int type, SaveSet* datas, wxArrayString objlist, unsigned int* objlistsort);

	static int ExportText(TextDataSet& data, wxString path, bool* exportlist = NULL, bool splitfile = true);
	static int ExportWorldTexts(WorldMapDataSet& data, wxString path);
	static int ExportBattleTexts(EnemyDataSet& data, wxString path, bool* exportlist = NULL, bool splitfile = true);
	static int ExportWorldPlaceNames(WorldMapDataStruct& data, wxString path);
	static int ExportSpecialText(SpecialTextDataSet& data, wxString path, bool* exportlist = NULL, bool splitfile = true);
	static int ExportCardNames(CardDataSet& data, wxString path);
	static int ExportLocalizationEntries(SpecialTextDataStruct& data, wxString path);
	static int ExportSpellNames(SpellDataSet& data, wxString path);
	static int ExportSupportNames(SupportDataSet& data, wxString path);
	static int ExportCommandNames(CommandDataSet& data, wxString path);
	static int ExportItemNames(ItemDataSet& data, wxString path);
	static int ExportKeyItemNames(ItemDataSet& data, wxString path);
	static int ExportFieldNames(FieldDataSet& data, wxString path);
	static int ExportCharacterNames(StatDataSet& data, wxString path);
	static int ExportEnemyScript(SaveSet* dataset, wxString path, bool* exportlist = NULL, bool splitfile = true, bool appendmode = false, int addedinfo = 0);
	static int ExportWorldScript(SaveSet* dataset, wxString path, bool* exportlist = NULL, bool splitfile = true, bool appendmode = false, int addedinfo = 0);
	static int ExportFieldScript(SaveSet* dataset, wxString path, bool* exportlist = NULL, bool splitfile = true, bool appendmode = false, int addedinfo = 0);
	static int ExportImageBackground(FieldDataSet& data, wxString path, bool* exportlist = NULL, bool mergetile = true, bool depthorder = true, int steamtitlelang = -1);
	static int ExportWalkmesh(FieldDataSet& data, wxString path, bool* exportlist);

private:
	wxMenu* list_popup_menu;
	
	void OnFilePick(wxFileDirPickerEvent& event);
	void OnSplitFileCheck(wxCommandEvent& event);
	void OnListRightClick(wxMouseEvent& event);
	void OnSelectRightClickMenu(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class BatchImportDialog : public BatchImportWindow {
public:
	int datatype;
	SaveSet* dataset;
	bool japanversion;
	bool datamodified;
	set<int> datasectionmodified;
	
	BatchImportDialog(wxWindow* p);
	~BatchImportDialog();
	int ShowModal(int type, SaveSet* datas, bool isjapan);

	static LogStruct ImportText(SaveSet* dataset, set<int>& sectionmodified, wxString filetext, bool adjustsize = true, bool isjapan = false, bool fatalwarning = false);
	static LogStruct ImportScript(SaveSet* dataset, set<int>& sectionmodified, int scripttype, wxString filescript, bool fatalwarning = false, bool* datamodif = NULL, vector<ScriptDataStruct*>* modifiedscripts = NULL);

private:
	wxMenu* list_popup_menu;
	
	void OnFilePick(wxFileDirPickerEvent& event);
	void OnListRightClick(wxMouseEvent& event);
	void OnSelectRightClickMenu(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
