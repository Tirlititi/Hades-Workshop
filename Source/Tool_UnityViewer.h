#ifndef _GUI_UNITYVIEWER_H
#define _GUI_UNITYVIEWER_H

#include "UnityArchiver.h"
#include "gui.h"

class ToolUnityViewer : public UnityViewerWindow {
public:
	wxString root_path;
	int column_sort;
	bool column_sort_ascending;
	
	UnityArchiveMetaData meta_data[UNITY_ARCHIVE_AMOUNT];
	UnityArchiveAssetBundle bundle_data[UNITY_ARCHIVE_DATA7-UNITY_ARCHIVE_DATA11+1];
	UnityArchiveIndexListData list_data;
	wxArrayString list_data_filename;
	
	ToolUnityViewer(wxWindow* parent);
	
	bool SetupRootPath(wxString path);
	bool DisplayArchive(UnityArchiveFile filetype);

private:
	void OnMenuSelection(wxCommandEvent& event);
	void OnSortColumn(wxListEvent& event);
};

#endif
