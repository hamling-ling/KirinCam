#pragma once

#include <vector>
#include <set>
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

	std::set<std::string> CameraFunctionCandidate() const;
	std::set<std::string> MoviewQualityCandidates() const;
	std::set<std::string> SteadyModeCandidates() const;
	std::set<std::string> ViewAngleCandidates() const;
	std::set<std::string> ShootModeCandidates() const;

	bool HasCameraFunctionCandidate() const;
	bool HasMoviewQualityCandidates() const;
	bool HasSteadyModeCandidates() const;
	bool HasViewAngleCandidates() const;
	bool HasShootModeCandidates() const;

	void SetCameraFunctionCandidate(const std::set<std::string>& candidates);
	void SetMoviewQualityCandidates(const std::set<std::string>& candidates);
	void SetSteadyModeCandidates(const std::set<std::string>& candidates);
	void SetViewAngleCandidates(const std::set<std::string>& candidates);
	void SetShootModeCandidates(const std::set<std::string>& candidates);

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
	std::set<std::string> _cameraFunctionCandidates;
	std::set<std::string> _moviewQualityCandidates;
	std::set<std::string> _steadyModeCandidates;
	std::set<std::string> _viewAngleCandidates;
	std::set<std::string> _shootModeCandidates;
};

