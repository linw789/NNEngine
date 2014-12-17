#include "GPUProgram.h"
#include "DebugUtilities.h"
#include "Shader.h"
#include "Texture.h"
#include "String.h"
#include "Light.h"

namespace NNE{

GPUProgram::GPUProgram(Sid name, const NNEchar *comp)
	:m_name(name)
{
	m_id = createFromFile(comp);
}

GPUProgram::GPUProgram(Sid name, const NNEchar *vert, const NNEchar *frag)
	:m_name(name)
{
	// create shader program
	m_id = createFromFile(vert, frag);
}

GPUProgram::GPUProgram(Sid name, const NNEchar *vert, const NNEchar *geom, const NNEchar *frag)
	:m_name(name)
{
	m_id = createFromFile(vert, geom, frag);
}

NNEbool GPUProgram::init()
{
	// every vertex shader should contain a uniform block called "SceneUniform".
	GL_DEBUG(m_sceneMatUBIndex = glGetUniformBlockIndex(m_id, "SceneUniform"));

	debugOutputOnceIf(GPU_INVALID_INDEX == m_sceneMatUBIndex, "Invalid Index: m_sceneMatUBIndex");

	// Matrices might be inactive because they are not used in the shaders
	if (GPU_INVALID_INDEX != m_sceneMatUBIndex)
	{
		// bind to uniform binding point
		GL_DEBUG(glUniformBlockBinding(m_id, m_sceneMatUBIndex, SCENE_MATRICES));
	}

	m_uniformLocations.reserve(10);

	GPUint unifloc;

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATRIX_WORLD"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATRIX_PVW"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATRIX_NORMAL_W"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATERIAL.ka"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATERIAL.kd"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATERIAL.ks"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "MATERIAL.shininess"));
	m_uniformLocations.push_back(unifloc);

	GL_DEBUG(unifloc = glGetUniformLocation(m_id, "BONE_XFORM"));
	m_uniformLocations.push_back(unifloc);

	return true;
}

GPUuint GPUProgram::createFromFile(const NNEchar *comp)
{
	return Shader::fromFile(comp);
}

GPUuint GPUProgram::createFromFile(const NNEchar *vert, const NNEchar *frag)
{
	return Shader::fromFile(vert, frag);
}

GPUuint GPUProgram::createFromFile(const NNEchar *vert, const NNEchar *geom, const NNEchar *frag)
{
	return Shader::fromFile(vert, geom, frag);
}

NNEvoid GPUProgram::applyLight(const NNEchar* uniformName, Light *alight)
{
	GLint light_index;
	GL_DEBUG(light_index = glGetUniformBlockIndex(m_id, uniformName));

	debugOutputOnceIf(GPU_INVALID_INDEX == light_index, "Invalid uniform name: " << uniformName);

	if (GPU_INVALID_INDEX != light_index)
	{
		// bind to uniform binding point
		GL_DEBUG(glUniformBlockBinding(m_id, light_index, alight->getBindingPoint()));
	}
}

NNEbool GPUProgram::updateUniformMatrix3(NNEuint index, NNEuint count, NNEfloat *data)
{
	debugAssertMsg(index < m_uniformLocations.size(), "access out of bound");
	NNEint loc = m_uniformLocations[index];

	if (GPU_INVALID_INDEX == loc)
	{
		//debugOutput("invalid uniform location");
		return false;
	}

	GL_DEBUG(glUseProgram(m_id));
	GL_DEBUG(glUniformMatrix3fv(loc, count, false, data));
	// glProgramUniform is available for GL 4.1+
	//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));

	return true;
}

NNEbool GPUProgram::updateUniformMatrix4(NNEuint index, NNEuint count, NNEfloat *data)
{
	debugAssertMsg(index < m_uniformLocations.size(), "access out of bound");
	NNEint location = m_uniformLocations[index];

	if (GPU_INVALID_INDEX == location)
	{
		//debugOutput("invalid uniform location");
		return false;
	}

	GL_DEBUG(glUseProgram(m_id));
	GL_DEBUG(glUniformMatrix4fv(location, count, false, data));
	// glProgramUniform is available for GL 4.1+
	//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));

	return true;
}

NNEbool GPUProgram::updateUniformVec3(NNEuint index, NNEuint count, NNEfloat *data)
{
	debugAssertMsg(index < m_uniformLocations.size(), "access out of bound");
	NNEint location = m_uniformLocations[index];

	if (GPU_INVALID_INDEX == location)
	{
		//debugOutput("invalid uniform location");
		return false;
	}

	GL_DEBUG(glUseProgram(m_id));
	GL_DEBUG(glUniform3fv(location, count, data));

	return true;
}

NNEbool GPUProgram::updateUniformFloat(NNEuint index, NNEuint count, NNEfloat *data)
{
	debugAssertMsg(index < m_uniformLocations.size(), "access out of bound");
	NNEint loc = m_uniformLocations[index];

	if (GPU_INVALID_INDEX == loc)
	{
		//debugOutput("invalid uniform location");
		return false;
	}

	GL_DEBUG(glUseProgram(m_id));
	GL_DEBUG(glUniform1fv(loc, count, data));

	return true;
}

NNEint GPUProgram::setSamplerUniform(const char *name, Texture *tex)
{
	GPUint uniformLoc;
	GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));

	debugOutputOnceIf(GPU_INVALID_INDEX == uniformLoc, "Invalid sampler uniform :" << name);

	if (GPU_INVALID_INDEX != uniformLoc)
	{
		GL_DEBUG(glUseProgram(m_id));
		GL_DEBUG(glUniform1i(uniformLoc, tex->getUnitId()));
	}

	return uniformLoc;
}

NNEint GPUProgram::updateUniformMatrix3(const char *name, NNEuint count, NNEfloat *data)
{
	GPUint uniformLoc;
	GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));

	debugOutputOnceIf(GPU_INVALID_INDEX == uniformLoc, "Invalid uniform name " << name);

	if (GPU_INVALID_INDEX != uniformLoc)
	{
		GL_DEBUG(glUseProgram(m_id));
		GL_DEBUG(glUniformMatrix3fv(uniformLoc, count, false, data));
		// glProgramUniform is available for GL 4.1+
		//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));
	}

	return uniformLoc;
}

NNEint GPUProgram::updateUniformMatrix4(const char *name, NNEuint count, NNEfloat *data)
{
	GPUint uniformLoc;
	GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));

	debugOutputOnceIf(GPU_INVALID_INDEX == uniformLoc, "Invalid uniform name: " << name);

	if (GPU_INVALID_INDEX != uniformLoc)
	{
		GL_DEBUG(glUseProgram(m_id));
		GL_DEBUG(glUniformMatrix4fv(uniformLoc, count, false, data));
		// glProgramUniform is available for GL 4.1+
		//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));
	}

	return uniformLoc;
}

NNEint GPUProgram::updateUniformVec3(const char *name, NNEuint count, NNEfloat *data)
{
	GPUint uniformLoc;
	GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));

	debugOutputOnceIf(GPU_INVALID_INDEX == uniformLoc, "Invalid uniform name: " << name);

	if (GPU_INVALID_INDEX != uniformLoc)
	{
		GL_DEBUG(glUseProgram(m_id));
		GL_DEBUG(glUniform3fv(uniformLoc, count, data));
		// glProgramUniform is available for GL 4.1+
		//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));
	}

	return uniformLoc;
}

NNEint GPUProgram::updateUniformFloat(const char *name, NNEuint count, NNEfloat *data)
{
	GPUint uniformLoc;
	GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));

	debugOutputOnceIf(GPU_INVALID_INDEX == uniformLoc, "Invalid uniform name: " << name);

	if (GPU_INVALID_INDEX != uniformLoc)
	{
		GL_DEBUG(glUseProgram(m_id));
		GL_DEBUG(glUniform1fv(uniformLoc, count, data));
		// glProgramUniform is only available for GL 4.1+
		//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));
	}

	return uniformLoc;
}

NNEint GPUProgram::updateUniformInt(const char *name, NNEuint count, NNEint *data)
{
	GPUint uniformLoc;
	GL_DEBUG(uniformLoc = glGetUniformLocation(m_id, name));

	debugOutputOnceIf(GPU_INVALID_INDEX == uniformLoc, "Invalid uniform name: " << name);

	if (GPU_INVALID_INDEX != uniformLoc)
	{
		GL_DEBUG(glUseProgram(m_id));
		GL_DEBUG(glUniform1iv(uniformLoc, count, data));
		// glProgramUniform is only available for GL 4.1+
		//GL_DEBUG(glProgramUniformMatrix4fv(m_id, uniformLoc, 1, false, data));
	}

	return uniformLoc;
}

} // namespace NNE