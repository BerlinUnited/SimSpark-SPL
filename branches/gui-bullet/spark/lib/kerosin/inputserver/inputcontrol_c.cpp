/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: inputcontrol_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "inputcontrol.h"

using namespace kerosin;
using namespace std;

FUNCTION(InputControl, setAdvanceTime)
{
    bool inAdvance;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inAdvance))
        )
        {
            return false;
        }

    obj->SetAdvanceTime(inAdvance);
    return true;
}

FUNCTION(InputControl, getAdvanceTime)
{
    return obj->GetAdvanceTime();
}


FUNCTION(InputControl,setFPSController)
{
    string inPath;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inPath))
        )
        {
            return false;
        }

    obj->SetFPSController(inPath);

    return true;
}

void CLASS(InputControl)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/SimControlNode);
    DEFINE_FUNCTION(setAdvanceTime);
    DEFINE_FUNCTION(getAdvanceTime);
    DEFINE_FUNCTION(setFPSController);
}
