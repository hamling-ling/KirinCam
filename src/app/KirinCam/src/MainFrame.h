#pragma once
#include "wx/wxprec.h"

class MainFrame :
	public wxFrame
{
public:
	MainFrame();
	virtual ~MainFrame();
private:
	void OnClose(wxCommandEvent& event);
	void OnNewWindow(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

