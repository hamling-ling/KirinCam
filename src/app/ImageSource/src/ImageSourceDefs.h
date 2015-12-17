#pragma once

#include <cstdint>
#include <vector>

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
	std::vector<uint8_t> image;
	std::vector<CameraFrameInformation> info;
public:
	CameraFrame() {
		sequenceNumber = 0;
		timeStamp = 0;
	}
};
