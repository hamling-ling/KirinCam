#pragma once

#include <vector>
#include <GL/glew.h>
#include "Texture.h"
#include "Triangle3v.h"

#define MAX_VERTICES	64

using namespace std;

class SimpleObject
{
private:
	struct textureSize {
		GLint width;
		GLint height;
	};

	GLuint m_bufferObject;
	GLuint m_vertexArrayObject;
	GLuint m_textureObject;
	GLuint m_verticesLen;
	vector<CTriangle3v> m_vertices;
	textureSize m_texSize;

public:
	SimpleObject();
	~SimpleObject(void);
	GLuint GetVertexArrayObject();
	GLuint GetVertexArrayLen();
	GLuint GetTextureObject();
	vector<CTriangle3v> &GetVertexArray();

	void BindBuffer(GLint vertexLocation,
					GLint normalLocation,
					GLint texCoordLocation,
					const GLfloat *normalsAndVertices,
					GLuint verticesLen,
					CTexture& texture);
	void ReplaceTexture(CTexture& texture);
};

