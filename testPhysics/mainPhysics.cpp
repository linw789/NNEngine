// if I add the inlcude path "NNEngine\source" and include "Game.h",
// the compiler will spit out weird error
#include "source\NNEngine.h"

using namespace NNE;

int WINAPI wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
	NNE::Game game;
	game.init();

	Mesh *cubeMesh = gResourceMngr.createMesh("cube_ccw", "..//Assets/Mesh/cube_ccw.obj");
	Mesh *planeMesh = gResourceMngr.createMesh("plane", "..//Assets/Mesh/plane.obj");
	//Mesh *cowMesh = gResourceMngr.createMesh("cow", "..//Assets/Mesh/cow.obj");

	Material *mtrl0 = gResourceMngr.createMaterial("mtrl0");
	mtrl0->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhongVSM"));

	Material *mtrl1 = gResourceMngr.createMaterial("mtrl1");
	mtrl1->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhongVSM"));
	mtrl1->setDiffuse(Vec3(0.5f, 0.2f, 0.2f));
	mtrl1->setAmbient(Vec3(0.4f, 0.1f, 0.1f));

	Material *planeMtrl = gResourceMngr.createMaterial("planeMtrl");
	planeMtrl->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhongVSM"));
	planeMtrl->setDiffuse((Vec3(1.0f, 1.0f, 1.0f)));
	planeMtrl->setAmbient(Vec3(0.1f, 0.1f, 0.2f));

	Material *mtrl2 = gResourceMngr.createMaterial("mtrl2");
	mtrl2->setGPUProgram(gResourceMngr.findGPUProgram("BlinnPhongVSM"));
	mtrl2->setDiffuse(Vec3(0.65f, 0.25f, 0.65f));
	mtrl2->setAmbient(Vec3(0.5f, 0.3f, 0.5f));

	//Drawable *cow = gSceneMngr.createDrawable("cow");
	//cow->setMaterial(mtrl0);
	//cow->setMesh(cowMesh);
	//cow->getNode()->translate(0, -0.4f, 0);

	Drawable *plane = gSceneMngr.createDrawable("plane");
	plane->setMaterial(planeMtrl);
	plane->setMesh(cubeMesh);
	plane->getNode()->scale(25.0f, 1.0f, 25.0f);
	//plane->getNode()->rotateZ(5.0f);
	plane->getNode()->translate(0, -5.0f, 0);

	Drawable *box0 = gSceneMngr.createDrawable("box0");
	box0->setMaterial(mtrl0);
	box0->setMesh(cubeMesh);
	box0->getNode()->rotateY(60.0f);
	//box0->getNode()->rotateZ(60.0f);
	box0->getNode()->translate(.0f, -3.0f, 0);

	Drawable *box1 = gSceneMngr.createDrawable("box1");
	box1->setMaterial(mtrl1);
	box1->setMesh(cubeMesh);
	box1->getNode()->translate(-0.5f, -1.0f, 0.0f);
	////box1->getNode()->rotateZ(70.0f);
	////box1->getNode()->rotate(123.0f, Vec3(20.0f, 1.0f, 12.0f)); 

	Drawable *box2 = gSceneMngr.createDrawable("box2");
	box2->setMaterial(mtrl2);
	box2->setMesh(cubeMesh);
	//box2->getNode()->rotateY(65.0f);
	//box2->getNode()->rotateX(30.0f);
	box2->getNode()->translate(0.0f, 1.0f, 0);

	Drawable *box3 = gSceneMngr.createDrawable("box3");
	box3->setMaterial(mtrl1);
	box3->setMesh(cubeMesh);
	box3->getNode()->translate(0.2f, 3.0f, 0);

	Drawable *box4 = gSceneMngr.createDrawable("box4");
	box4->setMaterial(mtrl1);
	box4->setMesh(cubeMesh);
	box4->getNode()->translate(-0.2f, 5.0f, 0);

	/*Drawable *box5 = gSceneMngr.createDrawable("box5");
	box5->setMaterial(mtrl1);
	box5->setMesh(cubeMesh);
	box5->getNode()->translate(6.0f, 3.0f, 0);*/

	Drawable *box6 = gSceneMngr.createDrawable("box6");
	box6->setMaterial(mtrl1);
	box6->setMesh(cubeMesh);
	box6->getNode()->translate(-3.0f, -2.8f, 0);

	Drawable *box7 = gSceneMngr.createDrawable("box7");
	box7->setMaterial(mtrl1);
	box7->setMesh(cubeMesh);
	box7->getNode()->translate(-3.0f, -0.8f, 0);

	Drawable *box8 = gSceneMngr.createDrawable("box8");
	box8->setMaterial(mtrl1);
	box8->setMesh(cubeMesh);
	box8->getNode()->translate(-3.0f, 1.2f, 0);

	Drawable *box9 = gSceneMngr.createDrawable("box9");
	box9->setMaterial(mtrl1);
	box9->setMesh(cubeMesh);
	box9->getNode()->translate(-3.0f, 3.2f, 0);

	Drawable *box10 = gSceneMngr.createDrawable("box10");
	box10->setMaterial(mtrl1);
	box10->setMesh(cubeMesh);
	box10->getNode()->translate(-3.0f, 5.2f, 0);

	Camera *cam = gSceneMngr.getActiveCamera();
	cam->getNode()->translate(0, 0, 20.0f);

	gSceneMngr.setShadowTechnique(Scene::VARIANCE_SHADOW_MAPPING);

	Light *defaultLight = gSceneMngr.createLight("default", Light::DIRECTIONAL, Vec4(-1.0f, -1.0f, -1.0f, 0.0f));
	defaultLight->enableShadowCast();

	gPhysicsWorld.setGravity(Vec3(0, -15.0f, 0));

	BoxShape boxShape(Vec3(1.0f, 1.0f, 1.0f));
	InfinitePlaneShape planeShape(Vec3(0, 1.0f, 0), -4.0f);
	BoxShape boxPlaneShape(Vec3(25.0f, 1.0f, 25.0f));

	RigidBody *rb_plane = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 0);
	RigidBody *rb_box0 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box1 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box2 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box3 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box4 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	
	RigidBody *rb_box6 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box7 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box8 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box9 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	RigidBody *rb_box10 = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
	
	//rb_box0->setLinearVelocity(Vec3(-5.0f, 0.0f, 0));
	//rb_box1->setLinearVelocity(Vec3(-6.0f, 0, 0));
	//rb_box2->setLinearVelocity(Vec3(-13.0f, 0.0f, 0));
	//rb_box4->setLinearVelocity(Vec3(-16.0f, 0, 0));
	

	plane->setRigidBody(rb_plane);
	box0->setRigidBody(rb_box0);
	box1->setRigidBody(rb_box1);
	box2->setRigidBody(rb_box2);
	box3->setRigidBody(rb_box3);
	box4->setRigidBody(rb_box4);
	
	box6->setRigidBody(rb_box6);
	box7->setRigidBody(rb_box7);
	box8->setRigidBody(rb_box8);
	box9->setRigidBody(rb_box9);
	box10->setRigidBody(rb_box10);

	game.setup();
	while (true)
	{
		if (gInputMngr.ifKeyUpStroke('G'))
		{
			Drawable *newbox = gSceneMngr.createDrawable("box5");
			newbox->setMaterial(mtrl1);
			newbox->setMesh(cubeMesh);
			newbox->getNode()->setPos(gSceneMngr.getActiveCamera()->getNode()->getPos());

			RigidBody *new_rb_box = gPhysicsWorld.createRigidBody(dynamic_cast<CollideShape *>(&boxShape), 1.0f);
			newbox->setRigidBody(new_rb_box);
			new_rb_box->setLinearVelocity(gSceneMngr.getActiveCamera()->getNode()->getAxisZ() *- 16.0f);
		}
		game.oneFrame();
	}

	return 0;
}