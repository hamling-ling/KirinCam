#pragma once
#include "StreamPresenter.h"
#include "ImageSourceDefs.h"

class StreamImagePresenter :
	public StreamPresenter
{
public:
	StreamImagePresenter();
	virtual ~StreamImagePresenter();
	virtual void Push(CameraFrame& camFrame);
};

