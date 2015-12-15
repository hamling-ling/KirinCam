#include "stdafx.h"
#include "EventObserver.h"
#include "Common.h"
#include "ErrorStatus.h"

#include <sstream>// debug
#include <iostream>// debug

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;
using boost::asio::ip::tcp;

static const string kImmediateGetEventRequest("{\"method\": \"getEvent\",\"params\" : [false],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
static const string kPollingGetEventRequest("{\"method\": \"getEvent\",\"params\" : [true],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
static const char* kEmptyString = "";

EventObserver::EventObserver()
:
_socket(IoService()),
_nullEventName(kEmptyString),
_events({
	make_pair(kAvailableApiList, &GetAvailableApiListChanged),	// 0
	make_pair(kCameraStatus, &CameraStatusChanged),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(kLiveviewStatus, &LiveviewStatusChanged),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(kStorageInformation, &StorageInfoChanged),		// 10
	make_pair(_nullEventName, &_nullEvent),
	make_pair(kCurrentCameraFunction, &CameraFunctionChanged),
	make_pair(kCurrentMoviewQuality, &MovieQualityChanged),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(kCurrentSteadyMode, &SteadyModeChanged),
	make_pair(kCurrentViewAngle, &ViewAngleChanged),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),						// 20
	make_pair(kCurrentShootMode, &ShootModeChanged),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),						// 30
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent),
	make_pair(_nullEventName, &_nullEvent)
})
{
	nameEventPair_t a = { _nullEventName, &_nullEvent };
	nameEventPair_t c(_nullEventName, &_nullEvent);
	std::array<nameEventPair_t, 1>  b = { make_pair(_nullEventName, &_nullEvent) };
}

EventObserver::~EventObserver()
{
}

void EventObserver::Stop()
{
	_socket.close();
	AsyncTask::Stop();
}

void EventObserver::Subscribe(const std::string& url)
{
	if (IsStarted()) {
		return;
	}

	splitUrl(url, _server, _port, _path);

	IoService().post(boost::bind(&EventObserver::EventReceiveProc, this));

	Start(NULL);
}

void EventObserver::EventReceiveProc()
{
	cout << __FUNCTION__ << " ENT" << endl;

	boost::system::error_code error = boost::asio::error::host_not_found;
	if (!connect()) {
		// todo:Raise event here
		return;
	}

	ptree pt;
	{
		if (InvokeCommand(_socket, _server, _port, _path, kImmediateGetEventRequest, pt) != 0) {
			cout << "EventSubscription Failed" << endl;
			// todo:Raise event here
			return;
		}
		if (!updateState(pt)) {
			return;
		}
	}

	int retryCount = 0;
	while (!IsStopping()) {
		if (!connect()) {
			// todo:Raise event here
			return;
		}

		if (InvokeCommand(_socket, _server, _port, _path, kPollingGetEventRequest, pt) != 0) {
			boost::this_thread::sleep(boost::posix_time::millisec(100));
			if (kGetEventRetryMax <= retryCount) {
				// todo Raise event here
				return;
			}
			retryCount++;
			continue;
		}

		retryCount = 0;
		if (!updateState(pt)) {
			return;
		}

		// debug
		stringstream ss;
		write_json(ss, pt);
		cout << ss.str() << endl;
	}
	cout << __FUNCTION__ << " EXIT" << endl;
}

bool EventObserver::connect()
{
	bool result = true;
	try
	{
		connectSocket(IoService(), _socket, _server, _port);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		result = false;
	}
	return result;
}

bool EventObserver::updateState(boost::property_tree::ptree& pt)
{
	ErrorStatus status;
	if (status.SetStatus(pt)) {
		if (status.StatusCode() == ErrorStatusCodeTimeout) {
			return true;
		}
		updatedObjects_t updatedObjIndexes;
		_stateManager.UpdateState(pt, updatedObjIndexes);
	}

	return true;
}

void EventObserver::raiseEvents(updatedObjects_t updatedIndexes)
{
	for (int i = 0; i < updatedIndexes.size(); i++) {
		if (updatedIndexes[i]) {
			raiseSingleEvent(i);
		}
	}
}

void EventObserver::raiseSingleEvent(int index) {
	const char* eventName = _events[index].first;
	CameraEvent_t* pevt = _events[index].second;
	(*pevt)(*this, eventName);
}
