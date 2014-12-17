#include "RigidBody.h"
#include "Math.h"

namespace NNE{

NNEvoid RigidBody::applyDamping()
{
	m_linearVelocity *= m_linearDamping;
	m_angularVelocity *= m_angularDamping;

	// 
	//NNEfloat threshold = 1e-5f;
	//if (Math::abs(m_linearVelocity.x) < threshold)
	//	m_linearVelocity.x = 0;
	//if (Math::abs(m_linearVelocity.y) < threshold)
	//	m_linearVelocity.y = 0;
	//if (Math::abs(m_linearVelocity.z) < threshold)
	//	m_linearVelocity.z = 0;
	//if (Math::abs(m_angularVelocity.x) < threshold)
	//	m_angularVelocity.x = 0;
	//if (Math::abs(m_angularVelocity.y) < threshold)
	//	m_angularVelocity.y = 0;
	//if (Math::abs(m_angularVelocity.z) < threshold)
	//	m_angularVelocity.z = 0;
}

NNEvoid RigidBody::integrateOrietation(NNEfloat deltatime)
{
	Quaternion q(m_angularVelocity.x,
				 m_angularVelocity.y,
				 m_angularVelocity.z, 0.0f);

	Quaternion spin = 0.5f * q * m_orientation;

	m_orientation += spin * deltatime;
	m_orientation.normalize();
	m_worldTransform.setRotate(Mat3(m_orientation));

	updateInertia();
}

} // namespace NNE