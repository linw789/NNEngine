#ifndef NNE_MESH_H
#define NNE_MESH_H

#include "Array.h"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include "Vec4.hpp"
#include "Color.hpp"
#include "String.h"
#include "VertexArrayCPU.h"
#include "BufferObjectGPU.h"
#include "VertexArrayObject.h"
#include "AABB.h"

namespace NNE{

class Drawable;

/*: public HasName // change to composition instead of inheritance*/
class Mesh
{
	friend class AssimpLoader;
	friend class HeightMap;

public:

	Mesh(Sid name, GPUenum shape)
		:m_name(name), m_primitiveType(shape)
	{

	}
	
	~Mesh()
	{}

	Sid getNameSid() const
	{
		return m_name;
	}

	NNEvoid setPrimitiveType(GPUenum t)
	{
		m_primitiveType = t;
	}

	GPUenum getPrimitiveType() const
	{
		return m_primitiveType;
	}

	NNEvoid setVertFormat(size_t pos, size_t normal, size_t tangent, size_t color,
		size_t texcoord0, size_t texcoord1, size_t boneweight)
	{
		m_va.setVertFormat(pos, normal, tangent, color, texcoord0, texcoord1, boneweight);
	}

	// vertex format consistency test
	NNEbool VFConsistencyTest(size_t pos, size_t normal, size_t tangent, size_t color,
		size_t texcoord0, size_t texcoord1, size_t boneweight)
	{
		return m_va.vertFormatTest(pos, normal, tangent, color, texcoord0, texcoord1, boneweight);
	}

	NNEvoid setVertex(NNEuint size, const NNEfloat *vert)
	{
		m_va.m_vertices.push_back((NNEchar *)vert, size);
	}

	NNEvoid setColor(NNEuint i, const Vec4 &clr)
	{
		// only when the vertex format previously specified contains
		// color attribute
		if (m_va.m_colorBytes)
		{
			*m_va.getColorPtr(i) = clr;
		}
	}

	// when adding a new vertex, addPos routine should be called at first
	NNEvoid addPosition(const Vec3 &pos)
	{
		m_va.addPos(pos);
	}

	NNEvoid addColor(const Vec4 &clr)
	{
		// only when the vertex format specified previously contains
		// color attribute
		if (m_va.m_colorBytes)
		{
			m_va.addColor(clr);
		}
	}

	// whether still contain vertex memory on CPU
	NNEbool hasVertCPU() const
	{
		return m_va.m_vertices.size() == 0? false : true; 
	}

	// whether still contain vertex memory on GPU
	NNEbool hasVertGPU() const
	{
		return m_vertBufGPU.getID() == 0? false : true;
	}

	NNEuint getIndexCount()
	{
		return m_va.m_triIndex.size();
	}

	GPUuint getVAO_Id()
	{
		return m_VAO.getID();
	}

	// create corresponding buffer object on gpu
	NNEvoid createVBO(GPUenum usageHint = GPU_WRITE_ONCE);
	
	// modify data and size
	NNEvoid updateVBO()
	{
		m_vertBufGPU.updateData(m_va.getVerticesBytes(), m_va.getVerticesPtr());
	}

	// modify sub-data, size remains the same
	NNEvoid updateSubVBO();

	// create corresponding index buffer object on gpu
	NNEvoid createIndexBO(GPUenum usageHint = GPU_WRITE_ONCE);

	// upload vertex data to GPU memory
	NNEvoid upload2GPU(GPUenum usageHint = GPU_WRITE_ONCE);

	// bind the m_vertBufGPU as the current vertex buffer object to GPU
	// if there is index, bind m_indexBufGPU as the current index buffer object to GPU
	// if this mesh uses VAO, directly bind VAO
	// use 3 functions to save if test
	//NNEvoid bindVertBuf();
	//NNEvoid bindVertIndexedBuf();

	virtual NNEvoid draw();

	NNEvoid drawNormal();

	AABB computeAABB();

	NNEvoid clear()
	{
		m_va.clear();
	}

private:

	Sid m_name;

	GPUenum m_primitiveType;

	// uncopyable class
	Mesh(const Mesh &);
	Mesh &operator= (const Mesh &);

public:
	// vertex array
	VertexArrayCPU m_va;

private:

	// corresponding vertex buffer object on gpu
	BufferObjectGPU m_vertBufGPU;
	// corresponding index buffer object on gpu
	BufferObjectGPU m_indexBufGPU;

	// vertex array object
	VertexArrayObject m_VAO;
};

} // namespace NNE

#endif // NNE_MESH_H