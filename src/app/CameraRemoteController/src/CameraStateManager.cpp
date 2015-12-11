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

	ptree& result = pt.get_child(kResult);
	unsigned int count = 0;

	lock_guard<recursive_mutex> lock(_mutex);

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
	ptree& names = pt.get_child(kNames);
	vector<string> list;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, names)
	{
		list.push_back(e.second.data());
	}

	if (_cameraState.UpdateAvailableList(list)) {
		updatedObjects.insert(kAvailableApiList);
	}
	return true;
}

bool CameraStateManager::parseCameraStatust(ptree& pt, set<string>& updatedObjects)
{
	return parseSimpleProperty(
		pt,
		updatedObjects,
		kCameraStatus,
		&CameraState::UpdateCameraStatus
		);
	/*//ptree& status = pt.get_child("cameraStatus");
	boost::optional<string> op = pt.get_optional<string>(kCameraStatus);
	if (!op) {
		return false;
	}

	if (_cameraState.UpdateCameraStatus(op.get())) {
		updatedObjects.insert(kCameraStatus);
	}
	return true;*/
}

bool CameraStateManager::parseLiveviewStatust(ptree& pt, set<string>& updatedObjects)
{
	return parseSimpleProperty(
		pt,
		updatedObjects,
		kLiveviewStatus,
		&CameraState::UpdateLiveviewStatus
		);
	/*boost::optional<string> op = pt.get_optional<string>(kLiveviewStatus);
	if (!op) {
		return false;
	}

	if (_cameraState.UpdateLiveviewStatus(op.get())) {
		updatedObjects.insert(kLiveviewStatus);
	}
	return true;*/
}

bool CameraStateManager::parse10th(ptree& pt, set<string>& updatedObjects)
{
	ptree& child = pt.get_child(".");

	vector<CameraStorageInformation> storageInfo;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, pt)
	{
		CameraStorageInformation info;
		BOOST_FOREACH(const boost::property_tree::ptree::value_type& e2, e.second) {

			if (e2.first.compare(kStorageId) == 0) {
				info.storageID = e2.second.data();
			}
			else if (e2.first.compare(kRecordTarget) == 0) {
				info.recordTarget = e2.second.data();
			}
			else if (e2.first.compare(kNumberOfRecordableImages) == 0) {
				info.numberOfRecordableImages = e2.second.data();
			}
			else if (e2.first.compare(kRecordableTime) == 0) {
				info.recordableTime = e2.second.data();
			}
			else if (e2.first.compare(kStorageDescription) == 0) {
				info.storageDescription = e2.second.data();
			}
		}
		storageInfo.push_back(info);
	}
	if (_cameraState.UpdateStorageInformation(storageInfo)) {
		updatedObjects.insert(kStorageInformation);
	}
	return true;
}

bool CameraStateManager::parseCameraFunction(ptree& pt, set<string>& updatedObjects)
{
	return parsePropWithCandidates(
		pt,
		updatedObjects,
		kCurrentCameraFunction,
		kCurrentCameraFunctionCandidates,
		&CameraState::UpdateCameraFunction,
		&CameraState::HasCameraFunctionCandidate,
		&CameraState::SetCameraFunctionCandidate
		);
}

bool CameraStateManager::parseMovieQuality(ptree& pt, set<string>& updatedObjects)
{
	return parsePropWithCandidates(
		pt,
		updatedObjects,
		kCurrentMoviewQuality,
		kMoviewQualityCandidates,
		&CameraState::UpdateMovieQuality,
		&CameraState::HasMoviewQualityCandidates,
		&CameraState::SetMoviewQualityCandidates
		);
}

bool CameraStateManager::parseSteadyMode(ptree& pt, set<string>& updatedObjects)
{
	return parsePropWithCandidates(
		pt,
		updatedObjects,
		kCurrentSteadyMode,
		kSteadyModeCandidates,
		&CameraState::UpdateSteadyMode,
		&CameraState::HasSteadyModeCandidates,
		&CameraState::SetSteadyModeCandidates
		);
}

bool CameraStateManager::parseViewAngle(ptree& pt, set<string>& updatedObjects)
{
	return parsePropWithCandidates(
		pt,
		updatedObjects,
		kCurrentViewAngle,
		kViewAngleCandidates,
		&CameraState::UpdateViewAngle,
		&CameraState::HasViewAngleCandidates,
		&CameraState::SetViewAngleCandidates
		);
}

bool CameraStateManager::parseShootMode(ptree& pt, set<string>& updatedObjects)
{
	return parsePropWithCandidates(
		pt,
		updatedObjects,
		kCurrentShootMode,
		kShootModeCandidates,
		&CameraState::UpdateShootMode,
		&CameraState::HasShootModeCandidates,
		&CameraState::SetShootModeCandidates
		);
}

bool CameraStateManager::parseNothing(ptree& pt, set<string>& updatedObjects)
{
	return true;
}

bool CameraStateManager::parseSimpleProperty(
	boost::property_tree::ptree& pt,
	std::set<std::string>& updatedObjects,
	const char* propName,
	bool (CameraState::*updator)(const std::string&)
	)
{
	boost::optional<string> op = pt.get_optional<string>(propName);
	if (!op) {
		return false;
	}

	CameraState* p = &_cameraState;
	if ((p->*updator)(op.get())) {
		updatedObjects.insert(propName);
	}

	return true;
}

bool CameraStateManager::parsePropWithCandidates(
	boost::property_tree::ptree& pt,
	std::set<std::string>& updatedObjects,
	const char* propName,
	const char* candName,
	bool (CameraState::*updator)(const string&),
	bool(CameraState::*candexist)() const,
	void (CameraState::*candsetter)(const set<string>&)
	)
{
	boost::optional<string> val = pt.get_optional<string>(propName);
	if (!val) {
		return false;
	}

	CameraState* p = &_cameraState;
	if (!(p->*candexist)()) {
		set<string> candidates;
		BOOST_FOREACH(const ptree::value_type& e, pt.get_child(candName))
		{
			cout << e.second.data() << endl;
			candidates.insert(e.second.data());
		}
		(p->*candsetter)(candidates);
	}

	if ((p->*updator)(val.get())) {
		updatedObjects.insert(kCurrentShootMode);
	}
	return true;
}
