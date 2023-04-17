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
	Vector2(UINT32 x, UINT32 y)
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
	Vector2 operator-(const float& other)
	{
		Vector2 temp;
		temp.x = x - other;
		temp.y = y - other;
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
	void operator+=(const float& other)
	{
		x = x + other;
		y = y + other;
	}
	void operator-=(const float& other)
	{
		x = x - other;
		y = y - other;
	}
	void operator*=(const float& other)
	{
		x = x * other;
		y = y * other;
	}
	void operator/=(const float& other)
	{
		if (other == 0.0f)
			return;
		x = x / other;
		y = y / other;
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

	bool operator==(const Vector2& other)
	{
		return (x == other.x && y == other.y);
	}
	bool operator<(const Vector2& other)
	{
		return (x < other.x && y < other.y);
	}
	bool operator>(const Vector2& other)
	{
		return (x > other.x && y > other.y);
	}
	void Clear()
	{
		x = 0.0f;
		y = 0.0f;
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
		float radian = (degree  / 180.0f) * PI;
		//vector.Normalize();
		float x = vector.x * cosf(radian) - vector.y * sinf(radian);
		float y = vector.x * sinf(radian) + vector.y * cosf(radian);
		
		return Vector2(x, y);
	}

	inline static float Dot(Vector2& v1, Vector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	inline static float Cross(Vector2& v1, Vector2& v2)
	{
		return v1.x * v2.y - v1.y * v2.x;
	}
	
	inline static float CCW(Vector2& p1, Vector2& p2, Vector2& target)
	{
		Vector2 a = p1 - target;
		Vector2 b = p2 - target;
		return Cross(a,b);
	}

	inline static void Swap(Vector2& p1, Vector2& p2)
	{
		Vector2 temp = Vector2{ p1.x,p1.y };
		p1 = Vector2{ p2.x, p2.y };
		p2 = temp;
	}

	inline static bool Line_Collision(Vector2 Line1_p1, Vector2 Line1_p2, Vector2 Line2_p1, Vector2 Line2_p2)
	{
		float Line1 = CCW(Line1_p1, Line1_p2,Line2_p1) * CCW(Line1_p1, Line1_p2,Line2_p2);
		float Line2 = CCW(Line2_p1, Line2_p2,Line1_p1) * CCW(Line2_p1, Line2_p2,Line1_p2);

		if (Line1 == 0.0f && Line2 == 0.0f)
		{
			if (Line1_p1 > Line1_p2)
				Swap(Line1_p1, Line1_p2);
			if (Line2_p1 > Line2_p2)
				Swap(Line2_p1, Line2_p2);

			return !(Line2_p2 < Line1_p1 || Line2_p1 < Line1_p2);
		}
		return (Line1 <= 0.0f && Line2 <= 0.0f);
	}
	inline static bool Intersection_Lines(Vector2 Line1_p1, Vector2 Line1_p2, Vector2 Line2_p1, Vector2 Line2_p2, Vector2& target)
	{
		if (Line_Collision(Line1_p1, Line1_p2, Line2_p1, Line2_p2))
		{
			Vector2 LVector1 = (Line1_p2 - Line1_p1);
			Vector2 LVector2 = (Line2_p2 - Line2_p1);
			float det = Cross(LVector1, LVector2);
			if (fabs(det) < 0.0001f)
			{
				if (Line1_p1 > Line1_p2)
					Swap(Line1_p1, Line1_p2);
				if (Line2_p1 > Line2_p2)
					Swap(Line2_p1, Line2_p2);
				if (Line1_p1 > Line2_p1)
					target = Line1_p1;
				else
					target = Line2_p1;
				return true;
			
			}
			else
			{
				Vector2 temp = (Line2_p1 - Line1_p1);
				target = Line1_p1 + (Line1_p2 - Line1_p1) * (Cross(temp,LVector2) / det);
				return true;
			}
		}
	}
}