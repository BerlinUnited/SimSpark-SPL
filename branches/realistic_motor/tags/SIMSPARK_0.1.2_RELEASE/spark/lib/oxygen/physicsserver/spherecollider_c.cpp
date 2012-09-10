/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

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

#include "spherecollider.h"

using namespace oxygen;
using namespace boost;
using namespace salt;

FUNCTION(SphereCollider,setRadius)
{
    float inRadius;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inRadius))
        )
        {
            return false;
        }

    obj->SetRadius(inRadius);
    return true;
}

FUNCTION(SphereCollider,getRadius)
{
    return obj->GetRadius();
}

FUNCTION(SphereCollider,getPointDepth)
{
    Vector3f inPos;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inPos))
        )
        {
            return 0;
        }

    return obj->GetPointDepth(inPos);
}

void CLASS(SphereCollider)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Collider);
    DEFINE_FUNCTION(setRadius);
    DEFINE_FUNCTION(getRadius);
    DEFINE_FUNCTION(getPointDepth);
}
