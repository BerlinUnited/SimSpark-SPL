#ifndef FILESYSTEM_H__
#define FILESYSTEM_H__

/*! \class FileSystem
	$Id: filesystem.h,v 1.3 2003/06/03 12:26:43 fruit Exp $
	
	FileSystem

	This class defines the interface which derived filesystems must implement.


	HISTORY:
		27.06.2002 MK
			- initial version

*/
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <salt/fileclasses.h>


namespace zeitgeist
{

class FileSystem : public Leaf
{
	//
	// types
	//
public:
	typedef void (*TCallback) (char *filename, void *param);

	//
	// functions
	//
public:
	FileSystem() : Leaf()	{}
	virtual ~FileSystem() {}
	virtual salt::RFile*	Open(const char* inName) = 0;
	virtual bool	SetPath(const char* inPath) = 0;

	/*!	iterate over all files managed by this filesystem and pick the one which matches the regular
		expression.
	*/
	virtual int ForEachFile(const char* expression, TCallback callback, void* param) = 0;

private:
	FileSystem(const FileSystem& obj);
	FileSystem& operator=(const FileSystem& obj);
};

DECLARE_ABSTRACTCLASS(FileSystem);

} //namespace zeitgeist

#endif //FILESYSTEM_H__
