#pragma once

#include <memory>
#include <boost/signals2.hpp>
#include "CameraRemoteController.h"

class Model;

enum ModelEventType {
	DeviceFinderNotification,
};

class ModelEventArgs {
public:
	ModelEventArgs(ModelEventType arg) : _type(arg){}
	~ModelEventArgs() {}
	const ModelEventType GetEventType() const { return _type; }
private:
	const ModelEventType _type;
};

class DeviceFinderEventArgs : public ModelEventArgs{
public :
	DeviceFinderEventArgs(uint32_t errorCode)
		: ModelEventArgs(DeviceFinderNotification), _errorCode(errorCode) {}
	~DeviceFinderEventArgs() {}
	const uint32_t GetErrorCode() const { return _errorCode; }
private:
	const uint32_t _errorCode;
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

	void devFinderEventHandler(AsyncTask* task, uint32_t errorCode);
};

