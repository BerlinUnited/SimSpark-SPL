/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: space.cpp,v 1.3 2003/08/31 12:16:49 rollmark Exp $

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

#include "space.h"
#include "world.h"
#include "body.h"
#include "../sceneserver/scene.h"
#include "../agentaspect/collisionperceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace boost;
using namespace kerosin;

static void collisionNearCallback (void *data, dGeomID obj1, dGeomID obj2)
{
        Space *space = (Space*)data;

        space->HandleCollide(obj1, obj2);
}

Space::Space() :
mODESpace(0), mODEContactGroup(0), mWorld(0)
{
}

Space::~Space()
{
        if (mODEContactGroup)
        {
                //dJointGroupEmpty(mODEContactGroup);
                dJointGroupDestroy(mODEContactGroup);
        }

        // release the ODE space
        if (mODESpace)
                dSpaceDestroy(mODESpace);
}

dSpaceID Space::GetODESpace() const
{
        return mODESpace;
}

void Space::Collide()
{
        // bind collision callback function to this object
        dSpaceCollide(mODESpace, this, collisionNearCallback);
}

void Space::HandleCollide(dGeomID obj1, dGeomID obj2)
{
        // exit without doing anything if the two bodies are connected by a joint
        dBodyID ODEBody1, ODEBody2;

        if (dGeomGetClass(obj1) == dPlaneClass && dGeomGetClass(obj2) == dPlaneClass) return;

        ODEBody1 = dGeomGetBody(obj1);
        ODEBody2 = dGeomGetBody(obj2);

        if (ODEBody1 && ODEBody2 && dAreConnected (ODEBody1, ODEBody2)) return;

        Body *body1 = NULL;
        if (ODEBody1)
                body1 = (Body*)dBodyGetData(ODEBody1);

        Body *body2 = NULL;
        if (ODEBody2)
                body2 = (Body*)dBodyGetData(ODEBody2);

        dContact contact;

        bool camera = false;
        if (dCollide (obj1, obj2, 0, &contact.geom, sizeof(dContactGeom)))
        {
                // this ensures that the camera only collides, but
                // does not affect the simulation
                if (body1 && make_shared(body1->GetParent())->GetChildOfClass("Camera").get() != 0)
                {
                        //ODEBody2 = 0;
                        camera = true;
                } else if (body2 && make_shared(body2->GetParent())->GetChildOfClass("Camera").get() != 0)
                {
                        //ODEBody1 = 0;
                        camera = true;
                }
                if (!camera)
                {
                        shared_ptr<CollisionPerceptor> colPerceptor;
                        // notify a potential collisionperceptor
                        if (body1)
                        {
                                colPerceptor = shared_static_cast<CollisionPerceptor>(make_shared(body1->GetParent())->GetChildOfClass("CollisionPerceptor", true));
                                if (colPerceptor && body2)
                                {
                                        colPerceptor->GetCollidees().push_back(make_shared(body2->GetParent()));
                                }
                        }
                        // notify a potential collisionperceptor
                        if (body2)
                        {
                                colPerceptor = shared_static_cast<CollisionPerceptor>(make_shared(body2->GetParent())->GetChildOfClass("CollisionPerceptor", true));
                                if (colPerceptor && body1)
                                {
                                        colPerceptor->GetCollidees().push_back(make_shared(body1->GetParent()));
                                }
                        }

                        contact.surface.mode = dContactBounce;
                        contact.surface.mu = dInfinity;
                        contact.surface.bounce = 0.8f;
                        contact.surface.bounce_vel = 2.0f;
                }
                else
                {
                        contact.surface.mode = 0;
                        contact.surface.mu = 0;
                }

                dJointID c = dJointCreateContact (mWorld, mODEContactGroup, &contact);
                dJointAttach (c, ODEBody1, ODEBody2);
        }
}

bool Space::ConstructInternal()
{
        if (!ODEObject::ConstructInternal()) return false;

        // create the ode space, 0 indicates that this space should
        // not be inserted into another space, i.e. we always create a
        // toplevel space object
        mODESpace = dHashSpaceCreate(0);
        if (mODESpace == 0)
            {
                return false;
            }

        // create a joint group for the contacts
        mODEContactGroup = dJointGroupCreate(0);
        if (mODEContactGroup == 0) return false;

        return true;
}

void Space::OnLink()
{
        shared_ptr<Scene> scene = GetScene();

        if (scene.get() != NULL)
        {
                shared_ptr<World> world = shared_static_cast<World>(scene->GetChildOfClass("World"));
                if (world)
                        mWorld = world->GetODEWorld();
                else
                        GetLog()->Error() << "ERROR: Could not find a World to link to..." << std::endl;
        }
}

void Space::OnUnlink()
{
        mWorld = 0;
}

void Space::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
    // remove all contact joints
    dJointGroupEmpty (mODEContactGroup);
}
