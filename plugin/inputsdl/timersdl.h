#ifndef TIMERSDL_H__
#define TIMERSDL_H__

/*!	\class TimerSDL
	$Id: timersdl.h,v 1.1 2003/04/30 15:42:58 anoncvs_rssview Exp $

	This returns the elapsed time since the last call in milliseconds.

	NOTE:

	HISTORY:
		24.10.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include "inputdevicesdl.h"

class TimerSDL : public InputDeviceSDL
{
	//
	// functions
	//
public:
	TimerSDL();
	virtual ~TimerSDL();

	//! init the device
	virtual bool Init(kerosin::InputSystem *inputSystem);

	//! this filters an SDL specific input
	virtual int EventFilter(const SDL_Event *event);

	//! return an Input
	void GetInput(kerosin::InputServer::Input &input);
private:
	unsigned long	mLastTicks;
};

DECLARE_CLASS(TimerSDL);

#endif //MOUSESDL_H__
