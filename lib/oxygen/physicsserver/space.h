/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h,v 1.4 2003/11/14 14:05:53 fruit Exp $

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

namespace oxygen
{

/** Space encapsulates an ODE space object. A space is a non-placeable
    geometry object ('geom') that can contain other geoms. It is similar to
    the rigid body concept of the `world', except that it applies to collision
    instead of dynamics.
*/
class Space : public ODEObject
{
    //
    // Functions
    //
public:
    Space();
    ~Space();

    /** returns the ID of the managed ODE space */
    dSpaceID        GetODESpace() const;

    /** starts ODE's collision culling system. ODE will quickly identify
        which pairs of geoms are potentially intersecting. Those pairs
        will be passed to the callback function HandleCollide, which in
        turn will notify the responsible collider classes.
    */
    void Collide();

    /** callback to handle a potential collision between two contained
        geoms. It will look up and notify the corresponding colliders
        for a potential collision. For all determined collisions an ODE
        contact joint will be created and inserted into
        mODEContactGroup, an ODE group of joints.
    */
    virtual void HandleCollide(dGeomID obj1, dGeomID obj2);

protected:
    /** creates ode dynamics space and makes sure that mODESpace is valid */
    virtual bool ConstructInternal();

    /** looks up the world this space belongs to and sets up mWorld */
    virtual void OnLink();

    virtual void OnUnlink();

private:
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    //
    // Members
    //
private:
    /** the managed ODE space */
    dSpaceID                mODESpace;

    /** the ODE group for all created contact joints */
    dJointGroupID   mODEContactGroup;

    /** the cached ODE world this space belongs to, updated vias OnLink() */
    dWorldID                mWorld;
};

    DECLARE_CLASS(Space);

} //namespace oxygen

#endif // OXYGEN_SPACE_H
