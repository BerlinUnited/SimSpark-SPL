#ifndef KEROSIN_H__
#define KEROSIN_H__

/*! \class Kerosin
	$Id: kerosin.h,v 1.1 2003/04/30 11:29:32 fruit Exp $
	
	Kerosin
	:TODO: Class description for Kerosin

	HISTORY:
		11.07.2002 MK
			- initial version

*/

#include "soundserver/soundserver.h"

#include "inputserver/inputserver.h"

#include "imageserver/imageserver.h"

#include "fontserver/fontserver.h"
#include "fontserver/font.h"

#include "openglserver/openglserver.h"

#include "textureserver/textureserver.h"
#include "textureserver/texture.h"

#include <zeitgeist/zeitgeist.h>

namespace kerosin
{

class Kerosin
{
	//
	// types
	//
public:
protected:
private:

	//
	// functions
	//
public:
	Kerosin(zeitgeist::Zeitgeist &zg);
	virtual ~Kerosin() {};
protected:
private:
	Kerosin(const Kerosin& obj);
	Kerosin& operator=(const Kerosin& obj);

	//
	// members
	//
public:
protected:
private:
};

} //namespace kerosin

#endif //KEROSIN_H__
