#include "Gui_Tools.h"

#include <wx/tokenzr.h>
#include <algorithm>
#include "main.h"
#include "Gui_TextureEditor.h"
#include "Gui_LoadingDialog.h"
#include "Hades_Strings.h"
#include "Steam_Strings.h"

//=============================//
//         ModManager          //
//=============================//

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
	unsigned int i;
	cddata = data;
	wxTreeListItem topitem,curitem,lowitem;
	SaveSet* dataset = &cddata->saveset;
	topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"Generic"));
	m_listtree->SetItemData(topitem,new ModManagerClientData(0,0));
	if (cddata->spellloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Spells"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_SPELL));
		if (cddata->spellmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->supportloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Supporting Abilities"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_SUPPORT));
		if (cddata->supportmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->cmdloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Commands"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_CMD));
		if (cddata->cmdmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->statloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Stats"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_STAT));
		if (cddata->statmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->partyspecialloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Party - Special"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_PARTY_SPECIAL));
		if (cddata->partyspecialmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->itemloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Items"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_ITEM));
		if (cddata->itemmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->shoploaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Shops"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_SHOP));
		if (cddata->shopmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->cardloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Tetra Master"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_CARD));
		if (cddata->cardmodified) m_listtree->CheckItem(curitem);
	}
	if (cddata->spellanimloaded) {
		curitem = m_listtree->AppendItem(topitem,_(L"Spell Animations"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_SPELL_ANIM));
		if (cddata->spellanimmodified) m_listtree->CheckItem(curitem);
	}
	if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
		m_listtree->CheckItem(topitem);
	if (cddata->enemyloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"Enemies"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,1));
		for (i=0;i<dataset->enemyset->battle_amount;i++) {
			curitem = m_listtree->AppendItem(topitem,_(dataset->enemyset->battle_name[i]));
			m_listtree->SetItemData(curitem,new ModManagerClientData(2,DATA_SECTION_ENMY,i));
			lowitem = m_listtree->AppendItem(curitem,_(L"Enemy Data"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_ENMY,i,CHUNK_TYPE_ENEMY_STATS));
			if (dataset->enemyset->battle[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem,_(L"Script"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_ENMY,i,CHUNK_TYPE_SCRIPT));
			if (dataset->enemyset->script[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem,_(L"Texts"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_ENMY,i,CHUNK_TYPE_TEXT));
			if (dataset->enemyset->text[i]->modified) m_listtree->CheckItem(lowitem);
			if (GetGameType()==GAME_TYPE_PSX) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Preloading"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_ENMY,i,CHUNK_TYPE_IMAGE_MAP));
				if (dataset->enemyset->preload[i]->modified) m_listtree->CheckItem(lowitem);
			}
			lowitem = m_listtree->AppendItem(curitem,_(L"Animations & Cameras"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_ENMY,i,CHUNK_TYPE_BATTLE_DATA));
			if (dataset->enemyset->battle_data[i]->modified) m_listtree->CheckItem(lowitem);
			if (!m_listtree->AreAllChildrenInState(curitem,wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->textloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"Texts"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,2));
		for (i=0;i<dataset->textset->amount;i++) {
			curitem = m_listtree->AppendItem(topitem,_(dataset->textset->name[i]));
			m_listtree->SetItemData(curitem,new ModManagerClientData(2,DATA_SECTION_TEXT,i));
			if (dataset->textset->text_data[i]!=NULL) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Texts"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_TEXT,i,CHUNK_TYPE_TEXT));
				if (dataset->textset->text_data[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType()==GAME_TYPE_PSX && dataset->textset->charmap[i]!=NULL) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Charmap"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_TEXT,i,CHUNK_TYPE_CHARMAP));
				if (dataset->textset->charmap[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType()==GAME_TYPE_PSX && dataset->textset->chartim[i]!=NULL) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Texture"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_TEXT,i,CHUNK_TYPE_TIM));
				if (dataset->textset->chartim[i]->parent_chunk->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem,wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->worldloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"World Maps"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,3));
		curitem = m_listtree->AppendItem(topitem,_(L"Common Data"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_WORLD_MAP));
		if (dataset->worldset->world_data->modified) m_listtree->CheckItem(curitem);
		for (i=0;i<dataset->worldset->amount;i++) {
			curitem = m_listtree->AppendItem(topitem,_(dataset->worldset->name[i]));
			m_listtree->SetItemData(curitem,new ModManagerClientData(2,DATA_SECTION_WORLD_MAP,i));
			lowitem = m_listtree->AppendItem(curitem,_(L"Script"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_WORLD_MAP,i,CHUNK_TYPE_SCRIPT));
			if (dataset->worldset->script[i]->modified) m_listtree->CheckItem(lowitem);
			if (GetGameType()==GAME_TYPE_PSX || i==0) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Texts"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_WORLD_MAP,i,CHUNK_TYPE_TEXT));
				if (dataset->worldset->text_data[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType()==GAME_TYPE_PSX) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Preloading"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_WORLD_MAP,i,CHUNK_TYPE_IMAGE_MAP));
				if (dataset->worldset->preload[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType()==GAME_TYPE_PSX && dataset->worldset->charmap[i]!=NULL) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Charmap"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_WORLD_MAP,i,CHUNK_TYPE_CHARMAP));
				if (dataset->worldset->charmap[i]->modified) m_listtree->CheckItem(lowitem);
			}
			if (GetGameType()==GAME_TYPE_PSX && dataset->worldset->chartim[i]!=NULL) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Texture"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_WORLD_MAP,i,CHUNK_TYPE_TIM));
				if (dataset->worldset->chartim[i]->parent_chunk->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem,wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->fieldloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"Fields"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,4));
		for (i=0;i<dataset->fieldset->amount;i++) {
			curitem = m_listtree->AppendItem(topitem,_(dataset->fieldset->script_data[i]->name.str_nice));
			m_listtree->SetItemData(curitem,new ModManagerClientData(2,DATA_SECTION_FIELD,i));
			lowitem = m_listtree->AppendItem(curitem,_(L"Script"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_FIELD,i,CHUNK_TYPE_SCRIPT));
			if (dataset->fieldset->script_data[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem,_(L"Preloading"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_FIELD,i,CHUNK_TYPE_IMAGE_MAP));
			if (dataset->fieldset->preload[i]->modified) m_listtree->CheckItem(lowitem);
			lowitem = m_listtree->AppendItem(curitem,_(L"Model Role"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_FIELD,i,CHUNK_TYPE_FIELD_ROLE));
			if (dataset->fieldset->role[i]->modified) m_listtree->CheckItem(lowitem);
			// ToDo : add textures and backgrounds
			if (!m_listtree->AreAllChildrenInState(curitem,wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->sceneloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"Battle Scenes"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,5));
		for (i=0;i<dataset->sceneset->scene_amount;i++) {
			curitem = m_listtree->AppendItem(topitem,_(dataset->sceneset->scene_name[i]));
			m_listtree->SetItemData(curitem,new ModManagerClientData(2,DATA_SECTION_BATTLE_SCENE,i));
			lowitem = m_listtree->AppendItem(curitem,_(L"Model"));
			m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_BATTLE_SCENE,i,CHUNK_TYPE_BATTLE_SCENE));
			if (dataset->sceneset->scene[i]->modified) m_listtree->CheckItem(lowitem);
			if (dataset->sceneset->image[i]!=NULL) {
				lowitem = m_listtree->AppendItem(curitem,_(L"Textures"));
				m_listtree->SetItemData(lowitem,new ModManagerClientData(3,DATA_SECTION_BATTLE_SCENE,i,CHUNK_TYPE_TIM));
				if (dataset->sceneset->image[i]->parent_chunk->modified) m_listtree->CheckItem(lowitem);
			}
			if (!m_listtree->AreAllChildrenInState(curitem,wxCHK_UNCHECKED))
				m_listtree->CheckItem(curitem);
		}
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->ffuiloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"Interface"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,6));
		curitem = m_listtree->AppendItem(topitem,_(L"UI Texts"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_MENU_UI));
		if (dataset->ffuiset->special_text->modified) m_listtree->CheckItem(curitem);
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->mipsloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"MIPS Code"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,7));
		curitem = m_listtree->AppendItem(topitem,_(L"Battle MIPS"));
		m_listtree->SetItemData(curitem,new ModManagerClientData(1,DATA_SECTION_MIPS));
		if (cddata->mipsset.battle_modified) m_listtree->CheckItem(curitem);
		if (!m_listtree->AreAllChildrenInState(topitem,wxCHK_UNCHECKED))
			m_listtree->CheckItem(topitem);
	}
	if (cddata->cilloaded) {
		topitem = m_listtree->AppendItem(m_listtree->GetRootItem(),_(L"CIL Code"));
		m_listtree->SetItemData(topitem,new ModManagerClientData(0,7));
		if (cddata->cilmodified) m_listtree->CheckItem(topitem);
	}
	return ModManagerWindow::ShowModal();
}

void ToolModMarkModification(CDDataStruct* data, ModManagerClientData* info, bool check) {
	unsigned int i;
	if (info->type==0) {
		if (!check) {
			if (info->section==1)
				data->enemymodified = false;
			else if (info->section==2)
				data->textmodified = false;
			else if (info->section==3)
				data->worldmodified = false;
			else if (info->section==4)
				data->fieldmodified = false;
			else if (info->section==5)
				data->scenemodified = false;
			else if (info->section==6)
				data->ffuimodified = false;
			else if (info->section==7) {
				if (GetGameType()==GAME_TYPE_PSX)
					data->mipsmodified = false;
			}
		}
		if (info->section==7 && GetGameType()!=GAME_TYPE_PSX)
			data->cilmodified = check;
	} else if (info->type==1) {
		if (info->section==DATA_SECTION_SPELL) {
			data->spellmodified = check;
		} else if (info->section==DATA_SECTION_SUPPORT) {
			data->supportmodified = check;
		} else if (info->section==DATA_SECTION_CMD) {
			data->cmdmodified = check;
		} else if (info->section==DATA_SECTION_STAT) {
			data->statmodified = check;
		} else if (info->section==DATA_SECTION_PARTY_SPECIAL) {
			data->partyspecialmodified = check;
		} else if (info->section==DATA_SECTION_ITEM) {
			data->itemmodified = check;
		} else if (info->section==DATA_SECTION_SHOP) {
			data->shopmodified = check;
		} else if (info->section==DATA_SECTION_CARD) {
			data->cardmodified = check;
		} else if (info->section==DATA_SECTION_SPELL_ANIM) {
			data->spellanimmodified = check;
			for (i=0;i<data->spellanimset.amount;i++) {
				if (!data->spellanimset.spell[i].is_empty)
					data->spellanimset.spell[i].modified_data = SPELL_ANIMATION_DATA_TYPE_WHOLE;
			}
		} else if (info->section==DATA_SECTION_WORLD_MAP) {
			if (check) {
				data->worldset.world_data->MarkDataModified();
				data->worldmodified = true;
			} else {
				data->worldset.world_data->modified = false;
				data->worldset.world_data->parent_chunk->modified = false;
			}
		} else if (info->section==DATA_SECTION_MENU_UI) {
			data->ffuiset.special_text->modified = check;
			if (check)
				data->ffuimodified = true;
		} else if (info->section==DATA_SECTION_ASSEMBLY) {
			data->mipsset.battle_modified = check;
			if (check)
				data->mipsmodified = true;
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
			}
		}
	} else if (info->type==3) {
		if (info->section==DATA_SECTION_ENMY) {
			if (check)
				data->enemymodified = true;
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
				data->textmodified = true;
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
				data->worldmodified = true;
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
				data->fieldmodified = true;
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
			}
		} else if (info->section==DATA_SECTION_BATTLE_SCENE) {
			if (check)
				data->scenemodified = true;
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

//=============================//
//    Background Converter     //
//=============================//

#define CONVERTER_MEMORY_LIMIT	100000000L

const static wxColor TILECOLOR_BOUNDARY = wxColor(0,0,0,255);
const static wxColor TILECOLOR_INTERIOR = wxColor(80,80,80,50);

wxString GetFieldNameOrDefault(CDDataStruct* cddata, unsigned int fieldindex) {
	if (cddata && cddata->fieldloaded)
		return _(cddata->fieldset.script_data[fieldindex]->name.GetStr(hades::TEXT_PREVIEW_TYPE));
	return _(SteamFieldScript[fieldindex].default_name);
}

ToolBackgroundEditor::ToolBackgroundEditor(wxWindow* p) :
	BackgroundEditorWindow(p) {
	
}

ToolBackgroundEditor::~ToolBackgroundEditor() {
}

int ToolBackgroundEditor::ShowModal(CDDataStruct* data) {
	unsigned int i;
	cddata = data;
	m_fieldchoice->Clear();
	if (cddata && cddata->fieldloaded) {
		for (i=0;i<cddata->fieldset.amount;i++)
			m_fieldchoice->Append(cddata->fieldset.script_data[i]->name.GetStr(hades::TEXT_PREVIEW_TYPE));
	} else {
		for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++)
			m_fieldchoice->Append(_(SteamFieldScript[i].default_name));
		m_usegametilebtn->Enable(false);
	}
	return BackgroundEditorWindow::ShowModal();
}

// imgfilename order must be the one of tiles
// depthorder must be so that imgfilename[depthorder[i]] is of increasing depth for i<tile_amount
// For i>=tile_amount, imgfilename[i] is the layer of a non-US title tile
int CreateBackgroundImage(wxString* imgfilename, wxString outputname, FieldTilesDataStruct& tiledata, unsigned int* depthorder, int dxtflags, wxBitmapType type = wxBITMAP_TYPE_ANY) {
	// Compute size of the atlas in order to have a roughly square image
	unsigned int atlassize, atlasw, atlash, atlastilecolcount, atlastilerowcount, tileamount, tilesize, tilegap, tileperiod;
	unsigned int tilesamountplustitle = tiledata.tiles_amount+tiledata.title_tile_amount*(STEAM_LANGUAGE_AMOUNT-1);
	unsigned int i,j,k,x,y,tilei;
	int imgindex;
	int res = 0;
	wxFile atlasout(outputname,wxFile::write);
	if (!atlasout.IsOpened())
		return 1;
	tilesize = tiledata.parent->tile_size;
	tilegap = tiledata.parent->tile_gap;
	tileperiod = tilesize+2*tilegap;
	tileamount = 0;
	for (i=0;i<tiledata.tiles_amount;i++)
		tileamount += tiledata.tiles[i].tile_amount;
	for (i=tiledata.tiles_amount+tiledata.title_tile_amount;i<tiledata.tiles_amount+tiledata.title_tile_amount*STEAM_LANGUAGE_AMOUNT;i++)
		tileamount += tiledata.tiles[i].tile_amount;
	atlasw = sqrt(tileamount);
	atlasw = max(atlasw,1u);
	atlash = tileamount/atlasw;
	atlash = (atlash*atlasw<tileamount) ? atlash+1 : atlash;
	atlastilecolcount = atlasw;
	atlastilerowcount = atlash;
	atlasw *= tileperiod;
	atlash *= tileperiod;
	while (atlasw%4) atlasw++; // For DXT5 compression, it is better to have sizes multiple of 4
	while (atlash%4) atlash++;
	atlassize = atlasw*atlash*4;
	// Load the image files
	wxImage* tblockimgarray = new wxImage[tilesamountplustitle];
	unsigned int tiledefaultsize = GetGameType()==GAME_TYPE_PSX ? FIELD_TILE_BASE_SIZE : FIELD_TILE_BASE_SIZE*2;
	unsigned int expectmemoryusage = 0;
	bool loadallimage = true; // If the images are too big, we don't load them all in the RAM simultaneously
	for (i=0;i<tilesamountplustitle;i++) {
		if (wxFile::Exists(imgfilename[i])) {
			tblockimgarray[i].LoadFile(imgfilename[i],type);
			tilei = i<tiledata.tiles_amount ? i : i+tiledata.title_tile_amount;
			FieldTilesCameraDataStruct& camera = tiledata.camera[tiledata.tiles[tilei].camera_id];
			if (camera.width/tiledefaultsize*tilesize>tblockimgarray[i].GetWidth() || camera.height/tiledefaultsize*tilesize>tblockimgarray[i].GetHeight()) {
				delete[] tblockimgarray;
				remove(outputname.c_str());
				return 2;
			}
			expectmemoryusage += (tblockimgarray[i].GetWidth()*tblockimgarray[i].GetHeight())*4;
			if (expectmemoryusage>=CONVERTER_MEMORY_LIMIT && loadallimage) {
				loadallimage = false;
				for (j=0;j<i;j++)
					tblockimgarray[j].Destroy();
			}
			if (!loadallimage)
				tblockimgarray[i].Destroy();
		} else {
			tblockimgarray[i].Destroy();
			res--;
		}
	}
	// If we avoid loading all the images, at least keep the alpha informations
/*	unsigned int* allimgwidth, *allimgheight;
	uint8_t** allimgalpha;
	if (!loadallimage) {
		allimgalpha = new uint8_t*[tilesamountplustitle];
		allimgwidth = new unsigned int[tilesamountplustitle];
		allimgheight = new unsigned int[tilesamountplustitle];
		for (i=0;i<tilesamountplustitle;i++)
			if (wxFile::Exists(imgfilename[i])) {
				tblockimgarray[i].LoadFile(imgfilename[i],type);
				allimgwidth[i] = tblockimgarray[i].GetWidth();
				allimgheight[i] = tblockimgarray[i].GetHeight();
				uint8_t* tmpalpha = tblockimgarray[i].GetAlpha();
				allimgalpha[i] = new uint8_t[tblockimgarray[i].GetWidth()*tblockimgarray[i].GetHeight()];
				memcpy(allimgalpha[i],tmpalpha,tblockimgarray[i].GetWidth()*tblockimgarray[i].GetHeight());
				tblockimgarray[i].Destroy();
			} else {
				allimgalpha[i] = NULL;
				allimgwidth[i] = 0;
				allimgheight[i] = 0;
			}
	}*/
	// Add each tile to the atlas in the order
	unsigned int imgtilex, imgtiley, imgwidth, imgheight, imgtilei;
	unsigned int atlasx, atlasy, atlasi;
	uint8_t* atlas = new uint8_t[atlassize]{0};
	// DEBUG: the transparent and half-transparent border of the pictures are hard to handle...
	// Several methods were tested to know what RGB values should be used when Alpha < 0xFF
	// It seems that (1) Alpha < 0xFF should be made fully transparent (relicas of upscaling)
	// and (2) we should use RGB value of the pixels of the other layers
	// Note that Steam default files use a kind of X-Y expansion of RGB instead of (2)
	uint8_t alphalimit;
	for (i=0;i<tilesamountplustitle;i++) {
		tilei = i<tiledata.tiles_amount ? i : i+tiledata.title_tile_amount;
		FieldTilesTileDataStruct& tile = tiledata.tiles[tilei];
		FieldTilesCameraDataStruct& camera = tiledata.camera[tile.camera_id];
		if (!loadallimage)
			tblockimgarray[i].LoadFile(imgfilename[i],type);
		wxImage& tblockimg = tblockimgarray[i];
		if (imgindex<0 || !tblockimg.IsOk())
			continue;
		uint8_t* imgdata = tblockimg.GetData();
		uint8_t* imgalpha = tblockimg.GetAlpha();
		imgwidth = tblockimg.GetWidth();
		imgheight = tblockimg.GetHeight();
		for (j=0;j<tile.tile_amount;j++) {
			switch (tile.tile_abr[j]) {
				case 0: alphalimit = tile.tile_trans[j] ? 0x7E : 0xFF; break;
				case 1: alphalimit = tile.tile_trans[j] ? 0x7E : 0xFF; break;
				case 2: alphalimit = 0x18; break;
				case 3: alphalimit = 0x7E; break;
			}
			imgtilex = (tile.pos_x+tile.tile_pos_x[j]-camera.pos_x)/FIELD_TILE_BASE_SIZE*tilesize;
			imgtiley = (tile.pos_y+tile.tile_pos_y[j]-camera.pos_y)/FIELD_TILE_BASE_SIZE*tilesize;
			atlasx = (tile.tile_steam_id[j]%atlastilecolcount)*tileperiod+tilegap;
			atlasy = (tile.tile_steam_id[j]/atlastilecolcount)*tileperiod+tilegap;
			// interior of the tile
			for (y=0;y<tilesize;y++)
				for (x=0;x<tilesize;x++) {
					atlasi = (atlasx+x+(atlasy+y)*atlasw)*4;
					imgtilei = imgtilex+x+(imgtiley+y)*imgwidth;
					atlas[atlasi] = imgdata[imgtilei*3];
					atlas[atlasi+1] = imgdata[imgtilei*3+1];
					atlas[atlasi+2] = imgdata[imgtilei*3+2];
					atlas[atlasi+3] = imgalpha[imgtilei];
					if (atlas[atlasi+3]<alphalimit) {
//						if (alphalimit==0xFF)
//							for (k=0;k<tiledata.tiles_amount;k++) {
//								if (depthorder[k]==i)
//									continue;
//								if (loadallimage) { // Loading all the images at once makes this step faster
//									if (!tblockimgarray[depthorder[k]].IsOk() || /*
//									*/ imgtilex+x>=tblockimgarray[depthorder[k]].GetWidth() || imgtiley+y>=tblockimgarray[depthorder[k]].GetHeight() || /*
//									*/ tblockimgarray[depthorder[k]].GetAlpha(imgtilex+x,imgtiley+y)<0xFF || /*
//									*/ tile.camera_id!=tiledata.tiles[depthorder[k]].camera_id)
//										continue;
//								} else {
//									if (allimgalpha[depthorder[k]]==NULL || /*
//									*/ imgtilex+x>=allimgwidth[depthorder[k]] || imgtiley+y>=allimgheight[depthorder[k]] || /*
//									*/ allimgalpha[depthorder[k]][imgtilex+x+(imgtiley+y)*allimgwidth[depthorder[k]]]<0xFF || /*
//									*/ tile.camera_id!=tiledata.tiles[depthorder[k]].camera_id)
//										continue;
//									tblockimgarray[depthorder[k]].LoadFile(imgfilename[depthorder[k]],type);
//								}
//								atlas[atlasi] = tblockimgarray[depthorder[k]].GetRed(imgtilex+x,imgtiley+y);
//								atlas[atlasi+1] = tblockimgarray[depthorder[k]].GetGreen(imgtilex+x,imgtiley+y);
//								atlas[atlasi+2] = tblockimgarray[depthorder[k]].GetBlue(imgtilex+x,imgtiley+y);
//								if (!loadallimage)
//									tblockimgarray[depthorder[k]].Destroy();
//								break;
//							}
					} else if (alphalimit<0xFF && atlas[atlasi+3]>0) {
						// Convert pre-multiplied alpha colors to real colors
						atlas[atlasi] = atlas[atlasi]*0xFF/atlas[atlasi+3];
						atlas[atlasi+1] = atlas[atlasi+1]*0xFF/atlas[atlasi+3];
						atlas[atlasi+2] = atlas[atlasi+2]*0xFF/atlas[atlasi+3];
						atlas[atlasi+3] = 0xFF;
					}
				}
			// vertical and horizontal borders of the tile : (2*bordercolor_interior + bordercolor_exterior)/3
			for (y=0;y<tilesize;y++)
				for (x=0;x<tilegap;x++) {
					atlasi = (atlasx-x-1+(atlasy+y)*atlasw)*4;
					if (/*false && */imgtilex>0 && imgalpha[imgtilex-1+(imgtiley+y)*imgwidth]>0) {
						atlas[atlasi] = (2*imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3]+imgdata[(imgtilex-1+(imgtiley+y)*imgwidth)*3])/3;
						atlas[atlasi+1] = (2*imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3+1]+imgdata[(imgtilex-1+(imgtiley+y)*imgwidth)*3+1])/3;
						atlas[atlasi+2] = (2*imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3+2]+imgdata[(imgtilex-1+(imgtiley+y)*imgwidth)*3+2])/3;
						atlas[atlasi+3] = max(imgalpha[imgtilex+(imgtiley+y)*imgwidth],imgalpha[imgtilex-1+(imgtiley+y)*imgwidth]);
					} else {
						atlas[atlasi] = imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3];
						atlas[atlasi+1] = imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3+1];
						atlas[atlasi+2] = imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3+2];
						atlas[atlasi+3] = imgalpha[imgtilex+(imgtiley+y)*imgwidth];
					}
					if (alphalimit<0xFF && atlas[atlasi+3]>=alphalimit) {
						atlas[atlasi] = atlas[atlasi]*0xFF/atlas[atlasi+3];
						atlas[atlasi+1] = atlas[atlasi+1]*0xFF/atlas[atlasi+3];
						atlas[atlasi+2] = atlas[atlasi+2]*0xFF/atlas[atlasi+3];
						atlas[atlasi+3] = 0xFF;
					}
					atlasi = (atlasx+tilesize+x+(atlasy+y)*atlasw)*4;
					if (/*false && */imgtilex+1<imgwidth && imgalpha[imgtilex+tilesize+(imgtiley+y)*imgwidth]>0) {
						atlas[atlasi] = (2*imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3]+imgdata[(imgtilex+tilesize+(imgtiley+y)*imgwidth)*3])/3;
						atlas[atlasi+1] = (2*imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3+1]+imgdata[(imgtilex+tilesize+(imgtiley+y)*imgwidth)*3+1])/3;
						atlas[atlasi+2] = (2*imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3+2]+imgdata[(imgtilex+tilesize+(imgtiley+y)*imgwidth)*3+2])/3;
						atlas[atlasi+3] = max(imgalpha[imgtilex+tilesize-1+(imgtiley+y)*imgwidth],imgalpha[imgtilex+tilesize+(imgtiley+y)*imgwidth]);
					} else {
						atlas[atlasi] = imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3];
						atlas[atlasi+1] = imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3+1];
						atlas[atlasi+2] = imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3+2];
						atlas[atlasi+3] = imgalpha[imgtilex+tilesize-1+(imgtiley+y)*imgwidth];
					}
					if (alphalimit<0xFF && atlas[atlasi+3]>=alphalimit) {
						atlas[atlasi] = atlas[atlasi]*0xFF/atlas[atlasi+3];
						atlas[atlasi+1] = atlas[atlasi+1]*0xFF/atlas[atlasi+3];
						atlas[atlasi+2] = atlas[atlasi+2]*0xFF/atlas[atlasi+3];
						atlas[atlasi+3] = 0xFF;
					}
				}
			for (x=0;x<tilesize;x++)
				for (y=0;y<tilegap;y++) {
					atlasi = (atlasx+x+(atlasy-y-1)*atlasw)*4;
					if (/*false && */imgtiley>0 && imgalpha[imgtilex+x+(imgtiley-1)*imgwidth]>0) {
						atlas[atlasi] = (2*imgdata[(imgtilex+x+imgtiley*imgwidth)*3]+imgdata[(imgtilex+x+(imgtiley-1)*imgwidth)*3])/3;
						atlas[atlasi+1] = (2*imgdata[(imgtilex+x+imgtiley*imgwidth)*3+1]+imgdata[(imgtilex+x+(imgtiley-1)*imgwidth)*3+1])/3;
						atlas[atlasi+2] = (2*imgdata[(imgtilex+x+imgtiley*imgwidth)*3+2]+imgdata[(imgtilex+x+(imgtiley-1)*imgwidth)*3+2])/3;
						atlas[atlasi+3] = max(imgalpha[imgtilex+x+imgtiley*imgwidth],imgalpha[imgtilex+x+(imgtiley-1)*imgwidth]);
					} else {
						atlas[atlasi] = imgdata[(imgtilex+x+imgtiley*imgwidth)*3];
						atlas[atlasi+1] = imgdata[(imgtilex+x+imgtiley*imgwidth)*3+1];
						atlas[atlasi+2] = imgdata[(imgtilex+x+imgtiley*imgwidth)*3+2];
						atlas[atlasi+3] = imgalpha[imgtilex+x+imgtiley*imgwidth];
					}
					if (alphalimit<0xFF && atlas[atlasi+3]>=alphalimit) {
						atlas[atlasi] = atlas[atlasi]*0xFF/atlas[atlasi+3];
						atlas[atlasi+1] = atlas[atlasi+1]*0xFF/atlas[atlasi+3];
						atlas[atlasi+2] = atlas[atlasi+2]*0xFF/atlas[atlasi+3];
						atlas[atlasi+3] = 0xFF;
					}
					atlasi = (atlasx+x+(atlasy+tilesize+y)*atlasw)*4;
					if (/*false && */imgtiley+1<imgheight && imgalpha[imgtilex+x+(imgtiley+tilesize)*imgwidth]>0) {
						atlas[atlasi] = (2*imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3]+imgdata[(imgtilex+x+(imgtiley+tilesize)*imgwidth)*3])/3;
						atlas[atlasi+1] = (2*imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3+1]+imgdata[(imgtilex+x+(imgtiley+tilesize)*imgwidth)*3+1])/3;
						atlas[atlasi+2] = (2*imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3+2]+imgdata[(imgtilex+x+(imgtiley+tilesize)*imgwidth)*3+2])/3;
						atlas[atlasi+3] = max(imgalpha[imgtilex+x+(imgtiley+tilesize-1)*imgwidth],imgalpha[imgtilex+x+(imgtiley+tilesize)*imgwidth]);
					} else {
						atlas[atlasi] = imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3];
						atlas[atlasi+1] = imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3+1];
						atlas[atlasi+2] = imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3+2];
						atlas[atlasi+3] = imgalpha[imgtilex+x+(imgtiley+tilesize-1)*imgwidth];
					}
					if (alphalimit<0xFF && atlas[atlasi+3]>=alphalimit) {
						atlas[atlasi] = atlas[atlasi]*0xFF/atlas[atlasi+3];
						atlas[atlasi+1] = atlas[atlasi+1]*0xFF/atlas[atlasi+3];
						atlas[atlasi+2] = atlas[atlasi+2]*0xFF/atlas[atlasi+3];
						atlas[atlasi+3] = 0xFF;
					}
				}
			// corners of the tile : (bordercolor_exterior1 + bordercolor_exterior2)/2 ; only the nearest pixel is non-transparent
			for (x=0;x<tilegap;x++)
				for (y=0;y<tilegap;y++) {
					atlasi = (atlasx-x-1+(atlasy-y-1)*atlasw)*4;
					atlas[atlasi] = (atlas[(atlasx-1+atlasy*atlasw)*4]+atlas[(atlasx+(atlasy-1)*atlasw)*4])/2;
					atlas[atlasi+1] = (atlas[(atlasx-1+atlasy*atlasw)*4+1]+atlas[(atlasx+(atlasy-1)*atlasw)*4+1])/2;
					atlas[atlasi+2] = (atlas[(atlasx-1+atlasy*atlasw)*4+2]+atlas[(atlasx+(atlasy-1)*atlasw)*4+2])/2;
					atlasi = (atlasx-x-1+(atlasy+tilesize+y)*atlasw)*4;
					atlas[atlasi] = (atlas[(atlasx-1+(atlasy+tilesize-1)*atlasw)*4]+atlas[(atlasx+(atlasy+tilesize)*atlasw)*4])/2;
					atlas[atlasi+1] = (atlas[(atlasx-1+(atlasy+tilesize-1)*atlasw)*4+1]+atlas[(atlasx+(atlasy+tilesize)*atlasw)*4+1])/2;
					atlas[atlasi+2] = (atlas[(atlasx-1+(atlasy+tilesize-1)*atlasw)*4+2]+atlas[(atlasx+(atlasy+tilesize)*atlasw)*4+2])/2;
					atlasi = (atlasx+tilesize+x+(atlasy-y-1)*atlasw)*4;
					atlas[atlasi] = (atlas[(atlasx+tilesize+atlasy*atlasw)*4]+atlas[(atlasx+tilesize-1+(atlasy-1)*atlasw)*4])/2;
					atlas[atlasi+1] = (atlas[(atlasx+tilesize+atlasy*atlasw)*4+1]+atlas[(atlasx+tilesize-1+(atlasy-1)*atlasw)*4+1])/2;
					atlas[atlasi+2] = (atlas[(atlasx+tilesize+atlasy*atlasw)*4+2]+atlas[(atlasx+tilesize-1+(atlasy-1)*atlasw)*4+2])/2;
					atlasi = (atlasx+tilesize+x+(atlasy+tilesize+y)*atlasw)*4;
					atlas[atlasi] = (atlas[(atlasx+tilesize+(atlasy+tilesize-1)*atlasw)*4]+atlas[(atlasx+tilesize-1+(atlasy+tilesize)*atlasw)*4])/2;
					atlas[atlasi+1] = (atlas[(atlasx+tilesize+(atlasy+tilesize-1)*atlasw)*4+1]+atlas[(atlasx+tilesize-1+(atlasy+tilesize)*atlasw)*4+1])/2;
					atlas[atlasi+2] = (atlas[(atlasx+tilesize+(atlasy+tilesize-1)*atlasw)*4+2]+atlas[(atlasx+tilesize-1+(atlasy+tilesize)*atlasw)*4+2])/2;
				}
			if (tilegap>0) {
				atlas[(atlasx-1+(atlasy-1)*atlasw)*4+3] = max(atlas[(atlasx-1+atlasy*atlasw)*4+3],atlas[(atlasx+(atlasy-1)*atlasw)*4+3]);
				atlas[(atlasx-1+(atlasy+tilesize)*atlasw)*4+3] = max(atlas[(atlasx-1+(atlasy+tilesize-1)*atlasw)*4+3],atlas[(atlasx+(atlasy+tilesize)*atlasw)*4+3]);
				atlas[(atlasx+tilesize+(atlasy-1)*atlasw)*4+3] = max(atlas[(atlasx+tilesize+atlasy*atlasw)*4+3],atlas[(atlasx+tilesize-1+(atlasy-1)*atlasw)*4+3]);
				atlas[(atlasx+tilesize+(atlasy+tilesize)*atlasw)*4+3] = max(atlas[(atlasx+tilesize+(atlasy+tilesize-1)*atlasw)*4+3],atlas[(atlasx+tilesize-1+(atlasy+tilesize)*atlasw)*4+3]);
			}
		}
	}
	unsigned int visiblepixcount;
	uint32_t r,g,b;
	int dx,dy;
	for (y=0;y<atlash;y++)
		for (x=0;x<atlasw;x++)
			if (atlas[(x+y*atlasw)*4+3]<0xFF) {
				visiblepixcount = 0;
				r = 0; g = 0; b = 0;
				for (dx=-3;dx<=3;dx++)
					for (dy=-3;dy<=3;dy++)
						if (x+dx>0 && x+dx<atlasw && y+dy>0 && y+dy<atlash && (x+dx)/tileperiod==x/tileperiod && (y+dy)/tileperiod==y/tileperiod && atlas[(x+dx+(y+dy)*atlasw)*4+3]==0xFF) {
							r += atlas[(x+dx+(y+dy)*atlasw)*4];
							g += atlas[(x+dx+(y+dy)*atlasw)*4+1];
							b += atlas[(x+dx+(y+dy)*atlasw)*4+2];
							visiblepixcount++;
						}
				if (visiblepixcount>0) {
					atlas[(x+y*atlasw)*4] = r/visiblepixcount;
					atlas[(x+y*atlasw)*4+1] = g/visiblepixcount;
					atlas[(x+y*atlasw)*4+2] = b/visiblepixcount;
//					atlas[(x+y*atlasw)*4+3] = 0;
				}
			}
/*	uint32_t r,g,b;
	unsigned int visiblepixcount;
	for (i=0;i<atlastilecolcount;i++)
		for (j=0;j<atlastilerowcount;j++) {
			atlasx = i*tileperiod;
			atlasy = j*tileperiod;
			visiblepixcount = 0;
			r = 0; g = 0; b = 0;
			for (x=0;x<tileperiod;x++)
				for (y=0;y<tileperiod;y++)
					if (atlas[(atlasx+x+(atlasy+y)*atlasw)*4+3]==0xFF) {
						r += atlas[(atlasx+x+(atlasy+y)*atlasw)*4];
						g += atlas[(atlasx+x+(atlasy+y)*atlasw)*4+1];
						b += atlas[(atlasx+x+(atlasy+y)*atlasw)*4+2];
						visiblepixcount++;
					}
			if (visiblepixcount>0) {
				r /= visiblepixcount;
				g /= visiblepixcount;
				b /= visiblepixcount;
				for (x=0;x<tileperiod;x++)
					for (y=0;y<tileperiod;y++)
						if (atlas[(atlasx+x+(atlasy+y)*atlasw)*4+3]<0xFF) {
							atlas[(atlasx+x+(atlasy+y)*atlasw)*4] = r;
							atlas[(atlasx+x+(atlasy+y)*atlasw)*4+1] = g;
							atlas[(atlasx+x+(atlasy+y)*atlasw)*4+2] = b;
							atlas[(atlasx+x+(atlasy+y)*atlasw)*4+3] = atlas[(atlasx+x+(atlasy+y)*atlasw)*4+3]<0x40 ? 0 : 0xFF;
						}
			}
		}*/
/*	for (i=0;i<5;i++)
	for (y=0;y<atlash;y++)
		for (x=0;x<atlasw;x++)
			if (atlas[(x+y*atlasw)*4+3]<0xFF) {
				if (y>0 && (atlas[(x+(y-1)*atlasw)*4+3]==0xFF || atlas[(x+(y-1)*atlasw)*4]>0)) {
					atlas[(x+y*atlasw)*4] = atlas[(x+(y-1)*atlasw)*4];
					atlas[(x+y*atlasw)*4+1] = atlas[(x+(y-1)*atlasw)*4+1];
					atlas[(x+y*atlasw)*4+2] = atlas[(x+(y-1)*atlasw)*4+2];
				} else if (y+1<atlash && (atlas[(x+(y+1)*atlasw)*4+3]==0xFF || atlas[(x+(y+1)*atlasw)*4]>0)) {
					atlas[(x+y*atlasw)*4] = atlas[(x+(y+1)*atlasw)*4];
					atlas[(x+y*atlasw)*4+1] = atlas[(x+(y+1)*atlasw)*4+1];
					atlas[(x+y*atlasw)*4+2] = atlas[(x+(y+1)*atlasw)*4+2];
				} else if (x>0 && (atlas[(x-1+y*atlasw)*4+3]==0xFF || atlas[(x-1+y*atlasw)*4]>0)) {
					atlas[(x+y*atlasw)*4] = atlas[(x-1+y*atlasw)*4];
					atlas[(x+y*atlasw)*4+1] = atlas[(x-1+y*atlasw)*4+1];
					atlas[(x+y*atlasw)*4+2] = atlas[(x-1+y*atlasw)*4+2];
				} else if (x+1<atlasw && (atlas[(x+1+y*atlasw)*4+3]==0xFF || atlas[(x+1+y*atlasw)*4]>0)) {
					atlas[(x+y*atlasw)*4] = atlas[(x+1+y*atlasw)*4];
					atlas[(x+y*atlasw)*4+1] = atlas[(x+1+y*atlasw)*4+1];
					atlas[(x+y*atlasw)*4+2] = atlas[(x+1+y*atlasw)*4+2];
				}
			}*/
/*	if (!loadallimage) {
		for (i=0;i<tilesamountplustitle;i++)
			if (allimgalpha[i]!=NULL)
				delete[] allimgalpha[i];
		delete[] allimgalpha;
		delete[] allimgwidth;
		delete[] allimgheight;
	}*/
	// Atlas are y-symetrized
	delete[] tblockimgarray;
	uint8_t tmp8;
	for (y=0;2*y<atlash;y++)
		for (x=0;x<atlasw;x++)
			for (i=0;i<4;i++) {
				tmp8 = atlas[(x+y*atlasw)*4+i];
				atlas[(x+y*atlasw)*4+i] = atlas[(x+(atlash-y-1)*atlasw)*4+i];
				atlas[(x+(atlash-y-1)*atlasw)*4+i] = tmp8;
			}
	// Convert the RGBA atlas into DXT5 compressed atlas
	uint32_t dxtatlassize;
	uint8_t* dxtatlas = TIMImageDataStruct::CreateSteamTextureFile(dxtatlassize,atlasw,atlash,atlas,0x0C,dxtflags);
	atlasout.Write(dxtatlas,dxtatlassize);
	atlasout.Close();
	delete[] dxtatlas;
	delete[] atlas;
	return res;
}

void ToolBackgroundEditor::GetFileNamesAndDepth(wxString basename, wxString multibackseparator, wxString endofname, FieldTilesDataStruct& tiledata, bool sortlayer, bool revertorder, wxString*& resname, unsigned int*& resdepth, bool usemultibackground) {
	unsigned int tilesamountplustitle = tiledata.tiles_amount+tiledata.title_tile_amount*(STEAM_LANGUAGE_AMOUNT-1);
	resname = new wxString[tilesamountplustitle];
	resdepth = new unsigned int[tiledata.tiles_amount];
	unsigned int i,j,tileimgindex;
	for (i=0;i<tilesamountplustitle;i++) {
		if (i<tiledata.tiles_amount && sortlayer) {
			tileimgindex = 0;
			for (j=0;j<tiledata.tiles_amount;j++) {
				if (&tiledata.tiles[i]==tiledata.tiles_sorted[j])
					break;
				if (!usemultibackground || tiledata.tiles[i].camera_id==tiledata.tiles_sorted[j]->camera_id)
					tileimgindex++;
			}
			resdepth[i] = i;
		} else {
			tileimgindex = 0;
			for (j=0;j<i;j++)
				if (!usemultibackground || tiledata.tiles[i].camera_id==tiledata.tiles[j].camera_id)
					tileimgindex++;
			if (i<tiledata.tiles_amount)
				for (j=0;j<tiledata.tiles_amount;j++)
					if (&tiledata.tiles[i]==tiledata.tiles_sorted[j]) {
						resdepth[i] = j;
						break;
					}
		}
		if (revertorder) {
			unsigned int camtileamount = 0;
			for (j=0;j<tilesamountplustitle;j++)
				if (!usemultibackground || tiledata.tiles[i].camera_id==tiledata.tiles[j].camera_id)
					camtileamount++;
			tileimgindex = camtileamount-tileimgindex-1;
			if (i<tiledata.tiles_amount)
				resdepth[i] = tiledata.tiles_amount-resdepth[i]-1;
		}
		if (usemultibackground)
			resname[i] << basename << (unsigned int)tiledata.tiles[i].camera_id << multibackseparator << tileimgindex << endofname;
		else
			resname[i] << basename << tileimgindex << endofname;
	}
}

void ToolBackgroundEditor::DrawImage(wxDC& dc) {
	if (main_img.IsOk()) {
		m_texturewindow->SetVirtualSize(main_img.GetWidth(),main_img.GetHeight());
		MACRO_CREATE_MEMORY_DC(dc,m_texturewindow)
		mdc.DrawBitmap(main_img,0,0);
		if (tile_img.IsOk())
			mdc.DrawBitmap(tile_img,0,0);
		m_texturewindow->DoPrepareDC(dc);
		dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
	} else {
		dc.Clear();
	}
}

void ToolBackgroundEditor::UpdateImage() {
	wxImage img = main_img_base.Copy();
	wxImage tileimg = tile_img_base.Copy();
	
	main_img = wxBitmap(img);
	tile_img = wxBitmap(tileimg);
	wxClientDC dc(m_texturewindow);
	DrawImage(dc);
}

void ToolBackgroundEditor::LoadAndMergeImages() {
	if (!wxFile::Exists(m_imagepicker->GetPath()) || m_fieldchoice->GetSelection()==wxNOT_FOUND) {
		main_img_base.Destroy();
		return;
	}
	unsigned int i,j,x,y;
	uint32_t pix,pix1,pix2;
	bool mainimgisempty = true;
	FieldTilesDataStruct* tileset = cddata->fieldset.background_data[m_fieldchoice->GetSelection()];
	wxFileName imgfilebasename = m_imagepicker->GetPath();
	wxString imgfileext = _(L".")+imgfilebasename.GetExt();
	int lastdigitchar = imgfilebasename.GetName().Len()-1;
	bool usemultiback;
	while (lastdigitchar>=0 && isdigit(imgfilebasename.GetName().GetChar(lastdigitchar)))
		lastdigitchar--;
	if (lastdigitchar>=0 && imgfilebasename.GetName().GetChar(lastdigitchar)==wxUniChar(L'_')) {
		usemultiback = true;
		lastdigitchar--;
		while (lastdigitchar>=0 && isdigit(imgfilebasename.GetName().GetChar(lastdigitchar)))
			lastdigitchar--;
	} else {
		usemultiback = false;
	}
	imgfilebasename.ClearExt();
	imgfilebasename.SetName(imgfilebasename.GetName().Mid(0,lastdigitchar+1));
	wxString* imgfilelist;
	unsigned int* imgorderlist;
	GetFileNamesAndDepth(imgfilebasename.GetFullPath(),_(L"_"),imgfileext,*tileset,m_sortlayer->IsChecked(),m_revertlayer->IsChecked(),imgfilelist,imgorderlist,usemultiback);
	for (i=0;i<m_tilelist->GetCount();i++) {
		if (!m_tilelist->IsSelected(i))
			continue;
		if (wxFile::Exists(imgfilelist[i])) {
			if (mainimgisempty) {
				mainimgisempty = false;
				main_img_base.LoadFile(imgfilelist[i],wxBITMAP_TYPE_ANY);
			} else {
				wxImage imgtoken = wxImage(imgfilelist[i],wxBITMAP_TYPE_ANY);
				for (x=0;x<main_img_base.GetWidth() && x<imgtoken.GetWidth();x++)
					for (y=0;y<main_img_base.GetHeight() && y<imgtoken.GetHeight();y++) {
						if (imgtoken.GetAlpha(x,y)>0) {
							pix1 = (main_img_base.GetAlpha(x,y) << 24) | (main_img_base.GetBlue(x,y) << 16) | (main_img_base.GetGreen(x,y) << 8) | main_img_base.GetRed(x,y);
							pix2 = (imgtoken.GetAlpha(x,y) << 24) | (imgtoken.GetBlue(x,y) << 16) | (imgtoken.GetGreen(x,y) << 8) | imgtoken.GetRed(x,y);
							pix = ImageMergePixels(pix1,pix2,TIM_BLENDMODE_ALPHA);
							main_img_base.SetRGB(x,y,pix & 0xFF,(pix >> 8) & 0xFF,(pix >> 16) & 0xFF);
							main_img_base.SetAlpha(x,y,(pix >> 24) & 0xFF);
						}
					}
			}
		}
	}
	delete[] imgfilelist;
	delete[] imgorderlist;
	if (mainimgisempty)
		main_img_base.Destroy();
}

void ToolBackgroundEditor::ComputeTileFilter(int x, int y) {
	if (!main_img_base.IsOk() || m_fieldchoice->GetSelection()==wxNOT_FOUND)
		return;
	if (x>=0 && y>=0) {
		
	} else {
		unsigned int i,j,x,y;
		tile_img_base.Create(main_img_base.GetSize());
		tile_img_base.SetAlpha();
		for (x=0;x<tile_img_base.GetWidth();x++)
			for (y=0;y<tile_img_base.GetHeight();y++)
				tile_img_base.SetAlpha(x,y,0);
		unsigned int imgtilex,imgtiley,tilesize;
		tilesize = m_resolution->GetValue();
		for (i=0;i<m_tilelist->GetCount();i++) {
			if (!m_tilelist->IsSelected(i))
				continue;
			wxImage tileimgtoken = wxImage(tile_img_base.GetSize());
			tileimgtoken.SetAlpha();
			for (x=0;x<tileimgtoken.GetWidth();x++)
				for (y=0;y<tileimgtoken.GetHeight();y++)
					tileimgtoken.SetAlpha(x,y,0);
			FieldTilesDataStruct& tileset = *cddata->fieldset.background_data[m_fieldchoice->GetSelection()];
			FieldTilesTileDataStruct& tile = i<tileset.tiles_amount ? tileset.tiles[i] : tileset.tiles[i+tileset.title_tile_amount];
			FieldTilesCameraDataStruct& camera = tileset.camera[tile.camera_id];
			for (j=0;j<tile.tile_amount;j++) {
				imgtilex = (tile.pos_x+tile.tile_pos_x[j]-camera.pos_x)/FIELD_TILE_BASE_SIZE*tilesize;
				imgtiley = (tile.pos_y+tile.tile_pos_y[j]-camera.pos_y)/FIELD_TILE_BASE_SIZE*tilesize;
				tileimgtoken.SetRGB(wxRect(imgtilex,imgtiley,tilesize,tilesize),TILECOLOR_INTERIOR.Red(),TILECOLOR_INTERIOR.Green(),TILECOLOR_INTERIOR.Blue());
				for (x=0;x<tilesize && imgtilex+x<tileimgtoken.GetWidth();x++)
					for (y=0;y<tilesize && imgtiley+y<tileimgtoken.GetHeight();y++) {
						tileimgtoken.SetAlpha(imgtilex+x,imgtiley+y,TILECOLOR_INTERIOR.Alpha());
						if (tile_img_base.GetAlpha(imgtilex+x,imgtiley+y)==0)
							tile_img_base.SetRGB(imgtilex+x,imgtiley+y,TILECOLOR_INTERIOR.Red(),TILECOLOR_INTERIOR.Green(),TILECOLOR_INTERIOR.Blue());
					}
			}
			for (x=0;x<tileimgtoken.GetWidth();x++)
				for (y=0;y<tileimgtoken.GetHeight();y++) {
					if (tileimgtoken.GetAlpha(x,y)!=0 && /*
					 */ (x==0 || y==0 || x+1==tileimgtoken.GetWidth() || y+1==tileimgtoken.GetHeight() || /*
					 */ tileimgtoken.GetAlpha(x-1,y)==0 || tileimgtoken.GetAlpha(x,y-1)==0 || tileimgtoken.GetAlpha(x+1,y)==0 || tileimgtoken.GetAlpha(x,y+1)==0)) {
						tileimgtoken.SetAlpha(x,y,TILECOLOR_BOUNDARY.Alpha());
						tile_img_base.SetRGB(x,y,TILECOLOR_BOUNDARY.Red(),TILECOLOR_BOUNDARY.Green(),TILECOLOR_BOUNDARY.Blue());
					}
					tile_img_base.SetAlpha(x,y,min(0xFF,tile_img_base.GetAlpha(x,y)+tileimgtoken.GetAlpha(x,y)));
				}
		}
	}
}

void ToolBackgroundEditor::OnFilePick(wxFileDirPickerEvent& event) {
	LoadAndMergeImages();
	ComputeTileFilter();
	UpdateImage();
}

void ToolBackgroundEditor::OnDirPick(wxFileDirPickerEvent& event) {
	wxString selpath = event.GetPath();
	m_exportdir->SetPath(selpath);
	m_massexportdir->SetPath(selpath);
	m_importdir->SetPath(selpath);
}

void ToolBackgroundEditor::OnRadioClick(wxCommandEvent& event) {
	
}

void ToolBackgroundEditor::OnFieldChoice(wxCommandEvent& event) {
	if (cddata && cddata->fieldloaded) {
		FieldTilesDataStruct& tileset = *cddata->fieldset.background_data[event.GetSelection()];
		unsigned int i;
		m_tilelist->Clear();
		for (i=0;i<tileset.tiles_amount+tileset.title_tile_amount*(STEAM_LANGUAGE_AMOUNT-1);i++) {
			wxString tilelabel;
			tilelabel << L"Tile Block " << i;
			m_tilelist->Append(tilelabel);
		}
	} // ToDo: else
}

void ToolBackgroundEditor::OnTileSelect(wxCommandEvent& event) {
	LoadAndMergeImages();
	ComputeTileFilter();
	UpdateImage();
}

void ToolBackgroundEditor::OnCheckBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_SORTLAYER || id==wxID_REVERTLAYER) {
		LoadAndMergeImages();
		ComputeTileFilter();
		UpdateImage();
	}
}

void ToolBackgroundEditor::OnSpinChange(wxSpinEvent& event) {
	int id = event.GetId();
	if (id==wxID_RESOLUTION) {
		ComputeTileFilter();
		UpdateImage();
	}
}

void ToolBackgroundEditor::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i,j;
	if (id==wxID_APPLY) {
		if (m_auinotebook->GetSelection()==0) { // Converter
			wxString destfilebase = m_exportdir->GetPath()+_(L"\\");
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++)
				if (SteamFieldScript[i].script_id==cddata->fieldset.struct_id[m_fieldchoice->GetSelection()]) {
					destfilebase += _(SteamFieldScript[i].background_name);
					break;
				}
			if (i==G_N_ELEMENTS(SteamFieldScript))
				destfilebase += _(HADES_STRING_UNKNOWN_FIELD);
			FieldTilesDataStruct* tileset = cddata->fieldset.background_data[m_fieldchoice->GetSelection()];
			unsigned int tilesizebackup = tileset->parent->tile_size;
			tileset->parent->tile_size = m_resolution->GetValue();
			wxFileName imgfilebasename = m_imagepicker->GetPath();
			wxString imgfileext = _(L".")+imgfilebasename.GetExt();
			unsigned int tileimgindex;
			int lastdigitchar = imgfilebasename.GetName().Len()-1;
			bool usemultiback;
			while (lastdigitchar>=0 && isdigit(imgfilebasename.GetName().GetChar(lastdigitchar)))
				lastdigitchar--;
			if (lastdigitchar>=0 && imgfilebasename.GetName().GetChar(lastdigitchar)==wxUniChar(L'_')) {
				usemultiback = true;
				lastdigitchar--;
				while (lastdigitchar>=0 && isdigit(imgfilebasename.GetName().GetChar(lastdigitchar)))
					lastdigitchar--;
			} else {
				usemultiback = false;
			}
			imgfilebasename.ClearExt();
			imgfilebasename.SetName(imgfilebasename.GetName().Mid(0,lastdigitchar+1));
			unsigned int tilesamountplustitle = tileset->tiles_amount+tileset->title_tile_amount*(STEAM_LANGUAGE_AMOUNT-1);
			wxString* imgfilelist;
			unsigned int* imgorderlist;
			GetFileNamesAndDepth(imgfilebasename.GetFullPath(),_(L"_"),imgfileext,*tileset,m_sortlayer->IsChecked(),m_revertlayer->IsChecked(),imgfilelist,imgorderlist,usemultiback);
			int res = CreateBackgroundImage(imgfilelist,destfilebase+_(L".tex"),*tileset,imgorderlist,m_dxtflagchoice->GetSelection());
			tileset->parent->tile_size = tilesizebackup;
			if (res<=0) {
				wxMessageDialog popupsuccess(this,HADES_STRING_STEAM_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
				popupsuccess.ShowModal();
			} else if (res==1) {
				wxLogError(HADES_STRING_OPEN_ERROR_CREATE,destfilebase+_(L".tex"));
			} else if (res==2) {
				wxLogError(HADES_STRING_INVALID_IMAGE_DIMENSIONS);
			}
			delete[] imgfilelist;
			delete[] imgorderlist;
		} else if (m_auinotebook->GetSelection()==1) { // Mass Converter
			wxString destfilebase = m_massexportdir->GetPath()+_(L"\\");
			wxString sourcefilebase = m_massimageimporter->GetPath()+_(L"\\");
			wxStringTokenizer sourcefileformat(m_massimageformat->GetValue(),_(L"%"),wxTOKEN_RET_EMPTY_ALL);
			if (sourcefileformat.CountTokens()!=4) {
				wxLogError(HADES_STRING_INVALID_IMAGE_NAME_FORMAT);
				return;
			}
			LogStruct log;
			wxString sourcefiletoken[4];
			wxString destfilefield;
			for (i=0;i<4;i++)
				sourcefiletoken[i] = sourcefileformat.GetNextToken();
			unsigned int fieldindex,fieldid;
			unsigned int tileimgindex;
			unsigned int counter = 0, countermax = 0;
			for (fieldindex=0;fieldindex<cddata->fieldset.amount;fieldindex++) {
				if (m_massfieldid->IsChecked())
					fieldid = cddata->fieldset.struct_id[fieldindex];
				else
					fieldid = fieldindex;
				wxString sourcefilenamecheck;
				sourcefilenamecheck << sourcefilebase << sourcefiletoken[0] << fieldid << sourcefiletoken[1] << 0 << sourcefiletoken[2] << 0 << sourcefiletoken[3];
				if (wxFile::Exists(sourcefilenamecheck))
					countermax++;
			}
			if (countermax==0) {
				wxLogError(HADES_STRING_EMPTY_PROCEDURE);
				return;
			}
			LoadingDialogInit(countermax,_(L"Converting..."));
			for (fieldindex=0;fieldindex<cddata->fieldset.amount;fieldindex++) {
				if (m_massfieldid->IsChecked())
					fieldid = cddata->fieldset.struct_id[fieldindex];
				else
					fieldid = fieldindex;
				wxString sourcefilenamecheck;
				sourcefilenamecheck << sourcefilebase << sourcefiletoken[0] << fieldid << sourcefiletoken[1] << 0 << sourcefiletoken[2] << 0 << sourcefiletoken[3];
				if (wxFile::Exists(sourcefilenamecheck)) {
					FieldTilesDataStruct* tileset = cddata->fieldset.background_data[fieldindex];
					unsigned int tilesamountplustitle = tileset->tiles_amount+tileset->title_tile_amount*(STEAM_LANGUAGE_AMOUNT-1);
					unsigned int tilesizebackup = tileset->parent->tile_size;
					wxString imgfilebase;
					imgfilebase << sourcefilebase << sourcefiletoken[0] << fieldid << sourcefiletoken[1];
					tileset->parent->tile_size = m_massresolution->GetValue();
					destfilefield = destfilebase+_(SteamFieldScript[fieldindex].background_name);
					wxString* imgfilelist;
					unsigned int* imgorderlist;
					GetFileNamesAndDepth(imgfilebase,sourcefiletoken[2],sourcefiletoken[3],*tileset,m_masssortlayer->IsChecked(),m_massrevertlayer->IsChecked(),imgfilelist,imgorderlist,true);
					int res = CreateBackgroundImage(imgfilelist,destfilefield+_(L".tex"),*tileset,imgorderlist,m_massdxtflagchoice->GetSelection());
					tileset->parent->tile_size = tilesizebackup;
					if (res<0) {
						wxString warnstr;
						warnstr.Printf(wxT(HADES_STRING_BACKGROUNDIMPORT_MISSING_LAYERS),-res,fieldid,GetFieldNameOrDefault(cddata,fieldindex));
						log.AddWarning(warnstr.wc_str());
					} else if (res==1) {
						wxString errstr;
						errstr.Printf(wxT(HADES_STRING_BACKGROUNDIMPORT_ERROR_CREATE),fieldid,GetFieldNameOrDefault(cddata,fieldindex));
						log.AddError(errstr.wc_str());
					} else if (res==2) {
						wxString errstr;
						errstr.Printf(wxT(HADES_STRING_BACKGROUNDIMPORT_ERROR_DIMENSIONS),fieldid,GetFieldNameOrDefault(cddata,fieldindex));
						log.AddError(errstr.wc_str());
					}
					delete[] imgfilelist;
					delete[] imgorderlist;
					LoadingDialogUpdate(++counter);
				}
			}
			LoadingDialogEnd();
			LogDialog logdial(this,log);
			logdial.ShowModal();
		} else { // Mass Importer
			wxString importdirname = m_importdir->GetPath()+_(L"\\");
			wxString importpdatadir = m_importlauncher->GetPath();
			if (!wxFile::Exists(importpdatadir) || importpdatadir.Find(_(L"FF9_Launcher.exe"))==wxNOT_FOUND) {
				wxLogError(HADES_STRING_INVALID_FF9LAUNCHER);
				return;
			}
			importpdatadir = importpdatadir.Mid(0,importpdatadir.Find(_(L"FF9_Launcher.exe")))+_(L"StreamingAssets\\");
			wxString importfilename;
			wxString filename;
			bool* copylist[9]{NULL};
			uint32_t* filenewsize[9]{NULL};
			fstream filebase[9];
			fstream filedest[9];
			UnityArchiveMetaData metafield[9];
			UnityArchiveAssetBundle fieldindexlist[9];
			bool isok = true;
			for (i=0;i<9;i++) {
				filename = importpdatadir;
				filename << _(L"p0data1") << (i+1) << _(L".bin");
				filebase[i].open(filename.mb_str(),ios::in | ios::binary);
				filedest[i].open((filename+_(L".tmp")).mb_str(),ios::out | ios::binary);
				if (!filebase[i].is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL,filename);
					isok = false;
				}
				if (!filedest[i].is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE,filename);
					isok = false;
				}
				if (!isok)
					break;
				metafield[i].Load(filebase[i]);
				filebase[i].seekg(metafield[i].GetFileOffset("",142));
				fieldindexlist[i].Load(filebase[i]);
				filebase[i].seekg(0);
				copylist[i] = new bool[metafield[i].header_file_amount];
				filenewsize[i] = new uint32_t[metafield[i].header_file_amount];
			}
			if (!isok) {
				for (i=0;i<9;i++) {
					if (copylist[i]) delete[] copylist[i];
					if (filenewsize[i]) delete[] filenewsize[i];
					if (filebase[i].is_open()) filebase[i].close();
					if (filedest[i].is_open()) filedest[i].close();
				}
				return;
			}
			LoadingDialogInit(11,_(L"Updating Unity Archives..."));
			int32_t fieldimagefile[G_N_ELEMENTS(SteamFieldScript)];
			string fieldbacknamelower;
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++) {
				fieldbacknamelower = SteamFieldScript[i].background_name;
				transform(fieldbacknamelower.begin(),fieldbacknamelower.end(),fieldbacknamelower.begin(),::tolower);
				filename = "assets/resources/fieldmaps/"+fieldbacknamelower+"/atlas.png";
				if (SteamFieldScript[i].file_id==0)
					fieldimagefile[i] = 0;
				else
					fieldimagefile[i] = metafield[SteamFieldScript[i].file_id-1].GetFileIndexByInfo(fieldindexlist[SteamFieldScript[i].file_id-1].GetFileInfo(filename.ToStdString()),28);
			}
			unsigned int importsuccess = 0;
			unsigned int importfail = 0;
			for (i=0;i<9;i++)
				for (j=0;j<metafield[i].header_file_amount;j++) {
					copylist[i][j] = true;
					filenewsize[i][j] = metafield[i].file_size[j];
				}
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++) {
				importfilename = importdirname+_(SteamFieldScript[i].background_name);
				if (wxFile::Exists(importfilename+_(L".tex")) && SteamFieldScript[i].file_id>0) {
					fstream importatlas(importfilename+_(L".tex").mb_str(),ios::in | ios::binary);
					if (!importatlas.is_open()) {
						importfail++;
						continue;
					}
					importatlas.seekg(0,ios::end);
					size_t importatlassize = importatlas.tellg();
					copylist[SteamFieldScript[i].file_id-1][fieldimagefile[i]] = false;
					filenewsize[SteamFieldScript[i].file_id-1][fieldimagefile[i]] = importatlassize;
					importatlas.close();
				}
				// ToDo : Tileset with .bgs
				// ToDo : Walkmesh with .bgi
			}
			LoadingDialogUpdate(1);
			uint32_t* unitydataoff[9];
			for (i=0;i<9;i++) {
				unitydataoff[i] = metafield[i].Duplicate(filebase[i],filedest[i],copylist[i],filenewsize[i]);
				LoadingDialogUpdate(2+i);
			}
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++) {
				importfilename = importdirname+_(SteamFieldScript[i].background_name);
				if (wxFile::Exists(importfilename+_(L".tex")) && SteamFieldScript[i].file_id>0) {
					fstream importatlas(importfilename+_(L".tex").mb_str(),ios::in | ios::binary);
					if (!importatlas.is_open()) {
						importfail++;
						continue;
					}
					filedest[SteamFieldScript[i].file_id-1].seekg(unitydataoff[SteamFieldScript[i].file_id-1][fieldimagefile[i]]);
					char* buffer = new char[filenewsize[SteamFieldScript[i].file_id-1][fieldimagefile[i]]];
					importatlas.read(buffer,filenewsize[SteamFieldScript[i].file_id-1][fieldimagefile[i]]);
					filedest[SteamFieldScript[i].file_id-1].write(buffer,filenewsize[SteamFieldScript[i].file_id-1][fieldimagefile[i]]);
					importatlas.close();
					delete[] buffer;
					importsuccess++;
				}
			}
			LoadingDialogUpdate(11);
			for (i=0;i<9;i++) {
				filebase[i].close();
				filedest[i].close();
				delete[] copylist[i];
				delete[] filenewsize[i];
				delete[] unitydataoff[i];
				filename = importpdatadir;
				filename << _(L"p0data1") << (i+1) << _(L".bin");
				remove(filename.mb_str());
				rename((filename+_(L".tmp")).mb_str(),filename.mb_str());
			}
			LoadingDialogEnd();
			wxString successtring;
			successtring.Printf(wxT(HADES_STRING_IMPORT_BACKGROUND_RESULT),importsuccess,importfail);
			wxMessageDialog popupsuccess(this,successtring,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
		}
	} else if (id==wxID_CLOSE) {
		EndModal(id);
	}
}

void ToolBackgroundEditor::OnPaintImage(wxPaintEvent& event) {
	wxPaintDC dc(m_texturewindow);
	DrawImage(dc);
}
