/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ballstateaspect.cpp,v 1.1.2.2 2004/01/29 10:30:56 rollmark Exp $

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
#include "ballstateaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>

using namespace oxygen;
using namespace boost;
using namespace std;

BallStateAspect::BallStateAspect() : ControlAspect()
{
    mBallOnField = false;
}

BallStateAspect::~BallStateAspect()
{
}

shared_ptr<AgentAspect> BallStateAspect::GetLastCollidingAgent()
{
    return mLastCollidingAgent;
}

void BallStateAspect::UpdateLastCollidingAgent()
{
    if (mBallRecorder.get() == 0)
        {
            return;
        }

    // get a list of agents that collided with the ball since the last
    // update of the recorder and remember the first returned node as
    // the last agent that collided with the ball.
    RecorderHandler::TParentList agents;
    mBallRecorder->GetParentsSupportingClass("AgentAspect",agents);

    if (agents.size() > 0)
        {
            mLastCollidingAgent = shared_static_cast<AgentAspect>
                (make_shared(agents.front()));
        }

    // empty the recorder buffer
    mBallRecorder->Reset();
}

void BallStateAspect::UpdateBallOnField()
{
    if (mFieldRecorder.get() == 0)
        {
            return;
        }

    // get a list of Ball nodes that collided with the
    RecorderHandler::TParentList ball;
    mFieldRecorder->GetParentsSupportingClass("Ball",ball);

    // the ball is on or above the playing field iff the ball collided
    // with the box collider around the playing field
    mBallOnField = (ball.size() > 0);

    // empty the recorder buffer
    mFieldRecorder->Reset();
}

void BallStateAspect::Update(float deltaTime)
{
    UpdateLastCollidingAgent();
    UpdateBallOnField();
}

void BallStateAspect::OnLink()
{
    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
        {
            GetLog()->Error() << "(BallStateAscpet) cannot get SceneServer\n";
            return;
        }

    shared_ptr<Scene> activeScene = sceneServer->GetActiveScene();

    if (activeScene.get() == 0)
        {
            GetLog()->Error() << "(BallStateAscpet) SceneServer reported no active scene\n";
            return;
        }

    string scenePath = activeScene->GetFullPath();

    // get the field recorder handler
    mFieldRecorder = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(scenePath + "FieldBox/recorder"));

    if (mFieldRecorder.get() == 0)
        {
            GetLog()->Error()
                << "(BallStateAspect) found no field collision recorder\n";
        }

    // get the ball recorder handler
    mBallRecorder = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(scenePath + "Ball/geometry/recorder"));

    if (mBallRecorder.get() == 0)
        {
            GetLog()->Error()
                << "(BallStateAspect) found no ball collision recorder\n";
        }
}

void BallStateAspect::OnUnlink()
{
    mBallRecorder.reset();
    mFieldRecorder.reset();
    mLastCollidingAgent.reset();
}

