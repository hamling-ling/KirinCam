#include "stdafx.h"
#include "CameraStateManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream> // debug

using namespace std;
using boost::property_tree::ptree;

CameraStateManager::CameraStateManager()
	:
		_funcs({	&CameraStateManager::parseAvailableApiList,	// 0
					&CameraStateManager::parseCameraStatust,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseLiveviewStatust,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parse10th,				// 10
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseCameraFunction,
					&CameraStateManager::parseMovieQuality,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseSteadyMode,
					&CameraStateManager::parseViewAngle,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,			// 20
					&CameraStateManager::parseShootMode,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,			// 30
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing,
					&CameraStateManager::parseNothing
					})
{
}

CameraStateManager::~CameraStateManager()
{
}

void CameraStateManager::UpdateState(const std::string& json, std::set<string>& updatedObjectNames)
{
	ptree pt;
	stringstream ss(json);
	read_json(ss, pt);

	ptree& result = pt.get_child("result");
	int count = 0;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, result)
	{
		ptree child = e.second;
		parserFunc_t parser = _funcs[count];
		(this->*parser)(child, updatedObjectNames);

		count++;
		if (_funcs.size() <= count) {
			break;
		}
	}
}

bool CameraStateManager::parseAvailableApiList(ptree& pt, set<string>& updatedObjects)
{
	ptree& names = pt.get_child("names");
	vector<string> list;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, names)
	{
		list.push_back(e.second.data());
	}

	if (_cameraState.UpdateAvailableList(list)) {
		updatedObjects.insert("availableApiList");
	}
	return true;
}

bool CameraStateManager::parseCameraStatust(ptree& pt, set<string>& updatedObjects)
{
	//ptree& status = pt.get_child("cameraStatus");
	boost::optional<string> op = pt.get_optional<string>("cameraStatus");
	if (!op) {
		return false;
	}

	if (_cameraState.UpdateCameraStatus(op.get())) {
		updatedObjects.insert("cameraStatus");
	}
	return true;
}

bool CameraStateManager::parseLiveviewStatust(ptree& pt, set<string>& updatedObjects)
{
	boost::optional<string> op = pt.get_optional<string>("liveviewStatus");
	if (!op) {
		return false;
	}
	if (_cameraState.UpdateCameraStatus(op.get())) {
		updatedObjects.insert("liveviewStatus");
	}
	return true;
}

bool CameraStateManager::parse10th(ptree& pt, set<string>& updatedObjects)
{
	ptree& child = pt.get_child(".");

	vector<CameraStorageInformation> storageInfo;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt)
	{
		//cout << e.second.data() << endl;
		CameraStorageInformation info;
		BOOST_FOREACH(const boost::property_tree::ptree::value_type& e2, e.second) {

			//cout << e2.first << ": " << e2.second.data() << endl;
			if (e2.first == "storageID") {
				info.storageID = e2.second.data();
			}
			else if (e2.first == "recordTarget") {
				info.recordTarget = e2.second.data();
			}
			else if (e2.first == "numberOfRecordableImages") {
				info.numberOfRecordableImages = e2.second.data();
			}
			else if (e2.first == "recordableTime") {
				info.recordableTime = e2.second.data();
			}
			else if (e2.first == "storageDescription") {
				info.storageDescription = e2.second.data();
			}
			storageInfo.push_back(info);
		}
	}
	if (_cameraState.UpdateStorageInformation(storageInfo)) {
		updatedObjects.insert("storageInformation");
	}
	return true;
}

bool CameraStateManager::parseCameraFunction(ptree& pt, set<string>& updatedObjects)
{
	boost::optional<string> val = pt.get_optional<string>("currentCameraFunction");
	if (!val) {
		return false;
	}
	if (_cameraState.UpdateCameraFunction(val.get())) {
		updatedObjects.insert("currentCameraFunction");
	}

	return true;
}

// 13th
bool CameraStateManager::parseMovieQuality(ptree& pt, set<string>& updatedObjects)
{
	boost::optional<string> val = pt.get_optional<string>("currentMovieQuality");
	if (!val) {
		return false;
	}
	if (_cameraState.UpdateMovieQuality(val.get())) {
		updatedObjects.insert("currentMovieQuality");
	}

	return true;
}

// 16th
bool CameraStateManager::parseSteadyMode(ptree& pt, set<string>& updatedObjects)
{
	boost::optional<string> val = pt.get_optional<string>("currentSteadyMode");
	if (!val) {
		return false;
	}
	if (_cameraState.UpdateSteadyMode(val.get())) {
		updatedObjects.insert("currentSteadyMode");
	}

	return true;
}

// 17th
bool CameraStateManager::parseViewAngle(ptree& pt, set<string>& updatedObjects)
{
	boost::optional<string> val = pt.get_optional<string>("currentViewAngle");
	if (!val) {
		return false;
	}
	if (_cameraState.UpdateViewAngle(val.get())) {
		updatedObjects.insert("currentViewAngle");
	}
	return true;
}

// 21st
bool CameraStateManager::parseShootMode(ptree& pt, set<string>& updatedObjects)
{
	boost::optional<string> val = pt.get_optional<string>("currentShootMode");
	if (!val) {
		return false;
	}
	if (_cameraState.UpdateShootMode(val.get())) {
		updatedObjects.insert("currentShootMode");
	}
	return true;
}

// others
bool CameraStateManager::parseNothing(ptree& pt, set<string>& updatedObjects)
{
	return true;
}
