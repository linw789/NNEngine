#include "CubicSpline.h"

namespace NNE{

NNEvoid CubicSpline3D::createCurve(Array<Vec3> &curve, Array<Vec3> &tangent,
	NNEfloat u0, NNEfloat u1, NNEfloat maxLength)
{
	NNEfloat mid = (u1 + u0) / 2.0f;
	Vec3 p0 = interpolate(u0);
	Vec3 p1 = interpolate(u1);
	NNEfloat sqrlenght = (p1 - p0).sqaureLength();
	if (sqrlenght < maxLength)
	{
		// this recursion actually pushes in points from starting poistion to end position
		curve.push_back(p0);
		//curve.push_back(p1); // duplicate

		// add tangent
		tangent.push_back(tangnetLerp(u0));
		//tangent.push_back(tangnetLerp(u1)); // duplicate
	}
	else
	{
		createCurve(curve, tangent, u0, mid, maxLength);
		createCurve(curve, tangent, mid, u1, maxLength);
	}
}

} // namespace NNE