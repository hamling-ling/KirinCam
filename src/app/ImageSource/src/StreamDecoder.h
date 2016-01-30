#pragma once

#include "StreamFlow.h"
#include <memory>
#include <atomic>

#define MAX_DECODER_QUEUE_SIZE	32
//#define DUMPJPEG

template<class T> class DataFlow;
class StreamDecoder :
	public StreamFlow
{
public:
	StreamDecoder(bool flip);
	virtual ~StreamDecoder();
	virtual void Push(std::shared_ptr<LiveViewPacket> packet);
private:
	typedef std::shared_ptr<LiveViewPacket> decoderFlowData_t;
	std::shared_ptr<DataFlow<decoderFlowData_t> > _flow;
	const bool _flip;

	void Run(std::atomic<bool>& canceled, decoderFlowData_t packetg);
#ifdef DUMPJPEG
	bool dumped;
#endif
};
