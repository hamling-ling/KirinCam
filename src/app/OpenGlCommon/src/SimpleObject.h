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
	GLuint g_bufferObject;
	GLuint g_vertexArrayObject;
	GLuint g_textureObject;
	GLuint m_verticesLen;
	vector<CTriangle3v> m_vertices;

public:
	SimpleObject();
	~SimpleObject(void);
	GLuint GetVertexArrayObject();
	GLuint GetVertexArrayLen();
	GLuint GetTextureObject();
	vector<CTriangle3v> &GetVertexArray();

	void SimpleObject::BindBuffer(GLint vertexLocation, GLint normalLocation, GLint texCoordLocation,
							  const GLfloat *normalsAndVertices, GLuint verticesLen,
							  CTexture& texture);
};

