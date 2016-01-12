#pragma once
#include <queue>
#include <memory>
#include "StreamPresenter.h"
#include "ImageSourceDefs.h"

class StreamFrameInfoPresenter :
	public StreamPresenter
{
public:
	StreamFrameInfoPresenter();
	virtual ~StreamFrameInfoPresenter();
	void Push(std::shared_ptr<LiveViewPacket> packet);
};

