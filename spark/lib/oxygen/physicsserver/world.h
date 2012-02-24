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
#ifndef OXYGEN_WORLD_H
#define OXYGEN_WORLD_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/physicsobject.h>

namespace oxygen
{

class WorldInt;

/** World encapsulates a world object. It is a container for
    rigid bodies and joints. Objects in different worlds can not
    interact, for example rigid bodies from two different worlds can
    not collide. All the objects in a world exist at the same point in
    time, thus one reason to use separate worlds is to simulate
    systems at different rates.
*/
class OXYGEN_API World : public PhysicsObject
{
    //
    // Functions
    //
public:
    World();
    virtual ~World();

    /** returns the ID of the managed world */
    long GetWorldID() const;

    /** sets the gravity vector of this world */
    void SetGravity(const salt::Vector3f& gravity);

    /** gets the gravity vector of this world */
    salt::Vector3f GetGravity() const;

    /** sets the Error Reduction Parameter of this world. The ERP
        specifies what proportion of a joint error will be fixed
        during the next simulation step. if ERP=0 then no correcting
        force is applied and the bodies will eventually drift apart as
        the simulation proceeds. If ERP=1 then the simulation will
        attempt to fix all joint error during the next time
        step. However, setting ERP=1 is not recommended, as the joint
        error will not be completely fixed due to various internal
        approximations. A value of ERP=0.1 to 0.8 is recommended (0.2
        is the default).
    */
    void SetERP(float erp);

    /** returns the Error Reduction Parameter of this World. */
    float GetERP() const;

    /** sets the Constraint Force mixing (CFM) value. If CFM is set to
        zero, the constraint will be hard. If CFM is set to a positive
        value, it will be possible to violate the constraint by
        `pushing on it' (for example, for contact constraints by
        forcing the two contacting objects together). In other words
        the constraint will be soft, and the softness will increase as
        CFM increases. Note that setting CFM to a negative value can
        have undesirable bad effects, such as instability.
    */
    void SetCFM(float cfm);

    /** returns the Constraint Force mixing (CFM) value. */
    float GetCFM() const;

    /** steps the world deltatime forward, i.e. performs physics
        simulation for a deltaTime seconds interval.
    */
    void Step(float deltaTime);

    bool GetAutoDisableFlag() const;
    void SetAutoDisableFlag(bool flag);

    /** Set and get the depth of the surface layer around all geometry
        objects. Contacts are allowed to sink into the surface layer up to
        the given depth before coming to rest. The default value is
        zero. Increasing this to some small value (e.g. 0.001) can help
        prevent jittering problems due to contacts being repeatedly made
        and broken.
    */
    void SetContactSurfaceLayer(float depth);
    float GetContactSurfaceLayer() const;

    /** destroy the managed world and all objects in it */
    virtual void DestroyPhysicsObject();

protected:
    /** creates a new physics world */
    virtual bool ConstructInternal();

    /** destroys the bodies managed by all objects that are
        in Wthis World
    */
    void DestroyWorldObjects();

    //
    // Members
    //
private:
    /** Pointer to the implementation of this class.
        If a method contains engine-specific commands, these commands are 
        carried out in a subclass of WorldInt that has the right calls for 
        the desired physics engine.
    */
    static boost::shared_ptr<WorldInt> mWorldImp;
    
    /** The ID of the managed physics world */
    long mWorldID;
};

DECLARE_CLASS(World);

};

#endif //OXYGEN_WORLD_H
