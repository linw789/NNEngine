#ifndef NNE_BUFFEROBJECT_GPU_H
#define NNE_BUFFEROBJECT_GPU_H

#include "PlatformSpecs.h"

#include "DebugUtilities.h"
// have to include this header if references of this type are declared
//#include "VertexArrayCPU.h"

namespace NNE{

/*
	Vertex data stored on gpu memory in the order of 
	position, normal, tangent, color, texcoord0, texcoord1.
*/
class BufferObjectGPU
{
public:

	// default constructor
	BufferObjectGPU()
		:m_id(0), m_target(GPU_INVALID_ENUM), m_usage(GPU_INVALID_ENUM)
	{}

	BufferObjectGPU(GPUenum type, size_t size, const void *data, GPUenum usage);

	~BufferObjectGPU()
	{
		GL_DEBUG(glDeleteBuffers(1, &m_id));
	}

	NNEuint getID() const
	{
		return m_id;
	}

	GPUenum getBufferType() const
	{
		return m_target;
	}

	NNEvoid create(GPUenum t, size_t size, const void *data, GPUenum u);

	// change data as well as the size of VBO
	NNEvoid updateData(size_t size, const void *data)
	{
		bind();
		GL_DEBUG(glBufferData(m_target, size, data, m_usage));
	}

	NNEvoid updateSubData(GPUintptr offset, GPUsizeiptr size, const GPUvoid *data)
	{
		bind();
		GL_DEBUG(glBufferSubData(m_target, offset, size, data));
	}

	NNEvoid bind() const
	{
		debugAssertMsg(0 != m_id, "Buffer doesn't exist!");
		GL_DEBUG(glBindBuffer(m_target, m_id));
	}

	NNEvoid unbind() const
	{
		GL_DEBUG(glBindBuffer(m_target, 0));
	}

	// used for uniform buffer
	NNEvoid bindRange(GPUint bindingPoint, GPUint offset, GPUsizei size)
	{
		bind();
		debugAssertMsg(0 != m_id, "Buffer doesn't exist");
		GL_DEBUG(glBindBufferRange(m_target, bindingPoint, m_id, offset, size));
	}

private:

	// uncopyable
	BufferObjectGPU(const BufferObjectGPU &);
	BufferObjectGPU &operator= (const BufferObjectGPU &);

	// buffer id
	NNEuint m_id;
	// buffer type
	GPUenum m_target;
	// usage hint
	GPUenum m_usage;
};

} // manespace NNE

#endif // NNE_BUFFEROBJECT_GPU_H