/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sharedlibrary.h,v 1.3 2003/05/19 21:37:49 fruit Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef SHAREDLIBRARY_H__
#define SHAREDLIBRARY_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/*! \class SharedLibrary
	$Id: sharedlibrary.h,v 1.3 2003/05/19 21:37:49 fruit Exp $
	
	SharedLibrary
	:TODO: Class description for SharedLibrary

	HISTORY:
		20.07.2002 MK
			- initial version

*/
#include "defines.h"

#include <string>
#include <cstdio>

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
	~SharedLibrary()	{	Close();	};

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
