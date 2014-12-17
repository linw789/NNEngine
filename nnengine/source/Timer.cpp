#include "Timer.h"

namespace NNE{

Timer gTimer;

NNEdouble Timer::newTime()
{
	NNEint64 current;
	QueryPerformanceCounter((LARGE_INTEGER *)&current);

	return (current - m_gameStartTick) * m_spt;
}

} // namespace NNE