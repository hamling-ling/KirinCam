#include "stdafx.h"
#include "Line.h"


CLine::CLine(void)
{
	MakeZero();
}

CLine::CLine(const CLine& tri)
{
	SetValue(tri.m_vecs);
}


CLine::CLine(const CVector3f vecs[VERTICES])
{
	SetValue(vecs);
}


CLine::CLine(const CVector3f& a, const CVector3f& b)
{
	SetValue(a, b);
}


CLine::~CLine()
{
}


void CLine::SetValue(const CVector3f vecs[VERTICES])
{
	SetValue(vecs[0], vecs[1]);
}


void CLine::SetValue(const CVector3f& a, const CVector3f& b)
{
	m_vecs[A] = a;
	m_vecs[B] = b;
}


void CLine::GetValue(CVector3f vecs[VERTICES]) const
{
	GetValue(vecs[A], vecs[B]);
}


void CLine::GetValue(CVector3f& a, CVector3f& b) const
{
	a = m_vecs[A];
	b = m_vecs[B];
}


CLine& CLine::operator=(const CLine& tri)
{
	SetValue(tri.m_vecs);
	return *this;
}


CVector3f& CLine::operator[](int i)
{
	return m_vecs[i];
}


CVector3f CLine::operator[](int i) const
{
	return m_vecs[i];
}


void CLine::MakeZero()
{
	CVector3f zero(0.0f, 0.0f, 0.0f);
	SetValue(zero, zero);
}


bool operator==(const CLine& l1, const CLine& l2)
{
	bool equal;
	equal = (l1[CLine::A] == l2[CLine::A]) &&
	        (l1[CLine::B] == l2[CLine::B]);

	return equal;
}


bool operator!=(const CLine& l1, const CLine& l2)
{
	bool notEqual;
	notEqual = (l1[CLine::A] != l2[CLine::A]) ||
	           (l1[CLine::B] != l2[CLine::B]);

	return notEqual;
}
