#pragma once
#include <cmath>

#define Epsilon 0.001f
#define PI 3.141592653f
#define DtoR(degree) (degree) / 180.0f * PI
#define RtoD(radian) (radian) * 180.0f / PI

struct Vector2
{
	float x, y;
	Vector2();
	Vector2(float fx, float fy);
	Vector2(const Vector2& rhs);
	Vector2 operator ^ (const Vector2& v0);
	float operator | (const Vector2& v0);
};

struct Vector3
{
	float x, y, z;
	Vector3();
	Vector3(float fx, float fy, float fz);
	Vector3(const Vector3& rhs);

	bool operator == (const Vector3& v0);
	float operator | (const Vector3& v0);
	Vector3 operator ^ (const Vector3& v0);
	Vector3 operator + (const Vector3& v0);
	Vector3 operator - (const Vector3& v0);
	Vector3 operator - ();

	template <typename X>
	inline Vector3 operator * (const X& Scala)
	{
		return Vector3(x * Scala, y * Scala, z * Scala);
	}
	inline Vector3 Normalize()
	{
		float invert = 1.0f / Length();
		return Vector3(x * invert, y * invert, z * invert);
	}
	inline Vector3& Normal()
	{
		if (Length() - 0.0f < 0.001f)
		{
			return *this;
		}
		float invert = 1.0f / Length();
		x *= invert;
		y *= invert;
		z *= invert;
		return *this;
	}

	inline float Length()
	{
		return sqrt(x * x + y * y + z * z);
	}
};

struct Vector4
{
	Vector4();
	Vector4(float fx, float fy, float fz, float fw);
	Vector4(const Vector4& rhs);
	float x, y, z, w;
};