#include "stdafx.h"
#include "DataFlow.h"
#include <functional>
#if 0
// debug
#include <iostream>
#include <thread>

using namespace std;

template<typename T>
DataFlow<T>::DataFlow(size_t maxQueueSize) : kMaxQueueSize(maxQueueSize)
{
	_exit = false;
	AsyncWorkArg arg;

	AsyncWorkFunc func = bind(
		&DataFlow::WorkFunc,
		this, std::placeholders::_1,
		std::placeholders::_2);

	_work.Start(func, arg);
}

template<typename T>
DataFlow<T>::~DataFlow()
{
	_exit = true;
	_cond.notify_one();
}

template <typename T>
void DataFlow<T>::Enqueue(T x)
{
	unique_lock<mutex> lock(_mutex);
	bool wasEmpty = _queue.empty();
	_queue.push(x);
	while (kMaxQueueSize < _queue.size()) {
		_queue.pop();
	}

	if (wasEmpty) {
		_cond.notify_one();
	}
}

template <typename T>
void DataFlow<T>::WorkFunc(atomic<bool>& canceled, AsyncWorkArg& arg)
{
	while (!canceled) {
		int x = 0;
		{
			unique_lock<mutex> lock(_mutex);

			_cond.wait(lock, [&] { return (!_queue.empty() || canceled || _exit); });
			if (canceled || _exit) {
				break;
			}

			x = _queue.front();
			_queue.pop();
		}

		cout << "working with " << x << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}
#endif
