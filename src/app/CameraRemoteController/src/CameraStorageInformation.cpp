#include "stdafx.h"
#include "CameraStorageInformation.h"


CameraStorageInformation::CameraStorageInformation()
{
}

CameraStorageInformation::CameraStorageInformation(const CameraStorageInformation& state)
{
	storageID = state.storageID;
	recordTarget = state.recordTarget;
	numberOfRecordableImages = state.numberOfRecordableImages;
	recordableTime = state.recordableTime;
	storageDescription = state.storageDescription;
}

CameraStorageInformation::~CameraStorageInformation()
{
}

CameraStorageInformation& CameraStorageInformation::operator = (const CameraStorageInformation& state)
{
	storageID = state.storageID;
	recordTarget = state.recordTarget;
	numberOfRecordableImages = state.numberOfRecordableImages;
	recordableTime = state.recordableTime;
	storageDescription = state.storageDescription;
}

bool CameraStorageInformation::operator == (const CameraStorageInformation& info) {
	if (storageID.compare(info.storageID) != 0) {
		return false;
	}
	if (recordTarget.compare(info.recordTarget) != 0) {
		return false;
	}
	if (numberOfRecordableImages.compare(info.numberOfRecordableImages) != 0) {
		return false;
	}
	if (recordableTime.compare(info.recordableTime) != 0) {
		return false;
	}
	if (storageDescription.compare(info.storageDescription) != 0) {
		return false;
	}

