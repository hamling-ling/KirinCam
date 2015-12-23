#include "stdafx.h"
#include "Success.h"
#include <string>
#include <boost/foreach.hpp>

using namespace std;
using boost::property_tree::ptree;

Success::Success()
{
}

Success::Success(const Success& success)
{
}

Success::~Success()
{
}

Success& Success::operator=(const Success& success)
{
	return *this;
}

bool Success::SetStatus(boost::property_tree::ptree& pt)
{
	try {
		boost::optional< ptree& > opt = pt.get_child_optional("result");
		if (!opt) {
			return false;
		}

		_content = opt.get();
	}
	catch (boost::exception& e) {
		return false;
	}

	return true;
}

const boost::property_tree::ptree&  Success::Content() const
{
	return _content;
}

const std::string Success::ContentAsString() const
{
	string content;
	BOOST_FOREACH(const ptree::value_type& child, _content) {
		content += child.second.get_value<std::string>();
	}
	return content;
}

bool Success::IsZero() const
{
	string content = ContentAsString();
	return (content.compare("0") == 0);
}
