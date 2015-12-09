#pragma once
#include "C:\Users\nobu\Documents\GitHub\KirinCam\src\app\CameraRemoteController\src\AsyncTask.h"
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
};

