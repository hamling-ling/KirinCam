#include "KirinGlCanvas.h"
#include <wx/wxprec.h>
#include <wx/app.h>
#include <wx/glcanvas.h>
#include "KirinGlContext.h"
#include "KirinCam.h"

wxBEGIN_EVENT_TABLE(KirinGlCanvas, wxGLCanvas)
EVT_PAINT(KirinGlCanvas::OnPaint)
EVT_KEY_DOWN(KirinGlCanvas::OnKeyDown)
EVT_TIMER(SpinTimer, KirinGlCanvas::OnSpinTimer)
wxEND_EVENT_TABLE()

static const int attributes[] = {
	WGL_DOUBLE_BUFFER_ARB,
	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	WGL_CONTEXT_MINOR_VERSION_ARB, 3,
	WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	0
};

KirinGlCanvas::KirinGlCanvas(wxWindow *parent) : wxGLCanvas(parent, wxID_ANY, attributes,
	wxDefaultPosition, wxDefaultSize,
	wxFULL_REPAINT_ON_RESIZE),
	m_xangle(30.0),
	m_yangle(30.0),
	m_spinTimer(this, SpinTimer)
{
}


KirinGlCanvas::~KirinGlCanvas()
{
}

void KirinGlCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);

	const wxSize ClientSize = GetClientSize();
	glViewport(0, 0, ClientSize.x, ClientSize.y);

	KirinGlContext& canvas = wxGetApp().GetContext(this);
	canvas.Draw(ClientSize.x, ClientSize.y);
	SwapBuffers();
}

void KirinGlCanvas::OnKeyDown(wxKeyEvent& event)
{
    float angle = 5.0;

    switch ( event.GetKeyCode() )
    {
        case WXK_RIGHT:
            Spin( 0.0, -angle );
            break;

        case WXK_LEFT:
            Spin( 0.0, angle );
            break;

        case WXK_DOWN:
            Spin( -angle, 0.0 );
            break;

        case WXK_UP:
            Spin( angle, 0.0 );
            break;

        case WXK_SPACE:
            if ( m_spinTimer.IsRunning() )
                m_spinTimer.Stop();
            else
                m_spinTimer.Start( 25 );
            break;

        default:
            event.Skip();
            return;
    }
}

void KirinGlCanvas::OnSpinTimer(wxTimerEvent& WXUNUSED(event))
{
    Spin(0.0, 4.0);
}

void KirinGlCanvas::Spin(float xSpin, float ySpin)
{
	m_xangle += xSpin;
	m_yangle += ySpin;

	Refresh(false);
}
