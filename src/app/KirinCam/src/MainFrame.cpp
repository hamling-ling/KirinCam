#include "MainFrame.h"
#include "KirinGlCanvas.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_NEW, MainFrame::OnNewWindow)
EVT_MENU(wxID_CLOSE, MainFrame::OnClose)
wxEND_EVENT_TABLE()

MainFrame::MainFrame()
	: wxFrame(NULL, wxID_ANY, wxT("wxWidgets OpenGL Sample"))
{
	new KirinGlCanvas(this);

	SetIcon(wxICON(sample));

	// Make a menubar
	wxMenu *menu = new wxMenu;
	menu->Append(wxID_NEW);
	menu->AppendSeparator();
	menu->Append(wxID_CLOSE);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menu, wxT("&Cube"));

	SetMenuBar(menuBar);

	CreateStatusBar();

	SetClientSize(400, 400);
	Show();

	// test IsDisplaySupported() function:
	static const int attribs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };
	wxLogStatus("Double-buffered display %s supported",
		wxGLCanvas::IsDisplaySupported(attribs) ? "is" : "not");
}


MainFrame::~MainFrame()
{
	Close(true);
}

void MainFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MainFrame::OnNewWindow(wxCommandEvent& WXUNUSED(event))
{
	new MainFrame();
}

