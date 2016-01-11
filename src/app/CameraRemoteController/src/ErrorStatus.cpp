#include "stdafx.h"
#include "ErrorStatus.h"
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;
using boost::property_tree::ptree;
using boost::lexical_cast;

ErrorStatus::ErrorStatus()
{
}

ErrorStatus::ErrorStatus(const ErrorStatus& status)
{
	_statusCode = status.StatusCode();
}

ErrorStatus::~ErrorStatus()
{
}

ErrorStatus& ErrorStatus::operator=(const ErrorStatus& status)
{
	_statusCode = status.StatusCode();
	return *this;
}

bool ErrorStatus::SetStatus(boost::property_tree::ptree& pt)
{
	int iErrorCode = 0;
	string strErrorMsg;
	try {
		boost::optional< ptree& > erropt = pt.get_child_optional("error");
		if (!erropt) {
			return false;
		}

		ptree& err = erropt.get();
		string strErrorCode;
		string strErrorMsg;
		ptree::iterator it = err.begin();
		if (it != err.end()) {
			strErrorCode = it->second.data();
			it++;
		}
		if (it != err.end()) {
			strErrorMsg = it->second.data();
			it++;
		}

		iErrorCode = lexical_cast<int>(strErrorCode);
	}
	catch (boost::exception& e) {
		return false;
	}

	_statusCode = errorStatusCodeFromInt(iErrorCode);
	_msg = strErrorMsg;

	return true;
}

ErrorStatusCode ErrorStatus::StatusCode() const
{
	return _statusCode;
}

const string& ErrorStatus::StatusMessage() const
{
	return _msg;
}

ErrorStatusCode ErrorStatus::errorStatusCodeFromInt(int code)
{
	ErrorStatusCode status = ErrorStatusCodeOK;
	switch (code) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 40400:
		case 40401:
		case 40402:
		case 40403:
		case 401:
		case 402:
		case 403:
		case 404:
		case 413:
		case 414:
		case 501:
		case 503:
			status = static_cast<ErrorStatusCode>(code);
			break;
		default:
			status = ErrorStatusCodeUnknown;
			break;
	}
	return status;
}
