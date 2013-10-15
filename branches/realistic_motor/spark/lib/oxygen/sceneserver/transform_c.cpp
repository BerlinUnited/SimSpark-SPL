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

#include "transform.h"

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;

FUNCTION(Transform,setLocalPos)
{
    Vector3f inPos;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inPos))
        )
        {
            return false;
        }

    obj->SetLocalPos(inPos);
    return true;
}

FUNCTION(Transform,setLocalRotation)
{
    Vector3f inRot;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inRot))
        )
        {
            return false;
        }

    obj->SetLocalRotationDeg(inRot);
    return true;
}

FUNCTION(Transform,setLocalTransform)
{
    // float InM00, float InM01, float InM02, float InM03,
    // float InM10, float InM11, float InM12, float InM13,
    // float InM20, float InM21, float InM22, float InM23,
    // float InM30, float InM31, float InM32, float InM33

    if (in.GetSize() != 16)
        {
            return false;
        }

    float m[16];

    ParameterList::TVector::const_iterator iter = in.begin();
    for (int i=0;i<16;++i)
        {
            if (! in.GetValue(iter,m[i]))
                {
                    return false;
                }
            ++iter;
        }

    obj->SetLocalTransform(Matrix(m));
    return true;
}

void CLASS(Transform)::DefineClass()
{
  DEFINE_BASECLASS(oxygen/BaseNode);
  DEFINE_FUNCTION(setLocalPos);
  DEFINE_FUNCTION(setLocalRotation);
  DEFINE_FUNCTION(setLocalTransform);
}
