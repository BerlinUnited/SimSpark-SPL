/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: ccylindercollider.cpp,v 1.4 2004/04/15 16:48:58 rollmark Exp $

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

#include "ccylindercollider.h"

using namespace oxygen;

CCylinderCollider::CCylinderCollider() : Collider()
{
}

void CCylinderCollider::SetParams(float radius, float length)
{
    dGeomCCylinderSetParams (mODEGeom, radius, length);
}

bool CCylinderCollider::ConstructInternal()
{
    if (! Collider::ConstructInternal())
        {
            return false;
        }

    // create a unit capped cylinder
    mODEGeom = dCreateCCylinder (0, 1.0f, 1.0f);

    return (mODEGeom != 0);
}
