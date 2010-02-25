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

   Kerosin

        HISTORY:
                11.07.2002 MK
                        - initial version

*/
#ifndef KEROSIN_KEROSIN_H
#define KEROSIN_KEROSIN_H

#include "kerosin_defines.h"
#include "soundserver/soundserver.h"
#include "imageserver/imageserver.h"
#include "fontserver/fontserver.h"
#include "fontserver/font.h"

#include "inputserver/inputserver.h"
#include "inputserver/inputsystem.h"
#include "inputserver/inputdevice.h"
#include "inputserver/inputcontrol.h"
#include "inputserver/inputitem.h"

#include "openglserver/openglserver.h"
#include "openglserver/openglsystem.h"
#include "renderserver/renderserver.h"
#include "renderserver/rendercontrol.h"
#include "renderserver/rendernode.h"
#include "textureserver/textureserver.h"
#include "textureserver/texture.h"
#include "materialserver/materialserver.h"
#include "materialserver/material.h"
#include "materialserver/material2dtexture.h"
#include "materialserver/materialexternal.h"
#include "materialserver/materialsolid.h"
#include "materialserver/materialexporter.h"

// scene graph
#include "sceneserver/singlematnode.h"
#include "sceneserver/box.h"
#include "sceneserver/axis.h"
#include "sceneserver/light.h"
#include "sceneserver/sphere.h"
#include "sceneserver/capsule.h"
#include "sceneserver/cylinder.h"
#include "sceneserver/staticmesh.h"

// control aspect
#include <zeitgeist/zeitgeist.h>

namespace kerosin
{

/** This is the main class, which initializes the Kerosin framework. It
 * registers all internal classes and automatically runs the default init
 * script 'sys/script/default.rb' if it exists.
*/
class KEROSIN_API Kerosin
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
    /** initializes the kerosin framework
        \param zg is a reference to an already initialized zeitgeist framwork
        in which the internal kerosin libraries get installed
    */
    Kerosin(zeitgeist::Zeitgeist &zg);
    virtual ~Kerosin() {};
protected:
private:
    Kerosin(const Kerosin& obj);
    Kerosin& operator=(const Kerosin& obj);

    //
    // members
    //
public:
protected:
private:
};

} //namespace kerosin

#endif //KEROSIN_KEROSIN_H
