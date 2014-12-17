#ifndef NNE_PERSISTENTMANIFOLD_H
#define NNE_PERSISTENTMANIFOLD_H

#include "PlatformSpecs.h"
#include "ContactPoint.h"
#include "ContactConstraint.h"
#include "Array.h"
#include "DebugRenderer.h"

namespace NNE{

class Collidable;

class PersistentManifold
{
public:

	enum
	{
		CACHED_POINT_SIZE = 4,
	};
	
	PersistentManifold()
		:m_pointNum(0), m_collidable0(NULL), m_collidable1(NULL)
	{}

	~PersistentManifold()
	{}

	NNEvoid clearInvalidPoints();

	// if the new contact point isn't close to any existing contact point,
	// add the new contact point
	NNEvoid addContactPoint(const ContactPoint &cp);

	// clip contact point so the manifold holds up to 4 contact points
	NNEvoid clipContactPoint();

	NNEvoid createContactConstraints(Array<ContactConstraint> &ccArray);

	NNEvoid drawContactPoints()
	{
		for (NNEint i = 0; i < m_pointNum; ++i)
		{
			gDebugRenderer.drawPoint(m_cachedPoints[i].cpws1, Color3(1, 0, 0), 2);
		}
	}

public:

	// contact points
	ContactPoint m_cachedPoints[CACHED_POINT_SIZE + 1];
	NNEint m_pointNum;

	Collidable *m_collidable0;
	Collidable *m_collidable1;
};

} // manespace NNE

#endif // NNE_PERSISTENTMANIFOLD_H