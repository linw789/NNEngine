#include "WinWindow.h"
#include "DebugUtilities.h"
#include "Input.h"
#include "Vec2.hpp"

namespace NNE{

bool WinWindow::init(const Descriptor &des)
{
	if (!create(des))
	{
		alwaysAssertMsg(false, "Fail to create the window!");
	}

	show();

	return true;
}

// get OS messages occur in the application which may contain several windows
void WinWindow::getOSEvent()
{
	MSG msg = {};

	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		switch(msg.message)
		{
		case WM_QUIT:
		
			break;
		
		case WM_KEYDOWN:

			gInputMngr.keyDown(msg.wParam, msg.lParam);

			// if key R is down and its previous status is up
			if (0x52 == msg.wParam && !(msg.lParam & 0x40000000))
			{
				gInputMngr.postKey_R_PressedActionEvent();
			}
			break;

		case WM_KEYUP:

			gInputMngr.keyUp(msg.wParam, msg.lParam);

			break;

		case WM_SYSKEYDOWN:
			break;
		case WM_SYSKEYUP:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_LBUTTONDBLCLK:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_RBUTTONDBLCLK:
			break;
		case WM_MOUSEMOVE:
			if (msg.wParam & MK_LBUTTON)
			{
				gInputMngr.postLBDOWNMOVE_EVENT();
			}
			else if (msg.wParam & MK_RBUTTON)
			{
				gInputMngr.postRBDOWNMOVE_EVENT();
			}
			else if (msg.wParam & MK_CONTROL)
			{
				gInputMngr.postKeyCtrlHeldMouseMove_EVENT();
			}
			break;

		case WM_MOUSEWHEEL:
			//http://stackoverflow.com/questions/92396/why-cant-variables-be-declared-in-a-switch-statement
			{
				NNEint delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);
				gInputMngr.postMouseWheelMove_EVENT(delta);
				break;
			}

		default:
			::TranslateMessage(&msg);
			// dipatch a message to a winproc
			// an application may have several windows,
			// this function will send message to its corresponding window
			// based on its hwnd value
			::DispatchMessage(&msg);
		}
	}
}

LRESULT WinWindow::handleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_SIZE:
		if (NULL != m_resizeCallback)
		{
			(*m_resizeCallback)(LOWORD(lparam), HIWORD(lparam));
		}
		break;

	default:
		return DefWindowProc(m_hwnd, msg, wparam, lparam);
	}

	return TRUE;
}

} // namespace NNE