#include "stdafx.h"
#include "CommonHeader.h"


StartByte::StartByte() : ConstByte(0xFF) {}
StartByte::~StartByte() {}

PayloadTypeByte::PayloadTypeByte(){}
PayloadTypeByte::~PayloadTypeByte(){}

SequenceNumber::SequenceNumber(){}
SequenceNumber::~SequenceNumber(){}

TimeStamp::TimeStamp() {}
TimeStamp::~TimeStamp(){}

CommonHeader::CommonHeader()
{
	_elements.push_back(&_startByte);
	_elements.push_back(&_payloadType);
	_elements.push_back(&_sequenceNumber);
	_elements.push_back(&_timeStamp);

	CalcSize();
}

CommonHeader::~CommonHeader(){}

PayloadType CommonHeader::PayloadType() const {
	return _payloadType.Value();
}

uint16_t CommonHeader::GetSequenceNumber() const {
	return _sequenceNumber.Value();
}

uint32_t CommonHeader::GetTimeStamp() const {
	return _timeStamp.Value();
}

