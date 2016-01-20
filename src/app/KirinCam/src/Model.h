#pragma once

#include <memory>
#include <boost/signals2.hpp>
#include "CameraRemoteController.h"

class Model;
class ModelEventArgs {
public:
	ModelEventArgs() {}
	~ModelEventArgs() {}
};
typedef boost::signals2::signal<void(const Model&, const ModelEventArgs&)> ModelEvent_t;

class Model
{
public:
	ModelEvent_t SearchComplete;

	static Model& Instance();
	bool Initialize();
	void Reset();
	std::shared_ptr<CameraController> GetCameraController();
private:
	Model();
	std::shared_ptr<CameraController> m_cameraController;
	std::shared_ptr<DeviceFinder> m_deviceFinder;

	void devFinderEventHandler(AsyncTask* task);
};

