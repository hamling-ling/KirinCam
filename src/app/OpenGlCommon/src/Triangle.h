#ifndef _TRIANGLE3F_H_
#define _TRIANGLE3F_H_

#include "Vector3f.h"


class CTriangle
{
public:
	enum {
		A = 0,
		B = 1,
		C = 2,
		VERTICES = 3
	};

private:
	CVector3f m_vecs[VERTICES];

public:
	CTriangle();
	CTriangle(const CTriangle& vec);
	CTriangle(const CVector3f vecs[VERTICES]);
	CTriangle(CVector3f x, CVector3f y, CVector3f z);
	virtual ~CTriangle();

	void SetValue(const CVector3f vecs[VERTICES]);
	void SetValue(CVector3f a, CVector3f b, CVector3f c);

	void GetValue(CVector3f vecs[VERTICES]) const;
	void GetValue(CVector3f& a, CVector3f& b, CVector3f& c) const;

	CVector3f GetCenter();

	CVector3f& operator[](int i);
	CVector3f operator[](int i) const;

	CTriangle& operator=(const CTriangle& tri);

	void MakeZero();

	friend bool operator==(const CTriangle& vec1, const CTriangle& vec2);
	friend bool operator!=(const CTriangle& vec1, const CTriangle& vec2);
};


#endif // _TRIANGLE3F_H_
