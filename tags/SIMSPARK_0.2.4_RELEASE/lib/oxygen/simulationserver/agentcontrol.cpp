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

AgentControl::AgentControl() : NetControl(), mSyncMode(false),
            mMultiThreads(true), mThreadBarrierNew(NULL), nThreads(0)
{
    mThreadBarrier = new boost::barrier(1);
    mLocalAddr.setPort(3100);
}

AgentControl::~AgentControl()
{
  delete mThreadBarrier;
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

    //Create a new thread and new barrier
    if(mMultiThreads)
    {
      /**@todo Make this safe! */
      if(mThreadBarrierNew != NULL)
        GetLog()->Error()
            << "(AgentControl) ERROR mThreadBarrierNew!=NULL!"
            << " Agents connecting/disconnecting in same frame !\n";
      nThreads++;
      mThreadBarrierNew = new boost::barrier(nThreads+1);
      boost::thread* newThread =
         mThreadGroup.create_thread(boost::bind(&AgentControl::AgentThread,
                                               this, client));
    }
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

        //if(!mMultiThreads)
        //{
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

                  // start cycle for this client
                  StartCycle(client, netBuff);
              }
        /*}
        else
        {
          mThreadAction = STARTCYCLE;
          WaitMaster(); //let threads start
          WaitMaster(); //wait for threads to finish
        }*/
    } while (!AgentsAreSynced());
}

void AgentControl::StartCycle(const boost::shared_ptr<Client> &client,
                              boost::shared_ptr<NetBuffer> &netBuff)
{
  // lookup the AgentAspect node corresponding to the client
  boost::shared_ptr<AgentAspect> agent =
      mGameControlServer->GetAgentAspect(client->id);
  if (agent.get() == 0)
  {
      return;
  }
  // parse and immediately realize the action
  string message;
  while (mNetMessage->Extract(netBuff,message))
  {
      agent->RealizeActions
          (mGameControlServer->Parse(client->id,message));
  }
}

void AgentControl::SenseAgent()
{
  //if(!mMultiThreads)
  //{
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
  /*}
  else
  {
    mThreadAction = SENSEAGENT;
    WaitMaster(); //let threads start
    WaitMaster(); //wait for threads to finish
  }*/
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

    if(!mMultiThreads)
    {
      // generate senses for all agents
      for (
           TAddrMap::iterator iter = mClients.begin();
           iter != mClients.end();
           ++iter
           )
          {
              const boost::shared_ptr<Client> &client = (*iter).second;
              EndCycle(client);
          }
    }
    else
    {
      mThreadAction = ENDCYCLE;
      WaitMaster(); //let threads start
      WaitMaster(); //wait for threads to finish
    }
}

void AgentControl::EndCycle(const boost::shared_ptr<Client> &client)
{
    boost::shared_ptr<BaseParser> parser = mGameControlServer->GetParser();
    if (parser.get() == 0)
    {
        GetLog()->Error()
            << "(AgentControl) ERROR:  got no parser from "
            << " the GameControlServer" << endl;
        return;
    }

    boost::shared_ptr<AgentAspect> agent =
      mGameControlServer->GetAgentAspect(client->id);
    if (agent.get() == 0)
    {
        return;
    }
    if (mSyncMode)
    {
        agent->SetSynced(false);
    }

    boost::shared_ptr<PredicateList> senseList = agent->QueryPerceptors();
    mClientSenses[client->id] = parser->Generate(senseList);
    if (mClientSenses[client->id].empty())
    {
        return;
    }

    mNetMessage->PrepareToSend(mClientSenses[client->id]);
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

void AgentControl::SetMultiThreaded(bool multiThreaded)
{
    mMultiThreads = multiThreaded;
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


void AgentControl::AgentThread(const boost::shared_ptr<Client> &client)
{
  boost::barrier *currentBarrier = mThreadBarrierNew;

  while(client->socket->isOpen())
  {
    WaitSlave(currentBarrier);

    //StartCycle not parallel:
    //  parser and agentState::addMessage not thread safe.
    //  additional synchronization required -> no speed-up !
    if(mThreadAction == STARTCYCLE)
    {

      TBufferMap::iterator buf = mBuffers.find(client->addr);
      if(buf != mBuffers.end())
      {
        boost::shared_ptr<NetBuffer>& netBuff = buf->second;
        if (netBuff.get() != 0 && !netBuff->IsEmpty())
          StartCycle(client, netBuff);
      }

    }

    // SenseAgent not parallel:  not enough computation, no speed-up !
    else if(mThreadAction == SENSEAGENT)
    {


      std::string& senses = mClientSenses[client->id];
      if (!senses.empty())
          SendClientMessage(client, senses);

    }

    // Here we get a speed-up !
    else if(mThreadAction == ENDCYCLE)
    {

      EndCycle(client);

    }

    WaitSlave(currentBarrier);
  }

  nThreads--;
  if(mThreadBarrierNew != NULL)
    GetLog()->Error()
        << "(AgentControl) ERROR mThreadBarrierNew!=NULL!"
        << " Agents connecting/disconnecting in same frame !\n";
  mThreadBarrierNew = new boost::barrier(nThreads+1);
  currentBarrier->wait();
}

void AgentControl::WaitMaster()
{
    if(mThreadBarrierNew != NULL)
    {
      boost::barrier *oldBarrier = mThreadBarrier;
      mThreadBarrier = mThreadBarrierNew;
      oldBarrier->wait();
      mThreadBarrier->wait();
      delete oldBarrier;
      mThreadBarrierNew = NULL;
    }
    else
      mThreadBarrier->wait();
}

void AgentControl::WaitSlave(boost::barrier* &currentBarrier)
{
    currentBarrier->wait();
    if(currentBarrier != mThreadBarrier)
    {
      currentBarrier = mThreadBarrier;
      currentBarrier->wait();
    }
}
