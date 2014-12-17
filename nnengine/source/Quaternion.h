#ifndef NNE_QUATERNION_H
#define NNE_QUATERNION_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Vec3.hpp"
#include "Math.h"

namespace NNE{

//http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
class Quaternion
{
public:
	
	Quaternion()
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f) 
	{}

	~Quaternion()
	{}

	Quaternion(NNEfloat xx, NNEfloat yy, NNEfloat zz, NNEfloat ww)
		:x(xx), y(yy), z(zz), w(ww)
	{}

	Quaternion(const Quaternion &q)
		:x(q.x), y(q.y), z(q.z), w(q.w)
	{}

	// construct a quaternion from the form of angle and axis
	// the quaternion represents a rotation around the axis by 
	// angle degree in counter-clockwise when looking at the opposite
	// direction of the axis
	Quaternion(Vec3 axis, NNEfloat deg)
	{
		// note: result quaternion is already normalized

		NNEfloat radian = Math::deg2rad(deg * 0.5f);

		NNEfloat sinValue = Math::sin(radian);

		NNEfloat sqrLength = axis.sqaureLength();

		debugAssertMsg(sqrLength != 0.0f, "Denominator cannot be zero");

		axis /= Math::sqrt(sqrLength);

		w = Math::cos(radian);
		x = axis.x * sinValue;
		y = axis.y * sinValue;
		z = axis.z * sinValue;
	}

	NNEbool operator == (const Quaternion &q) const
	{
		return w - q.w < 0.0000001f && w - q.w > -0.0000001f
			&& x - q.x < 0.0000001f && x - q.x > -0.0000001f
			&& y - q.y < 0.0000001f && y - q.y > -0.0000001f
			&& z - q.z < 0.0000001f && z - q.z > -0.0000001f;
	}

	Quaternion &operator = (const Quaternion &q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;

		return *this;
	}

	Quaternion &operator += (const Quaternion &q)
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	Quaternion &operator /= (NNEfloat d)
	{
		if (d < NNE_FLOAT_TOLERANCE)
		{
			debugOutput("denominator too small");
			return *this;
		}
		else
		{
			NNEfloat tmp = 1 / d;
			x *= d;
			y *= d;
			z *= d;
			w *= d;
		}

		return *this;
	}

	Quaternion operator + (const Quaternion &other) const
	{
		return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Quaternion operator * (NNEfloat val) const
	{
		return Quaternion(x * val, y * val, z * val, w * val);
	}

	Quaternion operator * (const Quaternion &q) const 
	{
		// p * q = (p.w * q.v + q.w * p.v + p.v cross q.v, p.w * q.w - p.v dot q.v);
		return Quaternion(w * q.x + x * q.w + y * q.z - z * q.y,
						  w * q.y + y * q.w + z * q.x - x * q.z,
	                      w * q.z + z * q.w + x * q.y - y * q.x,
						  w * q.w - x * q.x - y * q.y - z * q.z);
	}

	// negation operation
	Quaternion operator -() const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	NNEvoid normalize()
	{
		NNEfloat sqaureMag = x * x + y * y + z * z + w * w;

		if (sqaureMag < NNE_FLOAT_TOLERANCE)
		{
			debugAssert(false) ;
		}
		///debugAssertMsg(sqaureMag > NNE_DENOMINATOR_TOLERANCE, "denominator is too small");

		if (sqaureMag > 1.0000000001f || sqaureMag < 0.9999999999f)
		{
			NNEfloat tmp = 1 / Math::sqrt(sqaureMag);
			x *= tmp;
			y *= tmp;
			z *= tmp;
			w *= tmp;
		}
	}

	Quaternion getConjugate()
	{
		return Quaternion(-x, -y, -z, w);
	}

	NNEvoid setScalar(NNEfloat ww)
	{
		w = ww;
	}

	NNEvoid setVec3(const Vec3 &v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
	}

	Vec3 getVec3()
	{
		return Vec3(x, y, z);
	}

	// rotate a vector
	// quaternion multiplication involves vector cross product,
	// so in different hand system, cross product should be implemented differently.
	Vec3 rotateVector(const Vec3 &v3)
	{
		// rotation degree is zero
		if (0 == w || (0 == x && 0 == y && 0 == z))
		{
			return v3;
		}
		else
		{
			/*result = q * v * inv(q)*/
			/*Quaternion tmp, result;
			tmp.setScalar(0);
			tmp.setVec3(v3);
			// unit quaternion's inverse equals its conjugate
			result = (*this) * tmp * getConjugate();
			return result.getVec3();*/

			// faster quaternion-vector multiplicatoin
			// http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
			Vec3 v = getVec3();
			Vec3 tmp = 2 * Vec3::cross(v, v3);
			return v3 + w * tmp + Vec3::cross(v, tmp);
		}
	}

	static Quaternion normalize(const Quaternion &q);

	// parameter quaternions must be unit quaternion!
	// the return value is the cosine of the angle between two quaternions on the greatest arc 
	static NNEfloat dot(const Quaternion &q0, const Quaternion &q1);

	// http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/
	//         commutative    constant velocity    torque-minimal
	// slerp       no                yes                yes
	// nlerp       yes               no                 yes
	// log lerp    yes               yes                no

	// slerp is computationally expensive due to the operations of trigonometry
	// parameter quaternions must be unit quaternions!
	static Quaternion slerp(NNEfloat t, const Quaternion &startq, const Quaternion &endq);

	static Quaternion lerp(NNEfloat t, const Quaternion &startq, const Quaternion &endq)
	{
		return normalize(startq * (1 - t) + endq * t);
	}

public:

	NNEfloat x, y, z;

	NNEfloat w;
};

// global operator
inline Quaternion operator * (NNEfloat val, const Quaternion &q)
{
	return q * val;
}

} // manespace NNE

#endif // NNE_QUATERNION_H