#ifndef MOUSESDL_H__
#define MOUSESDL_H__

/*!	\class MouseSDL
	$Id: mousesdl.h,v 1.1 2003/04/30 11:06:18 fruit Exp $


	NOTE:

	HISTORY:
		22.08.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include "inputdevicesdl.h"

class MouseSDL : public InputDeviceSDL
{
	//
	// functions
	//
public:
	MouseSDL();
	virtual ~MouseSDL();

	//! init the device
	virtual bool Init(kerosin::InputSystem *inputSystem);

	//! this filters an SDL specific input
	virtual int EventFilter(const SDL_Event *event);
};

DECLARE_CLASS(MouseSDL);

#endif //MOUSESDL_H__
