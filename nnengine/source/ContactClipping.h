#ifndef NNE_CONTACTCLIPPING_H
#define NNE_CONTACTCLIPPING_H

#include "PlatformSpecs.h"
#include "Vec3.hpp"
#include "Array.h"

namespace NNE{

/*
	generate contact point of 2 colliding polyhedron by using 
	Sutherland-Hodgman clipping algorithm
*/
class ContactClipping
{
public:
	
	ContactClipping()
	{}

	~ContactClipping()
	{}

	// clip vertices against a plane
	// d = signed distance between the origin and the plane
	// if the origin is on the side that the normal points to, d is negative,
	// otherwise d is positive
	NNEvoid clipAgainstPlane(const Array<Vec3> &vertIn, Array<Vec3> &vertOut, const Vec3 &normal, NNEfloat d);

private:

};

} // manespace NNE

#endif // NNE_CONTACTCLIPPING_H