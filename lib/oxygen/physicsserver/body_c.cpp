/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body_c.cpp,v 1.8 2004/04/14 18:28:25 rollmark Exp $

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

#include "body.h"

using namespace boost;
using namespace oxygen;
using namespace salt;

FUNCTION(Body,enable)
{
   obj->Enable();
   return true;
}

FUNCTION(Body,disable)
{
    obj->Disable();
    return true;
}

FUNCTION(Body,isEnabled)
{
    return obj->IsEnabled();
}

FUNCTION(Body,useGravity)
{
    bool inB;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inB))
        )
        {
            return false;
        }

    obj->UseGravity(inB);
    return true;
}

FUNCTION(Body,setMass)
{
    float inMass;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inMass))
         )
        {
            return false;
        }

        obj->SetMass(inMass);
        return true;
}

FUNCTION(Body,getMass)
{
    return obj->GetMass();
}

FUNCTION(Body,setSphere)
{
    float inDensity;
    float inRadius;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inRadius))
        )
        {
            return false;
        }

    obj->SetSphere(inDensity,inRadius);
    return true;
}

FUNCTION(Body,setSphereTotal)
{
    float inMassTotal;
    float inRadius;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inRadius))
        )
        {
            return false;
        }

    obj->SetSphereTotal(inMassTotal,inRadius);
    return true;
}

FUNCTION(Body,setBox)
{
    float inDensity;
    Vector3f inSize;

    if (
        (in.GetSize() <= 1) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inSize))
        )
        {
            return false;
        }

    obj->SetBox(inDensity,inSize);
    return true;
}

FUNCTION(Body,setBoxTotal)
{
    float inMassTotal;
    Vector3f inSize;

    if (
        (in.GetSize() <= 1) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inSize))
        )
        {
            return false;
        }

    obj->SetBoxTotal(inMassTotal,inSize);
    return true;
}

FUNCTION(Body,setCylinder)
{
    float inDensity;
    float inRadius;
    float inLength;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    obj->SetCylinder(inDensity,inRadius,inLength);
    return true;
}

FUNCTION(Body,setCylinderTotal)
{
    float inMassTotal;
    float inRadius;
    float inLength;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    obj->SetCylinderTotal(inMassTotal,inRadius,inLength);
    return true;
}

FUNCTION(Body,setCappedCylinder)
{
    float inDensity;
    float inRadius;
    float inLength;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    obj->SetCappedCylinder(inDensity,inRadius,inLength);
    return true;
}

FUNCTION(Body,setCappedCylinderTotal)
{
    float inMassTotal;
    float inRadius;
    float inLength;

    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    obj->SetCappedCylinderTotal(inMassTotal,inRadius,inLength);
    return true;
}


FUNCTION(Body,setVelocity)
{
    Vector3f inVel;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inVel))
        )
        {
            return false;
        }

    obj->SetVelocity(inVel);
    return true;
}

FUNCTION(Body,setAngularVelocity)
{
    Vector3f inVel;

    if (
        (in.GetSize() == 1) ||
        (! in.GetValue(in.begin(), inVel))
        )
        {
            return false;
        }

    obj->SetAngularVelocity(inVel);
    return true;
}

FUNCTION(Body,addForce)
{
    Vector3f inForce;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inForce))
        )
        {
            return false;
        }

    obj->AddForce(inForce);
    return true;
}

FUNCTION(Body,addTorque)
{
    Vector3f inTorque;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inTorque))
        )
        {
            return false;
        }

    obj->AddForce(inTorque);
    return true;
}

FUNCTION(Body,setPosition)
{
    Vector3f inPos;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inPos))
        )
        {
            return false;
        }

    obj->AddForce(inPos);
    return true;
}


void CLASS(Body)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/ODEObject);
        DEFINE_FUNCTION(enable);
        DEFINE_FUNCTION(disable);
        DEFINE_FUNCTION(isEnabled);
        DEFINE_FUNCTION(useGravity);
        DEFINE_FUNCTION(setSphere);
        DEFINE_FUNCTION(setSphereTotal);
        DEFINE_FUNCTION(setBox);
        DEFINE_FUNCTION(setBoxTotal);
        DEFINE_FUNCTION(setCylinder);
        DEFINE_FUNCTION(setCylinderTotal);
        DEFINE_FUNCTION(setCappedCylinder);
        DEFINE_FUNCTION(setCappedCylinderTotal);
        DEFINE_FUNCTION(setMass);
        DEFINE_FUNCTION(getMass);
        DEFINE_FUNCTION(setVelocity);
        DEFINE_FUNCTION(setAngularVelocity);
        DEFINE_FUNCTION(addForce);
        DEFINE_FUNCTION(addTorque);
        DEFINE_FUNCTION(setPosition);
}
