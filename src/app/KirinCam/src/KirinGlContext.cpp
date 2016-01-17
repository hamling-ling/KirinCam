#include "KirinGlContext.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include "CvTexture.h"

KirinGlContext::KirinGlContext(wxGLCanvas *canvas)
	: wxGLContext(canvas)
{
	SetCurrent(*canvas);

	CheckGLError();
	glewInit();
	CheckGLError();
#if 0
#else
	g_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	LoadShaderSource(g_vertexShader, "Texture.vert");
	glCompileShader(g_vertexShader);
	CheckGLError();

	g_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	LoadShaderSource(g_fragmentShader, "Texture.frag");
	glCompileShader(g_fragmentShader);
	CheckGLError();

	g_shaderProgram = glCreateProgram();
	glAttachShader(g_shaderProgram, g_vertexShader);
	glAttachShader(g_shaderProgram, g_fragmentShader);

	glLinkProgram(g_shaderProgram);

	GLint vertexLocation = glGetAttribLocation(g_shaderProgram, "Vertex");
	GLint normalLocation = glGetAttribLocation(g_shaderProgram, "Normal");
	GLint texCoordLocation = glGetAttribLocation(g_shaderProgram, "TexCoord");

	glDeleteShader(g_vertexShader);
	glDeleteShader(g_fragmentShader);

	CvTexture texture;
	if (!texture.LoadBitmapFile("texture.jpg"))
	{
		wxLogError(wxT("Texture %s not found"), wxT("texture.jpg"));
	}
	pOrigObj = new SimpleObject();
	CheckGLError();
	pOrigObj->BindBuffer(vertexLocation, normalLocation, texCoordLocation,
		&(normalsAndVertices[0][0]), 3,
		texture);
	CheckGLError();
	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Vertex"));
	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "Normal"));
	glDisableVertexAttribArray(glGetAttribLocation(g_shaderProgram, "TexCoord"));

	glUseProgram(g_shaderProgram);
	GLint tex = glGetUniformLocation(g_shaderProgram, "surfaceTexture");
	glUniform1i(tex, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	const GLfloat lightPosition[4] = { 3.0f, 4.0f, 0.0f, 0.0f };
	const GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat lightAmbient[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	const GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	const GLfloat cubeDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat cubeAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	const GLfloat cubeSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat cubeShininess[1] = { 32.0f };

	glUseProgram(g_shaderProgram);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "lightPosition"), 1, lightPosition);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ld"), 1, lightDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "La"), 1, lightAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ls"), 1, lightSpecular);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Kd"), 1, cubeDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ka"), 1, cubeAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ks"), 1, cubeSpecular);
	glUniform1fv(glGetUniformLocation(g_shaderProgram, "shininess"), 1, cubeShininess);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, pOrigObj->GetTextureObject());
	glBindVertexArray(pOrigObj->GetVertexArrayObject());
	glDrawArrays(GL_TRIANGLES, 0, pOrigObj->GetVertexArrayLen());
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFlush();

	glUseProgram(0);
#endif
	CheckGLError();
}

KirinGlContext::~KirinGlContext()
{
	delete pOrigObj;
	glDeleteProgram(g_shaderProgram);
}


void KirinGlContext::Draw(float nWidth, float nHeight)
{
	CheckGLError();
#if 0
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

#else
	CvTexture tex;
	if (nWidth > 512) {
		tex.LoadBitmapFile("texture2.jpg");
		pOrigObj->ReplaceTexture(tex);
	}

	CMatrix4x4f perspective;;

	float l, r, b, t, n = 0.1f, f = 10.0f;
	t = n * tanf(PI * (45.0f / 2.0f) / 180.0f);
	b = -t;
	r = t * (float)nWidth / (float)nHeight;
	l = -r;
	perspective.MakePerspective(l, r, b, t, n, f);
	float projection[16];
	perspective.GetGLMat(projection);

	CMatrix4x4f rotationX;
	CMatrix4x4f rotationY;
	CMatrix4x4f translation;
	CMatrix4x4f lookAt;

	float distance = sqrtf(3.0f * 2.5f * 2.5f);
	float angle = atan2f(1.0f, sqrtf(2.0f)) * 180.0f / PI;

	translation.MakeTranslation(CVector3f(0.0f, 0.0f, -distance));
	rotationX.MakeRotation(CVector3f(1.0f, 0.0f, 0.0f), angle);
	rotationY.MakeRotation(CVector3f(0.0f, 1.0f, 0.0f), 0.0f);
	lookAt = translation * rotationX * rotationY;
	float modelViewMatrix[16];
	float viewMatrix[16];
	lookAt.GetGLMat(modelViewMatrix);
	lookAt.GetGLMat(viewMatrix);

	CMatrix4x4f lookAtPerspective;
	lookAtPerspective = perspective * lookAt;
	float modelViewProjectionMatrix[16];
	lookAtPerspective.GetGLMat(modelViewProjectionMatrix);

	CheckGLError();
	glUseProgram(g_shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "modelViewProjectionMatrix"), 1, GL_FALSE, modelViewProjectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "modelViewMatrix"), 1, GL_FALSE, modelViewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(g_shaderProgram, "viewMatrix"), 1, GL_FALSE, viewMatrix);

	const GLfloat lightPosition[4] = { 3.0f, 4.0f, 0.0f, 0.0f };
	const GLfloat lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat lightAmbient[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	const GLfloat lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	const GLfloat cubeDiffuse[4] = { 0.75f, 0.0f, 1.0f, 1.0f };
	const GLfloat cubeAmbient[4] = { 0.3f, 0.25f, 0.4f, 1.0f };
	const GLfloat cubeSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat cubeShininess[1] = { 32.0f };
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "lightPosition"), 1, lightPosition);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ld"), 1, lightDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "La"), 1, lightAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ls"), 1, lightSpecular);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Kd"), 1, cubeDiffuse);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ka"), 1, cubeAmbient);
	glUniform4fv(glGetUniformLocation(g_shaderProgram, "Ks"), 1, cubeSpecular);
	glUniform1fv(glGetUniformLocation(g_shaderProgram, "shininess"), 1, cubeShininess);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, pOrigObj->GetTextureObject());
	glBindVertexArray(pOrigObj->GetVertexArrayObject());
	glDrawArrays(GL_TRIANGLES, 0, pOrigObj->GetVertexArrayLen());
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFlush();
	glUseProgram(0);
#endif
	CheckGLError();
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

