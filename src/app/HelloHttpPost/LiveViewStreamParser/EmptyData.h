#pragma once
#pragma once

#include "BaseElement.h"

class EmptyData : public BaseElement
{
public:
	EmptyData()
	{
	}
	~EmptyData(){}
	void SetSize(size_t size) {
		BaseElement::SetSize(size);
	}
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		assert(Size() != UNDEFINED_SIZE);

		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(buf.size(), Size() - pos);
		buf.consume(copyBytes);
		SetPos(pos + copyBytes);

		return true;
	}
};

