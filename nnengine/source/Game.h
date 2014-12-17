#ifndef NNE_GAME_H
#define NNE_GAME_H

#include "PlatformSpecs.h"
#include "Matrix4.hpp"
#include "WinWindow.h"

namespace NNE{

//forward declaration
class WinWindow;

// singleton might be bad.
// http://jalf.dk/blog/2010/03/singletons-solving-problems-you-didnt-know-you-never-had-since-1995/
// http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
//
class Game
{
public:
	struct Descriptor
	{
		Descriptor()
			:showCmd(0)
		{}

		Descriptor(int nShowCmd)
			:showCmd(nShowCmd)
		{}

		int showCmd;
	};

	// constructor and destructor
	// making constructor and destructor empty to constain
	// the construction and destruction order of each object
	Game()
		:m_FPS(0),m_frameTime(0), m_currentTime(0)
	{};

	~Game(){};

	// initialize the game
	NNEbool init();

	// have to be static so the size of function pointer
	// is the size of free function pointer 
	static NNEvoid resizeCallback(NNEuint, NNEuint);

	NNEvoid updateFPS();

	NNEdouble getFPS()
	{
		return m_FPS;
	}

	NNEdouble getFrameTime()
	{
		return m_frameTime;
	}

	NNEvoid setup();

	// enter main the game loop
	NNEvoid run();

	NNEvoid oneFrame();

	// shut down the game
	NNEvoid close();

private:

	// hide the constructors
	Game(const Game &g);
	Game & operator = (const Game &g);

	Descriptor m_descriptor;

	NNEdouble m_FPS;
	// second per frame = 1.0 / m_FPS
	NNEdouble m_frameTime;
	NNEdouble m_currentTime;

	// game components
	WinWindow m_window;
};
	
} // namespace NNE

#endif // NNE_GAME_H