#include "Camera.h"
#include "Node.h"
#include "Math.h"
#include "Event.h"

namespace NNE{

NNEbool Camera::init(Vec3 &eye, Vec3 &focus, Vec3 &up,
			NNEfloat nearDis, NNEfloat farDist,
			NNEfloat fovy, NNEfloat aspect)
{
	debugAssertMsg(NULL != m_node, "Attach the camera to a node before initialization!");

	Vec3 axisZ = Vec3::normalize(Vec3(eye - focus));
	Vec3 axisX = Vec3::normalize(Vec3::cross(up, axisZ));
	Vec3 axisY = Vec3::normalize(Vec3::cross(axisZ, axisX));

	m_node->setAxisX(axisX);
	m_node->setAxisY(axisY);
	m_node->setAxisZ(axisZ);

	m_node->setPos(eye);

	/*updateViewMatrix();

	updatePerspective();

	updatePersViewMat();*/

	m_dirtyFlag |= VIEW_MATRIX_DIRTY | PERSPECTIVE_MATRIX_DIRTY;

	NNE_EVENTS_REGISTER_MEMBER_FUNC(ViewportChangedEvent, Camera, viewportChangedHandler, this);

	return true;
}

NNEbool Camera::updateViewMatrix()
{
	// create the orientation of the camera in the world space(right-handed)
	Vec3 axisX = Vec3::normalize(m_node->getAxisX());
	Vec3 axisY = Vec3::normalize(m_node->getAxisY());
	Vec3 axisZ = Vec3::normalize(m_node->getAxisZ());
	
	Vec3 eye = m_node->getPos();

	// basically the view matrix is the inverse of the camera's 
	// transform matrix based in the world space.
	// the inverse of a rotating matrix is equal to the transpose
	// of the same rotating matrix, because the matrix is othornormal
	// camera's world transform = T * R, therefore the inverse is inv(R) * inv(T)
	// extract the fourth column of inv(R) * inv(T) for translation

	//Mat4 viewMat;
	// first column
	m_viewMat.setAxisX(Vec3(axisX.x, axisY.x, axisZ.x));
	// second column
	m_viewMat.setAxisY(Vec3(axisX.y, axisY.y, axisZ.y));
	// third column
	m_viewMat.setAxisZ(Vec3(axisX.z, axisY.z, axisZ.z));
	// translation
	m_viewMat[3][0] = -Vec3::dot(axisX, eye); 
	m_viewMat[3][1] = -Vec3::dot(axisY, eye); 
	m_viewMat[3][2] = -Vec3::dot(axisZ, eye);
	// homogeneous
	m_viewMat[3][3] = 1.0f;

	return true;
}

NNEbool Camera::updatePerspective()
{
	NNEfloat tangent = tanf(Math::deg2rad(m_fovy * 0.5f)); // tangent of fovY
	NNEfloat half_h = m_near * tangent; // half height of the near plane
	NNEfloat half_w = half_h * m_aspect; // half width of the near plane
	NNEfloat tmp = 1 / (m_far - m_near);

	// assuming the viewport is symmetric
	m_persMat.zero();
	m_persMat[0][0] = m_near / half_w;
	m_persMat[1][1] = m_near / half_h;
	m_persMat[2][2] = -(m_far + m_near) * tmp;
	m_persMat[3][2] = -2 * m_far * m_near *tmp;
	m_persMat[2][3] = -1.0f;

	return true;
}

NNEbool Camera::updatePersViewMat()
{
	m_persViewMat = m_persMat * m_viewMat;

	return true;
}

} // namespace NNE