#ifndef NNE_RECT_H
#define NNE_RECT_H

#include "PlatformSpecs.h"
#include "Vec2.hpp"

namespace NNE{

template<class T>
class Rect
{
public:
	
	Rect()
		:x(0), y(0), w(0), h(0)
	{}

	Rect(T x, T y, T w, T h)
		:x(x), y(y), w(w), h(h)
	{}

	Rect(const Rect &other)
		:x(other.x), y(other.y), w(other.w), h(other.h)
	{}

	~Rect()
	{}

	Rect &operator = (const Rect &other)
	{
		x = other.x;
		y = other.y;
		w = other.w;
		h = other.h;

		return *this;
	}

	NNEvoid setPos(T x, T y)
	{
		x = x;
		y = y;
	}

	NNEvoid setWidthHeight(T w, T h)
	{
		w = w;
		h = h;
	}

	Vector2<T> getPos()
	{
		return Vector2<T>(x, y);
	}

	Vector2<T> getWidthHeight()
	{
		return Vector2<T>(w, h);
	}

	T getWidth()
	{
		return w;
	}

	T getHeight()
	{
		return h;
	}

	// aspect ratio: width / height
	NNEfloat getAspect()
	{
		return (NNEfloat)w / (NNEfloat)h;
	}

public:

	// corner position
	T x, y;
	
	// with and height
	T w, h;
};

#define Rectui Rect<NNEuint> 
#define Rectf Rect<NNEfloat>

} // manespace NNE

#endif // NNE_RECT_H