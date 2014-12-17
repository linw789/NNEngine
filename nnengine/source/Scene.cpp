#include "RenderDevice.h"
#include "Scene.h"
#include "Node.h"
#include "Drawable.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "Resource.h"
#include "GPUProgram.h"
#include "Matrix3.hpp"
#include "String.h"
#include "Material.h"
#include "DebugRenderer.h"

namespace NNE{

NNEbool Scene::init()
{
	m_BO_sceneUniform.create(GPU_UNIFORM_BUFFER, sizeof(Mat4) * 3 + sizeof(Vec4), NULL, GPU_WRITE_EVERY_FRAME);
	GL_DEBUG(m_BO_sceneUniform.bindRange(SCENE_MATRICES, 0, sizeof(Mat4) * 3 + sizeof(Vec4)));

	return true;
}

NNEvoid Scene::initSkybox(Texture *t)
{
	// load skybox mesh
	Mesh *amesh = gResourceMngr.createMesh("skybox_mesh", "..//Assets/Mesh/skybox.obj");

	// load skybox shader
	GPUProgram *gpupro = gResourceMngr.createGPUProgram("skybox", "..//Assets/Shader/skybox.vert", "..//Assets/Shader/skybox.frag");

	// create a material and set GPUProgram and texture
	Material *skyboxMtrl = gResourceMngr.createMaterial("skybox_mtrl");
	skyboxMtrl->setGPUProgram(gpupro);
	skyboxMtrl->setTexture(t);

	m_skybox = new Drawable(HashString("skybox"));

	linkWithNewNode(dynamic_cast<Entity *>(m_skybox));

	m_skybox->setMesh(amesh);
	m_skybox->setMaterial(skyboxMtrl);
}

NNEvoid Scene::update()
{
	updateSceneUniform();
}

NNEvoid Scene::add2Scene(Drawable *d)
{
	linkWithNewNode(d);

	m_DrawableArray.push_back(d);
}

Node *Scene::createNode()
{
	Node *anode = new Node();
	m_nodeArray.push_back(anode);

	return anode;
}

Drawable *Scene::createDrawable(const NNEchar *name)
{
	Sid sid = HashString(name);

	Drawable *aDrawable = new Drawable(sid);

	aDrawable->setMaterial(gResourceMngr.findMaterial("default"));

	m_DrawableArray.push_back(aDrawable);

	// dynamic_cast safely converts pointers and references to classes up, down, 
	// and sideways along the inheritance hierarchy.
	linkWithNewNode(dynamic_cast<Entity *>(aDrawable));

	return aDrawable;
}

Drawable *Scene::createDrawable(const NNEchar *name, Mesh *mesh)
{
	Drawable *aDrawable = createDrawable(name);

	aDrawable->setMesh(mesh);

	return aDrawable;
}

Camera *Scene::createCamera(const NNEchar *name)
{
	Sid sid = HashString(name);
	for (Array<Camera *>::iterator it = m_cameraArray.begin(); it != m_cameraArray.end(); ++it)
	{
		alwaysAssertMsg((*it)->getNameSid() != sid, "name already exists");
	}

	Camera *aCam = new Camera(sid);

	m_cameraArray.push_back(aCam);
	linkWithNewNode(dynamic_cast<Entity *>(aCam));

	aCam->init();

	return aCam;
}

Light *Scene::createLight(const NNEchar *name, NNEuint lt, Vec4 v4)
{
	Sid sid = HashString(name);
	for (Array<Light *>::iterator it = m_lightArray.begin(); it != m_lightArray.end(); ++it)
	{
		alwaysAssertMsg((*it)->getNameSid() != sid, "name already exists");
	}

	Light *alight = new Light(sid, lt, v4, gResourceMngr.getNewUniformBindingPoint());
	m_lightArray.push_back(alight);

	linkWithNewNode(dynamic_cast<Entity *>(alight));

	return alight;
}

Drawable *Scene::findDrawable(const NNEchar *name)
{
	Sid sid = HashString(name);
	for (Array<Drawable *>::iterator it = m_DrawableArray.begin(); it != m_DrawableArray.end(); ++it)
	{
		if ((*it)->getNameSid() == sid)
		{
			return (*it);
		}
	}

	return NULL;
}

NNEvoid Scene::updateSceneUniform()
{
	// Matrices must be placed close to each other in the Camera class so that
	// all matrices can be addressed by the first matrix's pointer
	m_BO_sceneUniform.bind();

	m_BO_sceneUniform.updateSubData(0, sizeof(Mat4) * 3, (GPUvoid *)m_activeCamera->getMatrixPtr());

	m_BO_sceneUniform.updateSubData(sizeof(Mat4) * 3, sizeof(Vec4), (GPUvoid *)m_activeCamera->getPosPtr());

	m_BO_sceneUniform.unbind();
}

NNEvoid Scene::genShadowMapPass(Light *alight)
{
	GL_DEBUG(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, m_framebufferAttachment, alight->getShadowTextureId(), 0));

	debugAssert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	gRenderDevice.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mat4 lightPVW;

	m_gpup_genShadowMap->apply();

	for (Array<Drawable *>::iterator it = m_DrawableArray.begin(); it != m_DrawableArray.end(); ++it)
	{
		if ((*it)->isCulled())
		{
		}
		else
		{
			//(*it)->update();

			lightPVW = alight->getPersViewMatrix() * (*it)->getGlobalTransform();
			m_gpup_genShadowMap->updateUniformMatrix4(GPUProgram::UNIFORM_LOC_PVW, 1, (NNEfloat *)&lightPVW);

			(*it)->getMesh()->draw();
		}
	}

	if (VARIANCE_SHADOW_MAPPING == m_shadowTechnique)
	{
		// gaussian blur x
		GL_DEBUG(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_auxiliaryTextureId, 0));
		gRenderDevice.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_gpup_gaussianBlurX->apply();
		GL_DEBUG(glActiveTexture(GL_TEXTURE0));
		GL_DEBUG(glBindTexture(GL_TEXTURE_2D, alight->getShadowTextureId()));
		NNEint shadowTextureUnit = 0;
		m_gpup_gaussianBlurX->updateUniformInt("shadowMap", 1, &shadowTextureUnit);

		m_screen_Quad->draw();

		// gaussian blur y
		GL_DEBUG(glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, alight->getShadowTextureId(), 0));
		gRenderDevice.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_gpup_gaussianBlurY->apply();
		GL_DEBUG(glActiveTexture(GL_TEXTURE0));
		GL_DEBUG(glBindTexture(GL_TEXTURE_2D, m_auxiliaryTextureId));
		m_gpup_gaussianBlurY->updateUniformInt("shadowMap", 1, &shadowTextureUnit);

		m_screen_Quad->draw();

		// generate mipmaps
	/*	GL_DEBUG(glActiveTexture(GL_TEXTURE0));
		GL_DEBUG(glBindTexture(GL_TEXTURE_2D, alight->getShadowTextureId()));
		GL_DEBUG(glGenerateMipmap(GL_TEXTURE_2D));*/
	}

	//Image testimage;
	//testimage.init(SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_Y, Image::RGB, Image::CHAR);
	//GL_DEBUG(glBindTexture(GL_TEXTURE_2D, alight->getShadowTextureId()));
	//GL_DEBUG(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, testimage.getDataBuffer()));
	//testimage.write2PPM("shadow.ppm", Image::INVERT_Y);
}

NNEvoid Scene::setup()
{
	switch(m_shadowTechnique)
	{
	case BASIC_SHADOW_MAPPING:
		GL_DEBUG(glGenFramebuffers(1, &m_shadowFramebufferId));
		GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebufferId));
		GL_DEBUG(glDrawBuffer(GL_NONE));
		GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		m_framebufferAttachment = GL_DEPTH_ATTACHMENT;

		m_gpup_genShadowMap = gResourceMngr.findGPUProgram("shadowMappingFirstPass");

		break;

	case VARIANCE_SHADOW_MAPPING:
		GL_DEBUG(glGenFramebuffers(1, &m_shadowFramebufferId));
		GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebufferId));
		m_framebufferAttachment = GL_COLOR_ATTACHMENT0;
		GL_DEBUG(glDrawBuffers(1, &m_framebufferAttachment));

		// initialize depth test buffer
		GL_DEBUG(glGenRenderbuffers(1, &m_depthTestBufferId));
		GL_DEBUG(glBindRenderbuffer(GL_RENDERBUFFER, m_depthTestBufferId));
		GL_DEBUG(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_Y));

		GL_DEBUG(glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
								 GL_DEPTH_ATTACHMENT, // 2. attachment point
								 GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
								 m_depthTestBufferId)); 

		GL_DEBUG(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		 
		GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// auxiliary texture buffer for separable gaussian blur
		GL_DEBUG(glGenTextures(1, &m_auxiliaryTextureId));
		GL_DEBUG(glBindTexture(GL_TEXTURE_2D, m_auxiliaryTextureId));
		GL_DEBUG(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_Y, 0, GL_RG, GL_FLOAT, 0));
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GL_DEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GL_DEBUG(glBindTexture(GL_TEXTURE_2D, 0));

		m_gpup_genShadowMap = gResourceMngr.findGPUProgram("VSM_genShadowMap");
		m_gpup_skinGenShadowMap = gResourceMngr.findGPUProgram("skin_shadowmapfirstpass");
		m_gpup_gaussianBlurX = gResourceMngr.findGPUProgram("vsm_GaussianBlurX");
		m_gpup_gaussianBlurY = gResourceMngr.findGPUProgram("vsm_GaussianBlurY");

		m_screen_Quad = gResourceMngr.createMesh("screen_Quad");
		m_screen_Quad->setPrimitiveType(GL_TRIANGLE_STRIP);
		m_screen_Quad->setVertFormat(3 * sizeof(NNEfloat), 0, 0, 0, 2 * sizeof(NNEfloat), 0, 0);
		// set triangles to clock wise winding so it won't be culled because of front face culling
		NNEfloat tmpPos[] = {
				-1, -1, 0, 0, 0, // vertex0
				1, -1, 0, 1, 0, // vertex1
				-1, 1, 0, 0, 1, // vertex2
				1, 1, 0, 1, 1 // vertex3
			};
		m_screen_Quad->setVertex(sizeof(tmpPos), tmpPos);
		m_screen_Quad->upload2GPU(GPU_WRITE_ONCE);

		break;
	}
}

NNEvoid Scene::renderColorPass()
{
	gRenderDevice.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// render skybox first
	if (NULL != m_skybox)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		gRenderDevice.depthMask(true);

		m_skybox->getNode()->setColumn4(m_activeCamera->getNode()->getColumn4());

		// perspective * view * world matrix
		Mat4 tmpPVW = m_activeCamera->getPersViewMat() * m_skybox->getGlobalTransform();
		m_skybox->getGPUProg()->updateUniformMatrix4(GPUProgram::UNIFORM_LOC_PVW, 1, (NNEfloat *)&tmpPVW);

		m_skybox->draw();

		gRenderDevice.depthMask(false);
	}

	// draw scene
	for (Array<Drawable *>::iterator it = m_DrawableArray.begin(); it != m_DrawableArray.end(); ++it)
	{
		if ((*it)->isCulled())
		{
		}
		else
		{
			//(*it)->update();

			// world matrix
			Mat4 tmpW = (*it)->getGlobalTransform();
			(*it)->getGPUProg()->updateUniformMatrix4(GPUProgram::UNIFORM_LOC_MATRIX_WORLD, 1, (NNEfloat *)&tmpW);

			// perspective * view * world matrix
			Mat4 tmpPVW = m_activeCamera->getPersViewMat() * tmpW;
			(*it)->getGPUProg()->updateUniformMatrix4(GPUProgram::UNIFORM_LOC_PVW, 1, (NNEfloat *)&tmpPVW);

			// normal in camera space equals transpose(inverse(rotateMat3(viewWorldMat))) * normal, 
			// however, if the scale factor is uniform the inverse operation can be skipped.
			Mat3 tmpNormalMat = Mat3::transpose(Mat3::inverse(tmpW.getRotateMat3()));
			(*it)->getGPUProg()->updateUniformMatrix3(GPUProgram::UNIFORM_LOC_MATRIX_NORMAL_W, 1, (NNEfloat *)&tmpNormalMat);

			/*Mat4 bias(0.5f, 0, 0, 0.5f,
						0, 0.5f, 0, 0.5f,
						0, 0, 0.5f, 0.5f,
						0, 0, 0, 1.0f);*/

			// Since orthographical projection keeps w value as 1 and no division by w required,
			// we can multiply bias matrix first. If prespective projection is used, we need to 
			// apply bias in Normal Device Coordinate(after division by w).
			if (m_lightArray.size() > 0)
			{
				Mat4 lightPVW = /*bias **/ m_lightArray[0]->getPersViewMatrix() * tmpW;
				(*it)->getGPUProg()->updateUniformMatrix4("LIGHT0_PVW", 1, (NNEfloat *)&lightPVW);
			}

			if (NO_SHADOW != m_shadowTechnique)
			{
				GL_DEBUG(glActiveTexture(GL_TEXTURE0));
				GL_DEBUG(glBindTexture(GL_TEXTURE_2D, m_lightArray[0]->getShadowTextureId()));
				//GL_DEBUG(glBindTexture(GL_TEXTURE_2D, m_auxiliaryTextureId));
				NNEint shadowTextureUnit = 0;
				(*it)->getGPUProg()->updateUniformInt("shadowMap", 1, &shadowTextureUnit);
			}

			//(*it)->playAnimation();
			(*it)->draw();
			//(*it)->drawSkeleton();
		}
	}
	
#if defined(NNE_DEBUG)
	// draw mesh normal
	gResourceMngr.getDrawNormalGPUProgram()->apply();
	//gResourceMngr.findGPUProgram(Resource::DRAW_NORMAL_GPUP)->apply();
	for (Array<Drawable *>::iterator it = m_DrawableArray.begin(); it != m_DrawableArray.end(); ++it)
	{
		if ((*it)->isCulled())
		{}
		else
		{
			if ((*it)->ifDrawNormal())
			{
				// perspective * view * world matrix
				Mat4 tmpPVW = m_activeCamera->getPersViewMat() * (*it)->getGlobalTransform();
				gResourceMngr.getDrawNormalGPUProgram()->updateUniformMatrix4(GPUProgram::UNIFORM_LOC_PVW, 1, (NNEfloat *)&tmpPVW);
				(*it)->getMesh()->drawNormal();
			}
		}
	}
#endif

}

NNEvoid Scene::renderScene()
{
	for (Array<Drawable *>::iterator it = m_DrawableArray.begin(); it != m_DrawableArray.end(); ++it)
	{
		if ((*it)->isCulled())
		{
		}
		else
		{
			(*it)->update();

			(*it)->playAnimation();
		}
	}

	if (NO_SHADOW != m_shadowTechnique)
	{
		Rectui rViewport = gRenderDevice.getViewport();
		gRenderDevice.setViewport(0, 0, SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_Y);
	
		if (VARIANCE_SHADOW_MAPPING != m_shadowTechnique)
		{
			/*
				If drawables are only flat triangles with no volume, frontface culling
				would discard them, hence fail to generate shadow map. Try replace flat plane
				with box
			*/
			gRenderDevice.setCullFace(GL_FRONT);
		}

		GL_DEBUG(glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFramebufferId));

		for (NNEuint i = 0; i < m_lightArray.size(); ++i)
		{
			Light *l = m_lightArray[i];
			if (l->isCastingShadow())
			{
				genShadowMapPass(l);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		gRenderDevice.setViewport(rViewport.x, rViewport.y, rViewport.w, rViewport.h);

		if (VARIANCE_SHADOW_MAPPING != m_shadowTechnique)
		{
			gRenderDevice.setCullFace(GL_BACK);
		}
	}

	renderColorPass();
}

// have to define it once, let the compiler know it has already existed
Scene gSceneMngr;

} // namespace NNE