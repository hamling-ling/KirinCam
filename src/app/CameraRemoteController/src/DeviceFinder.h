#pragma once

#include <string>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include "DeviceDescription.h"
#include "AsyncTask.h"

class DeviceFinder : public AsyncTask
{
public:
	DeviceFinder();
	virtual ~DeviceFinder();
	virtual void Start(asyncTaskCallback_t callback);
	DeviceDescription& GetDeviceDescription();

private:
	boost::asio::ip::udp::endpoint udp_endpoint_;
	boost::asio::ip::tcp::endpoint tcp_endpoint_;
	boost::asio::ip::udp::socket udp_socket_;
	boost::asio::ip::tcp::socket tcp_socket_;
	boost::asio::streambuf tcp_request_;
	boost::asio::streambuf tcp_response_;
	std::string deviceDescriptionUrl_;
	DeviceDescription deviceDescription_;
	std::string message_;
	enum { kDataMaxLength = 1024 };
	char data_[kDataMaxLength];

	std::stringstream content_;
	asyncTaskCallback_t callback_;

	void handleSendToMSearch(const boost::system::error_code& error);
	void handleMSearchRespHeader(const boost::system::error_code& err, size_t bytes_recvd);
	void GetDeviceDescription(std::string description_url);
	void handleGetDeviceDescriptionResponseHeader(const boost::system::error_code& error);
	void handleGetDeviceDescriptionResponseHeaderStatusCode(const boost::system::error_code& error, const size_t bytes_transferred);
	void handleGetDeviceDescriptionRensponseHeader(const boost::system::error_code& error);
	void handleGetDeviceDescriptionRensponseContent(const boost::system::error_code& error);
	void handleError(const boost::system::error_code& error, const std::string& msg);
	void handleStatusError(unsigned int statusCode, const std::string& msg);
	void Cleanup();
};

