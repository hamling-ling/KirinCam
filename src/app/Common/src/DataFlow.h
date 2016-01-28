#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "AsyncWork.h"
#include "Log.h"

template<class T>
class DataFlow
{
public:
	typedef std::function<void(std::atomic<bool>&, T)> DataFlowConsumeFunc;

	DataFlow(size_t maxQueueSize, DataFlowConsumeFunc func)
		: kMaxQueueSize(maxQueueSize)
	{
		_exit = false;
		_func = func;

		AsyncWorkArg arg;

		AsyncWorkFunc workFunc = bind(
			&DataFlow::ConsumeLoop,
			this, std::placeholders::_1,
			std::placeholders::_2);

		_work.Start(workFunc, arg);
	}

	~DataFlow()
	{
		_exit = true;
		_cond.notify_one();
	}

	void Enqueue(T x)
	{
		unique_lock<mutex> lock(_mutex);
		bool wasEmpty = _queue.empty();
		_queue.push(x);
		while (kMaxQueueSize < _queue.size()) {
			_queue.pop();
			LogWarn("queue overflow");
		}

		if (wasEmpty) {
			_cond.notify_one();
		}
	}

private:
	std::mutex _mutex;
	std::queue<T> _queue;
	std::condition_variable _cond;
	AsyncWork _work;
	bool _exit;
	const size_t kMaxQueueSize;
	DataFlowConsumeFunc _func;

	void ConsumeLoop(std::atomic<bool>& canceled, AsyncWorkArg& arg)
	{
		while (!canceled) {
			T x;
			{
				unique_lock<mutex> lock(_mutex);

				_cond.wait(lock, [&] { return (!_queue.empty() || canceled || _exit); });
				if (canceled || _exit) {
					break;
				}

				x = _queue.front();
				_queue.pop();
			}
			_func(canceled, x);
		}
	}
};

