#include "CommonUtility.h"

#include <wx/file.h>
#include <wx/translation.h>
#include "Configuration.h"
#include "File_Manipulation.h"

//==========================
// Non-templates

bool IsNumber(string s) {
	return !s.empty() && find_if(s.begin(), s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}

wxArrayString MemoriaUtility::LoadCSVLines(wxString filename, wxArrayString* metadata) {
	wxArrayString lines;
	if (!wxFile::Exists(filename))
		return lines;
	wxFile csvfile;
	if (!csvfile.Open(filename, wxFile::read))
		return lines;
	wxString fullfile, rest, line;
	bool success = csvfile.ReadAll(&fullfile);
	csvfile.Close();
	if (!success)
		return lines;
	csvfile.Close();
	while (!fullfile.IsEmpty()) {
		line = fullfile.BeforeFirst('\n', &rest);
		fullfile = rest;
		if (line.StartsWith(_(L"\r")))
			line = line.Mid(1);
		if (line.EndsWith(_(L"\r")))
			line = line.Left(line.Len() - 1);
		if (line.IsEmpty())
			continue;
		if (line.StartsWith(_(L"#"))) {
			if (metadata != NULL && line.StartsWith(_(L"#!"))) {
				line = line.Mid(2);
				line.Trim(false);
				metadata->Add(line);
			}
			continue;
		}
		lines.Add(line);
	}
	return lines;
}

wxArrayString MemoriaUtility::LoadCSVEntry(wxString csvline) {
	wxArrayString entries;
	wxString entry, rest;
	while (!csvline.IsEmpty()) {
		entry = csvline.BeforeFirst(';', &rest);
		csvline = rest;
		if (entry.StartsWith(_(L"#")))
			return entries;
		entries.Add(entry);
	}
	return entries;
}

wxArrayString MemoriaUtility::SplitEntryArray(wxString csventry) {
	wxArrayString values;
	wxString val, rest;
	while (!csventry.IsEmpty()) {
		val = csventry.BeforeFirst(',', &rest);
		csventry = rest;
		values.Add(val.Trim(true).Trim(false));
	}
	return values;
}

int MemoriaUtility::GetEntryEnum(wxString csventry) {
	long val;
	if (csventry.ToLong(&val))
		return val;
	if (csventry.AfterFirst(L'(').BeforeFirst(L')').ToLong(&val))
		return val;
	return 0;
}

bool MemoriaUtility::CSV_ComparerWithoutStart(wxString left, wxString right) {
	return left.AfterFirst(L';').IsSameAs(right.AfterFirst(L';'));
}

bool MemoriaUtility::CSV_ComparerWithoutEnd(wxString left, wxString right) {
	return left.BeforeLast(L';').IsSameAs(right.BeforeLast(L';'));
}

bool MemoriaUtility::CSV_ComparerWithoutBoth(wxString left, wxString right) {
	return left.AfterFirst(L';').BeforeLast(L';').IsSameAs(right.AfterFirst(L';').BeforeLast(L';'));
}

//==========================
// Template definitions

template<typename T>
int InsertAtId(vector<T>& base, T& newobj, int newobjid) {
	int index;
	for (index = base.size() - 1; index >= 0; index--) {
		if (base[index].id == newobjid) {
			return -1;
		} else if (base[index].id < newobjid) {
			base.insert(base.begin() + index + 1, newobj);
			base[index + 1].id = newobjid;
			return index + 1;
		}
	}
	base.insert(base.begin(), newobj);
	base[0].id = newobjid;
	return 0;
}

template<>
int InsertAtId<int>(vector<int>& base, int& newobj, int newobjid) {
	int index;
	for (index = base.size() - 1; index >= 0; index--) {
		if (base[index] == newobjid) {
			return -1;
		} else if (base[index] < newobjid) {
			base.insert(base.begin() + index + 1, newobj);
			base[index + 1] = newobjid;
			return index + 1;
		}
	}
	base.insert(base.begin(), newobj);
	base[0] = newobjid;
	return 0;
}

template<typename T>
int PrepareHWSFlexibleList(fstream& ffbin, vector<T>& objlist, vector<T>& nonmodifiedlist, vector<int>& insertedindices) {
	vector<int> objid;
	T objprototype;
	bool objexists;
	int objamount;
	int addindex;
	int i, j;
	nonmodifiedlist.clear();
	insertedindices.clear();
	HWSReadFlexibleChar(ffbin, objamount, true);
	objid.resize(objamount);
	for (i = 0; i < objamount; i++)
		HWSReadFlexibleChar(ffbin, objid[i], true);
	for (i = 0; i < (int)objlist.size(); i++) {
		objexists = false;
		for (j = 0; j < objamount; j++) {
			if (objlist[i].id == objid[j]) {
				objexists = true;
				break;
			}
		}
		if (!objexists) {
			nonmodifiedlist.push_back(objlist[i]);
			objlist.erase(objlist.begin() + i);
			i--;
		}
	}
	for (i = 0; i < objamount; i++) {
		addindex = InsertAtId(objlist, objprototype, objid[i]);
		if (addindex >= 0)
			insertedindices.push_back(addindex);
	}
	return objamount;
}

template<typename T>
int GetSteamTextSizeGeneric(vector<T>& list, FF9String T::* field, bool asmes, SteamLanguage lang) {
	int size = 0;
	if (list.size() > 0) {
		if (asmes && list[0].id != 0)
			size += 7 + to_string(list[0].id).length();
		size += (list[0].*field).GetLength(lang);
	}
	for (unsigned i = 1; i < list.size(); i++) {
		if (asmes && list[i - 1].id + 1 != list[i].id)
			size += 7 + to_string(list[i].id).length();
		size += (list[i].*field).GetLength(lang);
	}
	return size;
}

template<typename T>
void WriteSteamTextGeneric(fstream& f, vector<T>& list, FF9String T::*field, vector<int>* subindex, bool asmes, SteamLanguage lang) {
	unsigned int count = subindex != NULL ? subindex->size() : list.size();
	unsigned int index;
	int lastid = -1;
	for (unsigned int i = 0; i < count; i++) {
		index = subindex != NULL ? (*subindex)[i] : i;
		if (asmes && lastid + 1 != list[index].id)
			WriteString(f, "[TXID=" + to_string(list[index].id) + "]");
		SteamWriteFF9String(f, list[index].*field, lang);
		lastid = list[index].id;
	}
}

template<typename T>
bool MemoriaUtility::GetModifiedSteamTexts(vector<int>* result, int32_t baseassetid[], vector<T>& objlist, function<wstring(T&)> stringifier, SteamLanguage lang) {
	if (GetGameConfiguration() == NULL)
		return false;
	fstream resourcearchive(GetGameConfiguration()->steam_dir_data + "resources.assets", ios::in | ios::binary);
	if (!resourcearchive.is_open())
		return false;
	uint32_t assetoffset = GetGameConfiguration()->meta_res.GetFileOffsetByIndex(baseassetid[lang]);
	uint32_t assetsize = GetGameConfiguration()->meta_res.GetFileSizeByIndex(baseassetid[lang]);
	int strindex = 0;
	FF9String str;
	resourcearchive.seekg(assetoffset);
	while ((uint32_t)resourcearchive.tellg() - assetoffset + 4 < assetsize && strindex < (int)objlist.size()) {
		SteamReadFF9String(resourcearchive, str, lang);
		if (strindex >= (int)objlist.size() || str.multi_lang_str[lang].compare(stringifier(objlist[strindex])) != 0)
			result->push_back(strindex);
		strindex++;
	}
	while (strindex < (int)objlist.size())
		result->push_back(strindex++);
	resourcearchive.close();
	return true;
}

template<typename T>
string ConcatenateStrings(string delim, vector<T>& objlist, function<string(T)> stringifier, bool escapeempty) {
	if (escapeempty) {
		string resescape = "";
		string token;
		for (unsigned int i = 0; i < objlist.size(); i++) {
			token = stringifier(objlist[i]);
			if (token.empty())
				continue;
			if (!resescape.empty())
				resescape += delim;
			resescape += token;
		}
		return resescape;
	}
	if (objlist.size() == 0)
		return "";
	string result = stringifier(objlist[0]);
	for (unsigned int j = 1; j < objlist.size(); j++)
		result += delim + stringifier(objlist[j]);
	return result;
}
string ConcatenateStrings(string delim, vector<string>& objlist, bool escapeempty) {
	if (escapeempty) {
		string resescape = "";
		for (unsigned int i = 0; i < objlist.size(); i++) {
			if (objlist[i].empty())
				continue;
			if (!resescape.empty())
				resescape += delim;
			resescape += objlist[i];
		}
		return resescape;
	}
	if (objlist.size() == 0)
		return "";
	string result = objlist[0];
	for (unsigned int i = 1; i < objlist.size(); i++)
		result += delim + objlist[i];
	return result;
}

template<typename T>
bool MemoriaUtility::GenerateCSVGeneric(wxString modfolder, wxString csvpath, wxString csvheader, vector<T>& objlist, function<wxString(T&, int)> entryconstructor, function<bool(wxString, wxString)> entrycomparer, bool skipnonmodified) {
	wxArrayString basecsv;
	wxString content, entryline;
	if (skipnonmodified && GetGameConfiguration() != NULL)
		basecsv = MemoriaUtility::LoadCSVLines(_(GetGameConfiguration()->steam_dir_assets) + csvpath);
	for (int i = 0; i < (int)objlist.size(); i++) {
		entryline = entryconstructor(objlist[i], i);
		if (entryline.IsEmpty())
			continue;
		if (skipnonmodified && objlist[i].id < (int)basecsv.GetCount()) {
			if (entrycomparer(basecsv[objlist[i].id], entryline))
				continue;
		}
		content += entryline + L"\n";
	}
	if (content.IsEmpty())
		return true;
	wxFile csvfile;
	if (!csvfile.Open(modfolder + csvpath, wxFile::write))
		return false;
	if (!csvfile.Write(csvheader))
		return false;
	if (!csvfile.Write(content))
		return false;
	csvfile.Close();
	return true;
}


//==========================
// Template instantiations

#define MACRO_INSTANTIATE_InsertAtId(T) template int InsertAtId<T>(vector<T>&, T&, int);
#define MACRO_INSTANTIATE_PrepareHWSFlexibleList(T) template int PrepareHWSFlexibleList(fstream& ffbin, vector<T>& objlist, vector<T>& nonmodifiedlist, vector<int>& insertedindices);
#define MACRO_INSTANTIATE_GetSteamTextSizeGeneric(T) template int GetSteamTextSizeGeneric(vector<T>& list, FF9String T::*field, bool asmes, SteamLanguage lang);
#define MACRO_INSTANTIATE_WriteSteamTextGeneric(T) template void WriteSteamTextGeneric(fstream& f, vector<T>& list, FF9String T::*field, vector<int>* subindex, bool asmes, SteamLanguage lang);
#define MACRO_INSTANTIATE_GetModifiedSteamTexts(T) template bool MemoriaUtility::GetModifiedSteamTexts(vector<int>* result, int32_t baseassetid[], vector<T>& objlist, function<wstring(T&)> stringifier, SteamLanguage lang);
#define MACRO_INSTANTIATE_ConcatenateStrings(T) template string ConcatenateStrings(string delim, vector<T>& objlist, function<string(T)> stringifier, bool escapeempty);
#define MACRO_INSTANTIATE_GenerateCSVGeneric(T) template bool MemoriaUtility::GenerateCSVGeneric(wxString modfolder, wxString csvpath, wxString csvheader, vector<T>& objlist, function<wxString(T&, int)> entryconstructor, function<bool(wxString, wxString)> entrycomparer, bool skipnonmodified);

MACRO_INSTANTIATE_InsertAtId(SpellDataStruct)
MACRO_INSTANTIATE_InsertAtId(CommandDataStruct)
MACRO_INSTANTIATE_InsertAtId(ItemDataStruct)
MACRO_INSTANTIATE_InsertAtId(ItemUsableDataStruct)
MACRO_INSTANTIATE_InsertAtId(ItemWeaponDataStruct)
MACRO_INSTANTIATE_InsertAtId(ItemArmorDataStruct)
MACRO_INSTANTIATE_InsertAtId(ItemStatDataStruct)
MACRO_INSTANTIATE_InsertAtId(KeyItemDataStruct)
MACRO_INSTANTIATE_InsertAtId(SupportDataStruct)
MACRO_INSTANTIATE_InsertAtId(CharBattleParameterStruct)
MACRO_INSTANTIATE_InsertAtId(InitialStatDataStruct)
MACRO_INSTANTIATE_InsertAtId(InitialEquipDataStruct)
MACRO_INSTANTIATE_InsertAtId(AbilitySetDataStruct)
MACRO_INSTANTIATE_InsertAtId(CommandSetDataStruct)
MACRO_INSTANTIATE_InsertAtId(SynthesisDataStruct)
MACRO_INSTANTIATE_InsertAtId(ShopDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(SpellDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(CommandDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(ItemDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(ItemUsableDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(ItemWeaponDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(ItemArmorDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(ItemStatDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(KeyItemDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(SupportDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(CharBattleParameterStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(InitialStatDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(InitialEquipDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(AbilitySetDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(CommandSetDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(SynthesisDataStruct)
MACRO_INSTANTIATE_PrepareHWSFlexibleList(ShopDataStruct)
MACRO_INSTANTIATE_GetSteamTextSizeGeneric(SpellDataStruct)
MACRO_INSTANTIATE_GetSteamTextSizeGeneric(CommandDataStruct)
MACRO_INSTANTIATE_GetSteamTextSizeGeneric(ItemDataStruct)
MACRO_INSTANTIATE_GetSteamTextSizeGeneric(KeyItemDataStruct)
MACRO_INSTANTIATE_GetSteamTextSizeGeneric(SupportDataStruct)
MACRO_INSTANTIATE_GetSteamTextSizeGeneric(InitialStatDataStruct)
MACRO_INSTANTIATE_WriteSteamTextGeneric(SpellDataStruct)
MACRO_INSTANTIATE_WriteSteamTextGeneric(CommandDataStruct)
MACRO_INSTANTIATE_WriteSteamTextGeneric(ItemDataStruct)
MACRO_INSTANTIATE_WriteSteamTextGeneric(KeyItemDataStruct)
MACRO_INSTANTIATE_WriteSteamTextGeneric(SupportDataStruct)
MACRO_INSTANTIATE_WriteSteamTextGeneric(InitialStatDataStruct)
MACRO_INSTANTIATE_GetModifiedSteamTexts(SpellDataStruct)
MACRO_INSTANTIATE_GetModifiedSteamTexts(CommandDataStruct)
MACRO_INSTANTIATE_GetModifiedSteamTexts(ItemDataStruct)
MACRO_INSTANTIATE_GetModifiedSteamTexts(KeyItemDataStruct)
MACRO_INSTANTIATE_GetModifiedSteamTexts(SupportDataStruct)
MACRO_INSTANTIATE_ConcatenateStrings(int)
MACRO_INSTANTIATE_ConcatenateStrings(float)
MACRO_INSTANTIATE_ConcatenateStrings(wstring)
MACRO_INSTANTIATE_ConcatenateStrings(AnyAbilityStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(SpellDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(StatusSetStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(CommandDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(ItemDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(ItemUsableDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(ItemWeaponDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(ItemArmorDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(ItemStatDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(SupportDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(MagicSwordDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(CharBattleParameterStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(InitialStatDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(InitialEquipDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(AbilitySetDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(CommandSetDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(LevelDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(SynthesisDataStruct)
MACRO_INSTANTIATE_GenerateCSVGeneric(ShopDataStruct)




