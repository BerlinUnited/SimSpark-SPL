/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint_c.cpp,v 1.4 2004/05/01 11:29:58 rollmark Exp $

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
#include "joint.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

FUNCTION(Joint,attach)
{
    string inPath1;
    string inPath2;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0], inPath1)) ||
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

    return obj->GetCFM(static_cast<Joint::EAxisIndex>(inAxis));
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

    return obj->GetStopCFM(static_cast<Joint::EAxisIndex>(inAxis));
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

    return obj->GetStopERP(static_cast<Joint::EAxisIndex>(inAxis));
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

    return obj->GetSuspensionERP(static_cast<Joint::EAxisIndex>(inAxis));
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

    return obj->GetSuspensionCFM(static_cast<Joint::EAxisIndex>(inAxis));
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

void CLASS(Joint)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/ODEObject);
    DEFINE_FUNCTION(attach);
    DEFINE_FUNCTION(setHighStopDeg);
    DEFINE_FUNCTION(getHighStopDeg);
    DEFINE_FUNCTION(setLowStopDeg);
    DEFINE_FUNCTION(getLowStopDeg);
    DEFINE_FUNCTION(setHighStopPos);
    DEFINE_FUNCTION(getHighStopPos);
    DEFINE_FUNCTION(setLowStopPos);
    DEFINE_FUNCTION(getLowStopPos);
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
}

