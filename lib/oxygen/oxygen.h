/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: oxygen.h,v 1.4 2003/12/21 23:36:36 fruit Exp $

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
#ifndef OXYGEN_OXYGEN_H
#define OXYGEN_OXYGEN_H

#include "agentaspect/agentaspect.h"
#include "agentaspect/collisionperceptor.h"
#include "agentaspect/effector.h"
#include "agentaspect/perceptor.h"
#include "agentaspect/createeffector.h"

#include "controlaspect/controlaspect.h"

#include "gamecontrolserver/gamecontrolserver.h"
#include "gamecontrolserver/actionobject.h"

#include "physicsserver/body.h"
#include "physicsserver/boxcollider.h"
#include "physicsserver/ccylindercollider.h"
#include "physicsserver/collider.h"
#include "physicsserver/odeobject.h"
#include "physicsserver/physicsserver.h"
#include "physicsserver/planecollider.h"
#include "physicsserver/space.h"
#include "physicsserver/spherecollider.h"
#include "physicsserver/world.h"

#include "sceneserver/basenode.h"
#include "sceneserver/camera.h"
#include "sceneserver/fpscontroller.h"
#include "sceneserver/indexbuffer.h"
#include "sceneserver/scene.h"
#include "sceneserver/sceneserver.h"
#include "sceneserver/transform.h"

#ifdef HAVE_SPADES_HEADERS
#include "spadesserver/spadesserver.h"
#include "spadesserver/paramreader.h"
#endif

#include "monitorserver/monitorserver.h"
#include "monitorserver/monitorsystem.h"

// control aspect
#include <zeitgeist/zeitgeist.h>

namespace oxygen
{

/** This is the main class, which initializes the Oxygen framework. It
     registers all internal classes.
*/
class Oxygen
{
public:
    /** initializes the oxygen framework
        \param zg is a reference to an already initialized zeitgeist framwork
        in which the internal oxygen libraries get installed
    */
    Oxygen(zeitgeist::Zeitgeist& zg);
    virtual ~Oxygen() {}

private:
    Oxygen(const Oxygen& obj);
    Oxygen& operator=(const Oxygen& obj);

};

} //namespace oxygen


#endif // OXYGEN_OXYGEN_H
