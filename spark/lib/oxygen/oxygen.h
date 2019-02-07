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

#include <oxygen/oxygen_defines.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/agentaspect/effector.h>
#include <oxygen/agentaspect/perceptor.h>

#include <oxygen/controlaspect/controlaspect.h>

#include <oxygen/gamecontrolserver/baseparser.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <oxygen/gamecontrolserver/behavior.h>

//abstract physics classes
#include <oxygen/physicsserver/physicsobject.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/staticbody.h>
#include <oxygen/physicsserver/dynamicbody.h>
#include <oxygen/physicsserver/softbody.h>
#include <oxygen/physicsserver/bodycontroller.h>
#include <oxygen/physicsserver/dragcontroller.h>
#include <oxygen/physicsserver/velocitycontroller.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/boxcollider.h>
#include <oxygen/physicsserver/capsulecollider.h>
#include <oxygen/physicsserver/cylindercollider.h>
#include <oxygen/physicsserver/collider.h>
#include <oxygen/physicsserver/compoundcollider.h>
#include <oxygen/physicsserver/concavecollider.h>
#include <oxygen/physicsserver/conecollider.h>
#include <oxygen/physicsserver/convexcollider.h>
#include <oxygen/physicsserver/emptycollider.h>
#include <oxygen/physicsserver/physicsserver.h>
#include <oxygen/physicsserver/planecollider.h>
#include <oxygen/physicsserver/raycollider.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/contactjointhandler.h>
#include <oxygen/physicsserver/recorderhandler.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/generic6dofjoint.h>
#include <oxygen/physicsserver/angularmotor.h>
#include <oxygen/physicsserver/balljoint.h>
#include <oxygen/physicsserver/conetwistjoint.h>
#include <oxygen/physicsserver/hingejoint.h>
#include <oxygen/physicsserver/fixedjoint.h>
#include <oxygen/physicsserver/sliderjoint.h>
#include <oxygen/physicsserver/universaljoint.h>
#include <oxygen/physicsserver/hinge2joint.h>
#include <oxygen/physicsserver/battery.h>

#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/camera.h>
#include <oxygen/sceneserver/fpscontroller.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/sceneserver/sceneimporter.h>

#include <oxygen/simulationserver/simulationserver.h>
#include <oxygen/simulationserver/simcontrolnode.h>
#include <oxygen/simulationserver/netcontrol.h>
#include <oxygen/simulationserver/netmessage.h>
#include <oxygen/simulationserver/agentcontrol.h>
#include <oxygen/simulationserver/monitorcontrol.h>
#include <oxygen/simulationserver/monitorlogger.h>
#include <oxygen/simulationserver/traincontrol.h>
#include <oxygen/simulationserver/timersystem.h>

#include <oxygen/geometryserver/geometryserver.h>
#include <oxygen/geometryserver/meshexporter.h>
#include <oxygen/geometryserver/meshimporter.h>
#include <oxygen/geometryserver/stdmeshimporter.h>

#ifdef HAVE_SPADES_HEADERS
#include <oxygen/spadesserver/spadesserver.h>
#include <oxygen/spadesserver/paramreader.h>
#endif

#include <oxygen/monitorserver/monitorserver.h>
#include <oxygen/monitorserver/monitorsystem.h>
#include <oxygen/monitorserver/custommonitor.h>
#include <oxygen/monitorserver/monitoritem.h>
#include <oxygen/monitorserver/monitorcmdparser.h>

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
