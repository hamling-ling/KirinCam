#pragma once

#include <string>
#include <boost/asio.hpp>

bool isIpV4Address(const char* url);
void splitUrl(const std::string& url, std::string& server, std::string& port, std::string& path);
void connectSocket(
	boost::asio::io_service& io_service,
	boost::asio::ip::tcp::socket& socket,
	const std::string& server, const std::string &port);
