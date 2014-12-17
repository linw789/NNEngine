#include "StackAllocator.h"

namespace NNE{

NNEbool StackAllocator::init(size_t memsize)
{
	debugAssertMsg(memsize % 4 == 0, "memory size has to be 4 bytes aligned!");

	m_startPtr = ::malloc(m_size);

	m_LTopPtr = m_startPtr;

	m_endPtr = (NNEvoid *)((NNEuint)m_startPtr + m_size);

	m_HTopPtr = m_endPtr;

	return true;
}

NNEvoid *StackAllocator::allocL(size_t s, size_t align /* = 4 */)
{
	++m_numAllocL;

	// size for rawPointer + size for allocation order id + (alignment)
	size_t extraSize = sizeof(NNEvoid *) + sizeof(NNEuint) + (align);

	// Adding (align) advances the pointer to next aligned range.
	// For example, alignment is 4, rawPtr is at 0x0003(align range is 0x0000 to 0x0003),
	// adding (4) advances the pointer to 0x0006(align range is 0x0004 to 0x0007).
	// Then & ~(AND operator, NOT operator) wipes out least significant bits, 
	// makes the pointer back to aligned position, in this case 0x0004.
	NNEvoid *ptr = (NNEvoid *)(((NNEuint)m_LTopPtr + extraSize) & ~(align));

	// store allocation sequence number
	*((NNEuint *)ptr - 2) = m_numAllocL;

	// store the raw pointer
	*((NNEvoid **)ptr - 1) = m_LTopPtr;

	// 
	m_LTopPtr = (NNEvoid *)((NNEuint)m_LTopPtr + s + extraSize);

	alwaysAssertMsg((NNEuint)m_LTopPtr < (NNEuint)m_HTopPtr, "Lower stack and high stack collide!");

	return ptr;
}

NNEvoid StackAllocator::deallocL(NNEvoid *ptr)
{
	alwaysAssertMsg(*((NNEuint *)ptr - 2) == m_numAllocL, 
		"Need to deallocate memory from stack the opposite order allocated!");

	--m_numAllocL;

	m_LTopPtr = *((NNEvoid **)ptr - 1);
}

NNEvoid *StackAllocator::allocH(size_t s, size_t align)
{
	++m_numAllocH;

	// no need to store original address, because m_HTopPtr in this case
	// is the original address.
	// Instead, need to store object size for deallocation, and allocation order id
	size_t extraSize = sizeof(NNEuint) + sizeof(NNEuint) + (align);

	m_HTopPtr = (NNEvoid *)((NNEuint)m_HTopPtr - s - extraSize);

	alwaysAssertMsg((NNEuint)m_HTopPtr > (NNEuint)m_LTopPtr, "Lower stack and high stack collide!");

	NNEvoid *ptr = (NNEvoid *)(((NNEuint)m_HTopPtr + extraSize) &~ (align));

	// store allocation sequence number
	*((NNEuint *)ptr - 2) = m_numAllocH;

	// store the requested size
	*((NNEuint *)ptr - 1) = s;

	return ptr;
}

NNEvoid StackAllocator::deallocH(NNEvoid *ptr)
{
	alwaysAssertMsg(*((NNEuint *)ptr - 2) == m_numAllocH, 
		"Need to deallocate memory from stack the opposite order allocated!");

	--m_numAllocH;

	m_HTopPtr = (NNEvoid *)((NNEuint)ptr + *((NNEuint *)ptr - 1));
}

} // namespace NNE