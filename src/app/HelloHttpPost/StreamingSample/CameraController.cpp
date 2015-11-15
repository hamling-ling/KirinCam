#include "CameraController.h"
#include <ostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace std;
using boost::lexical_cast;
using boost::asio::ip::tcp;
using boost::property_tree::ptree;

bool isIpV4Address(const char* url)
{
	boost::regex exp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
	return boost::regex_match(url, exp);
}

void splitUrl(const std::string& url, std::string& server, std::string& port, std::string& path) {
	boost::regex reg("http://(\\d+\\.\\d+\\.\\d+\\.\\d+):(\\d+)(/.+)");
	boost::smatch result;
	if (boost::regex_match(url, result, reg)) {
		server = result.str(1);
		port = result.str(2);
		path = result.str(3);
	}
}

void connectSocket(boost::asio::io_service& io_service, tcp::socket& socket, const std::string& server, const std::string &port) {
	boost::system::error_code error = boost::asio::error::host_not_found;

	// connect
	if (isIpV4Address(server.c_str())) {
		boost::asio::ip::address addr = boost::asio::ip::address::from_string(server);
		boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(addr, lexical_cast<int>(port));
		socket.close();
		socket.connect(endpoint);
	}
	else {
		tcp::resolver::query query(server, "http");
		tcp::resolver resolver(io_service);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);
	}
}

CameraController::CameraController(DeviceDescription& deviceDescription)
{
	deviceDescription_ = deviceDescription;
}

CameraController::~CameraController()
{
}

bool CameraController::StartStreaming()
{
	IoService().post(boost::bind(&CameraController::StartStreamingInternal, this));
	Start(NULL);

	return true;
}

void CameraController::StartStreamingInternal()
{
	{
		std::string server;
		std::string port;
		std::string path;

		splitUrl(deviceDescription_.CameraServiceUrl(), server, port, path);
		postRequest(server, port, path);
	}

	{
		std::string server;
		std::string port;
		std::string path;

		splitUrl(liveViewUrl_, server, port, path);
		getRequest(server, port, path);
	}
}

int CameraController::postRequest(const std::string& server, const std::string port, const std::string path)
{
	try
	{
		boost::system::error_code error = boost::asio::error::host_not_found;
		boost::asio::io_service io_service;
		tcp::socket socket(io_service);
		connectSocket(io_service, socket, server, port);

		// prepare request command
		string json_request("{\"method\": \"startLiveview\",\"params\" : [],\"id\" : 1,\"version\" : \"1.0\"}\r\n");

		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "POST " << path << " HTTP/1.1\r\n";
		request_stream << "Host: " << "http://" << server << "\r\n";
		request_stream << "Accept: application/json-rpc\r\n";
		request_stream << "Content-Type: application/json-rpc; char-set=utf-8\r\n";
		request_stream << "Content-Length: " << json_request.length() << "\r\n";
		request_stream << "Connection: close\r\n\r\n";
		request_stream << json_request;

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line. The response streambuf will automatically
		// grow to accommodate the entire line. The growth may be limited by passing
		// a maximum size to the streambuf constructor.
		boost::asio::streambuf response;

		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return 1;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code " << status_code << "\n";
			return 1;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		stringstream json_response;
		if (response.size() > 0) {
			json_response << &response;
		}

		// Read until EOF, writing data to output as we go.
		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			json_response << &response;
		}

		ptree pt;
		read_json(json_response, pt);
		BOOST_FOREACH(const ptree::value_type& child, pt.get_child("result")) {
			liveViewUrl_ = child.second.get_value<std::string>();
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
	return 0;
}


int CameraController::getRequest(const std::string& server, const std::string port, const std::string path)
{
	try
	{
		boost::system::error_code error = boost::asio::error::host_not_found;
		boost::asio::io_service io_service;
		tcp::socket socket(io_service);
		connectSocket(io_service, socket, server, port);

		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << server << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		// Send the request.
		boost::asio::write(socket, request);

		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return 1;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code " << status_code << "\n";
			return 1;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		if (response.size() > 0)
			std::cout << &response;

		boost::asio::streambuf contentbuf;// total buffer
		std::ostream os(&contentbuf, ios::binary);

		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			const char* cp = boost::asio::buffer_cast<const char*>(response.data());
			os.write(cp, response.size());
			response.consume(response.size());

			if (contentbuf.size() > 1024 * 64) {
				Dump(contentbuf);
				break;
			}
		}
		socket.close();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
	return 0;
}

void CameraController::Dump(boost::asio::streambuf& buf)
{
	std::ofstream ofs("out.dat", std::ios::binary | std::ios::trunc);
	if (!ofs) {
		cout << "dump failed" << endl;
		return;
	}

	cout << "content size:" << buf.size() << endl;
	ofs << &buf;
}
