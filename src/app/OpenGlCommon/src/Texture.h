#ifndef _TEXTURE_H_
#define _TEXTURE_H_


#include "BitmapFile.h"


class CTexture : public CBitmapFile
{
protected:
	BITMAPINFO m_texBitmapInfo;
	int m_texWidth;
	int m_texHeight;
	unsigned char* m_texImage;

public:
	CTexture();
	virtual ~CTexture();

	const BITMAPINFO* GetTexBitmapInfo() const;

	int GetTexWidth() const;
	int GetTexHeight() const;

	const unsigned char* GetTexImage() const;

	virtual bool LoadBitmapFile(const char* file_name);

protected:
	bool CreateTexImage();
	void DeleteTexImage();
};


#endif // _TEXTURE_H_
