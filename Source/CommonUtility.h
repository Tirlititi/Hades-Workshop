#pragma once

// Utility API with minimal GUI

#include <inttypes.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <functional>
#include <wx/string.h>
#include <wx/arrstr.h>
#include "File_Manipulation.h"
using namespace std;

// Insert an object with a "id" field inside an already id-ordered vector
// Return the index of the newly inserted object or -1 if an object with the same id already exists
template<typename T> int InsertAtId(vector<T>& base, T& newobj, int newobjid);

// For HWS files containing a non-standard number of objects
// Prepare the object list to fit the objects stored in the HWS file
template<typename T> int PrepareHWSFlexibleList(fstream& ffbin, vector<T>& objlist, vector<T>& nonmodifiedlist, vector<int>& insertedindices);

// API for writing .mes assets or .hws parts, either in archives or in individual files
template<typename T> int GetSteamTextSizeGeneric(vector<T>& list, FF9String T::*field, bool asmes, SteamLanguage lang);
template<typename T> void WriteSteamTextGeneric(fstream& f, vector<T>& list, FF9String T::*field, vector<int>* subindex, bool asmes, SteamLanguage lang);

// Concatenate multiple strings using a delimiter between each, possibly with strings generated out of a list of non-strings
template<typename T> string ConcatenateStrings(string delim, vector<T>& objlist, function<string(T)> stringifier, bool escapeempty = false);
string ConcatenateStrings(string delim, vector<string>& objlist, bool escapeempty = false);
bool IsNumber(string s);

namespace MemoriaUtility {
	wxArrayString LoadCSVLines(wxString filename, wxArrayString* metadata = NULL);
	wxArrayString LoadCSVEntry(wxString csvline);
	wxArrayString SplitEntryArray(wxString csventry);
	int GetEntryEnum(wxString csventry);
	bool CSV_ComparerWithoutStart(wxString left, wxString right);
	bool CSV_ComparerWithoutEnd(wxString left, wxString right);
	bool CSV_ComparerWithoutBoth(wxString left, wxString right);
	template<typename T> bool GenerateCSVGeneric(wxString modfolder, wxString csvpath, wxString csvheader, vector<T>& objlist, function<wxString(T&, int)> entryconstructor, function<bool(wxString, wxString)> entrycomparer, bool skipnonmodified);

	template<typename T> bool GetModifiedSteamTexts(vector<int>* result, int32_t baseassetid[], vector<T>& objlist, function<wstring(T&)> stringifier, SteamLanguage lang);
}
