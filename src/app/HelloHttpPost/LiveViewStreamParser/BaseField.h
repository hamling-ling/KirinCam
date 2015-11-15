#pragma once

#include "BaseElement.h"

class BaseField : public BaseElement
{
public:
	BaseField();
	virtual ~BaseField();
	virtual bool Fill(boost::asio::streambuf& buf);

protected:
	std::list<BaseElement*> _elements;

	void CalcSize();
	void CalcPos();
	virtual void Fulfilled(int idx);

private:
	std::list<BaseElement* >::iterator _it;
};
