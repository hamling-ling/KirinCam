#pragma once
#include "OpenGlCommon.h"
#include <memory>
#include <opencv2/opencv.hpp>

class CvTexture :
	public CTexture
{
public:
	CvTexture();
	virtual ~CvTexture();

	virtual int GetTexWidth() const;
	virtual int GetTexHeight() const;

	virtual const unsigned char* GetTexImage() const;

	virtual bool LoadBitmapFile(const char* file_name);
private:
	cv::Mat _frame;
};
