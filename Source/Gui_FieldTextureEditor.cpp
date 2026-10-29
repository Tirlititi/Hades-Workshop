#include "Gui_FieldTextureEditor.h"

#include <wx/dir.h>
#include <sstream>
#include "Gui_TextureEditor.h"
#include "Hades_Strings.h"
#include "main.h"

#define DEFAULT_SPEED 		17
#define DEFAULT_HEIGHT		280.0
#define SELECTION_RED		0xFF
#define SELECTION_GREEN		0xCC
#define SELECTION_BLUE		0x00
#define ANIM_PLAYFLAG_PLAY	0x1
#define ANIM_PLAYFLAG_LOOP	0x2

wxString LastSearchedDirectory = wxEmptyString;
wxArrayString DirectorySearch;
wxArrayString GetFilesInDir(const wxString& dirname, const wxString& filespec) {
	if (dirname.IsSameAs(LastSearchedDirectory))
		return DirectorySearch;
	DirectorySearch.Clear();
	wxDir::GetAllFiles(dirname, &DirectorySearch, filespec, wxDIR_FILES);
	LastSearchedDirectory = dirname;
	return DirectorySearch;
}

void ImageDrawSelectionField(wxImage& img, int x, int y, int width, int height, unsigned char red = SELECTION_RED, unsigned char green = SELECTION_GREEN, unsigned char blue = SELECTION_BLUE);

ManageFieldTextureDialog::ManageFieldTextureDialog(wxWindow* parent, FieldTilesDataStruct& f) :
	ManageFieldTextureWindow(parent),
	timer(new wxTimer(this)),
	color_selected(-1),
	prevent_event(false) {
	m_tilex->Enable(false);
	m_tiley->Enable(false);
	m_tiledepth->Enable(false);
	m_tilepiecex->Enable(false);
	m_tilepiecey->Enable(false);
	m_tilepiecedepth->Enable(false);
	m_textureposx->Enable(false);
	m_textureposy->Enable(false);
	m_texturepaletteselection->Enable(false);
	wxArrayString choicelist;
	unsigned int i;
	tim = f.parent->tim_data[f.id]; // Unused now
	field.Copy(f);
	for (i = 0; i < field.tiles_amount; i++) {
		wstringstream buffer;
		buffer << "Tile Block " << i << endl;
		choicelist.Add(_(buffer.str()));
	}
	m_tilechecklist->Append(choicelist);
	for (i = 0; i < field.tiles_amount; i++)
		m_tilechecklist->Check(i, field.tiles[i].is_static || field.tiles[i].is_first_of_anim);
	animlist_img = new wxImageList(60, 20, true, 4);
	animcontrol_loopplay = wxBitmap(wxBITMAP(animcontrolloopplay_image));
	animcontrol_looppause = wxBitmap(wxBITMAP(animcontrollooppause_image));
	animcontrol_noloopplay = wxBitmap(wxBITMAP(animcontrolnoloopplay_image));
	animcontrol_nolooppause = wxBitmap(wxBITMAP(animcontrolnolooppause_image));
	animlist_img->Add(animcontrol_noloopplay);
	animlist_img->Add(animcontrol_nolooppause);
	animlist_img->Add(animcontrol_loopplay);
	animlist_img->Add(animcontrol_looppause);
	m_animlist->AssignImageList(animlist_img, wxIMAGE_LIST_SMALL);
	DisplayAnimationList();
	m_animlist->SetColumnWidth(0, 200);
	if (field.IsBGXFormat()) {
		m_panelvanilla->Show(false);
		m_panelbgx->Show(true);
		m_buttonexport->Show(false);
	} else {
		m_panelbgx->Show(false);
		m_panelvanilla->Show(true);
		m_buttonexport->Show(true);
	}
	if (!m_tilechecklist->IsEmpty()) {
		m_tilechecklist->SetSelection(0);
		SelectTileset(0);
	}
	menu = new wxMenu();
	menu->Append(new wxMenuItem(menu, wxID_ADD, HADES_STRING_GENERIC_ADD));
	menu->Append(new wxMenuItem(menu, wxID_REMOVE, HADES_STRING_GENERIC_REMOVE));
	menu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ManageFieldTextureDialog::OnRightClickMenu), NULL, this);
	imported_img = wxNullBitmap;
	main_img_base = wxNullImage;
	scale_ratio = 1.0;
	timer->Start(DEFAULT_SPEED);
	Connect(wxEVT_TIMER, wxTimerEventHandler(ManageFieldTextureDialog::OnTimer), NULL, this);
	m_texturepicker->GetTextCtrl()->Connect(wxEVT_SET_FOCUS, wxFocusEventHandler(ManageFieldTextureDialog::OnFocusFileImage), NULL, this);
}

ManageFieldTextureDialog::~ManageFieldTextureDialog() {
	m_texturepicker->GetTextCtrl()->Disconnect(wxEVT_SET_FOCUS, wxFocusEventHandler(ManageFieldTextureDialog::OnFocusFileImage), NULL, this);
	Disconnect(wxEVT_TIMER, wxTimerEventHandler(ManageFieldTextureDialog::OnTimer), NULL, this);
	delete timer;
}

int ManageFieldTextureDialog::ShowModal(unsigned int camera) {
	unsigned int i;
	bool* tileflag = new bool[field.tiles_amount];
	camera_id = camera;
	for (i = 0; i < field.tiles_amount; i++)
		tileflag[i] = m_tilechecklist->IsChecked(i);
	uint32_t* imgdata = field.ConvertAsImageAccurate(camera_id, tileflag, true);
	main_img_base = ConvertDataToImage(imgdata, field.camera[camera_id].width, field.camera[camera_id].height);
	delete[] imgdata;
	delete[] tileflag;
	scale_ratio = main_img_base.GetHeight() / DEFAULT_HEIGHT;
	UpdateImage();
	LastSearchedDirectory = wxEmptyString;
	return wxDialog::ShowModal();
}

void ManageFieldTextureDialog::DisplayAnimationList() {
	unsigned int i;
	anim_tick_time.resize(field.anim_amount);
	anim_tile_pos.resize(field.anim_amount);
	anim_play_flag.resize(field.anim_amount);
	for (i = 0; i < field.anim_amount; i++) {
		anim_tick_time[i] = field.anim[i].tile_duration[0];
		anim_tile_pos[i] = 0;
		anim_play_flag[i] = ANIM_PLAYFLAG_LOOP;
	}
	m_animlist->DeleteAllItems();
	for (i = 0; i < field.anim_amount; i++)
		m_animlist->InsertItem(i, _(L"Anim ") + wxString::Format(wxT("%u"), i), anim_play_flag[i]);
}

void ManageFieldTextureDialog::DrawImage(wxDC& dc) {
	if (main_img.IsOk()) {
		int mainposx = 0;
		int mainposy = 0;
		int extrax = 0;
		int extray = 0;
		if (imported_img.IsOk()) {
			mainposx = max(0, -imported_img_x) / scale_ratio;
			mainposy = max(0, -imported_img_y) / scale_ratio;
			extrax = max(0, int(imported_img_x / scale_ratio + imported_img.GetWidth() - main_img.GetWidth()));
			extray = max(0, int(imported_img_y / scale_ratio + imported_img.GetHeight() - main_img.GetHeight()));
		}
		m_texturewindow->SetVirtualSize(main_img.GetWidth() + mainposx + extrax, main_img.GetHeight() + mainposy + extray);
		MACRO_CREATE_MEMORY_DC(dc, m_texturewindow)
		mdc.DrawBitmap(main_img, mainposx, mainposy);
		if (imported_img.IsOk())
			mdc.DrawBitmap(imported_img, mainposx + imported_img_x / scale_ratio, mainposy + imported_img_y / scale_ratio);
		if (foreground_img.IsOk())
			mdc.DrawBitmap(foreground_img, mainposx, mainposy);
		m_texturewindow->DoPrepareDC(dc);
		dc.Blit(wxPoint(0, 0), mdc.GetSize(), &mdc, wxPoint(0, 0));
	}
}

void ManageFieldTextureDialog::UpdateImage() {
	int tileid = m_tilechecklist->GetSelection();
	wxImage img = main_img_base.Copy();
	/*
	wxImage fgimg(main_img_base.GetWidth(), main_img_base.GetHeight(), false);
	fgimg.SetAlpha();
	unsigned int i, s = main_img_base.GetWidth() * main_img_base.GetHeight();
	unsigned char* alpha = fgimg.GetAlpha();
	for (i = 0; i < s; i++)
		alpha[i] = 0;
	if (tileid != wxNOT_FOUND) {
		int width = tex_fixed_width >= 0 ? tex_fixed_width : tex_width[texid];
		int height = tex_fixed_height >= 0 ? tex_fixed_height : tex_height[texid];
		ImageDrawSelection(fgimg, tex_pos_x[texid] * tex_move_x, tex_pos_y[texid] * tex_move_y, width * tex_scale_x, height * tex_scale_y);
	}
	fgimg.Rescale(img.GetWidth(), img.GetHeight());
	foreground_img = wxBitmap(fgimg);
	*/
	img.Rescale(img.GetWidth() / scale_ratio, img.GetHeight() / scale_ratio);
	main_img = wxBitmap(img);
	wxClientDC dc(m_texturewindow);
	DrawImage(dc);
}

void ManageFieldTextureDialog::UpdateTexturePreview(int changeid) {
	bool* tileflag = new bool[field.tiles_amount];
	for (unsigned int i = 0; i < field.tiles_amount; i++)
		tileflag[i] = m_tilechecklist->IsChecked(i);
	uint32_t* imgdata = field.ConvertAsImageAccurate(camera_id, tileflag, true);
	main_img_base = ConvertDataToImage(imgdata, field.camera[camera_id].width, field.camera[camera_id].height);
	delete[] imgdata;
	delete[] tileflag;
	UpdateImage();
}

void ManageFieldTextureDialog::OnTileButton(wxCommandEvent& event) {
	UpdateTexturePreview(event.GetSelection());
}

void ManageFieldTextureDialog::OnTileSelection(wxCommandEvent& event) {
	SelectTileset(event.GetSelection());
}

void ManageFieldTextureDialog::OnTilePieceSelection(wxCommandEvent& event) {
	int blockid = m_tilechecklist->GetSelection();
	int tileid = event.GetSelection();
	m_tilepiecex->SetValue(field.tiles[blockid].tile[tileid].pos_x);
	m_tilepiecey->SetValue(field.tiles[blockid].tile[tileid].pos_y);
	m_tilepiecedepth->SetValue(field.tiles[blockid].tile[tileid].depth);
	m_tilepiecex->Enable(true);
	m_tilepiecey->Enable(true);
	m_tilepiecedepth->Enable(true);
}

void ManageFieldTextureDialog::OnAnimClick(wxMouseEvent& event) {
	wxClientDC dc(m_animlist);
	wxPoint mpos = event.GetLogicalPosition(dc);
	wxPoint mrpos = event.GetPosition();
	int button, col, hitflags;
	button = mpos.x / 20; // Not that great... Problems if HScroll is enabled
	col = m_animlist->HitTest(mrpos, hitflags);
	if (button >= 0 && button < 3 && col >= 0 && col < field.anim_amount) {
		switch (button) {
		case 0: // Loop
			if (anim_play_flag[col] & ANIM_PLAYFLAG_LOOP)
				anim_play_flag[col] &= ~ANIM_PLAYFLAG_LOOP;
			else
				anim_play_flag[col] |= ANIM_PLAYFLAG_LOOP;
			break;
		case 1: // Play/Pause
			if (anim_play_flag[col] & ANIM_PLAYFLAG_PLAY)
				anim_play_flag[col] &= ~ANIM_PLAYFLAG_PLAY;
			else
				anim_play_flag[col] |= ANIM_PLAYFLAG_PLAY;
			break;
		case 2: // Stop
			anim_play_flag[col] &= ~ANIM_PLAYFLAG_PLAY;
			anim_tile_pos[col] = 0;
			anim_tick_time[col] = field.anim[col].tile_duration[0];
			m_tilechecklist->Check(field.anim[col].tile_list[0], true);
			for (unsigned int i = 1; i < field.anim[col].tile_amount; i++)
				m_tilechecklist->Check(field.anim[col].tile_list[i], false);
			UpdateTexturePreview(-1);
			break;
		}
		m_animlist->SetItemImage(col, anim_play_flag[col]);
	}
}

void ManageFieldTextureDialog::OnTileRightClick(wxMouseEvent& event) {
	if (!field.IsBGXFormat())
		return;
	int newsel = m_tilechecklist->HitTest(event.GetPosition());
	if (newsel != wxNOT_FOUND) {
		menu_selection = newsel;
		menu_selection_type = 0;
		m_tilechecklist->SetSelection(newsel);
		SelectTileset(newsel);
		m_tilechecklist->PopupMenu(menu);
	}
}

void ManageFieldTextureDialog::OnAnimRightClick(wxMouseEvent& event) {
	if (!field.IsBGXFormat())
		return;
	int hitflags;
	int animsel = m_animlist->HitTest(event.GetPosition(), hitflags);
	if (animsel >= 0 && animsel < field.anim_amount)
		menu_selection = animsel;
	else
		menu_selection = wxNOT_FOUND;
	menu_selection_type = 1;
	m_animlist->PopupMenu(menu);
}

void ManageFieldTextureDialog::OnRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id == wxID_ADD) {
		if (menu_selection_type == 0) {
			ManageFieldTextureTilesetWindow dial(this);
			for (i = 0; i < field.camera_amount; i++)
				dial.m_camera->Append(wxString::Format(wxT("Background %u"), i + 1));
			dial.m_camera->SetSelection(camera_id);
			if (dial.ShowModal() == wxID_OK) {
				if (!field.AddTileset(dial.m_camera->GetSelection(), dial.m_filepicker->GetPath())) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL, dial.m_filepicker->GetPath());
					return;
				}
				m_tilechecklist->Append(wxString::Format(wxT("Tile Block %u"), field.tiles_amount - 1));
				m_tilechecklist->Check(field.tiles_amount - 1);
				UpdateTexturePreview(-1);
				m_tilechecklist->SetSelection(field.tiles_amount - 1);
				SelectTileset(field.tiles_amount - 1);
			}
		} else if (menu_selection_type == 1) {
			ManageFieldTextureAnimWindow dial(this);
			for (i = 0; i < field.tiles_amount; i++)
				dial.m_tilesetlist->Append(wxString::Format(wxT("Tile Block %u (camera %u)"), i, field.tiles[i].camera_id));
			if (dial.ShowModal() == wxID_OK) {
				vector<uint8_t> tilelist;
				uint8_t flags = 1;
				if (dial.m_flagloop->IsChecked())		flags |= 16;
				if (dial.m_flagpalindrome->IsChecked())	flags |= 32;
				for (i = 0; i < dial.m_tilesetlist->GetCount(); i++)
					if (dial.m_tilesetlist->IsChecked(i))
						tilelist.push_back(i);
				if (tilelist.size() < 2) {
					wxLogError(HADES_STRING_BACKGROUND_ANIM_TILE_COUNT);
					return;
				}
				uint8_t camid = field.tiles[tilelist[0]].camera_id;
				bool camok = true;
				for (i = 1; i < tilelist.size(); i++) {
					if (camid != field.tiles[tilelist[i]].camera_id) {
						camok = false;
						break;
					}
				}
				if (!camok) {
					wxLogError(HADES_STRING_BACKGROUND_ANIM_CAMERA);
					return;
				}
				if (!field.AddAnimation(tilelist, flags, dial.m_framerate->GetValue()))
					return;
				anim_tick_time.push_back(field.anim[field.anim_amount - 1].tile_duration[0]);
				anim_tile_pos.push_back(0);
				anim_play_flag.push_back(ANIM_PLAYFLAG_LOOP);
				m_animlist->InsertItem(m_animlist->GetItemCount(), wxString::Format(wxT("Anim %u"), field.anim_amount - 1), ANIM_PLAYFLAG_LOOP);
			}
		}
	} else if (id == wxID_REMOVE && menu_selection != wxNOT_FOUND) {
		if (menu_selection_type == 0) {
			field.RemoveTileset(menu_selection);
			m_tilechecklist->Delete(menu_selection);
			DisplayAnimationList();
			UpdateTexturePreview(-1);
			m_tilechecklist->SetSelection(min(menu_selection, (int)m_tilechecklist->GetCount() - 1));
			SelectTileset(m_tilechecklist->GetSelection());
		} else if (menu_selection_type == 1) {
			field.RemoveAnimation(menu_selection);
			DisplayAnimationList();
		}
	}
}

void ManageFieldTextureDialog::UpdateImportSpins() {
/*	m_texturelist->SetSelection(wxNOT_FOUND);
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
	m_textureheight->SetValue(imported_img_height);*/
}

void ManageFieldTextureDialog::DrawPaletteColors(int palid) {
/*	int pixelsize = (tim.format & 0x8) ? 10 : 5;
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
	}*/
}

void ManageFieldTextureDialog::SelectTileset(int sel) {
	wxArrayString choicelist;
	m_tilepiecelist->Clear();
	if (sel != wxNOT_FOUND) {
		m_tilex->SetValue(field.tiles[sel].pos_x);
		m_tiley->SetValue(field.tiles[sel].pos_y);
		m_tiledepth->SetValue(field.tiles[sel].depth);
		m_tilewidth->SetValue(field.tiles[sel].width);
		m_tileheight->SetValue(field.tiles[sel].height);
		m_tilescrollx->SetValue(field.tiles[sel].pos_dx);
		m_tilescrolly->SetValue(field.tiles[sel].pos_dy);
		if (field.IsBGXFormat()) {
			m_tilepath->SetPath(field.tiles[sel].memoria_bgx_path);
			m_tileshader->ChangeValue(field.tiles[sel].memoria_bgx_shader);
		} else {
			for (int i = 0; i < field.tiles[sel].tile_amount; i++)
				choicelist.Add(wxString::Format(wxT("Tile (%d, %d)"), field.tiles[sel].tile[i].pos_x, field.tiles[sel].tile[i].pos_y));
		}
	}
	m_tilepiecelist->Append(choicelist);
	m_tilex->Enable(sel != wxNOT_FOUND);
	m_tiley->Enable(sel != wxNOT_FOUND);
	m_tiledepth->Enable(sel != wxNOT_FOUND);
	m_tilepiecex->Enable(false);
	m_tilepiecey->Enable(false);
	m_tilepiecedepth->Enable(false);
}

void ManageFieldTextureDialog::OnModifyRadio(wxCommandEvent& event) {
	if (event.GetInt() == 0) {
		m_modifypanelimport->Show(false);
		m_modifypaneldraw->Show(true);
//		DrawPaletteColors(m_palettelist->GetSelection());
		m_modifypaneldraw->Layout();
		m_modifypaneldraw->GetParent()->GetSizer()->Layout();
		m_modifypaneldraw->Refresh();
	} else if (event.GetInt() == 1) {
		color_selected = -1;
		m_modifypaneldraw->Show(false);
		m_modifypanelimport->Show(true);
		m_modifypanelimport->Layout();
		m_modifypanelimport->GetParent()->GetSizer()->Layout();
		m_modifypanelimport->Refresh();
	}
}

void ManageFieldTextureDialog::OnSpinPosition(wxSpinEvent& event) {
	int id = event.GetId();
	int blockid = m_tilechecklist->GetSelection();
	int tileid = m_tilepiecelist->GetSelection();
	if (id == wxID_POSX1 && blockid != wxNOT_FOUND) {
		field.tiles[blockid].pos_x = event.GetPosition();
		field.modified = true;
		UpdateTexturePreview(blockid);
	} else if (id == wxID_POSY1 && blockid != wxNOT_FOUND) {
		field.tiles[blockid].pos_y = event.GetPosition();
		field.modified = true;
		UpdateTexturePreview(blockid);
	} else if (id == wxID_POSZ1 && blockid != wxNOT_FOUND) {
		field.tiles[blockid].depth = event.GetPosition();
		field.modified = true;
		field.SetupTilePerDepth();
		UpdateTexturePreview(blockid);
	} else if (id == wxID_WIDTH && blockid != wxNOT_FOUND) {
		field.tiles[blockid].width = event.GetPosition();
		field.modified = true;
		UpdateTexturePreview(blockid);
	} else if (id == wxID_HEIGHT && blockid != wxNOT_FOUND) {
		field.tiles[blockid].height = event.GetPosition();
		field.modified = true;
		UpdateTexturePreview(blockid);
	} else if (id == wxID_SCROLLX && blockid != wxNOT_FOUND) {
		field.tiles[blockid].pos_dx = event.GetPosition();
		field.tiles[blockid].is_scroll_with_offset = field.tiles[blockid].pos_dx != 0 || field.tiles[blockid].pos_dy != 0;
		field.modified = true;
	} else if (id == wxID_SCROLLY && blockid != wxNOT_FOUND) {
		field.tiles[blockid].pos_dy = event.GetPosition();
		field.tiles[blockid].is_scroll_with_offset = field.tiles[blockid].pos_dx != 0 || field.tiles[blockid].pos_dy != 0;
		field.modified = true;
	} else if (id == wxID_POSX2 && blockid != wxNOT_FOUND && tileid != wxNOT_FOUND) {
		field.tiles[blockid].tile[tileid].pos_x = event.GetPosition();
		field.modified = true;
		UpdateTexturePreview(blockid);
	} else if (id == wxID_POSY2 && blockid != wxNOT_FOUND && tileid != wxNOT_FOUND) {
		field.tiles[blockid].tile[tileid].pos_y = event.GetPosition();
		field.modified = true;
		UpdateTexturePreview(blockid);
	} else if (id == wxID_POSZ2 && blockid != wxNOT_FOUND && tileid != wxNOT_FOUND) {
		field.tiles[blockid].tile[tileid].depth = event.GetPosition();
		field.modified = true;
		field.SetupTilePerDepth();
		UpdateTexturePreview(blockid);
	}
/*	if (prevent_event)
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
	prevent_event = false;*/
}

void ManageFieldTextureDialog::OnTextChanged(wxCommandEvent& event) {
	int id = event.GetId();
	int blockid = m_tilechecklist->GetSelection();
	if (id == wxID_SHADER && blockid != wxNOT_FOUND) {
		field.tiles[blockid].memoria_bgx_shader = m_tileshader->GetValue();
		UpdateTexturePreview(blockid);
	}
}

void ManageFieldTextureDialog::OnPathSelection(wxFileDirPickerEvent& event) {
	int id = event.GetId();
	int blockid = m_tilechecklist->GetSelection();
	if (id == wxID_PATH && blockid != wxNOT_FOUND) {
		field.tiles[blockid].memoria_bgx_path = event.GetPath();
		UpdateTexturePreview(blockid);
	}
/*	if (!wxFileName::IsFileReadable(event.GetPath())) {
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
	}*/
}

void ManageFieldTextureDialog::OnFocusFileImage(wxFocusEvent& event) {
	if (imported_img.IsOk()) {
		prevent_event = true;
		UpdateImportSpins();
		UpdateImage();
		prevent_event = false;
	}
	event.Skip();
}

void ManageFieldTextureDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_IMPORT) {
/*		if (imported_img_base.IsOk()) {
			imported_img_base.Rescale(imported_img_width, imported_img_height);
			uint32_t* paldata;
			uint8_t* texdata = imported_img_base.GetData();
			uint8_t* alphadata = imported_img_base.GetAlpha();
			if (import_pal)
				paldata = TIMImageDataStruct::CreatePaletteFromData(texdata, alphadata, imported_img_width, imported_img_height);
			else
				paldata = pal[m_palettelist->GetSelection()];
			tim.Import(texdata, alphadata, imported_img_x, imported_img_y, imported_img_width, imported_img_height, paldata, char_flag ? m_palettelist->GetSelection() % 2 : -1);
			if (import_pal) {
				uint32_t** palarray = new uint32_t * [pal_amount + 1];
				memcpy(palarray, pal, pal_amount * sizeof(uint32_t*));
				palarray[pal_amount] = paldata;
				delete[] pal;
				pal = palarray;
				wstringstream buffer;
				buffer << "Palette " << ++pal_amount << endl;
				m_palettelist->Append(_(buffer.str()));
				m_palettelist->SetSelection(m_palettelist->GetCount() - 1);
			}
			m_texturepicker->GetTextCtrl()->Clear();
			imported_img_base.Destroy();
			imported_img = wxNullBitmap;
			m_textureposx->Enable(false);
			m_textureposy->Enable(false);
			m_texturewidth->Enable(false);
			m_textureheight->Enable(false);
			main_img_base = ConvertFullTIMToImage(tim, pal[m_palettelist->GetSelection()]);
			UpdateImage();
		}*/
	} else if (id == wxID_EXPORT) {
		FieldTextureExportWindow dial(this);
		dial.m_languagetitle->Enable(field.title_tile_amount > 0);
		if (dial.ShowModal() == wxID_OK) {
			if (dial.m_exportbgx->IsChecked()) {
				wxFileName fullpath(dial.m_filepicker->GetPath());
				fullpath.SetExt(L"bgx");
				wxString folder = fullpath.GetPath();
				wxString bgxname = fullpath.GetFullName();
				if (field.WriteBGX(folder.ToStdString(), bgxname.ToStdString())) {
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE, folder + bgxname);
					return;
				}
			} else {
				bool* tileflag = new bool[field.tiles_amount];
				for (unsigned int i = 0; i < field.tiles_amount; i++)
					tileflag[i] = m_tilechecklist->IsChecked(i) || !dial.m_onlyselected->IsChecked();
				int exportres = field.Export(dial.m_filepicker->GetPath().mb_str(), camera_id, tileflag, true, dial.m_mergetiles->IsChecked(), dial.m_exportorder->IsChecked(), dial.m_languagetitle->GetSelection() - 1);
				delete[] tileflag;
				if (exportres == 1) {
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE, dial.m_filepicker->GetPath());
					return;
				} else if (exportres == -1) {
					wxLogWarning(HADES_STRING_FIELDTEXTURE_SAVE_NOBGX);
					return;
				}
			}
			wxMessageDialog popupsuccess(this, HADES_STRING_FIELDTEXTURE_SAVE_SUCCESS, HADES_STRING_SUCCESS, wxOK | wxCENTRE);
			popupsuccess.ShowModal();
		}
	} else {
		EndModal(id);
	}
}

void ManageFieldTextureDialog::OnEntryWheel(wxMouseEvent& event) {
	int blockid = m_tilechecklist->GetSelection();
	bool wheelup = event.GetWheelRotation() > 0;
	int id = event.GetId();
	if (id == wxID_PATH && blockid != wxNOT_FOUND) {
		wxFileName fname = m_tilepath->GetFileName();
		wxArrayString filenames = GetFilesInDir(fname.GetPath(), _(L"*.png"));
		if (filenames.IsEmpty())
			return;
		int index = filenames.Index(fname.GetFullPath());
		if (index == wxNOT_FOUND)
			index = wheelup ? filenames.GetCount() - 1 : 0;
		else if (wheelup)
			index = index <= 0 ? filenames.GetCount() - 1 : index - 1;
		else
			index = index + 1 >= (int)filenames.GetCount() ? 0 : index + 1;
		m_tilepath->SetPath(filenames[index]);
		field.tiles[blockid].memoria_bgx_path = filenames[index];
		UpdateTexturePreview(blockid);
	} else if (id == wxID_SHADER && blockid != wxNOT_FOUND) {
		static wxArrayString shaderlist;
		if (shaderlist.IsEmpty()) {
			shaderlist.Add(_(L"PSX/FieldMap_Abr_None"));
			shaderlist.Add(_(L"PSX/FieldMap_Abr_0"));
			shaderlist.Add(_(L"PSX/FieldMap_Abr_1"));
			shaderlist.Add(_(L"PSX/FieldMap_Abr_2"));
			shaderlist.Add(_(L"PSX/FieldMap_Abr_3"));
		}
		int index = shaderlist.Index(m_tileshader->GetValue());
		if (index == wxNOT_FOUND)
			return;
		if (wheelup)
			index = index <= 0 ? shaderlist.GetCount() - 1 : index - 1;
		else
			index = index + 1 >= (int)shaderlist.GetCount() ? 0 : index + 1;
		m_tileshader->ChangeValue(shaderlist[index]);
		field.tiles[blockid].memoria_bgx_shader = shaderlist[index];
		UpdateTexturePreview(blockid);
	}
}

void ManageFieldTextureDialog::OnTextureMouseMove(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
		if (prevent_event)
			return;
		prevent_event = true;
		m_texturewindow->SetFocusIgnoringChildren();
		if (color_selected != -1) {
/*			wxClientDC dc(m_texturewindow);
			m_texturewindow->DoPrepareDC(dc);
			wxPoint mpos = event.GetLogicalPosition(dc);
			int timx = min(max(int(mpos.x * scale_ratio), 0), tim.GetWidth() - 1);
			int timy = min(max(int(mpos.y * scale_ratio), 0), tim.height - 1);
			tim.SetPixelValue(timx, timy, color_selected, char_flag ? m_palettelist->GetSelection() % 2 : -1);
			UpdateTexturePreview(-1);*/
		} else if (imported_img.IsOk()) {
			wxClientDC dc(m_texturewindow);
			m_texturewindow->DoPrepareDC(dc);
			wxPoint mpos = event.GetLogicalPosition(dc);
			imported_img_x = min(max(int(mpos.x * scale_ratio - imported_img_width / 2), m_textureposx->GetMin()), m_textureposx->GetMax());
			imported_img_y = min(max(int(mpos.y * scale_ratio - imported_img_height / 2), m_textureposy->GetMin()), m_textureposy->GetMax());
			m_textureposx->SetValue(imported_img_x);
			m_textureposy->SetValue(imported_img_y);
			DrawImage(dc);
		}
		prevent_event = false;
	}
}

void ManageFieldTextureDialog::OnTextureMouseWheel(wxMouseEvent& event) {
	if (event.GetWheelRotation() > 0)
		scale_ratio *= 0.95f;
	else
		scale_ratio = min(scale_ratio * 1.05, main_img_base.GetHeight() / DEFAULT_HEIGHT);
	UpdateImage();
}

void ManageFieldTextureDialog::OnPaletteMouseMove(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
/*		if (prevent_event)
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
		prevent_event = false;*/
	}
}

void ManageFieldTextureDialog::OnPaintTexture(wxPaintEvent& event) {
	wxPaintDC dc(m_texturewindow);
	DrawImage(dc);
}

void ManageFieldTextureDialog::OnPaintPalette(wxPaintEvent& event) {
//	DrawPaletteColors(m_palettelist->GetSelection());
}

void ManageFieldTextureDialog::OnTimer(wxTimerEvent& event) {
	bool update = false;
	for (unsigned int i = 0; i < field.anim_amount; i++) {
		if (anim_play_flag[i] & ANIM_PLAYFLAG_PLAY) {
			if (anim_tick_time[i] == 0) {
				if (anim_tile_pos[i] + 1 < field.anim[i].tile_amount || (anim_play_flag[i] & ANIM_PLAYFLAG_LOOP)) {
					m_tilechecklist->Check(field.anim[i].tile_list[anim_tile_pos[i]], false);
					anim_tile_pos[i]++;
					if (anim_tile_pos[i] >= field.anim[i].tile_amount)
						anim_tile_pos[i] = 0;
					m_tilechecklist->Check(field.anim[i].tile_list[anim_tile_pos[i]], true);
					anim_tick_time[i] = field.anim[i].tile_duration[anim_tile_pos[i]];
					update = true;
				}
			} else {
				anim_tick_time[i]--;
			}
		}
	}
	if (update)
		UpdateTexturePreview(-1);
}

void ImageDrawSelectionField(wxImage& img, int x, int y, int width, int height, unsigned char red, unsigned char green, unsigned char blue) {
	unsigned char* rawdata = img.GetData();
	unsigned char* rawalpha = img.GetAlpha();
	int thick = min(width, height) / 100 + 1;
	int i, j, posx, posy;

	#define MACRO_SETPIXELCOLOR() \
		rawdata[3 * (posx + posy)] = red; \
		rawdata[3 * (posx + posy) + 1] = green; \
		rawdata[3 * (posx + posy) + 2] = blue; \
		rawalpha[posx + posy] = 0xFF;

	width--;
	height--;
	for (i = 0; i <= width; i++) {
		posx = (x + i) % img.GetWidth();
		for (j = 0; j < thick; j++) {
			posy = ((y + j) % img.GetHeight()) * img.GetWidth();
			MACRO_SETPIXELCOLOR()
			posy = ((y + height - j) % img.GetHeight()) * img.GetWidth();
			MACRO_SETPIXELCOLOR()
		}
	}
	for (i = 0; i <= height; i++) {
		posy = (y + i) % img.GetHeight() * img.GetWidth();
		for (j = 0; j < thick; j++) {
			posx = (x + j) % img.GetWidth();
			MACRO_SETPIXELCOLOR()
			posx = (x + width - j) % img.GetWidth();
			MACRO_SETPIXELCOLOR()
		}
	}
}
