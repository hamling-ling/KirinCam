#pragma once

#include "Vertex.h"

class CTriangle3v
{
public:
	enum {
		A = 0,
		B = 1,
		C = 2,
		VERTICES = 3
	};

private:
	CVertex m_vecs[VERTICES];

public:
	CTriangle3v();
	CTriangle3v(const CTriangle3v& vec);
	CTriangle3v(const CVertex vecs[VERTICES]);
	CTriangle3v(const CVertex& a, const CVertex& b, const CVertex& c);
	virtual ~CTriangle3v();

	void SetValue(const CVertex vecs[VERTICES]);
	void SetValue(const CVertex& a, const CVertex& b, const CVertex& c);
	void SetNormal(const CVector3f& n);

	void GetValue(CVertex vecs[VERTICES]) const;
	void GetValue(CVertex& a, CVertex& b, CVertex& c) const;

	CVector3f GetCenter() const;

	CVertex& operator[](int i);
	CVertex operator[](int i) const;

	CTriangle3v& operator=(const CTriangle3v& tri);

	void MakeZero();

	friend bool operator==(const CTriangle3v& tri1, const CTriangle3v& tri2);
	friend bool operator!=(const CTriangle3v& tri1, const CTriangle3v& tri2);
};

