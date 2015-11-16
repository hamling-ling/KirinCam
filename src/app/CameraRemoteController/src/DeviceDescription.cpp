#include "stdafx.h"
#include "DeviceDescription.h"
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using boost::property_tree::ptree;

DeviceDescription::DeviceDescription()
{
}


DeviceDescription::~DeviceDescription()
{
}

bool DeviceDescription::ParseXml(std::string xml)
{
	stringstream ss(xml);

	ptree pt;
	read_xml(ss, pt);
	_liveViewUrl = pt.get<string>("root.device.av:X_ScalarWebAPI_DeviceInfo.av:X_ScalarWebAPI_ImagingDevice.av:X_ScalarWebAPI_LiveView_URL");
	for (ptree::value_type &v : pt.get_child("root.device.av:X_ScalarWebAPI_DeviceInfo.av:X_ScalarWebAPI_ServiceList")){
		string service = v.second.get<string>("av:X_ScalarWebAPI_ServiceType");
		if (service == "camera")
			_cameraServiceUrl = v.second.get<string>("av:X_ScalarWebAPI_ActionList_URL")
								+ "/" + service;
	}
	return !_liveViewUrl.empty();
}

const string& DeviceDescription::FriendlyName()
{
	return _friendlyName;
}

const string& DeviceDescription::LiveViewUrl()
{
	return _liveViewUrl;
}

const string& DeviceDescription::CameraServiceUrl()
{
	return _cameraServiceUrl;
}
