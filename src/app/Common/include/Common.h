#pragma once

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../src/AsyncWork.h"
#include "../src/DataFlow.h"
#include "../src/Log.h"

/**
 */
bool isIpV4Address(const char* url);

/**
 */
void splitUrl(const std::string& url, std::string& server, std::string& port, std::string& path);

/**
 */
void connectSocket(
	boost::asio::io_service& io_service,
	boost::asio::ip::tcp::socket& socket,
	const std::string& server, const std::string &port);

/**
 */
void Dump(boost::asio::streambuf& buf);

/**
 */
void Dump(const std::vector<uint8_t>& buf, const std::string& filename);

/**
 */
int InvokeCommand(
	boost::asio::ip::tcp::socket& socket,
	const std::string& server,
	const std::string& port,
	const std::string& path,
	const std::string& json_command,
	boost::property_tree::ptree& resultJson);

/**
 */
int InvokeCommand(	const std::string& server,
					const std::string& port,
					const std::string& path,
					const std::string& json_command,
					boost::property_tree::ptree& result);

/**
 */
int InvokeCommand(	const std::string& url,
					const std::string& json_command,
					boost::property_tree::ptree& resultJson);

void PrintPtree(boost::property_tree::ptree &pt);
