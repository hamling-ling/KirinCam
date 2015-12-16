#pragma once

#include "StreamFlow.h"
#include "ImageSourceDefs.h"
#include <queue>
#include <vector>

#define MAX_QUEUE_SIZE	32

class StreamPresenter :
	public StreamFlow
{
public:
	StreamPresenter();
	virtual ~StreamPresenter();
	void Push(std::shared_ptr<LiveViewPacket> packet);
	void GetImage(uint16_t seqNum, CameraFrame& frameData);
protected:
	virtual void Run();
private:
	static const int kQueueSize = 32;
	std::queue<std::shared_ptr<LiveViewPacket> > _imgQueue;
	std::queue<std::shared_ptr<LiveViewPacket> > _infoQueue;
};
