#pragma once

#include "StreamFlow.h"

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
};

