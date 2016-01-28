#include "StreamPresenter.h"
#include "Common.h"

using namespace std;

StreamPresenter::StreamPresenter()
{
}

StreamPresenter::~StreamPresenter()
{
}

bool StreamPresenter::GetCameraFrame(uint16_t seqNum, CameraFrame& camFrame)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_queue.empty()) {
		return false;
	}

	while (!_queue.empty()) {
		if (seqNum < _queue.front().sequenceNumber) {
			camFrame = _queue.front();
			_queue.pop();
			break;
		}
		_queue.pop();
		LogWarn("skip frame");
	}
	return true;
}
