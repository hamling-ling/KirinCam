#include "stdafx.h"
#include "../include/Common.h"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::lexical_cast;
using boost::asio::ip::tcp;

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
