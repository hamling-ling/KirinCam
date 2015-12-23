#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>

class Success
{
public:
	Success();
	Success(const Success& success);
	virtual ~Success();

	Success& operator=(const Success& success);
	bool SetStatus(boost::property_tree::ptree& pt);
	const boost::property_tree::ptree& Content() const;
	const std::string ContentAsString() const;
	bool IsZero() const;
private:
	boost::property_tree::ptree _content;
};

