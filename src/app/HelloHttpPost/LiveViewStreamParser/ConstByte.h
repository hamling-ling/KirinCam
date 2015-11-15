#pragma once

#include "FixedSizeData.h"

class ConstByte : public FixedSizeData<1>
{
public:
	ConstByte(uint8_t val) : _expectedValue(val) { }
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		const char* cp = boost::asio::buffer_cast<const char*>(buf.data());
		if (static_cast<const char>(_expectedValue) != *cp) {
			return false;
		}
		return FixedSizeData<1>::Fill(buf);
	}
protected:
	const uint8_t _expectedValue;
};
