#pragma once

#include "BaseField.h"
#include "ConstByte.h"
#include "NByteNumber.h"
#include "LiveViewStreamDefs.h"

static const std::vector<PayloadType> kPayloadTypes =
{
	kPayloadTypeLiveViewImages,
	kPayloadTypeLiveViewFrameInformation,
};

class StartByte : public ConstByte
{
public:
	StartByte() : ConstByte(0xFF) {}
	~StartByte() {}
};


class PayloadTypeByte : public ByteEnum<PayloadType>
{
public:
	PayloadTypeByte(){}
	~PayloadTypeByte(){}
protected:
	const std::vector<PayloadType>& AllKinds() { return kPayloadTypes; }
};


class SequenceNumber : public WByteNumber
{
public:
	SequenceNumber(){}
	~SequenceNumber(){}
};

class TimeStamp : public QuadByteNumber
{
public:
	TimeStamp() {}
	~TimeStamp(){}
};

class CommonHeader : public BaseField
{
public:
	CommonHeader()
	{
		_elements.push_back(&_startByte);
		_elements.push_back(&_payloadType);
		_elements.push_back(&_sequenceNumber);
		_elements.push_back(&_timeStamp);

		CalcSize();
	}

	~CommonHeader(){}
	PayloadType PayloadType() {
		return _payloadType.Value();
	}
private:

	StartByte _startByte;
	PayloadTypeByte _payloadType;
	SequenceNumber _sequenceNumber;
	TimeStamp _timeStamp;
};
