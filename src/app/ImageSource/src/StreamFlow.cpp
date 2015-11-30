#include "stdafx.h"
#include "StreamFlow.h"

using namespace std;

StreamFlow::StreamFlow() :
_isRunning(false),
_isStopping(false)
{
}

StreamFlow::~StreamFlow()
{
	Stop();
}

void StreamFlow::Connect(shared_ptr<StreamFlow> flow)
{
	_downStream = flow;
}

void StreamFlow::Push(boost::asio::streambuf& stream)
{
	lock_guard<recursive_mutex> lock(_mutex);
	_downStream->Push(stream);
}

void StreamFlow::Start()
{
	lock_guard<recursive_mutex> lock(_mutex);
	_thread = std::make_shared<std::thread>(&StreamFlow::Run, this);
}

void StreamFlow::Stop()
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (!_thread) {
		return;
	}

	if (!_isStopping) {
		_isStopping = true;
		if (_thread->joinable()) {
			_thread->join();
		}
		_isStopping = false;
	}
}

void StreamFlow::Run()
{
	_isRunning = true;
	while (!_isStopping) {
		// do something
	}
	_isRunning = false;
}
