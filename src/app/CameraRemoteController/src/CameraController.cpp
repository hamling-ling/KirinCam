#include "stdafx.h"
#include "CameraController.h"
#include <ostream>
#include <fstream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "Common.h"
#include "ImageSource.h"

using namespace std;
using boost::asio::ip::tcp;
using boost::property_tree::ptree;

CameraController::CameraController(DeviceDescription& deviceDescription)
{
	deviceDescription_ = deviceDescription;
}

CameraController::~CameraController()
{
	StopStreaming();
}

bool CameraController::StartStreaming()
{
	if (IsStarted()) {
		return true;
	}

	IoService().post(boost::bind(&CameraController::StartStreamingInternal, this));
	Start(NULL);

	return true;
}

void CameraController::StopStreaming()
{
	if (!IsStarted()) {
		return;
	}

	Stop();

}

void CameraController::StartStreamingInternal()
{
	
	{
		std::string server;
		std::string port;
		std::string path;

		splitUrl(deviceDescription_.CameraServiceUrl(), server, port, path);
		startLiveView(server, port, path);
	}

	imageSource_ = std::make_shared<ImageSource>(liveViewUrl_);
	imageSource_->Start();
}

int CameraController::startLiveView(const std::string& server, const std::string port, const std::string path)
{
	// prepare request command
	string json_request("{\"method\": \"startLiveview\",\"params\" : [],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
	ptree pt;

	if (InvokeCommand(server, port, path, json_request, pt) != 0) {
		return 1;
	}

	BOOST_FOREACH(const ptree::value_type& child, pt.get_child("result")) {
		liveViewUrl_ = child.second.get_value<std::string>();
	}

	return 0;
}
