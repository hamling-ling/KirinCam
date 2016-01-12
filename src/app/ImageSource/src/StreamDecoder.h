#pragma once

#include "StreamFlow.h"
#include <memory>
#include <atomic>

template<class T> class DataFlow;
class AsyncWorkArg;

class StreamDecoder :
	public StreamFlow
{
public:
	StreamDecoder();
	virtual ~StreamDecoder();

private:
	typedef std::shared_ptr<LiveViewPacket> flowData_t;
	std::shared_ptr<DataFlow<flowData_t> > _flow;

	void Run(std::atomic<bool>& canceled, AsyncWorkArg& arg);
};