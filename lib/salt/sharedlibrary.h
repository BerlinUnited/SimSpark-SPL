#ifndef SHAREDLIBRARY_H__
#define SHAREDLIBRARY_H__

/*! \class SharedLibrary
	$Id: sharedlibrary.h,v 1.1 2003/04/30 11:29:34 fruit Exp $
	
	SharedLibrary
	:TODO: Class description for SharedLibrary

	HISTORY:
		20.07.2002 MK
			- initial version

*/

#include <string>
#include "defines.h"
#include <stdio.h>

namespace salt
{

class SharedLibrary
{
	//
	// types
	//
public:
protected:
private:

	//
	// functions
	//
public:
	SharedLibrary() : mLibHandle(0) {};
	~SharedLibrary()	{	printf("~SharedLibrary\n"); Close();	};

	bool	Open(const std::string &libName);
	void*	GetProcAddress(const std::string &procName);
	void	Close();

	//
	// members
	//
private:
	void	*mLibHandle;
};

} //namespace salt

#endif //SHAREDLIBRARY_H__
