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

void StreamPresenter::GetImage(uint16_t seqNum, CameraFrame& camFrame)
{
	lock_guard<recursive_mutex> lock(_mutex);

	camFrame.image.clear();
	camFrame.info.clear();

	if (_imgQueue.empty()) {
		return;
	}

	shared_ptr<LiveViewPacket> imgPacket;
	while (!_imgQueue.empty()) {
		if (seqNum < _imgQueue.front()->GetHeader()->GetSequenceNumber()) {
			imgPacket = _imgQueue.front();
			_imgQueue.pop();
			break;
		}
		_imgQueue.pop();
	}

	if (!imgPacket) {
		return;
	}

	// copy image
	const VariableSizeData* data = imgPacket->GetImage();
	if (data == NULL || data->Size() == UNDEFINED_SIZE) {
		return;
	}
	camFrame.image.resize(data->Size());
	memcpy_s(&camFrame.image[0], camFrame.image.size(), data->Data(), data->Size());

	// make CameraFrameInformation
	uint16_t nextSeqNum = imgPacket->GetHeader()->GetSequenceNumber();
	while (!_infoQueue.empty()) {
		shared_ptr<LiveViewPacket> packet = _infoQueue.front();
		uint16_t tmpSeqNum = packet->GetHeader()->GetSequenceNumber();
		if ( tmpSeqNum == nextSeqNum) {
			if (packet->GetHeader()->PayloadType() == kPayloadTypeLiveViewFrameInformation) {
				// fill info
				shared_ptr<FramePayload> framePayload = dynamic_pointer_cast<FramePayload>(packet->GetPayload());
				const FrameData& frameData = framePayload->GetFrameData();
				const list <shared_ptr<FrameInformation> >& frameInfoList = frameData.Frames();
				for (auto v : frameInfoList) {
					CameraFrameInformation info;

					auto topLeft = v->GetTopLeft();
					info.rect.topLeft.x = topLeft.X();
					info.rect.topLeft.y = topLeft.Y();

					auto bottomRight = v->GetBottomRight();
					info.rect.bottomRight.x = bottomRight.X();
					info.rect.bottomRight.y = bottomRight.Y();

					info.category = v->GetCategory().Value();
					info.status = v->GetStatus().Value();

					camFrame.info.push_back(info);
				}

				_infoQueue.pop();
				break;
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
