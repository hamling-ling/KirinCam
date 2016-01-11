#pragma once

#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <functional>
#include "AsyncWorkArg.h"

enum AsyncWorkState {
	AsyncWorkStateIdle,
	AsyncWorkStateRunning,
	AsyncWorkStateStopping
};

typedef std::function<void(std::atomic<bool>&, AsyncWorkArg& arg)> AsyncWorkFunc;

class  AsyncWork
{
public:
	AsyncWork();
	virtual ~AsyncWork();
	virtual void Start(AsyncWorkFunc func, AsyncWorkArg arg);
	virtual void Stop();
	bool IsRunning();

private:
	// non null only when _state is AsyncWorkStateRunning or Stopping
	std::shared_ptr<std::thread> _thread;
	// guard _thread and IF
	std::recursive_mutex _mutex;
	// state
	std::atomic<AsyncWorkState> _state;
	// make it true to stop the thread
	std::atomic<bool> _exitFlag;
	// func to execute
	AsyncWorkFunc _func;
	// arg
	AsyncWorkArg _arg;

	void Work();
};

