#ifndef NNE_DEBUGRENDERER_H
#define NNE_DEBUGRENDERER_H

#include "PlatformSpecs.h"
#include "Vec3.hpp"
#include "Color.hpp"
#include "BufferObjectGPU.h"
#include "VertexArrayObject.h"
#include "Mesh.h"

namespace NNE{

class GPUProgram;

class DebugRenderer
{
	enum
	{
		LINE_START_POINT_ATTRIB_INDEX = 6,
		LINE_END_POINT_ATTRIB_INDEX = 7,
		MAX_LINE_NUM = 256,

		POINT_POS_ATTRIB_INDEX = 6,
		MAX_POINT_NUM = 256,

		DEBUG_TEXT_POS_ATTRIB_INDEX = 6,
		DEBUG_TEXT_TEX_LT_ATTRIB_INDEX = 7,
		DEBUG_TEXT_TEX_RB_ATTRIB_INDEX = 8,
		MAX_CHARACTER_NUM = 256,
	};

public:

	struct DebugText
	{
		Vec2 pos;
		// font texture coordinate
		Vec2 lt; // left top
		Vec2 rb; // riht bottom
		
		Color4 color;
	};
	
	DebugRenderer()
	{
	}

	~DebugRenderer()
	{}

	NNEvoid init();

	// duration is measured in number of frames, -1.0f means infinite duration
	NNEvoid drawLine(Vec3 start, Vec3 end, Color3 clr, NNEint duration);

	NNEvoid drawPoint(Vec3 pos, Color3 clr, NNEint duration);

	NNEvoid drawText(const NNEchar *str, NNEfloat size, Vec2 pos, Color3 clr);

	NNEvoid update();

	NNEvoid render();

private:

#if defined(NNE_DEBUG)

	NNEint m_duration[MAX_LINE_NUM];

	Vec3 m_lineVert[2 * MAX_LINE_NUM];

	// each line appears one solid color, instead of one color per vertex
	// also the last component indicates whether the line will be drawn, 1.0f means yes
	// 0.0f means no.
	// using instanced rendering to save memory
	Color4 m_color[MAX_LINE_NUM];

	// only store a point as the base mesh used for instanced rendering
	BufferObjectGPU m_baseBuf;

	// actual line mesh data
	BufferObjectGPU m_lineBuf;

	BufferObjectGPU m_colorBuf;

	VertexArrayObject m_vao;

	// point data
	NNEint m_pointDuration[MAX_POINT_NUM];

	Vec3 m_pointPos[MAX_POINT_NUM];

	Color4 m_pointColor[MAX_POINT_NUM];

	BufferObjectGPU m_pointBaseBuf;

	// actual point mesh data
	BufferObjectGPU m_pointPosBuf;

	BufferObjectGPU m_pointColorBuf;

	VertexArrayObject m_pointVAO;

	// text data
	DebugText m_text[MAX_CHARACTER_NUM];

	BufferObjectGPU m_textBuf;

	VertexArrayObject m_textVAO;

#endif
};

extern DebugRenderer gDebugRenderer;

} // manespace NNE

#endif // NNE_DEBUGRENDERER_H