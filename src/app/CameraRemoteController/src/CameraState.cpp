#include "stdafx.h"
#include "CameraState.h"

using namespace std;

CameraState::CameraState()
{
}

CameraState::CameraState(const CameraState& state)
{
	_apiList = state.GetAvailableApiList();
	_cameraStatus = state.GetCameraStatus();
	_liveviewStatus = state.GetLiveviewStatus();
	_storageInformation = state.GetStorationInformation();
	_cameraFunction = state.GetCameraFunction();
	_movieQuality = state.GetMoviewQuality();
	_steadyMode = state.GetSteadyMode();
	_viewAngle = state.GetViewAngle();
	_shootMode = state.GetShootMode();
	_cameraFunctionCandidates = state.CameraFunctionCandidate();
	_moviewQualityCandidates = state.MoviewQualityCandidates();
	_steadyModeCandidates = state.SteadyModeCandidates();
	_viewAngleCandidates = state.ViewAngleCandidates();
	_shootModeCandidates = state.ShootModeCandidates();
}

CameraState::~CameraState()
{
}

CameraState& CameraState::operator=(const CameraState& state)
{
	lock_guard<recursive_mutex> lock(_mutex);

	_apiList = state.GetAvailableApiList();
	_cameraStatus = state.GetCameraStatus();
	_liveviewStatus = state.GetLiveviewStatus();
	_storageInformation = state.GetStorationInformation();
	_cameraFunction = state.GetCameraFunction();
	_movieQuality = state.GetMoviewQuality();
	_steadyMode = state.GetSteadyMode();
	_viewAngle = state.GetViewAngle();
	_shootMode = state.GetShootMode();
	_cameraFunctionCandidates = state.CameraFunctionCandidate();
	_moviewQualityCandidates = state.MoviewQualityCandidates();
	_steadyModeCandidates = state.SteadyModeCandidates();
	_viewAngleCandidates = state.ViewAngleCandidates();
	_shootModeCandidates = state.ShootModeCandidates();
	return *this;
}

bool CameraState::UpdateAvailableList(const vector<string>& apiList)
{
	if (isContentsEqual<string>(_apiList, apiList)) {
		return false;
	}

	_apiList = apiList;
	return true;
}

bool CameraState::UpdateCameraStatus(const string& status)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_cameraStatus.compare(status) == 0) {
		return false;
	}
	_cameraStatus = status;
	return true;
}

bool CameraState::UpdateLiveviewStatus(const string& status)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_liveviewStatus.compare(status) == 0) {
		return false;
	}
	_liveviewStatus = status;
	return true;
}

bool CameraState::UpdateStorageInformation(const vector<CameraStorageInformation>& info)
{
	if (isContentsEqual<CameraStorageInformation>(_storageInformation, info)) {
		return false;
	}

	_storageInformation = info;
	return true;
}

bool CameraState::UpdateCameraFunction(const string& function)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_cameraFunction.compare(function) == 0) {
		return false;
	}
	_cameraFunction = function;
	return true;
}

bool CameraState::UpdateMovieQuality(const string& quality)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_movieQuality.compare(quality) == 0) {
		return false;
	}
	_movieQuality = quality;
	return true;
}

bool CameraState::UpdateSteadyMode(const string& steadyMode)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_steadyMode.compare(steadyMode) == 0) {
		return false;
	}
	_steadyMode = steadyMode;
	return true;

}
bool CameraState::UpdateViewAngle(const string& viewAngle)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_viewAngle.compare(viewAngle) == 0) {
		return false;
	}
	_viewAngle = viewAngle;
	return true;
}

bool CameraState::UpdateShootMode(const string& shootMode)
{
	lock_guard<recursive_mutex> lock(_mutex);
	if (_shootMode.compare(shootMode) == 0) {
		return false;
	}
	_shootMode = shootMode;
	return true;
}

std::vector<std::string> CameraState::GetAvailableApiList() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _apiList;
}

std::string CameraState::GetCameraStatus() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _cameraStatus;
}

std::string CameraState::GetLiveviewStatus() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _liveviewStatus;
}

std::vector<CameraStorageInformation> CameraState::GetStorationInformation() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _storageInformation;
}

std::string CameraState::GetCameraFunction() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _cameraFunction;
}

std::string CameraState::GetMoviewQuality() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _movieQuality;
}

std::string CameraState::GetSteadyMode() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _steadyMode;
}

std::string CameraState::GetViewAngle() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _viewAngle;
}

std::string CameraState::GetShootMode() const
{
	lock_guard<recursive_mutex> lock(_mutex);
	return _shootMode;
}

set<string> CameraState::CameraFunctionCandidate() const
{
	return _cameraFunctionCandidates;
}

set<string> CameraState::MoviewQualityCandidates() const
{
	return _moviewQualityCandidates;
}

set<string> CameraState::SteadyModeCandidates() const
{
	return _steadyModeCandidates;
}

set<string> CameraState::ViewAngleCandidates() const
{
	return _viewAngleCandidates;
}

set<string> CameraState::ShootModeCandidates() const
{
	return _shootModeCandidates;
}

bool CameraState::HasCameraFunctionCandidate() const
{
	return (!_cameraFunctionCandidates.empty());
}

bool CameraState::HasMoviewQualityCandidates() const
{
	return (!_moviewQualityCandidates.empty());
}

bool CameraState::HasSteadyModeCandidates() const
{
	return (!_steadyModeCandidates.empty());
}

bool CameraState::HasViewAngleCandidates() const
{
	return (!_viewAngleCandidates.empty());
}

bool CameraState::HasShootModeCandidates() const
{
	return (!_shootModeCandidates.empty());
}

void CameraState::SetCameraFunctionCandidate(const std::set<std::string>& candidates)
{
	_cameraFunctionCandidates.clear();
	_cameraFunctionCandidates.insert(candidates.begin(), candidates.end());
}

void CameraState::SetMoviewQualityCandidates(const std::set<std::string>& candidates)
{
	_moviewQualityCandidates.clear();
	_moviewQualityCandidates.insert(candidates.begin(), candidates.end());
}

void CameraState::SetSteadyModeCandidates(const std::set<std::string>& candidates)
{
	_steadyModeCandidates.clear();
	_steadyModeCandidates.insert(candidates.begin(), candidates.end());
}

void CameraState::SetViewAngleCandidates(const std::set<std::string>& candidates)
{
	_viewAngleCandidates.clear();
	_viewAngleCandidates.insert(candidates.begin(), candidates.end());
}

void CameraState::SetShootModeCandidates(const std::set<std::string>& candidates)
{
	_shootModeCandidates.clear();
	_shootModeCandidates.insert(candidates.begin(), candidates.end());
}

