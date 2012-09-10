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
#ifndef OXYGEN_TRAINCONTROL_H
#define OXYGEN_TRAINCONTROL_H

#include "simcontrolnode.h"
#include <oxygen/oxygen_defines.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <oxygen/gamecontrolserver/predicate.h>

namespace oxygen
{
class Behavior;

/** \class TrainControl is a simcontrolnode, and it's similar
    to AgentControl except that the network part is omited.
    An integrated agent can be registered with its behavior.
 */
class OXYGEN_API TrainControl : public oxygen::SimControlNode
{
public:
    struct Client
    {
    public:
        /** id of the client */
        int id;

        /** cached reference to Behavior */
        boost::shared_ptr<oxygen::Behavior> behavior;

        /** command message received from the integrated agent */
        std::string command;

        /** predicatelist sent to the integrated agent */
        boost::shared_ptr<oxygen::PredicateList> senselist;

    public:
        Client() : id(-1) {};
        Client(int i,
               boost::shared_ptr<oxygen::Behavior> b =
               boost::shared_ptr<oxygen::Behavior>()
               ) : id(i), behavior(b) {};
    };

    typedef std::set<boost::shared_ptr<Client> > TClientSet;

public:
    TrainControl();

    ~TrainControl();

    /** called once when the simulation is started.
    */
    virtual void InitSimulation();

    /** called once before the simulation is shut down */
    virtual void DoneSimulation();

    /** called when a new simulation cycle starts, before the
        physicial simulation is stepped
    */
    virtual void StartCycle();

    /** called when the current simulation cycle ends */
    virtual void EndCycle();

    /** called each simulation cycle to send sensors'
        information to the agent
    */
    virtual void SenseAgent();

    /** called each simulation cycle to carry out agent actions */
    virtual void ActAgent();

    /** called when an integrated agent register */
    void ClientConnect(boost::shared_ptr<Behavior> behavior);

    /** called when an integrated agent unregister */
    void ClientDisconnect(boost::shared_ptr<Behavior> behavior);

protected:
    void OnLink();

    void OnUnlink();

protected:
    /** set of all the clients */
    TClientSet mClientSet;

    /** cached reference to the GameControlServer */
    CachedPath<oxygen::GameControlServer> mGameControlServer;

    /** the id of next client */
    int mNextClientId;
};

DECLARE_CLASS(TrainControl);

} // namespace oxygen

#endif // OXYGEN_TRAINCONTROL_H
