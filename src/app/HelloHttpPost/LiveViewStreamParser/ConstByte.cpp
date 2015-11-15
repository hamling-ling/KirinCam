#include "stdafx.h"
#include "ConstByte.h"

ConstByte::ConstByte(uint8_t val) : _expectedValue(val) { }

bool ConstByte::Fill(boost::asio::streambuf& buf)
{
	const char* cp = boost::asio::buffer_cast<const char*>(buf.data());
	if (static_cast<const char>(_expectedValue) != *cp) {
		return false;
	}
	return FixedSizeData<1>::Fill(buf);
}
