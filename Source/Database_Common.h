#ifndef _DATABASE_COMMON_H
#define _DATABASE_COMMON_H

#include <wx/arrstr.h>

#ifndef G_N_ELEMENTS
#define G_N_ELEMENTS(arr) ((sizeof(arr))/(sizeof(arr[0])))
#endif

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
