#pragma once

#include "Vector3f.h"
#include "Vector2f.h"

class CVertex
{
public:
	enum {
		VX = 0,
		VY = 1,
		VZ = 2,
		NX = 3,
		NY = 4,
		NZ = 5,
		TX = 6,
		TY = 7,
		DIMENSION = 8
	};

private:
	float m_vec[DIMENSION];

public:
	CVertex();
	CVertex(const CVertex& vec);
	CVertex(const float vec[DIMENSION]);
	CVertex(const CVector3f& point, const CVector3f &normal, const CVector2f &tex);
	CVertex(float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	virtual ~CVertex();

	void SetValue(const float vec[DIMENSION]);
	void SetValue(const CVector3f& point, const CVector3f &normal, const CVector2f &tex);
	void SetValue(float vx, float vy, float vz, float nx, float ny, float nz, float tx, float ty);
	void SetNormal(const CVector3f& n);

	void GetValue(float vec[DIMENSION]) const;
	void GetValue(CVector3f &point, CVector3f& normal, CVector2f &tex) const;
	void GetValue(float& vx, float& vy, float& vz, float& nx, float& ny, float& nz, float& tx, float& ty) const;
	CVector3f GetNormal() const;
	CVector3f GetPoint() const;
	CVector2f GetTex() const;

	float& operator[](int i);
	float operator[](int i) const;

	CVertex& operator=(const CVertex& vec);

	float Length() const;

	void MakeZero();

	void Negate();

	float Normalize();

	friend bool operator==(const CVertex& vec1, const CVertex& vec2);
	friend bool operator!=(const CVertex& vec1, const CVertex& vec2);
};


