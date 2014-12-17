#ifndef NNE_CAMERA_H
#define NNE_CAMERA_H

#include "PlatformSpecs.h"
#include "Entity.h"
#include "String.h"
#include "Frustum.h"
#include "Vec3.hpp"
#include "Matrix4.hpp"
#include "Rect.h"
#include "Scene.h"

namespace NNE{

class Camera: public Entity
{

friend class Scene;

public:

	Camera(Sid name)
		:Entity(name), m_dirtyFlag (0), m_near(0.5f), m_far(500.0f), 
		m_fovy(60.0f), m_aspect(16.0f / 9.0f),
		m_viewMat(), m_persMat(), m_persViewMat()
	{
		m_viewMat.identity();
		m_persMat.identity();
		m_persViewMat.identity();
	}

	~Camera(){};

	enum MATRIX_DIRTY_BIT
	{
		CAMERA_CLEAN = 0,
		VIEW_MATRIX_DIRTY = 1,
		PERSPECTIVE_MATRIX_DIRTY = 2,
	};

	NNEbool init(Vec3 &eye = Vec3(0.0f, 0.0f, 0.0f),
		Vec3 &focus = Vec3(0.0f, 0.0f, -1.0f),
		Vec3 &up = Vec3(0.0f, 1.0f, 0.0f),
		NNEfloat nearDis = 0.5f, NNEfloat farDist = 1000.0f,
		NNEfloat fovy = 60.0f, NNEfloat aspect = 16.0f / 9.0f);

	Node *getNode()
	{
		//m_dirtyFlag |= VIEW_MATRIX_DIRTY;
		return m_node;
	}

	NNEvoid setClipDistance(NNEfloat n, NNEfloat f)
	{
		m_near = n;
		m_far = f;

		/*updatePerspective();
		updatePersViewMat();*/

		m_dirtyFlag |= PERSPECTIVE_MATRIX_DIRTY;
	}

	NNEvoid setFoV(NNEfloat fovy)
	{
		m_fovy = fovy;

		/*updatePerspective();
		updatePersViewMat();*/
		m_dirtyFlag |= PERSPECTIVE_MATRIX_DIRTY;
	}

	NNEvoid setAspect(NNEfloat a)
	{
		m_aspect = a;

		/*updatePerspective();
		updatePersViewMat();*/
		m_dirtyFlag |= PERSPECTIVE_MATRIX_DIRTY;
	}

	NNEvoid setLookat(const Vec3 &v)
	{
		m_node->setAxisZ(-v);
		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	NNEvoid setUpVector(const Vec3 &v)
	{
		m_node->setAxisY(v);
		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	NNEvoid setBiNormal(const Vec3 &v)
	{
		m_node->setAxisX(v);
		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	NNEvoid setPos(const Vec3 &v)
	{
		m_node->setPos(v);
		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	NNEvoid viewportChangedHandler(Rectui vp)
	{
		m_aspect = (NNEfloat)vp.getWidth() / (NNEfloat)vp.getHeight();

		m_dirtyFlag |= PERSPECTIVE_MATRIX_DIRTY;
	}

	NNEvoid mouseMoveLBDownHandler(Vec2i lastPos, Vec2i currPos)
	{
		NNEint horizontalDiff = currPos.x - lastPos.x;
		NNEint verticalDiff = currPos.y - lastPos.y;

		m_node->moveInAxisX(-horizontalDiff * 0.1f);
		m_node->moveInAxisY(verticalDiff * 0.1f);

		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	NNEvoid mouseMoveRBDownHandler(Vec2i lastPos, Vec2i currPos)
	{
		NNEint horizontalDiff = currPos.x - lastPos.x;
		NNEint verticalDiff = currPos.y - lastPos.y;

		if (0 != horizontalDiff)
		{
			m_node->rotate(-horizontalDiff * 0.5f, Vec3(0.0f, 1.0f, 0.0f));
		}
		if (0 != verticalDiff)
		{
			m_node->rotateX(-verticalDiff * 0.5f);
		}

		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	NNEvoid mouseWheelMoveHandler(NNEint delta)
	{
		m_node->moveInAxisZ(-delta * 0.01f);

		m_dirtyFlag |= VIEW_MATRIX_DIRTY;
	}

	const NNEfloat *getMatrixPtr()
	{
		(m_dirtyFlag & VIEW_MATRIX_DIRTY) && updateViewMatrix();

		(m_dirtyFlag & PERSPECTIVE_MATRIX_DIRTY) && updatePerspective();

		(m_dirtyFlag & (VIEW_MATRIX_DIRTY | PERSPECTIVE_MATRIX_DIRTY)) && updatePersViewMat();

		// clear bits
		m_dirtyFlag &= ~VIEW_MATRIX_DIRTY;
		m_dirtyFlag &= ~PERSPECTIVE_MATRIX_DIRTY;

		return m_viewMat.getDataPtr();
		
	}

	const Mat4 &getViewMat()
	{
		(m_dirtyFlag & VIEW_MATRIX_DIRTY) && updateViewMatrix();

		// clear bit
		m_dirtyFlag &= ~VIEW_MATRIX_DIRTY;

		return m_viewMat;
	}

	const Mat4 &getPersViewMat()
	{
		(m_dirtyFlag & VIEW_MATRIX_DIRTY) && updateViewMatrix();

		(m_dirtyFlag & PERSPECTIVE_MATRIX_DIRTY) && updatePerspective();

		(m_dirtyFlag & (VIEW_MATRIX_DIRTY | PERSPECTIVE_MATRIX_DIRTY)) && updatePersViewMat();

		// clear bits
		m_dirtyFlag &= ~VIEW_MATRIX_DIRTY;
		m_dirtyFlag &= ~PERSPECTIVE_MATRIX_DIRTY;

		return m_persViewMat;
	}

private:

	// calculate camera view matrix
	NNEbool updateViewMatrix();

	NNEbool updatePerspective();

	NNEbool updatePersViewMat();

private:

	NNEuint m_dirtyFlag;

	// distance of near and far clipping planes to the camera
	// value should be positive
	NNEfloat m_near;
	NNEfloat m_far;

	// field of view on Y axis
	NNEfloat m_fovy;
	// the ratio of the width to the height 
	NNEfloat m_aspect;

	// camera view matrix
	Mat4 m_viewMat;
	// camera perspective matrix
	Mat4 m_persMat;
	// m_persMat * m_viewMat
	Mat4 m_persViewMat;

	Frustum m_frustum;
};

} // manespace NNE

#endif // NNE_CAMERA_H