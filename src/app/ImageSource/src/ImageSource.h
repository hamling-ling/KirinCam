#pragma once

#include <string>
#include <memory>

class StreamSource;
class StreamDemuxer;
class StreamPresenter;

class ImageSource
{
public:
	ImageSource();
	~ImageSource();
	uint32_t SetSource(std::string& url);
private:
	std::shared_ptr<StreamSource> _source;
	std::shared_ptr<StreamDemuxer> _demuxer;
	std::shared_ptr<StreamPresenter> _presenter;
};

