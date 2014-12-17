#ifndef NNE_TEXT_H
#define NNE_TEXT_H

#include "PlatformSpecs.h"
#include "Array.h"
#include "BufferObjectGPU.h"
#include "VertexArrayObject.h"

namespace NNE{

class Text
{
public:
	
	Text()
	{}

	~Text()
	{}

private:
	
	// text string
	Array<NNEchar> m_textStr;

	BufferObjectGPU m_bo;

	VertexArrayObject m_vao;
};

} // manespace NNE

#endif // NNE_TEXT_H