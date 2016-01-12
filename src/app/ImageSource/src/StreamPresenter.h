#pragma once

#include "StreamFlow.h"
#include "ImageSourceDefs.h"
#include <queue>

#define MAX_PRESENTER_QUEUE_SIZE	32

class StreamPresenter :
	public StreamFlow
{
public:
	const size_t kMaxQueueSize = MAX_PRESENTER_QUEUE_SIZE;
	StreamPresenter();
	virtual ~StreamPresenter();
	virtual bool GetCameraFrame(uint16_t seqNum, CameraFrame& frameData);
protected:
	std::recursive_mutex _mutex;
	std::queue<CameraFrame> _queue;
};
