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
#include "rendercontrol.h"
#include <oxygen/sceneserver/sceneserver.h>

using namespace kerosin;
using namespace std;

FUNCTION(RenderControl, getFramesRendered)
{
    return obj->GetFramesRendered();
}

FUNCTION(RenderControl,setActive)
{
    bool inActive = true;

    if (
        (in.GetSize() < 0) ||
        (in.GetSize() > 1) ||
        ((in.GetSize() == 1) && (! in.GetValue(in[0], inActive)))
        )
        {
            return false;
        }

    obj->SetActive(inActive);
    return true;
}

void CLASS(RenderControl)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/SimControlNode);
    DEFINE_FUNCTION(getFramesRendered);
    DEFINE_FUNCTION(setActive)
}
