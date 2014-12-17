#ifndef NNE_SCENE_H
#define NNE_SCENE_H

#include "Array.h"
#include "Node.h"
#include "Entity.h"
#include "BufferObjectGPU.h"
#include "Matrix4.hpp"
#include "Skybox.h"
#include "Image.h"

namespace NNE{

class Mesh;
class Drawable;
class Camera;
class Light;
class Texture;

/*
	Manage scene node hierarchy and other entities in the scene.
	When the scene creates an entity(camera, drawable or light),
	it will automatically creates a node and associates the entity with the node.
*/
class Scene
{
	friend class Game;

public:

	enum
	{
		NO_SHADOW = 0,
		// shadow techniques
		// each light add a separat render pass
		BASIC_SHADOW_MAPPING = 1,
		VARIANCE_SHADOW_MAPPING = 2,

		// remember to change correspoding size in shader code
		SHADOW_MAP_SIZE_X = 512,
		SHADOW_MAP_SIZE_Y = 512,
	};
	
	Scene()
		:m_activeCamera(NULL), m_skybox(NULL),
		m_globalAmbientLight(0, 0, 0), 
		m_shadowTechnique(0),
		m_shadowFramebufferId(0)
	{}

	~Scene()
	{
		GL_DEBUG(glDeleteFramebuffers(1, &m_shadowFramebufferId));
		GL_DEBUG(glDeleteRenderbuffers(1, &m_depthTestBufferId));
	}

	NNEbool init();

	NNEvoid initSkybox(Texture *t);

	NNEvoid update();

	NNEvoid add2Scene(Drawable *);

	Node *createNode();

	// drawables could have same name
	// create an empty drawable
	Drawable *createDrawable(const NNEchar *name);

	// create a drawable based on a mesh
	Drawable *createDrawable(const NNEchar *name, Mesh *mesh);

	Camera *createCamera(const NNEchar *name);

	Light *createLight(const NNEchar *name, NNEuint lt, Vec4 v4);

	Drawable *findDrawable(const NNEchar *name);

	Camera *getActiveCamera()
	{
		return m_activeCamera;
	}

	NNEvoid setActiveCamera(Camera *cam)
	{
		m_activeCamera = cam;
	}

	NNEvoid setShadowTechnique(NNEuint st)
	{
		m_shadowTechnique = st;
	}

	NNEuint getShadowTechnique()
	{
		return m_shadowTechnique;
	}

	NNEvoid genShadowMapPass(Light *);

	// sort drawables by material and mesh,
	// called before rendering
	NNEvoid sortDrawable();

	NNEvoid setup();

	NNEvoid renderColorPass();

	NNEvoid renderScene();

private:

	// uncopyable
	Scene(const Scene &);
	Scene &operator = (const Scene &);

	// create a new node and bilaterally link it with the entity
	NNEvoid linkWithNewNode(Entity *entity);

	NNEvoid updateSceneUniform();

	NNEvoid frustumCull();

	//============================
	Array<Node *> m_nodeArray;

	Array<Drawable *> m_DrawableArray;

	Array<Camera *> m_cameraArray;

	Array<Light *> m_lightArray;

	Camera *m_activeCamera;

	Drawable *m_skybox;

	//=============================
	Vec3 m_globalAmbientLight;

	//=============================
	NNEuint m_shadowTechnique;

	// every light which is enabled for shadow casting has a depth texture,
	// and all depth textures are binded to this framebuffer.
	GPUuint m_shadowFramebufferId;

	GPUenum m_framebufferAttachment;

	// when you render color to frame buffer, you need a depth buffer to do depth test
	GPUuint m_depthTestBufferId;

	// used for gaussian blur
	GPUuint m_auxiliaryTextureId;

	GPUProgram *m_gpup_genShadowMap;

	GPUProgram *m_gpup_skinGenShadowMap;

	GPUProgram *m_gpup_gaussianBlurX;

	GPUProgram *m_gpup_gaussianBlurY;

	// in order to trigger fragment shader on every pixel on the screen
	Mesh *m_screen_Quad;

	//==================================
	// constant data(immutable throughout shader stages) per scene
	// accessible by all shader programs
	// it stores camera matrices
	BufferObjectGPU m_BO_sceneUniform;
};

inline NNEvoid Scene::linkWithNewNode(Entity *entity)
{
	Node *tmpNode = createNode();
	tmpNode->linkEntity(entity);
}

// Declare global objects so they can be accessed through out the engine.
// Declare extern in one header file and define them in one source file,
// then you can directly use them in other source files.
extern Scene gSceneMngr;

} // manespace NNE

#endif // NNE_SCENE_H