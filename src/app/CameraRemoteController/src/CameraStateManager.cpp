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

void CameraStateManager::UpdateState(const string& json, updatedObjects_t& updates)
{
	ptree pt;
	stringstream ss(json);
	read_json(ss, pt);

	return UpdateState(pt, updates);
}
 
void CameraStateManager::UpdateState(ptree& pt, updatedObjects_t& updates)
{
	ptree& result = pt.get_child(kResult);
	unsigned int count = 0;

	lock_guard<recursive_mutex> lock(_mutex);

	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, result)
	{
		ptree child = e.second;

		parserFunc_t parser = _funcs[count];
		try { (this->*parser)(child, updates); }
		catch (boost::exception& e) {}

		count++;
		if (_funcs.size() <= count) {
			break;
		}
	}
}

bool CameraStateManager::parseAvailableApiList(ptree& pt, updatedObjects_t& updates)
{
	ptree& names = pt.get_child(kNames);
	vector<string> list;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type& e, names)
	{
		list.push_back(e.second.data());
	}

	if (_cameraState.UpdateAvailableList(list)) {
		updates[0] = true;
	}

	return true;
}

bool CameraStateManager::parseCameraStatust(ptree& pt, updatedObjects_t& updates)
{
	return parseSimpleProperty(
		pt,
		1,
		updates,
		kCameraStatus,
		&CameraState::UpdateCameraStatus
		);
}

bool CameraStateManager::parseLiveviewStatust(ptree& pt, updatedObjects_t& updates)
{
	return parseSimpleProperty(
		pt,
		3,
		updates,
		kLiveviewStatus,
		&CameraState::UpdateLiveviewStatus
		);
}

bool CameraStateManager::parse10th(ptree& pt, updatedObjects_t& updates)
{
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
		updates[10] = true;
	}
	return true;
}

bool CameraStateManager::parseCameraFunction(ptree& pt, updatedObjects_t& updates)
{
	return parsePropWithCandidates(
		pt,
		12,
		updates,
		kCurrentCameraFunction,
		kCurrentCameraFunctionCandidates,
		&CameraState::UpdateCameraFunction,
		&CameraState::HasCameraFunctionCandidate,
		&CameraState::SetCameraFunctionCandidate
		);
}

bool CameraStateManager::parseMovieQuality(ptree& pt, updatedObjects_t& updates)
{
	return parsePropWithCandidates(
		pt,
		13,
		updates,
		kCurrentMoviewQuality,
		kMoviewQualityCandidates,
		&CameraState::UpdateMovieQuality,
		&CameraState::HasMoviewQualityCandidates,
		&CameraState::SetMoviewQualityCandidates
		);
}

bool CameraStateManager::parseSteadyMode(ptree& pt, updatedObjects_t& updates)
{
	return parsePropWithCandidates(
		pt,
		16,
		updates,
		kCurrentSteadyMode,
		kSteadyModeCandidates,
		&CameraState::UpdateSteadyMode,
		&CameraState::HasSteadyModeCandidates,
		&CameraState::SetSteadyModeCandidates
		);
}

bool CameraStateManager::parseViewAngle(ptree& pt, updatedObjects_t& updates)
{
	return parsePropWithCandidates(
		pt,
		17,
		updates,
		kCurrentViewAngle,
		kViewAngleCandidates,
		&CameraState::UpdateViewAngle,
		&CameraState::HasViewAngleCandidates,
		&CameraState::SetViewAngleCandidates
		);
}

bool CameraStateManager::parseShootMode(ptree& pt, updatedObjects_t& updates)
{
	return parsePropWithCandidates(
		pt,
		21,
		updates,
		kCurrentShootMode,
		kShootModeCandidates,
		&CameraState::UpdateShootMode,
		&CameraState::HasShootModeCandidates,
		&CameraState::SetShootModeCandidates
		);
}

bool CameraStateManager::parseNothing(ptree& pt, updatedObjects_t& updates)
{
	return true;
}

bool CameraStateManager::parseSimpleProperty(
	boost::property_tree::ptree& pt,
	int idx,
	updatedObjects_t& updates,
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
		updates[idx] = true;
	}

	return true;
}

bool CameraStateManager::parsePropWithCandidates(
	boost::property_tree::ptree& pt,
	int idx,
	updatedObjects_t& updates,
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
		updates[idx] = true;
	}
	return true;
}
