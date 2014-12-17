#ifndef NNE_COLLIDABLE_H
#define NNE_COLLIDABLE_H

#include "PlatformSpecs.h"
#include "Matrix4.hpp"
#include "CollideShape.h"
#include "Quaternion.h"

namespace NNE{

class Collidable
{
public:
	
	Collidable()
		:m_orientation(0, 0, 0, 1.0f), m_isMoved(true)
	{
		m_worldTransform.identity();
	}

	~Collidable()
	{}

	CollideShape *getCollideShape()
	{
		return m_collideShape;
	}

	NNEvoid setCollideShape(CollideShape *cs)
	{
		m_collideShape = cs;
	}

	NNEvoid setTransform(const Mat4 &m4)
	{
		m_worldTransform = m4;

		m_orientation = m_worldTransform.toQuaternion();
	}

	const Mat4 &getTransform()
	{
		return m_worldTransform;
	}

	NNEbool isMoved()
	{
		return m_isMoved;
	}

protected:

	Mat4 m_worldTransform;

	Quaternion m_orientation;

	CollideShape *m_collideShape;

	NNEbool m_isMoved;
};

} // manespace NNE

#endif // NNE_COLLIDABLE_H