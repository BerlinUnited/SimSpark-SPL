/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simcontrolnode.cpp,v 1.8 2008/04/14 13:30:54 yxu Exp $

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
#include "simcontrolnode.h"
#include "simulationserver.h"

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

SimControlNode::SimControlNode() 
    : Node(),mTime(0.0f),mStep(0.02f)
{
}

SimControlNode::~SimControlNode()
{
}

shared_ptr<SimulationServer> SimControlNode::GetSimulationServer()
{
    return shared_static_cast<SimulationServer>
        (make_shared(GetParent()));
}

void SimControlNode::SetSimTime( float now )
{
    mTime = now + mStep;
}

shared_ptr<Scene> SimControlNode::GetActiveScene()
{
    shared_ptr<SceneServer> sceneServer =
        GetSimulationServer()->GetSceneServer();

    if (sceneServer.get() ==0)
    {
        GetLog()->Error()
            << "(SimControlNode) ERROR: SceneServer not found\n";
        return shared_ptr<Scene>();
    }

    return sceneServer->GetActiveScene();
}
