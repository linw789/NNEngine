#ifndef NNE_RENDER_DEVICE_H
#define NNE_RENDER_DEVICE_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Color.hpp"
#include "BufferObjectGPU.h"
#include "VertexArrayObject.h"
#include "Scene.h"
#include "Vec4.hpp"
#include "Rect.h"
#include "Event.h"
#include "EventManager.h"

namespace NNE{

class WinWindow;
class Mesh;

class RenderDevice
{
public:

	struct Descriptor
	{
		Descriptor()
			:colorBits(8), alphaBits(8), depthBits(16), stencilBits(8)
		{}

		// bits of each color channel R, G, B
		int colorBits;

		// bits of alpha chennal
		int alphaBits;

		// bits of depth buffer
		int depthBits;

		// bits of stencil buffer
		int stencilBits;

	};

	struct States
	{
		States()
			:clearClr(0.0f, 0.0f, 0.0f, 1.0f), clearDepth(1.0f), clearStencil(0)
		{}

		Color4 clearClr;
		NNEfloat  clearDepth;
		NNEint  clearStencil;
	};

	// constructor and destructor
	// making constructor and destructor empty to constain
	// the construction and destruction order of each object
	RenderDevice(){};
	~RenderDevice(){};

	// initialize render device
	// create OpenGL context and associate it to the display window
	NNEbool init(WinWindow *win);

	NNEvoid setClearColor(NNEfloat r, NNEfloat g, NNEfloat b, NNEfloat a)
	{
		m_states.clearClr = Color4(r, g, b, a);
		glClearColor(r, g, b, a);
	}

	NNEvoid setClearDepth(NNEfloat d)
	{
		m_states.clearDepth = d;
		glClearDepth(d);
	}

	NNEvoid setClearStencil(NNEint s)
	{
		m_states.clearStencil = s;
		glClearStencil(s);
	}

	NNEvoid setPolygonMode(GLenum face, GLenum mode)
	{
		glPolygonMode(face, mode);
	}

	NNEvoid setFrontFace(GLenum ff)
	{
		glFrontFace(ff);
	}

	NNEvoid setCullFace(GLenum cf)
	{
		glCullFace(cf);
	}

	NNEvoid setFaceCulling(NNEbool fc)
	{
		if (fc)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
	}

	NNEvoid depthMask(NNEbool b)
	{
		glDepthMask(b);
	}

	NNEvoid clear(GLbitfield bit)
	{
		glClear(bit);
	}

	// start drawing one frame
	NNEvoid startFrame();

	// 
	NNEvoid drawScene(Scene &);

	// end one frame
	NNEvoid endFrame(); 

	// render one mesh
	NNEvoid drawMesh(Mesh *);

	// 
	NNEvoid swapBuffers();

	NNEvoid setViewport(NNEuint x, NNEuint y, NNEuint w, NNEuint h)
	{
		m_viewportRect = Rectui(x, y, w, h);
		glViewport(x, y, w, h);
	}

	Rectui getViewport()
	{
		return m_viewportRect;
	}

	NNEvoid windowResizeHandler(NNEuint w, NNEuint h)
	{
		m_viewportRect.setWidthHeight(w, h);

		// No need to call glViewport which will be called in the reszie call back
		// of the class Game
		/*Vec2i pos = m_viewportRect.getUpLeft();
		glViewport(pos.x, pos.y, w, h);*/

		// immediately signal the event
		EVENTS::ViewportChangedEvent *aevent = 
			new EVENTS::ViewportChangedEvent(m_viewportRect);
		EVENTS::gEventMngr.add2GeneralQ(aevent);
	}

	NNEvoid Key_R_PressedActionHandler()
	{
		static NNEuint count = 0;

		switch(count)
		{
		case 0:
			setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			++count;
			break;
		case 1:
			setPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			++count;
			break;
		case 2:
			setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			count = 0;
			break;
		default:
			count %= 3;
		}
	}
	
	NNEfloat getViewportAspect()
	{
		return m_viewportRect.getAspect();
	}

	// When you apply the const qualifier to a nonstatic member function, 
	// it affects only the this pointer. For a const-qualified member function of class C, 
	// the this pointer is of type C const*, whereas for a member function that is not const-qualified, 
	// the this pointer is of type C*.
	// A static member function does not have a this pointer, 
	// so const qualification of a static member function doesn't make any sense.
	// --- James McNellis(on Stack Overflow)
	//static void unbindBuffer(const BufferObjectGPU &bo) // const
	//{
	//	GL_DEBUG(glBindBuffer(bo.getBufferType(), 0));
	//}

private:

	// uncopyable
	RenderDevice(const RenderDevice &);
	RenderDevice &operator = (const RenderDevice &);

	bool createContext(WinWindow *window);
	void closeContext();

	// initialize GLEW
	void initGLEW();

private:

	// rectangle of view port
	Rectui m_viewportRect;

	// rendering context description
	Descriptor m_des;

	// rendering pipeline states
	States m_states;

	// the handle to the display window
	HWND m_hwnd;

	// the handle to device context
	HDC m_hdc;

	// the handle to opengl rendering context
	HGLRC m_hglrc;
};

// Declare global objects so they can be accessed through out the engine.
// Declare extern in one header file and define them in one source file,
// then you can directly use them in other source files.
extern RenderDevice gRenderDevice;

} // namespace NNE

#endif // NNE_RENDER_DEVICE_H