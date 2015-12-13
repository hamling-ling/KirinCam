#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

enum ErrorStatusCode {
	ErrorStatusCodeOK,
	ErrorStatusCodeAny,
	ErrorStatusCodeTimeout,
	ErrorStatusCodeIllegalArgument,
	ErrorStatusCodeIllegalDataFormat,
	ErrorStatusCodeIllegalRequest,
	ErrorStatusCodeIllegalResponse,
	ErrorStatusCodeIllegalState,
	ErrorStatusCodeIllegalType,
	ErrorStatusCodeIndexOutOfBounds,
	ErrorStatusCodeNoSuchElement,
	ErrorStatusCodeNoSuchMethod,
	ErrorStatusCodeUnsupportedVersion,
	ErrorStatusCodeShootingFail = 40400,
	ErrorStatusCodeCameraNotReady = 40401,
	ErrorStatusCodeAlreadyRunningPollingApi = 40402,
	ErrorStatusCodeStillCapturingNotFinished = 40403,
	ErrorStatusCodeSomeContentCouldNotBeDeleted = 41003,
	ErrorStatusCodeUnauthorized = 401,
	ErrorStatusCodeForbidden = 403,
	ErrorStatusCodeNotFound = 404,
	ErrorStatusCodeRequestEntityTooLarge = 413,
	ErrorStatusCodeRequestUriTooLong = 414,
	ErrorStatusCodeNotImplemented = 501,
	ErrorStatusCodeServiceUnavailable = 503,
	ErrorStatusCodeUnknown = 0xFFFFFFFF
};

class ErrorStatus
{
public:
	ErrorStatus();
	ErrorStatus(const ErrorStatus& status);
	virtual ~ErrorStatus();

	ErrorStatus& operator=(const ErrorStatus& status);
	bool SetStatus(boost::property_tree::ptree& pt);
	ErrorStatusCode StatusCode() const;
	const std::string& StatusMessage() const;
private:
	std::string _msg;
	ErrorStatusCode _statusCode;
	ErrorStatusCode errorStatusCodeFromInt(int code);
};

