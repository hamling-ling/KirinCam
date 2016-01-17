#include "stdafx.h"
#include "SimpleObject.h"


SimpleObject::SimpleObject()
{
}


SimpleObject::~SimpleObject(void)
{
	glDeleteTextures(1, &g_textureObject);
	glDeleteVertexArrays(1, &g_vertexArrayObject);
	glDeleteBuffers(1, &g_bufferObject);
}

GLuint SimpleObject::GetVertexArrayObject()
{
	return g_vertexArrayObject;
}

GLuint SimpleObject::GetVertexArrayLen()
{
	return m_verticesLen;
}

GLuint SimpleObject::GetTextureObject()
{
	return g_textureObject;
}

vector<CTriangle3v>& SimpleObject::GetVertexArray()
{
	return m_vertices;
}

void SimpleObject::BindBuffer(GLint vertexLocation, GLint normalLocation, GLint texCoordLocation,
							  const GLfloat *normalsAndVertices, GLuint verticesLen,
							  CTexture& texture)
{
	glGenBuffers(1, &g_bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8 * verticesLen, normalsAndVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &g_vertexArrayObject);
	glBindVertexArray(g_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufferObject);
	glVertexAttribPointer(vertexLocation,	3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (GLvoid *)(0 * sizeof (GLfloat)));
	glVertexAttribPointer(normalLocation,	3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (GLvoid *)(3 * sizeof (GLfloat)));
	glVertexAttribPointer(texCoordLocation,	2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), (GLvoid *)(6 * sizeof (GLfloat)));

	glEnableVertexAttribArray(vertexLocation);
	glEnableVertexAttribArray(normalLocation);
	glEnableVertexAttribArray(texCoordLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &g_textureObject);
	glBindTexture(GL_TEXTURE_2D, g_textureObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.GetTexWidth(), texture.GetTexHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.GetTexImage());
	glBindTexture(GL_TEXTURE_2D, 0);

	m_verticesLen = verticesLen;

	for(GLuint i = 0; i < verticesLen; i=i+3) {
		CVertex a(normalsAndVertices);
		normalsAndVertices += 6;
		CVertex b(normalsAndVertices);
		normalsAndVertices += 6;
		CVertex c(normalsAndVertices);
		CTriangle3v tri(a,b,c);

		m_vertices.push_back(tri);
	}
}
