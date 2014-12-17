#ifndef NNE_COLLIDESHAPE_H
#define NNE_COLLIDESHAPE_H

#include "PlatformSpecs.h"

namespace NNE{

class AABB;

class CollideShape
{
public:
	
	enum COLLIDING_SHAPE
	{
		CONVEX_POLYHEDRON = 0,
		BOX,
		INFINITE_PLANE,

		COLLIDING_SHAPE_NUM,
	};

	CollideShape()
	{}

	~CollideShape()
	{}

	virtual NNEvoid getAabb(AABB *aabb, const Mat4 &) const = 0;

	virtual NNEuint getShapeId() const = 0;

	virtual Vec3 getCenter()
	{
		return Vec3(0, 0, 0);
	}

	virtual Mat3 calculateInertia(NNEfloat mass) const = 0;

protected:
};

} // manespace NNE

#endif // NNE_COLLIDESHAPE_H