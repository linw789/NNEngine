#include "PoolAllocator.h"

namespace NNE{

PoolAllocator::~PoolAllocator()
{
	alwaysAssertMsg(m_freeBlockCount == m_memBlockCount, "MEMORY LEAK DETECTED!");

	// to do
}

NNEbool PoolAllocator::init(NNEuint numBlock, size_t memsize, size_t align /* = 4 */)
{
	// and for now poolAllocator can only handle 4 byte aligned data
	alwaysAssertMsg(memsize % align ==0, "Memory block size has to be 4 bytes algined!");

	m_memBlockCount = numBlock;
	m_freeBlockCount = numBlock;
	m_memBlockSize = memsize;
	m_alignment = align;

	// usually the address return by ::malloc is 4-byte aligned
	m_startPtr = ::malloc(numBlock * (m_memBlockSize + sizeof(NNEvoid *)));
	m_headPtr = (NNEvoid *)((NNEuint)m_startPtr + sizeof(NNEvoid *));

	// initialize each memory block
	for (NNEuint i = 0; i < m_memBlockCount - 1; ++i)
	{
		*getNextBlockPtr(i) = getMemBlock(i + 1);
	}
	// the last memory block
	{
		*getNextBlockPtr(m_memBlockCount - 1) = NULL;
	}

	return true;
}

NNEvoid *PoolAllocator::alloc()
{
	debugAssertMsg(m_freeBlockCount > 0, "out of memory");
	--m_freeBlockCount;
	NNEvoid *pRes = m_headPtr;
	m_headPtr = *getNextBlockPtr(m_headPtr);

	return pRes;
}

NNEvoid PoolAllocator::dealloc(NNEvoid *ptr)
{
	debugAssert(m_freeBlockCount <= m_memBlockCount);
	/*debugAssertMsg((NNEuint)ptr <= (NNEuint)m_startPtr + (m_memBlockSize + sizeof(NNEvoid *)) * (m_memBlockCount - 1),
		"Pointer address out bound!");*/
	++m_freeBlockCount;

	*getNextBlockPtr(ptr) = m_headPtr;
	m_headPtr = ptr;
}

} // namespace NNE