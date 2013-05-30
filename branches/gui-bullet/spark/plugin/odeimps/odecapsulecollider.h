/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef ODECAPSULECOLLIDER_H
#define ODECAPSULECOLLIDER_H

#include "odeconvexcollider.h"
#include <oxygen/physicsserver/int/capsulecolliderint.h>

class CapsuleColliderImp : public oxygen::CapsuleColliderInt, public ConvexColliderImp
{
public:
    /** CapsuleCollider encapsulates a capsule geometry
        object. A capsule is like a cylinder except it has
        half-sphere caps at its ends. This feature makes the internal
        collision detection code particularly fast and accurate.
    
        See physicsserver/int/capsulecolliderint.h for documentation 
    */

    CapsuleColliderImp();

    void SetParams(float radius, float length, long geomID);
    void SetRadius(float radius, long geomID);
    void SetLength(float length, long geomID);
    void GetParams(float& radius, float& length, long geomID);
    float GetRadius(long geomID);
    float GetLength(long geomID);
    float GetPointDepth(const salt::Vector3f& pos, long geomID);
    long CreateCapsule();
};

DECLARE_CLASS(CapsuleColliderImp);

#endif //ODECAPSULECOLLIDER_H
