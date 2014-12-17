#include "Font.h"
#include <fstream>

namespace NNE{

NNEvoid Font::loadFontInfo(const NNEchar *filename)
{
	std::ifstream input;
	input.open(filename, std::ifstream::in);
	if (input.is_open())
	{
		NNEchar tmpBuffer[256];
		// ignore the first 2 lines
		input.getline(tmpBuffer, 256);
		input.getline(tmpBuffer, 256);

		NNEuint charId;
		NNEuint x, y, w, h;
		while(input.good())
		{
			// get character id
			input.ignore(256, '=');
			input >> charId;
			// get x
			input.ignore(256, '=');
			input >> x;
			// get y
			input.ignore(256, '=');
			input >> y;
			// get w
			input.ignore(256, '=');
			input >> w;
			// get h
			input.ignore(256, '=');
			input >> h;

			m_charRect[charId].setPos(x / 255.0f, y / 255.0f);
			m_charRect[charId].setWidthHeight((x + w) / 255.0f, (y + h) / 255.0f);

			// go to the next line
			input.getline(tmpBuffer, 256);
		}
	}

	input.close();
}

NNEvoid Font::getTexcoord(NNEchar c, NNEfloat *ltx, NNEfloat *lty, NNEfloat *rbx, NNEfloat *rby)
{
	*ltx = m_charRect[(NNEuint)c].x;
	*lty = m_charRect[(NNEuint)c].y;
	*rbx = m_charRect[(NNEuint)c].w;
	*rby = m_charRect[(NNEuint)c].h;
}

} // namespace NNE