/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: objectstate.cpp,v 1.1.2.1 2004/02/07 18:43:37 fruit Exp $

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
#include "objectstate.h"
#include <soccer/soccerbase/soccerbase.h>

using namespace oxygen;

ObjectState::ObjectState() : BaseNode()
{
}

ObjectState::~ObjectState()
{
}

void
ObjectState::SetName(const std::string& name)
{
    mName = name;
}


std::string
ObjectState::GetName() const
{
    return mName;
}

void
ObjectState::SetID(const std::string& id)
{
    mID = id;
}

std::string
ObjectState::GetID() const
{
    return mID;
}

boost::shared_ptr<oxygen::Transform>
ObjectState::GetTransformParent() const
{
    return mTransformParent;
}

void
ObjectState::OnLink()
{
    BaseNode::OnLink();
    SoccerBase::GetTransformParent(*this,mTransformParent);
}

void
ObjectState::OnUnlink()
{
    BaseNode::OnUnlink();
    mTransformParent.reset();
}
