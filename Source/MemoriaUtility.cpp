#include "MemoriaUtility.h"

#include "Database_Resource.h"

SaveCustomFieldDialog::SaveCustomFieldDialog(wxWindow* parent, wxString defaultpath, SaveSet& saveset) : SaveCustomFieldWindow(parent) {
	svset = &saveset;
	m_dirpicker->SetPath(defaultpath);
	wxArrayString choicelist;
	unsigned int i;
	for (i = 0; i < svset->fieldset->amount; i++)
		choicelist.Add(wxString::Format(wxT("%04u: %s"), svset->fieldset->script_data[i]->object_id, svset->fieldset->script_data[i]->name.str_nice));
	m_basefield->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < svset->textset->amount; i++)
		choicelist.Add(wxString::Format(wxT("%04u: %s"), svset->textset->struct_id[i], svset->textset->name[i]));
	m_textidchoice->Append(choicelist);
	m_basefield->SetSelection(0);
	m_textidchoice->SetSelection(0);
	m_textid->SetValue(svset->textset->struct_id[0]);
	UpdateDictionaryPatchCommand();
}

int SaveCustomFieldDialog::DoGenerate(ConfigurationSet& config, SaveSet& saveset) {
	if (m_dirpicker->GetPath().IsEmpty())
		return -1;
	return CreateSteamCustomFieldAssets(m_dirpicker->GetPath().ToStdString(), config, saveset, m_basefield->GetSelection(), m_scriptid->GetValue(), m_areaid->GetValue(), m_mapid->GetValue().ToStdString(), m_fieldid->GetValue().ToStdString(), m_textid->GetValue());
}

void SaveCustomFieldDialog::UpdateDictionaryPatchCommand() {
	m_dictionarypatch->SetValue(wxString::Format(wxT("FieldScene %u %u %s %s %u"), m_scriptid->GetValue(), m_areaid->GetValue(), m_mapid->GetValue(), m_fieldid->GetValue(), m_textid->GetValue()));
}

void SaveCustomFieldDialog::OnTextBlockChoice(wxCommandEvent& event) {
	m_textid->SetValue(svset->textset->struct_id[event.GetSelection()]);
	UpdateDictionaryPatchCommand();
}

void SaveCustomFieldDialog::OnUpdateArg(wxCommandEvent& event) {
	UpdateDictionaryPatchCommand();
}

void SaveCustomFieldDialog::OnUpdateArgSpin(wxSpinEvent& event) {
	UpdateDictionaryPatchCommand();
}

SaveCustomBattleDialog::SaveCustomBattleDialog(wxWindow* parent, wxString defaultpath, SaveSet& saveset) : SaveCustomBattleWindow(parent) {
	svset = &saveset;
	m_dirpicker->SetPath(defaultpath);
	wxArrayString choicelist;
	unsigned int i;
	for (i = 0; i < svset->enemyset->battle_amount; i++)
		choicelist.Add(wxString::Format(wxT("%04u: %s"), svset->enemyset->battle_data[i]->object_id, svset->enemyset->battle_name[i]));
	m_basebattle->Append(choicelist);
	choicelist.Clear();
	for (i = 0; i < G_V_ELEMENTS(HADES_STRING_BATTLE_SCENE_NAME); i++)
		choicelist.Add(HADES_STRING_BATTLE_SCENE_NAME[i].label);
	m_backgroundidchoice->Append(choicelist);
	m_basebattle->SetSelection(0);
	m_backgroundidchoice->SetSelection(0);
	m_backgroundid->ChangeValue(_(L"BBG_") + HADES_STRING_BATTLE_SCENE_NAME[0].steamid);
	UpdateDictionaryPatchCommand();
}

int SaveCustomBattleDialog::DoGenerate(ConfigurationSet& config, SaveSet& saveset) {
	if (m_dirpicker->GetPath().IsEmpty())
		return -1;
	return CreateSteamCustomBattleAssets(m_dirpicker->GetPath().ToStdString(), config, saveset, m_basebattle->GetSelection(), m_scriptid->GetValue(), m_battleid->GetValue().ToStdString(), m_backgroundid->GetValue().ToStdString());
}

void SaveCustomBattleDialog::UpdateDictionaryPatchCommand() {
	m_dictionarypatch->SetValue(wxString::Format(wxT("BattleScene %u %s %s"), m_scriptid->GetValue(), m_battleid->GetValue(), m_backgroundid->GetValue()));
}

void SaveCustomBattleDialog::OnBackgroundChoice(wxCommandEvent& event) {
	m_backgroundid->ChangeValue(_(L"BBG_") + HADES_STRING_BATTLE_SCENE_NAME[event.GetSelection()].steamid);
	UpdateDictionaryPatchCommand();
}

void SaveCustomBattleDialog::OnUpdateArg(wxCommandEvent& event) {
	UpdateDictionaryPatchCommand();
}

void SaveCustomBattleDialog::OnUpdateArgSpin(wxSpinEvent& event) {
	UpdateDictionaryPatchCommand();
}
