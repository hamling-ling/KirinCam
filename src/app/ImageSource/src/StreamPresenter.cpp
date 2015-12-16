#include "stdafx.h"
#include "StreamPresenter.h"

using namespace std;

StreamPresenter::StreamPresenter()
{
}

StreamPresenter::~StreamPresenter()
{
}

void StreamPresenter::Push(std::shared_ptr<LiveViewPacket> packet)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (packet->GetHeader()->PayloadType() == kPayloadTypeLiveViewImages) {
		_imgQueue.push(packet);

		while (_imgQueue.size() > MAX_QUEUE_SIZE) {
			_imgQueue.pop();
		}
	}
	else if (packet->GetHeader()->PayloadType() == kPayloadTypeLiveViewFrameInformation) {
		_infoQueue.push(packet);

		while (_infoQueue.size() > MAX_QUEUE_SIZE) {
			_infoQueue.pop();
		}
	}
}

void StreamPresenter::GetImage(uint16_t seqNum, CameraFrame& frameData)
{
	lock_guard<recursive_mutex> lock(_mutex);

	frameData.image.clear();
	frameData.info.clear();

	if (_imgQueue.empty()) {
		return;
	}

	shared_ptr<LiveViewPacket> imgPacket;
	while (!_imgQueue.empty()) {
		if (seqNum < _imgQueue.front()->GetHeader()->GetSequenceNumber()) {
			break;
		}
		_imgQueue.pop();
	}

	if (!imgPacket) {
		return;
	}

	uint16_t nextSeqNum = imgPacket->GetHeader()->GetSequenceNumber();
	while (!_infoQueue.empty()) {
		shared_ptr<LiveViewPacket> packet = _infoQueue.front();
		uint16_t tmpSeqNum = packet->GetHeader()->GetSequenceNumber();
		if ( tmpSeqNum == nextSeqNum) {
			if (packet->GetHeader()->PayloadType() == kPayloadTypeLiveViewFrameInformation) {
				CameraFrameInformation info;
				// fill info
				frameData.info.push_back(info);
			}
		}
		else if (nextSeqNum < tmpSeqNum) {
			break;
		}
		_infoQueue.pop();
	}
}

void StreamPresenter::Run()
{
	// do nothing
}
