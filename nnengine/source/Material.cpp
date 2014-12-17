#include "Material.h"
#include "GPUProgram.h"
#include "Texture.h"
#include "GPUProgram.h"

namespace NNE{

NNEvoid Material::updateMaterialUniforms()
{
	debugAssertMsg(NULL != m_GPUprog, "no GPU program attached");

	m_GPUprog->updateUniformVec3(GPUProgram::UNIFORM_LOC_MTRL_KA, 1, (NNEfloat *)&m_ambient);
	m_GPUprog->updateUniformVec3(GPUProgram::UNIFORM_LOC_MTRL_KD, 1, (NNEfloat *)&m_diffuse);
	m_GPUprog->updateUniformVec3(GPUProgram::UNIFORM_LOC_MTRL_KS, 1, (NNEfloat *)&m_specular);
	m_GPUprog->updateUniformFloat(GPUProgram::UNIFORM_LOC_MTRL_SHININESS, 1, &m_shininess);
}

NNEvoid Material::apply()
{
	if (m_texture != NULL)
	{
		m_texture->bind();
	}
	
	if (m_GPUprog != NULL)
	{
		updateMaterialUniforms();
		m_GPUprog->apply();
	}
}

} // namespace NNE