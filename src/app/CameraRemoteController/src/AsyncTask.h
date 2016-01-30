#pragma once

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>

class AsyncTask;

typedef boost::function<void(AsyncTask*, uint32_t)> asyncTaskCallback_t;

class AsyncTask
{
public:
	AsyncTask();
	virtual ~AsyncTask();
	virtual void Start(asyncTaskCallback_t callback);
	virtual void Stop();
	bool IsStarted();
	bool IsStopping();

protected:
	boost::asio::io_service& IoService();
	boost::asio::io_service _ioService;
	void RaiseEvent(const boost::system::error_code &e);
	void RaiseEvent(uint32_t errorCode);

private:
	asyncTaskCallback_t _callback;

	boost::shared_ptr<boost::asio::io_service::work> _work;
	boost::thread _thread;
	bool _isRunning;
	bool _isStopping;

	void Run();
};

