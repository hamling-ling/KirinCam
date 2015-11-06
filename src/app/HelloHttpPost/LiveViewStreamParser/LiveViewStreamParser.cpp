// LiveViewStreamParser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>

enum PayloadType
{
	kPayloadTypeLiveViewImages = 0x02,
	kPayloadTypeLiveViewFrameInformation = 0x01,
};

uint8_t kCommonHaderStartByte = 0xFF;
uint8_t kPayloadHeaderStartCode[4] = { 0x24, 0x35, 0x68, 0x79 };

uint8_t kFrameDataVersion[2] = { 0x01, 0x00 };

class IIncrementalFill
{
public:
	IIncrementalFill() {}
	virtual ~IIncrementalFill() = 0;
	bool IsFull() { return _pos < _size; }
	virtual bool Fill(uint8_t *data, size_t size) = 0;
	uint32_t Size() { return _size; }
	void SetSize(uint32_t sz) { _size = sz; }
protected:

	void SetPos(uint32_t pos) { _pos = pos; }
	uint32_t Pos() { return _pos; }
private:
	uint32_t _pos;
	uint32_t _size;
};

class BaseElement : public IIncrementalFill
{
public:
	BaseElement() {}
	virtual ~BaseElement() = 0;
};

template<int byteLength>
class FixedSizeData : public BaseElement
{
public:
	FixedSizeData()
	{
		memset(_data, 0, sizeof(_data));
		SetSize(byteLength);
	}
	~FixedSizeData(){}
	virtual bool Fill(uint8_t *data, size_t size)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(size, Size() - pos);
		memcpy(_data, data, copyBytes);
		SetPos(pos + copyBytes);
		return true;
	}
	const uint8_t* Data() { return &_data[0]; }
private:
	uint8_t _data[byteLength];
};

template<int byteLength>
class FixedSizeEmptyData : public BaseElement
{
public:
	FixedSizeEmptyData()
	{
		SetSize(byteLength);
	}
	~FixedSizeEmptyData(){}
	virtual bool Fill(uint8_t *data, size_t size)
	{
		return false;
	}
};

class EmptyData : public BaseElement
{
public:
	EmptyData()
	{
	}
	~EmptyData(){}
	virtual bool Fill(uint8_t *data, size_t size)
	{
		return false;
	}
};

class VariableSizeData : public BaseElement
{
public:
	VariableSizeData()
	{
	}
	VariableSizeData(int size)
	{
		_data.reserve(size);
	}
	~VariableSizeData(){}
	virtual bool Fill(uint8_t *data, size_t size)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(size, Size() - pos);
		for (uint32_t i = 0; i < copyBytes; i++) {
			_data.push_back(*data);
			data++;
		}
		SetPos(pos + copyBytes);
		return true;
	}
	const uint8_t* Data() { return &_data[0]; }
private:
	std::vector<uint8_t> _data;
};

class BaseField : public BaseElement
{
public:
	BaseField(){}
	virtual ~BaseField(){}
	virtual bool Fill(uint8_t *data, size_t size)
	{
		//T.B.D
		return true;
	}
protected:
	std::vector<BaseElement*> _elements;
};

class StartByte : public FixedSizeData<1>
{
public:
	virtual bool Fill(uint8_t *data, size_t size)
	{
		if (kCommonHaderStartByte != *data) {
			return false;
		}
		return FixedSizeData<1>::Fill(data, size);
	}
};

class PayloadTypeByte : public FixedSizeData<1>
{
public:
	virtual bool Fill(uint8_t *data, size_t size)
	{
		if (kPayloadTypeLiveViewImages != *data &&
			kPayloadTypeLiveViewFrameInformation != *data) {
			return false;
		}
		return FixedSizeData<1>::Fill(data, size);
	}
};

class SequenceNumber : public FixedSizeData<2>
{
};

class TimeStamp : public FixedSizeData<4>
{
};

class StartCode : public FixedSizeData<4>
{
};

class TByteNumber : public FixedSizeData<3>
{
};

class DataVersion : public FixedSizeData<2>
{
};

class WByteNumber : public FixedSizeData<2>
{};

class Coordinate : public FixedSizeData<4>
{
};

class Category : public FixedSizeData<1>
{};

class Status : public FixedSizeData<1>
{};

class AdditionalStatus : public FixedSizeData<1>
{};

class PayloadHeader : public BaseField
{
};

class CommonHeader : public BaseField
{
public:
	CommonHeader()
	{
	}

	~CommonHeader(){}
private:

	StartByte _startByte;
	PayloadTypeByte _payloadType;
	SequenceNumber _sequenceNumber;
	TimeStamp _timeStamp;
};

class PayloadHeader : public BaseField
{
protected:
	StartCode _startCode;
	TByteNumber _dataSize;
	FixedSizeEmptyData<3> _paddingSize;
};

class ImagePayloadHeader : public PayloadHeader
{
	FixedSizeEmptyData<4> _reserved;
	FixedSizeEmptyData<1> _flag;
	FixedSizeEmptyData<5> _reserved;
};

class FramePayloadHeader : public PayloadHeader
{
	DataVersion _dataVersion;
	WByteNumber _frameCount;
	WByteNumber singleFrameDataSize;
	FixedSizeEmptyData<114> _reserved;
};

class FrameInformation : public BaseField
{
	Coordinate _topLeft;
	Coordinate _buttomRight;
	Category _category;
	Status _status;
	AdditionalStatus _additionalStatus;
};

class Payload : public BaseField
{
	PayloadHeader _header;
	BaseField _payload;
};

class ImagePayload : BaseField
{
	VariableSizeData _jpeg;
	EmptyData _padding;
};

class FramePayload : BaseField
{
	std::vector<FrameInformation*> _frameInformations;
	FixedSizeEmptyData<5> _reserved;
};



int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

