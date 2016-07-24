#ifndef _GUI_FIELDTEXTUREEDITOR_H
#define _GUI_FIELDTEXTUREEDITOR_H

class ManageFieldTextureDialog;

#include <inttypes.h>
#include "Fields.h"
#include "gui.h"

class ManageFieldTextureDialog : public ManageFieldTextureWindow {
public:
	FieldTilesDataStruct field;
	TIMImageDataStruct* tim;
	unsigned int camera_id;
	wxImageList* animlist_img;
	unsigned int* anim_tick_time;
	unsigned int* anim_tile_pos;
	unsigned int* anim_play_flag;
	wxBitmap animcontrol_loopplay;
	wxBitmap animcontrol_looppause;
	wxBitmap animcontrol_noloopplay;
	wxBitmap animcontrol_nolooppause;
	
	ManageFieldTextureDialog(wxWindow* parent, FieldTilesDataStruct& f);
	~ManageFieldTextureDialog();
	int ShowModal(unsigned int camera = 0);
	
private:
	wxTimer* timer;
	wxBitmap main_img;
	wxBitmap imported_img;
	wxBitmap foreground_img;
	wxImage main_img_base;
	wxImage imported_img_base;
	float scale_ratio;
	int imported_img_x;
	int imported_img_y;
	int imported_img_width;
	int imported_img_height;
	int color_selected;
	bool prevent_event;
	
	void DrawImage(wxDC& dc);
	void UpdateImage();
	void UpdateTexturePreview(int changeid);
	void UpdateImportSpins();
	void DrawPaletteColors(int palid = 0);
	
	void OnModifyRadio(wxCommandEvent& event);
	void OnTileButton(wxCommandEvent& event);
	void OnTileSelection(wxCommandEvent& event);
	void OnAnimClick(wxMouseEvent& event);
	void OnSpinPosition(wxSpinEvent& event);
	void OnChooseFileImage(wxFileDirPickerEvent& event);
	void OnFocusFileImage(wxFocusEvent& event);
	void OnButtonClick(wxCommandEvent& event);
	void OnTextureMouseMove(wxMouseEvent& event);
	void OnTextureMouseWheel(wxMouseEvent& event);
	void OnPaletteMouseMove(wxMouseEvent& event);
	void OnPaintTexture(wxPaintEvent& event);
	void OnPaintPalette(wxPaintEvent& event);
	void OnTimer(wxTimerEvent& event);
};

#endif
