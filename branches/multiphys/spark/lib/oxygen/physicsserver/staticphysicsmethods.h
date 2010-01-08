/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint.h 107 2009-11-25 06:09:10Z a-held $

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
#ifndef OXYGEN_STATICPHYSICSMETHODS_H
#define OXYGEN_STATICPHYSICSMETHODS_H

#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class Collider;
class Joint;
class RigidBody;
/** StaticPhysicsMethods is a workaround class. It defines static 
    engine-specific methods which are called from within other
    static methods in engine-unspecific classes.
    
    The StaticPhysicsMethods class should be implemented once for each 
    supported engine. The preprocessor must decide which file to use.
*/

class OXYGEN_API StaticPhysicsMethods{

public:    
    /** Gets the pointer to the Collider object that manages the geom
        specified by geomID.
    */
    static Collider* GetColliderPointer(long geomID);
    
    /** Gets the pointer to the Joint object that manages the joint
        specified by jointID.
    */
    static Joint* GetJoint(long jointID);
    
    /** Gets the pointer to the RigidBody object that manages the rigid body
        specified by bodyID.
    */              
    static RigidBody* GetBodyPointer(long bodyID);
    
    /** Checks if two bodies, specified by bodyID1 and bodyID2, are
        connected by a joint.
    */
    static bool AreConnected(long bodyID1, long bodyID2);
    
    /** Checks if two bodies, specified by bodyID1 and bodyID2, are
        connected by a joint that is not of the type specified by joint_type.
    */
    static bool AreConnectedExcluding(long bodyID1, long bodyID2, int joint_type);
};

} //namespace oxygen

#endif //OXYGEN_GENERICPHYSICSOBJECTS_H
