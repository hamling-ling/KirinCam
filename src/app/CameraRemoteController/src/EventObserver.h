#pragma once

#include "AsyncTask.h"
#include "CameraStateManager.h"
#include <string>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#define kGetEventRetryMax	5

class EventObserver :
	public AsyncTask
{
public:
	boost::signals2::signal<void(const EventObserver&, const std::string&)> GetAvailableApiListChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> CameraStatusChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> LiveviewStatusChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> StorageInfoChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> CameraFunctionChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> MovieQualityChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> SteadyModeChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> ViewAngleChanged;
	boost::signals2::signal<void(const EventObserver&, const std::string&)> ShootModeChanged;

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
	void raiseEvents(updatedObjects_t updatedIndexes);
	void raiseSingleEvent(int index);
};

