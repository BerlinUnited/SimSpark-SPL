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
#include <oxygen/physicsserver/hingejoint.h>

using namespace boost;
using namespace oxygen;
using namespace salt;


FUNCTION(HingeJoint,setAnchor)
{
    Vector3f inAnchor;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inAnchor))
        )
        {
            return false;
        }

    obj->SetAnchor(inAnchor);
    return true;
}

FUNCTION(HingeJoint,getAngle)
{
    return obj->GetAngle();
}

FUNCTION(HingeJoint,getAngleRate)
{
    return obj->GetAngleRate();
}

FUNCTION(HingeJoint,setAxis)
{
    if (in.GetSize() == 1)
    {
        // 0 = x-axis, 1 = y-axis, 2 = z-axis
        int inAxis;

        // test if we got a parameter and get the value
        if (
            /*(in.GetSize() == 0) ||*/
            (! in.GetValue(in.begin(), inAxis))
            )
            {
                return false;
            }

        obj->SetAxis(static_cast<Joint::EAxisIndex>(inAxis));
    	
        return true;
    }
    else if (in.GetSize() == 3)
    {
        Vector3f axis;
        if (! in.GetValue(in[0], axis.x()) ||
            ! in.GetValue(in[1], axis.y()) ||
            ! in.GetValue(in[2], axis.z()))
        {
            return false;
        }
        
        obj->SetAxis(axis);      
 
        return true;
    }
    else return false;
}

void CLASS(HingeJoint)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Generic6DOFJoint);
    DEFINE_FUNCTION(setAnchor);
    DEFINE_FUNCTION(getAngle);
    DEFINE_FUNCTION(getAngleRate);
    DEFINE_FUNCTION(setAxis);
}
