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

#include <oxygen/physicsserver/genericphysicsobjects.h>
#include <oxygen/physicsserver/physicsobject.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/int/physicsobjectint.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

boost::shared_ptr<PhysicsObjectInt> PhysicsObject::mPhysicsObjectImp;

PhysicsObject::PhysicsObject() : BaseNode()
{

}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::OnLink()
{
    if (mPhysicsObjectImp.get() == 0)
        mPhysicsObjectImp = dynamic_pointer_cast<PhysicsObjectInt>
            (GetCore()->New("PhysicsObjectImp"));
}

void PhysicsObject::OnUnlink()
{
    DestroyPhysicsObject();
}

/** returns the world node */
boost::shared_ptr<World> PhysicsObject::GetWorld() const
{
    boost::shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Scene node\n";
            return boost::shared_ptr<World>();
        }

    boost::shared_ptr<World> worldNode = dynamic_pointer_cast<World>
        (scene->GetChildOfClass("World"));
    if (worldNode.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no World node\n";
        }

    return worldNode;
}

boost::shared_ptr<Space> PhysicsObject::GetSpace()
{
    // try to find the nearest parent space object
    boost::weak_ptr<Space> parentSpace = FindParentSupportingClass<Space>();
    if (! parentSpace.expired())
        {
            return parentSpace.lock();
        }

    // return the global space instance
    boost::shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Scene node\n";
            return boost::shared_ptr<Space>();
        }

    boost::shared_ptr<Space> spaceNode = dynamic_pointer_cast<Space>
        (scene->GetChildOfClass("Space"));
    if (spaceNode.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Space node\n";
        }

    return spaceNode;
}

long PhysicsObject::GetWorldID()
{    
    boost::shared_ptr<World> world = GetWorld();
    
    if (world.get() == 0)
        {
            return 0;
        }

    long worldID = world->GetWorldID();
    
    if (worldID == 0)
        {
            GetLog()->Error()
                << "(ODEObject) ERROR: World returned empty ODE handle\n";
        }

    return worldID;
}

long PhysicsObject::FindSpaceID()
{
    boost::shared_ptr<Space> space = GetSpace();
    if (space.get() == 0)
        {
            return 0;
        }

    long spaceID = space->GetSpaceID();

    if (spaceID == 0)
        {
            GetLog()->Error()
                << "(ODEObject) ERROR: Space returned empty ODE handle\n";
        }

    return spaceID;
}

long PhysicsObject::GetParentSpaceID()
{
    return 0;
}

void PhysicsObject::ConvertRotationMatrix(const salt::Matrix& rot, GenericPhysicsMatrix& matrix)
{
    mPhysicsObjectImp->ConvertRotationMatrix(rot, matrix);
}

void PhysicsObject::ConvertRotationMatrix(const GenericPhysicsMatrix* matrix, salt::Matrix& rot) const
{
    mPhysicsObjectImp->ConvertRotationMatrix(matrix, rot);
}
