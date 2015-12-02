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
}
