#ifndef _DATABASE_COMMON_H
#define _DATABASE_COMMON_H

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/file.h>
#include <wx/translation.h>
using namespace std;

#define G_N_ELEMENTS(ARRAY) ((sizeof(ARRAY))/(sizeof(ARRAY[0])))
#define G_V_ELEMENTS(VECTOR) ((int)VECTOR.size())

struct SortedChoiceItem {
	unsigned int id;
	wxString label;
};

struct SortedChoiceItemWithHelp {
	unsigned int id;
	wxString label;
	wxString help;
};

#endif
