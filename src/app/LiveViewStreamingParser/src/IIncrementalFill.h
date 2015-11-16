#pragma once


#include <cstdint>
#include <boost/asio.hpp>

#define UNDEFINED_SIZE	0xFFFFFFFF

class IIncrementalFill
{
public:
	IIncrementalFill();
	virtual ~IIncrementalFill() = 0;
	bool IsFull();
	/**
	Fill returns false when:
	- unexpected data
	- fill called when full
	- returns true even if input data is empty
	*/
	virtual bool Fill(boost::asio::streambuf& buf) = 0;
	size_t Size() const;
	void SetSize(uint32_t sz);
	uint32_t Pos() const;
protected:
	void SetPos(uint32_t pos);
	virtual void Filled() = 0;
private:
	uint32_t _pos;
	size_t _size;
};
