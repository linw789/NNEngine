#include "BufferObjectGPU.h"
#include "VertexArrayCPU.h"

namespace NNE{

BufferObjectGPU::BufferObjectGPU(GPUenum t, size_t size, const void *data, GPUenum u)
{
	create(t, size, data, u);
}

NNEvoid BufferObjectGPU::create(GPUenum t, size_t size, const void *data, GPUenum u)
{
	m_target = t;
	m_usage = u;

	GL_DEBUG(glGenBuffers(1, &m_id));
	GL_DEBUG(glBindBuffer(t, m_id));

	GL_DEBUG(glBufferData(t, size, data, u));

	GL_DEBUG(glBindBuffer(t, 0));
}

} // namespace NNE