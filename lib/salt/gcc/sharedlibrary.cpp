/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sharedlibrary.cpp,v 1.2 2003/05/19 21:32:39 fruit Exp $

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
#include "../sharedlibrary.h"
#include <dlfcn.h>

using namespace salt;

bool SharedLibrary::Open(const std::string &libName)
{
	if (mLibHandle)
	{
		Close();
	}

	mLibHandle = ::dlopen((libName + ".so").c_str(), RTLD_LAZY);

	return (mLibHandle!=NULL);
}

void* SharedLibrary::GetProcAddress(const std::string &procName)
{
	if (mLibHandle)
	{
		return ::dlsym(mLibHandle, procName.c_str());
	}
	return NULL;
}

void SharedLibrary::Close()
{
	if (mLibHandle)
	{
		::dlclose(mLibHandle);
		mLibHandle = NULL;
	}
}
