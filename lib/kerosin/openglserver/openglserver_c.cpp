#include "openglserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(init)
{
	if (in.size() == 0)
	{
		OpenGLServer *ogl = static_cast<OpenGLServer*>(obj);
		ogl->Init();
	}
}

void CLASS(OpenGLServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Base);
	DEFINE_FUNCTION(init);
}
