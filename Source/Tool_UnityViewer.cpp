#include "Tool_UnityViewer.h"

#include <wx/tokenzr.h>
#include "Hades_Strings.h"
#include "Database_Resource.h"
#include "Database_SpellAnimation.h"
#include "main.h"

#define ASSET_COLUMN_INDEX		L"#"
#define ASSET_COLUMN_NAME		L"File Name"
#define ASSET_COLUMN_INTERNAME	L"Internal File Name"
#define ASSET_COLUMN_TYPE		L"Type"
#define ASSET_COLUMN_SIZE		L"Size"
#define ASSET_COLUMN_INFO		L"Infos"

struct SortItemInfo {
    wxListCtrl* assetlist;
    int column;
    bool ascending;
};

int wxCALLBACK SortItemCompare(wxIntPtr item1, wxIntPtr item2, wxIntPtr infoptr) {
	SortItemInfo* info = (SortItemInfo*)infoptr;
	wxListCtrl* assetlist = info->assetlist;
    int column = info->column;
    bool ascending = info->ascending;
	int result;
	if (column==1 || column==2 || column==5) {
		result = assetlist->GetItemText(item1,column).CmpNoCase(assetlist->GetItemText(item2,column));
	} else {
		long val1 = wxAtol(assetlist->GetItemText(item1,column));
		long val2 = wxAtol(assetlist->GetItemText(item2,column));
		if (val1<val2)
			result = -1;
		else if (val1>val2)
			result = 1;
		else
			result = 0;
	}
    if (ascending)
		return result;
	return -result;
}

ToolUnityViewer::ToolUnityViewer(wxWindow* parent) :
	UnityViewerWindow(parent) {
	m_assetlist->AppendColumn(_(ASSET_COLUMN_INDEX),wxLIST_FORMAT_LEFT,40);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_NAME),wxLIST_FORMAT_LEFT,300);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_INTERNAME),wxLIST_FORMAT_LEFT,120);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_TYPE),wxLIST_FORMAT_LEFT,90);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_SIZE),wxLIST_FORMAT_LEFT,50);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_INFO),wxLIST_FORMAT_LEFT,200);
	root_path = wxEmptyString;
	column_sort = 0;
	column_sort_ascending = true;
}

bool ToolUnityViewer::SetupRootPath(wxString path) {
	wxString filename;
	uint32_t offset;
	unsigned int i,j;
	if (path.Last()!=L'\\')
		path += L"\\";
	// Verify that the required files are available
	for (i=0;i<UNITY_ARCHIVE_AMOUNT;i++) {
		filename = path+UnityArchiveMetaData::GetArchiveName((UnityArchiveFile)i);
		if (!wxFile::Exists(filename)) {
			wxLogError(HADES_STRING_OPEN_ERROR_NONEXISTENT,filename);
			return false;
		} else if (!wxFile::Access(filename,wxFile::read)) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,filename);
			return false;
		}
	}
	// Popup a warning if the game is opened in HW's main module
	for (i=0;i<GetTopWindow()->CDPanelAmount;i++)
		if (path.Cmp(GetTopWindow()->CDPanel[i]->filename)==0) {
			wxMessageDialog popupwarning(this,HADES_STRING_UNITYVIEWER_GAME_OPEN,HADES_STRING_WARNING,wxOK|wxCENTRE);
			popupwarning.ShowModal();
		}
	// Read the asset lists
	root_path = path;
	for (i=0;i<UNITY_ARCHIVE_AMOUNT;i++) {
		filename = path+UnityArchiveMetaData::GetArchiveName((UnityArchiveFile)i);
		fstream unityarchive(filename.c_str(),ios::in | ios::binary);
		meta_data[i].Load(unityarchive);
		if (i>=UNITY_ARCHIVE_DATA11 && i<=UNITY_ARCHIVE_DATA7) {
			offset = meta_data[i].GetFileOffset("",142);
			if (offset>0) {
				unityarchive.seekg(offset);
				bundle_data[i-UNITY_ARCHIVE_DATA11].Load(unityarchive);
			}
		} else if (i==UNITY_ARCHIVE_MAINDATA) {
			offset = meta_data[i].GetFileOffset("",147);
			if (offset>0) {
				unityarchive.seekg(offset);
				list_data.Load(unityarchive);
				list_data_filename.Empty();
				list_data_filename.Alloc(list_data.amount);
				for (j=0;j<list_data.amount;j++)
					list_data_filename.Add(_(list_data.path[j]).AfterLast(L'/'));
			}
		}
		unityarchive.close();
	}
	return true;
}

bool ToolUnityViewer::DisplayArchive(UnityArchiveFile archivetype) {
	unsigned int unkcounter = 0;
	unsigned int i,j;
	long itemid;
	bool foundfullname;
	wxString fullname,infostr,typestr;
	m_assetlist->DeleteAllItems();
	for (i=0;i<meta_data[archivetype].header_file_amount;i++) {
		fullname = _(meta_data[archivetype].file_name[i]);
		foundfullname = false;
		if (archivetype>=UNITY_ARCHIVE_DATA11 && archivetype<=UNITY_ARCHIVE_DATA7) {
			UnityArchiveAssetBundle& bundle = bundle_data[archivetype-UNITY_ARCHIVE_DATA11];
			for (j=0;j<bundle.amount;j++)
				if (bundle.info[j]==meta_data[archivetype].file_info[i]) {
					fullname = _(bundle.path[j]);
					foundfullname = true;
					break;
				}
		}
		if (!foundfullname) {
			for (j=0;j<list_data.amount;j++)
				if (list_data_filename[j].IsSameAs(fullname,false) && list_data.index[j]==i+1) {
					fullname = _(list_data.path[j]);
					foundfullname = true;
					break;
				}
		}
		if (!foundfullname) {
			if (meta_data[archivetype].file_type1[i]==142) {
				fullname = _(L"AssetBundle");
				foundfullname = true;
			} else if (meta_data[archivetype].file_type1[i]==147) {
				fullname = _(L"ResourceManager");
				foundfullname = true;
			} else if (meta_data[archivetype].file_name_len[i]>0) {
				foundfullname = true;
			} else {
				fullname = _(L"NoName")+_(ConvertToString(++unkcounter));
			}
		}
		if (foundfullname)
			infostr = GetInfoString(fullname,meta_data[archivetype].file_type1[i],archivetype);
		else
			infostr = _(L"");
		typestr = _(UnityArchiveMetaData::GetTypeName(meta_data[archivetype].file_type1[i]));
		if (typestr.Len()>0)
			typestr = _(ConvertToString(meta_data[archivetype].file_type1[i]))+_(L" (")+typestr+_(L")");
		else
			typestr = _(ConvertToString(meta_data[archivetype].file_type1[i]));
		itemid = m_assetlist->InsertItem(i,_(ConvertToString(i+1)));
		m_assetlist->SetItem(itemid,1,m_menudisplaypath->IsChecked() ? fullname : fullname.AfterLast(L'/'));
		m_assetlist->SetItem(itemid,2,_(meta_data[archivetype].file_name[i]));
		m_assetlist->SetItem(itemid,3,typestr);
		m_assetlist->SetItem(itemid,4,_(ConvertToString(meta_data[archivetype].GetFileSizeById(i))));
		m_assetlist->SetItem(itemid,5,infostr);
		m_assetlist->SetItemData(itemid,itemid);
	}
	return true;
}

wxString ToolUnityViewer::GetInfoString(wxString filename, uint32_t filetype, UnityArchiveFile archive) {
	wxStringTokenizer pathtoken(filename,_(L"/\\"),wxTOKEN_RET_EMPTY_ALL);
	wxArrayString patharray;
	patharray.Alloc(pathtoken.CountTokens());
	while (pathtoken.HasMoreTokens())
		patharray.Add(pathtoken.GetNextToken());
	wxString name = patharray.Last();
	wxString ext = name.AfterFirst(L'.');
	if (name.IsSameAs(L"AssetBundle")) return _(L"Bundle of assets, contains informations about the assets of this archive and how they relate to each other");
	if (name.IsSameAs(L"ResourceManager")) return _(L"Contains the file full paths of other assets from other archives");
	if (archive==UNITY_ARCHIVE_DATA2) {
		if (patharray.Count()>=7 && patharray[2].IsSameAs(L"battlemap",false) && patharray[3].IsSameAs(L"battlemodel",false) && patharray[4].IsSameAs(L"6",false)) {
			unsigned int id = wxAtoi(patharray[5]);
			wxString partstr = _(L"[Unknown Model]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++)
				if (HADES_STRING_MODEL_NAME[i].id==id) {
					partstr = _(HADES_STRING_MODEL_NAME[i].label);
					break;
				}
			return _(UnityArchiveMetaData::GetTypeName(filetype))+_(L" of the weapon model ")+partstr;
		} else if ((patharray.Count()==5 && patharray[2].IsSameAs(L"battlemap",false) && patharray[3].IsSameAs(L"battleinfo",false)) || /*
				*/ (patharray.Count()==5 && patharray[2].IsSameAs(L"battlemap",false) && patharray[3].IsSameAs(L"battletexanim",false)) || /*
				*/ (patharray.Count()>=7 && patharray[2].IsSameAs(L"battlemap",false) && patharray[3].IsSameAs(L"battlemodel",false) && patharray[4].IsSameAs(L"battlemap_all",false))) {
			wxString btlid = patharray.Count()==5 ? name.Mid(4).BeforeFirst(L'.') : patharray[5].Mid(4);
			wxString partstr = _(L"[Unknown Battle Scene]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME);i++)
				if (btlid.IsSameAs(HADES_STRING_BATTLE_SCENE_NAME[i].steamid,false)) {
					partstr = _(HADES_STRING_BATTLE_SCENE_NAME[i].label);
					break;
				}
			if (patharray.Count()==5 && patharray[3].IsSameAs(L"battleinfo",false)) return _(L"Info for the battle scene ")+partstr;
			else if (patharray.Count()==5) return _(L"Texture Animation for the battle scene ")+partstr;
			return _(UnityArchiveMetaData::GetTypeName(filetype))+_(L" for the battle scene ")+partstr;
		} else if (patharray.Count()==6 && patharray[2].IsSameAs(L"battlemap",false) && patharray[3].IsSameAs(L"battlescene",false)) {
			wxString btlid = patharray[4];
			wxString partstr = _(L"[Unknown Battle]");
			for (unsigned int i=0;i<G_N_ELEMENTS(SteamBattleScript);i++)
				if (btlid.IsSameAs(SteamBattleScript[i].name,false)) {
					partstr = _(ConvertToString(SteamBattleScript[i].battle_id));
					break;
				}
			if (ext.IsSameAs(L"raw17.bytes",false)) return _(L"Animation Sequencing and Cameras for the battle ")+partstr;
			return _(L"Enemy Stats, Attacks and Groups for the battle ")+partstr;
		}
	} else if (archive==UNITY_ARCHIVE_RESOURCES) {
		if (name.IsSameAs(L"AnimationFolderMapping.txt",false)) return _(L"");
		if (name.IsSameAs(L"BattleMapList.txt",false)) return _(L"");
		if (name.IsSameAs(L"BtlEncountBgmMetaData.txt",false)) return _(L"");
		if (name.IsSameAs(L"IconPathMap.txt",false)) return _(L"");
		if (name.IsSameAs(L"Licence_Amazon.txt",false)) return _(L"");
		if (name.IsSameAs(L"Licence_Android.txt",false)) return _(L"");
		if (name.IsSameAs(L"Licence_EStore.txt",false)) return _(L"");
		if (name.IsSameAs(L"Licence_Steam.txt",false)) return _(L"");
		if (name.IsSameAs(L"Licence_iOS.txt",false)) return _(L"");
		if (name.IsSameAs(L"Localization.txt",false)) return _(L"UI texts in multi-languages");
		if (name.IsSameAs(L"MovieAudioMetaData.txt",false)) return _(L"Dictionary <Audio File Name, Audio ID> used by SoundMetaData");
		if (name.IsSameAs(L"MusicMetaData.txt",false)) return _(L"Dictionary <Music ID, Music File Name> used by SoundMetaData");
		if (name.IsSameAs(L"ResidentSpecialEffectMetaData.txt",false)) return _(L"Dictionary <??? ID, ??? File Name, ??? Type> used by SoundMetaData");
		if (name.IsSameAs(L"SongMetaData.txt",false)) return _(L"Dictionary <Song ID, Song File Name> used by SoundMetaData");
		if (name.IsSameAs(L"SoundEffectExtendedMetaData.txt",false)) return _(L"Dictionary <Sound ID, Sound File Name> used by SoundMetaData");
		if (name.IsSameAs(L"SoundEffectMetaData.txt",false)) return _(L"Dictionary <Sound ID, Sound File Name> used by SoundMetaData");
		if (name.IsSameAs(L"SpecialEffectMetaData.txt",false)) return _(L"Dictionary <Sound ID, Sound File Name, Sound Type> used by SoundMetaData");
		if (name.IsSameAs(L"StaffCredits_Amazon.txt",false)) return _(L"");
		if (name.IsSameAs(L"StaffCredits_EStore.txt",false)) return _(L"");
		if (name.IsSameAs(L"StaffCredits_Mobile.txt",false)) return _(L"");
		if (name.IsSameAs(L"StaffCredits_Steam.txt",false)) return _(L"");
		if (name.IsSameAs(L"WldBtlEncountBgmMetaData.txt",false)) return _(L"");
		if (name.IsSameAs(L"mapExtraOffsetList.txt",false)) return _(L"");
		if (name.IsSameAs(L"mapList.txt",false)) return _(L"");
		if (name.IsSameAs(L"mapLocalizeAreaTitle.txt",false)) return _(L"Multi-language tile informations for Field Backgrounds containing titles");
		if (name.IsSameAs(L"mapSPSExtraOffsetList.txt",false)) return _(L"");
		if (name.IsSameAs(L"models.txt",false)) return _(L"");
		if (name.IsSameAs(L"settingUtils.txt",false)) return _(L"");
		if (name.IsSameAs(L"MINIGAME_CARD_DATA_ADDRESS",false)) return _(L"Tetra Master card statistics used by CardPool");
		if (name.IsSameAs(L"MINIGAME_STAGE_ADDRESS",false)) return _(L"Tetra Master [Deck ID, Difficulty] informations used by EnemyData");
		if (name.IsSameAs(L"MINIGAME_CARD_LEVEL_ADDRESS",false)) return _(L"Tetra Master lists of cards played by opponents used by EnemyData");
		if (patharray.Count()==2 && patharray[0].IsSameAs(L"specialeffects",false) && name.Mid(0,2).IsSameAs(L"ef",false)) {
			unsigned int id = wxAtoi(name.Mid(2));
			wxString partstr = _(L"[Unknown Spell Animation]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_SPELL_MODEL);i++)
				if (HADES_STRING_SPELL_MODEL[i].id==id) {
					partstr = _(HADES_STRING_SPELL_MODEL[i].label);
					break;
				}
			return _(L"Special Effect of the spell animation ")+partstr;
		}
	}
	
	return _(L"");
}

void ToolUnityViewer::OnMenuSelection(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_OPEN) {
		wxFileDialog* openFileDialog = new wxFileDialog(this,HADES_STRING_OPEN_FRAME_NAME,"","",HADES_STRING_OPEN_STEAM_FILTER,wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if (openFileDialog->ShowModal()==wxID_CANCEL) {
			openFileDialog->Destroy();
			return;
		}
		wxString dirname = openFileDialog->GetPath();
		dirname.Truncate(dirname.find_last_of(L"/\\"));
		SetupRootPath(dirname);
		openFileDialog->Destroy();
	} else if (id==wxID_REOPEN) {
		if (root_path.Len()>0)
			SetupRootPath(root_path);
	} else if (id==wxID_CLOSE) {
		Close();
	} else {
		
		#define MACRO_OPENARCHIVE(PREFIX,SUFFIX) \
			if (id==PREFIX ## SUFFIX) DisplayArchive(UNITY_ARCHIVE_ ## SUFFIX);
		
		MACRO_OPENARCHIVE(wxID_P,DATA11)	MACRO_OPENARCHIVE(wxID_P,DATA12)	MACRO_OPENARCHIVE(wxID_P,DATA13)
		MACRO_OPENARCHIVE(wxID_P,DATA14)	MACRO_OPENARCHIVE(wxID_P,DATA15)	MACRO_OPENARCHIVE(wxID_P,DATA16)
		MACRO_OPENARCHIVE(wxID_P,DATA17)	MACRO_OPENARCHIVE(wxID_P,DATA18)	MACRO_OPENARCHIVE(wxID_P,DATA19)
		MACRO_OPENARCHIVE(wxID_P,DATA2)
		MACRO_OPENARCHIVE(wxID_P,DATA3)
		MACRO_OPENARCHIVE(wxID_P,DATA4)
		MACRO_OPENARCHIVE(wxID_P,DATA5)
		MACRO_OPENARCHIVE(wxID_P,DATA61)	MACRO_OPENARCHIVE(wxID_P,DATA62)	MACRO_OPENARCHIVE(wxID_P,DATA63)
		MACRO_OPENARCHIVE(wxID_P,DATA7)
		MACRO_OPENARCHIVE(wxID_,MAINDATA)
		MACRO_OPENARCHIVE(wxID_,RESOURCES)
/*		MACRO_OPENARCHIVE(wxID_,LEVEL0)		MACRO_OPENARCHIVE(wxID_,LEVEL1)		MACRO_OPENARCHIVE(wxID_,LEVEL2)		MACRO_OPENARCHIVE(wxID_,LEVEL3)
		MACRO_OPENARCHIVE(wxID_,LEVEL4)		MACRO_OPENARCHIVE(wxID_,LEVEL5)		MACRO_OPENARCHIVE(wxID_,LEVEL6)		MACRO_OPENARCHIVE(wxID_,LEVEL7)
		MACRO_OPENARCHIVE(wxID_,LEVEL8)		MACRO_OPENARCHIVE(wxID_,LEVEL9)		MACRO_OPENARCHIVE(wxID_,LEVEL10)	MACRO_OPENARCHIVE(wxID_,LEVEL11)
		MACRO_OPENARCHIVE(wxID_,LEVEL12)	MACRO_OPENARCHIVE(wxID_,LEVEL13)	MACRO_OPENARCHIVE(wxID_,LEVEL14)	MACRO_OPENARCHIVE(wxID_,LEVEL15)
		MACRO_OPENARCHIVE(wxID_,LEVEL16)	MACRO_OPENARCHIVE(wxID_,LEVEL17)	MACRO_OPENARCHIVE(wxID_,LEVEL18)	MACRO_OPENARCHIVE(wxID_,LEVEL19)
		MACRO_OPENARCHIVE(wxID_,LEVEL20)	MACRO_OPENARCHIVE(wxID_,LEVEL21)	MACRO_OPENARCHIVE(wxID_,LEVEL22)	MACRO_OPENARCHIVE(wxID_,LEVEL23)
		MACRO_OPENARCHIVE(wxID_,LEVEL24)	MACRO_OPENARCHIVE(wxID_,LEVEL25)	MACRO_OPENARCHIVE(wxID_,LEVEL26)	MACRO_OPENARCHIVE(wxID_,LEVEL27)
		MACRO_OPENARCHIVE(wxID_,SHARED0)	MACRO_OPENARCHIVE(wxID_,SHARED1)	MACRO_OPENARCHIVE(wxID_,SHARED2)	MACRO_OPENARCHIVE(wxID_,SHARED3)
		MACRO_OPENARCHIVE(wxID_,SHARED4)	MACRO_OPENARCHIVE(wxID_,SHARED5)	MACRO_OPENARCHIVE(wxID_,SHARED6)	MACRO_OPENARCHIVE(wxID_,SHARED7)
		MACRO_OPENARCHIVE(wxID_,SHARED8)	MACRO_OPENARCHIVE(wxID_,SHARED9)	MACRO_OPENARCHIVE(wxID_,SHARED10)	MACRO_OPENARCHIVE(wxID_,SHARED11)
		MACRO_OPENARCHIVE(wxID_,SHARED12)	MACRO_OPENARCHIVE(wxID_,SHARED13)	MACRO_OPENARCHIVE(wxID_,SHARED14)	MACRO_OPENARCHIVE(wxID_,SHARED15)
		MACRO_OPENARCHIVE(wxID_,SHARED16)	MACRO_OPENARCHIVE(wxID_,SHARED17)	MACRO_OPENARCHIVE(wxID_,SHARED18)	MACRO_OPENARCHIVE(wxID_,SHARED19)
		MACRO_OPENARCHIVE(wxID_,SHARED20)	MACRO_OPENARCHIVE(wxID_,SHARED21)	MACRO_OPENARCHIVE(wxID_,SHARED22)	MACRO_OPENARCHIVE(wxID_,SHARED23)
		MACRO_OPENARCHIVE(wxID_,SHARED24)	MACRO_OPENARCHIVE(wxID_,SHARED25)	MACRO_OPENARCHIVE(wxID_,SHARED26)	MACRO_OPENARCHIVE(wxID_,SHARED27)
		MACRO_OPENARCHIVE(wxID_,SHARED28)*/
	}
}

void ToolUnityViewer::OnSortColumn(wxListEvent& event) {
	int col = event.GetColumn();
	SortItemInfo info;
	unsigned int i;
	long it;
	if (col==column_sort)
		column_sort_ascending = !column_sort_ascending;
	else
		column_sort_ascending = true;
	column_sort = col;
	info.assetlist = (wxListCtrl*)m_assetlist;
	info.column = column_sort;
	info.ascending = column_sort_ascending;
	m_assetlist->SortItems(SortItemCompare,(wxIntPtr)&info);
	it = m_assetlist->GetTopItem();
	for (i=0;i<m_assetlist->GetItemCount();i++) {
		m_assetlist->SetItemData(it,i);
		it = m_assetlist->GetNextItem(it);
	}
}
