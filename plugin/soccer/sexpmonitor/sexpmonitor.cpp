/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sexpmonitor.cpp,v 1.1.2.8 2004/02/06 10:56:57 rollmark Exp $

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
#include "sexpmonitor.h"
#include <sstream>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <soccer/soccertypes.h>
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/ballstateaspect/ballstateaspect.h>
#include <soccer/agentstate/agentstate.h>
#include <netinet/in.h>

using namespace oxygen;
using namespace std;
using namespace boost;

SexpMonitor::SexpMonitor() : MonitorSystem()
{
}

SexpMonitor::~SexpMonitor()
{
}

bool
SexpMonitor::ConstructInternal()
{
  return true;
}

void
SexpMonitor::ParseMonitorMessage(std::string data)
{
    GetLog()->Debug() << "SexpMonitor received " << data << " from monitor\n";
}

std::string SexpMonitor::GenerateSexp(string type, TLeafList& list)
{
    stringstream ss;

    for (TLeafList::iterator i = list.begin();i != list.end();++i)
        {
            shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
            const salt::Vector3f& pos = j->GetWorldTransform().Pos();

            ss << "(" << type << " (pos " << pos[0] << " " << pos[1] << " " << pos[2] << "))";
        }

    return ss.str();
}

shared_ptr<AgentAspect> SexpMonitor::GetLastBallAgent()
{
    // query the ball state aspect for the last Agent
    shared_ptr<BallStateAspect> ballState = shared_dynamic_cast<BallStateAspect>
        (GetCore()->Get("/sys/server/gamecontrol/BallStateAspect"));

    if (ballState.get() == 0)
        {
            GetLog()->Error() << "(SexpMonitor) found no BallStateAspect" << endl;
            return shared_ptr<AgentAspect>();
        }

    return ballState->GetLastCollidingAgent();
}

string
SexpMonitor::GetGameStateData()
{
    if (mGameState.get() == 0)
        {
            return "";
        }

    stringstream ss;

    // team names
    std::string name = mGameState->GetTeamName(TI_LEFT);
    if (name != "")
        {
            ss << "(teamL " << mGameState->GetTeamName(TI_LEFT) << ")";
        }

    name = mGameState->GetTeamName(TI_RIGHT);
    if (name != "")
        {
            ss << "(teamR " << mGameState->GetTeamName(TI_RIGHT) << ")";
        }

    // game half
    ss << "(half "
       << ((mGameState->GetGameHalf() == GH_FIRST) ? "1" : "2")
       << ")";

    // time
    ss << "(time " << mGameState->GetTime() << ")";

    // scores
    ss << "(scoreL " << mGameState->GetScore(TI_LEFT) << ")";
    ss << "(scoreR " << mGameState->GetScore(TI_RIGHT) << ")";

    // gamestate
    ss << "(playMode " << mGameState->GetPlayMode() << ")";

    return ss.str();
}

string
SexpMonitor::GetAgentData(shared_ptr<Scene> activeScene)
{
    TLeafList nodes;
    activeScene->GetChildrenSupportingClass("AgentAspect", nodes, true);

    stringstream ss;

    for (TLeafList::iterator i = nodes.begin();i != nodes.end();++i)
        {
            shared_ptr<AgentAspect> aspect =
                shared_static_cast<AgentAspect>(*i);
            const salt::Vector3f& pos = aspect->GetWorldTransform().Pos();

            shared_ptr<AgentState> state = shared_static_cast<AgentState>
                (aspect->GetChildOfClass("AgentState"));

            ss << "(agent ";

            // pos
            ss << "(pos " << pos[0] << " " << pos[1] << " " << pos[2] << ")";

            if (state.get() != 0)
                {
                    ss << "(team ";

                    switch (state->GetTeamIndex())
                        {
                        case TI_NONE :
                            ss << "N";
                            break;
                        case TI_LEFT :
                            ss << "L";
                            break;
                        case TI_RIGHT :
                            ss << "R";
                            break;
                        }

                    ss << ")";

                    ss << "(unum " << state->GetUniformNumber() << ")";
                }

            ss << ")";
        }

    return ss.str();
}

string
SexpMonitor::GetMonitorInfo()
{
    // map from generic class types to be recognized to their
    // expression type
    static const struct ExpressionType
    {
        string className;
        string typeName;
    } typeMap[] =
        {
            {"FieldFlag","flag"},
            {"Ball","ball"},
        };

    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
        {
            GetLog()->Error() << "(SexpMonitor) cannot get SceneServer\n";
            return "";
        }

    shared_ptr<Scene> activeScene = sceneServer->GetActiveScene();
    stringstream expression;

    // AgentAspects
    expression << GetAgentData(activeScene);

    // generic classes
    int n = sizeof(typeMap) / sizeof(ExpressionType);
    for (int i=0;i<n;++i)
        {
            const ExpressionType& entry = typeMap[i];

            TLeafList nodes;
            activeScene->GetChildrenSupportingClass(entry.className, nodes, true);
            expression << GenerateSexp(entry.typeName, nodes);
        }

    // get the last agent touching the ball
    shared_ptr<AgentAspect> lastBallAgent = GetLastBallAgent();

    if (lastBallAgent.get() != 0)
        {
            TLeafList node;
            node.push_back(lastBallAgent);
            expression << GenerateSexp("ballAgent", node);
        }

    // collect info from the GameStateAspect
    expression << GetGameStateData();

    expression << endl;
    return expression.str();
}

string
SexpMonitor::PutFloatParam(const string& name)
{
    float value;

    string var = "Soccer."+name;

    if (! GetCore()->GetScriptServer()->GetVariable(var,value))
        {
            return "";
        }

    stringstream ss;
    ss << "(" << name << " " << value << ")";

    return ss.str();
}

string
SexpMonitor::GetMonitorHeaderInfo()
{
    stringstream ss;
    ss << "(init ";

    // field geometry parameter
    ss << PutFloatParam("FieldLength");
    ss << PutFloatParam("FieldWidth");
    ss << PutFloatParam("FieldHeight");
    ss << PutFloatParam("GoalWidth");
    ss << PutFloatParam("GoalDepth");
    ss << PutFloatParam("GoalHeight");
    ss << PutFloatParam("BorderSize");

    // agent parameter
    ss << PutFloatParam("AgentMass");
    ss << PutFloatParam("AgentRadius");
    ss << PutFloatParam("AgentMaxSpeed");

    // ball parameter
    ss << PutFloatParam("BallRadius");
    ss << PutFloatParam("BallMass");

    ss << ")";

    return ss.str();
}

void SexpMonitor::OnLink()
{
    SoccerBase::GetGameState(*this,mGameState);
}

void SexpMonitor::OnUnlink()
{
    mGameState.reset();
}


