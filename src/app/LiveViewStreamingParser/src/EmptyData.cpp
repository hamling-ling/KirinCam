#include "EmptyData.h"

EmptyData::EmptyData()
{
}

EmptyData::~EmptyData(){}

void EmptyData::SetSize(size_t size) {
	BaseElement::SetSize(size);
}
bool EmptyData::Fill(boost::asio::streambuf& buf)
{
	assert(Size() != UNDEFINED_SIZE);

	uint32_t pos = Pos();
	const uint32_t copyBytes = std::min(buf.size(), Size() - pos);
	buf.consume(copyBytes);
	SetPos(pos + copyBytes);

	return true;
}
