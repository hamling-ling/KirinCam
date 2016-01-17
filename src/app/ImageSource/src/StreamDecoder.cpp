#include "stdafx.h"
#include "StreamDecoder.h"
#include "Common.h"
#include <functional>

using namespace std;

StreamDecoder::StreamDecoder()
{
	DataFlow<decoderFlowData_t>::DataFlowConsumeFunc func = bind(
		&StreamDecoder::Run,
		this, std::placeholders::_1,
		std::placeholders::_2);

	_flow = make_shared<DataFlow<decoderFlowData_t> >(MAX_DECODER_QUEUE_SIZE, func);
#ifdef DUMPJPEG
	dumped = false;
#endif
}

StreamDecoder::~StreamDecoder()
{

}

void StreamDecoder::Push(std::shared_ptr<LiveViewPacket> packet)
{
	_flow->Enqueue(packet);
}

void StreamDecoder::Run(std::atomic<bool>& canceled, decoderFlowData_t packet)
{
#ifdef DUMPJPEG
	if (!dumped) {
		// debug
		const VariableSizeData* data = packet->GetImage();
		const vector<uint8_t> vec = data->Vec();
		Dump(vec, "out.jpg");
		dumped = true;
	}
#endif
	// push
	CameraFrame camFrame;

	// decode
	const VariableSizeData* data = packet->GetImage();
	if (data == NULL || data->Size() == UNDEFINED_SIZE) {
		return;
	}
	camFrame.image = cv::imdecode(cv::Mat(data->Vec()), 1);
	camFrame.sequenceNumber = packet->GetHeader()->GetSequenceNumber();
	camFrame.timeStamp = packet->GetHeader()->GetTimeStamp();

	for (auto it : _downStreams) {
		it->Push(camFrame);
	}
}
