/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Sat April 12 2008
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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
#include "traincontrol.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/gamecontrolserver/baseparser.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/gamecontrolserver/behavior.h>

using namespace std;
using namespace boost;
using namespace salt;
using namespace oxygen;
using namespace zeitgeist;

TrainControl::TrainControl() : mNextClientId(0)
{
}

TrainControl::~TrainControl()
{
}

void TrainControl::OnLink()
{
    SimControlNode::OnLink();

    RegisterCachedPath(mGameControlServer, "/sys/server/gamecontrol");
    if (mGameControlServer.expired())
    {
        GetLog()->Error()
            << "(TrainControl) ERROR: (OnLink) "
            << "GameControlServer get failed" << endl;
        return ;
    }
}

void TrainControl::OnUnlink()
{
    SimControlNode::OnUnlink();

    mGameControlServer.reset();
}

void TrainControl::InitSimulation()
{
}

void TrainControl::DoneSimulation()
{
}

void TrainControl::StartCycle()
{
    if (
        (mGameControlServer.get() == 0)
       )
    {
        return ;
    }

    TClientSet::iterator end = mClientSet.end();
    for (
         TClientSet::iterator iter = mClientSet.begin();
         iter != end;
         ++iter
        )
    {
        shared_ptr<Client> client = (*iter);
        shared_ptr<AgentAspect> agent =
            mGameControlServer->GetAgentAspect(client->id);
        if (agent.get() == 0)
        {
            GetLog()->Error()
                << "(TrainControl) ERROR: StartCycle agent " << client->id
                << " not found" << endl;
        }
        else
        {
            agent->RealizeActions
               (mGameControlServer->Parse(client->id, client->command));
        }
    }
}

void TrainControl::EndCycle()
{
    if (
         (mGameControlServer.get() == 0)
       )
    {
        return ;
    }

    shared_ptr<BaseParser> parser = mGameControlServer->GetParser();
    if (parser.get() == 0)
    {
        GetLog()->Error()
            << "(TrainControl) ERROR: got no parser from "
            << " the GameControlServer" << endl;
        return ;
    }

    TClientSet::iterator end = mClientSet.end();

    for (
         TClientSet::iterator iter = mClientSet.begin();
         iter != end;
         ++iter
        )
    {
        shared_ptr<Client> client = (*iter);
        shared_ptr<AgentAspect> agent =
            mGameControlServer->GetAgentAspect(client->id);
        if (agent.get() == 0)
        {
            GetLog()->Error()
                << "(TrainControl) ERROR: agent "
                << client->id << " not found" << endl;
        }
        else
        {
            client->senselist = agent->QueryPerceptors();
        }
    }
}

void TrainControl::SenseAgent()
{
}

void TrainControl::ActAgent()
{
    TClientSet::iterator end = mClientSet.end();
    for (
         TClientSet::iterator iter = mClientSet.begin();
         iter != end;
         ++iter
        )
    {
        shared_ptr<Client> client = (*iter);

        /** default command is null */
        string cmd("");
        shared_ptr<Behavior> behavior = client->behavior;
        if (behavior.get() != 0)
        {
            behavior->Think(client->senselist);
            client->command = behavior->GetCommand();
        }
    }
}

void TrainControl::ClientConnect(boost::shared_ptr<Behavior> behavior)
{
    if (
        (mGameControlServer.get() == 0) ||
        (behavior.get() == 0)
       )
    {
        return ;
    }

    shared_ptr<Client> client(new Client);
    client->id = ++mNextClientId;
    client->behavior = behavior;

    mGameControlServer->AgentConnect(client->id);
    mClientSet.insert(client);
}

void TrainControl::ClientDisconnect(boost::shared_ptr<Behavior> behavior)
{
    if (
        (mGameControlServer.get() == 0) ||
        (behavior.get() == 0)
       )
    {
        return ;
    }

    TClientSet::iterator end = mClientSet.end();
    for (
         TClientSet::iterator iter = mClientSet.begin();
         iter != end;
         ++iter
        )
    {
        shared_ptr<Client> client = (*iter);

        if (behavior.get() == (client->behavior).get())
        {
            mGameControlServer->AgentDisappear(client->id);
            mClientSet.erase(client);
            break ;
        }
    }
}
