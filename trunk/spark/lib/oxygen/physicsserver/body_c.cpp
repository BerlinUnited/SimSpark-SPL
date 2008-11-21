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

#include "body.h"

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;
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

FUNCTION(Body,setMassParameters)
{
    float inMass; // total mass of the rigid body
    Vector3f inCenter; // mass center in body frame

    // 3x3 inerta tensor in body frame
    // [ I11(0) I12(1) I13(2) ]
    // [ I12(3) I22(4) I23(5) ]
    // [ I13(6) I23(7) I33(8) ]
    // float inI[9];

    if (
        (in.GetSize() < 11)
        )
        {
            return false;
        }

    ParameterList::TVector::const_iterator iter = in.begin();
    if  (
         (! in.AdvanceValue(iter,inMass)) ||
         (! in.AdvanceValue(iter,inCenter))
         )
        {
            return false;
        }

    dMass mass;
    mass.mass = inMass;
    mass.c[0] = inCenter[0];
    mass.c[1] = inCenter[1];
    mass.c[2] = inCenter[2];

    for (int i=0;i<9;++i)
        {
            if (! in.AdvanceValue(iter,mass.I[i]))
                {
                    return false;
                }
        }

    obj->SetMassParameters(mass);
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

FUNCTION(Body,addSphere)
{
    float inDensity;
    float inRadius;
    
    Matrix inMat;
    
    if (
        (in.GetSize() < 2) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inRadius))
       )
    {
        return false;
    }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[2],inMat);
    
    obj->AddSphere(inDensity,inRadius,inMat);
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

FUNCTION(Body,addSphereTotal)
{
    float inMassTotal;
    float inRadius;
    
    Matrix inMat;
    
    if (
        (in.GetSize() < 2) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inRadius))
       )
    {
        return false;
    }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[2],inMat);
    
    obj->AddSphereTotal(inMassTotal,inRadius,inMat);
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

FUNCTION(Body,addBox)
{
    float inDensity;
    Vector3f inSize;

    Matrix inMat;
    
    if (
        (in.GetSize() <= 1) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inSize))
        )
        {
            return false;
        }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[4],inMat);

    obj->AddBox(inDensity,inSize,inMat);
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

FUNCTION(Body,addBoxTotal)
{
    float inMassTotal;
    Vector3f inSize;

    Matrix inMat;
    
    if (
        (in.GetSize() <= 1) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inSize))
        )
        {
            return false;
        }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[4],inMat);

    obj->AddBoxTotal(inMassTotal,inSize,inMat);
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

FUNCTION(Body,addCylinder)
{
    float inDensity;
    float inRadius;
    float inLength;

    Matrix inMat;
    
    if (
        (in.GetSize() < 3) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[3],inMat);
    
    obj->AddCylinder(inDensity,inRadius,inLength,inMat);
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

FUNCTION(Body,addCylinderTotal)
{
    float inMassTotal;
    float inRadius;
    float inLength;

    Matrix inMat;
    
    if (
        (in.GetSize() < 3) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[3],inMat);
    
    obj->AddCylinderTotal(inMassTotal,inRadius,inLength,inMat);
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

FUNCTION(Body,addCappedCylinder)
{
    float inDensity;
    float inRadius;
    float inLength;

    Matrix inMat;
    
    if (
        (in.GetSize() < 3) ||
        (! in.GetValue(in[0],inDensity)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    
    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[3],inMat);
    
    obj->AddCappedCylinder(inDensity,inRadius,inLength,inMat);
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

FUNCTION(Body,addCappedCylinderTotal)
{
    float inMassTotal;
    float inRadius;
    float inLength;

    Matrix inMat;
    
    if (
        (in.GetSize() != 3) ||
        (! in.GetValue(in[0],inMassTotal)) ||
        (! in.GetValue(in[1],inRadius)) ||
        (! in.GetValue(in[2],inLength))
        )
        {
            return false;
        }

    // Matrix is allowed to be missing, defaults to identity
    in.GetValue(in[3],inMat);
    
    obj->AddCappedCylinderTotal(inMassTotal,inRadius,inLength,inMat);
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

    obj->AddTorque(inTorque);
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

    obj->SetPosition(inPos);
    return true;
}

FUNCTION(Body,translateMass)
{
    Vector3f inV;

    if (
        (in.GetSize() == 0) ||
        (! in.GetValue(in.begin(), inV))
        )
        {
            return false;
        }

    obj->TranslateMass(inV);
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
        DEFINE_FUNCTION(addSphere);
        DEFINE_FUNCTION(setSphereTotal);
        DEFINE_FUNCTION(addSphereTotal);
        DEFINE_FUNCTION(setBox);
        DEFINE_FUNCTION(addBox);
        DEFINE_FUNCTION(setBoxTotal);
        DEFINE_FUNCTION(addBoxTotal);
        DEFINE_FUNCTION(setCylinder);
        DEFINE_FUNCTION(addCylinder);
        DEFINE_FUNCTION(setCylinderTotal);
        DEFINE_FUNCTION(addCylinderTotal);
        DEFINE_FUNCTION(setCappedCylinder);
        DEFINE_FUNCTION(addCappedCylinder);
        DEFINE_FUNCTION(setCappedCylinderTotal);
        DEFINE_FUNCTION(addCappedCylinderTotal);
        DEFINE_FUNCTION(setMass);
        DEFINE_FUNCTION(getMass);
        DEFINE_FUNCTION(setVelocity);
        DEFINE_FUNCTION(setAngularVelocity);
        DEFINE_FUNCTION(addForce);
        DEFINE_FUNCTION(addTorque);
        DEFINE_FUNCTION(setPosition);
        DEFINE_FUNCTION(setMassParameters);
        DEFINE_FUNCTION(translateMass);
}
