#ifndef INPUTDEVICE_H__
#define INPUTDEVICE_H__

/*	\class InputDevice
	$Id: inputdevice.h,v 1.2 2003/04/30 14:21:26 fruit Exp $

	InputDevice

	An InputDevice is the abstract base class which creates input
	for the inputserver. Common InputDevice would be the Keyboard
	and Mouse, but less common 'devices' such as AI agents (think
	of them pushing the controls) or time are also possible.

	The input devices are managed by the input server. As the
	application only communicates with the input server in general
	this makes adding new input devices transparent to the
	application (Good Thing(TM)).

	NOTE:

	HISTORY:
		21.08.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <zeitgeist/leaf.h>
#include "inputsystem.h"

namespace kerosin
{

class InputDevice : public zeitgeist::Leaf
{
	//
	// functions
	//	
public:
	InputDevice();
	~InputDevice();

	//! init the device
	virtual bool Init(kerosin::InputSystem *inputSystem);

	//
	// members
	//	
protected:
	InputSystem	*mInputSystem;
};

} //namespace kerosin

#endif //INPUTDEVICE_H__
