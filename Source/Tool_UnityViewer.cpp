#include "Tool_UnityViewer.h"

#include <wx/tokenzr.h>
#include "Squish/squish.h"
#include "Hades_Strings.h"
#include "Database_Text.h"
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
	m_loadgauge->SetRange(100);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_INDEX),wxLIST_FORMAT_LEFT,40);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_NAME),wxLIST_FORMAT_LEFT,300);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_INTERNAME),wxLIST_FORMAT_LEFT,120);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_TYPE),wxLIST_FORMAT_LEFT,90);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_SIZE),wxLIST_FORMAT_LEFT,50);
	m_assetlist->AppendColumn(_(ASSET_COLUMN_INFO),wxLIST_FORMAT_LEFT,200);
	current_archive = UNITY_ARCHIVE_AMOUNT;
	archive_name = wxEmptyString;
	root_path = wxEmptyString;
	root_path_ok = false;
	column_sort = 0;
	column_sort_ascending = true;
	use_x86 = false;
	assetmenu = new wxMenu();
	assetmenuexport = new wxMenuItem(assetmenu,wxID_EXPORT,HADES_STRING_GENERIC_EXPORT_SEL);
	assetmenuimport = new wxMenuItem(assetmenu,wxID_IMPORT,HADES_STRING_GENERIC_IMPORT_SEL);
	assetmenu->Append(assetmenuexport);
	assetmenu->Append(assetmenuimport);
	assetmenu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ToolUnityViewer::OnAssetRightClickMenu),NULL,this);
	UpdateMenuAvailability();
}

ToolUnityViewer::~ToolUnityViewer() {
	assetmenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ToolUnityViewer::OnAssetRightClickMenu),NULL,this);
}

bool ToolUnityViewer::SetupRootPath(wxString path, bool ignorestreaming) {
	wxString filename;
	uint32_t offset;
	unsigned int i,j;
	unsigned int starti = ignorestreaming ? (unsigned int)UNITY_ARCHIVE_DATA7+1 : 0;
	if (path.Last()!=L'\\')
		path += L"\\";
	// Unload the previous root path
	if (root_path_ok) {
		m_assetlist->DeleteAllItems();
		archive_name = wxEmptyString;
		for (i=starti;i<UNITY_ARCHIVE_AMOUNT;i++) {
			meta_data[i].Flush();
			if (i>=UNITY_ARCHIVE_DATA11 && i<=UNITY_ARCHIVE_DATA7)
				bundle_data[i-UNITY_ARCHIVE_DATA11].Flush();
		}
		if (starti<=UNITY_ARCHIVE_MAINDATA) {
			list_data.Flush();
			list_data_filename.Empty();
		}
	}
	// Verify that the required files are available
	for (i=starti;i<UNITY_ARCHIVE_AMOUNT;i++) {
		filename = path+UnityArchiveMetaData::GetArchiveName((UnityArchiveFile)i,use_x86);
		if (!wxFile::Exists(filename)) {
			wxLogError(HADES_STRING_OPEN_ERROR_NONEXISTENT,filename);
			root_path_ok = false;
			UpdateMenuAvailability();
			return root_path_ok;
		} else if (!wxFile::Access(filename,wxFile::read)) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,filename);
			root_path_ok = false;
			UpdateMenuAvailability();
			return root_path_ok;
		}
	}
	// Popup a warning if the game is opened in HW's main module
	for (i=0;i<GetTopWindow()->CDPanelAmount;i++)
		if (path.IsSameAs(GetTopWindow()->CDPanel[i]->filename)) {
			wxMessageDialog popupwarning(this,HADES_STRING_UNITYVIEWER_GAME_OPEN,HADES_STRING_WARNING,wxOK|wxCENTRE);
			popupwarning.ShowModal();
		}
	// Read the asset lists
	root_path = path;
	m_loadgauge->SetValue(0);
	for (i=starti;i<UNITY_ARCHIVE_AMOUNT;i++) {
		filename = path+UnityArchiveMetaData::GetArchiveName((UnityArchiveFile)i,use_x86);
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
				list_data_filename.Alloc(list_data.amount);
				for (j=0;j<list_data.amount;j++)
					list_data_filename.Add(_(list_data.path[j]).AfterLast(L'/'));
			}
		}
		unityarchive.close();
		m_loadgauge->SetValue((i+1-starti)*100/(UNITY_ARCHIVE_AMOUNT-starti));
	}
	m_loadgauge->SetValue(100);
	root_path_ok = true;
	UpdateMenuAvailability();
	return root_path_ok;
}

bool ToolUnityViewer::DisplayArchive(UnityArchiveFile archivetype) {
	unsigned int i,j;
	long itemid;
	bool foundfullname;
	wxString fullname,infostr,typestr;
	m_assetlist->DeleteAllItems();
	current_archive = archivetype;
	archive_name = _(UnityArchiveMetaData::GetArchiveName((UnityArchiveFile)current_archive,use_x86));
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
				fullname = _(L"NoName")+_(ConvertToString(i+1));
			}
		}
		if (foundfullname)
			infostr = GetInfoString(fullname,meta_data[archivetype].file_type1[i],current_archive);
		else
			infostr = _(L"");
		typestr = _(UnityArchiveMetaData::GetTypeName(meta_data[archivetype].file_type1[i]));
		if (typestr.Len()>0)
			typestr = _(ConvertToString(meta_data[archivetype].file_type1[i]))+_(L" (")+typestr+_(L")");
		else
			typestr = _(ConvertToString(meta_data[archivetype].file_type1[i]));
		itemid = m_assetlist->InsertItem(i,_(ConvertToString(i+1)));
		m_assetlist->SetItem(itemid,1,fullname);
		m_assetlist->SetItem(itemid,2,_(meta_data[archivetype].file_name[i]));
		m_assetlist->SetItem(itemid,3,typestr);
		m_assetlist->SetItem(itemid,4,_(ConvertToString(meta_data[archivetype].GetFileSizeByIndex(i))));
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
	if (archive>=UNITY_ARCHIVE_DATA11 && archive<=UNITY_ARCHIVE_DATA19) {
		if (patharray.Count()>=5 && patharray[2].IsSameAs(L"fieldmaps",false)) {
			wxString fldid = patharray[3];
			wxString partstr = _(L"[Unknown Field]");
			for (unsigned int i=0;i<G_N_ELEMENTS(SteamFieldScript);i++)
				if (fldid.IsSameAs(SteamFieldScript[i].background_name,false)) {
					partstr = _(SteamFieldScript[i].default_name)+_(L" (")+_(ConvertToString(SteamFieldScript[i].script_id))+_(L")");
					break;
				}
			if (ext.IsSameAs(L"bgs.bytes",false)) return _(L"Tileset (BG_SCENE) of the field ")+partstr;
			if (ext.IsSameAs(L"bgi.bytes",false)) return _(L"Walkmesh of the field ")+partstr;
			if (ext.IsSameAs(L"sps.bytes",false)) return _(L"Special Effect Model for the field ")+partstr;
			if (ext.IsSameAs(L"tcb.bytes",false)) return _(L"Texture of Special Effects for the field ")+partstr;
			if (ext.IsSameAs(L"png",false)) return _(L"Atlas Texture of the field ")+partstr;
			return _(UnityArchiveMetaData::GetTypeName(filetype))+_(L" of the field ")+partstr;
		}
	} else if (archive==UNITY_ARCHIVE_DATA2) {
		if (patharray.Count()>=7 && patharray[2].IsSameAs(L"battlemap",false) && patharray[3].IsSameAs(L"battlemodel",false) && patharray[4].IsSameAs(L"6",false)) {
			unsigned int id = wxAtoi(patharray[5]);
			wxString partstr = _(L"[Unknown Model]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++)
				if (HADES_STRING_MODEL_NAME[i].id==id) {
					partstr = HADES_STRING_MODEL_NAME[i].label;
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
					partstr = HADES_STRING_BATTLE_SCENE_NAME[i].label;
					break;
				}
			if (patharray.Count()==5 && patharray[3].IsSameAs(L"battleinfo",false)) return _(L"Info for the battle scene ")+partstr;
			if (patharray.Count()==5) return _(L"Texture Animation for the battle scene ")+partstr;
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
	} else if (archive==UNITY_ARCHIVE_DATA4) {
		if (patharray.Count()>=6 && patharray[2].IsSameAs(L"models",false) && wxAtoi(patharray[3])>0) {
			unsigned int modeltype = wxAtoi(patharray[3])<G_N_ELEMENTS(DATABASE_MODEL_TYPE) ? wxAtoi(patharray[3]) : 0;
			unsigned int id = wxAtoi(patharray[4]);
			wxString partstr = _(L"[Unknown Model]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++)
				if (HADES_STRING_MODEL_NAME[i].id==id) {
					partstr = HADES_STRING_MODEL_NAME[i].label;
					break;
				}
			return _(UnityArchiveMetaData::GetTypeName(filetype))+_(L" for the ")+DATABASE_MODEL_TYPE[modeltype]+_(L" model ")+partstr;
		} else if (patharray.Count()==5 && patharray[2].IsSameAs(L"models",false) && patharray[3].IsSameAs(L"geotexanim",false)) {
			if (name.Mid(0,4).IsSameAs(L"geo_",false)) {
				wxString partstr = _(L"[Unknown Model]");
				// ToDo
				return _(L"Texture Animation for the model ")+partstr;
			} else if (name.Mid(0,4).IsSameAs(L"tam_",false)) {
				wxString btlid = name.Mid(4).BeforeFirst(L'_').BeforeFirst(L'.');
				wxString partstr = _(L"[Unknown Battle Scene]");
				for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME);i++)
					if (btlid.IsSameAs(HADES_STRING_BATTLE_SCENE_NAME[i].steamid,false)) {
						partstr = HADES_STRING_BATTLE_SCENE_NAME[i].label;
						break;
					}
				return _(L"Texture Animation for the battle scene ")+partstr;
			}
		}
	} else if (archive==UNITY_ARCHIVE_DATA5) {
		if (patharray.Count()==5 && patharray[2].IsSameAs(L"animations",false)) {
			unsigned int modelid = wxAtoi(patharray[3]);
			unsigned int animid = wxAtoi(patharray[4]);
			wxString partstrmodel = _(L"[Unknown Model]");
			wxString partstranim = _(L"[Unknown Animation]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++)
				if (HADES_STRING_MODEL_NAME[i].id==modelid) {
					partstrmodel = HADES_STRING_MODEL_NAME[i].label;
					break;
				}
			// ToDo
			return _(L"Animation ")+partstranim+_(L" of the model ")+partstrmodel;
		}
	} else if (archive==UNITY_ARCHIVE_DATA7) {
		if (patharray.Count()>=4 && patharray[2].IsSameAs(L"commonasset",false) && /*
		*/ ((patharray.Count()==5 && patharray[3].IsSameAs(L"vibrationdata",false)) || /*
		*/ (patharray.Count()==5 && patharray[3].IsSameAs(L"mapconfigdata",false)) || /*
		*/ (patharray.Count()==6 && patharray[3].IsSameAs(L"eventengine",false) && patharray[4].IsSameAs(L"eventanimation",false)) || /*
		*/ (patharray.Count()==8 && patharray[3].IsSameAs(L"eventengine",false) && patharray[4].IsSameAs(L"eventbinary",false) && patharray[5].IsSameAs(L"field",false)))) {
			wxString fldid = name.BeforeFirst(L'.');
			wxString partstr = _(L"[Unknown Field]");
			for (unsigned int i=0;i<G_N_ELEMENTS(SteamFieldScript);i++)
				if (fldid.IsSameAs(SteamFieldScript[i].script_name,false)) {
					partstr = _(SteamFieldScript[i].default_name)+_(L" (")+_(ConvertToString(SteamFieldScript[i].script_id))+_(L")");
					break;
				}
			if (patharray.Count()==8) return _(L"Script of the field ")+partstr;
			if (patharray.Count()==6) return _(L"Model and Animation preloading list for the field ")+partstr;
			if (patharray[3].IsSameAs(L"vibrationdata",false)) return _(L"Vibration data for the field ")+partstr;
			return _(L"MCF Service of the field ")+partstr;
		} else if (patharray.Count()==8 && patharray[3].IsSameAs(L"eventengine",false) && patharray[4].IsSameAs(L"eventbinary",false) && patharray[5].IsSameAs(L"battle",false)) {
			wxString btlid = name.BeforeFirst(L'.');
			wxString partstr = _(L"[Unknown Battle]");
			for (unsigned int i=0;i<G_N_ELEMENTS(SteamBattleScript);i++)
				if (btlid.IsSameAs(SteamBattleScript[i].name,false)) {
					partstr = _(ConvertToString(SteamBattleScript[i].battle_id));
					break;
				}
			return _(L"AI Script of the battle ")+partstr;
		} else if (patharray.Count()==8 && patharray[3].IsSameAs(L"eventengine",false) && patharray[4].IsSameAs(L"eventbinary",false) && patharray[5].IsSameAs(L"world",false)) {
			wxString wldid = name.BeforeFirst(L'.');
			wxString partstr = _(L"[Unknown World Map]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_WORLD_BLOCK_NAME);i++)
				if (wldid.IsSameAs(HADES_STRING_WORLD_BLOCK_NAME[i].steamid,false)) {
					partstr = HADES_STRING_WORLD_BLOCK_NAME[i].label;
					break;
				}
			return _(L"Script of the ")+partstr;
		}
	} else if (archive==UNITY_ARCHIVE_RESOURCES) {
		if (patharray.Count()==4) {
			if (name.IsSameAs(L"AnimationFolderMapping.txt",false)) return _(L"");
			if (name.IsSameAs(L"aaaaBattleMapList.txt",false)) return _(L"");
			if (name.IsSameAs(L"BattleMapList.txt",false)) return _(L"");
			if (name.IsSameAs(L"BtlEncountBgmMetaData.txt",false)) return _(L"");
			if (name.IsSameAs(L"IconPathMap.txt",false)) return _(L"");
			if (name.IsSameAs(L"Licence_Amazon.txt",false)) return _(L"");
			if (name.IsSameAs(L"Licence_Android.txt",false)) return _(L"");
			if (name.IsSameAs(L"Licence_AndroidSqExMarket.txt",false)) return _(L"");
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
			if (name.IsSameAs(L"StaffCredits_AndroidSqExMarket.txt",false)) return _(L"");
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
		} else if (patharray.Count()==3) {
			if (name.IsSameAs(L"MINIGAME_CARD_DATA_ADDRESS",false)) return _(L"Tetra Master card statistics used by CardPool");
			if (name.IsSameAs(L"MINIGAME_STAGE_ADDRESS",false)) return _(L"Tetra Master [Deck ID, Difficulty] informations used by EnemyData");
			if (name.IsSameAs(L"MINIGAME_CARD_LEVEL_ADDRESS",false)) return _(L"Tetra Master lists of cards played by opponents used by EnemyData");
		} else if (patharray.Count()==5 && ext.IsSameAs(L"mes",false)) {
			if (name.IsSameAs(L"aa_name.mes",false)) return _(L"Names of the party's Active Abilities");
			if (name.IsSameAs(L"aa_help.mes",false)) return _(L"Helps of the party's Active Abilities");
			if (name.IsSameAs(L"sa_name.mes",false)) return _(L"Names of the party's Supporting Abilities");
			if (name.IsSameAs(L"sa_help.mes",false)) return _(L"Helps of the party's Supporting Abilities");
			if (name.IsSameAs(L"com_name.mes",false)) return _(L"Names of the party's Commands");
			if (name.IsSameAs(L"com_help.mes",false)) return _(L"Helps of the party's Commands");
			if (name.IsSameAs(L"follow.mes",false)) return _(L"UI texts in the category: ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[0];
			if (name.IsSameAs(L"libra.mes",false)) return _(L"UI texts in the category: ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[1];
			if (name.IsSameAs(L"cmdtitle.mes",false)) return _(L"UI texts in the category: ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[2];
			if (name.IsSameAs(L"ff9choco.mes",false)) return _(L"UI texts in the category: ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[3];
			if (name.IsSameAs(L"card.mes",false)) return _(L"UI texts in the category: ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[4];
			if (name.IsSameAs(L"minigame.mes",false)) return _(L"UI texts in the category: ")+HADES_STRING_SPECIAL_TEXT_BLOCK_STEAM[5];
			if (name.IsSameAs(L"minista.mes",false)) return _(L"Names of the Tetra Master cards");
			if (name.IsSameAs(L"worldloc.mes",false)) return _(L"Names of the World Map locations");
			if (name.IsSameAs(L"itm_name.mes",false)) return _(L"Names of the regular items");
			if (name.IsSameAs(L"itm_help.mes",false)) return _(L"Helps of the regular items");
			if (name.IsSameAs(L"itm_btl.mes",false)) return _(L"Battle helps of the regular items");
			if (name.IsSameAs(L"imp_name.mes",false)) return _(L"Names of the key items");
			if (name.IsSameAs(L"imp_help.mes",false)) return _(L"Helps of the key items");
			if (name.IsSameAs(L"imp_skin.mes",false)) return _(L"Descriptions of the key items");
			if (name.IsSameAs(L"loc_name.mes",false)) return _(L"Names of the different fields");
			if (patharray[3].IsSameAs(L"field",false)) {
				unsigned int id = wxAtoi(name.BeforeFirst(L'.'));
				wxString partstr = _(L"[Unknown Text Block]");
				for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_TEXT_BLOCK_NAME);i++)
					if (HADES_STRING_TEXT_BLOCK_NAME[i].id==id) {
						partstr = HADES_STRING_TEXT_BLOCK_NAME[i].label;
						break;
					}
				return _(L"Dialogs and Texts of ")+partstr;
			} else if (patharray[3].IsSameAs(L"battle",false)) {
				unsigned int id = wxAtoi(name.BeforeFirst(L'.'));
				return _(L"Texts from the battle ")+_(ConvertToString(id));
			}
		} else if (patharray.Count()==2 && patharray[0].IsSameAs(L"specialeffects",false) && name.Mid(0,2).IsSameAs(L"ef",false)) {
			unsigned int id = wxAtoi(name.Mid(2));
			wxString partstr = _(L"[Unknown Spell Animation]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_SPELL_MODEL);i++)
				if (HADES_STRING_SPELL_MODEL[i].id==id) {
					partstr = HADES_STRING_SPELL_MODEL[i].label;
					break;
				}
			return _(L"Special Effect of the spell animation ")+partstr;
		}
	}
	
	return _(L"");
}

void ToolUnityViewer::UpdateMenuAvailability() {
	wxMenuItemList& menuassets = m_menuassets->GetMenuItems();
	wxMenuItemList::iterator iter;
	for (iter=menuassets.begin();iter!=menuassets.end();iter++)
		(*iter)->Enable(root_path_ok);
	m_menureopen->Enable(root_path_ok);
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
		if (root_path_ok)
			SetupRootPath(root_path);
	} else if (id==wxID_CLOSE) {
		Close();
	} else if (id==wxID_FOLDER64 || id==wxID_FOLDER86) {
		use_x86 = id==wxID_FOLDER86;
		if (root_path_ok)
			SetupRootPath(root_path,true);
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
		MACRO_OPENARCHIVE(wxID_,LEVEL0)		MACRO_OPENARCHIVE(wxID_,LEVEL1)		MACRO_OPENARCHIVE(wxID_,LEVEL2)		MACRO_OPENARCHIVE(wxID_,LEVEL3)
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
		MACRO_OPENARCHIVE(wxID_,SHARED28)
	}
}

void ToolUnityViewer::OnAssetRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	if (id==wxID_EXPORT) {
		fstream filebase((root_path+archive_name).c_str(),ios::in|ios::binary);
		if (!filebase.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,root_path+archive_name);
			return;
		}
		unsigned int itcounter = 0, itamount = m_assetlist->GetSelectedItemCount();
		unsigned int expfileid, expfilesize;
		wxString basepath = root_path+_(L"HadesWorkshopAssets\\")+archive_name.AfterLast(L'\\').BeforeFirst(L'.')+_(L"\\");
		wxString path;
		char* buffer;
		long it = -1;
		for (;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			path = m_assetlist->GetItemText(it,1);
			path.Replace(_(L"/"),_(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath+path;
			wxFileName::Mkdir(path.BeforeLast(L'\\'),wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
			fstream filedest(path.c_str(),ios::out|ios::binary);
			if (!filedest.is_open()) {
				wxLogError(HADES_STRING_OPEN_ERROR_CREATE,path);
				continue;
			}
			expfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
			expfilesize = meta_data[current_archive].GetFileSizeByIndex(expfileid);
			filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid));
			buffer = new char[expfilesize];
			filebase.read(buffer,expfilesize);
			if (meta_data[current_archive].file_type1[expfileid]==28 && !m_menuconvertimgnone->IsChecked()) {
				uint32_t imgw, imgh;
				uint8_t* imgrgba;
				bool success = TIMImageDataStruct::ConvertFromSteamTexture((uint8_t*)buffer,&imgw,&imgh,&imgrgba);
				delete[] buffer;
				filedest.close();
				if (!success) {
					wxLogError(_(L"Format of '%s' not supported"),m_assetlist->GetItemText(it,1));
					continue;
				}
				unsigned char* imgrgb = (unsigned char*)malloc(3*imgw*imgh*sizeof(unsigned char));
				unsigned char* imgalpha = (unsigned char*)malloc(imgw*imgh*sizeof(unsigned char));
				for (unsigned int i=0;i<imgw*imgh;i++) {
					imgrgb[3*i] = imgrgba[4*i];
					imgrgb[3*i+1] = imgrgba[4*i+1];
					imgrgb[3*i+2] = imgrgba[4*i+2];
					imgalpha[i] = imgrgba[4*i+3];
				}
				delete[] imgrgba;
				wxImage img(imgw,imgh,imgrgb,imgalpha);
				if (m_menuconvertimgpng->IsChecked())		img.SaveFile(path,wxBITMAP_TYPE_PNG);
				else if (m_menuconvertimgtga->IsChecked())	img.SaveFile(path,wxBITMAP_TYPE_TGA);
				else if (m_menuconvertimgtiff->IsChecked())	img.SaveFile(path,wxBITMAP_TYPE_TIFF);
				else										img.SaveFile(path,wxBITMAP_TYPE_BMP);
			} else if (meta_data[current_archive].file_type1[expfileid]==49 && path.Len()>=10 && path.Mid(path.Len()-10).IsSameAs(L".akb.bytes",false) && !m_menuconvertaudionone->IsChecked()) {
				uint32_t magicakb;
				BufferInitPosition();
				BufferReadLong((uint8_t*)buffer,magicakb);
				if (magicakb==0x32424B41)
					filedest.write(&buffer[0x130],expfilesize-0x130);
				else
					filedest.write(buffer,expfilesize);
				filedest.close();
				delete[] buffer;
			} else {
				filedest.write(buffer,expfilesize);
				filedest.close();
				delete[] buffer;
			}
			m_loadgauge->SetValue((++itcounter)*100/itamount);
		}
		filebase.close();
		m_loadgauge->SetValue(100);
	} else if (id==wxID_IMPORT) {
		fstream filebase((root_path+archive_name).c_str(),ios::in|ios::binary);
		fstream filedest((root_path+archive_name+_(L".tmp")).c_str(),ios::out|ios::binary);
		bool overwritefile = true;
		if (!filebase.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,root_path+archive_name);
			return;
		}
		if (!filedest.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE,root_path+archive_name+_(L".tmp"));
			return;
		}
		m_loadgauge->SetValue(0);
		unsigned int itcounter = 0, itsuccesscounter = 0, itamount = m_assetlist->GetSelectedItemCount();
		unsigned int expfileid, expfilesize;
		unsigned int i;
		wxString basepath = root_path+_(L"HadesWorkshopAssets\\")+archive_name.AfterLast(L'\\').BeforeFirst(L'.')+_(L"\\");
		wxString path;
		char* buffer;
		bool* copylist = new bool[meta_data[current_archive].header_file_amount];
		uint32_t* filenewsize = new uint32_t[meta_data[current_archive].header_file_amount];
		for (i=0;i<meta_data[current_archive].header_file_amount;i++) {
			copylist[i] = true;
			filenewsize[i] = meta_data[current_archive].file_size[i];
		}
		long it = -1;
		for (;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			path = m_assetlist->GetItemText(it,1);
			path.Replace(_(L"/"),_(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath+path;
			if (!wxFileName::FileExists(path))
				continue;
			expfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
			if (meta_data[current_archive].file_type1[expfileid]==28 && !m_menuconvertimgnone->IsChecked()) {
				wxImage img;
				bool success;
				if (m_menuconvertimgpng->IsChecked())		success = img.LoadFile(path,wxBITMAP_TYPE_PNG);
				else if (m_menuconvertimgtga->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TGA);
				else if (m_menuconvertimgtiff->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TIFF);
				else										success = img.LoadFile(path,wxBITMAP_TYPE_BMP);
				if (!success)								success = img.LoadFile(path,wxBITMAP_TYPE_ANY);
				if (!success) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL_FORMAT,path);
					continue;
				}
				uint32_t textureformat = 0;
				if (m_menuconvertimgqualityalpha->IsChecked())		textureformat = 0x01;
				else if (m_menuconvertimgqualityrgb->IsChecked())	textureformat = 0x03;
				else if (m_menuconvertimgqualityrgba->IsChecked())	textureformat = 0x04;
				else if (m_menuconvertimgqualityargb->IsChecked())	textureformat = 0x05;
				else if (m_menuconvertimgqualitydxt1->IsChecked())	textureformat = 0x0A;
				else if (m_menuconvertimgqualitydxt5->IsChecked())	textureformat = 0x0C;
				else if (meta_data[current_archive].GetFileSizeByIndex(expfileid)>=0x10) {
					filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid)+0xC);
					textureformat = ReadLong(filebase);
				}
				uint32_t newsizetmp = TIMImageDataStruct::GetSteamTextureFileSize(img.GetWidth(),img.GetHeight(),textureformat);
				if (newsizetmp==0) {
					wxLogError(HADES_STRING_UNITYVIEWER_IMPORT_ERROR_FORMAT,path);
					continue;
				}
				filenewsize[expfileid] = newsizetmp;
			} else if (meta_data[current_archive].file_type1[expfileid]==49 && path.Len()>=10 && path.Mid(path.Len()-10).IsSameAs(L".akb.bytes",false) && !m_menuconvertaudionone->IsChecked()) {
				filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid));
				uint32_t magicakb = ReadLong(filebase);
				if (magicakb!=0x32424B41) {
					wxLogError(HADES_STRING_UNITYVIEWER_IMPORT_ERROR_FORMAT,path);
					continue;
				}
				fstream fileasset(path.c_str(),ios::in|ios::binary);
				if (!fileasset.is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
					continue;
				}
				uint32_t magicogg = ReadLong(fileasset);
				if (magicogg!=0x5367674F) {
					wxLogError(HADES_STRING_UNITYVIEWER_MISSING_OGG,path);
					continue;
				}
				fileasset.seekg(0,ios::end);
				filenewsize[expfileid] = (uint32_t)fileasset.tellg()+0x130;
				fileasset.close();
			} else {
				fstream fileasset(path.c_str(),ios::in|ios::binary);
				if (!fileasset.is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
					continue;
				}
				fileasset.seekg(0,ios::end);
				filenewsize[expfileid] = fileasset.tellg();
				fileasset.close();
			}
			copylist[expfileid] = false;
		}
		m_loadgauge->SetValue(10);
		filebase.seekg(0);
		uint32_t* unityfileoff = meta_data[current_archive].Duplicate(filebase,filedest,copylist,filenewsize,overwritefile);
		m_loadgauge->SetValue(50);
		for (it = -1;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			itcounter++;
			expfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
			if (copylist[expfileid])
				continue;
			path = m_assetlist->GetItemText(it,1);
			path.Replace(_(L"/"),_(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath+path;
			if (meta_data[current_archive].file_type1[expfileid]==28 && !m_menuconvertimgnone->IsChecked()) {
				wxImage img;
				bool success;
				if (m_menuconvertimgpng->IsChecked())		success = img.LoadFile(path,wxBITMAP_TYPE_PNG);
				else if (m_menuconvertimgtga->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TGA);
				else if (m_menuconvertimgtiff->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TIFF);
				else										success = img.LoadFile(path,wxBITMAP_TYPE_BMP);
				if (!success)								success = img.LoadFile(path,wxBITMAP_TYPE_ANY);
				if (!success) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL_FORMAT,path);
					overwritefile = false;
					continue;
				}
				uint32_t datasize, pixelindex;
				unsigned int x,y,w = img.GetWidth(), h = img.GetHeight();
				uint8_t* rgba = new uint8_t[w*h*4];
				bool hasalpha = img.HasAlpha();
				for (x=0;x<w;x++)
					for (y=0;y<h;y++) {
						pixelindex = (x+(y*w))*4;
						rgba[pixelindex] = img.GetRed(x,y);
						rgba[pixelindex+1] = img.GetGreen(x,y);
						rgba[pixelindex+2] = img.GetBlue(x,y);
						rgba[pixelindex+3] = hasalpha ? img.GetAlpha(x,y) : 0xFF;
					}
				uint32_t textureformat = 0;
				int quality =	m_menuconvertimgqualitylow->IsChecked() ? 0 :
								m_menuconvertimgqualitymedium->IsChecked() ? 1 :
								m_menuconvertimgqualityhigh->IsChecked() ? 2 : -1;
				if (m_menuconvertimgqualityalpha->IsChecked())		textureformat = 0x01;
				else if (m_menuconvertimgqualityrgb->IsChecked())	textureformat = 0x03;
				else if (m_menuconvertimgqualityrgba->IsChecked())	textureformat = 0x04;
				else if (m_menuconvertimgqualityargb->IsChecked())	textureformat = 0x05;
				else if (m_menuconvertimgqualitydxt1->IsChecked())	textureformat = 0x0A;
				else if (m_menuconvertimgqualitydxt5->IsChecked())	textureformat = 0x0C;
				else if (meta_data[current_archive].GetFileSizeByIndex(expfileid)>=0x10) {
					filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid)+0xC);
					textureformat = ReadLong(filebase);
				}
				buffer = (char*)TIMImageDataStruct::CreateSteamTextureFile(datasize,w,h,rgba,textureformat,quality);
				delete[] rgba;
				filedest.seekg(unityfileoff[expfileid]);
				filedest.write(buffer,datasize);
				delete[] buffer;
			} else if (meta_data[current_archive].file_type1[expfileid]==49 && path.Len()>=10 && path.Mid(path.Len()-10).IsSameAs(L".akb.bytes",false) && !m_menuconvertaudionone->IsChecked()) {
				fstream fileasset(path.c_str(),ios::in|ios::binary);
				if (!fileasset.is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
					overwritefile = false;
					continue;
				}
				uint32_t newsizeogg = filenewsize[expfileid]-0x130;
				uint32_t newsizeoggreal = newsizeogg;
				filedest.seekg(unityfileoff[expfileid]);
				filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid));
				buffer = new char[filenewsize[expfileid]];
				filebase.read(buffer,0x130);
				fileasset.read(&buffer[0x130],newsizeogg);
				for (i=filenewsize[expfileid]-1;buffer[i]==0 && i>0x130;i--)
					newsizeoggreal--;
				BufferInitPosition(8);		BufferWriteLong((uint8_t*)buffer,filenewsize[expfileid]);
//				BufferInitPosition(0x28);	BufferWriteLong((uint8_t*)buffer,akbid); // ToDo: be able to retrieve this kind of info without the old file
//				BufferInitPosition(0xE2);	BufferWriteShort((uint8_t*)buffer,sound/music?);
//				BufferInitPosition(0xE6);	BufferWriteShort((uint8_t*)buffer,samplerate at 0x28 of Ogg);
				BufferInitPosition(0xE8);	BufferWriteLong((uint8_t*)buffer,newsizeoggreal);
/*				BufferWriteLong((uint8_t*)buffer,newsizeoggreal); // ???
				BufferWriteLong((uint8_t*)buffer,newsizeoggreal); // LoopStart
				BufferWriteLong((uint8_t*)buffer,newsizeoggreal); // LoopEnd
				BufferInitPosition(0x124);	BufferWriteLong((uint8_t*)buffer,newsizeoggreal); // ???*/
				filedest.write(buffer,filenewsize[expfileid]);
				fileasset.close();
				delete[] buffer;
			} else {
				fstream fileasset(path.c_str(),ios::in|ios::binary);
				if (!fileasset.is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
					overwritefile = false;
					continue;
				}
				buffer = new char[filenewsize[expfileid]];
				fileasset.read(buffer,filenewsize[expfileid]);
				filedest.seekg(unityfileoff[expfileid]);
				filedest.write(buffer,filenewsize[expfileid]);
				fileasset.close();
				delete[] buffer;
			}
			m_loadgauge->SetValue(50+itcounter*50/itamount);
			itsuccesscounter++;
		}
		delete[] copylist;
		delete[] filenewsize;
		delete[] unityfileoff;
		filebase.close();
		filedest.close();
		m_loadgauge->SetValue(100);
		if (overwritefile)
			overwritefile = wxRenameFile(root_path+archive_name+_(L".tmp"),root_path+archive_name,true);
		if (!overwritefile) {
			meta_data[current_archive].Flush();
			fstream unityarchive((root_path+archive_name).c_str(),ios::in | ios::binary);
			meta_data[current_archive].Load(unityarchive);
			if (current_archive>=UNITY_ARCHIVE_DATA11 && current_archive<=UNITY_ARCHIVE_DATA7) {
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Flush();
				uint32_t offset = meta_data[current_archive].GetFileOffset("",142);
				if (offset>0) {
					unityarchive.seekg(offset);
					bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Load(unityarchive);
				}
			}
			if (current_archive==UNITY_ARCHIVE_MAINDATA) {
				list_data.Flush();
				list_data_filename.Empty();
				uint32_t offset = meta_data[current_archive].GetFileOffset("",147);
				if (offset>0) {
					unityarchive.seekg(offset);
					list_data.Load(unityarchive);
					list_data_filename.Alloc(list_data.amount);
					for (i=0;i<list_data.amount;i++)
						list_data_filename.Add(_(list_data.path[i]).AfterLast(L'/'));
				}
			}
			unityarchive.close();
		}
		for (it = -1;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			expfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
			if (copylist[expfileid])
				continue;
			m_assetlist->SetItem(it,4,_(ConvertToString(meta_data[current_archive].GetFileSizeByIndex(expfileid))));
		}
		if (itsuccesscounter==itamount) {
			wxString successtring;
			successtring.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_SUCCESS),itsuccesscounter);
			wxMessageDialog popupsuccess(this,successtring,HADES_STRING_SUCCESS,wxOK|wxCENTRE);
			popupsuccess.ShowModal();
		} else {
			wxString failstring;
			failstring.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_ERROR),itsuccesscounter,itamount-itsuccesscounter);
			wxMessageDialog popupfail(this,failstring,HADES_STRING_WARNING,wxOK|wxCENTRE);
			popupfail.ShowModal();
		}
	}
}

void ToolUnityViewer::OnAssetRightClick(wxListEvent& event) {
	unsigned int i;
	bool enableimport = true;
	for (i=0;i<GetTopWindow()->CDPanelAmount;i++)
		if (root_path.IsSameAs(GetTopWindow()->CDPanel[i]->filename)) {
			enableimport = false;
			break;
		}
	assetmenuimport->Enable(enableimport);
	m_assetlist->PopupMenu(assetmenu);
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
