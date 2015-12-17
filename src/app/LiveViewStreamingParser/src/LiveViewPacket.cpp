#include "stdafx.h"
#include "LiveViewPacket.h"
#include <istream>

using namespace std;

LiveViewPacket::LiveViewPacket()
{
	commonHeader_ = std::make_shared<CommonHeader>();
	_elements.push_back(commonHeader_.get());
	// here we do not push payload element yet
}

LiveViewPacket::~LiveViewPacket() {}

bool LiveViewPacket::Fill(boost::asio::streambuf& buf)
{
	SkipUntil(0xFF, buf);

	if (!commonHeader_->IsFull()) {
		if (!commonHeader_->Fill(buf)) {
			return false;
		}
		CalcPos();
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

		bool fillResult = payload_->Fill(buf);
		CalcPos();
		if (payload_->IsFull()) {
			CalcSize();
		}

		return fillResult;
	}
	return true;
}

const VariableSizeData* LiveViewPacket::GetImage() const {
	if (commonHeader_->PayloadType() != kPayloadTypeLiveViewImages) {
		return NULL;
	}
	std::shared_ptr<ImagePayload> sptr = std::dynamic_pointer_cast<ImagePayload>(payload_);
	return sptr->GetImage();
}

const shared_ptr<CommonHeader>& LiveViewPacket::GetHeader() const {
	return commonHeader_;
}

const shared_ptr<Payload>& LiveViewPacket::GetPayload() const {
	return payload_;
}

void LiveViewPacket::SkipUntil(char c, boost::asio::streambuf& buf) {
	if (Pos() == 0) {
		/* ì™èoÇµ */
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
