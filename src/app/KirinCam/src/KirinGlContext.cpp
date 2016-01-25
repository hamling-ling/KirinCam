#include "KirinGlContext.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include "CvTexture.h"
#include "Model.h"

KirinGlContext::KirinGlContext(wxGLCanvas *canvas)
	: wxGLContext(canvas)
{
	m_screenSize.SetValue(0.0f, 0.0f);
	m_imageSize.SetValue(640, 360);
	pOrigObj = NULL;
	SetCurrent(*canvas);

	CheckGLError();
	glewInit();
	CheckGLError();

	GLuint vertexShader;
	GLuint fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	LoadShaderSource(vertexShader, "Texture.vert");
	glCompileShader(vertexShader);
	DisplayCompileError(vertexShader);
	CheckGLError();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	LoadShaderSource(fragmentShader, "Texture.frag");
	glCompileShader(fragmentShader);
	DisplayCompileError(fragmentShader);
	CheckGLError();

	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);

	glLinkProgram(m_shaderProgram);
	DisplayLinkError(m_shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	CheckGLError();
	glDisableVertexAttribArray(glGetAttribLocation(m_shaderProgram, "Vertex"));
	glDisableVertexAttribArray(glGetAttribLocation(m_shaderProgram, "TexCoord"));

	glUseProgram(m_shaderProgram);
	GLint tex = glGetUniformLocation(m_shaderProgram, "surfaceTexture");
	glUniform1i(tex, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(m_shaderProgram);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(0);

	CheckGLError();
}

KirinGlContext::~KirinGlContext()
{
	delete pOrigObj;
	glDeleteProgram(m_shaderProgram);
}


void KirinGlContext::Draw(float width, float height)
{
	CheckGLError();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lock_guard<recursive_mutex> lock(m_mutex);
	shared_ptr<CameraController> cam = Model::Instance().GetCameraController();

	if (cam) {
		cam->GetImage(m_camFrame.sequenceNumber, m_camFrame);
		if (!m_camFrame.image.empty()) {

			CvTexture texture;
			texture.SetFrame(m_camFrame.image);

			glUseProgram(m_shaderProgram);

			if (!pOrigObj) {
				GLint vertexLocation = glGetAttribLocation(m_shaderProgram, "Vertex");
				GLint texCoordLocation = glGetAttribLocation(m_shaderProgram, "TexCoord");
				pOrigObj = new SimpleObject();
				pOrigObj->BindBuffer(vertexLocation, -1, texCoordLocation,
					&(normalsAndVertices[0][0]), 6,
					texture);
				CheckGLError();
				GLint tex = glGetUniformLocation(m_shaderProgram, "surfaceTexture");
				glUniform1i(tex, 0);
			}
			else {
				pOrigObj->ReplaceTexture(texture);
			}
			CheckGLError();

			UpdateScalling(width, height);

			glBindTexture(GL_TEXTURE_2D, pOrigObj->GetTextureObject());
			glBindVertexArray(pOrigObj->GetVertexArrayObject());
			glDrawArrays(GL_TRIANGLES, 0, pOrigObj->GetVertexArrayLen());
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			CheckGLError();

			glFlush();
			glUseProgram(0);
		}
	}

	CheckGLError();
}

void KirinGlContext::UpdateScalling(float width, float height)
{
	CVector2f latestScreenSize(width, height);
	if (m_screenSize != latestScreenSize) {
		float imgYovX = m_imageSize[CVector2f::Y] / m_imageSize[CVector2f::X];
		float scaleX = 1.0;
		float scaleY = 1.0;
		// scalling to fit image width to screen width
		float imageX = latestScreenSize[CVector2f::X];
		float imageY = m_imageSize[CVector2f::Y] * imageX / m_imageSize[CVector2f::X];
		if (latestScreenSize[CVector2f::Y] < imageY) {
			// if Y exceed screen height, use scalling to fit height
			imageY = latestScreenSize[CVector2f::Y];
			imageX = m_imageSize[CVector2f::X] * imageY / m_imageSize[CVector2f::Y];
			// in shader image is 1x1. height fixed so compute ? x 1
			scaleX = imageX / latestScreenSize[CVector2f::X];
		}
		else {
			scaleY = imageY / latestScreenSize[CVector2f::Y];
		}

		m_screenSize = latestScreenSize;

		GLint scallingSlot = glGetUniformLocation(m_shaderProgram, "Scalling");
		glUniform2f(scallingSlot, scaleX, scaleY);
		CheckGLError();
	}
}

void KirinGlContext::CheckGLError()
{
	GLenum errLast = GL_NO_ERROR;

	for (;; )
	{
		GLenum err = glGetError();
		if (err == GL_NO_ERROR)
			return;

		// normally the error is reset by the call to glGetError() but if
		// glGetError() itself returns an error, we risk looping forever here
		// so check that we get a different error than the last time
		if (err == errLast)
		{
			wxLogError(wxT("OpenGL error state couldn't be reset."));
			return;
		}

		errLast = err;

		wxLogError(wxT("OpenGL error %d"), err);
	}
}

void KirinGlContext::LoadShaderSource(GLuint shader, const char* fileName)
{
	FILE* fp = NULL;
	int size;
	char* buf;

	errno_t err = fopen_s(&fp, fileName, "rb");
	if (err != 0) {
		wxLogError(wxT("Shader %s not found."), fileName);
		return;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	buf = (char *)malloc(size);
	fseek(fp, 0, SEEK_SET);

	fread(buf, 1, size, fp);

	glShaderSource(shader, 1, (const GLchar **)&buf, &size);

	free(buf);
	fclose(fp);

}

void KirinGlContext::DisplayCompileError(GLuint shader)
{
	GLint compiled;
	int size;
	int len;
	char* buf;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (char *)malloc(size);
			glGetShaderInfoLog(shader, size, &len, buf);
			wxLogError(wxT("Shader Compile error %s"), buf);
			free(buf);
		}
	}
}

void KirinGlContext::DisplayLinkError(GLuint program)
{
	GLint linked;
	int size;
	int len;
	char* buf;

	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
		if (size > 0) {
			buf = (char *)malloc(size);
			glGetProgramInfoLog(program, size, &len, buf);
			wxLogError(wxT("Shader Link error %s"), buf);
			free(buf);
		}
	}
}
