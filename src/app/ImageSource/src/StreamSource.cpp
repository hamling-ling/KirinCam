#include "stdafx.h"
#include "StreamSource.h"
#include <sstream>
#include <iostream>//debug
#include <functional>
#include "Common.h"

using namespace std;
using boost::asio::ip::tcp;

//#define DUMPSTREAM

StreamSource::StreamSource(const std::string& url)
	: _url(url)
{
	splitUrl(url, _server, _port, _path);
}

StreamSource::~StreamSource()
{
}

void StreamSource::Start()
{
	AsyncWorkArg arg;

	AsyncWorkFunc workFunc = std::bind(
		&StreamSource::Run,
		this,
		std::placeholders::_1,
		std::placeholders::_2);

	_work->Start(workFunc, arg);
}

void StreamSource::Stop()
{
	StreamFlow::Stop();
}

void StreamSource::Run(atomic<bool>& canceled, AsyncWorkArg& arg)
{
	try {
		downloadStream();
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
}

void StreamSource::downloadStream() throw(std::exception)
{
	boost::system::error_code error = boost::asio::error::host_not_found;
	boost::asio::io_service io_service;
	tcp::socket socket(io_service);
	connectSocket(io_service, socket, _server, _port);

	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << makeGetRequest(_server, _path);

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
		throw std::runtime_error("Invalid response");
	}
	if (status_code != 200)
	{
		std::cout << "Response returned with status code " << status_code << "\n";
		throw std::runtime_error("200");
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

	while (
		/* check cancelation  && */
		boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)
		) {
		const char* cp = boost::asio::buffer_cast<const char*>(response.data());
		os.write(cp, response.size());
		response.consume(response.size());

#ifdef DUMPSTREAM
		if (contentbuf.size() > 1024 * 1024 * 16) {
			Dump(contentbuf);
			break;
		}
#else
		if (contentbuf.size() > 1024) {
			for (auto it : _downStreams) {
				it->Push(contentbuf);
			}
		}
#endif
	}
	socket.close();
}

std::string StreamSource::makeGetRequest(const std::string& server, const std::string& path)
{
	stringstream ss;
	ss << "GET " << path << " HTTP/1.0\r\n";
	ss << "Host: " << server << "\r\n";
	ss << "Accept: */*\r\n";
	ss << "Connection: close\r\n\r\n";
	return ss.str();
}
