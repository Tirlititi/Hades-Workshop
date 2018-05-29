#include "YandexTranslation.h"

#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <Sslprovider.h>
//#include <curl.h>
//#include <openssl.h>
#include <wx/translation.h>

#pragma comment(lib,"Ws2_32.lib")

namespace Yandex {

wxString GetTranslationURL(wxString text, SteamLanguage from, SteamLanguage to) {
	static const wxString countrycode[] = { L"en", L"en", L"ja", L"de", L"fr", L"it", L"es" };
	wxString fromstr = countrycode[from];
	wxString tostr = countrycode[to];
	wxString res = _(L"/api/v1.5/tr.json/translate?");
//	res += _(L"text=")+text;
	res += _(L"lang=")+fromstr+_(L"-")+tostr;
	res += _(L"&key=trnsl.1.1.20170122T144709Z.0dc75c0c3909ae24.38e8227d63d509bba6874d316ed1f25b72875543");
	return _(L"/api/v1.5/tr.json/translate?lang=en-fr&key=trnsl.1.1.20170122T144709Z.0dc75c0c3909ae24.38e8227d63d509bba6874d316ed1f25b72875543");
}

int ParseYandexAnswer(wxString jsonstr, wxString* output) {

//	*output = _(L"");
	return 0;
}

int Translate(wxString text, SteamLanguage from, SteamLanguage to, wxString* output) {
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
/*	plaintext.Replace(_(L"\n"),_(L"\\n"));
	plaintext.Replace(_(L"\r"),_(L"\\r"));
	plaintext.Replace(_(L"\t"),_(L"\\t"));*/
	wxString urlhoststr = _(L"translate.yandex.net");
	wxString urlpathstr = GetTranslationURL(plaintext,from,to);
	wxString httprequest = _(L"POST ") + urlpathstr + _(L" HTTP/1.1\r\n"/*
						*/L"Host: ") + urlhoststr + _(L"\r\n"/*
						*/L"Accept: */*\r\n"/*
						*/L"Content-Length: ") + wxString::Format(wxT("%u"),5+strlen(plaintext.c_str())) + _(L"\r\n"/*
						*/L"Content-Type: application/x-www-form-urlencoded\r\n"/*
						*/L"\r\ntext=") + plaintext;
	addrinfo* addrres = NULL, addrhints;
	SOCKADDR_IN SockAddr;
	WSADATA wsaData;
	SOCKET Socket;
	int error;
//	httprequest = "GET /api/v1.5/tr/translate HTTP/1.1\r\nHost: translate.yandex.net\r\n\r\n"; //api/v1.5/tr.json/translate
//	httprequest = "GET /~claroche/ HTTP/1.1\r\nHost: users.uoa.gr\r\n\r\n";
/*	httprequest =	"GET /api/v1.5/tr.json/translate?text=bonjour&lang=fr-ja HTTP/1.1\r\n"
					"Accept: text/html,application/xhtml+xml,application/xml;q=0.9\r\n" //,star/*;q=0.8
					"Accept-Encoding: gzip, deflate, br\r\n"
					"Accept-Language: fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
					"Cache-Control: max-age=0\r\n"
					"Connection: keep-alive\r\n"
					"Host: translate.yandex.net\r\n"
					"Upgrade-Insecure-Requests: 1\r\n"
					"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0\r\n\r\n";*/
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		return 1;
	addrhints.ai_family = AF_UNSPEC;
	addrhints.ai_protocol = IPPROTO_TCP;
	addrhints.ai_socktype = 0;
	addrhints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
	error = getaddrinfo("translate.yandex.net",NULL,NULL,&addrres);
	if (error!=0 || addrres==NULL) {
		WSACleanup();
		return error;
	}
	SockAddr.sin_port = htons(80); // 443 for https
	SockAddr.sin_family = AF_INET;
//	SockAddr.sin_addr = ((sockaddr_in*)addrres->ai_addr)->sin_addr;
//	SockAddr.sin_addr.S_un.S_addr = 0xC2CCB4D5;
	inet_pton(AF_INET,"213.180.204.194",&SockAddr.sin_addr.s_addr);
	freeaddrinfo(addrres);
	Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (Socket==INVALID_SOCKET) {
		WSACleanup();
		return 3;
	}
	if (connect(Socket,(SOCKADDR*)&SockAddr,sizeof(SockAddr)) != 0) {
		closesocket(Socket);
		WSACleanup();
		return 100+WSAGetLastError();
	}
	int datalen;
	datalen = send(Socket, httprequest.c_str(), strlen(httprequest.c_str()), 0);
	if (datalen<strlen(httprequest.c_str()) || datalen==SOCKET_ERROR) {
		closesocket(Socket);
		WSACleanup();
		return 5;
	}
	error = shutdown(Socket, SD_SEND);
	if (error==SOCKET_ERROR) {
		closesocket(Socket);
		WSACleanup();
		return 6;
	}
	wxString yandexanswer = _(L"");
	char buffer[10000];
	while ((datalen = recv(Socket,buffer,10000,0)) > 0) {
		for (i=0;i<datalen;i++)
			yandexanswer += buffer[i];
	}
	closesocket(Socket);
	WSACleanup();
	error = ParseYandexAnswer(yandexanswer,output);
	if (error!=0)
		return error;

	*output = httprequest+_(L"\nANSWER:\n")+yandexanswer;
	return 0;
}

}