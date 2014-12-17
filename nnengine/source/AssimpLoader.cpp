#include "AssimpLoader.h"
#include "String.h"
#include "Mesh.h"
#include "Skin.h"
#include "Animation.h"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Matrix4.hpp"
#include "Quaternion.h"
#include "String.h"

namespace NNE{

NNEbool AssimpLoader::loadAssimpMesh(const aiMesh *assimpMesh, Mesh * amesh)
{
	/*NNEuint vertCount = assimpMesh->mNumVertices;

	NNEuint oldVertNum = amesh->m_va.m_position.size();

	if (assimpMesh->HasPositions())
	{
		amesh->m_va.m_position.reserve(oldVertNum + vertCount);
		for (NNEuint i = 0; i < vertCount; i++)
		{
			NNEfloat tmp0 = assimpMesh->mVertices[i].x;
			NNEfloat tmp1 = assimpMesh->mVertices[i].y;
			NNEfloat tmp2 = assimpMesh->mVertices[i].z;

			amesh->m_va.m_position.push_back(Vec3(assimpMesh->mVertices[i].x,
												  assimpMesh->mVertices[i].y,
												  assimpMesh->mVertices[i].z));
		}
	}

	if (assimpMesh->HasNormals())
	{
		amesh->m_va.m_normal.reserve(oldVertNum + vertCount);
		for (NNEuint i = 0; i < vertCount; i++)
		{
			amesh->m_va.m_normal.push_back(Vec3(assimpMesh->mNormals[i].x,
												assimpMesh->mNormals[i].y,
												assimpMesh->mNormals[i].z));
		}
	}

	if (assimpMesh->HasTangentsAndBitangents())
	{
	}

	if (assimpMesh->HasVertexColors(0))
	{
		amesh->m_va.m_color.reserve(oldVertNum + vertCount);
		for (NNEuint i = 0; i < vertCount; i++)
		{
			amesh->m_va.m_color.push_back(Vec4(assimpMesh->mColors[0]->r,
											   assimpMesh->mColors[0]->g,
											   assimpMesh->mColors[0]->b,
											   assimpMesh->mColors[0]->a));
		}
	}

	if (assimpMesh->HasTextureCoords(0))
	{
		amesh->m_va.m_texcoord0.reserve(oldVertNum + vertCount);
		for (NNEuint i = 0; i < vertCount; i++)
		{
			amesh->m_va.m_texcoord0.push_back(Vec2(assimpMesh->mTextureCoords[0][i].x,
				                                    assimpMesh->mTextureCoords[0][i].y));
		}
	}

	if (assimpMesh->HasTextureCoords(1))
	{
		amesh->m_va.m_texcoord1.reserve(oldVertNum + vertCount);
		for (NNEuint i = 0; i < vertCount; i++)
		{
			amesh->m_va.m_texcoord1.push_back(Vec2(assimpMesh->mTextureCoords[1][i].x,
													assimpMesh->mTextureCoords[1][i].y));
		}
	}

	if (assimpMesh->HasFaces())
	{
		int m_indicesNum = assimpMesh->mNumFaces * 3;
		amesh->m_va.m_triIndex.reserve(amesh->m_va.m_triIndex.size() + m_indicesNum);
		for (NNEuint i = 0; i < assimpMesh->mNumFaces; ++i)
		{
			amesh->m_va.m_triIndex.push_back(oldVertNum + assimpMesh->mFaces[i].mIndices[0]);
			amesh->m_va.m_triIndex.push_back(oldVertNum + assimpMesh->mFaces[i].mIndices[1]);
			amesh->m_va.m_triIndex.push_back(oldVertNum + assimpMesh->mFaces[i].mIndices[2]);
		}
	}

	return true;*/

	size_t pos = assimpMesh->HasPositions() ? sizeof(Vec3) : 0;
	size_t norm = assimpMesh->HasNormals() ? sizeof(Vec3) : 0;
	size_t tang = assimpMesh->HasTangentsAndBitangents() ? sizeof(Vec4) : 0;
	size_t clr = assimpMesh->HasVertexColors(0) ? sizeof(Vec4) : 0;
	size_t tc0 = assimpMesh->HasTextureCoords(0) ? sizeof(Vec2) : 0;
	size_t tc1 = assimpMesh->HasTextureCoords(1) ? sizeof(Vec2) : 0;
	size_t bw = assimpMesh->HasBones() ? sizeof(BoneWeight) : 0;
		
	NNEbool consistent = amesh->VFConsistencyTest(pos, norm, tang, clr, tc0, tc1, bw);
	debugAssert(true == consistent);

	NNEuint vertCount = assimpMesh->mNumVertices;

	NNEuint oldVertNum = amesh->m_va.getVertNum();

	amesh->m_va.m_vertices.reserve((vertCount + oldVertNum) * amesh->m_va.m_vertBytes);

	for (NNEuint i = 0; i < vertCount; ++i)
	{
		if (pos)
		{
			Vec3 tmp(assimpMesh->mVertices[i].x,
				assimpMesh->mVertices[i].y,
				assimpMesh->mVertices[i].z);

			amesh->m_va.m_vertices.push_back((NNEchar *)&tmp, sizeof(Vec3));
		}

		if (norm)
		{
			Vec3 tmp(assimpMesh->mNormals[i].x, 
				assimpMesh->mNormals[i].y,
				assimpMesh->mNormals[i].z);

			amesh->m_va.m_vertices.push_back((NNEchar *)&tmp, sizeof(Vec3));
		}

		if (tang)
		{
		}

		if (clr)
		{
			Vec4 tmp(assimpMesh->mColors[0]->r,
					assimpMesh->mColors[0]->g,
					assimpMesh->mColors[0]->b,
					assimpMesh->mColors[0]->a);

			amesh->m_va.m_vertices.push_back((NNEchar *)&tmp, sizeof(Vec4));
		}

		if (tc0)
		{
			Vec2 tmp(assimpMesh->mTextureCoords[0][i].x,
				    assimpMesh->mTextureCoords[0][i].y);

			amesh->m_va.m_vertices.push_back((NNEchar *)&tmp, sizeof(Vec2));
		}

		if (tc1)
		{
			Vec2 tmp(assimpMesh->mTextureCoords[1][i].x,
				    assimpMesh->mTextureCoords[1][i].y);

			amesh->m_va.m_vertices.push_back((NNEchar *)&tmp, sizeof(Vec2));
		}

		// initialize boneWeight attribute with 0
		if (bw)
		{
			BoneWeight tmp;
			amesh->m_va.m_vertices.push_back((NNEchar *)&tmp, sizeof(BoneWeight));
		}
	}

	// load bone weight
	if (bw)
	{
		// in assimp structure, each bone contains a array of aiVertexWeight which
		// contains the vertex index and the weight of the bone influence on the vertex just indexed
		Skin *askin = (Skin *)amesh;

		Skeleton *sklt = askin->getSkeleton();

		for (NNEuint b = 0; b < assimpMesh->mNumBones; ++b)
		{
			NNEuint boneId = 0;
			Sid boneName = HashString(assimpMesh->mBones[b]->mName.data);

			// offset matrix(inverse bind pose matrix)
			Mat4 offset;
			for (NNEuint c = 0; c < 4; ++c)
			{
				for (NNEuint r = 0; r < 4; ++r)
				{
					offset[c][r] = assimpMesh->mBones[b]->mOffsetMatrix[r][c];
				}
			}
		
			for (NNEuint i = 0; i < sklt->m_boneArray.size(); ++i)
			{
				if (boneName == sklt->m_boneArray[i].name)
				{
					// index starts from 1
					boneId = i + 1;
					sklt->m_boneArray[i].offset_transform = offset;
					break;
				}
			}

			aiVertexWeight *vertWeights = assimpMesh->mBones[b]->mWeights;
			NNEuint numVertWeights = assimpMesh->mBones[b]->mNumWeights;

			for (NNEuint vw = 0; vw < numVertWeights; ++vw)
			{
				BoneWeight *bonew = 
					askin->m_va.getBoneWeightPtr(vertWeights[vw].mVertexId + oldVertNum);

				if (0 == bonew->getBoneIndex(0))
				{
					bonew->setBoneIndex(0, boneId);
					bonew->w0 = vertWeights[vw].mWeight;
				}
				else if (0 == bonew->getBoneIndex(1))
				{
					bonew->setBoneIndex(1, boneId);
					bonew->w1 = vertWeights[vw].mWeight;
				}
				else if (0 == bonew->getBoneIndex(2))
				{
					bonew->setBoneIndex(2, boneId);
					bonew->w2 = vertWeights[vw].mWeight;
				}
				else if (0 == bonew->getBoneIndex(3))
				{
					bonew->setBoneIndex(3, boneId);
					bonew->w3 = vertWeights[vw].mWeight;
				}
			}
		}
	}

	if (assimpMesh->HasFaces())
	{
		int m_indicesNum = assimpMesh->mNumFaces * 3;
		amesh->m_va.m_triIndex.reserve(amesh->m_va.m_triIndex.size() + m_indicesNum);
		for (NNEuint i = 0; i < assimpMesh->mNumFaces; ++i)
		{
			amesh->m_va.m_triIndex.push_back(oldVertNum + assimpMesh->mFaces[i].mIndices[0]);
			amesh->m_va.m_triIndex.push_back(oldVertNum + assimpMesh->mFaces[i].mIndices[1]);
			amesh->m_va.m_triIndex.push_back(oldVertNum + assimpMesh->mFaces[i].mIndices[2]);
		}
	}

	return true;
}

NNEbool AssimpLoader::loadNodeMesh(const aiScene *scene, const aiNode *node, Mesh *amesh)
{
	NNEuint numMesh = node->mNumMeshes;

	size_t pos = scene->mMeshes[node->mMeshes[0]]->HasPositions() ? sizeof(Vec3) : 0;
	size_t norm = scene->mMeshes[node->mMeshes[0]]->HasNormals() ? sizeof(Vec3) : 0;
	size_t tang = scene->mMeshes[node->mMeshes[0]]->HasTangentsAndBitangents() ? sizeof(Vec4) : 0;
	size_t clr = scene->mMeshes[node->mMeshes[0]]->HasVertexColors(0) ? sizeof(Vec4) : 0;
	size_t tc0 = scene->mMeshes[node->mMeshes[0]]->HasTextureCoords(0) ? sizeof(Vec2) : 0;
	size_t tc1 = scene->mMeshes[node->mMeshes[0]]->HasTextureCoords(1) ? sizeof(Vec2) : 0;
	size_t bw = scene->mMeshes[node->mMeshes[0]]->HasBones() ? sizeof(BoneWeight) : 0;

	amesh->setVertFormat(pos, norm, tang, clr, tc0, tc1, bw);

	for (NNEuint i = 0; i < numMesh; ++i)
	{
		if (!loadAssimpMesh(scene->mMeshes[node->mMeshes[i]], amesh))
		{
			return false;
		}
	}

	return true;
}

aiNode *firstMeshNode(aiNode *node)
{
	if (node->mNumMeshes > 0)
	{
		return node;
	}
	else
	{
		if (node->mNumChildren > 0)
		{
			for (NNEuint i = 0; i < node->mNumChildren; ++i)
			{
				aiNode *result = firstMeshNode(node->mChildren[i]);
				if (result)
				{
					return result;
				}
			}

			// none of its children contains mesh
			return NULL;
		}
		else
		{
			return NULL;
		}
	}
}

NNEbool AssimpLoader::loadOneMesh(const char *filename, Mesh *amesh)
{
	// create an instance of importer class
	Assimp::Importer importer;  

	// all meshes loaded in scene are in right-handed coordinate
	// you can specify aiProcess_MakeLeftHanded though
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate 
													  | aiProcess_GenSmoothNormals);
	debugAssertMsg(scene, "can't load mesh data");

	aiNode *meshNode = firstMeshNode(scene->mRootNode);
	debugAssertMsg(meshNode, "none of nodes has mesh");

	NNEbool result = loadNodeMesh(scene, meshNode, amesh);
	return result;
}

NNEvoid AssimpLoader::buildSkeleton(aiNode *node, Skeleton *sklt, NNEint parentIndex)
{
	Bone bone;
	bone.parentIndex = parentIndex;

	// aiMatrix4x4 is row-major
	for (NNEuint c = 0; c < 4; ++c)
	{
		for (NNEuint r = 0; r < 4; ++r)
		{
			bone.local_transform[c][r] = node->mTransformation[r][c];
		}
	}

	bone.name = HashString(node->mName.data);

	sklt->m_boneArray.push_back(bone);

	if (0 == node->mNumChildren)
	{
		return ;
	}
	else
	{
		NNEint boneIndex = sklt->m_boneArray.size() - 1;
		for (NNEuint i = 0; i < node->mNumChildren; ++i)
		{
			buildSkeleton(node->mChildren[i], sklt, boneIndex);
		}
	}
}

NNEbool AssimpLoader::loadOneSkin(const char *filename, Skin *askin)
{
	Assimp::Importer tmpImporter;

	const aiScene *scene = tmpImporter.ReadFile(filename, aiProcess_Triangulate 
														 | aiProcess_GenSmoothNormals);
	debugAssertMsg(scene, "can't load mesh data");

	// the first node that contains meshes
	aiNode *meshNode = firstMeshNode(scene->mRootNode);

	// load skeleton
	// find bone root
	aiNode *boneRoot = scene->mRootNode->FindNode(scene->mMeshes[meshNode->mMeshes[0]]->mBones[0]->mName);
	while(true)
	{
		aiNode *parent = boneRoot->mParent;

		if (NULL == parent)
		{
			break;
		}
		else
		{
			// if parent node has mesh
			if (parent->mNumMeshes)
			{
				break;
			}

			// if parent node has a child node containing mesh
			NNEbool haschild = false;
			for (NNEuint i = 0; i < parent->mNumChildren; ++i)
			{
				if (parent->mChildren[i]->mNumMeshes)
				{
					haschild = true;
					break;
				}
			}
			if (haschild)
			{
				break;
			}

			boneRoot = boneRoot->mParent;
		}
	}
	// cut out the first node generated by Assimp
	boneRoot = boneRoot->mChildren[0];

	// compute number of bones
	NNEuint numBones = 0;
	countNodes(boneRoot, numBones);
	debugAssertMsg(numBones < Skeleton::MAX_BONE_NUM, "exceed the maximum number of bones");

	askin->m_skeleton.m_boneArray.reserve(numBones);

	// this makes sure the first bone in the bone array is the root
	buildSkeleton(boneRoot, &askin->m_skeleton, -1);

	// load mesh(vectices, normals, boneWeights, etc.)
	if (!loadNodeMesh(scene, meshNode, dynamic_cast<Mesh *>(askin)))
	{
		return false;
	}

	//for (NNEuint i = 0; i < meshNode->mNumMeshes; ++i)
	//{
	//	loadSkinWeightsAndOffsetMatrix(scene->mMeshes[meshNode->mMeshes[i]], askin);	
	//}

	return true;
}

NNEbool AssimpLoader::loadOneAnimation(const char *filename, Animation *anim)
{
	Assimp::Importer tmpImporter;

	const aiScene *scene = tmpImporter.ReadFile(filename, aiProcess_Triangulate/* | aiProcess_FlipUVs*/);
	debugAssertMsg(scene, "can't load mesh data");

	aiAnimation *aianim = scene->mAnimations[0];

	NNEuint numBones = aianim->mNumChannels;

	anim->m_animNodes.reserve(numBones);
	
	for (NNEuint i = 0; i < numBones; ++i)
	{
		aiNodeAnim *nodeAnim = aianim->mChannels[i];
		
		AnimNode *animnode = new AnimNode();

		anim->m_animNodes.push_back(animnode);

		animnode->m_name = HashString(nodeAnim->mNodeName.data);

		// ticks per second
		NNEdouble TPS = (0 == aianim->mTicksPerSecond) ? 1 : aianim->mTicksPerSecond;

		anim->m_timeStamp.reserve(nodeAnim->mNumRotationKeys);
		for (NNEuint f = 0; f < nodeAnim->mNumRotationKeys; ++f)
		{
			anim->m_timeStamp.push_back(nodeAnim->mRotationKeys[f].mTime
											/ TPS);
		}

		// set the time of the last key as animation duration
		anim->m_duration = anim->m_timeStamp[nodeAnim->mNumRotationKeys - 1];

		// take rotation as the main transform channel, set frames of scale and translation
		// the same as that of rotation
		animnode->m_uniScale.reserve(nodeAnim->mNumRotationKeys);
		animnode->m_rotation.reserve(nodeAnim->mNumRotationKeys);
		animnode->m_translation.reserve(nodeAnim->mNumRotationKeys);
		for (NNEuint f = 0; f < nodeAnim->mNumRotationKeys; ++f)
		{
			// only uniform scale
			NNEfloat s = 1.0f; 
			if (f < nodeAnim->mNumScalingKeys)
			{
				aiVector3D tmp = nodeAnim->mScalingKeys[f].mValue;

				/*alwaysAssertMsg(tmp.x == tmp.y && tmp.x == tmp.z, 
					"non-uniform scalue in animation isn't allowed");*/

				s = tmp.x;
			}

			// rotation
			aiQuaternion tmp = nodeAnim->mRotationKeys[f].mValue;
			Quaternion q(tmp.x, tmp.y, tmp.z, tmp.w);
			q.normalize();

			// translation
			Vec3 t(0.0f, 0.0f, 0.0f);
			if (f < nodeAnim->mNumPositionKeys)
			{
				aiVector3D tmp = nodeAnim->mPositionKeys[f].mValue;
				t = Vec3(tmp.x, tmp.y, tmp.z);
			}

			animnode->m_uniScale.push_back(s);
			animnode->m_rotation.push_back(q);
			animnode->m_translation.push_back(t);
		}
	}
		
	return true;
}

NNEvoid AssimpLoader::loadSkinWeightsAndOffsetMatrix(aiMesh *assimpMesh, Skin *askin)
{
	// in assimp structure, each bone contains a array of aiVertexWeight which
	// contains the vertex index and the weight of the bone influence on the vertex just indexed

	//NNEuint oldVertNum = askin->m_va.getVertNum();
	//NNEuint vertCount = assimpMesh->mNumVertices;

	//askin->m_va.m_boneWight.reserve(oldVertNum + vertCount);

	//// initialize all boneWeights
	//for (NNEuint bw = oldVertNum; bw < oldVertNum + vertCount; ++bw)
	//{
	//	askin->m_va.m_boneWight.push_back(BoneWeight());
	//}

	//Skeleton *sklt = askin->getSkeleton();

	//for (NNEuint b = 0; b < assimpMesh->mNumBones; ++b)
	//{
	//	NNEuint boneId = 0;
	//	Sid boneName = HashString(assimpMesh->mBones[b]->mName.data);

	//	// offset matrix(inverse bind pose matrix)
	//	Mat4 offset;
	//	for (NNEuint c = 0; c < 4; ++c)
	//	{
	//		for (NNEuint r = 0; r < 4; ++r)
	//		{
	//			offset[c][r] = assimpMesh->mBones[b]->mOffsetMatrix[r][c];
	//		}
	//	}
	//	
	//	for (NNEuint i = 0; i < sklt->m_boneArray.size(); ++i)
	//	{
	//		if (boneName == sklt->m_boneArray[i].name)
	//		{
	//			// index starts from 1
	//			boneId = i + 1;
	//			sklt->m_boneArray[i].offset_transform = offset;
	//			break;
	//		}
	//	}

	//	aiVertexWeight *vertWeights = assimpMesh->mBones[b]->mWeights;
	//	NNEuint numVertWeights = assimpMesh->mBones[b]->mNumWeights;

	//	for (NNEuint vw = 0; vw < numVertWeights; ++vw)
	//	{
	//		BoneWeight *bonew = &(askin->m_va.m_boneWight[vertWeights[vw].mVertexId + oldVertNum]);
	//		if (0 == bonew->getBoneIndex(0))
	//		{
	//			bonew->setBoneIndex(0, boneId);
	//			bonew->w0 = vertWeights[vw].mWeight;
	//		}
	//		else if (0 == bonew->getBoneIndex(1))
	//		{
	//			bonew->setBoneIndex(1, boneId);
	//			bonew->w1 = vertWeights[vw].mWeight;
	//		}
	//		else if (0 == bonew->getBoneIndex(2))
	//		{
	//			bonew->setBoneIndex(2, boneId);
	//			bonew->w2 = vertWeights[vw].mWeight;
	//		}
	//		else if (0 == bonew->getBoneIndex(3))
	//		{
	//			bonew->setBoneIndex(3, boneId);
	//			bonew->w3 = vertWeights[vw].mWeight;
	//		}
	//	}
	//}
}

NNEvoid AssimpLoader::countNodes(aiNode *node, NNEuint &counter)
{
	counter++;

	if (0 == node->mNumChildren)
	{
		return ;
	}
	else
	{
		for (NNEuint i = 0; i < node->mNumChildren; ++i)
		{
			countNodes(node->mChildren[i], counter);
		}
	}
}

}