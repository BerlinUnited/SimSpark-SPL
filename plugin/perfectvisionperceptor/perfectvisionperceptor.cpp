/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perfectvisionperceptor.cpp,v 1.2.2.1 2003/12/23 12:10:27 rollmark Exp $

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
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>

#define TEST_ME 1

#if TEST_ME
#include <iostream>
#endif

using namespace oxygen;
using namespace boost;

PerfectVisionPerceptor::PerfectVisionPerceptor() : oxygen::Perceptor()
{
}

PerfectVisionPerceptor::~PerfectVisionPerceptor()
{
}

bool
PerfectVisionPerceptor::Percept(BaseParser::TPredicate& predicate)
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

    TLeafList transformList;
    activeScene->GetChildrenSupportingClass("Transform", transformList, true);

    for (TLeafList::iterator i = transformList.begin();
         i != transformList.end(); ++i)
        {
            shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
            const salt::Vector3f& pos = j->GetWorldTransform().Pos();

            BaseParser::TParameterList element;
            element.push_back((*i)->GetName());
            element.push_back(pos[0]);
            element.push_back(pos[1]);
            element.push_back(pos[2]);
            predicate.parameter.push_back(element);
        }

    return true;
}
