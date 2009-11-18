/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: world.h 56 2009-03-17 18:03:47Z hedayat $

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
#ifndef OXYGEN_WORLDIMP_H
#define OXYGEN_WORLDIMP_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/ode/odeobject.h>

namespace oxygen
{

/** World encapsulates an ODE world object. It is a container for
    rigid bodies and joints. Objects in different worlds can not
    interact, for example rigid bodies from two different worlds can
    not collide. All the objects in a world exist at the same point in
    time, thus one reason to use separate worlds is to simulate
    systems at different rates.
*/
class OXYGEN_API WorldImp : public ODEObject
{
    //
    // Functions
    //
public:
    WorldImp() : ODEObject(){};
    virtual ~WorldImp(){};

    /** returns the ID of the managed ODE world */
    virtual dWorldID GetODEWorldImp() const = 0;

    /** sets the gravity vector of this vorld */
    virtual void SetGravityImp(const salt::Vector3f& gravity) = 0;

    /** gets the gravity vector of this world */
    virtual salt::Vector3f GetGravityImp() const = 0;

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
    virtual void SetERPImp(float erp) = 0;

    /** returns the Error Reduction Parameter of this World.
     */
    virtual float GetERPImp() const = 0;

    /** sets the Constraint Force mixing (CFM) value. If CFM is set to
        zero, the constraint will be hard. If CFM is set to a positive
        value, it will be possible to violate the constraint by
        `pushing on it' (for example, for contact constraints by
        forcing the two contacting objects together). In other words
        the constraint will be soft, and the softness will increase as
        CFM increases. Note that setting CFM to a negative value can
        have undesirable bad effects, such as instability.
    */
    virtual void SetCFMImp(float cfm) = 0;

    /** returns the Constraint Force mixing (CFM) value. */
    virtual float GetCFMImp() const = 0;

    /** steps the world deltatime forward, i.e. performs physics
        simulation for a deltaTime seconds interval.
    */
    virtual void StepImp(float deltaTime) = 0;

    virtual bool GetAutoDisableFlagImp() const = 0;
    virtual void SetAutoDisableFlagImp(bool flag) = 0;

    /** Set and get the depth of the surface layer around all geometry
        objects. Contacts are allowed to sink into the surface layer up to
        the given depth before coming to rest. The default value is
        zero. Increasing this to some small value (e.g. 0.001) can help
        prevent jittering problems due to contacts being repeatedly made
        and broken.
    */
    virtual void SetContactSurfaceLayerImp(float depth) = 0;
    virtual float GetContactSurfaceLayerImp() const = 0;

    /** destroy the managed ODE object */
    virtual void DestroyODEObjectImp() = 0;

    /** creates them managed ODE world */
    virtual bool ConstructInternalImp() = 0;
};

DECLARE_ABSTRACTCLASS(WorldImp);

}

#endif //OXYGEN_WORLDIMP_H
