/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: world_c.cpp,v 1.5 2004/02/21 15:30:44 fruit Exp $

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

#include "world.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setGravity)
{
        if (in.size() == 3)
        {
                World *w = static_cast<World*>(obj);
                w->SetGravity(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2]));
        }
}

FUNCTION(setERP)
{
        if (in.size() == 1)
        {
                World *w = static_cast<World*>(obj);
                w->SetERP(any_cast<float>(in[0]));
        }
}

FUNCTION(setCFM)
{
        if (in.size() == 1)
        {
                World *w = static_cast<World*>(obj);
                w->SetCFM(any_cast<float>(in[0]));
        }
}

FUNCTION(step)
{
        if (in.size() == 1)
        {
                World *w = static_cast<World*>(obj);
                w->Step(any_cast<float>(in[0]));
        }
}

void CLASS(World)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/ODEObject);
        DEFINE_FUNCTION(setGravity);
        DEFINE_FUNCTION(setERP);
        DEFINE_FUNCTION(setCFM);
        DEFINE_FUNCTION(step);
}
