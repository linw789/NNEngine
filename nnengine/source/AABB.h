#ifndef NNE_AABB_H
#define NNE_AABB_H

#include "ConvexShape.h"
#include "Vec3.hpp"

namespace NNE{

class AABB
{
public:

	AABB()
		:m_min(0.0f, 0.0f, 0.0f), m_max(0.0f, 0.0f, 0.0f)
	{}

	AABB(const Vec3 &min, const Vec3 &max)
		:m_min(min), m_max(max)
	{}

	AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
		:m_min(minX, minY, minZ), m_max(maxX, maxY, maxZ)
	{}

	// union
	static NNEvoid merge(const AABB &l, const AABB &r, AABB &result)
	{
		result.m_min.x = l.m_min.x < r.m_min.x ? l.m_min.x : r.m_min.x;
		result.m_min.y = l.m_min.y < r.m_min.y ? l.m_min.y : r.m_min.y;
		result.m_min.z = l.m_min.z < r.m_min.z ? l.m_min.z : r.m_min.z;

		result.m_max.x = l.m_max.x > r.m_max.x ? l.m_max.x : r.m_max.x;
		result.m_max.y = l.m_max.y > r.m_max.y ? l.m_max.y : r.m_max.y;
		result.m_max.z = l.m_max.z > r.m_max.z ? l.m_max.z : r.m_max.z;
	}

	NNEvoid setMinMaxPoint(const Vec3 &minv, const Vec3 &maxv)
	{
		m_min = minv;
		m_max = maxv;
	}

	Vec3 getMinPoint() const
	{
		return m_min;
	}

	Vec3 getMaxPoint() const
	{
		return m_max;
	}

	// strech each edge by percentage
	NNEvoid bulge(NNEfloat percentage)
	{
		NNEfloat edgeX = m_max.x - m_min.x;
		NNEfloat edgeY = m_max.y - m_min.y;
		NNEfloat edgeZ = m_max.z - m_min.z;
		m_max.x += edgeX * percentage * 0.5f;
		m_max.y += edgeY * percentage * 0.5f;
		m_max.z += edgeZ * percentage * 0.5f;
		m_min.x -= edgeX * percentage * 0.5f;
		m_min.y -= edgeY * percentage * 0.5f;
		m_min.z -= edgeZ * percentage * 0.5f;
	}

	// compute volume of the AABB
	NNEfloat volume()
	{
		return (m_max.x - m_min.x)
			 * (m_max.y - m_min.y)
			 * (m_max.z - m_min.z);
	}

	NNEbool ifEnclose(const AABB &other) const
	{
		Vec3 otherMin = other.getMinPoint();
		Vec3 otherMax = other.getMaxPoint();
		return m_max.x >= otherMax.x
			&& m_max.y >= otherMax.y
			&& m_max.z >= otherMax.z
			&& m_min.x <= otherMin.x
			&& m_min.y <= otherMin.y
			&& m_min.z <= otherMin.z;
	}

	NNEbool collide(const AABB &aabb) const
	{
		if (m_max.x <= aabb.m_min.x || m_min.x >= aabb.m_max.x)
		{
			return false;
		}
		else if (m_max.y <= aabb.m_min.y || m_min.y >= aabb.m_max.y)
		{
			return false;
		}
		else if (m_max.z <= aabb.m_min.z || m_min.z >= aabb.m_max.z)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	NNEvoid setUserData(NNEint ud)
	{
		m_userData = ud;
	}

	NNEint getUserData()
	{
		return m_userData;
	}

	Vec3 furthestPointInDir(const Vec3 &dir) const;

private:

	// minX, minY, minZ
	Vec3 m_min;

	// maxX, maxY, maxZ
	Vec3 m_max;

	NNEint m_userData;
};

} // namespace NNE

#endif // NNE_AABB_H