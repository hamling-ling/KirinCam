#include "stdafx.h"
#include <cmath>
#include "Vector2f.h"
#include "OpenGlCommonDefs.h"


CVector2f::CVector2f()
{
	MakeZero();
}


CVector2f::CVector2f(const CVector2f& vec)
{
	SetValue(vec.m_vec);
}


CVector2f::CVector2f(const float vec[DIMENSION])
{
	SetValue(vec);
}


CVector2f::CVector2f(float x, float y)
{
	SetValue(x, y);
}


CVector2f::~CVector2f()
{
}


void CVector2f::SetValue(const float vec[DIMENSION])
{
	SetValue(vec[X], vec[Y]);
}


void CVector2f::SetValue(float x, float y)
{
	m_vec[X] = x;
	m_vec[Y] = y;
}


void CVector2f::GetValue(float vec[DIMENSION]) const
{
	GetValue(vec[X], vec[Y]);
}


void CVector2f::GetValue(float&x, float& y) const
{
	x = m_vec[X];
	y = m_vec[Y];
}


CVector2f& CVector2f::operator=(const CVector2f& vec)
{
	SetValue(vec.m_vec);
	return *this;
}


float& CVector2f::operator[](int i)
{
	return m_vec[i];
}


float CVector2f::operator[](int i) const
{
	return m_vec[i];
}


CVector2f& CVector2f::operator*=(float scalar)
{
	for (int i = 0; i < DIMENSION; i++) {
		m_vec[i] *= scalar;
	}

	return *this;
}


CVector2f& CVector2f::operator/=(float scalar)
{
	if (scalar == 0.0f) {
		scalar = 1.0f;
	}
	for (int i = 0; i < DIMENSION; i++) {
		m_vec[i] /= scalar;
	}

	return *this;
}


CVector2f& CVector2f::operator+=(const CVector2f& vec)
{
	for (int i = 0; i < DIMENSION; i++) {
		m_vec[i] += vec[i];
	}

	return *this;
}


CVector2f& CVector2f::operator-=(const CVector2f& vec)
{
	for (int i = 0; i < DIMENSION; i++) {
		m_vec[i] -= vec[i];
	}

	return *this;
}


CVector2f CVector2f::operator-() const
{
	CVector2f vec;
	for (int i = 0; i < DIMENSION; i++) {
		vec.m_vec[i] = - m_vec[i];
	}

	return vec;
}


CVector2f CVector2f::operator+() const
{
	CVector2f vec;
	for (int i = 0; i < DIMENSION; i++) {
		vec.m_vec[i] = + m_vec[i];
	}

	return vec;
}


float CVector2f::Dot(const CVector2f& vec) const
{
	float dot = 0.0f;
	for (int i = 0; i < DIMENSION; i++) {
		dot += m_vec[i] * vec[i];
	}

	return dot;
}


CVector2f CVector2f::Cross(const CVector2f& vec) const
{
	CVector2f cross;
	for (int i = 0; i < DIMENSION; i++) {
		cross[i] = m_vec[(i + 1) % DIMENSION] * vec[(i + 2) % DIMENSION] -
		           m_vec[(i + 2) % DIMENSION] * vec[(i + 1) % DIMENSION];
	}

	return cross;
}


float CVector2f::Length() const
{
	float dot = 0.0f;
	for (int i = 0; i < DIMENSION; i++) {
		dot += m_vec[i] * m_vec[i];
	}

	return (float)sqrt(dot);
}


void CVector2f::MakeZero()
{
	SetValue(0.0f, 0.0f);
}


void CVector2f::Negate()
{
	for (int i = 0; i < DIMENSION; i++) {
		m_vec[i] = - m_vec[i];
	}
}


float CVector2f::Normalize()
{
	float length = Length();

	if (length != 0.0f) {
		for (int i = 0; i < DIMENSION; i++) {
			m_vec[i] /= length;
		}
	}

	return length;
}


float CVector2f::Angle(const CVector2f& vec) const
{
	CVector2f normal1(m_vec);
	normal1.Normalize();
	CVector2f normal2(vec.m_vec);
	normal2.Normalize();

	float cos = normal1.Dot(normal2);
	float sin = normal1.Cross(normal2).Length();

	return (atan2f(sin, cos) * 180.0f / PI);
}


CVector2f operator+(const CVector2f& vec1, const CVector2f& vec2)
{
	CVector2f vec;
	for (int i = 0; i < CVector2f::DIMENSION; i++) {
		vec[i] = vec1[i] + vec2[i];
	}

	return vec;
}


CVector2f operator-(const CVector2f& vec1, const CVector2f& vec2)
{
	CVector2f vec;
	for (int i = 0; i < CVector2f::DIMENSION; i++) {
		vec[i] = vec1[i] - vec2[i];
	}

	return vec;
}


CVector2f operator*(float scalar, const CVector2f& vec2)
{
	CVector2f vec;
	for (int i = 0; i < CVector2f::DIMENSION; i++) {
		vec[i] = scalar * vec2[i];
	}

	return vec;
}


CVector2f operator*(const CVector2f& vec1, float scalar)
{
	CVector2f vec;
	for (int i = 0; i < CVector2f::DIMENSION; i++) {
		vec[i] = vec1[i] * scalar;
	}

	return vec;
}


CVector2f operator/(const CVector2f& vec1, float scalar)
{
	CVector2f vec;
	if (scalar == 0.0f) {
		scalar = 1.0f;
	}
	for (int i = 0; i < CVector2f::DIMENSION; i++) {
		vec[i] = vec1[i] / scalar;
	}

	return vec;
}


bool operator==(const CVector2f& vec1, const CVector2f& vec2)
{
	bool equal;
	equal = (vec1.m_vec[CVector2f::X] == vec2.m_vec[CVector2f::X]) &&
	        (vec1.m_vec[CVector2f::Y] == vec2.m_vec[CVector2f::Y]);

	return equal;
}


bool operator!=(const CVector2f& vec1, const CVector2f& vec2)
{
	bool notEqual;
	notEqual = (vec1.m_vec[CVector2f::X] != vec2.m_vec[CVector2f::X]) ||
	           (vec1.m_vec[CVector2f::Y] != vec2.m_vec[CVector2f::Y]);

	return notEqual;
}
