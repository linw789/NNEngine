#include "Skeleton.h"
#include "Matrix4.hpp"
#include "Resource.h"

namespace NNE{

NNEfloat Skeleton::s_boneXForm[MAX_BONE_NUM * 16];

NNEvoid Skeleton::updateBoneTransform()
{
	for (Array<Bone>::iterator it = m_boneArray.begin(); it != m_boneArray.end(); ++it)
	{
		// local transform = T * R * S
		(*it).local_transform = Mat4((*it).rotate);	
		(*it).local_transform.setPos((*it).pos);
		(*it).local_transform[0][0] *= (*it).scale;
		(*it).local_transform[1][1] *= (*it).scale;
		(*it).local_transform[2][2] *= (*it).scale;
			
		NNEint parentIndex = (*it).parentIndex;
		if (parentIndex != -1)
		{
			// bone hierarchy is stored in the array with the order that all parents are stored
			// before children, so the child only needs to multiply it's immediate parent's root
			// transform to get its root transform
			(*it).root_transform = m_boneArray[parentIndex].root_transform * (*it).local_transform;
		}
		else // it's the root
		{
			(*it).root_transform = (*it).local_transform;
		}
		// skip the offset matrix, if you want to draw skeleton only
		(*it).vert_transform = (*it).root_transform * (*it).offset_transform;
	}
}

NNEvoid Skeleton::setPose(Animation *anim, NNEdouble time)
{
	NNEuint nextAnimFrame;
	NNEfloat t = .0f;

	if (0.0 == time) // the beginning the one animation loop
	{
		m_preAnimFrame = 0;
		nextAnimFrame = m_preAnimFrame + 1;
	}
	else
	{
		nextAnimFrame = m_preAnimFrame + 1;

		NNEdouble nextTime = anim->getTimeStampAt(nextAnimFrame);

		// if the current time exceeds the next frame time, the animation
		// should interpolated between the next frame and the frame after that
		while (time > nextTime)
		{
			// advance both previous and next frames by one
			m_preAnimFrame++;

			nextTime = anim->getTimeStampAt(++nextAnimFrame);
		}
		NNEdouble preTime = anim->getTimeStampAt(m_preAnimFrame);

		t = NNEfloat((time - preTime) / (nextTime - preTime));
		debugAssertMsg(t >= 0.0f, "animation time corrupted");
	}

	NNEuint bonesize = m_boneArray.size();
	for (NNEuint i = 0; i < bonesize; ++i)
	{
		AnimNode *tmpAnimNode = anim->getAnimNode(i);
		m_boneArray[i].scale = tmpAnimNode->lerpUniScale(m_preAnimFrame, nextAnimFrame, t);
		m_boneArray[i].rotate = tmpAnimNode->slerpRotation(m_preAnimFrame, nextAnimFrame, t);
		m_boneArray[i].pos = tmpAnimNode->lerpTranslation(m_preAnimFrame, nextAnimFrame, t);
	}

	updateBoneTransform();
}

NNEbool Skeleton::addAnimation(Animation *anim)
{
	NNEbool res = true;

	// check if animation has the same bones with the same order as those in skeleton
	if (m_boneArray.size() == anim->getNumAnimNode())
	{
		NNEuint bonesize = m_boneArray.size();
		for (NNEuint i = 0; i < bonesize; ++i)
		{
			if (m_boneArray[i].name =! anim->getAnimNode(i)->m_name)
			{
				res = false;
				break;
			}
		}
	}
	else
	{
		res = false;
	}

	m_animArray.push_back(anim);

	return res;
}

NNEvoid Skeleton::uploadBoneXForm(GPUProgram *gpup)
{
	for (NNEuint i = 0; i < m_boneArray.size(); ++i)
	{
		memcpy(&s_boneXForm[i * 16], &m_boneArray[i].vert_transform[0][0], 
			sizeof(NNEfloat) * 16);
	}

	gpup->updateUniformMatrix4(GPUProgram::UNIFORM_LOC_BONE_XFORM, MAX_BONE_NUM, s_boneXForm);
}

} // namespace NNE