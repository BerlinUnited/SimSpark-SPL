/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "agentcontrol.h"
#include "simulationserver.h"
#include "netmessage.h"
#include <set>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

AgentControl::AgentControl() : NetControl(), mSyncMode(false)
{
    mLocalAddr.setPort(3100);
}

AgentControl::~AgentControl()
{
}

void AgentControl::OnLink()
{
    NetControl::OnLink();
    RegisterCachedPath(mGameControlServer, "/sys/server/gamecontrol");

    if (mGameControlServer.expired())
    {
        GetLog()->Error()
            << "(AgentControl) ERROR: GameControlServer not found.\n";
    }
}

void AgentControl::ClientConnect(boost::shared_ptr<Client> client)
{
    // Make sure that there is enough space in sense message cache vector
    if (client->id >= mClientSenses.size())
        mClientSenses.resize(client->id+1);

    if (mGameControlServer.get() == 0)
        {
            return;
        }

    mGameControlServer->AgentConnect(client->id);
}

void AgentControl::ClientDisconnect(boost::shared_ptr<Client> client)
{
    mClientSenses[client->id].clear();

    if (mGameControlServer.get() == 0)
        {
            return;
        }

    mGameControlServer->pushDisappearedAgent(client->id);
}

void AgentControl::StartCycle()
{
    do
    {
        NetControl::StartCycle();

        if (
            (mGameControlServer.get() == 0) ||
            (mNetMessage.get() == 0)
            )
            {
                return;
            }

        // pass all received messages on to the GameControlServer
        for (
             TBufferMap::iterator iter = mBuffers.begin();
             iter != mBuffers.end();
             ++iter
             )
            {
                boost::shared_ptr<NetBuffer>& netBuff = (*iter).second;
                if (
                    (netBuff.get() == 0) ||
                    (netBuff->IsEmpty())
                    )
                    {
                        continue;
                    }

                // lookup the client entry corresponding for the buffer
                // entry
                TAddrMap::iterator clientIter = mClients.find(netBuff->GetAddr());
                if (clientIter == mClients.end())
                    {
                        continue;
                    }
                boost::shared_ptr<Client>& client = (*clientIter).second;

                // lookup the AgentAspect node correspoding to the client
                boost::shared_ptr<AgentAspect> agent =
                    mGameControlServer->GetAgentAspect(client->id);
                if (agent.get() == 0)
                    {
                        continue;
                    }

                // parse and immediately realize the action
                string message;
                while (mNetMessage->Extract(netBuff,message))
                    {
                        agent->RealizeActions
                            (mGameControlServer->Parse(client->id,message));
                    }
            }
    } while (!AgentsAreSynced());
}

void AgentControl::SenseAgent()
{
    int clientID;
    for (
         TAddrMap::iterator iter = mClients.begin();
         iter != mClients.end();
         ++iter
         )
        {
            clientID = iter->second->id;
            if (!mClientSenses[clientID].empty())
                {
                    SendClientMessage(iter->second, mClientSenses[clientID]);
                }
        }
}

void AgentControl::EndCycle()
{
    NetControl::EndCycle();

    if (
        (mGameControlServer.get() == 0) ||
        (mNetMessage.get() == 0) ||
        (mClients.size() == 0)
        )
        {
            return;
        }

    boost::shared_ptr<BaseParser> parser = mGameControlServer->GetParser();
    if (parser.get() == 0)
        {
            GetLog()->Error()
                << "(AgentControl) ERROR:  got no parser from "
                << " the GameControlServer" << endl;
            return;
        }

    // generate senses for all agents
    for (
         TAddrMap::iterator iter = mClients.begin();
         iter != mClients.end();
         ++iter
         )
        {
            const boost::shared_ptr<Client> &client = (*iter).second;

            boost::shared_ptr<AgentAspect> agent =
                mGameControlServer->GetAgentAspect(client->id);
            if (agent.get() == 0)
                {
                    continue;
                }
            if (mSyncMode)
                {
                    agent->SetSynced(false);
                }

            boost::shared_ptr<PredicateList> senseList = agent->QueryPerceptors();
            mClientSenses[client->id] = parser->Generate(senseList);
            if (mClientSenses[client->id].empty())
                {
                    continue;
                }

            mNetMessage->PrepareToSend(mClientSenses[client->id]);
        }
}

void AgentControl::SetSyncMode(bool syncMode)
{
    mSyncMode = syncMode;
    if (mSyncMode)
    {
        BlockOnReadMessages(true);
        GetLog()->Normal()
            << "(AgentControl) Running in sync mode.\n";
    }
    else
    {
        BlockOnReadMessages(false);
        GetLog()->Normal()
            << "(AgentControl) Running in normal mode.\n";
    }
}

bool AgentControl::AgentsAreSynced()
{
    if (mSyncMode)
        {
            set<rcss::net::Addr> closedClients(mCloseClients.begin(),
                mCloseClients.end());

            for (
                 TAddrMap::const_iterator iter = mClients.begin();
                 iter != mClients.end();
                 ++iter
                 )
                {
                    if (closedClients.find(iter->first) != closedClients.end())
                        continue;

                    boost::shared_ptr<AgentAspect> agent =
                        mGameControlServer->GetAgentAspect(iter->second->id);
                    if (agent && !agent->IsSynced())
                    {
                        return false;
                    }
                }
        }
    return true;
}
