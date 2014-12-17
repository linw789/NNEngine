#ifndef NNE_EVENTMANAGER_H
#define NNE_EVENTMANAGER_H

#include "PlatformSpecs.h"
#include <deque>

namespace NNE{
namespace EVENTS{

class BaseEvent;

class EventManager
{
public:
	
	EventManager()
	{}

	~EventManager()
	{}

	NNEvoid init();

	NNEvoid add2GeneralQ(BaseEvent *be)
	{
		m_generalEventQ.push_back(be);
	}

	NNEvoid add2InputQ(BaseEvent *be)
	{
		m_inputEventQ.push_back(be);
	}

	NNEvoid handleEvents();

private:

	std::deque<BaseEvent *> m_generalEventQ;

	std::deque<BaseEvent *> m_inputEventQ;
};

extern EventManager gEventMngr;

} // namespace EVENTS
} // manespace NNE

#endif // NNE_EVENTMANAGER_H