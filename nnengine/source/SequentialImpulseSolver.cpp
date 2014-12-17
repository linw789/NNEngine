#include "SequentialImpulseSolver.h"
#include "RigidBody.h"

namespace NNE{

NNEbool SequentialImpulseSolver::resolve(ContactPoint *cm)
{
	// build jacobian
	Vec3 r0 = cm->cpws0 - cm->collidable0->getTransform() * cm->collidable0->getCollideShape()->getCenter();
	Vec3 r1 = cm->cpws1 - cm->collidable1->getTransform() * cm->collidable1->getCollideShape()->getCenter();
	Vec3 rn0 = Vec3::cross(r0, cm->normal);
	Vec3 rn1 = Vec3::cross(r1, cm->normal);
	m_jacobian[0] = -(cm->normal)[0]; 
	m_jacobian[1] = -(cm->normal)[1]; 
	m_jacobian[2] = -(cm->normal)[2];
	m_jacobian[3] = -rn0[0];
	m_jacobian[4] = -rn0[1];
	m_jacobian[5] = -rn0[2];
	m_jacobian[6] = (cm->normal)[0];
	m_jacobian[7] = (cm->normal)[1];
	m_jacobian[8] = (cm->normal)[2];
	m_jacobian[9] =  rn1[0];
	m_jacobian[10] = rn1[1];
	m_jacobian[11] = rn1[2];

	// lamda = -(J * V + b) / (J * M^-1 * J^T)
	// lamda = lamda * h(time step)
	
	RigidBody *body0 = static_cast<RigidBody *>(cm->collidable0);
	RigidBody *body1 = static_cast<RigidBody *>(cm->collidable1);
	Vec3 inertiaInv0 = body0->getDiagonalInvInertia();
	Vec3 inertiaInv1 = body1->getDiagonalInvInertia();
	Vec3 v0_old = body0->getLinearVelocity();
	Vec3 v1_old = body1->getLinearVelocity();
	Vec3 w0_old = body0->getAngularVelocity();
	Vec3 w1_old = body1->getAngularVelocity();

	m_oldVel[0] = v0_old[0];
	m_oldVel[1] = v0_old[1];
	m_oldVel[2] = v0_old[2];
	m_oldVel[3] = w0_old[0];
	m_oldVel[4] = w0_old[1];
	m_oldVel[5] = w0_old[2];
	m_oldVel[6] = v1_old[0];
	m_oldVel[7] = v1_old[1];
	m_oldVel[8] = v1_old[2];
	m_oldVel[9] = w1_old[0];
	m_oldVel[10] = w1_old[1];
	m_oldVel[11] = w1_old[2];

	NNEfloat relativeVel = m_jacobian * m_oldVel;
	//if (relativeVel > 0)
	//{
	//	return true;
	//}

	m_diagInvMass[0] = body0->getInvMass();
	m_diagInvMass[1] = body0->getInvMass();
	m_diagInvMass[2] = body0->getInvMass();
	m_diagInvMass[3] = inertiaInv0[0];
	m_diagInvMass[4] = inertiaInv0[1];
	m_diagInvMass[5] = inertiaInv0[2];
	m_diagInvMass[6] = body1->getInvMass();
	m_diagInvMass[7] = body1->getInvMass();
	m_diagInvMass[8] = body1->getInvMass();
	m_diagInvMass[9] = inertiaInv1[0];
	m_diagInvMass[10] = inertiaInv1[1];
	m_diagInvMass[11] = inertiaInv1[2];

	NNEfloat invMass = m_jacobian * m_diagInvMass * m_jacobian;

	NNEfloat baumgart = 0.5f * -(cm->depth + 0.0f);

	// lamda = lamda * timestep
	NNEfloat lamda = -(relativeVel + baumgart) / invMass;

	Vec3 linearImpulse0 = -cm->normal * lamda;
	Vec3 angularImpulse0 = -rn0 * lamda;
	Vec3 linearImpulse1 = cm->normal * lamda;
	Vec3 angularImpulse1 = rn1 * lamda;

	body0->applyLinearImpulse(linearImpulse0);
	body0->applyAngularImpulse(angularImpulse0);
	body1->applyLinearImpulse(linearImpulse1);
	body1->applyAngularImpulse(angularImpulse1);

	return true;
}

} // namespace NNE