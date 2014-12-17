#ifndef NNE_VERTEXARRAYOBJECT_H
#define NNE_VERTEXARRAYOBJECT_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "VertexArrayCPU.h"
#include "BufferObjectGPU.h"

namespace NNE{

// set all vertex attributes into one object,
// so they can be called all at one time to save gl call overhead.
// glVertexAttribPointer's first paremeter "index"​ will get its attribute 
// data from whatever buffer object is currently bound to GL_ARRAY_BUFFER.
// Therefore, one hase to call glBindBuffer(GL_ARRAY_BUFFER, id) before
// any glVertexAttribPointer calls.
class VertexArrayObject
{
public:

	VertexArrayObject()
		:m_id(0)
	{}

	~VertexArrayObject()
	{
		GL_DEBUG(glDeleteVertexArrays(1, &m_id));
	}

	GPUuint getID()
	{
		return m_id;
	}

	NNEvoid create();

	// VAO for vertex buffer without index 
	NNEvoid create(VertexArrayCPU &, BufferObjectGPU &);
	// VAO FOR indexed vertex buffer
	NNEvoid create(VertexArrayCPU &, BufferObjectGPU &, BufferObjectGPU &);

	NNEvoid bind()
	{
		debugAssertMsg(0 != m_id, "AVO doesn't exist!");
		GL_DEBUG(glBindVertexArray(m_id));
	}

	NNEvoid unbind()
	{
		GL_DEBUG(glBindVertexArray(0));
	}

	NNEvoid specifyAttrib(VertexArrayCPU &, BufferObjectGPU &);

	// index: attribute index, size: number of component
	NNEvoid specifyAttrib(GLuint index, GLint size, GLsizei stride, const GLvoid *offset, BufferObjectGPU &);

	NNEvoid setInstancingRate(GLuint index, GLuint r);

private:

	// uncopyable
	VertexArrayObject(const VertexArrayObject &);
	VertexArrayObject &operator= (const VertexArrayObject &);

	GPUuint m_id;
};

} // manespace NNE

#endif // NNE_VERTEXARRAYOBJECT_H