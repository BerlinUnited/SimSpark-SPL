/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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

#include <oxygen/physicsserver/physicsserver.h>

using namespace boost;
using namespace oxygen;

FUNCTION(PhysicsServer,setJointHeating)
{
    bool enable;
    if ( (in.GetSize() != 1) || (! in.GetValue(in.begin(),enable)) )
    {
        return false;
    }
    obj->SetJointHeating(enable);

    return true;
}

FUNCTION(PhysicsServer,setBatteryDischarge)
{
    bool enable;
    if ( (in.GetSize() != 1) || (! in.GetValue(in.begin(),enable)) )
    {
        return false;
    }
    obj->SetBatteryDischarge(enable);

    return true;
}

void CLASS(PhysicsServer)::DefineClass()
{
        DEFINE_BASECLASS(zeitgeist/Leaf);
        DEFINE_FUNCTION(setJointHeating);
        DEFINE_FUNCTION(setBatteryDischarge);
}
