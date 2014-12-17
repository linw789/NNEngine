#ifndef NNE_PHYSICS_WORLD_H
#define NNE_PHYSICS_WORLD_H

#include "PlatformSpecs.h"
#include "Array.h"
#include "Vec3.hpp"
#include "CollisionAlgorithm.h"
#include "ContactPoint.h"
#include "GJK_EPA.h"
#include "PlaneBoxCollisionDetector.h"
#include "ContactConstraint.h"
#include "PersistentManifold.h"

namespace NNE{

class Broadphase;
class Collidable;
class RigidBody;
class Constraint;

class PhysicsWorld
{
public:

	enum
	{
		PERSISTENT_MANIFOLD_NUM = 15,
		CONTACT_CONTSTRAINT_NUM = 40,
	};
	
	PhysicsWorld()
		:m_fixedDeltaTime(1.0 / 60), m_physicsTime(0),
		m_catchUpTime(0), m_gravity(0, -1.0f, 0)
	{}

	~PhysicsWorld();

	NNEvoid init();

	NNEvoid setGravity(const Vec3 &g)
	{
		m_gravity = g;
	}

	Collidable *createCollidable()
	{

	}

	RigidBody *createRigidBody(CollideShape *cs, NNEfloat mass);

	NNEvoid applyGravity();

	NNEvoid integrateVelocityByExternalForce(NNEfloat dt);

	NNEvoid clearInvalidContactPoint();

	NNEvoid clipContactPoint();

	NNEvoid resolveConstraints();

	NNEvoid applyDamping();

	NNEvoid eulerIntegrate();

	// run simulation once over delta time
	NNEvoid simulateOneStep(NNEdouble frameTime); 

private:

	NNEdouble m_fixedDeltaTime;

	NNEdouble m_physicsTime;

	// the amount of time by which the display is ahead the physics simulation
	NNEdouble m_catchUpTime;

	Vec3 m_gravity;

	Array<RigidBody *> m_rigidBodies;

	//Array<ContactPoint> m_contactPoints;

	Array<PersistentManifold> m_persistentManifolds;

	Array<ContactConstraint> m_contactConstraints;

	//Array<Constraint> m_constraints;

	Broadphase *m_broadphase;

	// collision algorthims
	GjkEpaCollisionDetector m_gjkEpa;

	PlaneBoxCollisionDetector m_planBoxDetector;
};

extern PhysicsWorld gPhysicsWorld;

} // manespace NNE

#endif // NNE_PHYSICS_WORLD_H