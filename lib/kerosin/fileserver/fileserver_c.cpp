#include "fileserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(exist)
{
	if (in.size() == 1)
	{
		shared_ptr<FileServer> fs = shared_static_cast<FileServer>(obj);
		bool ret = fs->Exist(boost::any_cast<char*>(in[0]));
		printf("Exist -> %d\n", ret);
	}
}

FUNCTION(mount)
{
	if (in.size() == 2)
	{
		shared_ptr<FileServer> fs = shared_static_cast<FileServer>(obj);
		bool ret = fs->Mount(boost::any_cast<char*>(in[0]), boost::any_cast<char*>(in[1]));
	}
}

FUNCTION(unmount)
{
	if (in.size() == 1)
	{
		shared_ptr<FileServer> fs = shared_static_cast<FileServer>(obj);
		bool ret = fs->Unmount(boost::any_cast<char*>(in[0]));
	}
	if (in.size() == 2)
	{
		shared_ptr<FileServer> fs = shared_static_cast<FileServer>(obj);
		bool ret = fs->Unmount(boost::any_cast<char*>(in[0]), boost::any_cast<char*>(in[1]));
	}
}

void CLASS(FileServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Node);
	DEFINE_FUNCTION(exist);
	DEFINE_FUNCTION(mount);
	DEFINE_FUNCTION(unmount);
}
