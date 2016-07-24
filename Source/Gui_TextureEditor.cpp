#include "Gui_TextureEditor.h"

#include <sstream>
#include "Hades_Strings.h"
#include "main.h"

#define DEFAULT_HEIGHT	128.0
#define SELECTION_RED	0xFF
#define SELECTION_GREEN	0xCC
#define SELECTION_BLUE	0x00

void ImageDrawSelection(wxImage& img, int x, int y, int width, int height, unsigned char red = SELECTION_RED, unsigned char green = SELECTION_GREEN, unsigned char blue = SELECTION_BLUE);

ManageTextureDialog::ManageTextureDialog(wxWindow* parent, TIMImageDataStruct textim,/*
					*/ unsigned int texamount, int canexpand, int canshrink, int texfixedwidth, int texfixedheight, bool texcanedit,/*
					*/ uint16_t* texposx, uint16_t* texposy, uint16_t texmovex, uint16_t texmovey,/*
					*/ uint16_t* texwidth, uint16_t* texheight, uint16_t texscalex, uint16_t texscaley,/*
					*/ unsigned int palamount, uint32_t** palette,/*
					*/ bool importpal, unsigned int importpalpixel,/*
					*/ bool charflag, uint8_t* chartexpos) :
	ManageTextureWindow(parent),
	tim(textim),
	tex_amount(texamount),
	can_expand(canexpand),
	can_shrink(canshrink),
	tex_fixed_width(texfixedwidth),
	tex_fixed_height(texfixedheight),
	tex_can_edit(texcanedit),
	tex_pos_x(texposx),
	tex_pos_y(texposy),
	tex_move_x(texmovex),
	tex_move_y(texmovey),
	tex_width(texwidth),
	tex_height(texheight),
	tex_scale_x(texscalex),
	tex_scale_y(texscaley),
	pal_amount(palamount),
	pal(palette),
	import_pal(importpal),
	import_pal_pixel(importpalpixel),
	char_flag(charflag),
	char_tex_pos(chartexpos),
	color_selected(-1),
	prevent_event(false) {
	unsigned int i;
	uint8_t* imgcpy = new uint8_t[tim.img_size-0xC];
	memcpy(imgcpy,tim.pixel_value,tim.img_size-0xC);
	tim.pixel_value = imgcpy;
	for (i=0;i<tex_amount;i++) {
		wstringstream buffer;
		buffer << "Painting " << i+1 << endl;
		m_texturelist->Append(_(buffer.str()));
	}
	for (i=0;i<pal_amount;i++) {
		wstringstream buffer;
		buffer << "Palette " << i+1 << endl;
		m_palettelist->Append(_(buffer.str()));
	}
	tex_popup_menu = new wxMenu();
	tex_popup_menu->Append(wxID_ADD,HADES_STRING_GENERIC_ADD);
	tex_popup_menu->Enable(wxID_ADD,tex_can_edit);
	tex_popup_menu->Append(wxID_REMOVE,HADES_STRING_GENERIC_REMOVE);
	tex_popup_menu->Enable(wxID_REMOVE,tex_can_edit);
	pal_popup_menu = new wxMenu();
	pal_popup_menu->Append(wxID_PALETTE,HADES_STRING_GENERIC_REMOVE);
	pal_popup_menu->Enable(wxID_PALETTE,import_pal);
	m_palettelist->SetSelection(0);
	m_textureposx->Enable(false);
	m_textureposy->Enable(false);
	m_texturewidth->Enable(false);
	m_textureheight->Enable(false);
	m_texturepaletteselection->Enable(false);
	imported_img = wxNullBitmap;
	main_img_base = ConvertFullTIMToImage(tim,pal[0]);
	scale_ratio = main_img_base.GetHeight()/DEFAULT_HEIGHT;
	UpdateImage();
	DrawPaletteColors();
	m_texturepicker->GetTextCtrl()->Connect(wxEVT_SET_FOCUS,wxFocusEventHandler(ManageTextureDialog::OnFocusFileImage),NULL,this);
	tex_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ManageTextureDialog::OnSelectRightClickMenu),NULL,this);
	pal_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ManageTextureDialog::OnSelectRightClickMenu),NULL,this);
}

ManageTextureDialog::~ManageTextureDialog() {
	m_texturepicker->GetTextCtrl()->Disconnect(wxEVT_SET_FOCUS,wxFocusEventHandler(ManageTextureDialog::OnFocusFileImage),NULL,this);
	tex_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ManageTextureDialog::OnSelectRightClickMenu),NULL,this);
	pal_popup_menu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ManageTextureDialog::OnSelectRightClickMenu),NULL,this);
	if (tex_pos_x)
		delete[] tex_pos_x;
	if (tex_pos_y)
		delete[] tex_pos_y;
	if (tex_width)
		delete[] tex_width;
	if (tex_height)
		delete[] tex_height;
	if (char_tex_pos)
		delete[] char_tex_pos;
	for (unsigned int i=0;i<pal_amount;i++)
		delete[] pal[i];
	delete[] pal;
}

int ManageTextureDialog::ShowModal() {
	return wxDialog::ShowModal();
}

void ManageTextureDialog::DrawImage(wxDC& dc) {
	if (main_img.IsOk()) {
		int mainposx = 0;
		int mainposy = 0;
		int extrax = 0;
		int extray = 0;
		if (imported_img.IsOk()) {
			mainposx = max(0,-imported_img_x)/scale_ratio;
			mainposy = max(0,-imported_img_y)/scale_ratio;
			extrax = max(0,int(imported_img_x/scale_ratio+imported_img.GetWidth()-main_img.GetWidth()));
			extray = max(0,int(imported_img_y/scale_ratio+imported_img.GetHeight()-main_img.GetHeight()));
		}
		m_texturewindow->SetVirtualSize(main_img.GetWidth()+mainposx+extrax,main_img.GetHeight()+mainposy+extray);
		MACRO_CREATE_MEMORY_DC(dc,m_texturewindow)
		mdc.DrawBitmap(main_img,mainposx,mainposy);
		if (imported_img.IsOk())
			mdc.DrawBitmap(imported_img,mainposx+imported_img_x/scale_ratio,mainposy+imported_img_y/scale_ratio);
		if (foreground_img.IsOk())
			mdc.DrawBitmap(foreground_img,mainposx,mainposy);
		m_texturewindow->DoPrepareDC(dc);
		dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
	}
}

void ManageTextureDialog::UpdateImage() {
	int texid = m_texturelist->GetSelection();
	wxImage img = main_img_base.Copy();
	wxImage fgimg(main_img_base.GetWidth(),main_img_base.GetHeight(),false);
	fgimg.SetAlpha();
	unsigned int i,s = main_img_base.GetWidth()*main_img_base.GetHeight();
	unsigned char* alpha = fgimg.GetAlpha();
	for (i=0;i<s;i++)
		alpha[i] = 0;
	if (texid!=wxNOT_FOUND && (!char_flag || m_palettelist->GetSelection()%2==char_tex_pos[texid])) {
		int width = tex_fixed_width>=0 ? tex_fixed_width : tex_width[texid];
		int height = tex_fixed_height>=0 ? tex_fixed_height : tex_height[texid];
		ImageDrawSelection(fgimg,tex_pos_x[texid]*tex_move_x,tex_pos_y[texid]*tex_move_y,width*tex_scale_x,height*tex_scale_y);
	}
	img.Rescale(img.GetWidth()/scale_ratio,img.GetHeight()/scale_ratio);
	fgimg.Rescale(img.GetWidth(),img.GetHeight());
	main_img = wxBitmap(img);
	foreground_img = wxBitmap(fgimg);
	wxClientDC dc(m_texturewindow);
	DrawImage(dc);
}

void ManageTextureDialog::UpdateTexturePreview(int changeid) {
	if (changeid==wxID_TEXTURE) {
		prevent_event = true;
		if (color_selected!=-1) {
			color_selected = -1;
			DrawPaletteColors(m_palettelist->GetSelection());
		}
		int texid = m_texturelist->GetSelection();
		m_textureposx->Enable(tex_can_edit);
		m_textureposy->Enable(tex_can_edit);
		m_textureposx->SetRange(0,tim.GetWidth()/tex_scale_x-1);
		m_textureposy->SetRange(0,tim.height/tex_scale_y-1);
		m_textureposx->SetValue(tex_pos_x[texid]);
		m_textureposy->SetValue(tex_pos_y[texid]);
		if (tex_fixed_width>=0) {
			m_texturewidth->Enable(false);
			m_texturewidth->SetRange(0,max(tex_fixed_width,m_texturewidth->GetValue()));
			m_texturewidth->SetValue(tex_fixed_width);
		} else {
			m_texturewidth->Enable();
			m_texturewidth->SetRange(0,-tex_fixed_width);
			m_texturewidth->SetValue(tex_width[texid]);
		}
		if (tex_fixed_height>=0) {
			m_textureheight->Enable(false);
			m_textureheight->SetRange(0,max(tex_fixed_height,m_textureheight->GetValue()));
			m_textureheight->SetValue(tex_fixed_height);
		} else {
			m_textureheight->Enable();
			m_textureheight->SetRange(1,-tex_fixed_height);
			m_textureheight->SetValue(tex_height[texid]);
		}
		if (char_flag) {
			m_texturepaletteselection->Enable();
			m_texturepaletteselection->SetValue(char_tex_pos[texid]);
		}
		prevent_event = false;
	} else if (changeid==wxID_PALETTE) {
		int palid = m_palettelist->GetSelection();
		DrawPaletteColors(palid);
		main_img_base = ConvertFullTIMToImage(tim,pal[palid]);
	}
	UpdateImage();
}

void ManageTextureDialog::OnListBoxSelect(wxCommandEvent& event) {
	UpdateTexturePreview(event.GetId());
}

void ManageTextureDialog::OnListBoxRightClick(wxMouseEvent& event) {
	int id = event.GetId();
	if (id==wxID_TEXTURE) {
		int newsel = m_texturelist->HitTest(event.GetPosition());
		if (newsel!=wxNOT_FOUND) {
			m_texturelist->SetSelection(newsel);
			UpdateTexturePreview(wxID_TEXTURE);
			m_texturelist->PopupMenu(tex_popup_menu);
		}
	} else if (id==wxID_PALETTE) {
		int newsel = m_palettelist->HitTest(event.GetPosition());
		if (newsel!=wxNOT_FOUND) {
			m_palettelist->SetSelection(newsel);
			UpdateTexturePreview(wxID_PALETTE);
			m_palettelist->PopupMenu(pal_popup_menu);
		}
	}
	event.Skip();
}

void ManageTextureDialog::OnSelectRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_ADD) {
		unsigned int texsel = m_texturelist->GetSelection();
		uint16_t* newtexposx = new uint16_t[tex_amount+1];
		uint16_t* newtexposy = new uint16_t[tex_amount+1];
		memcpy(newtexposx,tex_pos_x,tex_amount*sizeof(uint16_t));
		memcpy(newtexposy,tex_pos_y,tex_amount*sizeof(uint16_t));
		newtexposx[tex_amount] = newtexposx[texsel];
		newtexposy[tex_amount] = newtexposy[texsel];
		delete[] tex_pos_x;
		delete[] tex_pos_y;
		tex_pos_x = newtexposx;
		tex_pos_y = newtexposy;
		if (tex_fixed_width<0) {
			uint16_t* newtexwidth = new uint16_t[tex_amount+1];
			memcpy(newtexwidth,tex_width,tex_amount*sizeof(uint16_t));
			newtexwidth[tex_amount] = newtexwidth[texsel];
			delete[] tex_width;
			tex_width = newtexwidth;
		}
		if (tex_fixed_height<0) {
			uint16_t* newtexheight = new uint16_t[tex_amount+1];
			memcpy(newtexheight,tex_height,tex_amount*sizeof(uint16_t));
			newtexheight[tex_amount] = newtexheight[texsel];
			delete[] tex_height;
			tex_height = newtexheight;
		}
		if (char_flag) {
			uint8_t* newtexposp = new uint8_t[tex_amount+1];
			memcpy(newtexposp,char_tex_pos,tex_amount*sizeof(uint8_t));
			newtexposp[tex_amount] = m_palettelist->GetSelection()%2;
			delete[] char_tex_pos;
			char_tex_pos = newtexposp;
		}
		tex_amount++;
		wstringstream buffer;
		buffer << "Painting " << tex_amount << endl;
		m_texturelist->Append(_(buffer.str()));
		m_texturelist->SetSelection(m_texturelist->GetCount()-1);
		UpdateTexturePreview(wxID_TEXTURE);
	} else if (id==wxID_REMOVE) {
		if (tex_amount>1) {
			unsigned int texsel = m_texturelist->GetSelection();
			memmove(tex_pos_x+texsel,tex_pos_x+texsel+1,(tex_amount-texsel-1)*sizeof(uint16_t));
			memmove(tex_pos_y+texsel,tex_pos_y+texsel+1,(tex_amount-texsel-1)*sizeof(uint16_t));
			if (char_flag)
				memmove(char_tex_pos+texsel,char_tex_pos+texsel+1,(tex_amount-texsel-1)*sizeof(uint8_t));
			if (tex_fixed_width<0)
				memmove(tex_width+texsel,tex_width+texsel+1,(tex_amount-texsel-1)*sizeof(uint16_t));
			if (tex_fixed_height<0)
				memmove(tex_height+texsel,tex_height+texsel+1,(tex_amount-texsel-1)*sizeof(uint16_t));
			tex_amount--;
			m_texturelist->Delete(tex_amount);
			m_texturelist->SetSelection(min(texsel,tex_amount-1));
			UpdateTexturePreview(wxID_TEXTURE);
		}
	} else if (id==wxID_PALETTE) {
		if (pal_amount>1) {
			unsigned int palsel = m_palettelist->GetSelection();
			delete[] pal[palsel];
			memmove(pal+palsel,pal+palsel+1,(pal_amount-palsel-1)*sizeof(uint32_t*));
			pal_amount--;
			m_palettelist->Delete(pal_amount);
			m_palettelist->SetSelection(min(palsel,pal_amount-1));
			UpdateTexturePreview(wxID_PALETTE);
		}
	}
}

void ManageTextureDialog::UpdateImportSpins() {
	m_texturelist->SetSelection(wxNOT_FOUND);
	m_textureposx->Enable();
	m_textureposy->Enable();
	if (can_expand & 1)
		m_textureposx->SetRange(0,tim.GetWidth()-imported_img_width);
	else
		m_textureposx->SetRange(0,tim.GetWidth()-imported_img_width);
	if (can_expand & 2)
		m_textureposy->SetRange(0,tim.height-imported_img_height);
	else
		m_textureposy->SetRange(0,tim.height-imported_img_height);
	m_textureposx->SetValue(imported_img_x);
	m_textureposy->SetValue(imported_img_y);
	imported_img_x = m_textureposx->GetValue();
	imported_img_y = m_textureposy->GetValue();
	m_texturewidth->Enable();
	m_textureheight->Enable();
	m_texturewidth->SetRange(1,tim.GetWidth());
	m_textureheight->SetRange(1,tim.height);
	m_texturewidth->SetValue(imported_img_width);
	m_textureheight->SetValue(imported_img_height);
}

void ManageTextureDialog::DrawPaletteColors(int palid) {
	int pixelsize = (tim.format & 0x8) ? 10 : 5;
	unsigned int coloramount = char_flag ? 4 : (tim.format & 0x8) ? 16 : 256;
	int i,x,y;
	unsigned char r,g,b,a;
	wxClientDC dc(m_drawpanelcolor);
	wxPen pen = dc.GetPen();
	wxBrush brush = dc.GetBrush();
	dc.Clear();
	if (char_flag)
		palid -= palid%2;
	for (i=0;i<coloramount;i++) {
		x = (i%32)*pixelsize;
		y = (i/32)*pixelsize;
		b = pal[palid][i];
		g = (pal[palid][i] >> 8);
		r = (pal[palid][i] >> 16);
		a = (pal[palid][i] >> 24);
		if (color_selected==i) {
			pen.SetColour(SELECTION_RED,SELECTION_GREEN,SELECTION_BLUE);
			pen.SetStyle(wxPENSTYLE_SOLID);
		} else {
			pen.SetColour(r,g,b);
			pen.SetStyle(a ? wxPENSTYLE_SOLID : wxPENSTYLE_TRANSPARENT);
		}
		brush.SetColour(r,g,b);
		brush.SetStyle(a ? wxBRUSHSTYLE_SOLID : wxBRUSHSTYLE_TRANSPARENT);
		dc.SetPen(pen);
		dc.SetBrush(brush);
		dc.DrawRectangle(x,y,pixelsize,pixelsize);
	}
}

void ManageTextureDialog::OnModifyRadio(wxCommandEvent& event) {
	if (event.GetInt()==0) {
		m_modifypanelimport->Show(false);
		m_modifypaneldraw->Show(true);
		DrawPaletteColors(m_palettelist->GetSelection());
		m_modifypaneldraw->Layout();
		m_modifypaneldraw->GetParent()->GetSizer()->Layout();
		m_modifypaneldraw->Refresh();
	} else if (event.GetInt()==1) {
		color_selected = -1;
		m_modifypaneldraw->Show(false);
		m_modifypanelimport->Show(true);
		m_modifypanelimport->Layout();
		m_modifypanelimport->GetParent()->GetSizer()->Layout();
		m_modifypanelimport->Refresh();
	}
}

void ManageTextureDialog::OnSpinPosition(wxSpinEvent& event) {
	if (prevent_event)
		return;
	prevent_event = true;
	int id = event.GetId();
	int texid = m_texturelist->GetSelection();
	if (texid!=wxNOT_FOUND) {
		if (id==wxID_POSX)
			tex_pos_x[texid] = event.GetPosition();
		else if (id==wxID_POSY)
			tex_pos_y[texid] = event.GetPosition();
		else if (id==wxID_SIZEX)
			tex_width[texid] = event.GetPosition();
		else if (id==wxID_SIZEY)
			tex_height[texid] = event.GetPosition();
		else if (id==wxID_PALSEL)
			char_tex_pos[texid] = event.GetPosition();
		UpdateImage();
	} else if (imported_img.IsOk()) {
		if (id==wxID_POSX)
			imported_img_x = event.GetPosition();
		else if (id==wxID_POSY)
			imported_img_y = event.GetPosition();
		else if (id==wxID_SIZEX) {
			wxImage img = imported_img_base.Copy();
			imported_img_width = event.GetPosition();
			img.Rescale(imported_img_width/scale_ratio,imported_img_height/scale_ratio);
			ImageDrawSelection(img,0,0,img.GetWidth(),img.GetHeight(),0xFF,0,0xCC);
			imported_img = wxBitmap(img);
			UpdateImportSpins();
		} else if (id==wxID_SIZEY) {
			wxImage img = imported_img_base.Copy();
			imported_img_height = event.GetPosition();
			img.Rescale(imported_img_width/scale_ratio,imported_img_height/scale_ratio);
			ImageDrawSelection(img,0,0,img.GetWidth(),img.GetHeight(),0xFF,0,0xCC);
			imported_img = wxBitmap(img);
			UpdateImportSpins();
		}
		wxClientDC dc(m_texturewindow);
		DrawImage(dc);
	}
	prevent_event = false;
}

void ManageTextureDialog::OnChooseFileImage(wxFileDirPickerEvent& event) {
	if (!wxFileName::IsFileReadable(event.GetPath())) {
		imported_img_base.Destroy();
		imported_img = wxNullBitmap;
		m_textureposx->Enable(false);
		m_textureposy->Enable(false);
		m_texturewidth->Enable(false);
		m_textureheight->Enable(false);
		UpdateImage();
		return;
	}
	wxImage img(event.GetPath(),wxBITMAP_TYPE_TGA);
	if (img.IsOk() && main_img.IsOk()) {
		prevent_event = true;
		if (!img.HasAlpha())
			img.SetAlpha();
		imported_img_base = img.Copy();
		double scalex = double(min(img.GetWidth(),main_img.GetWidth()))/img.GetWidth();
		double scaley = double(min(img.GetHeight(),main_img.GetHeight()))/img.GetHeight();
		double scale = min(scalex,scaley);
		img.Rescale(scale*img.GetWidth(),scale*img.GetHeight());
		imported_img_x = 0;
		imported_img_y = 0;
		imported_img_width = img.GetWidth()*scale_ratio;
		imported_img_height = img.GetHeight()*scale_ratio;
		ImageDrawSelection(img,0,0,img.GetWidth(),img.GetHeight(),0xFF,0,0xCC);
		imported_img = wxBitmap(img);
		UpdateImportSpins();
		UpdateImage();
		prevent_event = false;
	}
}

void ManageTextureDialog::OnFocusFileImage(wxFocusEvent& event) {
	if (imported_img.IsOk()) {
		prevent_event = true;
		UpdateImportSpins();
		UpdateImage();
		prevent_event = false;
	}
	event.Skip();
}

void ManageTextureDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_IMPORT) {
		if (imported_img_base.IsOk()) {
			imported_img_base.Rescale(imported_img_width,imported_img_height);
			uint32_t* paldata;
			uint8_t* texdata = imported_img_base.GetData();
			uint8_t* alphadata = imported_img_base.GetAlpha();
			if (import_pal)
				paldata = TIMImageDataStruct::CreatePaletteFromData(texdata,alphadata,imported_img_width,imported_img_height);
			else
				paldata = pal[m_palettelist->GetSelection()];
			tim.Import(texdata,alphadata,imported_img_x,imported_img_y,imported_img_width,imported_img_height,paldata,char_flag ? m_palettelist->GetSelection()%2 : -1);
			if (import_pal) {
				uint32_t** palarray = new uint32_t*[pal_amount+1];
				memcpy(palarray,pal,pal_amount*sizeof(uint32_t*));
				palarray[pal_amount] = paldata;
				delete[] pal;
				pal = palarray;
				wstringstream buffer;
				buffer << "Palette " << ++pal_amount << endl;
				m_palettelist->Append(_(buffer.str()));
				m_palettelist->SetSelection(m_palettelist->GetCount()-1);
			}
			m_texturepicker->GetTextCtrl()->Clear();
			imported_img_base.Destroy();
			imported_img = wxNullBitmap;
			m_textureposx->Enable(false);
			m_textureposy->Enable(false);
			m_texturewidth->Enable(false);
			m_textureheight->Enable(false);
			main_img_base = ConvertFullTIMToImage(tim,pal[m_palettelist->GetSelection()]);
			UpdateImage();
		}
	} else {
		EndModal(id);
	}
}

void ManageTextureDialog::OnTextureMouseMove(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
		if (prevent_event)
			return;
		prevent_event = true;
		m_texturewindow->SetFocusIgnoringChildren();
		if (m_texturelist->GetSelection()!=wxNOT_FOUND && tex_can_edit) {
			int texid = m_texturelist->GetSelection();
			wxClientDC dc(m_texturewindow);
			m_texturewindow->DoPrepareDC(dc);
			wxPoint mpos = event.GetLogicalPosition(dc);
			uint16_t width = tex_fixed_width>=0 ? tex_fixed_width : tex_width[texid];
			uint16_t height = tex_fixed_height>=0 ? tex_fixed_height : tex_height[texid];
			tex_pos_x[texid] = min(max(int(mpos.x*scale_ratio/tex_scale_x-width/2),m_textureposx->GetMin()),m_textureposx->GetMax());
			tex_pos_y[texid] = min(max(int(mpos.y*scale_ratio/tex_scale_y-height/2),m_textureposy->GetMin()),m_textureposy->GetMax());
			m_textureposx->SetValue(tex_pos_x[texid]);
			m_textureposy->SetValue(tex_pos_y[texid]);
			UpdateImage();
		} else if (color_selected!=-1) {
			wxClientDC dc(m_texturewindow);
			m_texturewindow->DoPrepareDC(dc);
			wxPoint mpos = event.GetLogicalPosition(dc);
			int timx = min(max(int(mpos.x*scale_ratio),0),tim.GetWidth()-1);
			int timy = min(max(int(mpos.y*scale_ratio),0),tim.height-1);
			tim.SetPixelValue(timx,timy,color_selected,char_flag ? m_palettelist->GetSelection()%2 : -1);
			UpdateTexturePreview(wxID_PALETTE);
		} else if (imported_img.IsOk()) {
			wxClientDC dc(m_texturewindow);
			m_texturewindow->DoPrepareDC(dc);
			wxPoint mpos = event.GetLogicalPosition(dc);
			imported_img_x = min(max(int(mpos.x*scale_ratio-imported_img_width/2),m_textureposx->GetMin()),m_textureposx->GetMax());
			imported_img_y = min(max(int(mpos.y*scale_ratio-imported_img_height/2),m_textureposy->GetMin()),m_textureposy->GetMax());
			m_textureposx->SetValue(imported_img_x);
			m_textureposy->SetValue(imported_img_y);
			DrawImage(dc);
		}
		prevent_event = false;
	}
}

void ManageTextureDialog::OnTextureMouseWheel(wxMouseEvent& event) {
	if (event.GetWheelRotation()>0)
		scale_ratio *= 0.95;
	else
		scale_ratio = min(scale_ratio*1.05,main_img_base.GetHeight()/DEFAULT_HEIGHT);
	UpdateImage();
}

void ManageTextureDialog::OnPaletteMouseMove(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
		if (prevent_event)
			return;
		m_drawpanelcolor->SetFocusIgnoringChildren();
		prevent_event = true;
		wxClientDC dc(m_drawpanelcolor);
		wxPoint mpos = event.GetLogicalPosition(dc);
		int pixelsize = (tim.format & 0x8) ? 10 : 5;
		unsigned int coloramount = char_flag ? 4 : (tim.format & 0x8) ? 16 : 256;
		int newsel = (mpos.x/pixelsize)%32+(mpos.y/pixelsize)*32;
		if (mpos.x/pixelsize<32 && newsel<coloramount && color_selected!=newsel) {
			color_selected = newsel;
			DrawPaletteColors(m_palettelist->GetSelection());
			m_texturelist->SetSelection(wxNOT_FOUND);
			UpdateImage();
		}
		prevent_event = false;
	}
}

void ManageTextureDialog::OnPaintTexture(wxPaintEvent& event) {
	wxPaintDC dc(m_texturewindow);
	DrawImage(dc);
}

void ManageTextureDialog::OnPaintPalette(wxPaintEvent& event) {
	DrawPaletteColors(m_palettelist->GetSelection());
}

wxBitmap ConvertTIMToBitmap(TIMImageDataStruct& tdtex, TIMImageDataStruct& tdpal, uint16_t sizex, uint16_t sizey, uint16_t palpos, uint16_t texpos, bool usealpha) {
	uint32_t* imgpal = tdpal.ConvertAsPalette(palpos,usealpha);
	uint32_t* imgdata = tdtex.ConvertAsImage(texpos,sizex,sizey,imgpal);
	return wxBitmap(ConvertDataToImage(imgdata,sizex,sizey,usealpha));
}

wxImage ConvertFullTIMToImage(TIMImageDataStruct& tim, uint32_t* pal, uint16_t palpos, bool usealpha) {
	unsigned int i,j;
	uint8_t* imgdata = (uint8_t*)tim.ConvertAsFullImage(pal,palpos,usealpha);
	wxImage img(tim.GetWidth(),tim.height,false);
	img.SetAlpha();
	unsigned char* rawdata = img.GetData();
	unsigned char* rawalpha = img.GetAlpha();
	for (i=0;i<tim.GetWidth();i++)
		for (j=0;j<tim.height;j++) {
			rawdata[(i+j*tim.GetWidth())*3] = imgdata[(i+j*tim.GetWidth())*4+2];
			rawdata[(i+j*tim.GetWidth())*3+1] = imgdata[(i+j*tim.GetWidth())*4+1];
			rawdata[(i+j*tim.GetWidth())*3+2] = imgdata[(i+j*tim.GetWidth())*4];
			rawalpha[i+j*tim.GetWidth()] = imgdata[(i+j*tim.GetWidth())*4+3];
		}
	delete[] imgdata;
	return img;
}

wxImage ConvertDataToImage(uint32_t* data, uint16_t sizex, uint16_t sizey, bool usealpha) {
	wxImage res(sizex,sizey,false);
	unsigned int i,j;
	res.SetAlpha();
	unsigned char* rawdata = res.GetData();
	unsigned char* rawalpha = res.GetAlpha();
	for (i=0;i<sizex;i++)
		for (j=0;j<sizey;j++) {
			rawdata[(i+j*sizex)*3+2] = data[i+j*sizex] & 0xFF;
			rawdata[(i+j*sizex)*3+1] = (data[i+j*sizex] >> 8) & 0xFF;
			rawdata[(i+j*sizex)*3] = (data[i+j*sizex] >> 16) & 0xFF;
			rawalpha[i+j*sizex] = usealpha ? (data[i+j*sizex] >> 24) & 0xFF : 0xFF;
		}
	return res;
}

void ImageDrawSelection(wxImage& img, int x, int y, int width, int height, unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char* rawdata = img.GetData();
	unsigned char* rawalpha = img.GetAlpha();
	int thick = min(width,height)/100+1;
	int i,j,posx,posy;
	#define MACRO_SETPIXELCOLOR() \
		rawdata[3*(posx+posy)] = red; \
		rawdata[3*(posx+posy)+1] = green; \
		rawdata[3*(posx+posy)+2] = blue; \
		rawalpha[posx+posy] = 0xFF;
	width--;
	height--;
	for (i=0;i<=width;i++) {
		posx = (x+i)%img.GetWidth();
		for (j=0;j<thick;j++) {
			posy = ((y+j)%img.GetHeight())*img.GetWidth();
			MACRO_SETPIXELCOLOR()
			posy = ((y+height-j)%img.GetHeight())*img.GetWidth();
			MACRO_SETPIXELCOLOR()
		}
	}
	for (i=0;i<=height;i++) {
		posy = (y+i)%img.GetHeight()*img.GetWidth();
		for (j=0;j<thick;j++) {
			posx = (x+j)%img.GetWidth();
			MACRO_SETPIXELCOLOR()
			posx = (x+width-j)%img.GetWidth();
			MACRO_SETPIXELCOLOR()
		}
	}
}
