#include "Tool_UnityViewer.h"

#include "Hades_Strings.h"
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
		result = assetlist->GetItemText(item1,column).Cmp(assetlist->GetItemText(item2,column));
	} else {
		long val1 = wxAtol(assetlist->GetItemText(item1,column));
		long val2 = wxAtol(assetlist->GetItemText(item2,column));
		if (val1<val2)
			result = 1;
		else if (val1>val2)
			result = -1;
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
	m_assetlist->AppendColumn(_(ASSET_COLUMN_TYPE),wxLIST_FORMAT_LEFT,40);
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

bool ToolUnityViewer::DisplayArchive(UnityArchiveFile filetype) {
	unsigned int unkcounter = 0;
	unsigned int i,j;
	long itemid;
	bool foundfullname;
	wxString fullname;
	m_assetlist->DeleteAllItems();
	for (i=0;i<meta_data[filetype].header_file_amount;i++) {
		fullname = _(meta_data[filetype].file_name[i]);
		foundfullname = false;
		if (filetype>=UNITY_ARCHIVE_DATA11 && filetype<=UNITY_ARCHIVE_DATA7) {
			UnityArchiveAssetBundle& bundle = bundle_data[filetype-UNITY_ARCHIVE_DATA11];
			for (j=0;j<bundle.amount;j++)
				if (bundle.info[j]==meta_data[filetype].file_info[i]) {
					fullname = _(bundle.path[j]);
					foundfullname = true;
					break;
				}
		}
		if (!foundfullname) {
			for (j=0;j<list_data.amount;j++)
				if (list_data_filename[j].Cmp(fullname)==0 && list_data.index[j]==i+1) {
					fullname = _(list_data.path[j]);
					foundfullname = true;
					break;
				}
		}
		if (!foundfullname) {
			if (meta_data[filetype].file_type1[i]==142)
				fullname = _(L"AssetBundle");
			else if (meta_data[filetype].file_type1[i]==147)
				fullname = _(L"ResourceManager");
			else if (meta_data[filetype].file_name_len[i]==0)
				fullname = _(L"NoName")+_(ConvertToString(++unkcounter));
		}
		itemid = m_assetlist->InsertItem(i,_(ConvertToString(i+1)));
		m_assetlist->SetItem(itemid,1,m_menudisplaypath->IsChecked() ? fullname : fullname.AfterLast(L'/'));
		m_assetlist->SetItem(itemid,2,_(meta_data[filetype].file_name[i]));
		m_assetlist->SetItem(itemid,3,_(ConvertToString(meta_data[filetype].file_type1[i])));
		m_assetlist->SetItem(itemid,4,_(ConvertToString(meta_data[filetype].GetFileSizeById(i))));
		m_assetlist->SetItem(itemid,5,_(L""));
		m_assetlist->SetItemData(itemid,itemid);
	}
	return true;
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
