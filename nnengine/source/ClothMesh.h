#ifndef NNE_CLOTHMESH_H
#define NNE_CLOTHMESH_H

#include "PlatformSpecs.h"
#include "Mesh.h"
#include "Array.h"
#include "Vec4.hpp"

namespace NNE{

class ClothMesh: public Mesh
{
	enum
	{
		POSITION_ATTRIB_BINDING_POINT = 0,
	};

public:
	
	ClothMesh(Sid name, GPUenum shape)
		:Mesh(name, shape), m_preBOIndex(0), m_curBOIndex(1), m_nextBOIndex(2)
	{}

	~ClothMesh()
	{}

	NNEvoid buildMesh(NNEuint w, NNEuint h, NNEfloat interval);

	NNEvoid update();

	NNEvoid draw();

	NNEvoid setComputeShader(GPUuint cs)
	{
		m_computeProg = cs;
	}

private:

	NNEuint m_width;
	NNEuint m_height;

	NNEfloat m_interval;

	Array<Vec4> m_prePos;

	Array<Vec4> m_curPos;

	Array<Vec4> m_nextPos;

	Array<Vec4> m_normal;

	Array<NNEuint> m_triIndex;
	
	GLuint m_posBO[3];

	NNEuint m_preBOIndex;
	NNEuint m_curBOIndex;
	NNEuint m_nextBOIndex;

	GLuint m_normalBO;

	GLuint m_indexBO;

	GLuint m_vao;

	GLuint m_computeProg;
};

} // manespace NNE

#endif // NNE_CLOTHMESH_H