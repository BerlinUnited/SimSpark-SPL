/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: hingejoint_c.cpp,v 1.2 2004/04/14 18:25:08 rollmark Exp $

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
#include "hingejoint.h"

using namespace boost;
using namespace oxygen;
using namespace salt;


FUNCTION(HingeJoint,setHingeAnchor)
{
    Vector3f inAnchor;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inAnchor))
        )
        {
            return false;
        }

    obj->SetHingeAnchor(inAnchor);
    return true;
}

FUNCTION(HingeJoint,getHingeAngle)
{
    return obj->GetHingeAngle();
}

FUNCTION(HingeJoint,getHingeAngleRate)
{
    return obj->GetHingeAngleRate();
}

void CLASS(HingeJoint)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Joint);
    DEFINE_FUNCTION(setHingeAnchor);
    DEFINE_FUNCTION(getHingeAngle);
    DEFINE_FUNCTION(getHingeAngleRate);
}
