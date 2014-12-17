#include "PersistentManifold.h"

namespace NNE{

NNEvoid PersistentManifold::clearInvalidPoints()
{
	// determine whether existing contact points are persistent
	NNEuint tmpCount = 0;
	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		// transform in current frame multiplies local contact point in previous frame
		// local contact point in world space
		Vec3 lcpws0 = m_collidable0->getTransform() * m_cachedPoints[i].cp0;
		Vec3 lcpws1 = m_collidable1->getTransform() * m_cachedPoints[i].cp1;

		// world contact point in the current frame
		Vec3 cpws0 = m_cachedPoints[i].cpws0;
		Vec3 cpws1 = m_cachedPoints[i].cpws1;

		Vec3 err0 = cpws0 - lcpws0;
		Vec3 err1 = cpws1 - lcpws1;

		NNEbool stillPenetrating = Vec3::dot(m_cachedPoints[i].normal, lcpws1 - lcpws0) < 0;

		NNEfloat threshold = 0.05f;
		NNEfloat sqrLengthErr0 = err0.sqaureLength();
		NNEfloat sqrLengthErr1 = err1.sqaureLength();
		if (stillPenetrating && sqrLengthErr0 < threshold && sqrLengthErr1 < threshold)
		{
			m_cachedPoints[i].ifPersistent = true;
			tmpCount++;
		}
		else
		{
			m_cachedPoints[i].ifPersistent = false;
		}
	}

	NNEint j = 0;
	for (NNEint i = m_pointNum - 1; i > 0; --i)
	{
		if (m_cachedPoints[i].ifPersistent)
		{
			while(j < i)
			{
				if (false == m_cachedPoints[j].ifPersistent)
				{
					m_cachedPoints[j] = m_cachedPoints[i];
					j++;
					break;
				}
				j++;
			}
		}
		if (j == i)
			break;
	}
	m_pointNum = tmpCount;
}

NNEvoid PersistentManifold::addContactPoint(const ContactPoint &cp)
{
	NNEfloat threshold = 0.5f;

	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		Vec3 err0 = cp.cpws0 - m_cachedPoints[i].cpws0;
		Vec3 err1 = cp.cpws1 - m_cachedPoints[i].cpws1;

		if (err0.sqaureLength() < threshold && err1.sqaureLength() < threshold)
		{
			// the new contact point is close enough, discard
			return ;
		}
	}

	m_cachedPoints[m_pointNum] = cp;
	m_pointNum++;
}

NNEvoid PersistentManifold::clipContactPoint()
{
	if (m_pointNum <= 4)
		return ;

	NNEbool taken[CACHED_POINT_SIZE + 1] = {false};

	// find the one with deepest penetration
	NNEfloat maxPenetration = 0;
	NNEuint deepest = 0;
	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		if (m_cachedPoints[i].depth >= maxPenetration)
		{
			maxPenetration = m_cachedPoints[i].depth;
			deepest = i;
		}
	}
	taken[deepest] = true;

	// find the one that is furthest to the first one
	NNEfloat maxDist = 0;
	NNEuint second = 0;
	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		if (false == taken[i])
		{
			NNEfloat sqrDist = (m_cachedPoints[i].cpws1 - m_cachedPoints[deepest].cpws1).sqaureLength();
			if (sqrDist >= maxDist)
			{
				maxDist = sqrDist;
				second = i;
			}
		}
	}
	taken[second] = true;

	// find the one that is the furthest to the line formed by previous two
	maxDist = 0;
	Vec3 line = m_cachedPoints[deepest].cpws1 - m_cachedPoints[second].cpws1;
	NNEuint third = 0;
	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		if (false == taken[i])
		{
			Vec3 va = m_cachedPoints[i].cpws1 - m_cachedPoints[second].cpws1;
			Vec3 vb = Vec3::cross(line, va);
			Vec3 vc = Vec3::normalize(Vec3::cross(vb, line));
			NNEfloat dist = Vec3::dot(vc, va);
			NNEfloat sqrDist = dist * dist;
			if (sqrDist >= maxDist)
			{
				maxDist = sqrDist;
				third = i;
			}
		}
	}
	taken[third] = true;

	// find the one that is the furthest to the triangle formed by previous three
	maxDist = 0;
	Vec3 triNormal = Vec3::cross(line, m_cachedPoints[third].cpws1 - m_cachedPoints[second].cpws1);
	triNormal.normalize();
	NNEuint fourth = 0;
	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		if (false == taken[i])
		{
			NNEfloat dist = Vec3::dot(m_cachedPoints[i].cpws1 - m_cachedPoints[deepest].cpws1, triNormal);
			NNEfloat sqrDist = dist * dist;
			if (sqrDist >= maxDist)
			{
				maxDist = sqrDist;
				fourth = i;
			}
		}
	}
	taken[fourth] = true;

	NNEint j = 0;
	for (NNEint i = m_pointNum - 1; i >= 0; --i)
	{
		if (taken[i])
		{
			while(j < i)
			{
				if (!taken[j])
				{
					m_cachedPoints[j] = m_cachedPoints[i];
					break;
					j++;
				}
				j++;
			}
		}
		if (i == j)
			break;
	}

	m_pointNum = 4;
}

NNEvoid PersistentManifold::createContactConstraints(Array<ContactConstraint> &ccArray)
{
	for (NNEint i = 0; i < m_pointNum; ++i)
	{
		ContactConstraint *cc = ccArray.next();
		cc->setContactPoint(m_cachedPoints + i);
		cc->buildJacobian();
	}
}

} // namespace NNE