/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#include "openglserver.h"
#include "openglwrapper.h"
#include "openglsystem.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <sstream>

using namespace std;

#ifdef __APPLE__
// code below from http://developer.apple.com/qa/qa2001/qa1188.html
#import <mach-o/dyld.h>
void *NSGLGetProcAddress(const char *name)
{
    NSSymbol symbol;
    char *symbolName;
    // Prepend a '_' for the Unix C symbol mangling convention
    symbolName = (char*) malloc (strlen (name) + 2);
    strcpy(symbolName + 1, name);
    symbolName[0] = '_';
    symbol = NULL;
    if (NSIsSymbolNameDefined (symbolName))
        symbol = NSLookupAndBindSymbol (symbolName);
    free (symbolName);
    return symbol ? NSAddressOfSymbol (symbol) : NULL;
}
#endif

namespace kerosin
{
  class MapHolder
  {
  public:
    //! this structure will be used to map program names to OpenGL IDs
#if HAVE_HASH_MAP
    typedef hash_map<string, unsigned int> TProgramCache;
#else
    typedef map<string, unsigned int> TProgramCache;
#endif
    TProgramCache mPrograms;
  };
}

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

OpenGLServer::OpenGLServer() : Leaf(),
                               mWantsToQuit(false), mHolder( new MapHolder() )
{
}

OpenGLServer::~OpenGLServer()
{
}

void OpenGLServer::Quit()
{
    mWantsToQuit = true;
}

bool
OpenGLServer::WantsToQuit() const
{
    return mWantsToQuit;
}

void
OpenGLServer::Update()
{
    if (mGLSystem.get() == 0)
    {
        return;
    }

    mGLSystem->Update();
}

void
OpenGLServer::SwapBuffers() const
{
    if (mGLSystem.get() == 0)
    {
        return;
    }

    mGLSystem->SwapBuffers();
}

bool
OpenGLServer::Init(const string& openGLSysName)
{
    GetLog()->Normal() << "(OpenGLServer) Init " << openGLSysName << "\n";
    mGLSystem.reset();

    if (! openGLSysName.empty())
    {
        // create the OpenGLSystem
        mGLSystem = shared_dynamic_cast<OpenGLSystem>
            (GetCore()->New(openGLSysName));

        if (mGLSystem.get() == 0)
        {
            // could not create OpenGLSystem
            GetLog()->Error() << "(OpenGLServer) ERROR: unable to create "
                              << openGLSysName << "\n";
            return false;
        }

        if (mGLSystem->Init() == false)
        {
            GetLog()->Error() << "(InputServer) ERROR: unable to initialize "
                              << openGLSysName << "\n";
            return false;
        }
    }
    mSupportsFancyLighting = false;

    // prepare the set of available lights
    for (int i=0;i<GL_MAX_LIGHTS;++i)
    {
        mAvailableLights.insert(GL_LIGHT0+i);
    }

    return true;
}

/*!
  Set up the OpenGL viewport, initialize extension registry
*/
bool OpenGLServer::ConstructInternal()
{
    return true;
}

int
OpenGLServer::AllocLight()
{
    if (mAvailableLights.size() == 0)
    {
        return -1;
    }

    TLightSet::iterator iter = mAvailableLights.begin();
    int l = (*iter);
    mAvailableLights.erase(iter);

    return l;
}

void
OpenGLServer::PutLight(int l)
{
    glDisable(l);
    mAvailableLights.insert(l);
}

// Maybe see also http://rainwarrior.thenoos.net/dragon/sdl_glsl.html
// and http://www.evl.uic.edu/arao/cs594/sdlglsl.html to rework the stuff here more platform independently
// The code below uses stuff from the apple developer website (hidden in openglwrapper.h)
void* OpenGLServer::GetExtension(const char* name)
{
#ifdef WIN32
    return wglGetProcAddress(name);
#elif defined(__APPLE__)
    return NSGLGetProcAddress(name);
#else
    return reinterpret_cast<void*>(glXGetProcAddress((unsigned char*)name));
#endif
}

#define PROC_ADDRESS(_ptr, _function)\
    static _ptr proc = (_ptr) GetExtension(#_function);

void
OpenGLServer::glActiveTextureARB(unsigned int texture)
{
#ifdef __APPLE__
    // this is ugly, but I don't know where are the function prototypes in mac os X?
    return ::glActiveTextureARB(texture);
#else
    PROC_ADDRESS(PFNGLACTIVETEXTUREARBPROC, "glActiveTextureARB");

    if (! proc)
        {
            return;
        }

    (proc)(static_cast<GLenum>(texture));
#endif
}

long int
OpenGLServer::GetWindowHandle() const
{
    if (mGLSystem.get() == 0)
    {
        return 0;
    }
    return mGLSystem->GetWindowHandle();
}

bool
OpenGLServer::IsGLLocked() const
{
    if (mGLSystem.get() == 0)
    {
        return true;
    }
    return mGLSystem->IsGLLocked();
}
