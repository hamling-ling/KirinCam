#pragma once
#include "wx/wxprec.h"
#include <vector>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "wx/glcanvas.h"
#include "OpenGlCommon.h"

class KirinGlContext :
	public wxGLContext
{
public:
	KirinGlContext(wxGLCanvas *canvas);
	virtual ~KirinGlContext();
	void Draw(float xangle, float yangle);
	static void CheckGLError();
	static void LoadShaderSource(GLuint shader, const char* fileName);

private:
	GLuint m_textures[6];

	GLuint g_vertexShader;
	GLuint g_fragmentShader;
	GLuint g_shaderProgram;

	const GLfloat normalsAndVertices[3][8] =
	{
		{ -0.5f, -0.5f, 0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f },
		{ 0.5f, -0.5f, 0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f },
		{ 0.0f,  1.0f, 0.5f, 0.0f,  0.0f,  1.0f, 0.5f, 1.0f },
	};

	SimpleObject* pOrigObj;
	std::vector<SimpleObject*> objects;
};
