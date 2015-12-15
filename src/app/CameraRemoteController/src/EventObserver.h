#pragma once

#include "AsyncTask.h"
#include "CameraStateManager.h"
#include <string>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#define kGetEventRetryMax	5

class EventObserver;

typedef boost::signals2::signal<void(const EventObserver&, const char*)> CameraEvent_t;

class EventObserver :
	public AsyncTask
{
public:
	CameraEvent_t GetAvailableApiListChanged;
	CameraEvent_t CameraStatusChanged;
	CameraEvent_t LiveviewStatusChanged;
	CameraEvent_t StorageInfoChanged;
	CameraEvent_t CameraFunctionChanged;
	CameraEvent_t MovieQualityChanged;
	CameraEvent_t SteadyModeChanged;
	CameraEvent_t ViewAngleChanged;
	CameraEvent_t ShootModeChanged;

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
	CameraEvent_t _nullEvent;
	const char* _nullEventName;
	typedef std::pair<const char*, CameraEvent_t*> nameEventPair_t;
	const std::array<nameEventPair_t, EVENT_IDX_MAX> _events;
	CameraStateManager _stateManager;

	bool connect();
	bool updateState(boost::property_tree::ptree& pt);
	void raiseEvents(updatedObjects_t updatedIndexes);
	void raiseSingleEvent(int index);
};

