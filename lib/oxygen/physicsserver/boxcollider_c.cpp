/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: boxcollider_c.cpp,v 1.8 2004/04/15 18:32:24 rollmark Exp $

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

#include "boxcollider.h"

using namespace boost;
using namespace oxygen;
using namespace salt;

FUNCTION(BoxCollider,setBoxLengths)
{
    Vector3f inExtents;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inExtents))
        )
        {
            return false;
        }

    obj->SetBoxLengths(inExtents);
    return true;
}

FUNCTION(BoxCollider,getBoxLength)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inAxis))
        )
        {
            return 0;
        }

    return obj->GetBoxLength(inAxis);
}

FUNCTION(BoxCollider,getPointDepth)
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

void CLASS(BoxCollider)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Collider);
    DEFINE_FUNCTION(setBoxLengths);
    DEFINE_FUNCTION(getBoxLength);
    DEFINE_FUNCTION(getPointDepth);
}
