#ifndef FONTSERVER_H__
#define FONTSERVER_H__

/*	$Id: fontserver.h,v 1.1 2003/04/30 11:29:33 fruit Exp $

	The fontserver manages Font objects. It prevents a single font from being loaded
	several times by the runtime system.
	
	NOTE:

	HISTORY:
		15.10.01 - MK
			- Initial version
		02.10.02 - MK
			- Moved to Kerosin (major changes)

	TODO:

	TOFIX:
*/

#include <zeitgeist/base.h>
// strange as this may seem
#include <ft2build.h>
#include FT_FREETYPE_H

namespace kerosin
{

class Font;

class FontServer : public zeitgeist::Base
{
public:
	FontServer();
	~FontServer();

	//! load a font with a given size
	boost::shared_ptr<Font>	GetFont(const std::string &name, unsigned int size = 12);
	
	//! test if a specific font has been loaded
	boost::shared_ptr<Font>	FindFont(const std::string &name, unsigned int size) const;

	// rendering
	void	Begin();
	void	End();

protected:
	bool					LoadFont(const std::string &name, unsigned int size, boost::shared_ptr<Font> &font);

private:
	typedef std::list<boost::shared_ptr<kerosin::Font> > TFontList;
	TFontList	mFonts;		// this is a registry of loaded fonts

	// FreeType
	FT_Library  mFreeTypeLib;
};

DECLARE_CLASS(FontServer);

} //namespace kerosin

#endif //__FONTSERVER_H__