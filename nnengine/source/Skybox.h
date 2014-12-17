#ifndef NNE_SKYBOX_H
#define NNE_SKYBOX_H

#include "PlatformSpecs.h"
#include "Drawable.h"

namespace NNE{

class Skybox: public Drawable
{
public:
	
	Skybox(Sid name)
		:Drawable(name)
	{
		
	}

	~Skybox()
	{}

private:
};

} // manespace NNE

#endif // NNE_SKYBOX_H