#ifndef _GUI_TOOLS_H
#define _GUI_TOOLS_H

class ToolModManager;

#include "Configuration.h"
#include "Gui_Manipulation.h"
#include "gui.h"

class ToolModManager : public ModManagerWindow {
public:
	CDDataStruct* cddata;
	
	ToolModManager(wxWindow* p);
	~ToolModManager();
	int ShowModal(CDDataStruct* data);

private:
	wxMenu* list_popup_menu;
	
	void OnTreeListCheck(wxTreeListEvent& event);
	void OnButtonClick(wxCommandEvent& event);
};

#endif
