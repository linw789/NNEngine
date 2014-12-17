#include "EventManager.h"
#include "Event.h"

namespace NNE{
namespace EVENTS{

NNEvoid EventManager::init()
{

}

NNEvoid EventManager::handleEvents()
{
	// handle input events
	while(! m_inputEventQ.empty())
	{
		BaseEvent *evt = m_inputEventQ.front();
		m_inputEventQ.pop_front();
		evt->signal();
	}

	// handle general events
	while(!m_generalEventQ.empty())
	{
		BaseEvent *evt = m_generalEventQ.front();
		// this call must preceed evt->singal, because it's possible
		// new events being added to event queue inside the signal call
		m_generalEventQ.pop_front();// make sure event gets deleted
		evt->signal();
	}
}

EventManager gEventMngr;

} // namespace EVENTS
} // namespace NNE