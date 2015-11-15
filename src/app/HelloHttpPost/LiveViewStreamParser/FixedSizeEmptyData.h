#pragma once
#include "BaseElement.h"
#include <algorithm>

template<int byteLength>
class FixedSizeEmptyData : public BaseElement
{
public:
	FixedSizeEmptyData()
	{
		SetSize(byteLength);
	}
	~FixedSizeEmptyData(){}
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(buf.size(), Size() - pos);
		buf.consume(copyBytes);
		SetPos(pos + copyBytes);

		return true;
	}
};
