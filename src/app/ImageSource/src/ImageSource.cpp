#include "ImageSource.h"
#include "StreamSource.h"
#include "StreamDemuxer.h"
#include "StreamDecoder.h"
#include "StreamImagePresenter.h"
#include "StreamFrameInfoPresenter.h"

using namespace std;

ImageSource::ImageSource(const std::string& url)
{
	_source = make_shared<StreamSource>(url);
	_demuxer = make_shared<StreamDemuxer>();
	_decoder = make_shared<StreamDecoder>();
	_imagePresenter = make_shared<StreamImagePresenter>();
	_frameInfoPresenter = make_shared<StreamFrameInfoPresenter>();

	_source->Connect(_demuxer);
	_demuxer->Connect(_decoder);
	_demuxer->Connect(_frameInfoPresenter);

	_decoder->Connect(_imagePresenter);
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
	if (_frameInfoPresenter) {
		_frameInfoPresenter->Stop();
	}
	if (_decoder) {
		_decoder->Stop();
	}
	if (_imagePresenter) {
		_imagePresenter->Stop();
	}
}

uint32_t ImageSource::Start()
{
	lock_guard<recursive_mutex> lock(_mutex);
	// TODO:avoid multiple call
	// TODO:error event
	_frameInfoPresenter->Start();
	_imagePresenter->Start();
	_demuxer->Start();
	_source->Start();

	return 0;
}

bool ImageSource::GetImage(uint16_t seqNum, CameraFrame& frame)
{
	lock_guard<recursive_mutex> lock(_mutex);

	if (!_imagePresenter) {
		return false;
	}

	if (_frameInfoPresenter) {
		_frameInfoPresenter->GetCameraFrame(seqNum, frame);
	}
	return _imagePresenter->GetCameraFrame(seqNum, frame);
}
