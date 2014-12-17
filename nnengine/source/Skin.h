#ifndef NNE_SKIN_H
#define NNE_SKIN_H

#include "PlatformSpecs.h"
#include "Mesh.h"
#include "String.h"
#include "Skeleton.h"
#include "Animation.h"

namespace NNE{

// extend the Mesh class to contain bone information
class Skin: public Mesh
{

friend class AssimpLoader;

public:

	Skin(Sid name)
		:Mesh(name, GPU_TRIANGLES),
		m_loopNum(0), m_currentAnim(NULL), m_animStartTime(0.0)
	{

	}

	~Skin()
	{}

	NNEvoid uploadBone2GPU();

	NNEvoid addAnimation();

	Skeleton *getSkeleton()
	{
		return &m_skeleton;
	}

	NNEvoid beginAnimation(const char *animName, NNEint numloop);

	NNEvoid animate();

private:

	Skeleton m_skeleton;

	// how many loop the animation will be played
	// -1 menas play infinitely
	NNEint m_loopNum;

	// current animation being played
	Animation *m_currentAnim;

	NNEdouble m_animStartTime;
};

} // manespace NNE

#endif // NNE_SKIN_H