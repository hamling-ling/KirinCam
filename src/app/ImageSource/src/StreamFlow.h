#pragma once

#include <memory>
#include <mutex>
#include <vector>
#include <boost/asio.hpp>
#include "LiveViewStreamParser.h"
#include "ImageSourceDefs.h"

template<class T> class DataFlow;

class StreamFlow
{
public:
	StreamFlow();
	virtual ~StreamFlow();
	virtual void Connect(std::shared_ptr<StreamFlow> flow);
	virtual void Push(boost::asio::streambuf& stream);
	virtual void Push(std::shared_ptr<LiveViewPacket> packet);
	virtual void Push(std::vector<uint8_t> data);
	virtual void Push(CameraFrame& camFrame);
	virtual void Start();
	virtual void Stop();
protected:
	std::vector<std::shared_ptr<StreamFlow> > _downStreams;
};

