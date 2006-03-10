/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Mon May 9 2005
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: pantilteffector_c.cpp,v 1.1 2006/03/10 13:46:47 fruit Exp $

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

#include "pantilteffector.h"

using namespace oxygen;

FUNCTION(PanTiltEffector,setMaxPanAngleDelta)
{
    int inMaxPanAngleDelta;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inMaxPanAngleDelta))
        )
    {
        return false;
    }

    obj->SetMaxPanAngleDelta(inMaxPanAngleDelta);
    return true;
}

FUNCTION(PanTiltEffector,setMaxTiltAngleDelta)
{
    int inMaxTiltAngleDelta;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inMaxTiltAngleDelta))
        )
    {
        return false;
    }

    obj->SetMaxTiltAngleDelta(inMaxTiltAngleDelta);
    return true;
}

FUNCTION(PanTiltEffector,setSigma)
{
    float inSigma;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inSigma))
        )
    {
        return false;
    }

    obj->SetSigma(inSigma);
    return true;
}

void
CLASS(PanTiltEffector)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/Effector);
    DEFINE_FUNCTION(setSigma);
    DEFINE_FUNCTION(setMaxPanAngleDelta);
    DEFINE_FUNCTION(setMaxTiltAngleDelta);
}
