/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sharedlibrary.cpp,v 1.6 2004/06/13 06:30:29 fruit Exp $

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
#include <iostream>

using namespace salt;

bool
SharedLibrary::Open(const std::string &libName)
{
    if (mLibHandle)
    {
        Close();
    }
#if INIT_DEBUG
    std::cerr << "(SharedLibrary) Opening " << libName + ".so\n";
#endif
    mLibHandle = ::dlopen((libName + ".so").c_str(), RTLD_LAZY);

    if (mLibHandle == NULL)
    {
        std::cerr << "(SharedLibrary) ERROR: dlopen faild for " << libName
                  << " with: \n\t" << dlerror() << std::endl;
    }

    return (mLibHandle!=NULL);
}

void*
SharedLibrary::GetProcAddress(const std::string &procName)
{
    if (mLibHandle)
    {
        return ::dlsym(mLibHandle, procName.c_str());
    }
    return NULL;
}

void
SharedLibrary::Close()
{
    if (mLibHandle)
    {
        ::dlclose(mLibHandle);
        mLibHandle = NULL;
    }
}
