#ifndef NNE_VEC3_H
#define NNE_VEC3_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Math.h"

namespace NNE{

class Vec3
{
public:

	// constructors
	Vec3()
	{}

	Vec3(float v0, float v1, float v2)
		:x(v0), y(v1), z(v2)
	{}

	Vec3(const Vec3 &other)
		:x(other.x), y(other.y), z(other.z)
	{}

	Vec3(const NNEfloat *val)
		:x(val[0]), y(val[1]), z(val[2])
	{}

	// operator override
	Vec3 &operator = (const Vec3 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	Vec3 &operator += (const Vec3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vec3 &operator -= (const Vec3 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vec3 &operator *= (const Vec3 &other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	Vec3 &operator += (float val)
	{
		x += val;
		y += val;
		z += val;

		return *this;
	}

	Vec3 &operator -= (float val)
	{
		x -= val;
		y -= val;
		z -= val;

		return *this;
	}

	Vec3 &operator *= (float val)
	{
		x *= val;
		y *= val;
		y *= val;

		return *this;
	}

	Vec3 &operator /= (float val)
	{
		float tmp = 1 / val;
		x *= tmp;
		y *= tmp;
		z *= tmp;

		return *this;
	}

	bool operator == (const Vec3 &other)
	{
		return (x == other.x) && (y == other.y) && (z == other.z);
	}

	bool operator != (const Vec3 &other)
	{
		return (x != other.x) || (y != other.y) || (z != other.z); 
	}

	// different from cross product
	Vec3 operator * (const Vec3 &other) const
	{
		return Vec3(x * other.x, y * other.y, z * other.z);
	}

	Vec3 operator + (const Vec3 &other) const
	{
		return Vec3(x + other.x, y + other.y, z + other.z);
	}

	Vec3 operator - (const Vec3 &other) const
	{
		return Vec3(x - other.x, y - other.y, z - other.z);
	}

	// negate operator
	Vec3 operator - () const
	{
		return Vec3(-x, -y, -z);
	}

	Vec3 operator * (float val) const
	{
		return Vec3(x * val, y * val, z * val);
	}

	Vec3 operator + (float val) const
	{
		return Vec3(x + val, y + val, z + val);
	}

	Vec3 operator - (float val) const
	{
		return Vec3(x - val, y - val, z - val);
	}

	Vec3 operator / (float val) const
	{
		debugAssertMsg(val != 0.0f, "Denominator cannot be zero");

		return Vec3(x, y, z) * (1 / val);
	}

	// as right value
	const NNEfloat operator[] (int i) const
	{
		debugAssertMsg(i < 3, "indexing out bound");
		debugAssertMsg(i >= 0, "indexing out bound");
		return m_values[i];
	}

	// return reference as left value
	NNEfloat &operator[] (int i)
	{
		debugAssertMsg(i < 3, "indexing out bound");
		debugAssertMsg(i >= 0, "indexing out bound");
		return m_values[i];
	}

	NNEvoid zero()
	{
		::memset(m_values, 0, 3 * sizeof(NNEfloat));
	}

	NNEbool isZero()
	{
		return 0 == x && 0 == y && 0 ==z;
	}

	NNEbool isProximateZero()
	{
		return (x < 1e-6f && x > -1e-6f)
			&& (y < 1e-6f && y > -1e-6f)
			&& (z < 1e-6f && z > -1e-6f);
	}

	// utility functions
	float sqaureLength() const
	{
		return x * x + y * y + z * z;
	}

	float length() const
	{
		return Math::sqrt(sqaureLength());
	}

	// static functions
	static Vec3 normalize(const Vec3 &v3)
	{
		Vec3 result = v3;

		result.normalize();

		return result;
	}
	
	NNEvoid normalize()
	{
		NNEfloat sqrLength = sqaureLength();

		// if the vector is zero, just return it
		//debugAssertMsg(sqrLength != 0.0f, "Denominator cannot be zero");

		// test whether the length of axis exceeds the shreshold
		if (Math::abs(sqrLength - 1.0f) > 1e-8f)
		{
			*this /= Math::sqrt(sqrLength);
		}
	}

	static float dot(const Vec3 &lv, const Vec3 &rv)
	{
		return lv.x * rv.x + lv.y * rv.y + lv.z * rv.z;
	}

	static Vec3 cross(const Vec3 &lv, const Vec3 &rv)
	{
		return Vec3(lv.y * rv.z - lv.z * rv.y, lv.z * rv.x - lv.x * rv.z,
				lv.x * rv.y - lv.y * rv.x);
	}

	// http://en.wikipedia.org/wiki/Triple_product
	// (va x vb) x vc
	static Vec3 vectorTriple(const Vec3 &va, const Vec3 &vb, const Vec3 &vc)
	{
		return vb * dot(vc, va) - va * dot(vc, vb);
	}

public:

	// could implement swizzle in future
	// union provides more entries with no overhead
	union 
	{
		float m_values[3];
		
		struct{float x, y, z;};

		struct{float r, g, b;};

		struct{float s, t, p;};
	};
};

// override global operators
// !!! 
inline Vec3 operator + (const NNEfloat val, const Vec3 &other)
{
	return other + val;
}

inline Vec3 operator * (const NNEfloat val, const Vec3 &other)
{
	return other * val;
}

} // namespace NNE

#endif // NNE_VEC3_H