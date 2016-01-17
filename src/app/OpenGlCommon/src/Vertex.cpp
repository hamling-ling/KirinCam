#include "stdafx.h"
#include "Vertex.h"



CVertex::CVertex()
{
	MakeZero();
}


CVertex::CVertex(const CVertex& vec)
{
	SetValue(vec.m_vec);
}


CVertex::CVertex(const float vec[DIMENSION])
{
	SetValue(vec);
}


CVertex::CVertex(const CVector3f& point, const CVector3f &normal, const CVector2f &tex)
{
	SetValue(point, normal, tex);
}


CVertex::CVertex(float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	SetValue(vx , vy, vz, nx, ny, nz, tx, ty);
}


CVertex::~CVertex()
{
}


void CVertex::SetValue(const float vec[DIMENSION])
{
	for(int i = 0; i < DIMENSION; i++)
		m_vec[i] = vec[i];
}


void CVertex::SetValue(const CVector3f& point, const CVector3f &normal, const CVector2f &tex)
{
	SetValue(
		point[CVector3f::X],
		point[CVector3f::Y],
		point[CVector3f::Z],
		normal[CVector3f::X],
		normal[CVector3f::Y],
		normal[CVector3f::Z],
		tex[CVector2f::X],
		tex[CVector2f::Y]);
}


void CVertex::SetValue(float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty)
{
	m_vec[VX] = vx;
	m_vec[VY] = vy;
	m_vec[VZ] = vz;
	m_vec[NX] = nx;
	m_vec[NY] = ny;
	m_vec[NZ] = nz;
	m_vec[TX] = tx;
	m_vec[TY] = ty;
}


void CVertex::SetNormal(const CVector3f& n)
{
	m_vec[NX] = n[CVector3f::X];
	m_vec[NY] = n[CVector3f::Y];
	m_vec[NZ] = n[CVector3f::Z];
}


void CVertex::GetValue(float vec[DIMENSION]) const
{
	for(int i = 0; i < DIMENSION; i++)
		vec[i] = m_vec[i];
}


void CVertex::GetValue(CVector3f &point, CVector3f& normal, CVector2f &tex) const
{
	point.SetValue(m_vec[CVertex::VX], m_vec[CVertex::VY], m_vec[CVertex::VZ]);
	normal.SetValue(m_vec[CVertex::NX], m_vec[CVertex::NY], m_vec[CVertex::NZ]);
	tex.SetValue(m_vec[CVector2f::X], m_vec[CVector2f::Y]);
}


void CVertex::GetValue(float& vx, float& vy, float& vz, float& nx, float& ny, float& nz, float& tx, float& ty) const
{
	vx = m_vec[VX];
	vy = m_vec[VY];
	vz = m_vec[VZ];
	nx = m_vec[NX];
	ny = m_vec[NY];
	nz = m_vec[NZ];
	tx = m_vec[TX];
	ty = m_vec[TY];
}


CVector3f CVertex::GetNormal() const 
{
	return CVector3f(&(m_vec[NX]));
}


CVector3f CVertex::GetPoint() const
{

	return CVector3f(&(m_vec[VX]));
}


CVector2f CVertex::GetTex() const
{
	return CVector2f(&(m_vec[TX]));
}


CVertex& CVertex::operator=(const CVertex& vec)
{
	SetValue(vec.m_vec);
	return *this;
}


float& CVertex::operator[](int i)
{
	return m_vec[i];
}


float CVertex::operator[](int i) const
{
	return m_vec[i];
}


void CVertex::MakeZero()
{
	for(int i = 0; i < DIMENSION; i++)
		m_vec[i] = 0.0f;
}


bool operator==(const CVertex& vec1, const CVertex& vec2)
{
	for(int i = 0; i < CVertex::DIMENSION; i++)
		if(vec1.m_vec[i] != vec2.m_vec[i])
			return false;

	return true;
}


bool operator!=(const CVertex& vec1, const CVertex& vec2)
{
	bool notEqual = !(vec1 == vec2);

	return notEqual;
}
