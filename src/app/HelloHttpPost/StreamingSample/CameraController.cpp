#include "CameraController.h"
#include <ostream>
#include <string>

using namespace std;
using namespace boost;

CameraController::CameraController(DeviceDescription& deviceDescription)
	:
	tcp_socket_(_ioService),
	liveview_socket_(_ioService)
{
	deviceDescription_ = deviceDescription;
}


CameraController::~CameraController()
{
}

bool CameraController::StartStreaming()
{
	ostream request_stream(&tcp_request_);
	const string& url = deviceDescription_.CameraServiceUrl();
	size_t last_slash = url.rfind('/');
	size_t colon = url.rfind(':');
	string server = url.substr(7, colon - 7);
	string port = url.substr(colon + 1, last_slash - colon - 1);
	string path = url.substr(last_slash);
	string json_request("{\"method\": \"startLiveview\",\"params\" : [],\"id\" : 1,\"version\" : \"1.0\"}\r\n");
	request_stream << "POST " << path << "/camera HTTP/1.1\r\n";
	request_stream << "Accept: application/json-rpc\r\n";
	request_stream << "Content-Length: " << json_request.length() << "\r\n";
	request_stream << "Content-Type: application/json-rpc\r\n";
	request_stream << "Host:" << url << "\r\n";
	request_stream << "\r\n";
	request_stream << json_request;
	asio::ip::tcp::resolver::query query(server, port);
	content_.clear();
	content_.str("");

	boost::asio::ip::address addr = boost::asio::ip::address::from_string(server);
	boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(addr, lexical_cast<unsigned short>(port.c_str()));
	tcp_socket_.connect(endpoint);

	boost::asio::async_write(tcp_socket_, tcp_request_,
		boost::bind(&CameraController::handleWriteRequest, this,
		boost::asio::placeholders::error));

	Start(NULL);
	return true;
}

void CameraController::handleWriteRequest(const boost::system::error_code& err) {
	if (err) {
		std::cout << "Error: " << err.message() << "\n";
		Close();
		return;
	}
	boost::asio::async_read_until(tcp_socket_, tcp_response_, "\0",
		boost::bind(&CameraController::handleReadStatusLine, this,
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CameraController::handleReadStatusLine(const boost::system::error_code& err, const size_t bytes_transferred) {
	if (err) {
		std::cout << "Error: " << err.message() << "\n";
		Close();
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
		std::cout << "Invalid response\n";
		return;
	}
	if (status_code != 200) {
		std::cout << "Response returned with status code ";
		std::cout << status_code << "\n";
		return;
	}

	// Get streaming request
	getLiveImage();
}

void CameraController::getLiveImage() {
	const string& url = deviceDescription_.LiveViewUrl();

	size_t last_slash = url.rfind('/');
	size_t colon = url.rfind(':');
	string server = url.substr(7, colon - 7);
	string port = url.substr(colon + 1, last_slash - colon - 1);
	string path = url.substr(last_slash);

	stringstream ss;
	ss << "GET " << path << " HTTP/1.1\r\n";
	ss << "Accept: image/jpeg\r\n";
	ss << "Host: " << server << "\r\n";
	ss << "\r\n";

	cout << ss.str();
	ostream request_stream(&tcp_request_);
	request_stream << ss.str();

	boost::asio::ip::address addr = boost::asio::ip::address::from_string(server);
	boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(addr, lexical_cast<unsigned short>(port));
	liveview_socket_.connect(endpoint);

	boost::asio::async_write(
		liveview_socket_,
		liveview_request_,
		boost::bind(&CameraController::handleGetLiveImageResponse,
		this,
		boost::asio::placeholders::error));
}


void CameraController::handleGetLiveImageResponse(const boost::system::error_code& error)
{
	if (error) {
		cout << "handleGetLiveImageResponse error: " << error.message() << endl;
		Close();
		return;
	}

	boost::asio::async_read_until(liveview_socket_, liveview_response_, "\r\n\r\n",
		boost::bind(&CameraController::handleGetLiveImageStatusCode, this,
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CameraController::handleGetLiveImageStatusCode(const boost::system::error_code& error, const size_t bytes_transferred)
{
	if (error) {
		cout << "handleGetLiveImageStatusCode error: " << error.message() << endl;
		Close();
		return;
	}

	// Check that response is OK.
	std::istream response_stream(&liveview_response_);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
		std::cout << "Invalid response" << endl;
		Close();
		return;
	}
	if (status_code != 200) {
		std::cout << "Response returned with status code ";
		std::cout << status_code << endl;
		Close();
		return;
	}
	// Read the response headers, which are terminated by a blank line.
	boost::asio::async_read_until(liveview_socket_, liveview_response_, "\r\n\r\n",
		boost::bind(&CameraController::handleGetLiveImageHeader, this,
		boost::asio::placeholders::error));
}

void CameraController::handleGetLiveImageHeader(const boost::system::error_code& error)
{
	if (error) {
		cout << "handleGetLiveImageHeader error: " << error.message() << endl;
		return;
	}

	// Process the response headers.
	std::istream response_stream(&liveview_response_);
	std::string header;

	while (std::getline(response_stream, header))
		std::cout << header << endl;
	std::cout << endl;

	boost::asio::async_read_until(liveview_socket_, liveview_response_, "\0",
		boost::bind(&CameraController::handleGetLiveViewContent, this,
		boost::asio::placeholders::error));
}

void CameraController::handleGetLiveViewContent(const boost::system::error_code& error)
{
	if (!error) {
		// Write all of the data that has been read so far.
		content_ << &liveview_response_;

		// Continue reading remaining data until EOF.
		boost::asio::async_read(tcp_socket_, liveview_response_,
			boost::asio::transfer_at_least(1),
			boost::bind(&CameraController::handleGetLiveViewContent, this,
			boost::asio::placeholders::error));
	}
	else if (error == boost::asio::error::eof) {
		std::cout << "End of Content" << endl;
	}
	else {
		std::cout << "Error: " << error << endl;
		Close();
	}
}

void CameraController::Close()
{
	tcp_socket_.close();
}