#include "Light.h"

namespace NNE{

Light::Light(Sid name, NNEuint lt, const Vec4 &v4, NNEuint bindingpoint)
		:Entity(name), m_bindingPoint(bindingpoint), m_lightType(lt), m_dir_pos(v4),
		m_ambient(0.4f, 0.4f, 0.4f), m_intensity(0.6f, 0.6f, 0.6f),
		m_castShadow(false), m_shadowTextureId(0)
{
	m_BO_uniformLightInfo.create(GPU_UNIFORM_BUFFER, sizeof(Vec4) * 3, NULL, GPU_WRITE_EVERY_FRAME);
	m_BO_uniformLightInfo.bindRange(m_bindingPoint, 0, sizeof(Vec4) * 3);

	// try to encompass the entire scene as tightly as possible
	NNEfloat extentx = 30.0f, extenty = 30.0, extentz = 80.0f;
	Mat4 lightOrtho = Mat4::orthoProjection(extentx, -extentx, extenty, -extenty, extentz, 0);
	Mat4 lightView = Mat4::lookAt(-12.0f * getDirection(), Vec3(0, 0, 0), Vec3(0, 1.0f, 0));
	m_persViewMat = lightOrtho * lightView;

	// debug draw
	/*Vec3 worldAxisX(lightView[0][0], lightView[1][0], lightView[2][0]);
	Vec3 worldAxisY(lightView[0][1], lightView[1][1], lightView[2][1]);
	Vec3 worldAxisZ(lightView[0][2], lightView[1][2], lightView[2][2]);
	Vec3 teststart0 = -12.0f * getDirection() + extenty * worldAxisY + extentx * worldAxisX;
	Vec3 testend0 = teststart0 - extentz * worldAxisZ;
	gDebugRenderer.drawLine(teststart0, testend0, Color3(0.8f, 0.8f, 0.1f), -1);
	Vec3 teststart1 = -12.0f * getDirection() + extenty * worldAxisY - extentx * worldAxisX;
	Vec3 testend1 = teststart1 - extentz * worldAxisZ;
	gDebugRenderer.drawLine(teststart1, testend1, Color3(0.8f, 0.8f, 0.1f), -1);
	Vec3 teststart2 = -12.0f * getDirection() - extenty * worldAxisY + extentx * worldAxisX;
	Vec3 testend2 = teststart2 - extentz * worldAxisZ;
	gDebugRenderer.drawLine(teststart2, testend2, Color3(0.8f, 0.8f, 0.1f), -1);
	Vec3 teststart3 = -12.0f * getDirection() - extenty * worldAxisY - extentx * worldAxisX;
	Vec3 testend3 = teststart3 - extentz * worldAxisZ;
	gDebugRenderer.drawLine(teststart3, testend3, Color3(0.8f, 0.8f, 0.1f), -1);

	gDebugRenderer.drawLine(teststart0, teststart1, Color3(0.8f, 0.8f, 0.1f), -1);
	gDebugRenderer.drawLine(teststart1, teststart3, Color3(0.8f, 0.8f, 0.1f), -1);
	gDebugRenderer.drawLine(teststart2, teststart3, Color3(0.8f, 0.8f, 0.1f), -1);
	gDebugRenderer.drawLine(teststart2, teststart0, Color3(0.8f, 0.8f, 0.1f), -1);*/
}

NNEvoid Light::enableShadowCast()
{
	m_castShadow = true;

	// allocate memory and do setup for shadow depth texture
	GL_DEBUG(glGenTextures(1, &m_shadowTextureId));
	GL_DEBUG(glBindTexture(GL_TEXTURE_2D, m_shadowTextureId));

	switch(gSceneMngr.getShadowTechnique())
	{
	case Scene::BASIC_SHADOW_MAPPING:

		GL_DEBUG(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 
			Scene::SHADOW_MAP_SIZE_X, Scene::SHADOW_MAP_SIZE_Y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0));

		// only used for GL_DEPTH_COMPONENT*, indicating the GLSL texture function will do comparasion
		// using the function specified by GL_TEXTURE_COMPARE_FUNC.
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));

		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		break;

	case Scene::VARIANCE_SHADOW_MAPPING:

		// use GL_RG32F instead of GL_RG
		GL_DEBUG(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 
			Scene::SHADOW_MAP_SIZE_X, Scene::SHADOW_MAP_SIZE_Y, 0, GL_RG, GL_FLOAT, 0));

		// allocate memory for mipmaps
		//GL_DEBUG(glGenerateMipmap(GL_TEXTURE_2D));
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST_MIPMAP_LINEAR*/));
		break;
	}

		
		
	// use GL_LINEAR filter together with GLSL sampler2DShadow, GLSL texture function will do depth tests on 
	// 4 nearest texels and return a distanced weighted average value.
	// otherwise, GLSL texture function returns a binary float(either 1.0f or 0.0f) 
	// indicating whether the tested pixel is in shadow
	GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	
	GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GL_DEBUG(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace NNE