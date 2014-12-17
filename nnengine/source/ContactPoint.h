#ifndef NNE_CONTACT_POINT_H
#define NNE_CONTACT_POINT_H

#include "PlatformSpecs.h"
#include "Vec3.hpp"
#include "ConvexShape.h"

namespace NNE{

class Collidable;

class ContactPoint
{
public:
	
	ContactPoint()
		:ifPersistent(false), nlamda(0), tlamda(0)
	{}

	~ContactPoint()
	{}

public:

	//Vec3 contacts[4];

	// contact points in world space
	Vec3 cpws0, cpws1;

	// 2 contact points in each collider space
	Vec3 cp0, cp1;

	// contact normal in world space
	// pointing from collidable0 to collidable1
	Vec3 normal;

	// minimum penetration depth
	NNEfloat depth;

	NNEbool ifPersistent;

	// accumulated normal lamda
	NNEfloat nlamda;

	// accumulated tangent lamda
	NNEfloat tlamda;

	Collidable *collidable0;
	Collidable *collidable1;
};

} // manespace NNE

#endif // NNE_CONTACT_POINT_H