/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collider.h,v 1.5 2003/11/14 14:05:53 fruit Exp $

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
#ifndef OXYGEN_COLLIDER_H
#define OXYGEN_COLLIDER_H

#include "odeobject.h"

namespace oxygen
{

class Space;
class World;

/** Collider encapsulates an ODE geometry object ('geom' for short). Geoms are
    the fundamental objects in the collision system. They represents a single
    rigid shape (such as a sphere or box), or represent a group of other
    geoms- this is a special kind of geom called a 'space'. A geom is
    associated with rigid body objects. This allows the collision engine to
    get the position and orientation of the geoms from the bodies. A body and
    a geom together represent all the properties of the simulated object.
*/
class Collider : public ODEObject
{
    //
    // Functions
    //
public:
    Collider();
    virtual ~Collider();

protected:

    /** registers the managed geom to the Space of the Scene and our
     * associated ODE body */
    virtual void OnLink();

    /** unregisters the managed geom from the Space of the Scene. The geom is
     * removed from the body later within the destructor.
     */
    virtual void OnUnlink();

    //
    // Members
    //
protected:

    /** the world this collider is associated with */
    boost::shared_ptr<World>    mWorld;

    /** the space this collider is associated with */
    boost::shared_ptr<Space>    mSpace;

    /** the ode collision geometry */
    dGeomID                     mODEGeom;
};

DECLARE_CLASS(Collider);

} //namespace oxygen

#endif //OXYGEN_COLLIDER_H
