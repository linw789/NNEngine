#ifndef NNE_BOX_SHAPE_H
#define NNE_BOX_SHAPE_H

#include "PlatformSpecs.h"
#include "ConvexShape.h"
#include "AABB.h"
#include "Matrix4.hpp"
#include "Matrix3.hpp"

namespace NNE{

class BoxShape: public ConvexShape
{
public:
	
	BoxShape(const Vec3 &extents)
		:m_halfExtents(extents)
	{}

	~BoxShape()
	{}

	Vec3 furthestPointInDir(const Vec3 &dir) const
	{
		return Vec3(dir.x < 0 ? -m_halfExtents.x : m_halfExtents.x, 
					dir.y < 0 ? -m_halfExtents.y : m_halfExtents.y,
					dir.z < 0 ? -m_halfExtents.z : m_halfExtents.z);
	}

	NNEvoid getAabb(AABB *aabb, const Mat4 &worldMat) const
	{
		// inversely rotated local axes
		Vec3 invAxisX = Vec3(worldMat[0][0], worldMat[1][0], worldMat[2][0]);
		Vec3 invAxisY = Vec3(worldMat[0][1], worldMat[1][1], worldMat[2][1]);
		Vec3 invAxisZ = Vec3(worldMat[0][2], worldMat[1][2], worldMat[2][2]);

		Vec3 maxX_point = furthestPointInDir(invAxisX);
		Vec3 maxY_point = furthestPointInDir(invAxisY);
		Vec3 maxZ_point = furthestPointInDir(invAxisZ);

		Vec3 center_worldspace = worldMat.getPos();

		// transform max_point into world space, but we only need one component 
		// from each point, so following
		NNEfloat maxX = Vec3::dot(invAxisX, maxX_point) + center_worldspace.x;
		NNEfloat maxY = Vec3::dot(invAxisY, maxY_point) + center_worldspace.y;;
		NNEfloat maxZ = Vec3::dot(invAxisZ, maxZ_point) + center_worldspace.z;;

		NNEfloat minX = center_worldspace.x - maxX + center_worldspace.x;
		NNEfloat minY = center_worldspace.y - maxY + center_worldspace.y;
		NNEfloat minZ = center_worldspace.z - maxZ + center_worldspace.z;

		aabb->setMinMaxPoint(Vec3(minX, minY, minZ), Vec3(maxX, maxY, maxZ));
	}

	NNEvoid getAabbSlow(AABB *aabb, const Mat4 &worldMat)
	{
		Vec3 tmp = worldMat * Vec3(m_halfExtents.x, m_halfExtents.y, m_halfExtents.z);
		NNEfloat minX, minY, minZ, maxX, maxY, maxZ;
		minX = maxX = tmp.x;
		minY = maxY = tmp.y;
		minZ = maxZ = tmp.z;

		Vec3 vertices[7] = {worldMat * Vec3(m_halfExtents.x, -m_halfExtents.y, m_halfExtents.z),
							worldMat * Vec3(m_halfExtents.x, m_halfExtents.y, -m_halfExtents.z),
							worldMat * Vec3(m_halfExtents.x, -m_halfExtents.y, -m_halfExtents.z),
							worldMat * Vec3(-m_halfExtents.x, m_halfExtents.y, m_halfExtents.z),
							worldMat * Vec3(-m_halfExtents.x, -m_halfExtents.y, m_halfExtents.z),
							worldMat * Vec3(-m_halfExtents.x, m_halfExtents.y, -m_halfExtents.z),
							worldMat * Vec3(-m_halfExtents.x, -m_halfExtents.y, -m_halfExtents.z)};

		for (NNEuint i = 0; i < 7; ++i)
		{
			if (vertices[i].x < minX)
			{
				minX = vertices[i].x;
			}
			else if (vertices[i].x > maxX)
			{
				maxX = vertices[i].x;
			}

			if (vertices[i].y < minY)
			{
				minY = vertices[i].y;
			}
			else if (vertices[i].y > maxY)
			{
				maxY = vertices[i].y;
			}

			if (vertices[i].z < minZ)
			{
				minZ = vertices[i].z;
			}
			else if (vertices[i].z > maxZ)
			{
				maxZ = vertices[i].z;
			}
		}

		aabb->setMinMaxPoint(Vec3(minX, minY, minZ), Vec3(maxX, maxY, maxZ));
	}

	NNEuint getShapeId() const
	{
		return CollideShape::BOX;
	}

	Vec3 getHalfExtents()
	{
		return m_halfExtents;
	}

	Mat3 calculateInertia(NNEfloat mass) const
	{
		Mat3 inertia;

		if (0 == mass)
		{
			// zero Mat3
			return inertia;
		}

		inertia[0][0] = 4 * (m_halfExtents.y * m_halfExtents.y + m_halfExtents.z * m_halfExtents.z);
		inertia[1][1] = 4 * (m_halfExtents.x * m_halfExtents.x + m_halfExtents.z * m_halfExtents.z);
		inertia[2][2] = 4 * (m_halfExtents.x * m_halfExtents.x + m_halfExtents.y * m_halfExtents.y);

		// inertia = (mass / 12) * inertia
		inertia = mass * 0.083333333f * inertia;

		return inertia;
	}

private:

	// assume box centered at the origin
	Vec3 m_halfExtents;
};

} // manespace NNE

#endif // NNE_BOX_SHAPE_H