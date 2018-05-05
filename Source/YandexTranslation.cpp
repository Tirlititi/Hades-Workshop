#include "YandexTranslation.h"

#include <wx/translation.h>

namespace Yandex {

wxString GetTranslationURL(wxString text, SteamLanguage from, SteamLanguage to) {
	static const wxString countrycode[] = { L"en", L"en", L"ja", L"de", L"fr", L"it", L"es" };
	wxString fromstr = countrycode[from];
	wxString tostr = countrycode[to];
	wxString res = _(L"https://translate.yandex.net/api/v1.5/tr.json/translate?");
	res += _(L"text=")+text;
	res += _(L"&key=trnsl.1.1.20170122T144709Z.0dc75c0c3909ae24.38e8227d63d509bba6874d316ed1f25b72875543");
	res += _(L"&lang=")+fromstr+_(L"-")+tostr;
	return res;
}

wxString ParseYandexAnswer(wxString jsonstr) {

	return _(L"");
}

void SendTranslationRequest(wxString text, SteamLanguage from, SteamLanguage to) {
	unsigned int i;
	wxString plaintext = _(L"");
	for (i=0;i<text.Len();i++) {
		if (text[i]==L'[') {
			while (i++<text.Len())
				if (text[i]==L']')
					break;
		} else {
			plaintext += text[i];
		}
	}
	wxString urlstr = GetTranslationURL(plaintext,from,to);

}

}