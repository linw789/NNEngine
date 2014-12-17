#ifndef NNE_VEC4_H
#define NNE_VEC4_H

#include "DebugUtilities.h"
#include "Vec3.hpp"

namespace NNE{

class Vec4
{
public:

	// could implement swizzle in future
	// union provides more entries with no overhead
	union 
	{
		float m_values[4];
		
		struct{float x, y, z, w;};

		struct{float r, g, b, a;};
	};

	// constructors
	Vec4()
		:x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{}

	Vec4(float v0, float v1, float v2, float v3)
		:x(v0), y(v1), z(v2), w(v3)
	{}

	Vec4(const Vec4 &other, float val)
		:x(other.x), y(other.y), z(other.z), w(val)
	{}

	Vec4(const Vec4 &other)
		:x(other.x), y(other.y), z(other.z), w(other.w)
	{}

	Vec4(const Vec3 &v3, NNEfloat val)
		:x(v3.x), y(v3.y), z(v3.z), w(val)
	{}

	// operator override
	Vec4 &operator = (const Vec4 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}

	Vec4 &operator += (const Vec4 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	Vec4 &operator -= (const Vec4 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	Vec4 &operator *= (const Vec4 &other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	Vec4 &operator += (float val)
	{
		x += val;
		y += val;
		z += val;
		w += val;

		return *this;
	}

	Vec4 &operator -= (float val)
	{
		x -= val;
		y -= val;
		z -= val;
		w -= val;

		return *this;
	}

	Vec4 &operator *= (float val)
	{
		x *= val;
		y *= val;
		y *= val;
		w *= val;

		return *this;
	}

	Vec4 &operator /= (float val)
	{
		float tmp = 1 / val;
		x *= tmp;
		y *= tmp;
		z *= tmp;
		w *= tmp;

		return *this;
	}

	bool operator == (const Vec4 &other)
	{
		return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
	}

	bool operator != (const Vec4 &other)
	{
		return (x != other.x) || (y != other.y) || (z != other.z) || (w != other.w); 
	}

	// different from cross product
	Vec4 operator * (const Vec4 &other) const
	{
		return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	Vec4 operator + (const Vec4 &other) const
	{
		return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Vec4 operator - (const Vec4 &other) const
	{
		return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	// negate operator
	Vec4 &operator - ()
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}

	Vec4 operator * (float val) const
	{
		return Vec4(x * val, y * val, z * val, w * val);
	}

	Vec4 operator + (float val) const
	{
		return Vec4(x + val, y + val, z + val, w + val);
	}

	Vec4 operator - (float val) const
	{
		return Vec4(x - val, y - val, z - val, w - val);
	}

	Vec4 operator / (float val) const
	{
		debugAssertMsg(val != 0.0f, "Denominator cannot be zero");

		return Vec4(x, y, z, w) * (1 / val);
	}

	// as right value
	const float operator[] (int i) const
	{
		debugAssertMsg(i < 4, "indexing out bound");
		debugAssertMsg(i >= 0, "indexing out bound");
		return m_values[i];
	}

	// return reference as left value
	float &operator[] (int i)
	{
		debugAssertMsg(i < 4, "indexing out bound");
		debugAssertMsg(i >= 0, "indexing out bound");
		return m_values[i];
	}

	Vec3 xyz()
	{
		return Vec3(x, y, z);
	}

	static NNEfloat dot(const Vec4 &v0, const Vec4 &v1)
	{
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
	}
};

// override global operators
// see the link to know how to define global functions in header file
// http://stackoverflow.com/questions/6964819/function-already-defined-error-in-c
inline Vec4 operator + (float val, const Vec4 &other)
{
	return other + val;
}

inline Vec4 operator * (float val, const Vec4 &other)
{
	return other * val;
}

} // namespace NNE

#endif // NNE_VEC3_H