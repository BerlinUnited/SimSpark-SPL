/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: kerosin.h,v 1.8 2004/03/04 13:49:37 rollmark Exp $

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

#include "soundserver/soundserver.h"
#include "inputserver/inputserver.h"
#include "imageserver/imageserver.h"
#include "fontserver/fontserver.h"
#include "fontserver/font.h"
#include "openglserver/openglserver.h"
#include "renderserver/renderserver.h"
#include "textureserver/textureserver.h"
#include "textureserver/texture.h"
#include "materialserver/materialserver.h"
#include "materialserver/material.h"

// scene graph
#include "sceneserver/axis.h"
#include "sceneserver/light.h"
#include "sceneserver/sphere.h"
#include "sceneserver/staticmesh.h"

// control aspect
#include <zeitgeist/zeitgeist.h>

namespace kerosin
{

/** This is the main class, which initializes the Kerosin framework. It
 * registers all internal classes and automatically runs the default init
 * script 'sys/script/default.rb' if it exists.
*/
class Kerosin
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
