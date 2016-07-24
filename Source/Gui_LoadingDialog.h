#ifndef _GUI_LOADINGDIALOG_H
#define _GUI_LOADINGDIALOG_H

#include <wx/progdlg.h>

class LoadingDialog : public wxGenericProgressDialog {
public:
	unsigned int object_amount;
	bool is_active;
	
	LoadingDialog(wxWindow* parent, const wxString& title = _(L"Progression...")); 
	~LoadingDialog();
};

void LoadingDialogCreate(wxWindow* parent);
void LoadingDialogInit(unsigned int objamount, const wxString& title);
void LoadingDialogUpdate(unsigned int objdone, wxString message = wxEmptyString);
void LoadingDialogEnd();

#endif
