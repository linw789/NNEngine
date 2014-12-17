#ifndef NNE_FRUSTUM_H
#define NNE_FRUSTUM_H

#include "PlatformSpecs.h"

namespace NNE{

/*
	Pyramid with four side facets and two parallel plane cutting it 
*/
class Frustum
{
public:
	
	Frustum()
	{}

	~Frustum()
	{}

private:

	// near plane and far plane
	NNEfloat m_near;
	NNEfloat m_far;


};

} // manespace NNE

#endif // NNE_FRUSTUM_H