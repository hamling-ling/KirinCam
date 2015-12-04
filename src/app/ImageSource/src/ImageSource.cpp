#include "stdafx.h"
#include "../include/ImageSource.h"
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
	_presenter->Start();
	_demuxer->Start();
	_source->Start();

	return 0;
}
