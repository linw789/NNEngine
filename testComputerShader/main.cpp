// if I add the inlcude path "NNEngine\source" and include "Game.h",
// the compiler will spit out weird error
#include "source\NNEngine.h"

using namespace NNE;

int WINAPI wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
	NNE::Game game;
	game.init();
	
	Mesh *cowMesh = gResourceMngr.createMesh("cow", "..//Assets/Mesh/cow.obj");
	ClothMesh *clothm = gResourceMngr.createClothMesh("cloth", 10, 10, 1.0f);

	GPUProgram *clothSim_cs = gResourceMngr.createGPUProgram("clothsim", "Assets/Shaders/cloth_simulate.comp");

	clothm->setComputeShader(clothSim_cs->getProgramId());

	Material *mtrl0 = gResourceMngr.createMaterial("mtrl0");
	mtrl0->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhong"));

	Drawable *cloth = gSceneMngr.createDrawable("cloth");
	cloth->setMaterial(mtrl0);
	cloth->setMesh(clothm);
	cloth->getNode()->rotateX(90.0f);

	Camera *cam = gSceneMngr.getActiveCamera();
	cam->getNode()->translate(0, 0, 20.0f);

	game.setup();
	while (true)
	{
		game.oneFrame();
	}

	return 0;
}