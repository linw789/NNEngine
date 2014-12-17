#include "Quaternion.h"

namespace NNE{

Quaternion Quaternion::normalize(const Quaternion &q)
{
	Quaternion result = q;
	result.normalize();
	return result;
}

NNEfloat Quaternion::dot(const Quaternion &q0, const Quaternion &q1)
{
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

Quaternion Quaternion::slerp(NNEfloat t, const Quaternion &startq, const Quaternion &endq)
{
	// cosine of angle between startq and endq
	NNEfloat cosOfAngle = dot(startq, endq);

	// to correct from float point error
	cosOfAngle = cosOfAngle > 1.0f ? 1.0f : cosOfAngle;
	NNEfloat angle = Math::acos(cosOfAngle);
	
	// check if it's the shortest arc
	Quaternion tmpStartq = startq;
	//if (cosOfAngle < 0.0f)
	//{
	//	tmpStartq = -startq;
	//	// acos(-cosAngle) < acos(cosAngle)
	//	cosOfAngle = -cosOfAngle;
	//}

	//NNEfloat sinAngle = Math::sin(angle);

	// this ensures sinAngle is bigger than 0, hence the shortest path
	NNEfloat sinAngle = Math::sqrt(1 - cosOfAngle * cosOfAngle);
	if (sinAngle < 1e-6f)
	{
		return startq;
	}
	NNEfloat oneOverSinAngle = 1.0f / sinAngle;

	NNEfloat sinR = Math::sin((1 - t) * angle);
	NNEfloat sinT = Math::sin(t * angle);

	Quaternion result = (sinR * oneOverSinAngle) * tmpStartq + (sinT * oneOverSinAngle) * endq;
	result.normalize();

	return result;
}

} // namespace NNE