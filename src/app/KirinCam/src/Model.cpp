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

void Model::devFinderEventHandler(AsyncTask* task)
{

}
