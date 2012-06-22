/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint_c.cpp 289 2012-01-25 18:03:43Z yxu $

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
#include <oxygen/physicsserver/joint.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

FUNCTION(Joint,attach)
{
    string inPath1;
    string inPath2;

    if (in.GetSize() > 2)
        {
            return false;
        }

    if (
        (in.GetSize() >= 1) &&
        (! in.GetValue(in[0], inPath1))
        )
        {
            return false;
        }

    if (
        (in.GetSize() == 2) &&
        (! in.GetValue(in[1], inPath2))
        )
        {
            return false;
        }

    obj->Attach(inPath1,inPath2);
    return true;
}

FUNCTION(Joint,setLowStopDeg)
{
    int inAxis;
    float inDeg;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inDeg))
        )
        {
            return false;
        }

    obj->SetLowStopDeg(static_cast<Joint::EAxisIndex>(inAxis),inDeg);
    return true;
}

FUNCTION(Joint,getLowStopDeg)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
    {
        return false;
    }

    return obj->GetLowStopDeg(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint,setHighStopDeg)
{
    int inAxis;
    float inDeg;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inDeg))
        )
        {
            return false;
        }

    obj->SetHighStopDeg(static_cast<Joint::EAxisIndex>(inAxis),inDeg);
    return true;
}

FUNCTION(Joint,getHighStopDeg)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
    {
        return false;
    }

    return obj->GetHighStopDeg(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint,setLowStopPos)
{
    int inAxis;
    float inPos;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inPos))
        )
        {
            return false;
        }

    obj->SetLowStopPos(static_cast<Joint::EAxisIndex>(inAxis),inPos);
    return true;
}

FUNCTION(Joint,getLowStopPos)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
    {
        return false;
    }

    return obj->GetLowStopPos(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint,setHighStopPos)
{
    int inAxis;
    float inPos;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inPos))
        )
        {
            return false;
        }

    obj->SetHighStopPos(static_cast<Joint::EAxisIndex>(inAxis),inPos);
    return true;
}

FUNCTION(Joint,getHighStopPos)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
    {
        return false;
    }

    return obj->GetHighStopPos(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setFudgeFactor)
{
    int in_axis;
    float fudge_factor;
   
    if (in.GetSize() != 2 ||
        ! in.GetValue(in[0], in_axis) ||
        ! in.GetValue(in[1], fudge_factor))
    {
        return false;
    }
    
    obj->SetFudgeFactor(static_cast<Joint::EAxisIndex>(in_axis), fudge_factor);
    return true; 
}

FUNCTION(Joint, getFudgeFactor)
{
    int in_axis;
    if (in.GetSize() != 1 ||
        ! in.GetValue(in[0], in_axis))
    {
        return false;
    }

    return obj->GetFudgeFactor(static_cast<Joint::EAxisIndex>(in_axis));
}

FUNCTION(Joint,setBounce)
{
    int inAxis;
    float inBounce;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inBounce))
        )
        {
            return false;
        }

    obj->SetBounce(static_cast<Joint::EAxisIndex>(inAxis),inBounce);
    return true;
}

FUNCTION(Joint, getBounce)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
    {
        return false;
    }

    return obj->GetBounce(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setCFM)
{
    int inAxis;
    float inCFM;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inCFM))
        )
        {
            return false;
        }

    obj->SetCFM(static_cast<Joint::EAxisIndex>(inAxis), inCFM);
    return true;
}

FUNCTION(Joint, getCFM)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return 0;
        }

    return obj->GetCFM(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setStopCFM)
{
    int inAxis;
    float inStopCFM;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inStopCFM))
        )
        {
            return false;
        }

    obj->SetStopCFM(static_cast<Joint::EAxisIndex>(inAxis), inStopCFM);
    return true;
}

FUNCTION(Joint, getStopCFM)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return 0;
        }

    return obj->GetStopCFM(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setStopERP)
{
    int inAxis;
    float inStopERP;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inStopERP))
        )
        {
            return false;
        }

    obj->SetStopERP(static_cast<Joint::EAxisIndex>(inAxis), inStopERP);
    return true;
}

FUNCTION(Joint, getStopERP)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return 0;
        }

    return obj->GetStopERP(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setSuspensionERP)
{
    int inAxis;
    float inSuspensionERP;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inSuspensionERP))
        )
        {
            return false;
        }

    obj->SetSuspensionERP(static_cast<Joint::EAxisIndex>(inAxis), inSuspensionERP);
    return true;
}

FUNCTION(Joint, getSuspensionERP)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return 0;
        }

    return obj->GetSuspensionERP(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setSuspensionCFM)
{
    int inAxis;
    float inSuspensionCFM;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inSuspensionCFM))
        )
        {
            return false;
        }

    obj->SetSuspensionCFM(static_cast<Joint::EAxisIndex>(inAxis), inSuspensionCFM);
    return true;
}

FUNCTION(Joint, getSuspensionCFM)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return 0;
        }

    return obj->GetSuspensionCFM(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setLinearMotorVelocity)
{
    int inAxis;
    float inVel;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inVel))
        )
        {
            return false;
        }

    obj->SetLinearMotorVelocity(static_cast<Joint::EAxisIndex>(inAxis), inVel);
    return true;
}

FUNCTION(Joint, getLinearMotorVelocity)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return false;
        }

    return obj->GetLinearMotorVelocity(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, getAngularMotorVelocity)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return false;
        }

    return obj->GetAngularMotorVelocity(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setAngularMotorVelocity)
{
    int inAxis;
    float inDeg;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inDeg))
        )
        {
            return false;
        }

    obj->SetAngularMotorVelocity(static_cast<Joint::EAxisIndex>(inAxis), inDeg);
    return true;
}

FUNCTION(Joint, setMaxMotorForce)
{
    int inAxis;
    float inF;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inAxis)) ||
        (! in.GetValue(in[1], inF))
        )
        {
            return false;
        }

    obj->SetMaxMotorForce(static_cast<Joint::EAxisIndex>(inAxis), inF);
    return true;
}

FUNCTION(Joint, getMaxMotorForce)
{
    int inAxis;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAxis))
        )
        {
            return false;
        }

    return obj->GetMaxMotorForce(static_cast<Joint::EAxisIndex>(inAxis));
}

FUNCTION(Joint, setJointMaxSpeed1)
{
    float inRad;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inRad))
        )
        {
            return false;
        }

    obj->SetJointMaxSpeed1(inRad);
    return true;
}

FUNCTION(Joint, setJointMaxSpeed2)
{
    float inRad;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inRad))
        )
        {
            return false;
        }

    obj->SetJointMaxSpeed2(inRad);
    return true;
}

FUNCTION(Joint,enableFeedback)
{
    bool inSet;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inSet))
        )
        {
            return false;
        }

    obj->EnableFeedback(inSet);
    return true;
}

void CLASS(Joint)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/PhysicsObject);
    DEFINE_FUNCTION(attach);
    DEFINE_FUNCTION(setHighStopDeg);
    DEFINE_FUNCTION(getHighStopDeg);
    DEFINE_FUNCTION(setLowStopDeg);
    DEFINE_FUNCTION(getLowStopDeg);
    DEFINE_FUNCTION(setHighStopPos);
    DEFINE_FUNCTION(getHighStopPos);
    DEFINE_FUNCTION(setLowStopPos);
    DEFINE_FUNCTION(getLowStopPos);
    DEFINE_FUNCTION(setFudgeFactor);
    DEFINE_FUNCTION(getFudgeFactor);
    DEFINE_FUNCTION(getBounce);
    DEFINE_FUNCTION(setBounce);
    DEFINE_FUNCTION(getCFM);
    DEFINE_FUNCTION(setCFM);
    DEFINE_FUNCTION(getStopCFM);
    DEFINE_FUNCTION(setStopCFM);
    DEFINE_FUNCTION(getStopERP);
    DEFINE_FUNCTION(setStopERP);
    DEFINE_FUNCTION(getSuspensionERP);
    DEFINE_FUNCTION(setSuspensionERP);
    DEFINE_FUNCTION(getSuspensionCFM);
    DEFINE_FUNCTION(setSuspensionCFM);
    DEFINE_FUNCTION(setLinearMotorVelocity);
    DEFINE_FUNCTION(getLinearMotorVelocity);
    DEFINE_FUNCTION(setAngularMotorVelocity);
    DEFINE_FUNCTION(getAngularMotorVelocity);
    DEFINE_FUNCTION(setMaxMotorForce);
    DEFINE_FUNCTION(getMaxMotorForce);
    DEFINE_FUNCTION(setJointMaxSpeed1);
    DEFINE_FUNCTION(setJointMaxSpeed2);
    DEFINE_FUNCTION(enableFeedback);
}

