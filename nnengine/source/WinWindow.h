#ifndef NNE_WIN_WINDOW_H
#define NNE_WIN_WINDOW_H

#include "PlatformSpecs.h"
#include "Window.h"

namespace NNE{

/*
	This approach could make a member function the callback,
	so that all operations on the window are constrained within
	the class itself, eliminating global static callback function.
	However, this approach requires that objects must have handles as reference (HWND in this case).
	see http://msdn.microsoft.com/en-us/library/ff381400(v=vs.85).aspx
*/
template<class WIN_CLASS>
class WinWindowBase : public Window
{
	typedef NNEvoid (*ResizeFuncType)(NNEuint, NNEuint);

public:

	WinWindowBase()
		:m_hwnd(NULL), m_resizeCallback(NULL)
	{}

	virtual ~WinWindowBase(){};

	// only static member function can be pass in to other functions as parameter
	static LRESULT CALLBACK _winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// create the window
	BOOL create(const Window::Descriptor &descriptor);

	NNEvoid show()
	{
		::ShowWindow(m_hwnd, SW_SHOW);
	}

	NNEvoid getOSEvent();

	NNEvoid setResizeCallback(ResizeFuncType cb)
	{
		m_resizeCallback = cb;
	}

	// accessor
	HWND getHandle()
	{
		return m_hwnd;
	}

protected:
	// handle to the instance
	HWND m_hwnd;

	ResizeFuncType m_resizeCallback;

	void killWindow();

	// pure virtual functions
	virtual PCWSTR className() const = 0;
	virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

template<class WIN_CLASS>
LRESULT CALLBACK WinWindowBase<WIN_CLASS>::_winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WIN_CLASS *pWindow = NULL;

	if (WM_NCCREATE == msg)
	{
		CREATESTRUCT *pCreate = (CREATESTRUCT *)lParam;
		pWindow = (WIN_CLASS *)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);

		pWindow->m_hwnd = hwnd;
	}
	else
	{
		pWindow = (WIN_CLASS *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (pWindow)
	{
		return pWindow->handleMessage(msg, wParam, lParam);
	}
	else
	{
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

}

template<class WIN_CLASS>
BOOL WinWindowBase<WIN_CLASS>::create(const Window::Descriptor &descriptor)
{
	RECT clientArea;//contain rectangle top left and bottom right
	clientArea.left = (long)descriptor.x;
	clientArea.right = (long)descriptor.width + descriptor.x;
	clientArea.top = (long)descriptor.y;
	clientArea.bottom = (long)descriptor.height + descriptor.y;

	WNDCLASS wc = {0};
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WIN_CLASS::_winProc;// WIN_CLASS is the same class derives from MSWindowBase 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ::GetModuleHandle(NULL);
	wc.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className();// ClassName() has to be implemented by subclasses

	if(!RegisterClass(&wc))
	{
		::MessageBox(NULL, L"Fail to register window class", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	// adjust clientArea to frame area of the window
	::AdjustWindowRectEx(&clientArea, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	m_hwnd = CreateWindowEx(descriptor.dwExStyle, className(), descriptor.lpWindowName, descriptor.dwStyle, 
		clientArea.left, clientArea.top, clientArea.right - clientArea.left, clientArea.bottom - clientArea.top, 
		/*hWndParent*/NULL, /*hMenu*/NULL, ::GetModuleHandle(NULL), this);//"this" pointes to the instantiated class itself

	if(!m_hwnd)
	{
		::MessageBox(NULL, L"Window creation failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	return TRUE;
}

template<class WIN_CLASS>
void WinWindowBase<WIN_CLASS>::killWindow()
{
	if(m_hwnd && !DestroyWindow(m_hwnd))
	{
		MessageBox(NULL, L"Release m_hwnd failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		m_hwnd = NULL;
	}

	if(!UnregisterClass(ClassName(), GetModuleHandle(NULL)))
	{
		MessageBox(NULL, L"Unregister Class failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
	}
}

class WinWindow: public WinWindowBase<WinWindow>
{
public:
	// cannot declare default construtor or destructor without defining them
	// constructor and destructor
	// making constructor and destructor empty to constain
	// the construction and destruction order of each object
	WinWindow(){};
	~WinWindow(){};

	bool init(const Descriptor &);

	NNEvoid getOSEvent();

	// implementation of pure virtual functions in WinWindowBase
	PCWSTR className() const // passed into WNDCLASS structure
	{
		return L"NNEngine Window Class";
	}

	LRESULT handleMessage(UINT msg, WPARAM wparam, LPARAM lparam);

private:
	
	
};

} // namespace NNE

#endif // NNE_WIN_WINDOW_H