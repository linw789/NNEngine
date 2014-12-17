#include "VertexArrayObject.h"
#include "RenderDevice.h"

namespace NNE{

NNEvoid VertexArrayObject::create()
{
	debugAssertMsg(0 == m_id, "VAO has already been created!");

	GL_DEBUG(glGenVertexArrays(1, &m_id));
}

// http://www.parashift.com/c++-faq-lite/const-correctness.html
// void VertexArrayObject::create(const VertexArrayCPU &va, const BufferObject &bo)
NNEvoid VertexArrayObject::create(VertexArrayCPU &va, BufferObjectGPU &bo)
{
	debugAssertMsg(0 == m_id, "VAO has already been created!");
	debugAssertMsg(0 != bo.getID(), "Buffer Object doesn't exist!");

	GL_DEBUG(glGenVertexArrays(1, &m_id));
	GL_DEBUG(glBindVertexArray(m_id));
	bo.bind();

	specifyAttrib(va, bo);

	GL_DEBUG(glBindVertexArray(0));
	bo.unbind();
}

NNEvoid VertexArrayObject::create(VertexArrayCPU &va, BufferObjectGPU &bo, BufferObjectGPU &indObj)
{
	debugAssertMsg(0 == m_id, "VAO has already been created!");
	debugAssertMsg(0 != bo.getID(), "Buffer Object doesn't exist!");
	debugAssertMsg(0 != indObj.getID(), "Index Buffer Object doesn't exist!");

	GL_DEBUG(glGenVertexArrays(1, &m_id));
	GL_DEBUG(glBindVertexArray(m_id));

	bo.bind();
	indObj.bind(); // bind index buffer to VAO
	
	specifyAttrib(va, bo);

	GL_DEBUG(glBindVertexArray(0));
	bo.unbind();
	indObj.unbind();
}

void VertexArrayObject::specifyAttrib(VertexArrayCPU &va, BufferObjectGPU &bo)
{
	//NNEuint vertCount = va.getVertNum();
	size_t offsetPtr = 0;
	//debugAssertMsg(0 != vertCount, "Vertex Array has to have at least position data");

	// specify position attribute
	// When you refer to an enum inside a class where the enum is declared, 
	// you do not need to specify the name of the enum.
	// GL_DEBUG(glEnableVertexAttribArray(ATTRIB_TYPE::POSITION));

	GL_DEBUG(glEnableVertexAttribArray(NNE_POSITION_ATTRIB));
	GL_DEBUG(glVertexAttribPointer(NNE_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, 
		va.m_vertBytes, (GLvoid *)offsetPtr));

	offsetPtr += va.m_positionBytes;

	if (va.m_normalBytes) // layout(location = 1)
	{
		GL_DEBUG(glEnableVertexAttribArray(NNE_NORMAL_ATTRIB));
		GL_DEBUG(glVertexAttribPointer(NNE_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, 
			va.m_vertBytes, (GLvoid *)offsetPtr));
		
		offsetPtr += va.m_normalBytes;
	}

	if (va.m_tangentBytes) // layout(location = 2)
	{
		GL_DEBUG(glEnableVertexAttribArray(NNE_TANGENT_ATTRIB));
		GL_DEBUG(glVertexAttribPointer(NNE_TANGENT_ATTRIB, 4, GL_FLOAT, GL_FALSE, 
			va.m_vertBytes, (GLvoid *)offsetPtr));
		
		offsetPtr += va.m_tangentBytes;
	}

	if (va.m_colorBytes) // layout(location = 3)
	{
		GL_DEBUG(glEnableVertexAttribArray(NNE_COLOR_ATTRIB));
		GL_DEBUG(glVertexAttribPointer(NNE_COLOR_ATTRIB, 4, GL_FLOAT, GL_FALSE, 
			va.m_vertBytes, (GLvoid *)offsetPtr));
		
		offsetPtr += va.m_colorBytes;
	}

	if (va.m_texcoord0Bytes) // layout(location = 4)
	{
		GL_DEBUG(glEnableVertexAttribArray(NNE_TEXCOORD0_ATTRIB));
		GL_DEBUG(glVertexAttribPointer(NNE_TEXCOORD0_ATTRIB, 2, GL_FLOAT, GL_FALSE, 
			va.m_vertBytes, (GLvoid *)offsetPtr));
		
		offsetPtr += va.m_texcoord0Bytes;
	}

	if (va.m_texcoord1Bytes) // layout(location = 5)
	{
		GL_DEBUG(glEnableVertexAttribArray(NNE_TEXCOORD1_ATTRIB));
		GL_DEBUG(glVertexAttribPointer(NNE_TEXCOORD1_ATTRIB, 2, GL_FLOAT, GL_FALSE, 
			va.m_vertBytes, (GLvoid *)offsetPtr));
		
		offsetPtr += va.m_texcoord1Bytes;
	}

	if (va.m_boneWeightBytes) // layout(location = 6 for bone index, 7 for bone weight)
	{
		GL_DEBUG(glEnableVertexAttribArray(NNE_BONE_INDEX_ATTRIB));
		// use integer version of this routine to specify bone index
		GL_DEBUG(glVertexAttribIPointer(NNE_BONE_INDEX_ATTRIB, 1, GL_UNSIGNED_INT, 
			va.m_vertBytes, (GLvoid *)offsetPtr));

		// specify bone weights
		GL_DEBUG(glEnableVertexAttribArray(NNE_BONE_WEIGHT_ATTRIB));
		GL_DEBUG(glVertexAttribPointer(NNE_BONE_WEIGHT_ATTRIB, 4, GL_FLOAT, GL_FALSE, 
			va.m_vertBytes, (GLvoid *)(offsetPtr + sizeof(NNEuint))));
	}
}

NNEvoid VertexArrayObject::specifyAttrib(GLuint index, GLint size, GLsizei stride, const GLvoid *offset, BufferObjectGPU &bo)
{
	bind();

	GL_DEBUG(glEnableVertexAttribArray(index));
	bo.bind();
	GL_DEBUG(glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset));
	bo.unbind();

	unbind();
}

NNEvoid VertexArrayObject::setInstancingRate(GLuint index, GLuint r)
{
	bind();

	GL_DEBUG(glVertexAttribDivisor(index, r));

	unbind();
}

} // namespace NNE