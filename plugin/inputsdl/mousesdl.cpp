#include "mousesdl.h"
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;

MouseSDL::MouseSDL() :
InputDeviceSDL()
{
}

MouseSDL::~MouseSDL()
{
}

bool MouseSDL::Init(kerosin::InputSystem *inputSystem)
{
	if (InputDevice::Init(inputSystem) == false) return false;

	return true;
}

int MouseSDL::EventFilter(const SDL_Event *event)
{
	// we only want mouse events
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		{
			InputServer::Input input(InputServer::eButton);
			if (event->button.button == SDL_BUTTON_LEFT)
				input.code = mInputSystem->GetInputServer()->Codes().IC_MOUSE_LEFT;
			else if (event->button.button == SDL_BUTTON_RIGHT)
				input.code = mInputSystem->GetInputServer()->Codes().IC_MOUSE_RIGHT;
			else if (event->button.button == SDL_BUTTON_MIDDLE)
				input.code = mInputSystem->GetInputServer()->Codes().IC_MOUSE_MIDDLE;
			else return 1;

			input.data.l = (event->type == SDL_MOUSEBUTTONDOWN);
			
			mInputSystem->AddInputInternal(input);
			return 0;
		}
		break;
	case SDL_MOUSEMOTION:
		{
			InputServer::Input input(InputServer::eAxis, mInputSystem->GetInputServer()->Codes().IC_AXISX);
			input.data.l = event->motion.xrel;
			mInputSystem->AddInputInternal(input);

			input.code = mInputSystem->GetInputServer()->Codes().IC_AXISY;
			input.data.l = event->motion.yrel;
			mInputSystem->AddInputInternal(input);		
			return 0;
		}
		break;
	}
	return 1;
}