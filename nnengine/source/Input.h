#ifndef NNE_INPUT_H
#define NNE_INPUT_H

#include "PlatformSpecs.h"
#include "WinWindow.h"
#include "EventManager.h"
#include "Event.h"
#include "Vec2.hpp"

namespace NNE{

class Input
{
	//http://msdn.microsoft.com/en-us/library/windows/desktop/ms646267(v=vs.85).aspx
	enum FLAG
	{
		CURRENT_KEY_DOWN = 0x00000002,
		CURRENT_KEY_STATE = 0x00000002,

		KEY_DOWN_STROKE = 0x00000004,
		KEY_UP_STROKE = 0x00000008,

		PREVIOUS_KEY_DOWN = 0x00000001,
		PREVIOUS_KEY_UP = 0x00000000,
		PREVIOUS_KEY_STATE = 0x00000001,

		// The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
		// The value is always 1 for a WM_KEYUP message.
		PREVIOUS_STATE = 0x40000000,
		// 0 for WM_KEYDOWN message
		// 1 for WM_KEYUP message
		TRANSITION_STATE = 0x80000000,
	};

public:
	
	Input()
	{
		memset(&m_keyStatus, 0, 256 * sizeof(NNEuint));
	}

	~Input()
	{}

	NNEvoid update(WinWindow *win);

	NNEvoid keyDown(WPARAM wparam, LPARAM lparam)
	{
		 m_keyStatus[wparam] |= CURRENT_KEY_DOWN;
	}

	NNEvoid keyUp(WPARAM wparam, LPARAM lparam)
	{
		m_keyStatus[wparam]  &= ~CURRENT_KEY_DOWN;
		m_keyUpStroke = wparam;
	}

	// key down stroke
	//NNEbool ifKeyDownStroke(NNEchar key)
	//{
	//}

	NNEbool ifKeyDown(NNEchar key)
	{
		if (CURRENT_KEY_DOWN == (m_keyStatus[key] & CURRENT_KEY_STATE))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// key up stroke action
	NNEbool ifKeyUpStroke(NNEchar key)
	{
		return key == m_keyUpStroke;
	}

	// key 'R' pressed action not held
	NNEvoid postKey_R_PressedActionEvent()
	{
		EVENTS::Key_R_PressedActionEvent *keyr = new EVENTS::Key_R_PressedActionEvent();
		EVENTS::gEventMngr.add2InputQ(keyr);
	}

	// mouse move while left button down
	NNEvoid postLBDOWNMOVE_EVENT()
	{
		EVENTS::MouseMoveLBDownEvent *mmlbdEvent = new EVENTS::MouseMoveLBDownEvent(m_lastMousePos, m_currentMousePos);
		EVENTS::gEventMngr.add2InputQ(mmlbdEvent);
	}

	// mouse move while right button down
	NNEvoid postRBDOWNMOVE_EVENT()
	{
		EVENTS::MouseMoveRBDownEvent *mmrbdEvent = new EVENTS::MouseMoveRBDownEvent(m_lastMousePos, m_currentMousePos);
		EVENTS::gEventMngr.add2InputQ(mmrbdEvent);
	}

	NNEvoid postMouseWheelMove_EVENT(NNEint delta)
	{
		EVENTS::MouseWheelMoveEvent *mwmevent = new EVENTS::MouseWheelMoveEvent(delta);
		EVENTS::gEventMngr.add2InputQ(mwmevent);
	}

	NNEvoid postKeyCtrlHeldMouseMove_EVENT()
	{
		EVENTS::Key_CTRL_Held_MouseMoveEvent *kchmmEvent = new EVENTS::Key_CTRL_Held_MouseMoveEvent(m_lastMousePos, m_currentMousePos);
		EVENTS::gEventMngr.add2InputQ(kchmmEvent);
	}

private:

	// If the least significant bit is set, the key is down.
	// If the second least significant bit is set, the key was previously down
	NNEint m_keyStatus[256];
	NNEint m_keyDownStroke;
	NNEint m_keyUpStroke;

	//// mouse buttons
	//NNEuint m_mouse

	// last frame mouse position
	Vec2i m_lastMousePos;

	// current frame mouse position
	Vec2i m_currentMousePos;
};

extern Input gInputMngr;

} // manespace NNE

#endif // NNE_INPUT_H