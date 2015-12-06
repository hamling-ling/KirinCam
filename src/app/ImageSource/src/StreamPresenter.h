#pragma once

#include "StreamFlow.h"
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
	void GetImage(std::vector<uint8_t>& buf);
protected:
	virtual void Run();
private:
	static const int kQueueSize = 32;
	std::queue<std::shared_ptr<LiveViewPacket> > _queue;
};
