#include "Statuses.h"

#include "main.h"
#include "Database_CSV.h"
#include "Database_Spell.h"
#include "CommonUtility.h"

#define STATUSDATA_HWS_VERSION 0

#define STATUSDATA_CSV_CHECK L"%id%;%priority%;%tick%;%duration%;%clear%;%immune%;%sps%;%sps_attach%;%sps_pos%;%shp%;%shp_attach%;%shp_pos%;%color_kind%;%color_priority%;%color%"
#define STATUSDATA_CSV_DEFAULT L"%name%;" STATUSDATA_CSV_CHECK L";# %name%"

wxString StatusDataStruct::GetFieldValue(wxString fieldname) {
	if (fieldname.IsSameAs("name")) return name;
	if (fieldname.IsSameAs("id")) return wxString::Format(wxT("%d"), id);
	if (fieldname.IsSameAs("priority")) return wxString::Format(wxT("%d"), priority);
	if (fieldname.IsSameAs("tick")) return wxString::Format(wxT("%d"), tick);
	if (fieldname.IsSameAs("duration")) return wxString::Format(wxT("%d"), duration);
	if (fieldname.IsSameAs("clear")) return FormatStatusSet(clear);
	if (fieldname.IsSameAs("immune")) return FormatStatusSet(immune);
	if (fieldname.IsSameAs("sps")) return wxString::Format(wxT("%d"), sps);
	if (fieldname.IsSameAs("sps_attach")) return wxString::Format(wxT("%d"), sps_attach);
	if (fieldname.IsSameAs("sps_pos")) return wxString::Format(wxT("%d, %d, %d"), sps_pos[0], sps_pos[1], sps_pos[2]);
	if (fieldname.IsSameAs("shp")) return wxString::Format(wxT("%d"), shp);
	if (fieldname.IsSameAs("shp_attach")) return wxString::Format(wxT("%d"), shp_attach);
	if (fieldname.IsSameAs("shp_pos")) return wxString::Format(wxT("%d, %d, %d"), shp_pos[0], shp_pos[1], shp_pos[2]);
	if (fieldname.IsSameAs("color_kind")) return wxString::Format(wxT("%d"), color_kind);
	if (fieldname.IsSameAs("color_priority")) return wxString::Format(wxT("%d"), color_priority);
	if (fieldname.IsSameAs("color")) return wxString::Format(wxT("%d, %d, %d"), color[0], color[1], color[2]);
	if (auto search = custom_field.find(fieldname); search != custom_field.end()) return search->second;
	if (auto search = parent->custom_field.find(fieldname); search != parent->custom_field.end()) return search->second;
	return _(L"");
}

bool StatusDataStruct::CompareWithCSV(wxArrayString entries) {
	if (id >= (int)entries.GetCount())
		return false;
	if (!custom_field.empty())
		return false;
	wxString rightcsv = MemoriaUtility::GenerateDatabaseEntryGeneric(*this, _(STATUSDATA_CSV_CHECK));
	return MemoriaUtility::CompareEntries(entries[id].AfterFirst(L';').BeforeLast(L';'), rightcsv);
}

void StatusDataSet::Load(fstream& ffbin, ConfigurationSet& config) {
	csv_header = _(HADES_STRING_CSV_STATUSDATA_HEADER);
	csv_format = _(STATUSDATA_CSV_DEFAULT);
	status.resize(STATUS_AMOUNT);
	for (int i = 0; i < STATUS_AMOUNT; i++) {
		status[i].parent = this;
		status[i].id = i;
		status[i].name = HADES_STRING_STATUS[i];
	}
	status[0].priority = 2;
	status[0].clear = { 27 };
	status[0].immune = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	// TODO
}

void StatusDataSet::Write(fstream& ffbin, ConfigurationSet& config) {
}

void StatusDataSet::LoadHWS(fstream& ffhws, bool usetext) {
	// TODO
}

void StatusDataSet::WriteHWS(fstream& ffhws) {
	// TODO
}

bool StatusDataSet::GenerateCSV(string basefolder) {
	return MemoriaUtility::GenerateDatabaseGeneric<StatusDataStruct>(_(basefolder), _(HADES_STRING_CSV_STATUSDATA_FILE), csv_header, _(L"\n"), _(L"\n"), status, csv_format, true);
}
