#pragma once

#include "BaseField.h"
#include "FixedSizeData.h"
#include "NByteNumber.h"
#include "FixedSizeEmptyData.h"
#include "EmptyData.h"
#include "ConstByte.h"
#include "VariableSizeData.h"
#include "LiveViewStreamDefs.h"
#include <vector>

static const std::vector<FrameCategory> kFrameCategories = {
	kFrameCategoryInvalid,
	kFrameCategoryContrastAF,
	kFrameCategoryPhaseDetectioniAF,
	kFrameCategoryReserved,
	kFrameCategoryFace,
	kFrameCategoryTracking,
};

static const std::vector<FrameStatus> kFrameStatus = {
	kFrameStatusInvalid,
	kFrameStatusNormal,
	kFrameStatusMain,
	kFrameStatusSub,
	kFrameStatusFocused,
	kFrameStatusReserved1,
	kFrameStatusReserved1,
	kFrameStatusReserved1,
};

static const std::vector<FrameAdditionalStatus> kFrameAdditionalStatus = {
	kFrameAdditionalStatusInvalid,
	kFrameAdditionalStatusNormal,
	kFrameAdditionalStatusLargeFrame,
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
	uint32_t DataSize() { return _dataSize.Value(); }
	uint8_t PaddingSize() { return _paddingSize.Value(); }
protected:
	StartCode _startCode;
	TriByteNumber _dataSize;
	SingleByteNumber _paddingSize;
};

class ImagePayloadHeader : public PayloadHeader
{
public:
	ImagePayloadHeader()
	{
		_elements.push_back(&_reserved);
		_elements.push_back(&_flag);
		_elements.push_back(&_reserved2);

		CalcSize();
	}
	~ImagePayloadHeader(){}

	// debug
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		bool ret = PayloadHeader::Fill(buf);
		bool isFull = IsFull();
		return ret;
	}

private:
	FixedSizeEmptyData<4> _reserved;
	FixedSizeEmptyData<1> _flag;
	FixedSizeEmptyData<115> _reserved2;
};

class FramePayloadHeader : public PayloadHeader
{
public:
	FramePayloadHeader()
	{
		_elements.push_back(&_dataVersion);
		_elements.push_back(&_frameCount);
		_elements.push_back(&_singleFrameDataSize);
		_elements.push_back(&_reserved);

		CalcSize();
	}
	~FramePayloadHeader(){}
	const DataVersion& Version() { return _dataVersion; }
	uint16_t FrameCount() { return _frameCount.Value(); }
	uint16_t SingleFrameDataSize() { return _singleFrameDataSize.Value(); }
private:
	DataVersion _dataVersion;
	WByteNumber _frameCount;
	WByteNumber _singleFrameDataSize;
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

		CalcSize();
	}
	~FrameInformation(){}
	const Coordinate& GetTopLeft() const { return _topLeft; }
	const Coordinate& GetBottomRight() const { return _buttomRight; }
	const Category& GetCategory() const { return _category; }
	const Status& GetStatus() const { return _status; }
	const AdditionalStatus& GetAdditiopnalStatus() const { return _additionalStatus; }
private:
	Coordinate _topLeft;
	Coordinate _buttomRight;
	Category _category;
	Status _status;
	AdditionalStatus _additionalStatus;
};


class ImageData : public BaseField
{
public:
	ImageData()
	{
		_elements.push_back(&_jpeg);
		_elements.push_back(&_padding);
	}
	~ImageData(){}
	void SetSize(size_t sz_jpg, size_t sz_pad) {
		_jpeg.SetSize(sz_jpg);
		_padding.SetSize(sz_pad);
		CalcSize();
	}
	const VariableSizeData* GetImage() const { return &_jpeg; }
private:
	VariableSizeData _jpeg;
	EmptyData _padding;
};

class FrameData : public BaseField
{
public:
	FrameData()
	{
	}
	~FrameData(){}
	void SetFrameNumber(int num)
	{
		_elements.clear();
		for (int i = 0; i < num; i++) {
			std::shared_ptr<FrameInformation> sptr = std::make_shared<FrameInformation>();
			_frameInformations.push_back(sptr);
			_elements.push_back(sptr.get());
		}
		_elements.push_back(&_reserved);

		CalcSize();
	}
	const std::list<std::shared_ptr<FrameInformation> >& Frames() const {
		return _frameInformations;
	}
private:
	std::list<std::shared_ptr<FrameInformation> > _frameInformations;
	FixedSizeEmptyData<5> _reserved;
};

class Payload : public BaseField
{
public:
	Payload(){}
	~Payload(){}
	virtual const PayloadHeader& GetPayloadHeader() const = 0;
};

class ImagePayload : public Payload
{
public:
	ImagePayload()
	{
		_elements.push_back(&_header);
		_elements.push_back(&_imageData);
	}
	~ImagePayload(){}
	const VariableSizeData* GetImage() const { return _imageData.GetImage(); }
	virtual const PayloadHeader& GetPayloadHeader() const {
		return _header;
	}
protected:
	virtual void Fulfilled(int idx) {
		if (idx == 0) {
			uint32_t jpgSize = _header.DataSize();
			uint8_t padSize = _header.PaddingSize();
			_imageData.SetSize(jpgSize, padSize);
			CalcSize();
		}
	}

private:
	ImagePayloadHeader _header;
	ImageData _imageData;
};

class FramePayload : public Payload
{
public:
	FramePayload()
	{
		_elements.push_back(&_header);
		_elements.push_back(&_frameData);
	}
	~FramePayload(){}
	virtual const PayloadHeader& GetPayloadHeader() const {
		return _header;
	}
	const FrameData& GetFrameData() const {
		return _frameData;
	}

protected:
	virtual void Fulfilled(int idx) {
		if (idx == 0) {
			_frameData.SetFrameNumber(_header.FrameCount());
			CalcSize();
		}
	}
private:
	FramePayloadHeader _header;
	FrameData _frameData;
};
