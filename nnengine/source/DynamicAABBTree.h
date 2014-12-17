#ifndef NNE_DYNAMICAABBTREE_H
#define NNE_DYNAMICAABBTREE_H

#include "PlatformSpecs.h"
#include "Broadphase.h"
#include "AABB.h"
#include "Array.h"
#include "BroadphasePair.h"
#include "RigidBody.h"

namespace NNE{

/*
	Broad phase phycis collision detection

	reference:
	http://allenchou.net/2014/02/game-physics-broadphase-dynamic-aabb-tree/
	http://www.randygaul.net/2013/08/06/dynamic-aabb-tree/
*/
class DynamicAABBTree : public Broadphase
{
public:

	enum
	{
		QUERY_ARRAY_CAPACITY = 256,
	};

	// tree node
	// all nodes are store in a array, using index to reference parent 
	// or child node. -1 means NULL
	struct tNode
	{
		tNode()
			:parent(-1), left(-1), right(-1),
			fatAabb(), proxy(NULL)
		{}

		// in binary tree, each node either contains none or 2 children
		NNEbool isLeaf()
		{
			return NULL != proxy;
		}

		NNEvoid updateFatAabb(NNEfloat margin)
		{
			fatAabb.setMinMaxPoint(proxy->m_aabb.getMinPoint(), proxy->m_aabb.getMaxPoint());
			fatAabb.bulge(margin);
		}

		AABB getAABB()
		{
			if (isLeaf())
			{
				return proxy->m_aabb;
			}
			else
			{
				return fatAabb;
			}
		}

		// test whether the fat aabb still encloses the aabb
		NNEbool insideFatAabb()
		{
			return fatAabb.ifEnclose(proxy->m_aabb);
		}

		union
		{
			NNEint parent;
			// when the node is unused it's part of a free list
			NNEint next; 
		};

		// children
		NNEint left;
		NNEint right;

		// when the node is a branch node the FatAabb is the union of its two children's aabbs
		// when the node is a leaf the FatAabb is the looser bound of the proxy
		AABB fatAabb;
	
		BroadphaseProxy *proxy;
	};
	
	DynamicAABBTree(NNEuint n);

	~DynamicAABBTree()
	{}

	NNEvoid destroy()
	{
		Broadphase::destroy();
	}

	NNEvoid addCollidable(Collidable *c)
	{
		BroadphaseProxy *proxy = createProxy(c);
		addProxy(proxy);
		insert(proxy);
	}

	NNEvoid insert(BroadphaseProxy *proxy);

	NNEvoid remove(BroadphaseProxy *proxy);

	// if any node's aabb breach out its fatAabb, remove the node
	// and re-insert it back
	NNEvoid update();

	// find all proxies that overlap with the parameter proxy
	NNEvoid findPairs(BroadphaseProxy *proxy);

	// make sure the proxies in the array has unique id in ascending order
	NNEvoid genBroadphasePairs();

	NNEvoid reset();

private:

	// branch aabb encloses its two children's aabb
	NNEvoid updateBranchAabb(NNEint index);

	NNEint m_root;

	NNEint m_freeListHead;

	Array<tNode> m_nodes;
};

} // manespace NNE

#endif // NNE_DYNAMICAABBTREE_H