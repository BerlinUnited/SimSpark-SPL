#ifndef KEYBOARDSDL_H__
#define KEYBOARDSDL_H__

/*!	\class KeyboardSDL
	$Id: keyboardsdl.h,v 1.1 2003/04/30 11:06:18 fruit Exp $


	NOTE:

	HISTORY:
		22.08.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include "inputdevicesdl.h"

class KeyboardSDL : public InputDeviceSDL
{
	//
	// functions
	//
public:
	KeyboardSDL();
	virtual ~KeyboardSDL();

	//! init the device
	virtual bool Init(kerosin::InputSystem *inputSystem);

	//! this filters an SDL specific input
	virtual int EventFilter(const SDL_Event *event);
};

DECLARE_CLASS(KeyboardSDL);

#endif //KEYBOARDSDL_H__
