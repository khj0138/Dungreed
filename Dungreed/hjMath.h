#pragma once
#include <math.h>
#define PI 3.1415926535

struct Vector2
{
	static Vector2 Up;
	static Vector2 Right;

	static Vector2 One;
	static Vector2 Zero;

	float x;
	float y;

	Vector2()
		: x(0.0f)
		, y(0.0f)
	{

	}
	Vector2(float x, float y)
		: x(x)
		, y(y)
	{

	}
	Vector2(int x, int y)
		: x(x)
		, y(y)
	{

	}
	Vector2(const Vector2&) = default;
	Vector2& operator=(const Vector2&) = default;

	Vector2(Vector2&&) = default;
	Vector2& operator=(Vector2&&) = default;
	
	Vector2 operator+(const Vector2& other)
	{
		Vector2 temp;
		temp.x = x + other.x;
		temp.y = y + other.y;
		return temp;
	}
	Vector2 operator-(const Vector2& other)
	{
		Vector2 temp;
		temp.x = x - other.x;
		temp.y = y - other.y;
		return temp;
	}
	Vector2 operator*(const Vector2& other)
	{
		Vector2 temp;
		temp.x = x * other.x;
		temp.y = y * other.y;
		return temp;
	}
	Vector2 operator/(const Vector2& other)
	{
		if (other.x == 0 || other.y == 0)
			return Vector2{0.0f, 0.0f};
		Vector2 temp;
		temp.x = x / other.x;
		temp.y = y / other.y;
		return temp;
	}
	Vector2 operator*(const float& other)
	{
		Vector2 temp;
		temp.x = x * other;
		temp.y = y * other;
		return temp;
	}
	Vector2 operator/(const float& other)
	{
		if (other == 0)
			return Vector2{ 0.0f, 0.0f };
		Vector2 temp;
		temp.x = x / other;
		temp.y = y / other;
		return temp;
	}

	void operator+=(const Vector2& other)
	{
		x = x + other.x;
		y = y + other.y;
	}
	void operator-=(const Vector2& other)
	{
		x = x - other.x;
		y = y - other.y;
	}
	void operator*=(const Vector2& other)
	{
		x = x + other.x;
		y = y + other.y;
	}
	void operator/=(const Vector2& other)
	{
		if (other.x == 0 || other.y == 0)
			return;
		x = x / other.x;
		y = y / other.y;
	}

	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	Vector2& Normalize()
	{
		float length = Length();
		x /= length;
		y /= length;

		return *this;
	}
};

namespace hj::math
{
	inline static Vector2 Rotate(Vector2 vector, float degree)
	{
		
		float vectDegree = atan2(vector.y, vector.x);
		if (vectDegree + degree > 180.0f) 
		{
			degree = -360.f + degree;
		}
		else if (vectDegree + degree < -180.0f)
		{
			degree = 360.f + degree;
		}
		float radian = (degree  / 180.0f) * PI;
		vector.Normalize();
		float x = vector.x * cosf(radian) - vector.y * sinf(radian);
		float y = vector.x * sinf(radian) + vector.y * cosf(radian);
		
		return Vector2(x, y);
	}
}