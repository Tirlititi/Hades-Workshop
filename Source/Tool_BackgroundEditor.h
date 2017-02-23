#ifndef _TOOL_BACKGROUNDEDITOR_H
#define _TOOL_BACKGROUNDEDITOR_H

class ToolBackgroundEditor;

#include "Gui_Manipulation.h"
#include "gui.h"

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
	
	void GetFileNamesAndDepth(wxString basename, wxString multibackseparator, wxString endofname, FieldTilesDataStruct& tiledata, bool sortlayer, bool revertorder, wxString*& resname, unsigned int*& resdepth, bool usemultibackground = true);
	void DrawImage(wxDC& dc);
	void UpdateImage();
	void LoadAndMergeImages();
	void ComputeTileFilter(int x = -1, int y = -1);
	
	void OnFilePick(wxFileDirPickerEvent& event);
	void OnDirPick(wxFileDirPickerEvent& event);
	void OnRadioClick(wxCommandEvent& event);
	void OnFieldChoice(wxCommandEvent& event);
	void OnTileSelect(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnSpinChange(wxSpinEvent& event);
	void OnButtonClick(wxCommandEvent& event);
	void OnPaintImage(wxPaintEvent& event);
};

#endif
