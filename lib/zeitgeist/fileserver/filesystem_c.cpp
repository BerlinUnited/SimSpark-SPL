#include "filesystem.h"

using namespace boost;
using namespace zeitgeist;

FUNCTION(open)
{
	if (in.size() == 1)
	{
		FileSystem *fs = static_cast<FileSystem*>(obj);
		fs->Open(boost::any_cast<char*>(in[0]));
	}
}


void CLASS(FileSystem)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Base);
	DEFINE_FUNCTION(open);
}
