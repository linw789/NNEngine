#ifndef NNE_BROADPHASEPAIR_H
#define NNE_BROADPHASEPAIR_H

#include "PlatformSpecs.h"
#include "AABB.h"
#include "Collidable.h"
#include "CollisionAlgorithm.h"
#include "ContactPoint.h"

namespace NNE{

/*
	used in the broad phase collision detection
*/
class BroadphaseProxy
{
friend class DynamicAABBTree;
friend class BroadphasePair;
friend class NSqrBroadphase;

public:

	BroadphaseProxy(Collidable *c)
		:m_clientObject(c),
		m_uniqueId(-1)
	{

	}

	NNEvoid updateAabb()
	{
		if (m_clientObject->isMoved())
		{
			CollideShape *cs = m_clientObject->getCollideShape();
			cs->getAabb(&m_aabb, m_clientObject->getTransform());
		}
	}

	NNEvoid setUniqueId(NNEint id)
	{
		m_uniqueId = id;
	}

	NNEint getUniqueId()
	{
		return m_uniqueId;
	}

	NNEuint getShapeId()
	{
		return m_clientObject->getCollideShape()->getShapeId();
	}

public:

	// point to rigidbody when used in dynamics
	Collidable *m_clientObject;

	AABB m_aabb;

	// this variable is used to help detect duplicating pair
	// when search collision in broad phase, always query proxy with
	// the uniqueId in ascending order.
	NNEint m_uniqueId;
};

class BroadphasePair
{
public:
	
	BroadphasePair(BroadphaseProxy *p0, BroadphaseProxy *p1)
		:m_proxy0(p0), m_proxy1(p1)
	{
		m_collsionAlgo = CollisionAlgorithm::findAlgo(m_proxy0->getShapeId(), m_proxy1->getShapeId());
	}

	~BroadphasePair()
	{}

	NNEbool genContactPoint(ContactPoint *cp)
	{
		NNEbool collided = m_collsionAlgo->testCollision(
			dynamic_cast<ConvexShape *>(m_proxy0->m_clientObject->getCollideShape()), 
			m_proxy0->m_clientObject->getTransform(),
			dynamic_cast<ConvexShape *>(m_proxy1->m_clientObject->getCollideShape()),
			m_proxy1->m_clientObject->getTransform(),
			cp);

		if (collided)
		{
			cp->collidable0 = m_proxy0->m_clientObject;
			cp->collidable1 = m_proxy1->m_clientObject;

			return true;
		}
		else
		{
			return false;
		}
	}

public:

	BroadphaseProxy *m_proxy0;

	BroadphaseProxy *m_proxy1;

	CollisionAlgorithm *m_collsionAlgo;
};

} // manespace NNE

#endif // NNE_BROADPHASEPAIR_H