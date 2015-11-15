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
	StartByte();
	~StartByte();
};


class PayloadTypeByte : public ByteEnum<PayloadType>
{
public:
	PayloadTypeByte();
	~PayloadTypeByte();
protected:
	const std::vector<PayloadType>& AllKinds() { return kPayloadTypes; }
};


class SequenceNumber : public WByteNumber
{
public:
	SequenceNumber();
	~SequenceNumber();
};

class TimeStamp : public QuadByteNumber
{
public:
	TimeStamp();
	~TimeStamp();
};

class CommonHeader : public BaseField
{
public:
	CommonHeader();
	~CommonHeader();
	PayloadType PayloadType();
private:

	StartByte _startByte;
	PayloadTypeByte _payloadType;
	SequenceNumber _sequenceNumber;
	TimeStamp _timeStamp;
};
