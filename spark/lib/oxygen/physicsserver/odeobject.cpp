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
#include <oxygen/physicsserver/odeobject.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/sceneserver/scene.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;

ODEObject::ODEObject() : PhysicsObject()
{
}

ODEObject::~ODEObject()
{
}

void ODEObject::OnUnlink()
{
    DestroyODEObject();
}

/** returns the world node */
shared_ptr<World> ODEObject::GetWorld()
{
    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Scene node\n";
            return shared_ptr<World>();
        }

    shared_ptr<World> worldNode = shared_dynamic_cast<World>
        (scene->GetChildOfClass("World"));
    if (worldNode.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no World node\n";
        }

    return worldNode;
}

shared_ptr<Space> ODEObject::GetSpace()
{
    // try to find the nearest parent space object
    weak_ptr<Space> parentSpace = FindParentSupportingClass<Space>();
    if (! parentSpace.expired())
        {
            return parentSpace.lock();
        }

    // return the global space instance
    shared_ptr<Scene> scene = GetScene();
    if (scene.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Scene node\n";
            return shared_ptr<Space>();
        }

    shared_ptr<Space> spaceNode = shared_dynamic_cast<Space>
        (scene->GetChildOfClass("Space"));
    if (spaceNode.get() == 0)
        {
            GetLog()->Error() << "(ODEObject) ERROR: found no Space node\n";
        }

    return spaceNode;
}

dWorldID ODEObject::GetWorldID()
{
    shared_ptr<World> world = GetWorld();
    if (world.get() == 0)
        {
            return 0;
        }

    dWorldID worldId = world->GetODEWorld();
    if (worldId == 0)
        {
            GetLog()->Error()
                << "(ODEObject) ERROR: World returned empty ODE handle\n";
        }

    return worldId;
}

dSpaceID ODEObject::FindSpaceID()
{
    shared_ptr<Space> space = GetSpace();
    if (space.get() == 0)
        {
            return 0;
        }

    dSpaceID spaceId = space->GetODESpace();

    if (spaceId == 0)
        {
            GetLog()->Error()
                << "(ODEObject) ERROR: Space returned empty ODE handle\n";
        }

    return spaceId;
}

dSpaceID ODEObject::GetParentSpaceID()
{
    return 0;
}

void ODEObject::ConvertRotationMatrix(const salt::Matrix& rot, dMatrix3& matrix)
{
    matrix[0] = rot.m[0];
    matrix[1] = rot.m[4];
    matrix[2] = rot.m[8];
    matrix[3] = 0;
    matrix[4] = rot.m[1];
    matrix[5] = rot.m[5];
    matrix[6] = rot.m[9];
    matrix[7] = 0;
    matrix[8] = rot.m[2];
    matrix[9] = rot.m[6];
    matrix[10] = rot.m[10];
    matrix[11] = 0;
}

void ODEObject::ConvertRotationMatrix(const dReal* matrix, salt::Matrix& rot) const
{
    rot.m[0] = matrix[0] ;
    rot.m[4] = matrix[1] ;
    rot.m[8] = matrix[2];
    rot.m[12] = matrix[3];
    rot.m[1] = matrix[4];
    rot.m[5] = matrix[5];
    rot.m[9] = matrix[6];
    rot.m[13] = matrix[7];
    rot.m[2] = matrix[8];
    rot.m[6] = matrix[9];
    rot.m[10] = matrix[10] ;
    rot.m[14] = matrix[11];
    rot.m[3] = 0.0;
    rot.m[7] = 0.0;
    rot.m[11] = 0.0;
    rot.m[15] = 1.0;
}
