#pragma once

#include "Vector3f.h"

class CLine
{
public:
		enum {
		A = 0,
		B = 1,
		VERTICES = 2
	};

private:
	CVector3f m_vecs[VERTICES];

public:
	CLine();
	CLine(const CLine& vec);
	CLine(const CVector3f vecs[VERTICES]);
	CLine(const CVector3f& a, const CVector3f& b);
	virtual ~CLine();

	void SetValue(const CVector3f vecs[VERTICES]);
	void SetValue(const CVector3f& a, const CVector3f& b);

	void GetValue(CVector3f vecs[VERTICES]) const;
	void GetValue(CVector3f& a, CVector3f& b) const;

	CVector3f& operator[](int i);
	CVector3f operator[](int i) const;

	CLine& operator=(const CLine& tri);

	void MakeZero();

	friend bool operator==(const CLine& vec1, const CLine& vec2);
	friend bool operator!=(const CLine& vec1, const CLine& vec2);
};

