#include <wx/wx.h>
#include <wx/splash.h>

// application class
class wxMiniApp : public wxApp
{
public:
	// function called at the application initialization
	virtual bool OnInit();
};

IMPLEMENT_APP(wxMiniApp);

bool wxMiniApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	wxBitmap bitmap = wxBITMAP_PNG(hadesworkshop_splash);
	wxSplashScreen* splash = new wxSplashScreen(bitmap,
		wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
		6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
		wxBORDER_SIMPLE|wxSTAY_ON_TOP);
	splash->Show(true);
	splash->SetIcon(wxICON(hadesworkshop_icon));
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	CreateProcess(TEXT("HadesWorkshop.exe"),
			NULL,NULL,NULL,FALSE,
			CREATE_NEW_CONSOLE,
			NULL,NULL,&si,&pi);
	return true;
}
