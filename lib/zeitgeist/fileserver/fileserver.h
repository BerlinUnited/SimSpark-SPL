#ifndef FILESERVER_H__
#define FILESERVER_H__

/*!	\class FileServer
	$Id: fileserver.h,v 1.1 2003/04/30 11:29:35 fruit Exp $

	FileServer - global interface for file system access

	What the FileServer does:
		- Load files from a FileSystem
		- Manage different file systems

	The file server is an extremely useful subsystem, since it allows to
	access various implemented file systems. The regular file system is a
	standard directory-based implementation. The ZIP file system can load
	files from within ZIP-files. Before actually being able to open files,
	the FileServer is initialized with several FileSystems, which are
	associated with different paths. For each path, you have to pass in a
	file system. Now, when trying to open a file, each registered file
	system is asked to open the file ... the last one wins. This allows
	for some nice effects.

	File systems are associated with id strings ("STD", "ZIP").

	HISTORY:
		12.07.01 - MK
			- Initial version
		14.07.01 - MK
			- ClassServer was added, so the FileServer doesn't manage the
			  file system factories anymore (only concrete file system instances)
		11.10.01 - MK
			- Made singleton functionality more secure
		20.11.01 - AF
			- added the priority of the file systems : LIFO
			- added the information of the file system id and its path to every mounted file system
			- prevented the odditity to mount a file system more than once
			- added GetFirstFileSystem and GetNextFileSystem to iterate through the mounted file systems
		26.01.02 - MR
			- added ForEachFile
		27.06.02 - MK
			- converted this to the Zeitgeist framework
		09.07.02 - MK
			- converted this to the Kerosin framework
		10.10.02 - MK
			- back to Zeitgeist framework

	TODO:

	TOFIX:
*/


#include <list>
#include "../node.h"
#include "filesystem.h"

namespace zeitgeist
{

class FileServer : public Node
{
	//
	// functions
	//
public:
	FileServer();
	~FileServer();

	// each registered file system is search for a file with this name. 
	// the last mounted are the first searched
	salt::RFile*	Open(const char *inName);

	//! Check if a file exists
	bool	Exist(const char *inName);

	// a file system may be added only once, on each further try nothing is done and false returned
	bool	Mount(const char* inID, const char* inPath);

	// if no file system id is given, for a first try FileSystemSTD is assumed, then the type is ignored
	bool	Unmount(const char* inPath);
	bool	Unmount(const char* inClass, const char* inPath);
	
	/** iterate through files. directory, name and extension give directory, name and extension a file must match.
	  * directory,name and extension may be NULL, in wich case every directory,extension and/or name matches.
	  */
	int ForEachFile(const char* directory, const char* name, const char* extension, FileSystem::TCallback callback, void* param);

private:
	FileServer(const FileServer&);
	FileServer& operator=(const FileServer&);
};

DECLARE_CLASS(FileServer)

} //namespace zeitgeist

#endif //FILESERVER_H__
