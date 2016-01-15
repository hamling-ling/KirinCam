#pragma once

#include "BaseElement.h"
#include <vector>

class VariableSizeData : public BaseElement
{
public:
	VariableSizeData()
	{
	}
	VariableSizeData(int size)
	{
		SetSize(size);
	}
	~VariableSizeData(){}
	void SetSize(size_t size) {
		_data.reserve(size);
		BaseElement::SetSize(size);
	}
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(buf.size(), Size() - pos);

		const char* phead = boost::asio::buffer_cast<const char*>(buf.data());
		const char* ptail = phead + copyBytes;
		_data.insert(_data.end(), phead, ptail);
		buf.consume(copyBytes);

		SetPos(pos + copyBytes);

		BaseElement::Fill(buf);

		return true;
	}
	const uint8_t* Data() const { return &_data[0]; }
	const std::vector<uint8_t>& Vec() const { return _data; }

private:
	std::vector<uint8_t> _data;
};
