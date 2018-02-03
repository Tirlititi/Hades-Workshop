#ifndef _FILE_BATCHING_H
#define _FILE_BATCHING_H

struct BatchImportLog;
class BatchExportDialog;
class BatchImportDialog;

#include "Configuration.h"
#include "gui.h"

#define BATCHING_SCRIPT_INFO_FILENAME	1
#define BATCHING_SCRIPT_INFO_ARGUMENT	2

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
	bool* dataloaded;
	
	BatchExportDialog(wxWindow* p);
	~BatchExportDialog();
	int ShowModal(int type, SaveSet* datas, wxArrayString objlist, unsigned int* objlistsort, bool* dload);

	int ExportText(wxString path, bool* exportlist = NULL, bool splitfile = true);
	int ExportSpecialText(wxString path, bool* exportlist = NULL, bool splitfile = true);
	int ExportEnemyScript(wxString path, bool* exportlist = NULL, bool splitfile = true, int addedinfo = 0);
	int ExportWorldScript(wxString path, bool* exportlist = NULL, bool splitfile = true, int addedinfo = 0);
	int ExportFieldScript(wxString path, bool* exportlist = NULL, bool splitfile = true, int addedinfo = 0);
	int ExportImageBackground(wxString path, bool* exportlist = NULL, bool mergetile = true, bool depthorder = true, int steamtitlelang = -1);

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

	LogStruct ImportText(wxString filetext, bool adjustsize = true, bool isjapan = false, bool fatalwarning = false);
	LogStruct ImportSpecialText(wxString filetext, bool fatalwarning = false);

private:
	wxMenu* list_popup_menu;
	
	void OnFilePick(wxFileDirPickerEvent& event);
	void OnListRightClick(wxMouseEvent& event);
	void OnSelectRightClickMenu(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
