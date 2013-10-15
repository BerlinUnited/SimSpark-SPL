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

#ifndef OXYGEN_COLLIDERINT_H
#define OXYGEN_COLLIDERINT_H

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
    virtual ~ColliderInt() {}

    /** Gets the pointer to the Collider object that manages the geom
        specified by geomID.
    */
    virtual Collider* GetColliderPointer(long geomID) = 0;

    /** sets the relative position of the managed geom directly. If
        the geom is connected to a body, the position of the body will
        also be changed
    */
    virtual void SetPosition(const salt::Vector3f& globalPos, long geomID) = 0;

    /** sets the relative position of the managed geom to the body's center.
        Only use for colliders encapsulated in a TransformCollider!
    */
    virtual void SetLocalPosition(const salt::Vector3f& pos, long geomID) = 0;

    /** returns the absolute position of the managed geom */
    virtual salt::Vector3f GetPosition(long geomID) const = 0;

    /** sets the relative orientation of the managed geom directly. If
        the geom is connected to a body, the orientation of the body
        will also be changed
     */
    virtual void SetRotation(const salt::Matrix& rot, long geomID) = 0;

    /** returns true if the geom managed by this
        Collider intersects with the geom managed by the given collider
     */
    virtual bool Intersect(boost::shared_ptr<Collider> collider, long geomID) = 0;

    /** returns the handle ID of the containing parent space */
    virtual long GetParentSpaceID(long geomID) = 0;

    /** Destroys the collider specified by \param geomID */
    virtual void DestroyGeom(long geomID) = 0;

    /** Registers the collider specified by \param geomID to the transform
        collider specified by \param parentGeomID
    */
    virtual void TransformSetGeom(long parentGeomID, long geomID) = 0;

    /** Puts the collider specified by \param geomID into the space
        specified by spaceID. It also registers \param collider as the
        abstract collider object managing this collider.
    */
    virtual void SetSpace(long spaceID, long geomID, Collider* collider) = 0;

    /** Registers the collider specified by \param geomID to the body
        specified by \param bodyID
    */
    virtual void SetBody(long bodyID, long geomID) = 0;

    /** Removes the collider specified by \param geomID from the space
        specified by \param spaceID
    */
    virtual void RemoveFromSpace(long geomID, long spaceID) = 0;
};

} //namespace oxygen

#endif //OXYGEN_COLLIDERINT_H
