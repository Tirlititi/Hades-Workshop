#ifndef _DATABASE_COMMON_H
#define _DATABASE_COMMON_H

#include <wx/arrstr.h>

#define G_N_ELEMENTS(ARRAY) ((sizeof(ARRAY))/(sizeof(ARRAY[0])))

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
