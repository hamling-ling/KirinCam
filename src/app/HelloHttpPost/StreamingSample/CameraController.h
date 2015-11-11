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
	boost::asio::ip::tcp::endpoint tcp_endpoint_;
	boost::asio::ip::tcp::socket tcp_socket_;
	boost::asio::ip::tcp::endpoint liveview_endpoint_;
	boost::asio::ip::tcp::socket liveview_socket_;
	boost::asio::streambuf tcp_request_;
	boost::asio::streambuf tcp_response_;
	boost::asio::streambuf liveview_request_;
	boost::asio::streambuf liveview_response_;
	boost::asio::streambuf contentbuf_;
	//std::stringstream content_;

	void handleWriteRequest(const boost::system::error_code& err);
	void handleReadStatusLine(const boost::system::error_code& err, const size_t bytes_transferred);
	void CameraController::getLiveImage();
	void handleGetLiveImageResponse(const boost::system::error_code& err);
	void handleGetLiveImageStatusCode(const boost::system::error_code& error, const size_t bytes_transferred);
	void handleGetLiveImageHeader(const boost::system::error_code& error);
	void handleGetLiveViewContent(const boost::system::error_code& error);

	void Cleanup();
	void Dump();
};

