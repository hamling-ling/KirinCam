#include "StreamDemuxer.h"

using namespace std;

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
				for (auto it : _downStreams) {
					PushPacket(_packet);
					_packet = std::make_shared<LiveViewPacket>();
				}
			}
		}
	}
}

void StreamDemuxer::PushPacket(shared_ptr<LiveViewPacket> packet)
{
	shared_ptr<StreamFlow> downStream;

	switch (packet->GetHeader()->PayloadType()) {
	case kPayloadTypeLiveViewImages:
		_downStreams[0]->Push(packet);
		break;
	case kPayloadTypeLiveViewFrameInformation:
		_downStreams[1]->Push(packet);
		break;
	default:
		break;
	}
}

