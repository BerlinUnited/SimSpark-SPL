#include "fileserver.h"
#include <salt/fileclasses.h>
#include <zeitgeist/core.h>

using namespace boost;
using namespace salt;
using namespace zeitgeist;

FileServer::FileServer() : Node()
{
}

FileServer::~FileServer()
{
}

RFile* FileServer::Open(const char* inName)
{
	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		shared_ptr<FileSystem> fileSys = shared_static_cast<FileSystem>(*i);
		RFile *file = fileSys->Open(inName);
		//first successful is returned
		if(file != NULL) { return file; }
	}

	// try to open it via the regular file system
	RFile *file = new StdFile();
	if (file->Open(inName))
	{
		return file;
	}

	// too bad ... didn't work
	delete file;
	return NULL;
}

bool FileServer::Exist(const char *inName)
{
	RFile *file = Open(inName);

	if(file == NULL) return false;

	delete file;
	return true;
}

// this routine registers a new file system instance with the server
bool FileServer::Mount(const char *inFileSysName, const char *inPath)
{
	shared_ptr<FileSystem> fileSys = shared_static_cast<FileSystem>(GetChild(inPath));

	if (fileSys)
	{
		// we already have a file system which is bound to the same name
		if (fileSys->GetClass()->GetName().compare(inFileSysName) == 0)
		{
			// as the file system has the same type, we can return true
			return true;
		}
		else
		{
			// already have a file system of a different type, so return false
			return false;
		}
	}

	// try to instantiate the file system
	fileSys = shared_static_cast<FileSystem>(GetCore()->New(inFileSysName));
	
	if(fileSys)
	{
		if(fileSys->SetPath(inPath))
		{
			// try to name the file system
			fileSys->SetName(inPath);

			// link it into our hierarchy
			if (AddChildReference(fileSys))
			{
				return true;
			}
		}
		// once we reach this point something hase gone wrong
	}

	return false;
}

bool FileServer::Unmount(const char* inPath)
{
	// try to remove a std file system first
	if (Unmount ("FileSystemSTD", inPath))
	{
		return true;
	}

	shared_ptr<Leaf> leaf = GetChild(inPath);

	if(leaf)
	{
		leaf->Unlink();
		return true;
	}
	
	return false;
}
	
bool FileServer::Unmount(const char* inFileSysName, const char* inPath)
{
	shared_ptr<FileSystem> fileSystem = shared_static_cast<FileSystem>(GetChild(inPath));

	if(fileSystem)
	{
		if (fileSystem->GetClass()->GetName().compare(inFileSysName) == 0)
		{
			fileSystem->Unlink();
			return true;
		}
	}

	return false;
}

int FileServer::ForEachFile(const char* /*directory*/, const char* /*name*/, const char* /*extension*/, FileSystem::TCallback /*callback*/, void* /*param*/)
{
	int count = 0;
	/*for (TDescriptionList::iterator iter = mFileSystems.begin (); iter != mFileSystems.end (); ++iter)
	{
		count += iter->fileSystem->ForEachFile(directory,name,extension,callback,param);
	}*/

	return count;
}

