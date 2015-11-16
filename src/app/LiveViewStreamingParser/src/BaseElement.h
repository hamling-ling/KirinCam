#pragma once

#include "IIncrementalFill.h"

class BaseElement : public IIncrementalFill
{
public:
	BaseElement();
	virtual ~BaseElement();
	virtual bool Fill(boost::asio::streambuf& buf);
protected:
	void Filled();
};
