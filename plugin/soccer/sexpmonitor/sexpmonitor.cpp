/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sexpmonitor.cpp,v 1.8 2004/05/10 14:10:46 fruit Exp $

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
#include <soccer/soccerbase/soccerbase.h>
#include <soccer/ball/ball.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/ballstateaspect/ballstateaspect.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/fieldflag/fieldflag.h>
#include <netinet/in.h>
#include <soccer/sayeffector/sayeffector.h>

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
SexpMonitor::ParseMonitorMessage(const string& data)
{
    GetLog()->Debug() << "SexpMonitor received " << data << " from monitor\n";

    if (mGameState.get() == 0)
    {
        return;
    }

    if (mSexpParser.get() == 0)
    {
        GetLog()->Error() << "(SexpMonitor) ERROR: can't get SexpParser\n";
        mPredicates = shared_ptr<PredicateList>();
        return;
    }

    // get the predicates
    mPredicates = mSexpParser->Parse(data);

    if (mCommandParser.get() == 0)
    {
        GetLog()->Error() << "(SexpMonitor) ERROR: can't get TrainerCommandParser\n";
        return;
    }

    // interpret the commands in the predicates
    mCommandParser->ParsePredicates(*mPredicates);
}

void
SexpMonitor::AddGameState(std::ostringstream& ss)
{
    if (mGameState.get() == 0)
    {
        return;
    }

    // time
    ss << "(time " << mGameState->GetTime() << ")";

    // team names
    std::string teamname;
    if (!mSentLeftTeamname)
    {
        teamname = mGameState->GetTeamName(TI_LEFT);
        if (!teamname.empty())
        {
            ss << "(team_left " << teamname << ")";
            mSentLeftTeamname = true;
        }
    }

    if (!mSentRightTeamname)
    {
        teamname = mGameState->GetTeamName(TI_RIGHT);
        if (!teamname.empty())
        {
            ss << "(team_right " << teamname << ")";
            mSentRightTeamname = true;
        }
    }

    // game half
    TGameHalf half = mGameState->GetGameHalf();
    if (half != mLastHalf)
    {
        mLastHalf = half;
        ss << "(half " << half << ")";
    }

    // scores
    int left_score = mGameState->GetScore(TI_LEFT);
    if (left_score != mLastLeftScore)
    {
        mLastLeftScore = left_score;
        ss << "(score_left " << left_score << ")";
    }
    int right_score = mGameState->GetScore(TI_RIGHT);
    if (right_score != mLastRightScore)
    {
        mLastRightScore = right_score;
        ss << "(score_right " << right_score << ")";
    }

    // gamestate
    TPlayMode play_mode = mGameState->GetPlayMode();
    if (play_mode != mLastPlayMode)
    {
        mLastPlayMode = play_mode;
        ss << "(play_mode " << play_mode << ")";
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
            ss <<"(say " <<sayEff->GetText()<<")";
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
SexpMonitor::GetMonitorInfo()
{
    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
    {
        GetLog()->Error() << "(SexpMonitor) cannot get SceneServer\n";
        return "";
    }
    shared_ptr<Scene> activeScene = sceneServer->GetActiveScene();
    std::ostringstream expression;

    expression << "(Info ";

    string reply;
    if (mCommandParser->SendAck(reply))
    {
        expression << "(ack " + reply + ")";
    }

    AddGameState(expression);
    AddAgents(activeScene, expression);
    AddFlags(activeScene, expression);
    AddBall(activeScene, expression);

    expression << ")\n\0";

    return expression.str();
}

string
SexpMonitor::PutFloatParam(const string& name)
{
    float value;
    if (! SoccerBase::GetSoccerVar(*this,name,value))
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
    // if a new monitor connected, we have to resend all required data
    ResetSentFlags();

    stringstream ss;
    ss << "(Init ";

    // field geometry parameter
    ss << PutFloatParam("FieldLength");
    ss << PutFloatParam("FieldWidth");
    ss << PutFloatParam("FieldHeight");
    ss << PutFloatParam("GoalWidth");
    ss << PutFloatParam("GoalDepth");
    ss << PutFloatParam("GoalHeight");
    ss << PutFloatParam("BorderSize");
    ss << PutFloatParam("FreeKickDistance");
    ss << PutFloatParam("WaitBeforeKickOff");

    // agent parameter
    ss << PutFloatParam("AgentMass");
    ss << PutFloatParam("AgentRadius");
    ss << PutFloatParam("AgentMaxSpeed");

    // ball parameter
    ss << PutFloatParam("BallRadius");
    ss << PutFloatParam("BallMass");

    // soccer rule parameters
    ss << PutFloatParam("RuleGoalPauseTime");
    ss << PutFloatParam("RuleFreeKickPauseTime");
    ss << PutFloatParam("RuleHalfTime");

    ss << ")\n\0";

    return ss.str();
}

void SexpMonitor::OnLink()
{
    SoccerBase::GetGameState(*this,mGameState);

    // we need the SexpParser to generate the predicates
    // from S-Expressions
    mSexpParser = shared_dynamic_cast<oxygen::BaseParser>(GetCore()->New("SexpParser"));

    if (mSexpParser.get() == 0)
    {
        GetLog()->Error() << "ERROR: (SexpMonitor) failed to create SexpParser\n";
        return;
    }

    // we need the TrainerCommandParser to parse the predicates
    // and interpret the commands
    mCommandParser = shared_dynamic_cast<TrainerCommandParser>
      (GetCore()->New("TrainerCommandParser"));

    if (mCommandParser.get() == 0)
    {
        GetLog()->Error() << "ERROR: (SexpMonitor) failed to create parser TrainerCommandParser\n";
        return;
    }

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
    mSexpParser.reset();
    mBallState.reset();
}

void
SexpMonitor::ResetSentFlags()
{
    mSentLeftTeamname = false;
    mSentRightTeamname = false;
    mLastHalf = GH_NONE;
    mLastLeftScore = -1;
    mLastRightScore = -1;
    mLastPlayMode = PM_NONE;
    mSentFlags = false;
}
