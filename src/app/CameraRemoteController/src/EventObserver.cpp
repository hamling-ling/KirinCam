#include "stdafx.h"
#include "EventObserver.h"
#include "Common.h"

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
	string json_request("{\"method\": \"getEvent\",\"params\" : [false],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
	ptree pt;

	if (InvokeCommand(_server, _port, _path, json_request, pt) != 0) {
		return;
	}

	// debug
	stringstream ss;
	write_json(ss, pt);
	cout << ss.str() << endl;
}
