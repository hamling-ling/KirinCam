#pragma once

#include "AsyncTask.h"
#include "CameraStateManager.h"

#define kGetEventRetryMax	5

class EventObserver :
	public AsyncTask
{
public:
	EventObserver();
	virtual ~EventObserver();
	void Subscribe(const std::string& url);
	void EventReceiveProc();
private:
	std::string _server;
	std::string _port;
	std::string _path;

	CameraStateManager _stateManager;
};

