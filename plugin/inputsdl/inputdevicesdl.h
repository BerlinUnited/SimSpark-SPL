#ifndef INPUTDEVICESDL_H__
#define INPUTDEVICESDL_H__

/*	\class InputDeviceSDL
	$Id: inputdevicesdl.h,v 1.2 2003/04/30 15:42:58 anoncvs_rssview Exp $

	InputDeviceSDL

	This class introduces SDL-specific callback functions for the event filtering.

	NOTE:

	HISTORY:
		21.08.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <kerosin/inputserver/inputdevice.h>
#include <kerosin/inputserver/inputserver.h>
#include <SDL/SDL.h>

class InputDeviceSDL : public kerosin::InputDevice
{
	//
	// functions
	//	
public:
	//! this filters an SDL specific input
	virtual int EventFilter(const SDL_Event *event) = 0;
};

#endif //INPUTDEVICESDL_H__
