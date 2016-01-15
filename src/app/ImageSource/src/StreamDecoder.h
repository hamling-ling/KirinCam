#pragma once

#include "StreamFlow.h"
#include <memory>
#include <atomic>

#define MAX_DECODER_QUEUE_SIZE	32

template<class T> class DataFlow;
class StreamDecoder :
	public StreamFlow
{
public:
	StreamDecoder();
	virtual ~StreamDecoder();
	virtual void Push(std::shared_ptr<LiveViewPacket> packet);
private:
	typedef std::shared_ptr<LiveViewPacket> decoderFlowData_t;
	std::shared_ptr<DataFlow<decoderFlowData_t> > _flow;

	void Run(std::atomic<bool>& canceled, decoderFlowData_t packetg);
};
