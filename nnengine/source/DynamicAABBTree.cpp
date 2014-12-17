#include "DynamicAABBTree.h"
#include "DebugUtilities.h"

namespace NNE{

DynamicAABBTree::DynamicAABBTree(NNEuint n)
	:Broadphase(n), m_root(-1)
{
	m_nodes.assign(n);

	// initialized the free list with all nodes
	m_freeListHead = 0;
	NNEuint tmpsize = m_nodes.size() - 1;
	for (NNEuint i = 0; i < tmpsize; ++i)
	{
		m_nodes[i].next = i + 1;
	}
}

NNEvoid DynamicAABBTree::insert(BroadphaseProxy *proxy)
{
	proxy->updateAabb();

	if (-1 == m_root)
	{
		m_root = m_freeListHead;
		m_freeListHead = m_nodes[m_freeListHead].next;
		m_nodes[m_root].parent = -1;
		m_nodes[m_root].proxy = proxy;
		m_nodes[m_root].updateFatAabb(0.2f);
		proxy->m_aabb.setUserData(m_root);
	}
	else
	{
		NNEint tmpi = m_root; // start from the root
		while(-1 != tmpi)
		{
			if (m_nodes[tmpi].isLeaf())
			{
				// add a branch node containing the current node and 
				// the newly inserted node as its children

				// left child index
				NNEint left = m_freeListHead;
				m_freeListHead = m_nodes[m_freeListHead].next;
				m_nodes[left].parent = tmpi;
				m_nodes[left].proxy = m_nodes[tmpi].proxy;
				m_nodes[left].fatAabb = m_nodes[tmpi].fatAabb;
				m_nodes[left].proxy->m_aabb.setUserData(left);

				// right child(newly inserted) index
				NNEint right = m_freeListHead;
				m_freeListHead = m_nodes[m_freeListHead].next;
				m_nodes[right].proxy = proxy;
				m_nodes[right].updateFatAabb(0.2f);
				m_nodes[right].parent = tmpi;
				m_nodes[right].proxy->m_aabb.setUserData(right);

				// new branch node
				m_nodes[tmpi].proxy = NULL;
				m_nodes[tmpi].left = left;
				m_nodes[tmpi].right = right;
				updateBranchAabb(tmpi);

				tmpi = -1;
			}
			else
			{
				// union the inserted aabb with each child of the node,
				// propagate down to the child with smaller volume
				NNEint left = m_nodes[tmpi].left;
				NNEint right = m_nodes[tmpi].right;
				AABB Laabb;
				AABB::merge(m_nodes[left].getAABB(), proxy->m_aabb, Laabb);
				AABB Raabb;
				AABB::merge(m_nodes[right].getAABB(), proxy->m_aabb, Raabb);
				if (Laabb.volume() < Raabb.volume())
				{
					tmpi = left;
				}
				else
				{
					tmpi = right;
				}
			}
		}
	}
}

NNEvoid DynamicAABBTree::remove(BroadphaseProxy *proxy)
{
	NNEint nodeIndx = proxy->m_aabb.getUserData();
	debugAssertMsg(-1 != nodeIndx, "the proxy hasn't been inserted");
	m_nodes[nodeIndx].proxy = NULL;
	proxy->m_aabb.setUserData(-1);

	NNEint parentIndex = m_nodes[nodeIndx].parent;
	// if it's the root node
	if (-1 == parentIndex)
	{
		m_root = -1;
		// put the node back to the free list
		m_nodes[nodeIndx].next = m_freeListHead;
		m_freeListHead = nodeIndx;
	}
	else
	{
		NNEint siblingIndex = m_nodes[parentIndex].left == nodeIndx ? 
			m_nodes[parentIndex].right : m_nodes[parentIndex].left;
		
		NNEint grandParent = m_nodes[parentIndex].parent;
		// if has grandParent
		if (-1 != grandParent)
		{
			m_nodes[siblingIndex].parent = grandParent;
			if (m_nodes[grandParent].left == parentIndex)
			{
				m_nodes[grandParent].left = siblingIndex;
			}
			else
			{
				m_nodes[grandParent].right = siblingIndex;
			}
			updateBranchAabb(grandParent);
		}
		else
		{
			m_root = siblingIndex;
			m_nodes[siblingIndex].parent = -1;
		}

		// put the node and its parent back to the free list
		m_nodes[nodeIndx].next = m_freeListHead;
		m_freeListHead = nodeIndx;
		m_nodes[parentIndex].left = -1;
		m_nodes[parentIndex].right = -1;
		m_nodes[parentIndex].next = m_freeListHead;
		m_freeListHead = parentIndex;
	}	
}

NNEvoid DynamicAABBTree::updateBranchAabb(NNEint i)
{
	AABB::merge(m_nodes[m_nodes[i].left].getAABB(), m_nodes[m_nodes[i].right].getAABB(), m_nodes[i].fatAabb);
	NNEint tmpi = m_nodes[i].parent;
	while (-1 != tmpi)
	{
		AABB::merge(m_nodes[m_nodes[tmpi].left].getAABB(), m_nodes[m_nodes[tmpi].right].getAABB(), m_nodes[tmpi].fatAabb);
		tmpi = m_nodes[tmpi].parent;
	}
}

NNEvoid DynamicAABBTree::update()
{
	// first update all proxies' aabbs
	for (NNEuint i = 0; i < m_nodes.size(); ++i)
	{
		if (NULL != m_nodes[i].proxy)
		{
			m_nodes[i].proxy->updateAabb();
		}
	}

	// if any node's aabb breach out its fatAabb, remove the node
	// and re-insert it back
	for (NNEuint i = 0; i < m_nodes.size(); ++i)
	{
		if (m_nodes[i].isLeaf())
		{
			if (!m_nodes[i].insideFatAabb())
			{
				BroadphaseProxy *tmp = m_nodes[i].proxy;
				remove(tmp);
				insert(tmp);
			}
		}
	}

	// generate possible colliding pairs
	genBroadphasePairs();
}

NNEvoid DynamicAABBTree::findPairs(BroadphaseProxy *proxy)
{
	// store node indices
	NNEint nodeIndexArray[QUERY_ARRAY_CAPACITY];
	NNEuint sp = 1;
	nodeIndexArray[0] = m_root;

	while (sp)
	{
		debugAssert(sp < QUERY_ARRAY_CAPACITY);

		tNode *tmpNode = &m_nodes[nodeIndexArray[--sp]];

		if (tmpNode->getAABB().collide(proxy->m_aabb))
		{
			if (tmpNode->isLeaf())
			{
				// avoid the chance of duplicating pairs like proxy_ab and proxy_ba
				if (proxy->getUniqueId() < tmpNode->proxy->getUniqueId())
				{
					m_pairCache.addPair(proxy, tmpNode->proxy);
				}
			}
			else
			{
				nodeIndexArray[sp++] = tmpNode->left;
				nodeIndexArray[sp++] = tmpNode->right;	
			}
		}
	}
}

NNEvoid DynamicAABBTree::genBroadphasePairs()
{
	for (NNEuint i = 0; i < m_proxyArray.size(); ++i)
	{
		findPairs(m_proxyArray[i]);
	}
}

NNEvoid DynamicAABBTree::reset()
{
	m_freeListHead = 0;
	for (NNEuint i = 0; i < m_nodes.size(); ++i)
	{
		m_nodes[i].next = i + 1;
		m_nodes[i].proxy = NULL;
		m_nodes[i].left = -1;
		m_nodes[i].right = -1;
	}
}

} // namespace NNE