#include "inputsystemsdl.h"
#include "keyboardsdl.h"
#include "mousesdl.h"
#include "timersdl.h"
#include <zeitgeist/zeitgeist.h>

ZEITGEIST_EXPORT_BEGIN()
	ZEITGEIST_EXPORT(InputSystemSDL);
	ZEITGEIST_EXPORT(KeyboardSDL);
	ZEITGEIST_EXPORT(MouseSDL);
	ZEITGEIST_EXPORT(TimerSDL);
ZEITGEIST_EXPORT_END()
