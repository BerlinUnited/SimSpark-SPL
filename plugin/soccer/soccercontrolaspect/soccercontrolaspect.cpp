/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccercontrolaspect.cpp,v 1.1.2.1 2004/01/29 19:53:53 rollmark Exp $

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
#include "soccercontrolaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/physicsserver/recorderhandler.h>
#include <oxygen/physicsserver/body.h>
#include <soccer/ball/ball.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

SoccerControlAspect::SoccerControlAspect() : ControlAspect()
{
}

SoccerControlAspect::~SoccerControlAspect()
{
}

void SoccerControlAspect::OnLink()
{
    shared_ptr<Scene> scene = GetActiveScene();
    if (scene.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no active scene node\n";
            return;
        }

    mScenePath = scene->GetFullPath();
}

shared_ptr<RecorderHandler> SoccerControlAspect::GetBallRecorder()
{
    shared_ptr<RecorderHandler> node = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(mScenePath + "Ball/geometry/recorder"));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no ball collision recorder\n";
        }

    return node;
}

shared_ptr<RecorderHandler> SoccerControlAspect::GetFieldRecorder()
{
    shared_ptr<RecorderHandler> node = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(mScenePath + "FieldBox/recorder"));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no field collision recorder\n";
        }

    return node;
}

shared_ptr<Ball> SoccerControlAspect::GetBall()
{
    shared_ptr<Ball> node = shared_dynamic_cast<Ball>
        (GetCore()->Get(mScenePath + "Ball"));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no ball node\n";
        }

    return node;
}

shared_ptr<Body> SoccerControlAspect::GetBallBody()
{
    shared_ptr<Body> node = shared_dynamic_cast<Body>
        (GetCore()->Get(mScenePath + "Ball/physics"));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no ball body node\n";
        }

    return node;
}

void SoccerControlAspect::GetSoccerVar(const std::string& name, float& value)
{
    static const string nSpace = "Soccer.";
    GetCore()->GetScriptServer()->GetVariable(string(nSpace + name),value);
}


