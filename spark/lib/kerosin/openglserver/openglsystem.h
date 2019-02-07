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
#ifndef KEROSIN_OPENGLSYSTEM_H
#define KEROSIN_OPENGLSYSTEM_H

/*      \class OpenGLSystem

        HISTORY: 04.12.05 - Initial version
*/

#include <zeitgeist/node.h>

namespace kerosin
{

class OpenGLSystem : public zeitgeist::Node
{
    //
    // functions
    //
public:
    //! init the opengl subsystem
    virtual bool Init() = 0;

    //! called periodically to allow for any event processing
    virtual void Update() = 0;

    /** called after each frame in order to swap buffer in
        OpenGLSystems that support double buffering
    */
    virtual void SwapBuffers() = 0;

    //! get a handle for the active window.
    virtual unsigned long int GetWindowHandle() const { return 0; }

    //! return if the GL subsystem locks using OpenGL to get exclusive access.
    virtual bool IsGLLocked() const { return false; }

    //! return if the GL subsystem have received a quit event.
    virtual bool WantsToQuit() const { return false; }

protected:
    //
    // members
    //
protected:
};

DECLARE_ABSTRACTCLASS(OpenGLSystem);

} // namespace kerosin

#endif //KEROSIN_OPENGLSYSTEM_H
