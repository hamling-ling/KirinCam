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

EventObserver::EventObserver()
{
}

EventObserver::~EventObserver()
{
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

	{
		string json_request("{\"method\": \"getEvent\",\"params\" : [false],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
		ptree pt1st;

		if (InvokeCommand(_server, _port, _path, json_request, pt1st) != 0) {
			cout << "EventSubscription Failed" << endl;
			// todo:Raise event here
			return;
		}
	}

	string json_request("{\"method\": \"getEvent\",\"params\" : [true],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
	ptree pt2nd;
	bool retryCount = 0;
	while (!IsStopping()) {
		if (InvokeCommand(_server, _port, _path, json_request, pt2nd) != 0) {
			boost::this_thread::sleep(boost::posix_time::millisec(100));
			if (kGetEventRetryMax <= retryCount) {
				// todo Raise event here
				return;
			}
			retryCount++;
			continue;
		}

		ErrorStatus status;
		if (status.SetStatus(pt2nd)) {
			if (status.StatusCode() == ErrorStatusCodeTimeout) {
				continue;
			}
			if (kGetEventRetryMax <= retryCount) {
				// todo Raise event here
				return;
			}
			retryCount++;
			continue;
		}

		set<string> updatedObjNames;
		_stateManager.UpdateState(pt2nd, updatedObjNames);

		retryCount = 0;
		// debug
		stringstream ss;
		write_json(ss, pt2nd);
		cout << ss.str() << endl;
	}
	cout << __FUNCTION__ << " EXIT" << endl;
}
