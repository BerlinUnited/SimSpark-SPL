/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: physicsserver.cpp,v 1.3.8.1 2004/01/09 13:44:17 rollmark Exp $

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

#include "physicsserver.h"
#include <ode/ode.h>

using namespace oxygen;

PhysicsServer::PhysicsServer()
{
}

PhysicsServer::~PhysicsServer()
{
    // release memory associated with ode
    dCloseODE();
}

Body* PhysicsServer::GetBody(dBodyID id)
{
  return id ?
    static_cast<Body*>(dBodyGetData(id)) : 0;
}
