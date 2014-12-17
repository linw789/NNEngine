#include "Drawable.h"
#include "Mesh.h"
#include "Skin.h"
#include "Material.h"
#include "Skeleton.h"
#include "Array.h"
#include "DebugRenderer.h"

namespace NNE{

NNEvoid Drawable::setMesh(Mesh *m)
{
	debugAssert(NULL != m);

	m->upload2GPU();

	m_mesh = m;
}

NNEvoid Drawable::setSkin(Skin *s)
{
	debugAssert(NULL != s);

	m_skin = s;

	m_mesh = dynamic_cast<Mesh *>(s);

	m_skin->upload2GPU();
}

NNEvoid Drawable::setMaterial(Material *m)
{
	debugAssert(NULL != m);
	m_mtrl = m;
}

NNEvoid Drawable::setCollidable(Collidable *c)
{
	m_collidable = c;
	m_collidable->setTransform(m_node->getLocalTransform());

	m_isPhysicsEnabled = true;
}

NNEvoid Drawable::draw()
{
	debugAssertMsg(NULL != m_mesh, "The drawable doesn't has a mesh");

	m_mtrl->apply();

	m_mesh->draw();
}

NNEvoid Drawable::playAnimation()
{
	if (NULL != m_skin)
	{
		m_skin->animate();
		m_skin->getSkeleton()->uploadBoneXForm(m_mtrl->getGPUProg());
	}
}

// need to modify code to eliminate offset matrix
NNEvoid Drawable::drawSkeleton()
{
	debugAssertMsg(m_skin, "no skeleton to be drawn");

	Skeleton *sklt = m_skin->getSkeleton();

	NNEuint bonesize = sklt->m_boneArray.size();
	for (NNEuint i = 0; i < bonesize; ++i)
	{
		NNEint parent = sklt->m_boneArray[i].parentIndex;
		if (parent > -1)
		{
			Mat4 startp = m_node->getGlobalTransform() * sklt->m_boneArray[i].root_transform;
			Mat4 endp = m_node->getGlobalTransform() * sklt->m_boneArray[parent].root_transform;

			gDebugRenderer.drawLine(startp.getPos(), endp.getPos(), Color3(0.8f, NNEfloat(i) / bonesize, 0.0f), 2);
		}
	}
}

NNEvoid Drawable::update()
{
	if (m_isPhysicsEnabled)
	{
		m_node->setTransform(m_collidable->getTransform());
	}
}

} // namespace NNE