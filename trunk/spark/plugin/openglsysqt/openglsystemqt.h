/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: openglsystemwx.h 3 2008-11-21 02:38:08Z hedayat $

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

   OpenGLSystemWX

   HISTORY: 04.12.05 - MR - Initial version
*/
#ifndef OPENGLSYSTEMQT_H__
#define OPENGLSYSTEMQT_H__

#include <kerosin/openglserver/openglsystem.h>

class OpenGLSystemQT : public kerosin::OpenGLSystem
{
    //
    // functions
    //
public:
    OpenGLSystemQT();
    virtual ~OpenGLSystemQT();

    //! init the opengl subsystem
    virtual bool Init();

    //! called periodically to allow for any event processing
    virtual void Update();

    /** called after each frame in order to swap buffer in
        OpenGLSystems that support double buffering
    */
    virtual void SwapBuffers();
};

DECLARE_CLASS(OpenGLSystemQT);

#endif //OPENGLSYSTEMQT_H__
