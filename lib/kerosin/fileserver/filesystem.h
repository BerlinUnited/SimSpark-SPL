#ifndef FILESYSTEM_H__
#define FILESYSTEM_H__

/*! \class FileSystem
	$Id: filesystem.h,v 1.1 2003/04/30 11:29:32 fruit Exp $
	
	FileSystem

	This class defines the interface which derived filesystems must implement.


	HISTORY:
		27.06.2002 MK
			- initial version

*/

#include <zeitgeist/base.h>
#include <salt/fileclasses.h>


namespace kerosin
{

class FileSystem : public zeitgeist::Base
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
	FileSystem() : Base()	{}
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

} //namespace kerosin

#endif //FILESYSTEM_H__
