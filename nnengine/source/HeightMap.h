#ifndef NNE_HEIGHTMAP_H
#define NNE_HEIGHTMAP_H

#include "PlatformSpecs.h"
#include "String.h"

namespace NNE{

class Mesh;

class HeightMap
{
public:
	
	HeightMap()
	{}

	~HeightMap()
	{}

	static NNEvoid toMesh(const NNEchar *filename, Mesh *, NNEfloat interval = 1.0f);

private:


};

} // manespace NNE

#endif // NNE_HEIGHTMAP_H