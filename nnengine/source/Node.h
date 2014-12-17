#ifndef NNE_NODE_H
#define NNE_NODE_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Matrix4.hpp"
#include "Vec3.hpp"
#include "Array.h"
#include "Quaternion.h"

namespace NNE{

class Entity;

/*
	Scene node managed by class Scene
	node is basically used for representing
	heirarchical transformation
*/
class Node: public Mat4
{
public:
	
	Node()
		:Mat4(), m_parent(NULL), m_entity(NULL)
	{
		identity();
	}

	~Node()
	{}

	NNEvoid addChild(Node *n)
	{
		m_children.push_back(n);
	}

	// a node can only link with one entity
	// it's bilateral link
	NNEvoid linkEntity(Entity *e);

	NNEvoid setTransform(const Mat4 &m4)
	{
		memcpy(m_val, m4.getDataPtr(), 16 * sizeof(NNEfloat));
	}

	const Mat4 &getLocalTransform()
	{
		return *(dynamic_cast<Mat4 *>(this));
	}

	const Mat4 getGlobalTransform()
	{
		if (NULL == m_parent)
		{
			return *(dynamic_cast<Mat4 *>(this));
		}
		else
		{
			return *(dynamic_cast<Mat4 *>(this)) * m_parent->getGlobalTransform();
		}
	}

private:

	// parent node that this node is attached to
	Node *m_parent;
	
	// array of all children being attached to this node
	Array<Node *> m_children;

	// one node can only contain one entity
	Entity *m_entity;
};

} // manespace NNE

#endif // NNE_NODE_H