#pragma once

#include "StreamFlow.h"

class StreamSource :
	public StreamFlow
{
public:
	StreamSource(const std::string& url);
	virtual ~StreamSource();
	virtual void Start();
	virtual void Stop();
protected:
	virtual void Run();
private:
	std::string _url;
	std::string _server;
	std::string _path;
	std::string _port;

	std::string makeGetRequest(const std::string& server, const std::string& path);
	void downloadStream();
};

