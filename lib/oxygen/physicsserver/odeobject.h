/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.h,v 1.10 2007/06/16 11:01:35 yxu Exp $

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
#ifndef OXYGEN_ODEOBJECT_H
#define OXYGEN_ODEOBJECT_H

#include <oxygen/sceneserver/basenode.h>
//#include <ode/ode.h>
#include "odewrapper.h"

namespace oxygen
{
class Space;
class World;

/** ODEObject is the base of all classes encapsulating ODE concepts
 */
class ODEObject : public BaseNode
{
public:
    //
    // Functions
    //
    ODEObject() : BaseNode() {};
    virtual ~ODEObject() {};
    
    /** returns the nearest parent space ODE handle */
    dSpaceID FindSpaceID();

    /** returns the ODE handle ID of the containing parent space */
    virtual dSpaceID GetParentSpaceID();

protected:
    /** returns the world node */
    boost::shared_ptr<World> GetWorld();

    /** finds the nearest parent space node */
    boost::shared_ptr<Space> GetSpace();

    /** returns the ODE world handle */
    dWorldID GetWorldID();

    /** converts the rotation part of a salt::Matrix to an ODE
        dMatrix3 */
    void ConvertRotationMatrix(const salt::Matrix& rot, dMatrix3& matrix);

    /** coverts the ODE dMatrix3 to the rotation part of a salt::Matrix */
    void ConvertRotationMatrix(const dReal* matrix, salt::Matrix& rot) const;
};

DECLARE_ABSTRACTCLASS(ODEObject);

} //namespace oxygen

#endif //OXYGEN_ODEOBJECT_H
