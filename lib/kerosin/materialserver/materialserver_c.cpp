#include "materialserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(touch)
{
	if (in.size() == 1)
	{
		MaterialServer *ms = static_cast<MaterialServer*>(obj);
		ms->Touch(any_cast<char*>(in[0]));
	}
}

void CLASS(MaterialServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Leaf);
	DEFINE_FUNCTION(touch);
}
