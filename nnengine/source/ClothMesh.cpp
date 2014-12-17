#include "ClothMesh.h"

namespace NNE{

NNEvoid ClothMesh::buildMesh(NNEuint w, NNEuint h, NNEfloat interval)
{
	m_width = w;
	m_height = h;
	m_interval = interval;

	m_prePos.reserve(m_width * m_height);
	m_curPos.reserve(m_width * m_height);
	m_nextPos.reserve(m_width * m_height);
	m_normal.reserve(m_width * m_height);

	NNEfloat xs = -((w - 1) / 2.0f);
	NNEfloat zs = -((h - 1) / 2.0f);
	for (NNEuint i = 0; i < m_width; ++i) // x axis
	{
		for (NNEuint j = 0; j < m_height; ++j) // z axis
		{
			//NNEuint index = (i * m_width + j); 

			m_prePos.push_back(Vec4(xs + i, 0, zs + j, 0));

			m_curPos.push_back(Vec4(xs + i, 0, zs + j, 0));

			m_nextPos.push_back(Vec4(xs + i, 0, zs + j, 0));

			m_normal.push_back(Vec4(0, 1, 0, 0));
		}
	}

	m_triIndex.reserve((m_width - 1) * (m_height - 1) * 2 * 3);
	for (NNEuint i = 0; i < m_width - 1; ++i)
	{
		for (NNEuint j = 0; j < m_height - 1; ++j)
		{
			NNEuint ul = i * m_width + j; // upper left
			NNEuint bl = ul + 1;
			NNEuint br = (i + 1) * m_width + j + 1;
			NNEuint ur = br - 1;
			m_triIndex.push_back(ul);
			m_triIndex.push_back(bl);
			m_triIndex.push_back(ur);
			m_triIndex.push_back(bl);
			m_triIndex.push_back(br);
			m_triIndex.push_back(ur);
		}
	}

	// create vertex  buffer object
	glGenBuffers(3, m_posBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_posBO[0]);
	// 4 floats is because alignment
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_prePos.getBytes(), m_prePos.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_posBO[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_curPos.getBytes(), m_curPos.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_posBO[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_nextPos.getBytes(), m_nextPos.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_normalBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBO);
	glBufferData(GL_ARRAY_BUFFER, m_normal.getBytes(), m_normal.data(), GL_DYNAMIC_DRAW);

	// create index buffer object
	glGenBuffers(1, &m_indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triIndex.getBytes(), m_triIndex.data(), GL_STATIC_DRAW);

	// create vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// position attribute
	glVertexAttribFormat(0,        // attribute index used in shaders
						 3,		   // number of elements per vertex
						 GL_FLOAT, // type of vertex data
						 GL_FALSE, // whether be normalized
						 0);	   // offset in vertex array
	glEnableVertexAttribArray(0);
	glVertexAttribBinding(0, POSITION_ATTRIB_BINDING_POINT);

	//glBindBuffer(GL_ARRAY_BUFFER, m_posBO[m_curBOIndex]);
	//// position attribute
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_normalBO);
	// normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(NNEfloat), 0);

	// index buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBO);

	glBindVertexArray(0);
}

NNEvoid ClothMesh::update()
{
	glUseProgram(m_computeProg);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_posBO[m_preBOIndex]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_posBO[m_curBOIndex]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_posBO[m_nextBOIndex]);

	glDispatchCompute(m_width, 1, m_height);

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

NNEvoid ClothMesh::draw()
{
	glBindVertexArray(m_vao);

	glBindVertexBuffer(POSITION_ATTRIB_BINDING_POINT, m_posBO[m_nextBOIndex], 0, 4 * sizeof(NNEfloat));

	glDisable(GL_CULL_FACE);

	glDrawElements(GL_TRIANGLES, m_triIndex.size(), GL_UNSIGNED_INT, 0);

	NNEuint tmp = m_preBOIndex;
	m_preBOIndex = m_curBOIndex;
	m_curBOIndex = m_nextBOIndex;
	m_nextBOIndex = tmp;

	glEnable(GL_CULL_FACE);

	update();
}

} // namespace NNE