#include "Gui_SmallDialogs.h"

#include "main.h"
#include "Hades_Strings.h"
#include "Database_Animation.h"
#include "Database_SpellAnimation.h"

#define MACRO_FLAG_DISPLAY8(VARIABLE,CHECKBOX)	CHECKBOX ## 1->SetValue(VARIABLE & 0x1); CHECKBOX ## 2->SetValue(VARIABLE & 0x2); CHECKBOX ## 3->SetValue(VARIABLE & 0x4); CHECKBOX ## 4->SetValue(VARIABLE & 0x8);\
												CHECKBOX ## 5->SetValue(VARIABLE & 0x10); CHECKBOX ## 6->SetValue(VARIABLE & 0x20); CHECKBOX ## 7->SetValue(VARIABLE & 0x40); CHECKBOX ## 8->SetValue(VARIABLE & 0x80);
#define MACRO_FLAG_DISPLAY16(VARIABLE,CHECKBOX)	CHECKBOX ## 1->SetValue(VARIABLE & 0x1); CHECKBOX ## 2->SetValue(VARIABLE & 0x2); CHECKBOX ## 3->SetValue(VARIABLE & 0x4); CHECKBOX ## 4->SetValue(VARIABLE & 0x8);\
												CHECKBOX ## 5->SetValue(VARIABLE & 0x10); CHECKBOX ## 6->SetValue(VARIABLE & 0x20); CHECKBOX ## 7->SetValue(VARIABLE & 0x40); CHECKBOX ## 8->SetValue(VARIABLE & 0x80);\
												CHECKBOX ## 9->SetValue(VARIABLE & 0x100); CHECKBOX ## 10->SetValue(VARIABLE & 0x200); CHECKBOX ## 11->SetValue(VARIABLE & 0x400); CHECKBOX ## 12->SetValue(VARIABLE & 0x800);\
												CHECKBOX ## 13->SetValue(VARIABLE & 0x1000); CHECKBOX ## 14->SetValue(VARIABLE & 0x2000); CHECKBOX ## 15->SetValue(VARIABLE & 0x4000); CHECKBOX ## 16->SetValue(VARIABLE & 0x8000);
#define MACRO_FLAG_DISPLAY32(VARIABLE,CHECKBOX)	CHECKBOX ## 1->SetValue(VARIABLE & 0x1); CHECKBOX ## 2->SetValue(VARIABLE & 0x2); CHECKBOX ## 3->SetValue(VARIABLE & 0x4); CHECKBOX ## 4->SetValue(VARIABLE & 0x8);\
												CHECKBOX ## 5->SetValue(VARIABLE & 0x10); CHECKBOX ## 6->SetValue(VARIABLE & 0x20); CHECKBOX ## 7->SetValue(VARIABLE & 0x40); CHECKBOX ## 8->SetValue(VARIABLE & 0x80);\
												CHECKBOX ## 9->SetValue(VARIABLE & 0x100); CHECKBOX ## 10->SetValue(VARIABLE & 0x200); CHECKBOX ## 11->SetValue(VARIABLE & 0x400); CHECKBOX ## 12->SetValue(VARIABLE & 0x800);\
												CHECKBOX ## 13->SetValue(VARIABLE & 0x1000); CHECKBOX ## 14->SetValue(VARIABLE & 0x2000); CHECKBOX ## 15->SetValue(VARIABLE & 0x4000); CHECKBOX ## 16->SetValue(VARIABLE & 0x8000);\
												CHECKBOX ## 17->SetValue(VARIABLE & 0x10000); CHECKBOX ## 18->SetValue(VARIABLE & 0x20000); CHECKBOX ## 19->SetValue(VARIABLE & 0x40000); CHECKBOX ## 20->SetValue(VARIABLE & 0x80000);\
												CHECKBOX ## 21->SetValue(VARIABLE & 0x100000); CHECKBOX ## 22->SetValue(VARIABLE & 0x200000); CHECKBOX ## 23->SetValue(VARIABLE & 0x400000); CHECKBOX ## 24->SetValue(VARIABLE & 0x800000);\
												CHECKBOX ## 25->SetValue(VARIABLE & 0x1000000); CHECKBOX ## 26->SetValue(VARIABLE & 0x2000000); CHECKBOX ## 27->SetValue(VARIABLE & 0x4000000); CHECKBOX ## 28->SetValue(VARIABLE & 0x8000000);\
												CHECKBOX ## 29->SetValue(VARIABLE & 0x10000000); CHECKBOX ## 30->SetValue(VARIABLE & 0x20000000); CHECKBOX ## 31->SetValue(VARIABLE & 0x40000000); CHECKBOX ## 32->SetValue(VARIABLE & 0x80000000);



// Discardable message
DiscardableMessageWindow::DiscardableMessageWindow(wxWindow* parent, wxString text) : DiscardableMessage(parent) {
	m_label->SetLabel(text); Fit(); m_buttonok->SetFocus();
}

void DiscardableMessageWindow::OnButtonClickDiscard(wxCommandEvent& event) {
	EndModal(wxID_DISCARD);
}

void DiscardableMessageWindow::OnButtonClickOk(wxCommandEvent& event) {
	EndModal(wxID_OK);
}

// Image Map
ImageMapEditDialog::ImageMapEditDialog(wxWindow* parent, ImageMapDataStruct* imgmap, ImageMapDataStruct* enemy, GlobalImageMapStruct* global, SaveSet& saveset) :
	ImageMapEditWindow(parent) {
	GlobalMapCommonDirStruct* dir;
	int objectnameindex;
	unsigned int i, j;
	map.Copy(*imgmap);
	enemymap = enemy;
	globalmap = global;
	extradata = imgmap->GetExtraSize() / 0x10;
	if (saveset.sectionloaded[DATA_SECTION_ENMY])
		enemyset = saveset.enemyset;
	else
		enemyset = NULL;
	if (saveset.sectionloaded[DATA_SECTION_FIELD])
		fieldset = saveset.fieldset;
	else
		fieldset = NULL;

	#define MACRO_IMAGE_MAP_INITFILELIST(OBJKIND,DIR,COND,OBJAMOUNT,OBJID,OBJSTR) \
		dir = &globalmap->common_dir[DIR]; \
		globalliststr[OBJKIND].Alloc(dir->file_amount); \
		globallistid[OBJKIND] = new uint16_t*[dir->file_amount]; \
		for (i=0;i<dir->file_amount;i++) { \
			objectnameindex = -1; \
			if (COND) \
				for (j=0;j<OBJAMOUNT;j++) \
					if (OBJID==dir->file_id[i]) { \
						objectnameindex = j; \
						break; \
					} \
			if (objectnameindex>=0) \
				globalliststr[OBJKIND].Add(_(OBJSTR)+wxString::Format(wxT(" (%u)"),dir->file_id[i])); \
			else \
				globalliststr[OBJKIND].Add(HADES_STRING_IMAGE_MAP_OBJECT[OBJKIND]+wxString::Format(wxT("%u"),dir->file_id[i])); \
			globallistid[OBJKIND][i] = new uint16_t(dir->file_id[i]); \
		}

	MACRO_IMAGE_MAP_INITFILELIST(MAP_OBJECT_ENEMY, GLOBAL_MAP_DIR_ENEMY, enemyset, enemyset->battle_amount, enemyset->battle_data[j]->object_id, _(enemyset->battle_name[objectnameindex]))
	MACRO_IMAGE_MAP_INITFILELIST(MAP_OBJECT_SCENE, GLOBAL_MAP_DIR_BATTLE_SCENE, true, G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME), HADES_STRING_BATTLE_SCENE_NAME[j].id, _(HADES_STRING_BATTLE_SCENE_NAME[objectnameindex].label))
	MACRO_IMAGE_MAP_INITFILELIST(MAP_OBJECT_MUSIC, GLOBAL_MAP_DIR_MUSIC, true, G_N_ELEMENTS(HADES_STRING_MUSIC_NAME), HADES_STRING_MUSIC_NAME[j].id, _(HADES_STRING_MUSIC_NAME[objectnameindex].label))
	MACRO_IMAGE_MAP_INITFILELIST(MAP_OBJECT_AUDIO, GLOBAL_MAP_DIR_AUDIO, true, G_N_ELEMENTS(HADES_STRING_AUDIO_NAME), HADES_STRING_AUDIO_NAME[j].id, _(HADES_STRING_AUDIO_NAME[objectnameindex].label))
	MACRO_IMAGE_MAP_INITFILELIST(MAP_OBJECT_FIELD, GLOBAL_MAP_DIR_FIELD, fieldset, fieldset->amount, fieldset->script_data[j]->object_id, _(fieldset->script_data[objectnameindex]->name.GetStr(hades::TEXT_PREVIEW_TYPE)))
	MACRO_IMAGE_MAP_INITFILELIST(MAP_OBJECT_WORLD, GLOBAL_MAP_DIR_WORLD_MAP, true, G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME), HADES_STRING_WORLD_BLOCK_NAME[j].id, _(HADES_STRING_WORLD_BLOCK_NAME[objectnameindex].label))
	unsigned int mdlamount1 = globalmap->common_dir[GLOBAL_MAP_DIR_MODEL_PARTY].file_amount;
	unsigned int mdlamount2 = globalmap->common_dir[GLOBAL_MAP_DIR_MODEL_WEAPON].file_amount;
	unsigned int mdlamount3 = globalmap->common_dir[GLOBAL_MAP_DIR_MODEL_ENEMY].file_amount;
	unsigned int mdlamount = mdlamount1 + mdlamount2 + mdlamount3;
	uint16_t objid;
	globalliststr[MAP_OBJECT_MODEL].Alloc(mdlamount);
	globallistid[MAP_OBJECT_MODEL] = new uint16_t * [mdlamount];
	for (i = 0; i < mdlamount; i++) {
		objectnameindex = -1;
		if (i < mdlamount1)
			objid = globalmap->common_dir[GLOBAL_MAP_DIR_MODEL_PARTY].file_id[i];
		else if (i < mdlamount1 + mdlamount2)
			objid = globalmap->common_dir[GLOBAL_MAP_DIR_MODEL_WEAPON].file_id[i - mdlamount1];
		else
			objid = globalmap->common_dir[GLOBAL_MAP_DIR_MODEL_ENEMY].file_id[i - mdlamount1 - mdlamount2];
		for (j = 0; j < G_N_ELEMENTS(HADES_STRING_MODEL_NAME); j++)
			if (HADES_STRING_MODEL_NAME[j].id == objid) {
				objectnameindex = j;
				break;
			}
		if (objectnameindex >= 0)
			globalliststr[MAP_OBJECT_MODEL].Add(_(HADES_STRING_MODEL_NAME[j].label) + wxString::Format(wxT(" (%u)"), objid));
		else
			globalliststr[MAP_OBJECT_MODEL].Add(HADES_STRING_IMAGE_MAP_OBJECT[MAP_OBJECT_MODEL] + wxString::Format(wxT("%u"), objid));
		globallistid[MAP_OBJECT_MODEL][i] = new uint16_t(objid);
	}
}

int ImageMapEditDialog::ShowModal() {
	m_objecttypechoice->SetSelection(0);
	m_fulllist->Append(globalliststr[0], (void**)globallistid[0]);
	RefreshMapList();
	return ImageMapEditWindow::ShowModal();
}

void ImageMapEditDialog::RefreshMapList() {
	Map_Object kind = m_objecttypechoice->GetSelection();
	Chunk_Type chunktype = ImageMapDataStruct::GetChunkTypeFromMapObject(kind);
	int mapsel = m_maplist->GetSelection();
	int objectnameindex;
	unsigned int i, j;
	m_maplist->Clear();

	#define MACRO_IMAGE_MAP_REFRESHMAPLIST(COND,OBJAMOUNT,OBJID,OBJSTR) \
		objectnameindex = -1; \
		if (COND) \
			for (j=0;j<OBJAMOUNT;j++) \
				if (OBJID==map.data_id[i]) { \
					objectnameindex = j; \
					break; \
				} \
		if (objectnameindex>=0) \
			m_maplist->Append(OBJSTR+wxString::Format(wxT(" (%u)"),map.data_id[i]),new uint16_t(map.data_id[i])); \
		else \
			m_maplist->Append(HADES_STRING_IMAGE_MAP_OBJECT[kind]+wxString::Format(wxT("%u"),map.data_id[i]),new uint16_t(map.data_id[i]));

	switch (kind) {
	case MAP_OBJECT_ENEMY:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype) {
				bool alreadyput = false;
				for (j = 0; j < m_maplist->GetCount(); j++)
					if (*(uint16_t*)m_maplist->GetClientData(j) == map.data_id[i]) {
						alreadyput = true;
						break;
					}
				if (alreadyput)
					continue;
				MACRO_IMAGE_MAP_REFRESHMAPLIST(enemyset, enemyset->battle_amount, enemyset->battle_data[j]->object_id, _(enemyset->battle_name[objectnameindex]))
			}
		break;
	case MAP_OBJECT_SCENE:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype) {
				MACRO_IMAGE_MAP_REFRESHMAPLIST(true, G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME), HADES_STRING_BATTLE_SCENE_NAME[j].id, _(HADES_STRING_BATTLE_SCENE_NAME[objectnameindex].label))
			}
		break;
	case MAP_OBJECT_MUSIC:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype) {
				MACRO_IMAGE_MAP_REFRESHMAPLIST(true, G_N_ELEMENTS(HADES_STRING_MUSIC_NAME), HADES_STRING_MUSIC_NAME[j].id, _(HADES_STRING_MUSIC_NAME[objectnameindex].label))
			}
		break;
	case MAP_OBJECT_AUDIO:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype) {
				MACRO_IMAGE_MAP_REFRESHMAPLIST(true, G_N_ELEMENTS(HADES_STRING_AUDIO_NAME), HADES_STRING_AUDIO_NAME[j].id, _(HADES_STRING_AUDIO_NAME[objectnameindex].label))
			}
		break;
	case MAP_OBJECT_FIELD:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype && map.data_id[i] < 9000) {
				MACRO_IMAGE_MAP_REFRESHMAPLIST(fieldset, fieldset->amount, fieldset->script_data[j]->object_id, _(fieldset->script_data[objectnameindex]->name.GetStr(hades::TEXT_PREVIEW_TYPE)))
			}
		break;
	case MAP_OBJECT_WORLD:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype && map.data_id[i] >= 9000) {
				MACRO_IMAGE_MAP_REFRESHMAPLIST(true, G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME), HADES_STRING_WORLD_BLOCK_NAME[j].id, _(HADES_STRING_WORLD_BLOCK_NAME[objectnameindex].label))
			}
		break;
	case MAP_OBJECT_MODEL:
		for (i = 0; i < map.amount; i++)
			if (map.data_type[i] == chunktype) {
				MACRO_IMAGE_MAP_REFRESHMAPLIST(true, G_N_ELEMENTS(HADES_STRING_MODEL_NAME), HADES_STRING_MODEL_NAME[j].id, _(HADES_STRING_MODEL_NAME[objectnameindex].label))
			}
		break;
	}
	if (mapsel != wxNOT_FOUND)
		m_maplist->SetSelection(min(mapsel, (int)m_maplist->GetCount() - 1));
}

void ImageMapEditDialog::OnChoice(wxCommandEvent& event) {
	int sel = event.GetSelection();
	m_fulllist->Clear();
	m_fulllist->Append(globalliststr[sel], (void**)globallistid[sel]);
	m_maplist->SetSelection(wxNOT_FOUND);
	RefreshMapList();
}

bool DiscardImgMapSize = false;
void ImageMapEditDialog::OnList(wxCommandEvent& event) {
	int sel = event.GetSelection();
	int id = event.GetId();
	if (id == wxID_FULLLIST) {
		int added = map.AddData(m_objecttypechoice->GetSelection(), *(uint16_t*)m_fulllist->GetClientData(sel), globalmap, enemymap, extradata);
		if (added > 0) {
			if (m_objecttypechoice->GetSelection() == MAP_OBJECT_MODEL && added == 1) {
				ImageMapAddModelWindow newmodeldial(this);
				newmodeldial.ShowModal();
				map.data_related_id[map.amount - 1] = newmodeldial.m_entryvalue->GetValue();
			}
			extradata -= added;
			RefreshMapList();
		} else if (added == 0) {
		} else if (!DiscardImgMapSize) {
			DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_IMAGE_MAP_REACH_LIMIT);
			if (popup.ShowModal() == wxID_DISCARD)
				DiscardImgMapSize = true;
		}
	} else if (id == wxID_MAPLIST) {
		extradata += map.RemoveData(m_objecttypechoice->GetSelection(), *(uint16_t*)m_maplist->GetClientData(sel));
		RefreshMapList();
	}
}

void ImageMapEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADD) {
		int sel = m_fulllist->GetSelection();
		if (sel != wxNOT_FOUND) {
			int added = map.AddData(m_objecttypechoice->GetSelection(), *(uint16_t*)m_fulllist->GetClientData(sel), globalmap, enemymap, extradata);
			if (added > 0) {
				if (m_objecttypechoice->GetSelection() == MAP_OBJECT_MODEL && added == 1) {
					ImageMapAddModelWindow newmodeldial(this);
					newmodeldial.ShowModal();
					map.data_related_id[map.amount - 1] = newmodeldial.m_entryvalue->GetValue();
				}
				extradata -= added;
				RefreshMapList();
			} else if (added == 0) {
			} else if (!DiscardImgMapSize) {
				DiscardableMessageWindow popup(wxGetApp().GetTopWindow(), HADES_STRING_IMAGE_MAP_REACH_LIMIT);
				if (popup.ShowModal() == wxID_DISCARD)
					DiscardImgMapSize = true;
			}
		}
	} else if (id == wxID_REMOVE) {
		int sel = m_maplist->GetSelection();
		if (sel != wxNOT_FOUND) {
			extradata += map.RemoveData(m_objecttypechoice->GetSelection(), *(uint16_t*)m_maplist->GetClientData(sel));
			RefreshMapList();
		}
	} else
		EndModal(id);
}

// Steam Image Map
SteamImageMapEditDialog::SteamImageMapEditDialog(wxWindow* parent, ImageMapDataStruct* imgmap) :
	SteamImageMapEditWindow(parent) {
	unsigned int i;
	map.Copy(*imgmap);
	for (i = 0; i < map.type_amount; i++)
		m_objecttypechoice->Append(map.type_name[i]);
}

int SteamImageMapEditDialog::ShowModal() {
	wxString listvalue = _(L"");
	unsigned int i;
	type_sel = 0;
	m_objecttypechoice->SetSelection(0);
	for (i = 0; i + 1 < map.data_amount[0]; i++)
		listvalue += map.data_name[0][i] + _(L"\n");
	if (map.data_amount[0] > 0)
		listvalue += map.data_name[0][i];
	m_objectlist->ChangeValue(listvalue);
	return SteamImageMapEditWindow::ShowModal();
}

void SteamImageMapEditDialog::UpdatePreloadList() {
	unsigned int i, j;
	wxString objliststr = m_objectlist->GetValue();
	delete[] map.data_name[type_sel];
	if (objliststr.Len() == 0) {
		map.data_amount[type_sel] = 0;
		map.data_name[type_sel] = new string[0];
		return;
	}
	map.data_amount[type_sel] = 1;
	for (i = 0; i < objliststr.Len(); i++)
		if (objliststr[i] == L'\n')
			map.data_amount[type_sel]++;
	map.data_name[type_sel] = new string[map.data_amount[type_sel]];
	j = 0;
	map.data_name[type_sel][0] = "";
	for (i = 0; i < objliststr.Len(); i++)
		if (objliststr[i] == L'\n')
			map.data_name[type_sel][++j] = "";
		else
			map.data_name[type_sel][j] += objliststr[i];
}

void SteamImageMapEditDialog::OnChoice(wxCommandEvent& event) {
	wxString newlistvalue = _(L"");
	unsigned int i;
	UpdatePreloadList();
	type_sel = event.GetSelection();
	for (i = 0; i + 1 < map.data_amount[type_sel]; i++)
		newlistvalue += map.data_name[type_sel][i] + _(L"\n");
	if (map.data_amount[type_sel] > 0)
		newlistvalue += map.data_name[type_sel][i];
	m_objectlist->ChangeValue(newlistvalue);
}

void SteamImageMapEditDialog::OnButtonClick(wxCommandEvent& event) {
	UpdatePreloadList();
	EndModal(event.GetId());
}

// SFX
SpellModelWindow::SpellModelWindow(wxWindow* parent) : SpellAnimationWindow(parent) {
}

int SpellModelWindow::ShowModal(Spell_Target_Type tt, Spell_Target_Amount ta, uint16_t currentmodel, uint16_t currentmodelalt, bool alt) {
	int mainsel = -1, allsel = -1;
	unsigned int i, j = 0;
	if (ta == SPELL_TARGET_AMOUNT_VARIABLE) {
		if (alt)
			ta = SPELL_TARGET_AMOUNT_ONE;
		else
			ta = SPELL_TARGET_AMOUNT_GROUP;
	}
	if (ta == SPELL_TARGET_AMOUNT_RANDOM)
		ta = SPELL_TARGET_AMOUNT_GROUP;
	if (tt == SPELL_TARGET_TYPE_IRRELEVANT)
		tt = SPELL_TARGET_TYPE_ANY;
	uint16_t model = alt ? currentmodelalt : currentmodel;
	m_modelmain->Clear();
	for (i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_MODEL); i++) {
		if (model == HADES_STRING_SPELL_MODEL[i].id)
			allsel = i;
		if (HADES_STRING_SPELL_MODEL[i].bug_rate == 0) {
			if (HADES_STRING_SPELL_MODEL[i].target_type == SPELL_TARGET_TYPE_ANY || tt == HADES_STRING_SPELL_MODEL[i].target_type) {
				if (tt == SPELL_TARGET_TYPE_EVERYONE || tt == SPELL_TARGET_TYPE_SELF || ta == HADES_STRING_SPELL_MODEL[i].target_amount) {
					m_modelmain->Append(HADES_STRING_SPELL_MODEL[i].label, (void*)&HADES_STRING_SPELL_MODEL[i]);
					if (model == HADES_STRING_SPELL_MODEL[i].id)
						mainsel = j;
					j++;
				}
			}
		}
	}
	m_modelall->SetSelection(max(0, allsel));
	m_modelmain->SetSelection(max(0, mainsel));
	m_modelint->SetValue(model);
	if (mainsel >= 0) {
		m_panelall->Show(false);
		m_panelmain->Show(true);
		m_panelmain->Layout();
		m_panelmain->GetParent()->GetSizer()->Layout();
		m_panelmain->Refresh();
		m_enableall->SetValue(false);
	} else {
		m_panelmain->Show(false);
		m_panelall->Show(true);
		m_panelall->Layout();
		m_panelall->GetParent()->GetSizer()->Layout();
		m_panelall->Refresh();
		m_enableall->SetValue(true);
	}
	m_targetinfo->SetLabel(HADES_STRING_SPELL_TARGET_INFO_BASE + HADES_STRING_SPELL_TARGET_INFO[tt][ta]);
	return wxDialog::ShowModal();
}

void SpellModelWindow::OnSpin(wxSpinEvent& event) {
	int newmodel = event.GetPosition();
	for (unsigned int i = 0; i < G_N_ELEMENTS(HADES_STRING_SPELL_MODEL); i++)
		if (newmodel == HADES_STRING_SPELL_MODEL[i].id) {
			m_modelall->SetSelection(i);
			break;
		}
}

void SpellModelWindow::OnChoice(wxCommandEvent& event) {
	m_modelint->SetValue(HADES_STRING_SPELL_MODEL[m_modelall->GetSelection()].id);
}

void SpellModelWindow::OnCheckBox(wxCommandEvent& event) {
	if (event.GetInt()) {
		m_panelmain->Show(false);
		m_panelall->Show(true);
		m_panelall->Layout();
		m_panelall->GetParent()->GetSizer()->Layout();
		m_panelall->Refresh();
	} else {
		m_panelall->Show(false);
		m_panelmain->Show(true);
		m_panelmain->Layout();
		m_panelmain->GetParent()->GetSizer()->Layout();
		m_panelmain->Refresh();
	}
}

void SpellModelWindow::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}

// Status sets
SpellStatusDialog::SpellStatusDialog(wxWindow* parent) : SpellStatusWindow(parent) {
}

int SpellStatusDialog::ShowModal(uint32_t statusflags) {
	flags = statusflags;
	m_statusbox1->SetValue(flags & 0x1);			m_statusbox2->SetValue(flags & 0x2);			m_statusbox3->SetValue(flags & 0x4);			m_statusbox4->SetValue(flags & 0x8);
	m_statusbox5->SetValue(flags & 0x10);			m_statusbox6->SetValue(flags & 0x20);			m_statusbox7->SetValue(flags & 0x40);			m_statusbox8->SetValue(flags & 0x80);
	m_statusbox9->SetValue(flags & 0x100);			m_statusbox10->SetValue(flags & 0x200);			m_statusbox11->SetValue(flags & 0x400);			m_statusbox12->SetValue(flags & 0x800);
	m_statusbox13->SetValue(flags & 0x1000);		m_statusbox14->SetValue(flags & 0x2000);		m_statusbox15->SetValue(flags & 0x4000);		m_statusbox16->SetValue(flags & 0x8000);
	m_statusbox17->SetValue(flags & 0x10000);		m_statusbox18->SetValue(flags & 0x20000);		m_statusbox19->SetValue(flags & 0x40000);		m_statusbox20->SetValue(flags & 0x80000);
	m_statusbox21->SetValue(flags & 0x100000);		m_statusbox22->SetValue(flags & 0x200000);		m_statusbox23->SetValue(flags & 0x400000);		m_statusbox24->SetValue(flags & 0x800000);
	m_statusbox25->SetValue(flags & 0x1000000);		m_statusbox26->SetValue(flags & 0x2000000);		m_statusbox27->SetValue(flags & 0x4000000);		m_statusbox28->SetValue(flags & 0x8000000);
	m_statusbox29->SetValue(flags & 0x10000000);	m_statusbox30->SetValue(flags & 0x20000000);	m_statusbox31->SetValue(flags & 0x40000000);	m_statusbox32->SetValue(flags & 0x80000000);
	return SpellStatusWindow::ShowModal();
}

void SpellStatusDialog::OnButtonClick(wxCommandEvent& event) {
	flags = 0;
	flags |= m_statusbox1->IsChecked() ? 1 : 0;	flags |= m_statusbox2->IsChecked() << 1;	flags |= m_statusbox3->IsChecked() << 2;	flags |= m_statusbox4->IsChecked() << 3;
	flags |= m_statusbox5->IsChecked() << 4;	flags |= m_statusbox6->IsChecked() << 5;	flags |= m_statusbox7->IsChecked() << 6;	flags |= m_statusbox8->IsChecked() << 7;
	flags |= m_statusbox9->IsChecked() << 8;	flags |= m_statusbox10->IsChecked() << 9;	flags |= m_statusbox11->IsChecked() << 10;	flags |= m_statusbox12->IsChecked() << 11;
	flags |= m_statusbox13->IsChecked() << 12;	flags |= m_statusbox14->IsChecked() << 13;	flags |= m_statusbox15->IsChecked() << 14;	flags |= m_statusbox16->IsChecked() << 15;
	flags |= m_statusbox17->IsChecked() << 16;	flags |= m_statusbox18->IsChecked() << 17;	flags |= m_statusbox19->IsChecked() << 18;	flags |= m_statusbox20->IsChecked() << 19;
	flags |= m_statusbox21->IsChecked() << 20;	flags |= m_statusbox22->IsChecked() << 21;	flags |= m_statusbox23->IsChecked() << 22;	flags |= m_statusbox24->IsChecked() << 23;
	flags |= m_statusbox25->IsChecked() << 24;	flags |= m_statusbox26->IsChecked() << 25;	flags |= m_statusbox27->IsChecked() << 26;	flags |= m_statusbox28->IsChecked() << 27;
	flags |= m_statusbox29->IsChecked() << 28;	flags |= m_statusbox30->IsChecked() << 29;	flags |= m_statusbox31->IsChecked() << 30;	flags |= m_statusbox32->IsChecked() << 31;
	EndModal(event.GetId());
}

// Stat sets
ItemStatDialog::ItemStatDialog(wxWindow* parent) : ItemStatWindow(parent) {
}

int ItemStatDialog::ShowModal(ItemStatDataStruct& is) {
	m_speed->SetValue(is.speed);
	m_strength->SetValue(is.strength);
	m_magic->SetValue(is.magic);
	m_spirit->SetValue(is.spirit);
	MACRO_FLAG_DISPLAY8(is.element_absorb, m_elementabsorb)
	MACRO_FLAG_DISPLAY8(is.element_immune, m_elementimmune)
	MACRO_FLAG_DISPLAY8(is.element_half, m_elementhalf)
	MACRO_FLAG_DISPLAY8(is.element_weak, m_elementweak)
	MACRO_FLAG_DISPLAY8(is.element_boost, m_elementboost)
	return wxDialog::ShowModal();
}

void ItemStatDialog::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}

// Card sets
CardSetDialog::CardSetDialog(wxWindow* parent) : CardSetWindow(parent) {
}

int CardSetDialog::ShowModal(CardDataSet& cs, unsigned int set) {
	wxArrayString cardlist;
	cardlist.Alloc(CARD_AMOUNT);
	for (unsigned int i = 0; i < CARD_AMOUNT; i++)
		cardlist.Add(_(cs.card[i].name.GetStr(hades::TEXT_PREVIEW_TYPE)));
	m_choicecard1->Append(cardlist);	m_choicecard2->Append(cardlist);
	m_choicecard3->Append(cardlist);	m_choicecard4->Append(cardlist);
	m_choicecard5->Append(cardlist);	m_choicecard6->Append(cardlist);
	m_choicecard7->Append(cardlist);	m_choicecard8->Append(cardlist);
	m_choicecard9->Append(cardlist);	m_choicecard10->Append(cardlist);
	m_choicecard11->Append(cardlist);	m_choicecard12->Append(cardlist);
	m_choicecard13->Append(cardlist);	m_choicecard14->Append(cardlist);
	m_choicecard15->Append(cardlist);	m_choicecard16->Append(cardlist);
	m_choicecard1->SetSelection(cs.set[set].card[0]);	m_choicecard2->SetSelection(cs.set[set].card[1]);
	m_choicecard3->SetSelection(cs.set[set].card[2]);	m_choicecard4->SetSelection(cs.set[set].card[3]);
	m_choicecard5->SetSelection(cs.set[set].card[4]);	m_choicecard6->SetSelection(cs.set[set].card[5]);
	m_choicecard7->SetSelection(cs.set[set].card[6]);	m_choicecard8->SetSelection(cs.set[set].card[7]);
	m_choicecard9->SetSelection(cs.set[set].card[8]);	m_choicecard10->SetSelection(cs.set[set].card[9]);
	m_choicecard11->SetSelection(cs.set[set].card[10]);	m_choicecard12->SetSelection(cs.set[set].card[11]);
	m_choicecard13->SetSelection(cs.set[set].card[12]);	m_choicecard14->SetSelection(cs.set[set].card[13]);
	m_choicecard15->SetSelection(cs.set[set].card[14]);	m_choicecard16->SetSelection(cs.set[set].card[15]);
	return wxDialog::ShowModal();
}

void CardSetDialog::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}

// Playable character parameters
CharacterParameterDialog::CharacterParameterDialog(wxWindow* parent) : CharacterParameterWindow(parent) {
}

int CharacterParameterDialog::ShowModal(CharBattleParameterStruct& btl, InitialStatDataStruct& stat) {
	int i;

	#define MACRO_DISPLAY_STATUS_PARAM(NUM) \
		m_statusbone ## NUM->SetValue(btl.status_bone[NUM]); \
		m_statusy ## NUM->SetValue(btl.status_offy[NUM]); \
		m_statusz ## NUM->SetValue(btl.status_offz[NUM]);

	m_namekeyword->ChangeValue(_(stat.name_keyword));
	m_avatersprite->ChangeValue(_(btl.avatar_sprite));
	m_row->SetValue(stat.default_row != 0);
	m_winpose->SetValue(stat.default_winpose != 0);
	m_category1->SetValue((stat.default_category & 0x01) != 0);
	m_category2->SetValue((stat.default_category & 0x02) != 0);
	m_category3->SetValue((stat.default_category & 0x04) != 0);
	m_category4->SetValue((stat.default_category & 0x08) != 0);
	m_category5->SetValue((stat.default_category & 0x10) != 0);
	m_category6->SetValue((stat.default_category & 0x20) != 0);
	m_category7->SetValue((stat.default_category & 0x40) != 0);
	m_category8->SetValue((stat.default_category & 0x80) != 0);
	m_modelname->ChangeValue(_(btl.model));
	m_trancemodelname->ChangeValue(_(btl.model_trance));
	m_tranceglowr->SetValue(btl.trance_color[0]);
	m_tranceglowg->SetValue(btl.trance_color[1]);
	m_tranceglowb->SetValue(btl.trance_color[2]);
	m_attacksfx->SetValue(btl.attack_sfx);
	m_attacksound1->SetValue(btl.attack_sound[0]);
	m_attacksound2->SetValue(btl.attack_sound[1]);
	m_weaponbone->SetValue(btl.weapon_bone);
	MACRO_DISPLAY_STATUS_PARAM(0)
	MACRO_DISPLAY_STATUS_PARAM(1)
	MACRO_DISPLAY_STATUS_PARAM(2)
	MACRO_DISPLAY_STATUS_PARAM(3)
	MACRO_DISPLAY_STATUS_PARAM(4)
	MACRO_DISPLAY_STATUS_PARAM(5)
	m_shadowbone1->SetValue(btl.shadow_bone[0]);
	m_shadowbone2->SetValue(btl.shadow_bone[1]);
	m_shadowsizex->SetValue(btl.shadow_size[0]);
	m_shadowsizey->SetValue(btl.shadow_size[1]);
	m_shadowsizez->SetValue(btl.shadow_size[2]);
	wxArrayString modellabel;
	int modelsel = 0;
	int trancemodelsel = 0;
	modellabel.Alloc(G_N_ELEMENTS(HADES_STRING_MODEL_NAME));
	for (i = 0; i < G_N_ELEMENTS(HADES_STRING_MODEL_NAME); i++) {
		modellabel.Add(HADES_STRING_MODEL_NAME[i].label);
		if (HADES_STRING_MODEL_NAME[i].steamid.IsSameAs(_(btl.model)))
			modelsel = i;
		if (HADES_STRING_MODEL_NAME[i].steamid.IsSameAs(_(btl.model_trance)))
			trancemodelsel = i;
	}
	m_modelchoice->Append(modellabel);
	m_modelchoice->SetSelection(modelsel);
	m_trancemodelchoice->Append(modellabel);
	m_trancemodelchoice->SetSelection(trancemodelsel);
	wxArrayString animlabel;
	wxArrayString animname;
	int saatindex = 0;
	for (i = 0; i < G_N_ELEMENTS(SPELLANIM_ARGUMENT_STRING); i++)
		if (SPELLANIM_ARGUMENT_STRING[i].type == SAAT_ANIMATION) {
			saatindex = i;
			break;
		}
	m_listanimchoice.resize(BATTLE_MOTION_AMOUNT);
	m_listanimname.resize(BATTLE_MOTION_AMOUNT);
	for (i = 0; i < BATTLE_MOTION_AMOUNT; i++) {
		wxStaticText* label = new wxStaticText(m_animscrolled, wxID_ANY, _(SPELLANIM_ARGUMENT_STRING[saatindex + i].label), wxDefaultPosition, wxDefaultSize, 0);
		m_listanimchoice[i] = new wxChoice(m_animscrolled, wxID_ANY, wxDefaultPosition, wxDefaultSize, animlabel);
		m_listanimname[i] = new wxTextCtrl(m_animscrolled, wxID_ANY, _(btl.anim[i]), wxDefaultPosition, wxDefaultSize, 0);
		m_animsizer->Add(label, 0, wxALL, 5);
		m_animsizer->Add(m_listanimchoice[i], 0, wxALL, 5);
		m_animsizer->Add(m_listanimname[i], 0, wxALL | wxEXPAND, 5);
		m_listanimchoice[i]->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CharacterParameterDialog::OnAnimChoice), NULL, this);
	}
	ColorSelection();
	ModelSelection();
	m_animscrolled->Layout();
	m_animscrolled->Refresh();
	return wxDialog::ShowModal();
}

void CharacterParameterDialog::ApplyModifications(CharBattleParameterStruct& btl, InitialStatDataStruct& stat) {

	#define MACRO_RETRIEVE_STATUS_PARAM(NUM) \
		btl.status_bone[NUM] = m_statusbone ## NUM->GetValue(); \
		btl.status_offy[NUM] = m_statusy ## NUM->GetValue(); \
		btl.status_offz[NUM] = m_statusz ## NUM->GetValue();

	stat.name_keyword = m_namekeyword->GetValue().ToStdWstring();
	while (stat.name_keyword.length() < 4)
		stat.name_keyword += L"X";
	btl.avatar_sprite = m_avatersprite->GetValue().ToStdWstring();
	stat.default_row = m_row->IsChecked() ? 1 : 0;
	stat.default_winpose = m_winpose->IsChecked() ? 1 : 0;
	stat.default_category = (m_category1->IsChecked() ? 0x01 : 0) | (m_category2->IsChecked() ? 0x02 : 0) | (m_category3->IsChecked() ? 0x04 : 0) | (m_category4->IsChecked() ? 0x08 : 0)
						  | (m_category5->IsChecked() ? 0x10 : 0) | (m_category6->IsChecked() ? 0x20 : 0) | (m_category7->IsChecked() ? 0x40 : 0) | (m_category8->IsChecked() ? 0x80 : 0);
	btl.model = m_modelname->GetValue().ToStdWstring();
	btl.model_trance = m_trancemodelname->GetValue().ToStdWstring();
	btl.trance_color[0] = m_tranceglowr->GetValue();
	btl.trance_color[1] = m_tranceglowg->GetValue();
	btl.trance_color[2] = m_tranceglowb->GetValue();
	btl.attack_sfx = m_attacksfx->GetValue();
	btl.attack_sound[0] = m_attacksound1->GetValue();
	btl.attack_sound[1] = m_attacksound2->GetValue();
	btl.weapon_bone = m_weaponbone->GetValue();
	MACRO_RETRIEVE_STATUS_PARAM(0)
	MACRO_RETRIEVE_STATUS_PARAM(1)
	MACRO_RETRIEVE_STATUS_PARAM(2)
	MACRO_RETRIEVE_STATUS_PARAM(3)
	MACRO_RETRIEVE_STATUS_PARAM(4)
	MACRO_RETRIEVE_STATUS_PARAM(5)
	btl.shadow_bone[0] = m_shadowbone1->GetValue();
	btl.shadow_bone[1] = m_shadowbone2->GetValue();
	btl.shadow_size[0] = m_shadowsizex->GetValue();
	btl.shadow_size[1] = m_shadowsizey->GetValue();
	btl.shadow_size[2] = m_shadowsizez->GetValue();
	for (int i = 0; i < BATTLE_MOTION_AMOUNT; i++)
		btl.anim[i] = m_listanimname[i]->GetValue().ToStdWstring();
}

uint8_t _bmp_static_1010[300];
void CharacterParameterDialog::ColorSelection() {
	uint8_t r = m_tranceglowr->GetValue();
	uint8_t g = m_tranceglowg->GetValue();
	uint8_t b = m_tranceglowb->GetValue();
	wxImage img;
	for (int i = 0; i < 300; i += 3) {
		_bmp_static_1010[i] = r;
		_bmp_static_1010[i + 1] = g;
		_bmp_static_1010[i + 2] = b;
	}
	img.Create(10, 10, _bmp_static_1010, true);
	wxBitmap bmp(img);
	m_colorbmp->SetBitmap(bmp);
}

void CharacterParameterDialog::ModelSelection() {
	int modelsel = m_modelchoice->GetSelection();
	if (modelsel == wxNOT_FOUND)
		return;
	animname.Clear();
	animlabel.Clear();
	uint32_t modelid = HADES_STRING_MODEL_NAME[modelsel].id;
	int animcount = G_N_ELEMENTS(DATABASE_ANIMATION_SINGLEID);
	int i, j;
	for (i = 0; i < animcount; i++) {
		if (DATABASE_ANIMATION_SINGLEID[i].model_id == modelid) {
			animname.Add(DATABASE_ANIMATION_SINGLEID[i].steam_id);
			animlabel.Add(DATABASE_ANIMATION_SINGLEID[i].label);
		}
	}
	animcount = G_N_ELEMENTS(DATABASE_ANIMATION_DOUBLEID);
	for (i = 0; i < animcount; i++) {
		if (DATABASE_ANIMATION_DOUBLEID[i].model_id == modelid) {
			animname.Add(DATABASE_ANIMATION_DOUBLEID[i].steam_id);
			animlabel.Add(DATABASE_ANIMATION_DOUBLEID[i].label);
		}
	}
	for (i = 0; i < (int)m_listanimchoice.size(); i++) {
		m_listanimchoice[i]->Set(animlabel);
		for (j = 0; j < (int)animname.GetCount(); j++)
			if (m_listanimname[i]->GetValue().IsSameAs(animname[j])) {
				m_listanimchoice[i]->SetSelection(j);
				break;
			}
	}
}

void CharacterParameterDialog::OnChangeColor(wxSpinEvent& event) {
	ColorSelection();
}

void CharacterParameterDialog::OnChangeModel(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_MODEL) {
		m_modelname->ChangeValue(HADES_STRING_MODEL_NAME[event.GetSelection()].steamid);
		ModelSelection();
	} else if (id == wxID_TRANCE) {
		m_trancemodelname->ChangeValue(HADES_STRING_MODEL_NAME[event.GetSelection()].steamid);
	}
}

void CharacterParameterDialog::OnAnimChoice(wxCommandEvent& event) {
	for (unsigned int i = 0; i < m_listanimchoice.size(); i++)
		if (m_listanimchoice[i] == event.GetEventObject()) {
			m_listanimname[i]->ChangeValue(animname[event.GetSelection()]);
			return;
		}
}

void CharacterParameterDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_OK || id == wxID_CANCEL) {
		for (unsigned int i = 0; i < m_listanimchoice.size(); i++)
			m_listanimchoice[i]->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(CharacterParameterDialog::OnAnimChoice), NULL, this);
		EndModal(id);
	}
}

// Enemy resources
EnemyResourceDialog::EnemyResourceDialog(wxWindow* parent) : EnemyResourceWindow(parent) {
}

int EnemyResourceDialog::ShowModal(EnemyStatDataStruct& es, BattleDataStruct& bd) {
	unsigned int i, j;
	unsigned int maxtextamount = es.parent->parent->text[es.parent->id]->amount - es.parent->stat_amount - es.parent->spell_amount;
	for (i = 0; i < es.parent->stat_amount; i++)
		if (i != es.id)
			maxtextamount -= es.parent->stat[i].text_amount;
	m_textamount->SetMax(maxtextamount);
	m_textamount->SetValue(es.text_amount);
	m_radius->SetValue(es.radius);
	m_mesh->SetValue(es.mesh);							m_meshvanish->SetValue(es.mesh_vanish);
	m_bonetarget->SetValue(es.bone_target);
	m_bonecamera1->SetValue(es.bone_camera1);			m_bonecamera2->SetValue(es.bone_camera2);			m_bonecamera3->SetValue(es.bone_camera3);
	m_boneselection1->SetValue(es.selection_bone[0]);	m_boneselection2->SetValue(es.selection_bone[1]);	m_boneselection3->SetValue(es.selection_bone[2]);
	m_boneselection4->SetValue(es.selection_bone[3]);	m_boneselection5->SetValue(es.selection_bone[4]);	m_boneselection6->SetValue(es.selection_bone[5]);
	m_boneshadow1->SetValue(es.shadow_bone1);			m_boneshadow2->SetValue(es.shadow_bone2);
	m_shadowsizex->SetValue(es.shadow_size_x);			m_shadowsizey->SetValue(es.shadow_size_y);
	m_soundengage->SetValue(es.sound_engage);			m_sounddeath->SetValue(es.sound_death);
	m_animidle->SetValue(es.anim_idle);					m_animhit->SetValue(es.anim_hit);					m_animdeath->SetValue(es.anim_death);
	m_animidlealt->SetValue(es.anim_idle_alt);			m_animhitalt->SetValue(es.anim_hit_alt);			m_animdeathalt->SetValue(es.anim_death_alt);
	m_listanim.resize(es.sequence_anim_amount);			m_listanimid.resize(es.sequence_anim_amount);		m_listanimdelete.resize(es.sequence_anim_amount);
	int32_t animindex = AnimationDatabase::GetIndexFromModelId(es.model);
	if (animindex >= 0)
		while (AnimationDatabase::GetModelId(animindex) == es.model) {
			anim.Add(AnimationDatabase::GetDescription(animindex));
			anim_id.push_back(AnimationDatabase::GetId(animindex));
			animindex++;
		}
	m_animidlechoice->Append(anim);		m_animhitchoice->Append(anim);		m_animdeathchoice->Append(anim);
	m_animidlealtchoice->Append(anim);	m_animhitaltchoice->Append(anim);	m_animdeathaltchoice->Append(anim);
	for (i = 0; i < anim_id.size(); i++) {
		if (es.anim_idle == anim_id[i])		m_animidlechoice->SetSelection(i);
		if (es.anim_hit == anim_id[i])		m_animhitchoice->SetSelection(i);
		if (es.anim_death == anim_id[i])		m_animdeathchoice->SetSelection(i);
		if (es.anim_idle_alt == anim_id[i])	m_animidlealtchoice->SetSelection(i);
		if (es.anim_hit_alt == anim_id[i])	m_animhitaltchoice->SetSelection(i);
		if (es.anim_death_alt == anim_id[i])	m_animdeathaltchoice->SetSelection(i);
	}
	if (GetGameType() == GAME_TYPE_PSX) {
		m_animidle->Enable(false);		m_animhit->Enable(false);		m_animdeath->Enable(false);
		m_animidlealt->Enable(false);	m_animhitalt->Enable(false);	m_animdeathalt->Enable(false);
		m_attackanimadd->Enable(false);
	}
	for (i = 0; i < es.sequence_anim_amount; i++) {
		m_listanim[i] = new wxChoice(m_animscrolled, wxID_ANY, wxDefaultPosition, wxDefaultSize, anim);
		m_listanimid[i] = new wxSpinCtrl(m_animscrolled, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535, bd.animation_id[es.sequence_anim_base + i]);
		m_listanimdelete[i] = new wxButton(m_animscrolled, wxID_ANY, _(L"x"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
		for (j = 0; j < anim.GetCount(); j++)
			if (anim_id[j] == bd.animation_id[es.sequence_anim_base + i]) {
				m_listanim[i]->SetSelection(j);
				break;
			}
		m_animsizer->Add(m_listanim[i], 0, wxALL | wxEXPAND, 2);
		m_animsizer->Add(m_listanimid[i], 0, wxALL, 2);
		m_animsizer->Add(m_listanimdelete[i], 0, wxALL, 2);
		if (GetGameType() == GAME_TYPE_PSX) {
			m_listanim[i]->Enable(false);
			m_listanimid[i]->Enable(false);
			m_listanimdelete[i]->Enable(false);
		} else {
			m_listanim[i]->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(EnemyResourceDialog::OnAnimChoice), NULL, this);
			m_listanimdelete[i]->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EnemyResourceDialog::OnButtonClick), NULL, this);
		}
	}
	m_animscrolled->Layout();
	m_animscrolled->Refresh();
	return wxDialog::ShowModal();
}

void EnemyResourceDialog::OnAnimChoice(wxCommandEvent& event) {
	wxSpinCtrl* linkedspin = NULL;
	int id = event.GetId();
	switch (id) {
	case wxID_IDLE:		linkedspin = m_animidle;		break;
	case wxID_HIT:		linkedspin = m_animhit;			break;
	case wxID_DEATH:	linkedspin = m_animdeath;		break;
	case wxID_IDLEALT:	linkedspin = m_animidlealt;		break;
	case wxID_HITALT:	linkedspin = m_animhitalt;		break;
	case wxID_DEATHALT:	linkedspin = m_animdeathalt;	break;
	default:
		for (unsigned int i = 0; i < m_listanim.size(); i++)
			if (m_listanim[i] == event.GetEventObject()) {
				linkedspin = m_listanimid[i];
				break;
			}
	}
	linkedspin->SetValue(anim_id[event.GetSelection()]);
}

void EnemyResourceDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_OK || id == wxID_CANCEL) {
		for (unsigned int i = 0; i < m_listanim.size(); i++) {
			m_listanim[i]->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(EnemyResourceDialog::OnAnimChoice), NULL, this);
			m_listanimdelete[i]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EnemyResourceDialog::OnButtonClick), NULL, this);
		}
		EndModal(id);
	} else if (id == wxID_ANIM) {
		m_listanim.push_back(new wxChoice(m_animscrolled, wxID_ANY, wxDefaultPosition, wxDefaultSize, anim));
		m_listanimid.push_back(new wxSpinCtrl(m_animscrolled, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 65535));
		m_listanimdelete.push_back(new wxButton(m_animscrolled, wxID_ANY, _(L"x"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT));
		if (anim.GetCount() > 0) {
			m_listanim.back()->SetSelection(0);
			m_listanimid.back()->SetValue(anim_id[0]);
		}
		m_animsizer->Add(m_listanim.back(), 0, wxALL | wxEXPAND, 2);
		m_animsizer->Add(m_listanimid.back(), 0, wxALL, 2);
		m_animsizer->Add(m_listanimdelete.back(), 0, wxALL, 2);
		m_listanim.back()->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(EnemyResourceDialog::OnAnimChoice), NULL, this);
		m_listanimdelete.back()->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EnemyResourceDialog::OnButtonClick), NULL, this);
		m_animscrolled->FitInside();
		m_animscrolled->Layout();
		m_animscrolled->Refresh();
	} else {
		wxButton* deletechoice = static_cast<wxButton*>(event.GetEventObject());
		unsigned int deleteid;
		for (deleteid = 0; deleteid < m_listanimdelete.size(); deleteid++)
			if (deletechoice == m_listanimdelete[deleteid])
				break;
		if (deleteid < m_listanimdelete.size()) {
			m_listanim[deleteid]->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(EnemyResourceDialog::OnAnimChoice), NULL, this);
			m_listanimdelete[deleteid]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EnemyResourceDialog::OnButtonClick), NULL, this);
			m_listanim[deleteid]->Destroy();
			m_listanimid[deleteid]->Destroy();
			m_listanimdelete[deleteid]->Destroy();
			m_listanim.erase(m_listanim.begin() + deleteid);
			m_listanimid.erase(m_listanimid.begin() + deleteid);
			m_listanimdelete.erase(m_listanimdelete.begin() + deleteid);
		}
	}
}

bool EnemyResourceDialog::ApplyModifications(EnemyStatDataStruct& es, BattleDataStruct& bd) {
	bool result = true;
	es.text_amount = m_textamount->GetValue();
	es.radius = m_radius->GetValue();
	es.mesh = m_mesh->GetValue();							es.mesh_vanish = m_meshvanish->GetValue();
	es.bone_target = m_bonetarget->GetValue();
	es.bone_camera1 = m_bonecamera1->GetValue();			es.bone_camera2 = m_bonecamera2->GetValue();			es.bone_camera3 = m_bonecamera3->GetValue();
	es.selection_bone[0] = m_boneselection1->GetValue();	es.selection_bone[1] = m_boneselection2->GetValue();	es.selection_bone[2] = m_boneselection3->GetValue();
	es.selection_bone[3] = m_boneselection4->GetValue();	es.selection_bone[4] = m_boneselection5->GetValue();	es.selection_bone[5] = m_boneselection6->GetValue();
	es.shadow_bone1 = m_boneshadow1->GetValue();			es.shadow_bone2 = m_boneshadow2->GetValue();
	es.shadow_size_x = m_shadowsizex->GetValue();			es.shadow_size_y = m_shadowsizey->GetValue();
	es.sound_engage = m_soundengage->GetValue();			es.sound_death = m_sounddeath->GetValue();
	es.anim_idle = m_animidle->GetValue();					es.anim_hit = m_animhit->GetValue();					es.anim_death = m_animdeath->GetValue();
	es.anim_idle_alt = m_animidlealt->GetValue();			es.anim_hit_alt = m_animhitalt->GetValue();				es.anim_death_alt = m_animdeathalt->GetValue();
	if (GetGameType() != GAME_TYPE_PSX) {
		unsigned int i, firstmodif;
		for (i = 0; i < m_listanimid.size() && i < es.sequence_anim_amount; i++)
			if (m_listanimid[i]->GetValue() != bd.animation_id[es.sequence_anim_base + i])
				break;
		firstmodif = i;
		for (i = firstmodif; i < es.sequence_anim_amount;)
			es.parent->RemoveAnimation(es.sequence_anim_base + i);
		for (i = firstmodif; i < m_listanimid.size(); i++)
			result = (es.parent->AddAnimation(es.id, m_listanimid[i]->GetValue()) == 0) && result;
	}
	return result;
}
