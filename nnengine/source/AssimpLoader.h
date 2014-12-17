#ifndef NNE_ASSIMPLOADER_H
#define NNE_ASSIMPLOADER_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Skeleton.h"

#include <assimp/Importer.hpp>  // c++ importer interface
#include <assimp/scene.h>       // output data structure
#include <assimp/postprocess.h> // post processing flags

namespace NNE{

class Mesh;
class Skin;
class Animation;

class AssimpLoader
{
public:

	// load the first node that has meshes
	// many meshes may be attached to the same node, consider them as one mesh
	static NNEbool loadOneMesh(const char *filename, Mesh *);

	static NNEbool loadOneSkin(const char *filename, Skin *);

	static NNEbool loadOneAnimation(const char *filename, Animation *);

private:

	// load one assimp mesh in to NNE::Mesh
	// note: NNE::Mesh could have already be populated with data, the assimp mesh 
	// to be added should share the same aiNode with ones already loaded into the NNE::Mesh
	static NNEbool loadAssimpMesh(const aiMesh *assimpMesh, Mesh * amesh);

	// load all meshes of the node as one NNE::Mesh
	static NNEbool loadNodeMesh(const aiScene *, const aiNode *node, Mesh *amesh);

	static NNEvoid buildSkeleton(aiNode *node, Skeleton *sklt, NNEint parentIndex);

	static NNEvoid loadSkinWeightsAndOffsetMatrix(aiMesh *assimpMesh, Skin *askin);

	static NNEvoid countNodes(aiNode *node, NNEuint &counter);
};

} // manespace NNE

#endif // NNE_ASSIMPLOADER_H