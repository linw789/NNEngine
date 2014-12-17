#ifndef NNE_CONVEX_SHAPE_H
#define NNE_CONVEX_SHAPE_H

#include "PlatformSpecs.h"
#include "Collidable.h"
#include "Vec3.hpp"

namespace NNE{

/*
	
*/
class ConvexShape : public CollideShape
{

public:
	
	ConvexShape()
	{}

	~ConvexShape()
	{}

	// find the furthest point inside the convex along the given direction
	virtual Vec3 furthestPointInDir(const Vec3 &dir) const = 0;
};

} // manespace NNE

#endif // NNE_CONVEX_SHAPE_H