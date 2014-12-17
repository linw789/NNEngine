#ifndef NNE_VEC2_H
#define NNE_VEC2_H

#include "DebugUtilities.h"

#include <cmath>

namespace NNE{

template<typename T>
class Vector2
{
public:

	// could implement swizzle in future
	// union provides more entries with no overhead
	union 
	{
		T m_values[2];
		
		struct{T x, y;};

		struct{T u, v;};

		struct{T s, t;};

		struct{T w, h;};
	};

	// constructors
	Vector2()
		:x(0), y(0)
	{}

	Vector2(T v0, T v1)
		:x(v0), y(v1)
	{}

	Vector2(const Vector2<T> &other)
		:x(other.x), y(other.y)
	{}

	// operator override
	Vector2<T> &operator = (const Vector2<T> &other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vector2<T> &operator += (const Vector2<T> &other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2<T> &operator -= (const Vector2<T> &other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vector2<T> &operator *= (const Vector2<T> &other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	Vector2<T> &operator += (T val)
	{
		x += val;
		y += val;

		return *this;
	}

	Vector2<T> &operator -= (T val)
	{
		x -= val;
		y -= val;

		return *this;
	}

	Vector2<T> &operator *= (T val)
	{
		x *= val;
		y *= val;

		return *this;
	}

	// this operation could probably lose precision if T is int type
	Vector2<T> &operator /= (T val)
	{
		// woud this be fast than just two divisions?
		// don't know how compiler would optimize this
		T tmp = 1 / val;
		x *= tmp;
		y *= tmp;

		return *this;
	}

	bool operator == (const Vector2<T> &other)
	{
		return (x == other.x) && (y == other.y);
	}

	bool operator != (const Vector2<T> &other)
	{
		return (x != other.x) || (y != other.y); 
	}

	Vector2<T> operator * (const Vector2<T> &other) const
	{
		return Vector2<T>(x * other.x, y * other.y);
	}

	Vector2<T> operator + (const Vector2<T> &other) const
	{
		return Vector2<T>(x + other.x, y + other.y);
	}

	Vector2<T> operator - (const Vector2<T> &other) const
	{
		return Vector2<T>(x - other.x, y - other.y);
	}

	Vector2<T> operator * (T val) const
	{
		return Vector2<T>(x * val, y * val);
	}

	Vector2<T> operator + (T val) const
	{
		return Vector2<T>(x + val, y + val);
	}

	Vector2<T> operator - (T val) const
	{
		return Vector2<T>(x - val, y - val);
	}

	Vector2<NNEfloat> operator / (T val) const
	{
		debugAssertMsg(val != 0.0f, "Denominator cannot be zero");

		// would this be faster than two divisions?
		return Vector2<NNEfloat>(x, y) * (1 / (NNEfloat)val);
	}

	// as right value
	const T operator[] (NNEuint i) const
	{
		debugAssertMsg(i < 2, "indexing out bound");
		debugAssertMsg(i >= 0, "indexing out bound");
		return m_values[i];
	}

	// return reference as left value
	T &operator[] (NNEuint i)
	{
		debugAssertMsg(i < 2, "indexing out bound");
		debugAssertMsg(i >= 0, "indexing out bound");
		return m_values[i];
	}

	// utility functions
	T sqaureLength()
	{
		return (x * x + y * y);
	}

	NNEfloat length()
	{
		return sqrtf((NNEfloat)sqaureLength());
	}
};

typedef Vector2<NNEint> Vec2i;
typedef Vector2<NNEfloat> Vec2;

// override global operators
template<typename T>
inline Vector2<T> operator + (T val, const Vector2<T> &other)
{
	return other + val;
}

template<typename T>
inline Vector2<T> operator * (T val, const Vector2<T> &other)
{
	return other * val;
}

} // namespace NNE

#endif // NNE_VEC2_H