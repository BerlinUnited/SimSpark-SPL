/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sharedlibrary.h,v 1.4 2003/08/21 08:56:49 rollmark Exp $

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

   SharedLibrary

   Manages a shared library in a common way across different platforms.

   HISTORY:
	20.07.2002 MK
	- initial version

*/
#ifndef SHAREDLIBRARY_H__
#define SHAREDLIBRARY_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "defines.h"
#include <string>
#include <cstdio>

namespace salt
{

/** SharedLibrary defines a commmon interface for the usage of shared
 *  libraries. The implementation of this class depends on the
 *  platform and used compiler. Up to now two implementations for
 *  Windows using the Visual C++ Compiler and Linux using the gcc
 *  compiler are available.
 */
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

  /** constructs a sharedLibrary object */
	SharedLibrary() : mLibHandle(0) {};

  /** destroys a sharedLibrary, releasing a previously opened library */
	~SharedLibrary()	{	Close();	};

  /** opens a sharedLibrary, releasing a previously opened library */
	bool	Open(const std::string &libName);

  /**  returns a pointer to the exported function procName of the
    *  library or NULL if the function does not exist.
    */
	void*	GetProcAddress(const std::string &procName);

  /** releases a previously opened library */
	void	Close();

	//
	// members
	//
private:
  /** a platform dependent handle to the managed library */
	void	*mLibHandle;
};

} //namespace salt

#endif //SHAREDLIBRARY_H__
