#ifndef NNE_STACKALLOCATOR_H
#define NNE_STACKALLOCATOR_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"

namespace NNE{

/*
	double ended stack allocator
*/
class StackAllocator
{
public:
	
	StackAllocator()
		:m_size(0), m_numAllocL(0), m_numAllocH(0)
	{}

	~StackAllocator()
	{
		alwaysAssertMsg(m_startPtr == m_LTopPtr, "MEMORY LEAK DETECTED!");
		alwaysAssertMsg(m_endPtr == m_HTopPtr, "MEMORY LEAK DETECTED!");

		::free(m_startPtr);
	}

	NNEbool init(size_t);

	// allocate memory from lower stack
	// memory allocated by allocL is expected on last long
	// add 8 bytes before the aligned address, the first 4 bytes
	// store the allocation sequence id which ensure the order of 
	// deallocation, the other 4 bytes store the original address.
	NNEvoid *allocL(size_t s, size_t align = 4);

	NNEvoid deallocL(NNEvoid *);

	// allocate memory from high stack
	// memory allocated by allocH is expected to be temporary
	NNEvoid *allocH(size_t s, size_t align = 4);

	NNEvoid deallocH(NNEvoid *);

private:

	// uncopyable
	StackAllocator(const StackAllocator &);
	StackAllocator &operator = (const StackAllocator &);

	// the lower starting address of the memory chunck
	NNEvoid *m_startPtr;

	// the higher end address of the memory chunck
	NNEvoid *m_endPtr;

	// the top pointer of the stack starting from the lower end
	NNEvoid *m_LTopPtr;

	// the top pointer of the stack starting from the high end
	NNEvoid *m_HTopPtr;

	// the number of allocation from the lower stack
	NNEuint m_numAllocL;

	// the number of allocation from the high stack
	NNEuint m_numAllocH;

	// byte size of the entire memory chunck
	size_t m_size;
};

} // manespace NNE

#endif // NNE_STACKALLOCATOR_H