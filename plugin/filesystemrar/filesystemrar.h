#ifndef FILESYSTEMRAR_H__
#define FILESYSTEMRAR_H__

/*!	\class FileSystemRAR
	$Id: filesystemrar.h,v 1.1 2003/04/30 11:06:16 fruit Exp $

	FileSystemRAR

	This class is a wrapper to the unrar.dll capable of handling RAR-compressed
	archives. Unfortunately, the unrar.dll is only available for Windows
	platforms :(

	Another issue to note for this is the way this filesystem works. It does not keep
	the archive open over subsequent Open() calls. Instead it has to reopen the archive
	again and again. This is a limitation of the API provided by the unrar.dll and very
	annoying.

	NOTE:

	HISTORY:
		10.08.02 - MK
			- Initial version

	TODO:
		- implement ForEachFile platform independent

	TOFIX:
*/

#include <string>
#include <kerosin/fileserver/filesystem.h>

class FileSystemRAR : public kerosin::FileSystem
{
	//
	// functions
	//
public:
	FileSystemRAR();
	virtual ~FileSystemRAR();

	virtual salt::RFile*	Open(const char* inName);
	virtual bool			SetPath(const char* inPath);
	virtual int				ForEachFile(const char* expression, TCallback callback, void* param);

	//
	// members
	//
protected:
	std::string	mArchiveName;
};

DECLARE_CLASS(FileSystemRAR);

#endif //FILESYSTEMRAR_H__
