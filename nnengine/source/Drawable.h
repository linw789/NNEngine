#ifndef NNE_DRAWABLE_H
#define NNE_DRAWABLE_H

#include "Entity.h"
#include "String.h"
#include "Material.h"
#include "Vec2.hpp"
#include "Collidable.h"
#include "RigidBody.h"

namespace NNE{

class Mesh;
class Skin;
class Material;
class GPUProgram;
class Skeleton;
class Collidable;
class RigidBody;

/*
	Represent anything that can be rendered on the screan.
*/
class Drawable: public Entity
{
public:
	
	// drawable has to have a name
	Drawable(Sid name)
		:Entity(name), m_mesh(NULL), m_skin(NULL), 
		m_mtrl(NULL), m_visbile(true), m_isCulled(false),
		m_ifDrawNormal(false), m_isPhysicsEnabled(false)
	{}

	Drawable()
		:Entity(), m_mesh(NULL), m_mtrl(NULL), m_visbile(true), m_isCulled(false),
		m_ifDrawNormal(false)
	{}

	virtual ~Drawable()
	{}

	NNEvoid setMesh(Mesh *m);

	NNEvoid setSkin(Skin *s);

	NNEvoid setMaterial(Material *m);

	NNEvoid setCollidable(Collidable *c);

	NNEvoid setRigidBody(RigidBody *r)
	{
		setCollidable(dynamic_cast<Collidable *>(r));
	}

	NNEvoid setVisbility(NNEbool v)
	{
		m_visbile = v;
	}

	NNEbool isCulled()
	{
		return m_isCulled;
	}

	NNEvoid setDrawNormal(NNEbool d)
	{
		m_ifDrawNormal = d;
	}

	NNEbool ifDrawNormal()
	{
		return m_ifDrawNormal;
	}

	NNEvoid enablePhysics()
	{
		m_isPhysicsEnabled = true;
	}

	NNEvoid disablePhysics()
	{
		m_isPhysicsEnabled = false;
	}

	NNEvoid draw();

	NNEvoid playAnimation();

	// for debug use
	// need to modify code to eliminate offset matrix
	NNEvoid drawSkeleton();

	Mesh *getMesh()
	{
		return m_mesh;
	}

	Skin *getSkin()
	{
		return m_skin;
	}

	Material *getMaterial()
	{
		return m_mtrl;
	}

	GPUProgram *getGPUProg()
	{
		return m_mtrl->getGPUProg();
	}

	NNEvoid update();

protected:

	Mesh *m_mesh;

	Skin *m_skin;

	Material *m_mtrl;

	Collidable *m_collidable;

	NNEbool m_visbile;

	NNEbool m_isCulled;

	NNEbool m_ifDrawNormal;

	NNEbool m_isPhysicsEnabled;
};

} // manespace NNE

#endif // NNE_DRAWABLE_H