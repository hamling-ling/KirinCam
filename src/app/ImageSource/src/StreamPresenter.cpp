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
	_queue.push(packet);

	while (_queue.size() > MAX_QUEUE_SIZE) {
		_queue.pop();
	}
}

void StreamPresenter::GetImage(vector<uint8_t>& buf)
{
	lock_guard<recursive_mutex> lock(_mutex);

	buf.clear();
	if (_queue.empty()) {
		return;
	}

	shared_ptr<LiveViewPacket> packet = _queue.front();
	_queue.pop();

	const VariableSizeData* data = packet->GetImage();
	if (data == NULL || data->Size() == UNDEFINED_SIZE) {
		return;
	}

	buf.resize(data->Size());

	memcpy_s(&buf[0], buf.size(), data->Data(), data->Size());
}

void StreamPresenter::Run()
{
	// do nothing
}
