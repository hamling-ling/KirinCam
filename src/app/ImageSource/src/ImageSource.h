#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <vector>

class StreamSource;
class StreamDemuxer;
class StreamPresenter;


class ImageSource
{
public:
	ImageSource(const std::string& url);
	~ImageSource();
	uint32_t Start();
	void GetImage(std::vector<uint8_t>& buf);
private:
	std::recursive_mutex _mutex;
	std::shared_ptr<StreamSource> _source;
	std::shared_ptr<StreamDemuxer> _demuxer;
	std::shared_ptr<StreamPresenter> _presenter;
};

