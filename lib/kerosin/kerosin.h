/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: kerosin.h,v 1.4 2003/09/03 12:15:46 fruit Exp $

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

#ifndef KEROSIN_H__
#define KEROSIN_H__

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
#include "sceneserver/camera.h"
#include "sceneserver/light.h"
#include "sceneserver/staticmesh.h"
#include "sceneserver/transform.h"

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

#endif //KEROSIN_H__
