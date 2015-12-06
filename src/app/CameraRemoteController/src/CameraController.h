#pragma once

#include <sstream>
#include <mutex>
#include <vector>
#include <boost/asio.hpp>
#include "AsyncTask.h"
#include "DeviceDescription.h"

class ImageSource;

class CameraController : public AsyncTask
{
public:
	CameraController(DeviceDescription& deviceDescription);
	virtual ~CameraController();
	bool StartStreaming();
	void StopStreaming();
	void GetImage(std::vector<uint8_t> &buf);
private:
	DeviceDescription deviceDescription_;
	std::recursive_mutex imageSourceMutex_;

	// http://10.0.0.1:60152/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21
	std::string liveViewUrl_;
	std::shared_ptr<ImageSource> imageSource_;

	void StartStreamingInternal();

	/**
		@param server 10.0.0.1 for HDR-AS15
		@param port 10000
		@param path /sony/camera
	 */
	int startLiveView(const std::string& server, const std::string port, const std::string path);
	void Dump(boost::asio::streambuf& bu);
};

