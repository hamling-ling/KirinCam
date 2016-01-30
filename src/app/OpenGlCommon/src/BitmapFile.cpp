#include "stdafx.h"
#include <cstdio>

#include "BitmapFile.h"


CBitmapFile::CBitmapFile()
{
	::memset(&m_bitmapInfo, 0x00, sizeof (BITMAPINFO));
	m_imageNumComponent = 0;
	m_imageWidth = 0;
	m_imageHeight = 0;
	m_image = NULL;
}


CBitmapFile::~CBitmapFile()
{
	if (m_image) {
		delete [] m_image;
		m_image = NULL;
	}
}


const BITMAPINFO*
CBitmapFile::GetBitmapInfo() const
{
	return &m_bitmapInfo;
}


const BITMAPINFOHEADER*
CBitmapFile::GetBitmapInfoHeader() const
{
	return &m_bitmapInfo.bmiHeader;
}


int
CBitmapFile::GetWidth() const
{
	return m_imageWidth;
}


int
CBitmapFile::GetHeight() const
{
	return m_imageHeight;
}


const unsigned char*
CBitmapFile::GetImage() const
{
	return m_image;
}


bool
CBitmapFile::LoadBitmapFile(const char* file_name)
{
	BITMAPFILEHEADER bitmapFileHeader;
	FILE* fp;
	size_t numRead;

	::fopen_s(&fp, file_name, "rb");
	if (!fp) {
		return false;
	}

	::memset(&bitmapFileHeader, 0x00, sizeof (BITMAPFILEHEADER));
	numRead = ::fread(&bitmapFileHeader, sizeof (BITMAPFILEHEADER), 1, fp);
	if (numRead != 1) {
		::fclose(fp);
		return false;
	}

	if (bitmapFileHeader.bfType != *(unsigned short*)"BM") {
		::fclose(fp);
		return false;
	}

	numRead = ::fread(&m_bitmapInfo.bmiHeader, sizeof (BITMAPINFOHEADER), 1, fp);
	if (numRead != 1) {
		::fclose(fp);
		return false;
	}

	m_imageNumComponent = m_bitmapInfo.bmiHeader.biBitCount >> 3;
	m_imageWidth = m_bitmapInfo.bmiHeader.biWidth;
	m_imageHeight = m_bitmapInfo.bmiHeader.biHeight;

	int bound;
	int imageSize;
		// (3 * m_imageWidth) % 4 ‚ª 0 ‚Ì‚Ì‚½‚ß‚É‚à‚¤ˆê“x 4 ‚Ì—]è‚ğ‚Æ‚é
	bound = ((4 - (3 * m_imageWidth) % 4)) % 4;
	imageSize = (3 * m_imageWidth + bound) * m_imageHeight;

	if (m_image) {
		delete [] m_image;
		m_image = NULL;
	}
	m_image = new unsigned char [imageSize];
	if (!m_image) {
		m_imageNumComponent = 0;
		m_imageWidth = 0;
		m_imageHeight = 0;
		::fclose(fp);
		return false;
	}

	// 24Bits
	if (m_imageNumComponent == 3) {
		::fseek(fp, bitmapFileHeader.bfOffBits, 0);
		numRead = ::fread(m_image, sizeof (unsigned char), imageSize, fp);
		if (numRead != imageSize) {
			delete [] m_image;
			m_image = NULL;
			m_imageNumComponent = 0;
			m_imageWidth = 0;
			m_imageHeight = 0;
			::fclose(fp);
			return false;
		}
	}
	// Other
	else {
		return false;
	}

	::fclose(fp);

	return true;
}
