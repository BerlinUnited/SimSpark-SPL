/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: oxygen.cpp,v 1.4.2.1 2003/12/25 12:27:55 rollmark Exp $

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
    zg.GetCore()->RegisterClassObject(new CLASS(SceneServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(BaseNode), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Camera), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(FPSController), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Scene), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Transform), "kerosin/");

    // physics
    zg.GetCore()->RegisterClassObject(new CLASS(PhysicsServer), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Body), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(ODEObject), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Space), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(World), "kerosin/");

    // collider
    zg.GetCore()->RegisterClassObject(new CLASS(Collider), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(PlaneCollider), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(SphereCollider), "kerosin/");

    // agentaspect
    zg.GetCore()->RegisterClassObject(new CLASS(AgentAspect), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Effector), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(Perceptor), "kerosin/");

#ifdef HAVE_SPADES_HEADERS
    // spades
    zg.GetCore()->RegisterClassObject(new CLASS(SpadesServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ParamReader), "oxygen/");
#endif

    // monitor
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(MonitorSystem), "oxygen/");

    // game control and controlaspects
    zg.GetCore()->RegisterClassObject(new CLASS(GameControlServer), "oxygen/");
    zg.GetCore()->RegisterClassObject(new CLASS(ControlAspect), "kerosin/");
    zg.GetCore()->RegisterClassObject(new CLASS(ActionObject), "kerosin/");

    // run the oxygen init script
    zg.GetCore()->GetRoot()->GetScript()->RunInitScript
        (
         "oxygen.rb",
         "../../lib/oxygen"
         );
}
