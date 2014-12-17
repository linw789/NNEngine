#include "BroadphasePairCache.h"

namespace NNE{

NNEvoid BroadphasePairCache::addPair(BroadphaseProxy *proxy0, BroadphaseProxy *proxy1)
{
	m_pairs.push_back(BroadphasePair(proxy0, proxy1));
}

NNEvoid BroadphasePairCache::processPairs(Array<PersistentManifold> &pmArray)
{
	ContactPoint tmpCP;

	for (NNEuint i = 0; i < m_pairs.size(); ++i)
	{
		if (m_pairs[i].genContactPoint(&tmpCP))
		{
			NNEint k = -1;
			for (NNEuint j = 0; j < pmArray.size(); ++j)
			{
				if (pmArray[j].m_collidable0 == m_pairs[i].m_proxy0->m_clientObject
					&& pmArray[j].m_collidable1 == m_pairs[i].m_proxy1->m_clientObject)
				{
					k = j;
					break;
				}
			}
			if (k > -1)
			{
				PersistentManifold *test = &(pmArray[k]);
				pmArray[k].addContactPoint(tmpCP);
			}
			else
			{
				PersistentManifold *pm = pmArray.next();
				pm->m_collidable0 = m_pairs[i].m_proxy0->m_clientObject;
				pm->m_collidable1 = m_pairs[i].m_proxy1->m_clientObject;
				pm->addContactPoint(tmpCP);
				NNEint tess = 1;
			}
		}
	}
}

} // namespace NNE