#include "inputsystemsdl.h"
#include "inputdevicesdl.h"
#include <kerosin/inputserver/inputserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <SDL/SDL_thread.h>

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

// this saves a lot of writing and makes changes local
#define INIT_CODE(val) mInputServer->mCodes.IC_##val = SDLK_##val;
#define INIT_CODE_EX(kVal, sdlVal) mInputServer->mCodes.IC_##kVal = SDLK_##sdlVal;

/*!
	This function is used to filter SDL Events and delegate them to the input
	device which registered for the event type.

	NOTE: This function may run in a different thread, so care has to be taken
	when accessing the global object. Things to watch out for:

		- gDeviceList changes (done by individual devices (construction/destruction)) handled
		- Events are pushed into a device (done by EventFilter)
		- Events are taken out of a device (this is done by the inputserver)
*/

InputSystemSDL *gInputSystem;

static int EventFilterCallback(const SDL_Event *event)
{
	if (gInputSystem)
	{
		return gInputSystem->EventFilter(event);
	}
	else
		return 1;
}

InputSystemSDL::InputSystemSDL() :
InputSystem(), mMutex(NULL)
{
	gInputSystem = this;
}

InputSystemSDL::~InputSystemSDL()
{
	GetLog()->Normal() << "InputSystemSDL shutting down" << std::endl;
	SDL_LockMutex(mMutex);
	SDL_SetEventFilter(NULL);
	gInputSystem = NULL;
	SDL_UnlockMutex(mMutex);

	SDL_DestroyMutex(mMutex);
	mMutex = NULL;
}

bool InputSystemSDL::Init(InputServer *inputServer)
{
	if (InputSystem::Init(inputServer) == false) return false;

	// input codes have been set (jey)

	// here we check whether SDL has been initialized prior to adding this
	// input is part of the video subsystem (because of event loops, etc..)
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		GetLog()->Error() << "ERROR: SDL not initialized!" << std::endl;
		return false;
	}

	// we need a mutex object
	mMutex = SDL_CreateMutex();

	// setup the filtering function
	SDL_SetEventFilter(EventFilterCallback);

	return true;
}

void InputSystemSDL::InitInputCodes()
{
	// numbers
	INIT_CODE(0);
	INIT_CODE(1);
	INIT_CODE(2);
	INIT_CODE(3);
	INIT_CODE(4);
	INIT_CODE(5);
	INIT_CODE(6);
	INIT_CODE(7);
	INIT_CODE(8);
	INIT_CODE(9);
	// function keys
	INIT_CODE(F1);
	INIT_CODE(F2);
	INIT_CODE(F3);
	INIT_CODE(F4);
	INIT_CODE(F5);
	INIT_CODE(F6);
	INIT_CODE(F7);
	INIT_CODE(F8);
	INIT_CODE(F9);
	INIT_CODE(F10);
	INIT_CODE(F11);
	INIT_CODE(F12);
	// alpabet
	INIT_CODE_EX(A, a);
	INIT_CODE_EX(B, b);
	INIT_CODE_EX(C, c);
	INIT_CODE_EX(D, d);
	INIT_CODE_EX(E, e);
	INIT_CODE_EX(F, f);
	INIT_CODE_EX(G, g);
	INIT_CODE_EX(H, h);
	INIT_CODE_EX(I, i);
	INIT_CODE_EX(J, j);
	INIT_CODE_EX(K, k);
	INIT_CODE_EX(L, l);
	INIT_CODE_EX(M, m);
	INIT_CODE_EX(N, n);
	INIT_CODE_EX(O, o);
	INIT_CODE_EX(P, p);
	INIT_CODE_EX(Q, q);
	INIT_CODE_EX(R, r);
	INIT_CODE_EX(S, s);
	INIT_CODE_EX(T, t);
	INIT_CODE_EX(U, u);
	INIT_CODE_EX(V, v);
	INIT_CODE_EX(W, w);
	INIT_CODE_EX(X, x);
	INIT_CODE_EX(Y, y);
	INIT_CODE_EX(Z, z);
	// keypad
	INIT_CODE(KP0);
	INIT_CODE(KP1);
	INIT_CODE(KP2);
	INIT_CODE(KP3);
	INIT_CODE(KP4);
	INIT_CODE(KP5);
	INIT_CODE(KP6);
	INIT_CODE(KP7);
	INIT_CODE(KP8);
	INIT_CODE(KP9);
	INIT_CODE(KP_PERIOD);
	INIT_CODE(KP_DIVIDE);
	INIT_CODE(KP_MULTIPLY);
	INIT_CODE(KP_MINUS);
	INIT_CODE(KP_PLUS);
	INIT_CODE(KP_ENTER);
	INIT_CODE(KP_EQUALS);
	// arrows + home/end pad
	INIT_CODE(UP);
	INIT_CODE(DOWN);
	INIT_CODE(RIGHT);
	INIT_CODE(LEFT);
	INIT_CODE(INSERT);
	INIT_CODE(HOME);
	INIT_CODE(END);
	INIT_CODE(PAGEUP);
	INIT_CODE(PAGEDOWN);
	// key state modifier keys
	INIT_CODE(NUMLOCK);
	INIT_CODE(CAPSLOCK);
	INIT_CODE(SCROLLOCK);
	INIT_CODE(RSHIFT);
	INIT_CODE(LSHIFT);
	INIT_CODE(RCTRL);
	INIT_CODE(LCTRL);
	INIT_CODE(RALT);
	INIT_CODE(LALT);
	INIT_CODE(RMETA);
	INIT_CODE(LMETA);
	INIT_CODE(LSUPER);	// Left "Windows" key
	INIT_CODE(RSUPER);	// Right "Windows" key
	INIT_CODE(MODE);	// "Alt Gr" key
	INIT_CODE(COMPOSE);	// Multi-key compose key
	// other keys (cursor control, punctuation)
	INIT_CODE(BACKSPACE);
	INIT_CODE(TAB);
	INIT_CODE(CLEAR);
	INIT_CODE(RETURN);
	INIT_CODE(PAUSE);
	INIT_CODE(ESCAPE);
	INIT_CODE(SPACE);
	INIT_CODE(EXCLAIM);
	INIT_CODE(QUOTEDBL);
	INIT_CODE(HASH);
	INIT_CODE(DOLLAR);
	INIT_CODE(AMPERSAND);
	INIT_CODE(QUOTE);
	INIT_CODE(LEFTPAREN);
	INIT_CODE(RIGHTPAREN);
	INIT_CODE(ASTERISK);
	INIT_CODE(PLUS);
	INIT_CODE(COMMA);
	INIT_CODE(MINUS);
	INIT_CODE(PERIOD);
	INIT_CODE(SLASH);
	INIT_CODE(COLON);
	INIT_CODE(SEMICOLON);
	INIT_CODE(LESS);
	INIT_CODE(EQUALS);
	INIT_CODE(GREATER);
	INIT_CODE(QUESTION);
	INIT_CODE(AT);
	INIT_CODE(LEFTBRACKET);
	INIT_CODE(BACKSLASH);
	INIT_CODE(RIGHTBRACKET);
	INIT_CODE(CARET);
	INIT_CODE(UNDERSCORE);
	INIT_CODE(BACKQUOTE);
	INIT_CODE(DELETE);
	// Miscellaneous function keys
	INIT_CODE(HELP);
	INIT_CODE(PRINT);
	INIT_CODE(SYSREQ);
	INIT_CODE(BREAK);
	INIT_CODE(MENU);
	INIT_CODE(POWER);	// Power Macintosh power key 
	INIT_CODE(EURO);	// Some european keyboards 
	INIT_CODE(UNDO);	// Atari keyboard has Undo
	// mouse buttons
	mInputServer->mCodes.IC_MOUSE_LEFT		= 0x1000;
	mInputServer->mCodes.IC_MOUSE_RIGHT		= 0x1001;
	mInputServer->mCodes.IC_MOUSE_MIDDLE	= 0x1002;
	//mouse axis
	mInputServer->mCodes.IC_AXISX	= 0x2000;
	mInputServer->mCodes.IC_AXISY	= 0x2001;
	mInputServer->mCodes.IC_AXISZ	= 0x2002;
	// timer
	mInputServer->mCodes.IC_AXIST	= 0x3000;
}

bool InputSystemSDL::CreateDevice(const std::string &deviceName)
{
	// first, we mangle the deviceName to avoid nameclashes
	std::string mangledName = deviceName + "SDL";

	shared_ptr<InputDevice> device = shared_static_cast<InputDevice>(GetCore()->New(mangledName));

	if (device.get() == NULL)
	{
		GetLog()->Error() << "ERROR: Creating device '" << mangledName << "'" << std::endl;
		return false;
	}
	
	// initialize the device
	if (device->Init(this) == false)
	{
		GetLog()->Error() << "ERROR: Initializing device '" << mangledName << "'" << std::endl;
		return false;
	}

	SDL_LockMutex(mMutex);
	// try to link the device into the inputserver
	if (AddChildReference(device) == false)
	{
		GetLog()->Error() << "ERROR: Linking device '" << mangledName << "'" << std::endl;
		SDL_UnlockMutex(mMutex);
		return false;
	}
	SDL_UnlockMutex(mMutex);

	return true;
}

int InputSystemSDL::EventFilter(const SDL_Event *event)
{
	SDL_LockMutex(mMutex);
	// loop through all children
	for (TBaseList::iterator i = mChildren.begin(); i!=mChildren.end(); ++i)
	{
		shared_ptr<InputDeviceSDL> device = shared_static_cast<InputDeviceSDL>(*i);
		// every device gets a chance to filter the event, the first one
		// who claims it, gets it
		if (device->EventFilter(event) == 0) return 0;
	}
	SDL_UnlockMutex(mMutex);

	return 1;
}

void InputSystemSDL::AddInput(kerosin::InputServer::Input &input)
{
	SDL_LockMutex(mMutex);
	InputSystem::AddInput(input);
	SDL_UnlockMutex(mMutex);
}

bool InputSystemSDL::GetInput(kerosin::InputServer::Input &input)
{
	SDL_LockMutex(mMutex);
	bool ret = InputSystem::GetInput(input);
	SDL_UnlockMutex(mMutex);

	return ret;
}
