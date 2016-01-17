#ifndef _BITMAPFILE_H_
#define _BITMAPFILE_H_


#include <windows.h>


class CBitmapFile
{
protected:
	BITMAPINFO m_bitmapInfo;
	int m_imageWidth;
	int m_imageHeight;
	int m_imageNumComponent;
	unsigned char* m_image;

public:
	CBitmapFile();
	virtual ~CBitmapFile();

	const BITMAPINFO* GetBitmapInfo() const;
	const BITMAPINFOHEADER* GetBitmapInfoHeader() const;

	int GetWidth() const;
	int GetHeight() const;

	const unsigned char* GetImage() const;

	virtual bool LoadBitmapFile(const char* file_name);
};


#endif // _BITMAPFILE_H_
