#include "stdafx.h"
#include "Triangle3v.h"


CTriangle3v::CTriangle3v()
{
	MakeZero();
}


CTriangle3v::CTriangle3v(const CTriangle3v& tri)
{
	SetValue(tri.m_vecs);
}


CTriangle3v::CTriangle3v(const CVertex vecs[VERTICES])
{
	SetValue(vecs);
}


CTriangle3v::CTriangle3v(const CVertex& a, const CVertex& b, const CVertex& c)
{
	SetValue(a, b, c);
}


CTriangle3v::~CTriangle3v()
{
}


void CTriangle3v::SetValue(const CVertex vecs[VERTICES])
{
	SetValue(vecs[A], vecs[B], vecs[C]);
}


void CTriangle3v::SetValue(const CVertex& a, const CVertex& b, const CVertex& c)
{
	m_vecs[A] = a;
	m_vecs[B] = b;
	m_vecs[C] = c;
}


void CTriangle3v::SetNormal(const CVector3f& n)
{
	m_vecs[A].SetNormal(n);
	m_vecs[B].SetNormal(n);
	m_vecs[C].SetNormal(n);
}


void CTriangle3v::GetValue(CVertex vecs[VERTICES]) const
{
	GetValue(vecs[A], vecs[B], vecs[C]);
}


void CTriangle3v::GetValue(CVertex& a, CVertex& b, CVertex& c) const
{
	a = m_vecs[A];
	b = m_vecs[B];
	c = m_vecs[C];
}

CVector3f CTriangle3v::GetCenter() const
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	for(int i = 0; i < VERTICES; i++) {
		x += m_vecs[i][CVertex::VX];
		y += m_vecs[i][CVertex::VY];
		z += m_vecs[i][CVertex::VZ];
	}
	return CVector3f(x/3.0f, y/3.0f, z/3.0f);
}

CTriangle3v& CTriangle3v::operator=(const CTriangle3v& tri)
{
	SetValue(tri.m_vecs);
	return *this;
}


CVertex& CTriangle3v::operator[](int i)
{
	return m_vecs[i];
}


CVertex CTriangle3v::operator[](int i) const
{
	return m_vecs[i];
}


void CTriangle3v::MakeZero()
{
	for(int i = 0; i < VERTICES; i++) {
		m_vecs[i].MakeZero();
	}
}


bool operator==(const CTriangle3v& tri1, const CTriangle3v& tri2)
{
	bool equal;
	equal = (tri1[CTriangle3v::A] == tri2[CTriangle3v::A]) &&
	        (tri1[CTriangle3v::B] == tri2[CTriangle3v::B]) &&
	        (tri1[CTriangle3v::C] == tri2[CTriangle3v::C]);

	return equal;
}


bool operator!=(const CTriangle3v& tri1, const CTriangle3v& tri2)
{
	bool notEqual;
	notEqual = (tri1[CTriangle3v::A] != tri2[CTriangle3v::A]) ||
	           (tri1[CTriangle3v::B] != tri2[CTriangle3v::B]) ||
	           (tri1[CTriangle3v::C] != tri2[CTriangle3v::C]);

	return notEqual;
}
