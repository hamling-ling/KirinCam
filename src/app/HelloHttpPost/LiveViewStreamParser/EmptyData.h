#pragma once

#include "BaseElement.h"

class EmptyData : public BaseElement
{
public:
	EmptyData();
	~EmptyData();
	void SetSize(size_t size);
	virtual bool Fill(boost::asio::streambuf& buf);
};

