/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorclientcontrol.cpp,v 1.1 2004/04/25 17:11:43 rollmark Exp $

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

    mMonitorParser = shared_dynamic_cast<MonitorParser>
      (GetCore()->Get("/sys/server/parser"));

    if (mMonitorParser.get() == 0)
        {
          GetLog()->Error()
              << "(MonitorClientControl) ERROR: MonitorParser not found\n";
          return;
        }
}

bool MonitorClientControl::MangleExpr(MonitorParser::Expression& expr, string& name)
{
    switch (expr.etype)
        {
        default:
            return false;

        case MonitorParser::ET_BALL:
            name = "ball";
            break;

        case MonitorParser::ET_FLAG:
            {
                // flags are differing only in the pos
                stringstream ss;
                ss << "flag"
                   << expr.pos[0]
                   << expr.pos[1]
                   << expr.pos[2];
                name = ss.str();
                break;
            }

        case MonitorParser::ET_AGENT:
            {
                stringstream ss;
                ss << "agent";

                switch (expr.team)
                    {
                    case TI_LEFT:
                        ss << "L";
                        break;

                    case TI_RIGHT:
                        ss << "R";
                        break;

                    default:
                    case TI_NONE :
                        ss << "N";
                        break;
                    }

                ss << expr.unum;
                name = ss.str();
            }
        }

    return true;
}

shared_ptr<Transform> MonitorClientControl::GetSphere(MonitorParser::Expression& expr)
{
    string name;
    shared_ptr<Scene> scene =
        GetSimulationServer()->GetSceneServer()->GetActiveScene();
    if (
        (scene.get() == 0) ||
        (! MangleExpr(expr, name))
        )
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

    string fktName;

    // create a new visual
    switch (expr.etype)
        {
        default:
            return shared_ptr<Transform>();

        case MonitorParser::ET_BALL:
            fktName = "addBall";
            break;

        case MonitorParser::ET_AGENT:
            {
                stringstream ss;
                ss << "addAgent";

                switch (expr.team)
                    {
                    case TI_NONE:
                        ss << "N";
                        break;

                    case TI_LEFT:
                        ss << "L";
                        break;

                    case TI_RIGHT:
                        ss << "R";
                        break;
                    }

                fktName = ss.str();
                break;
            }

        case MonitorParser::ET_FLAG:
            fktName = "addFlag";
            break;
        }

    stringstream ss;
    ss << fktName << "('" << name << "')";
    GetCore()->GetScriptServer()->Eval(ss.str());

    return shared_dynamic_cast<Transform>
        (scene->GetChild(name));
}

void MonitorClientControl::StartCycle()
{
    if (
        (mCommServer.get() == 0) ||
        (! mCommServer->GetMessage())
        )
        {
            return;
        }

    static bool initialUpdate = true;


    boost::shared_ptr<oxygen::PredicateList> predicates =
        mCommServer->GetPredicates();

    MonitorParser::TExprList exprList;

    if (
        (predicates.get() != 0) &&
        (predicates->GetSize() > 0)
        )
        {
            // parse the received expressions
            mMonitorParser->ParsePredicates(*predicates,mGameState,
                                            mGameParam,exprList);
        }

    if (initialUpdate)
        {
            // publish the soccer default values to the scripts
            shared_ptr<ScriptServer> script = GetCore()->GetScriptServer();
            script->CreateVariable("Soccer.FieldLength", mGameParam.GetFieldLength());
            script->CreateVariable("Soccer.FieldWidth",  mGameParam.GetFieldWidth());
            script->CreateVariable("Soccer.FieldHeight", mGameParam.GetFieldHeight());
            script->CreateVariable("Soccer.BorderSize",  mGameParam.GetBorderSize());
            script->CreateVariable("Soccer.LineWidth",   mGameParam.GetLineWidth());
            script->CreateVariable("Soccer.GoalWidth",   mGameParam.GetGoalWidth());
            script->CreateVariable("Soccer.GoalDepth",   mGameParam.GetGoalDepth());
            script->CreateVariable("Soccer.GoalHeight",  mGameParam.GetGoalHeight());
            script->CreateVariable("Soccer.AgentMass",   mGameParam.GetAgentMass());
            script->CreateVariable("Soccer.AgentRadius",  mGameParam.GetAgentRadius());
            script->CreateVariable("Soccer.AgentMaxSpeed",  mGameParam.GetAgentMaxSpeed());
            script->CreateVariable("Soccer.BallRadius",  mGameParam.GetBallRadius());
            script->CreateVariable("Soccer.BallMass",    mGameParam.GetBallMass());

            // create the playing field with the acutal dimensions
            script->Eval("addField()");
            initialUpdate = false;
        }

    for (
         MonitorParser::TExprList::iterator iter = exprList.begin();
         iter != exprList.end();
         ++iter)
        {
            MonitorParser::Expression& expr = (*iter);
            shared_ptr<Transform> node = GetSphere(expr);

            if (node.get() == 0)
                {
                    continue;
                }

            Vector3f pos(expr.pos[0],expr.pos[1],expr.pos[2]);
            node->SetLocalPos(pos);
        }
}
