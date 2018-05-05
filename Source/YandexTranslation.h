#ifndef _YANDEXTRANSLATION_H
#define _YANDEXTRANSLATION_H

#include <wx/string.h>
#include "File_Manipulation.h"

namespace Yandex {

	void SendTranslationRequest(wxString text, SteamLanguage from, SteamLanguage to);
	wxString GetTranslationURL(wxString text, SteamLanguage from, SteamLanguage to);
	wxString ParseYandexAnswer(wxString jsonstr);

}

#endif
