/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: collider.cpp,v 1.2 2003/08/29 22:08:21 rollmark Exp $

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

#include "collider.h"
#include "../sceneserver/scene.h"
#include "space.h"
#include "body.h"
#include "world.h"

using namespace boost;
using namespace kerosin;

Collider::Collider() :
ODEObject(), mODEGeom(0)
{
}

Collider::~Collider()
{
	//printf("~Collider '%s'\n", GetClass()->GetName().c_str());

	if (mODEGeom)
	{
		dGeomDestroy(mODEGeom);
		mODEGeom = 0;
	}
}

void Collider::OnLink()
{
	ODEObject::OnLink();

	shared_ptr<Scene> scene = GetScene();
	
	if (scene.get() != NULL)
	{
		mWorld = shared_static_cast<World>(scene->GetChildOfClass("World"));
		mSpace = shared_static_cast<Space>(scene->GetChildOfClass("Space"));
		dSpaceID space = mSpace->GetODESpace();

		// if we have a space and an object, add it to the space
		if (mODEGeom && space && !dSpaceQuery(space, mODEGeom))
		{
			dSpaceAdd(space, mODEGeom);
		}
		
		// if there is a Body hanging on our parent, link to it
		shared_ptr<Body> body = shared_static_cast<Body>(make_shared(GetParent())->GetChildOfClass("Body"));
		if (body.get() != NULL)
		{
			dGeomSetBody (mODEGeom, body->GetODEBody());
		}
	}
}

void Collider::OnUnlink()
{
	ODEObject::OnUnlink();

	dSpaceID space = mSpace->GetODESpace();

	// remove collision geometry from space
	if (mODEGeom && space && dSpaceQuery(space, mODEGeom))
	{
		dSpaceRemove(space, mODEGeom);
	}

	mSpace.reset();
}
