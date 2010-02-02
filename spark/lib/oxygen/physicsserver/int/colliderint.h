/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_COLLIDERINT_H
#define OXYGEN_COLLIDERINT_H

#include <oxygen/physicsserver/ode/odewrapper.h>
#include <oxygen/oxygen_defines.h>
#include <salt/matrix.h>
#include <set>
#include <string>

namespace oxygen
{
class Collider;

class OXYGEN_API ColliderInt
{

public:
    /** Gets the pointer to the Collider object that manages the geom
        specified by geomID.
    */
    virtual Collider* GetColliderPointer(long geomID) = 0;
    
    virtual void SetPosition(const salt::Vector3f& globalPos, long geomID) = 0;
    virtual void SetLocalPosition(const salt::Vector3f& pos, long geomID) = 0;
    virtual salt::Vector3f GetPosition(long geomID) const = 0;
    virtual void SetRotation(const salt::Matrix& rot, long geomID) = 0;
    virtual bool Intersect(boost::shared_ptr<Collider> collider, long geomID) = 0;
    virtual long GetParentSpaceID(long geomID) = 0;
    virtual void DestroyGeom(long geomID) = 0;
    virtual void TransformSetGeom(long parentGeomID, long geomID) = 0;
    virtual void SetSpace(long spaceID, long geomID, Collider* collider) = 0;
    virtual void SetBody(long bodyID, long geomID) = 0;
    virtual void RemoveFromSpace(long geomID, long spaceID) = 0;
};

} //namespace oxygen

#endif //OXYGEN_COLLIDERINT_H
