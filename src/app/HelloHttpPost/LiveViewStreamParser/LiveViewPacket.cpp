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

const VariableSizeData* LiveViewPacket::GetImage() const {
	if (commonHeader_->PayloadType() != kPayloadTypeLiveViewImages) {
		return NULL;
	}
	std::shared_ptr<ImagePayload> sptr = std::dynamic_pointer_cast<ImagePayload>(payload_);
	return sptr->GetImage();
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
