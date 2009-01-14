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
#include "sexpmonitor.h"
#include <sstream>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <soccertypes.h>
#include <soccerbase/soccerbase.h>
#include <soccerbase/soccerbase.h>
#include <ball/ball.h>
#include <gamestateaspect/gamestateaspect.h>
#include <ballstateaspect/ballstateaspect.h>
#include <agentstate/agentstate.h>
#include <fieldflag/fieldflag.h>
#include <sayeffector/sayeffector.h>

#ifndef WIN32
#include <netinet/in.h>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace boost;

SexpMonitor::SexpMonitor() : MonitorSystem(), mSendRotMatrix(false)
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
SexpMonitor::ParseMonitorMessage(const string& data)
{
    GetLog()->Debug() << "SexpMonitor received " << data << " from monitor\n";

    if (mGameState.get() == 0)
    {
        return;
    }

    if (mCommandParser.get() == 0)
    {
        GetLog()->Error() << "(SexpMonitor) ERROR: can't get TrainerCommandParser\n";
        return;
    }

    // interpret the commands in the predicates
    mCommandParser->ParseMonitorMessage(data);
}

void
SexpMonitor::AddPredicates(std::ostringstream& ss, const PredicateList& pList)
{
    for (
         PredicateList::TList::const_iterator iter = pList.begin();
         iter != pList.end();
         ++iter
         )
        {
            const Predicate& pred = (*iter);

            ss << "(";
            ss << pred.name;

            const ParameterList& paramList = pred.parameter;
            ParameterList::TVector::const_iterator pIter = paramList.begin();

            std::string param;
            while (
                   (pIter != paramList.end()) &&
                   (paramList.AdvanceValue(pIter, param))
                   )
                {
                    ss << " ";
                    ss << param;
                }

            ss << ")";
        }
}

void
SexpMonitor::AddAgents(shared_ptr<Scene> activeScene, std::ostringstream& ss) const
{
    TLeafList nodes;
    activeScene->ListChildrenSupportingClass<AgentAspect>(nodes, true);

    for (TLeafList::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        shared_ptr<AgentAspect> aspect = shared_static_cast<AgentAspect>(*i);
        const salt::Vector3f& pos = aspect->GetWorldTransform().Pos();

        ss << "(P ";

        shared_ptr<AgentState> state = shared_static_cast<AgentState>
            (aspect->GetChildOfClass("AgentState"));

        shared_ptr<SayEffector> sayEff = shared_static_cast<SayEffector>
            (aspect->GetChildOfClass("SayEffector"));

        if (state.get() != 0)
        {
            ss << "(s " << state->GetTeamIndex() << ")";
            ss << "(id " << state->GetUniformNumber() << ")";
        }

        // pos
        ss << "(pos " << pos << ")";

        if (mSendRotMatrix)
        {
            // world transform
            ss << "(rot ";

            const salt::Matrix& trans = aspect->GetWorldTransform();
            for (int i=0;i<16;++i)
            {
                ss << trans.m[i] << " ";
            }

            ss << ")";
        }

        // extra field if the agent was the last colliding with the ball
        shared_ptr<AgentAspect> agent;
        TTime time;
        mBallState->GetLastCollidingAgent(agent,time);
        if (agent == aspect)
        {
            ss << "(last)";
        }
        if (sayEff !=0 && sayEff->IfText())
        {
            //ss <<"(say " <<sayEff->GetText()<<")";
        }
        ss << ")";
    }
}

void
SexpMonitor::AddFlags(shared_ptr<Scene> activeScene, std::ostringstream& ss)
{
    // the flags don't change, so we need to send them only once
    if (mSentFlags)
    {
        return;
    }
    mSentFlags = true;

    TLeafList nodes;
    activeScene->ListChildrenSupportingClass<FieldFlag>(nodes, true);

    for (TLeafList::iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        shared_ptr<FieldFlag> flag = shared_static_cast<FieldFlag>(*i);
        const salt::Vector3f& pos = flag->GetWorldTransform().Pos();
        shared_ptr<ObjectState> state = shared_dynamic_cast<ObjectState>
            (flag->GetChildOfClass("ObjectState"));

        if (state.get() == 0) continue;

        ss << "(" << state->GetPerceptName()[0] << " ";
        // id
        ss << "(id " << state->GetID() << ")";
        // pos
        ss << "(pos " << pos[0] << " " << pos[1] << " " << pos[2] << ")";

        ss << ")";
    }
}

void
SexpMonitor::AddBall(shared_ptr<Scene> activeScene, std::ostringstream& ss) const
{
    shared_ptr<Ball> ball = shared_static_cast<Ball>(activeScene->GetChild("Ball"));
    const salt::Vector3f& pos = ball->GetWorldTransform().Pos();
    ss << "(B ";
    // pos
    ss << "(pos " << pos[0] << " " << pos[1] << " " << pos[2] << ")";
    ss << ")";
}

string
SexpMonitor::GetMonitorInfo(const oxygen::PredicateList& pList)
{
    if (mGameState->IsFinished())
    {
        return "(Die)\n";
    }

    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
    {
        GetLog()->Error() << "(SexpMonitor) cannot get SceneServer\n";
        return "";
    }
    shared_ptr<Scene> activeScene = sceneServer->GetActiveScene();
    ostringstream expression;

    expression << "(Info ";

    string reply;
    if (mCommandParser->SendAck(reply))
    {
        expression << "(ack " + reply + ")";
    }

    AddPredicates(expression, pList);
    AddAgents(activeScene, expression);
    AddFlags(activeScene, expression);
    AddBall(activeScene, expression);

    expression << ")\n";

    return expression.str();
}

string
SexpMonitor::GetMonitorHeaderInfo(const oxygen::PredicateList& pList)
{
    // if a new monitor connected, we have to resend all required data
    ResetSentFlags();

    ostringstream ss;
    ss << "(Init ";
    AddPredicates(ss, pList);
    ss << ")\n";

    return ss.str();
}

void
SexpMonitor::OnLink()
{
    SoccerBase::GetGameState(*this,mGameState);

    // we need the TrainerCommandParser to parse the predicates
    // and interpret the commands
    mCommandParser = shared_dynamic_cast<TrainerCommandParser>
      (GetCore()->New("TrainerCommandParser"));

    if (mCommandParser.get() == 0)
    {
        GetLog()->Error() << "ERROR: (SexpMonitor) failed to create parser TrainerCommandParser\n";
        return;
    }

    mCommandParser->SetName("TrainerCommandParser");
    AddChildReference(mCommandParser);

    UpdateCached();
}

void
SexpMonitor::UpdateCached()
{
    mBallState = shared_dynamic_cast<BallStateAspect>
        (GetCore()->Get("/sys/server/gamecontrol/BallStateAspect"));

    if (mBallState.get() == 0)
    {
        GetLog()->Error() << "(SexpMonitor) found no BallStateAspect\n";
    }
}


void
SexpMonitor::OnUnlink()
{
    mGameState.reset();
    mCommandParser.reset();
    mBallState.reset();
}

void
SexpMonitor::ResetSentFlags()
{
    mSentFlags = false;
}

void
SexpMonitor::SendRotationMatrix(bool send)
{
    mSendRotMatrix = send;
}
