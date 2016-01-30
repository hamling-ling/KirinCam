#include "BaseField.h"

BaseField::BaseField() {}

BaseField::~BaseField(){}

bool BaseField::Fill(boost::asio::streambuf& buf)
{
	if (IsFull()) {
		return false;
	}

	if (Pos() == 0) {
		_it = _elements.begin();
	}

	while (buf.size() > 0 && _it != _elements.end()) {
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
			Fulfilled(std::distance(_elements.begin(), _it));
			_it++;
			continue;
		}
		break;
	}
	CalcPos();
	return true;
}

void BaseField::CalcSize()
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

void BaseField::CalcPos()
{
	uint32_t sum = 0;
	for (auto v : _elements) {
		sum += v->Pos();
	}
	SetPos(sum);
}

void BaseField::Fulfilled(int idx) { }
