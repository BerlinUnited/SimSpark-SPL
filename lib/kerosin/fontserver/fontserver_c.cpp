#include "fontserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(getFont)
{
	if (in.size() == 1)
	{
		FontServer *fs = static_cast<FontServer*>(obj);
		fs->GetFont(any_cast<char*>(in[0]));
	}
	if (in.size() == 2)
	{
		FontServer *fs = static_cast<FontServer*>(obj);
		fs->GetFont(any_cast<char*>(in[0]), any_cast<int>(in[1]));
	}
}

void CLASS(FontServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Leaf);
	DEFINE_FUNCTION(getFont);
}
