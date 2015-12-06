#include "stdafx.h"
#include "IIncrementalFill.h"


IIncrementalFill::IIncrementalFill() : _pos(0), _size(UNDEFINED_SIZE) {}
IIncrementalFill::~IIncrementalFill(){}
bool IIncrementalFill::IsFull() {
	return _size <= _pos;
}
size_t IIncrementalFill::Size() const { return _size; }
void IIncrementalFill::SetSize(uint32_t sz) { _size = sz; }
uint32_t IIncrementalFill::Pos() const { return _pos; }
void IIncrementalFill::SetPos(uint32_t pos) { _pos = pos; }
