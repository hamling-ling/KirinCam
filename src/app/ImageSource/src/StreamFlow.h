#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include "LiveViewStreamParser.h"

class StreamFlow
{
public:
	StreamFlow();
	virtual ~StreamFlow();
	virtual void Connect(std::shared_ptr<StreamFlow> flow);
	virtual void Push(boost::asio::streambuf& stream);
	virtual void Start();
	virtual void Stop();
protected:
	virtual void Run();
	bool IsStopping();
	std::shared_ptr<StreamFlow> _downStream;
	std::shared_ptr<std::thread> _thread;
	std::recursive_mutex _mutex;
	bool _isStopping;
	bool _isRunning;
};

