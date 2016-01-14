#pragma once
#include "StreamPresenter.h"
#include "ImageSourceDefs.h"
#include <mutex>
#include <queue>
#include <memory>

class StreamImagePresenter :
	public StreamPresenter
{
public:
	StreamImagePresenter();
	virtual ~StreamImagePresenter();
	virtual void Push(CameraFrame& camFrame);
};

