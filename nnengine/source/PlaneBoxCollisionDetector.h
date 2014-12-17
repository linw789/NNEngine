#ifndef NNE_PLANEBOXCOLLISIONDETECTOR_H
#define NNE_PLANEBOXCOLLISIONDETECTOR_H

#include "PlatformSpecs.h"
#include "CollisionAlgorithm.h"
#include "ConvexShape.h"
#include "Matrix4.hpp"
#include "ContactPoint.h"
#include "ContactClipping.h"
#include "Array.h"

namespace NNE{

class PlaneBoxCollisionDetector: public CollisionAlgorithm
{
public:
	
	PlaneBoxCollisionDetector()
	{}

	~PlaneBoxCollisionDetector()
	{}

	NNEbool testCollision(ConvexShape *shape0, const Mat4 &transform0,
					ConvexShape *shape1, const Mat4 &transform1, 
					ContactPoint *cp);

private:
};

} // manespace NNE

#endif // NNE_PLANEBOXCOLLISIONDETECTOR_H