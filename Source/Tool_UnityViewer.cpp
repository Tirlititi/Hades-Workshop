#include "Tool_UnityViewer.h"

#include <wx/tokenzr.h>
#include <list>
#include "Squish/squish.h"
#include "Hades_Strings.h"
#include "Database_Text.h"
#include "Database_Resource.h"
#include "Database_SpellAnimation.h"
#include "Database_Animation.h"
#include "Gui_Preferences.h"
#include "main.h"

#define ASSET_COLUMN_INDEX		L"#"
#define ASSET_COLUMN_NAME		L"File Name"
#define ASSET_COLUMN_INTERNAME	L"Internal File Name"
#define ASSET_COLUMN_TYPE		L"Type"
#define ASSET_COLUMN_SIZE		L"Size"
#define ASSET_COLUMN_INFO		L"Infos"

// Conversion utility for non-formatted (without "0x") 16 characters-long hexadecimal strings
int64_t ConvertStringToLong(wxString value) {
	uint64_t charval, res = 0;
	uint8_t shift = 0;
	char c;
	unsigned int i;
	for (i=0;i<value.Len();i++) {
		c = value[value.Len()-i-1];
		if (c>='0' && c<='9')
			charval = c-'0';
		else if (c>='a' && c<='f')
			charval = c-'a'+10;
		else if (c>='A' && c<='F')
			charval = c-'A'+10;
		else
			charval = 0;
		res |= (charval << shift);
		shift += 4;
	}
	return *reinterpret_cast<int64_t*>(&res);
}

wxString ConvertLongToString(int64_t value) {
	uint64_t uvalue = *reinterpret_cast<int64_t*>(&value);
	wxString res = _(L"");
	uint8_t charval;
	unsigned int i;
	char c;
	for (i=0;i<16;i++) {
		charval = uvalue%0x10;
		if (charval<10)
			c = '0'+charval;
		else
			c = 'A'+(charval-10);
		res = c+res;
		uvalue >>= 4;
	}
	return res;
}

static string AudioFileNames[] = {
	"MusicMetaData.txt",
	"SongMetaData.txt",
	"SoundEffectMetaData.txt",
	"SoundEffectExtendedMetaData.txt",
	"MovieAudioMetaData.txt"
};

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

class AnimationTransitionWindow : public wxDialog {
public:
	wxButton* m_buttonok;
	wxButton* m_buttoncancel;
	wxButton* m_buttonswap;
	wxStaticText* m_swapslot1;
	wxStaticText* m_swapslot2;
	wxSpinCtrlDouble* m_spinduration;

	wxString swap_str1;
	wxString swap_str2;
	bool is_swap;

private:
	void OnButtonClickOk(wxCommandEvent& event) { EndModal(wxID_OK); }
	void OnButtonClickCancel(wxCommandEvent& event) { EndModal(wxID_CANCEL); }
	void OnButtonClickSwap(wxCommandEvent& event) {
		is_swap = !is_swap;
		m_swapslot1->SetLabelText(is_swap ? swap_str2 : swap_str1);
		m_swapslot2->SetLabelText(is_swap ? swap_str1 : swap_str2);
	}

public:
	AnimationTransitionWindow(wxWindow* parent, wxString str1, wxString str2, float duration) : wxDialog(parent, wxID_ANY, HADES_STRING_UNITYVIEWER_GENERATE_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP) {
		wxBoxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer* modalsizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* durationsizer = new wxBoxSizer(wxHORIZONTAL);
		wxBoxSizer* ordersizer = new wxBoxSizer(wxHORIZONTAL);
		m_buttonok = new wxButton(this, wxID_OK, _("OK"));
		m_buttoncancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
		m_buttonswap = new wxButton(this, wxID_REPLACE, _("<- Swap ->"));
		m_swapslot1 = new wxStaticText(this, wxID_ANY, str1);
		m_swapslot2 = new wxStaticText(this, wxID_ANY, str2);
		m_spinduration = new wxSpinCtrlDouble(this, wxID_FREQUENCE, _(L"Transition duration"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0.1, 100.0, duration, 1.0);
		modalsizer->Add(m_buttoncancel, 0, wxALL, 5);
		modalsizer->Add(m_buttonok, 0, wxALL, 5);
		durationsizer->Add(new wxStaticText(this, wxID_ANY, _(L"Transition duration")), 0, wxALL, 5);
		durationsizer->Add(m_spinduration, 0, wxALL, 5);
		ordersizer->Add(m_swapslot1, 0, wxALL, 5);
		ordersizer->Add(m_buttonswap, 0, wxALL, 5);
		ordersizer->Add(m_swapslot2, 0, wxALL, 5);
		mainsizer->Add(ordersizer);
		mainsizer->Add(durationsizer);
		mainsizer->Add(modalsizer);
		SetSizer(mainsizer);
		Layout();
		Fit();
		m_buttonok->SetFocus();
		is_swap = false;
		swap_str1 = str1;
		swap_str2 = str2;
		m_buttonok->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationTransitionWindow::OnButtonClickOk), NULL, this);
		m_buttoncancel->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationTransitionWindow::OnButtonClickCancel), NULL, this);
		m_buttonswap->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AnimationTransitionWindow::OnButtonClickSwap), NULL, this);
	}
};

ToolUnityViewer::ToolUnityViewer(wxWindow* parent) :
	UnityViewerWindow(parent) {
	PreferencesDialog::LoadToolUnityConfig(this);
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
	use_x86 = m_menufolderx86->IsChecked();
	assetmenu = new wxMenu();
	assetmenuexport = new wxMenuItem(assetmenu, wxID_EXPORT, HADES_STRING_GENERIC_EXPORT_SEL);
	assetmenuimport = new wxMenuItem(assetmenu, wxID_IMPORT, HADES_STRING_GENERIC_IMPORT_SEL);
	assetmenuadd = new wxMenuItem(assetmenu, wxID_ADD, HADES_STRING_GENERIC_ADD);
	assetmenuinvsel = new wxMenuItem(assetmenu, wxID_SELECTALL, HADES_STRING_UNITYVIEWER_INVERT_SELECTION);
	assetmenuinfo = new wxMenuItem(assetmenu, wxID_INFO, HADES_STRING_UNITYVIEWER_EXPORT_HIERARCHY);
	assetmenugenanim = new wxMenuItem(assetmenu, wxID_DUPLICATE, HADES_STRING_UNITYVIEWER_GENERATE_ANIMATION);
	assetmenuremove = new wxMenuItem(assetmenu, wxID_REMOVE, HADES_STRING_UNITYVIEWER_REMOVE_ASSETS);
	assetmenu->Append(assetmenuexport);
	assetmenu->Append(assetmenuimport);
	assetmenu->Append(assetmenuadd);
	assetmenu->Append(assetmenuinvsel);
	assetmenu->Append(assetmenuinfo);
	// Better let people use .json text editing than providing this feature which is too simple and may be confusing
//	assetmenu->Append(assetmenugenanim);
	assetmenu->AppendSeparator();
	assetmenu->Append(assetmenuremove);
	assetmenu->Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ToolUnityViewer::OnAssetRightClickMenu),NULL,this);
	UpdateMenuAvailability();
}

ToolUnityViewer::~ToolUnityViewer() {
	PreferencesDialog::SaveToolUnityConfig(this);
	assetmenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ToolUnityViewer::OnAssetRightClickMenu),NULL,this);
}

bool ToolUnityViewer::SetupRootPath(wxString path, bool ignorestreaming) {
	wxString filename;
	uint32_t offset;
	int32_t fileindex;
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
		}
		if (starti<=UNITY_ARCHIVE_MAINDATA) {
			list_data.Flush();
		}
		if (starti<=UNITY_ARCHIVE_RESOURCES) {
			audio_data.Flush();
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
		fstream unityarchive((const char*)filename.c_str(),ios::in | ios::binary);
		meta_data[i].Load(unityarchive,(UnityArchiveFile)i);
		if (i>=UNITY_ARCHIVE_DATA11 && i<=UNITY_ARCHIVE_DATA7) {
			offset = meta_data[i].GetFileOffset("",142);
			if (offset>0) {
				unityarchive.seekg(offset);
				bundle_data[i-UNITY_ARCHIVE_DATA11].Load(unityarchive);
			}
		} else if (i==UNITY_ARCHIVE_MAINDATA) {
			fileindex = meta_data[i].GetFileIndex("",147);
			if (fileindex>=0) {
				unityarchive.seekg(meta_data[i].GetFileOffsetByIndex(fileindex));
				list_data.Load(unityarchive,meta_data[i].GetFileSizeByIndex(fileindex));
			}
		} else if (i==UNITY_ARCHIVE_RESOURCES) {
			for (j=0;j<G_N_ELEMENTS(AudioFileNames);j++) {
				offset = meta_data[i].GetFileOffset(AudioFileNames[j],49);
				if (offset>0) {
					unityarchive.seekg(offset);
					audio_data.Load(unityarchive,true);
				}
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
	wxString typestr, fullname, infostr;
	bool foundfullname;
	unsigned int i;
	int itemid;
	current_archive = archivetype;
	archive_name = _(UnityArchiveMetaData::GetArchiveName(current_archive,use_x86));
	column_sort = 0;
	column_sort_ascending = true;
	m_assetlist->DeleteAllItems();
	for (i=0;i<meta_data[current_archive].header_file_amount;i++) {
		fullname = GetFullName(current_archive,i,&foundfullname);
		infostr = foundfullname ? GetInfoString(fullname,meta_data[current_archive].file_type1[i],current_archive) : _(L"");
		typestr = _(UnityArchiveMetaData::GetTypeName(meta_data[current_archive].file_type1[i]));
		if (typestr.Len()>0)
			typestr = _(ConvertToString(meta_data[current_archive].file_type1[i]))+_(L" (")+typestr+_(L")");
		else
			typestr = _(ConvertToString(meta_data[current_archive].file_type1[i]));
		itemid = m_assetlist->InsertItem(i,_(ConvertToString(i+1)));
		m_assetlist->SetItem(itemid,1,fullname);
		m_assetlist->SetItem(itemid,2,_(meta_data[current_archive].file_name[i]));
		m_assetlist->SetItem(itemid,3,typestr);
		m_assetlist->SetItem(itemid,4,_(ConvertToString(meta_data[current_archive].GetFileSizeByIndex(i))));
		m_assetlist->SetItem(itemid,5,infostr);
		m_assetlist->SetItemData(itemid,itemid);
	}
	return true;
}

wxString ToolUnityViewer::GetFullName(UnityArchiveFile archivetype, unsigned int fileid, bool* found) {
	if (archivetype>=UNITY_ARCHIVE_DATA11 && archivetype<=UNITY_ARCHIVE_DATA7)
		return meta_data[archivetype].GetFileFullName(fileid,archivetype,&bundle_data[archivetype-UNITY_ARCHIVE_DATA11],&list_data,found);
	return meta_data[archivetype].GetFileFullName(fileid,archivetype,NULL,&list_data,found);
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
			unsigned int modelcat = DATABASE_MODEL_CATEGORY_LIST_ALL;
			wxString partstrmodel = _(L"[Unknown Model]");
			wxString partstranim = _(L"[Unknown Animation]");
			for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_MODEL_NAME);i++)
				if (HADES_STRING_MODEL_NAME[i].id==modelid) {
					partstrmodel = HADES_STRING_MODEL_NAME[i].label;
					modelcat = DATABASE_MODEL_CATEGORY_TO_LIST(HADES_STRING_MODEL_NAME[i].category);
					break;
				}
			int32_t animindex = AnimationDatabase::GetIndex(animid,modelcat);
			if (animindex>=0)
				partstranim = AnimationDatabase::GetDescription(animindex);
			return _(L"Animation ")+partstranim+_(L" of the model ")+partstrmodel;
		}
	} else if (archive>=UNITY_ARCHIVE_DATA61 && archive<=UNITY_ARCHIVE_DATA63) {
		if (patharray.Count()==6 && patharray[2].IsSameAs(L"sounds",false)) {
			unsigned int modeltype = wxAtoi(patharray[3])<G_N_ELEMENTS(DATABASE_MODEL_TYPE) ? wxAtoi(patharray[3]) : 0;
			wxString id = patharray[3]+'/'+patharray[4]+'/'+name.BeforeFirst(L'.');
			if (patharray[4].IsSameAs(L"song_",false) && id[id.Len()-2]==L'_')
				id[id.Len()-1] = '0';
			int soundid = audio_data.GetObjectIndex("name",id.ToStdString());
			if (soundid<0) return _(L"[Unknown Sound]");
			string soundtype = audio_data.GetObjectValue("type",soundid);
			int soundindex = atoi(audio_data.GetObjectValue("soundIndex",soundid).c_str());
			if (soundtype==HADES_STRING_AUDIO_TYPE[AUDIO_TYPE_MUSIC])
				for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_MUSIC_NAME);i++)
					if (HADES_STRING_MUSIC_NAME[i].id==soundindex)
						return _(soundtype)+_(L" ")+HADES_STRING_MUSIC_NAME[i].label;
			if (soundtype==HADES_STRING_AUDIO_TYPE[AUDIO_TYPE_SFX])
				for (unsigned int i=0;i<G_N_ELEMENTS(HADES_STRING_AUDIO_NAME);i++)
					if (HADES_STRING_AUDIO_NAME[i].id==soundindex)
						return _(soundtype)+_(L" ")+HADES_STRING_AUDIO_NAME[i].label;
			return _(soundtype)+_(L" of index ")+_(audio_data.GetObjectValue("soundIndex",soundid));
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
			if (name.IsSameAs(L"BtlEncountBgmMetaData.txt",false)) return _(L"List of background musics played during the field encounters");
			if (name.IsSameAs(L"WldBtlEncountBgmMetaData.txt",false)) return _(L"List of background musics played during the World Map encounters");
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

void InitLinkFileDialog(UnityLinkFileDialog& dial, wxListCtrl* listctrl, UnityArchiveMetaData* metadata) {
	unsigned int fileid;
	long it = -1;
	dial.filelist.Empty();
	dial.fileinfolist.clear();
	if (listctrl->GetItemCount()==0)
		return;
	for (;;) {
		it = listctrl->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_DONTCARE);
		if (it==-1) break;
		fileid = wxAtoi(listctrl->GetItemText(it,0))-1;
		dial.filelist.Add(_(L"#")+listctrl->GetItemText(it,0)+_(L": ")+listctrl->GetItemText(it,1)+_(L" (")+_(UnityArchiveMetaData::GetTypeName(metadata->file_type1[fileid]))+_(L")"));
		dial.fileinfolist.push_back(metadata->file_info[fileid]);
	}
	dial.m_existinglist->Set(dial.filelist);
	dial.m_existinglist->SetSelection(0);
	dial.m_fileinfo->ChangeValue(ConvertLongToString(dial.fileinfolist[0]));
}

bool ToolUnityViewer::PrepareAssetForImport(bool isnewfile, fstream& filebase, wxString path, uint32_t ftype, vector<ModelDataStruct>& importmodel, vector<int64_t>& infotoavoid, UnityArchiveFileCreator& filestoadd, UnityLinkFileDialog& linkfiledialog, bool linkfiledialoginit, vector<bool>& copylist, /*	Common Arguments
										*/	vector<uint32_t>& filenewsize, unsigned int impfileid, long it, wxString filearchivedir, vector<GameObjectHierarchy>* importmodelbasehierarchy, /* Replace file
										*/	int64_t newfileinfo, string newfileinternalname, string newfilepath) { // New file
	bool newfilehasbundle = newfilepath.length()>0;
	unsigned int i,j;
	char* buffer;
	wxString wxnewfilepath = _(newfilepath);
	wxnewfilepath.Replace(_(L"\\"),_(L"/"));
	newfilepath = wxnewfilepath.ToStdString();
	if (ftype==28 && !m_menuconvertimgnone->IsChecked()) {
		wxImage img;
		bool success;
		if (m_menuconvertimgpng->IsChecked())		success = img.LoadFile(path,wxBITMAP_TYPE_PNG);
		else if (m_menuconvertimgtga->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TGA);
		else if (m_menuconvertimgtiff->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TIFF);
		else										success = img.LoadFile(path,wxBITMAP_TYPE_BMP);
		if (!success)								success = img.LoadFile(path,wxBITMAP_TYPE_ANY);
		if (!success) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL_FORMAT,path);
			return false;
		}
		uint32_t textureformat = 0;
		if (m_menuconvertimgqualityalpha->IsChecked())		textureformat = 0x01;
		else if (m_menuconvertimgqualityrgb->IsChecked())	textureformat = 0x03;
		else if (m_menuconvertimgqualityrgba->IsChecked())	textureformat = 0x04;
		else if (m_menuconvertimgqualityargb->IsChecked())	textureformat = 0x05;
		else if (m_menuconvertimgqualitydxt1->IsChecked())	textureformat = 0x0A;
		else if (m_menuconvertimgqualitydxt5->IsChecked())	textureformat = 0x0C;
		else if (isnewfile) {
			wxString warningstr;
			warningstr.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_DEFAULT_FORMAT),path);
			wxMessageDialog popupwarning(this,warningstr,HADES_STRING_WARNING,wxOK|wxCENTRE);
			popupwarning.ShowModal();
			textureformat = 0x04;
		} else if (meta_data[current_archive].GetFileSizeByIndex(impfileid)>=0x10) {
			filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(impfileid)+0xC);
			textureformat = ReadLong(filebase);
		}
		uint32_t newsizetmp = TIMImageDataStruct::GetSteamTextureFileSize(img.GetWidth(),img.GetHeight(),textureformat);
		if (newsizetmp==0) {
			wxLogError(HADES_STRING_UNITYVIEWER_IMPORT_ERROR_FORMAT,path);
			return false;
		}
		if (isnewfile) {
			filestoadd.Add(ftype,newsizetmp,newfileinfo,newfileinternalname);
			if (newfilehasbundle) {
				int32_t bundleindex = meta_data[current_archive].GetFileIndex("",142);
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].AddFile(newfilepath,filestoadd.file_index[filestoadd.file_index.size()-1],newfileinfo);
				if (bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_start[0]==0)
					bundle_data[current_archive-UNITY_ARCHIVE_DATA11].AddFileToBundle(newfileinfo);
				copylist[bundleindex] = false;
			}
		} else {
			filenewsize[impfileid] = newsizetmp;
		}
	} else if (ftype==49 && path.Len()>=10 && path.Mid(path.Len()-10).IsSameAs(L".akb.bytes",false) && !m_menuconvertaudionone->IsChecked()) {
		if (isnewfile) {
			fstream fileasset((const char*)path.c_str(),ios::in|ios::binary);
			if (!fileasset.is_open()) {
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
				return false;
			}
			uint32_t magicogg = ReadLong(fileasset);
			if (magicogg!=0x5367674F) {
				wxLogError(HADES_STRING_UNITYVIEWER_MISSING_OGG,path);
				return false;
			}
			fileasset.seekg(0,ios::end);
			filestoadd.Add(ftype,fileasset.tellg(),newfileinfo);
			if (newfilehasbundle) {
				int32_t bundleindex = meta_data[current_archive].GetFileIndex("",142);
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].AddFile(newfilepath,filestoadd.file_index[filestoadd.file_index.size()-1],newfileinfo);
				copylist[bundleindex] = false;
			}
			fileasset.close();
		} else {
			filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(impfileid));
			uint32_t magicakb = ReadLong(filebase);
			if (magicakb!=0x32424B41) {
				wxLogError(HADES_STRING_UNITYVIEWER_IMPORT_ERROR_FORMAT,path);
				return false;
			}
			fstream fileasset((const char*)path.c_str(),ios::in|ios::binary);
			if (!fileasset.is_open()) {
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
				return false;
			}
			uint32_t magicogg = ReadLong(fileasset);
			if (magicogg!=0x5367674F) {
				wxLogError(HADES_STRING_UNITYVIEWER_MISSING_OGG,path);
				return false;
			}
			fileasset.seekg(0,ios::end);
			filenewsize[impfileid] = (uint32_t)fileasset.tellg()+0x130;
			filenewsize[impfileid] += GetAlignOffset(filenewsize[impfileid],0x10);
			fileasset.close();
		}
	} else if (ftype==1 && (path.AfterLast(L'.').IsSameAs(L"fbx",false) || path.AfterLast(L'.').IsSameAs(L"prefab",false)) && !m_menuconvertmodelnone->IsChecked()) {
		bool modelflushunused = m_menuimportmodelflush->IsChecked() && !isnewfile;
		bool modelimportmesh = m_menuimportmodelmesh->IsChecked();
		bool modelimportanim = m_menuimportmodelanims->IsChecked();
		int modelmergepolicy = isnewfile ? 2 :
			m_menuimportmodelexistingfiles->IsChecked() ? 0 :
			m_menuimportmodelmerge->IsChecked() ? 1 :
			m_menuimportmodelimportall->IsChecked() ? 2 : -1;
		int32_t bundleindex = meta_data[current_archive].GetFileIndex("",142);
		int bundlefilelist = (!isnewfile && bundleindex>=0) ? bundle_data[current_archive-UNITY_ARCHIVE_DATA11].GetFileBundle(meta_data[current_archive].file_info[impfileid]) : -1;
		GameObjectHierarchy* modelhierarchy = NULL;
		wxString animsearchpath = _(L"");
		if (!isnewfile) {
			filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(impfileid));
			buffer = new char[meta_data[current_archive].GetFileSizeByIndex(impfileid)];
			filebase.read(buffer,meta_data[current_archive].GetFileSizeByIndex(impfileid));
			int64_t rootinfo = GameObjectHierarchy::GetRootInfoFromObject((uint8_t*)buffer);
			int32_t modelrootid = meta_data[current_archive].GetFileIndexByInfo(rootinfo);
			delete[] buffer;
			if (modelrootid<0) {
				wxLogError(HADES_STRING_UNITYVIEWER_MODEL_BAD_HIERARCHY);
				return false;
			}
			importmodelbasehierarchy->resize(importmodelbasehierarchy->size()+1);
			modelhierarchy = &importmodelbasehierarchy->at(importmodelbasehierarchy->size()-1);
			modelhierarchy->BuildHierarchy(filebase,meta_data[current_archive],modelrootid);
		}
		importmodel.resize(importmodel.size()+1);
		ModelDataStruct& newmodel = importmodel[importmodel.size()-1];
		newmodel.steam_name = (isnewfile ? _(newfilepath).AfterLast(L'/').BeforeFirst(L'.') : m_assetlist->GetItemText(it,1).BeforeFirst(L'.').AfterLast(L'/')).ToStdString();
		if (newmodel.Import((const char*)path.c_str())!=0) {
			wxLogError(HADES_STRING_UNITYVIEWER_UNKNOWN_FORMAT,path);
			importmodelbasehierarchy->pop_back();
			importmodel.pop_back();
			return false;
		}
		newmodel.hierarchy->meta_data = &meta_data[current_archive];
		vector<unsigned int> potentiallylinkedfiles;
		if (!isnewfile && bundleindex>=0 && bundlefilelist>=0) {
			for (i=bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_start[bundlefilelist]+1;i<bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_end[bundlefilelist];i++) {
				int auxfileindex = meta_data[current_archive].GetFileIndexByInfo(bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_info[i]);
				if (auxfileindex>=0)
					potentiallylinkedfiles.push_back(auxfileindex);
			}
		}
//		newmodel.hierarchy->DEBUGDisplayHierarchy();
		if (newmodel.hierarchy->root_node->node_type==4 && static_cast<TransformStruct*>(newmodel.hierarchy->root_node)->child_object) {
			GameObjectNode* rootobj = static_cast<TransformStruct*>(newmodel.hierarchy->root_node)->child_object;
			if (isnewfile) {
				rootobj->node_info = newfileinfo;
				rootobj->file_index = meta_data[current_archive].header_file_amount;
			} else {
				rootobj->node_info = meta_data[current_archive].file_info[impfileid];
				rootobj->file_index = impfileid;
			}
		}
		newmodel.SetupPostImportData(filebase,potentiallylinkedfiles,meta_data,infotoavoid,&bundle_data[UNITY_ARCHIVE_DATA5-UNITY_ARCHIVE_DATA11],modelhierarchy,modelmergepolicy);
		path.Replace(_(L"\\"),_(L"/"));
		if (modelimportmesh) {
			for (i=0;i<newmodel.material.size();i++) {
				for (j=0;j<newmodel.material[i].size();j++) {
					if (newmodel.material[i][j].maintex.file_info==0) {
						if (!linkfiledialoginit) {
							InitLinkFileDialog(linkfiledialog,m_assetlist,&meta_data[current_archive]);
							linkfiledialoginit = true;
						}
						wxString message = _(HADES_STRING_UNITYVIEWER_LINK_TEXTURE);
						message += path.AfterLast(L'/') + _(L" (Mesh ") + newmodel.mesh[i].name + _(L"):\t") + (newmodel.material[i][j].maintex.file_name!="" ? newmodel.material[i][j].maintex.file_name : "Unamed Texture");
						if (linkfiledialog.ShowModal(message)==wxID_OK)
							newmodel.material[i][j].maintex.file_info = linkfiledialog.info_selected;
					}
				}
			}
			unsigned int nodefileindex,nodefiletype,meshcounter = 0, meshmatcounter = 0, totalmatcounter = 0, animcounter = 0;
			unsigned int missedfiles = 0;
			vector<int> nodebundledataindex;
			vector<int64_t> modelbundle;
			uint32_t nodefilesize;
			string importassetinternalname, importassetpath;
			string rootassetshortname = (isnewfile ? _(newfilepath) : path).AfterLast(L'/').BeforeFirst(L'.');
			for (i=0;i<newmodel.hierarchy->node_list.size();i++) {
				nodefileindex = newmodel.hierarchy->node_list[i]->file_index;
				nodefiletype = newmodel.hierarchy->node_list[i]->node_type;
				if (nodefiletype==43)		nodefilesize = newmodel.mesh[meshcounter].GetDataSize();
				else if (nodefiletype==21)	nodefilesize = newmodel.material[meshcounter][meshmatcounter].GetDataSize();
				else if (nodefiletype==74)	nodefilesize = newmodel.animation[animcounter].GetDataSize();
				else						nodefilesize = newmodel.hierarchy->node_list[i]->GetDataSize();
				if (newmodel.hierarchy->node_list[i]->node_info==0) { // New file without new allocation
					missedfiles++;
				} else if (nodefileindex>=meta_data[current_archive].header_file_amount) { // New file with new allocation
					modelbundle.push_back(newmodel.hierarchy->node_list[i]->node_info);
					infotoavoid.push_back(newmodel.hierarchy->node_list[i]->node_info);
					importassetinternalname = "";
					importassetpath = "";
					if (nodefiletype==43) {
						importassetinternalname = wxString::Format(wxT("custmesh%d"),meshcounter);
						importassetpath = isnewfile ? newfilepath : filearchivedir.ToStdString();
					} else if (nodefiletype==21) {
						importassetinternalname = wxString::Format(wxT("%s_%d"),rootassetshortname,totalmatcounter);
						importassetpath = (isnewfile ? _(newfilepath) : filearchivedir).BeforeLast(L'/').ToStdString();
						if (importassetpath.length()>0)
							importassetpath += "/";
						importassetpath += wxString::Format(wxT("materials/%s_%d.mat"),rootassetshortname,totalmatcounter).ToStdString();
					} else if (nodefiletype==74) {
						importassetpath = isnewfile ? newfilepath : filearchivedir.ToStdString();
					} else if (isnewfile && newmodel.hierarchy->root_node->node_type==4 && static_cast<TransformStruct*>(newmodel.hierarchy->root_node)->child_object==newmodel.hierarchy->node_list[i]) {
						importassetpath = newfilepath;
						infotoavoid.pop_back(); // Was already listed
					}
					filestoadd.Add(newmodel.hierarchy->node_list[i]->node_type,nodefilesize,newmodel.hierarchy->node_list[i]->node_info,importassetinternalname);
					if (importassetpath.length()>0 && bundleindex>=0) {
						nodebundledataindex.push_back(filestoadd.file_index.size()-1);
						bundle_data[current_archive-UNITY_ARCHIVE_DATA11].AddFile(importassetpath,filestoadd.file_index[filestoadd.file_index.size()-1],newmodel.hierarchy->node_list[i]->node_info);
						copylist[bundleindex] = false;
					}
				} else { // Old file updated
					modelbundle.push_back(newmodel.hierarchy->node_list[i]->node_info);
					filenewsize[nodefileindex] = nodefilesize;
					copylist[nodefileindex] = false;
				}
				if (nodefiletype==43) {
					meshcounter++;
					meshmatcounter = 0;
				} else if (nodefiletype==21) {
					meshmatcounter++;
					totalmatcounter++;
				} else if (nodefiletype==74) {
					animcounter++;
				}
			}
			if (missedfiles>0) {
				wxString warningstr;
				warningstr.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_MISSING),missedfiles);
				wxMessageDialog popupwarning(this,warningstr,HADES_STRING_WARNING,wxOK|wxCENTRE);
				popupwarning.ShowModal();
			}
			for (i=0;i<nodebundledataindex.size();i++) // DEBUG: Importing 1 model + another file at once may bug because the bundle_data index is wrong
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].index[bundle_data[current_archive-UNITY_ARCHIVE_DATA11].amount-nodebundledataindex.size()+i] = filestoadd.file_index[nodebundledataindex[i]];
			for (meshcounter=0;meshcounter<newmodel.material.size();meshcounter++)
				for (meshmatcounter=0;meshmatcounter<newmodel.material[meshcounter].size();meshmatcounter++) {
					ModelMaterialData& mat = newmodel.material[meshcounter][meshmatcounter];
					if (mat.shader_info!=0) {
						for (i=0;i<modelbundle.size();i++)
							if (modelbundle[i]==mat.shader_info)
								break;
						if (i>=modelbundle.size())
							modelbundle.push_back(mat.shader_info);
					}
					if (mat.maintex.file_info!=0) {
						for (i=0;i<modelbundle.size();i++)
							if (modelbundle[i]==mat.maintex.file_info)
								break;
						if (i>=modelbundle.size())
							modelbundle.push_back(mat.maintex.file_info);
					}
				}
			if (!isnewfile && bundleindex>=0 && bundlefilelist>=0) {
				for (i=bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_start[bundlefilelist]+1;i<bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_end[bundlefilelist];i++) {
					int auxfileindex = meta_data[current_archive].GetFileIndexByInfo(bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_info[i]);
					if (auxfileindex>=0 && meta_data[current_archive].file_type1[auxfileindex]==49 && meta_data[current_archive].file_name[auxfileindex].length()>4 && meta_data[current_archive].file_name[auxfileindex].substr(meta_data[current_archive].file_name[auxfileindex].length()-4)==".tab")
						modelbundle.push_back(bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_info[i]);
				}
			}
			if (bundleindex>=0) {
				if (bundlefilelist>=0)
					bundle_data[current_archive-UNITY_ARCHIVE_DATA11].RemoveFileBundle(bundlefilelist);
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].AddFileBundle(6,modelbundle);
				copylist[bundleindex] = false;
			}
		} // if (modelimportmesh)
		bool countassetforimport = modelimportmesh;
		for (i=0;i<newmodel.animation.size();i++)
			if (newmodel.animation[i].file_id>=0 && newmodel.animation[i].anim_id==0xFFFFFFFF) { // Animations that are in the same archive as the model
				filenewsize[newmodel.animation[i].file_id] = newmodel.animation[i].GetDataSize();
				copylist[newmodel.animation[i].file_id] = false;
				countassetforimport = true;
			} else if (current_archive==UNITY_ARCHIVE_DATA5) { // ToDo: handle that case (filestoadd.Add)...
			}
		if (modelflushunused && !isnewfile) {
			for (i=0;i<modelhierarchy->node_list.size();i++) {
				for (j=0;j<newmodel.hierarchy->node_list.size();j++)
					if (modelhierarchy->node_list[i]->file_index==newmodel.hierarchy->node_list[j]->file_index)
						break;
				if (j>=newmodel.hierarchy->node_list.size()) {
					// ToDo
				}
			}
		}
		return countassetforimport;
	} else {
		fstream fileasset((const char*)path.c_str(),ios::in|ios::binary);
		if (!fileasset.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
			return false;
		}
		fileasset.seekg(0,ios::end);
		if (isnewfile) {
			filestoadd.Add(ftype,fileasset.tellg(),newfileinfo);
			if (newfilehasbundle) {
				int32_t bundleindex = meta_data[current_archive].GetFileIndex("",142);
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].AddFile(newfilepath,filestoadd.file_index[filestoadd.file_index.size()-1],newfileinfo);
				copylist[bundleindex] = false;
			}
		} else {
			filenewsize[impfileid] = fileasset.tellg();
		}
		fileasset.close();
	}
	if (!isnewfile)
		copylist[impfileid] = false;
	return true;
}

bool ToolUnityViewer::PerformImportOfAsset(bool isnewfile, fstream& filebase, fstream& filedest, UnityArchiveMetaData& newmetadata, wxString path, uint32_t ftype, vector<ModelDataStruct>& importmodel, unsigned int& importmodelcounter, unsigned int impfileid) {
	unsigned int i,j;
	char* buffer;
	uint32_t filenewsize;
	if (ftype==28 && !m_menuconvertimgnone->IsChecked()) {
		wxImage img;
		bool success;
		if (m_menuconvertimgpng->IsChecked())		success = img.LoadFile(path,wxBITMAP_TYPE_PNG);
		else if (m_menuconvertimgtga->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TGA);
		else if (m_menuconvertimgtiff->IsChecked())	success = img.LoadFile(path,wxBITMAP_TYPE_TIFF);
		else										success = img.LoadFile(path,wxBITMAP_TYPE_BMP);
		if (!success)								success = img.LoadFile(path,wxBITMAP_TYPE_ANY);
		if (!success) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL_FORMAT,path);
			return false;
		}
		uint32_t pixelindex;
		unsigned int x,y,w = img.GetWidth(), h = img.GetHeight();
		uint8_t* rgba = new uint8_t[w*h*4];
		bool hasalpha = img.HasAlpha();
		for (x=0;x<w;x++)
			for (y=0;y<h;y++) {
				pixelindex = (x+y*w)*4;
				rgba[pixelindex] = img.GetRed(x,y);
				rgba[pixelindex+1] = img.GetGreen(x,y);
				rgba[pixelindex+2] = img.GetBlue(x,y);
				rgba[pixelindex+3] = hasalpha ? img.GetAlpha(x,y) : 0xFF;
			}
		img.Destroy();
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
		else if (isnewfile)									textureformat = 0x04;
		else if (meta_data[current_archive].GetFileSizeByIndex(impfileid)>=0x10) {
			filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(impfileid)+0xC);
			textureformat = ReadLong(filebase);
		}
		buffer = (char*)TIMImageDataStruct::CreateSteamTextureFile(filenewsize,w,h,rgba,textureformat,quality);
		delete[] rgba;
		filedest.seekg(newmetadata.GetFileOffsetByIndex(impfileid));
		filedest.write(buffer,filenewsize);
		delete[] buffer;
	} else if (!isnewfile && ftype==49 && path.Len()>=10 && path.Mid(path.Len()-10).IsSameAs(L".akb.bytes",false) && !m_menuconvertaudionone->IsChecked()) {
		fstream fileasset((const char*)path.c_str(),ios::in|ios::binary);
		if (!fileasset.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
			return false;
		}
		uint32_t loopstart = 0;
		uint32_t loopend = 0;
		uint32_t secondaryloopstart = 0;
		uint32_t secondaryloopend = 0;
		uint32_t newsizeogg;
		fileasset.seekg(0,ios::end);
		newsizeogg = fileasset.tellg();
		filenewsize = newsizeogg+0x130;
		fileasset.seekg(0);
		filedest.seekg(newmetadata.GetFileOffsetByIndex(impfileid));
		filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(impfileid));
		buffer = new char[filenewsize];
		filebase.read(buffer,0x130);
		fileasset.read(&buffer[0x130],newsizeogg);
		for (i=newsizeogg+0x130;i<filenewsize;i++)
			buffer[i] = 0;
		for (i=0x130;i<newsizeogg;i++) {
			if (i+10<newsizeogg && strncmp(&buffer[i],"LoopStart=",10)==0)
				loopstart = atoi(&buffer[i+10]);
			else if (i+8<newsizeogg && strncmp(&buffer[i],"LoopEnd=",8)==0)
				loopend = atoi(&buffer[i+8]);
			if (i+11<newsizeogg && strncmp(&buffer[i],"LoopStart2=",11)==0)
				secondaryloopstart = atoi(&buffer[i+11]);
			else if (i+9<newsizeogg && strncmp(&buffer[i],"LoopEnd2=",9)==0)
				secondaryloopend = atoi(&buffer[i+9]);
		}
		BufferInitPosition(8);		BufferWriteLong((uint8_t*)buffer,filenewsize);
//		BufferInitPosition(0x28);	BufferWriteLong((uint8_t*)buffer,akbid); // ToDo: be able to retrieve this kind of info without the old file
//		BufferInitPosition(0xE2);	BufferWriteShort((uint8_t*)buffer,sound/music?);
//		BufferInitPosition(0xE6);	BufferWriteShort((uint8_t*)buffer,samplerate at 0x28 of Ogg);
		BufferInitPosition(0xE8);	BufferWriteLong((uint8_t*)buffer,newsizeogg);
//		BufferWriteLong((uint8_t*)buffer,samplecount);
		BufferInitPosition(0xF0);	BufferWriteLong((uint8_t*)buffer,loopstart);
		BufferWriteLong((uint8_t*)buffer,loopend);
		BufferInitPosition(0xFC);	BufferWriteLong((uint8_t*)buffer,secondaryloopstart); // For the music "Final Battle"
		BufferWriteLong((uint8_t*)buffer,secondaryloopend);
//		BufferInitPosition(0x124);	BufferWriteLong((uint8_t*)buffer,???);
		filedest.write(buffer,filenewsize);
		fileasset.close();
		delete[] buffer;
	} else if (ftype==1 && (path.AfterLast(L'.').IsSameAs(L"fbx",false) || path.AfterLast(L'.').IsSameAs(L"prefab",false)) && !m_menuconvertmodelnone->IsChecked()) {
		ModelDataStruct& newmodel = importmodel[importmodelcounter++];
		newmodel.hierarchy->meta_data = &newmetadata;
		for (i=0;i<newmodel.hierarchy->node_list.size();i++)
			if (newmodel.hierarchy->node_list[i]->file_index>=meta_data[current_archive].header_file_amount)
				for (j=newmetadata.header_file_amount ; j-- > 0 ;) // New node: the node's file_index is most likely wrong
					if (newmodel.hierarchy->node_list[i]->node_info==newmetadata.file_info[j]) {
						newmodel.hierarchy->node_list[i]->file_index = j;
						break;
					}
		newmodel.Write(filedest);
		for (i=0;i<newmodel.animation.size();i++)
			if (newmodel.animation[i].file_id>=0 && newmodel.animation[i].anim_id==0xFFFFFFFF) { // Animations that are in the same archive as the model
				filedest.seekg(newmetadata.GetFileOffsetByIndex(newmodel.animation[i].file_id));
				newmodel.animation[i].Write(filedest);
			} else if (current_archive==UNITY_ARCHIVE_DATA5) { // ToDo: handle that case...
			}
	} else {
		fstream fileasset((const char*)path.c_str(),ios::in|ios::binary);
		if (!fileasset.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,path);
			return false;
		}
		fileasset.seekg(0,ios::end);
		filenewsize = fileasset.tellg();
		fileasset.seekg(0);
		buffer = new char[filenewsize];
		fileasset.read(buffer,filenewsize);
		filedest.seekg(newmetadata.GetFileOffsetByIndex(impfileid));
		filedest.write(buffer,filenewsize);
		fileasset.close();
		delete[] buffer;
	}
	return true;
}

int ToolUnityViewer::PerformImportOfAnimations(vector<ModelDataStruct>& importmodel) {
	int modelmergepolicy =	m_menuimportmodelexistingfiles->IsChecked() ? 0 :
		m_menuimportmodelmerge->IsChecked() ? 1 :
		m_menuimportmodelimportall->IsChecked() ? 2 : -1;
	wxString animarchivename = root_path+_(UnityArchiveMetaData::GetArchiveName(UNITY_ARCHIVE_DATA5,use_x86));
	unsigned int i,j,k,importmodelcounter;
	int nbanimtoupdate = 0;
	if (modelmergepolicy<0 || current_archive==UNITY_ARCHIVE_DATA5 || !m_menuimportmodelanims->IsChecked())
		return 0;
	for (i=0;i<importmodel.size();i++)
		for (j=0;j<importmodel[i].animation.size();j++)
			if ((modelmergepolicy!=0 && (importmodel[i].animation[j].anim_id!=0xFFFFFFFF || importmodel[i].animation[j].file_id<0))
			 || (modelmergepolicy==0 && importmodel[i].animation[j].anim_id!=0xFFFFFFFF && importmodel[i].animation[j].file_id<0))
				nbanimtoupdate++;
	if (nbanimtoupdate==0)
		return 0;
	fstream filebaseanim((const char*)(animarchivename.c_str()),ios::in|ios::binary);
	fstream filedestanim((const char*)(animarchivename+_(L".tmp")).c_str(),ios::out|ios::binary);
	if (!filebaseanim.is_open())
		return -1;
	if (!filedestanim.is_open())
		return -2;
	vector<bool> copylistanim(meta_data[UNITY_ARCHIVE_DATA5].header_file_amount);
	vector<uint32_t> filenewsizeanim(meta_data[UNITY_ARCHIVE_DATA5].header_file_amount);
	for (i=0;i<meta_data[UNITY_ARCHIVE_DATA5].header_file_amount;i++) {
		copylistanim[i] = true;
		filenewsizeanim[i] = meta_data[UNITY_ARCHIVE_DATA5].file_size[i];
	}
	UnityArchiveFileCreator filestoaddanim(&meta_data[UNITY_ARCHIVE_DATA5]);
	UnityAddFileDialog adddial(this,meta_data,UNITY_ARCHIVE_DATA5);
	adddial.m_buttonadd->Enable(false);
	int animtypechoice = 0;
	for (i=0;i<adddial.file_type_choice.size();i++)
		if (adddial.file_type_choice[i]==74) {
			animtypechoice = i;
			break;
		}
	for (importmodelcounter=0;importmodelcounter<importmodel.size();importmodelcounter++) {
		ModelDataStruct& model = importmodel[importmodelcounter];
		for (i=0;i<model.animation.size();i++)
			if (model.animation[i].anim_id!=0xFFFFFFFF || model.animation[i].file_id<0) {
				if ((modelmergepolicy!=0 && model.animation[i].file_id<0) || modelmergepolicy==2) { // New animation: ask the import properties to the user
					UnityAddFilePanelDialog* newanimpanel = adddial.AddPanel();
					adddial.m_filebook->SetPageText(adddial.m_filebook->GetPageCount()-1,_(model.animation[i].name));
					newanimpanel->m_filepicker->SetPath(_(model.animation[i].name));
					newanimpanel->m_filepicker->Enable(false);
					newanimpanel->m_filetype->SetSelection(animtypechoice);
					newanimpanel->m_filetype->Enable(false);
					newanimpanel->m_fileinternalname->Enable(UnityArchiveMetaData::HasFileTypeName(74));
					if (adddial.has_bundle) {
						newanimpanel->m_addbundleinfo->SetValue(true);
						newanimpanel->EnableBundlePanel(true);
						newanimpanel->m_filename->SetValue(_(L"assets/resources/animations/")+_(model.steam_name)+(model.steam_name.length()>0 ? _("/") : _(L""))+_(model.animation[i].name)+_(L".anim"));
					}
				} else if (model.animation[i].file_id>=0) { // Update existing animation
					copylistanim[model.animation[i].file_id] = false;
					filenewsizeanim[model.animation[i].file_id] = model.animation[i].GetDataSize();
				}
			}
	}
	if (adddial.panel_amount>0)
		adddial.m_filebook->SetSelection(0);
	if (adddial.panel_amount>0 && adddial.ShowModal()!=wxID_OK) {
		filebaseanim.close();
		filedestanim.close();
		return -3;
	}
	j = 0;
	int32_t bundleindexanim = meta_data[UNITY_ARCHIVE_DATA5].GetFileIndex("",142);
	for (importmodelcounter=0;importmodelcounter<importmodel.size();importmodelcounter++) {
		ModelDataStruct& model = importmodel[importmodelcounter];
		for (i=0;i<model.animation.size();i++)
			if (model.animation[i].anim_id!=0xFFFFFFFF || model.animation[i].file_id<0) {
				if ((modelmergepolicy!=0 && model.animation[i].file_id<0) || modelmergepolicy==2) {
					UnityAddFilePanelDialog* panel = static_cast<UnityAddFilePanelDialog*>(adddial.m_filebook->GetPage(j++));
					uint32_t ftype = adddial.file_type_choice[panel->m_filetype->GetSelection()];
					int64_t newfileinfo = ConvertStringToLong(panel->m_fileinfo->GetValue());
					string newfilename = panel->m_fileinternalname->IsEnabled() ? panel->m_fileinternalname->GetValue() : "";
					string newfilepath = panel->m_addbundleinfo->IsChecked() ? panel->m_filename->GetValue() : "";
					filestoaddanim.Add(ftype,model.animation[i].GetDataSize(),newfileinfo);
					if (newfilepath.length()>0) {
						bundle_data[UNITY_ARCHIVE_DATA5-UNITY_ARCHIVE_DATA11].AddFile(newfilepath,filestoaddanim.file_index[filestoaddanim.file_index.size()-1],newfileinfo);
						bundle_data[UNITY_ARCHIVE_DATA5-UNITY_ARCHIVE_DATA11].AddFileToBundle(newfileinfo);
						copylistanim[bundleindexanim] = false;
					}
				}
			}
	}
	if (bundleindexanim>=0 && !copylistanim[bundleindexanim])
		filenewsizeanim[bundleindexanim] = bundle_data[UNITY_ARCHIVE_DATA5-UNITY_ARCHIVE_DATA11].GetDataSize();
	filebaseanim.seekg(0);
	UnityArchiveMetaData newmetadataanim;
	meta_data[UNITY_ARCHIVE_DATA5].Duplicate(filebaseanim,filedestanim,copylistanim,filenewsizeanim,&filestoaddanim,&newmetadataanim);
	if (bundleindexanim>=0 && !copylistanim[bundleindexanim]) {
		for (k=0;k<filestoaddanim.file_index.size();k++)
			if (filestoaddanim.file_index[k]<=bundleindexanim)
				bundleindexanim++;
		filedestanim.seekg(newmetadataanim.GetFileOffsetByIndex(bundleindexanim));
		bundle_data[UNITY_ARCHIVE_DATA5-UNITY_ARCHIVE_DATA11].Write(filedestanim);
	}
	j = 0;
	for (importmodelcounter=0;importmodelcounter<importmodel.size();importmodelcounter++) {
		ModelDataStruct& model = importmodel[importmodelcounter];
		for (i=0;i<model.animation.size();i++)
			if (model.animation[i].anim_id!=0xFFFFFFFF || model.animation[i].file_id<0) {
				if ((modelmergepolicy!=0 && model.animation[i].file_id<0) || modelmergepolicy==2) {
					filedestanim.seekg(newmetadataanim.GetFileOffsetByIndex(filestoaddanim.file_index[j++]));
					model.animation[i].Write(filedestanim);
				} else if (model.animation[i].file_id>=0) {
					for (k=0;k<filestoaddanim.file_index.size();k++)
						if (filestoaddanim.file_index[k]<=model.animation[i].file_id)
							model.animation[i].file_id++;
					filedestanim.seekg(newmetadataanim.GetFileOffsetByIndex(model.animation[i].file_id));
					model.animation[i].Write(filedestanim);
				}
			}
	}
	filebaseanim.close();
	filedestanim.close();
	wxRenameFile(animarchivename+_(L".tmp"),animarchivename,true);
	meta_data[UNITY_ARCHIVE_DATA5].Flush();
	meta_data[UNITY_ARCHIVE_DATA5].Copy(&newmetadataanim);
	return nbanimtoupdate;
}

void ToolUnityViewer::OnAssetRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	unsigned int i;
	if (id==wxID_EXPORT) {
		fstream filebase((const char*)(root_path+archive_name).c_str(),ios::in|ios::binary);
		if (!filebase.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL,root_path+archive_name);
			return;
		}
		unsigned int itcounter = 0, itamount = m_assetlist->GetSelectedItemCount();
		unsigned int expfileid, expfilesize;
		wxString basepath = root_path+_(L"HadesWorkshopAssets\\")+archive_name.AfterLast(L'\\').BeforeFirst(L'.')+_(L"\\");
		wxString path,filearchivedir;
		char* buffer;
		long it = -1;
		for (;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			filearchivedir = m_assetlist->GetItemText(it,1);
			path = filearchivedir;
			path.Replace(_(L"/"),_(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath+path;
			wxFileName::Mkdir(path.BeforeLast(L'\\'),wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
			expfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
//{fstream fout("aaab.txt",ios::app|ios::out); fout << "EXPORT: " << expfileid << " with info " << (unsigned long long)meta_data[current_archive].file_info[expfileid] << endl; fout.close();}
			expfilesize = meta_data[current_archive].GetFileSizeByIndex(expfileid);
			filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid));
			buffer = new char[expfilesize];
			filebase.read(buffer,expfilesize);
			if (meta_data[current_archive].file_type1[expfileid]==28 && !m_menuconvertimgnone->IsChecked()) {
				uint32_t imgw, imgh;
				uint8_t* imgrgba;
				bool success = TIMImageDataStruct::ConvertFromSteamTexture((uint8_t*)buffer,&imgw,&imgh,&imgrgba);
				delete[] buffer;
				if (!success) {
					wxLogError(_(HADES_STRING_UNITYVIEWER_UNKNOWN_FORMAT),m_assetlist->GetItemText(it,1));
					continue;
				}
				unsigned char* imgrgb = (unsigned char*)malloc(3*imgw*imgh*sizeof(unsigned char));
				unsigned char* imgalpha = (unsigned char*)malloc(imgw*imgh*sizeof(unsigned char));
				for (i=0;i<imgw*imgh;i++) {
					imgrgb[3*i] = imgrgba[4*i];
					imgrgb[3*i+1] = imgrgba[4*i+1];
					imgrgb[3*i+2] = imgrgba[4*i+2];
					imgalpha[i] = imgrgba[4*i+3];
				}
				delete[] imgrgba;
				wxImage img(imgw,imgh,imgrgb,imgalpha);
				if (m_menuconvertimgpng->IsChecked())		success = img.SaveFile(path,wxBITMAP_TYPE_PNG);
				else if (m_menuconvertimgtga->IsChecked())	success = img.SaveFile(path,wxBITMAP_TYPE_TGA);
				else if (m_menuconvertimgtiff->IsChecked())	success = img.SaveFile(path,wxBITMAP_TYPE_TIFF);
				else										success = img.SaveFile(path,wxBITMAP_TYPE_BMP);
				if (!success) {
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE, path);
					continue;
				}
			} else if (meta_data[current_archive].file_type1[expfileid]==49 && path.Len()>=10 && path.Mid(path.Len()-10).IsSameAs(L".akb.bytes",false) && !m_menuconvertaudionone->IsChecked()) {
				fstream filedest((const char*)path.c_str(), ios::out | ios::binary);
				if (!filedest.is_open()) {
					delete[] buffer;
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE, path);
					continue;
				}
				uint32_t magicakb;
				BufferInitPosition();
				BufferReadLong((uint8_t*)buffer,magicakb);
				if (magicakb==0x32424B41) {
					uint32_t oggsize;
					BufferInitPosition(0xE8);
					BufferReadLong((uint8_t*)buffer,oggsize);
					filedest.write(&buffer[0x130],oggsize);
				} else {
					filedest.write(buffer,expfilesize);
				}
				filedest.close();
				delete[] buffer;
			} else if (meta_data[current_archive].file_type1[expfileid] == 1 && (path.AfterLast(L'.').IsSameAs(L"fbx", false) || path.AfterLast(L'.').IsSameAs(L"prefab", false)) && !m_menuconvertmodelnone->IsChecked()) {
				// TODO: .prefab files are (mostly) tiles for the World Map model ; export them all as one model?
				int64_t rootinfo = GameObjectHierarchy::GetRootInfoFromObject((uint8_t*)buffer);
				/*int fbundle = bundle_data[current_archive-UNITY_ARCHIVE_DATA11].GetFileBundle(rootinfo);
				fstream fout("aaab.txt",ios::app|ios::out); fout << "EXPORTING " << path << endl;
				fout << "Root bundle: " << fbundle << endl;
				if (fbundle>=0) {
					fout << "Contain Files: " << endl;
					for (i=bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_start[fbundle]+1;i<bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_index_end[fbundle];i++) {
						fout << (int)meta_data[current_archive].GetFileIndexByInfo(bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_info[i]) << " with info " << (unsigned long long)bundle_data[current_archive-UNITY_ARCHIVE_DATA11].bundle_info[i] << endl;
					}
				}
				fout.close();*/
				int32_t modelrootid = meta_data[current_archive].GetFileIndexByInfo(rootinfo);
				delete[] buffer;
				if (modelrootid < 0) {
					wxLogError(HADES_STRING_UNITYVIEWER_MODEL_BAD_HIERARCHY);
					continue;
				}
				GameObjectHierarchy modelhierarchy;
				modelhierarchy.BuildHierarchy(filebase, meta_data[current_archive], modelrootid);
				wxString filepathbase = m_assetlist->GetItemText(it, 1).BeforeLast(L'.');
				wxString descriptionstr = m_assetlist->GetItemText(it, 5);
				ModelDataStruct model;
				if (filepathbase.Len() == 0)
					filepathbase = m_assetlist->GetItemText(it, 1);
				descriptionstr.Replace(_(UnityArchiveMetaData::GetTypeName(meta_data[current_archive].file_type1[expfileid])), _(L"Full Model"));
				model.description = descriptionstr.ToStdString();
				model.steam_name = filepathbase.AfterLast(L'/').AfterLast(L'\\').ToStdString();
				if (!model.Read(filebase, &modelhierarchy)) {
					wxLogError(HADES_STRING_UNITYVIEWER_MODEL_BAD_HIERARCHY);
					continue;
				}
				long itclip = -1;
				wxString fileobjectid, gameobjectid = GetFullName(current_archive, expfileid);
				for (;;) {
					itclip = m_assetlist->GetNextItem(itclip, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
					if (itclip == -1) break;
					if (meta_data[current_archive].file_type1[wxAtoi(m_assetlist->GetItemText(itclip, 0)) - 1] == 74) {
						fileobjectid = GetFullName(current_archive, wxAtoi(m_assetlist->GetItemText(itclip, 0)) - 1);
						if (gameobjectid.IsSameAs(fileobjectid)) {
							ModelAnimationData animation;
							filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(wxAtoi(m_assetlist->GetItemText(itclip, 0)) - 1));
							animation.Read(filebase, &modelhierarchy);
							animation.anim_id = 0xFFFFFFFF;
							model.animation.push_back(animation);
						}
					}
				}
				if (current_archive != UNITY_ARCHIVE_DATA5) {
					wxString animarchivefilename, animsearchpath;
					wxString fileanimname = _(UnityArchiveMetaData::GetArchiveName(UNITY_ARCHIVE_DATA5, use_x86));
					bool foundanimarchivefilename;
					fstream fileanimbase((const char*)(root_path + fileanimname).c_str(), ios::in | ios::binary);
					if (!fileanimbase.is_open()) {
						wxLogError(HADES_STRING_OPEN_ERROR_FAIL, root_path + fileanimname);
						continue;
					}
					animsearchpath = _(L"assets/resources/animations/") + _(model.steam_name) + _(L"/");
					for (i = 0; i < meta_data[UNITY_ARCHIVE_DATA5].header_file_amount; i++) {
						animarchivefilename = GetFullName(UNITY_ARCHIVE_DATA5, i, &foundanimarchivefilename);
						if (foundanimarchivefilename && animsearchpath.IsSameAs(animarchivefilename.Mid(0, animsearchpath.Len()))) {
							ModelAnimationData animation;
							fileanimbase.seekg(meta_data[UNITY_ARCHIVE_DATA5].GetFileOffsetByIndex(i));
							animation.Read(fileanimbase);
							animation.anim_id = wxAtoi(animarchivefilename.Mid(animsearchpath.Len()));
							if (_(animation.name).IsNumber()) { // If possible, name the animation with its Steam ID, not numerical ID
								int32_t animdbindex = AnimationDatabase::GetIndex(animation.anim_id);
								if (animdbindex >= 0) {
									animation.name = AnimationDatabase::GetSteamId(animdbindex);
									animation.name_len = animation.name.length();
								}
							}
							model.animation.push_back(animation);
						}
					}
					fileanimbase.close();
				}
				if (m_menuconvertmodelfbxtext->IsChecked())			model.Export(path.BeforeLast(L'.').c_str(), MODEL_FILE_FORMAT_FBX_ASCII);
				else if (m_menuconvertmodelautocad->IsChecked())	model.Export(path.BeforeLast(L'.').c_str(), MODEL_FILE_FORMAT_AUTOCAD);
				else if (m_menuconvertmodelcollada->IsChecked())	model.Export(path.BeforeLast(L'.').c_str(), MODEL_FILE_FORMAT_COLLADA);
				else if (m_menuconvertmodelwave->IsChecked())		model.Export(path.BeforeLast(L'.').c_str(), MODEL_FILE_FORMAT_WAVEFRONT);
//				else if (m_menuconvertmodel3ds->IsChecked())		model.Export(path.BeforeLast(L'.').c_str(), MODEL_FILE_FORMAT_3DS_MAX);
				else												model.Export(path.BeforeLast(L'.').c_str(), MODEL_FILE_FORMAT_FBX_BINARY);
				/* Old version: OBJ Exporter
				fstream fobj((path.BeforeLast(L'.')+_(L".obj")).c_str(),ios::out);
				if (!fobj.is_open())
					continue;
				fstream fmtl((path.BeforeLast(L'.')+_(L".mtl")).c_str(),ios::out);
				if (!fmtl.is_open()) {
					fobj.close();
					continue;
				}
				fobj << "mtllib " << (filepathbase.AfterLast(L'/')+_(L".mtl")).c_str() << endl;
				unsigned int matcounter = 0;
				for (i=0;i<modellist.size();i++) {
					wxString mtlname = modelfilematerial.size()==1 ? _(L"mat") : wxString::Format(wxT("mat%d_"),i);
					wxString objname = wxString::Format(wxT("Object_%d"),i);
					wxString texfilename;
					int32_t texfileid;
					for (j=0;j<modellist[i].material.size();j++) {
						texfileid = meta_data[current_archive].GetFileIndexByInfo(modellist[i].material[j].maintex_file_info);
						if (texfileid>=0)
							texfilename = _(meta_data[current_archive].file_name[texfileid])+_(".png");
						else
							texfilename = _(L"TextureNotFound");
						modellist[i].material[j].Export(fmtl,wxString::Format(wxT("%s%d"),mtlname,j).c_str(),texfilename.c_str());
					}
					modellist[i].mesh.Export(fobj,objname.c_str(),mtlname.c_str(),i==0);
				}
				fobj.close();
				fmtl.close();
				*/
			} else if (meta_data[current_archive].file_type1[expfileid] == 74 && !m_menuconvertanimnone->IsChecked()) {
				delete[] buffer;
				filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(expfileid));
				fstream filedest((const char*)(path + _(L".json")).c_str(), ios::out);
				if (!filedest.is_open()) {
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE, path);
					continue;
				}
				ModelAnimationData anim;
				anim.Read(filebase);
				anim.WriteAsJSON(filedest);
				filedest.close();
			} else {
				fstream filedest((const char*)path.c_str(), ios::out | ios::binary);
				if (!filedest.is_open()) {
					delete[] buffer;
					wxLogError(HADES_STRING_OPEN_ERROR_CREATE, path);
					continue;
				}
				filedest.write(buffer,expfilesize);
				filedest.close();
				delete[] buffer;
			}
			m_loadgauge->SetValue((++itcounter)*100/itamount);
		}
		filebase.close();
		m_loadgauge->SetValue(100);
	} else if (id==wxID_IMPORT) {
		fstream filebase((const char*)(root_path+archive_name).c_str(),ios::in|ios::binary);
		fstream filedest((const char*)(root_path+archive_name+_(L".tmp")).c_str(),ios::out|ios::binary);
		bool overwritefile = true;
		UnityLinkFileDialog linkfiledialog(this);
		bool linkfiledialoginit = false;
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
		unsigned int impfileid;
		wxString basepath = root_path+_(L"HadesWorkshopAssets\\")+archive_name.AfterLast(L'\\').BeforeFirst(L'.')+_(L"\\");
		wxString path,filearchivedir;
		vector<bool> copylist(meta_data[current_archive].header_file_amount);
		vector<uint32_t> filenewsize(meta_data[current_archive].header_file_amount);
		for (i=0;i<meta_data[current_archive].header_file_amount;i++) {
			copylist[i] = true;
			filenewsize[i] = meta_data[current_archive].file_size[i];
		}
		vector<GameObjectHierarchy> importmodelbasehierarchy;
		vector<ModelDataStruct> importmodel;
		unsigned int importmodelcounter = 0;
		UnityArchiveFileCreator filestoadd(&meta_data[current_archive]);
		vector<int64_t> infotoavoid;
		long it = -1;
		for (;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			filearchivedir = m_assetlist->GetItemText(it,1);
			path = filearchivedir;
			path.Replace(_(L"/"),_(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath+path;
			if (!wxFileName::FileExists(path))
				continue;
			impfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
			if (!PrepareAssetForImport(false,filebase,path,meta_data[current_archive].file_type1[impfileid],importmodel,infotoavoid,filestoadd,linkfiledialog,linkfiledialoginit,copylist,filenewsize,impfileid,it,filearchivedir,&importmodelbasehierarchy,0,"",""))
				continue;
			itcounter++;
		}
		m_loadgauge->SetValue(10);
		int32_t bundleindex = meta_data[current_archive].GetFileIndex("",142);
		bool bundleisupdated = bundleindex>=0 && !copylist[bundleindex];
		if (bundleisupdated)
			filenewsize[bundleindex] = bundle_data[current_archive-UNITY_ARCHIVE_DATA11].GetDataSize();
		filebase.seekg(0);
		UnityArchiveMetaData newmetadata;
		if (itcounter>0) {
			meta_data[current_archive].Duplicate(filebase,filedest,copylist,filenewsize,&filestoadd,&newmetadata);
			if (bundleisupdated) {
				filedest.seekg(newmetadata.GetFileOffsetByIndex(bundleindex));
				bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Write(filedest);
			}
		}
		m_loadgauge->SetValue(50);
		for (it = -1;;) {
			it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
			if (it==-1) break;
			impfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
			if (copylist[impfileid])
				continue;
			path = m_assetlist->GetItemText(it,1);
			path.Replace(_(L"/"),_(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath+path;
			if (!PerformImportOfAsset(false,filebase,filedest,newmetadata,path,meta_data[current_archive].file_type1[impfileid],importmodel,importmodelcounter,impfileid)) {
				overwritefile = false;
				continue;
			}
			m_loadgauge->SetValue(50+itcounter*40/itamount);
			itsuccesscounter++;
		}
		filebase.close();
		filedest.close();
		wxString animarchivename = root_path+_(UnityArchiveMetaData::GetArchiveName(UNITY_ARCHIVE_DATA5,use_x86));
		int nbanimtoupdate = 0;
		m_loadgauge->SetValue(90);
		if (overwritefile)
			nbanimtoupdate = PerformImportOfAnimations(importmodel);
		m_loadgauge->SetValue(100);
		if (overwritefile && itcounter>0)
			overwritefile = wxRenameFile(root_path+archive_name+_(L".tmp"),root_path+archive_name,true);
		if (overwritefile && itcounter>0) {
			meta_data[current_archive].Flush();
			meta_data[current_archive].Copy(&newmetadata);
			fstream unityarchive((const char*)(root_path+archive_name).c_str(),ios::in | ios::binary);
//			meta_data[current_archive].Load(unityarchive,(UnityArchiveFile)current_archive);
			if (current_archive>=UNITY_ARCHIVE_DATA11 && current_archive<=UNITY_ARCHIVE_DATA7) {
				uint32_t offset = meta_data[current_archive].GetFileOffset("",142);
				if (offset>0) {
					unityarchive.seekg(offset);
					bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Load(unityarchive);
				}
			}
			if (current_archive==UNITY_ARCHIVE_MAINDATA) {
				list_data.Flush();
				int32_t listindex = meta_data[current_archive].GetFileIndex("",147);
				if (listindex>=0) {
					unityarchive.seekg(meta_data[current_archive].GetFileOffsetByIndex(listindex));
					list_data.Load(unityarchive,meta_data[current_archive].GetFileSizeByIndex(listindex));
				}
			}
			unityarchive.close();
			if (filestoadd.file_index.size()>0) { // New files added or removed: refresh the list
				DisplayArchive(current_archive);
			} else { // Files are only updated: display the updated sizes
				for (it = -1;;) {
					it = m_assetlist->GetNextItem(it,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
					if (it==-1) break;
					impfileid = wxAtoi(m_assetlist->GetItemText(it,0))-1;
					if (copylist[impfileid])
						continue;
					m_assetlist->SetItem(it,4,_(ConvertToString(meta_data[current_archive].GetFileSizeByIndex(impfileid))));
				}
			}
		} else if (bundleisupdated) { // Bundle was updated but the archive replacement failed
			if (current_archive>=UNITY_ARCHIVE_DATA11 && current_archive<=UNITY_ARCHIVE_DATA7) {
				fstream unityarchive((const char*)(root_path+archive_name).c_str(),ios::in | ios::binary);
				uint32_t offset = meta_data[current_archive].GetFileOffset("",142);
				if (offset>0) {
					unityarchive.seekg(offset);
					bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Load(unityarchive);
				}
			}
		}
		wxString popupmessage, popuptitle;
		wxString animresultstring = _(L"");
		if (nbanimtoupdate==-3) animresultstring = _(HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL);
		else if (nbanimtoupdate==-2) animresultstring.Printf(HADES_STRING_OPEN_ERROR_FAIL/* + */HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL,animarchivename);
		else if (nbanimtoupdate==-1) animresultstring.Printf(HADES_STRING_OPEN_ERROR_CREATE/* + */HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL,animarchivename+_(L".tmp"));
		else if (nbanimtoupdate>0) animresultstring.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORTANIM_SUCCESS),nbanimtoupdate);
		if (itsuccesscounter==itamount) {
			popuptitle = _(HADES_STRING_SUCCESS);
			popupmessage.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_SUCCESS),itsuccesscounter);
		} else {
			popuptitle = _(HADES_STRING_WARNING);
			popupmessage.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_ERROR),itsuccesscounter,itamount-itsuccesscounter);
		}
		if (!overwritefile) popupmessage += _(HADES_STRING_UNITYVIEWER_IMPORT_NO_REPLACE);
		if (animresultstring.Len()>0) popupmessage += _(L"\n\n");
		wxMessageDialog popupdialog(this,popupmessage+animresultstring,popuptitle,wxOK|wxCENTRE);
		popupdialog.ShowModal();
	} else if (id==wxID_ADD) {
		UnityAddFileDialog adddial(this,meta_data,current_archive);
		if (adddial.ShowModal()==wxID_OK) {
			fstream filebase((const char*)(root_path+archive_name).c_str(),ios::in|ios::binary);
			fstream filedest((const char*)(root_path+archive_name+_(L".tmp")).c_str(),ios::out|ios::binary);
			UnityLinkFileDialog linkfiledialog(this);
			bool linkfiledialoginit = false;
			if (!filebase.is_open()) {
				wxLogError(HADES_STRING_OPEN_ERROR_FAIL,root_path+archive_name);
				return;
			}
			if (!filedest.is_open()) {
				wxLogError(HADES_STRING_OPEN_ERROR_CREATE,root_path+archive_name+_(L".tmp"));
				return;
			}
			m_loadgauge->SetValue(0);
			unsigned int itcounter = 0, itsuccesscounter = 0, itamount = 0;
			wxString path;
			vector<ModelDataStruct> importmodel;
			unsigned int importmodelcounter = 0;
			UnityArchiveFileCreator filestoadd(&meta_data[current_archive]);
			vector<int64_t> infotoavoid;
			vector<int> bookaddfileindex;
			bookaddfileindex.resize(adddial.m_filebook->GetPageCount());
			vector<bool> copylist(meta_data[current_archive].header_file_amount);
			vector<uint32_t> filenewsize(meta_data[current_archive].header_file_amount);
			for (i=0;i<meta_data[current_archive].header_file_amount;i++) {
				copylist[i] = true;
				filenewsize[i] = meta_data[current_archive].file_size[i];
			}
			for (i=0;i<adddial.m_filebook->GetPageCount();i++) {
				UnityAddFilePanelDialog* panel = static_cast<UnityAddFilePanelDialog*>(adddial.m_filebook->GetPage(i));
				if (panel->m_filepicker->GetPath().Len()==0)
					continue;
				infotoavoid.push_back(ConvertStringToLong(panel->m_fileinfo->GetValue()));
			}
			for (i=0;i<adddial.m_filebook->GetPageCount();i++) {
				bookaddfileindex[i] = -1;
				UnityAddFilePanelDialog* panel = static_cast<UnityAddFilePanelDialog*>(adddial.m_filebook->GetPage(i));
				path = panel->m_filepicker->GetPath();
				if (path.Len()==0)
					continue;
				itamount++;
				uint32_t ftype = adddial.file_type_choice[panel->m_filetype->GetSelection()];
				int64_t newfileinfo = ConvertStringToLong(panel->m_fileinfo->GetValue());
				string newfilename = panel->m_fileinternalname->IsEnabled() ? panel->m_fileinternalname->GetValue() : "";
				string newfilepath = panel->m_addbundleinfo->IsChecked() ? panel->m_filename->GetValue() : "";
				unsigned int tmpcnt = importmodel.size();
				bookaddfileindex[i] = filestoadd.file_index.size();
				vector<uint32_t> dummynewsize;
				if (!PrepareAssetForImport(true,filebase,path,ftype,importmodel,infotoavoid,filestoadd,linkfiledialog,linkfiledialoginit,copylist,dummynewsize,0,0,"",NULL,newfileinfo,newfilename,newfilepath)) {
					bookaddfileindex[i] = -1;
					continue;
				}
				if (tmpcnt<importmodel.size()) { // Point bookaddfileindex[i] to the root's object and not the root itself
					for (tmpcnt=bookaddfileindex[i];tmpcnt<filestoadd.file_index.size();tmpcnt++)
						if (filestoadd.file_info[tmpcnt]==newfileinfo) {
							bookaddfileindex[i] = tmpcnt;
							break;
						}
				}
				itcounter++;
			}
			m_loadgauge->SetValue(10);
			int32_t bundleindex = meta_data[current_archive].GetFileIndex("",142);
			bool bundleisupdated = bundleindex>=0 && !copylist[bundleindex];
			if (bundleisupdated)
				filenewsize[bundleindex] = bundle_data[current_archive-UNITY_ARCHIVE_DATA11].GetDataSize();
			filebase.seekg(0);
			UnityArchiveMetaData newmetadata;
			if (itcounter>0) {
				meta_data[current_archive].Duplicate(filebase,filedest,copylist,filenewsize,&filestoadd,&newmetadata);
				if (bundleisupdated) {
					filedest.seekg(newmetadata.GetFileOffsetByIndex(bundleindex));
					bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Write(filedest);
				}
			}
			m_loadgauge->SetValue(50);
			unsigned int impfileid;
			for (i=0;i<adddial.m_filebook->GetPageCount();i++) {
				if (bookaddfileindex[i]<0)
					continue;
				UnityAddFilePanelDialog* panel = static_cast<UnityAddFilePanelDialog*>(adddial.m_filebook->GetPage(i));
				path = panel->m_filepicker->GetPath();
				uint32_t ftype = filestoadd.file_type[bookaddfileindex[i]];
				impfileid = filestoadd.file_index[bookaddfileindex[i]];
				if (!PerformImportOfAsset(true,filebase,filedest,newmetadata,path,ftype,importmodel,importmodelcounter,impfileid))
					continue;
				itsuccesscounter++;
				m_loadgauge->SetValue(50+itsuccesscounter*50/itamount);
			}
			filebase.close();
			filedest.close();
			bool overwritefile = itsuccesscounter==itamount;
			wxString animarchivename = root_path+_(UnityArchiveMetaData::GetArchiveName(UNITY_ARCHIVE_DATA5,use_x86));
			int nbanimtoupdate = 0;
			m_loadgauge->SetValue(90);
			if (overwritefile)
				nbanimtoupdate = PerformImportOfAnimations(importmodel);
			m_loadgauge->SetValue(100);
			if (overwritefile && itcounter>0)
				overwritefile = wxRenameFile(root_path+archive_name+_(L".tmp"),root_path+archive_name,true);
			if (overwritefile && itcounter>0) {
				meta_data[current_archive].Flush();
				meta_data[current_archive].Copy(&newmetadata);
				fstream unityarchive((const char*)(root_path+archive_name).c_str(),ios::in | ios::binary);
				if (current_archive>=UNITY_ARCHIVE_DATA11 && current_archive<=UNITY_ARCHIVE_DATA7) {
					uint32_t offset = meta_data[current_archive].GetFileOffset("",142);
					if (offset>0) {
						unityarchive.seekg(offset);
						bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Load(unityarchive);
					}
				}
				if (current_archive==UNITY_ARCHIVE_MAINDATA) {
					list_data.Flush();
					int32_t listindex = meta_data[current_archive].GetFileIndex("",147);
					if (listindex>=0) {
						unityarchive.seekg(meta_data[current_archive].GetFileOffsetByIndex(listindex));
						list_data.Load(unityarchive,meta_data[current_archive].GetFileSizeByIndex(listindex));
					}
				}
				unityarchive.close();
				DisplayArchive(current_archive);
			} else if (bundleisupdated) { // Bundle was updated but the archive replacement failed
				if (current_archive>=UNITY_ARCHIVE_DATA11 && current_archive<=UNITY_ARCHIVE_DATA7) {
					fstream unityarchive((const char*)(root_path+archive_name).c_str(),ios::in | ios::binary);
					uint32_t offset = meta_data[current_archive].GetFileOffset("",142);
					if (offset>0) {
						unityarchive.seekg(offset);
						bundle_data[current_archive-UNITY_ARCHIVE_DATA11].Load(unityarchive);
					}
				}
			}
			wxString popupmessage, popuptitle;
			wxString animresultstring = _(L"");
			if (nbanimtoupdate==-3) animresultstring = _(HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL);
			else if (nbanimtoupdate==-2) animresultstring.Printf(HADES_STRING_OPEN_ERROR_FAIL/* + */HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL,animarchivename);
			else if (nbanimtoupdate==-1) animresultstring.Printf(HADES_STRING_OPEN_ERROR_CREATE/* + */HADES_STRING_UNITYVIEWER_IMPORTANIM_CANCEL,animarchivename+_(L".tmp"));
			else if (nbanimtoupdate>0) animresultstring.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORTANIM_SUCCESS),nbanimtoupdate);
			if (itsuccesscounter==itamount) {
				popuptitle = _(HADES_STRING_SUCCESS);
				popupmessage.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_SUCCESS),itsuccesscounter);
			} else {
				popuptitle = _(HADES_STRING_WARNING);
				popupmessage.Printf(wxT(HADES_STRING_UNITYVIEWER_IMPORT_ERROR),itsuccesscounter,itamount-itsuccesscounter);
			}
			if (!overwritefile) popupmessage += _(HADES_STRING_UNITYVIEWER_IMPORT_NO_REPLACE);
			if (animresultstring.Len()>0) popupmessage += _(L"\n\n");
			wxMessageDialog popupdialog(this,popupmessage+animresultstring,popuptitle,wxOK|wxCENTRE);
			popupdialog.ShowModal();
		}
	} else if (id == wxID_SELECTALL) {
		long it = -1;
		for (;;) {
			it = m_assetlist->GetNextItem(it);
			if (it == -1) break;
			m_assetlist->SetItemState(it, m_assetlist->GetItemState(it, wxLIST_STATE_SELECTED) != 0 ? 0 : wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
		}
	} else if (id == wxID_INFO) {
		wxString basepath = root_path + _(L"HadesWorkshopAssets\\") + archive_name.AfterLast(L'\\').BeforeFirst(L'.') + _(L"\\");
		wxString path;
		fstream filebase((const char*)(root_path + archive_name).c_str(), ios::in | ios::binary);
		if (!filebase.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL, root_path + archive_name);
			return;
		}
		long it = -1;
		int i, j;
		vector<unsigned int> itemtoselect;
		for (;;) {
			it = m_assetlist->GetNextItem(it, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (it == -1) break;
			unsigned int fileid = wxAtoi(m_assetlist->GetItemText(it, 0)) - 1;
			path = m_assetlist->GetItemText(it, 1);
			if (meta_data[current_archive].file_type1[fileid] != 1)
				continue;
			int32_t rootid = fileid;
			if (path.AfterLast(L'.').IsSameAs(L"fbx", false) || path.AfterLast(L'.').IsSameAs(L"prefab", false)) {
				filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(fileid));
				char* buffer = new char[meta_data[current_archive].GetFileSizeByIndex(fileid)];
				filebase.read(buffer, meta_data[current_archive].GetFileSizeByIndex(fileid));
				int64_t rootinfo = GameObjectHierarchy::GetRootInfoFromObject((uint8_t*)buffer);
				rootid = meta_data[current_archive].GetFileIndexByInfo(rootinfo);
				delete[] buffer;
				if (rootid < 0) {
					wxLogError(HADES_STRING_UNITYVIEWER_MODEL_BAD_HIERARCHY);
					continue;
				}
			}
			path.Replace(_(L"/"), _(L"\\"));
			if (!m_menuexportpath->IsChecked()) path = path.AfterLast(L'\\');
			path = basepath + path + _(L".hierarchy.txt");
			wxFileName::Mkdir(path.BeforeLast(L'\\'), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
			fstream filedest((const char*)path.c_str(), ios::out);
			if (!filedest.is_open()) {
				wxLogError(HADES_STRING_OPEN_ERROR_CREATE, path);
				continue;
			}
			GameObjectHierarchy modelhierarchy;
			modelhierarchy.BuildHierarchy(filebase, meta_data[current_archive], rootid);
			filedest << HADES_STRING_UNITYVIEWER_HIERARCHY_HEADER << (rootid + 1) << endl;
			if (current_archive >= UNITY_ARCHIVE_DATA11 && current_archive <= UNITY_ARCHIVE_DATA7)
				filedest << modelhierarchy.PrintHierarchy(current_archive, &bundle_data[current_archive - UNITY_ARCHIVE_DATA11], &list_data);
			else
				filedest << modelhierarchy.PrintHierarchy(current_archive, NULL, &list_data);
			filedest.close();
			for (i = 0; i < modelhierarchy.node_list.size(); i++)
				if (modelhierarchy.node_list[i] != NULL)
					itemtoselect.push_back(modelhierarchy.node_list[i]->file_index);
		}
		filebase.close();
		it = -1;
		for (;;) {
			it = m_assetlist->GetNextItem(it);
			if (it == -1) break;
			for (j = 0; j < itemtoselect.size(); j++)
				if (itemtoselect[j] == wxAtoi(m_assetlist->GetItemText(it, 0)) - 1) {
					m_assetlist->SetItemState(it, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
					break;
				}
		}
	} else if (id == wxID_REMOVE) {
		wxMessageDialog popupwarning(this, _(HADES_STRING_UNITYVIEWER_WARNING_REMOVE), HADES_STRING_WARNING, wxCANCEL | wxYES_NO | wxCENTRE);
		int modalresult = popupwarning.ShowModal();
		if (modalresult != wxID_YES && modalresult != wxID_NO)
			return;
		fstream filebase((const char*)(root_path + archive_name).c_str(), ios::in | ios::binary);
		fstream filedest((const char*)(root_path + archive_name + _(L".tmp")).c_str(), ios::out | ios::binary);
		if (!filebase.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL, root_path + archive_name);
			return;
		}
		if (!filedest.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE, root_path + archive_name + _(L".tmp"));
			return;
		}
		long it = -1;
		vector<bool> filetokeep(meta_data[current_archive].header_file_amount, true);
		for (;;) {
			it = m_assetlist->GetNextItem(it, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (it == -1) break;
			unsigned int fileid = wxAtoi(m_assetlist->GetItemText(it, 0)) - 1;
			filetokeep[fileid] = false;
		}
		meta_data[current_archive].DuplicateWithDeletion(filebase, filedest, filetokeep);
		filebase.close();
		filedest.close();
		if (modalresult == wxID_YES) {
			wxString extension = archive_name.AfterFirst(L'.');
			wxRenameFile(root_path + archive_name + _(L".tmp"), root_path + archive_name.BeforeFirst('.') + _(L" - Copy with deletions") + (extension.IsEmpty() ? _(L"") : _(L".") + extension), true);
			return;
		}
		if (!wxRenameFile(root_path + archive_name + _(L".tmp"), root_path + archive_name, true)) {
			wxLogError(HADES_STRING_OPEN_ERROR_OVERWRITE, root_path + archive_name);
			return;
		}
		meta_data[current_archive].Flush();
		fstream unityarchive((const char*)(root_path + archive_name).c_str(), ios::in | ios::binary);
		meta_data[current_archive].Load(unityarchive,(UnityArchiveFile)current_archive);
		if (current_archive >= UNITY_ARCHIVE_DATA11 && current_archive <= UNITY_ARCHIVE_DATA7) {
			uint32_t offset = meta_data[current_archive].GetFileOffset("", 142);
			if (offset > 0) {
				unityarchive.seekg(offset);
				bundle_data[current_archive - UNITY_ARCHIVE_DATA11].Load(unityarchive);
			}
		}
		if (current_archive == UNITY_ARCHIVE_MAINDATA) {
			list_data.Flush();
			int32_t listindex = meta_data[current_archive].GetFileIndex("", 147);
			if (listindex >= 0) {
				unityarchive.seekg(meta_data[current_archive].GetFileOffsetByIndex(listindex));
				list_data.Load(unityarchive, meta_data[current_archive].GetFileSizeByIndex(listindex));
			}
		}
		unityarchive.close();
		DisplayArchive(current_archive);
	} else if (id == wxID_DUPLICATE) {
		fstream filebase((const char*)(root_path + archive_name).c_str(), ios::in | ios::binary);
		if (!filebase.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_FAIL, root_path + archive_name);
			return;
		}
		long it = -1;
		vector<unsigned int> animid;
		vector<wxString> transitionname;
		for (;;) {
			it = m_assetlist->GetNextItem(it, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (it == -1) break;
			animid.push_back(wxAtoi(m_assetlist->GetItemText(it, 0)) - 1);
			transitionname.push_back(m_assetlist->GetItemText(it, 1).AfterLast(L'\\').AfterLast(L'/').BeforeFirst(L'.'));
		}
		if (animid.size() != 2 || meta_data[current_archive].file_type1[animid[0]] != 74 || meta_data[current_archive].file_type1[animid[1]] != 74)
			return;
		float transduration = 1.0f;
		AnimationTransitionWindow popupdialog(this, transitionname[0], transitionname[1], transduration);
		if (popupdialog.ShowModal() != wxID_OK)
			return;
		transduration = popupdialog.m_spinduration->GetValue();
		if (popupdialog.is_swap) {
			animid.insert(animid.begin(), animid.back());
			animid.pop_back();
			transitionname.insert(transitionname.begin(), transitionname.back());
			transitionname.pop_back();
		}
		wxString filedestname = root_path + _(L"HadesWorkshopAssets\\") + archive_name.AfterLast(L'\\').BeforeFirst(L'.') + _(L"\\") + transitionname[0] + _(L"_") + transitionname[1] + _(L".anim");
		wxFileName::Mkdir(filedestname.BeforeLast(L'\\'), wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		fstream filedest;
		if (m_menuconvertanimjson->IsChecked())	filedest.open((const char*)(filedestname + _(L".json")).c_str(), ios::out);
		else									filedest.open((const char*)filedestname.c_str(), ios::out | ios::binary);
		if (!filedest.is_open()) {
			wxLogError(HADES_STRING_OPEN_ERROR_CREATE, filedestname);
			return;
		}
		ModelAnimationData anim1, anim2, transition;
		filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(animid[0]));
		anim1.Read(filebase);
		filebase.seekg(meta_data[current_archive].GetFileOffsetByIndex(animid[1]));
		anim2.Read(filebase);
		transition = ModelAnimationData::GenerateFromTwoStances(anim1, anim2, transduration);
		if (m_menuconvertanimjson->IsChecked())	transition.WriteAsJSON(filedest);
		else									transition.Write(filedest);
		filebase.close();
		filedest.close();
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
	bool enablegenanim = false;
	if (m_assetlist->GetSelectedItemCount() == 2) {
		long it = -1;
		enablegenanim = true;
		for (;;) {
			it = m_assetlist->GetNextItem(it, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (it == -1) break;
			if (meta_data[current_archive].file_type1[wxAtoi(m_assetlist->GetItemText(it, 0)) - 1] != 74) enablegenanim = false;
		}
	}
	assetmenuimport->Enable(enableimport);
	assetmenuadd->Enable(enableimport);
	assetmenugenanim->Enable(enablegenanim);
	assetmenuremove->Enable(enableimport);
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
		m_assetlist->SetItemData(it,it);
		it = m_assetlist->GetNextItem(it);
	}
}

int UnityLinkFileDialog::ShowModal(wxString message) {
	if (infovalidator==NULL) {
		infovalidator = new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
		infovalidator->SetCharIncludes(_(L"0123456789abcdefABCDEF"));
		infovalidator->SetWindow(m_fileinfo);
		m_fileinfo->SetValidator(*infovalidator);
	}
	m_message->ChangeValue(message);
	return wxDialog::ShowModal();
}

void UnityLinkFileDialog::OnFileSelect(wxCommandEvent& event) {
	m_fileinfo->ChangeValue(ConvertLongToString(fileinfolist[m_existinglist->GetSelection()]));
}

void UnityLinkFileDialog::OnFileInfoEdit(wxCommandEvent& event) {
	m_buttonok->Enable(m_fileinfo->GetValue().Len()==16);
}

void UnityLinkFileDialog::OnButtonClick(wxCommandEvent& event) {
	info_selected = ConvertStringToLong(m_fileinfo->GetValue());
	return EndModal(event.GetId());
}

UnityAddFileDialog::UnityAddFileDialog(wxWindow* parent, UnityArchiveMetaData metadatalist[UNITY_ARCHIVE_AMOUNT], unsigned int archiveindex) :
	UnityAddFileWindow(parent),
	meta_data_list(metadatalist),
	meta_data(&metadatalist[archiveindex]),
	panel_amount(0) {
	unsigned int i;
	string tname;
	has_bundle = false;
	for (i=0;i<meta_data->header_file_amount;i++)
		if (meta_data->file_type1[i]==142) {
			has_bundle = true;
			break;
		}
	for (i=0;i<290;i++) {
		tname = UnityArchiveMetaData::GetTypeName(i);
		if (tname.length()>0) {
			file_type_choice.push_back(i);
			file_type_name.Add(tname);
		}
	}
}

int UnityAddFileDialog::ShowModal() {
	if (panel_amount==0)
		AddPanel();
	return wxDialog::ShowModal();
}

bool UnityAddFileDialog::IsInfoUnused(int64_t info, int paneltoignore) {
	unsigned int i,j;
	for (i=0;i<panel_amount;i++)
		if (i!=paneltoignore && ConvertStringToLong(static_cast<UnityAddFilePanelDialog*>(m_filebook->GetPage(i))->m_fileinfo->GetValue())==info)
			return false;
	for (i=0;i<UNITY_ARCHIVE_AMOUNT;i++)
		for (j=0;j<meta_data_list[i].header_file_amount;j++)
			if (meta_data_list[i].file_info[j]==info)
				return false;
	return true;
}

void UnityAddFileDialog::GenerateValidInfo(unsigned int panelid) {
	int64_t newfileinfo = meta_data->file_info[meta_data->header_file_amount-1]+1;
	while (!IsInfoUnused(newfileinfo,panelid))
		newfileinfo++;
	static_cast<UnityAddFilePanelDialog*>(m_filebook->GetPage(panelid))->m_fileinfo->SetValue(ConvertLongToString(newfileinfo));
}

int UnityAddFileDialog::VerifyInfoValidity() {
	int res = 0;
	int64_t infoval;
	for (unsigned int panelid=0;panelid<m_filebook->GetPageCount();panelid++) {
		UnityAddFilePanelDialog* panel = static_cast<UnityAddFilePanelDialog*>(m_filebook->GetPage(panelid));
		if (panel->m_fileinfo->GetValue().Len()!=16) {
			panel->m_fileinfo->SetBackgroundColour(wxColor(0xB0,0x20,0x20));
			panel->m_fileinfo->SetToolTip(_(HADES_STRING_UNITYVIEWER_INVALID_FILE_INFO));
			res = max(res,3);
			continue;
		}
		infoval = ConvertStringToLong(panel->m_fileinfo->GetValue());
		if (!IsInfoUnused(infoval,panelid)) {
			panel->m_fileinfo->SetBackgroundColour(wxColor(0xFF,0x70,0x00));
			panel->m_fileinfo->SetToolTip(_(HADES_STRING_UNITYVIEWER_USED_FILE_INFO));
			res = max(res,2);
		} else if (infoval<meta_data->file_info[meta_data->header_file_amount-1]) {
			wxString tooltip;
			unsigned int i,findex = 0;
			while (meta_data->file_info[findex]<infoval)
				findex++;
			for (i=0;i<m_filebook->GetPageCount();i++)
				if (infoval>ConvertStringToLong(static_cast<UnityAddFilePanelDialog*>(m_filebook->GetPage(i))->m_fileinfo->GetValue()))
					findex++;
			tooltip.Printf(wxT(HADES_STRING_UNITYVIEWER_LOW_FILE_INFO),findex+1);
			panel->m_fileinfo->SetBackgroundColour(wxColor(0x80,0x00,0xFF));
			panel->m_fileinfo->SetToolTip(tooltip);
			res = max(res,1);
		} else {
			panel->m_fileinfo->SetBackgroundColour(wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ));
			panel->m_fileinfo->UnsetToolTip();
		}
	}
	m_buttonok->Enable(res<2);
	return res;
}

UnityAddFilePanelDialog* UnityAddFileDialog::AddPanel() {
	UnityAddFilePanelDialog* addedpanel = new UnityAddFilePanelDialog(this);
	addedpanel->m_filetype->Append(file_type_name);
	addedpanel->m_filetype->Select(0);
	addedpanel->m_fileinternalname->Enable(UnityArchiveMetaData::HasFileTypeName(file_type_choice[0]));
	addedpanel->EnableBundlePanel(false);
	m_filebook->AddPage(addedpanel,_(HADES_STRING_GENERIC_NEW),true);
	panel_amount++;
	GenerateValidInfo(panel_amount-1);
	return addedpanel;
}

void UnityAddFileDialog::OnButtonClick(wxCommandEvent& event) {
	if (event.GetId()==wxID_ADD) {
		AddPanel();
		return;
	}
	int validityproblem = VerifyInfoValidity();
	if (validityproblem==2) {
		wxMessageDialog popupwarning(this,_(HADES_STRING_UNITYVIEWER_WARNING_FILE_INFO),HADES_STRING_WARNING,wxCANCEL|wxOK|wxCENTRE);
		if (popupwarning.ShowModal()!=wxID_OK)
			return;
	}
	EndModal(event.GetId());
}

UnityAddFilePanelDialog::UnityAddFilePanelDialog(UnityAddFileDialog* p) : UnityAddFilePanel(p->m_filebook), parent(p) {
	infovalidator = new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);
	infovalidator->SetCharIncludes(_(L"0123456789abcdefABCDEF"));
	infovalidator->SetWindow(m_fileinfo);
	m_fileinfo->SetValidator(*infovalidator);
	m_addbundleinfo->Enable(parent->has_bundle);
}

void UnityAddFilePanelDialog::EnableBundlePanel(bool enable) {
	m_filenamelabel->Enable(enable);
	m_filename->Enable(enable);
}

void UnityAddFilePanelDialog::OnFilePick(wxFileDirPickerEvent& event) {
	if (!wxFile::Exists(event.GetPath())) {
		SetLabel(_(HADES_STRING_GENERIC_NEW));
		return;
	}
	wxString shortname = event.GetPath().AfterLast(L'\\').AfterLast(L'/').BeforeFirst(L'.');
	SetLabel(shortname);
	m_fileinternalname->SetValue(shortname);
	parent->m_filebook->SetPageText(parent->m_filebook->FindPage(this),shortname);
}

void UnityAddFilePanelDialog::OnTypeChoice(wxCommandEvent& event) {
	m_fileinternalname->Enable(UnityArchiveMetaData::HasFileTypeName(parent->file_type_choice[event.GetSelection()]));
}

void UnityAddFilePanelDialog::OnFileInfoEdit(wxCommandEvent& event) {
	parent->VerifyInfoValidity();
}

void UnityAddFilePanelDialog::OnCheckBox(wxCommandEvent& event) {
	EnableBundlePanel(event.IsChecked());
}

void UnityAddFilePanelDialog::OnButtonClick(wxCommandEvent& event) {
	parent->GenerateValidInfo(parent->m_filebook->FindPage(this));
}
