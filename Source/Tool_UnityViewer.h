#ifndef _GUI_UNITYVIEWER_H
#define _GUI_UNITYVIEWER_H

#include "UnityArchiver.h"
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
	
	void UpdateMenuAvailability();
	
	void OnMenuSelection(wxCommandEvent& event);
	void OnAssetRightClickMenu(wxCommandEvent& event);
	void OnAssetRightClick(wxListEvent& event);
	void OnSortColumn(wxListEvent& event);
};

#endif
