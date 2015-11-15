#include "stdafx.h"
#include "NByteNumber.h"

SingleByteNumber::SingleByteNumber() : _value(0) {}

SingleByteNumber::~SingleByteNumber(){}

uint8_t SingleByteNumber::Value() { return _value; }

void SingleByteNumber::Filled()
{
	const uint8_t *data = Data();
	_value = *(data + 0);
}


WByteNumber::WByteNumber() : _value(0) {}
WByteNumber::~WByteNumber(){}
uint16_t WByteNumber::Value() { return _value; }
void WByteNumber::Filled()
{
	const uint8_t *data = Data();
	const uint8_t val0 = *(data + 0);
	const uint8_t val1 = *(data + 1);

	_value = val0 << 8 | val1;
}
