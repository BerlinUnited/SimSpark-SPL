#ifndef PATH_H__
#define PATH_H__

/*! \class Path
	$Id: path.h,v 1.2 2003/04/30 14:21:48 fruit Exp $
	
	Path

	This class serves as a helper for paths within the object hierarchy. It is
	capable of cleaning paths and separating a path into path components.

	HISTORY:
		13.06.2002 MK
			- initial version
		31.08.2002 MK
			- moved to salt

*/

#include <string>
#include <list>

namespace salt
{

class Path
{
	//
	// types
	//
public:
	typedef std::list<std::string>	TStringList;

	//
	// functions
	//
public:
	Path(const std::string &path = "");

	void Set(const std::string &path);

	std::string GetCleanPath(const std::string &sep = "/") const;

	bool IsAbsolute() const;
	bool IsEmpty() const;

	const std::string& Front() const;
	void PopFront();

	const std::string& Back() const;
	void PopBack();
	
private:
	Path(const Path &obj);
	Path& operator=(const Path &obj);

	void Tokenize(const std::string &path);

	//
	// members
	//
public:
protected:
private:
	TStringList mPathComponents;
	bool		mIsAbsolute;
};

} //namespace salt

#endif //PATH_H__
