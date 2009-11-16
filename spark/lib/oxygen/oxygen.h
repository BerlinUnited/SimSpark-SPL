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

#include "oxygen_defines.h"
#include "agentaspect/agentaspect.h"
#include "agentaspect/effector.h"
#include "agentaspect/perceptor.h"

#include "controlaspect/controlaspect.h"

#include "gamecontrolserver/baseparser.h"
#include "gamecontrolserver/gamecontrolserver.h"
#include "gamecontrolserver/actionobject.h"
#include "gamecontrolserver/behavior.h"

//general includes
#include <oxygen/physicsserver/physicsserver.h>
#include <oxygen/physicsserver/physicsobject.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/boxcollider.h>
#include <oxygen/physicsserver/ccylindercollider.h>
#include <oxygen/physicsserver/planecollider.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/balljoint.h>
#include <oxygen/physicsserver/hingejoint.h>
#include <oxygen/physicsserver/hinge2joint.h>
#include <oxygen/physicsserver/fixedjoint.h>
#include <oxygen/physicsserver/sliderjoint.h>
#include <oxygen/physicsserver/universaljoint.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/bodycontroller.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/contactjointhandler.h>
#include <oxygen/physicsserver/dragcontroller.h>
#include <oxygen/physicsserver/recorderhandler.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/raycollider.h>
#include <oxygen/physicsserver/velocitycontroller.h>

//ODE-specific includes
#include <oxygen/physicsserver/odeobject.h>

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
class OXYGEN_API Oxygen
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
