#include "imageserver.h"
#include "image.h"
#include <salt/fileclasses.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <boost/scoped_ptr.hpp>

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

shared_ptr<FileServer> gFileServer;

//------------------------------------------------------------------------------------------------
// FileServer hooks for DevIL
//------------------------------------------------------------------------------------------------
ILHANDLE ILAPIENTRY FSOpen(const ILstring inName)
{
	return gFileServer->Open(inName);
	//return fopen(inName, "rb");
}

ILvoid ILAPIENTRY FSClose(ILHANDLE handle)
{
	//printf("CLOSE\n");
	RFile *file = (RFile*)handle;
	file->Close();
	delete file;
	//fclose((FILE*)handle);
}

ILboolean ILAPIENTRY FSEof(ILHANDLE handle)
{
	RFile *file = (RFile*)handle;

	const ILboolean val= file->Eof(); 
	//gFileServer->GetLog()->Debug().Printf("eof: %x\n", val);
	return val;
}

ILint ILAPIENTRY FSGetc(ILHANDLE handle)
{
	RFile *file = (RFile*)handle;

	const int val = file->Getc(); 
	//gFileServer->GetLog()->Debug().Printf("getc: %x\n", val);
	return val;
}

ILint ILAPIENTRY FSRead(void *buffer, ILuint size, ILuint count, ILHANDLE handle)
{
	RFile *file = (RFile*)handle;
	
	const size_t readSize = file->Read(buffer, size, count);
	//gFileServer->GetLog()->Debug().Printf("read(%d, %d): %d\n", size, count, readSize);
	return readSize;
}

ILint ILAPIENTRY FSSeek(ILHANDLE handle, ILint offset, ILint origin)
{
	RFile *file = (RFile*)handle;

	const int val = file->Seek(offset, origin);
	//gFileServer->GetLog()->Debug().Printf("seek: %d\n", val);
	return val;
}

ILint ILAPIENTRY FSTell(ILHANDLE handle)
{
	RFile *file = (RFile*)handle;

	const int val = file->Tell();
	//gFileServer->GetLog()->Debug().Printf("tell: %d\n", val);
	return val;
}

//------------------------------------------------------------------------------------------------
// ImageServer implementation
//------------------------------------------------------------------------------------------------

// constructor
ImageServer::ImageServer()
{
	// initialize DevIL
	ilInit();

	// and setup the default behavior
	// (this might come out of a config file at a later point)
	ilEnable(IL_FILE_OVERWRITE);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

	// register FileServer hooks for DevIL
	ilSetRead(
		FSOpen,
		FSClose,
		FSEof,
		FSGetc,
		FSRead,
		FSSeek,
		FSTell);
}

//
// This function loads the file inName. If inType is IL_TYPE_UNKNOWN,
// then the library will try to find a handler by the file extension provided.
// This behavior is done automatically by the library!
//
boost::shared_ptr<Image> ImageServer::Load(const char *inName, ILenum inType)
{
	// create a new image
	boost::shared_ptr<Image> image(new Image());

	// make it active with DevIL
	image->Bind();

	// set the file server
	gFileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));

	// load the image
	ilLoad(inType, (ILstring)inName);

	// set the file server to 0 again
	gFileServer.reset();

	// check for errors
	if(HandleErrors() == true)
	{
		// release the image and return
		return boost::shared_ptr<Image>();
	}

	return image;
}

bool ImageServer::Save(const boost::shared_ptr<Image> &inImage, const char *inName, ILenum inType)
{
	// make the image active
	inImage->Bind();

	// set the file server
	gFileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));

	// save the image
	ilSave(inType, (ILstring)inName);

	// set the file server to 0 again
	gFileServer.reset();

	// check for errors
	if(HandleErrors() == true)
	{
		return false;
	}

	return true;
}

//
//	This routine checks for DevIL errors and logs them. The function returns
//	'true' if an error has occured and 'false' if not.
//
bool ImageServer::HandleErrors()
{
	bool ret = false;
	ILenum error;

	// check if we have any errors and log them accordingly
	while ((error = ilGetError()) != IL_NO_ERROR)
	{
		ret = true;
		GetLog()->Error().Printf("DevIL Error: %x\n", error); 
	} 

	return ret;
}