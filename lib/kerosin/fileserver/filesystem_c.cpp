#include "filesystem.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(open)
{
	if (in.size() == 1)
	{
		shared_ptr<FileSystem> fs = shared_static_cast<FileSystem>(obj);
		fs->Open(boost::any_cast<char*>(in[0]));
	}
}


void CLASS(FileSystem)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Base);
	DEFINE_FUNCTION(open);
}
