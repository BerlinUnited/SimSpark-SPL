/*	$Id: font.h,v 1.3 2003/08/22 22:43:15 rollmark Exp $
	
	NOTE:

	HISTORY:
		08.10.01 - MK
			- Initial version
		12.10.01 - MK
			- This now supports fonts generated with Fluid Studios Font Generator
		16.10.01 - MK
			- Calculates minimum texture size required
			- Only needs one pixel in-between characters when placing them in the texture
		16.10.01 - MR
			- Added support to calculate the width of a string printed with this font
		02.10.02 - MK
			- Moved to Kerosin (major changes)

	TODO:
		- better texture usage

	TOFIX:
		- ImageServer activation hack (see FIXME in cpp file)
*/

#ifndef FONT_H__
#define FONT_H__

#include <string>
#include <salt/vector.h>
#include "glyph.h"

namespace kerosin
{

class FontServer;

/** Font allows the use of a 'texture'-based font. The font is loaded from
    Fluid Studios Font Files, which can be generated with a small utility from
    any Windows font. Care has to be taken, that the resulting font will fit
    in a 256x256 texture! Our font will only contain characters for the ASCII
    value range 32-128 ... this should cover the major alphanumeric
    characters!
 */
class Font
{
private:
	struct GlyphMetric
	{
		unsigned int						mByteWidth;
		unsigned int						mByteHeight;
		unsigned int						mXOffset;
		unsigned int						mYOffset;
		unsigned int						mAdvance;
		salt::Vector2f						mTC1, mTC2;		
	};
public:
	Font(FontServer &fontServer);
	~Font();

	bool	Init(const std::string &name, unsigned int size, FT_Face face);
	bool	Bind(int vRows = -1);
	void	Dump();
	void	DrawString(float x, float y, const char *string);
	void	Printf(float x, float y, const char *format, ...);
	void	RowPrintf(float x, float row, const char *format, ...);
	float   GetStringWidth(const char* string, int numChar = -1);
	float	GetRowHeight()	{	return (float)mRowHeight;	}

	const std::string&	GetName() const {	return mName;	}
	unsigned int		GetSize() const	{	return mSize;	}

private:
	GlyphMetric		mMetrics[96];	// the metrics of all glyphs
	unsigned int	mTexID;			// OpenGL Texture ID
	unsigned int	mRowHeight;		// height (in pixels) of a row
	std::string		mName;	// name of font
	unsigned int	mSize;	// size of font
	FontServer&		mFontServer;
};

} // namespace kerosin

#endif //FONT_H__
