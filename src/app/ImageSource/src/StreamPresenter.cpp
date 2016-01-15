#include "stdafx.h"
#include "StreamPresenter.h"

using namespace std;

StreamPresenter::StreamPresenter()
{
}

StreamPresenter::~StreamPresenter()
{
}

bool StreamPresenter::GetCameraFrame(uint16_t seqNum, CameraFrame& camFrame)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_queue.empty()) {
		return false;
	}

	while (!_queue.empty()) {
		if (seqNum < _queue.front().sequenceNumber) {
			camFrame = _queue.front();
			_queue.pop();
			break;
		}
		_queue.pop();
	}
	return true;
#if 0
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
#endif
}
