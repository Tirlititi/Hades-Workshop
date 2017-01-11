#ifndef _GUI_TOOLS_H
#define _GUI_TOOLS_H

class ToolModManager;

#include "Configuration.h"
#include "Gui_Manipulation.h"
#include "gui.h"

class ToolModManager : public ModManagerWindow {
public:
	CDDataStruct* cddata;
	
	ToolModManager(wxWindow* p);
	~ToolModManager();
	int ShowModal(CDDataStruct* data);

private:
	wxMenu* list_popup_menu;
	
	void OnTreeListCheck(wxTreeListEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class ToolBackgroundEditor : public BackgroundEditorWindow {
public:
	CDDataStruct* cddata;
	
	ToolBackgroundEditor(wxWindow* p);
	~ToolBackgroundEditor();
	int ShowModal(CDDataStruct* data);

private:
	wxBitmap main_img;
	wxBitmap tile_img;
	wxImage main_img_base;
	wxImage tile_img_base;
	
	void DrawImage(wxDC& dc);
	void UpdateImage();
	void LoadAndMergeImages();
	void ComputeTileFilter(int x = -1, int y = -1);
	
	void OnFilePick(wxFileDirPickerEvent& event);
	void OnRadioClick(wxCommandEvent& event);
	void OnFieldChoice(wxCommandEvent& event);
	void OnTileSelect(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnSpinChange(wxSpinEvent& event);
	void OnButtonClick(wxCommandEvent& event);
	void OnPaintImage(wxPaintEvent& event);
};

#endif
