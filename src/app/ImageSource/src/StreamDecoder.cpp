#include "stdafx.h"
#include "StreamDecoder.h"
#include "Common.h"

using namespace std;

StreamDecoder::StreamDecoder()
{
	DataFlow<flowData_t>::DataFlowConsumeFunc func = bind(
		&StreamDecoder::Run,
		this, std::placeholders::_1,
		std::placeholders::_2);

	_flow = make_shared<DataFlow<flowData_t>>(32, func);
}

StreamDecoder::~StreamDecoder()
{

}

void StreamDecoder::Run(std::atomic<bool>& canceled, AsyncWorkArg& arg)
{
	// decode
	

	// push
	CameraFrame camFrame;
	for (auto it : _downStreams) {
		it->Push(camFrame);
	}
}
