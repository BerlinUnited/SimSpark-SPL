#ifndef INPUTSYSTEMSDL_H__
#define INPUTSYSTEMSDL_H__

/*!	\class InputSystemSDL
	$Id: inputsystemsdl.h,v 1.2 2003/04/30 15:42:58 anoncvs_rssview Exp $


	NOTE:

	HISTORY:
		21.08.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <deque>
#include <kerosin/inputserver/inputsystem.h>
#include <kerosin/inputserver/inputserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <SDL/SDL.h>

struct SDL_mutex;

class TimerSDL;

class InputSystemSDL : public kerosin::InputSystem
{
	//
	// functions
	//
public:
	InputSystemSDL();
	virtual ~InputSystemSDL();

	virtual bool	Init(kerosin::InputServer *inputServer);
	virtual bool	CreateDevice(const std::string &deviceName);

	SDL_mutex*		GetMutex() const;

	int				EventFilter(const SDL_Event *event);

	//! add the input to the queue
	virtual void AddInput(kerosin::InputServer::Input &input);
	//! retrieve an input from the queue
	virtual bool GetInput(kerosin::InputServer::Input &input);
protected:
	virtual bool UpdateTimerInput(kerosin::InputServer::Input &input);

	//
	// members
	//
private:
	SDL_mutex					*mMutex;

	boost::shared_ptr<TimerSDL>	mTimer;
};

DECLARE_CLASS(InputSystemSDL);

#endif //INPUTSYSTEMSDL_H__
