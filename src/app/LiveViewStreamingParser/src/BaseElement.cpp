#include "stdafx.h"
#include "BaseElement.h"

using namespace std;
using namespace boost;

BaseElement::BaseElement() {}
BaseElement::~BaseElement(){};
bool BaseElement::Fill(asio::streambuf& buf)
{
	if (IsFull()) {
		Filled();
	}
	return true;
}
void BaseElement::Filled() {}

