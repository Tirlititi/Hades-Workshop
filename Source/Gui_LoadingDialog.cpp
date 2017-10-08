#include "Gui_LoadingDialog.h"

LoadingDialog::LoadingDialog(wxWindow* parent, const wxString& title) :
	wxGenericProgressDialog(title,wxEmptyString,100,parent,wxPD_AUTO_HIDE),
	object_amount(1),
	is_active(false) {
	Show(false);
}

LoadingDialog::~LoadingDialog() {
}

static LoadingDialog* TheLoadingDialog = NULL;
wxWindow* TheLoadingDialogParent = NULL;

// Doesn't really create the dialog (it would be briefly shown otherwise)...
void LoadingDialogCreate(wxWindow* parent) {
	TheLoadingDialogParent = parent;
}

void LoadingDialogInit(unsigned int objamount, const wxString& title) {
	if (TheLoadingDialog==NULL)
		TheLoadingDialog = new LoadingDialog(TheLoadingDialogParent);
	if (TheLoadingDialog->is_active)
		return;
	TheLoadingDialog->wxTopLevelWindow::SetTitle(title);
	TheLoadingDialog->object_amount = objamount;
	TheLoadingDialog->is_active = true;
	TheLoadingDialog->wxDialog::Show(true);
	TheLoadingDialog->wxGenericProgressDialog::Update(0,wxString::Format(wxT("0 / %u"),objamount));
	return;
}

void LoadingDialogUpdate(unsigned int objdone, wxString message) {
	if (!TheLoadingDialog->is_active || TheLoadingDialog->object_amount==0)
		return;
	int newvalue = 100*objdone/TheLoadingDialog->object_amount;
	int oldvalue = TheLoadingDialog->wxGenericProgressDialog::GetValue();
	if (message==wxEmptyString)
		message = wxString::Format(wxT("%u / %u"),objdone,TheLoadingDialog->object_amount);
	if (newvalue>=oldvalue+10)
		TheLoadingDialog->wxGenericProgressDialog::Update(newvalue<=99 ? newvalue : 99,message);
	else
		TheLoadingDialog->wxGenericProgressDialog::Update(oldvalue,message);
}

void LoadingDialogEnd() {
	if (!TheLoadingDialog->is_active)
		return;
	TheLoadingDialog->wxGenericProgressDialog::Update(100,_(L"Done"));
	TheLoadingDialog->is_active = false;
	TheLoadingDialog->wxDialog::Show(false);
}
