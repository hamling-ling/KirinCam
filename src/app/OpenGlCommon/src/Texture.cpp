#include "stdafx.h"
#include "Texture.h"


CTexture::CTexture()
{
	m_texImage = NULL;
	m_texWidth = 0;
	m_texHeight = 0;
}


CTexture::~CTexture()
{
	if (m_texImage) {
		delete [] m_texImage;
	}
	m_texImage = NULL;
	m_texWidth = 0;
	m_texHeight = 0;
}


const BITMAPINFO*
CTexture::GetTexBitmapInfo() const
{
	return &m_texBitmapInfo;
}


const unsigned char*
CTexture::GetTexImage() const
{
	return m_texImage;
}


int
CTexture::GetTexWidth() const
{
	return m_texWidth;
}


int
CTexture::GetTexHeight() const
{
	return m_texHeight;
}


bool
CTexture::LoadBitmapFile(const char* file_name)
{
	if (!CBitmapFile::LoadBitmapFile(file_name)) {
		return false;
	}
	if (!CreateTexImage()) {
		return false;
	}

	return true;
}


bool
CTexture::CreateTexImage()
{
	DeleteTexImage();

	if (!m_image) {
		return false;
	}

	int shift;
	int size;

	//shift = 0;
	//size = sizeof (int) * 8;
	int i;

	//for (i = 0; i < size; i++) {
	//	if ((m_imageWidth >> i) & 0x0001) {
	//		shift = i;
	//	}
	//}
	//m_texWidth = 0x0001 << shift;
	m_texWidth = m_imageWidth;

	//for (i = 0; i < size; i++) {
	//	if ((m_imageHeight >> i) & 0x0001) {
	//		shift = i;
	//	}
	//}
	//m_texHeight = 0x0001 << shift;
	m_texHeight = m_imageHeight;

	m_texImage = new unsigned char [3 * m_texWidth * m_texHeight];
	if (!m_texImage) {
		return false;
	}

	int xStart = (m_imageWidth - m_texWidth) / 2;
	int xEnd = xStart + m_texWidth;
	int yStart = (m_imageHeight - m_texHeight) / 2;
	int yEnd = yStart + m_texHeight;
	int bound = (4 - (3 * m_imageWidth) % 4) % 4;
	int lineByteSize = 3 * m_imageWidth + bound;
	for (int y = yStart; y < yEnd; y++) {
		for (int x = xStart; x < xEnd; x++) {
			m_texImage[3 * (m_texWidth * (y - yStart) + (x - xStart))] = m_image[lineByteSize * y + 3 * x + 2];
			m_texImage[3 * (m_texWidth * (y - yStart) + (x - xStart)) + 1] = m_image[lineByteSize * y + 3 * x + 1];
			m_texImage[3 * (m_texWidth * (y - yStart) + (x - xStart)) + 2] = m_image[lineByteSize * y + 3 * x];
		}
	}

	::memcpy(&m_texBitmapInfo, &m_bitmapInfo, sizeof (BITMAPINFO));
	m_texBitmapInfo.bmiHeader.biWidth = m_texWidth;
	m_texBitmapInfo.bmiHeader.biHeight = m_texHeight;

	return true;
}


void
CTexture::DeleteTexImage()
{
	if (m_texImage) {
		delete [] m_texImage;
		m_texImage = NULL;
	}
	m_texWidth = 0;
	m_texHeight = 0;
	::memset(&m_texBitmapInfo, 0x00, sizeof (BITMAPINFO));
}
