#ifndef _FILE_BATCHING_H
#define _FILE_BATCHING_H

struct BatchImportLog;
class BatchExportDialog;
class BatchImportDialog;

#include "Configuration.h"
#include "gui.h"

int FileBatch_ExportText(wxString path, TextDataSet& data, bool* exportlist = NULL, bool splitfile = true);
LogStruct FileBatch_ImportText(wxString filetext, TextDataSet& data, bool adjustsize = true, bool isjapan = false, bool fatalwarning = false);
int FileBatch_ExportSpecialText(wxString path, SpecialTextDataSet& data, bool* exportlist = NULL, bool splitfile = true);
LogStruct FileBatch_ImportSpecialText(wxString filetext, SpecialTextDataSet& data, bool fatalwarning = false);

int FileBatch_ExportEnemyScript(wxString path, EnemyDataSet& data, bool* exportlist = NULL, bool splitfile = true);
int FileBatch_ExportWorldScript(wxString path, WorldMapDataSet& data, bool* exportlist = NULL, bool splitfile = true);
int FileBatch_ExportFieldScript(wxString path, FieldDataSet& data, bool* exportlist = NULL, bool splitfile = true);

int FileBatch_ExportImageBackground(wxString path, FieldDataSet& data, bool* exportlist = NULL, bool mergetile = true);

//int FileBatch_ExportSceneModel(wxString path, BattleSceneDataSet& data, bool* exportlist = NULL);

class BatchExportDialog : public BatchExportWindow {
public:
	// 1 : texts
	// 2 : ui texts
	// 3 : enemy scripts
	// 4 : world scripts
	// 5 : field scripts
	// 10 : field backgrounds
	int datatype;
	unsigned int* sortlist;
	SaveSet* dataset;
	
	BatchExportDialog(wxWindow* p);
	~BatchExportDialog();
	int ShowModal(int type, SaveSet* datas, wxArrayString objlist, unsigned int* objlistsort);

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
	
	BatchImportDialog(wxWindow* p);
	~BatchImportDialog();
	int ShowModal(int type, SaveSet* datas, bool isjapan);

private:
	wxMenu* list_popup_menu;
	
	void OnFilePick(wxFileDirPickerEvent& event);
	void OnListRightClick(wxMouseEvent& event);
	void OnSelectRightClickMenu(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
