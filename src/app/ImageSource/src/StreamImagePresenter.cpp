#include "stdafx.h"
#include "StreamImagePresenter.h"
#include <queue>
#include <memory>

using namespace std;

StreamImagePresenter::StreamImagePresenter()
{
}


StreamImagePresenter::~StreamImagePresenter()
{
}

void StreamImagePresenter::Push(CameraFrame& cameraFrame)
{
	lock_guard<recursive_mutex> lock(_mutex);
	_queue.push(cameraFrame);

	while (_queue.size() > kMaxQueueSize) {
		_queue.pop();
	}
}
