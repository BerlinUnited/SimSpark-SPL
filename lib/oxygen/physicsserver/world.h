/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: world.h,v 1.3 2003/08/31 21:53:45 fruit Exp $

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

   World
*/
#ifndef OXYGEN_WORLD_H
#define OXYGEN_WORLD_H

#include "odeobject.h"

namespace oxygen
{

/** World encapsulates an ODE world object. It is a container for rigid bodies
    and joints. Objects in different worlds can not interact, for example
    rigid bodies from two different worlds can not collide. All the objects in
    a world exist at the same point in time, thus one reason to use separate
    worlds is to simulate systems at different rates.
*/
class World : public ODEObject
{
    //
    // Functions
    //
public:
    World();
    ~World();

    /** returns the ID of the managed ODE world */
    dWorldID        GetODEWorld() const;

    /** sets the gravity vector of this vorld */
    void    SetGravity(float x, float y, float z);

    /** sets the gavity vector of this world */
    void    SetGravity(const salt::Vector3f &v);

    /** sets the Error Reduction Parameter of this world. The ERP
        specifies what proportion of a joint error will be fixed during
        the next simulation step. if ERP=0 then no correcting force is
        applied and the bodies will eventually drift apart as the
        simulation proceeds. If ERP=1 then the simulation will attempt to
        fix all joint error during the next time step. However, setting
        ERP=1 is not recommended, as the joint error will not be
        completely fixed due to various internal approximations. A value
        of ERP=0.1 to 0.8 is recommended (0.2 is the default).
    */
    void    SetERP(float erp);

    /** returns the Error Reduction Parameter of this World.
     */
    float   GetERP() const;

    /** sets the Constraint Force mixing (CFM) value. If CFM is set to
        zero, the constraint will be hard. If CFM is set to a positive
        value, it will be possible to violate the constraint by `pushing
        on it' (for example, for contact constraints by forcing the two
        contacting objects together). In other words the constraint will
        be soft, and the softness will increase as CFM increases. Note
        that setting CFM to a negative value can have undesirable bad
        effects, such as instability.
    */
    void    SetCFM(float cfm);

    /** returns the Constraint Force mixing (CFM) value. */
    float   GetCFM() const;

    /** steps the world deltatime forward, i.e. performs physics
        simulation for a deltaTime seconds interval.
    */
    void    Step(float deltaTime);

protected:
    /** creates ode dynamics world and makes sure that mODEWorld is valid */
    virtual bool ConstructInternal();

    //
    // Members
    //
private:
    /** the dynamics world represented by this object */
    dWorldID        mODEWorld;
};

    DECLARE_CLASS(World);

};

#endif //OXYGEN_WORLD_H
