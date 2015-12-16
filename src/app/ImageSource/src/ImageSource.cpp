#include "stdafx.h"
#include "ImageSource.h"
#include "StreamSource.h"
#include "StreamDemuxer.h"
#include "StreamPresenter.h"

using namespace std;

ImageSource::ImageSource(const std::string& url)
{
	_source = std::make_shared<StreamSource>(url);
	_demuxer = std::make_shared<StreamDemuxer>();
	_presenter = std::make_shared<StreamPresenter>();

	_source->Connect(_demuxer);
	_demuxer->Connect(_presenter);
}

ImageSource::~ImageSource()
{
	lock_guard<recursive_mutex> lock(_mutex);

	if (_source) {
		_source->Stop();
	}
	if (_demuxer) {
		_demuxer->Stop();
	}
	if (_presenter) {
		_presenter->Stop();
	}
}

uint32_t ImageSource::Start()
{
	lock_guard<recursive_mutex> lock(_mutex);
	// TODO:avoid multiple call
	// TODO:error event
	_presenter->Start();
	_demuxer->Start();
	_source->Start();

	return 0;
}

void ImageSource::GetImage(uint16_t seqNum, CameraFrame& frame)
{
	lock_guard<recursive_mutex> lock(_mutex);

	if (!_presenter) {
		return;
	}

	_presenter->GetImage(seqNum, frame);
}
