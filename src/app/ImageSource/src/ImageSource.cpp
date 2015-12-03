#include "stdafx.h"
#include "../include/ImageSource.h"
#include "StreamSource.h"
#include "StreamDemuxer.h"
#include "StreamPresenter.h"

using namespace std;

ImageSource::ImageSource()
{
	_source = nullptr;
}

ImageSource::~ImageSource()
{
	_source->Stop();
	_demuxer->Stop();
	_presenter->Stop();
}

uint32_t ImageSource::SetSource(const std::string& url)
{
	// TODO: mutex, initialize
	_source = std::make_shared<StreamSource>(url);
	_demuxer = std::make_shared<StreamDemuxer>();
	_presenter = std::make_shared<StreamPresenter>();

	_source->Connect(_demuxer);
	_demuxer->Connect(_presenter);

	_presenter->Start();
	_demuxer->Start();
	_source->Start();

	return 0;
}
