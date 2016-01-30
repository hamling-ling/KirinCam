#pragma once

#include <string>
#include <memory>
#include <mutex>
#include "ImageSourceDefs.h"

class StreamSource;
class StreamDemuxer;
class StreamDecoder;
class StreamImagePresenter;
class StreamFrameInfoPresenter;

class ImageSource
{
public:
	ImageSource(const std::string& url, bool flipImage);
	~ImageSource();
	uint32_t Start();
	bool GetImage(uint16_t seqNum, CameraFrame& frame);
private:
	std::recursive_mutex _mutex;
	std::shared_ptr<StreamSource> _source;
	std::shared_ptr<StreamDemuxer> _demuxer;
	std::shared_ptr<StreamDecoder> _decoder;
	std::shared_ptr<StreamImagePresenter> _imagePresenter;
	std::shared_ptr<StreamFrameInfoPresenter> _frameInfoPresenter;
};

