/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: openglserver.h,v 1.7 2003/11/14 14:05:52 fruit Exp $

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

// #ifdef HAVE_CONFIG_H
// #include "config.h"
// #endif

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include "glextensionreg.h"

namespace kerosin
{
#if 0
}
#endif

class MapHolder;

class OpenGLServer : public zeitgeist::Leaf
{
    //
    // types
    //
private:
    //! this structure will be used to map program names to OpenGL IDs
// #if HAVE_HASH_MAP
//     typedef std::hash_map<std::string, unsigned int> TProgramCache;
// #else
//     typedef std::map<std::string, unsigned int> TProgramCache;
// #endif
    //
    // functions
    //
public:
    OpenGLServer();
    ~OpenGLServer();

    //! setup application window, initialize OpenGL, autoregister extensions
    bool Init();

    boost::shared_ptr<GLExtensionReg> GetExtensionReg() const;
    //! if this is called, the application will 'want to quit'
    void Quit();
    //! true if somebody called 'Quit'
    bool WantsToQuit() const;
    //! pump SDL event loop
    void Update();
    //! swap opengl buffer
    void SwapBuffers() const;

    //! vertex and fragment program loading
    unsigned int LoadARBProgram(GLenum target, const char* fileName);
    //! vertex and fragment program loading
    unsigned int LoadARBVertexProgram(const char* fileName);
    //! vertex and fragment program loading
    unsigned int LoadARBFragmentProgram(const char* fileName);

    bool SupportsFancyLighting() const { return mSupportsFancyLighting; }
    void ToggleFancyLighting();

protected:
    //! set up opengl viewport
    virtual bool ConstructInternal();

    //
    // members
    //
private:
    //! this contains information on all available extensions
    boost::shared_ptr<GLExtensionReg> mExtensionReg;
    //! a flag, which can be used to control the shutdown of the display window and the application
    bool mWantsToQuit;
    //! cache of loaded vertex and fragment programs (assumes that path names to the programs are unique)
    boost::shared_ptr< MapHolder > mHolder;
    //! flag whether the OpenGL-driver can do fancy lighting or not
    bool mSupportsFancyLighting;
};

DECLARE_CLASS(OpenGLServer);

} //namespace kerosin

#endif //KEROSIN_OPENGLSERVER_H
