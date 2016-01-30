#pragma once

class CVector2f
{
public:
	enum {
		X = 0,
		Y = 1,
		DIMENSION = 2
	};

private:
	float m_vec[DIMENSION];

public:
	CVector2f();
	CVector2f(const CVector2f& vec);
	CVector2f(const float vec[DIMENSION]);
	CVector2f(float x, float y);
	virtual ~CVector2f();

	void SetValue(const float vec[DIMENSION]);
	void SetValue(float x, float yz);

	void GetValue(float vec[DIMENSION]) const;
	void GetValue(float& x, float& y) const;

	float& operator[](int i);
	float operator[](int i) const;

	CVector2f& operator=(const CVector2f& vec);

	CVector2f& operator*=(float scalar);
	CVector2f& operator/=(float scalar);

	CVector2f& operator+=(const CVector2f& vec);
	CVector2f& operator-=(const CVector2f& vec);

	CVector2f operator-() const;
	CVector2f operator+() const;

	float Dot(const CVector2f& vec) const;
	CVector2f Cross(const CVector2f& vec) const;

	float Length() const;

	void MakeZero();

	void Negate();

	float Normalize();

	float Angle(const CVector2f& vec) const;

	friend CVector2f operator+(const CVector2f& vec1, const CVector2f& vec2);
	friend CVector2f operator-(const CVector2f& vec1, const CVector2f& vec2);

	friend CVector2f operator*(float scalar, const CVector2f& vec2);
	friend CVector2f operator*(const CVector2f& vec1, float scalar);
	friend CVector2f operator/(const CVector2f& vec1, float scalar);

	friend bool operator==(const CVector2f& vec1, const CVector2f& vec2);
	friend bool operator!=(const CVector2f& vec1, const CVector2f& vec2);
};


