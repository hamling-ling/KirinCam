#pragma once

#include "CommonHeader.h"
#include "Payloads.h"

class LiveViewPacket : public BaseField
{
public:
	LiveViewPacket();
	~LiveViewPacket();
	virtual bool Fill(boost::asio::streambuf& buf);
	const VariableSizeData* GetImage() const;
	const std::shared_ptr<CommonHeader>& GetHeader() const;
private:
	std::shared_ptr<CommonHeader> commonHeader_;
	std::shared_ptr<Payload> payload_;

	void SkipUntil(char c, boost::asio::streambuf& buf);
};
