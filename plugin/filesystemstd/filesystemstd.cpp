#include "filesystemstd.h"
#include <salt/fileclasses.h>

using namespace salt;

// constructor
FileSystemSTD::FileSystemSTD()
{
}

// destructor
FileSystemSTD::~FileSystemSTD()
{
}

bool FileSystemSTD::SetPath(const char* inPath)
{
	// release memory for old path
	mPath = inPath;

	return true;
}

//
// This function is really simple. It appends inName to mPath and
// tries to open the combined name as a readonly file.
//
salt::RFile* FileSystemSTD::Open(const char* inName)
{
	//Assert(inName != NULL, "No name specified for openening a file");

	RFile*		file		= new StdFile();
	std::string	fileName	= mPath;
	
	fileName += inName;	// append inName

	if(file->Open(fileName.c_str()) == false)
	{
		// hmm, couldn't open file
		delete file;
		return NULL;
	}

	return file;
}

int FileSystemSTD::ForEachFile(const char* expression, TCallback callback, void* param) 
{
	// todo
	return 0;
}
