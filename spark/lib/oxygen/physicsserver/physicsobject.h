/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.h 56 2009-03-17 18:03:47Z hedayat $
   $Id: odeobject.h 56 2009-03-17 18:03:47Z hedayat $

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
#ifndef OXYGEN_PHYSICSOBJECT_H
#define OXYGEN_PHYSICSOBJECT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/odewrapper.h>
#include <oxygen/sceneserver/basenode.h>

namespace oxygen
{
class Space;
class World;

/*PhysicsObjects provides an interface for all classes
encapsulating physics objects.*/

class OXYGEN_API PhysicsObject : public BaseNode
{
public: 
    PhysicsObject() : BaseNode(){};
    virtual ~PhysicsObject(){};
    
    /** This rountine is called, before the hierarchy object is
        removed from the parent. It can be overridden to support
        custom 'unlink' behavior.
    */
    virtual void OnUnlink() = 0;
    
    /** returns the ODE world handle */
    virtual dWorldID GetWorldID() = 0;
    
    /** returns the nearest parent space ODE handle */
    virtual dSpaceID FindSpaceID() = 0;
    
    /** returns the ODE handle ID of the containing parent space */
    virtual dSpaceID GetParentSpaceID() = 0;
    
    /** destroy the managed ODE object */
    virtual void DestroyODEObject() = 0;
    
protected:
    /** returns the world node */
    virtual boost::shared_ptr<World> GetWorld() = 0;
    
    /** finds the nearest parent space node */
    virtual boost::shared_ptr<Space> GetSpace() = 0;
    
    /** converts the rotation part of a salt::Matrix to an ODE
        dMatrix3 */
    virtual void ConvertRotationMatrix(const salt::Matrix& rot, dMatrix3& matrix) = 0;
    
    /** coverts the ODE dMatrix3 to the rotation part of a salt::Matrix */
    virtual void ConvertRotationMatrix(const dReal* matrix, salt::Matrix& rot) const = 0;
};

DECLARE_ABSTRACTCLASS(PhysicsObject);

} //namespace oxygen

#endif //OXYGEN_PHYSICSOBJECT_H
