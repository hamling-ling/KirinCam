#pragma once

#include "FixedSizeData.h"

class ConstByte : public FixedSizeData<1>
{
public:
	ConstByte(uint8_t val);
	virtual bool Fill(boost::asio::streambuf& buf);
protected:
	const uint8_t _expectedValue;
};
