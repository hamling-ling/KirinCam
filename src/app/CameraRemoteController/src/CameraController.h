#pragma once

#include <sstream>
#include <boost/asio.hpp>
#include "AsyncTask.h"
#include "DeviceDescription.h"

class CameraController : public AsyncTask
{
public:
	CameraController(DeviceDescription& deviceDescription);
	virtual ~CameraController();
	bool StartStreaming();

private:
	DeviceDescription deviceDescription_;
	std::string liveViewUrl_;

	void StartStreamingInternal();
	/**
		@param server 10.0.0.1 for HDR-AS15
		@param port 10000
		@param path /sony/camera
	 */
	int postRequest(const std::string& server, const std::string port, const std::string path);
	/**
		@param server 10.0.0.1 for HDR - AS15
		@param port 60152
		@param path /liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21
	 */
	int getRequest(const std::string& server, const std::string port, const std::string path);
	void Dump(boost::asio::streambuf& bu);
};

