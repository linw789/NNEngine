#ifndef NNE_FONT_H
#define NNE_FONT_H

#include "PlatformSpecs.h"
#include "String.h"
#include "Rect.h"
#include "Texture.h"
#include "String.h"

namespace NNE{

class Font
{
public:

	Font(Sid n)
		:m_name(n)
	{}

	~Font()
	{}

	Sid getNameSid()
	{
		return m_name;
	}

	NNEvoid setTexture(Texture *t)
	{
		m_fontTexture = t;
	}

	NNEvoid loadFontInfo(const NNEchar *filename);

	// left-top corner x, left-top corner y,
	// right-bottom corner x, right-bottom curner y
	NNEvoid getTexcoord(NNEchar c, NNEfloat *ltx, NNEfloat *lty, NNEfloat *rbx, NNEfloat *rby);

private:

	Sid m_name;

	Texture *m_fontTexture;

	// position and size of each character on the font texture
	Rectf m_charRect[255];
};

} // manespace NNE

#endif // NNE_FONT_H