#include "keyboardsdl.h"
#include <kerosin/inputserver/inputserver.h>
#include <zeitgeist/logserver/logserver.h>

using namespace kerosin;

KeyboardSDL::KeyboardSDL() :
InputDeviceSDL()
{
}

KeyboardSDL::~KeyboardSDL()
{
}

bool KeyboardSDL::Init(kerosin::InputSystem *inputSystem)
{
	if (InputDevice::Init(inputSystem) == false) return false;

	return true;
}

int KeyboardSDL::EventFilter(const SDL_Event *event)
{
	// we only want keyboard events
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		mInputSystem->GetInputServer()->mModifierState = 0;
		if (event->key.keysym.mod & KMOD_LSHIFT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eLShift;
		if (event->key.keysym.mod & KMOD_RSHIFT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eRShift;
		if (event->key.keysym.mod & KMOD_LALT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eLAlt;
		if (event->key.keysym.mod & KMOD_RALT) mInputSystem->GetInputServer()->mModifierState |= InputServer::eRAlt;
		if (event->key.keysym.sym != 0)
		{
			printf("SDL: %d %d\n", event->key.keysym.sym, event->key.keysym.scancode);
			InputServer::Input input(InputServer::eButton, event->key.keysym.sym);
			input.data.l = (event->type == SDL_KEYDOWN);
			mInputSystem->AddInputInternal(input);
			return 0;
		}
	}
	return 1;
}