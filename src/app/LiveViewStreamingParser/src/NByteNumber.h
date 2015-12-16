#pragma once

#include "FixedSizeData.h"
#include <algorithm>
#include <vector>

class SingleByteNumber : public FixedSizeData<1>
{
public:
	SingleByteNumber();
	~SingleByteNumber();
	uint8_t Value() const ;
protected:
	void Filled();
private:
	uint8_t _value;
};

class WByteNumber : public FixedSizeData<2>
{
public:
	WByteNumber();
	~WByteNumber();
	uint16_t Value() const;
protected:
	void Filled();
private:
	uint16_t _value;
};

class TriByteNumber : public FixedSizeData<3>
{
public:
	TriByteNumber() : _value(0) {}
	~TriByteNumber(){}
	uint32_t Value() const { return _value; }
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		const uint8_t val0 = *(data + 0);
		const uint8_t val1 = *(data + 1);
		const uint8_t val2 = *(data + 2);
		// todo:endian
		_value = val0 << 16 | val1 << 8 | val2;
	}
private:
	uint32_t _value;
};

class QuadByteNumber : public FixedSizeData<4>
{
public:
	QuadByteNumber() : _value(0) {}
	~QuadByteNumber(){}
	uint32_t Value() const { return _value; }
protected:
	void Filled()
	{
		const uint8_t *data = Data();
		const uint8_t val0 = *(data + 0);
		const uint8_t val1 = *(data + 1);
		const uint8_t val2 = *(data + 2);
		const uint8_t val3 = *(data + 3);
		// todo:endian
		_value = val0 << 24 | val1 << 16 | val2 << 8 | val3;
	}
private:
	uint32_t _value;
};


template<typename T>
class ByteEnum : public FixedSizeData<1>
{
public:
	ByteEnum(){}
	~ByteEnum(){}
	T Value() const { return _value; }
	virtual bool Fill(boost::asio::streambuf& buf)
	{
		const T* cp = boost::asio::buffer_cast<const T*>(buf.data());
		const std::vector<T>& allKinds = AllKinds();

		std::vector<T>::const_iterator it = std::find(allKinds.begin(), allKinds.end(), *cp);
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
