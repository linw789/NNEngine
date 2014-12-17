#ifndef NNE_LIGHT_H
#define NNE_LIGHT_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "String.h"
#include "Entity.h"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "BufferObjectGPU.h"
#include "Scene.h"
#include "Camera.h"
#include "Matrix4.hpp"
#include "DebugRenderer.h"

namespace NNE{

class Light : public Entity
{

public:

	enum
	{
		DIRECTIONAL,
		POINT,
		SPOT
	};
	
	Light(Sid name, NNEuint lt, const Vec4 &v4, NNEuint bindingpoint);

	~Light()
	{
		GL_DEBUG(glDeleteTextures(1, &m_shadowTextureId));
	}

	NNEvoid init()
	{
		
	}

	NNEuint getBindingPoint()
	{
		return m_bindingPoint;
	}

	NNEvoid updateLightBufferObject()
	{
		// transform to camera space
		//m_dir_pos = gSceneMngr.getActiveCamera()->getViewMat() * (*m_node) * m_dir_pos;
		m_BO_uniformLightInfo.updateSubData(0, sizeof(Vec4) * 3, (GLvoid *)&m_dir_pos);
	}

	Vec3 getDirection()
	{
		return Vec3(m_dir_pos.x, m_dir_pos.y, m_dir_pos.z);
	}

	NNEbool isCastingShadow()
	{
		return m_castShadow;
	}

	NNEvoid enableShadowCast();

	NNEvoid disableShadowCast()
	{
		m_castShadow = false;

		GL_DEBUG(glDeleteTextures(1, &m_shadowTextureId));
	}

	GPUuint getShadowTextureId()
	{
		return m_shadowTextureId;
	}

	Mat4 getPersViewMatrix()
	{
		return m_persViewMat;
	}

private:

	NNEuint m_bindingPoint;

	NNEuint m_lightType;

	// using Vec4 instead of Vec3 comply with the std140 alignment
	// directoin or position in camera space
	Vec4 m_dir_pos;

	Vec3 m_ambient;
	NNEfloat dummy1;

	Vec3 m_intensity;
	NNEfloat dummy2;

	BufferObjectGPU m_BO_uniformLightInfo;

	GPUuint m_shadowTextureId;

	NNEbool m_castShadow;

	Mat4 m_persViewMat;

	// used for gaussian blur
	static GPUuint m_auxiliaryTextureId;

	static NNEuint m_shadowLightNum;
};

} // manespace NNE

#endif // NNE_LIGHT_H