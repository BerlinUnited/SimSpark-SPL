/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perceptorhandler.cpp,v 1.1.2.1 2004/01/12 14:50:06 rollmark Exp $

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
#include "perceptorhandler.h"
#include <oxygen/physicsserver/collider.h>
#include <oxygen/agentaspect/collisionperceptor.h>
#include <oxygen/sceneserver/transform.h>

using namespace oxygen;
using namespace boost;

void
PerceptorHandler::HandleCollision(dGeomID collidee, dContact& /*contact*/)
{
  // find the first CollisionPerceptor below our closest Transform node
  shared_ptr<Transform> transformParent = shared_static_cast<Transform>
    (make_shared(GetParentSupportingClass("Transform")));

  if (transformParent.get() == 0)
    {
      return;
    }

  shared_ptr<CollisionPerceptor> perceptor =
    shared_static_cast<CollisionPerceptor>
    (transformParent->GetChildOfClass("CollisionPerceptor", true));

  if (perceptor.get() == 0)
    {
      return;
    }

  // now find the closest Transform node above the collidee
  Collider* colNode = Collider::GetCollider(collidee);
  if (colNode == 0)
    {
      return;
    }

  shared_ptr<Transform> colTransformParent = shared_static_cast<Transform>
    (make_shared(colNode->GetParentSupportingClass("Transform")));

  if (colTransformParent.get() == 0)
    {
      return;
    }

  // notify the CollisionPerceptor
  perceptor->AddCollidee(colTransformParent);
}
