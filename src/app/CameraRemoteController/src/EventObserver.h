#pragma once

#include "AsyncTask.h"
#include "CameraStateManager.h"
#pragma warning(disable:4996)
#define _SCL_SECURE_NO_WARNINGS
//#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#define kGetEventRetryMax	5

class EventObserver :
	public AsyncTask
{
public:
	//boost::signals2::signal<void(const EventObserver&)> getAvailableApiListChanged;

	EventObserver();
	virtual ~EventObserver();
	void Subscribe(const std::string& url);
	void EventReceiveProc();
	void Stop();
private:
	std::string _server;
	std::string _port;
	std::string _path;
	boost::asio::ip::tcp::socket _socket;

	CameraStateManager _stateManager;

	bool connect();
	bool updateState(boost::property_tree::ptree& pt);
};

