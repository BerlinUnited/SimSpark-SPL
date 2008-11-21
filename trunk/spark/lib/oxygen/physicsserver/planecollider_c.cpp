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

#include "planecollider.h"

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace salt;

FUNCTION(PlaneCollider,setParams)
{
    if (in.GetSize() == 4)
        {
            // try to read 4 floats
            float inF[4];
            int i;
            for (i=0;i<4;++i)
                {
                    if (! in.GetValue(in[i],inF[i]))
                        {
                            break;
                        }
                }

            if (i == 4)
                {
                    obj->SetParams(inF[0],inF[1],inF[2],inF[3]);
                    return true;
                }
        }

    // try to read two Vector3f
    Vector3f inPos;
    Vector3f inNormal;

    ParameterList::TVector::const_iterator iter = in.begin();
    if (
        (! in.AdvanceValue(iter,inPos)) ||
        (! in.AdvanceValue(iter,inNormal))
        )
        {
            return false;
        }

    obj->SetParams(inPos,inNormal);
    return true;
}

FUNCTION(PlaneCollider,getPointDepth)
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

void CLASS(PlaneCollider)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Collider);
    DEFINE_FUNCTION(setParams);
    DEFINE_FUNCTION(getPointDepth);
}
