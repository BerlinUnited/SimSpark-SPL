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
#ifndef OXYGEN_SPACE_H
#define OXYGEN_SPACE_H

#include "odeobject.h"
#include <set>

namespace oxygen
{
class Transform;
class Body;
class Collider;

/** Space encapsulates an ODE space object. A space is a non-placeable
    geometry object ('geom') that can contain other geoms. It is
    similar to the rigid body concept of the `world', except that it
    applies to collision instead of dynamics.
*/
class Space : public ODEObject
{
public:
    typedef std::set<dSpaceID> TSpaceIdSet;

public:
    Space();
    virtual ~Space();

    /** returns the ID of the managed ODE space */
    dSpaceID GetODESpace() const;

    /** retuns the ID of joint group for all created contact joints */
    dJointGroupID GetODEJointGroup() const;

    /** starts ODE's collision culling system. ODE will quickly
        identify which pairs of geoms are potentially
        intersecting. Those pairs will be passed to the callback
        function HandleCollide, which in turn will notify the
        responsible collider classes.
    */
    void Collide();

    /** destroy the managed ODE object */
    virtual void DestroyODEObject();

    /** returns the ODE handle ID of the containing parent space */
    virtual dSpaceID GetParentSpaceID();

    /** returns true if this is the top global, i.e. top level space object */
    bool IsGlobalSpace();

    /** enable/disable inner collision */
    void DisableInnerCollision(bool disable);

    /** query disabled inner collision flag */
    bool GetDisableInnerCollision() const;

protected:
    static void collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2);

    /** unregisters the managed Space of the Scene. */
    virtual void OnUnlink();

    /** registers the managed space to the containing parent space */
    virtual void OnLink();

    /** callback to handle a potential collision between two contained
        geoms. It will look up and notify the corresponding colliders
        for a potential collision.
    */
    void HandleCollide(dGeomID obj1, dGeomID obj2);

    /** handle the collision between two geoms from which at least one
        is a space geom
    */
    void HandleSpaceCollide(dGeomID obj1, dGeomID obj2);

    /** creates them managed ODE space and a contact joint group */
    virtual bool ConstructInternal();

    /** updates internal state after physics calculation */
    virtual void PostPhysicsUpdateInternal();

    /** destroys the ODE bodies managed by all Body objects that are
        registered to this Space
    */
    void DestroySpaceObjects();

    //
    // Members
    //
private:
    /** the managed ODE space */
    dSpaceID mODESpace;

    /** the ODE group for all created contact joints */
    dJointGroupID mODEContactGroup;

private:
    /** set of spaces with disabled inner collision */
    static TSpaceIdSet gDisabledInnerCollisionSet;
};

DECLARE_CLASS(Space);

} //namespace oxygen

#endif // OXYGEN_SPACE_H
