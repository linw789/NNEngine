#ifndef NNE_MATH_H
#define NNE_MATH_H

#include "PlatformSpecs.h"

#include <cmath>

namespace NNE{

class Math
{
public:

	static NNEfloat PI_f;
	static NNEfloat HALF_PI_f;
	static NNEdouble PI_d;
	static NNEdouble HALF_PI_d;

	// convert degree to radian
	NNE_FORCE_INLINE static NNEfloat deg2rad(NNEfloat deg)
	{
		return deg / 180 * PI_f;
	}

	// convert radian to degree
	NNE_FORCE_INLINE static NNEfloat rad2deg(NNEfloat rad)
	{
		return rad / PI_f * 180;
	}

	NNE_FORCE_INLINE static NNEfloat sqrt(NNEfloat x)
	{
		return sqrtf(x);
	}

	NNE_FORCE_INLINE static NNEfloat pow(NNEfloat x, NNEfloat y)
	{
		return powf(x, y);
	}

	NNE_FORCE_INLINE static NNEfloat sin_deg(NNEfloat x)
	{
		return sinf(deg2rad(x));
	}

	NNE_FORCE_INLINE static NNEfloat cos_deg(NNEfloat x)
	{
		return cosf(deg2rad(x));
	}

	NNE_FORCE_INLINE static NNEfloat sin(NNEfloat x)
	{
		return sinf(x);
	}

	NNE_FORCE_INLINE static NNEfloat cos(NNEfloat x)
	{
		return cosf(x);
	}

	NNE_FORCE_INLINE static NNEfloat acos(NNEfloat x)
	{
		return acosf(x);
	}

	NNE_FORCE_INLINE static NNEfloat abs(NNEfloat x)
	{
		return ::abs(x);
	}

	NNE_FORCE_INLINE static NNEfloat max(NNEfloat a, NNEfloat b)
	{
		return a > b ? a : b;
	}

	NNE_FORCE_INLINE static NNEfloat min(NNEfloat a, NNEfloat b)
	{
		return a < b ? a : b;
	}

	NNE_FORCE_INLINE static NNEfloat clamp(NNEfloat x, NNEfloat min, NNEfloat max)
	{
		if (x < min)
		{
			return min;
		}
		else if (x > max)
		{
			return max;
		}
		else
		{
			return x;
		}
	}
};

} // namespace NNE

#endif // NNE_MATH_H