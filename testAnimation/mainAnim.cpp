// if I add the inlcude path "NNEngine\source" and include "Game.h",
// the compiler will spit out weird error
#include "source\NNEngine.h"

using namespace NNE;

int WINAPI wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
	NNE::Game game;
	game.init();

	Skin *guardSkin = gResourceMngr.createSkin("guard", "..//Assets/Skin/boblampclean.md5mesh");

	//Mesh *guardMesh = gResourceMngr.createMesh("guard", "..//Assets/Skin/boblampclean.md5mesh");

	Mesh *cubeMesh = gResourceMngr.createMesh("cube_ccw", "..//Assets/Mesh/cube_ccw.obj");

	Material *mtrl0 = gResourceMngr.createMaterial("mtrl0");
	//mtrl0->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhong"));
	mtrl0->setGPUProgram(gResourceMngr.findGPUProgram("skinning"));

	Material *planeMtrl = gResourceMngr.createMaterial("planeMtrl");
	planeMtrl->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhongVSM"));
	planeMtrl->setDiffuse((Vec3(1.0f, 1.0f, 1.0f)));
	planeMtrl->setAmbient(Vec3(0.1f, 0.1f, 0.2f));

	//Drawable *plane = gSceneMngr.createDrawable("plane");
	//plane->setMaterial(planeMtrl);
	//plane->setMesh(cubeMesh);
	//plane->getNode()->scale(25.0f, 1.0f, 25.0f);
	////plane->getNode()->rotateZ(5.0f);
	//plane->getNode()->translate(0, -5.0f, 0);

	Drawable *guard = gSceneMngr.createDrawable("guard");
	guard->setMaterial(mtrl0);

	guard->getNode()->scale(0.2f, 0.2f, 0.2f);
	guard->getNode()->translate(0.0f, -4.0f, 0.0f);
	guard->getNode()->rotateX(-90.0f);

	//guard->setMesh(guardMesh);

	guard->setSkin(guardSkin);

	Animation *boblampclean = gResourceMngr.createAnimation("boblampclean", "..//Assets/Animation/boblampclean.md5anim");

	guard->getSkin()->getSkeleton()->addAnimation(boblampclean);

	guard->getSkin()->beginAnimation("boblampclean", -1);

	Camera *cam = gSceneMngr.getActiveCamera();
	cam->getNode()->translate(0, 0, 20.0f);

	//gSceneMngr.setShadowTechnique(Scene::VARIANCE_SHADOW_MAPPING);
	Light *defaultLight = gSceneMngr.createLight("default", Light::DIRECTIONAL, Vec4(-1.0f, -1.0f, -1.0f, 0.0f));
	//defaultLight->enableShadowCast();

	game.run();

	return 0;
}