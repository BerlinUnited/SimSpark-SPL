/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: dragcontroller_c.cpp,v 1.1 2004/04/05 08:45:45 rollmark Exp $

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

#include "dragcontroller.h"

using namespace oxygen;

FUNCTION(DragController,getLinearDrag)
{
    return obj->GetLinearDrag();
}

FUNCTION(DragController,setLinearDrag)
{
    float inDrag;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inDrag))
         )
        {
            return false;
        }

        obj->SetLinearDrag(inDrag);
        return true;
}

FUNCTION(DragController,getAngularDrag)
{
    return obj->GetAngularDrag();
}

FUNCTION(DragController,setAngularDrag)
{
    float inDrag;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inDrag))
         )
        {
            return false;
        }

        obj->SetAngularDrag(inDrag);
        return true;
}

void CLASS(DragController)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/BodyController);
    DEFINE_FUNCTION(getLinearDrag);
    DEFINE_FUNCTION(setLinearDrag);
    DEFINE_FUNCTION(getAngularDrag);
    DEFINE_FUNCTION(setAngularDrag);
}
