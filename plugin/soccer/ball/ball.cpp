/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ball.cpp,v 1.1.2.1 2004/01/20 19:06:47 rollmark Exp $

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
#include "ball.h"
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

void Ball::OnLink()
{
  Transform::OnLink();
  SetName("Ball");

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
              << "ERROR: (Ball) cannot create Sphere\n";
          return;
      }

  physics->SetName("_physics");
  AddChildReference(physics);
  geometry->SetRadius(0.3f);

  geometry->SetName("_geometry");
  AddChildReference(geometry);
}
