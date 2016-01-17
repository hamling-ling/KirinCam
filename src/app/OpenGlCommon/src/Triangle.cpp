#include "stdafx.h"
#include "Triangle.h"


CTriangle::CTriangle()
{
	MakeZero();
}


CTriangle::CTriangle(const CTriangle& tri)
{
	SetValue(tri.m_vecs);
}


CTriangle::CTriangle(const CVector3f vecs[VERTICES])
{
	SetValue(vecs);
}


CTriangle::CTriangle(CVector3f a, CVector3f b, CVector3f c)
{
	SetValue(a, b, c);
}


CTriangle::~CTriangle()
{
}


void CTriangle::SetValue(const CVector3f vecs[VERTICES])
{
	SetValue(vecs[0], vecs[1], vecs[2]);
}


void CTriangle::SetValue(CVector3f a, CVector3f b, CVector3f c)
{
	m_vecs[A] = a;
	m_vecs[B] = b;
	m_vecs[C] = c;
}


void CTriangle::GetValue(CVector3f vecs[VERTICES]) const
{
	GetValue(vecs[A], vecs[B], vecs[C]);
}


void CTriangle::GetValue(CVector3f& a, CVector3f& b, CVector3f& c) const
{
	a = m_vecs[A];
	b = m_vecs[B];
	c = m_vecs[C];
}

CVector3f CTriangle::GetCenter()
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	for(int i = 0; i < VERTICES; i++) {
		x += m_vecs[i][CVector3f::X];
		y += m_vecs[i][CVector3f::Y];
		z += m_vecs[i][CVector3f::Z];
	}
	return CVector3f(x/3.0f, y/3.0f, z/3.0f);
}

CTriangle& CTriangle::operator=(const CTriangle& tri)
{
	SetValue(tri.m_vecs);
	return *this;
}


CVector3f& CTriangle::operator[](int i)
{
	return m_vecs[i];
}


CVector3f CTriangle::operator[](int i) const
{
	return m_vecs[i];
}


void CTriangle::MakeZero()
{
	CVector3f zero(0.0f, 0.0f, 0.0f);
	SetValue(zero, zero, zero);
}


bool operator==(const CTriangle& tri1, const CTriangle& tri2)
{
	bool equal;
	equal = (tri1[CTriangle::A] == tri2[CTriangle::A]) &&
	        (tri1[CTriangle::B] == tri2[CTriangle::B]) &&
	        (tri1[CTriangle::C] == tri2[CTriangle::C]);

	return equal;
}


bool operator!=(const CTriangle& tri1, const CTriangle& tri2)
{
	bool notEqual;
	notEqual = (tri1[CTriangle::A] != tri2[CTriangle::A]) ||
	           (tri1[CTriangle::B] != tri2[CTriangle::B]) ||
	           (tri1[CTriangle::C] != tri2[CTriangle::C]);

	return notEqual;
}
