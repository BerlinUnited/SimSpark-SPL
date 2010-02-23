/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.cpp 3 2008-11-21 02:38:08Z hedayat $

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

#include "odephysicsobject.h"

using namespace oxygen;
using namespace boost;
using namespace std;

PhysicsObjectImp::PhysicsObjectImp(){
}

void PhysicsObjectImp::ConvertRotationMatrix(const salt::Matrix& rot, GenericPhysicsMatrix& matrix)
{
    dMatrix3& odeMatrix = (dMatrix3&) matrix;
    
    odeMatrix[0] = rot.m[0];
    odeMatrix[1] = rot.m[4];
    odeMatrix[2] = rot.m[8];
    odeMatrix[3] = 0;
    odeMatrix[4] = rot.m[1];
    odeMatrix[5] = rot.m[5];
    odeMatrix[6] = rot.m[9];
    odeMatrix[7] = 0;
    odeMatrix[8] = rot.m[2];
    odeMatrix[9] = rot.m[6];
    odeMatrix[10] = rot.m[10];
    odeMatrix[11] = 0;
}

void PhysicsObjectImp::ConvertRotationMatrix(const GenericPhysicsMatrix* matrix, salt::Matrix& rot) const
{
    dReal* odeMatrix = (dReal*) matrix;

    rot.m[0] = odeMatrix[0];
    rot.m[4] = odeMatrix[1];
    rot.m[8] = odeMatrix[2];
    rot.m[12] = odeMatrix[3];
    rot.m[1] = odeMatrix[4];
    rot.m[5] = odeMatrix[5];
    rot.m[9] = odeMatrix[6];
    rot.m[13] = odeMatrix[7];
    rot.m[2] = odeMatrix[8];
    rot.m[6] = odeMatrix[9];
    rot.m[10] = odeMatrix[10];
    rot.m[14] = odeMatrix[11];
    rot.m[3] = 0.0;
    rot.m[7] = 0.0;
    rot.m[11] = 0.0;
    rot.m[15] = 1.0;
}
