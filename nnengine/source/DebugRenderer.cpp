#include "DebugRenderer.h"
#include "GPUProgram.h"
#include "Resource.h"
#include "Font.h"

namespace NNE{

NNEvoid DebugRenderer::init()
{
#if defined(NNE_DEBUG)

	// initialize debug line data
	memset(m_duration, 0, sizeof(NNEint) * MAX_LINE_NUM);
	memset(m_lineVert, 0, sizeof(Vec3) * 2 * MAX_LINE_NUM);
	memset(m_color, 0, sizeof(Color4) * MAX_LINE_NUM);

	// initialize dubug point data
	memset(m_pointDuration, 0, sizeof(NNEint) * MAX_POINT_NUM);
	memset(m_pointPos, 0, sizeof(Vec3) * MAX_POINT_NUM);
	memset(m_pointColor, 0, sizeof(Color4) * MAX_POINT_NUM);
	
	// initialize debug text data
	memset(m_text, 0, sizeof(DebugText) * MAX_CHARACTER_NUM);

	// don't even need to specify the base mesh data, which will be modified by 
	// geometry shader to construct line mesh.
	m_baseBuf.create(GL_ARRAY_BUFFER, sizeof(Vec3), NULL, GL_STATIC_DRAW);

	m_lineBuf.create(GL_ARRAY_BUFFER, sizeof(Vec3) * 2 * MAX_LINE_NUM, NULL, GL_DYNAMIC_DRAW);
	m_lineBuf.updateSubData(0, sizeof(Vec3) * 2 * MAX_LINE_NUM, (GLvoid *)m_lineVert);

	m_colorBuf.create(GL_ARRAY_BUFFER, sizeof(Color4) * MAX_LINE_NUM, NULL, GL_DYNAMIC_DRAW);
	m_colorBuf.updateSubData(0, sizeof(Color4) * MAX_LINE_NUM, (GLvoid *)m_color);

	m_vao.create();
	// At first I tried to store all line meshes in to position attribute, 
	// however this way openGL sees all line as one mesh data and will draw
	// all the lines MAX_LINE_NUM times.
	m_vao.specifyAttrib(NNE_POSITION_ATTRIB, 3, 0, 0, m_baseBuf);

	m_vao.specifyAttrib(LINE_START_POINT_ATTRIB_INDEX, 3, sizeof(Vec3) * 2, (GLvoid *)0, m_lineBuf);
	m_vao.setInstancingRate(LINE_START_POINT_ATTRIB_INDEX, 1);

	m_vao.specifyAttrib(LINE_END_POINT_ATTRIB_INDEX, 3, sizeof(Vec3) * 2, (GLvoid *)(sizeof(Vec3)), m_lineBuf);
	m_vao.setInstancingRate(LINE_END_POINT_ATTRIB_INDEX, 1);

	m_vao.specifyAttrib(NNE_COLOR_ATTRIB, 4, 0, (GLvoid *)0, m_colorBuf);
	m_vao.setInstancingRate(NNE_COLOR_ATTRIB, 1);

	// create debug point vertec array object
	m_pointBaseBuf.create(GL_ARRAY_BUFFER, sizeof(Vec3), NULL, GL_STATIC_DRAW);

	m_pointPosBuf.create(GL_ARRAY_BUFFER, sizeof(Vec3) * MAX_POINT_NUM, NULL, GL_DYNAMIC_DRAW);
	m_pointPosBuf.updateSubData(0, sizeof(Vec3) * MAX_POINT_NUM, (GLvoid *)m_pointPos);

	m_pointColorBuf.create(GL_ARRAY_BUFFER, sizeof(Color4) * MAX_POINT_NUM, NULL, GL_DYNAMIC_DRAW);
	m_pointColorBuf.updateSubData(0, sizeof(Color4) * MAX_POINT_NUM, (GLvoid *)m_pointColor);

	m_pointVAO.create();
	m_pointVAO.specifyAttrib(NNE_POSITION_ATTRIB, 3, 0, (GLvoid *)0, m_pointBaseBuf);

	m_pointVAO.specifyAttrib(POINT_POS_ATTRIB_INDEX, 3, 0, 0, m_pointPosBuf);
	m_pointVAO.setInstancingRate(POINT_POS_ATTRIB_INDEX, 1);

	m_pointVAO.specifyAttrib(NNE_COLOR_ATTRIB, 4, 0, (GLvoid *)0, m_pointColorBuf);
	m_pointVAO.setInstancingRate(NNE_COLOR_ATTRIB, 1);

	// create debug text vertex array object
	m_textVAO.create();

	m_textBuf.create(GL_ARRAY_BUFFER, sizeof(DebugText) * MAX_CHARACTER_NUM, NULL, GL_DYNAMIC_DRAW);
	m_textBuf.updateSubData(0, sizeof(DebugText) * MAX_CHARACTER_NUM, (GPUvoid *)m_text);

	m_textVAO.specifyAttrib(NNE_POSITION_ATTRIB, 3, 0, 0, m_baseBuf);

	m_textVAO.specifyAttrib(DEBUG_TEXT_POS_ATTRIB_INDEX, 2, sizeof(DebugText), (GLvoid *)0, m_textBuf);
	m_textVAO.setInstancingRate(DEBUG_TEXT_POS_ATTRIB_INDEX, 1);

	m_textVAO.specifyAttrib(DEBUG_TEXT_TEX_LT_ATTRIB_INDEX, 2, sizeof(DebugText), (GLvoid *)(sizeof(Vec2)), m_textBuf);
	m_textVAO.setInstancingRate(DEBUG_TEXT_TEX_LT_ATTRIB_INDEX, 1);

	m_textVAO.specifyAttrib(DEBUG_TEXT_TEX_RB_ATTRIB_INDEX, 2, sizeof(DebugText), (GLvoid *)(sizeof(Vec2) * 2), m_textBuf);
	m_textVAO.setInstancingRate(DEBUG_TEXT_TEX_RB_ATTRIB_INDEX, 1);

	m_textVAO.specifyAttrib(NNE_COLOR_ATTRIB, 2, sizeof(DebugText), (GLvoid *)(sizeof(Vec2) * 3), m_textBuf);
	m_textVAO.setInstancingRate(NNE_COLOR_ATTRIB, 1);

#endif

}

NNEvoid DebugRenderer::drawLine(Vec3 start, Vec3 end, Color3 clr, NNEint duration)
{
#if defined(NNE_DEBUG)

	for (NNEint i = 0; i < MAX_LINE_NUM; ++i)
	{
		// this line has expired, we can reuse it
		if (0 == m_duration[i])
		{
			m_duration[i] = duration;
			m_color[i] = Color4(clr, 1.0f); // make sure it will be drawn
			m_lineVert[i * 2 + 0] = start;
			m_lineVert[i * 2 + 1] = end;

			m_lineBuf.updateSubData(sizeof(Vec3) * 2 * i, sizeof(Vec3) * 2, (GLvoid *)&m_lineVert[i * 2]);
			m_colorBuf.updateSubData(sizeof(Color4) * i, sizeof(Color4), (GLvoid *)&m_color[i]);

			return;
		}
	}
	debugAssertMsg(false, "exceeding the maximum number of lines."); 

#endif
}

NNEvoid DebugRenderer::drawPoint(Vec3 pos, Color3 clr, NNEint duration)
{
#if defined(NNE_DEBUG)

	for (NNEint i = 0; i < MAX_LINE_NUM; ++i)
	{
		// this point has expired, we can reuse it
		if (0 == m_pointDuration[i])
		{
			m_pointDuration[i] = duration;
			m_pointColor[i] = Color4(clr, 1.0f); // make sure it will be drawn
			m_pointPos[i] = pos;

			m_pointPosBuf.updateSubData(sizeof(Vec3) * i, sizeof(Vec3), (GLvoid *)&m_pointPos[i]);
			m_pointColorBuf.updateSubData(sizeof(Color4) * i, sizeof(Color4), (GLvoid *)&m_pointColor[i]);

			return;
		}
	}
	debugAssertMsg(false, "exceeding the number of points."); 

#endif
}

NNEvoid DebugRenderer::drawText(const NNEchar *str, NNEfloat size, Vec2 pos, Color3 clr)
{
#if defined(NNE_DEBUG)

	Font *dfont = gResourceMngr.getDefaultFont();
	NNEuint strlength = strlen(str);
	NNEuint j = 0;
	for (NNEuint i = 0; i < strlength; ++i)
	{
		for (; j < MAX_CHARACTER_NUM; ++j)
		{
			if (0.0f == m_text[j].color.a)
			{
				m_text[j].color = Color4(clr, 1.0f); // make sure it will be drawn
				m_text[j].pos = pos;
				NNEfloat ltx, lty, rbx, rby;
				dfont->getTexcoord(str[i], &ltx, &lty, &rbx, &rby);
				m_text[j].lt = Vec2(ltx, lty);
				m_text[j].rb = Vec2(rbx, rby);
				m_textBuf.updateSubData(sizeof(DebugText) * j, sizeof(DebugText), (GLvoid *)&m_text[j]);
				break;
			}
		}
		debugAssertMsg(false, "exceeding the limited number of line could drawn."); 
		j++;
	}

#endif
}

NNEvoid DebugRenderer::update()
{
#if defined(NNE_DEBUG)

	for (NNEint i = 0; i < MAX_LINE_NUM; ++i)
	{
		if (m_duration[i] > 0)
		{
			m_duration[i]--;
			if (0 == m_duration[i])
			{
				m_color[i].a = 0.0f;
				m_colorBuf.updateSubData(sizeof(Color4) * i, sizeof(Color4), (GLvoid *)&m_color[i]);
			}
		}
		else // m_duration[i] == -1.0f
		{
			// do nothing
		}
	}

	for (NNEint i = 0; i < MAX_POINT_NUM; ++i)
	{
		if (m_pointDuration[i] > 0)
		{
			m_pointDuration[i]--;
			if (0 == m_pointDuration[i])
			{
				m_pointColor[i].a = 0.0f;
				m_pointColorBuf.updateSubData(sizeof(Color4) * i, sizeof(Color4), (GLvoid *)&m_pointColor[i]);
			}
		}
	}

#endif

}

NNEvoid DebugRenderer::render()
{

#if defined(NNE_DEBUG)

	update();

	gResourceMngr.findGPUProgram(Resource::DEBUG_DRAW_LINE_GPUP)->apply();
	m_vao.bind();
	GL_DEBUG(glDrawArraysInstanced(GL_POINTS, 0, 1, MAX_LINE_NUM));
	m_vao.unbind();

	glDisable(GL_DEPTH_TEST);

	gResourceMngr.findGPUProgram(Resource::DEBUG_DRAW_POINT_GPUP)->apply();
	m_pointVAO.bind();
	GL_DEBUG(glDrawArraysInstanced(GL_POINTS, 0, 1, MAX_POINT_NUM));
	m_pointVAO.unbind();

	glEnable(GL_DEPTH_TEST);

	// render text


	// zero all text data
	memset(m_text, 0, MAX_CHARACTER_NUM * sizeof(DebugText));
#endif

}

DebugRenderer gDebugRenderer;

} // namespace NNE