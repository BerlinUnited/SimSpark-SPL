#ifndef FILESYSTEMSTD_H__
#define FILESYSTEMSTD_H__

/*!	\class FileSystemSTD
	$Id: filesystemstd.h,v 1.1 2003/04/30 11:06:17 fruit Exp $

	FileSystemSTD - The 'Standard' File System

	OK, this one is really simple. It just loads a file from a
	given base directory ... no fancy conversions, just a simple
	Open("blah").

	NOTE:

	HISTORY:
		13.07.01 - MK
			- Initial version
		29.08.01 - MK
			- Made the Open() accept a 'const char*' instead of just a 'char*'
			- Added some comments
		26.01.02 - MR
			- added ForEachFile
		27.06.02 - MK
			- moved to Zeitgeist
		09.07.02 - MR
			- moved to Kerosin

	TODO:
		- implement ForEachFile platform independent

	TOFIX:
*/

#include <string>
#include <kerosin/fileserver/filesystem.h>

class FileSystemSTD : public kerosin::FileSystem
{
	//
	// Methods
	//
public:
	FileSystemSTD();
	virtual ~FileSystemSTD();

	virtual salt::RFile*	Open(const char* inName);
	virtual bool			SetPath(const char* inPath);
	virtual int				ForEachFile(const char* expression, TCallback callback, void* param);

	//
	// members
	//
protected:
	std::string	mPath;
};

DECLARE_CLASS(FileSystemSTD);

#endif //__FILESYS_STD_H__
