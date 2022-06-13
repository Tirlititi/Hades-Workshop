#ifndef _GUI_WALKMESHEDITOR_H
#define _GUI_WALKMESHEDITOR_H

class WalkmeshEditDialog;

#include "gui.h"
#include "Gui_GLWindow.h"
#include "File_Manipulation.h"
#include "Configuration.h"
using namespace std;

class WalkmeshEditDialog : public WalkmeshEditWindow {
public:
	FieldWalkmeshDataStruct walkmesh;
	uint32_t extra_size;
	vector<vector<int>> duplicate_vertex;
	//vector<vector<int>> path_connection;
	GLWindow* gl_window;
	int path_selection;
	int triangle_selection;

	WalkmeshEditDialog(wxWindow* parent, FieldTilesDataStruct* tiles, FieldWalkmeshDataStruct* walk);
	~WalkmeshEditDialog();
	int ShowModal();
	void ComputeDuplicateVertice(vector<int> news);
	void DisplayWalpath(int sel);
	void DisplayTriangle(int sel);
	void ChangeVertexCoordinate(int triangleid, int vertid, int coordaxis, int16_t c);
	void ChangeWalkpathCoordinate(int pathid, int coordaxis, int16_t c);

private:
	wxMenu* path_popup_menu;
	wxMenu* triangle_popup_menu;

	void OnListBox(wxCommandEvent& event);
	void OnDisplayDClick(wxMouseEvent& event);
	void OnWalkpathRightClick(wxMouseEvent& event);
	void OnTriangleRightClick(wxMouseEvent& event);
	void OnWalkpathRightClickMenu(wxCommandEvent& event);
	void OnTriangleRightClickMenu(wxCommandEvent& event);
	void OnSpinCtrl(wxSpinEvent& event);
	void OnSpinCtrlDouble(wxSpinDoubleEvent& event);
	void OnChoice(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
