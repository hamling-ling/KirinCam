#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "CameraState.h"


class CameraStateManager
{
public:
	CameraStateManager();
	virtual ~CameraStateManager();
	void UpdateState(const std::string& json);

private:
	CameraState _cameraState;

	bool parseAvailableApiList(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseCameraStatust(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseLiveviewStatust(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parse10th(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseCameraFunction(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseMovieQuality(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseSteadyMode(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseViewAngle(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
	bool parseShootMode(boost::property_tree::ptree& pt, std::vector<string>& updatedObjects);
};

