#ifndef NNE_EVENT_H
#define NNE_EVENT_H

#include "PlatformSpecs.h"
#include "Sink.h"
#include "Rect.h"

namespace NNE{
namespace EVENTS{

/* =============================================================================
   Event Implementation Macro */

// specify event handler function return type
#define NNE_EVENTS_HANDLER_RETURN_TYPE(RT) RT
// specify event handler function parameter
#define NNE_EVENTS_HANDLER_PARAMS(...) __VA_ARGS__
// arguments whose types should match handler funcions' parameters 
// and will be passed into handler functions
#define NNE_EVENTS_SIGNAL_ARGS(...) __VA_ARGS__

#define NNE_EVENTS_IMPLEMENT(RETURN_TYPE, PARAMS, ARGS, SINK_RESERVE_SIZE) \
	private: \
		/* The way to define static member only in header file*/ \
		/* This should be thread safe*/ \
		/* http://stackoverflow.com/questions/18860895/way-to-initialize-static-members-in-the-header */ \
		static Sink<RETURN_TYPE (PARAMS)> & getSink() \
		{ \
			static Sink<RETURN_TYPE (PARAMS)> m_s_sink(SINK_RESERVE_SIZE); \
			return m_s_sink; \
		} \
		\
	public: \
		template<RETURN_TYPE (*freeFunc)(PARAMS)> \
		static NNEvoid registerEventHandler() \
		{ \
			getSink().addEventHandler<freeFunc>(); \
		} \
		\
		template<class C, RETURN_TYPE (C::*memberFunc)(PARAMS)> \
		static NNEvoid registerEventHandler(C *objptr) \
		{ \
			getSink().addEventHandler<C, memberFunc>(objptr); \
		} \
		\
		NNEvoid signal() \
		{ \
			getSink().invoke(ARGS); \
		} 

/* Event Implementation Macro
============================================================================= */

/* =============================================================================
   register event handler macro helpers */

#define NNE_EVENTS_REGISTER_MEMBER_FUNC(AEVENT, ACLASS, AHANDLER, AINSTANCE_PTR) \
	NNE::EVENTS::##AEVENT::registerEventHandler<##ACLASS, &##ACLASS::##AHANDLER>(AINSTANCE_PTR);

#define NNE_EVENTS_REGISTER_FREE_FUNC(AEVENT, AHANDLER) \
	NNE::EVENTS::##AEVENT::registerEventHandler<&##AHANDLER>();

/* register event handler macro helpers
============================================================================= */


/*
Event base class, every unique event has to derive from this class.

Each Event class holds a static event handler sink, therefore events of the 
same Event class will have the same handler sink respond to.
*/
class BaseEvent
{
public:

	BaseEvent()
	{}

	virtual ~BaseEvent()
	{}

	virtual void signal() = 0;
};

struct WindowResizeEvent : public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(NNEuint, NNEuint),
						 NNE_EVENTS_SIGNAL_ARGS(width, height), 1);

	
	WindowResizeEvent(NNEuint w, NNEuint h)
		:BaseEvent(), width(w), height(h)
	{}

	~WindowResizeEvent()
	{}

	NNEuint width;
	NNEuint height;
};

struct ViewportChangedEvent: public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(Rectui),
						 NNE_EVENTS_SIGNAL_ARGS(viewport), 3);

	ViewportChangedEvent(Rectui rect)
		:viewport(rect)
	{}

	~ViewportChangedEvent(){}

	Rectui viewport;
};

// input events
// mouse move while left button down
struct MouseMoveLBDownEvent: public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(Vec2i, Vec2i),
						 NNE_EVENTS_SIGNAL_ARGS(oldPos, newPos), 3);

	MouseMoveLBDownEvent(Vec2i pos0, Vec2i pos1)
		:oldPos(pos0), newPos(pos1)
	{}

	~MouseMoveLBDownEvent(){};

	Vec2i oldPos;
	Vec2i newPos;
};

// mouse move while right button down
struct MouseMoveRBDownEvent: public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(Vec2i, Vec2i),
						 NNE_EVENTS_SIGNAL_ARGS(oldPos, newPos), 3);

	MouseMoveRBDownEvent(Vec2i pos0, Vec2i pos1)
		:oldPos(pos0), newPos(pos1)
	{}

	~MouseMoveRBDownEvent(){};

	Vec2i oldPos;
	Vec2i newPos;
};

struct MouseWheelMoveEvent: public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(NNEint),
						 NNE_EVENTS_SIGNAL_ARGS(delta), 3);

	MouseWheelMoveEvent(NNEint d)
		:delta(d)
	{}

	~MouseWheelMoveEvent(){};

	NNEint delta;
};

// keyboard events
struct Key_R_PressedActionEvent: public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(),
						 NNE_EVENTS_SIGNAL_ARGS(), 3);

	Key_R_PressedActionEvent(){}

	~Key_R_PressedActionEvent(){}
};

struct Key_CTRL_Held_MouseMoveEvent: public BaseEvent
{
	NNE_EVENTS_IMPLEMENT(NNE_EVENTS_HANDLER_RETURN_TYPE(NNEvoid),
						 NNE_EVENTS_HANDLER_PARAMS(Vec2i, Vec2i),
						 NNE_EVENTS_SIGNAL_ARGS(oldPos, newPos), 3);

	Key_CTRL_Held_MouseMoveEvent(Vec2i pos0, Vec2i pos1)
		:oldPos(pos0), newPos(pos1)
	{}

	~Key_CTRL_Held_MouseMoveEvent(){};

	Vec2i oldPos;
	Vec2i newPos;
};

} // namespace EVENTS
} // manespace NNE

#endif // NNE_EVENT_H