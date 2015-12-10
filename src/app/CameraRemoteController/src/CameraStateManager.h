#pragma once

#include <string>
#include <array>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include <boost/function.hpp>
#include "CameraState.h"

class CameraStateManager
{
public:
	CameraStateManager();
	virtual ~CameraStateManager();
	void UpdateState(const std::string& json, std::set<std::string>& updatedObjectNames);

private:
	typedef bool (CameraStateManager::*parserFunc_t)(boost::property_tree::ptree&, std::set<std::string>&);
	const std::array<parserFunc_t,35> _funcs;
	CameraState _cameraState;

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
	bool parseNothing(boost::property_tree::ptree& pt, std::set<std::string>& updatedObjects);
};

