#ifndef NNE_COLLISIONALGORITHM_H
#define NNE_COLLISIONALGORITHM_H

#include "PlatformSpecs.h"
#include "ContactPoint.h"
#include "Matrix4.hpp"

namespace NNE{

class ConvexShape;

/*
	serve as interface for various collision detection algorithms
*/
class CollisionAlgorithm
{
public:

	CollisionAlgorithm()
	{}

	~CollisionAlgorithm()
	{}

	static NNEvoid setAlgo(CollisionAlgorithm *ca, NNEuint shape0, NNEuint shape1)
	{
		m_collisionAlgos[shape0][shape1] = ca;
		m_collisionAlgos[shape1][shape0] = ca;
	}

	static CollisionAlgorithm *findAlgo(NNEuint shape0, NNEuint shape1)
	{
		return m_collisionAlgos[shape0][shape1];
	}

	virtual NNEbool testCollision(ConvexShape *shape0, const Mat4 &transform0,
					ConvexShape *shape1, const Mat4 &transform1, 
					ContactPoint *cp) = 0;

private:

	static CollisionAlgorithm *m_collisionAlgos[CollideShape::COLLIDING_SHAPE_NUM][CollideShape::COLLIDING_SHAPE_NUM];
};

} // manespace NNE

#endif // NNE_COLLISIONALGORITHM_H