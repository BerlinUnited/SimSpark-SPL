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
#ifndef OXYGEN_OXYGEN_H
#define OXYGEN_OXYGEN_H

#include "agentaspect/agentaspect.h"
#include "agentaspect/effector.h"
#include "agentaspect/perceptor.h"

#include "controlaspect/controlaspect.h"

#include "gamecontrolserver/baseparser.h"
#include "gamecontrolserver/gamecontrolserver.h"
#include "gamecontrolserver/actionobject.h"
#include "gamecontrolserver/behavior.h"

#include "physicsserver/body.h"
#include "physicsserver/bodycontroller.h"
#include "physicsserver/dragcontroller.h"
#include "physicsserver/velocitycontroller.h"
#include "physicsserver/transformcollider.h"
#include "physicsserver/boxcollider.h"
#include "physicsserver/ccylindercollider.h"
#include "physicsserver/collider.h"
#include "physicsserver/odeobject.h"
#include "physicsserver/physicsserver.h"
#include "physicsserver/planecollider.h"
#include "physicsserver/raycollider.h"
#include "physicsserver/space.h"
#include "physicsserver/spherecollider.h"
#include "physicsserver/world.h"
#include "physicsserver/collisionhandler.h"
#include "physicsserver/contactjointhandler.h"
#include "physicsserver/recorderhandler.h"
#include "physicsserver/joint.h"
#include "physicsserver/balljoint.h"
#include "physicsserver/hingejoint.h"
#include "physicsserver/fixedjoint.h"
#include "physicsserver/sliderjoint.h"
#include "physicsserver/universaljoint.h"
#include "physicsserver/hinge2joint.h"

#include "sceneserver/basenode.h"
#include "sceneserver/camera.h"
#include "sceneserver/fpscontroller.h"
#include "sceneserver/scene.h"
#include "sceneserver/sceneserver.h"
#include "sceneserver/transform.h"
#include "sceneserver/sceneimporter.h"

#include "simulationserver/simulationserver.h"
#include "simulationserver/simcontrolnode.h"
#include "simulationserver/netcontrol.h"
#include "simulationserver/netmessage.h"
#include "simulationserver/agentcontrol.h"
#include "simulationserver/monitorcontrol.h"
#include "simulationserver/monitorlogger.h"
#include "simulationserver/traincontrol.h"

#include "geometryserver/geometryserver.h"
#include "geometryserver/meshexporter.h"
#include "geometryserver/meshimporter.h"
#include "geometryserver/stdmeshimporter.h"

#ifdef HAVE_SPADES_HEADERS
#include "spadesserver/spadesserver.h"
#include "spadesserver/paramreader.h"
#endif

#include "monitorserver/monitorserver.h"
#include "monitorserver/monitorsystem.h"
#include "monitorserver/custommonitor.h"
#include "monitorserver/monitoritem.h"
#include "monitorserver/monitorcmdparser.h"

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
    /** initializes the oxygen framework \param zg is a reference to
        an already initialized zeitgeist framework in which the
        internal oxygen libraries get installed
    */
    Oxygen(zeitgeist::Zeitgeist& zg);
    virtual ~Oxygen() {}

private:
    Oxygen(const Oxygen& obj);
    Oxygen& operator=(const Oxygen& obj);

};

} //namespace oxygen


#endif // OXYGEN_OXYGEN_H
