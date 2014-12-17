#include "Node.h"
#include "Entity.h"

namespace NNE{

// a node can only link with one entity
// it's bilateral link
void Node::linkEntity(Entity *e)
{
	debugAssertMsg(m_entity == NULL, "This node has already linked with other entity");
	m_entity = e;
	e->setNode(this);
}

} // namespace NNE