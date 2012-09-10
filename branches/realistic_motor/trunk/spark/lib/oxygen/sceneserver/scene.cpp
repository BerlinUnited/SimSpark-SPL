/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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

#include "scene.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;

Scene::Scene() : BaseNode(), mModified(false), mModifiedNum(0), mLastCacheUpdate(0)
{
}

Scene::~Scene()
{
}

void Scene::UpdateCacheInternal()
{
    mLastCacheUpdate = mModifiedNum;
}

int Scene::GetLastCacheUpdate()
{
    return mLastCacheUpdate;
}

const salt::Matrix& Scene::GetWorldTransform() const
{
    return mIdentityMatrix;
}

void Scene::SetWorldTransform(const salt::Matrix &/*transform*/)
{
}

void Scene::SetModified(bool modified)
{
    mModified = modified;
    if ( modified ) mModifiedNum++;
}

bool Scene::GetModified()
{
    return mModified;
}

int Scene::GetModifiedNum()
{
    return mModifiedNum;
}
