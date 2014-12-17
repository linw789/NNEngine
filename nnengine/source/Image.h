#ifndef NNE_IMAGE_H
#define NNE_IMAGE_H

#include "PlatformSpecs.h"
#include "String.h"

namespace NNE{

class Image
{

public:

	enum
	{
		RGB,
		RGBA,

		// component data type
		CHAR,
		FLOAT,

		INVERT_Y,
	};
	
	Image()
		:m_width(0), m_height(0), m_buffer(NULL)
	{}

	Image(NNEuint w, NNEuint h, NNEuint format, NNEuint datatype)
		:m_width(w), m_height(h), m_format(format), m_numComponent(0)
	{
		switch(format)
		{
		case RGB:
			m_numComponent = 3;
			break;
		case RGBA:
			m_numComponent = 4;
			break;
		default:
			m_numComponent = 3;
		}

		switch(datatype)
		{
		case CHAR:
			m_componentSize = 1;
			break;
		case FLOAT:
			m_componentSize = 4;
			break;
		default:
			m_componentSize = 1;
		}

		m_buffer = (NNEchar *)::malloc(m_width * m_height * m_numComponent * m_componentSize);
	}

	~Image()
	{}

	NNEvoid init(NNEuint w, NNEuint h, NNEuint format, NNEuint datatype)
	{
		m_width = w;
		m_height = h;
		m_format = format;

		switch(format)
		{
		case RGB:
			m_numComponent = 3;
			break;
		case RGBA:
			m_numComponent = 4;
			break;
		default:
			m_numComponent = 3;
		}

		switch(datatype)
		{
		case CHAR:
			m_componentSize = 1;
			break;
		case FLOAT:
			m_componentSize = 4;
			break;
		default:
			m_componentSize = 1;
		}

		m_buffer = (NNEchar *)::malloc(m_width * m_height * m_numComponent * m_componentSize);
	}

	NNEchar *getDataBuffer()
	{
		return m_buffer;
	}

	// datasize: the size in byte of each component of one pixel
	NNEbool write2PPM(const NNEchar *filename, NNEuint flag = NNE_INVERT_Y);

private:

	NNEuint m_width;

	NNEuint m_height;

	NNEuint m_format;

	// number of element for one pixel
	NNEuint m_numComponent;

	// the size of each element of RGB
	size_t  m_componentSize;

	NNEchar *m_buffer;
};

} // manespace NNE

#endif // NNE_IMAGE_H