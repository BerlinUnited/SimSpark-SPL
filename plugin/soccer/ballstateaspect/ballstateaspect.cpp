/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ballstateaspect.cpp,v 1.1.2.1 2004/01/25 12:01:54 rollmark Exp $

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

using namespace oxygen;
using namespace boost;
using namespace std;

BallStateAspect::BallStateAspect() : ControlAspect()
{
}

BallStateAspect::~BallStateAspect()
{
}

shared_ptr<AgentAspect> BallStateAspect::GetLastCollidingAgent()
{
    return mLastCollidingAgent;
}

void BallStateAspect::Update(float deltaTime)
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

void BallStateAspect::OnLink()
{
    mBallRecorder = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get("/usr/scene/Ball/geometry/recorder"));

    if (mBallRecorder.get() == 0)
        {
            GetLog()->Error()
                << "(BallStateAspect) found no ball collision recorder" << endl;
        }
}

void BallStateAspect::OnUnlink()
{
    mBallRecorder.reset();
    mLastCollidingAgent.reset();
}

