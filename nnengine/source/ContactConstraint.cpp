#include "ContactConstraint.h"
#include "RigidBody.h"
#include "Matrix.h"
#include "Math.h"

namespace NNE{

NNEvoid ContactConstraint::buildJacobian()
{
	// build normal jacobian
	Vec3 r0 = m_cp->cpws0 - m_cp->collidable0->getTransform() * m_cp->collidable0->getCollideShape()->getCenter();
	Vec3 r1 = m_cp->cpws1 - m_cp->collidable1->getTransform() * m_cp->collidable1->getCollideShape()->getCenter();
	Vec3 rn0 = Vec3::cross(r0, m_cp->normal);
	Vec3 rn1 = Vec3::cross(r1, m_cp->normal);
	m_normal_J[0] = -m_cp->normal.x; 
	m_normal_J[1] = -m_cp->normal.y; 
	m_normal_J[2] = -m_cp->normal.z;
	m_normal_J[3] = -rn0.x;
	m_normal_J[4] = -rn0.y;
	m_normal_J[5] = -rn0.z;
	m_normal_J[6] = m_cp->normal.x;
	m_normal_J[7] = m_cp->normal.y;
	m_normal_J[8] = m_cp->normal.z;
	m_normal_J[9] =  rn1.x;
	m_normal_J[10] = rn1.y;
	m_normal_J[11] = rn1.z;

	// lamda = -(J * V + b) / (J * M^-1 * J^T)
	// lamda = lamda * h(time step)
	
	RigidBody *body0 = static_cast<RigidBody *>(m_cp->collidable0);
	RigidBody *body1 = static_cast<RigidBody *>(m_cp->collidable1);
	Vec3 inertiaInv0 = body0->getDiagonalInvInertia();
	Vec3 inertiaInv1 = body1->getDiagonalInvInertia();
	Vec3 v0 = body0->getLinearVelocity();
	Vec3 w0 = body0->getAngularVelocity();
	Vec3 v1 = body1->getLinearVelocity();
	Vec3 w1 = body1->getAngularVelocity();

	// relative velocity projected on the normal and the restituion velocity
	m_restitution = body0->getElasticity() * body1->getElasticity();

	// build friction jacobian
	Vec3 tv0 = v0 + Vec3::cross(w0, r0);
	Vec3 tv1 = v1 + Vec3::cross(w1, r1);
	Vec3 proj_v0 = tv0 - Vec3::dot(tv0, m_cp->normal) * m_cp->normal;
	Vec3 proj_v1 = tv1 - Vec3::dot(tv1, m_cp->normal) * m_cp->normal;
	Vec3 rv = proj_v1 - proj_v0;
	NNEfloat tangentLength = rv.length();
	Vec3 tangent = (0 == tangentLength? Vec3(0, 0, 0) : rv / tangentLength);
	Vec3 rt0 = Vec3::cross(r0, tangent);
	Vec3 rt1 = Vec3::cross(r1, tangent);
	m_tangent_J[0] = -tangent[0];
	m_tangent_J[1] = -tangent[1];
	m_tangent_J[2] = -tangent[2];
	m_tangent_J[3] = -rt0[0];
	m_tangent_J[4] = -rt0[1];
	m_tangent_J[5] = -rt0[2];
	m_tangent_J[6] = tangent[0];
	m_tangent_J[7] = tangent[1];
	m_tangent_J[8] = tangent[2];
	m_tangent_J[9] = rt1[0];
	m_tangent_J[10] = rt1[1];
	m_tangent_J[11] = rt1[2];

	m_friction = body0->getFrictionCoeff() * body1->getFrictionCoeff();

	DiagonalMatrix<12> invMass;
	invMass[0] = body0->getInvMass();
	invMass[1] = body0->getInvMass();
	invMass[2] = body0->getInvMass();
	invMass[3] = inertiaInv0[0];
	invMass[4] = inertiaInv0[1];
	invMass[5] = inertiaInv0[2];
	invMass[6] = body1->getInvMass();
	invMass[7] = body1->getInvMass();
	invMass[8] = body1->getInvMass();
	invMass[9] = inertiaInv1[0];
	invMass[10] = inertiaInv1[1];
	invMass[11] = inertiaInv1[2];

	m_normalMassInv = 1.0f / (m_normal_J * invMass * m_normal_J);

	NNEfloat tangentMass = m_tangent_J * invMass * m_tangent_J;
	m_tangentMassInv = 0 == tangentMass ? 0 : 1.0f / tangentMass;

	NNEfloat test = m_cp->nlamda;

	// use old lamda to calculate impulse and apply it before the iterations of sequential impulse
	Vec3 linearImpulse0 = Vec3(m_normal_J[0], m_normal_J[1], m_normal_J[2]) * m_cp->nlamda
		+ Vec3(m_tangent_J[0], m_tangent_J[1], m_tangent_J[2]) * m_cp->tlamda;

	Vec3 angularImpulse0 = Vec3(m_normal_J[3], m_normal_J[4], m_normal_J[5]) * m_cp->nlamda
		+ Vec3(m_tangent_J[3], m_tangent_J[4], m_tangent_J[5]) * m_cp->tlamda;

	Vec3 linearImpulse1 = Vec3(m_normal_J[6], m_normal_J[7], m_normal_J[8]) * m_cp->nlamda
		+ Vec3(m_tangent_J[6], m_tangent_J[7], m_tangent_J[8]) * m_cp->tlamda;

	Vec3 angularImpulse1 = Vec3(m_normal_J[9], m_normal_J[10], m_normal_J[11]) * m_cp->nlamda
		+ Vec3(m_tangent_J[9], m_tangent_J[10], m_tangent_J[11]) * m_cp->tlamda;

	body0->applyLinearImpulse(linearImpulse0);
	body0->applyAngularImpulse(angularImpulse0);
	body1->applyLinearImpulse(linearImpulse1);
	body1->applyAngularImpulse(angularImpulse1);
}

NNEvoid ContactConstraint::resolve(NNEfloat inv_dt)
{
	RigidBody *body0 = static_cast<RigidBody *>(m_cp->collidable0);
	RigidBody *body1 = static_cast<RigidBody *>(m_cp->collidable1);
	Vec3 v0 = body0->getLinearVelocity();
	Vec3 v1 = body1->getLinearVelocity();
	Vec3 w0 = body0->getAngularVelocity();
	Vec3 w1 = body1->getAngularVelocity();

	m_oldVelocity[0] = v0[0];
	m_oldVelocity[1] = v0[1];
	m_oldVelocity[2] = v0[2];
	m_oldVelocity[3] = w0[0];
	m_oldVelocity[4] = w0[1];
	m_oldVelocity[5] = w0[2];
	m_oldVelocity[6] = v1[0];
	m_oldVelocity[7] = v1[1];
	m_oldVelocity[8] = v1[2];
	m_oldVelocity[9] = w1[0];
	m_oldVelocity[10] = w1[1];
	m_oldVelocity[11] = w1[2];

	NNEfloat relativeNormalVel = m_normal_J * m_oldVelocity;

	//if (relativeNormalVel > 0)
	//{
	//	*outlamda = 0;
	//	return ;
	//}

	NNEfloat relativeTangentVel = m_tangent_J * m_oldVelocity;

	NNEfloat slop = 0.018f;
	// alpha = beta / timeStep, 0 < beta < 1, tunning beta to get more stable result
	NNEfloat alpha = 0.1f * inv_dt;

	NNEfloat baumgart = -alpha * Math::max(m_cp->depth - slop, 0);

	// lamda is actually lamda * timestep
	NNEfloat lamdaNew = -(relativeNormalVel + baumgart) * m_normalMassInv;
	// make sure the new lamda wouldn't make the accumulated lamda negative
	lamdaNew  = Math::max(lamdaNew , -m_cp->nlamda);
	m_cp->nlamda += lamdaNew;

	// solve friction constraint
	NNEfloat tLamdaNew = -relativeTangentVel * m_tangentMassInv;
	NNEfloat tLamdaLimit = m_cp->nlamda * m_friction;
	// -tLamdaLimit < m_cp->tlamda + tlamdaNew < tLamdaLimit
	tLamdaNew = Math::clamp(tLamdaNew, -tLamdaLimit - m_cp->tlamda, tLamdaLimit - m_cp->tlamda);
	m_cp->tlamda += tLamdaNew; 

	Vec3 linearImpulse0 = Vec3(m_normal_J[0], m_normal_J[1], m_normal_J[2]) * lamdaNew
		+ Vec3(m_tangent_J[0], m_tangent_J[1], m_tangent_J[2]) * tLamdaNew;

	Vec3 angularImpulse0 = Vec3(m_normal_J[3], m_normal_J[4], m_normal_J[5]) * lamdaNew
		+ Vec3(m_tangent_J[3], m_tangent_J[4], m_tangent_J[5]) * tLamdaNew;

	Vec3 linearImpulse1 = Vec3(m_normal_J[6], m_normal_J[7], m_normal_J[8]) * lamdaNew
		+ Vec3(m_tangent_J[6], m_tangent_J[7], m_tangent_J[8]) * tLamdaNew;

	Vec3 angularImpulse1 = Vec3(m_normal_J[9], m_normal_J[10], m_normal_J[11]) * lamdaNew
		+ Vec3(m_tangent_J[9], m_tangent_J[10], m_tangent_J[11]) * tLamdaNew;

	body0->applyLinearImpulse(linearImpulse0);
	body0->applyAngularImpulse(angularImpulse0);
	body1->applyLinearImpulse(linearImpulse1);
	body1->applyAngularImpulse(angularImpulse1);
}

} // namespace NNE