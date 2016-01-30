#include "StreamFlow.h"
#include "Common.h"

using namespace std;

StreamFlow::StreamFlow()
{
}

StreamFlow::~StreamFlow()
{
	Stop();
}

void StreamFlow::Connect(shared_ptr<StreamFlow> flow)
{
	_downStreams.push_back(flow);
}

void StreamFlow::Push(boost::asio::streambuf& stream)
{
}

void StreamFlow::Push(std::shared_ptr<LiveViewPacket> packet)
{
}

void StreamFlow::Push(std::vector<uint8_t> data)
{

}

void StreamFlow::Push(CameraFrame& camFrame)
{

}

void StreamFlow::Start()
{
}

void StreamFlow::Stop()
{
}
