/*
	file: 
*/

#include <iostream>
#include <vector>
#include <cstdio>

#include "source/DebugUtilities.h"
#include "source/Vec2.hpp"
#include "source/Vec3.hpp"
#include "source/Vec4.hpp"
#include "source/Matrix4.hpp"
#include "source/PoolAllocator.h"
#include "source/Array.h"
#include "source/EventManager.h"
#include "source/Event.h"
#include "source/Quaternion.h"
#include "source/Matrix.h"
#include "source/Vector.h"
#include "source/ProjectedGaussSeidel.h"
#include "source/AABB.h"
#include "source/BoxShape.h"
#include "source/Quaternion.h"
//#include "source/NNEngine.h"

using namespace std;
using namespace NNE;

void testVec3();
void testMatrix4();
void memcopyFromVector();
void localValueRef();
void arrayInUnion();
void variablePtrInStruct();
class staticMem
{
public:
	static int a;
	int b;
};
int staticMem::a;
void staticMemStorage();
PoolAllocator g_palloc;
void poolAllocation();
void arrayTest();
struct Listener
{
	void handler(NNEuint a, NNEuint b)
	{
		cout << "Handling event " << a << " " << b <<  endl;
	}
};
void eventSystem();
void testMat2quat();
void testUnicodeCpp();
void gaussSeidel();
void getAABBtest();

int main(int argc, char *argv[])
{
	//debugAssertMsg(1==2, "program fails!");
	
	//testVec3();

	//testMatrix4();

	//memcopyFromVector();

	//localValueRef();

	//arrayInUnion();

	//variablePtrInStruct();

	//staticMemStorage();

	//poolAllocation();

	//eventSystem();

	testMat2quat();

	//testUnicodeCpp();

	//gaussSeidel();

	//getAABBtest();

	cin.get();
}

void testVec3()
{
	float aa = 4.0f;
	Vec3 bb(1.0f, 2.0f, 3.0f);
	Vec3 cc = aa * bb;

	Vec3 dd(0.4f, 0.8f, 1.4f);

	dd = cc * 5.0f;

	Vec3 ff = cc * dd;

	cout << cc.x << endl << cc.y << endl << cc.z;
}

void testMatrix4()
{
	// can only include header file from other project
	// functions defined in cpp file in other project won't compiled
	// when you only include corresponding header file, like what
	// I did with Matrix4.h before
	// you have to either define functions in header files or
	// compile that project as lib

	Mat4 m0(1.0f, 5.0f, 1.0f, 5.0f, 
		2.0f, 6.0f, 2.0f, 6.0f,
		3.0f, 7.0f, 3.0f, 7.0f,
		4.0f, 8.0f, 4.0f, 8.0f);

	Mat4 m1(1.0f, 6.0f, 1.0f, 6.0f,
		3.0f, 7.0f, 3.0f, 7.0f,
		2.0f, 4.0f, 2.0f, 4.0f,
		5.0f, 8.0f, 5.0f, 8.0f);

	Mat4 res = m0 * m1;

	Vec4 testv4(4.0f, 2.0f, 5.0f, 8.0f);
	Vec4 testvv(2.0f, 6.0f, 7.0f, 1.0f);

	Vec4 res1 = m1 * testv4;

	Vec4 res2 = testvv * m0;

	cout << res[0][0] << endl << res[1][0] << endl << res[2][0]
		<< endl << res[3][0] << endl;

	cout << res2[0] << endl << res2[1] << endl << res2[2] << endl << res2[3] << endl;
}

void memcopyFromVector()
{
	std::vector<Vec3> vec3array;
	vec3array.push_back(Vec3(1.0f, 3.0f, 4.0f));
	vec3array.push_back(Vec3(2.0f, 2.02f, 3.1f));

	Vec4 aa(1.1f, 2.2f, 3.3f, 4.4f);


	float *dest = new float[10];

	memcpy(dest, vec3array.data(), sizeof(float) * 6);
	memcpy((void *)(dest + 6), &aa[0], sizeof(float) * 4);

	cout << dest[1] << endl << dest[4] << endl << dest[8] 
		<< endl << sizeof(dest);

	delete []dest;
}

void func(Vec4 &v)
{
	Vec4 &ref = v;
	ref.x = 3.0f;
}

void localValueRef()
{
	Vec4 aa(0.0f, 1.0f, 1.0f, 0.0f);

	func(aa);

	cout << aa.x;
}

void arrayInUnion()
{
	union mat
	{
		int aa[2][2];
		int bb[4];
	};
	
	mat test; 
	test.aa[0][0] = 0;
	test.aa[0][1] = 1;
	test.aa[1][0] = 2;
	test.aa[1][1] = 3;

	cout << test.bb[0] << " " << test.bb[2];
}

void variablePtrInStruct()
{
	struct intBundle
	{
		int a;
		int b;
		int c;
	};

	intBundle ib;
	ib.a = 0;
	ib.b = 1;
	ib.c = 2;

	int bb[2] = {4, 4};

	int *aa = (int *)&ib;

	memcpy(aa, &bb, sizeof(int) * 2);
	//memcpy((void *)(&ib + sizeof(int)), &bb, sizeof(int));

	//cout << &ib << endl << &ib.a << endl;

	cout << ib.a << " " << ib.b << endl;
}

void staticMemStorage()
{
	staticMem sm;
	staticMem::a = 0;
	sm.b = 1;

	cout << sizeof(sm) << endl << sizeof(staticMem) << endl;
}

void poolAllocation()
{
	g_palloc.init(5, sizeof(int));
	
	int *a = (int *)g_palloc.alloc(); *a = 1;
	int *b = (int *)g_palloc.alloc(); *b = 2;
	int *c = (int *)g_palloc.alloc(); *c = 3;
	int *d = (int *)g_palloc.alloc(); *d = 4;
	int *e = (int *)g_palloc.alloc(); *e = 5;
	
	g_palloc.dealloc(a);
	g_palloc.dealloc(d);
	unsigned int numFree = g_palloc.getFreeBlockCount();

	int *f = (int *)g_palloc.alloc(); *f = 6;
	int *g = (int *)g_palloc.alloc(); *g = 7;

	cout << a << endl << b << endl << c << endl << d << endl << e << endl;
	cout << endl << f << endl << g;
}

void eventHandler(NNEuint a, NNEuint b)
{
	cout << endl << "Handling event " << a << " " << b  <<  endl;
}

void eventSystem()
{
	Listener alistener;

	EVENTS::WindowResizeEvent winrEvent(640, 480);
	EVENTS::WindowResizeEvent winrEvent22(640, 480);
	EVENTS::gEventMngr.add2GeneralQ(&winrEvent);
	EVENTS::gEventMngr.add2GeneralQ(&winrEvent22);

	NNE_EVENTS_REGISTER_MEMBER_FUNC(WindowResizeEvent, Listener, handler, &alistener);
	NNE_EVENTS_REGISTER_FREE_FUNC(WindowResizeEvent, eventHandler);

	EVENTS::gEventMngr.handleEvents();
}

void testMat2quat()
{
	Quaternion q0(3.232f, 102.3433f, 0.232424f, 45.0f);
	q0.normalize();
	Mat4 mat0 = Mat4(q0);

	Quaternion q1 = mat0.toQuaternion();

	if (q0 == q1)
	{
		cout << endl << "matrix4 to quaterion works correctly";
	}
	else
	{
		cout << endl << "matrix4 to quaterion function failed";
	}
}

void testUnicodeCpp()
{
	// Visual Studio will request you to change the file encoding,
	// whenever it detects there is some Unicode character.
	wchar_t *unicodeChinese = L"黄金时代";
	// can't output Unicode stream
	cout << wcslen(unicodeChinese) << endl;
}

void gaussSeidel()
{
	ProjectedGaussSeidel pgs;

	Matrix<3, 3> A;
	A[0][0] = 12.0f; A[0][1] = 3.0f; A[0][2] = -5.0f;
	A[1][0] = 1.0f; A[1][1] = 5.0f; A[1][2] = 3.0f;
	A[2][0] = 3.0f; A[2][1] = 7.0f; A[2][2] = 13.0f;

	Vector<3> b;
	b[0] = 1.0f; b[1] = 28.0f; b[2] = 76;

	Vector<3> guess;
	guess[0] = 1.0f; guess[1] = 0.0f; guess[2] = 1.0f;

	Vector<3> result;

	pgs.solve(A, b, guess, result);

	float r0 = result[0],
		  r1 = result[1],
		  r2 = result[2];
	int a = 0;
}

void getAABBtest()
{
	AABB aabb0, aabb1;
	BoxShape box(Vec3(1.0f, 1.0f, 1.0f));
	Mat4 rotation(1.0f, 0.0f, 0.0f, 0.0f,
				   0.0f, 1.0f, 0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);
	rotation.rotateX(60.0f);
	rotation.rotateY(30.0f);

	Mat4 translation(1.0f, 0.0f, 0.0f, 0.0f,
				   0.0f, 1.0f, 0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   1.0f, 2.0f, 3.0f, 1.0f);
	
	Mat4 transform = rotation * translation;

	box.getAabb(&aabb0, transform);
	box.getAabbSlow(&aabb1, transform);

	Quaternion q = translation.toQuaternion();
}