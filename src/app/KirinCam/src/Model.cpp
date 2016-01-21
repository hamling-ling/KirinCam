#include "Model.h"

using namespace std;

Model& Model::Instance()
{
	static Model instance;
	return instance;
}

Model::Model()
{
	m_deviceFinder = make_shared<DeviceFinder>();
}

bool Model::Initialize()
{
	function<void(AsyncTask*, uint32_t)> handler = bind(&Model::devFinderEventHandler,
		this, std::placeholders::_1, std::placeholders::_2);
	m_deviceFinder->Start(handler);
	return true;
}

void Model::Reset()
{
	m_cameraController.reset();
}

shared_ptr<CameraController> Model::GetCameraController()
{
	return m_cameraController;
}

void Model::devFinderEventHandler(AsyncTask* task, uint32_t errorCode)
{
	DeviceFinderEventArgs arg(errorCode);
	SearchComplete(*this, arg);
}
