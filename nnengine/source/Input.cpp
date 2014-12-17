#include "Input.h"

namespace NNE{

NNEvoid Input::update(WinWindow *win)
{
	m_lastMousePos = m_currentMousePos;

	::POINT p;
	::GetCursorPos(&p);
	::ScreenToClient(win->getHandle(), &p);

	m_currentMousePos.x = p.x;
	m_currentMousePos.y = p.y;

	m_keyDownStroke = -1;
	m_keyUpStroke= -1;
}

Input gInputMngr;

} // namespace NNE