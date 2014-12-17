#ifndef NNE_BROADPHASEPAIRCACHE_H
#define NNE_BROADPHASEPAIRCACHE_H

#include "PlatformSpecs.h"
#include "Array.h"
#include "BroadphasePair.h"
#include "ContactPoint.h"
#include "PersistentManifold.h"

namespace NNE{
/*
	store and manipulate an array of pairs of aabbs generated from broad phase collision test

	reference: bulletphysics.org
*/
class BroadphasePairCache
{

class CollisionAlgorithm;

public:

	BroadphasePairCache(NNEuint n)
	{
		m_pairs.reserve(n);
	}

	~BroadphasePairCache()
	{}

	// if proxy0's uniqueId is smaller than proxy1's, ignore the pair
	NNEvoid addPair(BroadphaseProxy *proxy0, BroadphaseProxy *proxy1);
		
	// narrow phase collision detection
	NNEvoid processPairs(Array<PersistentManifold> &pmArray);

	NNEvoid clear()
	{
		m_pairs.clear();
	}

private:

	Array<BroadphasePair> m_pairs;
};

} // manespace NNE

#endif // NNE_BROADPHASEPAIRCACHE_H