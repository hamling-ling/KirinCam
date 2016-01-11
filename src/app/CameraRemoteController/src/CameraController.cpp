#include "stdafx.h"
#include "CameraController.h"
#include <ostream>
#include <fstream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "Common.h"
#include "ImageSourceApi.h"
#include "EventObserver.h"
#include "Success.h"
#include "ErrorStatus.h"

using namespace std;
using boost::asio::ip::tcp;
using boost::property_tree::ptree;

CameraController::CameraController(DeviceDescription& deviceDescription)
{
	deviceDescription_ = deviceDescription;
	eventObserver_ = std::make_shared<EventObserver>();
}

CameraController::~CameraController()
{
	if (eventObserver_) {
		eventObserver_->Stop();
	}
	StopStreaming();
}

bool CameraController::SubscribeEvent()
{
	string url = deviceDescription_.CameraServiceUrl();
	if (url.empty()) {
		return false;
	}

	eventObserver_->Subscribe(deviceDescription_.CameraServiceUrl());
	return true;
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

void CameraController::GetImage(uint16_t seqNum, CameraFrame& frame)
{
	lock_guard<recursive_mutex> lock(imageSourceMutex_);

	if (!imageSource_) {
		return;
	}

	imageSource_->GetImage(seqNum, frame);
}

bool CameraController::StartRecording()
{
	string json_request("{\"method\": \"startMovieRec\",\"params\": [],\"id\": 1,\"version\": \"1.0\"} \r\n");
	return (invokeCameraService(json_request) != 0);
}

bool CameraController::StopRecording()
{
	string json_request("{\"method\": \"stopMovieRec\",\"params\": [],\"id\": 1,\"version\": \"1.0\"} \r\n");
	return (invokeCameraService(json_request) != 0);
}

EventObserver& CameraController::GetEventObserver()
{
	return *eventObserver_;
}

void CameraController::StartStreamingInternal()
{
	if (startLiveView() != 0) {
		return;
	}

	lock_guard<recursive_mutex> lock(imageSourceMutex_);
	imageSource_ = std::make_shared<ImageSource>(liveViewUrl_);
	imageSource_->Start();
}

int CameraController::startLiveView()
{
	// prepare request command
	string json_request("{\"method\": \"startLiveview\",\"params\" : [],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
	ptree pt;

	if (invokeCameraService(json_request, pt) != 0) {
		return 1;
	}

	Success success;
	if (success.SetStatus(pt)) {
		liveViewUrl_ = success.ContentAsString();
		return 0;
	}

	return -1;
}

int CameraController::invokeCameraService(const string& cmd, ptree& result)
{
	string url = deviceDescription_.CameraServiceUrl();
	if (url.empty()) {
		return false;
	}

	return InvokeCommand(url, cmd, result);
}

int CameraController::invokeCameraService(const string& cmd)
{
	string url = deviceDescription_.CameraServiceUrl();
	if (url.empty()) {
		return false;
	}

	ptree pt;
	if (InvokeCommand(url, cmd, pt) != 0) {
		return false;
	}

	Success success;
	if (success.SetStatus(pt)) {
		if (success.IsZero()) {
			return 0;
		}
	}

	ErrorStatus err;
	if (err.SetStatus(pt)) {
		cerr << err.StatusCode() << ":" << err.StatusMessage() << endl;
		return false;
	}

	cerr << "InvokeCameraService returned unknown response" << endl;
	return false;
}
