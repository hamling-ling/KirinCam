#pragma once

#include <string>

class DeviceDescription
{
public:
	DeviceDescription();
	virtual ~DeviceDescription();
	bool ParseXml(std::string xml);
	const std::string& FriendlyName();
	const std::string& LiveViewUrl();
	const std::string& CameraServiceUrl();
private:
	std::string _friendlyName;
	std::string _manufacturer;
	std::string _manufacturerUrl;
	std::string _modelName;
	std::string _modelUrl;
	std::string _udn;
	std::string _liveViewUrl;
	std::string _cameraServiceUrl;
};

