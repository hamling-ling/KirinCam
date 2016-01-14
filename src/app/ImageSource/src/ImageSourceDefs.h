#pragma once

#include <cstdint>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

struct FramePoint {
	uint16_t x;
	uint16_t y;
};

struct CamFrameRect {
	FramePoint bottomRight;
	FramePoint topLeft;
};

struct CameraFrameInformation {
	CamFrameRect rect;
	uint8_t category;
	uint8_t status;
	uint8_t status2;
};

struct CameraFrame {
	uint16_t sequenceNumber;
	uint32_t timeStamp;
	cv::Mat image;
	std::vector<CameraFrameInformation> info;
public:
	CameraFrame() {
		sequenceNumber = 0;
		timeStamp = 0;
	}
};
