#include "stdafx.h"
#include "SimpleObject.h"

SimpleObject::SimpleObject()
{
	m_texSize.width = 0;
	m_texSize.height = 0;
}


SimpleObject::~SimpleObject(void)
{
	glDeleteTextures(1, &m_textureObject);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
	glDeleteBuffers(1, &m_bufferObject);
}

GLuint SimpleObject::GetVertexArrayObject()
{
	return m_vertexArrayObject;
}

GLuint SimpleObject::GetVertexArrayLen()
{
	return m_verticesLen;
}

GLuint SimpleObject::GetTextureObject()
{
	return m_textureObject;
}

vector<CTriangle3v>& SimpleObject::GetVertexArray()
{
	return m_vertices;
}

void SimpleObject::BindBuffer(GLint vertexLocation, GLint normalLocation, GLint texCoordLocation,
							  const GLfloat *normalsAndVertices, GLuint verticesLen,
							  CTexture& texture)
{
	glGenBuffers(1, &m_bufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof (GLfloat) * 8 * verticesLen, normalsAndVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferObject);
	if (vertexLocation != -1) {
		glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0 * sizeof(GLfloat)));
		glEnableVertexAttribArray(vertexLocation);
	}
	if (normalLocation != -1) {
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(normalLocation);
	}
	if (texCoordLocation != -1) {
		glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(texCoordLocation);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &m_textureObject);
	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.GetTexWidth(), texture.GetTexHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, texture.GetTexImage());
	m_texSize.width = texture.GetTexWidth();
	m_texSize.height = texture.GetTexHeight();
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

void SimpleObject::ReplaceTexture(CTexture& texture)
{
	glBindTexture(GL_TEXTURE_2D, m_textureObject);

	if (texture.GetTexWidth() != m_texSize.width ||
		texture.GetTexHeight() != m_texSize.height) {
		// delete old one and create new texture
		glDeleteTextures(1, &m_textureObject);
		glGenTextures(1, &m_textureObject);
		glBindTexture(GL_TEXTURE_2D, m_textureObject);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.GetTexWidth(), texture.GetTexHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, texture.GetTexImage());
		m_texSize.width = texture.GetTexWidth();
		m_texSize.height = texture.GetTexHeight();
	}
	else {
		// replace with new image
		glTexSubImage2D(
			GL_TEXTURE_2D,				// target
			0,							// level
			0,							// xoffset
			0,							// yoffset
			texture.GetTexWidth(),		// width
			texture.GetTexHeight(),		// height
			GL_BGR,
			GL_UNSIGNED_BYTE,
			texture.GetTexImage()
			);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
