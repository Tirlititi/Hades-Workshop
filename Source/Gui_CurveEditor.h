#ifndef _GUI_CURVEEDITOR_H
#define _GUI_CURVEEDITOR_H

class StatAutoFillDialog;

#include <inttypes.h>
#include "gui.h"

class CurveEditorDialog : public CurveEditorWindow {
public:
	uint32_t* init_x;
	uint32_t* init_y;
	uint32_t* new_x;
	uint32_t* new_y;
	unsigned int val_amount;
	uint32_t max_x;
	uint32_t max_y;
	
	int ShowModal(unsigned int amount, uint32_t* xval, uint32_t* yval);
	CurveEditorDialog(wxWindow* parent);
	~CurveEditorDialog();

private:
	int win_size_x;
	int win_size_y;
	unsigned int last_vali;
	
	wxPoint ConvertCoordToPoint(uint32_t x, uint32_t y);
	wxPoint* ConvertArrayToPoints(int amount, uint32_t* arrayx, uint32_t* arrayy);
	void DrawImage(wxDC& dc, wxPoint displaycoord = wxDefaultPosition, wxString coordstr = wxEmptyString);
	void OnMouse(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnCheckBox(wxCommandEvent& event);
};

#endif
