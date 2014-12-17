/*
	file: MemoryManagement.h

	reference: G3D\system.h

	this is a custom memory allocation.
*/
#ifndef NNE_POOL_ALLOCATOR_H
#define NNE_POOL_ALLOCATOR_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"

namespace NNE{

/*
	Use free list to implement Pool Allocator.
	For each element memory block, add 4 bytes in front
	as a pointer to the next element memory block.

	Allocator must be defined as a global object, which means
	the allocator itself resides on the executable's BSS segment,
	however, the memory it manipulates resides on a heap allocated
	beforehand by a memory manager.

	For now, PoolAllocator can only be 4 bytes aligned, both the
	starting pointer and the size of each memory block are 4 bytes
	aligned.
*/
class PoolAllocator
{
public:

	PoolAllocator()
		:m_startPtr(NULL), m_headPtr(NULL), m_memBlockCount(0), m_freeBlockCount(0), 
		m_memBlockSize(0), m_alignment(4)
	{}

	~PoolAllocator();

	NNEbool init(NNEuint numBlock, size_t memsize, size_t align = 4);

	// get one memory block 
	NNEvoid *alloc();

	// 
	NNEvoid dealloc(NNEvoid *);

	NNEuint getFreeBlockCount()
	{
		return m_freeBlockCount;
	}

private:

	// uncopyable
	PoolAllocator(const PoolAllocator &);
	PoolAllocator &operator = (const PoolAllocator &);

	NNEvoid *getMemBlock(NNEuint index);

	// get the pointer appended to each memory block
	// this pointer stores the pointer of the next memory block
	NNEvoid **getNextBlockPtr(NNEvoid *p);

	NNEvoid **getNextBlockPtr(NNEuint index);

	// use byte for pointer calculation
	NNEvoid *m_startPtr;

	// byte size of the memory pool = (memory block size + nextPtr size) * block count
	NNEuint m_memBlockCount;

	NNEuint m_freeBlockCount; 

	// byte size of each memory block
	size_t m_memBlockSize;

	size_t m_alignment;

	// free list head ponter
	NNEvoid *m_headPtr;
};

inline NNEvoid *PoolAllocator::getMemBlock(NNEuint index)
{
	debugAssertMsg(index < m_memBlockCount, "pointer address out bound");

	return (NNEvoid *)((NNEuint)m_startPtr + sizeof(NNEvoid *) + (m_memBlockSize + sizeof(NNEvoid *)) * index);
}

inline NNEvoid **PoolAllocator::getNextBlockPtr(NNEvoid *ptr)
{
	return (NNEvoid **)ptr - 1;
}

inline NNEvoid **PoolAllocator::getNextBlockPtr(NNEuint index)
{
	debugAssertMsg(index < m_memBlockCount, "pointer address out bound");

	return (NNEvoid **)getMemBlock(index) - 1;
}

} // namespace NNE

#endif // NNE_POOL_ALLOCATOR_H
