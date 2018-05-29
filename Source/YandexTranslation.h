#ifndef _YANDEXTRANSLATION_H
#define _YANDEXTRANSLATION_H

#include <wx/string.h>
#include "File_Manipulation.h"

namespace Yandex {

	int Translate(wxString text, SteamLanguage from, SteamLanguage to, wxString* output);
	int ParseYandexAnswer(wxString jsonstr, wxString* output);
	wxString GetTranslationURL(wxString text, SteamLanguage from, SteamLanguage to);

}

#endif
