#include "timersdl.h"
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;

TimerSDL::TimerSDL() :
InputDeviceSDL(), mLastTicks(0)
{
}

TimerSDL::~TimerSDL()
{
}

bool TimerSDL::Init(kerosin::InputSystem *inputSystem)
{
	if (InputDevice::Init(inputSystem) == false) return false;
	mLastTicks = SDL_GetTicks();

	return true;
}

int TimerSDL::EventFilter(const SDL_Event *event)
{
	return 1;
}

void TimerSDL::GetInput(kerosin::InputServer::Input &input)
{
	input.type	= InputServer::eAxis;
	input.code	= mInputSystem->GetInputServer()->IC_AXIST;

	unsigned int ticks = SDL_GetTicks();
	// FIXME: ticks might wrap
	input.data.l = ticks - mLastTicks;
	mLastTicks = ticks;
}