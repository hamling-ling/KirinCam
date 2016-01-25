#pragma once
#include "wx/wxprec.h"
#include <vector>
#include <mutex>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "wx/glcanvas.h"
#include "OpenGlCommon.h"
#include "CameraRemoteController.h"

class KirinGlContext :
	public wxGLContext
{
public:
	KirinGlContext(wxGLCanvas *canvas);
	virtual ~KirinGlContext();
	void Draw(float width, float height);
	void UpdateScalling(float width, float height);
	static void CheckGLError();
	static void LoadShaderSource(GLuint shader, const char* fileName);
	static void DisplayCompileError(GLuint shader);
	static void DisplayLinkError(GLuint program);

private:
	GLuint m_textures[6];
	GLuint m_shaderProgram;

	const GLfloat normalsAndVertices[6][8] =
	{
		{ -1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f },
		{ 1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f },
		{ 1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f },
		{ -1.0f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f },
	};

	SimpleObject* pOrigObj;
	CVector2f m_screenSize;
	CVector2f m_imageSize;
	CameraFrame m_camFrame;
	std::recursive_mutex m_mutex;
};
