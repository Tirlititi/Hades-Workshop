#include "Gui_WalkmeshEditor.h"

#include <set>
#include "Hades_Strings.h"

class WalkmeshAddTriangleDialog : public WalkmeshAddTriangleWindow {
public:
	WalkmeshEditDialog* parent;
	vector<int> triangle_single;
	vector<vector<int>> triangle_single_side;
	vector<pair<int, int>> triangle_couple;
	vector<pair<int, int>> triangle_couple_side;
	vector<pair<int, int>> triangle_couple_common_vertex;
	//vector<vector<int>> triangle_triple;
	int16_t third_vertex_x;
	int16_t third_vertex_y;
	int16_t third_vertex_z;

	WalkmeshAddTriangleDialog(WalkmeshEditDialog* p, int initialpath) : WalkmeshAddTriangleWindow(p), parent(p) {
		FieldWalkmeshDataStruct* walk = &parent->walkmesh;
		wxArrayString choicelist;
		int i, j, k, l, di;
		for (i = 0; i < walk->walkpath_amount; i++)
			choicelist.Add(wxString::Format(wxT("Walkpath %d"), i));
		m_walkpath->Append(choicelist);
		m_walkpath->SetSelection(initialpath);
		choicelist.Clear();
		vector<vector<int>> emptyside;
		for (i = 0; i < walk->triangle_amount; i++) {
			vector<int> triemptyside;
			if (walk->triangle_adjacenttriangle[i].index[0] < 0)	triemptyside.push_back(0);
			if (walk->triangle_adjacenttriangle[i].index[1] < 0)	triemptyside.push_back(1);
			if (walk->triangle_adjacenttriangle[i].index[2] < 0)	triemptyside.push_back(2);
			emptyside.push_back(triemptyside);
			if (triemptyside.size() > 0) {
				triangle_single.push_back(i);
				triangle_single_side.push_back(triemptyside);
				choicelist.Add(wxString::Format(wxT("Triangle %d"), i));
			}
		}
		m_adjacentsingle->Append(choicelist);
		choicelist.Clear();
		vector<int> vcommon1, vcommon2;
		for (i = 0; i < walk->triangle_amount; i++) {
			if (emptyside[i].size() == 0)
				continue;
			for (j = i + 1; j < walk->triangle_amount; j++) {
				if (emptyside[j].size() == 0)
					continue;
				int16_t* v1 = walk->triangle_vertex[i].index;
				int16_t* v2 = walk->triangle_vertex[j].index;
				if (walk->triangle_walkpath[i] == walk->triangle_walkpath[j]) {
					for (k = 0; k < 3; k++)
						for (l = 0; l < 3; l++)
							if (v1[k] == v2[l]) {
								vcommon1.push_back(k);
								vcommon2.push_back(l);
							}
				} else {
					for (di = 0; di < parent->duplicate_vertex.size(); di++)
						for (k = 0; k < 3; k++)
							for (l = 0; l < 3; l++)
								if ((parent->duplicate_vertex[di][0] == v1[k] && parent->duplicate_vertex[di][1] == v2[l]) || (parent->duplicate_vertex[di][0] == v2[l] && parent->duplicate_vertex[di][1] == v1[k])) {
									vcommon1.push_back(k);
									vcommon2.push_back(l);
								}
				}
				if (vcommon1.size() > 0) {
					int verttoside1[3] = { -1, -1, -1 };
					int verttoside2[3] = { -1, -1, -1 };
					for (k = 0; k < emptyside[i].size(); k++) {
						if (emptyside[i][k] == 0) {
							verttoside1[0] = 0;
							verttoside1[2] = 0;
						} else if (emptyside[i][k] == 1) {
							verttoside1[0] = 1;
							verttoside1[1] = 1;
						} else {
							verttoside1[1] = 2;
							verttoside1[2] = 2;
						}
					}
					for (k = 0; k < emptyside[j].size(); k++) {
						if (emptyside[j][k] == 0) {
							verttoside2[0] = 0;
							verttoside2[2] = 0;
						} else if (emptyside[j][k] == 1) {
							verttoside2[0] = 1;
							verttoside2[1] = 1;
						} else {
							verttoside2[1] = 2;
							verttoside2[2] = 2;
						}
					}
					for (k = 0; k < vcommon1.size(); k++)
						if (verttoside1[vcommon1[k]] >= 0 && verttoside2[vcommon2[k]] >= 0) {
							triangle_couple.push_back({ i, j });
							triangle_couple_side.push_back({ verttoside1[vcommon1[k]], verttoside2[vcommon2[k]] });
							triangle_couple_common_vertex.push_back({ vcommon1[k], vcommon2[k] });
							choicelist.Add(wxString::Format(wxT("Triangles %d - %d"), i, j));
						}
					vcommon1.clear();
					vcommon2.clear();
				}
			}
		}
		m_adjacentcouple->Append(choicelist);
		if (triangle_single.size() > 0)
			m_adjacentsingle->SetSelection(0);
		if (triangle_couple.size() > 0)
			m_adjacentcouple->SetSelection(0);
		DisplaySingleTrianglePos(true, true);
	}

	void GetVerticeFromEdge(int triid, int edgenum, int& v1, int& v2, int* v3 = NULL) {
		if (edgenum == 0) {
			v1 = parent->walkmesh.triangle_vertex[triid].index[2];
			v2 = parent->walkmesh.triangle_vertex[triid].index[0];
			if (v3) *v3 = parent->walkmesh.triangle_vertex[triid].index[1];
		} else if (edgenum == 1) {
			v1 = parent->walkmesh.triangle_vertex[triid].index[0];
			v2 = parent->walkmesh.triangle_vertex[triid].index[1];
			if (v3) *v3 = parent->walkmesh.triangle_vertex[triid].index[2];
		} else {
			v1 = parent->walkmesh.triangle_vertex[triid].index[1];
			v2 = parent->walkmesh.triangle_vertex[triid].index[2];
			if (v3) *v3 = parent->walkmesh.triangle_vertex[triid].index[0];
		}
	}

	void DisplayIsolatedTrianglePos() {
		vector<int16_t> trianglepts;
		int pathid = m_walkpath->GetSelection();
		int16_t pathx = parent->walkmesh.offset_x + parent->walkmesh.walkpath_orgx[pathid];
		int16_t pathy = parent->walkmesh.offset_y + parent->walkmesh.walkpath_orgy[pathid];
		int16_t pathz = parent->walkmesh.offset_z + parent->walkmesh.walkpath_orgz[pathid];
		trianglepts.push_back(pathx + m_trianglex1->GetValue());
		trianglepts.push_back(pathy + m_triangley1->GetValue());
		trianglepts.push_back(pathz - m_trianglez1->GetValue());
		trianglepts.push_back(pathx + m_trianglex2->GetValue());
		trianglepts.push_back(pathy + m_triangley2->GetValue());
		trianglepts.push_back(pathz - m_trianglez2->GetValue());
		trianglepts.push_back(pathx + m_trianglex3->GetValue());
		trianglepts.push_back(pathy + m_triangley3->GetValue());
		trianglepts.push_back(pathz - m_trianglez3->GetValue());
		parent->gl_window->DisplayFieldPolygon(trianglepts);
	}

	void DisplaySingleTrianglePos(bool initside, bool initpos) {
		if (triangle_single.size() == 0) {
			m_side->Enable(false);
			return;
		}
		int triid = m_adjacentsingle->GetSelection();
		if (initside) {
			m_side->Clear();
			m_side->Append(HADES_STRING_WALKMESH_SIDE_1);
			if (triangle_single_side[triid].size() >= 2)
				m_side->Append(HADES_STRING_WALKMESH_SIDE_2);
			if (triangle_single_side[triid].size() >= 3)
				m_side->Append(HADES_STRING_WALKMESH_SIDE_3);
			m_side->SetSelection(0);
		}
		int tripathid = parent->walkmesh.triangle_walkpath[triangle_single[triid]];
		int vertpathid = m_walkpath->GetSelection();
		int16_t tripathx = parent->walkmesh.offset_x + parent->walkmesh.walkpath_orgx[tripathid];
		int16_t tripathy = parent->walkmesh.offset_y + parent->walkmesh.walkpath_orgy[tripathid];
		int16_t tripathz = parent->walkmesh.offset_z + parent->walkmesh.walkpath_orgz[tripathid];
		int16_t vertpathx = parent->walkmesh.offset_x + parent->walkmesh.walkpath_orgx[vertpathid];
		int16_t vertpathy = parent->walkmesh.offset_y + parent->walkmesh.walkpath_orgy[vertpathid];
		int16_t vertpathz = parent->walkmesh.offset_z + parent->walkmesh.walkpath_orgz[vertpathid];
		vector<int16_t> trianglepts;
		int v1, v2, v3;
		GetVerticeFromEdge(triangle_single[triid], triangle_single_side[triid][m_side->GetSelection()], v1, v2, &v3);
		int16_t absv1x = tripathx + parent->walkmesh.vertex_x[v1];
		int16_t absv1y = tripathy + parent->walkmesh.vertex_y[v1];
		int16_t absv1z = tripathz + parent->walkmesh.vertex_z[v1];
		int16_t absv2x = tripathx + parent->walkmesh.vertex_x[v2];
		int16_t absv2y = tripathy + parent->walkmesh.vertex_y[v2];
		int16_t absv2z = tripathz + parent->walkmesh.vertex_z[v2];
		int16_t absv3x = tripathx + parent->walkmesh.vertex_x[v3];
		int16_t absv3y = tripathy + parent->walkmesh.vertex_y[v3];
		int16_t absv3z = tripathz + parent->walkmesh.vertex_z[v3];
		if (initpos) {
			third_vertex_x = absv1x + absv2x - absv3x - vertpathx;
			third_vertex_y = absv1y + absv2y - absv3y - vertpathy;
			third_vertex_z = absv1z + absv2z - absv3z - vertpathz;
			m_trianglex4->SetValue(third_vertex_x);
			m_triangley4->SetValue(third_vertex_y);
			m_trianglez4->SetValue(-third_vertex_z);
		} else {
			third_vertex_x = m_trianglex4->GetValue();
			third_vertex_y = m_triangley4->GetValue();
			third_vertex_z = -m_trianglez4->GetValue();
		}
		trianglepts.push_back(absv1x);
		trianglepts.push_back(absv1y);
		trianglepts.push_back(absv1z);
		trianglepts.push_back(absv2x);
		trianglepts.push_back(absv2y);
		trianglepts.push_back(absv2z);
		trianglepts.push_back(vertpathx + third_vertex_x);
		trianglepts.push_back(vertpathy + third_vertex_y);
		trianglepts.push_back(vertpathz + third_vertex_z);
		parent->gl_window->DisplayFieldPolygon(trianglepts);
	}

	void DisplayCoupleTrianglePos() {
		if (triangle_couple.size() == 0)
			return;
		int coupleid = m_adjacentcouple->GetSelection();
		int t1pathid = parent->walkmesh.triangle_walkpath[triangle_couple[coupleid].first];
		int t2pathid = parent->walkmesh.triangle_walkpath[triangle_couple[coupleid].second];
		int16_t t1pathx = parent->walkmesh.offset_x + parent->walkmesh.walkpath_orgx[t1pathid];
		int16_t t1pathy = parent->walkmesh.offset_y + parent->walkmesh.walkpath_orgy[t1pathid];
		int16_t t1pathz = parent->walkmesh.offset_z + parent->walkmesh.walkpath_orgz[t1pathid];
		int16_t t2pathx = parent->walkmesh.offset_x + parent->walkmesh.walkpath_orgx[t2pathid];
		int16_t t2pathy = parent->walkmesh.offset_y + parent->walkmesh.walkpath_orgy[t2pathid];
		int16_t t2pathz = parent->walkmesh.offset_z + parent->walkmesh.walkpath_orgz[t2pathid];
		vector<int16_t> trianglepts;
		int v11, v12, tri2, side2, v2;
		GetVerticeFromEdge(triangle_couple[coupleid].first, triangle_couple_side[coupleid].first, v11, v12);
		tri2 = triangle_couple[coupleid].second;
		side2 = triangle_couple_side[coupleid].second;
		trianglepts.push_back(t1pathx + parent->walkmesh.vertex_x[v12]);
		trianglepts.push_back(t1pathy + parent->walkmesh.vertex_y[v12]);
		trianglepts.push_back(t1pathz + parent->walkmesh.vertex_z[v12]);
		trianglepts.push_back(t1pathx + parent->walkmesh.vertex_x[v11]);
		trianglepts.push_back(t1pathy + parent->walkmesh.vertex_y[v11]);
		trianglepts.push_back(t1pathz + parent->walkmesh.vertex_z[v11]);
		if (triangle_couple_common_vertex[coupleid].second == side2)
			v2 = parent->walkmesh.triangle_vertex[tri2].index[(side2 + 2) % 3];
		else
			v2 = parent->walkmesh.triangle_vertex[tri2].index[side2];
		trianglepts.push_back(t2pathx + parent->walkmesh.vertex_x[v2]);
		trianglepts.push_back(t2pathy + parent->walkmesh.vertex_y[v2]);
		trianglepts.push_back(t2pathz + parent->walkmesh.vertex_z[v2]);
		parent->gl_window->DisplayFieldPolygon(trianglepts);
	}

private:
	void OnChangeWalkpath(wxCommandEvent& event) {
		int sel = m_adjacency->GetSelection();
		if (sel == 0)
			DisplayIsolatedTrianglePos();
		else if (sel == 1)
			DisplaySingleTrianglePos(false, true);
		else if (sel == 2)
			DisplayCoupleTrianglePos();
	}

	void OnAdjacencyTypeChange(wxBookCtrlEvent& event) {
		int sel = event.GetSelection();
		if (sel == 0)
			DisplayIsolatedTrianglePos();
		else if (sel == 1)
			DisplaySingleTrianglePos(true, true);
		else if (sel == 2)
			DisplayCoupleTrianglePos();
	}

	void OnChangeTrianglePos(wxSpinEvent& event) {
		int id = event.GetId();
		if (id == wxID_TRIX4 || id == wxID_TRIY4 || id == wxID_TRIZ4) {
			DisplaySingleTrianglePos(false, false);
		} else {
			DisplayIsolatedTrianglePos();
		}
	}

	void OnChangeSingleTriangle(wxCommandEvent& event) {
		DisplaySingleTrianglePos(event.GetId() == wxID_TRIANGLE, true);
	}

	void OnChangeCoupleTriangle(wxCommandEvent& event) {
		DisplayCoupleTrianglePos();
	}

	void OnButtonClick(wxCommandEvent& event) {
		parent->gl_window->DisplayFieldClear(true, false, false, false);
		EndModal(event.GetId());
	}
};

WalkmeshEditDialog::WalkmeshEditDialog(wxWindow* parent, FieldTilesDataStruct* tiles, FieldWalkmeshDataStruct* walk) : WalkmeshEditWindow(parent) {
	walkmesh = *walk;
	extra_size = walk->GetExtraSize();
	path_selection = -1;
	triangle_selection = -1;
	ComputeDuplicateVertice({});
	path_popup_menu = new wxMenu();
	path_popup_menu->Append(wxID_ADD, HADES_STRING_GENERIC_ADD);
	path_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(WalkmeshEditDialog::OnWalkpathRightClickMenu), NULL, this);
	triangle_popup_menu = new wxMenu();
	triangle_popup_menu->Append(wxID_ADD, HADES_STRING_GENERIC_ADD);
	triangle_popup_menu->Append(wxID_PROPERTIES, HADES_STRING_WALKMESH_PROPAGATE_FLAGS);
	triangle_popup_menu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(WalkmeshEditDialog::OnTriangleRightClickMenu), NULL, this);
	gl_window = new GLWindow(m_displaypanel, NULL, 512);
	gl_window->DisplayField(tiles, &walkmesh);
	gl_window->Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(WalkmeshEditDialog::OnDisplayDClick), NULL, this);
	for (int i = 0; i < walkmesh.walkpath_amount; i++)
		m_pathlist->Append(wxString::Format(wxT("Walkpath %d"), i));
	//set<pair<int, int>> pathconnected;
	//for (i = 0; i < walkmesh.walkpath_amount; i++) {
	//	for (j = 0; j < walkmesh.walkpath_triangleamount[i]; j++) {
	//		uint16_t adjtri[3] = {
	//			walkmesh.triangle_adjacenttriangle1[walkmesh.walkpath_trianglelist[i][j]],
	//			walkmesh.triangle_adjacenttriangle2[walkmesh.walkpath_trianglelist[i][j]],
	//			walkmesh.triangle_adjacenttriangle3[walkmesh.walkpath_trianglelist[i][j]]
	//		};
	//		for (k = 0; k < 3; k++)
	//			if (adjtri[k] != -1) {
	//				pathconnected.insert({ i, walkmesh.triangle_walkpath[adjtri[k]] });
	//				pathconnected.insert({ walkmesh.triangle_walkpath[adjtri[k]], i });
	//			}
	//	}
	//}
	//vector<pair<int, int>> connectiontoadd;
	//set<pair<int, int>>::iterator it1, it2;
	//bool keepconnecting = true;
	//while (keepconnecting) {
	//	keepconnecting = false;
	//	connectiontoadd.clear();
	//	for (it1 = pathconnected.begin(); it1 != pathconnected.end(); it1++)
	//		for (it2 = pathconnected.begin(); it2 != pathconnected.end(); it2++) {
	//			if (it1->first == it2->first && pathconnected.find({ it1->second, it2->second }) == pathconnected.end())
	//				connectiontoadd.push_back({ it1->second, it2->second });
	//			else if (it1->first == it2->second && pathconnected.find({ it1->second, it2->first }) == pathconnected.end())
	//				connectiontoadd.push_back({ it1->second, it2->first });
	//			else if (it1->second == it2->first && pathconnected.find({ it1->first, it2->second }) == pathconnected.end())
	//				connectiontoadd.push_back({ it1->first, it2->second });
	//			else if (it1->second == it2->second && pathconnected.find({ it1->first, it2->first }) == pathconnected.end())
	//				connectiontoadd.push_back({ it1->first, it2->first });
	//		}
	//	for (i = 0; i < connectiontoadd.size(); i++) {
	//		pathconnected.insert(connectiontoadd[i]);
	//		pathconnected.insert({ connectiontoadd[i].second, connectiontoadd[i].first });
	//		keepconnecting = true;
	//	}
	//}
	//for (i = 0; i < walkmesh.walkpath_amount; i++) {
	//	vector<int> connection;
	//	for (j = 0; j < walkmesh.walkpath_amount; j++)
	//		if (i != j && pathconnected.find({ i, j }) != pathconnected.end())
	//			connection.push_back(j);
	//	path_connection.push_back(connection);
	//}
	m_pathlist->SetSelection(0);
	DisplayWalpath(0);
}

WalkmeshEditDialog::~WalkmeshEditDialog() {
	gl_window->Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(WalkmeshEditDialog::OnDisplayDClick), NULL, this);
}

int WalkmeshEditDialog::ShowModal() {
	return WalkmeshEditWindow::ShowModal();
}

void WalkmeshEditDialog::ComputeDuplicateVertice(vector<int> news) {
	vector<vector<int>> vabspos;
	vector<int> vpath;
	int i, j;
	vabspos.resize(walkmesh.vertex_amount);
	vpath.resize(walkmesh.vertex_amount);
	for (i = 0; i < walkmesh.triangle_amount; i++) {
		int16_t* triv = walkmesh.triangle_vertex[i].index;
		for (j = 0; j < 3; j++) {
			if (vabspos[triv[j]].size() == 3)
				continue;
			vabspos[triv[j]].resize(3);
			vabspos[triv[j]][0] = walkmesh.walkpath_orgx[walkmesh.triangle_walkpath[i]] + walkmesh.vertex_x[triv[j]];
			vabspos[triv[j]][1] = walkmesh.walkpath_orgy[walkmesh.triangle_walkpath[i]] + walkmesh.vertex_y[triv[j]];
			vabspos[triv[j]][2] = walkmesh.walkpath_orgz[walkmesh.triangle_walkpath[i]] + walkmesh.vertex_z[triv[j]];
			vpath[triv[j]] = walkmesh.triangle_walkpath[i];
		}
	}
	if (news.size() == 0) {
		for (i = 0; i < walkmesh.vertex_amount; i++)
			for (j = i + 1; j < walkmesh.vertex_amount; j++)
				if (vabspos[i].size() == 3 && vabspos[j].size() == 3 && vabspos[i][0] == vabspos[j][0] && vabspos[i][1] == vabspos[j][1] && vabspos[i][2] == vabspos[j][2])
					duplicate_vertex.push_back({ i, j, vpath[i], vpath[j] });
	} else {
		for (i = 0; i < news.size(); i++)
			for (j = 0; j < news[i]; j++)
				if (vabspos[news[i]].size() == 3 && vabspos[j].size() == 3 && vabspos[news[i]][0] == vabspos[j][0] && vabspos[news[i]][1] == vabspos[j][1] && vabspos[news[i]][2] == vabspos[j][2])
					duplicate_vertex.push_back({ j, news[i], vpath[j], vpath[news[i]] });
	}
}

void WalkmeshEditDialog::DisplayWalpath(int sel) {
	int trianglesel = sel == path_selection && m_trianglelist->GetSelection() != wxNOT_FOUND ? m_trianglelist->GetSelection() : 0;
	bool hastriangle = walkmesh.walkpath_triangleamount[sel] > 0;
	int16_t x, y, z;
	int i;
	m_trianglelist->Clear();
	for (i = 0; i < walkmesh.walkpath_triangleamount[sel]; i++)
		m_trianglelist->Append(wxString::Format(wxT("Triangle %d"), walkmesh.walkpath_trianglelist[sel][i]));
	walkmesh.GetPathCoordinate(m_pathpostype->GetSelection(), sel, x, y, z);
	m_pathx->SetValue(x);
	m_pathy->SetValue(y);
	m_pathz->SetValue(-z);
	path_selection = sel;
	gl_window->DisplayFieldPath(sel);
	m_trianglex1->Enable(hastriangle);
	m_triangley1->Enable(hastriangle);
	m_trianglez1->Enable(hastriangle);
	m_trianglex2->Enable(hastriangle);
	m_triangley2->Enable(hastriangle);
	m_trianglez2->Enable(hastriangle);
	m_trianglex3->Enable(hastriangle);
	m_triangley3->Enable(hastriangle);
	m_trianglez3->Enable(hastriangle);
	m_active->Enable(hastriangle);
	m_altstep->Enable(hastriangle);
	m_preventnpc->Enable(hastriangle);
	m_preventpc->Enable(hastriangle);
	m_trianglethetax->Enable(hastriangle);
	m_trianglethetay->Enable(hastriangle);
	m_triangled->Enable(hastriangle);
	m_trianglenormaltype->Enable(hastriangle);
	if (hastriangle) {
		m_trianglelist->SetSelection(trianglesel);
		DisplayTriangle(trianglesel);
	} else {
		triangle_selection = -1;
		gl_window->DisplayFieldTrianglePath(-1);
	}
}

void WalkmeshEditDialog::DisplayTriangle(int sel) {
	int absoluteid = walkmesh.walkpath_trianglelist[path_selection][sel];
	int v1 = walkmesh.triangle_vertex[absoluteid].index[0];
	int v2 = walkmesh.triangle_vertex[absoluteid].index[1];
	int v3 = walkmesh.triangle_vertex[absoluteid].index[2];
	int n = walkmesh.triangle_normal[absoluteid];
	m_active->SetValue((walkmesh.triangle_flag[absoluteid] & WALKMESH_TRIFLAG_ACTIVE) != 0);
	m_altstep->SetValue((walkmesh.triangle_flag[absoluteid] & WALKMESH_TRIFLAG_ALTERNATE_STEP) != 0);
	m_preventnpc->SetValue((walkmesh.triangle_flag[absoluteid] & WALKMESH_TRIFLAG_PREVENT_NPC) != 0);
	m_preventpc->SetValue((walkmesh.triangle_flag[absoluteid] & WALKMESH_TRIFLAG_PREVENT_PC) != 0);
	m_trianglex1->SetValue(walkmesh.vertex_x[v1]);
	m_triangley1->SetValue(walkmesh.vertex_y[v1]);
	m_trianglez1->SetValue(-walkmesh.vertex_z[v1]);
	m_trianglex2->SetValue(walkmesh.vertex_x[v2]);
	m_triangley2->SetValue(walkmesh.vertex_y[v2]);
	m_trianglez2->SetValue(-walkmesh.vertex_z[v2]);
	m_trianglex3->SetValue(walkmesh.vertex_x[v3]);
	m_triangley3->SetValue(walkmesh.vertex_y[v3]);
	m_trianglez3->SetValue(-walkmesh.vertex_z[v3]);
	m_trianglethetax->SetValue(walkmesh.triangle_thetax[absoluteid]);
	m_trianglethetay->SetValue(walkmesh.triangle_thetay[absoluteid]);
	m_triangled->SetValue(walkmesh.triangle_d[absoluteid] / 65536.0);
	m_trianglenormalx->Enable(false);
	m_trianglenormaly->Enable(false);
	m_trianglenormalz->Enable(false);
	m_trianglenormaloverz->Enable(false);
	if (n >= 0) {
		m_trianglenormaltype->SetSelection(1);
		m_trianglenormalx->SetValue(walkmesh.normal_x[n] / 65536.0);
		m_trianglenormaly->SetValue(walkmesh.normal_y[n] / 65536.0);
		m_trianglenormalz->SetValue(-walkmesh.normal_z[n] / 65536.0);
		m_trianglenormaloverz->SetValue(-walkmesh.normal_overz[n] / 65536.0);
	} else {
		m_trianglenormaltype->SetSelection(0);
		m_trianglenormalx->SetValue(0.0);
		m_trianglenormaly->SetValue(0.0);
		m_trianglenormalz->SetValue(-1.0);
		m_trianglenormaloverz->SetValue(-1.0);
	}
	gl_window->DisplayFieldTrianglePath(absoluteid);
	triangle_selection = absoluteid;
}

void WalkmeshEditDialog::ChangeVertexCoordinate(int triangleid, int vertid, int coordaxis, int16_t c) {
	set<pair<int, int>> linkedpathvert;
	vector<int> changedvert;
	int i, j, v1, v2, v3;
	if (m_changeallpos->IsChecked()) {
		for (i = 0; i < duplicate_vertex.size(); i++) {
			if (duplicate_vertex[i][0] == vertid)
				linkedpathvert.insert({ duplicate_vertex[i][3], duplicate_vertex[i][1] });
			else if (duplicate_vertex[i][1] == vertid)
				linkedpathvert.insert({ duplicate_vertex[i][2], duplicate_vertex[i][0] });
		}
	}
	if (coordaxis == 0)			walkmesh.vertex_x[vertid] = c;
	else if (coordaxis == 1)	walkmesh.vertex_y[vertid] = c;
	else if (coordaxis == 2)	walkmesh.vertex_z[vertid] = c;
	changedvert.push_back(vertid);
	for (auto p = linkedpathvert.begin(); p != linkedpathvert.end(); p++) {
		walkmesh.vertex_x[p->second] = walkmesh.walkpath_orgx[walkmesh.triangle_walkpath[triangleid]] + walkmesh.vertex_x[vertid] - walkmesh.walkpath_orgx[p->first];
		walkmesh.vertex_y[p->second] = walkmesh.walkpath_orgy[walkmesh.triangle_walkpath[triangleid]] + walkmesh.vertex_y[vertid] - walkmesh.walkpath_orgy[p->first];
		walkmesh.vertex_z[p->second] = walkmesh.walkpath_orgz[walkmesh.triangle_walkpath[triangleid]] + walkmesh.vertex_z[vertid] - walkmesh.walkpath_orgz[p->first];
		changedvert.push_back(p->second);
	}
	for (i = 0; i < walkmesh.triangle_amount; i++) {
		v1 = walkmesh.triangle_vertex[i].index[0];
		v2 = walkmesh.triangle_vertex[i].index[1];
		v3 = walkmesh.triangle_vertex[i].index[2];
		for (j = 0; j < changedvert.size(); j++)
			if (v1 == changedvert[j] || v2 == changedvert[j] || v3 == changedvert[j]) {
				walkmesh.triangle_centerx[i] = (walkmesh.vertex_x[v1] + walkmesh.vertex_x[v2] + walkmesh.vertex_x[v3]) / 3;
				walkmesh.triangle_centery[i] = (walkmesh.vertex_y[v1] + walkmesh.vertex_y[v2] + walkmesh.vertex_y[v3]) / 3;
				walkmesh.triangle_centerz[i] = (walkmesh.vertex_z[v1] + walkmesh.vertex_z[v2] + walkmesh.vertex_z[v3]) / 3;
				if ((i == triangleid && m_trianglenormaltype->GetSelection() == 1) || (i != triangleid && walkmesh.triangle_normal[i] >= 0))
					walkmesh.ComputeNormal(i);
				break;
			}
	}
	if (m_trianglenormaltype->GetSelection() == 1) {
		m_trianglenormalx->SetValue(walkmesh.normal_x[walkmesh.triangle_normal[triangleid]] / 65536.0);
		m_trianglenormaly->SetValue(walkmesh.normal_y[walkmesh.triangle_normal[triangleid]] / 65536.0);
		m_trianglenormalz->SetValue(-walkmesh.normal_z[walkmesh.triangle_normal[triangleid]] / 65536.0);
		m_trianglenormaloverz->SetValue(-walkmesh.normal_overz[walkmesh.triangle_normal[triangleid]] / 65536.0);
	}
}

void WalkmeshEditDialog::ChangeWalkpathCoordinate(int pathid, int coordaxis, int16_t c) {
	if (m_changeallpos->IsChecked()) {
		int16_t oldx, oldy, oldz, dc = 0;
		walkmesh.GetPathCoordinate(m_pathpostype->GetSelection(), pathid, oldx, oldy, oldz);
		if (coordaxis == 0)			dc = c - oldx;
		else if (coordaxis == 1)	dc = c - oldy;
		else if (coordaxis == 2)	dc = c - oldz;
		for (int i = 0; i < walkmesh.walkpath_amount; i++) {
			if (i == pathid)
				continue;
			walkmesh.GetPathCoordinate(m_pathpostype->GetSelection(), i, oldx, oldy, oldz);
			if (coordaxis == 0)			walkmesh.SetPathCoordinate(-1, i, coordaxis, oldx + dc);
			else if (coordaxis == 1)	walkmesh.SetPathCoordinate(-1, i, coordaxis, oldy + dc);
			else if (coordaxis == 2)	walkmesh.SetPathCoordinate(-1, i, coordaxis, oldz + dc);
		}
	}
	walkmesh.SetPathCoordinate(-1, pathid, coordaxis, c);
}

void WalkmeshEditDialog::OnListBox(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_PATH)
		DisplayWalpath(event.GetSelection());
	else if (id == wxID_TRIANGLE)
		DisplayTriangle(event.GetSelection());
}

void WalkmeshEditDialog::OnDisplayDClick(wxMouseEvent& event) {
	int triid = gl_window->GetMouseTriangle(event);
	if (triid >= 0) {
		int pathid = walkmesh.triangle_walkpath[triid];
		m_pathlist->SetSelection(pathid);
		DisplayWalpath(pathid);
		for (int i = 0; i < walkmesh.walkpath_triangleamount[pathid]; i++)
			if (walkmesh.walkpath_trianglelist[pathid][i] == triid) {
				m_trianglelist->SetSelection(i);
				DisplayTriangle(i);
				break;
			}
	}
	event.Skip();
}

void WalkmeshEditDialog::OnWalkpathRightClick(wxMouseEvent& event) {
	m_pathlist->PopupMenu(path_popup_menu);
	event.Skip();
}

void WalkmeshEditDialog::OnTriangleRightClick(wxMouseEvent& event) {
	m_trianglelist->PopupMenu(triangle_popup_menu);
	event.Skip();
}

void WalkmeshEditDialog::OnWalkpathRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADD) {
		if (walkmesh.AddWalkpath(extra_size, 0, 0, 0) != 0) {
			wxMessageDialog popup(this, HADES_STRING_DATA_REACH_LIMIT, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
			popup.ShowModal();
		} else {
			m_pathlist->Append(wxString::Format(wxT("Walkpath %d"), walkmesh.walkpath_amount - 1));
			m_pathlist->SetSelection(walkmesh.walkpath_amount - 1);
			DisplayWalpath(walkmesh.walkpath_amount - 1);
		}
	}
}

void WalkmeshEditDialog::OnTriangleRightClickMenu(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_ADD) {
		WalkmeshAddTriangleDialog dial(this, m_pathlist->GetSelection());
		if (dial.ShowModal() == wxID_OK) {
			int pathid = dial.m_walkpath->GetSelection();
			int tritype = dial.m_adjacency->GetSelection();
			int vertcount = walkmesh.vertex_amount;
			int result;
			if (tritype == 2 && dial.m_adjacentcouple->GetSelection() != wxNOT_FOUND) {
				int sel = dial.m_adjacentcouple->GetSelection();
				result = walkmesh.AddTriangle2(extra_size, pathid, WALKMESH_TRIFLAG_ACTIVE, 0, 0, 0,
					dial.triangle_couple[sel].first, dial.triangle_couple[sel].second,
					dial.triangle_couple_side[sel].first, dial.triangle_couple_side[sel].second,
					dial.triangle_couple_common_vertex[sel].first, dial.triangle_couple_common_vertex[sel].second);
			} else if (tritype == 1 && dial.m_adjacentsingle->GetSelection() != wxNOT_FOUND) {
				int trisel = dial.m_adjacentsingle->GetSelection();
				result = walkmesh.AddTriangle1(extra_size, pathid, WALKMESH_TRIFLAG_ACTIVE, 0, 0, 0,
					dial.triangle_single[trisel], dial.triangle_single_side[trisel][dial.m_side->GetSelection()],
					dial.third_vertex_x, dial.third_vertex_y, dial.third_vertex_z);
			} else if (tritype == 0) {
				result = walkmesh.AddTriangle(extra_size, pathid, WALKMESH_TRIFLAG_ACTIVE, 0, 0, 0,
					{ static_cast<int16_t>(dial.m_trianglex1->GetValue()), static_cast<int16_t>(dial.m_trianglex2->GetValue()), static_cast<int16_t>(dial.m_trianglex3->GetValue()) },
					{ static_cast<int16_t>(dial.m_triangley1->GetValue()), static_cast<int16_t>(dial.m_triangley2->GetValue()), static_cast<int16_t>(dial.m_triangley3->GetValue()) },
					{ static_cast<int16_t>(-dial.m_trianglez1->GetValue()), static_cast<int16_t>(-dial.m_trianglez2->GetValue()), static_cast<int16_t>(-dial.m_trianglez3->GetValue()) });
			} else {
				result = -1;
			}
			if (result == 0) {
				vector<int> newduplicates;
				for (int i = vertcount; i < walkmesh.vertex_amount; i++)
					newduplicates.push_back(i);
				if (newduplicates.size() > 0)
					ComputeDuplicateVertice(newduplicates);
				gl_window->RecomputeGeometry(false);
				m_pathlist->SetSelection(pathid);
				DisplayWalpath(pathid);
				m_trianglelist->SetSelection(walkmesh.walkpath_triangleamount[pathid] - 1);
				DisplayTriangle(walkmesh.walkpath_triangleamount[pathid] - 1);
			} else if (result == -1) {
				wxMessageDialog popup(this, HADES_STRING_WALKMESH_ERROR_NO_TRIANGLE, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
				popup.ShowModal();
			} else if (result == 1) {
				wxMessageDialog popup(this, HADES_STRING_DATA_REACH_LIMIT, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
				popup.ShowModal();
			} else if (result == 2) {
				wxMessageDialog popup(this, HADES_STRING_WALKMESH_INVALID_TRIANGLE, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
				popup.ShowModal();
			} else if (result == 3) {
				wxMessageDialog popup(this, HADES_STRING_WALKMESH_WRONG_CONNECTION, HADES_STRING_ERROR, wxOK | wxSTAY_ON_TOP | wxCENTRE);
				popup.ShowModal();
			}
		}
	} else if (id == wxID_PROPERTIES && triangle_selection >= 0) {
		int path = walkmesh.triangle_walkpath[triangle_selection];
		int count = walkmesh.walkpath_triangleamount[path];
		uint16_t flag = walkmesh.triangle_flag[triangle_selection];
		for (int i = 0; i < count; i++)
			walkmesh.triangle_flag[walkmesh.walkpath_trianglelist[path][i]] = flag;
	}
}

void WalkmeshEditDialog::OnSpinCtrl(wxSpinEvent& event) {
	int id = event.GetId();
	if (id == wxID_PATHX)		ChangeWalkpathCoordinate(path_selection, 0, event.GetPosition());
	else if (id == wxID_PATHY)	ChangeWalkpathCoordinate(path_selection, 1, event.GetPosition());
	else if (id == wxID_PATHZ)	ChangeWalkpathCoordinate(path_selection, 2, -event.GetPosition());
	else if (id == wxID_TRIX1)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[0], 0, event.GetPosition());
	else if (id == wxID_TRIY1)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[0], 1, event.GetPosition());
	else if (id == wxID_TRIZ1)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[0], 2, -event.GetPosition());
	else if (id == wxID_TRIX2)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[1], 0, event.GetPosition());
	else if (id == wxID_TRIY2)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[1], 1, event.GetPosition());
	else if (id == wxID_TRIZ2)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[1], 2, -event.GetPosition());
	else if (id == wxID_TRIX3)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[2], 0, event.GetPosition());
	else if (id == wxID_TRIY3)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[2], 1, event.GetPosition());
	else if (id == wxID_TRIZ3)	ChangeVertexCoordinate(triangle_selection, walkmesh.triangle_vertex[triangle_selection].index[2], 2, -event.GetPosition());
	else {
		if (id == wxID_ANGLE1)			walkmesh.triangle_thetax[triangle_selection] = event.GetPosition();
		else if (id == wxID_ANGLE2)		walkmesh.triangle_thetay[triangle_selection] = event.GetPosition();
		return;
	}
	gl_window->RecomputeGeometry();
}

void WalkmeshEditDialog::OnSpinCtrlDouble(wxSpinDoubleEvent& event) {
	int id = event.GetId();
	if (id == wxID_DISTANCE)	walkmesh.triangle_d[triangle_selection] = (int32_t)round(event.GetValue() * 65536.0);
	else if (id == wxID_TRIX4)	walkmesh.normal_x[walkmesh.triangle_normal[triangle_selection]] = (int32_t)round(event.GetValue() * 65536.0);
	else if (id == wxID_TRIY4)	walkmesh.normal_y[walkmesh.triangle_normal[triangle_selection]] = (int32_t)round(event.GetValue() * 65536.0);
	else if (id == wxID_TRIZ4)	walkmesh.normal_z[walkmesh.triangle_normal[triangle_selection]] = (int32_t)round(-event.GetValue() * 65536.0);
	else if (id == wxID_OVERZ)	walkmesh.normal_overz[walkmesh.triangle_normal[triangle_selection]] = (int32_t)round(-event.GetValue() * 65536.0);
}

void WalkmeshEditDialog::OnChoice(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_POSTYPE) {
		int16_t x, y, z;
		walkmesh.GetPathCoordinate(event.GetSelection(), path_selection, x, y, z);
		m_pathx->SetValue(x);
		m_pathy->SetValue(y);
		m_pathz->SetValue(-z);
		gl_window->field_walk_offset_type = event.GetSelection();
		gl_window->RecomputeGeometry();
	} else if (id == wxID_NORMAL) {
		int sel = event.GetSelection();
		if (sel != 0) {
			if (walkmesh.triangle_normal[triangle_selection] < 0) {
				if (walkmesh.AddNormal(extra_size, triangle_selection, 0.0, 0.0, 1.0, 1.0) != 0) {
					wxMessageDialog popup(NULL, HADES_STRING_DATA_REACH_LIMIT, HADES_STRING_ERROR, wxOK | wxCENTRE);
					popup.ShowModal();
					sel = 0;
					m_trianglenormaltype->SetSelection(0);
				}
			}
		}
		if (sel == 1)
			walkmesh.ComputeNormal(triangle_selection);
		if (sel == 0) {
			walkmesh.RemoveNormal(extra_size, triangle_selection);
			m_trianglenormalx->SetValue(0.0);
			m_trianglenormaly->SetValue(0.0);
			m_trianglenormalz->SetValue(-1.0);
			m_trianglenormaloverz->SetValue(-1.0);
		} else {
			m_trianglenormalx->SetValue(walkmesh.normal_x[walkmesh.triangle_normal[triangle_selection]] / 65536.0);
			m_trianglenormaly->SetValue(walkmesh.normal_y[walkmesh.triangle_normal[triangle_selection]] / 65536.0);
			m_trianglenormalz->SetValue(-walkmesh.normal_z[walkmesh.triangle_normal[triangle_selection]] / 65536.0);
			m_trianglenormaloverz->SetValue(-walkmesh.normal_overz[walkmesh.triangle_normal[triangle_selection]] / 65536.0);
		}
		m_trianglenormalx->Enable(sel == 2);
		m_trianglenormaly->Enable(sel == 2);
		m_trianglenormalz->Enable(sel == 2);
		m_trianglenormaloverz->Enable(sel == 2);
	}
}

void WalkmeshEditDialog::OnCheckBox(wxCommandEvent& event) {
	int id = event.GetId();
	uint16_t mask = 0;
	if (id == wxID_ACTIVE)		mask = ~WALKMESH_TRIFLAG_ACTIVE;
	else if (id == wxID_STEP)	mask = ~WALKMESH_TRIFLAG_ALTERNATE_STEP;
	else if (id == wxID_NPC)	mask = ~WALKMESH_TRIFLAG_PREVENT_NPC;
	else if (id == wxID_PC)		mask = ~WALKMESH_TRIFLAG_PREVENT_PC;
	if (mask != 0) {
		if (event.GetInt())	walkmesh.triangle_flag[triangle_selection] |= ~mask;
		else				walkmesh.triangle_flag[triangle_selection] &= mask;
	}
}

void WalkmeshEditDialog::OnButtonClick(wxCommandEvent& event) {
	int id = event.GetId();
	if (id == wxID_OK || id == wxID_CANCEL)
		EndModal(id);
}
