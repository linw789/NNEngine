#include "Image.h"

#include <fstream>

namespace NNE{

/* 
	Memory mapped file could improve performance.
	Using C I/O instead of iostream might be faster.
*/
NNEbool Image::write2PPM(const NNEchar *filename, NNEuint flag/* = INVERT_Y*/)
{
	std::ofstream outfile;
	outfile.open(filename, std::ofstream::binary | std::ofstream::trunc);
	if (outfile.is_open())
	{
		//write general file header
		outfile << "P6" << '\n'; // "P6" binary data, "P3" ASCII data
		outfile << m_width << '\n';
		outfile << m_height << '\n';
		//exam how many byte each component of every RGB color contains
		if(1 == m_componentSize)
			outfile << 255 << '\n';
		else if (2 == m_componentSize)
			outfile << 65535 << '\n';

		if (INVERT_Y == flag)
		{
			// byte size of one width of the image
			size_t widthByte = m_width * m_numComponent * m_componentSize;

			NNEchar *tmpData = (NNEchar *)::malloc(widthByte * m_height);

			for (NNEuint i = 0; i < m_height; ++i)
			{
				memcpy(tmpData + i * widthByte, m_buffer + (m_height - 1 - i) * widthByte, widthByte);
			}

			outfile.write(tmpData, m_width * m_height * m_numComponent * m_componentSize);

			::free(tmpData);
		}
		else
		{
			outfile.write(m_buffer, m_width * m_height * m_numComponent * m_componentSize);
		}
		

		outfile.close();
	}

	return true;
}

} // namespace NNE