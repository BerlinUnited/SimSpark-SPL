#include "filesystemrar.h"
#include <salt/fileclasses.h>
#include <windows.h>
#include <unrar/unrar.h>

using namespace salt;

static int index = 0;

static int PASCAL RARCallback(UINT msg, LONG UserData, LONG P1, LONG P2)
{
	unsigned char *buffer = (unsigned char*)UserData;

	switch(msg)
	{
		case UCM_PROCESSDATA:
			memcpy(&buffer[index], (const void*)P1, P2);
			index += P2;
			return 1;
	}

	return 0;
}

// constructor
FileSystemRAR::FileSystemRAR()
{
}

// destructor
FileSystemRAR::~FileSystemRAR()
{
}

bool FileSystemRAR::SetPath(const char* inPath)
{
	mArchiveName = inPath;
	FILE *f = fopen(inPath, "rb");
	bool ret = (f != NULL);
	
	if (f) fclose(f);

	return ret;
}

//
// This function is really simple. It appends inName to mPath and
// tries to open the combined name as a readonly file.
//
salt::RFile* FileSystemRAR::Open(const char* inName)
{
	RAROpenArchiveData	openArchiveData;

	memset(&openArchiveData, 0, sizeof(RAROpenArchiveData));
	openArchiveData.ArcName		= (char*)mArchiveName.c_str();
	openArchiveData.OpenMode	= RAR_OM_EXTRACT;
	
	HANDLE handle = RAROpenArchive(&openArchiveData);

	if (openArchiveData.OpenResult)
	{
		RARCloseArchive(handle);
		// something went wrong
		return NULL;
	}

	// set the callback
	RARSetCallback(handle, NULL, 0);

	RARHeaderData headerData;
	
	while (RARReadHeader(handle, &headerData)==0)
	{
		if (strcmp(headerData.FileName, inName) == 0)
		{
			// we have a match
			unsigned char *buffer = new unsigned char[headerData.UnpSize];
			MemFile *file = new MemFile();
			file->Open(buffer, headerData.UnpSize);
			RARSetCallback(handle, RARCallback, (LONG)buffer);
			index = 0;
			if (RARProcessFile(handle, RAR_TEST, NULL, NULL) != 0)
			{
				delete file;
				file = NULL;
			}
			RARSetCallback(handle, NULL, 0);
			RARCloseArchive(handle);
			return file;
		}
		else
		{
			RARProcessFile(handle, RAR_SKIP, NULL, NULL);
		}
	}

	RARSetCallback(handle, NULL, 0);
	RARCloseArchive(handle);
	return NULL;
}

int FileSystemRAR::ForEachFile(const char* expression, TCallback callback, void* param) 
{
	// todo
	return 0;
}
