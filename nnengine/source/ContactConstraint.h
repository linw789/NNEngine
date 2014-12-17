#ifndef NNE_CONTACTCONSTRAINT_H
#define NNE_CONTACTCONSTRAINT_H

#include "PlatformSpecs.h"
#include "Constraint.h"
#include "ContactPoint.h"
#include "Vector.h"
#include "Vec3.hpp"

namespace NNE{

class ContactConstraint: public Constraint
{
public:
	
	ContactConstraint()
		:m_normalMassInv(0),
		m_friction(0), m_restitution(0)
	{}

	~ContactConstraint()
	{}

	NNEvoid setContactPoint(ContactPoint *cp)
	{
		m_cp = cp;
		m_normalMassInv = 0;
		m_friction = 0; 
		m_restitution = 0;
	}

	NNEvoid buildJacobian();

	NNEvoid resolve(NNEfloat inv_dt);

private:

	ContactPoint *m_cp;

	Vector<12> m_normal_J;

	Vector<12> m_tangent_J;

	Vector<12> m_oldVelocity;

	// m_normalMassInv = 1 / J_normal * invMass * J_normal^T
	NNEfloat m_normalMassInv;

	// m_tangentMassInv = 1 / J_tangent * invMass * J_tangent^T
	NNEfloat m_tangentMassInv;

	NNEfloat m_friction;

	NNEfloat m_restitution;
};

} // manespace NNE

#endif // NNE_CONTACTCONSTRAINT_H