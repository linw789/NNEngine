#ifndef NNE_SKELETON_H
#define NNE_SKELETON_H

#include "PlatformSpecs.h"
#include "Quaternion.h"
#include "Array.h"
#include "Matrix4.hpp"
#include "String.h"
#include "Animation.h"
#include "GPUProgram.h"

namespace NNE{

struct Bone
{
	Bone()
		:scale(1.0f),
		rotate(0.0f, 0.0f, 0.0f, 0.0f),
		pos(0.0f, 0.0f, 0.0f),
		parentIndex(-1)		
	{
		local_transform.identity();
		rest_transform.identity();
		offset_transform.identity();
	}

	// the local coordinate relative to its parent
	NNEfloat scale; // uniform scale
	Quaternion rotate; // orientation
	Vec3 pos; // translation

	Mat4 local_transform;
	// local transform when no animation applied
	Mat4 rest_transform;
	// in root bone's coordinate
	Mat4 root_transform;
	// the final transform with offset matrix applied that 
	// will influence the vertices
	Mat4 vert_transform;
	// transform the vertex in to local bone coordinate
	Mat4 offset_transform;

	// the index of its parent in the boneArray
	// -1 means it's the root
	NNEint parentIndex;

	// bone name
	Sid name;
};


class Skeleton
{

friend class AssimpLoader;
friend class Drawable;
friend class Skin;

public:
	
	enum
	{
		MAX_BONE_NUM = 64 // 64 mat4
	};

	Skeleton()
		:m_preAnimFrame(0), m_bonexformUniformLoc(-1)
	{}

	~Skeleton()
	{}

	Animation *findAnim(Sid name)
	{
		for (Array<Animation *>::iterator it = m_animArray.begin(); it != m_animArray.end() ; ++it)
		{
			if (name == (*it)->getNameSid())
			{
				return (*it);
			}
		}

		return NULL;
	}

	// update bone's transform to root bone's coordinate
	NNEvoid updateBoneTransform();

	// time is measured in second
	// time is 0.0 when the animation just begins
	NNEvoid setPose(Animation *anim, NNEdouble elapsedTime);

	NNEbool addAnimation(Animation *anim);

	NNEvoid uploadBoneXForm(GPUProgram *);

private:

	// array of all bones in the skeleton
	Array<Bone> m_boneArray;

	// animations this skeleton has
	Array<Animation *> m_animArray;

	// 4 floats for quaternion, 3 for translation and 1 for uniform scale
	// caution: all skeleton instances share this single buffer
	static NNEfloat s_boneXForm[MAX_BONE_NUM * 16];

	// uniform location of bone fransform
	NNEint m_bonexformUniformLoc;

	// init: preAnimFrame = 0
	NNEuint m_preAnimFrame;
};

} // manespace NNE

#endif // NNE_SKELETON_H