/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: oxygen.cpp,v 1.15 2004/04/14 18:23:39 rollmark Exp $

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
#include "oxygen.h"

using namespace oxygen;
using namespace zeitgeist;

Oxygen::Oxygen(zeitgeist::Zeitgeist& zg)
{
    // scene graph
    zg.GetCore()->RegisterClassObject(new CLASS(SceneServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BaseNode), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Camera), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(FPSController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Scene), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Transform), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SceneImporter), "oxygen/");

    // physics
    zg.GetCore()->RegisterClassObject(new CLASS(PhysicsServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Body), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BodyController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(DragController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(VelocityController), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ODEObject), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Space), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(World), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Joint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BallJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(HingeJoint), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SliderJoint), "oxygen/");

    // collider
    zg.GetCore()->RegisterClassObject(new CLASS(Collider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(BoxCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(CCylinderCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(PlaneCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RayCollider), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(SphereCollider), "oxygen/");

    // collision handler
    zg.GetCore()->RegisterClassObject(new CLASS(CollisionHandler), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ContactJointHandler), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(RecorderHandler), "oxygen/");

    // agentaspect
    zg.GetCore()->RegisterClassObject(new CLASS(AgentAspect), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Effector), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(Perceptor), "oxygen/");

#ifdef HAVE_SPADES_HEADERS
    // spades
    zg.GetCore()->RegisterClassObject(new CLASS(SpadesServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ParamReader), "oxygen/");
#endif

    // monitor
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorSystem), "oxygen/");

    // game control and controlaspects
    zg.GetCore()->RegisterClassObject(new CLASS(BaseParser), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(GameControlServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ControlAspect), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ActionObject), "oxygen/");

    // run the oxygen init script
    zg.GetCore()->GetRoot()->GetScript()->RunInitScript
        (
         "oxygen.rb",
         "lib/oxygen",
         ScriptServer::IS_COMMON
         );
}
