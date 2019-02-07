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

#ifndef ODECOLLIDER_H
#define ODECOLLIDER_H

#include "odephysicsobject.h"
#include <oxygen/physicsserver/int/colliderint.h>

class ColliderImp : public oxygen::ColliderInt, public PhysicsObjectImp
{

public:    
    ColliderImp();
    virtual ~ColliderImp();
    
    /** \class Collider encapsulates a geometry object- geom for
        short. Geoms are the fundamental objects in the collision
        system. They represent a single rigid shape as for example a
        sphere or a box. A special kind of geom called 'space' can
        represent a group of other geoms. A placeable geom can be
        associated with rigid body objects. This allows the collision
        engine to get the position and orientation of the geoms from the
        bodies. A body and a geom together represent all the properties of
        the simulated object.
    
        See physicsserver/int/colliderint.h for documentation.
    */
    
    oxygen::Collider* GetColliderPointer(long geomID);
    void SetPosition(const salt::Vector3f& globalPos, long geomID);
    void SetLocalPosition(const salt::Vector3f& pos, long GeomID);
    salt::Vector3f GetPosition(long geomID) const;
    void SetRotation(const salt::Matrix& rot, long geomID);
    bool Intersect(boost::shared_ptr<oxygen::Collider> collider, long geomID);
    long GetParentSpaceID(long geomID);
    void DestroyGeom(long geomID);
    void TransformSetGeom(long parentGeomID, long geomID);
    void SetSpace(long spaceID, long geomID, oxygen::Collider* collider);
    void SetBody(long bodyID, long geomID);
    void RemoveFromSpace(long geomID, long spaceID);
};

DECLARE_CLASS(ColliderImp);

#endif //ODECOLLIDER_H
