/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: createeffector.cpp,v 1.2.2.3 2004/01/15 21:00:07 fruit Exp $

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
#include "createeffector.h"
#include "createaction.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/spherecollider.h>

using namespace oxygen;
using namespace boost;

CreateEffector::CreateEffector() : Effector()
{
}

void
CreateEffector::OnLink()
{
    GetScript()->CreateVariable("Agent.Mass", 75.0f);
    GetScript()->CreateVariable("Agent.Radius", 1.0f);
    GetScript()->CreateVariable("Agent.MaxSpeed", 10.0f);
}

float
CreateEffector::GetAgentMass() const
{
    float agentMass = 75.0;
    GetScript()->GetVariable("Agent.Mass", agentMass);

    return agentMass;
}

float
CreateEffector::GetAgentRadius() const
{
    float agentRadius = 1.0;
    GetScript()->GetVariable("Agent.Radius", agentRadius);

    return agentRadius;
}

float
CreateEffector::GetAgentMaxSpeed() const
{
    float agentMaxSpeed = 10.0;
    GetScript()->GetVariable("Agent.MaxSpeed", agentMaxSpeed);

    return agentMaxSpeed;
}

bool CreateEffector::Realize(shared_ptr<ActionObject> action)
{
  shared_ptr<CreateAction> createAction =
    shared_dynamic_cast<CreateAction>(action);

  if (createAction.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (CreateEffector) cannot realize an unknown ActionObject\n";
      return false;
    }

  //
  // construct an agent depending on the type information stored in
  // the CreateAction object; for now just create a default agent
  //

  shared_ptr<AgentAspect> aspect = GetAgentAspect();

  if (aspect.get() == 0)
    {
      GetLog()->Error()
        << "ERROR: (CreateEffector) cannot find the AgentAspect\n";
      return false;
    }

  // move different AgentAspect away from each other
  // position has to be set before adding child references
  static float x = 25.0;
  static float y = 5;
  static float z = 15.0;
  aspect->SetLocalPos(x, y, z);
  x+=10;
  y+=50;

  // construct the nodes below the AgentAspect

  // add a sphere body and collider
  shared_ptr<Body> physics =
      shared_dynamic_cast<Body>(GetCore()->New("kerosin/Body"));

  shared_ptr<SphereCollider>geometry =
      shared_dynamic_cast<SphereCollider>(GetCore()->New("kerosin/SphereCollider"));

  if (
      (physics.get() == 0) ||
      (geometry.get() == 0)
      )
      {
          GetLog()->Error()
              << "ERROR: (CreateEffector) cannot create Sphere\n";
          return false;
      } else
          {
              GetLog()->Warning()
              << "(CreateEffector) creating sphere physics\n";
              physics->SetName("_physics");
              aspect->AddChildReference(physics);
              // should be OK to use 1.0 as density here, because
              // both SetSphere and SetMass do an ODE::dMassAdjust
              physics->SetSphere(1.0, GetAgentRadius());
              physics->SetMass(GetAgentMass());
              physics->SetMaxSpeed(GetAgentMaxSpeed());

              geometry->SetName("_geometry");
              aspect->AddChildReference(geometry);
              geometry->SetRadius(GetAgentRadius());
              GetLog()->Warning()
              << "(CreateEffector) creating sphere physics done\n";
          }

  // add forceeffector
  shared_ptr<Effector> effector =
      shared_dynamic_cast<Effector>(GetCore()->New("ForceEffector"));

  if (effector.get() == 0)
      {
          GetLog()->Error() << "ERROR: (CreateEffector) cannot create the ForceEffector\n";
          return false;
      }

  effector->SetName("_ForceEffector");
  aspect->AddChildReference(effector);

  // add init effector
  effector = shared_dynamic_cast<Effector>(GetCore()->New("InitEffector"));

  if (effector.get() == 0)
  {
      GetLog()->Error() << "ERROR: (CreateEffector) cannot create the InitEffector\n";
      return false;
  }

  effector->SetName("_InitEffector");
  aspect->AddChildReference(effector);

  // add perfect vision perceptor
  shared_ptr<Perceptor> perceptor =
      shared_dynamic_cast<Perceptor>(GetCore()->New("PerfectVisionPerceptor"));

  if (perceptor.get() == 0)
      {
          GetLog()->Error() << "ERROR: (CreateEffector) cannot create the PerfectVisionPerceptor\n";
          return false;
      }

  perceptor->SetName("_PerfectVisionPerceptor");
  aspect->AddChildReference(perceptor);

  // add agent state
  shared_ptr<BaseNode> state =
      shared_dynamic_cast<BaseNode>(GetCore()->New("AgentState"));

  if (state.get() == 0)
      {
          GetLog()->Error() << "ERROR: (CreateEffector) cannot create the AgentState\n";
          return false;
      }

  state->SetName("_AgentState");
  aspect->AddChildReference(state);

  GetLog()->Debug() << "(CreateEffector) created dummy agent" << std::endl;

  return true;
}

shared_ptr<ActionObject>
CreateEffector::GetActionObject(const Predicate& predicate)
{
  if (predicate.name != GetPredicate())
    {
      GetLog()->Error() << "ERROR: (CreateEffector) invalid predicate"
                        << predicate.name << "\n";
      return shared_ptr<ActionObject>(new ActionObject(GetPredicate()));
    }

  //
  // don't care for the supplied parameters for now; the desired agent
  // type should be passed later on and stored in the CreateAction object
  //

  return shared_ptr<CreateAction>(new CreateAction());
}

