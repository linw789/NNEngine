#include "NSqrBroadphase.h"
#include "CollideShape.h"

namespace NNE{

NNEvoid NSqrBroadphase::update()
{
	for (NNEuint i = 0; i < m_proxyArray.size(); ++i)
	{
		m_proxyArray[i]->updateAabb();
	}

	genBroadphasePairs();
}

NNEvoid NSqrBroadphase::genBroadphasePairs()
{
	NNEint proxyNum = m_proxyArray.size();
	for (NNEint i = 0; i < proxyNum - 1; ++i)
	{
		for (NNEint j = i + 1; j < proxyNum; ++j)
		{
			if (m_proxyArray[i]->m_aabb.collide(m_proxyArray[j]->m_aabb))
			{
				m_pairCache.addPair(m_proxyArray[i], m_proxyArray[j]);
			}
		}
	}
}

} // namespace NNE