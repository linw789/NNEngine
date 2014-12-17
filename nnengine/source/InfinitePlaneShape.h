#ifndef NNE_INFINITEPLANESHAPE_H
#define NNE_INFINITEPLANESHAPE_H

#include "PlatformSpecs.h"
#include "ConvexShape.h"
#include "Vec3.hpp"
#include "AABB.h"
#include "Matrix4.hpp"

namespace NNE{

class InfinitePlaneShape: public ConvexShape
{
public:
	
	InfinitePlaneShape(const Vec3 &normal, NNEfloat d)
		:m_normal(normal), m_d(d)
	{}

	~InfinitePlaneShape()
	{}

	NNEvoid getAabb(AABB *aabb, const Mat4 &) const
	{
		aabb->setMinMaxPoint(Vec3(-NNE_LARGE_FLOAT, -NNE_LARGE_FLOAT, -NNE_LARGE_FLOAT),
			Vec3(NNE_LARGE_FLOAT, NNE_LARGE_FLOAT, NNE_LARGE_FLOAT));
	}

	NNEuint getShapeId() const
	{
		return CollideShape::INFINITE_PLANE;
	}

	NNEvoid setNormal(const Vec3 &n)
	{
		m_normal = n;
	}

	Vec3 getNormal()
	{
		return m_normal;
	}

	NNEvoid setDistance(NNEfloat d)
	{
		m_d = d;
	}

	NNEfloat getDistance()
	{
		return m_d;
	}

	// do not use this function
	Vec3 furthestPointInDir(const Vec3 &dir) const
	{
		return Vec3(0, 0, 0);
	}

	Mat3 calculateInertia(NNEfloat mass) const
	{
		return Mat3();
	}

private:

	Vec3 m_normal;

	// signed distance to origin
	// if the origin is on the side that the normal points to, d is negative,
	// otherwise d is positive
	NNEfloat m_d;
};

} // manespace NNE

#endif // NNE_INFINITEPLANESHAPE_H