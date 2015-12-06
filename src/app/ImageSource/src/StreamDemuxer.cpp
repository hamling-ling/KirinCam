#include "stdafx.h"
#include "StreamDemuxer.h"

StreamDemuxer::StreamDemuxer()
{
	_packet = std::make_shared<LiveViewPacket>();
}

StreamDemuxer::~StreamDemuxer()
{
}

void StreamDemuxer::Push(boost::asio::streambuf& stream)
{
	while (stream.size()) {
		if (_packet->Fill(stream)) {
			if (_packet->IsFull()) {
				_downStream->Push(_packet);
				_packet = std::make_shared<LiveViewPacket>();
			}
		}
	}
}

void StreamDemuxer::Run()
{
	// do nothing
}
