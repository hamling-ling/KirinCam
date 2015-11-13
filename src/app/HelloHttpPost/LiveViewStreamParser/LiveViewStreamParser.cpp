// LiveViewStreamParser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <cstdint>
#include <memory>
#include <list>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;

#define UNDEFINED_SIZE	0xFFFFFFFF

enum PayloadType
{
	kPayloadTypeLiveViewImages = 0x01,
	kPayloadTypeLiveViewFrameInformation = 0x02,
};

enum FrameCategory {
	kFrameCategoryInvalid = 0x00,
	kFrameCategoryContrastAF = 0x01,
	kFrameCategoryPhaseDetectioniAF = 0x02,
	kFrameCategoryReserved = 0x03,
	kFrameCategoryFace = 0x04,
	kFrameCategoryTracking = 0x05,
};

enum FrameStatus {
	kFrameStatusInvalid = 0x00,
	kFrameStatusNormal = 0x01,
	kFrameStatusMain = 0x02,
	kFrameStatusSub = 0x03,
	kFrameStatusFocused = 0x04,
	kFrameStatusReserved1 = 0x05,
	kFrameStatusReserved2 = 0x06,
	kFrameStatusReserved3 = 0x07,
};

enum FrameAdditionalStatus {
	kFrameAdditionalStatusInvalid = 0x00,
	kFrameAdditionalStatusNormal = 0x01,
	kFrameAdditionalStatusLargeFrame = 0x02,
};

std::vector<PayloadType> kPayloadTypes =
{
	kPayloadTypeLiveViewImages,
	kPayloadTypeLiveViewFrameInformation,

};

std::vector<FrameCategory> kFrameCategories = {
	kFrameCategoryInvalid,
	kFrameCategoryContrastAF,
	kFrameCategoryPhaseDetectioniAF,
	kFrameCategoryReserved,
	kFrameCategoryFace,
	kFrameCategoryTracking,
};

std::vector<FrameStatus> kFrameStatus = {
	kFrameStatusInvalid,
	kFrameStatusNormal,
	kFrameStatusMain,
	kFrameStatusSub,
	kFrameStatusFocused,
	kFrameStatusReserved1,
	kFrameStatusReserved1,
	kFrameStatusReserved1,
};

std::vector<FrameAdditionalStatus> kFrameAdditionalStatus = {
	kFrameAdditionalStatusInvalid,
	kFrameAdditionalStatusNormal,
	kFrameAdditionalStatusLargeFrame,
};

uint8_t kCommonHaderStartByte = 0xFF;
uint8_t kPayloadHeaderStartCode[4] = { 0x24, 0x35, 0x68, 0x79 };
uint8_t kFrameDataVersion[2] = { 0x01, 0x00 };


class IIncrementalFill
{
public:
	IIncrementalFill() : _pos(0), _size(UNDEFINED_SIZE) {}
	virtual ~IIncrementalFill() = 0;
	bool IsFull() { return _size <= _pos; }
	/**
		Fill returns false when:
		- unexpected data
		- fill called when full
		- returns true even if input data is empty
	*/
	virtual bool Fill(asio::streambuf& buf) = 0;
	size_t Size() { return _size; }
	void SetSize(uint32_t sz) { _size = sz; }
	uint32_t Pos() { return _pos; }
protected:
	void SetPos(uint32_t pos) { _pos = pos; }
	virtual void Filled() = 0;
private:
	uint32_t _pos;
	size_t _size;
};

IIncrementalFill::~IIncrementalFill(){}

class BaseElement : public IIncrementalFill
{
public:
	BaseElement() {}
	virtual ~BaseElement(){};
	virtual bool Fill(asio::streambuf& buf)
	{
		if (IsFull()) {
			Filled();
		}
		return true;
	}
protected:
	void Filled() {}
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
	virtual bool Fill(asio::streambuf& buf)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(buf.size(), Size() - pos);
		const char* phead = asio::buffer_cast<const char*>(buf.data());
		memcpy(_data, phead, copyBytes);
		buf.consume(copyBytes);

		SetPos(pos + copyBytes);

		BaseElement::Fill(buf);

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
	virtual bool Fill(asio::streambuf& buf)
	{
		return true;
	}
};

class EmptyData : public BaseElement
{
public:
	EmptyData()
	{
	}
	~EmptyData(){}
	virtual bool Fill(asio::streambuf& buf)
	{
		return true;
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
	virtual bool Fill(asio::streambuf& buf)
	{
		uint32_t pos = Pos();
		const uint32_t copyBytes = std::min(buf.size(), Size() - pos);

		const char* phead = asio::buffer_cast<const char*>(buf.data());
		const char* ptail = phead + copyBytes;
		_data.insert(_data.end(), phead, ptail);
		buf.consume(copyBytes);

		SetPos(pos + copyBytes);

		BaseElement::Fill(buf);

		return true;
	}
	const uint8_t* Data() { return &_data[0]; }
private:
	std::vector<uint8_t> _data;
};

class BaseField : public BaseElement
{
public:
	BaseField() {}
	virtual ~BaseField(){}
	virtual bool Fill(asio::streambuf& buf)
	{
		if (IsFull()) {
			return false;
		}

		if (Pos() == 0) {
			_it = _elements.begin();
		}

		while (buf.size() > 0 && _it !=  _elements.end()) {
			assert(!(*_it)->IsFull());

			/**
			filled    , full    , data remaining : => fill next elem
			filled    , full    , data runout    : => fill next elem
			filled    , not full, data remaining : => error
			filled    , not full, data runout    : => success
			not filled, full    , data remaining : => fill next elem
			not filled, full    , data runout    : => should not occurr
			not filled, not full, data remaining : => unexpected byte,
			not filled, not full, data runout    : => should not occurr
			*/
			if (!(*_it)->Fill(buf)) {
				return false;
			}
			if ((*_it)->IsFull()) {
				_it++;
			}
		}
		CalcPos();
		return true;
	}

protected:
	std::list<BaseElement*> _elements;

	void CalcSize()
	{
		size_t sum = 0;
		for (auto v : _elements) {
			size_t size = v->Size();
			if (UNDEFINED_SIZE == size) {
				sum = UNDEFINED_SIZE;
				break;
			}
			else {
				sum += size;
			}
		}
		SetSize(sum);
	}

	void CalcPos()
	{
		uint32_t sum = 0;
		for (auto v : _elements) {
			sum += v->Pos();
		}
		SetPos(sum);
	}
private:
	std::list<BaseElement* >::iterator _it;
};

class ConstByte : public FixedSizeData<1>
{
public:
	ConstByte(uint8_t val) : _expectedValue(val) { }
	virtual bool Fill(asio::streambuf& buf)
	{
		const char* cp = boost::asio::buffer_cast<const char*>(buf.data());
		if (static_cast<const char>(_expectedValue) != *cp) {
			return false;
		}
		return FixedSizeData<1>::Fill(buf);
	}
protected:
	const uint8_t _expectedValue;
};

class StartByte : public ConstByte
{
public:
	StartByte() : ConstByte(0xFF) {}
	~StartByte() {}
};

class SingleByteNumber : public FixedSizeData<1>
{
public:
	SingleByteNumber() : _value(0) {}
	~SingleByteNumber(){}
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		_value = *(data + 0);
	}
	uint8_t Value() { return _value; }
private:
	uint8_t _value;
};

class WByteNumber : public FixedSizeData<2>
{
public:
	WByteNumber() : _value(0) {}
	~WByteNumber(){}
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		const uint8_t val0 = *(data + 0);
		const uint8_t val1 = *(data + 1);

		_value = val0 << 8 | val1;
	}
	uint16_t Value() { return _value; }
private:
	uint16_t _value;
};

class TriByteNumber : public FixedSizeData<3>
{
public:
	TriByteNumber() : _value(0) {}
	~TriByteNumber(){}
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		const uint8_t val0 = *(data + 0);
		const uint8_t val1 = *(data + 1);
		const uint8_t val2 = *(data + 2);

		_value = val0 << 16 | val1 << 8 | val2;
	}
	uint32_t Value() { return _value; }
private:
	uint32_t _value;
};

class QuadByteNumber : public FixedSizeData<4>
{
public:
	QuadByteNumber() : _value(0) {}
	~QuadByteNumber(){}
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		const uint8_t val0 = *(data + 0);
		const uint8_t val1 = *(data + 1);
		const uint8_t val2 = *(data + 2);
		const uint8_t val3 = *(data + 3);

		_value = val0 << 24 | val1 << 16 | val2 << 8 | val3;
	}
	uint32_t Value() { return _value; }
private:
	uint32_t _value;
};


template<typename T>
class ByteEnum : public FixedSizeData<1>
{
public:
	ByteEnum(){}
	~ByteEnum(){}
	T Value() { return _value; }
	virtual bool Fill(asio::streambuf& buf)
	{
		const T* cp = asio::buffer_cast<const T*>(buf.data());
		const std::vector<T>& allKinds = AllKinds();

		vector<T>::const_iterator it = std::find(allKinds.begin(), allKinds.end(), *cp);
		if (it == allKinds.end()) {
			return false;
		}

		return FixedSizeData<1>::Fill(buf);
	}
protected:
	virtual const std::vector<T>& AllKinds() = 0;
	void Filled()
	{
		const uint8_t *data = Data();
		_value = static_cast<T>(*data);
	}
private:
	T _value;
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

class StartCode : public BaseField
{
public:
	StartCode()
		:
		_byte0(kPayloadHeaderStartCode[0]),
		_byte1(kPayloadHeaderStartCode[1]),
		_byte2(kPayloadHeaderStartCode[2]),
		_byte3(kPayloadHeaderStartCode[3])
	{
		_elements.push_back(&_byte0);
		_elements.push_back(&_byte1);
		_elements.push_back(&_byte2);
		_elements.push_back(&_byte3);
		SetSize(4);
	}
	~StartCode(){}
private:
	ConstByte _byte0;
	ConstByte _byte1;
	ConstByte _byte2;
	ConstByte _byte3;
};

class DataVersion : public FixedSizeData<2>
{
public:
	DataVersion() : _version(0.f), _major(0), _minor(0) {}
	~DataVersion(){}
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		_major = *(data + 0);
		_minor = *(data + 1);
		_version = _major + _minor * 0.1f;
	}
	uint8_t Major() { return _major; }
	uint8_t Minor() { return _minor; }
	float Version() { return _version; }
private:
	float _version;
	uint8_t _major;
	uint8_t _minor;
};

class Coordinate : public FixedSizeData<4>
{
public:
	Coordinate(){}
	~Coordinate(){}
	uint16_t X() { return _x; }
	uint16_t Y() { return _y; }
	void Filled()
	{
		const uint8_t *data = Data();
		_x = *(data + 0);
		_y = *(data + 1);
	}
private:
	uint16_t _x;
	uint16_t _y;
};

class Category : public ByteEnum<FrameCategory>
{
public:
	Category(){}
	~Category(){}
protected:
	const std::vector<FrameCategory>& AllKinds() { return kFrameCategories; }
};

class Status : public ByteEnum<FrameStatus>
{
public:
	Status(){}
	~Status(){}
protected:
	const std::vector<FrameStatus>& AllKinds() { return kFrameStatus; }
};

class AdditionalStatus : public ByteEnum<FrameAdditionalStatus>
{
public:
	AdditionalStatus(){}
	~AdditionalStatus(){}
protected:
	const std::vector<FrameAdditionalStatus>& AllKinds() { return kFrameAdditionalStatus; }
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

class PayloadHeader : public BaseField
{
public:
	PayloadHeader()
	{
		_elements.push_back(&_startCode);
		_elements.push_back(&_dataSize);
		_elements.push_back(&_paddingSize);
	}
	~PayloadHeader(){}
protected:
	StartCode _startCode;
	TriByteNumber _dataSize;
	FixedSizeEmptyData<3> _paddingSize;
};

class ImagePayloadHeader : public PayloadHeader
{
public:
	ImagePayloadHeader()
	{
		_elements.push_back(&_reserved);
		_elements.push_back(&_flag);
		_elements.push_back(&_reserved2);
	}
	~ImagePayloadHeader(){}
private:
	FixedSizeEmptyData<4> _reserved;
	FixedSizeEmptyData<1> _flag;
	FixedSizeEmptyData<5> _reserved2;
};

class FramePayloadHeader : public PayloadHeader
{
public:
	FramePayloadHeader()
	{
		_elements.push_back(&_dataVersion);
		_elements.push_back(&_frameCount);
		_elements.push_back(&singleFrameDataSize);
		_elements.push_back(&_reserved);
	}
	~FramePayloadHeader(){}
private:
	DataVersion _dataVersion;
	WByteNumber _frameCount;
	WByteNumber singleFrameDataSize;
	FixedSizeEmptyData<114> _reserved;
};

class FrameInformation : public BaseField
{
public:
	FrameInformation()
	{
		_elements.push_back(&_topLeft);
		_elements.push_back(&_buttomRight);
		_elements.push_back(&_category);
		_elements.push_back(&_status);
		_elements.push_back(&_additionalStatus);
	}
	~FrameInformation(){}
private:
	Coordinate _topLeft;
	Coordinate _buttomRight;
	Category _category;
	Status _status;
	AdditionalStatus _additionalStatus;
};

class Payload : public BaseField
{
public:
	Payload()
	{
		_elements.push_back(&_header);
		_elements.push_back(&_payload);
	}
	~Payload(){}
private:
	PayloadHeader _header;
	BaseField _payload;
};

class ImagePayload : public Payload
{
public:
	ImagePayload()
	{
		_elements.push_back(&_jpeg);
		_elements.push_back(&_padding);
	}
	~ImagePayload(){}
private:
	VariableSizeData _jpeg;
	EmptyData _padding;
};

class FramePayload : public Payload
{
public:
	FramePayload()
	{

	}
	~FramePayload(){}
	void SetFrameNumber(int num)
	{
		_elements.clear();
		for (int i = 0; i < num; i++) {
			std::shared_ptr<FrameInformation> sptr = std::make_shared<FrameInformation>();
			_frameInformations.push_back(sptr);
			_elements.push_back(sptr.get());
		}
		_elements.push_back(&_reserved);
	}
private:
	std::list<std::shared_ptr<FrameInformation> > _frameInformations;
	FixedSizeEmptyData<5> _reserved;
};

class Packet : public BaseField
{
public:
	Packet()
	{
		commonHeader_ = std::make_shared<CommonHeader>();
		_elements.push_back(commonHeader_.get());
		// here we do not push payload element yet
	}
	~Packet() {}
	virtual bool Fill(asio::streambuf& buf)
	{
		SkipUntil(0xFF, buf);

		if (!commonHeader_->IsFull()) {
			if (!commonHeader_->Fill(buf)) {
				return false;
			}
		}

		if (commonHeader_->IsFull()) {
			if (!payload_) {
				if (commonHeader_->PayloadType() == kPayloadTypeLiveViewImages) {
					std::shared_ptr<ImagePayload> sptr = std::make_shared<ImagePayload>();
					payload_ = std::dynamic_pointer_cast<Payload>(sptr);
				}
				else {
					std::shared_ptr<FramePayload> sptr = std::make_shared<FramePayload>();
					payload_ = std::dynamic_pointer_cast<Payload>(sptr);
				}
				_elements.push_back(payload_.get());
			}
			return payload_->Fill(buf);
		}
		return true;
	}

	void SkipUntil(char c, asio::streambuf& buf) {
		if (Pos() == 0) {
			/* 頭出し */
			istream ifs(&buf);
			while (!ifs.eof()) {
				char c = ifs.peek();
				if (c == static_cast<char>(0xFF)) {
					break;
				}
				ifs.read(&c, sizeof(c));
			}
		}
	}
private:
	std::shared_ptr<CommonHeader> commonHeader_;
	std::shared_ptr<Payload> payload_;
};

int _tmain(int argc, _TCHAR* argv[])
{
	asio::streambuf buf;
	ifstream ifs("out.dat", ios::in | ios::binary);
	if (!ifs) {
		std::cout << "err" << std::endl;
		return 1;
	}

	char tmp[64];
	ostream ofs(&buf);
	while (!ifs.eof()){
		size_t readsize = ifs.read(&tmp[0], sizeof(tmp)).gcount();
		cout << "readsize:" << readsize << endl;
		ofs.write(&tmp[0], readsize);
	}

	Packet packet;
	packet.Fill(buf);
	return 0;
}

