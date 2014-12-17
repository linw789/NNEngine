#include "PhysicsWorld.h"
#include "Collidable.h"
#include "RigidBody.h"
#include "Broadphase.h"
#include "DynamicAABBTree.h"
#include "BroadphasePairCache.h"
#include "CollisionAlgorithm.h"
#include "CollideShape.h"
#include "NSqrBroadphase.h"
#include "Constraint.h"
#include "Input.h"

namespace NNE{

PhysicsWorld::~PhysicsWorld()
{
	m_broadphase->destroy();

	for (NNEuint i = 0; i < m_rigidBodies.size(); ++i)
	{
		delete m_rigidBodies[i];
	}
}

NNEvoid PhysicsWorld::init()
{
	// use dynamic aabb tree for broad phase method,
	// expecting 100 collidable ojects
	m_broadphase = new DynamicAABBTree(30);

	//m_broadphase = new NSqrBroadphase(30);

	m_persistentManifolds.reserve(PERSISTENT_MANIFOLD_NUM);
	m_contactConstraints.reserve(CONTACT_CONTSTRAINT_NUM);

	CollisionAlgorithm::setAlgo(dynamic_cast<CollisionAlgorithm *>(&m_gjkEpa), 
		CollideShape::CONVEX_POLYHEDRON, CollideShape::CONVEX_POLYHEDRON);

	CollisionAlgorithm::setAlgo(dynamic_cast<CollisionAlgorithm *>(&m_gjkEpa), 
		CollideShape::BOX, CollideShape::BOX);

	CollisionAlgorithm::setAlgo(dynamic_cast<CollisionAlgorithm *>(&m_planBoxDetector),
		CollideShape::INFINITE_PLANE, CollideShape::BOX);

	CollisionAlgorithm::setAlgo(dynamic_cast<CollisionAlgorithm *>(&m_planBoxDetector),
		CollideShape::BOX, CollideShape::INFINITE_PLANE);
}

RigidBody *PhysicsWorld::createRigidBody(CollideShape *cs, NNEfloat mass)
{
	RigidBody *body = new RigidBody();
	body->setCollideShape(cs);
	body->setMass(mass);
	body->setInertia(cs->calculateInertia(mass));

	m_rigidBodies.push_back(body);

	m_broadphase->addCollidable(dynamic_cast<Collidable *>(body));

	return body;
}

NNEvoid PhysicsWorld::applyGravity()
{
	for (NNEuint i = 0; i < m_rigidBodies.size(); ++i)
	{
		if (!m_rigidBodies[i]->isStatic() && m_rigidBodies[i]->isInGravity())
		{
			Vec3 gravityForce = m_gravity * m_rigidBodies[i]->getMass();
			m_rigidBodies[i]->addCentralForce(gravityForce);
		}
	}
}

NNEvoid PhysicsWorld::integrateVelocityByExternalForce(NNEfloat dt)
{
	for (NNEuint i = 0; i < m_rigidBodies.size(); ++i)
	{
		m_rigidBodies[i]->integrateVelocity(dt);
		m_rigidBodies[i]->clearForce();
	}
}

NNEvoid PhysicsWorld::clearInvalidContactPoint()
{
	for (NNEuint i = 0; i < m_persistentManifolds.size(); ++i)
	{
		m_persistentManifolds[i].clearInvalidPoints();
	}
}

NNEvoid PhysicsWorld::clipContactPoint()
{
	for (NNEuint i = 0; i < m_persistentManifolds.size(); ++i)
	{
		m_persistentManifolds[i].clipContactPoint();
	}
}

NNEvoid PhysicsWorld::resolveConstraints()
{
	for (NNEuint i = 0; i < m_persistentManifolds.size(); ++i)
	{
		m_persistentManifolds[i].createContactConstraints(m_contactConstraints);
	}

	NNEuint iterations = 25;

	while(iterations)
	{
		for (NNEuint i = 0; i < m_contactConstraints.size(); ++i)
		{
			m_contactConstraints[i].resolve(60.0f);
		}

		iterations--;
	}
}

NNEvoid PhysicsWorld::applyDamping()
{
	for (NNEuint i = 0; i < m_rigidBodies.size(); ++i)
	{
		m_rigidBodies[i]->applyDamping();
	}
}

NNEvoid PhysicsWorld::eulerIntegrate()
{
	for (NNEuint i = 0; i < m_rigidBodies.size(); ++i)
	{
		m_rigidBodies[i]->integrate((NNEfloat)m_fixedDeltaTime);
	}
}

NNEvoid PhysicsWorld::simulateOneStep(NNEdouble frametime)
{
	/*
	Because rendering will cost certain amount of time after physics simulation is done,
	the physics time will always be one renderTime late. But is okay as long as it's consistant.

	simulationTime + renderTime < deltaTime
	realTime = displayTime - lastPhysicsTime
	
	t * deltaTime < realTime < (t + 1) * deltaTime

	t * deltaTime = currentState
	(t + 1) * deltaTime = nextState

	interpolate(currentState, nextState)
	*/

	m_catchUpTime += frametime;

	while (m_catchUpTime > m_fixedDeltaTime)
	{
		// currentState = nextState
		// calculate nextState

		applyGravity();

		integrateVelocityByExternalForce((NNEfloat)m_fixedDeltaTime);

		m_broadphase->update();

		BroadphasePairCache *pairs = m_broadphase->getBroadphasePairs();

		clearInvalidContactPoint();
		pairs->processPairs(m_persistentManifolds);
		clipContactPoint();

		resolveConstraints();

		applyDamping();

		eulerIntegrate();

		m_broadphase->clearBroadphasePairs();
		m_contactConstraints.clear();

		m_catchUpTime -= m_fixedDeltaTime;
		// m_catchUpTime += physics simulation time
	}

	NNEdouble alpha = m_catchUpTime / m_fixedDeltaTime;

	// interpolation between the current state and the next state

	//if (true/*gInputMngr.ifKeyUpStroke('G')*/)
	//{
	//	applyGravity();

	//	integrateVelocityByExternalForce((NNEfloat)m_fixedDeltaTime);

	//	m_broadphase->update();

	//	BroadphasePairCache *pairs = m_broadphase->getBroadphasePairs();

	//	clearInvalidContactPoint();
	//	pairs->processPairs(m_persistentManifolds);
	//	clipContactPoint();

	//	resolveConstraints();

	//	applyDamping();

	//	eulerIntegrate();

	//	m_broadphase->clearBroadphasePairs();
	//	//m_persistentManifolds.clear();
	//	m_contactConstraints.clear();
	//	//m_contactPoints.clear();
	//}

	//for (NNEint i = 0; i < m_persistentManifolds.size(); ++i)
	//{
	//	m_persistentManifolds[i].drawContactPoints();
	//}
}

PhysicsWorld gPhysicsWorld;

} // namespace NNE