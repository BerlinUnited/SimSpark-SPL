/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: body_c.cpp,v 1.3 2003/08/31 21:53:45 fruit Exp $

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

FUNCTION(enable)
{
        if (in.size() == 0)
        {
                Body *b = static_cast<Body*>(obj);
                b->Enable();
        }
}

FUNCTION(disable)
{
        if (in.size() == 0)
        {
                Body *b = static_cast<Body*>(obj);
                b->Disable();
        }
}

FUNCTION(useGravity)
{
        if (in.size() == 1)
        {
                Body *b = static_cast<Body*>(obj);
                b->UseGravity(any_cast<bool>(in[0]));
        }
}

FUNCTION(setMass)
{
        if (in.size() == 1)
        {
                Body *b = static_cast<Body*>(obj);
                b->SetMass(any_cast<float>(in[0]));
        }
}

FUNCTION(setMaxSpeed)
{
        if (in.size() == 1)
        {
                Body *b = static_cast<Body*>(obj);
                b->SetMaxSpeed(any_cast<float>(in[0]));
        }
}

FUNCTION(setSphere)
{
        if (in.size() == 2)
        {
                Body *b = static_cast<Body*>(obj);
                b->SetSphere(any_cast<float>(in[0]), any_cast<float>(in[1]));
        }
}

void CLASS(Body)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/ODEObject);
        DEFINE_FUNCTION(enable);
        DEFINE_FUNCTION(disable);
        DEFINE_FUNCTION(useGravity);
        DEFINE_FUNCTION(setMass);
        DEFINE_FUNCTION(setMaxSpeed);
        DEFINE_FUNCTION(setSphere);
}
