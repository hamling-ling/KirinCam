#include "stdafx.h"
#include "AsyncWork.h"
#include <iostream>

using namespace std;

AsyncWork::AsyncWork()
{
	_state = AsyncWorkStateIdle;
	_exitFlag = false;
}


AsyncWork::~AsyncWork()
{
	Stop();
}

void AsyncWork::Start(AsyncWorkFunc func, AsyncWorkArg arg)
{
	lock_guard<recursive_mutex> lock(_mutex);

	AsyncWorkState expected = AsyncWorkStateIdle;
	AsyncWorkState desired = AsyncWorkStateRunning;
	if (_state.compare_exchange_weak(expected, desired)) {
		Stop();
		_func = func;
		_arg = arg;
		_thread = std::make_shared<std::thread>(&AsyncWork::Work, this);
	}
}

void AsyncWork::Stop()
{
	lock_guard<recursive_mutex> lock(_mutex);

	AsyncWorkState expected = AsyncWorkStateRunning;
	AsyncWorkState desired = AsyncWorkStateStopping;
	AsyncWorkState next = AsyncWorkStateIdle;
	if (_state.compare_exchange_weak(expected, desired)) {
		_state = AsyncWorkStateIdle;
	}

	if (_thread) {
		if (_thread->joinable()) {
			_exitFlag = true;
			_thread->join();
			_exitFlag = false;
		}
	}
}

bool AsyncWork::IsRunning()
{
	return (_state == AsyncWorkStateRunning);
}

void AsyncWork::Work()
{
	if (!_exitFlag) {
		_func(_exitFlag, _arg);
	}
	_state = AsyncWorkStateIdle;
}