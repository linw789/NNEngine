#include "PlatformSpecs.h"
#include "Game.h"
#include "DebugUtilities.h"
#include "RenderDevice.h"
#include "Resource.h"
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "GPUProgram.h"
#include "Texture.h"
#include "Material.h"
#include "Drawable.h"
#include "Event.h"
#include "EventManager.h"
#include "Input.h"
#include "DebugRenderer.h"
#include "Timer.h"
#include "Font.h"
#include "PhysicsWorld.h"
#include "Timer.h"
#include "Array.h"

namespace NNE{

bool Game::init()
{
	allocDebugConsole();

	gTimer.init();

	EVENTS::gEventMngr.init();

	m_window;

	Window::Descriptor winDes;
	//winDes.setSize(640, 480);
	winDes.setUpLeftPos(80, 40);
	// set resize callback before window init
	m_window.setResizeCallback(&Game::resizeCallback);
	m_window.init(winDes);

	gRenderDevice.init(&m_window);
	gRenderDevice.setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	gRenderDevice.setClearDepth(1.0f);
	gRenderDevice.setClearStencil(0);
	NNE_EVENTS_REGISTER_MEMBER_FUNC(Key_R_PressedActionEvent, RenderDevice, Key_R_PressedActionHandler, &gRenderDevice);

	gResourceMngr.init();
	gSceneMngr.init();

	gDebugRenderer.init();

	gPhysicsWorld.init();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	GPUProgram *BlinnPhongGPUProg = gResourceMngr.createGPUProgram("BlinnPhong", "..//Assets/Shader/BlinnPhong.vert", "..//Assets/Shader/BlinnPhong.frag");

	GPUProgram *gpuskinning = gResourceMngr.createGPUProgram("skinning", "..//Assets/Shader/skin.vert", "..//Assets/Shader/skin.frag");

	GPUProgram *defaultGPUP = gResourceMngr.createGPUProgram("default", "..//Assets/Shader/default.vert", "..//Assets/Shader/default.frag");

	Material *defaultMtrl = gResourceMngr.createMaterial("default");
	defaultMtrl->setGPUProgram(defaultGPUP);

	Camera *cam = gSceneMngr.createCamera("Default");
	NNE_EVENTS_REGISTER_MEMBER_FUNC(MouseMoveLBDownEvent, Camera, mouseMoveLBDownHandler, cam);
	NNE_EVENTS_REGISTER_MEMBER_FUNC(MouseMoveRBDownEvent, Camera, mouseMoveRBDownHandler, cam);
	NNE_EVENTS_REGISTER_MEMBER_FUNC(MouseWheelMoveEvent, Camera, mouseWheelMoveHandler, cam);
	gSceneMngr.setActiveCamera(cam);

	gTimer.start();

	return true;
}

NNEvoid Game::resizeCallback(NNEuint w, NNEuint h)
{
	0 == h && (h = 1);

	// when call showWindow system will pass a message "WM_SIZE",
	// that's where this function gets called.
	gRenderDevice.setViewport(0, 0, w, h);

	EVENTS::WindowResizeEvent *winrEvent = new EVENTS::WindowResizeEvent(w, h);
	EVENTS::gEventMngr.add2GeneralQ(winrEvent);
	// don't add to the event loop, directly call handlers.
	// because when the window is first created 
	//winrEvent->signal();
}

NNEvoid Game::updateFPS()
{
	NNEdouble newtime = gTimer.newTime();
	m_frameTime = newtime - m_currentTime;
	m_currentTime = newtime;
	m_FPS = 1.0 / m_frameTime;
}

NNEvoid Game::setup()
{
	// setup lights 
	Array<Light *> &lights = gSceneMngr.m_lightArray;
	Array<GPUProgram *> &gpups = gResourceMngr.m_GPUProgArray;

	NNEuint lightNum = lights.size();

	for (NNEuint i = 0; i < lightNum; ++i)
	{
		for (NNEuint j = 0; j < gpups.size(); ++j)
		{
			gpups[j]->applyLight("LightBlock0", lights[i]);
		}
		lights[i]->updateLightBufferObject();
	}

	// set up scene
	gSceneMngr.setup();
}

NNEvoid Game::run()
{
	bool done = false;

	setup();

	while(!done)
	{
		gInputMngr.update(&m_window);

		m_window.getOSEvent();

		EVENTS::gEventMngr.handleEvents();

		updateFPS();

		gPhysicsWorld.simulateOneStep(getFrameTime());

		gSceneMngr.update();

		gSceneMngr.renderScene();

		gDebugRenderer.render();

		gRenderDevice.swapBuffers();
	}
}

NNEvoid Game::oneFrame()
{
	gInputMngr.update(&m_window);

	m_window.getOSEvent();

	EVENTS::gEventMngr.handleEvents();

	updateFPS();

	gPhysicsWorld.simulateOneStep(getFrameTime());

	gSceneMngr.update();

	gSceneMngr.renderScene();

	gDebugRenderer.render();

	gRenderDevice.swapBuffers();
}

} // namespace NNE