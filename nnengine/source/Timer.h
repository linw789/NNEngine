#ifndef NNE_TIMER_H
#define NNE_TIMER_H

#include "PlatformSpecs.h"

namespace NNE{

class Timer
{
public:
	
	Timer()
	{}

	~Timer()
	{}

	NNEvoid start()
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&m_gameStartTick);
	}

	NNEvoid init()
	{
		QueryPerformanceFrequency((LARGE_INTEGER *)&m_tps);
		m_spt = NNEdouble(1.0) / m_tps;
	}

	NNEdouble newTime();

private:

	// 
	NNEint64 m_gameStartTick;

	// cpu ticks per second
	NNEint64 m_tps;

	// 1 / m_tps
	NNEdouble m_spt;
};

extern Timer gTimer;

} // manespace NNE

#endif // NNE_TIMER_H