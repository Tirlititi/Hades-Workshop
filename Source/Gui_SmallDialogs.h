#pragma once

#include "gui.h"
#include "Gui_Manipulation.h"
#include "Configuration.h"

class DiscardableMessageWindow : public DiscardableMessage {
public:
	DiscardableMessageWindow(wxWindow* parent, wxString text);

private:
	void OnButtonClickDiscard(wxCommandEvent& event);
	void OnButtonClickOk(wxCommandEvent& event);
};

class ImageMapEditDialog : public ImageMapEditWindow {
public:
	ImageMapDataStruct map;
	ImageMapDataStruct* enemymap;
	GlobalImageMapStruct* globalmap;
	EnemyDataSet* enemyset;
	FieldDataSet* fieldset;
	wxArrayString globalliststr[7];
	uint16_t** globallistid[7];
	int extradata;

	ImageMapEditDialog(wxWindow* parent, ImageMapDataStruct* imgmap, ImageMapDataStruct* enemy, GlobalImageMapStruct* global, SaveSet& saveset);
	int ShowModal();

private:
	void RefreshMapList();

	void OnChoice(wxCommandEvent& event);
	void OnList(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class SteamImageMapEditDialog : public SteamImageMapEditWindow {
public:
	ImageMapDataStruct map;
	unsigned int type_sel;

	SteamImageMapEditDialog(wxWindow* parent, ImageMapDataStruct* imgmap);
	int ShowModal();
	void UpdatePreloadList();

private:
	void OnChoice(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class SpellModelWindow : public SpellAnimationWindow {
public:
	SpellModelWindow(wxWindow* parent);
	int ShowModal(Spell_Target_Type tt, Spell_Target_Amount ta, uint16_t currentmodel, uint16_t currentmodelalt = 0, bool alt = false);

	void OnSpin(wxSpinEvent& event);
	void OnChoice(wxCommandEvent& event);
	void OnCheckBox(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class SpellStatusDialog : public SpellStatusWindow {
public:
	int status_set_id;
	CDDataStruct* cddata;
	set<int> status_list;

	SpellStatusDialog(wxWindow* parent);
	int ShowModal(CDDataStruct* data, StatusSetStruct& status);

	static wxString StatusDisplayName(int status);

private:
	void OnButtonClick(wxCommandEvent& event);
	void OnBaseListSelect(wxCommandEvent& event);
	void OnSpinCtrl(wxSpinEvent& event);
};

class ItemStatDialog : public ItemStatWindow {
public:
	ItemStatDialog(wxWindow* parent);
	int ShowModal(ItemStatDataStruct& is);

private:
	void OnButtonClick(wxCommandEvent& event);
};

class CardSetDialog : public CardSetWindow {
public:
	CardSetDialog(wxWindow* parent);
	int ShowModal(CardDataSet& cs, unsigned int set);

private:
	void OnButtonClick(wxCommandEvent& event);
};

class CharacterParameterDialog : public CharacterParameterWindow {
public:
	vector<wxChoice*> m_listanimchoice;
	vector<wxTextCtrl*> m_listanimname;
	wxArrayString animname;
	wxArrayString animlabel;

	CharacterParameterDialog(wxWindow* parent);
	int ShowModal(CharBattleParameterStruct& btl, InitialStatDataStruct& stat);
	void ApplyModifications(CharBattleParameterStruct& btl, InitialStatDataStruct& stat);
	void ColorSelection();
	void ModelSelection();

private:
	void OnChangeColor(wxSpinEvent& event);
	void OnChangeModel(wxCommandEvent& event);
	void OnAnimChoice(wxCommandEvent& event);
	void OnAnimKey(wxKeyEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class EnemyResourceDialog : public EnemyResourceWindow {
public:
	vector<wxChoice*> m_listanim;
	vector<wxSpinCtrl*> m_listanimid;
	vector<wxButton*> m_listanimdelete;
	wxArrayString anim;
	vector<uint16_t> anim_id;

	EnemyResourceDialog(wxWindow* parent);
	int ShowModal(EnemyStatDataStruct& es, BattleDataStruct& bd);
	bool ApplyModifications(EnemyStatDataStruct& es, BattleDataStruct& bd);

private:
	void OnAnimChoice(wxCommandEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

class DatabaseFormatDialog : public DatabaseFormatWindow {
public:
	int field_count;
	vector<wxTextCtrl*> field_name;
	vector<wxTextCtrl*> field_default;
	vector<wxButton*> field_delete_btn;
	wxString* header_original;
	wxString* format_original;
	map<wxString, wxString>* fields_original;

	DatabaseFormatDialog(wxWindow* parent, wxString* header, wxString* format, map<wxString, wxString>* fields);
	map<wxString, wxString> GetFieldMap();
	void ApplyChanges();

private:
	void AddField(wxString name, wxString defaultValue);
	void RemoveField(int field_index);
	void OnButtonClick(wxCommandEvent& event);
};

class DatabaseFieldDialog : public DatabaseFieldWindow {
public:
	vector<wxTextCtrl*> field_name;
	vector<wxTextCtrl*> field_value;
	map<wxString, wxString>* fields_original;

	DatabaseFieldDialog(wxWindow* parent, map<wxString, wxString>* fields, map<wxString, wxString>& parentfields);
	map<wxString, wxString> GetFieldMap();

private:
	void AddField(wxString name, wxString val, wxString defaultValue);
	void OnButtonClick(wxCommandEvent& event);
};