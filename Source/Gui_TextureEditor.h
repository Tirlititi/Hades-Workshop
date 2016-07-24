#ifndef _GUI_TEXTUREEDITOR_H
#define _GUI_TEXTUREEDITOR_H

class ManageTextureDialog;

#include <inttypes.h>
#include "TIMImages.h"
#include "gui.h"

#define MACRO_CREATE_MEMORY_DC(DC,SCROLLWIN) \
	int _macrowidth, _macroheight, _macrowidth2, _macroheight2, _macroi; \
	DC.GetSize(&_macrowidth,&_macroheight); \
	SCROLLWIN->GetVirtualSize(&_macrowidth2,&_macroheight2); \
	if (_macrowidth<_macrowidth2) _macrowidth = _macrowidth2; \
	if (_macroheight<_macroheight2) _macroheight = _macroheight2; \
	unsigned char* _macrobmpdata = (unsigned char*)malloc(_macrowidth*_macroheight*3); \
	for (_macroi=0;_macroi<_macrowidth*_macroheight*3;_macroi++) \
		_macrobmpdata[_macroi] = 255; \
	wxImage _macrodcimg(_macrowidth,_macroheight,_macrobmpdata); \
	wxBitmap _macrodcbmp(_macrodcimg); \
	wxMemoryDC mdc(_macrodcbmp);

class ManageTextureDialog : public ManageTextureWindow {
public:
	TIMImageDataStruct tim;
	unsigned int tex_amount;
	int can_expand; // 0 = can't ; 1 = x only ; 2 = y only ; 3 = both
	int can_shrink; // same here
	int tex_fixed_width;
	int tex_fixed_height;
	bool tex_can_edit;
	uint16_t* tex_pos_x;
	uint16_t* tex_pos_y;
	uint16_t tex_move_x;
	uint16_t tex_move_y;
	uint16_t* tex_width;
	uint16_t* tex_height;
	uint16_t tex_scale_x;
	uint16_t tex_scale_y;
	unsigned int pal_amount;
	uint32_t** pal;
	bool import_pal;
	unsigned int import_pal_pixel;
	bool char_flag;
	uint8_t* char_tex_pos;
	
	ManageTextureDialog(wxWindow* parent, TIMImageDataStruct textim,/*
					*/ unsigned int texamount, int canexpand, int canshrink, int texfixedwidth, int texfixedheight, bool texcanedit,/*
					*/ uint16_t* texposx, uint16_t* texposy, uint16_t texmovex, uint16_t texmovey,/*
					*/ uint16_t* texwidth, uint16_t* texheight, uint16_t texscalex, uint16_t texscaley,/*
					*/ unsigned int palamount, uint32_t** palette,/*
					*/ bool importpal, unsigned int importpalpixel,/*
					*/ bool charflag = false, uint8_t* chartexpos = NULL);
	~ManageTextureDialog();
	int ShowModal();
	
private:
	wxMenu* tex_popup_menu;
	wxMenu* pal_popup_menu;
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
	void OnListBoxSelect(wxCommandEvent& event);
	void OnListBoxRightClick(wxMouseEvent& event);
	void OnSelectRightClickMenu(wxCommandEvent& event);
	void OnSpinPosition(wxSpinEvent& event);
	void OnChooseFileImage(wxFileDirPickerEvent& event);
	void OnFocusFileImage(wxFocusEvent& event);
	void OnButtonClick(wxCommandEvent& event);
	void OnTextureMouseMove(wxMouseEvent& event);
	void OnTextureMouseWheel(wxMouseEvent& event);
	void OnPaletteMouseMove(wxMouseEvent& event);
	void OnPaintTexture(wxPaintEvent& event);
	void OnPaintPalette(wxPaintEvent& event);
};

wxBitmap ConvertTIMToBitmap(TIMImageDataStruct& tdtex, TIMImageDataStruct& tdpal, uint16_t sizex, uint16_t sizey, uint16_t palpos, uint16_t texpos, bool usealpha=true);
wxImage ConvertFullTIMToImage(TIMImageDataStruct& tim, uint32_t* pal=NULL, uint16_t palpos=0, bool usealpha=true);
wxImage ConvertDataToImage(uint32_t* data, uint16_t sizex, uint16_t sizey, bool usealpha=true);

#endif
