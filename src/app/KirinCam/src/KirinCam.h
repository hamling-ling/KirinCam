#pragma once
#include "wx/wxprec.h"

class wxGLCanvas;
class KirinGlContext;

class KirinCam :
	public wxApp
{
public:
	KirinCam();
	virtual ~KirinCam();
	KirinGlContext& GetContext(wxGLCanvas *canvas);

	// virtual wxApp methods
	virtual bool OnInit() wxOVERRIDE;
	virtual int OnExit() wxOVERRIDE;
private:
	KirinGlContext *m_glContext;
};

DECLARE_APP(KirinCam)

