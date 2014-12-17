#include "VertexArrayCPU.h"
#include "BufferObjectGPU.h"
#include "DebugUtilities.h"

namespace NNE{

NNEuint BoneWeight::getBoneIndex(NNEuint i)
{
	NNEuint bi = 0;
	switch(i)
	{
	case 0:
		// set chars to 0, except the first char
		bi = b0123 & 0xff000000;
		// shift the highest to the lowest
		bi = bi >> 24;
		break;
			
	case 1:
		bi = b0123 & 0x00ff0000;
		bi = bi >> 16;
		break;
			
	case 2:
		bi = b0123 & 0x0000ff00;
		bi = bi >> 8;
		break;
			
	case 3:
		bi = b0123 & 0x000000ff;
		break;
			
	default:
		bi = b0123 & 0x000000ff;
		break;
	}

	return bi;
}

NNEvoid BoneWeight::setBoneIndex(NNEuint i, NNEuint indx)
{
	NNEuint bi = indx;
	switch(i)
	{
	case 0:
		// set the first char to 0
		b0123 &= 0x00ffffff;
		// shift the lowest to the highest
		bi = bi << 24;
		// copy bits
		b0123 |= bi;
		break;
			
	case 1:
		b0123 &= 0xff00ffff;
		bi = bi << 16;
		b0123 |= bi;
 		break;
			
	case 2:
		b0123 &= 0xffff00ff;
		bi = bi << 8;
		b0123 |= bi;
		break;
			
	case 3:
		b0123 &= 0xffffff00;
		b0123 |= bi;
		break;
			
	default:
		b0123 &= 0xffffff00;
		b0123 |= bi;
		break;
	}
}

//std::shared_ptr<NNEfloat> VertexArrayCPU::getArrayPtr()
//{
//	NNEuint positionBytes = getPositionBytes();
//	NNEuint normalBytes = getNormalBytes();
//	NNEuint tangentBytes = getTangentBytes();
//	NNEuint colorBytes = getColorBytes();
//	NNEuint texCoord0Bytes = getTexcoord0Bytes();
//	NNEuint texCoord1Bytes = getTexcoord1Bytes();
//	NNEuint boneWeightBytes = getBoneWeightBytes();
//	
//	//array of vertices
//	std::shared_ptr<NNEfloat> vertexData ((NNEfloat *)::malloc(positionBytes + normalBytes + tangentBytes
//								+ colorBytes + texCoord0Bytes + texCoord1Bytes
//								+ boneWeightBytes));
//
//	// Vec2, Vec3, Vec4 are not POD, but use memcpy anyway
//	NNEuchar *destPtr = (NNEuchar *)vertexData.get();
//	if (positionBytes > 0)
//	{
//		memcpy(destPtr, m_position.data(), positionBytes);
//		destPtr += positionBytes;
//	}
//
//	if (normalBytes > 0)
//	{
//		memcpy(destPtr, m_normal.data(), normalBytes);
//		destPtr += normalBytes;
//	}
//
//	if (tangentBytes > 0)
//	{
//		memcpy(destPtr, m_tangent.data(), tangentBytes);
//		destPtr += tangentBytes;
//	}
//
//	if (colorBytes > 0)
//	{
//		memcpy(destPtr, m_color.data(), colorBytes);
//		destPtr += colorBytes;
//	}
//
//	if (texCoord0Bytes > 0)
//	{
//		memcpy(destPtr, m_texcoord0.data(), texCoord0Bytes);
//		destPtr += texCoord0Bytes;
//	}
//
//	if (texCoord1Bytes > 0)
//	{
//		memcpy(destPtr, m_texcoord1.data(), texCoord1Bytes);
//		destPtr += texCoord1Bytes;
//	}
//
//	if (boneWeightBytes > 0)
//	{
//		memcpy(destPtr, m_boneWight.data(), boneWeightBytes);
//		destPtr += boneWeightBytes;
//	}
//
//	destPtr = nullptr;
//
//	return vertexData;
//}

} // namespace NNE