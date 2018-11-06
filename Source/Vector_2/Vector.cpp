#include "Vector.h"

Vector2::Vector2()
{}
Vector2::Vector2(float fx, float fy) : x(fx), y(fy)
{}
Vector2::Vector2(const Vector2& rhs) : x(rhs.x), y(rhs.y)
{}

Vector2 Vector2::operator ^ (const Vector2& v0)
{
	return Vector2(0, (x * v0.y - y * v0.x));
}
float Vector2::operator | (const Vector2& v0)
{
	return (x * v0.x) + (y * v0.y);
}


////////////////////////////////////////////////////////////////////////////////


Vector3::Vector3()
{

}
Vector3::Vector3(float fx, float fy, float fz) : x(fx), y(fy), z(fz)
{

}
Vector3::Vector3(const Vector3& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{

}

bool Vector3::operator == (const Vector3& v0)
{
	if (fabs(x - v0.x) < Epsilon)
	{
		if (fabs(y - v0.y) < Epsilon)
		{
			if (fabs(z - v0.z) < Epsilon)
			{
				return true;
			}
		}
	}
	return false;
}
float Vector3::operator | (const Vector3& v0)
{
	return ((x * v0.x) + (y * v0.y) + (z * v0.z));
}
Vector3 Vector3::operator ^ (const Vector3& v0)
{
	return Vector3(
		y * v0.z - z * v0.y, 
		z * v0.x - x * v0.z, 
		x * v0.y - y * v0.x);
}
Vector3 Vector3::operator + (const Vector3& v0)
{
	return Vector3(v0.x + x, v0.y + y, v0.z + z);
}
Vector3 Vector3::operator - (const Vector3& v0)
{
	return Vector3(x - v0.x, y - v0.y, z - v0.z);
}
Vector3 Vector3::operator - ()
{
	Vector3 temp = *this;
	return temp * -1;
}

////////////////////////////////////////////////////////////////////////////////


Vector4::Vector4()
{

}
Vector4::Vector4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw)
{
}
Vector4::Vector4(const Vector4& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{

}