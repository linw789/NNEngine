#include "Mesh.h"
#include "DebugUtilities.h"
#include "AssimpLoader.h"

namespace NNE{

NNEvoid Mesh::createVBO(GPUenum usageHint/* = GPU_WRITE_ONCE*/)
{
	// VBO has not been created
	if (0 == m_vertBufGPU.getID())
	{
		NNEuint verticesBytes = m_va.getVerticesBytes();

		if (GPU_WRITE_ONCE != usageHint)
		{
			m_vertBufGPU.create(GPU_VERTEX_BUFFER, verticesBytes, NULL, usageHint);
		}
		else
		{
			debugAssertMsg(verticesBytes != 0, "This vertex array doesn't contain vertex!");
			m_vertBufGPU.create(GPU_VERTEX_BUFFER, verticesBytes, m_va.getVerticesPtr(), GPU_WRITE_ONCE);
		}
	}
}

NNEvoid Mesh::createIndexBO(GPUenum usageHint/* = GPU_WRITE_ONCE*/)
{
	// VBO has not been created
	if (0 == m_indexBufGPU.getID())
	{
		NNEuint indexBytes = m_va.getIndexBytes();

		if (GPU_WRITE_ONCE != usageHint)
		{
			m_indexBufGPU.create(GPU_INDEX_BUFFER, indexBytes, NULL, usageHint);
		}
		else
		{
			debugAssertMsg(indexBytes != 0, "This vertex array doesn't contan index!");
			m_indexBufGPU.create(GPU_INDEX_BUFFER, indexBytes, m_va.getIndexPtr(),  GPU_WRITE_ONCE);
		}
	}
}

NNEvoid Mesh::upload2GPU(GPUenum usageHint/* = GPU_WRITE_ONCE*/)
{
	if (0 == m_VAO.getID())
	{
		// vertecies data are still in memory
		if (hasVertCPU())
		{
			if (GPU_WRITE_ONCE == usageHint)
			{
				createVBO(usageHint);
				if (m_va.hasIndex())
				{
					createIndexBO(usageHint);
					m_VAO.create(m_va, m_vertBufGPU, m_indexBufGPU);
				}
				else
				{
					m_VAO.create(m_va, m_vertBufGPU);
				}
			
			}
			// create dynamic VBO on GPU
			else
			{
				createVBO(usageHint);
				createIndexBO(usageHint);
				m_VAO.create(m_va, m_vertBufGPU, m_indexBufGPU);
			}
		}
		else // no vertices data in memory
		{
			//debugAssertMsg(false, "no vertices data available");
		}
	}
}

NNEvoid Mesh::draw()
{
	m_VAO.bind();
	
	if (getIndexCount() == 0) // no index
	{
		GL_DEBUG(glDrawArrays(m_primitiveType, 0, m_va.getVertNum()));
	}
	else
	{
		GL_DEBUG(glDrawElements(m_primitiveType, getIndexCount(), GPU_UINT, 0));
	}

	m_VAO.unbind();
}

NNEvoid Mesh::drawNormal()
{
	m_VAO.bind();
	GL_DEBUG(glDrawArrays(GL_POINTS, 0, m_va.getVertNum()));
	m_VAO.unbind();
}

AABB Mesh::computeAABB()
{
	Vec3 minVert, maxVert;
	minVert = *m_va.getPosPtr(0);
	maxVert = minVert;

	NNEuint vertNum = m_va.getVertNum();
	Vec3 tmp;
	for (NNEuint i = 1; i < vertNum; ++i)
	{
		tmp = *m_va.getPosPtr(i);
		if (tmp.x < minVert.x)
		{
			minVert.x = tmp.x;
		}
		else if (tmp.x > maxVert.x)
		{
			maxVert.x = tmp.x;
		}
		if (tmp.y < minVert.y)
		{
			minVert.y = tmp.y;
		}
		else if (tmp.y > maxVert.y)
		{
			maxVert.y = tmp.y;
		}
		if (tmp.z < minVert.z)
		{
			minVert.z = tmp.z;
		}
		else if (tmp.z > maxVert.z)
		{
			maxVert.z = tmp.z;
		}
	}

	return AABB(minVert, maxVert);
}

} // namespace NNE