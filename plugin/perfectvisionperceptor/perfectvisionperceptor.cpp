/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perfectvisionperceptor.cpp,v 1.5 2004/02/26 21:15:47 fruit Exp $

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
#include "perfectvisionperceptor.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>

#define TEST_ME 0

#if TEST_ME
#include <iostream>
#endif

using namespace oxygen;
using namespace boost;

PerfectVisionPerceptor::PerfectVisionPerceptor() : oxygen::Perceptor()
{
    mInvertX = mInvertY = mInvertZ = false;
}

PerfectVisionPerceptor::~PerfectVisionPerceptor()
{
}

bool
PerfectVisionPerceptor::Percept(Predicate& predicate)
{
    predicate.name = "PerfectVision";
    predicate.parameter.clear();

    if (mSceneServer.get() == 0)
    {
        mSceneServer = shared_static_cast<SceneServer>
            (GetCore()->Get("/sys/server/scene"));
    }

    if (mSceneServer.get() == 0)
    {
        return false;
    }

    shared_ptr<Scene> activeScene = mSceneServer->GetActiveScene();
    if (activeScene.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (PerfectVisionPerceptor) SceneServer reports no active scene\n";
        return false;
    }

    // we want positions relative to the closest parent transform node
    shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
        (make_shared(GetParentSupportingClass("Transform")));

    salt::Vector3f myPos(0,0,0);
    if (parent.get() == 0)
    {
        GetLog()->Warning()
            << "WARNING: (PerfectVisionPerceptor) parent node is not derived from TransformNode\n";
    } else
        {
            myPos = parent->GetWorldTransform().Pos();
        }

    TLeafList transformList;
    activeScene->GetChildrenSupportingClass("Transform", transformList, true);

    for (TLeafList::iterator i = transformList.begin();
         i != transformList.end(); ++i)
    {
        shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
        const salt::Vector3f& pos = j->GetWorldTransform().Pos() - myPos;

        Predicate::TParameterList position;
        position.push_back(std::string("pos"));
        position.push_back(mInvertX ? -pos[0] : pos[0]);
        position.push_back(mInvertY ? -pos[1] : pos[1]);
        position.push_back(mInvertZ ? -pos[2] : pos[2]);

        Predicate::TParameterList element;

        element.push_back((*i)->GetName());
        element.push_back(position);

        predicate.parameter.push_back(element);
    }

    return true;
}

void
PerfectVisionPerceptor::SetInversion(bool x, bool y, bool z)
{
    mInvertX = x;
    mInvertY = y;
    mInvertZ = z;
}

