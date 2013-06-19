#include "glyph.h"

using namespace kerosin;

Glyph::Glyph()
{
	Reset();
}

bool Glyph::LoadGlyph(FT_Face face, unsigned int charCode)
{
	int error = FT_Load_Char( face, charCode, FT_LOAD_RENDER);

	if (error)
	{
		return false;
	}

	Reset();

	mByteWidth = face->glyph->bitmap.width;
	mByteHeight = face->glyph->bitmap.rows;

	mData.reset(new unsigned char[mByteWidth*mByteHeight]);

	//printf("Glyph: '%c' %d\n", charCode, face->glyph->bitmap.pitch);
	//printf("  Res:    %dx%d\n", face->glyph->bitmap.width, face->glyph->bitmap.rows);
	for(int y=0; y<face->glyph->bitmap.rows; ++y)
	{
		for(int x=0; x<face->glyph->bitmap.width; ++x)
		{
			mData[y*mByteWidth + x] = face->glyph->bitmap.buffer[y*face->glyph->bitmap.pitch + x];
		}
	}
	mXOffset = face->glyph->metrics.horiBearingX >> 6;
	mYOffset = face->glyph->metrics.horiBearingY >> 6;
	mAdvance = face->glyph->advance.x >> 6;
	//printf("  Offset: %dx%d\n", mXOffset, mYOffset);
	//printf("  Advance:%d\n", mAdvance);

	return true;
}

void Glyph::Reset()
{
	mByteWidth	= 0;
	mByteHeight	= 0;
	mXOffset	= 0;
	mYOffset	= 0;
	mAdvance	= 0;
	mData.reset();
}
