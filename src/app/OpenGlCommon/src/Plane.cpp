#include "stdafx.h"
#include "Plane.h"


CPlane::CPlane()
{
	MakeZero();
}


CPlane::CPlane(const CPlane& p)
{
	SetValue(p.m_vecs);
}


CPlane::CPlane(const CVector3f vecs[VERTICES])
{
	SetValue(vecs);
}


CPlane::CPlane(const CVector3f& n, const CVector3f& p)
{
	SetValue(n,p);
}


CPlane::~CPlane()
{
}


void CPlane::SetValue(const CVector3f vecs[VERTICES])
{
	SetValue(vecs[N], vecs[P]);
}


void CPlane::SetValue(const CVector3f& n, const CVector3f& p)
{
	m_vecs[N] = n;
	m_vecs[P] = p;
}


void CPlane::GetValue(CVector3f vecs[VERTICES]) const
{
	GetValue(vecs[N], vecs[P]);
}


void CPlane::GetValue(CVector3f& n, CVector3f& p) const
{
	n = m_vecs[N];
	p = m_vecs[P];
}


CPlane& CPlane::operator=(const CPlane& tri)
{
	SetValue(tri.m_vecs);
	return *this;
}


CVector3f& CPlane::operator[](int i)
{
	return m_vecs[i];
}


CVector3f CPlane::operator[](int i) const
{
	return m_vecs[i];
}


void CPlane::MakeZero()
{
	for(int i = 0; i < VERTICES; i++) {
		m_vecs[i].MakeZero();
	}
}


bool operator==(const CPlane& p1, const CPlane& p2)
{
	bool equal;
	equal = (p1[CPlane::N] == p2[CPlane::N]) &&
	        (p1[CPlane::P] == p2[CPlane::P]);

	return equal;
}


bool operator!=(const CPlane& p1, const CPlane& p2)
{
	bool notEqual;
	notEqual = (p1[CPlane::N] != p2[CPlane::N]) ||
	           (p1[CPlane::P] != p2[CPlane::P]);

	return notEqual;
}
