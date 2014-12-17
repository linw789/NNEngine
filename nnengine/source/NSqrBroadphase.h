#ifndef NNE_NSQRBROADPHASE_H
#define NNE_NSQRBROADPHASE_H

#include "PlatformSpecs.h"
#include "Broadphase.h"

namespace NNE{

class NSqrBroadphase: public Broadphase
{
public:
	
	NSqrBroadphase(NNEuint n)
		:Broadphase(n)
	{}

	~NSqrBroadphase()
	{}

	NNEvoid destroy()
	{
		Broadphase::destroy();
	}

	NNEvoid addCollidable(Collidable *c)
	{
		BroadphaseProxy *proxy = createProxy(c);
		addProxy(proxy);
	}

	NNEvoid update();

	NNEvoid genBroadphasePairs();
};

} // manespace NNE

#endif // NNE_NSQRBROADPHASE_H