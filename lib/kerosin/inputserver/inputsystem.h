#ifndef INPUTSYSTEM_H__
#define INPUTSYSTEM_H__

/*	\class InputSystem
	$Id: inputsystem.h,v 1.3 2003/08/22 22:43:15 rollmark Exp $

	InputSystem

	An input system is the basic abstraction for an input API. You would
	want to have a specific input system for SDL, DirectX, Windows API, X,
	etc..

	The job of InputSystem is:
		- Initialize the input API
		- Initialize the inputcodes used in the InputServer
		- Handle the creation of individual devices

	NOTE:

	HISTORY:
		21.08.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <deque>
#include <zeitgeist/node.h>
#include "inputserver.h"

namespace kerosin
{

class InputSystem : public zeitgeist::Node
{
	//
	// functions
	//
public:
	InputSystem();
	virtual ~InputSystem();

	//! init the subsystem
	virtual bool Init(kerosin::InputServer *inputServer);
	
	//! creates an instance of a device via zeitgeist object creation. Should use name mangling.
	virtual bool CreateDevice(const std::string &deviceName) = 0;
	
	/*! Add the input to the queue. Uses AddInputInternal. This was necessary to
		allow derived classes to wrap this call in a mutex and use the definitely
		unwrapped addition via AddInputInternal().
	*/
	virtual void AddInput(InputServer::Input &input);
	//! this is the actual addition of input to the queue. It should only be used by InputDevices!
	void AddInputInternal(InputServer::Input &input);
	//! retrieve an input from the queue
	virtual bool GetInput(InputServer::Input &input);

	InputServer*	GetInputServer()	{	return mInputServer;	}
protected:
	virtual bool UpdateTimerInput(InputServer::Input &input);

	//
	// members
	//
protected:
	InputServer						*mInputServer;
private:
	std::deque<InputServer::Input>	mInputQueue;
};

}

#endif //INPUTSYSTEM_H__

