#pragma once

#include "BaseElement.h"

template<int byteLength>
class FixedSizeData : public BaseElement
{
public:
	FixedSizeData()
	{
		memset(_data, 0, sizeof(_data));
		SetSize(byteLength);
	}
	~FixedSizeData(){}
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(buf.size(), Size() - pos);
		const char* phead = boost::asio::buffer_cast<const char*>(buf.data());
		memcpy(_data, phead, copyBytes);
		buf.consume(copyBytes);

		SetPos(pos + copyBytes);

		BaseElement::Fill(buf);

		return true;
	}
	const uint8_t* Data() const { return &_data[0]; }
private:
	uint8_t _data[byteLength];

};