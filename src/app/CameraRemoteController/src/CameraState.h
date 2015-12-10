#pragma once

#include <vector>
#include <string>
#include <mutex>
#include "CameraStorageInformation.h"

template<class T>
bool isContentsEqual(const std::vector<T>& a, const std::vector<T>& b) {
	bool result = true;
	if (a.size() != b.size()) {
		return false;
	}

	for (auto v : b) {
		std::vector<T>::const_iterator it = find(a.begin(), a.end(), v);
		if (it == a.end()) {
			result = false;
			break;
		}
	}

	return result;
}

class CameraState
{
public:
	CameraState();
	CameraState(const CameraState& state);
	virtual ~CameraState();
	CameraState& operator=(const CameraState& state);

	bool UpdateAvailableList(const std::vector<std::string>& apiList);
	bool UpdateCameraStatus(const std::string& status);
	bool UpdateLiveviewStatus(const std::string& status);
	bool UpdateStorageInformation(const std::vector<CameraStorageInformation>& info);
	bool UpdateCameraFunction(const std::string& function);
	bool UpdateMovieQuality(const std::string& quality);
	bool UpdateSteadyMode(const std::string& steadyMode);
	bool UpdateViewAngle(const std::string& viewAngle);
	bool UpdateShootMode(const std::string& shootMode);

	std::vector<std::string> GetAvailableApiList() const;
	std::string GetCameraStatus() const;
	std::string GetLiveviewStatus() const;
	std::vector<CameraStorageInformation> GetStorationInformation() const;
	std::string GetCameraFunction() const;
	std::string GetMoviewQuality() const;
	std::string GetSteadyMode() const;
	std::string GetViewAngle() const;
	std::string GetShootMode() const;
private:
	mutable std::recursive_mutex _mutex;
	std::vector<std::string> _apiList;
	std::string _cameraStatus;
	std::string _liveviewStatus;
	std::vector<CameraStorageInformation> _storageInformation;
	std::string _cameraFunction;
	std::string _movieQuality;
	std::string _steadyMode;
	std::string _viewAngle;
	std::string _shootMode;
};

