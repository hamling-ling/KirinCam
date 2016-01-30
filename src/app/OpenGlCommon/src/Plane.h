#pragma once

#include "Vector3f.h"

class CPlane
{
public:
	enum {
		N = 0,
		P = 1,
		VERTICES = 2
	};

private:
	CVector3f m_vecs[VERTICES];

public:
	CPlane();
	CPlane(const CPlane& p);
	CPlane(const CVector3f vecs[VERTICES]);
	CPlane(const CVector3f& n, const CVector3f& p);
	virtual ~CPlane();

	void SetValue(const CVector3f vecs[VERTICES]);
	void SetValue(const CVector3f& n, const CVector3f& p);

	void GetValue(CVector3f vecs[VERTICES]) const;
	void GetValue(CVector3f& n, CVector3f& p) const;

	CVector3f& operator[](int i);
	CVector3f operator[](int i) const;

	CPlane& operator=(const CPlane& tri);

	void MakeZero();

	friend bool operator==(const CPlane& p1, const CPlane& p2);
	friend bool operator!=(const CPlane& p1, const CPlane& p2);
};


