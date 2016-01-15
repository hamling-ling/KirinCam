#pragma once
#include "StreamPresenter.h"
#include "ImageSourceDefs.h"

class StreamFrameInfoPresenter :
	public StreamPresenter
{
public:
	StreamFrameInfoPresenter();
	virtual ~StreamFrameInfoPresenter();
	virtual void Push(std::shared_ptr<LiveViewPacket> packet);
};

