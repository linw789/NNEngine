#ifndef NNE_ENTITY_H
#define NNE_ENTITY_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "String.h"
#include "Matrix4.hpp"
#include "Node.h"
#include "Vec4.hpp"

namespace NNE{

class Node;

/*
	base class for all types of entities that are managed by Scene,
	such as Camera, Light, Drawable and etc. Subclasses need to be
	attached to nodes to make effect in the scene.
*/
class Entity
{
public:

	// each entity must have a name
	Entity(Sid n)
		:m_name(n), m_node(NULL)
	{}

	// construct with no name
	Entity()
		:m_name(0), m_node(NULL)
	{}

	virtual ~Entity()
	{}

	Sid getNameSid()
	{
		return m_name;
	}

	virtual void init()
	{
		debugAssertMsg(NULL != m_node, "Attach the camera to a node before initialization!");
	}

	// set the node this entity is attached to 
	void setNode(Node *n)
	{
		debugAssertMsg(NULL == m_node, "This entity has already been attached to a node!");
		m_node = n;
	}

	virtual Node *getNode()
	{
		return m_node;
	}

	NNEfloat *getPosPtr() const
	{
		return (*m_node)[3];
	}

	const Mat4 getLocalTransform()
	{
		return m_node->getLocalTransform();
	}

	const Mat4 getGlobalTransform()
	{
		return m_node->getGlobalTransform();
	}

protected:

	Sid m_name;

	// node pointer that this entity is attached to,
	Node *m_node;
};

} // manespace NNE

#endif // NNE_ENTITY_H