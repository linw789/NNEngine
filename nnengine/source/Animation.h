#ifndef NNE_ANIMATION_H
#define NNE_ANIMATION_H

#include "PlatformSpecs.h"
#include "Skeleton.h"
#include "Array.h"
#include "Vec3.hpp"
#include "Quaternion.h"
#include "String.h"

namespace NNE{

struct AnimNode
{
	NNEfloat lerpUniScale(NNEuint pre, NNEuint next, NNEfloat t)
	{
		return m_uniScale[pre] * (1 - t) + m_uniScale[next] * t;
	}

	Vec3 lerpTranslation(NNEuint pre, NNEuint next, NNEfloat t)
	{
		return m_translation[pre] * (1 - t) + m_translation[next] * t;
	}

	Quaternion slerpRotation(NNEuint pre, NNEuint next, NNEfloat t)
	{
		return Quaternion::slerp(t, m_rotation[pre], m_rotation[next]);
	}

	// only uniform scale
	Array<NNEfloat> m_uniScale;

	Array<Quaternion> m_rotation;

	Array<Vec3> m_translation;

	Sid m_name;
};

class Animation
{

friend class AssimpLoader;

public:
	
	Animation(Sid name)
		:m_name(name)
	{}

	~Animation()
	{}

	Sid getNameSid()
	{
		return m_name;
	}

	NNEuint getNumAnimNode()
	{
		return m_animNodes.size();
	}

	AnimNode *getAnimNode(NNEuint index)
	{
		return m_animNodes[index];
	}

	NNEdouble getAnimDuration()
	{
		return m_duration;
	}

	// get the time stamp at the ith frame
	NNEdouble getTimeStampAt(NNEuint i)
	{
		return m_timeStamp[i];
	}

private:

	// duration time in second
	NNEdouble m_duration;

	Array<NNEdouble> m_timeStamp;

	Array<AnimNode *> m_animNodes;

	Sid m_name;
};

} // manespace NNE

#endif // NNE_ANIMATION_H