/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: effector.cpp,v 1.7 2007/06/14 17:55:19 jboedeck Exp $

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

#include "effector.h"
#include "agentaspect.h"

using namespace oxygen;
using namespace boost;

shared_ptr<AgentAspect>
Effector::GetAgentAspect()
{
  return shared_static_cast<AgentAspect>
    (make_shared(GetParentSupportingClass("AgentAspect")));
}

bool Effector::Realize(boost::shared_ptr<ActionObject> action)
{
    if ( action.get() == 0 )
        return false;
    mAction = action;
    return true;
}
