#include "CvTexture.h"

#include <opencv2/core/core.hpp>
#include "opencvlib.h"

CvTexture::CvTexture()
{
}


CvTexture::~CvTexture()
{
}


int CvTexture::GetTexWidth() const
{
	return _frame.cols;
}

int CvTexture::GetTexHeight() const
{
	return _frame.rows;

}

const unsigned char* CvTexture::GetTexImage() const
{
	return _frame.ptr();
}

bool CvTexture::LoadBitmapFile(const char* file_name)
{
	_frame = cv::imread(file_name);
	if (_frame.empty()) {
		return false;
	}

	cv::flip(_frame, _frame, 0);
	return true;
}

bool CvTexture::SetFrame(cv::Mat frame)
{
	_frame = frame;
	return true;
}
