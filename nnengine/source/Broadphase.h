#ifndef NNE_BROADPHASE_H
#define NNE_BROADPHASE_H

#include "PlatformSpecs.h"
#include "BroadphasePairCache.h"
#include "BroadphasePair.h"
#include "Collidable.h"
#include "RigidBody.h"

namespace NNE{

class Broadphase
{
public:
	
	Broadphase(NNEuint i/*number of proxies*/)
		:m_pairCache(i)
	{
		m_proxyArray.reserve(i);
	}

	virtual ~Broadphase()
	{}

	virtual NNEvoid destroy()
	{
		for (NNEuint i = 0; i < m_proxyArray.size(); ++i)
		{
			delete m_proxyArray[i];
		}
	}

	BroadphaseProxy *createProxy(Collidable *cldb)
	{
		BroadphaseProxy *proxy = new BroadphaseProxy(cldb);
		return proxy;
	}

	NNEvoid addProxy(BroadphaseProxy *proxy)
	{
		proxy->setUniqueId(m_proxyArray.size());
		m_proxyArray.push_back(proxy);
	}

	virtual NNEvoid addCollidable(Collidable *c) = 0;

	// update broad phase and generate possible colliding pairs every frame
	virtual NNEvoid update() = 0;

	BroadphasePairCache *getBroadphasePairs()
	{
		return &m_pairCache;
	}

	NNEvoid clearBroadphasePairs()
	{
		m_pairCache.clear();
	}

protected:

	BroadphasePairCache m_pairCache;

	Array<BroadphaseProxy *> m_proxyArray;
};

} // manespace NNE

#endif // NNE_BROADPHASE_H