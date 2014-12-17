#ifndef NNE_GPU_PROGRAM_H
#define NNE_GPU_PROGRAM_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Array.h"
#include "String.h"

namespace NNE{

class Texture;
class Light;

/*
	A wrapper for gpu shader program, like Effect in DirectX lingo.
*/
class GPUProgram
{
public:
	
	enum
	{
		UNIFORM_LOC_MATRIX_WORLD = 0,
		UNIFORM_LOC_PVW = 1,
		UNIFORM_LOC_MATRIX_NORMAL_W = 2,
		UNIFORM_LOC_MTRL_KA = 3,
		UNIFORM_LOC_MTRL_KD = 4,
		UNIFORM_LOC_MTRL_KS = 5,
		UNIFORM_LOC_MTRL_SHININESS = 6,
		UNIFORM_LOC_BONE_XFORM = 7,
	};

	GPUProgram()
		:m_id(0)
	{}

	GPUProgram(Sid name, const NNEchar *comp);

	GPUProgram(Sid name, const NNEchar *vert, const NNEchar *frag);

	GPUProgram(Sid name, const NNEchar *vert, const NNEchar *geom, const NNEchar *frag);

	~GPUProgram()
	{}

	Sid getNameSid()
	{
		return m_name;
	}

	GPUuint getProgramId()
	{
		return m_id;
	}

	NNEbool init();

	GPUuint createFromFile(const NNEchar *comp);

	// create shader program from vertex and fragment shader files
	GPUuint createFromFile(const NNEchar *vert, const NNEchar *frag);

	// create shader program from geometry, vertex and fragment shader files
	GPUuint createFromFile(const NNEchar *vert, const NNEchar *geom, const NNEchar *frag);

	NNEvoid apply()
	{
		GL_DEBUG(glUseProgram(m_id));
	}

	NNEvoid unapply()
	{
		GL_DEBUG(glUseProgram(0));
	}

	NNEvoid applyLight(const char* uniformName, Light *);

	GPUint getUniformLocation(const char *name)
	{
		GPUint uniformLoc;
		GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));
		return uniformLoc;
	}

	// the location of the uniform is known 
	// and is stored in the member variable m_uniformLocations
	// caution: index is not the actual uniform location, it's the 
	// index in m_uniformlocations which stores all actual uniform locations
	NNEbool updateUniformMatrix3(NNEuint index, NNEuint count, NNEfloat *data);

	NNEbool updateUniformMatrix4(NNEuint index, NNEuint count, NNEfloat *data);

	NNEbool updateUniformVec3(NNEuint index, NNEuint count, NNEfloat *data);

	NNEbool updateUniformFloat(NNEuint index, NNEuint count, NNEfloat *data);

	// return location of the uniform
	NNEint setSamplerUniform(const char *name, Texture *);

	NNEint updateUniformMatrix3(const char *name, NNEuint count, NNEfloat *data);

	NNEint updateUniformMatrix4(const char *name, NNEuint count, NNEfloat *data);

	NNEint updateUniformVec3(const char *name, NNEuint count, NNEfloat *data);

	NNEint updateUniformFloat(const char *name, NNEuint count, NNEfloat *data);

	NNEint updateUniformInt(const char *name, NNEuint count, NNEint *data);



private:

	Sid m_name;

	// shader program id
	GPUuint m_id;

	// built-in per scene matrices uniform block index
	GPUint m_sceneMatUBIndex;

	// array of custom uniform buffer index
	Array<GPUint> m_userMatUBIndices;

	Array<GPUint> m_uniformLocations;
};

} // manespace NNE

#endif // NNE_GPU_PROGRAM_H