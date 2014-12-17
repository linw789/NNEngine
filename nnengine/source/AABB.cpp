#include "AABB.h"

namespace NNE{

Vec3 AABB::furthestPointInDir(const Vec3 &dir) const
{
	Vec3 boxVert[7];
	boxVert[0] = m_min;
	boxVert[1] = Vec3(m_min.x, m_min.y, m_max.z);
	boxVert[2] = Vec3(m_min.x, m_max.y, m_min.z);
	boxVert[3] = Vec3(m_min.x, m_max.y, m_max.z);
	boxVert[4] = Vec3(m_max.x, m_min.y, m_min.z);
	boxVert[5] = Vec3(m_max.x, m_min.y, m_max.z);
	boxVert[6] = Vec3(m_max.x, m_max.y, m_min.z);

	Vec3 result = m_max;
	NNEfloat d = Vec3::dot(m_max, dir);

	for (NNEuint i = 0; i < 7; ++i)
	{
		NNEfloat tmp = Vec3::dot(boxVert[i], dir);
		if (tmp > d)
		{
			d = tmp;
			result = boxVert[i];
		}
	}

	return result;
}

} // namespace NNE