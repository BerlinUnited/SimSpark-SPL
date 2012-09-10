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
#include "soccercontrolaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/physicsserver/recorderhandler.h>
#include <oxygen/physicsserver/body.h>
#include <ball/ball.h>
#include <soccerbase/soccerbase.h>

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
    boost::shared_ptr<Scene> scene = GetActiveScene();
    if (scene.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no active scene node\n";
            return;
        }

    mScenePath = scene->GetFullPath();
}

boost::shared_ptr<RecorderHandler> SoccerControlAspect::GetBallRecorder()
{
    string ballRecorder;
    SoccerBase::GetSoccerVar(*this,"BallRecorder",ballRecorder);
    boost::shared_ptr<RecorderHandler> node = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(mScenePath + ballRecorder));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no ball collision recorder\n";
        }

    return node;
}

boost::shared_ptr<RecorderHandler> SoccerControlAspect::GetLeftGoalRecorder()
{
    string goalRecorder;
    SoccerBase::GetSoccerVar(*this,"LeftGoalRecorder",goalRecorder);
    boost::shared_ptr<RecorderHandler> node = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(mScenePath + goalRecorder));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no left goal collision recorder in path: "<<goalRecorder<<'\n';
        }

    return node;
}

boost::shared_ptr<RecorderHandler> SoccerControlAspect::GetRightGoalRecorder()
{
    string goalRecorder;
    SoccerBase::GetSoccerVar(*this,"RightGoalRecorder",goalRecorder);
    boost::shared_ptr<RecorderHandler> node = shared_dynamic_cast<RecorderHandler>
        (GetCore()->Get(mScenePath + goalRecorder));

    if (node.get() == 0)
        {
            GetLog()->Error()
                << "(SoccerControlAspect) found no right goal collision recorder in path: "<<goalRecorder<<'\n';
        }

    return node;
}


