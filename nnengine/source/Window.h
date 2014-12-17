/*
	reference: http://www.songho.ca/opengl/gl_mvc.html
			   GLG3D/OSWindow.h
*/

#ifndef NNE_WINDOW_H
#define NNE_WINDOW_H

#include "PlatformSpecs.h"

namespace NNE{

/*
  This is a abstraction of a window system, containing basic information
  and necessary functions to construct a window for display.
  Different platforms should create different sub-classes inheriting from this class.
  MSWindow.h for MSWindows already exists
*/
class Window
{
public:

	struct Descriptor
	{
		Descriptor()
			:lpWindowName(L"NNEngine test"), dwExStyle(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE),
			 dwStyle(WS_OVERLAPPEDWINDOW), width(854), height(480), x(80), y(40),
			 isFullScreen(false)
		{}

		NNEvoid setWindowName(PCWSTR n)
		{
			lpWindowName = n;
		}

		NNEvoid setSize(NNEuint w, NNEuint h)
		{
			width = w;
			height = h;
		}

		NNEvoid setUpLeftPos(NNEuint xx, NNEuint yy)
		{
			x = xx;
			y = yy;
		}

		NNEvoid setIsFullscreen(NNEbool full)
		{
			isFullScreen = full;
		}

		// window title, PCWSTR = pointer const wide(unicode) string 
		PCWSTR lpWindowName;

		// The extended window style of the window being created
		DWORD dwExStyle;

		// The style of the window being created
		DWORD dwStyle;

		// window client area in pixel
		NNEuint width, height;

		// position of upper-left corner of window client area
		NNEuint x, y;

		// whether it's in full-screen 
		NNEbool isFullScreen;

		// window title
		// should I use std::string?
		//char winTitle[128];
	};

	virtual NNEvoid create(){};

	virtual ~Window(){};

protected:
	// window attributes are set to protected so that its subclasses can have direct access to them
	//Descriptor m_descriptor;
};

} // namespace NNE

#endif // NNE_WINDOW_H