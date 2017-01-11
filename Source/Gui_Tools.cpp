#include "Gui_Tools.h"

#include "main.h"
#include "Gui_TextureEditor.h"
#include "Gui_LoadingDialog.h"
#include "Hades_Strings.h"
#include "Steam_Strings.h"
#include "Squish/squish.h"

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

const static wxColor TILECOLOR_BOUNDARY = wxColor(0,0,0,255);
const static wxColor TILECOLOR_INTERIOR = wxColor(80,80,80,50);

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
		m_fieldchoice->SetSelection(0);
	} else {
		m_usegametilebtn->Enable(false);
		m_fieldchoice->Enable(false);
	}
	return BackgroundEditorWindow::ShowModal();
}

int CreateBackgroundImage(wxString imgfilename, wxString outputname, FieldTilesDataStruct& tiledata, int* sortedimg = NULL, wxBitmapType type = wxBITMAP_TYPE_ANY) {
	// Compute size of the atlas so it's a roughly square image
	unsigned int atlassize, atlasw, atlash, atlastilecolcount, tileamount, tilesize, tilegap, tileperiod;
	unsigned int i,j,x,y;
	uint8_t* atlas;
	wxFile atlasout(outputname,wxFile::write);
	if (!atlasout.IsOpened())
		return 1;
	tilesize = tiledata.parent->tile_size;
	tilegap = tiledata.parent->tile_gap;
	tileperiod = tilesize+2*tilegap;
	tileamount = 0;
	for (i=0;i<tiledata.tiles_amount;i++)
		tileamount += tiledata.tiles[i].tile_amount;
	atlasw = sqrt(tileamount);
	atlasw = max(atlasw,1u);
	atlash = tileamount/atlasw;
	atlash = (atlash*atlasw<tileamount) ? atlash+1 : atlash;
	atlastilecolcount = atlasw;
	atlasw *= tileperiod;
	atlash *= tileperiod;
//atlasw = 1024; atlash = 1024; atlastilecolcount = 28; // DEBUG
	while (atlasw%4) atlasw++; // For DXT5 compression, it is better to have sizes multiple of 4
	while (atlash%4) atlash++;
	atlassize = atlasw*atlash*4;
	atlas = new uint8_t[atlassize]{0};
	// Add each tile to the atlas in the order
	unsigned int imgtilex, imgtiley, imgwidth;
	unsigned int atlasx, atlasy;
	for (i=0;i<tiledata.tiles_amount;i++) {
		FieldTilesTileDataStruct& tile = tiledata.tiles[i];
		FieldTilesCameraDataStruct& camera = tiledata.camera[tile.camera_id];
		wxImage tblockimg;
		if (sortedimg) {
			if (sortedimg[i]<0 || sortedimg[i]>=wxImage::GetImageCount(imgfilename))
				continue;
			tblockimg.LoadFile(imgfilename,type,sortedimg[i]);
		} else {
			if (i>=wxImage::GetImageCount(imgfilename))
				break;
			tblockimg.LoadFile(imgfilename,type,i);
		}
		uint8_t* imgdata = tblockimg.GetData();
		uint8_t* imgalpha = tblockimg.GetAlpha();
		imgwidth = tblockimg.GetWidth();
		for (j=0;j<tile.tile_amount;j++) {
			imgtilex = (tile.pos_x+tile.tile_pos_x[j]-camera.pos_x)/FIELD_TILE_BASE_SIZE*tilesize;
			imgtiley = (tile.pos_y+tile.tile_pos_y[j]-camera.pos_y)/FIELD_TILE_BASE_SIZE*tilesize;
			atlasx = (tile.tile_steam_id[j]%atlastilecolcount)*tileperiod+tilegap;
			atlasy = (tile.tile_steam_id[j]/atlastilecolcount)*tileperiod+tilegap;
			for (y=0;y<tilesize;y++)
				for (x=0;x<tilesize;x++) {
					atlas[(atlasx+x+(atlasy+y)*atlasw)*4] = imgdata[(imgtilex+x+(imgtiley+y)*imgwidth)*3];
					atlas[(atlasx+x+(atlasy+y)*atlasw)*4+1] = imgdata[(imgtilex+x+(imgtiley+y)*imgwidth)*3+1];
					atlas[(atlasx+x+(atlasy+y)*atlasw)*4+2] = imgdata[(imgtilex+x+(imgtiley+y)*imgwidth)*3+2];
					atlas[(atlasx+x+(atlasy+y)*atlasw)*4+3] = imgalpha[imgtilex+x+(imgtiley+y)*imgwidth];
				}
			for (y=0;y<tilesize;y++)
				for (x=0;x<tilegap;x++) {
					atlas[(atlasx-x-1+(atlasy+y)*atlasw)*4] = imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3];
					atlas[(atlasx-x-1+(atlasy+y)*atlasw)*4+1] = imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3+1];
					atlas[(atlasx-x-1+(atlasy+y)*atlasw)*4+2] = imgdata[(imgtilex+(imgtiley+y)*imgwidth)*3+2];
					atlas[(atlasx-x-1+(atlasy+y)*atlasw)*4+3] = imgalpha[imgtilex+(imgtiley+y)*imgwidth];
					atlas[(atlasx+tilesize+x+(atlasy+y)*atlasw)*4] = imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3];
					atlas[(atlasx+tilesize+x+(atlasy+y)*atlasw)*4+1] = imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3+1];
					atlas[(atlasx+tilesize+x+(atlasy+y)*atlasw)*4+2] = imgdata[(imgtilex+tilesize-1+(imgtiley+y)*imgwidth)*3+2];
					atlas[(atlasx+tilesize+x+(atlasy+y)*atlasw)*4+3] = imgalpha[imgtilex+tilesize-1+(imgtiley+y)*imgwidth];
				}
			for (x=0;x<tilesize;x++)
				for (y=0;y<tilegap;y++) {
					atlas[(atlasx+(atlasy-y-1)*atlasw)*4] = imgdata[(imgtilex+x+imgtiley*imgwidth)*3];
					atlas[(atlasx+(atlasy-y-1)*atlasw)*4+1] = imgdata[(imgtilex+x+imgtiley*imgwidth)*3+1];
					atlas[(atlasx+(atlasy-y-1)*atlasw)*4+2] = imgdata[(imgtilex+x+imgtiley*imgwidth)*3+2];
					atlas[(atlasx+(atlasy-y-1)*atlasw)*4+3] = imgalpha[imgtilex+x+imgtiley*imgwidth];
					atlas[(atlasx+(atlasy+tilesize+y)*atlasw)*4] = imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3];
					atlas[(atlasx+(atlasy+tilesize+y)*atlasw)*4+1] = imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3+1];
					atlas[(atlasx+(atlasy+tilesize+y)*atlasw)*4+2] = imgdata[(imgtilex+x+(imgtiley+tilesize-1)*imgwidth)*3+2];
					atlas[(atlasx+(atlasy+tilesize+y)*atlasw)*4+3] = imgalpha[imgtilex+x+(imgtiley+tilesize-1)*imgwidth];
				}
		}
	}
	// Atlas are y-symetrized
	uint8_t tmp8;
	for (y=0;2*y<atlash;y++)
		for (x=0;x<atlasw;x++) {
			for (i=0;i<4;i++) {
				tmp8 = atlas[(x+y*atlasw)*4+i];
				atlas[(x+y*atlasw)*4+i] = atlas[(x+(atlash-y-1)*atlasw)*4+i];
				atlas[(x+(atlash-y-1)*atlasw)*4+i] = tmp8;
			}
		}
	// Convert the RGBA atlas into DXT5 compressed atlas
	uint32_t dxtatlassize;
	uint8_t* dxtatlas = TIMImageDataStruct::CreateSteamTextureFile(dxtatlassize,atlasw,atlash,atlas);
	/* DEBUG
	int dxtflag = squish::kDxt5;
	unsigned int dxtatlassize = squish::GetStorageRequirements(atlasw,atlash,dxtflag);
	uint8_t* dxtatlas = new uint8_t[dxtatlassize];
	squish::CompressImage(atlas,atlasw,atlash,dxtatlas,dxtflag);
	uint32_t headervalue;
	headervalue = atlasw;			atlasout.Write(&headervalue,4);
	headervalue = atlash;			atlasout.Write(&headervalue,4);
	headervalue = atlasw*atlash;	atlasout.Write(&headervalue,4);
	headervalue = 0x0C;				atlasout.Write(&headervalue,4); // DXT5 format
	headervalue = 1;				atlasout.Write(&headervalue,4); // mip count
	headervalue = 0x100;			atlasout.Write(&headervalue,4); // flags
	headervalue = 1;				atlasout.Write(&headervalue,4); // image count
	headervalue = 2;				atlasout.Write(&headervalue,4); // dimension
	headervalue = 1;				atlasout.Write(&headervalue,4); // filter mode
	headervalue = 0;				atlasout.Write(&headervalue,4); // anisotropic
	headervalue = 0;				atlasout.Write(&headervalue,4); // mip bias
	headervalue = 1;				atlasout.Write(&headervalue,4); // wrap mode
	headervalue = 0;				atlasout.Write(&headervalue,4); // lightmap format
	headervalue = 1;				atlasout.Write(&headervalue,4); // color space
	headervalue = atlasw*atlash;	atlasout.Write(&headervalue,4);*/
	atlasout.Write(dxtatlas,dxtatlassize);
	atlasout.Close();
	delete[] dxtatlas;
	delete[] atlas;
	return 0;
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
	if (!wxFile::Exists(m_imagepicker->GetPath())) {
		main_img_base.Destroy();
		return;
	}
	unsigned int i,ti,x,y;
	wxArrayInt tileselindexlist;
	bool imgisempty = true;
	uint32_t pix,pix1,pix2;
	m_tilelist->GetSelections(tileselindexlist);
	for (ti=0;ti<tileselindexlist.GetCount();ti++) {
		if (tileselindexlist[ti]<wxImage::GetImageCount(m_imagepicker->GetPath())) {
			if (imgisempty) {
				imgisempty = false;
				main_img_base.LoadFile(m_imagepicker->GetPath(),wxBITMAP_TYPE_ANY,tileselindexlist[ti]);
			} else {
				wxImage imgtoken = wxImage(m_imagepicker->GetPath(),wxBITMAP_TYPE_ANY,tileselindexlist[ti]);
				for (x=0;x<main_img_base.GetWidth();x++)
					for (y=0;y<main_img_base.GetWidth();y++) {
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
	if (imgisempty)
		main_img_base.Destroy();
}

void ToolBackgroundEditor::ComputeTileFilter(int x, int y) {
	if (x>=0 && y>=0) {
		
	} else {
		unsigned int i,ti,x,y;
		wxArrayInt tileselindexlist;
		FieldTilesTileDataStruct* tileptr;
		tile_img_base.Create(main_img_base.GetSize());
		tile_img_base.SetAlpha();
		for (x=0;x<tile_img_base.GetWidth();x++)
			for (y=0;y<tile_img_base.GetHeight();y++)
				tile_img_base.SetAlpha(x,y,0);
		unsigned int imgtilex,imgtiley,tilesize;
		tilesize = m_resolution->GetValue();
		if (m_tilelist->GetSelections(tileselindexlist)>0)
			for (ti=0;ti<tileselindexlist.GetCount();ti++) {
				wxImage tileimgtoken = wxImage(tile_img_base.GetSize());
				tileimgtoken.SetAlpha();
				for (x=0;x<tileimgtoken.GetWidth();x++)
					for (y=0;y<tileimgtoken.GetHeight();y++)
						tileimgtoken.SetAlpha(x,y,0);
				if (m_sorttileset->IsChecked())
					tileptr = cddata->fieldset.background_data[m_fieldchoice->GetSelection()]->tiles_sorted[tileselindexlist[ti]];
				else
					tileptr = &cddata->fieldset.background_data[m_fieldchoice->GetSelection()]->tiles[tileselindexlist[ti]];
				FieldTilesTileDataStruct& tile = *tileptr;
				FieldTilesCameraDataStruct& camera = cddata->fieldset.background_data[m_fieldchoice->GetSelection()]->camera[tile.camera_id];
				for (i=0;i<tile.tile_amount;i++) {
					imgtilex = (tile.pos_x+tile.tile_pos_x[i]-camera.pos_x)/FIELD_TILE_BASE_SIZE*tilesize;
					imgtiley = (tile.pos_y+tile.tile_pos_y[i]-camera.pos_y)/FIELD_TILE_BASE_SIZE*tilesize;
					tileimgtoken.SetRGB(wxRect(imgtilex,imgtiley,tilesize,tilesize),TILECOLOR_INTERIOR.Red(),TILECOLOR_INTERIOR.Green(),TILECOLOR_INTERIOR.Blue());
					for (x=0;x<tilesize && imgtilex+x<tileimgtoken.GetWidth();x++)
						for (y=0;y<tilesize && imgtiley+y<tileimgtoken.GetHeight();y++)
							tileimgtoken.SetAlpha(imgtilex+x,imgtiley+y,TILECOLOR_INTERIOR.Alpha());
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
	if (wxFile::Exists(event.GetPath())) {
		LoadAndMergeImages();
		ComputeTileFilter();
	} else {
		main_img_base.Destroy();
	}
	UpdateImage();
}

void ToolBackgroundEditor::OnRadioClick(wxCommandEvent& event) {
	
}

void ToolBackgroundEditor::OnFieldChoice(wxCommandEvent& event) {
	FieldTilesDataStruct& tileset = *cddata->fieldset.background_data[event.GetSelection()];
	unsigned int i;
	m_tilelist->Clear();
	for (i=0;i<tileset.tiles_amount;i++) {
		wxString tilelabel;
		tilelabel << L"Tile Block " << i;
		m_tilelist->Append(tilelabel);
	}
}

void ToolBackgroundEditor::OnTileSelect(wxCommandEvent& event) {
	LoadAndMergeImages();
	ComputeTileFilter();
	UpdateImage();
}

void ToolBackgroundEditor::OnCheckBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_SORTTILE) {
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
		if (m_auinotebook->GetSelection()==0) {
			wxString destfilebase = m_exportdir->GetPath()+_(L"\\");
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++)
				if (SteamFieldScript[i].script_id==cddata->fieldset.struct_id[m_fieldchoice->GetSelection()]) {
					destfilebase += _(SteamFieldScript[i].background_name);
					break;
				}
			if (i==G_N_ELEMENTS(SteamFieldScript))
				destfilebase += _(HADES_STRING_UNKNOWN_FIELD);
			FieldTilesDataStruct* tileset;
			tileset = cddata->fieldset.background_data[m_fieldchoice->GetSelection()];
			unsigned int tilesizebackup = tileset->parent->tile_size;
			tileset->parent->tile_size = m_resolution->GetValue();
			int* sortedimg = NULL;
			if (m_sorttileset->IsChecked()) {
				sortedimg = new int[tileset->tiles_amount];
				for (i=0;i<tileset->tiles_amount;i++) {
					sortedimg[i] = -1;
					for (j=0;j<tileset->tiles_amount;j++)
						if (&tileset->tiles[i]==tileset->tiles_sorted[j]) {
							sortedimg[i] = j;
							break;
						}
				}
			}
			int res = CreateBackgroundImage(m_imagepicker->GetPath(),destfilebase+_(L".tex"),*tileset,sortedimg);
			tileset->parent->tile_size = tilesizebackup;
			if (res==0) {
				wxMessageDialog popupsuccess(this,HADES_STRING_STEAM_SAVE_SUCCESS,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
				popupsuccess.ShowModal();
			} else if (res==1) {
				wxLogError(HADES_STRING_OPEN_ERROR_CREATE,destfilebase+_(L".tex"));
			}
			if (sortedimg)
				delete[] sortedimg;
		} else {
			wxString importdirname = m_importdir->GetPath()+_(L"\\");
			wxString importpdatadir = m_importlauncher->GetPath();
			if (!wxFile::Exists(importpdatadir) || importpdatadir.Find(_(L"FF9_Launcher.exe"))==wxNOT_FOUND) {
				wxLogError(HADES_STRING_INVALID_FF9LAUNCHER);
				return;
			}
			importpdatadir = importpdatadir.Mid(0,importpdatadir.Find(_(L"FF9_Launcher.exe")))+_(L"StreamingAssets\\");
			wxString importfilename;
			wxString pdatafilename;
			ConfigurationSet& config = cddata->config;
			bool* copylist[9];
			uint32_t* filenewsize[9];
			fstream filebase[9];
			fstream filedest[9];
			bool isok = true;
			for (i=0;i<9;i++) {
				copylist[i] = new bool[config.meta_field[i].header_file_amount];
				filenewsize[i] = new uint32_t[config.meta_field[i].header_file_amount];
				pdatafilename = importpdatadir;
				pdatafilename << _(L"p0data1") << (i+1) << _(L".bin");
				filebase[i].open(pdatafilename.mb_str(),ios::in | ios::binary);
				filedest[i].open((pdatafilename+_(L".tmp")).mb_str(),ios::out | ios::binary);
				if (!filebase[i].is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL,pdatafilename);
					isok = false;
				}
				if (!filedest[i].is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE,pdatafilename);
					isok = false;
				}
			}
			if (!isok) {
				for (i=0;i<9;i++) {
					delete[] copylist[i];
					delete[] filenewsize[i];
					if (filebase[i].is_open())
						filebase[i].close();
					if (filedest[i].is_open())
						filedest[i].close();
				}
				return;
			}
			LoadingDialogInit(11,_(L"Updating Unity Archives..."));
			unsigned int importsuccess = 0;
			unsigned int importfail = 0;
			for (i=0;i<9;i++)
				for (j=0;j<config.meta_field[i].header_file_amount;j++) {
					copylist[i][j] = true;
					filenewsize[i][j] = config.meta_field[i].file_size[j];
				}
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++) {
				importfilename = importdirname+_(SteamFieldScript[i].background_name);
				if (wxFile::Exists(importfilename+_(L".tex"))) {
					fstream importatlas(importfilename+_(L".tex").mb_str(),ios::in | ios::binary);
					if (!importatlas.is_open()) {
						importfail++;
						continue;
					}
					importatlas.seekg(0,ios::end);
					size_t importatlassize = importatlas.tellg();
					copylist[SteamFieldScript[i].file_id-1][config.field_image_file[i]] = true;
					filenewsize[SteamFieldScript[i].file_id-1][config.field_image_file[i]] = importatlassize;
					importatlas.close();
				}
				// ToDo : Tileset with .bgs
				// ToDo : Walkmesh with .bgi
			}
			LoadingDialogUpdate(1);
			uint32_t* unitydataoff[9];
			for (i=0;i<9;i++) {
				unitydataoff[i] = config.meta_field[i].Duplicate(filebase[i],filedest[i],copylist[i],filenewsize[i]);
				LoadingDialogUpdate(2+i);
			}
			for (i=0;i<G_N_ELEMENTS(SteamFieldScript);i++) {
				importfilename = importdirname+_(SteamFieldScript[i].background_name);
				if (wxFile::Exists(importfilename+_(L".tex"))) {
					fstream importatlas(importfilename+_(L".tex").mb_str(),ios::in | ios::binary);
					if (!importatlas.is_open()) {
						importfail++;
						continue;
					}
					filedest[SteamFieldScript[i].file_id-1].seekg(unitydataoff[SteamFieldScript[i].file_id-1][config.field_image_file[i]]);
					char* buffer = new char[filenewsize[SteamFieldScript[i].file_id-1][config.field_image_file[i]]];
					importatlas.read(buffer,filenewsize[SteamFieldScript[i].file_id-1][config.field_image_file[i]]);
					filedest[SteamFieldScript[i].file_id-1].write(buffer,filenewsize[SteamFieldScript[i].file_id-1][config.field_image_file[i]]);
					importatlas.close();
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
				pdatafilename = importpdatadir;
				pdatafilename << _(L"p0data1") << (i+1) << _(L".bin");
				remove(pdatafilename.mb_str());
				rename((pdatafilename+_(L".tmp")).mb_str(),pdatafilename.mb_str());
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
