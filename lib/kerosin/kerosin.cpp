#include "kerosin.h"
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace zeitgeist;

Kerosin::Kerosin(zeitgeist::Zeitgeist &zg)
{
	zg.GetCore()->RegisterClassObject(new CLASS(SoundServer), "kerosin/");

	zg.GetCore()->RegisterClassObject(new CLASS(InputServer), "kerosin/");

	zg.GetCore()->RegisterClassObject(new CLASS(ImageServer), "kerosin/");

	zg.GetCore()->RegisterClassObject(new CLASS(FontServer), "kerosin/");

	zg.GetCore()->RegisterClassObject(new CLASS(OpenGLServer), "kerosin/");

	zg.GetCore()->RegisterClassObject(new CLASS(TextureServer), "kerosin/");

	// load default setting
	zg.GetCore()->GetRoot()->GetScript()->Run("script/default.rb");
}
