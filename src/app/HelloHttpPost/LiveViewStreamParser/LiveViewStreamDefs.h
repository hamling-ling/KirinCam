#pragma once

#include <cstdint>

enum PayloadType : uint8_t
{
	kPayloadTypeLiveViewImages = 0x01,
	kPayloadTypeLiveViewFrameInformation = 0x02,
};

enum FrameCategory : uint8_t {
	kFrameCategoryInvalid = 0x00,
	kFrameCategoryContrastAF = 0x01,
	kFrameCategoryPhaseDetectioniAF = 0x02,
	kFrameCategoryReserved = 0x03,
	kFrameCategoryFace = 0x04,
	kFrameCategoryTracking = 0x05,
};

enum FrameStatus : uint8_t {
	kFrameStatusInvalid = 0x00,
	kFrameStatusNormal = 0x01,
	kFrameStatusMain = 0x02,
	kFrameStatusSub = 0x03,
	kFrameStatusFocused = 0x04,
	kFrameStatusReserved1 = 0x05,
	kFrameStatusReserved2 = 0x06,
	kFrameStatusReserved3 = 0x07,
};

enum FrameAdditionalStatus {
	kFrameAdditionalStatusInvalid = 0x00,
	kFrameAdditionalStatusNormal = 0x01,
	kFrameAdditionalStatusLargeFrame = 0x02,
};

static const uint8_t kCommonHaderStartByte = 0xFF;
static const uint8_t kPayloadHeaderStartCode[4] = { 0x24, 0x35, 0x68, 0x79 };
static const uint8_t kFrameDataVersion[2] = { 0x01, 0x00 };
