#ifndef _GUI_UNITYVIEWER_H
#define _GUI_UNITYVIEWER_H

class ToolUnityViewer;
class UnityLinkFileDialog;
class UnityAddFileDialog;
class UnityAddFilePanelDialog;

#include "UnityArchiver.h"
#include "GameObject.h"
#include "ModelMesh.h"
#include "gui.h"

class ToolUnityViewer : public UnityViewerWindow {
public:
	UnityArchiveFile current_archive;
	wxString archive_name;
	wxString root_path;
	bool root_path_ok;
	int column_sort;
	bool column_sort_ascending;
	bool use_x86;
	
	UnityArchiveMetaData meta_data[UNITY_ARCHIVE_AMOUNT];
	UnityArchiveAssetBundle bundle_data[UNITY_ARCHIVE_DATA7-UNITY_ARCHIVE_DATA11+1];
	UnityArchiveIndexListData list_data;
	UnityArchiveDictionary audio_data;
	wxArrayString list_data_filename;
	
	ToolUnityViewer(wxWindow* parent);
	~ToolUnityViewer();
	
	bool SetupRootPath(wxString path, bool ignorestreaming = false);
	bool DisplayArchive(UnityArchiveFile archivetype);
	wxString GetFullName(UnityArchiveFile archivetype, unsigned int fileid, bool* found = NULL);
	wxString GetInfoString(wxString filename, uint32_t filetype, UnityArchiveFile archive);

private:
	wxMenu* assetmenu;
	wxMenuItem* assetmenuexport;
	wxMenuItem* assetmenuimport;
	wxMenuItem* assetmenuadd;
	
	void UpdateMenuAvailability();
	bool PrepareAssetForImport(bool isnewfile, fstream& filebase, wxString path, uint32_t ftype, vector<ModelDataStruct>& importmodel, UnityArchiveFileCreator& filestoadd, UnityLinkFileDialog& linkfiledialog, bool linkfiledialoginit, /*	Common Arguments
							*/ uint32_t* filenewsize, bool* copylist, unsigned int impfileid, long it, wxString filearchivedir, vector<GameObjectHierarchy>* importmodelbasehierarchy, /* Replace file
							*/ uint64_t newfileinfo, string newfileinternalname, string newfilepath); // New file
	bool PerformImportOfAsset(bool isnewfile, fstream& filebase, fstream& filedest, UnityArchiveMetaData& newmetadata, wxString path, uint32_t ftype, vector<ModelDataStruct>& importmodel, unsigned int& importmodelcounter, unsigned int impfileid);

	void OnMenuSelection(wxCommandEvent& event);
	void OnAssetRightClickMenu(wxCommandEvent& event);
	void OnAssetRightClick(wxListEvent& event);
	void OnSortColumn(wxListEvent& event);
};

class UnityLinkFileDialog : public UnityLinkFileWindow {
public:
	wxArrayString filelist;
	vector<uint64_t> fileinfolist;

	uint64_t info_selected;

	UnityLinkFileDialog(wxWindow* parent) : UnityLinkFileWindow(parent), infovalidator(NULL) {}
	int ShowModal(wxString message);

private:
	wxTextValidator* infovalidator;

	void OnFileSelect(wxCommandEvent& event);
	void OnFileInfoEdit(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class UnityAddFileDialog : public UnityAddFileWindow {
public:
	UnityArchiveMetaData* meta_data;
	unsigned int panel_amount;
	vector<uint32_t> file_type_choice;
	wxArrayString file_type_name;
	bool has_bundle;

	UnityAddFileDialog(wxWindow* parent, UnityArchiveMetaData* metadata) : UnityAddFileWindow(parent), meta_data(metadata) {}
	int ShowModal();
	bool IsInfoUnused(uint64_t info, int paneltoignore = -1);
	void GenerateValidInfo(unsigned int panelid);
	int VerifyInfoValidity();

private:
	void AddPanel();

	void OnButtonClick(wxCommandEvent& event);
};

class UnityAddFilePanelDialog : public UnityAddFilePanel {
public:
	UnityAddFilePanelDialog(UnityAddFileDialog* parent);
	void EnableBundlePanel(bool enable = true);

private:
	UnityAddFileDialog* parent;
	wxTextValidator* infovalidator;

	void OnFilePick(wxFileDirPickerEvent& event);
	void OnTypeChoice(wxCommandEvent& event);
	void OnFileInfoEdit(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
