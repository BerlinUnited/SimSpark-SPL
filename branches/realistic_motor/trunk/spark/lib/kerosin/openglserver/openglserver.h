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
#ifndef KEROSIN_OPENGLSERVER_H
#define KEROSIN_OPENGLSERVER_H

#include <string>
#include <set>
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{
#if 0
}
#endif

class MapHolder;
class OpenGLSystem;

class KEROSIN_API OpenGLServer : public zeitgeist::Leaf
{
protected:
    //! set of OpenGL light constants
    typedef std::set<int> TLightSet;

    //
    // functions
    //
public:
    OpenGLServer();
    ~OpenGLServer();

    /** Initializes the OpenGLServer and sets up the OpenGLSystem with
        the given class name. Passing an empty string for
        openGLSysName is correct. In this case OpenGLServer assumes,
        that the OpenGL context is already setup
    */
    bool Init(const std::string& openGLSysName);

    //! if this is called, the application will 'want to quit'
    void Quit();

    //! true if somebody called 'Quit'
    bool WantsToQuit() const;

    //! pump event loop of OpenGLSubSystem
    void Update();

    //! swap opengl buffer
    void SwapBuffers() const;

    /** returns the next availble GL light constant or -1 if no more
        lights are available
    */
    int AllocLight();

    /** marks the GL light constant as available */
    void PutLight(int l);

    /** returns the address of an OpenGL extension by name */
    static void* GetExtension(const char* name);

    /** looks up and calls glActiveTextureARB extension if available */
    static void glActiveTextureARB(unsigned int texture);

    //! return the main window handle from the gl subsystem
    long int GetWindowHandle() const;

    //! return if the GL subsystem locks using OpenGL to get exclusive access
    bool IsGLLocked() const;

protected:
    //! set up opengl viewport
    virtual bool ConstructInternal();

    //
    // members
    //
protected:
    //! a flag, which can be used to control the shutdown of the display window and the application
    bool mWantsToQuit;

    //! cache of loaded vertex and fragment programs (assumes that path names to the programs are unique)
    boost::shared_ptr< MapHolder > mHolder;

    //! flag whether the OpenGL-driver can do fancy lighting or not
    bool mSupportsFancyLighting;

    //! the set of available OpenGL light constants
    TLightSet mAvailableLights;

    //! the OpenGL subsystem used
    boost::shared_ptr<OpenGLSystem> mGLSystem;
};

DECLARE_CLASS(OpenGLServer);

} //namespace kerosin

#endif //KEROSIN_OPENGLSERVER_H
