#pragma once
#include <GL/glew.h>
#include "wx/wxprec.h"
#include <wx/glcanvas.h>

// control ids
enum
{
	SpinTimer = wxID_HIGHEST + 1
};

class KirinGlCanvas :
	public wxGLCanvas
{
public:
	KirinGlCanvas(wxWindow *parent);
	virtual ~KirinGlCanvas();
private:
	void OnPaint(wxPaintEvent& event);
	void Spin(float xSpin, float ySpin);
	void OnKeyDown(wxKeyEvent& event);
	void OnSpinTimer(wxTimerEvent& WXUNUSED(event));

	// angles of rotation around x- and y- axis
	float m_xangle,
		m_yangle;

	wxTimer m_spinTimer;

	wxDECLARE_EVENT_TABLE();
};

