#pragma once

#include <string>
#include <array>
#include <set>
#include <mutex>
#include <boost/property_tree/ptree.hpp>
#include <boost/function.hpp>
#include "CameraState.h"

static const char* kResult = "result";
static const char* kNames = "names";
static const char* kAvailableApiList = "availableApiList";
static const char* kCameraStatus = "cameraStatus";
static const char* kLiveviewStatus = "liveviewStatus";
static const char* kStorageId = "storageID";
static const char* kRecordTarget = "recordTarget";
static const char* kNumberOfRecordableImages = "numberOfRecordableImages";
static const char* kRecordableTime = "recordableTime";
static const char* kStorageDescription = "storageDescription";
static const char* kStorageInformation = "storageInformation";
static const char* kCurrentCameraFunction = "currentCameraFunction";
static const char* kCurrentCameraFunctionCandidates = "cameraFunctionCandidates";
static const char* kCurrentMoviewQuality = "currentMovieQuality";
static const char* kMoviewQualityCandidates = "movieQualityCandidates";
static const char* kCurrentSteadyMode = "currentSteadyMode";
static const char* kSteadyModeCandidates = "steadyModeCandidates";
static const char* kCurrentViewAngle = "currentViewAngle";
static const char* kViewAngleCandidates = "viewAngleCandidates";
static const char* kCurrentShootMode = "currentShootMode";
static const char* kShootModeCandidates = "shootModeCandidates";

class CameraStateManager
{
public:
	CameraStateManager();
	virtual ~CameraStateManager();
	void UpdateState(const std::string& json, std::set<std::string>& updatedObjectNames);
	void UpdateState(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjectNames);

private:
	typedef bool (CameraStateManager::*parserFunc_t)(boost::property_tree::ptree&, std::set<std::string>&);
	const std::array<parserFunc_t,35> _funcs;
	CameraState _cameraState;
	std::recursive_mutex _mutex;

	/** 0 */
	bool parseAvailableApiList(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 1 */
	bool parseCameraStatust(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 3 */
	bool parseLiveviewStatust(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 10 */
	bool parse10th(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 12 */
	bool parseCameraFunction(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 13 */
	bool parseMovieQuality(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 16 */
	bool parseSteadyMode(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 17 */
	bool parseViewAngle(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** 21 */
	bool parseShootMode(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
	/** others */
	bool parseNothing(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);

	bool parseSimpleProperty(
		boost::property_tree::ptree& pt,
		std::set<std::string>& updatedObjects,
		const char* propName,
		bool (CameraState::*updator)(const std::string&)
		);

	bool parsePropWithCandidates(
		boost::property_tree::ptree& pt,
		std::set<std::string>& updatedObjects,
		const char* propName,
		const char* candName,
		bool (CameraState::*updator)(const std::string&),
		bool(CameraState::*candexist)() const,
		void (CameraState::*candsetter)(const std::set<std::string>&)
		);
};

