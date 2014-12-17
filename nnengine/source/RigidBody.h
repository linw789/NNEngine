#ifndef NNE_RIGIDBODY_H
#define NNE_RIGIDBODY_H

#include "PlatformSpecs.h"
#include "Collidable.h"
#include "Vec3.hpp"
#include "Matrix4.hpp"
#include "Matrix3.hpp"
#include "Quaternion.h"

namespace NNE{

class RigidBody : public Collidable
{
public:
	
	RigidBody()
		:m_mass(1.0f), m_invMass(1.0f),
		m_linearVelocity(0, 0, 0), m_angularVelocity(0, 0, 0),
		m_totalForce(0, 0, 0), m_totalTorque(0, 0, 0),
		m_linearDamping(0.8f), m_angularDamping(0.8f), 
		m_frictionCoeff(0.6f), m_inGravity(true),
		m_isStatic(false), m_elasticity(0.7f)
	{
		// see discussion/issue report here: http://code.google.com/p/bullet/issues/detail?id=74
		m_linearDamping = Math::pow(m_linearDamping, 0.01666667f);
		m_angularDamping = Math::pow(m_angularDamping, 0.01666667f);
	}

	~RigidBody()
	{}

	NNEvoid setMass(NNEfloat mass)
	{
		m_mass = mass;
		if (0 == m_mass)
		{
			m_invMass = 0;
		}
		else
		{
			m_invMass = 1.0f / m_mass;
		}
	}

	NNEvoid setInertia(const Mat3 &inertia)
	{
		if (0 == m_mass)
		{
			m_invLocalInertia.zero();
		}
		else
		{
			m_invLocalInertia = Mat3::inverse(inertia);
			updateInertia();
		}
	}

	NNEvoid updateInertia()
	{
		Mat3 rotate = m_worldTransform.getRotateMat3();
		m_invInertia = rotate * m_invLocalInertia * Mat3::transpose(rotate);
	}

	Vec3 getDiagonalInvInertia()
	{
		return Vec3(m_invInertia[0][0], m_invInertia[1][1], m_invInertia[2][2]);
	}

	NNEvoid setLinearDamping(NNEfloat ld, NNEfloat timestep)
	{
		m_linearDamping = Math::pow(1 - ld, timestep);
	}

	NNEvoid setAngularDamping(NNEfloat ad, NNEfloat timestep)
	{
		m_angularDamping = Math::pow(1 - ad, timestep);
	}

	NNEvoid addCentralForce(const Vec3 &force)
	{
		m_totalForce += force;
	}
	
	NNEvoid addTorque(const Vec3 &torque)
	{
		m_totalTorque += torque;
	}

	NNEvoid addForce(const Vec3 &force, const Vec3 &forcePoint)
	{
		addCentralForce(force);
		addTorque(Vec3::cross(forcePoint - m_worldTransform.getPos(), force));
	}

	NNEvoid setInGravity(NNEbool b)
	{
		m_inGravity = b;
	}

	NNEbool isInGravity()
	{
		return m_inGravity;
	}

	NNEvoid setIfStatic(NNEbool b)
	{
		m_isStatic = b;
	}

	NNEbool isStatic()
	{
		return m_isStatic;
	}

	NNEvoid applyDamping();

	NNEvoid applyLinearImpulse(const Vec3 &impulse)
	{
		if (0 != m_invMass)
		{
			m_linearVelocity += impulse * m_invMass;
		}
	}

	NNEvoid applyAngularImpulse(const Vec3 &impulse)
	{
		if (0 != m_invMass)
		{
			// not sure whether it's post multiplication or pre multiplication
			// should be pre-multiplication based on how torque is calculated
			m_angularVelocity += m_invInertia * impulse;
		}
	}

	NNEvoid applyImpulse(const Vec3 &impulse, const Vec3 &impulsePoint)
	{
		if (0 != m_invMass)
		{
			applyLinearImpulse(impulse);
			applyAngularImpulse(Vec3::cross(impulsePoint - m_worldTransform.getPos(), impulse));
		}
	}

	NNEfloat getMass()
	{
		return m_mass;
	}

	NNEfloat getInvMass()
	{
		return m_invMass;
	}

	Vec3 getLinearVelocity()
	{
		return m_linearVelocity;
	}

	NNEfloat getFrictionCoeff()
	{
		return m_frictionCoeff;
	}

	NNEfloat getElasticity()
	{
		return m_elasticity;
	}

	NNEvoid setLinearVelocity(const Vec3 &v)
	{
		m_linearVelocity = v;
	}

	Vec3 getAngularVelocity()
	{
		return m_angularVelocity;
	}

	NNEvoid integrateVelocity(NNEfloat dt)
	{
		if (m_isStatic)
			return;

		m_linearVelocity += m_totalForce * m_invMass * dt;
		m_angularVelocity += m_invInertia * m_totalTorque * dt;

		// clamp the angular velocity, high angular velocity fails collision detection
		//NNEfloat angvel = m_angularVelocity.length();
		
	}

	NNEvoid integratePosition(NNEfloat deltaTime)
	{
		Vec3 oldPos = m_worldTransform.getPos();
		m_worldTransform.setPos(oldPos + deltaTime * m_linearVelocity);
	}

	// to do exponential map
	NNEvoid integrateOrietation(NNEfloat deltatime);

	NNEvoid integrate(NNEfloat deltaTime)
	{
		if (0 != m_invMass)
		{
			integratePosition(deltaTime);
			integrateOrietation(deltaTime);
		}
	}

	NNEvoid clearForce()
	{
		m_totalForce.zero();
		m_totalTorque.zero();
	}

private:

	NNEfloat m_mass;
	NNEfloat m_invMass;
	
	// inverse of initial inertia tensor
	Mat3 m_invLocalInertia;
	// inverse of current inertia tensor
	Mat3 m_invInertia;

	// inverse of Inertia tensor diagonal matrix
	// assuming rigid body is symmetrical around 3 axes 
	// Vec3 m_invInertiaDiag;

	Vec3 m_linearVelocity;
	Vec3 m_angularVelocity;

	Vec3 m_totalForce;
	Vec3 m_totalTorque;

	// clamp to [0.0, 1.0]
	NNEfloat m_linearDamping;
	NNEfloat m_angularDamping;

	// 0.0f ~ 1.0f
	NNEfloat m_frictionCoeff;

	// 0.0f ~ 1.0f
	NNEfloat m_elasticity;

	// whether this object will be effected by the gravity
	NNEbool m_inGravity;

	NNEbool m_isStatic;
};

} // manespace NNE

#endif // NNE_RIGIDBODY_H