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
#include "perceptorhandler.h"
#include "collisionperceptor.h"
#include <oxygen/physicsserver/collider.h>
#include <oxygen/sceneserver/transform.h>

using namespace oxygen;
using namespace boost;

void 
PerceptorHandler::OnLink()
{
    // find the first CollisionPerceptor below our closest Transform node
    boost::shared_ptr<Transform> transformParent = static_pointer_cast<Transform>
        (FindParentSupportingClass<Transform>().lock());    

  if (transformParent.get() == 0)
    {
      return;
    }

  //boost::shared_ptr<CollisionPerceptor> perceptor =
  //  static_pointer_cast<CollisionPerceptor>
  //  (transformParent->GetChildOfClass("CollisionPerceptor", true));

  mColPercept = dynamic_pointer_cast<CollisionPerceptor>
      (transformParent->GetChildSupportingClass("CollisionPerceptor", true));

  if (mColPercept.get() == 0)
    {
        GetLog()->Error() << "PerceptorHandler: no suitable child node found!\n";
        return;
    }
}

void
PerceptorHandler::OnUnlink()
{
    mColPercept.reset();
}

void
PerceptorHandler::HandleCollision
(boost::shared_ptr<Collider> collidee, GenericContact& /*contact*/)
{
    if (mColPercept.get() == 0)
        return;

  // now find the closest Transform node above the collidee
  boost::shared_ptr<Transform> colTransformParent = static_pointer_cast<Transform>
    (collidee->FindParentSupportingClass<Transform>().lock());

  if (colTransformParent.get() == 0)
    {
      return;
    }

  // notify the CollisionPerceptor
  mColPercept->AddCollidee(colTransformParent);
}
