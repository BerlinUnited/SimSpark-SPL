#ifndef GLYPH_H__
#define GLYPH_H__

// strange as this may seem
#include <ft2build.h>
#include FT_FREETYPE_H

#include <boost/shared_array.hpp>

namespace kerosin
{

class Glyph
{
public:
	Glyph();

	bool LoadGlyph(FT_Face face, unsigned int charCode);

	unsigned int						mByteWidth;
	unsigned int						mByteHeight;
	unsigned int						mXOffset;
	unsigned int						mYOffset;
	unsigned int						mAdvance;
	boost::shared_array<unsigned char>	mData;
private:
	void Reset();
};

} //namespace kerosin

#endif //GLYPH_H__
