/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorclientcontrol.cpp,v 1.2 2004/05/10 14:10:45 fruit Exp $

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
#include "monitorclientcontrol.h"
#include <commserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/simulationserver/simulationserver.h>
#include <sstream>

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace boost;
using namespace std;

void MonitorClientControl::OnLink()
{
    mCommServer = shared_dynamic_cast<CommServer>
        (GetCore()->Get("/sys/server/comm"));

    if (mCommServer.get() == 0)
    {
        GetLog()->Error()
            << "(MonitorClientControl) ERROR: CommServer not found\n";
    }
}

shared_ptr<Transform>
MonitorClientControl::GetBall()
{
    const std::string name = "ball";

    shared_ptr<Scene> scene =
        GetSimulationServer()->GetSceneServer()->GetActiveScene();
    if (scene.get() == 0)
    {
        return shared_ptr<Transform>();
    }

    // try to find the transform node
    shared_ptr<Transform> node = shared_dynamic_cast<Transform>
        (scene->GetChild(name));

    if (node.get() != 0)
    {
        return node;
    }

    GetCore()->GetScriptServer()->Eval("addBall('" + name + "')");
    return shared_dynamic_cast<Transform>(scene->GetChild(name));
}

shared_ptr<Transform>
MonitorClientControl::GetAgent(TTeamIndex side, int unum)
{
    shared_ptr<Scene> scene =
        GetSimulationServer()->GetSceneServer()->GetActiveScene();

    if (scene.get() == 0)
    {
        return shared_ptr<Transform>();
    }

    std::string name;
    std::string fktName;

    switch (side)
    {
    case TI_LEFT:
        name = "agentL";
        fktName = "addAgentL";
        break;
    case TI_RIGHT:
        name = "agentR";
        fktName = "addAgentR";
        break;
    default:
        name = "agentN";
        fktName = "addAgentN";
        break;
    }

    std::ostringstream ss;
    ss << unum;
    name += ss.str();

    // try to find the transform node
    shared_ptr<Transform> node = shared_dynamic_cast<Transform>
        (scene->GetChild(name));

    if (node.get() != 0)
    {
        return node;
    }

    GetCore()->GetScriptServer()->Eval(fktName+"('"+ name +"')");
    return shared_dynamic_cast<Transform>(scene->GetChild(name));
}

shared_ptr<Transform>
MonitorClientControl::GetFlag(GameState::EFlagType i)
{
    shared_ptr<Scene> scene =
        GetSimulationServer()->GetSceneServer()->GetActiveScene();

    std::string name = mGameState.GetFlagName(i);
    if (scene.get() == 0 || name.empty())
    {
        return shared_ptr<Transform>();
    }

    name = "flag" + name;

    // try to find the transform node
    shared_ptr<Transform> node = shared_dynamic_cast<Transform>
        (scene->GetChild(name));

    if (node.get() != 0)
    {
        return node;
    }

    GetCore()->GetScriptServer()->Eval("addFlag('" + name + "')");

    return shared_dynamic_cast<Transform>(scene->GetChild(name));
}

void
MonitorClientControl::StartCycle()
{
    if (
        (mCommServer.get() == 0) ||
        (! mCommServer->ReadMessage())
        )
    {
        return;
    }

    boost::shared_ptr<oxygen::PredicateList> predicates =
        mCommServer->GetPredicates();

    mGameState.ProcessInput(predicates);

    static bool initialUpdate = true;

    if (initialUpdate)
    {
        // publish the soccer default values to the scripts
        shared_ptr<ScriptServer> script = GetCore()->GetScriptServer();
        script->CreateVariable("Soccer.FieldLength", mGameState.GetFieldLength());
        script->CreateVariable("Soccer.FieldWidth",  mGameState.GetFieldWidth());
        script->CreateVariable("Soccer.FieldHeight", mGameState.GetFieldHeight());
        script->CreateVariable("Soccer.BorderSize",  mGameState.GetBorderSize());
        script->CreateVariable("Soccer.LineWidth",   mGameState.GetLineWidth());
        script->CreateVariable("Soccer.GoalWidth",   mGameState.GetGoalWidth());
        script->CreateVariable("Soccer.GoalDepth",   mGameState.GetGoalDepth());
        script->CreateVariable("Soccer.GoalHeight",  mGameState.GetGoalHeight());
        script->CreateVariable("Soccer.AgentMass",   mGameState.GetAgentMass());
        script->CreateVariable("Soccer.AgentRadius",  mGameState.GetAgentRadius());
        script->CreateVariable("Soccer.AgentMaxSpeed",  mGameState.GetAgentMaxSpeed());
        script->CreateVariable("Soccer.BallRadius",  mGameState.GetBallRadius());
        script->CreateVariable("Soccer.BallMass",    mGameState.GetBallMass());

        // create the playing field with the actual dimensions
        script->Eval("addField()");
        initialUpdate = false;
    }

    Vector3f pos;
    for (int i = static_cast<int>(GameState::eFLAG_1_L);
         i != static_cast<int>(GameState::eILLEGAL);
         ++i)
    {
        if (mGameState.GetFlag(static_cast<GameState::EFlagType>(i), pos))
        {
            shared_ptr<Transform> node = GetFlag(static_cast<GameState::EFlagType>(i));

            if (node.get() != 0)
            {
                node->SetLocalPos(pos);
            }
        }
    }

    int i = 0;
    int unum;
    TTeamIndex side;
    float size;
    while (mGameState.GetPlayer(i, pos, side, unum, size))
    {
        shared_ptr<Transform> node = GetAgent(side, unum);

        if (node.get() != 0)
        {
            node->SetLocalPos(pos);
        }
        ++i;
    }

    if (mGameState.GetBall(pos, size))
    {
        shared_ptr<Transform> node = GetBall();

        if (node.get() != 0)
        {
            node->SetLocalPos(pos);
        }
    }

}
