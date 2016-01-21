#include "DeviceFinder.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

static const std::string kMcAddrStr("239.255.255.250");
static const std::string kMcPortStr("1900");
static const boost::asio::ip::address kMcAddr = boost::asio::ip::address::from_string(kMcAddrStr);
static const unsigned short kMcPort=boost::lexical_cast<unsigned short>(kMcPortStr);

DeviceFinder::DeviceFinder()
	:
	udp_endpoint_(kMcAddr, kMcPort),
	udp_socket_(_ioService, udp_endpoint_.protocol()),
	tcp_socket_(_ioService)
{
}

DeviceFinder::~DeviceFinder()
{
}

void DeviceFinder::Start(asyncTaskCallback_t callback)
{
	stringstream os;
	os << "M-SEARCH * HTTP/1.1\r\n";
	os << "HOST: " << kMcAddrStr << ":" << kMcPortStr << "\r\n";
	os << "MAN: \"ssdp:discover\"\r\n";
	os << "MX: 4\r\n";
	os << "ST: urn:schemas-sony-com:service:ScalarWebAPI:1\r\n";
	os << "\r\n";
	message_ = os.str();
	udp_socket_.set_option(asio::ip::multicast::outbound_interface(asio::ip::address_v4::from_string("10.0.1.1")));
	udp_socket_.async_send_to(
		boost::asio::buffer(message_), udp_endpoint_,
		boost::bind(&DeviceFinder::handleSendToMSearch, this,
		boost::asio::placeholders::error));

	AsyncTask::Start(callback);
}

DeviceDescription& DeviceFinder::GetDeviceDescription()
{
	return deviceDescription_;
}

void DeviceFinder::handleSendToMSearch(const boost::system::error_code& error)
{
	if (error) {
		Cleanup();
		handleError(error, __FUNCTION__);
		return;
	}

	udp_socket_.async_receive(
		asio::buffer(data_),// "\r\n",
		boost::bind(&DeviceFinder::handleMSearchRespHeader, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void DeviceFinder::handleMSearchRespHeader(const boost::system::error_code& error, size_t bytes_recvd)
{
	if (error) {
		handleError(error, __FUNCTION__);
		return;
	}

	// Check that response is OK.
	stringstream http_response = stringstream(data_);
	string http_version;
	http_response >> http_version;
	unsigned int status_code;
	http_response >> status_code;
	string status_message;
	getline(http_response, status_message);
	if (!http_response || http_version.substr(0, 5) != "HTTP/") {
		Cleanup();
		handleStatusError(-1, status_message);
		return;
	}
	if (status_code != 200) {
		Cleanup();
		handleStatusError(status_code, status_message);
		return;
	}
	string header;
	string description_url;
	while (getline(http_response, header) && header != "\r") {
		if (header.substr(0, 9) == "LOCATION:") {
			description_url = header.substr(10, header.length() - 11);
		}
		cout << header << "\n";
	}
	cout << "\n";

	GetDeviceDescription(description_url);
}

void DeviceFinder::GetDeviceDescription(std::string description_url)
{
	size_t last_slash = description_url.rfind('/');
	size_t colon = description_url.rfind(':');
	string server = description_url.substr(7, colon - 7);
	string port = description_url.substr(colon + 1, last_slash - colon - 1);

	string path = description_url.substr(last_slash);
	stringstream ss;
	ss << "GET " << path << " HTTP/1.1\r\n";
	ss << "Host: " << server << "\r\n";
	ss << "Accept: */*\r\n";
	ss << "Connection: close\r\n\r\n";

	ostream request_stream(&tcp_request_);
	request_stream << ss.str();

	cout << "-- device description get request ---" << endl;
	cout << ss.str() << endl;
	cout << "-- end of device description get request --" << endl;

	// The connection was successful. Send the request.
	boost::asio::ip::address addr = boost::asio::ip::address::from_string(server);
	boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(addr, lexical_cast<unsigned short>(port));
	tcp_socket_.connect(endpoint);

	boost::asio::async_write(
		tcp_socket_,
		tcp_request_,
		boost::bind(&DeviceFinder::handleGetDeviceDescriptionResponseHeader,
this,
boost::asio::placeholders::error));
}

void DeviceFinder::handleGetDeviceDescriptionResponseHeader(const boost::system::error_code& error)
{
	if (error) {
		Cleanup();
		handleError(error, __FUNCTION__);
		return;
	}

	boost::asio::async_read_until(tcp_socket_, tcp_response_, "\0",
		boost::bind(&DeviceFinder::handleGetDeviceDescriptionResponseHeaderStatusCode, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void DeviceFinder::handleGetDeviceDescriptionResponseHeaderStatusCode(const boost::system::error_code& error, const size_t bytes_transferred)
{
	if (error) {
		handleError(error, __FUNCTION__);
		Cleanup();
		return;
	}

	// Check that response is OK.
	std::istream response_stream(&tcp_response_);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
		Cleanup();
		handleStatusError(-1, status_message);
		return;
	}
	if (status_code != 200) {
		Cleanup();
		handleStatusError(status_code, status_message);
		return;
	}
	// Read the response headers, which are terminated by a blank line.
	boost::asio::async_read_until(tcp_socket_, tcp_response_, "\r\n\r\n",
		boost::bind(&DeviceFinder::handleGetDeviceDescriptionRensponseHeader, this,
			boost::asio::placeholders::error));
}

void DeviceFinder::handleGetDeviceDescriptionRensponseHeader(const boost::system::error_code& error)
{
	if (error) {
		handleError(error, __FUNCTION__);
		return;
	}

	// Process the response headers.
	std::istream response_stream(&tcp_response_);
	std::string header;

	while (std::getline(response_stream, header))
		std::cout << header << endl;
	std::cout << endl;

	boost::asio::async_read_until(tcp_socket_, tcp_response_, "\0",
		boost::bind(&DeviceFinder::handleGetDeviceDescriptionRensponseContent, this,
			boost::asio::placeholders::error));
}

void DeviceFinder::handleGetDeviceDescriptionRensponseContent(const boost::system::error_code& error)
{
	if (!error) {
		// Write all of the data that has been read so far.
		content_ << &tcp_response_;

		cout << "-- Device Description --" << endl;
		cout << content_.str();
		cout << "-- End of Device Description --" << endl;

		// Continue reading remaining data until EOF.
		boost::asio::async_read(tcp_socket_, tcp_response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&DeviceFinder::handleGetDeviceDescriptionRensponseContent, this,
				boost::asio::placeholders::error));
	}
	else if (error == boost::asio::error::eof) {
		cout << "End of Content" << endl;

		deviceDescription_.ParseXml(content_.str());
		Cleanup();

		handleStatusError(0, "success");// no error
	}
	else {
		Cleanup();
		handleError(error, __FUNCTION__);
	}
}

void DeviceFinder::handleError(const boost::system::error_code& error, const std::string& msg)
{
	cerr << msg << " : " << error.message() << endl;
	RaiseEvent(error);
}

void DeviceFinder::handleStatusError(unsigned int statusCode, const std::string& msg)
{
	cerr << "error status_code : " << statusCode << endl;
	RaiseEvent((uint32_t)statusCode);
}

void DeviceFinder::Cleanup()
{
	udp_socket_.close();
	tcp_socket_.close();
	tcp_request_.consume(tcp_request_.size());
	tcp_response_.consume(tcp_response_.size());
	content_.clear();
}
