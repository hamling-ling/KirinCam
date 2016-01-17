#include "KirinCam.h"
#include <GL/glew.h>
#include <wx/glcanvas.h>
#include "KirinGlContext.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(KirinCam);

KirinCam::KirinCam()
{
	m_glContext = NULL;
}


KirinCam::~KirinCam()
{
}

bool KirinCam::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	new MainFrame();

	return true;
}

int KirinCam::OnExit()
{
	delete m_glContext;

	return wxApp::OnExit();
}

KirinGlContext& KirinCam::GetContext(wxGLCanvas *canvas)
{
	KirinGlContext *glContext;
	if (!m_glContext)
	{
		// Create the OpenGL context for the first mono window which needs it:
		// subsequently created windows will all share the same context.
		m_glContext = new KirinGlContext(canvas);
	}
	glContext = m_glContext;

	glContext->SetCurrent(*canvas);

	return *glContext;
}

