#include "RenderDevice.h"
#include "DebugUtilities.h"
#include "WinWindow.h"
#include "Mesh.h"
#include "Event.h"

// The .. tells the OS to jump one directory up when the compiler asks for the file.
#include "../../thirdparty/include/GL/glew.h"

namespace NNE{

NNEbool RenderDevice::init(WinWindow *window)
{
	createContext(window);
	initGLEW();

	// register setViewport method of RenderDevice to WindowResizeEvent
	NNE_EVENTS_REGISTER_MEMBER_FUNC(WindowResizeEvent, RenderDevice, windowResizeHandler, &gRenderDevice);

	setFaceCulling(true);
	setFrontFace(GL_CCW);
	setCullFace(GL_BACK);
	
	return true;
}

NNEvoid RenderDevice::drawMesh(Mesh *amesh)
{


	amesh->draw();
}

NNEvoid RenderDevice::swapBuffers()
{
	::SwapBuffers(m_hdc);
}

NNEbool RenderDevice::createContext(WinWindow *window)
{
	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		m_des.colorBits * 3,						// Select Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		m_des.alphaBits,							// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		m_des.depthBits,							// depthBuffer-bit Z-Buffer (Depth Buffer)  
		m_des.stencilBits,							// stencilBuffer-bit Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	m_hwnd = window->getHandle();

	m_hdc = GetDC(m_hwnd);
	debugAssertMsg(m_hdc, "Cannot retrive OpenGL Device Context");

	int pixelFormatId = ChoosePixelFormat(m_hdc, &pfd);
	debugAssertMsg(pixelFormatId, "not pixel format ID");

	if (SetPixelFormat(m_hdc, pixelFormatId, &pfd) == false)
	{
		debugAssertMsg(false, "cannot set pixel format");
	}

	m_hglrc = wglCreateContext(m_hdc);
	debugAssertMsg(m_hglrc, "cannot create OpenGL context");

	if (wglMakeCurrent(m_hdc, m_hglrc) == false)
	{
		debugAssertMsg(false, "cannot make current gl context");
	}

	return true;
}

NNEvoid RenderDevice::closeContext()
{
	if(m_hglrc)
	{
		if(!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, L"Release DC and RC failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		}

		if(!wglDeleteContext(m_hglrc))
		{
			MessageBox(NULL, L"Release rendering context failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		}

		m_hglrc = NULL;
	}

	if(m_hdc && !ReleaseDC(m_hwnd, m_hdc))
	{
		MessageBox(NULL, L"Release device context failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		m_hdc = NULL;
	}
}

NNEvoid RenderDevice::initGLEW()
{
	debugOutput("OpenGL Version:" << glGetString(GL_VERSION) << "\n"
				<< "OpenGL Vendor:" << glGetString(GL_VENDOR) << "\n"
				<< "OpenGL Renderer:" << glGetString(GL_RENDERER) << "\n"
				<< "OpenGL Shading Language Version:" 
				<< glGetString(GL_SHADING_LANGUAGE_VERSION));

	GLenum glewErr = glewInit();
	if (GLEW_OK != glewErr)
	{
		debugOutput(glewGetErrorString(glewErr));
		debugAssertMsg(false, "Fail to initilize GLEW");
	}
}

// have to define it once, let the compiler know it has already existed
RenderDevice gRenderDevice;

} // namespace NNE