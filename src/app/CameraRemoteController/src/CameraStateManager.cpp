#include "stdafx.h"
#include "CameraStateManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream> // debug

using namespace std;
using boost::property_tree::ptree;

CameraStateManager::CameraStateManager()
{
}


CameraStateManager::~CameraStateManager()
{
}

bool CameraStateManager::parseAvailableApiList(ptree& pt, vector<string>& updatedObjects)
{
	ptree& names = pt.get_child("names");
	vector<string> list;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, names)
	{
		list.push_back(e.second.data());
	}

	if (_cameraState.UpdateAvailableList(list)) {
		updatedObjects.push_back("availableApiList");
	}
	return true;
}

bool CameraStateManager::parseCameraStatust(ptree& pt, vector<string>& updatedObjects)
{
	//ptree& status = pt.get_child("cameraStatus");
	boost::optional<string> op = pt.get_optional<string>("cameraStatus");
	if (!op) {
		return false;
	}

	if (_cameraState.UpdateCameraStatus(op.get())) {
		updatedObjects.push_back("cameraStatus");
	}
	return true;
}

bool CameraStateManager::parseLiveviewStatust(ptree& pt, vector<string>& updatedObjects)
{
	boost::optional<string> op = pt.get_optional<string>("liveviewStatus");
	if (!op) {
		return false;
	}
	if (_cameraState.UpdateCameraStatus(op.get())) {
		updatedObjects.push_back("liveviewStatus");
	}
	return true;
}

bool CameraStateManager::parse10th(ptree& pt, vector<string>& updatedObjects)
{
	ptree& child = pt.get_child(".");
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt)
	{
		cout << e.second.data() << endl;
		BOOST_FOREACH(const boost::property_tree::ptree::value_type& e2, e.second) {
			cout << e2.first << ": " << e2.second.data() << endl;
		}
	}
}

bool CameraStateManager::parseCameraFunction(ptree& pt, vector<string>& updatedObjects)
{
	boost::optional<string> camFunc = pt.get_optional<string>("currentCameraFunction");
	cout << camFunc.get() << endl;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt.get_child("cameraFunctionCandidates"))
	{
		cout << e.second.data() << endl;
	}
}

// 13th
bool CameraStateManager::parseMovieQuality(ptree& pt, vector<string>& updatedObjects)
{
	boost::optional<string> movieQuality = pt.get_optional<string>("currentMovieQuality");
	cout << movieQuality.get() << endl;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt.get_child("movieQualityCandidates"))
	{
		cout << e.second.data() << endl;
	}
}

// 16th
bool CameraStateManager::parseSteadyMode(ptree& pt, vector<string>& updatedObjects)
{
	boost::optional<string> mode = pt.get_optional<string>("currentSteadyMode");
	cout << mode.get() << endl;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt.get_child("steadyModeCandidates"))
	{
		cout << e.second.data() << endl;
	}
}

// 17th
bool CameraStateManager::parseViewAngle(ptree& pt, vector<string>& updatedObjects)
{
	boost::optional<string> angle = pt.get_optional<string>("currentViewAngle");
	cout << angle.get() << endl;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt.get_child("viewAngleCandidates"))
	{
		cout << e.second.data() << endl;
	}
}

// 21st
bool CameraStateManager::parseShootMode(ptree& pt, vector<string>& updatedObjects)
{
	boost::optional<string> mode = pt.get_optional<string>("currentShootMode");
	cout << mode.get() << endl;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt.get_child("shootModeCandidates"))
	{
		cout << e.second.data() << endl;
	}

}
