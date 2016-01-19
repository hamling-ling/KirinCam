#include "StreamFrameInfoPresenter.h"

using namespace std;

StreamFrameInfoPresenter::StreamFrameInfoPresenter()
{
}


StreamFrameInfoPresenter::~StreamFrameInfoPresenter()
{
}

void StreamFrameInfoPresenter::Push(shared_ptr<LiveViewPacket> packet)
{
	if (packet->GetHeader()->PayloadType() != kPayloadTypeLiveViewFrameInformation) {
		return;
	}

	// fill info
	CameraFrame camFrame;
	camFrame.sequenceNumber = packet->GetHeader()->GetSequenceNumber();
	camFrame.timeStamp = packet->GetHeader()->GetTimeStamp();

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

	lock_guard<recursive_mutex> lock(_mutex);
	_queue.push(camFrame);

	while (_queue.size() > kMaxQueueSize) {
		_queue.pop();
	}
}
