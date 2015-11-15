// HelloHttpPost.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using namespace std;
using boost::asio::ip::tcp;
using boost::lexical_cast;

bool isIpAddress(const char* url)
{
	boost::regex exp("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
	return boost::regex_match(url, exp);
}

void connectSocket(boost::asio::io_service& io_service, tcp::socket& socket, const std::string& url, const std::string &port) {
	boost::system::error_code error = boost::asio::error::host_not_found;

	// connect
	if (isIpAddress(url.c_str())) {
		boost::asio::ip::address addr = boost::asio::ip::address::from_string(url);
		boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(addr, lexical_cast<int>(port));
		socket.close();
		socket.connect(endpoint);
	}
	else {
		tcp::resolver::query query(url, "http");
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

void Dump(boost::asio::streambuf& buf)
{
	std::ofstream ofs("out.dat", std::ios::binary | std::ios::trunc);
	if (!ofs) {
		cout << "dump failed" << endl;
		return;
	}

	cout << "content size:" << buf.size() << endl;
	ofs << &buf;
}

int postRequest(char* url, char* port, char* path)
{
	try
	{
		boost::system::error_code error = boost::asio::error::host_not_found;
		boost::asio::io_service io_service;
		tcp::socket socket(io_service);
		connectSocket(io_service, socket, url, port);

		// prepare request command
		string json_request("{\"method\": \"startLiveview\",\"params\" : [],\"id\" : 1,\"version\" : \"1.0\"}\r\n");

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "POST " << path << " HTTP/1.1\r\n";
		request_stream << "Host: " << "http://" << url << "\r\n";
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
		//boost::asio::read(socket, response);

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

		// Read until EOF, writing data to output as we go.
		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			std::cout << &response;
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
	return 0;
}


int getRequest(char* url, char* port, char* path)
{
	try
	{
		boost::system::error_code error = boost::asio::error::host_not_found;
		boost::asio::io_service io_service;
		tcp::socket socket(io_service);
		connectSocket(io_service, socket, url, port);

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << url << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

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
		if (response.size() > 0)
			std::cout << &response;

		boost::asio::streambuf contentbuf;// total buffer
		std::ostream os(&contentbuf, ios::binary);

		// Read until EOF, writing data to output as we go.
		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			//std::cout << &response;
			const char* cp = boost::asio::buffer_cast<const char*>(response.data());
			os.write(cp, response.size());
			response.consume(response.size());

			if (contentbuf.size() > 1024*128) {
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

int main(int argc, char* argv[])
{
	{
		char url[] = "10.0.0.1";
		char port[] = "10000";
		char path[] = "/sony/camera";
		postRequest(url, port, path);
	}

	{
		char url[] = "10.0.0.1";
		char port[] = "60152";
		char path[] = "/liveview.JPG?%211234%21http%2dget%3a%2a%3aimage%2fjpeg%3a%2a%21%21%21%21%21";

		getRequest(url, port, path);
	}
	return 0;
}

