#include "AsyncTask.h"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

boost::shared_ptr<boost::asio::io_service::work> kNullWork = boost::shared_ptr<boost::asio::io_service::work>();


AsyncTask::AsyncTask()
{
	_work = boost::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(_ioService));
	_isRunning = false;
	_isStopping = false;
}

AsyncTask::~AsyncTask()
{
	Stop();
}

void AsyncTask::Start(asyncTaskCallback_t callback)
{
	if (IsStarted()) {
		return;
	}
	_callback = callback;

	_thread = boost::thread(boost::bind(&AsyncTask::Run, this));
}

void AsyncTask::Stop()
{
	if (_isStopping) {
		return;
	}

	_isStopping = true;
	_ioService.stop();

	_work = kNullWork;
	if (_thread.joinable()) {
		_thread.join();
	}
	_ioService.reset();
	_ioService.run();
	_isStopping = false;
}

boost::asio::io_service& AsyncTask::IoService()
{
	return _ioService;
}

void AsyncTask::Run()
{
	_isRunning = true;
	_ioService.run();
	_isRunning = false;
}

bool AsyncTask::IsStarted()
{
	return _isRunning;
}

bool AsyncTask::IsStopping()
{
	return _isStopping;
}

void AsyncTask::RaiseEvent(const boost::system::error_code &e) {
	if (e == boost::asio::error::operation_aborted) {
		return;
	}
	else if (e) {
		return;
	}

	if (_callback) {
		_callback(this, e.value());
	}
}

void AsyncTask::RaiseEvent(uint32_t errorCode) {
	if (_callback) {
		_callback(this, errorCode);
	}
}
