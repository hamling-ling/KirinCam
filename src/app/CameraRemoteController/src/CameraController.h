#pragma once

#include <sstream>
#include <mutex>
#include <vector>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include "AsyncTask.h"
#include "DeviceDescription.h"
#include "ImageSourceApi.h"

class ImageSource;
class EventObserver;
class Success;

class CameraController : public AsyncTask
{
public:
	CameraController(DeviceDescription& deviceDescription);
	virtual ~CameraController();
	bool SubscribeEvent();
	bool StartStreaming();
	void StopStreaming();
	bool GetImage(uint16_t seqNum, CameraFrame& frame);
	bool StartRecording();
	bool StopRecording();
	EventObserver& GetEventObserver();

private:
	DeviceDescription deviceDescription_;
	std::recursive_mutex imageSourceMutex_;

	// http://10.0.0.1:60152/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21
	std::string liveViewUrl_;
	std::shared_ptr<ImageSource> imageSource_;
	std::shared_ptr<EventObserver> eventObserver_;

	void StartStreamingInternal();

	/**
		@param server 10.0.0.1 for HDR-AS15
		@param port 10000
		@param path /sony/camera
	 */
	int startLiveView();

	int invokeCameraService(const std::string& cmd, boost::property_tree::ptree& result);
	int invokeCameraService(const std::string& cmd);
};

