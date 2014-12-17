#ifndef NNE_VERTEXARRAY_H
#define NNE_VERTEXARRAY_H

#include "PlatformSpecs.h"
#include "Array.h"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

namespace NNE{

// each vertex would influenced by 4 bones at most
struct BoneWeight
{
	BoneWeight()
		:b0123(0),
		w0(0.0f), w1(0.0f), w2(0.0f), w3(0.0f)
	{}

	// i = 0, 1, 2, 3
	NNEuint getBoneIndex(NNEuint i);

	NNEvoid setBoneIndex(NNEuint i, NNEuint indx);

	// index of bone
	// packed 4 char into one integer, so the maximum number of bones is limited to 255
	// bone index starts from 1, 0 means no bone index assigned yet
	NNEuint b0123;
	// weight of each bone
	NNEfloat w0, w1, w2, w3;
};

// contain vertex data including position, normal,
// tangent, color, texcoord0, texcoord1.
// all those types of data should have the exact number
// of elements in them, if they have any element at all.
class VertexArrayCPU
{
	friend class Mesh;

public:

	VertexArrayCPU()
		:m_positionBytes(0), 
		m_normalBytes(0), 
		m_tangentBytes(0), 
		m_colorBytes(0),
		m_texcoord0Bytes(0), 
		m_texcoord1Bytes(0), 
		m_boneWeightBytes(0),
		m_vertBytes(0)
	{
	}

	~VertexArrayCPU()
	{}

	// Methods that do not modify a class should be declared as const
	//NNEbool hasPosition() const
	//{
	//	return m_position.size() == 0? false : true;
	//}

	//NNEbool hasNormal() const
	//{
	//	return m_normal.size() == 0? false : true;
	//}

	//NNEbool hasTangent() const
	//{
	//	return m_tangent.size() == 0? false : true; 
	//}

	//NNEbool hasColor() const
	//{
	//	return m_color.size() == 0? false : true; 
	//}

	//NNEbool hasTexcoord0() const
	//{
	//	return m_texcoord0.size() == 0? false : true;
	//}

	//NNEbool hasTexcoord1() const
	//{
	//	return m_texcoord1.size() == 0? false : true;
	//}

	//NNEbool hasBoneWeight() const
	//{
	//	return m_boneWight.size() == 0? false : true;
	//}

	//NNEbool hasIndex() const
	//{
	//	return m_triIndex.size() == 0? false : true;
	//}

	//NNEuint vertexCount() const
	//{
	//	return m_position.size();
	//}

	//size_t getPositionBytes() const
	//{
	//	return m_position.getBytes();
	//}

	//size_t getNormalBytes() const
	//{
	//	return m_normal.getBytes();
	//}

	//size_t getTangentBytes() const
	//{
	//	return m_tangent.getBytes();
	//}

	//size_t getColorBytes() const
	//{
	//	return m_color.getBytes();
	//}

	//size_t getTexcoord0Bytes() const
	//{
	//	return m_texcoord0.getBytes();
	//}

	//size_t getTexcoord1Bytes() const
	//{
	//	return m_texcoord1.getBytes();
	//}

	//size_t getBoneWeightBytes() const
	//{
	//	return m_boneWight.getBytes();
	//}

	//// return the total byte size of all vertex data in this object
	//size_t getTotalBytes() const
	//{
	//	return getPositionBytes() + getNormalBytes() + getTangentBytes()
	//			+ getColorBytes() + getTexcoord0Bytes() + getTexcoord1Bytes()
	//			+ getBoneWeightBytes();
	//}

	//size_t getCapacityBytes() const
	//{
	//	return m_position.getCapacityBytes() + m_normal.getCapacityBytes()
	//		+ m_tangent.getCapacityBytes() + m_color.getCapacityBytes()
	//		+ m_texcoord0.getCapacityBytes() + m_texcoord1.getCapacityBytes()
	//		+ m_boneWight.getCapacityBytes();
	//}

	

	// specify data type size for each vertex attribute
	NNEvoid setVertFormat(size_t pos, size_t normal, size_t tangent, size_t color,
		size_t texcoord0, size_t texcoord1, size_t boneweight)
	{
		m_positionBytes = pos;
		m_normalBytes = normal;
		m_tangentBytes = tangent;
		m_colorBytes = color;
		m_texcoord0Bytes = texcoord0;
		m_texcoord1Bytes = texcoord1;
		m_boneWeightBytes = boneweight;

		m_vertBytes = m_positionBytes + m_normalBytes + m_tangentBytes + m_colorBytes
			+ m_texcoord0Bytes + m_texcoord1Bytes + m_boneWeightBytes;
	}

	NNEbool vertFormatTest(size_t pos, size_t normal, size_t tangent, size_t color,
		size_t texcoord0, size_t texcoord1, size_t boneweight)
	{
		return pos == m_positionBytes && normal == m_normalBytes
			&& tangent == m_tangentBytes && color == m_colorBytes
			&& texcoord0 == m_texcoord0Bytes && texcoord1 == m_texcoord1Bytes
			&& boneweight == m_boneWeightBytes;
	}

	Vec3 *getPosPtr(NNEuint i)
	{
		return (Vec3 *)(&m_vertices[m_vertBytes * i]);
	}

	Vec4 *getColorPtr(NNEuint i)
	{
		return (Vec4 *)(&m_vertices[m_vertBytes * i + m_positionBytes + m_normalBytes
									+ m_tangentBytes]);
	}

	// when adding a new vertex, addPos routine should be called at first
	NNEvoid addPos(const Vec3 &pos)
	{
		// add m_vertBytes of char to the m_vertices
		m_vertices.push_back('0', m_vertBytes);
		// modify position
		Vec3 *tmp = getPosPtr(getVertNum() - 1);
		*tmp = pos;
	}

	NNEvoid addColor(const Vec4 &clr)
	{
		Vec4 *tmp = getColorPtr(getVertNum() - 1);
		*tmp = clr;
	}

	// get the pointer of ith bone weight in m_vertices
	BoneWeight *getBoneWeightPtr(NNEuint i)
	{
		return (BoneWeight *)(&m_vertices[m_vertBytes * i + m_positionBytes + m_normalBytes
				+ m_tangentBytes + m_colorBytes + m_texcoord0Bytes + m_texcoord1Bytes]);
	}

	NNEuint getVertNum()
	{
		if (0 == m_vertBytes)
		{
			return 0;
		}
		else
		{
			return m_vertices.getBytes() / m_vertBytes;
		}
	}

	size_t getVerticesBytes()
	{
		return m_vertices.getBytes();
	}

	// return the size in bytes of the index
	size_t getIndexBytes() const
	{
		return m_triIndex.getBytes();
	}

	NNEbool hasIndex()
	{
		return 0 == m_triIndex.size()? false : true;
	}

	// functions only used for creating gpu buffer

	// merge all vertex data into one array in the order
	// of position, normal, tangent, color, texcoord0, texcoord1.
	// data are stored one type after another, not interleaved.
	//
	// vertex data array generated by this function is usually for
	// temporary use such as parameter for glBufferData.
	//std::shared_ptr<NNEfloat> getArrayPtr();

	NNEchar *getVerticesPtr()
	{
		return m_vertices.data();
	}

	// get C array of index
	NNEuint *getIndexPtr()
	{
		return m_triIndex.data();
	}

	NNEvoid clear()
	{
		m_vertices.clear();
		m_triIndex.clear();
	}

public:

	/*
		Decide to store all vertex data into one array, which
		could be directly passed into glBufferData, saving computation
		otherwise would need to create a temporary array.

		the above is actually not a good idea when it comes to the point
		where the mesh is deformable and procedural generated.

		actually it's better for dynamically generated mesh to store all 
		vertex data(pos, normal, color, etc) into one block.

		getArrayPtr() won't be called frequently anyway.
	*/
	/*
	Array<Vec3> m_position;

	Array<Vec3> m_normal;

	Array<Vec4>	m_tangent;

	Array<Vec4> m_color;

	Array<Vec2> m_texcoord0;

	Array<Vec2> m_texcoord1;

	// bone information for skinned mesh
	Array<BoneWeight> m_boneWeight;
	*/

	// byte size for each vertex attribute
	size_t m_positionBytes;

	size_t m_normalBytes;

	size_t m_tangentBytes;

	size_t m_colorBytes;

	size_t m_texcoord0Bytes;

	size_t m_texcoord1Bytes;

	size_t m_boneWeightBytes;

	// number of bytes occupied by one vertex
	size_t m_vertBytes;

	Array<NNEchar> m_vertices;

	// triangle index
	Array<NNEuint> m_triIndex;

private:
};

} // manespace NNE

#endif // NNE_VERTEXARRAY_H