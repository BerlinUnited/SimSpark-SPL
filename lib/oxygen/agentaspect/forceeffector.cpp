/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceeffector.cpp,v 1.3.4.1 2003/12/04 17:33:26 rollmark Exp $

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

#include "forceeffector.h"
#include <oxygen/physicsserver/body.h>
#include <oxygen/gamecontrolserver/actionobject.h>

using namespace boost;
using namespace oxygen;
using namespace salt;

ForceEffector::ForceEffector()
{
    mForce.Set(0, 0, 0);
    mMaxForce = 5.0f;
}

bool ForceEffector::Perform(boost::shared_ptr<BaseNode> &base, float )
{
    if (!base) return false;

    // base should be a transform, or some other node, which has a Body-child
    shared_ptr<Body> body = shared_static_cast<Body>(base->GetChildOfClass("Body"));

    if (!body) return false;

    if (mForce.Length() > mMaxForce) mForce = mMaxForce/mForce.Length()*mForce;

    dBodyAddForce(body->GetODEBody(), mForce.x(), mForce.y(), mForce.z());

    mForce.Set(0,0,0);
    return true;
}

void ForceEffector::AddForce(const salt::Vector3f& force)
{
    mForce += force;
}

shared_ptr<ActionObject>
 ForceEffector::GetActionObject(const BaseParser::TPredicate& /*predicate*/)
{
  return shared_ptr<ActionObject>(new ActionObject());
}

