/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: universaljoint_c.cpp,v 1.2 2004/04/15 18:31:24 rollmark Exp $

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
#include "universaljoint.h"

using namespace boost;
using namespace oxygen;
using namespace salt;


FUNCTION(UniversalJoint,setAnchor)
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

FUNCTION(UniversalJoint,getAngle)
{
    int inAxisIndex;

    if (
        (in.GetSize() != 0) ||
        (! in.GetValue(in.begin(), inAxisIndex))
        )
        {
            return false;
        }

    obj->GetAngle(static_cast<Joint::EAxisIndex>(inAxisIndex));
    return true;
}

FUNCTION(UniversalJoint,getAngleRate)
{
    int inAxisIndex;

    if (
        (in.GetSize() != 0) ||
        (! in.GetValue(in.begin(), inAxisIndex))
         )
    {
        return false;
    }

    obj->GetAngleRate(static_cast<Joint::EAxisIndex>(inAxisIndex));
    return true;
}

void CLASS(UniversalJoint)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Joint);
    DEFINE_FUNCTION(setAnchor);
    DEFINE_FUNCTION(getAngle);
    DEFINE_FUNCTION(getAngleRate);
}
