#pragma once

#include <string>

struct CameraStorageInformation {
public:
	CameraStorageInformation();
	~CameraStorageInformation();
	CameraStorageInformation(const CameraStorageInformation& state);
	CameraStorageInformation& operator=(const CameraStorageInformation& state);

	std::string storageID;
	std::string recordTarget;
	std::string numberOfRecordableImages;
	std::string recordableTime;
	std::string storageDescription;

	bool operator == (const CameraStorageInformation& info) const;
};
