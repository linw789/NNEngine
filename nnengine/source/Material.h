#ifndef NNE_MATERIAL_H
#define NNE_MATERIAL_H

#include "PlatformSpecs.h"
#include "Array.h"
#include "Vec4.hpp"
#include "Vec3.hpp"
#include "GPUProgram.h"
#include "String.h"

namespace NNE{

class GPUProgram;
class Texture;

/*
	default material class
*/
class Material
{

public:

	Material(Sid name)
		:m_name(name),
		m_ambient(0.6f, 0.6f, 0.6f),
		m_diffuse(0.8f, 0.8f, 0.8f),
		m_specular(0.0f, 0.0f, 0.0f),
		m_emissive(0.0f, 0.0f, 0.0f),
		m_shininess(0.0f),
		m_texture(NULL),
		m_GPUprog(NULL)
	{}

	~Material()
	{}

	Sid getNameSid()
	{
		return m_name;
	}

	NNEvoid setAmbient(const Vec3 &amb)
	{
		m_ambient = amb;
	}

	NNEvoid setDiffuse(const Vec3 &dif)
	{
		m_diffuse = dif;
	}

	NNEvoid updateMaterialUniforms();

	NNEvoid setGPUProgram(GPUProgram *s)
	{
		m_GPUprog = s;

		updateMaterialUniforms();
	}

	GPUProgram *getGPUProg()
	{
		return m_GPUprog;
	}

	NNEvoid setTexture(Texture *tex)
	{
		m_texture = tex;
	}

	NNEvoid apply();

	/*NNEvoid update*/

private:

	Sid m_name;

    Vec3 m_ambient;
	NNEint m_ambLoc;

	Vec3 m_diffuse;
	NNEint m_difLoc;

	Vec3 m_specular;
	NNEint m_speLoc;

	Vec3 m_emissive;
	NNEint m_emiLoc;

	NNEfloat m_shininess;
	NNEint m_shiLoc;

	GPUProgram *m_GPUprog;

	Texture *m_texture;
};

} // manespace NNE

#endif // NNE_MATERIAL_H