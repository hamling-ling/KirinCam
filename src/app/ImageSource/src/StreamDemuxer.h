#pragma once

#include "StreamFlow.h"
#include "ImageSourceDefs.h"
#include <queue>
#include <memory>

#define MAX_DEMUXER_QUEUE_SIZE	32

class StreamDemuxer :
	public StreamFlow
{
public:
	StreamDemuxer();
	virtual ~StreamDemuxer();
	virtual void Push(boost::asio::streambuf& stream);
protected:
	virtual void Run();
private:
	std::shared_ptr<LiveViewPacket> _packet;
	virtual void PushPacket(std::shared_ptr<LiveViewPacket> packet);
};

