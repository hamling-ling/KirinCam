#pragma once

#include "StreamFlow.h"
#include <queue>

class StreamPresenter :
	public StreamFlow
{
public:
	StreamPresenter();
	virtual ~StreamPresenter();
	void Push(std::shared_ptr<LiveViewPacket> packet);
private:
	static const int kQueueSize = 32;
	std::queue<std::shared_ptr<LiveViewPacket> > _queue;
};
