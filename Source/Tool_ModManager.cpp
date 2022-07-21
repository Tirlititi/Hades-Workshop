#include "Tool_ModManager.h"

//#include <wx/tokenzr.h>
//#include <algorithm>
//#include "main.h"
#include "Gui_TextureEditor.h"
//#include "Gui_LoadingDialog.h"
//#include "Hades_Strings.h"
//#include "Steam_Strings.h"
#include "Database_SpellAnimation.h"

struct ModManagerClientData : public wxClientData {
	unsigned int type;
	unsigned int section;
	unsigned int index;
	Chunk_Type chunk_type;
	
	ModManagerClientData(unsigned int t, unsigned int sec, uint16_t obji = 0, Chunk_Type chkt = 0xFF) :
		type(t),
		section(sec),
		index(obji),
		chunk_type(chkt) {}
};

ToolModManager::ToolModManager(wxWindow* p) :
	ModManagerWindow(p) {
	m_listtree->AppendColumn(wxEmptyString);
	m_listtree->Connect( wxEVT_TREELIST_ITEM_CHECKED, wxTreeListEventHandler( ToolModManager::OnTreeListCheck ), NULL, this );
}

ToolModManager::~ToolModManager() {
	m_listtree->Disconnect( wxEVT_TREELIST_ITEM_CHECKED, wxTreeListEventHandler( ToolModManager::OnTreeListCheck ), NULL, this );
}

int ToolModManager::ShowModal(CDDataStruct* data) {
	unsigned int i, j;
	cddata = data;
	wxTreeListItem topitem, curitem, lowitem;
	SaveSet* dataset = &cddata->saveset;
	topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"Generic"));
	m_listtree->SetItemData(topitem, new ModManagerClientData(0, 0));
	if (cddata->saveset.sectionloaded[DATA_SECTION_SPELL]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Spells"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_SPELL));
		if (cddata->saveset.sectionmodified[DATA_SECTION_SPELL]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_SUPPORT]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Supporting Abilities"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_SUPPORT));
		if (cddata->saveset.sectionmodified[DATA_SECTION_SUPPORT]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_CMD]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Commands"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_CMD));
		if (cddata->saveset.sectionmodified[DATA_SECTION_CMD]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_STAT]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Stats"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_STAT));
		if (cddata->saveset.sectionmodified[DATA_SECTION_STAT]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_PARTY_SPECIAL]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Party - Special"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_PARTY_SPECIAL));
		if (cddata->saveset.sectionmodified[DATA_SECTION_PARTY_SPECIAL]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_ITEM]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Items"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_ITEM));
		if (cddata->saveset.sectionmodified[DATA_SECTION_ITEM]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_SHOP]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Shops"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_SHOP));
		if (cddata->saveset.sectionmodified[DATA_SECTION_SHOP]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_CARD]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Tetra Master"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_CARD));
		if (cddata->saveset.sectionmodified[DATA_SECTION_CARD]) m_listtree->CheckItem(curitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_SPELL_ANIM]) {
		curitem = m_listtree->AppendItem(topitem, _(L"Spell Animations"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_SPELL_ANIM));
		for (i = 0; i < cddata->spellanimset.amount; i++)
			if (!cddata->spellanimset.spell[i].is_empty) {
				for (j = 0; j < G_N_ELEMENTS(HADES_STRING_SPELL_MODEL); j++)
					if (HADES_STRING_SPELL_MODEL[j].id == i) {
						lowitem = m_listtree->AppendItem(curitem, _(HADES_STRING_SPELL_MODEL[j].label));
						break;
					}
				if (j == G_N_ELEMENTS(HADES_STRING_SPELL_MODEL))
					lowitem = m_listtree->AppendItem(curitem, wxString::Format(wxT("Spell Animation %u"), i));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(2, DATA_SECTION_SPELL_ANIM, i));
				if (cddata->spellanimset.spell[i].modified_data != 0) m_listtree->CheckItem(lowitem);
			}
		if (!m_listtree->AreAllChildrenInState(curitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(curitem);
	}
	if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
		m_listtree->CheckItem(topitem);
	if (cddata->saveset.sectionloaded[DATA_SECTION_ENMY]) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"Enemies"));
		m_listtree->SetItemData(topitem, new ModManagerClientData(0, 1));
		for (i = 0; i < dataset->enemyset->battle_amount; i++) {
			curitem = m_listtree->AppendItem(topitem, _(dataset->enemyset->battle_name[i]));
			m_listtree->SetItemData(curitem, new ModManagerClientData(2, DATA_SECTION_ENMY, i));
			lowitem = m_listtree->AppendItem(curitem, _(L"Enemy Data"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_ENMY, i, CHUNK_TYPE_ENEMY_STATS));
			if (dataset->enemyset->battle[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem, _(L"Script"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_ENMY, i, CHUNK_TYPE_SCRIPT));
			if (dataset->enemyset->script[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem, _(L"Texts"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_ENMY, i, CHUNK_TYPE_TEXT));
			if (dataset->enemyset->text[i]->modified) m_listtree->CheckItem(lowitem);
			if (GetGameType() == GAME_TYPE_PSX) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Preloading"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_ENMY, i, CHUNK_TYPE_IMAGE_MAP));
				if (dataset->enemyset->preload[i]->modified) m_listtree->CheckItem(lowitem);
			}
			lowitem = m_listtree->AppendItem(curitem, _(L"Animations & Cameras"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_ENMY, i, CHUNK_TYPE_BATTLE_DATA));
			if (dataset->enemyset->battle_data[i]->modified) m_listtree->CheckItem(lowitem);
			if (!m_listtree->AreAllChildrenInState(curitem, wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_TEXT]) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"Texts"));
		m_listtree->SetItemData(topitem, new ModManagerClientData(0, 2));
		for (i = 0; i < dataset->textset->amount; i++) {
			curitem = m_listtree->AppendItem(topitem, _(dataset->textset->name[i]));
			m_listtree->SetItemData(curitem, new ModManagerClientData(2, DATA_SECTION_TEXT, i));
			if (dataset->textset->text_data[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Texts"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_TEXT, i, CHUNK_TYPE_TEXT));
				if (dataset->textset->text_data[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType() == GAME_TYPE_PSX && dataset->textset->charmap[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Charmap"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_TEXT, i, CHUNK_TYPE_CHARMAP));
				if (dataset->textset->charmap[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType() == GAME_TYPE_PSX && dataset->textset->chartim[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Texture"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_TEXT, i, CHUNK_TYPE_TIM));
				if (dataset->textset->chartim[i]->parent_chunk->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem, wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_WORLD_MAP]) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"World Maps"));
		m_listtree->SetItemData(topitem, new ModManagerClientData(0, 3));
		curitem = m_listtree->AppendItem(topitem, _(L"Common Data"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_WORLD_MAP));
		if (dataset->worldset->world_data->modified) m_listtree->CheckItem(curitem);
		for (i = 0; i < dataset->worldset->amount; i++) {
			curitem = m_listtree->AppendItem(topitem, _(dataset->worldset->name[i]));
			m_listtree->SetItemData(curitem, new ModManagerClientData(2, DATA_SECTION_WORLD_MAP, i));
			lowitem = m_listtree->AppendItem(curitem, _(L"Script"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_WORLD_MAP, i, CHUNK_TYPE_SCRIPT));
			if (dataset->worldset->script[i]->modified) m_listtree->CheckItem(lowitem);
			if (GetGameType() == GAME_TYPE_PSX || i == 0) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Texts"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_WORLD_MAP, i, CHUNK_TYPE_TEXT));
				if (dataset->worldset->text_data[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType() == GAME_TYPE_PSX) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Preloading"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_WORLD_MAP, i, CHUNK_TYPE_IMAGE_MAP));
				if (dataset->worldset->preload[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType() == GAME_TYPE_PSX && dataset->worldset->charmap[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Charmap"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_WORLD_MAP, i, CHUNK_TYPE_CHARMAP));
				if (dataset->worldset->charmap[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType() == GAME_TYPE_PSX && dataset->worldset->chartim[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Texture"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_WORLD_MAP, i, CHUNK_TYPE_TIM));
				if (dataset->worldset->chartim[i]->parent_chunk->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem, wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_FIELD]) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"Fields"));
		m_listtree->SetItemData(topitem, new ModManagerClientData(0, 4));
		for (i = 0; i < dataset->fieldset->amount; i++) {
			curitem = m_listtree->AppendItem(topitem, _(dataset->fieldset->script_data[i]->name.str_nice));
			m_listtree->SetItemData(curitem, new ModManagerClientData(2, DATA_SECTION_FIELD, i));
			lowitem = m_listtree->AppendItem(curitem, _(L"Script"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_FIELD, i, CHUNK_TYPE_SCRIPT));
			if (dataset->fieldset->script_data[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem, _(L"Preloading"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_FIELD, i, CHUNK_TYPE_IMAGE_MAP));
			if (dataset->fieldset->preload[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem, _(L"Model Role"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_FIELD, i, CHUNK_TYPE_FIELD_ROLE));
			if (dataset->fieldset->role[i]->modified) m_listtree->CheckItem(lowitem);
			if (dataset->fieldset->background_data[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Background"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_FIELD, i, CHUNK_TYPE_FIELD_TILES));
				if (dataset->fieldset->background_data[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (dataset->fieldset->walkmesh[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Walkmesh"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_FIELD, i, CHUNK_TYPE_FIELD_WALK));
				if (dataset->fieldset->walkmesh[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem, wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_BATTLE_SCENE]) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"Battle Scenes"));
		m_listtree->SetItemData(topitem, new ModManagerClientData(0, 5));
		for (i = 0; i < dataset->sceneset->scene_amount; i++) {
			curitem = m_listtree->AppendItem(topitem, _(dataset->sceneset->scene_name[i]));
			m_listtree->SetItemData(curitem, new ModManagerClientData(2, DATA_SECTION_BATTLE_SCENE, i));
			lowitem = m_listtree->AppendItem(curitem, _(L"Model"));
			m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_BATTLE_SCENE, i, CHUNK_TYPE_BATTLE_SCENE));
			if (dataset->sceneset->scene[i]->modified) m_listtree->CheckItem(lowitem);
			if (dataset->sceneset->image[i] != NULL) {
				lowitem = m_listtree->AppendItem(curitem, _(L"Textures"));
				m_listtree->SetItemData(lowitem, new ModManagerClientData(3, DATA_SECTION_BATTLE_SCENE, i, CHUNK_TYPE_TIM));
				if (dataset->sceneset->image[i]->parent_chunk->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem, wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_MENU_UI]) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"Interface"));
		m_listtree->SetItemData(topitem, new ModManagerClientData(0, 6));
		curitem = m_listtree->AppendItem(topitem, _(L"UI Texts"));
		m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_MENU_UI));
		if (dataset->ffuiset->special_text->modified) m_listtree->CheckItem(curitem);
		if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->saveset.sectionloaded[DATA_SECTION_ASSEMBLY]) {
		if (cddata->gametype == GAME_TYPE_PSX) {
			topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"MIPS Code"));
			m_listtree->SetItemData(topitem, new ModManagerClientData(0, 7));
			curitem = m_listtree->AppendItem(topitem, _(L"Battle MIPS"));
			m_listtree->SetItemData(curitem, new ModManagerClientData(1, DATA_SECTION_MIPS));
			if (cddata->mipsset.battle_modified) m_listtree->CheckItem(curitem);
			if (!m_listtree->AreAllChildrenInState(topitem, wxCHK_UNCHECKED))
				m_listtree->CheckItem(topitem);
		} else {
			topitem = m_listtree->AppendItem(m_listtree->GetRootItem(), _(L"CIL Code"));
			m_listtree->SetItemData(topitem, new ModManagerClientData(0, 7));
			if (cddata->saveset.sectionmodified[DATA_SECTION_ASSEMBLY]) m_listtree->CheckItem(topitem);
		}
	}
	return ModManagerWindow::ShowModal();
}

void ToolModMarkModification(CDDataStruct* data, ModManagerClientData* info, bool check) {
	unsigned int i;
	if (info->type==0) {
		if (!check) {
			if (info->section==1)
				data->saveset.sectionmodified[DATA_SECTION_ENMY] = false;
			else if (info->section==2)
				data->saveset.sectionmodified[DATA_SECTION_TEXT] = false;
			else if (info->section==3)
				data->saveset.sectionmodified[DATA_SECTION_WORLD_MAP] = false;
			else if (info->section==4)
				data->saveset.sectionmodified[DATA_SECTION_FIELD] = false;
			else if (info->section==5)
				data->saveset.sectionmodified[DATA_SECTION_BATTLE_SCENE] = false;
			else if (info->section==6)
				data->saveset.sectionmodified[DATA_SECTION_MENU_UI] = false;
			else if (info->section==7) {
				if (GetGameType()==GAME_TYPE_PSX)
					data->saveset.sectionmodified[DATA_SECTION_ASSEMBLY] = false;
			}
		}
		if (info->section==7 && GetGameType()!=GAME_TYPE_PSX)
			data->saveset.sectionmodified[DATA_SECTION_ASSEMBLY] = check;
	} else if (info->type==1) {
		if (info->section==DATA_SECTION_SPELL) {
			data->saveset.sectionmodified[DATA_SECTION_SPELL] = check;
		} else if (info->section==DATA_SECTION_SUPPORT) {
			data->saveset.sectionmodified[DATA_SECTION_SUPPORT] = check;
		} else if (info->section==DATA_SECTION_CMD) {
			data->saveset.sectionmodified[DATA_SECTION_CMD] = check;
		} else if (info->section==DATA_SECTION_STAT) {
			data->saveset.sectionmodified[DATA_SECTION_STAT] = check;
		} else if (info->section==DATA_SECTION_PARTY_SPECIAL) {
			data->saveset.sectionmodified[DATA_SECTION_PARTY_SPECIAL] = check;
		} else if (info->section==DATA_SECTION_ITEM) {
			data->saveset.sectionmodified[DATA_SECTION_ITEM] = check;
		} else if (info->section==DATA_SECTION_SHOP) {
			data->saveset.sectionmodified[DATA_SECTION_SHOP] = check;
		} else if (info->section==DATA_SECTION_CARD) {
			data->saveset.sectionmodified[DATA_SECTION_CARD] = check;
		} else if (info->section==DATA_SECTION_SPELL_ANIM) {
			data->saveset.sectionmodified[DATA_SECTION_SPELL_ANIM] = check;
		} else if (info->section==DATA_SECTION_WORLD_MAP) {
			if (check) {
				data->worldset.world_data->MarkDataModified();
				data->saveset.sectionmodified[DATA_SECTION_WORLD_MAP] = true;
			} else {
				data->worldset.world_data->modified = false;
				data->worldset.world_data->parent_chunk->modified = false;
			}
		} else if (info->section==DATA_SECTION_MENU_UI) {
			data->ffuiset.special_text->modified = check;
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_MENU_UI] = true;
		} else if (info->section==DATA_SECTION_ASSEMBLY) {
			data->mipsset.battle_modified = check;
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_ASSEMBLY] = true;
		}
	} else if (info->type==2) {
		if (!check) {
			if (info->section==DATA_SECTION_ENMY) {
				data->enemyset.battle[info->index]->parent_cluster->modified = false;
			} else if (info->section==DATA_SECTION_TEXT) {
				if (data->textset.text_data[info->index]!=NULL)
					data->textset.text_data[info->index]->parent_cluster->modified = false;
				else
					data->textset.charmap[info->index]->parent_cluster->modified = false;
			} else if (info->section==DATA_SECTION_WORLD_MAP) {
				data->worldset.script[info->index]->parent_cluster->modified = false;
			} else if (info->section==DATA_SECTION_FIELD) {
				data->fieldset.script_data[info->index]->parent_cluster->modified = false;
			} else if (info->section==DATA_SECTION_BATTLE_SCENE) {
				data->sceneset.scene[info->index]->parent_cluster->modified = false;
			} else if (info->section==DATA_SECTION_SPELL_ANIM) {
				data->spellanimset.spell[info->index].modified_data = 0;
			}
		} else {
			if (info->section==DATA_SECTION_SPELL_ANIM) {
				data->spellanimset.spell[info->index].modified_data = SPELL_ANIMATION_DATA_TYPE_WHOLE;
			}
		}
	} else if (info->type==3) {
		if (info->section==DATA_SECTION_ENMY) {
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_ENMY] = true;
			if (info->chunk_type==CHUNK_TYPE_ENEMY_STATS) {
				if (check) {
					data->enemyset.battle[info->index]->MarkDataModified();
				} else {
					data->enemyset.battle[info->index]->modified = false;
					data->enemyset.battle[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_SCRIPT) {
				if (check) {
					data->enemyset.script[info->index]->MarkDataModified();
				} else {
					data->enemyset.script[info->index]->modified = false;
					data->enemyset.script[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_TEXT) {
				if (check) {
					data->enemyset.text[info->index]->MarkDataModified();
				} else {
					data->enemyset.text[info->index]->modified = false;
					data->enemyset.text[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_IMAGE_MAP) {
				if (check) {
					data->enemyset.preload[info->index]->MarkDataModified();
				} else {
					data->enemyset.preload[info->index]->modified = false;
					data->enemyset.preload[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_BATTLE_DATA) {
				if (check) {
					data->enemyset.battle_data[info->index]->MarkDataModified();
				} else {
					data->enemyset.battle_data[info->index]->modified = false;
					data->enemyset.battle_data[info->index]->parent_chunk->modified = false;
				}
			}
		} else if (info->section==DATA_SECTION_TEXT) {
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_TEXT] = true;
			if (info->chunk_type==CHUNK_TYPE_TEXT) {
				if (check) {
					data->textset.text_data[info->index]->MarkDataModified();
				} else {
					data->textset.text_data[info->index]->modified = false;
					data->textset.text_data[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_CHARMAP) {
				if (check) {
					data->textset.charmap[info->index]->MarkDataModified();
				} else {
					data->textset.charmap[info->index]->modified = false;
					data->textset.charmap[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_TIM) {
				for (i=0;i<data->textset.tim_amount[info->index];i++) {
					if (check) {
						data->textset.chartim[info->index][i].MarkDataModified();
					} else {
						data->textset.chartim[info->index][i].modified = false;
						data->textset.chartim[info->index][i].parent_chunk->modified = false;
					}
				}
			}
		} else if (info->section==DATA_SECTION_WORLD_MAP) {
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_WORLD_MAP] = true;
			if (info->chunk_type==CHUNK_TYPE_SCRIPT) {
				if (check) {
					data->worldset.script[info->index]->MarkDataModified();
				} else {
					data->worldset.script[info->index]->modified = false;
					data->worldset.script[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_TEXT) {
				if (check) {
					data->worldset.text_data[info->index]->MarkDataModified();
				} else {
					data->worldset.text_data[info->index]->modified = false;
					data->worldset.text_data[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_IMAGE_MAP) {
				if (check) {
					data->worldset.preload[info->index]->MarkDataModified();
				} else {
					data->worldset.preload[info->index]->modified = false;
					data->worldset.preload[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_CHARMAP) {
				if (check) {
					data->worldset.charmap[info->index]->MarkDataModified();
				} else {
					data->worldset.charmap[info->index]->modified = false;
					data->worldset.charmap[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_TIM) {
				for (i=0;i<data->worldset.tim_amount[info->index];i++) {
					if (check) {
						data->worldset.chartim[info->index][i].MarkDataModified();
					} else {
						data->worldset.chartim[info->index][i].modified = false;
						data->worldset.chartim[info->index][i].parent_chunk->modified = false;
					}
				}
			}
		} else if (info->section==DATA_SECTION_FIELD) {
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_FIELD] = true;
			if (info->chunk_type==CHUNK_TYPE_SCRIPT) {
				if (check) {
					data->fieldset.script_data[info->index]->MarkDataModified();
				} else {
					data->fieldset.script_data[info->index]->modified = false;
					data->fieldset.script_data[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_IMAGE_MAP) {
				if (check) {
					data->fieldset.preload[info->index]->MarkDataModified();
				} else {
					data->fieldset.preload[info->index]->modified = false;
					data->fieldset.preload[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_FIELD_ROLE) {
				if (check) {
					data->fieldset.role[info->index]->MarkDataModified();
				} else {
					data->fieldset.role[info->index]->modified = false;
					data->fieldset.role[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type == CHUNK_TYPE_FIELD_TILES) {
				if (check) {
					data->fieldset.background_data[info->index]->MarkDataModified();
				} else {
					data->fieldset.background_data[info->index]->modified = false;
					data->fieldset.background_data[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type == CHUNK_TYPE_FIELD_WALK) {
				if (check) {
					data->fieldset.walkmesh[info->index]->MarkDataModified();
				} else {
					data->fieldset.walkmesh[info->index]->modified = false;
					data->fieldset.walkmesh[info->index]->parent_chunk->modified = false;
				}
			}
		} else if (info->section==DATA_SECTION_BATTLE_SCENE) {
			if (check)
				data->saveset.sectionmodified[DATA_SECTION_BATTLE_SCENE] = true;
			if (info->chunk_type==CHUNK_TYPE_BATTLE_SCENE) {
				if (check) {
					data->sceneset.scene[info->index]->MarkDataModified();
				} else {
					data->sceneset.scene[info->index]->modified = false;
					data->sceneset.scene[info->index]->parent_chunk->modified = false;
				}
			} else if (info->chunk_type==CHUNK_TYPE_TIM) {
				if (check) {
					data->sceneset.image[info->index]->MarkDataModified();
				} else {
					data->sceneset.image[info->index]->modified = false;
					data->sceneset.image[info->index]->parent_chunk->modified = false;
				}
			}
		}
	}
}

void ToolModManager::OnTreeListCheck(wxTreeListEvent& event) {
	wxTreeListItem item = event.GetItem();
	wxCheckBoxState checkstate = m_listtree->GetCheckedState(item);
	ToolModMarkModification(cddata,(ModManagerClientData*)m_listtree->GetItemData(item),checkstate==wxCHK_CHECKED);
	for (wxTreeListItem child = m_listtree->GetFirstChild(item);child.IsOk();child = m_listtree->GetNextSibling(child)) {
		for (wxTreeListItem cchild = m_listtree->GetFirstChild(child);cchild.IsOk();cchild = m_listtree->GetNextSibling(cchild)) {
			m_listtree->CheckItem(cchild,checkstate);
			ToolModMarkModification(cddata,(ModManagerClientData*)m_listtree->GetItemData(cchild),checkstate==wxCHK_CHECKED);
		}
		m_listtree->CheckItem(child,checkstate);
		ToolModMarkModification(cddata,(ModManagerClientData*)m_listtree->GetItemData(child),checkstate==wxCHK_CHECKED);
	}
}

void ToolModManager::OnButtonClick(wxCommandEvent& event) {
	EndModal(event.GetId());
}
