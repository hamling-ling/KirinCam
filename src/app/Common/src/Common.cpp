#include "stdafx.h"
#include "../include/Common.h"
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>

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

void Dump(const vector<uint8_t>& buf, const string& filename)
{
	std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
	if (!ofs) {
		cout << "dump failed" << endl;
		return;
	}

	const char* ptr = (const char*)&buf[0];
	ofs.write(ptr, buf.size());
}


int InvokeCommand(
	tcp::socket& socket,
	const std::string& server,
	const std::string& port,
	const std::string& path,
	const std::string& json_command,
	boost::property_tree::ptree& resultJson)
{
	int retCode = 0;
	boost::system::error_code error = boost::asio::error::host_not_found;
	try
	{
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "POST " << path << " HTTP/1.1\r\n";
		request_stream << "Host: " << "http://" << server << "\r\n";
		request_stream << "Accept: application/json-rpc\r\n";
		request_stream << "Content-Type: application/json-rpc; char-set=utf-8\r\n";
		request_stream << "Content-Length: " << json_command.length() << "\r\n";
		//request_stream << "Connection: close\r\n\r\n";
		request_stream << "\r\n\r\n";
		request_stream << json_command;

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
			retCode = -1;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code " << status_code << "\n";
			retCode = status_code;
			return retCode;
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

		read_json(json_response, resultJson);
	}
	catch (std::exception& e)
	{
		cerr << "Exception: " << e.what() << endl;
		cerr << "json:" << json_command << endl;
		retCode = -1;
	}
	return retCode;
}

int InvokeCommand(	const std::string& server,
					const std::string& port,
					const std::string& path,
					const std::string& json_command,
					boost::property_tree::ptree& resultJson)
{
	int retCode = 0;

	boost::system::error_code error = boost::asio::error::host_not_found;
	boost::asio::io_service io_service;
	tcp::socket socket(io_service);
	try
	{
		connectSocket(io_service, socket, server, port);
	}
	catch (std::exception& e)
	{
		cout << "Exception: " << e.what() << endl;
		cout << "json:" << json_command << endl;
		retCode = -1;
	}
	return InvokeCommand(socket, server, port, path, json_command, resultJson);
}

int InvokeCommand(	const std::string& url,
					const std::string& json_command,
					boost::property_tree::ptree& resultJson)
{
	std::string server;
	std::string port;
	std::string path;

	splitUrl(url, server, port, path);

	return InvokeCommand(server, port, path, json_command, resultJson);
}

void PrintPtree(boost::property_tree::ptree &pt)
{
	stringstream ss;
	write_json(ss, pt);
	cout << ss.str() << endl;
}
