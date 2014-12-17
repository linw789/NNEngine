#include "Game.h"
#include "Matrix4.hpp"

using namespace NNE;

// Microsoft C runtime library (CRT) provides an implementation of main that calls either WinMain or wWinMain.
// wWinMain accept lpCmdLine as Unicode, while WinMain ANSI
int WINAPI wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
	Mat4 m0, m1;
	Mat4 mm = m0 * m1;

	Game theGame;
	theGame.init();
	theGame.run();

	return 0;
}