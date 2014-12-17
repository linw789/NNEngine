#ifndef NNE_MEMORYMANAGER_H
#define NNE_MEMORYMANAGER_H

#include "PlatformSpecs.h"

namespace NNE{

class MemoryManager
{
public:
	
	// default: allocating 4 * 1024 * 1024 bytes on system memory
	MemoryManager(size_t memszie = 4194304)
		:m_size(memszie)
	{
		centralHeap = ::malloc(memszie);
	}

	~MemoryManager()
	{} 

	//
	NNEvoid *alloc(size_t size, size_t alignment = 4);

	NNEvoid dealloc(NNEvoid *ptr);

private:

	NNEvoid *centralHeap;

	// memory size in byte
	size_t m_size;
};

} // manespace NNE

#endif // NNE_MEMORYMANAGER_H