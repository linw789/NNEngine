#ifndef NNE_MEMORYPOOL_H
#define NNE_MEMORYPOOL_H

#include "PlatformSpecs.h"

namespace NNE{

/*
	A chunck of memory divided into equally sized
	blocks used for PoolAllocator
*/
class MemoryPool
{
public:
	
	MemoryPool()
	{}

	~MemoryPool()
	{}

private:

	// use byte for pointer calculation
	NNEvoid *m_startPtr;

	// byte size of the memory pool = (memory block size + nextPtr size) * block count
	NNEuint m_blockCount;

	NNEuint m_freeBlockCount; 

	// byte size of each memory block
	size_t m_blockSize;

	size_t m_alignment;

	// free list head ponter
	NNEvoid *m_headPtr;
};

} // manespace NNE

#endif // NNE_MEMORYPOOL_H